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

#include "AndroidXRSystemExtensionPropertiesBlueprintFunctionLibrary.h"
#include "IAndroidXRSystemExtensionPropertiesModule.h"
#include "AndroidXRSystemExtensionPropertiesEventProxy.h"

#define RETURN_IF_SYSTEM_EXTENSION_PROPERTIES_MODULE_NOT_FOUND() if (!IAndroidXRSystemExtensionPropertiesModule::IsAvailable()){return false;}

UAndroidXRSystemExtensionPropertiesEventProxy* UAndroidXRSystemExtensionPropertiesBlueprintFunctionLibrary::GetEventProxy()
{
    return UAndroidXRSystemExtensionPropertiesEventProxy::GetInstance();
}

bool UAndroidXRSystemExtensionPropertiesBlueprintFunctionLibrary::EnumerateSystemExtensionProperties(TArray<FAndroidXRSystemExtensionInformation>& ExtensionProperties)
{
    RETURN_IF_SYSTEM_EXTENSION_PROPERTIES_MODULE_NOT_FOUND()
    return IAndroidXRSystemExtensionPropertiesModule::Get().EnumerateSystemExtensionProperties(ExtensionProperties);
}