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
#include "AndroidXRTypes.h"
#include "AndroidXRGlobalPassthroughDimmingEventProxy.h"

/**
 * The public interface to this module.  In most cases, this interface is only
 * public to sibling modules within this plugin.
 */
class ANDROIDXRGLOBALPASSTHROUGHDIMMING_API IAndroidXRGlobalPassthroughDimmingModule : public IModuleInterface
{
public:
    /**
     * Singleton-like access to this module's interface. This is just for
     * convenience! Beware of calling this during the shutdown phase, though. Your
     * module might have been unloaded already.
     *
     * @return Returns singleton instance, loading the module on demand if needed
     */
    static inline IAndroidXRGlobalPassthroughDimmingModule& Get()
    {
        return FModuleManager::LoadModuleChecked<IAndroidXRGlobalPassthroughDimmingModule>("AndroidXRGlobalPassthroughDimming");
    }

    /**
     * Checks to see if this module is loaded and ready.  It is only valid to call
     * Get() if IsAvailable() returns true.
     *
     * @return True if the module is loaded and ready to use
     */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("AndroidXRGlobalPassthroughDimming");
    }

    /**
     * Enumerates the global dimming levels supported by the system.
     * If returns true but SupportedGlobalDimmingLevels is empty
     * all levels are supported.
     *
     * @param[out] True if all levels between 0 and 1 are supported.
     * SupportedGlobalDimmingLevels will be empty if this is true.
     * @param[out] An array of supported levels, but can be empty if all
     * levels between 0 and 1 are supported.
     * @return True if the underlying API call was successful.
     */
    virtual bool EnumerateSupportedGlobalDimmingLevels(bool& AllLevelsSupported, TArray<float>& SupportedGlobalDimmingLevels) = 0;

    /**
     * Checks if global dimming is supported by the system.
     *
     * @return True if the system supports global dimming.
     */
    virtual bool GetGlobalDimmingSupported() const = 0;

    /**
     * Sets the preferred global dimming level.
     *
     * @param[in] The preferred global dimming level.
     * @return True if the underlying API call was successful.
     */
    virtual bool SetGlobalDimmingLevel(float PreferredDimmingLevel) = 0;

    /**
     * Gets the current global dimming level.
     *
     * @param[out] The current global dimming level.
     * @return True if the underlying API call was successful.
     */
    virtual bool GetGlobalDimmingLevel(float& CurrentDimmingLevel) const = 0;

    /**
    * Gets the AndroidXRGlobalPassthroughDimming event proxy, which allows users to bind to
    * delegates which fired in response to XrEvents.
    * @return AndroidXRGlobalPassthroughDimming event proxy static instance.
    */
    virtual UAndroidXRGlobalPassthroughDimmingEventProxy* GetEventProxy() = 0;
};
