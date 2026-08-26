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
#pragma once
#include "CoreMinimal.h"
#include "Templates/UniqueObj.h"
#include "Templates/UniquePtr.h"
#include <AndroidXRHelpers.h>


DECLARE_LOG_CATEGORY_EXTERN(LogAndroidXRSpatialFutures, Log, All);
class ANDROIDXRSPATIALENTITIES_API FAndroidXRFutures
{
public:
    using FOnFutureReady = TFunction<void(XrFutureEXT Future, bool bWasCancelled)>;
    FAndroidXRFutures() = default;
    ~FAndroidXRFutures();
    void Initialize(XrInstance Instance, PFN_xrPollFutureEXT InPollFuture, PFN_xrCancelFutureEXT InCancelFuture);
    void Reset();
    void Submit(XrFutureEXT Future, FOnFutureReady OnReady, double TimeOutSeconds = 0.0);
    void Poll();
    void CancelAll();

    int32 NumPending() const
    {
       return ActiveFutures.Num();
    }
    bool ShouldPoll() const
    {
        return ActiveFutures.Num() > 0;
    }
    TDelegate<void()> OnAllFuturesCompleted{};
private:
    typedef struct FFutureInfo
    {
        XrFutureEXT Future{};
        FOnFutureReady OnReady{};
        double TimeOutSeconds{};
    }FEntry;

    XrInstance Instance{};
    PFN_xrPollFutureEXT PollFuture{};
    PFN_xrCancelFutureEXT CancelFuture{};

    TArray<FEntry> ActiveFutures{};
};