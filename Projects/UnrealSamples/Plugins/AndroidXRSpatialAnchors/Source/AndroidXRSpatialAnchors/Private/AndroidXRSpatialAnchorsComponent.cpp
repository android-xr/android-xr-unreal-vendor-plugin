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

#include "AndroidXRSpatialAnchorsComponent.h"
#include "IAndroidXRSpatialAnchorsModule.h"

UAndroidXRSpatialAnchorsComponent::UAndroidXRSpatialAnchorsComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bAutoActivate = true;
}

void UAndroidXRSpatialAnchorsComponent::BeginPlay()
{
    Super::BeginPlay();
    Subsystem = GEngine->GetEngineSubsystem<UAndroidXRSpatialAnchorsSubsystem>();
}

void UAndroidXRSpatialAnchorsComponent::Initialize(UAndroidXRSpatialAnchorEntity* InAnchor)
{
    AnchorInfo = InAnchor;
    bInitialized = true;
}

bool UAndroidXRSpatialAnchorsComponent::CreateAnchor()
{
    if(!Subsystem || !Subsystem->IsTracking())
    {
        return false;
    }
    bInitialized = Subsystem->CreateSpatialAnchor(GetComponentTransform(), AnchorInfo);
    return bInitialized;
}

bool UAndroidXRSpatialAnchorsComponent::DestroyAnchor()
{
    if(!Subsystem || !Subsystem->IsTracking() || !bInitialized)
    {
        return false;
    }
    bInitialized = !Subsystem->DestroySpatialAnchor(AnchorInfo);
    return true;
}

bool UAndroidXRSpatialAnchorsComponent::PersistAnchor()
{
    if(!Subsystem || !Subsystem->IsTracking() || !IAndroidXRSpatialAnchorsModule::IsAvailable() || !bInitialized)
    {
        PersistFailedHandler(ESpatialAnchorPeristenceResult::InvalidEntity);
        return false;
    }
    FAnchorPersistOperationDelegate PersistedCallback{};
    PersistedCallback.AddUniqueDynamic(this, &UAndroidXRSpatialAnchorsComponent::AnchorPersistedHandler);

    FAnchorPersistOperationFailedDelegate PersistFailedCallback{};
    PersistFailedCallback.AddUniqueDynamic(this, &UAndroidXRSpatialAnchorsComponent::PersistFailedHandler);
    //For persist operations, have to access the module directly:
    return IAndroidXRSpatialAnchorsModule::Get().PersistAnchor(AnchorInfo, PersistedCallback, PersistFailedCallback);
}

bool UAndroidXRSpatialAnchorsComponent::UnpersistAnchor()
{
    if(!Subsystem || !Subsystem->IsTracking() || !IAndroidXRSpatialAnchorsModule::IsAvailable() || !bInitialized)
    {
        UnpersistFailedHandler(ESpatialAnchorPeristenceResult::InvalidEntity);
        return false;
    }

    FAnchorPersistOperationDelegate UnpersistedCallback{};
    UnpersistedCallback.AddUniqueDynamic(this, &UAndroidXRSpatialAnchorsComponent::AnchorUnpersistedHandler);

    FAnchorPersistOperationFailedDelegate UnpersistFailedCallback{};
    UnpersistFailedCallback.AddUniqueDynamic(this, &UAndroidXRSpatialAnchorsComponent::UnpersistFailedHandler);
    //For persist operations, have to access the module directly:
    return IAndroidXRSpatialAnchorsModule::Get().UnPersistAnchor(AnchorInfo, UnpersistedCallback, UnpersistFailedCallback);
}

void UAndroidXRSpatialAnchorsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(!bUpdateComponentTransform || !bInitialized || !AnchorInfo)
    {
        return;
    }
    auto AnchorTransform = AnchorInfo->AnchorTransform;
    AnchorTransform.SetScale3D(GetComponentScale());
    SetWorldTransform(AnchorTransform);
}

void UAndroidXRSpatialAnchorsComponent::AnchorPersistedHandler(UAndroidXRSpatialAnchorEntity* Anchor)
{
    AnchorInfo = Anchor;
    bInitialized = true;
    OnAnchorPersisted.Broadcast();
}

void UAndroidXRSpatialAnchorsComponent::AnchorUnpersistedHandler(UAndroidXRSpatialAnchorEntity* Anchor)
{
    AnchorInfo = Anchor;
    OnAnchorUnpersisted.Broadcast();
}

void UAndroidXRSpatialAnchorsComponent::PersistFailedHandler(ESpatialAnchorPeristenceResult Result)
{
    OnAnchorPersistFailed.Broadcast(Result);
}

void UAndroidXRSpatialAnchorsComponent::UnpersistFailedHandler(ESpatialAnchorPeristenceResult Result)
{
    OnAnchorUnpersistFailed.Broadcast(Result);
}

