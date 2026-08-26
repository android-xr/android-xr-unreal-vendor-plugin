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
#include "Modules/ModuleManager.h"
#include "AndroidXRLightEstimationTypes.h"

/**
* The public interface to this module.  In most cases, this interface is only
* public to sibling modules within this plugin.
*/
class ANDROIDXRLIGHTESTIMATION_API IAndroidXRLightEstimationModule : public IModuleInterface
{
public:
    /**
     * Singleton-like access to this module's interface.  This is just for
     * convenience! Beware of calling this during the shutdown phase, though. Your
     * module might have been unloaded already.
     *
     * @return Returns singleton instance, loading the module on demand if needed
     */
    static inline IAndroidXRLightEstimationModule& Get()
    {
        return FModuleManager::LoadModuleChecked<IAndroidXRLightEstimationModule>("AndroidXRLightEstimation");
    }

    /**
     * Checks to see if this module is loaded and ready.  It is only valid to call
     * Get() if IsAvailable() returns true.
     *
     * @return True if the module is loaded and ready to use
     */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("AndroidXRLightEstimation");
    }

    /**
    * Returns whether or not light estimation is supported on the device.
    * @return True if light estimation is supported.
    */
    virtual bool IsLightEstimationSupported() const= 0;

    /**
    * Creates a light estimator.
    * @param[out] The created light estimator.
    * @return True if the underlying API call was successful.
    */
    virtual bool CreateLightEstimator(FAndroidXRLightEstimator& LightEstimator) const = 0;

    /**
    * Destroys a light estimator.
    * @param[in/out] The light estimator to destroy.
    * @return True if the underlying API call was successful.
    */
    virtual bool DestroyLightEstimator(FAndroidXRLightEstimator& LightEstimator) const = 0;

    /**
    * Gets the directional light data.
    * @param[in] The light estimator to get the directional light data.
    * @param[out] The directional light data.
    * @return True if the underlying API call was successful.
    */
    virtual bool GetDirectionalLightData(const FAndroidXRLightEstimator& LightEstimator, FAndroidXRDirectionalLightData& DirectionalLightData) = 0;
    /**
    * Gets the ambient light data.
    * @param[in] The light estimator to get the ambient light data.
    * @param[out] The ambient light data.
    * @return True if the underlying API call was successful.
    */
    virtual bool GetAmbientLightData(const FAndroidXRLightEstimator& LightEstimator, FAndroidXRAmbientLightData& AmbientLightData) = 0;

    /**
    * Gets the spherical harmonics data.
    * @param[in] The light estimator to get the spherical harmonics data.
    * @param[out] The spherical harmonics data.
    * @return True if the underlying API call was successful.
    */
    virtual bool GetSphericalHarmonicsData(const FAndroidXRLightEstimator& LightEstimator, EAndroidXRSphericalHarmonicsKind HarmonicsKind, FAndroidXRSphericalHarmonicsData& SphericalHarmonicsData) = 0;

    /**
    * Returns whether light estimation cubemap is supported for the device.
    * @return True if device supports light estimation cubemap.
    */
    virtual bool IsCubemapLightEstimationSupported() const = 0;

    /**
    * Gets the supported cubemap formats.
    * @param[out] The supported cubemap formats.
    * @return True if the underlying API call was successful.
    */
    virtual bool EnumerateSupportedCubemapFormats(TArray<EAndroidXRCubemapColorFormat>& Formats) const = 0;

    /**
    * Gets the supported cubemap resolutions.
    * @param[out] The supported cubemap resolutions.
    * @return True if the underlying API call was successful.
    */
    virtual bool EnumerateSupportedCubemapResolutions(TArray<int>& Resolutions) const = 0;

    /**
    * Creates a cubemap light estimator.
    * @param[in] The configuration for creating the cubemap estimator.
    * @param[out] The created cubemap light estimator.
    * @return True if the underlying API call was successful.
    */
    virtual bool CreateCubemapLightEstimator(FAndroidXRCubemapConfiguration& Config, FAndroidXRCubemapEstimator& CubemapEstimator) const = 0;

    /**
    * Gets the cubemap data.
    * @param[in] The cubemap estimator.
    * @param[out] The cubemap light data.
    * @return True if the underlying API call was successful.
    */
    virtual bool GetCubemapData(const FAndroidXRCubemapEstimator& CubemapEstimator, FAndroidXRCubemapLightData& CubemapData) = 0;

    /**
    * Destroys a cubemap estimator
    * @param[in/out] The cubemap estimator to destroy.
    * @return True if the underlying API call was successful.
    */
    virtual bool DestroyCubemapEstimator(FAndroidXRCubemapEstimator& CubemapEstimator) const = 0;

    /**
    * Creates a cubemap texture for a cubemap light estimator.
    * @param[in] The cubemap estimator for which to create the cubemap texture.
    * @param[in] The texture cube for which to create the cubemap textures.
    * @return True if the underlying API call was successful.
    */
    virtual bool CreateCubemapTextureForLightEstimation(const FAndroidXRCubemapEstimator& CubemapEstimator, UTextureCube*& TextureCube) const = 0;

    /**
    * Applies a cubemap data to a texture cube.
    * @param[in] The cubemap data to apply.
    * @param[in] The texture to update with the cubemap data.
    * @return True if the underlying API call was successful.
    */
    virtual bool ApplyLightEstimationCubemap(const FAndroidXRCubemapLightData& CubemapData, UTextureCube* TextureCube) const = 0;
};
