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

#include "AndroidXRSceneMeshingComponent.h"
#include "IAndroidXRSceneMeshingModule.h"
#include "Components/BoxComponent.h"
#include "AndroidXRTypeConversions.h"

// Holds brick data for MR Mesh until the brick is constructed
struct FCachedMeshBrickData
{
    IMRMesh::FBrickId BrickId{};
    TArray<FVector3f> VertexPositions;
    TArray<FVector2D> VertexUVs;
    TArray<FPackedNormal> VertexTangentXZs;
    TArray<FColor> VertexColors;
    TArray<MRMESH_INDEX_TYPE> Indexes;
    FBox BoxBounds;

    FCachedMeshBrickData(int32 InBrickId,
        const FAndroidXRSceneSubmeshState& SubmeshState,
        const FAndroidXRSceneSubmeshData& SubmeshData)
    : BrickId(InBrickId)
    {
        auto ToVector3f = [](const FVector& Vector)
        {
            return FVector3f(Vector.X, Vector.Y, Vector.Z);
        };

        // Convert vertexes
        VertexPositions.Reserve(SubmeshData.VertexPositions.Num());
        VertexColors.Reserve(SubmeshData.VertexPositions.Num());

        for (int32 VertexIndex = 0; VertexIndex < SubmeshData.VertexPositions.Num(); ++VertexIndex)
        {
            // Convert vertex position to world space
            auto WorldVertexPosition = SubmeshState.PoseInBaseSpace.
                TransformPositionNoScale(SubmeshData.VertexPositions[VertexIndex]);
            VertexPositions.Add(ToVector3f(WorldVertexPosition));

            // Semantic vertex coloring
            auto VertexColor = FColor::White;
            switch (AndroidXR::Convert(static_cast
                <XrSceneMeshSemanticLabelANDROID>(SubmeshData.VertexSemantics[VertexIndex])))
            {
                case ESceneMeshSemanticLabel::Floor:
                    VertexColor = FColor::Red;
                    break;
                case ESceneMeshSemanticLabel::Ceiling:
                    VertexColor = FColor::Green;
                    break;
                case ESceneMeshSemanticLabel::Wall:
                    VertexColor = FColor::Blue;
                    break;
                case ESceneMeshSemanticLabel::Table:
                    VertexColor = FColor::Orange;
                    break;
                case ESceneMeshSemanticLabel::Other:
                default:
                    break;
            }
            VertexColors.Add(VertexColor);
        }

        // Bounds
        BoxBounds = FBox::BuildAABB(SubmeshState.
            PoseInBaseSpace.GetLocation(), SubmeshState.Extents);

        // Indexes
        Indexes.Reserve(SubmeshData.Indexes.Num());
        for (const auto& Index : SubmeshData.Indexes)
        {
            Indexes.Add(Index);
        }

        // Per the comment in MRMeshComponent.cpp::RenderThread_UploadNewSection,
        // At least one set of UVs, VertexColors, and TangentXZs are all required

        // UVs
        VertexUVs.AddZeroed(VertexPositions.Num());

        // XZ tangents
        if (SubmeshData.VertexNormals.IsEmpty())
        {
            // Won't light correctly but also won't crash
            VertexTangentXZs.AddZeroed(SubmeshData.VertexPositions.Num() * 2);
        }
        else
        {
            VertexTangentXZs.Reserve(SubmeshData.VertexPositions.Num() * 2);
            for (const auto& Norm : SubmeshData.VertexNormals)
            {
                auto Perp = Norm.X < Norm.Z ?
                    FVector::XAxisVector : FVector::YAxisVector;
                auto Tang = FVector::CrossProduct(Norm, Perp).GetSafeNormal();

                VertexTangentXZs.Add(Tang);
                VertexTangentXZs.Add(Norm);
            }
        }
    }
};

// Holds onto the cached data until MR Mesh has received and used it
class FMeshBrickDataReceipt : public IMRMesh::FBrickDataReceipt
{
protected:
    TSharedPtr<FCachedMeshBrickData, ESPMode::ThreadSafe> SubmeshData;
public:
    FMeshBrickDataReceipt(TSharedPtr<FCachedMeshBrickData, ESPMode::ThreadSafe> InMeshData)
    : SubmeshData(InMeshData)
    {

    }
};

