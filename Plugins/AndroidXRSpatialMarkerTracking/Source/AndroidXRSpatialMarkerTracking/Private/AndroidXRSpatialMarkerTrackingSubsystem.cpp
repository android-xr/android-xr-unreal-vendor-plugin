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

#include "AndroidXRSpatialMarkerTrackingSubsystem.h"
#include "IAndroidXRSpatialMarkerTrackingModule.h"

void UAndroidXRSpatialMarkerTrackingSubsystem::GetTrackedMarkers(TArray<UAndroidXRSpatialMarkerEntity*>& TrackedMarkers, bool bShouldUpdate)
{
    if(!IAndroidXRSpatialMarkerTrackingModule::IsAvailable())
    {
        return;
    }
    IAndroidXRSpatialMarkerTrackingModule::Get().GetTrackedEntities(TrackedMarkers, bShouldUpdate);
}

void UAndroidXRSpatialMarkerTrackingSubsystem::GetRequiredPermissions(TArray<FString>& Permissions)
{
    Permissions.Add(TEXT("android.permission.SCENE_UNDERSTANDING_COARSE"));
    Permissions.Add(TEXT("android.permission.SCENE_UNDERSTANDING_FINE"));
}

void UAndroidXRSpatialMarkerTrackingSubsystem::BindEntityEvents()
{
    if(!IAndroidXRSpatialMarkerTrackingModule::IsAvailable())
    {
        return;
    }
    MarkerAddedHandle = IAndroidXRSpatialMarkerTrackingModule::Get().OnEntityAdded.AddUObject(this, &UAndroidXRSpatialMarkerTrackingSubsystem::SpatialMarkerAddedHandler);
    MarkerRemovedHandle = IAndroidXRSpatialMarkerTrackingModule::Get().OnEntityRemoved.AddUObject(this, &UAndroidXRSpatialMarkerTrackingSubsystem::SpatialMarkerRemovedHandler);
}

void UAndroidXRSpatialMarkerTrackingSubsystem::UnbindEntityEvents()
{
    if(!IAndroidXRSpatialMarkerTrackingModule::IsAvailable())
    {
        return;
    }
    IAndroidXRSpatialMarkerTrackingModule::Get().OnEntityAdded.Remove(MarkerAddedHandle);
    IAndroidXRSpatialMarkerTrackingModule::Get().OnEntityRemoved.Remove(MarkerRemovedHandle);
}

FAndroidXRSpatialCapabilityModule* UAndroidXRSpatialMarkerTrackingSubsystem::GetCapabilityModule()
{
    if(!IAndroidXRSpatialMarkerTrackingModule::IsAvailable())
    {
        return nullptr;
    }
    return &IAndroidXRSpatialMarkerTrackingModule::Get();
}

void UAndroidXRSpatialMarkerTrackingSubsystem::SpatialMarkerAddedHandler(TObjectPtr<UAndroidXRSpatialMarkerEntity> AddedMarker)
{
    OnSpatialMarkerAdded.Broadcast(AddedMarker);
}

void UAndroidXRSpatialMarkerTrackingSubsystem::SpatialMarkerRemovedHandler(TObjectPtr<UAndroidXRSpatialMarkerEntity> RemovedMarker)
{
    OnSpatialMarkerRemoved.Broadcast(RemovedMarker);
}

void UAndroidXRSpatialMarkerTrackingSubsystem::ConfigureMarkerTracking(const FSpatialMarkerTrackingConfig& MarkerTrackingConfig)
{
    if(!IAndroidXRSpatialMarkerTrackingModule::IsAvailable())
    {
        return;
    }
    IAndroidXRSpatialMarkerTrackingModule::Get().Configure(MarkerTrackingConfig);
}

