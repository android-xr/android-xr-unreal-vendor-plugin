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
#include "AndroidXRSpatialTrackingSubsystem.h"
#include "AndroidXRTypes.h"
#include "AndroidXRSpatialEntitiesTypes.h"
#include "AndroidXRSpatialPlaneEntity.h"
#include "AndroidXRSpatialDiscoveryRaycastTypes.h"
#include "AndroidXRSpatialDiscoveryRaycastSubsystem.generated.h"
/**
 * The spatial discovery raycast subsystem
 */
UCLASS()
class ANDROIDXRSPATIALDISCOVERYRAYCAST_API UAndroidXRSpatialDiscoveryRaycastSubsystem : public UAndroidXRTrackingSubsystem
{
	GENERATED_BODY()
public:

    /**
    * Returns whether discovery raycast is supported for the capability
    * @return True if discovery raycast is supported for the capability
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Discovery Raycast")
    bool IsRaycastDiscoverySupported(ESpatialCapability Capability) const;

    /**
    * Raycasts for entities tracked by a spatial context
    * @param[in] The spatial context to raycast against
    * @param[in] The ray configuration to perfrom the raycast with
    * @param[out] An array of found entities by the raycast
    * @return True if the underlying API call was successful
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Discovery Raycast")
    bool RaycastForEntities(const FSpatialContext& SpatialContext, const FSpatialRaycastConfig& RaycastConfig, TArray<FSpatialRaycastEntityResult>& FoundEntities);
    /**
    * Raycasts for planes
    * @param[in] The ray configuration to perform the discovery raycast
    * @param[out] The planes found by the raycast
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Discovery Raycast")
    bool RaycastForPlanes(const FSpatialRaycastConfig& RaycastConfig, TArray<FSpatialRaycastPlaneResult>& FoundPlanes);

protected:
    void GetRequiredPermissions(TArray<FString>& Permissions) override;
};
