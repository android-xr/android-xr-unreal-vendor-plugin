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
#include "GameFramework/Actor.h"
#include "AndroidXRSpatialPlaneTrackingTypes.h"
#include "AndroidXRSpatialPlaneEntity.h"
#include "ProceduralMeshComponent.h"
#include "SpatialPlaneActor.generated.h"

USTRUCT(BlueprintType)
struct FSpatialPlaneRenderFilter
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bShouldRenderBounds{true};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bShouldRenderMesh{true};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bShouldRenderPolygons{true};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAlignmentFilter{};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESpatialPlaneAlignment Alignment{};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSemanticFilter{};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESpatialPlaneSemanticLabel SemanticLabel{};
};

typedef struct FSpatialPlaneInfo
{
    int ProceduralMeshSection{};
    int BoundsInstancedIndex{};
    int PolygonMeshSection{};
}FSpatialPlaneInfo;

UCLASS(Blueprintable)
class UNREALSAMPLES_API ASpatialPlaneActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASpatialPlaneActor();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UInstancedStaticMeshComponent> PlaneBoundsInstancedMesh;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UProceduralMeshComponent> PlanePolygonProceduralMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UMaterialInterface> PlanePolygonMaterial;

    TArray<FVector4> PositionAndColorIndex;

    UPROPERTY()
    TMap<ESpatialPlaneSemanticLabel, TObjectPtr<UMaterialInstanceDynamic>> PlanePolygonMaterialInstance;

    UFUNCTION(BlueprintCallable)
    void DrawPlanes(const TSet<UAndroidXRSpatialPlaneEntity*>& Planes);

    UFUNCTION(BlueprintCallable)
    void UpdateRenderFilter(const FSpatialPlaneRenderFilter& InRenderFilter);
protected:
    void BeginPlay() override;
public:

    UFUNCTION(BlueprintCallable)
    void ClearVisuals();

    int RenderBounds(UAndroidXRSpatialPlaneEntity* Plane, const FColor& PlaneColor);
    int RenderPolygon(const FSpatialPlanePolygon& Polygon, ESpatialPlaneSemanticLabel Label);
    int RenderMesh(const FSpatialPlaneMesh& Mesh, ESpatialPlaneSemanticLabel PlaneLabel);
    TArray<int32>& GetIndexBuffer(int VertexCount);
private:
    FSpatialPlaneRenderFilter RenderFilter{};
    TMap<int, TArray<int32>> IndexBufferTable{};
    TMap<UAndroidXRSpatialPlaneEntity*, int> BoundsIndexTable{};
    TMap<UAndroidXRSpatialPlaneEntity*, int> MeshIndexTable{};
    TMap<UAndroidXRSpatialPlaneEntity*, int> PolygonIndexTable{};
    TSet<UAndroidXRSpatialPlaneEntity*> DrawnPlanes{};

    void DrawPlane(UAndroidXRSpatialPlaneEntity* Plane);
    void StopPlaneRender(UAndroidXRSpatialPlaneEntity* Plane);
};