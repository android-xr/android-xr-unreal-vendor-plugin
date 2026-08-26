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

#include "AndroidXRSystemExtensionPropertiesTypes.h"
#include "AndroidXRSystemExtensionPropertiesBlueprintFunctionLibrary.generated.h"

class UAndroidXRSystemExtensionPropertiesEventProxy;

UCLASS(ClassGroup=AndroidXRSystemExtensionProperties)
class ANDROIDXRSYSTEMEXTENSIONPROPERTIES_API UAndroidXRSystemExtensionPropertiesBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
        /**
        * Gets the AndroidXRSystemExtensionProperties event proxy, which allows users to bind to
        * delegates which fired in response to XrEvents.
        * @return AndroidXRSystemExtensionProperties event proxy static instance.
        */
        UFUNCTION(BlueprintCallable, Category = "AndroidXR|System Extension Properties Function Library")
        static UAndroidXRSystemExtensionPropertiesEventProxy* GetEventProxy();

        /**
        * Get the current properties of extensions on the system.
        *
        * @param[out] An array of FAndroidXRSystemExtensionInformation containing
        * extensions and their properties from the system.
        * @return True if the underlying API call was successful.
        */
        UFUNCTION(BlueprintCallable, Category = "AndroidXR|System Extension Properties Function Library")
        static bool EnumerateSystemExtensionProperties(TArray<FAndroidXRSystemExtensionInformation>& ExtensionProperties);
};
