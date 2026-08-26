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
#include "AndroidXRSpatialPlaneTrackingTypes.h"
#include "AndroidXRSpatialPlaneEntity.h"
#include "AndroidXRSpatialTrackingSubsystem.h"
#include "AndroidXRSpatialPlaneTrackingSubsystem.generated.h"

/**
 * The plane tracking subsystem
 */
UCLASS()
class ANDROIDXRSPATIALPLANETRACKING_API UAndroidXRSpatialPlaneTrackingSubsystem : public UAndroidXRSpatialEntitiesSubsystem
{
    GENERATED_BODY()
public:
    /**
    * The callback for when a spatial plane is added
    */
    UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Plane Tracking")
    FSpatialPlaneChangedDelegate OnSpatialPlaneAdded{};

    /**
    * The callback for when a spatial plane is removed
    */
    UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Spatial Plane Tracking")
    FSpatialPlaneChangedDelegate OnSpatialPlaneRemoved{};

    /**
    * Get the planes being currently tracked
    * @param[out] The tracked planes
    * @param[in] Whether to force an update before updating the tracked planes array
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Plane Tracking")
    void GetTrackedPlanes(TArray<UAndroidXRSpatialPlaneEntity*>& TrackedPlanes, bool bShouldUpdate);

protected:
    void GetRequiredPermissions(TArray<FString>& Permissions) override;

    void BindEntityEvents() override;
    void UnbindEntityEvents() override;
    FAndroidXRSpatialCapabilityModule* GetCapabilityModule() override;

private:
    void SpatialPlaneAddedHandler(TObjectPtr<UAndroidXRSpatialPlaneEntity> AddedPlane);
    void SpatialPlaneRemovedHandler(TObjectPtr<UAndroidXRSpatialPlaneEntity> RemovedPlane);

    FDelegateHandle PlaneAddedHandle{};
    FDelegateHandle PlaneRemovedHandle{};
};
