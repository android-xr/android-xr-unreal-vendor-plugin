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
#include "AndroidXRSpatialPlaneEntity.h"
#include "AndroidXRSpatialDiscoveryRaycastTypes.generated.h"

/**
* The ray configuration to perform raycasts with.
*/
USTRUCT(BlueprintType)
struct FSpatialRaycastConfig
{
    GENERATED_BODY()

    //The origin of the ray
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Raycast")
    FVector RayOrigin{};

    //The direction of the ray
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Raycast")
    FVector RayDirection{};

    //The length of the ray
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Raycast")
    float RayLength{};
};

/**
* The raycast hit information
*/
USTRUCT(BlueprintType)
struct FSpatialRaycastHitInfo
{
    GENERATED_BODY()

    //The hit transform of the raycast
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Raycast")
    FTransform HitTransform{};

    //The hit distance squared from the ray origin to the hit entity
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Raycast")
    float DistanceSquared{};
};

/**
* The raycast entity hit result
*/
USTRUCT(BlueprintType)
struct FSpatialRaycastEntityResult
{
    GENERATED_BODY()

    //The hit info of the raycast
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Raycast")
    FSpatialRaycastHitInfo HitInfo{};

    //The Id of the entity hit
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Raycast")
    FGuid EntityId{};
};

/**
* The raycast plane hit result
*/
USTRUCT(BlueprintType)
struct FSpatialRaycastPlaneResult
{
    GENERATED_BODY()

    //The hit info of the raycast
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Raycast")
    FSpatialRaycastHitInfo HitInfo{};

    //The hit plane by the raycast
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Raycast")
    UAndroidXRSpatialPlaneEntity* HitPlane{};
};
