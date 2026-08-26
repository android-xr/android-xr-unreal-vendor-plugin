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
#include "DynamicResolutionState.h"
#include "AndroidXRRecommendedResolutionDynamicResolutionState.h"
#include "IAndroidXRRecommendedResolutionModule.h"
#include "IOpenXRExtensionPlugin.h"

#include <AndroidXRHelpers.h>

DECLARE_LOG_CATEGORY_EXTERN(LogAndroidXRRecommendedResolution, Log, All);

class FAndroidXRRecommendedResolution : public IAndroidXRRecommendedResolutionModule,
                                 public IOpenXRExtensionPlugin
{
public:
    FAndroidXRRecommendedResolution() = default;

    /** IModuleInterface */
    void StartupModule() override;
    void ShutdownModule() override;

    /** IOpenXRExtensionPlugin */
    FString GetDisplayName() override
    {
        return FString(TEXT("AndroidXRRecommendedResolution"));
    }

    void PostCreateInstance(XrInstance InInstance) override;
    void PostCreateSession(XrSession InSession) override;
    bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
    void OnEvent(XrSession InSession, const XrEventDataBaseHeader* InHeader) override;
    UAndroidXRRecommendedResolutionEventProxy* GetEventProxy() override
    {
        return UAndroidXRRecommendedResolutionEventProxy::GetInstance();
    }

private:
    XrInstance Instance{};
    XrSession Session{};
    bool DynamicResolutionStateReady = false;
};
