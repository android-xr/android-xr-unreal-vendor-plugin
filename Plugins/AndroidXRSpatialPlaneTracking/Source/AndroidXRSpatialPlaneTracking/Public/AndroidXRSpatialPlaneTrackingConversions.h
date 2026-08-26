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

namespace AndroidXR
{
    inline ESpatialPlaneAlignment Convert(XrSpatialPlaneAlignmentEXT NativeType)
    {
        switch(NativeType)
        {
            case XR_SPATIAL_PLANE_ALIGNMENT_HORIZONTAL_UPWARD_EXT:
                return ESpatialPlaneAlignment::HorizontalUpward;
            case XR_SPATIAL_PLANE_ALIGNMENT_HORIZONTAL_DOWNWARD_EXT:
                return ESpatialPlaneAlignment::HorizontalDownward;
            case XR_SPATIAL_PLANE_ALIGNMENT_VERTICAL_EXT:
                return ESpatialPlaneAlignment::Vertical;
            case XR_SPATIAL_PLANE_ALIGNMENT_ARBITRARY_EXT:
                return ESpatialPlaneAlignment::Arbitrary;
            default:
                break;
        }
        return ESpatialPlaneAlignment::Arbitrary;
    }

    inline ESpatialPlaneSemanticLabel Convert(XrSpatialPlaneSemanticLabelEXT NativeType)
    {
        switch(NativeType)
        {
            case XR_SPATIAL_PLANE_SEMANTIC_LABEL_UNCATEGORIZED_EXT:
                return ESpatialPlaneSemanticLabel::Uncategorized;
            case XR_SPATIAL_PLANE_SEMANTIC_LABEL_FLOOR_EXT:
                return ESpatialPlaneSemanticLabel::Floor;
            case XR_SPATIAL_PLANE_SEMANTIC_LABEL_WALL_EXT:
                return ESpatialPlaneSemanticLabel::Wall;
            case XR_SPATIAL_PLANE_SEMANTIC_LABEL_CEILING_EXT:
                return ESpatialPlaneSemanticLabel::Ceiling;
            case XR_SPATIAL_PLANE_SEMANTIC_LABEL_TABLE_EXT:
                return ESpatialPlaneSemanticLabel::Table;
            default:
                break;
        }
        return ESpatialPlaneSemanticLabel::Uncategorized;
    }
}
