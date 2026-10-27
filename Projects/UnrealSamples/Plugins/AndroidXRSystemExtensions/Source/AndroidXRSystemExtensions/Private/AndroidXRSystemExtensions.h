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

#include "AndroidXRSystemExtensionsEventProxy.h"
#include "CoreMinimal.h"
#include "IAndroidXRSystemExtensionsModule.h"
#include "IOpenXRExtensionPlugin.h"
#include <AndroidXRHelpers.h>

DECLARE_LOG_CATEGORY_EXTERN(LogAndroidXRSystemExtensions, Log, All);

class FAndroidXRSystemExtensions : public IAndroidXRSystemExtensionsModule, public IOpenXRExtensionPlugin
{
public:
    FAndroidXRSystemExtensions();

    /** IModuleInterface */
    void StartupModule() override;
    void ShutdownModule() override;

    /** IOpenXRExtensionPlugin */
    FString GetDisplayName() override
    {
        return FString(TEXT("AndroidXRSystemExtensions"));
    }

    void PostCreateInstance(XrInstance InInstance) override;
    void PostCreateSession(XrSession InSession) override;
    void PostGetSystem(XrInstance InInstance, XrSystemId InSystem) override;
    bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
    void OnEvent(XrSession InSession, const XrEventDataBaseHeader* InHeader) override;

    /** IAndroidXRSystemExtensionsModule */
    bool EnumerateSystemExtensions(TArray<FAndroidXRSystemExtensionInformation>& Extensions) override;
    UAndroidXRSystemExtensionsEventProxy* GetEventProxy() override
    {
        return UAndroidXRSystemExtensionsEventProxy::GetInstance();
    }

private:
    XrInstance Instance{};
    XrSession Session{};
    XrSystemId SystemId{};
    TArray<FAndroidXRSystemExtensionInformation> CachedExtensions{};

    bool InternalEnumerateSystemExtensions();

    #define SYSTEMEXTENSIONS_FUNCTIONS(HelperMacro) \
        HelperMacro(xrEnumerateSystemExtensionPropertiesANDROID)

    SYSTEMEXTENSIONS_FUNCTIONS(DECLARE_OPENXR_FUNC);
};