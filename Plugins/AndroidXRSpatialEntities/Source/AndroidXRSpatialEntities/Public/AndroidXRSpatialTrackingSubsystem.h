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
#include "AndroidXRSpatialEntitiesTypes.h"
#include "AndroidXRSpatialEntitiesModuleBase.h"
#include "AndroidXRSpatialTrackingSubsystem.generated.h"

/**
* The base class of all spatial tracking subsystems
*/
UCLASS()
class ANDROIDXRSPATIALENTITIES_API UAndroidXRSpatialEntitiesSubsystem : public UAndroidXRTrackingSubsystem
{
    GENERATED_BODY()
public:
    void CreateTracker() override;
    void DestroyTracker() override;

    void Initialize(FSubsystemCollectionBase& Collection) override;

    /**
    * Whether the underlying tracking subsystem is ready or not
    * @return The tracking status of the underlying spatial tracking subsystem
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Entities")
    bool IsReady() const
    {
        if(!CapabilityModule)
        {
            return false;
        }
        return CapabilityModule->IsReady();
    }

    /**
    * Gets all the supported capabilities
    * @param[out] An array of supported capabilities
    * @return Whether the underlying API call was successful
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Entities")
    bool GetSupportedCapabilities(TArray<ESpatialCapability>& Capabilities);

    /**
    * Gets the capability info for a given capability
    * @param[in] The capability to get the information for
    * @param[out] The capability info
    * @return Whether the underlying API call was successful
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Entities")
    bool GetCapabilityInfo(ESpatialCapability Capability, FAndroidXRSpatialCapabilityInfo& CapabilityInfo);

    /**
    * Creates a spatial discovery snapshot request
    * @return Whether the discovery request was created or not
    **/
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Entities")
    bool CreateDiscoverySnapshotRequest();

protected:
    virtual FAndroidXRSpatialCapabilityModule* GetCapabilityModule()
    {
        return nullptr;
    }

    virtual void BindEntityEvents()
    {
    }
    virtual void UnbindEntityEvents()
    {
    }
private:
    void SpatialContextCreatedHandler();
    void SpatialContextDestroyedHandler();
    FAndroidXRSpatialCapabilityModule* CapabilityModule{};
};
