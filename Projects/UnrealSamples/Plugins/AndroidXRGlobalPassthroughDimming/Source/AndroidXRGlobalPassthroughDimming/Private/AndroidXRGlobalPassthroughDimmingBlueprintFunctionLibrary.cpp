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

#include "AndroidXRGlobalPassthroughDimmingBlueprintFunctionLibrary.h"
#include "IOpenXRHMD.h"
#include "IXRTrackingSystem.h"
#include "AndroidXRGlobalPassthroughDimming.h"
#include "AndroidXRGlobalPassthroughDimmingEventProxy.h"
#include "IAndroidXRGlobalPassthroughDimmingModule.h"

#define RETURN_IF_GLOBAL_PASSTHROUGH_DIMMING_MODULE_NOT_FOUND() if (!IAndroidXRGlobalPassthroughDimmingModule::IsAvailable()){return false;}

UAndroidXRGlobalPassthroughDimmingEventProxy* UAndroidXRGlobalPassthroughDimmingBlueprintFunctionLibrary::GetEventProxy()
{
    return UAndroidXRGlobalPassthroughDimmingEventProxy::GetInstance();
}

bool UAndroidXRGlobalPassthroughDimmingBlueprintFunctionLibrary::GetGlobalDimmingSupported()
{
    RETURN_IF_GLOBAL_PASSTHROUGH_DIMMING_MODULE_NOT_FOUND();

    return IAndroidXRGlobalPassthroughDimmingModule::Get().GetGlobalDimmingSupported();
}

bool UAndroidXRGlobalPassthroughDimmingBlueprintFunctionLibrary::EnumerateSupportedGlobalDimmingLevels(bool& AllLevelsSupported, TArray<float>& SupportedGlobalDimmingLevels)
{
    RETURN_IF_GLOBAL_PASSTHROUGH_DIMMING_MODULE_NOT_FOUND()
    return IAndroidXRGlobalPassthroughDimmingModule::Get().EnumerateSupportedGlobalDimmingLevels(AllLevelsSupported, SupportedGlobalDimmingLevels);
}

bool UAndroidXRGlobalPassthroughDimmingBlueprintFunctionLibrary::SetGlobalDimmingLevel(float PreferredDimmingLevel)
{
    RETURN_IF_GLOBAL_PASSTHROUGH_DIMMING_MODULE_NOT_FOUND()
    return IAndroidXRGlobalPassthroughDimmingModule::Get().SetGlobalDimmingLevel(PreferredDimmingLevel);
}

bool UAndroidXRGlobalPassthroughDimmingBlueprintFunctionLibrary::GetGlobalDimmingLevel(float& CurrentDimmingLevel)
{
    RETURN_IF_GLOBAL_PASSTHROUGH_DIMMING_MODULE_NOT_FOUND()
    return IAndroidXRGlobalPassthroughDimmingModule::Get().GetGlobalDimmingLevel(CurrentDimmingLevel);
}