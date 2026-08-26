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
#include <AndroidXRHelpers.h>

class FAndroidXRRecommendedResolutionDynamicResolutionState : public IDynamicResolutionState
{
public:
    bool IsSupported() const override;
    void ResetHistory() override {}

    void SetupMainViewFamily(class FSceneViewFamily& ViewFamily) override;
    void SetTemporalUpscaler(const UE::Renderer::Private::ITemporalUpscaler* InTemporalUpscaler) override {}

    void UpdateResolutionRecommendation(float InRecommendedWidth, float InRecommendedHeight);

    static TSharedPtr<FAndroidXRRecommendedResolutionDynamicResolutionState>& GetInstance();

protected:
    // Core logic: return the resolution fraction (0.0 to 1.0)
    DynamicRenderScaling::TMap<float> GetResolutionFractionsApproximation() const override { return CurrentFraction; }
    DynamicRenderScaling::TMap<float> GetResolutionFractionsUpperBound() const override { return 1.0f; }

    void SetEnabled(bool bEnable) override { bIsEnabled = bEnable;}
    bool IsEnabled() const override { return bIsEnabled; }

    void ProcessEvent(EDynamicResolutionStateEvent Event) override {};

private:
    static inline TSharedPtr<FAndroidXRRecommendedResolutionDynamicResolutionState> Instance{};
    bool bIsEnabled{};
    float CurrentFraction{ 1.0f };
    float RecommendedWidth{ 1.0f };
    float RecommendedHeight{ 1.0f };
};