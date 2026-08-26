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
#include "AndroidXRSpatialPlaneTrackingTypes.h"
#include "AndroidXRSpatialEntity.h"
#include "AndroidXRSpatialPlaneEntity.generated.h"

/**
 * A spatial plane's mesh data
 */
USTRUCT(BlueprintType)
struct FSpatialPlaneMesh
{
    GENERATED_BODY()

    /**
    * Whether the mesh data is valid
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Plane Tracking")
    bool bIsValid{};

    /**
    * The origin of the plane mesh
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Plane Tracking")
    FTransform MeshOrigin{};

    /**
    * The vertices of the plane mesh
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Plane Tracking")
    TArray<FVector> Vertices{};

    /**
    * The indices of the plane mesh
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Plane Tracking")
    TArray<int> Indices{};
};

/**
* A spatial plane's polygon data
*/
USTRUCT(BlueprintType)
struct FSpatialPlanePolygon
{
    GENERATED_BODY()

    /**
    * Whether the polygon data is valid
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Plane Tracking")
    bool bIsValid{};

    /**
    * The origin of the plane polygon
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Plane Tracking")
    FTransform PolygonOrigin{};

    /**
    * The vertices of the spatial plane's polygon
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Plane Tracking")
    TArray<FVector> Vertices{};
};

/**
* A spatial plane entity
*/
UCLASS(BlueprintType)
class ANDROIDXRSPATIALPLANETRACKING_API UAndroidXRSpatialPlaneEntity : public UAndroidXRSpatialEntity
{
    GENERATED_BODY()
public:

    /**
    * The transform of the spatial plane
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Plane Tracking")
    FTransform PlaneTransform{};

    /**
    * The extents of the spatial plane
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Plane Tracking")
    FVector2D PlaneExtents{};

    /**
    * The spatial plane alignment
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Plane Tracking")
    ESpatialPlaneAlignment Alignment{};

    /**
    * The spatial plane's semantic label
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Plane Tracking")
    ESpatialPlaneSemanticLabel SemanticLabel{};

    /**
    * The mesh data of the spatial plane
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Plane Tracking")
    FSpatialPlaneMesh PlaneMesh{};

    /**
    * The polygon data of the spatial plane
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Plane Tracking")
    FSpatialPlanePolygon PlanePolygon{};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpatialPlaneChangedDelegate, UAndroidXRSpatialPlaneEntity*, Plane);