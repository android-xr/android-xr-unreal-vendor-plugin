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
#include "AndroidXRSpatialMarkerTrackingTypes.h"

namespace AndroidXR
{
    inline ESpatialMarkerAprilTagDict Convert(XrSpatialMarkerAprilTagDictEXT NativeType)
    {
        switch(NativeType)
        {
            case XR_SPATIAL_MARKER_APRIL_TAG_DICT_16H5_EXT:
                return ESpatialMarkerAprilTagDict::DICT_16H5_EXT;
            case XR_SPATIAL_MARKER_APRIL_TAG_DICT_25H9_EXT:
                return ESpatialMarkerAprilTagDict::DICT_25H9_EXT;
            case XR_SPATIAL_MARKER_APRIL_TAG_DICT_36H10_EXT:
                return ESpatialMarkerAprilTagDict::DICT_36H10_EXT;
            case XR_SPATIAL_MARKER_APRIL_TAG_DICT_36H11_EXT:
                return ESpatialMarkerAprilTagDict::DICT_36H11_EXT;
            default:
                break;
        }
        return ESpatialMarkerAprilTagDict::DICT_16H5_EXT;
    }

    inline XrSpatialMarkerAprilTagDictEXT Convert(ESpatialMarkerAprilTagDict UnrealType)
    {
        switch(UnrealType)
        {
            case ESpatialMarkerAprilTagDict::DICT_16H5_EXT:
                return XrSpatialMarkerAprilTagDictEXT::XR_SPATIAL_MARKER_APRIL_TAG_DICT_16H5_EXT;
            case ESpatialMarkerAprilTagDict::DICT_25H9_EXT:
                return XrSpatialMarkerAprilTagDictEXT::XR_SPATIAL_MARKER_APRIL_TAG_DICT_25H9_EXT;
            case ESpatialMarkerAprilTagDict::DICT_36H10_EXT:
                return XrSpatialMarkerAprilTagDictEXT::XR_SPATIAL_MARKER_APRIL_TAG_DICT_36H10_EXT;
            case ESpatialMarkerAprilTagDict::DICT_36H11_EXT:
                return XrSpatialMarkerAprilTagDictEXT::XR_SPATIAL_MARKER_APRIL_TAG_DICT_36H11_EXT;
            default:
                break;
        }
        return XrSpatialMarkerAprilTagDictEXT::XR_SPATIAL_MARKER_APRIL_TAG_DICT_16H5_EXT;
    }

    inline ESpatialMarkerArucoDict Convert(XrSpatialMarkerArucoDictEXT NativeType)
    {
        switch(NativeType)
        {
            case XR_SPATIAL_MARKER_ARUCO_DICT_4X4_50_EXT:
                return ESpatialMarkerArucoDict::DICT_4X4_50_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_4X4_100_EXT:
                return ESpatialMarkerArucoDict::DICT_4X4_100_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_4X4_250_EXT:
                return ESpatialMarkerArucoDict::DICT_4X4_250_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_4X4_1000_EXT:
                return ESpatialMarkerArucoDict::DICT_4X4_1000_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_5X5_50_EXT:
                return ESpatialMarkerArucoDict::DICT_5X5_50_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_5X5_100_EXT:
                return ESpatialMarkerArucoDict::DICT_5X5_100_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_5X5_250_EXT:
                return ESpatialMarkerArucoDict::DICT_5X5_250_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_5X5_1000_EXT:
                return ESpatialMarkerArucoDict::DICT_5X5_1000_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_6X6_50_EXT:
                return ESpatialMarkerArucoDict::DICT_6X6_50_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_6X6_100_EXT:
                return ESpatialMarkerArucoDict::DICT_6X6_100_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_6X6_250_EXT:
                return ESpatialMarkerArucoDict::DICT_6X6_250_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_6X6_1000_EXT:
                return ESpatialMarkerArucoDict::DICT_6X6_1000_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_7X7_50_EXT:
                return ESpatialMarkerArucoDict::DICT_7X7_50_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_7X7_100_EXT:
                return ESpatialMarkerArucoDict::DICT_7X7_100_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_7X7_250_EXT:
                return ESpatialMarkerArucoDict::DICT_7X7_250_EXT;
            case XR_SPATIAL_MARKER_ARUCO_DICT_7X7_1000_EXT:
                return ESpatialMarkerArucoDict::DICT_7X7_1000_EXT;
            default:
                break;
        }
        return ESpatialMarkerArucoDict::DICT_4X4_50_EXT;
    }

    inline XrSpatialMarkerArucoDictEXT Convert(ESpatialMarkerArucoDict UnrealType)
    {
        switch(UnrealType)
        {
            case ESpatialMarkerArucoDict::DICT_4X4_50_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_4X4_50_EXT;
            case ESpatialMarkerArucoDict::DICT_4X4_100_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_4X4_100_EXT;
            case ESpatialMarkerArucoDict::DICT_4X4_250_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_4X4_250_EXT;
            case ESpatialMarkerArucoDict::DICT_4X4_1000_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_4X4_1000_EXT;
            case ESpatialMarkerArucoDict::DICT_5X5_50_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_5X5_50_EXT;
            case ESpatialMarkerArucoDict::DICT_5X5_100_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_5X5_100_EXT;
            case ESpatialMarkerArucoDict::DICT_5X5_250_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_5X5_250_EXT;
            case ESpatialMarkerArucoDict::DICT_5X5_1000_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_5X5_1000_EXT;
            case ESpatialMarkerArucoDict::DICT_6X6_50_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_6X6_50_EXT;
            case ESpatialMarkerArucoDict::DICT_6X6_100_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_6X6_100_EXT;
            case ESpatialMarkerArucoDict::DICT_6X6_250_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_6X6_250_EXT;
            case ESpatialMarkerArucoDict::DICT_6X6_1000_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_6X6_1000_EXT;
            case ESpatialMarkerArucoDict::DICT_7X7_50_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_7X7_50_EXT;
            case ESpatialMarkerArucoDict::DICT_7X7_100_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_7X7_100_EXT;
            case ESpatialMarkerArucoDict::DICT_7X7_250_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_7X7_250_EXT;
            case ESpatialMarkerArucoDict::DICT_7X7_1000_EXT:
                return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_7X7_1000_EXT;
            default:
                break;
        }
        return XrSpatialMarkerArucoDictEXT::XR_SPATIAL_MARKER_ARUCO_DICT_4X4_50_EXT;
    }
}