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

#include "AndroidXRAnchorPersistenceBlueprintFunctionLibrary.h"
#include "IAndroidXRAnchorPersistenceModule.h"
#include "AndroidXRBlueprintFunctionLibrary.h"

bool UAndroidXRAnchorPersistenceBlueprintFunctionLibrary::IsValidAnchorPersistence(const FAndroidXRAnchorPersistence&AnchorPersistence)
{
    return AnchorPersistence.Persistence != XR_NULL_HANDLE;
}

bool UAndroidXRAnchorPersistenceBlueprintFunctionLibrary::GetAnchorPersistenceSupported()
{
    if (IAndroidXRAnchorPersistenceModule::IsAvailable())
    {
        return IAndroidXRAnchorPersistenceModule::Get().GetAnchorPersistenceSupported();
    }
    return false;
}

bool UAndroidXRAnchorPersistenceBlueprintFunctionLibrary::EnumerateSupportedPersistenceAnchorTypes(TArray<EAndroidXRTrackableType> &SupportedTypes)
{
    if (IAndroidXRAnchorPersistenceModule::IsAvailable())
    {
        return IAndroidXRAnchorPersistenceModule::Get().
            EnumerateSupportedPersistenceAnchorTypes(SupportedTypes);
    }
    return false;
}

bool UAndroidXRAnchorPersistenceBlueprintFunctionLibrary::CreateAnchorPersistence(FAndroidXRAnchorPersistence&AnchorPersistence)
{
    if (IAndroidXRAnchorPersistenceModule::IsAvailable())
    {
        return IAndroidXRAnchorPersistenceModule::Get().
            CreateAnchorPersistence(AnchorPersistence);
    }
    return false;
}

bool UAndroidXRAnchorPersistenceBlueprintFunctionLibrary::DestroyAnchorPersistence(FAndroidXRAnchorPersistence&AnchorPersistence)
{
    if (IAndroidXRAnchorPersistenceModule::IsAvailable())
    {
        return IAndroidXRAnchorPersistenceModule::Get().
            DestroyAnchorPersistence(AnchorPersistence);
    }
    return false;
}

bool UAndroidXRAnchorPersistenceBlueprintFunctionLibrary::GetAnchorPersistState(const FAndroidXRAnchorPersistence&AnchorPersistence,
    const FGuid &PersistedID,
    EAndroidXRAnchorPersistState &PersistState)
{
    if (IAndroidXRAnchorPersistenceModule::IsAvailable())
    {
        return IAndroidXRAnchorPersistenceModule::Get().
            GetAnchorPersistState(AnchorPersistence, PersistedID, PersistState);
    }
    return false;
}

bool UAndroidXRAnchorPersistenceBlueprintFunctionLibrary::EnumeratePersistedAnchors(const FAndroidXRAnchorPersistence&AnchorPersistence,
    TArray<FGuid> &PersistedAnchors)
{
    if (IAndroidXRAnchorPersistenceModule::IsAvailable())
    {
        return IAndroidXRAnchorPersistenceModule::Get().
            EnumeratePersistedAnchors(AnchorPersistence, PersistedAnchors);
    }
    return false;
}

UPersistAnchor* UPersistAnchor::PersistAnchorAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FAndroidXRSpace& AnchorSpace)
{
    auto PersistAnchor = NewObject<UPersistAnchor>();
    PersistAnchor->AnchorSpace = AnchorSpace;
    PersistAnchor->AnchorPersistence = AnchorPersistence;
    return PersistAnchor;
}

void UPersistAnchor::Activate()
{
    FOnAnchorPersisted OnAnchorPersistedCallback;
    OnAnchorPersistedCallback.BindUObject(this, &UPersistAnchor::PersistAnchorCallback);
    FOnAnchorAsyncActionFailed OnActionFailedCallback;
    GetAsyncActionFailedCallback(OnActionFailedCallback);
    if (IAndroidXRAnchorPersistenceModule::IsAvailable())
    {
        IAndroidXRAnchorPersistenceModule::Get().PersistAnchorAsync(AnchorPersistence, AnchorSpace, OnAnchorPersistedCallback, OnActionFailedCallback);
    }
}

