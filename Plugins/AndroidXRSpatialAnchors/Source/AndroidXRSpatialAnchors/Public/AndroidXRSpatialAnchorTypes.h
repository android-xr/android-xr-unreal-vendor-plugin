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
#include "AndroidXRTypes.h"

/**
* The persistence state of an anchor
*/
UENUM(BlueprintType)
enum class ESpatialAnchorPersistState : uint8
{
    //The anchor was loaded successfully
    Loaded,
    //The anchor could not be found
    NotFound
};

/**
* The spatial anchor persistence operation result
*/
UENUM(BlueprintType)
enum class ESpatialAnchorPeristenceResult : uint8
{
    //The operation was successful
    Success,
    //The persisted id was not found
    PersistedIdNotFound,
    //The underlying entity was not being tracked
    EntityNotTracking,
    //The persist/unpersist request timed out
    TimedOut,
    //Persistence operations are not supported
    PersistenceUnsupported,
    //The persistence context is invalid
    InvalidPersistenceContext,
    //The spatial context is invalid
    InvalidSpatialContext,
    //The entity cannot execute the persistence operation
    InvalidEntity
};