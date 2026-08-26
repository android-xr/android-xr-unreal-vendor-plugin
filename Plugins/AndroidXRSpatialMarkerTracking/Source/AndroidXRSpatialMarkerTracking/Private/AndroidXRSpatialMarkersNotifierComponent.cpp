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

#include "AndroidXRSpatialMarkersNotifierComponent.h"
#include "AndroidXRSpatialMarkerTrackingSubsystem.h"

UAndroidXRSpatialEntitiesSubsystem* UAndroidXRSpatialMarkersNotifierComponent::GetCapabilitySubsystem()
{
    if(!MarkerTrackingSubsystem)
    {
        MarkerTrackingSubsystem = GEngine->GetEngineSubsystem<UAndroidXRSpatialMarkerTrackingSubsystem>();
    }
    return MarkerTrackingSubsystem;
}

void UAndroidXRSpatialMarkersNotifierComponent::BindToTrackingEvents()
{
    if(!MarkerTrackingSubsystem)
    {
        return;
    }
    MarkerTrackingSubsystem->OnSpatialMarkerAdded.AddUniqueDynamic(this, &UAndroidXRSpatialMarkersNotifierComponent::MarkerAddedHandler);
    MarkerTrackingSubsystem->OnSpatialMarkerRemoved.AddUniqueDynamic(this, &UAndroidXRSpatialMarkersNotifierComponent::MarkerRemovedHandler);
}

void UAndroidXRSpatialMarkersNotifierComponent::UnbindFromTrackingEvents()
{
    TrackedMarkers.Reset();
    MarkerTrackingSubsystem->OnSpatialMarkerAdded.RemoveAll(this);
    MarkerTrackingSubsystem->OnSpatialMarkerRemoved.RemoveAll(this);
}

void UAndroidXRSpatialMarkersNotifierComponent::OnStartTracking()
{
    MarkerTrackingSubsystem->ConfigureMarkerTracking(MarkerTrackingConfig);
}

void UAndroidXRSpatialMarkersNotifierComponent::MarkerAddedHandler(UAndroidXRSpatialMarkerEntity* Marker)
{
    TrackedMarkers.Add(Marker);
    OnMarkerAdded.Broadcast(Marker);
}

void UAndroidXRSpatialMarkersNotifierComponent::MarkerRemovedHandler(UAndroidXRSpatialMarkerEntity* Marker)
{
    TrackedMarkers.Remove(Marker);
    OnMarkerRemoved.Broadcast(Marker);
}
