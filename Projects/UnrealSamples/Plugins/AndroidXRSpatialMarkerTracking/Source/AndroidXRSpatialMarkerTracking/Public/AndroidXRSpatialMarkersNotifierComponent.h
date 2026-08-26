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
#include "AndroidXRSpatialMarkerEntity.h"
#include "AndroidXRSpatialMarkerTrackingTypes.h"
#include "AndroidXRSpatialMarkerTrackingSubsystem.h"
#include "AndroidXRSpatialComponentBase.h"
#include "AndroidXRSpatialMarkersNotifierComponent.generated.h"

/**
* The component that keeps track of tracked marker entities
*/
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ANDROIDXRSPATIALMARKERTRACKING_API UAndroidXRSpatialMarkersNotifierComponent : public UAndroidXRSpatialComponentBase
{
	GENERATED_BODY()
public:

	/**
	* The configuration to start marker tracking with
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Spatial Marker Tracking")
	FSpatialMarkerTrackingConfig MarkerTrackingConfig{};

	/**
	* The callback for when a marker is added by the runtime
	*/
	UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Marker Tracking")
	FSpatialMarkerChangedDelegate OnMarkerAdded{};

	/**
	* The callback for when the runtime removes a marker
	*/
	UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Marker Tracking")
	FSpatialMarkerChangedDelegate OnMarkerRemoved{};

	/**
	* The markers being tracked
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Marker Tracking")
	TArray<UAndroidXRSpatialMarkerEntity*> TrackedMarkers{};

protected:
	UAndroidXRSpatialEntitiesSubsystem* GetCapabilitySubsystem() override;
	void BindToTrackingEvents() override;
	void UnbindFromTrackingEvents() override;
	void OnStartTracking() override;
private:

	UAndroidXRSpatialMarkerTrackingSubsystem* MarkerTrackingSubsystem;

	UFUNCTION()
	void MarkerAddedHandler(UAndroidXRSpatialMarkerEntity* Marker);

	UFUNCTION()
	void MarkerRemovedHandler(UAndroidXRSpatialMarkerEntity* Marker);
};
