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

#include "AndroidXRRecommendedResolutionDynamicResolutionState.h"
#include <AndroidXRRecommendedResolutionTypes.h>
#include <AndroidXRTypeConversions.h>
#include <LegacyScreenPercentageDriver.h>
#include "OpenXRCore.h"
#include "Math/UnrealMathUtility.h"
#include "Modules/ModuleManager.h"

bool FAndroidXRRecommendedResolutionDynamicResolutionState::IsSupported() const
{
    return FModuleManager::Get().IsModuleLoaded(TEXT("AndroidXRRecommendedResolution"));
}

void FAndroidXRRecommendedResolutionDynamicResolutionState::SetupMainViewFamily(class FSceneViewFamily& ViewFamily)
{
    if (ViewFamily.RenderTarget)
    {
        FIntPoint ViewportSize = ViewFamily.RenderTarget->GetSizeXY();
        float CurrentHeight = ViewportSize.Y;
        float CurrentWidth = ViewportSize.X;

        float HeightFraction = RecommendedHeight / CurrentHeight;
        float Widthfraction = RecommendedWidth / CurrentWidth;
        float LowestFramesFraction = FMath::Min(HeightFraction, Widthfraction);
        LowestFramesFraction = FMath::Clamp(LowestFramesFraction, 0.0f, 1.0f);

        if (!FMath::IsNearlyEqual(LowestFramesFraction, CurrentFraction, 0.001f))
        {
            CurrentFraction = LowestFramesFraction;

            ViewFamily.SetScreenPercentageInterface(new FLegacyScreenPercentageDriver(ViewFamily, CurrentFraction, 1.0f));
        }
    }
}

TSharedPtr<FAndroidXRRecommendedResolutionDynamicResolutionState>& FAndroidXRRecommendedResolutionDynamicResolutionState::GetInstance()
{
    if (!Instance.IsValid())
    {
        Instance = MakeShared<FAndroidXRRecommendedResolutionDynamicResolutionState>();
    }
    return Instance;
}

void FAndroidXRRecommendedResolutionDynamicResolutionState::UpdateResolutionRecommendation(float InRecommendedWidth, float InRecommendedHeight)
{
    RecommendedHeight = InRecommendedHeight;
    RecommendedWidth = InRecommendedWidth;
}