UAndroidXRSceneMeshingComponent::UAndroidXRSceneMeshingComponent()
{
    bAutoActivate = true;
    SearchVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SearchVolume"));
    SearchVolume->SetupAttachment(this);
    SearchVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SearchVolume->SetCanEverAffectNavigation(false);
    SearchVolume->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
    SearchVolume->SetBoxExtent(FVector(50.0f), false);
}

void UAndroidXRSceneMeshingComponent::BeginPlay()
{
    Super::BeginPlay();

    // Look for a child that is a MR Mesh component
    TArray<USceneComponent*> ChildComponents;
    GetChildrenComponents(true, ChildComponents);
    for (const auto& ChildComponent : ChildComponents)
    {
        if (ChildComponent->IsA<UMRMeshComponent>())
        {
            ConnectMrMesh(Cast<UMRMeshComponent>(ChildComponent));
            break;
        }
    }
}

void UAndroidXRSceneMeshingComponent::UpdateUnique(const TArray<FAndroidXRSceneSubmeshState> &SubmeshStates,
    const TArray<FAndroidXRSceneSubmeshData> &SubmeshData)
{
    TArray<FGuid> AddedSubmeshIds;
    TArray<FGuid> UpdatedSubmeshIds;
    TArray<FGuid> RemovedSubmeshIds;
    TMap<FGuid, FAndroidXRSceneSubmeshState> NewUniqueMeshStates;
    TMap<FGuid, FAndroidXRSceneSubmeshData> NewUniqueMeshData;

    // Get adds/updates
    for (int32 BrickIndex = 0; BrickIndex < SubmeshStates.Num(); ++BrickIndex)
    {
        const auto& MeshState = SubmeshStates[BrickIndex];

        NewUniqueMeshStates.Add(MeshState.SubmeshId, MeshState);
        NewUniqueMeshData.Add(MeshState.SubmeshId, SubmeshData[BrickIndex]);

        if (UniqueSubmeshStates.Contains(MeshState.SubmeshId))
        {
            if (MeshState.LastUpdatedTimespan > UniqueSubmeshStates[MeshState.SubmeshId].LastUpdatedTimespan)
            {
                UpdatedSubmeshIds.Add(MeshState.SubmeshId);
            }
        }
        else
        {
            AddedSubmeshIds.Add(MeshState.SubmeshId);
        }
    }

    // Get removes
    for (const auto& UniqueMeshStateKvp : UniqueSubmeshStates)
    {
        if (!NewUniqueMeshStates.Contains(UniqueMeshStateKvp.Key))
        {
            RemovedSubmeshIds.Add(UniqueMeshStateKvp.Key);
        }
    }

    // Update unique lists
    UniqueSubmeshStates = MoveTemp(NewUniqueMeshStates);
    UniqueSubmeshData = MoveTemp(NewUniqueMeshData);

    DeltaDelegate.Broadcast(AddedSubmeshIds, UpdatedSubmeshIds, RemovedSubmeshIds);
}

