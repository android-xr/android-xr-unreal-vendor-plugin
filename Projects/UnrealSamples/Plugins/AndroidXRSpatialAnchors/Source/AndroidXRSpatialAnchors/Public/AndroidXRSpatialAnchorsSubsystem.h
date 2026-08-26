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
#include "AndroidXRTrackingSubsystem.h"
#include "AndroidXRSpatialTrackingSubsystem.h"
#include "AndroidXRSpatialAnchorTypes.h"
#include "AndroidXRSpatialAnchorEntity.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AndroidXRSpatialTrackingSubsystem.h"
#include "AndroidXRSpatialAnchorsSubsystem.generated.h"

/**
* The Spatial Anchors subsystem
*/
UCLASS()
class ANDROIDXRSPATIALANCHORS_API UAndroidXRSpatialAnchorsSubsystem : public UAndroidXRSpatialEntitiesSubsystem
{
    GENERATED_BODY()
public:

    /**
    * Creates a spatial anchor at the specified transform
    * @param[in] The transform at which to create the anchor
    * @param[out] The created anchor
    * @return Whether the underlying API call was successful
    */
    UFUNCTION(BlueprintCallable, Category="AndroidXR|Spatial Anchors")
    bool CreateSpatialAnchor(const FTransform& AnchorTransform,UAndroidXRSpatialAnchorEntity*& CreatedAnchor);

    /**
    * Destroys a spatial anchor
    * @param[in] The anchor to destroy
    * @return Whether the underlying API call was successful
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Anchors")
    bool DestroySpatialAnchor(UAndroidXRSpatialAnchorEntity* AnchorToDestroy);

    /**
    * Loads persistent anchors. The callbacks of OnAnchorAdded will be fired for the loaded anchors
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Anchors")
    bool LoadPersistedAnchors();

    /**
    * The callback for when the runtime detects anchors in the environment (when loading persisted anchors for example)
    */
    UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Anchors")
    FAnchorPersistOperationDelegate OnAnchorAdded{};

    /**
    * The callback for when the runtime removes tracked anchors from the environment (when loading persisted anchors for example)
    */
    UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Anchors")
    FAnchorPersistOperationDelegate OnAnchorRemoved{};

protected:
    void GetRequiredPermissions(TArray<FString>& Permissions) override;

    void BindEntityEvents() override;
    void UnbindEntityEvents() override;
    FAndroidXRSpatialCapabilityModule* GetCapabilityModule() override;
private:

    void AnchorAddedHandler(TObjectPtr<UAndroidXRSpatialAnchorEntity> AddedAnchor);
    void AnchorRemovedHandler(TObjectPtr<UAndroidXRSpatialAnchorEntity> RemovedAnchor);

    FDelegateHandle AnchorAddedHandle{};
    FDelegateHandle AnchorRemovedHandle{};
};

/**
* The async operations associated with spatial anchors
*/
UCLASS(ClassGroup = AndroidXRSpatialAnchors)
class ANDROIDXRSPATIALANCHORS_API UAndroidXRSpatialAnchorsPersistenceAsyncAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
public:

    /**
    * The callback for when the async operation succeeds
    */
    UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Anchors")
    FAnchorPersistOperationDelegate OnOperationSuccess{};

    /**
    * The callback for when the async operation fails
    */
    UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Anchors")
    FAnchorPersistOperationFailedDelegate OnOperationFailed{};

    /**
    * Persists a spatial anchor
    * @param[in] The anchor to persist
    * @return The asynchronous operation for persistence
    */
    UFUNCTION(BlueprintCallable, Category="AndroidXR|Spatial Anchors")
    static UAndroidXRSpatialAnchorsPersistenceAsyncAction* PersistAnchorAsync(UAndroidXRSpatialAnchorEntity* AnchorToPersist);

    /**
    * Unpersists a spatial anchor
    * @param[in] The anchor to unpersist
    * @return The asynchronous operation for unpersistence
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Anchors")
    static UAndroidXRSpatialAnchorsPersistenceAsyncAction* UnPersistAnchorAsync(UAndroidXRSpatialAnchorEntity* AnchorToUnPersist);

    void Activate() override;

private:
    UAndroidXRSpatialAnchorEntity* Anchor{};
    bool bUnPersistOperation;

    UFUNCTION()
    void PersistOperationFinishedHandler(UAndroidXRSpatialAnchorEntity* Anchor);

    UFUNCTION()
    void PersistOperationFailedHandler(ESpatialAnchorPeristenceResult Result);
};
