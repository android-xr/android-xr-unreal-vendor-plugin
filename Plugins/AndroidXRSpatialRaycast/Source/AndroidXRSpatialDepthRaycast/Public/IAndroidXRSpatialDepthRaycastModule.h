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
#include "AndroidXRSpatialDiscoveryRaycastTypes.h"
#include "AndroidXRSpatialEntitiesModuleBase.h"

class ANDROIDXRSPATIALDEPTHRAYCAST_API IAndroidXRSpatialDepthRaycastModule : public FAndroidXRSpatialCapabilityModule
{
public:
    /**
    * Singleton-like access to this module's interface.  This is just for
    * convenience! Beware of calling this during the shutdown phase, though. Your
    * module might have been unloaded already.
    *
    * @return Returns singleton instance, loading the module on demand if needed
    */
    static inline IAndroidXRSpatialDepthRaycastModule& Get()
    {
        return FModuleManager::LoadModuleChecked<IAndroidXRSpatialDepthRaycastModule>("AndroidXRSpatialDepthRaycast");
    }

    /**
     * Checks to see if this module is loaded and ready.  It is only valid to call
     * Get() if IsAvailable() returns true.
     *
     * @return True if the module is loaded and ready to use
     */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("AndroidXRSpatialDepthRaycast");
    }

    /**
    * Checks for depth raycast support
    * @return Returns true if the runtime supports depth raycast
    */
    virtual bool IsDepthRaycastSupported() const = 0;

    /**
    * Checks if the depth raycast can be performed (if a previous depth raycast request is being processed, this will return false)
    * @return Returns true if the runtime can perform a depth raycast
    */
    virtual bool CanPerformDepthRaycast() const = 0;

    /**
    * Perform a depth raycast asynchronously
    * @param[in] The raycast config to perform depth raycast with
    * @param[in] The callback to execute when the operation finishes
    * @param[in] The callback for when the operation fails
    * @return True if the underlying API call was successful
    */
    virtual bool DepthRaycastAsync(const FSpatialRaycastConfig& RaycastConfig, TDelegate<void(const FSpatialRaycastHitInfo&)> DepthRaycastCallback, TDelegate<void()> OnFailedCallback) = 0;
};
