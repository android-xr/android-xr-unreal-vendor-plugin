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

#include "AndroidXRSceneMeshing.h"
#include "AndroidXRTypeConversions.h"
#include <IOpenXRHMDModule.h>
#include "IOpenXRHMD.h"
#include "IXRTrackingSystem.h"

DEFINE_LOG_CATEGORY(LogAndroidXRSceneMeshing);

FAndroidXRSceneMeshing::FAndroidXRSceneMeshing()
{
}

void FAndroidXRSceneMeshing::StartupModule()
{
    IAndroidXRSceneMeshingModule::StartupModule();
    RegisterOpenXRExtensionModularFeature();
}

void FAndroidXRSceneMeshing::ShutdownModule()
{
    UnregisterOpenXRExtensionModularFeature();
    IAndroidXRSceneMeshingModule::ShutdownModule();
}

void FAndroidXRSceneMeshing::PostCreateInstance(XrInstance InInstance)
{
    Instance = InInstance;

    ResolveOpenXRFunctions(Instance,
        {
            ANDROIDXRSCENEMESHING_FUNCTIONS(RESOLVE_OPENXR_FUNC)
        },
        [](const char* UnresolvableFunctionName, XrResult Result)
        {
            UE_LOG(LogAndroidXRSceneMeshing, Error,
                TEXT("Unable to resolve function pointer %s (error %s)"),
                UTF8_TO_TCHAR(UnresolvableFunctionName),
                OpenXRResultToString(Result));
        });
}

void FAndroidXRSceneMeshing::PostCreateSession(XrSession InSession)
{
    Session = InSession;
}

bool FAndroidXRSceneMeshing::GetRequiredExtensions(TArray<const ANSICHAR *> &OutExtensions)
{
    OutExtensions.Add(XR_ANDROID_SCENE_MESHING_EXTENSION_NAME);
    return true;
}

