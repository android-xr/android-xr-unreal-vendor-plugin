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
#include "AndroidXRTypes.h"
#include "AndroidXRAnchorPersistenceTypes.generated.h"

/**
 * Contains FAndroidXRSpace associated with an anchor, along with
 * an FGuid that uniquely identifies the anchor (if persisted).
 */
USTRUCT(BlueprintType)
struct FAndroidXRAnchor
{
    GENERATED_BODY()

    // The space that represents this anchor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Anchor")
    FAndroidXRSpace Space{};

    // If the anchor is persisted this will be a valid UUID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Anchor")
    FGuid PersistedID{};

    // Last located transform of the anchor's space
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Anchor")
    FTransform Transform{};
};

/**
 * Contains opaque pointer to XrDeviceAnchorPersistenceANDROID.
 */
USTRUCT(BlueprintType)
struct FAndroidXRAnchorPersistence
{
    GENERATED_BODY()

    XrDeviceAnchorPersistenceANDROID Persistence{};
};

/**
 * Anchor persist state
 */
UENUM(BlueprintType)
enum class EAndroidXRAnchorPersistState : uint8
{
    // Anchor has not been requested to be persisted by the app.
    PersistNotRequested,
    // Anchor has been requested to be persisted but not persisted yet.
    PersistPending,
    // Anchor has been successfully persisted by the runtime.
    Persisted,
};

DECLARE_DELEGATE_TwoParams(FOnAnchorPersisted, const FAndroidXRSpace& /*Persisted Anchor Space*/, const FGuid& /*Persisted Anchor's Guid*/);
DECLARE_DELEGATE_TwoParams(FOnPersistedAnchorSpaceCreated, const FAndroidXRSpace& /*Created anchor space*/, const FGuid& /*Created Persisted Anchor's Guid*/);
DECLARE_DELEGATE(FOnAnchorUnPersisted);
DECLARE_DELEGATE(FOnAnchorAsyncActionFailed);
