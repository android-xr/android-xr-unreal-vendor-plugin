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
#include "IAndroidXRSpatialDepthRaycastModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAndroidXRSpatialDepthRaycast, Log, All);

class ANDROIDXRSPATIALDEPTHRAYCAST_API FAndroidXRSpatialDepthRaycast : public IAndroidXRSpatialDepthRaycastModule
{
public:
    bool IsDepthRaycastSupported() const override;
    bool DepthRaycastAsync(const FSpatialRaycastConfig& RaycastConfig, TDelegate<void(const FSpatialRaycastHitInfo&)> DepthRaycastCallback, TDelegate<void()> OnFailedCallback) override;
    bool CanPerformDepthRaycast() const override;
protected:
    void UpdateEnabledComponents() override;
    void GetCapabilityConfigurations(TArray<XrSpatialCapabilityConfigurationBaseHeaderEXT*>& Headers) override;
    FSpatialComponentList* GetSpatialComponentList(XrSpatialComponentTypeEXT ComponentType) override;
    void GetCapabilityExtensions(TArray<const ANSICHAR*>& Extensions) override;
    bool ShouldAutoDiscoverEntities() override;
    void OnSnapshotDataObtained() override;
    void OnDiscoverySnapshotRequestCreated(XrSpatialDiscoverySnapshotCreateInfoEXT& CreateInfo) override;
    void OnQueryDataFailed() override;
    void SpatialContextDestroyed() override;
private:
    TFSpatialComponentList<XrSpatialComponentRaycastResultListANDROID, XrSpatialRaycastResultDataANDROID> RaycastComponents{ XR_TYPE_SPATIAL_COMPONENT_RAYCAST_RESULT_LIST_ANDROID };
    XrSpatialCapabilityConfigurationDepthRaycastANDROID DepthRaycastConfiguration{ XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_DEPTH_RAYCAST_ANDROID, nullptr, XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_DEPTH_RAYCAST_ANDROID };
    bool bProcessingRequest{};
    XrSpatialRaycastInfoANDROID CurrentRaycastConfig{};
    TDelegate<void(const FSpatialRaycastHitInfo&)> CurrentDepthRaycastCallback{};
    TDelegate<void()> CurrentFailedCallback{};
};