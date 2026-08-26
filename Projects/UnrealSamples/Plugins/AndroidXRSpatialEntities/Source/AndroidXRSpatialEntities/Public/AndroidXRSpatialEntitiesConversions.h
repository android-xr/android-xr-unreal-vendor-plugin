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
namespace AndroidXR
{
    inline ESpatialCapability Convert(XrSpatialCapabilityEXT NativeType)
    {
        switch(NativeType)
        {
            case XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_PLANE_TRACKING_EXT: return ESpatialCapability::PlaneTracking;
            case XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_QR_CODE_EXT: return ESpatialCapability::MarkerTracking_QR_Code;
            case XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_MICRO_QR_CODE_EXT: return ESpatialCapability::MarkerTracking_Micro_QR_Code;
            case XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_ARUCO_MARKER_EXT: return ESpatialCapability::MarkerTracking_Aruco;
            case XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_APRIL_TAG_EXT: return ESpatialCapability::MarkerTracking_April_Tag;
            case XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_ANCHOR_EXT: return ESpatialCapability::Anchor;
            case XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_OBJECT_TRACKING_ANDROID: return ESpatialCapability::ObjectTracking;
            case XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_DEPTH_RAYCAST_ANDROID: return ESpatialCapability::DepthRaycast;
        }
        return ESpatialCapability::PlaneTracking;
    }

    inline XrSpatialCapabilityEXT Convert(ESpatialCapability UnrealType)
    {
        switch(UnrealType) {
            case ESpatialCapability::DepthRaycast: return XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_DEPTH_RAYCAST_ANDROID;
            case ESpatialCapability::PlaneTracking: return XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_PLANE_TRACKING_EXT;
            case ESpatialCapability::MarkerTracking_QR_Code: return XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_QR_CODE_EXT;
            case ESpatialCapability::MarkerTracking_Micro_QR_Code: return XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_MICRO_QR_CODE_EXT;
            case ESpatialCapability::MarkerTracking_Aruco: return XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_ARUCO_MARKER_EXT;
            case ESpatialCapability::MarkerTracking_April_Tag: return XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_MARKER_TRACKING_APRIL_TAG_EXT;
            case ESpatialCapability::Anchor: return XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_ANCHOR_EXT;
            case ESpatialCapability::ObjectTracking: return XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_OBJECT_TRACKING_ANDROID;
        }
        return XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_PLANE_TRACKING_EXT;
    }

    inline ESpatialComponentType Convert(XrSpatialComponentTypeEXT NativeType)
    {
        switch(NativeType)
        {
            case XR_SPATIAL_COMPONENT_TYPE_BOUNDED_2D_EXT:
                return ESpatialComponentType::Bounded2D;
            case XR_SPATIAL_COMPONENT_TYPE_BOUNDED_3D_EXT:
                return ESpatialComponentType::Bounded3D;
            case XR_SPATIAL_COMPONENT_TYPE_PARENT_EXT:
                return ESpatialComponentType::Parent;
            case XR_SPATIAL_COMPONENT_TYPE_MESH_3D_EXT:
                return ESpatialComponentType::Mesh3D;
            case XR_SPATIAL_COMPONENT_TYPE_PLANE_ALIGNMENT_EXT:
                return ESpatialComponentType::PlaneAlignment;
            case XR_SPATIAL_COMPONENT_TYPE_MESH_2D_EXT:
                return ESpatialComponentType::Mesh2D;
            case XR_SPATIAL_COMPONENT_TYPE_POLYGON_2D_EXT:
                return ESpatialComponentType::Polygon2D;
            case XR_SPATIAL_COMPONENT_TYPE_PLANE_SEMANTIC_LABEL_EXT:
                return ESpatialComponentType::PlaneSemanticLabel;
            case XR_SPATIAL_COMPONENT_TYPE_MARKER_EXT:
                return ESpatialComponentType::Marker;
            case XR_SPATIAL_COMPONENT_TYPE_ANCHOR_EXT:
                return ESpatialComponentType::Anchor;
            case XR_SPATIAL_COMPONENT_TYPE_PERSISTENCE_EXT:
                return ESpatialComponentType::Persistence;
            case XR_SPATIAL_COMPONENT_TYPE_OBJECT_SEMANTIC_LABEL_ANDROID:
                return ESpatialComponentType::ObjectSemanticLabel;
            case XR_SPATIAL_COMPONENT_TYPE_RAYCAST_RESULT_ANDROID:
                return ESpatialComponentType::RaycastResult;
            case XR_SPATIAL_COMPONENT_TYPE_SUBSUMED_BY_ANDROID:
                return ESpatialComponentType::SubsumedByAndroid;
            default:
                break;
        }
        return ESpatialComponentType::None;
    }

