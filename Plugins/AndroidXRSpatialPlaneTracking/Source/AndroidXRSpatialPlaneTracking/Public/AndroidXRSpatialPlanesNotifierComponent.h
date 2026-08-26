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
#include "AndroidXRSpatialPlaneEntity.h"
#include "AndroidXRSpatialPlaneTrackingTypes.h"
#include "AndroidXRSpatialPlaneTrackingSubsystem.h"
#include "AndroidXRSpatialComponentBase.h"
#include "AndroidXRSpatialPlanesNotifierComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAndroidXRSpatialPlaneComponent, Log, All);

/**
* The plane tracking component that keeps track of spatial planes tracked by the runtime
*/
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ANDROIDXRSPATIALPLANETRACKING_API UAndroidXRSpatialPlanesNotifierComponent : public UAndroidXRSpatialComponentBase
{
	GENERATED_BODY()
public:
	void BeginPlay() override;

	/**
	* The callback for when a spatial plane is added
	*/
	UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Plane Tracking")
	FSpatialPlaneChangedDelegate OnPlaneAdded{};

	/**
	* The callback for when a spatial plane is removed
	*/
	UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Plane Tracking")
	FSpatialPlaneChangedDelegate OnPlaneRemoved{};

	/**
	* The planes being currently tracked
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AndroidXR|Spatial Plane Tracking")
	TSet<UAndroidXRSpatialPlaneEntity*> TrackedPlanes{};

protected:
	void BindToTrackingEvents() override;
	void UnbindFromTrackingEvents() override;
	UAndroidXRSpatialEntitiesSubsystem* GetCapabilitySubsystem() override;
private:
	UAndroidXRSpatialPlaneTrackingSubsystem* PlaneTrackingSubsystem;

	UFUNCTION()
	void PlaneAddedHandler(UAndroidXRSpatialPlaneEntity* AddedPlane);

	UFUNCTION()
	void PlaneRemovedHandler(UAndroidXRSpatialPlaneEntity* RemovedPlane);
};