void UAndroidXRSceneMeshingComponent::UpdateMrMesh()
{
    if (IsValid(MrMesh))
    {
        struct FSubmeshPair
        {
            const FAndroidXRSceneSubmeshState* State;
            const FAndroidXRSceneSubmeshData* Data;
        };

        // Generate mapping of IDs to states/data
        TMap<FGuid, FSubmeshPair> BrickableSubmeshPairs;
        for (const auto& SubmeshStateKvp : UniqueSubmeshStates)
        {
            BrickableSubmeshPairs.Add(SubmeshStateKvp.Key,
                {
                    .State = &SubmeshStateKvp.Value,
                    .Data = &UniqueSubmeshData[SubmeshStateKvp.Key]
                });
        }

        // Send empty bricks for any published bricks that are no longer in the query results,
        // remove them from the published bricks map
        TMap<FGuid, FPublishedBrickInfo> NewPublishedBricks;
        for (auto MapIterator = PublishedBricks.begin(); MapIterator != PublishedBricks.end(); ++MapIterator)
        {
            if (BrickableSubmeshPairs.Contains(MapIterator->Key))
            {
                NewPublishedBricks.Add(*MapIterator);
            }
            else
            {
                const static TArray<FVector3f> EmptyVertexes;
                const static TArray<FVector2D> EmptyUVs;
                const static TArray<FPackedNormal> EmptyTangents;
                const static TArray<FColor> EmptyVertexColors;
                const static TArray<MRMESH_INDEX_TYPE> EmptyIndexes;

                static_cast<IMRMesh*>(MrMesh)->SendBrickData(IMRMesh::FSendBrickDataArgs
                    {
                        nullptr,
                        MapIterator->Value.BrickId,
                        EmptyVertexes,
                        EmptyUVs,
                        EmptyTangents,
                        EmptyVertexColors,
                        EmptyIndexes
                    });
            }
        }
        PublishedBricks = MoveTemp(NewPublishedBricks);

        // Remove any pending publishes that are not in the query results and remove
        // any query results that are already in the pending list
        TArray<FGuid> NewSubmeshesPendingPublish;
        for (const auto& SubmeshPendingPublish : SubmeshesPendingPublish)
        {
            if (BrickableSubmeshPairs.Contains(SubmeshPendingPublish))
            {
                BrickableSubmeshPairs.Remove(SubmeshPendingPublish);
                NewSubmeshesPendingPublish.Add(SubmeshPendingPublish);
            }
        }
        SubmeshesPendingPublish = MoveTemp(NewSubmeshesPendingPublish);

        // Add new and updated brickable submeshes to pending list
        for (const auto& BrickableSubmeshPairKvp : BrickableSubmeshPairs)
        {
            // If the submesh has already been published, only republish it if
            // it has been updated
            if (auto PublishedBrick = PublishedBricks.Find(BrickableSubmeshPairKvp.Key))
            {
                if (PublishedBrick->LastUpdatedTimespan >= BrickableSubmeshPairKvp.Value.State->LastUpdatedTimespan)
                {
                    continue;
                }
            }
            SubmeshesPendingPublish.Add(BrickableSubmeshPairKvp.Key);
        }

        // SubmeshesPendingPublish now contains all unique submeshes found this frame that
        // are not already published

        // Get submeshes to brick this frame from the pending list
        TArray<FGuid> SubmeshIdsToBrick;
        if (MaxBricksPerFrame > 0)
        {
            auto NumBricks = FMath::Min(MaxBricksPerFrame, SubmeshesPendingPublish.Num());
            for (int32 BrickIndex = 0; BrickIndex < NumBricks; ++BrickIndex)
            {
                SubmeshIdsToBrick.Add(SubmeshesPendingPublish[BrickIndex]);
            }
            SubmeshesPendingPublish.RemoveAt(0, NumBricks);
        }
        else
        {
            SubmeshIdsToBrick = MoveTemp(SubmeshesPendingPublish);
            SubmeshesPendingPublish.Empty();
        }

        // Publish blocks to bricks
        for (const auto& SubmeshIdToBrick : SubmeshIdsToBrick)
        {
            auto BrickId = NextMeshBrickId;
            if (PublishedBricks.Contains(SubmeshIdToBrick))
            {
                BrickId = PublishedBricks[SubmeshIdToBrick].BrickId;
            }
            else
            {
                ++NextMeshBrickId;
            }

            auto CachedData = MakeShared<FCachedMeshBrickData,
                ESPMode::ThreadSafe>(BrickId,
                    *BrickableSubmeshPairs[SubmeshIdToBrick].State,
                    *BrickableSubmeshPairs[SubmeshIdToBrick].Data);

            PublishedBricks.Add(SubmeshIdToBrick,
                {
                    BrickId,
                    BrickableSubmeshPairs[SubmeshIdToBrick].State->LastUpdatedTimespan,
                    CachedData->BoxBounds,
                    BrickableSubmeshPairs[SubmeshIdToBrick].State->PoseInBaseSpace.GetLocation()
                });

            static_cast<IMRMesh*>(MrMesh)->SendBrickData(IMRMesh::FSendBrickDataArgs
                {
                    MakeShared<FMeshBrickDataReceipt>(CachedData),
                    BrickId,
                    CachedData->VertexPositions,
                    CachedData->VertexUVs,
                    CachedData->VertexTangentXZs,
                    CachedData->VertexColors,
                    CachedData->Indexes,
                    CachedData->BoxBounds
                });
        }
    }
}

