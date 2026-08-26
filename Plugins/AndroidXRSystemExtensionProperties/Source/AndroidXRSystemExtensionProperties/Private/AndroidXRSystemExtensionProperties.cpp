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

#include "AndroidXRSystemExtensionProperties.h"
#include "AndroidXRSystemExtensionPropertiesBlueprintFunctionLibrary.h"
#include "AndroidXRSystemExtensionPropertiesEventProxy.h"
#include "AndroidXRTypeConversions.h"
#include <IOpenXRHMDModule.h>
#include <IXRTrackingSystem.h>
#include <IHeadMountedDisplay.h>
#include "IOpenXRHMD.h"
#include <vector>

DEFINE_LOG_CATEGORY(LogAndroidXRSystemExtensionProperties);
#define RETURN_IF_FUNCTION_NOT_FOUND(Function) if(!Function){ UE_LOG(LogAndroidXRSystemExtensionProperties, Error, TEXT("Unable to locate function %s"), TEXT(#Function)); return false;}

FAndroidXRSystemExtensionProperties::FAndroidXRSystemExtensionProperties()
{
}

void FAndroidXRSystemExtensionProperties::StartupModule()
{
    IAndroidXRSystemExtensionPropertiesModule::StartupModule();
    RegisterOpenXRExtensionModularFeature();
}

void FAndroidXRSystemExtensionProperties::ShutdownModule()
{
    UnregisterOpenXRExtensionModularFeature();
    IAndroidXRSystemExtensionPropertiesModule::ShutdownModule();
}

bool FAndroidXRSystemExtensionProperties::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
    OutExtensions.Add(XR_ANDROID_ENUMERATE_SYSTEM_EXTENSION_PROPERTIES_EXTENSION_NAME);
    return true;
}

void FAndroidXRSystemExtensionProperties::PostCreateInstance(XrInstance InInstance)
{
    Instance = InInstance;

    ResolveOpenXRFunctions(Instance,
        {
            SYSTEMEXTENSIONPROPERTIES_FUNCTIONS(RESOLVE_OPENXR_FUNC)
        },
        [](const char* UnresolvableFunctionName, XrResult Result)
        {
            UE_LOG(LogAndroidXRSystemExtensionProperties, Error,
                TEXT("Unable to resolve function pointer %s (error %s)"),
                UTF8_TO_TCHAR(UnresolvableFunctionName),
                OpenXRResultToString(Result));
        });
}

void FAndroidXRSystemExtensionProperties::PostCreateSession(XrSession InSession)
{
    Session = InSession;
    InternalEnumerateSystemExtensionProperties();
}

void FAndroidXRSystemExtensionProperties::PostGetSystem(XrInstance InInstance, XrSystemId InSystem)
{
    SystemId = InSystem;
}

void FAndroidXRSystemExtensionProperties::OnEvent(XrSession InSession, const XrEventDataBaseHeader* InHeader)
{
    if (InHeader->type == XR_TYPE_EVENT_DATA_SYSTEM_PROPERTIES_CHANGED_ANDROID)
    {
        if (InternalEnumerateSystemExtensionProperties())
        {
            UAndroidXRSystemExtensionPropertiesBlueprintFunctionLibrary::GetEventProxy()->
                OnSystemExtensionPropertiesChanged.Broadcast(CachedExtensionProperties);
        }
    }
}
bool FAndroidXRSystemExtensionProperties::EnumerateSystemExtensionProperties(TArray<FAndroidXRSystemExtensionInformation>& ExtensionProperties)
{
    ExtensionProperties.Reset();

    if (CachedExtensionProperties.IsEmpty())
    {
        if (!InternalEnumerateSystemExtensionProperties())
        {
            return false;
        }
    }

    ExtensionProperties = CachedExtensionProperties;
    return true;
}

bool FAndroidXRSystemExtensionProperties::InternalEnumerateSystemExtensionProperties()
{
    RETURN_IF_FUNCTION_NOT_FOUND(xrEnumerateSystemExtensionPropertiesANDROID);

    CachedExtensionProperties.Reset();

    // Enumerate the extensions to see which ones are now supported based on hardware changes
    uint32_t ExtensionsCount;
    xrEnumerateSystemExtensionPropertiesANDROID(Instance, SystemId, 0, &ExtensionsCount, NULL);

    TArray<XrSystemExtensionPropertiesANDROID> Properties;
    Properties.Init({ .type = XR_TYPE_SYSTEM_EXTENSION_PROPERTIES_ANDROID }, ExtensionsCount);

    XrResult Result = xrEnumerateSystemExtensionPropertiesANDROID(
        Instance,
        SystemId,
        ExtensionsCount,
        &ExtensionsCount,
        Properties.GetData()
    );
    if (!XR_UNQUALIFIED_SUCCESS(Result))
    {
        UE_LOG(LogAndroidXRSystemExtensionProperties, Error,
            TEXT("EnumerateSystemExtensionProperties: xrEnumerateSystemExtensionPropertiesANDROID failed with error %s"),
            OpenXRResultToString(Result));
        return false;
    }

    Algo::Transform(Properties, CachedExtensionProperties, [](const XrSystemExtensionPropertiesANDROID Extension)
        {
            return FAndroidXRSystemExtensionInformation{
            .ExtensionName = Extension.properties.extensionName,
            .ExtensionVersion = static_cast<int32>(Extension.properties.extensionVersion),
            .bIsSupported = !!Extension.isSupported
            };
        });

    return true;
}

IMPLEMENT_MODULE(FAndroidXRSystemExtensionProperties, AndroidXRSystemExtensionProperties)