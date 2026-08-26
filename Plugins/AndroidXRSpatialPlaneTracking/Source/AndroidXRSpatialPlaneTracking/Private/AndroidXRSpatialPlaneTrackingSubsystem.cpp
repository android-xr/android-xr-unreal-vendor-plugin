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

#include "AndroidXRSpatialPlaneTrackingSubsystem.h"
#include "IAndroidXRSpatialPlaneTrackingModule.h"

void UAndroidXRSpatialPlaneTrackingSubsystem::GetTrackedPlanes(TArray<UAndroidXRSpatialPlaneEntity*>& TrackedPlanes, bool bShouldUpdate)
{
    if(!IAndroidXRSpatialPlaneTrackingModule::IsAvailable())
    {
        return;
    }
    IAndroidXRSpatialPlaneTrackingModule::Get().GetTrackedEntities(TrackedPlanes, bShouldUpdate);
}

void UAndroidXRSpatialPlaneTrackingSubsystem::GetRequiredPermissions(TArray<FString>& Permissions)
{
    Permissions.Add(TEXT("android.permission.SCENE_UNDERSTANDING_COARSE"));
    Permissions.Add(TEXT("android.permission.SCENE_UNDERSTANDING_FINE"));
}

void UAndroidXRSpatialPlaneTrackingSubsystem::BindEntityEvents()
{
    if(!IAndroidXRSpatialPlaneTrackingModule::IsAvailable())
    {
        return;
    }
    PlaneAddedHandle = IAndroidXRSpatialPlaneTrackingModule::Get().OnEntityAdded.AddUObject(this, &UAndroidXRSpatialPlaneTrackingSubsystem::SpatialPlaneAddedHandler);
    PlaneRemovedHandle = IAndroidXRSpatialPlaneTrackingModule::Get().OnEntityRemoved.AddUObject(this, &UAndroidXRSpatialPlaneTrackingSubsystem::SpatialPlaneRemovedHandler);
}

void UAndroidXRSpatialPlaneTrackingSubsystem::UnbindEntityEvents()
{
    if(!IAndroidXRSpatialPlaneTrackingModule::IsAvailable())
    {
        return;
    }
    IAndroidXRSpatialPlaneTrackingModule::Get().OnEntityAdded.Remove(PlaneAddedHandle);
    IAndroidXRSpatialPlaneTrackingModule::Get().OnEntityRemoved.Remove(PlaneRemovedHandle);
}

FAndroidXRSpatialCapabilityModule* UAndroidXRSpatialPlaneTrackingSubsystem::GetCapabilityModule()
{
    if(!IAndroidXRSpatialPlaneTrackingModule::IsAvailable())
    {
        return nullptr;
    }
    return &IAndroidXRSpatialPlaneTrackingModule::Get();
}

void UAndroidXRSpatialPlaneTrackingSubsystem::SpatialPlaneAddedHandler(TObjectPtr<UAndroidXRSpatialPlaneEntity> AddedPlane)
{
    OnSpatialPlaneAdded.Broadcast(AddedPlane);
}

void UAndroidXRSpatialPlaneTrackingSubsystem::SpatialPlaneRemovedHandler(TObjectPtr<UAndroidXRSpatialPlaneEntity> RemovedPlane)
{
    OnSpatialPlaneRemoved.Broadcast(RemovedPlane);
}