    inline XrSpatialComponentTypeEXT Convert(ESpatialComponentType UnrealType)
    {
        switch(UnrealType)
        {
            case ESpatialComponentType::Bounded2D:
                return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_BOUNDED_2D_EXT;
            case ESpatialComponentType::Bounded3D:
                return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_BOUNDED_3D_EXT;
            case ESpatialComponentType::Parent:
                return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_PARENT_EXT;
            case ESpatialComponentType::Mesh3D:
                return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_MESH_3D_EXT;
            case ESpatialComponentType::PlaneAlignment:
                return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_PLANE_ALIGNMENT_EXT;
            case ESpatialComponentType::Mesh2D:
                return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_MESH_2D_EXT;
            case ESpatialComponentType::Polygon2D:
                return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_POLYGON_2D_EXT;
            case ESpatialComponentType::PlaneSemanticLabel:
                return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_PLANE_SEMANTIC_LABEL_EXT;
            case ESpatialComponentType::Marker:
                return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_MARKER_EXT;
            case ESpatialComponentType::Anchor:
                return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_ANCHOR_EXT;
            case ESpatialComponentType::Persistence:
                return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_PERSISTENCE_EXT;
            case ESpatialComponentType::ObjectSemanticLabel:
                return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_OBJECT_SEMANTIC_LABEL_ANDROID;
            case ESpatialComponentType::RaycastResult:
                return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_RAYCAST_RESULT_ANDROID;
            case ESpatialComponentType::SubsumedByAndroid:
                return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_SUBSUMED_BY_ANDROID;
            default:
                break;
        }
        return XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_BOUNDED_2D_EXT;
    }

    inline XrSpatialCapabilityFeatureEXT Convert(ESpatialCapabilityFeature UnrealType)
    {
        switch(UnrealType)
        {
            case ESpatialCapabilityFeature::FixedSizeMarkers:
                return XrSpatialCapabilityFeatureEXT::XR_SPATIAL_CAPABILITY_FEATURE_MARKER_TRACKING_FIXED_SIZE_MARKERS_EXT;
            case ESpatialCapabilityFeature::StaticMarkers:
                return XrSpatialCapabilityFeatureEXT::XR_SPATIAL_CAPABILITY_FEATURE_MARKER_TRACKING_STATIC_MARKERS_EXT;
            default:
                break;
        }
        return XrSpatialCapabilityFeatureEXT::XR_SPATIAL_CAPABILITY_FEATURE_MAX_ENUM_EXT;
    }

    inline ESpatialCapabilityFeature Convert(XrSpatialCapabilityFeatureEXT NativeType)
    {
        switch(NativeType)
        {
            case XR_SPATIAL_CAPABILITY_FEATURE_MARKER_TRACKING_FIXED_SIZE_MARKERS_EXT:
                return ESpatialCapabilityFeature::FixedSizeMarkers;
            case XR_SPATIAL_CAPABILITY_FEATURE_MARKER_TRACKING_STATIC_MARKERS_EXT:
                return ESpatialCapabilityFeature::StaticMarkers;
            default:
                break;
        }
        return ESpatialCapabilityFeature::StaticMarkers;
    }

    inline XrSpatialEntityTrackingStateEXT Convert(ESpatialEntityTrackingState UnrealType)
    {
        switch(UnrealType)
        {
            case ESpatialEntityTrackingState::Stopped:
                return XrSpatialEntityTrackingStateEXT::XR_SPATIAL_ENTITY_TRACKING_STATE_STOPPED_EXT;
            case ESpatialEntityTrackingState::Paused:
                return XrSpatialEntityTrackingStateEXT::XR_SPATIAL_ENTITY_TRACKING_STATE_PAUSED_EXT;
            case ESpatialEntityTrackingState::Tracking:
                return XrSpatialEntityTrackingStateEXT::XR_SPATIAL_ENTITY_TRACKING_STATE_MAX_ENUM_EXT;
            default:
                break;
        }
        return XrSpatialEntityTrackingStateEXT::XR_SPATIAL_ENTITY_TRACKING_STATE_STOPPED_EXT;
    }

    inline ESpatialEntityTrackingState Convert(XrSpatialEntityTrackingStateEXT NativeType)
    {
        switch(NativeType)
        {
            case XR_SPATIAL_ENTITY_TRACKING_STATE_STOPPED_EXT:
                return ESpatialEntityTrackingState::Stopped;
            case XR_SPATIAL_ENTITY_TRACKING_STATE_PAUSED_EXT:
                return ESpatialEntityTrackingState::Paused;
            case XR_SPATIAL_ENTITY_TRACKING_STATE_TRACKING_EXT:
                return ESpatialEntityTrackingState::Tracking;
            default:
                break;
        }
        return ESpatialEntityTrackingState::Stopped;
    }

    inline FGuid Convert(XrSpatialEntityIdEXT NativeId)
    {
        return FGuid
        {
            static_cast<uint32>(NativeId >> 32),
            static_cast<uint32>(NativeId & 0xFFFFFFFF),
            0,
            0
        };
    }

    inline XrSpatialEntityIdEXT Convert(const FGuid& UnrealType)
    {
        int64 Result;
        Result = static_cast<int64>(UnrealType.A) << 32;
        Result |= UnrealType.B;
        return static_cast<XrSpatialEntityIdEXT>(Result);
    }
}
