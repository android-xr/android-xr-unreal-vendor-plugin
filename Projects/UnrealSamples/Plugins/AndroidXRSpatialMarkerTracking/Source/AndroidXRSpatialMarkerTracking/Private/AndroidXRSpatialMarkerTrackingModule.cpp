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

#include "AndroidXRSpatialMarkerTrackingModule.h"
#include "AndroidXRSpatialMarkerEntity.h"
#include "AndroidXRSpatialEntitiesConversions.h"
#include <IXRTrackingSystem.h>
#include "AndroidXRSpatialMarkerTrackingConversions.h"

void GetComponentsForConfiguration(FSpatialMarkerTrackingFeature& Feature, XrSpatialCapabilityEXT Capability, FNativeSpatialCapabilitiesInfo& CapabilitiesInfo, TArray<XrSpatialComponentTypeEXT>& Components)
{
    if(!CapabilitiesInfo.IsCapabilitySupported(Capability) || !Feature.bEnableMarkerType)
    {
        return;
    }
    Components.Append(CapabilitiesInfo.Capabilities[Capability].SupportedComponents.Array());
}

template<typename T>
void GetCapabilityConfiguration(T& Configuration, FNativeSpatialCapabilitiesInfo& CapabilitiesInfo, FSpatialMarkerTrackingFeature& Feature, TArray<XrSpatialComponentTypeEXT>& Components, TArray<XrSpatialCapabilityConfigurationBaseHeaderEXT*>& Headers)
{
    if(!Feature.bEnableMarkerType || !CapabilitiesInfo.IsCapabilitySupported(Configuration.NativeConfiguration.capability))
    {
        return;
    }
    if(Feature.bOptimizeForMarkerSize && CapabilitiesInfo.IsFeatureSupported(Configuration.NativeConfiguration.capability, XrSpatialCapabilityFeatureEXT::XR_SPATIAL_CAPABILITY_FEATURE_MARKER_TRACKING_FIXED_SIZE_MARKERS_EXT))
    {
        XrSpatialMarkerSizeEXT& MarkerSizeFeature = Configuration.MarkerSizeFeature;
        MarkerSizeFeature.markerSideLength = Feature.MarkerSideLength;
        Configuration.NativeConfiguration.next = &MarkerSizeFeature;
    }

    if(Feature.bOptimizeForStaticMarker && CapabilitiesInfo.IsFeatureSupported(Configuration.NativeConfiguration.capability, XrSpatialCapabilityFeatureEXT::XR_SPATIAL_CAPABILITY_FEATURE_MARKER_TRACKING_STATIC_MARKERS_EXT))
    {
        XrSpatialMarkerStaticOptimizationEXT& StaticSizeFeature = Configuration.StaticMarkerFeature;
        StaticSizeFeature.optimizeForStaticMarker = true;
        Configuration.NativeConfiguration.next = &StaticSizeFeature;
    }
    Configuration.NativeConfiguration.enabledComponentCount = static_cast<uint32_t>(Components.Num());
    Configuration.NativeConfiguration.enabledComponents = Components.GetData();
    Headers.Add(reinterpret_cast<XrSpatialCapabilityConfigurationBaseHeaderEXT*>(&Configuration.NativeConfiguration));
}

void FAndroidXRSpatialMarkerTrackingModule::Configure(const FSpatialMarkerTrackingConfig& InMarkerTrackingConfiguration)
{
    MarkerTrackingConfiguration = InMarkerTrackingConfiguration;
}

void FAndroidXRSpatialMarkerTrackingModule::GetCapabilityConfigurations(TArray<XrSpatialCapabilityConfigurationBaseHeaderEXT*>& Configurations)
{
    EnabledComponents.Reset();
    EnabledComponents.Add(XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_MARKER_EXT);
    EnabledComponents.Add(XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_BOUNDED_2D_EXT);

    ArucoMarkerConfiguiration.NativeConfiguration.arUcoDict = AndroidXR::Convert(MarkerTrackingConfiguration.ArucoMarkerSettings.ArucoDict);
    AprilTagConfiguration.NativeConfiguration.aprilDict = AndroidXR::Convert(MarkerTrackingConfiguration.AprilTagSettings.AprilTagDict);

    GetCapabilityConfiguration(ArucoMarkerConfiguiration, CapabilitiesInfo, MarkerTrackingConfiguration.ArucoMarkerSettings, EnabledComponents, Configurations);
    GetCapabilityConfiguration(AprilTagConfiguration, CapabilitiesInfo, MarkerTrackingConfiguration.AprilTagSettings, EnabledComponents, Configurations);
    GetCapabilityConfiguration(QRCodeConfiguration, CapabilitiesInfo, MarkerTrackingConfiguration.QRCodeSettings, EnabledComponents, Configurations);
    GetCapabilityConfiguration(MicroQRCodeConfiguration, CapabilitiesInfo, MarkerTrackingConfiguration.MicroQRCodeSettings, EnabledComponents, Configurations);
}

