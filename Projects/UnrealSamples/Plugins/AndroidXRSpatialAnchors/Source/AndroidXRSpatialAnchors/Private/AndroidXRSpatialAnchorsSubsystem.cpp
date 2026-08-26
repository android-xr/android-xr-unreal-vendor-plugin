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

#include "AndroidXRSpatialAnchorsSubsystem.h"
#include "IAndroidXRSpatialAnchorsModule.h"

bool UAndroidXRSpatialAnchorsSubsystem::CreateSpatialAnchor(const FTransform& AnchorTransform, UAndroidXRSpatialAnchorEntity*& CreatedAnchor)
{
    if(!IAndroidXRSpatialAnchorsModule::IsAvailable())
    {
        return false;
    }
    return IAndroidXRSpatialAnchorsModule::Get().CreateSpatialAnchor(AnchorTransform, CreatedAnchor);
}

bool UAndroidXRSpatialAnchorsSubsystem::DestroySpatialAnchor(UAndroidXRSpatialAnchorEntity* AnchorToDestroy)
{
    if(!IAndroidXRSpatialAnchorsModule::IsAvailable())
    {
        return false;
    }
    return IAndroidXRSpatialAnchorsModule::Get().DestroySpatialAnchor(AnchorToDestroy);
}

bool UAndroidXRSpatialAnchorsSubsystem::LoadPersistedAnchors()
{
    if(!IAndroidXRSpatialAnchorsModule::IsAvailable())
    {
        return false;
    }
    return IAndroidXRSpatialAnchorsModule::Get().LoadPersistedAnchors();
}

void UAndroidXRSpatialAnchorsSubsystem::GetRequiredPermissions(TArray<FString>& Permissions)
{
    Permissions.Add(TEXT("android.permission.SCENE_UNDERSTANDING_COARSE"));
    Permissions.Add(TEXT("android.permission.SCENE_UNDERSTANDING_FINE"));
}

void UAndroidXRSpatialAnchorsSubsystem::BindEntityEvents()
{
    if(!IAndroidXRSpatialAnchorsModule::IsAvailable())
    {
        return;
    }
    AnchorAddedHandle = IAndroidXRSpatialAnchorsModule::Get().OnEntityAdded.AddUObject(this, &UAndroidXRSpatialAnchorsSubsystem::AnchorAddedHandler);
    AnchorRemovedHandle = IAndroidXRSpatialAnchorsModule::Get().OnEntityRemoved.AddUObject(this, &UAndroidXRSpatialAnchorsSubsystem::AnchorRemovedHandler);
}

void UAndroidXRSpatialAnchorsSubsystem::UnbindEntityEvents()
{
    if(!IAndroidXRSpatialAnchorsModule::IsAvailable())
    {
        return;
    }
    IAndroidXRSpatialAnchorsModule::Get().OnEntityAdded.Remove(AnchorAddedHandle);
    IAndroidXRSpatialAnchorsModule::Get().OnEntityRemoved.Remove(AnchorRemovedHandle);
}

FAndroidXRSpatialCapabilityModule* UAndroidXRSpatialAnchorsSubsystem::GetCapabilityModule()
{
    if(!IAndroidXRSpatialAnchorsModule::IsAvailable())
    {
        return nullptr;
    }
    return &IAndroidXRSpatialAnchorsModule::Get();
}

void UAndroidXRSpatialAnchorsSubsystem::AnchorAddedHandler(TObjectPtr<UAndroidXRSpatialAnchorEntity> AddedAnchor)
{
    OnAnchorAdded.Broadcast(AddedAnchor);
}

void UAndroidXRSpatialAnchorsSubsystem::AnchorRemovedHandler(TObjectPtr<UAndroidXRSpatialAnchorEntity> RemovedAnchor)
{
    OnAnchorRemoved.Broadcast(RemovedAnchor);
}

UAndroidXRSpatialAnchorsPersistenceAsyncAction* UAndroidXRSpatialAnchorsPersistenceAsyncAction::PersistAnchorAsync(UAndroidXRSpatialAnchorEntity* AnchorToPersist)
{
    auto PersistOperation = NewObject<UAndroidXRSpatialAnchorsPersistenceAsyncAction>();
    PersistOperation->bUnPersistOperation = false;
    PersistOperation->Anchor = AnchorToPersist;
    return PersistOperation;
}

UAndroidXRSpatialAnchorsPersistenceAsyncAction* UAndroidXRSpatialAnchorsPersistenceAsyncAction::UnPersistAnchorAsync(UAndroidXRSpatialAnchorEntity* AnchorToUnPersist)
{
    auto UnPersistOperation = NewObject<UAndroidXRSpatialAnchorsPersistenceAsyncAction>();
    UnPersistOperation->bUnPersistOperation = true;
    UnPersistOperation->Anchor = AnchorToUnPersist;
    return UnPersistOperation;
}

void UAndroidXRSpatialAnchorsPersistenceAsyncAction::Activate()
{
    if(!IAndroidXRSpatialAnchorsModule::IsAvailable())
    {
        //if not available, execute failed:
        OnOperationFailed.Broadcast(ESpatialAnchorPeristenceResult::EntityNotTracking);
        return;
    }

    FAnchorPersistOperationDelegate PersistFinishedCallback{};
    PersistFinishedCallback.AddUniqueDynamic(this, &UAndroidXRSpatialAnchorsPersistenceAsyncAction::PersistOperationFinishedHandler);

    FAnchorPersistOperationFailedDelegate PersitFailedCallback{};
    PersitFailedCallback.AddUniqueDynamic(this, &UAndroidXRSpatialAnchorsPersistenceAsyncAction::PersistOperationFailedHandler);

    if(!bUnPersistOperation)
    {
        IAndroidXRSpatialAnchorsModule::Get().PersistAnchor(Anchor, PersistFinishedCallback, PersitFailedCallback);
    }
    else
    {
        IAndroidXRSpatialAnchorsModule::Get().UnPersistAnchor(Anchor, PersistFinishedCallback, PersitFailedCallback);
    }
}

void UAndroidXRSpatialAnchorsPersistenceAsyncAction::PersistOperationFinishedHandler(UAndroidXRSpatialAnchorEntity* InAnchor)
{
    OnOperationSuccess.Broadcast(InAnchor);
}

void UAndroidXRSpatialAnchorsPersistenceAsyncAction::PersistOperationFailedHandler(ESpatialAnchorPeristenceResult Result)
{
    OnOperationFailed.Broadcast(Result);
}
