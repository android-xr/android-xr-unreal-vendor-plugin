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

#include "AndroidXRGlobalPassthroughDimming.h"
#include "AndroidXRGlobalPassthroughDimmingBlueprintFunctionLibrary.h"
#include "AndroidXRGlobalPassthroughDimmingEventProxy.h"
#include "AndroidXRTypeConversions.h"
#include <IOpenXRHMDModule.h>
#include <IXRTrackingSystem.h>
#include <IHeadMountedDisplay.h>
#include "IOpenXRHMD.h"

DEFINE_LOG_CATEGORY(LogAndroidXRGlobalPassthroughDimming);
#define LOG_IF_OPENXR_ERROR_AND_RETURN(Tag, Function, Result) if(!XR_UNQUALIFIED_SUCCESS(Result)){ UE_LOG(LogAndroidXRGlobalPassthroughDimming, Error, TEXT("%s : %s failed with error %s"), TEXT(#Tag), TEXT(#Function), OpenXRResultToString(Result)); return false;}
#define RETURN_IF_FUNCTION_NOT_FOUND(Function) if(!Function){ UE_LOG(LogAndroidXRGlobalPassthroughDimming, Error, TEXT("Unable to locate function %s"), TEXT(#Function)); return false;}

FAndroidXRGlobalPassthroughDimming::FAndroidXRGlobalPassthroughDimming(){}

void FAndroidXRGlobalPassthroughDimming::StartupModule()
{
    IAndroidXRGlobalPassthroughDimmingModule::StartupModule();
    RegisterOpenXRExtensionModularFeature();
}

void FAndroidXRGlobalPassthroughDimming::ShutdownModule()
{
    UnregisterOpenXRExtensionModularFeature();
    IAndroidXRGlobalPassthroughDimmingModule::ShutdownModule();
}

bool FAndroidXRGlobalPassthroughDimming::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
    OutExtensions.Add(XR_ANDROID_GLOBAL_PASSTHROUGH_DIMMING_EXTENSION_NAME);
    return true;
}

void FAndroidXRGlobalPassthroughDimming::PostCreateInstance(XrInstance InInstance)
{
    Instance = InInstance;

    ResolveOpenXRFunctions(Instance,
        {
            GLOBALPASSTHROUGHDIMMING_FUNCTIONS(RESOLVE_OPENXR_FUNC)
        },
        [](const char* UnresolvableFunctionName, XrResult Result)
        {
            UE_LOG(LogAndroidXRGlobalPassthroughDimming, Error,
                TEXT("Unable to resolve function pointer %s (error %s)"),
                UTF8_TO_TCHAR(UnresolvableFunctionName),
                OpenXRResultToString(Result));
        });
}

void FAndroidXRGlobalPassthroughDimming::PostCreateSession(XrSession InSession)
{
    Session = InSession;
    bool bAllLevelsSupported{};
    EnumerateSupportedGlobalDimmingLevels(bAllLevelsSupported, CachedSupportedGlobalDimmingLevels);
}

void FAndroidXRGlobalPassthroughDimming::OnEvent(XrSession InSession, const XrEventDataBaseHeader* InHeader)
{
    if (InHeader->type == XR_TYPE_EVENT_DATA_GLOBAL_DIMMING_LEVEL_CHANGED_ANDROID)
    {
        float DimmingLevel{ 0.0f };
        if (GetGlobalDimmingLevel(DimmingLevel))
        {
            AtomicDimmingLevel = -1.0f;
            UAndroidXRGlobalPassthroughDimmingBlueprintFunctionLibrary::GetEventProxy()->
                OnGlobalDimmingLevelChanged.Broadcast(DimmingLevel);
        }
    }
}

const void* FAndroidXRGlobalPassthroughDimming::OnEndFrame(XrSession InSession, XrTime DisplayTime, const void* InNext)
{
    GlobalDimmingFrameEndInfo.next = InNext;
    GlobalDimmingFrameEndInfo.globalDimmingLevel = AtomicDimmingLevel;

    return &GlobalDimmingFrameEndInfo;
}

