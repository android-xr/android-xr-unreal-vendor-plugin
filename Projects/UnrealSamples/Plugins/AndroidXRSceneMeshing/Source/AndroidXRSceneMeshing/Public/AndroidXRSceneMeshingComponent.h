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

#include "Components/SceneComponent.h"
#include "AndroidXRTypes.h"
#include "MRMeshComponent.h"
#include "AndroidXRSceneMeshingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAndroidXRWorldMeshDeltaDelegateMulticast,
    const TArray<FGuid>&, AddedMeshBlockIds,
    const TArray<FGuid>&, UpdatedMeshBlockIds,
    const TArray<FGuid>&, RemovedMeshBlockIds);

class UBoxComponent;

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class ANDROIDXRSCENEMESHING_API UAndroidXRSceneMeshingComponent : public USceneComponent
{
    GENERATED_BODY()
protected:
    // Raw results delegate, receives all mesh blocks found with the current parameters
    UPROPERTY(BlueprintAssignable)
    FAndroidXRWorldMeshDeltaDelegateMulticast ResultDelegate;

    // Difference between previous and current detected mesh blocks
    UPROPERTY(BlueprintAssignable)
    FAndroidXRWorldMeshDeltaDelegateMulticast DeltaDelegate;

    // Bounding box for query
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
    TObjectPtr<UBoxComponent> SearchVolume;

    // Semantic label for query
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
    EAndroidXRSceneMeshSemanticLabelSet SemanticLabelSet{ EAndroidXRSceneMeshSemanticLabelSet::Default };

    // Whether or not query should include normals
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
    bool bEnableNormals{ true };

    // If >0, limits the number of MR Mesh blocks generated per frame
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
    int32 MaxBricksPerFrame{ };

    // Unique submesh states
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TMap<FGuid, FAndroidXRSceneSubmeshState> UniqueSubmeshStates;

    // Unique submesh data
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TMap<FGuid, FAndroidXRSceneSubmeshData> UniqueSubmeshData;

    // Current tracker
    UPROPERTY(BlueprintReadOnly)
    FAndroidXRSceneMeshingTracker MeshingTracker{ };

    // Current tracking state (refreshed whenever a snapshot is taken)
    UPROPERTY(BlueprintReadOnly)
    ESceneMeshTrackingState TrackingState{ };

    // If load-balancing is enabled (using MaxBlocksPerFrame) this contains the
    // IDs of submeshes pending submission to the attached MR Mesh component
    TArray<FGuid> SubmeshesPendingPublish;

    // Maps submesh GUIDs to published MR Mesh bricks
    struct FPublishedBrickInfo
    {
        IMRMesh::FBrickId BrickId;
        FTimespan LastUpdatedTimespan;
        FBox Bounds;
        FVector Location;
    };
    TMap<FGuid, FPublishedBrickInfo> PublishedBricks;

    // Child MR Mesh component
    TObjectPtr<UMRMeshComponent> MrMesh;

    // To uniquely tag MR Mesh bricks
    IMRMesh::FBrickId NextMeshBrickId{ };

    void BeginPlay() override;
    void UpdateUnique(const TArray<FAndroidXRSceneSubmeshState> &SubmeshStates,
        const TArray<FAndroidXRSceneSubmeshData>& SubmeshData);
    void UpdateMrMesh();
public:
    UAndroidXRSceneMeshingComponent();

    // Creates a mesh tracker (destroys any existing one first)
    UFUNCTION(BlueprintCallable)
    void CreateTracker();

    // Destroys a mesh tracker
    UFUNCTION(BlueprintCallable)
    void DestroyTracker();

    // Performas a mesh query and broadcasts delegates
    UFUNCTION(BlueprintCallable)
    void RequestSubmeshes();

    // Clears all recorded unique submeshes
    UFUNCTION(BlueprintCallable)
    void ClearUniqueSubmeshes();

    // Connects a MR Mesh component to this component for visualization
    UFUNCTION(BlueprintCallable)
    void ConnectMrMesh(UMRMeshComponent* InMrMesh);

    // Removes all history of published bricks
    UFUNCTION(BlueprintCallable)
    void ClearPublishedMeshHistory();

    // Draws bounding boxes of published bricks (does not work in Shipping)
    UFUNCTION(BlueprintCallable)
    void DrawPublishedBrickBounds();
};
