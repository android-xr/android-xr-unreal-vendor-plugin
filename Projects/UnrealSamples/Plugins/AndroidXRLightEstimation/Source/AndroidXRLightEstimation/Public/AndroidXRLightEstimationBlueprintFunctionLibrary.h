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

#include "AndroidXRTypes.h"
#include "IAndroidXRLightEstimationModule.h"
#include "AndroidXRLightEstimationBlueprintFunctionLibrary.generated.h"

UCLASS(ClassGroup = AndroidXRLightEstimation)
class ANDROIDXRLIGHTESTIMATION_API UAndroidXRLightEstimationBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    /**
    * Returns whether or not light estimation is supported on the device.
    * @return True if light estimation is supported.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Light Estimation")
    static bool IsLightEstimationSupported();

    /**
    * Creates a light estimator.
    * @param[out] The created light estimator.
    * @return True if the underlying API call was successful.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Light Estimation")
    static bool CreateLightEstimator(FAndroidXRLightEstimator& LightEstimator);

    /**
    * Destroys a light estimator.
    * @param[in/out] The light estimator to destroy.
    * @return True if the underlying API call was successful.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Light Estimation")
    static bool DestroyLightEstimator(UPARAM(ref) FAndroidXRLightEstimator& LightEstimator);

    /**
    * Gets the directional light data.
    * @param[in] The light estimator to get the directional light data.
    * @param[out] The directional light data.
    * @return True if the underlying API call was successful.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Light Estimation")
    static bool GetDirectionalLightData(const FAndroidXRLightEstimator& LightEstimator, FAndroidXRDirectionalLightData& DirectionalLightData);

    /**
    * Gets the ambient light data.
    * @param[in] The light estimator to get the ambient light data.
    * @param[out] The ambient light data.
    * @return True if the underlying API call was successful.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Light Estimation")
    static bool GetAmbientLightData(const FAndroidXRLightEstimator& LightEstimator, FAndroidXRAmbientLightData& AmbientLightData);

    /**
    * Gets the spherical harmonics data.
    * @param[in] The light estimator to get the spherical harmonics data.
    * @param[out] The spherical harmonics data.
    * @return True if the underlying API call was successful.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Light Estimation")
    static bool GetSphericalHarmonicsData(const FAndroidXRLightEstimator& LightEstimator, EAndroidXRSphericalHarmonicsKind HarmonicsKind, FAndroidXRSphericalHarmonicsData& SphericalHarmonicsData);

    /**
    * Returns whether light estimation cubemap is supported for the device.
    * @return True if device supports light estimation cubemap.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Light Estimation")
    static bool IsCubemapLightEstimationSupported();

    /**
    * Gets the supported cubemap formats.
    * @param[out] The supported cubemap formats.
    * @return True if the underlying API call was successful.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Light Estimation")
    static bool EnumerateSupportedCubemapFormats(TArray<EAndroidXRCubemapColorFormat>& Formats);

    /**
    * Gets the supported cubemap resolutions.
    * @param[out] The supported cubemap resolutions.
    * @return True if the underlying API call was successful.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Light Estimation")
    static bool EnumerateSupportedCubemapResolutions(TArray<int>& Resolutions);

    /**
    * Creates a cubemap light estimator.
    * @param[in] The configuration for creating the cubemap estimator.
    * @param[out] The created cubemap light estimator.
    * @return True if the underlying API call was successful.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Light Estimation")
    static bool CreateCubemapLightEstimator(UPARAM(ref) FAndroidXRCubemapConfiguration& Config, FAndroidXRCubemapEstimator& CubemapEstimator);

    /**
    * Gets the cubemap data.
    * @param[in] The cubemap estimator.
    * @param[out] The cubemap light data.
    * @return True if the underlying API call was successful.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Light Estimation")
    static bool GetCubemapData(const FAndroidXRCubemapEstimator& CubemapEstimator, FAndroidXRCubemapLightData& CubemapData);

    /**
    * Destroys a cubemap estimator
    * @param[in/out] The cubemap estimator to destroy.
    * @return True if the underlying API call was successful.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Light Estimation")
    static bool DestroyCubemapEstimator(UPARAM(ref) FAndroidXRCubemapEstimator& CubemapEstimator);

    /**
    * Creates a cubemap texture for a cubemap light estimator.
    * @param[in] The cubemap estimator for which to create the cubemap texture.
    * @param[in] The texture cube for which to create the cubemap textures.
    * @return True if the underlying API call was successful.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Light Estimation")
    static bool CreateCubemapTextureForLightEstimation(const FAndroidXRCubemapEstimator& CubemapEstimator, UPARAM(ref) UTextureCube*& TextureCube);

    /**
    * Applies a cubemap data to a texture cube.
    * @param[in] The cubemap data to apply.
    * @param[in] The texture to update with the cubemap data.
    * @return True if the underlying API call was successful.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Light Estimation")
    static bool ApplyLightEstimationCubemap(const FAndroidXRCubemapLightData& CubemapData, UTextureCube* TextureCube);
};