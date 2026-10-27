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

#include "AnchorPersistenceSubsystem.h"
#include "AndroidXRTypeConversions.h"

DEFINE_LOG_CATEGORY(LogAndroidXRAnchorPersistenceSubsystem);

void UAnchorPersistenceSubsystem::Tick(float DeltaTime)
{
    PersistAnchors();
    CreatePersistedAnchorSpaces();
    UnpersistAnchors();
}

void UAnchorPersistenceSubsystem::SetOpenXRFunctions(PFN_xrPersistAnchorANDROID PersistFunction, PFN_xrUnpersistAnchorANDROID UnPersistFunction,PFN_xrCreatePersistedAnchorSpaceANDROID CreatePersistSpaceFunction, PFN_xrDestroySpace DestroySpaceFunction)
{
    xrPersistAnchorAndroid = PersistFunction;
    xrUnpersistAnchorANDROID = UnPersistFunction;
    xrCreatePersistedAnchorSpaceANDROID = CreatePersistSpaceFunction;
    xrDestroySpace = DestroySpaceFunction;
}

bool UAnchorPersistenceSubsystem::IsTickable() const
{
    return bShouldRunAnchorSubsystem;
}

void UAnchorPersistenceSubsystem::ShouldRunAnchorPersistence(bool bShouldRun)
{
    bShouldRunAnchorSubsystem = bShouldRun;
}

void UAnchorPersistenceSubsystem::PersistAnchors()
{
    TArray<XrSpace> PersistedAnchors{};
    for (auto& PersistInfoPair : AnchorsToPersist)
    {
        auto& PersistInfo = PersistInfoPair.Value;

        XrPersistedAnchorSpaceInfoANDROID SpaceInfo
        {
            .type = XR_TYPE_PERSISTED_ANCHOR_SPACE_INFO_ANDROID,
            .anchor = PersistInfo.AnchorSpace.Space
        };

        XrUuidEXT NativeUniqueId;
        auto Result = xrPersistAnchorAndroid(PersistInfo.AnchorPersistence.Persistence, &SpaceInfo, &NativeUniqueId);
        if (Result == XR_ERROR_PERSISTED_DATA_NOT_READY_ANDROID)
        {
            continue;
        }
        PersistedAnchors.Add(PersistInfo.AnchorSpace.Space);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            PersistInfo.OnAnchorPersisted.ExecuteIfBound(PersistInfo.AnchorSpace, ToFGuid(NativeUniqueId));
        }
        else
        {
            UE_LOG(LogAndroidXRAnchorPersistenceSubsystem, Error,
                TEXT("PersistAnchor: "
                    "xrPersistAnchorAndroid failed with error %s (%d) "),
                OpenXRResultToString(Result), Result);
            PersistInfo.OnAsycActionFailed.ExecuteIfBound();
        }
    }
    for (auto PersistedAnchor : PersistedAnchors)
    {
        AnchorsToPersist.Remove(PersistedAnchor);
    }
}

void UAnchorPersistenceSubsystem::CreatePersistedAnchorSpaces()
{
    TArray<FGuid> CreatedAnchors{};
    for (auto& CreatePersistAnchorInfoPair : PersistedAnchorsToCreate)
    {
        auto& PersistedInfo = CreatePersistAnchorInfoPair.Value;
        if (xrCreatePersistedAnchorSpaceANDROID != nullptr)
        {
            XrPersistedAnchorSpaceCreateInfoANDROID CreateInfo
            {
                .type = XR_TYPE_PERSISTED_ANCHOR_SPACE_CREATE_INFO_ANDROID,
                .anchorId = ToXrUuid(PersistedInfo.PersistedId)
            };
            XrSpace NativeAnchorSpace;
            auto Result = xrCreatePersistedAnchorSpaceANDROID(PersistedInfo.AnchorPersistence.Persistence, &CreateInfo, &NativeAnchorSpace);
            if (Result == XR_ERROR_PERSISTED_DATA_NOT_READY_ANDROID)
            {
                continue;
            }
            CreatedAnchors.Add(PersistedInfo.PersistedId);
            if (XR_UNQUALIFIED_SUCCESS(Result))
            {
                if (PersistedInfo.AnchorSpace.Space != nullptr)
                {
                    xrDestroySpace(PersistedInfo.AnchorSpace.Space);
                }
                PersistedInfo.AnchorSpace.Space = NativeAnchorSpace;
                PersistedInfo.OnPersistedAnchorCreated.ExecuteIfBound(PersistedInfo.AnchorSpace , PersistedInfo.PersistedId);
            }
            else
            {
                UE_LOG(LogAndroidXRAnchorPersistenceSubsystem, Error,
                    TEXT("CreatePersistedAnchorSpace: "
                        "xrCreatePersistedAnchorSpaceANDROID failed with error %s (%d) "),
                    OpenXRResultToString(Result), Result);
                PersistedInfo.OnAsycActionFailed.ExecuteIfBound();
            }
        }
    }
    for (auto& AnchorId : CreatedAnchors)
    {
        PersistedAnchorsToCreate.Remove(AnchorId);
    }
}

