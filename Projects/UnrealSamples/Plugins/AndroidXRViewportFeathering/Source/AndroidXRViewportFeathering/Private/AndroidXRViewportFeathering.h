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

#include "CoreMinimal.h"
#include "IAndroidXRViewportFeatheringModule.h"
#include "IOpenXRExtensionPlugin.h"
#include "AndroidXRViewportFeatheringTypes.h"
#include <AndroidXRHelpers.h>

DECLARE_LOG_CATEGORY_EXTERN(LogAndroidXRViewportFeathering, Log, All);

class FAndroidXRViewportFeathering : public IAndroidXRViewportFeatheringModule,
                                 public IOpenXRExtensionPlugin
{
public:
    FAndroidXRViewportFeathering();

    /** IModuleInterface */
    void StartupModule() override;
    void ShutdownModule() override;

    /** IOpenXRExtensionPlugin */
    FString GetDisplayName() override
    {
        return FString(TEXT("AndroidXRViewportFeathering"));
    }

    void PostCreateInstance(XrInstance InInstance) override;
    void PostCreateSession(XrSession InSession) override;
    bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;

    /** IAndroidXRViewportFeatheringModule */
    bool GetPixelsAffectedByFeathering(TArray<FAndroidXRViewportFeatheringInsetInfo>& FeatheringViews) const override;
    bool IsViewportFeatheringSupported() const override;

private:
    XrInstance Instance{};
    XrSession Session{};

    #define VIEWPORTFEATHERING_FUNCTIONS(HelperMacro) \
        HelperMacro(xrEnumerateViewConfigurationViews) \
        HelperMacro(xrEnumerateInstanceExtensionProperties)
    VIEWPORTFEATHERING_FUNCTIONS(DECLARE_OPENXR_FUNC);
};