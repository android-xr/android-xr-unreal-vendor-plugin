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
#include "AndroidXRSpatialAnchorTypes.h"
namespace AndroidXR
{
    inline ESpatialAnchorPersistState Convert(XrSpatialPersistenceStateEXT NativeType)
    {
        switch(NativeType)
        {
            case XrSpatialPersistenceStateEXT::XR_SPATIAL_PERSISTENCE_STATE_LOADED_EXT:
            {
                return ESpatialAnchorPersistState::Loaded;
            }
            case XrSpatialPersistenceStateEXT::XR_SPATIAL_PERSISTENCE_STATE_NOT_FOUND_EXT:
            {
                return ESpatialAnchorPersistState::NotFound;
            }
            default:
                break;
        }
        return ESpatialAnchorPersistState::NotFound;
    }

    inline ESpatialAnchorPeristenceResult Convert(XrSpatialPersistenceContextResultEXT NativeType)
    {
        switch(NativeType)
        {
            case XR_SPATIAL_PERSISTENCE_CONTEXT_RESULT_SUCCESS_EXT:
                return ESpatialAnchorPeristenceResult::Success;
            case XR_SPATIAL_PERSISTENCE_CONTEXT_RESULT_ENTITY_NOT_TRACKING_EXT:
                return ESpatialAnchorPeristenceResult::EntityNotTracking;
            case XR_SPATIAL_PERSISTENCE_CONTEXT_RESULT_PERSIST_UUID_NOT_FOUND_EXT:
                return ESpatialAnchorPeristenceResult::PersistedIdNotFound;
            default:
                break;
        }
        return ESpatialAnchorPeristenceResult::TimedOut;
    }
}