void UAndroidXRSceneMeshingComponent::CreateTracker()
{
    if (IAndroidXRSceneMeshingModule::IsAvailable())
    {
        const auto& MeshingModule = IAndroidXRSceneMeshingModule::Get();

        if (MeshingTracker.Tracker != nullptr)
        {
            MeshingModule.DestroySceneMeshingTracker(MeshingTracker);
        }
        MeshingModule.CreateSceneMeshingTracker(SemanticLabelSet, bEnableNormals, MeshingTracker);
    }
}

void UAndroidXRSceneMeshingComponent::DestroyTracker()
{
    if(IAndroidXRSceneMeshingModule::IsAvailable() && MeshingTracker.Tracker)
    {
        IAndroidXRSceneMeshingModule::Get().DestroySceneMeshingTracker(MeshingTracker);
    }
}

void UAndroidXRSceneMeshingComponent::RequestSubmeshes()
{
    if (IAndroidXRSceneMeshingModule::IsAvailable())
    {
        if (MeshingTracker.Tracker != nullptr)
        {
            const auto& MeshingModule = IAndroidXRSceneMeshingModule::Get();

            FAndroidXRSceneMeshSnapshot MeshSnapshot;
            if (MeshingModule.CreateSceneMeshSnapshot(MeshingTracker,
                SearchVolume->GetComponentTransform(),
                SearchVolume->GetScaledBoxExtent(),
                MeshSnapshot,
                TrackingState))
            {
                TArray<FAndroidXRSceneSubmeshState> SubmeshStates;
                if (MeshingModule.GetAllSubmeshStates(MeshSnapshot, SubmeshStates))
                {
                    TArray<FGuid> SubmeshIds;
                    SubmeshIds.Reserve(SubmeshStates.Num());
                    for (const auto& SubmeshState : SubmeshStates)
                    {
                        SubmeshIds.Add(SubmeshState.SubmeshId);
                    }

                    TArray<FAndroidXRSceneSubmeshData> SubmeshData;
                    if (MeshingModule.GetSubmeshData(MeshSnapshot, SubmeshIds, SubmeshData))
                    {
                        UpdateUnique(SubmeshStates, SubmeshData);
                        UpdateMrMesh();
                    }
                }

                MeshingModule.DestroySceneMeshSnapshot(MeshSnapshot);
            }
        }
    }
}

void UAndroidXRSceneMeshingComponent::ClearUniqueSubmeshes()
{
    UniqueSubmeshStates.Empty();
    UniqueSubmeshData.Empty();
}

void UAndroidXRSceneMeshingComponent::ConnectMrMesh(UMRMeshComponent *InMrMesh)
{
    if (IsValid(MrMesh))
    {
        MrMesh->SetConnected(false);
    }

    MrMesh = InMrMesh;

    if (IsValid(MrMesh))
    {
        MrMesh->SetConnected(true);
    }
}

void UAndroidXRSceneMeshingComponent::ClearPublishedMeshHistory()
{
    PublishedBricks.Empty();
    SubmeshesPendingPublish.Empty();
    NextMeshBrickId = 0;
}

void UAndroidXRSceneMeshingComponent::DrawPublishedBrickBounds()
{
    for (const auto& PublishedBrickKvp : PublishedBricks)
    {
        DrawDebugBox(GetWorld(), PublishedBrickKvp.Value.Location,
            PublishedBrickKvp.Value.Bounds.GetExtent(), FColor::Yellow);
    }
}
