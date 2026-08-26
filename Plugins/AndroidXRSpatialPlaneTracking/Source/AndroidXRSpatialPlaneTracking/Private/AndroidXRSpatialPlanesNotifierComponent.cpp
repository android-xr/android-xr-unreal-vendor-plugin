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
#include "AndroidXRSpatialPlanesNotifierComponent.h"
#include "AndroidXRSpatialPlaneTrackingSubsystem.h"

DEFINE_LOG_CATEGORY(LogAndroidXRSpatialPlaneComponent)

void UAndroidXRSpatialPlanesNotifierComponent::BeginPlay()
{
    PlaneTrackingSubsystem = GEngine->GetEngineSubsystem<UAndroidXRSpatialPlaneTrackingSubsystem>();
    Super::BeginPlay();
}

UAndroidXRSpatialEntitiesSubsystem* UAndroidXRSpatialPlanesNotifierComponent::GetCapabilitySubsystem()
{
    return PlaneTrackingSubsystem;
}

void UAndroidXRSpatialPlanesNotifierComponent::BindToTrackingEvents()
{
    if(!PlaneTrackingSubsystem)
    {
        return;
    }
    PlaneTrackingSubsystem->OnSpatialPlaneAdded.AddUniqueDynamic(this, &UAndroidXRSpatialPlanesNotifierComponent::PlaneAddedHandler);
    PlaneTrackingSubsystem->OnSpatialPlaneRemoved.AddUniqueDynamic(this, &UAndroidXRSpatialPlanesNotifierComponent::PlaneRemovedHandler);

}

void UAndroidXRSpatialPlanesNotifierComponent::UnbindFromTrackingEvents()
{
    if(!PlaneTrackingSubsystem)
    {
        return;
    }
    TrackedPlanes.Reset();
    PlaneTrackingSubsystem->OnSpatialPlaneAdded.RemoveAll(this);
    PlaneTrackingSubsystem->OnSpatialPlaneRemoved.RemoveAll(this);
}

void UAndroidXRSpatialPlanesNotifierComponent::PlaneAddedHandler(UAndroidXRSpatialPlaneEntity* AddedPlane)
{
    TrackedPlanes.Add(AddedPlane);
    OnPlaneAdded.Broadcast(AddedPlane);
}

void UAndroidXRSpatialPlanesNotifierComponent::PlaneRemovedHandler(UAndroidXRSpatialPlaneEntity* RemovedPlane)
{
    TrackedPlanes.Remove(RemovedPlane);
    OnPlaneRemoved.Broadcast(RemovedPlane);
}
