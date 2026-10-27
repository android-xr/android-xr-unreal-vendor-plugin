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

#include "AndroidXRViewportFeathering.h"
#include "AndroidXRTypeConversions.h"
#include <IOpenXRHMDModule.h>
#include <IXRTrackingSystem.h>
#include <IHeadMountedDisplay.h>
#include "IOpenXRHMD.h"

DEFINE_LOG_CATEGORY(LogAndroidXRViewportFeathering);
#define LOG_IF_OPENXR_ERROR_AND_RETURN(Tag, Function, Result) if(!XR_UNQUALIFIED_SUCCESS(Result)){ UE_LOG(LogAndroidXRViewportFeathering, Error, TEXT("%s : %s failed with error %s"), TEXT(#Tag), TEXT(#Function), OpenXRResultToString(Result)); return false;}
#define RETURN_IF_FUNCTION_NOT_FOUND(Function) if(!Function){ UE_LOG(LogAndroidXRViewportFeathering, Error, TEXT("Unable to locate function %s"), TEXT(#Function)); return false;}

FAndroidXRViewportFeathering::FAndroidXRViewportFeathering(){}

void FAndroidXRViewportFeathering::StartupModule()
{
    IAndroidXRViewportFeatheringModule::StartupModule();
    RegisterOpenXRExtensionModularFeature();
}

void FAndroidXRViewportFeathering::ShutdownModule()
{
    UnregisterOpenXRExtensionModularFeature();
    IAndroidXRViewportFeatheringModule::ShutdownModule();
}

bool FAndroidXRViewportFeathering::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
    OutExtensions.Add(XR_ANDROID_VIEWPORT_FEATHERING_EXTENSION_NAME);
    return true;
}

void FAndroidXRViewportFeathering::PostCreateInstance(XrInstance InInstance)
{
    Instance = InInstance;

    ResolveOpenXRFunctions(Instance,
        {
            VIEWPORTFEATHERING_FUNCTIONS(RESOLVE_OPENXR_FUNC)
        },
        [](const char* UnresolvableFunctionName, XrResult Result)
        {
            UE_LOG(LogAndroidXRViewportFeathering, Error,
                TEXT("Unable to resolve function pointer %s (error %s)"),
                UTF8_TO_TCHAR(UnresolvableFunctionName),
                OpenXRResultToString(Result));
        });
}

void FAndroidXRViewportFeathering::PostCreateSession(XrSession InSession)
{
    Session = InSession;
}

bool FAndroidXRViewportFeathering::GetPixelsAffectedByFeathering(TArray<FAndroidXRViewportFeatheringInsetInfo>& FeatheringViews) const
{
    RETURN_IF_FUNCTION_NOT_FOUND(xrEnumerateViewConfigurationViews)

    uint32_t ViewCountOutput{};
    auto Result = xrEnumerateViewConfigurationViews(Instance,
        IOpenXRHMDModule::Get().GetSystemId(),
        XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
        0,
        &ViewCountOutput,
        nullptr);

    LOG_IF_OPENXR_ERROR_AND_RETURN(GetPixelsAffectedByFeathering, xrEnumerateViewConfigurationViews, Result);

    if (ViewCountOutput == 0) {
        return false;
    }

    TArray<XrViewConfigurationView> Views;
    Views.Init({ .type = XR_TYPE_VIEW_CONFIGURATION_VIEW }, ViewCountOutput);

    TArray<XrViewportFeatheringConfigViewANDROID> FeatheringConfigViews;
    FeatheringConfigViews.Init({ .type = XR_TYPE_VIEWPORT_FEATHERING_CONFIG_VIEW_ANDROID }, ViewCountOutput);

    for (uint32_t i = 0; i < ViewCountOutput; i++) {
        Views[i].next = &FeatheringConfigViews[i];
    }

    Result = xrEnumerateViewConfigurationViews(Instance,
        IOpenXRHMDModule::Get().GetSystemId(),
        XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
        Views.Num(),
        &ViewCountOutput,
        Views.GetData());

    LOG_IF_OPENXR_ERROR_AND_RETURN(GetPixelsAffectedByFeathering, xrEnumerateViewConfigurationViews, Result);

    FeatheringViews.Reset(FeatheringConfigViews.Num());

    Algo::Transform(FeatheringConfigViews, FeatheringViews, [](XrViewportFeatheringConfigViewANDROID Item)
    {
        return FAndroidXRViewportFeatheringInsetInfo
        {
            .InsetLeft = static_cast<int32>(Item.insetLeft),
            .InsetRight = static_cast<int32>(Item.insetRight),
            .InsetTop = static_cast<int32>(Item.insetTop),
            .InsetBottom = static_cast<int32>(Item.insetBottom)
        };
    });

    return true;
}

bool FAndroidXRViewportFeathering::IsViewportFeatheringSupported() const
{
    RETURN_IF_FUNCTION_NOT_FOUND(xrEnumerateInstanceExtensionProperties)
    uint32_t ExtensionCount{};
    XrResult Result = xrEnumerateInstanceExtensionProperties(nullptr, 0, &ExtensionCount, nullptr);

    LOG_IF_OPENXR_ERROR_AND_RETURN(GetViewportFeatheringSupported, xrEnumerateInstanceExtensionProperties, Result);

    if (ExtensionCount > 0)
    {
        TArray<XrExtensionProperties> Extensions;
        Extensions.Init({ .type = XR_TYPE_EXTENSION_PROPERTIES }, ExtensionCount);

        Result = xrEnumerateInstanceExtensionProperties(nullptr, ExtensionCount, &ExtensionCount, Extensions.GetData());

        LOG_IF_OPENXR_ERROR_AND_RETURN(GetViewportFeatheringSupported, xrEnumerateInstanceExtensionProperties, Result);

        return Extensions.ContainsByPredicate([](const XrExtensionProperties& Extension)
        {
            return (strcmp(Extension.extensionName, XR_ANDROID_VIEWPORT_FEATHERING_EXTENSION_NAME) == 0);
        });
    }

    return false;
}

IMPLEMENT_MODULE(FAndroidXRViewportFeathering, AndroidXRViewportFeathering)