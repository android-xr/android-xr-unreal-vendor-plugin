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

#include "AndroidXRGlobalDimmingBlueprintFunctionLibrary.h"
#include "IOpenXRHMD.h"
#include "IXRTrackingSystem.h"
#include "AndroidXRGlobalDimming.h"
#include "AndroidXRGlobalDimmingEventProxy.h"
#include "IAndroidXRGlobalDimmingModule.h"

#define RETURN_IF_GLOBAL_DIMMING_MODULE_NOT_FOUND() if (!IAndroidXRGlobalDimmingModule::IsAvailable()){return false;}

UAndroidXRGlobalDimmingEventProxy* UAndroidXRGlobalDimmingBlueprintFunctionLibrary::GetEventProxy()
{
    return UAndroidXRGlobalDimmingEventProxy::GetInstance();
}

bool UAndroidXRGlobalDimmingBlueprintFunctionLibrary::GetGlobalDimmingSupported()
{
    RETURN_IF_GLOBAL_DIMMING_MODULE_NOT_FOUND();

    return IAndroidXRGlobalDimmingModule::Get().GetGlobalDimmingSupported();
}

bool UAndroidXRGlobalDimmingBlueprintFunctionLibrary::EnumerateSupportedGlobalDimmingLevels(bool& AllLevelsSupported, TArray<float>& SupportedGlobalDimmingLevels)
{
    RETURN_IF_GLOBAL_DIMMING_MODULE_NOT_FOUND()
    return IAndroidXRGlobalDimmingModule::Get().EnumerateSupportedGlobalDimmingLevels(AllLevelsSupported, SupportedGlobalDimmingLevels);
}

bool UAndroidXRGlobalDimmingBlueprintFunctionLibrary::SetGlobalDimmingLevel(float PreferredDimmingLevel)
{
    RETURN_IF_GLOBAL_DIMMING_MODULE_NOT_FOUND()
    return IAndroidXRGlobalDimmingModule::Get().SetGlobalDimmingLevel(PreferredDimmingLevel);
}

bool UAndroidXRGlobalDimmingBlueprintFunctionLibrary::GetGlobalDimmingLevel(float& CurrentDimmingLevel)
{
    RETURN_IF_GLOBAL_DIMMING_MODULE_NOT_FOUND()
    return IAndroidXRGlobalDimmingModule::Get().GetGlobalDimmingLevel(CurrentDimmingLevel);
}