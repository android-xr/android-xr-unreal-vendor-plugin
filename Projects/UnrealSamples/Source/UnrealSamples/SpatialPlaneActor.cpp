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

#include "SpatialPlaneActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UnrealSamples.h"
#include "Components/InstancedStaticMeshComponent.h"

static const FColor kDefaultColor(0x5D, 0xD6, 0xF5, 0xFF);
static constexpr float kBoundsOpacity = 0.25f;
static constexpr float kPolygonOpacity = 0.75f;

static int CreatePolygon(UProceduralMeshComponent* ProceduralMesh,
                          TObjectPtr<UMaterialInstanceDynamic>* MaterialInstance,
                          const TArray<FVector>& Vertexes,
                          const TArray<int32>& Indices)
{
    if(ProceduralMesh)
    {
        auto SectionIndex = ProceduralMesh->GetNumSections();
        ProceduralMesh->CreateMeshSection(SectionIndex, Vertexes, Indices, {}, {}, {}, {}, false);
        if(MaterialInstance)
        {
            ProceduralMesh->SetMaterial(SectionIndex, *MaterialInstance);
        }
        return SectionIndex;
    }
    return -1;
}

inline static FColor LabelToColor(ESpatialPlaneSemanticLabel PlaneLabel)
{
    auto LabelColor(kDefaultColor);
    switch(PlaneLabel)
    {
        case ESpatialPlaneSemanticLabel::Uncategorized:
            LabelColor = FColor::White;
            break;
        case ESpatialPlaneSemanticLabel::Wall:
            LabelColor = FColor::Blue;
            break;
        case ESpatialPlaneSemanticLabel::Floor:
            LabelColor = FColor::Green;
            break;
        case ESpatialPlaneSemanticLabel::Ceiling:
            LabelColor = FColor::Yellow;
            break;
        case ESpatialPlaneSemanticLabel::Table:
            LabelColor = FColor::Orange;
            break;
        default:
            break;
    }
    return LabelColor;
}

void ASpatialPlaneActor::DrawPlanes(const TSet<UAndroidXRSpatialPlaneEntity*>& Planes)
{
    ClearVisuals();
    for(auto Plane : Planes)
    {
        DrawPlane(Plane);
    }
}

void ASpatialPlaneActor::UpdateRenderFilter(const FSpatialPlaneRenderFilter& InRenderFilter)
{
    RenderFilter = InRenderFilter;
    for(auto Plane : DrawnPlanes)
    {
        StopPlaneRender(Plane);
        DrawPlane(Plane);
    }
}

void ASpatialPlaneActor::BeginPlay()
{
    Super::BeginPlay();
    if(PlanePolygonMaterial != nullptr)
    {
        // One material instance for each plane label
        for(int32 LabelIndex = 0; LabelIndex <= static_cast<int32>(ESpatialPlaneSemanticLabel::Table); ++LabelIndex)
        {
            auto MaterialInstance = UMaterialInstanceDynamic::
                Create(PlanePolygonMaterial, PlanePolygonProceduralMesh);

            auto LabelEnum = static_cast<ESpatialPlaneSemanticLabel>(LabelIndex);
            MaterialInstance->SetVectorParameterValue(FName(TEXT("Color")), LabelToColor(LabelEnum));
            MaterialInstance->SetScalarParameterValue(FName(TEXT("Opacity")), kPolygonOpacity);
            PlanePolygonMaterialInstance.Add(LabelEnum, MaterialInstance);
        }
    }
}

void ASpatialPlaneActor::DrawPlane(UAndroidXRSpatialPlaneEntity* Plane)
{
    if(Plane->TrackingState != ESpatialEntityTrackingState::Tracking)
    {
        return;
    }
    auto PlaneColor = LabelToColor(Plane->SemanticLabel);
    auto PlaneLabel = Plane->SemanticLabel;

    if(RenderFilter.bAlignmentFilter && Plane->Alignment != RenderFilter.Alignment)
    {
        return;
    }

    if(RenderFilter.bSemanticFilter && PlaneLabel != RenderFilter.SemanticLabel)
    {
        return;
    }

    if(RenderFilter.bShouldRenderBounds)
    {
        auto BoundsIndex = RenderBounds(Plane, PlaneColor);
        BoundsIndexTable.FindOrAdd(Plane) = BoundsIndex;
    }
    if(RenderFilter.bShouldRenderMesh)
    {
        auto MeshSectionIndex = RenderMesh(Plane->PlaneMesh, PlaneLabel);
        MeshIndexTable.FindOrAdd(Plane) = MeshSectionIndex;
    }
    if(RenderFilter.bShouldRenderPolygons)
    {
        auto PolygonIndex = RenderPolygon(Plane->PlanePolygon, PlaneLabel);
        PolygonIndexTable.FindOrAdd(Plane) = PolygonIndex;
    }
}

