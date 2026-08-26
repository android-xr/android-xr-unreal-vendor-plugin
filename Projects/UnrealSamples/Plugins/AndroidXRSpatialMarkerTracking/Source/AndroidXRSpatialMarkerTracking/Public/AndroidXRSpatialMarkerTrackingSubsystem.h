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
#include "AndroidXRSpatialMarkerTrackingTypes.h"
#include "AndroidXRSpatialMarkerEntity.h"
#include "AndroidXRSpatialTrackingSubsystem.h"
#include "AndroidXRSpatialMarkerTrackingSubsystem.generated.h"
/**
 * The marker tracking subsystem
 */
UCLASS()
class ANDROIDXRSPATIALMARKERTRACKING_API UAndroidXRSpatialMarkerTrackingSubsystem : public UAndroidXRSpatialEntitiesSubsystem
{
    GENERATED_BODY()
public:
    /**
    * Configures the marker tracking subsystem
    * @param[in] The marker tracking configuration
    */
    UFUNCTION(BlueprintCallable, Category="AndroidXR|Spatial Marker Tracking")
    void ConfigureMarkerTracking(const FSpatialMarkerTrackingConfig& MarkerTrackingConfig);

    /**
    * The callback for when a marker is added by the runtime
    */
    UPROPERTY(BlueprintAssignable, Category="AndroidXR|Spatial Marker Tracking")
    FOnSpatialMarkerChangedDelegate OnSpatialMarkerAdded{};

    /**
    * The callback for when a marker is removed by the runtime
    */
    UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Marker Tracking")
    FOnSpatialMarkerChangedDelegate OnSpatialMarkerRemoved{};

    /**
    * Gets the currently tracked markerszs
    * @param[out] Array of tracked markers
    * @param[in] Whether to force an update before populating the tracked markers array
    */
    UFUNCTION(BlueprintCallable, Category="AndroidXR|Spatial Marker Tracking")
    void GetTrackedMarkers(TArray<UAndroidXRSpatialMarkerEntity*>& TrackedMarkers, bool bShouldUpdate);

protected:
    void GetRequiredPermissions(TArray<FString>& Permissions) override;
    void BindEntityEvents() override;
    void UnbindEntityEvents() override;
    FAndroidXRSpatialCapabilityModule* GetCapabilityModule() override;
private:
    void SpatialMarkerAddedHandler(TObjectPtr<UAndroidXRSpatialMarkerEntity> AddedMarker);
    void SpatialMarkerRemovedHandler(TObjectPtr<UAndroidXRSpatialMarkerEntity> RemovedMarker);

    FDelegateHandle MarkerAddedHandle{};
    FDelegateHandle MarkerUpdatedHandle{};
    FDelegateHandle MarkerRemovedHandle{};
};
