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

#include "AndroidXRSpatialDepthRaycast.h"
#include <IXRTrackingSystem.h>
#include <IHeadMountedDisplay.h>
#include "IOpenXRHMD.h"
#include "AndroidXRSpatialDepthRaycastSubsystem.h"

DEFINE_LOG_CATEGORY(LogAndroidXRSpatialDepthRaycast);

#define LOG_IF_OPENXR_ERROR_AND_RETURN(Tag, Function, Result) if(!XR_UNQUALIFIED_SUCCESS(Result)){ UE_LOG(LogAndroidXRSpatialDepthRaycast, Error, TEXT("%s : %s failed with error %s"), TEXT(#Tag), TEXT(#Function), OpenXRResultToString(Result)); return false;}
#define RETURN_IF_FUNCTION_NOT_FOUND(Function) if(!Function){ UE_LOG(LogAndroidXRSpatialDepthRaycast, Error, TEXT("Unable to locate function %s"), TEXT(#Function)); return false;}

IMPLEMENT_MODULE(FAndroidXRSpatialDepthRaycast, AndroidXRSpatialDepthRaycast);

bool FAndroidXRSpatialDepthRaycast::IsDepthRaycastSupported() const
{
    return CapabilitiesInfo.IsCapabilitySupported(XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_DEPTH_RAYCAST_ANDROID);
}

bool FAndroidXRSpatialDepthRaycast::DepthRaycastAsync(const FSpatialRaycastConfig& RaycastConfig, TDelegate<void(const FSpatialRaycastHitInfo&)> DepthRaycastCallback, TDelegate<void()> OnFailedCallback)
{
    if(bProcessingRequest || !bSpatialContextCreated)
    {
        OnFailedCallback.ExecuteIfBound();
        return false;
    }
    auto HMD = GEngine->XRSystem.Get()->GetIOpenXRHMD();
    auto WorldToMetersScale = GEngine->XRSystem.Get()->GetWorldToMetersScale();
    CurrentRaycastConfig = XrSpatialRaycastInfoANDROID
    {
        .type = XR_TYPE_SPATIAL_RAYCAST_INFO_ANDROID,
        .space = HMD->GetTrackingSpace(),
        .time = HMD->GetDisplayTime(),
        .origin = ToXrVector(RaycastConfig.RayOrigin, WorldToMetersScale),
        .direction = ToXrVector(RaycastConfig.RayDirection, WorldToMetersScale),
        .maxDistance = RaycastConfig.RayLength / WorldToMetersScale
    };
    CurrentDepthRaycastCallback = MoveTemp(DepthRaycastCallback);
    CurrentFailedCallback = MoveTemp(OnFailedCallback);
    bProcessingRequest = true;
    if(!CreateDiscoverySnapshotRequest())
    {
        CurrentFailedCallback.ExecuteIfBound();
        CurrentFailedCallback.Unbind();
        return false;
    }
    return true;
}

bool FAndroidXRSpatialDepthRaycast::CanPerformDepthRaycast() const
{
    return bSpatialContextCreated && !bProcessingRequest;
}

void FAndroidXRSpatialDepthRaycast::UpdateEnabledComponents()
{
    EnabledComponents.Add(XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_RAYCAST_RESULT_ANDROID);
}

void FAndroidXRSpatialDepthRaycast::GetCapabilityConfigurations(TArray<XrSpatialCapabilityConfigurationBaseHeaderEXT*>& Headers)
{
    DepthRaycastConfiguration.enabledComponents = EnabledComponents.GetData();
    DepthRaycastConfiguration.enabledComponentCount = static_cast<uint32_t>(EnabledComponents.Num());
    Headers.Add(reinterpret_cast<XrSpatialCapabilityConfigurationBaseHeaderEXT*>(&DepthRaycastConfiguration));
}

FSpatialComponentList* FAndroidXRSpatialDepthRaycast::GetSpatialComponentList(XrSpatialComponentTypeEXT ComponentType)
{
    if(ComponentType == XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_RAYCAST_RESULT_ANDROID)
    {
        return &RaycastComponents;
    }
    return nullptr;
}

void FAndroidXRSpatialDepthRaycast::GetCapabilityExtensions(TArray<const ANSICHAR*>& Extensions)
{
    Extensions.Add(XR_ANDROID_SPATIAL_DISCOVERY_RAYCAST_EXTENSION_NAME);
}

bool FAndroidXRSpatialDepthRaycast::ShouldAutoDiscoverEntities()
{
    return false;
}

void FAndroidXRSpatialDepthRaycast::OnSnapshotDataObtained()
{
    bProcessingRequest = false;
    if(RaycastComponents.NativeBuffer.IsEmpty())
    {
        OnQueryDataFailed();
        return;
    }
    TArray<FSpatialRaycastHitInfo> Results{};
    auto WorldToMetersScale = GEngine->XRSystem.Get()->GetWorldToMetersScale();
    FSpatialRaycastHitInfo Result
    {
        .HitTransform = ToFTransform(RaycastComponents.NativeBuffer[0].hitPose, WorldToMetersScale),
        .DistanceSquared = RaycastComponents.NativeBuffer[0].distanceSquared
    };
    CurrentDepthRaycastCallback.ExecuteIfBound(Result);
    CurrentDepthRaycastCallback.Unbind();
}

void FAndroidXRSpatialDepthRaycast::OnDiscoverySnapshotRequestCreated(XrSpatialDiscoverySnapshotCreateInfoEXT& CreateInfo)
{
    CreateInfo.next = &CurrentRaycastConfig;
}

void FAndroidXRSpatialDepthRaycast::OnQueryDataFailed()
{
    CurrentFailedCallback.ExecuteIfBound();
    CurrentFailedCallback.Unbind();
}

void FAndroidXRSpatialDepthRaycast::SpatialContextDestroyed()
{
    IAndroidXRSpatialDepthRaycastModule::SpatialContextDestroyed();
    bProcessingRequest = false;
    CurrentDepthRaycastCallback.Unbind();
    CurrentFailedCallback.Unbind();
}
