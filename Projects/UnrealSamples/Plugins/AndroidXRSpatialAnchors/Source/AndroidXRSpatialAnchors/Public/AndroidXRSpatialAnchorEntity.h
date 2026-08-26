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
#include "AndroidXRSpatialEntity.h"
#include "AndroidXRSpatialAnchorTypes.h"
#include "AndroidXRSpatialAnchorEntity.generated.h"

/**
* A tracked Anchor entity
*/
UCLASS(BlueprintType)
class ANDROIDXRSPATIALANCHORS_API UAndroidXRSpatialAnchorEntity : public UAndroidXRSpatialEntity
{
    GENERATED_BODY()
public:

    /**
    * The anchor's transform
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Anchors")
    FTransform AnchorTransform{};

    /**
    * The Id of the anchor
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Anchors")
    FGuid AnchorId{};

    /**
    * Whether the anchor is persisted or not
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Anchors")
    bool bIsPersisted{};

    /**
    * If persisted, the persisted anchor Id
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Anchors")
    FGuid PersistedId{};

    /**
    * The persisted state of the anchor entity
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Anchors")
    ESpatialAnchorPersistState PersistState{};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnchorPersistOperationDelegate, UAndroidXRSpatialAnchorEntity*, Anchor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnchorPersistOperationFailedDelegate, ESpatialAnchorPeristenceResult, FailureReason);
