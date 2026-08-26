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

#include "AndroidXRGlobalPassthroughDimmingBlueprintFunctionLibrary.generated.h"

class AndroidXRGlobalPassthroughDimmingEventProxy;

UCLASS(ClassGroup=AndroidXRGlobalPassthroughDimming)
class ANDROIDXRGLOBALPASSTHROUGHDIMMING_API UAndroidXRGlobalPassthroughDimmingBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    public:
        /**
        * Gets the AndroidXRGlobalPassthroughDimming event proxy, which allows users to bind to
        * delegates which fired in response to XrEvents.
        * @return AndroidXRGlobalPassthroughDimming event proxy static instance.
        */
        UFUNCTION(BlueprintCallable, Category = "AndroidXR|Global Passthrough Dimming Function Library")
        static UAndroidXRGlobalPassthroughDimmingEventProxy* GetEventProxy();

        /**
        * Checks if global dimming is supported by the system.
        * @return True if the system supports global dimming.
        */
        UFUNCTION(BlueprintCallable, Category = "AndroidXR|Global Passthrough Dimming Function Library")
        static bool GetGlobalDimmingSupported();

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
        UFUNCTION(BlueprintCallable, Category = "AndroidXR|Global Passthrough Dimming Function Library")
        static bool EnumerateSupportedGlobalDimmingLevels(bool& AllLevelsSupported, TArray<float>& SupportedGlobalDimmingLevels);

        /**
        * Sets the preferred global dimming level. EnumerateSupportedGlobalDimmingLevels will
        * provide the supported levels.
        *
        * @param[in] The preferred global dimming level. Must be between
        * 0 and 1. Values will be clamped. Call EnumerateSupportedGlobalDimmingLevels
        * to return the supported dimming levels.
        * @return True if the underlying API call was successful.
        */
        UFUNCTION(BlueprintCallable, Category = "AndroidXR|Global Passthrough Dimming Function Library")
        static bool SetGlobalDimmingLevel(float PreferredDimmingLevel);

        /**
        * Gets the current global dimming level.
        *
        * @param[out] The current global dimming level.
        * @return True if the underlying API call was successful.
        */
        UFUNCTION(BlueprintCallable, Category = "AndroidXR|Global Passthrough Dimming Function Library")
        static bool GetGlobalDimmingLevel(float& CurrentDimmingLevel);
};
