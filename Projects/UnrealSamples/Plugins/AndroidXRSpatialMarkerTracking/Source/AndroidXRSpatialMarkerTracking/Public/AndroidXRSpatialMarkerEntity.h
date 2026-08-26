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
#include "AndroidXRSpatialEntity.h"
#include "AndroidXRSpatialMarkerEntity.generated.h"

/**
* The marker type
*/
UENUM(BlueprintType)
enum class ESpatialMarkerType : uint8
{
	//Aruco marker
	Aruco,
	//QR code
	QR,
	//MicroQR code
	MicroQR,
	//April tag
	AprilTag
};

/**
* A spatial marker entity
*/
UCLASS(BlueprintType)
class ANDROIDXRSPATIALMARKERTRACKING_API UAndroidXRSpatialMarkerEntity : public UAndroidXRSpatialEntity
{
	GENERATED_BODY()
public:

	/**
	* Whether the marker entity is valid
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Marker Tracking")
	bool bIsValid{};

	/**
	* The type of marker associated with this entity
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Marker Tracking")
	ESpatialMarkerType MarkerType{};

	/**
	* The id of the marker
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Marker Tracking")
	int MarkerId{};

	/**
	* The transform of the marker
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Marker Tracking")
	FTransform MarkerTransform{};

	/**
	* The 2D extents of the marker
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Marker Tracking")
	FVector2D MarkerScale{};

	/**
	* Whether the marker has string data or a byte buffer
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Marker Tracking")
	bool bHasStringData{};

	/**
	* The string data of the marker
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Marker Tracking")
	FString MarkerStringData{};

	/**
	* The byte buffer if the marker does not support strings
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Marker Tracking")
	TArray<uint8> MarkerRawData{};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpatialMarkerChangedDelegate, UAndroidXRSpatialMarkerEntity*, AddedMarker);
