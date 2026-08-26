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

#include "AndroidXRGlobalPassthroughDimmingEventProxy.h"
#include "CoreMinimal.h"
#include "IAndroidXRGlobalPassthroughDimmingModule.h"
#include "IOpenXRExtensionPlugin.h"
#include <AndroidXRHelpers.h>

DECLARE_LOG_CATEGORY_EXTERN(LogAndroidXRGlobalPassthroughDimming, Log, All);

class FAndroidXRGlobalPassthroughDimming : public IAndroidXRGlobalPassthroughDimmingModule,
                                 public IOpenXRExtensionPlugin
{
public:
    FAndroidXRGlobalPassthroughDimming();

    /** IModuleInterface */
    void StartupModule() override;
    void ShutdownModule() override;

    /** IOpenXRExtensionPlugin */
    FString GetDisplayName() override
    {
        return FString(TEXT("AndroidXRGlobalPassthroughDimming"));
    }

    void PostCreateInstance(XrInstance InInstance) override;
    void PostCreateSession(XrSession InSession) override;
    bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
    void OnEvent(XrSession InSession, const XrEventDataBaseHeader* InHeader) override;
    const void* OnEndFrame(XrSession InSession, XrTime DisplayTime, const void* InNext) override;

    /** IAndroidXRGlobalPassthroughDimming */
    bool GetGlobalDimmingSupported() const override;
    bool EnumerateSupportedGlobalDimmingLevels(bool& AllLevelsSupported, TArray<float>& SupportedGlobalDimmingLevels) override;
    bool SetGlobalDimmingLevel(float PreferredDimmingLevel) override;
    bool GetGlobalDimmingLevel(float& CurrentDimmingLevel) const override;
    UAndroidXRGlobalPassthroughDimmingEventProxy* GetEventProxy() override
    {
        return UAndroidXRGlobalPassthroughDimmingEventProxy::GetInstance();
    }

private:
    XrInstance Instance{};
    XrSession Session{};
    bool bLevelsHaveBeenEnumerated{ false };
    TArray<float> CachedSupportedGlobalDimmingLevels{};
    std::atomic<float> AtomicDimmingLevel{ 0.0f };
    XrGlobalDimmingFrameEndInfoANDROID GlobalDimmingFrameEndInfo
    {
        .type = XR_TYPE_GLOBAL_DIMMING_FRAME_END_INFO_ANDROID,
        // -1.0 to indicate that the current dimming level should be kept.
        .globalDimmingLevel = -1.0f
    };

#define GLOBALPASSTHROUGHDIMMING_FUNCTIONS(HelperMacro) \
        HelperMacro(xrGetGlobalDimmingLevelANDROID) \
        HelperMacro(xrEnumerateSupportedGlobalDimmingLevelsANDROID) \
        HelperMacro(xrGetSystemProperties)
    GLOBALPASSTHROUGHDIMMING_FUNCTIONS(DECLARE_OPENXR_FUNC);
};