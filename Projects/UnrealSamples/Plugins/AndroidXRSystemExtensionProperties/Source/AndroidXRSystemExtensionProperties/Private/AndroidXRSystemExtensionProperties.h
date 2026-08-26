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

#include "AndroidXRSystemExtensionPropertiesEventProxy.h"
#include "CoreMinimal.h"
#include "IAndroidXRSystemExtensionPropertiesModule.h"
#include "IOpenXRExtensionPlugin.h"
#include <AndroidXRHelpers.h>

DECLARE_LOG_CATEGORY_EXTERN(LogAndroidXRSystemExtensionProperties, Log, All);

class FAndroidXRSystemExtensionProperties : public IAndroidXRSystemExtensionPropertiesModule, public IOpenXRExtensionPlugin
{
public:
    FAndroidXRSystemExtensionProperties();

    /** IModuleInterface */
    void StartupModule() override;
    void ShutdownModule() override;

    /** IOpenXRExtensionPlugin */
    FString GetDisplayName() override
    {
        return FString(TEXT("AndroidXRSystemExtensionProperties"));
    }

    void PostCreateInstance(XrInstance InInstance) override;
    void PostCreateSession(XrSession InSession) override;
    void PostGetSystem(XrInstance InInstance, XrSystemId InSystem) override;
    bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
    void OnEvent(XrSession InSession, const XrEventDataBaseHeader* InHeader) override;

    /** IAndroidXRSystemExtensionPropertiesModule */
    bool EnumerateSystemExtensionProperties(TArray<FAndroidXRSystemExtensionInformation>& ExtensionProperties) override;
    UAndroidXRSystemExtensionPropertiesEventProxy* GetEventProxy() override
    {
        return UAndroidXRSystemExtensionPropertiesEventProxy::GetInstance();
    }

private:
    XrInstance Instance{};
    XrSession Session{};
    XrSystemId SystemId{};
    TArray<FAndroidXRSystemExtensionInformation> CachedExtensionProperties{};

    bool InternalEnumerateSystemExtensionProperties();

    #define SYSTEMEXTENSIONPROPERTIES_FUNCTIONS(HelperMacro) \
        HelperMacro(xrEnumerateSystemExtensionPropertiesANDROID)

    SYSTEMEXTENSIONPROPERTIES_FUNCTIONS(DECLARE_OPENXR_FUNC);
};