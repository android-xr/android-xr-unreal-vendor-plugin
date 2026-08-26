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
#include "AndroidXRSpatialMarkerTrackingTypes.generated.h"

/**
* The types of April tag dicts
*/
UENUM(BlueprintType)
enum class ESpatialMarkerAprilTagDict : uint8
{
    DICT_16H5_EXT,
    DICT_25H9_EXT,
    DICT_36H10_EXT,
    DICT_36H11_EXT,
};

/**
* The types of Aruco marker dicts
*/
UENUM(BlueprintType)
enum class ESpatialMarkerArucoDict : uint8
{
    DICT_4X4_50_EXT,
    DICT_4X4_100_EXT,
    DICT_4X4_250_EXT,
    DICT_4X4_1000_EXT,
    DICT_5X5_50_EXT,
    DICT_5X5_100_EXT,
    DICT_5X5_250_EXT,
    DICT_5X5_1000_EXT,
    DICT_6X6_50_EXT,
    DICT_6X6_100_EXT,
    DICT_6X6_250_EXT,
    DICT_6X6_1000_EXT,
    DICT_7X7_50_EXT,
    DICT_7X7_100_EXT,
    DICT_7X7_250_EXT,
    DICT_7X7_1000_EXT
};

/**
* The configuration settings for a specific marker type
*/
USTRUCT(BlueprintType)
struct FSpatialMarkerTrackingFeature
{
    GENERATED_BODY()

    /**
    * Whether to enable this marker type
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Marker Tracking")
    bool bEnableMarkerType{true};

    /**
    * Whether to optimize for static markerszs
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Marker Tracking")
    bool bOptimizeForStaticMarker{};

    /**
    * Whether to optimize for a marker size
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Marker Tracking")
    bool bOptimizeForMarkerSize{};

    /**
    * The marker length to optimize for
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Marker Tracking")
    float MarkerSideLength{};

    /**
    * The Aruco dict to look for
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Marker Tracking")
    ESpatialMarkerArucoDict ArucoDict{};

    /**
    * The April tag dict to look for
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Marker Tracking")
    ESpatialMarkerAprilTagDict AprilTagDict{};
};

/**
* The marker tracking configuration for all the marker types
*/
USTRUCT(BlueprintType)
struct FSpatialMarkerTrackingConfig
{
    GENERATED_BODY()

    /**
    * The QR code configuration
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Marker Tracking|QR Code")
    FSpatialMarkerTrackingFeature QRCodeSettings{};

    /**
    * The Aruco marker configuration
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Marker Tracking|Aruco Marker")
    FSpatialMarkerTrackingFeature ArucoMarkerSettings{};

    /**
    * The Micro QR code configuration
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Marker Tracking|Micro QR Code")
    FSpatialMarkerTrackingFeature MicroQRCodeSettings{};

    /**
    * The April tag configuration
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Marker Tracking|April Tag")
    FSpatialMarkerTrackingFeature AprilTagSettings{};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpatialMarkerChangedDelegate, UAndroidXRSpatialMarkerEntity*, Marker);