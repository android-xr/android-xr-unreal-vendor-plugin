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
#include "AndroidXRSpatialAnchorEntity.h"
#include "AndroidXRSpatialAnchorTypes.h"
#include "AndroidXRSpatialAnchorsSubsystem.h"
#include "AndroidXRSpatialComponentBase.h"
#include "AndroidXRSpatialAnchorsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpatialAnchorPersistedDelegate);

/**
* The anchor component that can be added to create, destroy, and persist an anchor
*
*/
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ANDROIDXRSPATIALANCHORS_API UAndroidXRSpatialAnchorsComponent : public USceneComponent
{
	GENERATED_BODY()
public:

	UAndroidXRSpatialAnchorsComponent();
	void BeginPlay() override;

	/**
	* Whether to update the component transform with the tracked anchor's transform
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Anchors")
	bool bUpdateComponentTransform{true};

	/**
	* The associated anchor entity with this component. This can be set through Initialize or will be set by calling CreateAnchor
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Anchors")
	UAndroidXRSpatialAnchorEntity* AnchorInfo{};

	/**
	* The callback for when the anchor associated with this component is persisted
	*/
	UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Anchors")
	FSpatialAnchorPersistedDelegate OnAnchorPersisted{};

	/**
	* The callback for when the persist operation associated with this component fails
	*/
	UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Anchors")
	FAnchorPersistOperationFailedDelegate OnAnchorPersistFailed{};

	/**
	* The callback for when the anchor associated with this component is unpersisted
	*/
	UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Anchors")
	FSpatialAnchorPersistedDelegate OnAnchorUnpersisted{};

	/**
	* The callback for when the unpersist operation fails
	*/
	UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Anchors")
	FAnchorPersistOperationFailedDelegate OnAnchorUnpersistFailed{};

	/**
	* Set the associated anchor entity for this component (for example, after loading persisted anchors)
	*/
	UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Anchors")
	void Initialize(UAndroidXRSpatialAnchorEntity* InAnchor);

	/**
	* Create an anchor and associate it with this component
	* @return Whether the anchor was created
	*/
	UFUNCTION(BlueprintCallable)
	bool CreateAnchor();

	/**
	* Destroys the anchor associated it with this component
	* @return Whether the anchor was destroyed
	*/
	UFUNCTION(BlueprintCallable)
	bool DestroyAnchor();

	/**
	* Persist the anchor associated with this component
	*/
	UFUNCTION(BlueprintCallable)
	bool PersistAnchor();

	/**
	* Unpersist the anchor associated with this component
	*/
	UFUNCTION(BlueprintCallable)
	bool UnpersistAnchor();

	/**
	* Returns whether the component has an anchor associated with it
	* @return Whether the component has an anchor associated with it
	*/
	UFUNCTION(BlueprintCallable)
	bool IsValidAnchor() const
	{
		return bInitialized;
	}

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:

	UFUNCTION()
	void AnchorPersistedHandler(UAndroidXRSpatialAnchorEntity* Anchor);

	UFUNCTION()
	void AnchorUnpersistedHandler(UAndroidXRSpatialAnchorEntity* Anchor);

	UFUNCTION()
	void PersistFailedHandler(ESpatialAnchorPeristenceResult Result);

	UFUNCTION()
	void UnpersistFailedHandler(ESpatialAnchorPeristenceResult Result);

	UAndroidXRSpatialAnchorsSubsystem* Subsystem{};
	bool bInitialized{};
};