FSpatialComponentList* FAndroidXRSpatialMarkerTrackingModule::GetSpatialComponentList(XrSpatialComponentTypeEXT ComponentType)
{
    if(ComponentType == XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_MARKER_EXT)
    {
        return &MarkerComponents;
    }
    return nullptr;
}

void FAndroidXRSpatialMarkerTrackingModule::GetMarkerIntData(UAndroidXRSpatialMarkerEntity* MarkerEntity, XrSpatialSnapshotEXT Snapshot, XrSpatialBufferEXT Buffer)
{
    MarkerEntity->bHasStringData = false;
    FAndroidXRSpatialBuffer IntBuffer{ Buffer };
    TArray<uint8_t> NativeBuffer{};
    if(!GetSpatialBuffer(Snapshot, IntBuffer))
    {
        return;
    }
    IntBuffer.FillData(NativeBuffer);
    Algo::Transform(NativeBuffer, MarkerEntity->MarkerRawData, [](uint8_t Data)
    {
        return static_cast<uint8>(Data);
    });
}

void FAndroidXRSpatialMarkerTrackingModule::GetMarkerStringData(UAndroidXRSpatialMarkerEntity* MarkerEntity, XrSpatialSnapshotEXT Snapshot, XrSpatialBufferEXT Buffer)
{
    MarkerEntity->bHasStringData = true;
    FAndroidXRSpatialBuffer StringBuffer{ Buffer };
    TArray<char> NativeBuffer{};
    if(!GetSpatialBuffer(Snapshot, StringBuffer))
    {
        return;
    }
    StringBuffer.FillData(NativeBuffer);
    MarkerEntity->MarkerStringData = FString{ NativeBuffer };
}

void FAndroidXRSpatialMarkerTrackingModule::ApplyToEntity(TObjectPtr<UAndroidXRSpatialMarkerEntity> MarkerEntity, const FSpatialEntityInfo& EntityInfo)
{
    auto WorldToMetersScale = GEngine->XRSystem->GetWorldToMetersScale();

    XrSpatialMarkerDataEXT MarkerData{};
    XrSpatialBounded2DDataEXT BoundsData{};
    auto BoundsComponent = EntityInfo.Bounds;
    if(!MarkerComponents.Get(EntityInfo.ComponentIndex, MarkerData) || !BoundsComponent->Get(EntityInfo.ComponentIndex, BoundsData))
    {
        return;
    }
    MarkerEntity->bIsValid = true;
    MarkerEntity->MarkerId = MarkerData.markerId;
    MarkerEntity->MarkerScale = ToFVector2D(BoundsData.extents, WorldToMetersScale);
    MarkerEntity->MarkerTransform = ToFTransform(BoundsData.center, WorldToMetersScale);

    FAndroidXRSpatialBuffer DataBuffer{ MarkerData.data };
    switch(MarkerData.capability)
    {
        case XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_ARUCO_MARKER_EXT:
        {
            MarkerEntity->MarkerType = ESpatialMarkerType::Aruco;
            GetMarkerIntData(MarkerEntity, EntityInfo.Snapshot, MarkerData.data);
            break;
        }
        case XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_QR_CODE_EXT:
        {
            MarkerEntity->MarkerType = ESpatialMarkerType::QR;
            GetMarkerStringData(MarkerEntity, EntityInfo.Snapshot, MarkerData.data);
            break;
        }
        case XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_MICRO_QR_CODE_EXT:
        {
            MarkerEntity->MarkerType = ESpatialMarkerType::MicroQR;
            GetMarkerStringData(MarkerEntity, EntityInfo.Snapshot, MarkerData.data);
            break;
        }
        case XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_APRIL_TAG_EXT:
        {
            MarkerEntity->MarkerType = ESpatialMarkerType::AprilTag;
            GetMarkerIntData(MarkerEntity, EntityInfo.Snapshot, MarkerData.data);
            break;
        }
        default:
            break;
    }
}

void FAndroidXRSpatialMarkerTrackingModule::GetCapabilityExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
    OutExtensions.Add(XR_EXT_SPATIAL_MARKER_TRACKING_EXTENSION_NAME);
}

void FAndroidXRSpatialMarkerTrackingModule::UpdateEnabledComponents()
{
    GetComponentsForConfiguration(MarkerTrackingConfiguration.AprilTagSettings, XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_APRIL_TAG_EXT, CapabilitiesInfo, EnabledComponents);
    GetComponentsForConfiguration(MarkerTrackingConfiguration.ArucoMarkerSettings, XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_ARUCO_MARKER_EXT, CapabilitiesInfo, EnabledComponents);
    GetComponentsForConfiguration(MarkerTrackingConfiguration.QRCodeSettings, XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_QR_CODE_EXT, CapabilitiesInfo, EnabledComponents);
    GetComponentsForConfiguration(MarkerTrackingConfiguration.MicroQRCodeSettings, XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_MICRO_QR_CODE_EXT, CapabilitiesInfo, EnabledComponents);
}

IMPLEMENT_MODULE(FAndroidXRSpatialMarkerTrackingModule, AndroidXRSpatialMarkerTracking);