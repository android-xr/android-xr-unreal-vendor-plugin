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

#include "AndroidXRSpatialPlaneTracking.h"
#include "AndroidXRSpatialPlaneTrackingConversions.h"
#include <IXRTrackingSystem.h>
#include <IHeadMountedDisplay.h>
#include "IOpenXRHMD.h"

void AddComponentIfSupported(FNativeSpatialCapabilitiesInfo& CapabilitiesInfo, XrSpatialComponentTypeEXT ComponentType, TArray<XrSpatialComponentTypeEXT>& OutComponents)
{
    if(CapabilitiesInfo.IsComponentSupported(XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_PLANE_TRACKING_EXT, ComponentType))
    {
        OutComponents.Add(ComponentType);
    }
}

void FAndroidXRSpatialPlaneTracking::GetCapabilityExtensions(TArray<const ANSICHAR*>& Extensions)
{
    Extensions.Add(XR_EXT_SPATIAL_PLANE_TRACKING_EXTENSION_NAME);
}

void FAndroidXRSpatialPlaneTracking::UpdateEnabledComponents()
{
    AddComponentIfSupported(CapabilitiesInfo, XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_BOUNDED_2D_EXT, EnabledComponents);
    AddComponentIfSupported(CapabilitiesInfo, XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_PLANE_ALIGNMENT_EXT, EnabledComponents);
    AddComponentIfSupported(CapabilitiesInfo, XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_POLYGON_2D_EXT, EnabledComponents);
    AddComponentIfSupported(CapabilitiesInfo, XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_MESH_2D_EXT, EnabledComponents);
    AddComponentIfSupported(CapabilitiesInfo, XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_PLANE_SEMANTIC_LABEL_EXT, EnabledComponents);
}

void FAndroidXRSpatialPlaneTracking::GetCapabilityConfigurations(TArray<XrSpatialCapabilityConfigurationBaseHeaderEXT*>& Headers)
{
    PlaneConfiguration.enabledComponentCount = static_cast<uint32_t>(EnabledComponents.Num());
    PlaneConfiguration.enabledComponents = EnabledComponents.GetData();
    Headers.Add(reinterpret_cast<XrSpatialCapabilityConfigurationBaseHeaderEXT*>(&PlaneConfiguration));
}

FSpatialComponentList* FAndroidXRSpatialPlaneTracking::GetSpatialComponentList(XrSpatialComponentTypeEXT ComponentType)
{
    switch(ComponentType)
    {
        case XR_SPATIAL_COMPONENT_TYPE_PLANE_ALIGNMENT_EXT:
            return &AlignmentList;
        case XR_SPATIAL_COMPONENT_TYPE_MESH_2D_EXT:
            return &MeshList;
        case XR_SPATIAL_COMPONENT_TYPE_POLYGON_2D_EXT:
            return &PolygonList;
        case XR_SPATIAL_COMPONENT_TYPE_PLANE_SEMANTIC_LABEL_EXT:
            return &LabelsList;
            break;
    }
    return nullptr;
}


void FAndroidXRSpatialPlaneTracking::ApplyToEntity(TObjectPtr<UAndroidXRSpatialPlaneEntity> Entity, const FSpatialEntityInfo& EntityInfo)
{
    auto WorldToMetersScale = GEngine->XRSystem->GetWorldToMetersScale();
    //Check if bounds exists:
    XrSpatialPlaneAlignmentEXT PlaneAlignment;
    if(AlignmentList.Get(EntityInfo.ComponentIndex, PlaneAlignment))
    {
        Entity->Alignment = AndroidXR::Convert(PlaneAlignment);
    }
    XrSpatialPlaneSemanticLabelEXT PlaneLabel{};
    if(LabelsList.Get(EntityInfo.ComponentIndex, PlaneLabel))
    {
        Entity->SemanticLabel = AndroidXR::Convert(PlaneLabel);
    }
    XrSpatialBounded2DDataEXT Bounds{};
    if(EntityInfo.Bounds && EntityInfo.Bounds->Get(EntityInfo.ComponentIndex, Bounds))
    {
        Entity->PlaneTransform = ToFTransform(Bounds.center, WorldToMetersScale);
        Entity->PlaneExtents = ToFVector2D(Bounds.extents, WorldToMetersScale);
    }
    XrSpatialMeshDataEXT Mesh2D{};
    if(MeshList.Get(EntityInfo.ComponentIndex, Mesh2D))
    {
        TArray<XrVector3f> NativeVertices{};
        TArray<uint32_t> NativeIndices{};

        FAndroidXRSpatialBuffer VertexBuffer{ Mesh2D.vertexBuffer };
        FAndroidXRSpatialBuffer IndexBuffer{ Mesh2D.indexBuffer };

        if(GetSpatialBuffer(EntityInfo.Snapshot, VertexBuffer) && GetSpatialBuffer(EntityInfo.Snapshot, IndexBuffer))
        {
            auto& PlaneMesh = Entity->PlaneMesh;
            PlaneMesh.bIsValid = true;
            VertexBuffer.FillData(NativeVertices);
            IndexBuffer.FillData(NativeIndices);

            PlaneMesh.MeshOrigin = ToFTransform(Mesh2D.origin, WorldToMetersScale);
            PlaneMesh.Vertices.Reset();
            PlaneMesh.Indices.Reset();
            Algo::Transform(NativeVertices, PlaneMesh.Vertices, [WorldToMetersScale](const XrVector3f Input)
            {
                return ToFVector(Input, WorldToMetersScale);
            });
            Algo::Transform(NativeIndices, PlaneMesh.Indices, [](const uint32_t Input)
            {
                return static_cast<int>(Input);
            });
        }
    }

    XrSpatialPolygon2DDataEXT PlanePolygonData{};
    if(PolygonList.Get(EntityInfo.ComponentIndex, PlanePolygonData))
    {
        FAndroidXRSpatialBuffer PolygonBuffer{ PlanePolygonData.vertexBuffer };
        TArray<XrVector2f> NativePolygonVertices{};

        auto& PlanePolygon = Entity->PlanePolygon;
        if(GetSpatialBuffer(EntityInfo.Snapshot, PolygonBuffer))
        {
            PlanePolygon.bIsValid = true;
            PlanePolygon.PolygonOrigin = ToFTransform(PlanePolygonData.origin, WorldToMetersScale);
            PolygonBuffer.FillData(NativePolygonVertices);

            PlanePolygon.Vertices.Reset();

            Algo::Transform(NativePolygonVertices, PlanePolygon.Vertices, [&PlanePolygon, WorldToMetersScale](const XrVector2f& NativeVertex)
            {
                return PlanePolygon.PolygonOrigin.TransformPositionNoScale(ToFVector(XrVector3f{ NativeVertex.x, NativeVertex.y, 0 }, WorldToMetersScale));
            });
        }
    }
}


IMPLEMENT_MODULE(FAndroidXRSpatialPlaneTracking, AndroidXRSpatialPlaneTracking)