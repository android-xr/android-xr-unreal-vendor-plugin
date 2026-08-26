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
#include "AndroidXRSpatialPlaneTrackingTypes.h"
#include "AndroidXRSpatialPlaneEntity.h"
#include "IAndroidXRSpatialPlaneTrackingModule.h"
#include "AndroidXRSpatialEntitiesModuleBase.h"

class ANDROIDXRSPATIALPLANETRACKING_API FAndroidXRSpatialPlaneTracking : public IAndroidXRSpatialPlaneTrackingModule
{
protected:
    void GetCapabilityExtensions(TArray<const ANSICHAR*>& Extensions) override;
    void UpdateEnabledComponents() override;
    void GetCapabilityConfigurations(TArray<XrSpatialCapabilityConfigurationBaseHeaderEXT*>& Headers) override;
    FSpatialComponentList* GetSpatialComponentList(XrSpatialComponentTypeEXT ComponentType) override;

    void ApplyToEntity(TObjectPtr<UAndroidXRSpatialPlaneEntity> Entity, const FSpatialEntityInfo& EntityInfo) override;
private:
    XrSpatialCapabilityConfigurationPlaneTrackingEXT PlaneConfiguration{ XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_PLANE_TRACKING_EXT, nullptr, XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_PLANE_TRACKING_EXT };

    TFSpatialComponentList<XrSpatialComponentPolygon2DListEXT, XrSpatialPolygon2DDataEXT> PolygonList{ XR_TYPE_SPATIAL_COMPONENT_POLYGON_2D_LIST_EXT };
    TFSpatialComponentList<XrSpatialComponentMesh2DListEXT, XrSpatialMeshDataEXT> MeshList{ XR_TYPE_SPATIAL_COMPONENT_MESH_2D_LIST_EXT };
    TFSpatialComponentList<XrSpatialComponentPlaneAlignmentListEXT, XrSpatialPlaneAlignmentEXT> AlignmentList{ XR_TYPE_SPATIAL_COMPONENT_PLANE_ALIGNMENT_LIST_EXT };
    TFSpatialComponentList<XrSpatialComponentPlaneSemanticLabelListEXT, XrSpatialPlaneSemanticLabelEXT> LabelsList{ XR_TYPE_SPATIAL_COMPONENT_PLANE_SEMANTIC_LABEL_LIST_EXT };
};