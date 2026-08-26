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

#include "AndroidXRLightEstimationBlueprintFunctionLibrary.h"
#define RETURN_IF_MODULE_NOT_FOUND() if(!IAndroidXRLightEstimationModule::IsAvailable()){ return false; }

bool UAndroidXRLightEstimationBlueprintFunctionLibrary::IsLightEstimationSupported()
{
    RETURN_IF_MODULE_NOT_FOUND();
    return IAndroidXRLightEstimationModule::Get().IsLightEstimationSupported();
}

bool UAndroidXRLightEstimationBlueprintFunctionLibrary::CreateLightEstimator(FAndroidXRLightEstimator& LightEstimator)
{
    RETURN_IF_MODULE_NOT_FOUND();
    return IAndroidXRLightEstimationModule::Get().CreateLightEstimator(LightEstimator);
}

bool UAndroidXRLightEstimationBlueprintFunctionLibrary::DestroyLightEstimator(FAndroidXRLightEstimator& LightEstimator)
{
    RETURN_IF_MODULE_NOT_FOUND();
    return IAndroidXRLightEstimationModule::Get().DestroyLightEstimator(LightEstimator);
}

bool UAndroidXRLightEstimationBlueprintFunctionLibrary::GetDirectionalLightData(const FAndroidXRLightEstimator& LightEstimator, FAndroidXRDirectionalLightData& DirectionalLightData)
{
    RETURN_IF_MODULE_NOT_FOUND();
    return IAndroidXRLightEstimationModule::Get().GetDirectionalLightData(LightEstimator, DirectionalLightData);
}

bool UAndroidXRLightEstimationBlueprintFunctionLibrary::GetAmbientLightData(const FAndroidXRLightEstimator& LightEstimator, FAndroidXRAmbientLightData& AmbientLightData)
{
    RETURN_IF_MODULE_NOT_FOUND();
    return IAndroidXRLightEstimationModule::Get().GetAmbientLightData(LightEstimator, AmbientLightData);
}

bool UAndroidXRLightEstimationBlueprintFunctionLibrary::GetSphericalHarmonicsData(const FAndroidXRLightEstimator& LightEstimator, EAndroidXRSphericalHarmonicsKind HarmonicsKind, FAndroidXRSphericalHarmonicsData& SphericalHarmonicsData)
{
    RETURN_IF_MODULE_NOT_FOUND();
    return IAndroidXRLightEstimationModule::Get().GetSphericalHarmonicsData(LightEstimator, HarmonicsKind, SphericalHarmonicsData);
}

bool UAndroidXRLightEstimationBlueprintFunctionLibrary::IsCubemapLightEstimationSupported()
{
    RETURN_IF_MODULE_NOT_FOUND();
    return IAndroidXRLightEstimationModule::Get().IsCubemapLightEstimationSupported();
}

bool UAndroidXRLightEstimationBlueprintFunctionLibrary::EnumerateSupportedCubemapFormats(TArray<EAndroidXRCubemapColorFormat>& Formats)
{
    RETURN_IF_MODULE_NOT_FOUND();
    return IAndroidXRLightEstimationModule::Get().EnumerateSupportedCubemapFormats(Formats);
}

bool UAndroidXRLightEstimationBlueprintFunctionLibrary::EnumerateSupportedCubemapResolutions(TArray<int>& Resolutions)
{
    RETURN_IF_MODULE_NOT_FOUND();
    return IAndroidXRLightEstimationModule::Get().EnumerateSupportedCubemapResolutions(Resolutions);
}

bool UAndroidXRLightEstimationBlueprintFunctionLibrary::CreateCubemapLightEstimator(FAndroidXRCubemapConfiguration& Config, FAndroidXRCubemapEstimator& CubemapEstimator)
{
    RETURN_IF_MODULE_NOT_FOUND();
    return IAndroidXRLightEstimationModule::Get().CreateCubemapLightEstimator(Config, CubemapEstimator);
}

bool UAndroidXRLightEstimationBlueprintFunctionLibrary::DestroyCubemapEstimator(FAndroidXRCubemapEstimator& CubemapEstimator)
{
    RETURN_IF_MODULE_NOT_FOUND();
    return IAndroidXRLightEstimationModule::Get().DestroyCubemapEstimator(CubemapEstimator);
}

bool UAndroidXRLightEstimationBlueprintFunctionLibrary::GetCubemapData(const FAndroidXRCubemapEstimator& CubemapEstimator, FAndroidXRCubemapLightData& CubemapData)
{
    RETURN_IF_MODULE_NOT_FOUND();
    return IAndroidXRLightEstimationModule::Get().GetCubemapData(CubemapEstimator, CubemapData);
}

bool UAndroidXRLightEstimationBlueprintFunctionLibrary::CreateCubemapTextureForLightEstimation(const FAndroidXRCubemapEstimator& CubemapEstimator, UTextureCube*& TextureCube)
{
    RETURN_IF_MODULE_NOT_FOUND();
    return IAndroidXRLightEstimationModule::Get().CreateCubemapTextureForLightEstimation(CubemapEstimator, TextureCube);
}

bool UAndroidXRLightEstimationBlueprintFunctionLibrary::ApplyLightEstimationCubemap(const FAndroidXRCubemapLightData& CubemapData, UTextureCube* TextureCube)
{
    RETURN_IF_MODULE_NOT_FOUND();
    return IAndroidXRLightEstimationModule::Get().ApplyLightEstimationCubemap(CubemapData, TextureCube);
}