bool FAndroidXRSceneMeshing::EnumerateSupportedSemanticLabelSets(TArray<EAndroidXRSceneMeshSemanticLabelSet> &SupportedSets) const
{
    if (xrEnumerateSupportedSemanticLabelSetsANDROID != nullptr)
    {
        uint32_t NumSemanticLabelSets;
        auto Result = xrEnumerateSupportedSemanticLabelSetsANDROID(Instance,
            IOpenXRHMDModule::Get().GetSystemId(),
            0,
            &NumSemanticLabelSets,
            nullptr);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            TArray<XrSceneMeshSemanticLabelSetANDROID> NativeSupportedSets;
            NativeSupportedSets.SetNum(NumSemanticLabelSets);
            Result = xrEnumerateSupportedSemanticLabelSetsANDROID(Instance,
                IOpenXRHMDModule::Get().GetSystemId(),
                NativeSupportedSets.Num(),
                &NumSemanticLabelSets,
                NativeSupportedSets.GetData());
            if (XR_UNQUALIFIED_SUCCESS(Result))
            {
                for (const auto& NativeSupportedSet : NativeSupportedSets)
                {
                    SupportedSets.Add(AndroidXR::Convert(NativeSupportedSet));
                }
                return true;
            }
        }

        UE_LOG(LogAndroidXRSceneMeshing, Error,
            TEXT("xrEnumerateSupportedSemanticLabelSetsANDROID failed with error %s"),
            OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXRSceneMeshing::CreateSceneMeshingTracker(EAndroidXRSceneMeshSemanticLabelSet SemanticLabelSet,
    bool bEnableNormals,
    FAndroidXRSceneMeshingTracker& MeshingTracker) const
{
    if (xrCreateSceneMeshingTrackerANDROID != nullptr)
    {
        XrSceneMeshingTrackerCreateInfoANDROID CreateInfo
        {
            .type = XR_TYPE_SCENE_MESHING_TRACKER_CREATE_INFO_ANDROID,
            .semanticLabelSet = AndroidXR::Convert(SemanticLabelSet),
            .enableNormals = bEnableNormals
        };

        XrSceneMeshingTrackerANDROID NativeTracker;
        auto Result = xrCreateSceneMeshingTrackerANDROID(Session,
            &CreateInfo, &NativeTracker);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            MeshingTracker.bHasNormals = bEnableNormals;
            MeshingTracker.Tracker = NativeTracker;
            return true;
        }

        UE_LOG(LogAndroidXRSceneMeshing, Error,
            TEXT("xrCreateSceneMeshingTrackerANDROID failed with error %s"),
            OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXRSceneMeshing::DestroySceneMeshingTracker(FAndroidXRSceneMeshingTracker& MeshingTracker) const
{
    if (xrDestroySceneMeshingTrackerANDROID != nullptr)
    {
        auto Result = xrDestroySceneMeshingTrackerANDROID(MeshingTracker.Tracker);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            MeshingTracker.bHasNormals = false;
            MeshingTracker.Tracker = nullptr;
            return true;
        }

        UE_LOG(LogAndroidXRSceneMeshing, Error,
            TEXT("xrDestroySceneMeshingTrackerANDROID failed with error %s"),
            OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXRSceneMeshing::CreateSceneMeshSnapshot(const FAndroidXRSceneMeshingTracker& MeshingTracker,
    const FTransform& BoundsTransform,
    const FVector& BoundsExtents,
    FAndroidXRSceneMeshSnapshot& MeshSnapshot,
    ESceneMeshTrackingState& TrackingState) const
{
    if (xrCreateSceneMeshSnapshotANDROID != nullptr)
    {
        if (auto HMD = GEngine->XRSystem.Get()->GetIOpenXRHMD())
        {
            auto WorldToMetersScale = GEngine->XRSystem->GetWorldToMetersScale();
            XrSceneMeshSnapshotCreateInfoANDROID CreateInfo
            {
                .type = XR_TYPE_SCENE_MESH_SNAPSHOT_CREATE_INFO_ANDROID,
                .baseSpace = HMD->GetTrackingSpace(),
                .time = HMD->GetDisplayTime(),
                .boundingBox =
                {
                    .center = ToXrPose(BoundsTransform, WorldToMetersScale),
                    .extents = ToXrExtent3Df(BoundsExtents, WorldToMetersScale)
                }
            };

            // Bounding box is not yet implemented in OS: b/421446800

            XrSceneMeshSnapshotCreationResultANDROID NativeCreationResult
            {
                .type = XR_TYPE_SCENE_MESH_SNAPSHOT_CREATION_RESULT_ANDROID
            };

            auto Result = xrCreateSceneMeshSnapshotANDROID(MeshingTracker.Tracker,
                &CreateInfo,
                &NativeCreationResult);

            if (XR_UNQUALIFIED_SUCCESS(Result))
            {
                MeshSnapshot.bHasNormals = MeshingTracker.bHasNormals;
                MeshSnapshot.Snapshot = NativeCreationResult.snapshot;
                TrackingState =
                    AndroidXR::Convert(NativeCreationResult.trackingState);
                return true;
            }

            UE_LOG(LogAndroidXRSceneMeshing, Error,
                TEXT("xrCreateSceneMeshSnapshotANDROID failed with error %s"),
                OpenXRResultToString(Result));
        }
    }
    return false;
}

bool FAndroidXRSceneMeshing::DestroySceneMeshSnapshot(FAndroidXRSceneMeshSnapshot& MeshSnapshot) const
{
    if (xrDestroySceneMeshSnapshotANDROID != nullptr)
    {
        auto Result = xrDestroySceneMeshSnapshotANDROID(MeshSnapshot.Snapshot);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            MeshSnapshot.bHasNormals = false;
            MeshSnapshot.Snapshot = nullptr;
            return true;
        }

        UE_LOG(LogAndroidXRSceneMeshing, Error,
            TEXT("xrDestroySceneMeshSnapshotANDROID failed with error %s"),
            OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXRSceneMeshing::GetAllSubmeshStates(const FAndroidXRSceneMeshSnapshot& MeshSnapshot,
    TArray<FAndroidXRSceneSubmeshState>& SubmeshStates) const
{
    if (xrGetAllSubmeshStatesANDROID != nullptr)
    {
        uint32 NumSubmeshes;
        auto Result = xrGetAllSubmeshStatesANDROID(MeshSnapshot.Snapshot,
            0,
            &NumSubmeshes,
            nullptr);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            TArray<XrSceneSubmeshStateANDROID> NativeSubmeshes;
            NativeSubmeshes.Init({ .type = XR_TYPE_SCENE_SUBMESH_STATE_ANDROID }, NumSubmeshes);
            Result = xrGetAllSubmeshStatesANDROID(MeshSnapshot.Snapshot,
                NativeSubmeshes.Num(),
                &NumSubmeshes,
                NativeSubmeshes.GetData());
            if (XR_UNQUALIFIED_SUCCESS(Result))
            {
                SubmeshStates.Reserve(NativeSubmeshes.Num());

                auto WorldToMeters = GEngine->XRSystem->GetWorldToMetersScale();
                for (const auto& NativeSubmesh : NativeSubmeshes)
                {
                    SubmeshStates.Add(
                        {
                            .SubmeshId = ToFGuid(NativeSubmesh.submeshId),
                            .LastUpdatedTimespan = ToFTimespan(NativeSubmesh.lastUpdatedTime),
                            .PoseInBaseSpace = ToFTransform(NativeSubmesh.
                                submeshPoseInBaseSpace, WorldToMeters),
                            .Extents = ToFVector(NativeSubmesh.bounds, WorldToMeters)
                        }
                    );
                }
                return true;
            }
        }

        UE_LOG(LogAndroidXRSceneMeshing, Error,
            TEXT("xrGetAllSubmeshStatesANDROID failed with error %s"),
            OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXRSceneMeshing::GetSubmeshData(const FAndroidXRSceneMeshSnapshot& MeshSnapshot,
    const TArray<FGuid>& SubmeshIds,
    TArray<FAndroidXRSceneSubmeshData>& SubmeshData) const
{
    if (xrGetSubmeshDataANDROID != nullptr)
    {
        TArray<XrSceneSubmeshDataANDROID> NativeSubmeshData;
        NativeSubmeshData.Reserve(SubmeshIds.Num());

        for (const auto& SubmeshId : SubmeshIds)
        {
            NativeSubmeshData.Add(
                {
                    .type = XR_TYPE_SCENE_SUBMESH_DATA_ANDROID,
                    .submeshId = ToXrUuid(SubmeshId),
                }
            );
        }

        // Get buffer sizes for all the submeshes
        auto Result = xrGetSubmeshDataANDROID(MeshSnapshot.Snapshot,
            NativeSubmeshData.Num(),
            NativeSubmeshData.GetData());
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            // Count vertexes and indexes
            uint32 BufferSizeV = 0;
            uint32 BufferSizeI = 0;
            for (auto& NativeSubmeshDatum : NativeSubmeshData)
            {
                BufferSizeV += NativeSubmeshDatum.vertexCountOutput;
                BufferSizeI += NativeSubmeshDatum.indexCountOutput;
            }

            // Allocate vertex/index storage
            TArray<XrVector3f> NativeVertexPositions;
            NativeVertexPositions.SetNum(BufferSizeV);
            TArray<uint8_t> NativeVertexSemantics;
            NativeVertexSemantics.SetNum(BufferSizeV);
            TArray<uint32_t> NativeIndexes;
            NativeIndexes.SetNum(BufferSizeI);

            uint32 BufferOffsetV = 0;
            uint32 BufferOffsetI = 0;

            // Set counts and pointers for vertexes/semantics/indexes
            for (auto& NativeSubmeshDatum : NativeSubmeshData)
            {
                NativeSubmeshDatum.vertexPositions =
                    NativeVertexPositions.GetData() + BufferOffsetV;
                NativeSubmeshDatum.vertexSemantics =
                    NativeVertexSemantics.GetData() + BufferOffsetV;

                BufferOffsetV += NativeSubmeshDatum.vertexCountOutput;

                NativeSubmeshDatum.vertexCapacityInput = NativeSubmeshDatum.vertexCountOutput;

                NativeSubmeshDatum.indices = NativeIndexes.GetData() + BufferOffsetI;

                BufferOffsetI += NativeSubmeshDatum.indexCountOutput;

                NativeSubmeshDatum.indexCapacityInput = NativeSubmeshDatum.indexCountOutput;
            }

            // Set count and pointers for normals
            TArray<XrVector3f> NativeVertexNormals;
            if (MeshSnapshot.bHasNormals)
            {
                NativeVertexNormals.SetNum(BufferSizeV);

                BufferOffsetV = 0;
                for (auto& NativeSubmeshDatum : NativeSubmeshData)
                {
                    NativeSubmeshDatum.vertexNormals =
                        NativeVertexNormals.GetData() + BufferOffsetV;
                    BufferOffsetV += NativeSubmeshDatum.vertexCountOutput;
                }
            }

            // Get submesh data
            Result = xrGetSubmeshDataANDROID(MeshSnapshot.Snapshot,
                NativeSubmeshData.Num(),
                NativeSubmeshData.GetData());
            if (XR_UNQUALIFIED_SUCCESS(Result))
            {
                SubmeshData.Reserve(NativeSubmeshData.Num());

                // Convert submesh data
                auto WorldToMeters = GEngine->XRSystem->GetWorldToMetersScale();
                for (const auto& NativeSubmeshDatum : NativeSubmeshData)
                {
                    FAndroidXRSceneSubmeshData SubmeshDatum;

                    // ID
                    SubmeshDatum.SubmeshId = ToFGuid(NativeSubmeshDatum.submeshId);

                    // Positions/semantics
                    SubmeshDatum.VertexPositions.Reserve(NativeSubmeshDatum.vertexCapacityInput);
                    SubmeshDatum.VertexSemantics.Reserve(NativeSubmeshDatum.vertexCapacityInput);
                    for (BufferOffsetV = 0; BufferOffsetV < NativeSubmeshDatum.vertexCapacityInput; ++BufferOffsetV)
                    {
                        SubmeshDatum.VertexPositions.Add(ToFVector(NativeSubmeshDatum.
                            vertexPositions[BufferOffsetV], WorldToMeters));
                        SubmeshDatum.VertexSemantics.Add(NativeSubmeshDatum.vertexSemantics[BufferOffsetV]);
                    }

                    // Normals
                    if (MeshSnapshot.bHasNormals)
                    {
                        SubmeshDatum.VertexNormals.Reserve(NativeSubmeshDatum.vertexCapacityInput);
                        for (BufferOffsetV = 0; BufferOffsetV < NativeSubmeshDatum.vertexCapacityInput; ++BufferOffsetV)
                        {
                            SubmeshDatum.VertexNormals.Add(ToFVector(NativeSubmeshDatum.vertexNormals[BufferOffsetV]));
                        }
                    }

                    // Indexes
                    SubmeshDatum.Indexes.Reserve(NativeSubmeshDatum.indexCapacityInput);
                    for (BufferOffsetI = 0; BufferOffsetI < NativeSubmeshDatum.indexCapacityInput; ++BufferOffsetI)
                    {
                        SubmeshDatum.Indexes.Add(NativeSubmeshDatum.indices[BufferOffsetI]);
                    }

                    SubmeshData.Add(SubmeshDatum);
                }

                return true;
            }
        }

        UE_LOG(LogAndroidXRSceneMeshing, Error,
            TEXT("xrGetSubmeshDataANDROID failed with error %s"),
            OpenXRResultToString(Result));
    }
    return false;
}

IMPLEMENT_MODULE(FAndroidXRSceneMeshing, AndroidXRSceneMeshing);

