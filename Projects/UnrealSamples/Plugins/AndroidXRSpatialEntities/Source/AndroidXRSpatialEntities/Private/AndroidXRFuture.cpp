/* Copyright 2026 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "AndroidXRFuture.h"

DEFINE_LOG_CATEGORY(LogAndroidXRSpatialFutures);

FAndroidXRFutures::~FAndroidXRFutures()
{
    CancelAll();
}

void FAndroidXRFutures::Initialize(XrInstance InInstance, PFN_xrPollFutureEXT InPollFuture, PFN_xrCancelFutureEXT InCancelFuture)
{
    Instance = InInstance;
    PollFuture = InPollFuture;
    CancelFuture = InCancelFuture;
}

void FAndroidXRFutures::Reset()
{
    ActiveFutures.Reset();
}

void FAndroidXRFutures::Submit(XrFutureEXT Future, FOnFutureReady OnReady, double TimeOutSeconds)
{
    check(Future != XR_NULL_FUTURE_EXT);
    check(static_cast<bool>(OnReady));
    const auto Deadline = TimeOutSeconds > 0.0 ? FPlatformTime::Seconds() + TimeOutSeconds : 0.0;
    ActiveFutures.Add(FFutureInfo{ .Future = Future, .OnReady = MoveTemp(OnReady), .TimeOutSeconds = Deadline});
}

void FAndroidXRFutures::Poll()
{
    if(!PollFuture || ActiveFutures.IsEmpty())
    {
        return;
    }

    const auto Now = FPlatformTime::Seconds();
    for(auto Index = ActiveFutures.Num() - 1; Index >= 0; Index--)
    {
        auto& Entry = ActiveFutures[Index];
        XrFuturePollInfoEXT PollInfo{
            .type = XR_TYPE_FUTURE_POLL_INFO_EXT,
            .future = Entry.Future
        };

        XrFuturePollResultEXT PollResult
        {
            .type = XR_TYPE_FUTURE_POLL_RESULT_EXT
        };

        auto Result = PollFuture(Instance, &PollInfo, &PollResult);
        if(XR_FAILED(Result))
        {
            UE_LOG(LogAndroidXRSpatialFutures, Error, TEXT("xrPollFuture failed with error %s"), OpenXRResultToString(Result));
            auto EntryCopy = MoveTemp(Entry);
            ActiveFutures.RemoveAtSwap(Index, EAllowShrinking::No);
            EntryCopy.OnReady(EntryCopy.Future, true);
            continue;
        }

        if(PollResult.state == XR_FUTURE_STATE_READY_EXT)
        {
            auto EntryCopy = MoveTemp(Entry);
            ActiveFutures.RemoveAtSwap(Index, EAllowShrinking::No);
            EntryCopy.OnReady(EntryCopy.Future, false);
            continue;
        }
        if(Entry.TimeOutSeconds > 0.0 && Now >= Entry.TimeOutSeconds)
        {
            if(CancelFuture)
            {
                XrFutureCancelInfoEXT CancelInfo
                {
                    .type = XR_TYPE_FUTURE_CANCEL_INFO_EXT,
                    .future = Entry.Future
                };
                Result = CancelFuture(Instance, &CancelInfo);
                if(XR_FAILED(Result))
                {
                    UE_LOG(LogAndroidXRSpatialFutures, Error, TEXT("xrCancelFuture failed with error %s"), OpenXRResultToString(Result));
                }
            }
            auto EntryCopy = MoveTemp(Entry);
            ActiveFutures.RemoveAtSwap(Index, EAllowShrinking::No);
            EntryCopy.OnReady(Entry.Future, true);
        }
    }
    if(ActiveFutures.Num() == 0)
    {
        OnAllFuturesCompleted.ExecuteIfBound();
    }
}

void FAndroidXRFutures::CancelAll()
{
    auto Entries = MoveTemp(ActiveFutures);
    ActiveFutures.Reset();

    for(auto& Entry : Entries)
    {
        XrFutureCancelInfoEXT CancelInfo
        {
            .type = XR_TYPE_FUTURE_CANCEL_INFO_EXT,
            .future = Entry.Future
        };
    }
}