bool FAndroidXRGlobalPassthroughDimming::EnumerateSupportedGlobalDimmingLevels(bool& bAllLevelsSupported, TArray<float>& SupportedGlobalDimmingLevels)
{
    RETURN_IF_FUNCTION_NOT_FOUND(xrEnumerateSupportedGlobalDimmingLevelsANDROID);

    if (bLevelsHaveBeenEnumerated)
    {
        bAllLevelsSupported = CachedSupportedGlobalDimmingLevels.IsEmpty();
        SupportedGlobalDimmingLevels = CachedSupportedGlobalDimmingLevels;
        return true;
    }

    SupportedGlobalDimmingLevels.Reset();

    uint32_t CapacityOutput{ 0 };

    auto Result = xrEnumerateSupportedGlobalDimmingLevelsANDROID(Instance,
        IOpenXRHMDModule::Get().GetSystemId(),
        CapacityOutput,
        &CapacityOutput,
        nullptr);
    LOG_IF_OPENXR_ERROR_AND_RETURN(InternalEnumerateSupportedGlobalDimmingLevels, xrEnumerateSupportedGlobalDimmingLevelsANDROID, Result);

    if (CapacityOutput == 0)
    {
        bAllLevelsSupported = true;
        return true;
    }

    SupportedGlobalDimmingLevels.SetNum(CapacityOutput);

    Result = xrEnumerateSupportedGlobalDimmingLevelsANDROID(Instance,
        IOpenXRHMDModule::Get().GetSystemId(),
        SupportedGlobalDimmingLevels.Num(),
        &CapacityOutput,
        SupportedGlobalDimmingLevels.GetData());
    LOG_IF_OPENXR_ERROR_AND_RETURN(InternalEnumerateSupportedGlobalDimmingLevels, xrEnumerateSupportedGlobalDimmingLevelsANDROID, Result);

    if (!bLevelsHaveBeenEnumerated)
    {
        bLevelsHaveBeenEnumerated = true;
        CachedSupportedGlobalDimmingLevels = SupportedGlobalDimmingLevels;
    }

    return true;
}

bool FAndroidXRGlobalPassthroughDimming::GetGlobalDimmingSupported() const
{
    RETURN_IF_FUNCTION_NOT_FOUND(xrGetSystemProperties);
    XrSystemGlobalDimmingPropertiesANDROID GlobalDimmingProperties{ XR_TYPE_SYSTEM_GLOBAL_DIMMING_PROPERTIES_ANDROID };
    XrSystemProperties SystemProperties{
        .type = XR_TYPE_SYSTEM_PROPERTIES,
        .next = &GlobalDimmingProperties };

    auto Result = xrGetSystemProperties(Instance, IOpenXRHMDModule::Get().GetSystemId(), &SystemProperties);
    LOG_IF_OPENXR_ERROR_AND_RETURN(GetGlobalDimmingSupported, xrGetSystemProperties, Result);

    return !!GlobalDimmingProperties.supportsGlobalDimming;
}

bool FAndroidXRGlobalPassthroughDimming::SetGlobalDimmingLevel(float PreferredDimmingLevel)
{
    AtomicDimmingLevel = FMath::Clamp(PreferredDimmingLevel, 0.0f, 1.0f);
    return true;
}

bool FAndroidXRGlobalPassthroughDimming::GetGlobalDimmingLevel(float& CurrentDimmingLevel) const
{
    RETURN_IF_FUNCTION_NOT_FOUND(xrGetGlobalDimmingLevelANDROID);

    auto Result = xrGetGlobalDimmingLevelANDROID(Session, &CurrentDimmingLevel);
    LOG_IF_OPENXR_ERROR_AND_RETURN(GetGlobalDimmingLevel, xrGetGlobalDimmingLevelANDROID, Result);
    return true;
}


IMPLEMENT_MODULE(FAndroidXRGlobalPassthroughDimming, AndroidXRGlobalPassthroughDimming)