void ASpatialPlaneActor::StopPlaneRender(UAndroidXRSpatialPlaneEntity* Plane)
{
    auto BoundsIndexEntry = BoundsIndexTable.Find(Plane);
    if(BoundsIndexEntry && PlaneBoundsInstancedMesh->IsValidInstance(*BoundsIndexEntry))
    {
        PlaneBoundsInstancedMesh->RemoveInstance(*BoundsIndexEntry);
    }

    auto PolygonIndex = PolygonIndexTable.Find(Plane);
    if(PolygonIndex)
    {
        PlanePolygonProceduralMesh->ClearMeshSection(*PolygonIndex);
    }

    auto MeshIndex = MeshIndexTable.Find(Plane);
    if(MeshIndex)
    {
        PlanePolygonProceduralMesh->ClearMeshSection(*MeshIndex);
    }
}

ASpatialPlaneActor::ASpatialPlaneActor()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
    SetRootComponent(RootComponent);

    PlaneBoundsInstancedMesh = CreateDefaultSubobject
        <UInstancedStaticMeshComponent>(TEXT("BoundsMesh"));
    PlaneBoundsInstancedMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
    PlaneBoundsInstancedMesh->SetGenerateOverlapEvents(false);
    PlaneBoundsInstancedMesh->SetNumCustomDataFloats(4); // RGBA floats
    PlaneBoundsInstancedMesh->SetupAttachment(RootComponent);

    PlanePolygonProceduralMesh = CreateDefaultSubobject
        <UProceduralMeshComponent>(TEXT("PolygonMesh"));
    PlanePolygonProceduralMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
    PlanePolygonProceduralMesh->SetGenerateOverlapEvents(false);
    PlanePolygonProceduralMesh->SetupAttachment(RootComponent);

    SetActorEnableCollision(false);
}

void ASpatialPlaneActor::ClearVisuals()
{
    PositionAndColorIndex.Empty();
    PlanePolygonProceduralMesh->ClearAllMeshSections();
    PlaneBoundsInstancedMesh->ClearInstances();
}

int ASpatialPlaneActor::RenderBounds(UAndroidXRSpatialPlaneEntity* Plane, const FColor& PlaneColor)
{
    if(auto StaticMesh = PlaneBoundsInstancedMesh->GetStaticMesh())
    {
        auto BoxSphereBounds = StaticMesh->GetBounds();

        auto InstanceTransform(Plane->PlaneTransform);
        InstanceTransform.SetScale3D(FVector(
            0.01f,
            Plane->PlaneExtents.X / BoxSphereBounds.BoxExtent.Y,
            Plane->PlaneExtents.Y / BoxSphereBounds.BoxExtent.Z
        ));

        auto InstanceIndex = PlaneBoundsInstancedMesh->AddInstance(InstanceTransform, true);

        PlaneBoundsInstancedMesh->SetCustomData(InstanceIndex,
                                                {
                                                    PlaneColor.R / 255.0f,
                                                    PlaneColor.G / 255.0f,
                                                    PlaneColor.B / 255.0f,
                                                    kBoundsOpacity
                                                });
        return InstanceIndex;
    }
    return 0;
}

int ASpatialPlaneActor::RenderPolygon(const FSpatialPlanePolygon& Polygon, ESpatialPlaneSemanticLabel Label)
{
    if(Polygon.Vertices.Num() > 0)
    {
        auto MaterialInstance = PlanePolygonMaterialInstance.Find(Label);
        TArray<FVector> Vertices{};
        Vertices.Add(Polygon.PolygonOrigin.GetLocation());
        Vertices.Append(Polygon.Vertices);
        auto& Indices = GetIndexBuffer(Vertices.Num());
        return CreatePolygon(PlanePolygonProceduralMesh, MaterialInstance,
                      Vertices, Indices);
    }
    return 0;
}

int ASpatialPlaneActor::RenderMesh(const FSpatialPlaneMesh& Mesh, ESpatialPlaneSemanticLabel Label)
{
    auto MaterialInstance = PlanePolygonMaterialInstance.Find(Label);
    return CreatePolygon(PlanePolygonProceduralMesh,  MaterialInstance, Mesh.Vertices, Mesh.Indices);
}

TArray<int32>& ASpatialPlaneActor::GetIndexBuffer(int VertexCount)
{
    auto IndexBufferPtr = IndexBufferTable.Find(VertexCount);
    if(IndexBufferPtr)
    {
        return *IndexBufferPtr;
    }
    auto& Indices = IndexBufferTable.Add(VertexCount);
    for(auto Index = 1; Index < VertexCount; Index++)
    {
        Indices.Add(0);
        Indices.Add(Index);
        if(Index + 1 < VertexCount)
        {
            Indices.Add(Index + 1);
        }
        else
        {
            Indices.Add(1);
        }
    }
    return Indices;
}