void UPersistAnchor::PersistAnchorCallback(const FAndroidXRSpace& PersistedAnchorSpace, const FGuid& PersistedAnchorId)
{
    OnAnchorPersisted.Broadcast(PersistedAnchorSpace, PersistedAnchorId);
}

UUnpersistAnchor* UUnpersistAnchor::UnpersistAnchorAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FGuid& PersistedId)
{
    auto UnpersistAnchor = NewObject<UUnpersistAnchor>();
    UnpersistAnchor->AnchorPersistence = AnchorPersistence;
    UnpersistAnchor->PersistedId = PersistedId;
    return UnpersistAnchor;
}

void UUnpersistAnchor::Activate()
{
    FOnAnchorUnPersisted OnAnchorUnpersistedCallback;
    OnAnchorUnpersistedCallback.BindUObject(this, &UUnpersistAnchor::UnpersistAnchorCallback);

    FOnAnchorAsyncActionFailed OnAnchorUnPersistFailedCallback;
    GetAsyncActionFailedCallback(OnAnchorUnPersistFailedCallback);
    if (IAndroidXRAnchorPersistenceModule::IsAvailable())
    {
        IAndroidXRAnchorPersistenceModule::Get().UnpersistAnchorAsync(AnchorPersistence, PersistedId, OnAnchorUnpersistedCallback, OnAnchorUnPersistFailedCallback);
    }
}

void UUnpersistAnchor::UnpersistAnchorCallback()
{
    OnAnchorUnpersisted.Broadcast();
}



UCreatePersistedAnchorSpace* UCreatePersistedAnchorSpace::CreatePersistedAnchorSpaceAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FGuid& PersistedId, const FAndroidXRSpace& AnchorSpace)
{
    auto CreatePersistedAnchorSpace = NewObject<UCreatePersistedAnchorSpace>();
    CreatePersistedAnchorSpace->AnchorPersistence = AnchorPersistence;
    CreatePersistedAnchorSpace->AnchorSpace = AnchorSpace;
    CreatePersistedAnchorSpace->PersistedId = PersistedId;
    return CreatePersistedAnchorSpace;
}

void UCreatePersistedAnchorSpace::Activate()
{
    FOnPersistedAnchorSpaceCreated OnPersistedAnchorSpaceCreatedCallback;
    OnPersistedAnchorSpaceCreatedCallback.BindUObject(this, &UCreatePersistedAnchorSpace::PersistedAnchorCreatedCallback);

    FOnAnchorAsyncActionFailed OnPersistedAnchorCreateFailedCallback;
    GetAsyncActionFailedCallback(OnPersistedAnchorCreateFailedCallback);
    if (IAndroidXRAnchorPersistenceModule::IsAvailable())
    {
        IAndroidXRAnchorPersistenceModule::Get().CreatePersistedAnchorSpaceAsync(AnchorPersistence, PersistedId, AnchorSpace, OnPersistedAnchorSpaceCreatedCallback, OnPersistedAnchorCreateFailedCallback);
    }
}

void UCreatePersistedAnchorSpace::PersistedAnchorCreatedCallback(const FAndroidXRSpace& CreatedAnchorSpace, const FGuid& PersistedAnchorId)
{
    OnPersistedAnchorSpaceCreated.Broadcast(CreatedAnchorSpace, PersistedAnchorId);
}

void UAndroidXRAnchorPersistenceAsyncAction::GetAsyncActionFailedCallback(FOnAnchorAsyncActionFailed& ActionFailedCallback)
{
    ActionFailedCallback.BindUObject(this, &UAndroidXRAnchorPersistenceAsyncAction::OnActionFailedCallback);
}

void UAndroidXRAnchorPersistenceAsyncAction::OnActionFailedCallback()
{
    OnAsyncActionFailed.Broadcast();
}
