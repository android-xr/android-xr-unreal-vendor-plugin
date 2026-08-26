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
#include "IAndroidXRLightEstimationModule.h"
#include "IOpenXRExtensionPlugin.h"
#include <AndroidXRHelpers.h>
#include "AndroidXRCubeBuffer.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAndroidXRLightEstimation, Log, All);

class FAndroidXRLightEstimation : public IAndroidXRLightEstimationModule,
    public IOpenXRExtensionPlugin
{
public:
    FAndroidXRLightEstimation();

    /** IModuleInterface */
    void StartupModule() override;
    void ShutdownModule() override;

    /** IOpenXRExtensionPlugin */
    FString GetDisplayName() override
    {
        return FString(TEXT("AndroidXRLightEstimation"));
    }

    void PostCreateInstance(XrInstance InInstance) override;
    void PostCreateSession(XrSession InSession) override;
    bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;

    bool IsLightEstimationSupported() const override;
    bool CreateLightEstimator(FAndroidXRLightEstimator& LightEstimator) const override;
    bool DestroyLightEstimator(FAndroidXRLightEstimator& LightEstimator) const override;

    bool GetDirectionalLightData(const FAndroidXRLightEstimator& LightEstimator, FAndroidXRDirectionalLightData& DirectionalLightData) override;
    bool GetAmbientLightData(const FAndroidXRLightEstimator& LightEstimator, FAndroidXRAmbientLightData& AmbientLightData) override;
    bool GetSphericalHarmonicsData(const FAndroidXRLightEstimator& LightEstimator, EAndroidXRSphericalHarmonicsKind HarmonicsKind, FAndroidXRSphericalHarmonicsData& SphericalHarmonicsData) override;

    bool IsCubemapLightEstimationSupported() const override;
    bool EnumerateSupportedCubemapFormats(TArray<EAndroidXRCubemapColorFormat>& Formats) const override;
    bool EnumerateSupportedCubemapResolutions(TArray<int>& Resolutions) const override;
    bool CreateCubemapLightEstimator(FAndroidXRCubemapConfiguration& Config, FAndroidXRCubemapEstimator& CubemapEstimator) const override;
    bool DestroyCubemapEstimator(FAndroidXRCubemapEstimator& CubemapEstimator) const override;
    bool GetCubemapData(const FAndroidXRCubemapEstimator& CubemapEstimator, FAndroidXRCubemapLightData& CubemapData) override;
    bool CreateCubemapTextureForLightEstimation(const FAndroidXRCubemapEstimator& CubemapEstimator, UTextureCube*& TextureCube) const override;
    bool ApplyLightEstimationCubemap(const FAndroidXRCubemapLightData& CubemapData, UTextureCube* TextureCube) const override;

private:
    XrInstance Instance{};
    XrSession Session{};

    FAndroidXRCubeBuffer CubeBuffer{};

    XrResult GetLightEstimateData(const FAndroidXRLightEstimator& LightEstimator, void* LightEstimateDataToAttach);

    XrLightEstimateGetInfoANDROID LightEstimateGetInfo{ XR_TYPE_LIGHT_ESTIMATE_GET_INFO_ANDROID };
    XrLightEstimateANDROID LightEstimate{ XR_TYPE_LIGHT_ESTIMATE_ANDROID };

#define LIGHTESTIMATION_FUNCTIONS(HelperMacro) \
        HelperMacro(xrCreateLightEstimatorANDROID) \
        HelperMacro(xrDestroyLightEstimatorANDROID) \
        HelperMacro(xrGetLightEstimateANDROID) \
        HelperMacro(xrEnumerateCubemapLightingResolutionsANDROID) \
        HelperMacro(xrEnumerateCubemapLightingColorFormatsANDROID)
    LIGHTESTIMATION_FUNCTIONS(DECLARE_OPENXR_FUNC);
};
