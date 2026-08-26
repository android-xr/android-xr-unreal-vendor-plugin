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

#include "AndroidXRRecommendedResolution.h"
#include "AndroidXRRecommendedResolutionBlueprintFunctionLibrary.h"
#include "AndroidXRRecommendedResolutionTypes.h"
#include "AndroidXRRecommendedResolutionEventProxy.h"
#include <IOpenXRHMDModule.h>
#include <IXRTrackingSystem.h>
#include "IOpenXRHMD.h"

DEFINE_LOG_CATEGORY(LogAndroidXRRecommendedResolution);

void FAndroidXRRecommendedResolution::StartupModule()
{
    IAndroidXRRecommendedResolutionModule::StartupModule();
    RegisterOpenXRExtensionModularFeature();
}

void FAndroidXRRecommendedResolution::ShutdownModule()
{
    UnregisterOpenXRExtensionModularFeature();
    IAndroidXRRecommendedResolutionModule::ShutdownModule();
}

bool FAndroidXRRecommendedResolution::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
    OutExtensions.Add(XR_ANDROID_RECOMMENDED_RESOLUTION_EXTENSION_NAME);
    return true;
}

void FAndroidXRRecommendedResolution::PostCreateInstance(XrInstance InInstance)
{
    Instance = InInstance;
}

void FAndroidXRRecommendedResolution::PostCreateSession(XrSession InSession)
{
    Session = InSession;
}

void FAndroidXRRecommendedResolution::OnEvent(XrSession InSession, const XrEventDataBaseHeader* InHeader)
{
    if (InHeader->type == XR_TYPE_EVENT_DATA_RECOMMENDED_RESOLUTION_CHANGED_ANDROID)
    {
        uint32_t ViewCountOutput{};
        auto Result = xrEnumerateViewConfigurationViews(Instance,
            IOpenXRHMDModule::Get().GetSystemId(),
            XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
            0,
            &ViewCountOutput,
            nullptr);

        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            TArray<XrViewConfigurationView> Views;
            Views.Init({ .type = XR_TYPE_VIEW_CONFIGURATION_VIEW }, ViewCountOutput);

            Result = xrEnumerateViewConfigurationViews(Instance,
                IOpenXRHMDModule::Get().GetSystemId(),
                XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
                Views.Num(),
                &ViewCountOutput,
                Views.GetData());

            if (XR_UNQUALIFIED_SUCCESS(Result))
            {
                if (!DynamicResolutionStateReady && GEngine && GEngine->GetDynamicResolutionUserSetting())
                {
                    GEngine->ChangeDynamicResolutionStateAtNextFrame(FAndroidXRRecommendedResolutionDynamicResolutionState::GetInstance());
                    DynamicResolutionStateReady = true;
                }
                FVector2D RecommendedResolutionVector{ static_cast<double>(Views[0].recommendedImageRectWidth), static_cast<double>(Views[0].recommendedImageRectHeight) };
                FVector2D MaxResolutionVector{ static_cast<double>(Views[0].maxImageRectWidth), static_cast<double>(Views[0].maxImageRectHeight) };
                UAndroidXRRecommendedResolutionBlueprintFunctionLibrary::GetEventProxy()->
                OnRecommendedResolutionChanged.Broadcast(
                    RecommendedResolutionVector,
                    MaxResolutionVector
                );
                ENQUEUE_RENDER_COMMAND(UpdateRecommendedResolution)([RecommendedResolution = RecommendedResolutionVector](FRHICommandListImmediate& RHICmdList) {
                    if (FAndroidXRRecommendedResolutionDynamicResolutionState::GetInstance().IsValid())
                    {
                        FAndroidXRRecommendedResolutionDynamicResolutionState::GetInstance()->UpdateResolutionRecommendation(RecommendedResolution.X, RecommendedResolution.Y);
                    }
                });
            }
        }
        else {
            UE_LOG(LogAndroidXRRecommendedResolution, Error,
                TEXT("OnEvent: received RECOMMENDED_RESOLUTION, "
                    "xrEnumerateViewConfigurationViews failed with error %s"),
                OpenXRResultToString(Result));
        }
    }
}

IMPLEMENT_MODULE(FAndroidXRRecommendedResolution, AndroidXRRecommendedResolution);