void UAnchorPersistenceSubsystem::UnpersistAnchors()
{
    TArray<FGuid> UnPersistedAnchors{};
    for (auto& UnPersistInfoPair : AnchorsToUnPersist)
    {
        auto& UnPersistInfo = UnPersistInfoPair.Value;

        if (xrUnpersistAnchorANDROID != nullptr)
        {
            auto XrUuid = ToXrUuid(UnPersistInfo.PersistedId);
            auto Result = xrUnpersistAnchorANDROID(UnPersistInfo.AnchorPersistence.Persistence, &XrUuid);
            if (Result == XR_ERROR_PERSISTED_DATA_NOT_READY_ANDROID)
            {
                continue;
            }
            UnPersistedAnchors.Add(UnPersistInfo.PersistedId);
            if (XR_UNQUALIFIED_SUCCESS(Result))
            {
                UnPersistInfo.OnAnchorUnPersisted.ExecuteIfBound();
            }
            else
            {
                UE_LOG(LogAndroidXRAnchorPersistenceSubsystem, Error,
                    TEXT("UnpersistAnchor: "
                        "xrUnpersistAnchorANDROID failed with error %s"),
                    OpenXRResultToString(Result));
                UnPersistInfo.OnAsycActionFailed.ExecuteIfBound();
            }
        }
    }
    for (auto UnPersistedAnchor : UnPersistedAnchors)
    {
        AnchorsToUnPersist.Remove(UnPersistedAnchor);
    }
}

UAnchorPersistenceSubsystem::UAnchorPersistenceSubsystem()
{
    SetTickableTickType(ETickableTickType::Conditional);
}

void UAnchorPersistenceSubsystem::PersistAnchorAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FAndroidXRSpace& AnchorSpace, FOnAnchorPersisted& OnAnchorPersisted, FOnAnchorAsyncActionFailed& OnPersistAnchorFailed)
{
    if (AnchorsToPersist.Contains(AnchorSpace.Space))
    {
        return;
    }
    FAnchorPersistInfo PersistInfo{};
    PersistInfo.AnchorPersistence = AnchorPersistence;
    PersistInfo.AnchorSpace = AnchorSpace;
    PersistInfo.OnAnchorPersisted = OnAnchorPersisted;
    PersistInfo.OnAsycActionFailed = OnPersistAnchorFailed;
    AnchorsToPersist.Add(AnchorSpace.Space, PersistInfo);
}

void UAnchorPersistenceSubsystem::CreatePersistedAnchorSpaceAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FGuid& PersistedId, const FAndroidXRSpace& AnchorSpace, FOnPersistedAnchorSpaceCreated& OnPersistedAnchorCreated, FOnAnchorAsyncActionFailed& OnPersistAnchorCreateFailed)
{
    if (PersistedAnchorsToCreate.Contains(PersistedId))
    {
        return;
    }
    FPersistAnchorCreateInfo CreateInfo{};
    CreateInfo.AnchorPersistence = AnchorPersistence;
    CreateInfo.PersistedId = PersistedId;
    CreateInfo.AnchorSpace = AnchorSpace;
    CreateInfo.OnPersistedAnchorCreated = OnPersistedAnchorCreated;
    CreateInfo.OnAsycActionFailed = OnPersistAnchorCreateFailed;
    PersistedAnchorsToCreate.Add(PersistedId, CreateInfo);
}

void UAnchorPersistenceSubsystem::UnpersistAnchorAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FGuid& PersistedId, FOnAnchorUnPersisted& OnAnchorUnPersisted, FOnAnchorAsyncActionFailed& OnUnpersistFailed)
{
    if (AnchorsToUnPersist.Contains(PersistedId))
    {
        return;
    }
    FUnPersistAnchorInfo AnchorInfo{};
    AnchorInfo.AnchorPersistence = AnchorPersistence;
    AnchorInfo.PersistedId = PersistedId;
    AnchorInfo.OnAnchorUnPersisted = OnAnchorUnPersisted;
    AnchorInfo.OnAsycActionFailed = OnUnpersistFailed;
    AnchorsToUnPersist.Add(PersistedId, AnchorInfo);
}

