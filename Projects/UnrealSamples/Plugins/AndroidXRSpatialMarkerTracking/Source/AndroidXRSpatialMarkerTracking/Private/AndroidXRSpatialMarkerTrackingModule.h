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
#include "AndroidXRSpatialEntitiesTypes.h"
#include "AndroidXRSpatialMarkerTrackingTypes.h"
#include "AndroidXRSpatialMarkerEntity.h"
#include "IAndroidXRSpatialMarkerTrackingModule.h"
#include "AndroidXRSpatialEntitiesModuleBase.h"

template<typename TConfigurationType>
struct TFSpatialMarkerNativeConfiguration
{
    TFSpatialMarkerNativeConfiguration(XrStructureType ConfigurationType, XrSpatialCapabilityEXT Capability)
        : NativeConfiguration{ ConfigurationType , nullptr, Capability}
    {
    }

    TConfigurationType NativeConfiguration{};
    XrSpatialMarkerSizeEXT MarkerSizeFeature{XR_TYPE_SPATIAL_MARKER_SIZE_EXT};
    XrSpatialMarkerStaticOptimizationEXT StaticMarkerFeature{XR_TYPE_SPATIAL_MARKER_STATIC_OPTIMIZATION_EXT};
};

class ANDROIDXRSPATIALMARKERTRACKING_API FAndroidXRSpatialMarkerTrackingModule : public IAndroidXRSpatialMarkerTrackingModule
{
public:
    void Configure(const FSpatialMarkerTrackingConfig& Config) override;

    void ApplyToEntity(TObjectPtr<UAndroidXRSpatialMarkerEntity> MarkerEntity, const FSpatialEntityInfo& EntityInfo) override;

    void GetCapabilityExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
    void UpdateEnabledComponents() override;
    void GetCapabilityConfigurations(TArray<XrSpatialCapabilityConfigurationBaseHeaderEXT*>& Headers) override;
    FSpatialComponentList* GetSpatialComponentList(XrSpatialComponentTypeEXT ComponentType) override;

private:
    FSpatialMarkerTrackingConfig MarkerTrackingConfiguration;


    TFSpatialMarkerNativeConfiguration<XrSpatialCapabilityConfigurationAprilTagEXT> AprilTagConfiguration{ XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_APRIL_TAG_EXT,XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_APRIL_TAG_EXT };
    TFSpatialMarkerNativeConfiguration<XrSpatialCapabilityConfigurationArucoMarkerEXT> ArucoMarkerConfiguiration{ XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_ARUCO_MARKER_EXT, XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_ARUCO_MARKER_EXT };
    TFSpatialMarkerNativeConfiguration<XrSpatialCapabilityConfigurationQrCodeEXT> QRCodeConfiguration{ XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_QR_CODE_EXT, XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_QR_CODE_EXT };
    TFSpatialMarkerNativeConfiguration<XrSpatialCapabilityConfigurationMicroQrCodeEXT> MicroQRCodeConfiguration{ XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_MICRO_QR_CODE_EXT, XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_MICRO_QR_CODE_EXT };


    TFSpatialComponentList<XrSpatialComponentMarkerListEXT, XrSpatialMarkerDataEXT> MarkerComponents{ XR_TYPE_SPATIAL_COMPONENT_MARKER_LIST_EXT };

    void GetMarkerIntData(UAndroidXRSpatialMarkerEntity* MarkerEntity, XrSpatialSnapshotEXT Snapshot, XrSpatialBufferEXT Buffer);
    void GetMarkerStringData(UAndroidXRSpatialMarkerEntity* MarkerEntity, XrSpatialSnapshotEXT Snapshot, XrSpatialBufferEXT Buffer);
};
