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
#include "AndroidXRSpatialEntitiesTypes.h"
#include "AndroidXRSpatialDiscoveryRaycastTypes.h"
#include "AndroidXRSpatialPlaneEntity.h"
#include "AndroidXRSpatialEntitiesModuleBase.h"

class ANDROIDXRSPATIALDISCOVERYRAYCAST_API IAndroidXRSpatialDiscoveryRaycastModule : public IModuleInterface
{
public:

    /**
    * Singleton-like access to this module's interface.  This is just for
    * convenience! Beware of calling this during the shutdown phase, though. Your
    * module might have been unloaded already.
    *
    * @return Returns singleton instance, loading the module on demand if needed
    */
    static inline IAndroidXRSpatialDiscoveryRaycastModule& Get()
    {
        return FModuleManager::LoadModuleChecked<IAndroidXRSpatialDiscoveryRaycastModule>("AndroidXRSpatialDiscoveryRaycast");
    }

    /**
     * Checks to see if this module is loaded and ready.  It is only valid to call
     * Get() if IsAvailable() returns true.
     *
     * @return True if the module is loaded and ready to use
     */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("AndroidXRSpatialDiscoveryRaycast");
    }

    /**
    * Returns whether discovery raycast is supported for the capability
    * @return True if discovery raycast is supported for the capability
    */
    virtual bool IsRaycastDiscoverySupported(ESpatialCapability Capability) const = 0;

    /**
    * Raycasts for entities tracked by a spatial context
    * @param[in] The spatial context to raycast against
    * @param[in] The ray configuration to perfrom the raycast with
    * @param[out] An array of found entities by the raycast
    * @return True if the underlying API call was successful
    */
    virtual bool RaycastForEntities(const FSpatialContext& SpatialContext, const FSpatialRaycastConfig& RaycastConfig, TArray<FSpatialRaycastEntityResult>& FoundEntities) = 0;

    /**
    * Raycasts for planes
    * @param[in] The ray configuration to perform the discovery raycast
    * @param[out] The planes found by the raycast
    */
    virtual bool RaycastForPlanes(const FSpatialRaycastConfig& RaycastConfig, TArray<FSpatialRaycastPlaneResult>& FoundPlanes) = 0;
};
