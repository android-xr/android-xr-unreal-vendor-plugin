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
#include "Modules/ModuleManager.h"
#include "AndroidXRViewportFeatheringTypes.h"

/**
 * The public interface to this module.  In most cases, this interface is only
 * public to sibling modules within this plugin.
 */
class ANDROIDXRVIEWPORTFEATHERING_API IAndroidXRViewportFeatheringModule : public IModuleInterface
{
public:
    /**
     * Singleton-like access to this module's interface. This is just for
     * convenience! Beware of calling this during the shutdown phase, though. Your
     * module might have been unloaded already.
     *
     * @return Returns singleton instance, loading the module on demand if needed
     */
    static inline IAndroidXRViewportFeatheringModule& Get()
    {
        return FModuleManager::LoadModuleChecked<IAndroidXRViewportFeatheringModule>("AndroidXRViewportFeathering");
    }

    /**
     * Checks to see if this module is loaded and ready.  It is only valid to call
     * Get() if IsAvailable() returns true.
     *
     * @return True if the module is loaded and ready to use
     */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("AndroidXRViewportFeathering");
    }

    /**
     * Acquire information about the feathered insets for each side of the view.
     *
     * @param[out] An array of FAndroidXRViewportFeatheringConfigView with the
     * number of pixels on each side of the view that is affected by feathering.
     * @return True if the underlying API call was successful.
     */
    virtual bool GetPixelsAffectedByFeathering(TArray<FAndroidXRViewportFeatheringInsetInfo>& FeatheringViews) const = 0;

    /**
     * Check if Viewport Feathering is supported on the device.
     *
     * @return True if the underlying API call was successful.
     */
    virtual bool IsViewportFeatheringSupported() const = 0;
};
