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
#include "AndroidXRTypes.h"
#include "Math/SHMath.h"
#include "AndroidXRLightEstimationTypes.generated.h"

 /**
 * The state of a light estimate data
 */
UENUM(BlueprintType)
enum class EAndroidXRLightEstimationState : uint8
{
    //The light estimate data is valid
    Valid,
    //The light estimate data is invalid
    Invalid
};

/**
* The kind of spherical harmonics
*/
UENUM(BlueprintType)
enum class EAndroidXRSphericalHarmonicsKind : uint8
{
    //The spherical harmonic coefficients represent the environment light radiance function, including the contribution from the main light.
    Total,
    //The spherical harmonic coefficients represent the environment light radiance function, excluding the contribution from the main light.
    Ambient
};

/**
* The color format of the cubemap light estimate data
*/
UENUM(BlueprintType)
enum class EAndroidXRCubemapColorFormat : uint8
{
    //Three 32 bit floats for each channel of R,G, and B.
    R32G32B32,
    //Four 32 bit floats for R,G,B, and A channels.
    R32G32B32A32,
    //Four usigned 16 bit ints for R,G,B, and A channels.
    R16G16B16A16
};

/**
* An AndroidXR light estimator
*/
USTRUCT(BlueprintType)
struct FAndroidXRLightEstimator
{
    GENERATED_BODY()

    XrLightEstimatorANDROID LightEstimator{};
};

/**
* An AndroidXR cubemap light estimator
*/
USTRUCT(BlueprintType)
struct FAndroidXRCubemapEstimator
{
    GENERATED_BODY()

    // The light estimator that this cubemap estimator is associated with.
    UPROPERTY(BlueprintReadOnly)
    FAndroidXRLightEstimator LightEstimator{};

    // The resolution of the cubemap for this estimator.
    UPROPERTY(BlueprintReadOnly)
    int CubemapResolution{};

    // The color format of the cubemap for this estimator.
    UPROPERTY(BlueprintReadOnly)
    EAndroidXRCubemapColorFormat ColorFormat{};
};

/**
* The directional light estimate data
*/
USTRUCT(BlueprintType)
struct FAndroidXRDirectionalLightData
{
    GENERATED_BODY()

    //The state of the directional light data
    UPROPERTY(BlueprintReadOnly)
    EAndroidXRLightEstimationState State{};

    //The directional light color
    UPROPERTY(BlueprintReadOnly)
    FLinearColor Color{};

    //The direction of the light
    UPROPERTY(BlueprintReadOnly)
    FVector Direction{};
};

/**
* The ambient light estimate data
*/
USTRUCT(BlueprintType)
struct FAndroidXRAmbientLightData
{
    GENERATED_BODY()

    //The state of the ambient light estimate
    UPROPERTY(BlueprintReadOnly)
    EAndroidXRLightEstimationState State{};

    //The color of the ambient light
    UPROPERTY(BlueprintReadOnly)
    FLinearColor Color{};

    //The color correction for the ambient light
    UPROPERTY(BlueprintReadOnly)
    FLinearColor ColorCorrection{};
};

/**
* The spherical harmonics container for each channel
*/
USTRUCT(BlueprintType)
struct FAndroidXRSphericalHarmonics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<float> R;

    UPROPERTY(BlueprintReadOnly)
    TArray<float> G;

    UPROPERTY(BlueprintReadOnly)
    TArray<float> B;
};

/**
* The spherical harmonics light estimate data
*/
USTRUCT(BlueprintType)
struct FAndroidXRSphericalHarmonicsData
{
    GENERATED_BODY()

    //The state of the spherical harmonics light estimate data
    UPROPERTY(BlueprintReadOnly)
    EAndroidXRLightEstimationState State{};

    //The kind of spherical harmonic
    UPROPERTY(BlueprintReadOnly)
    EAndroidXRSphericalHarmonicsKind HarmonicsKind{};

    //The spherical harmonics data
    UPROPERTY(BlueprintReadOnly)
    FAndroidXRSphericalHarmonics Harmonics{};
};

/**
* The cubemap configuration to create an estimator
*/
USTRUCT(BlueprintType)
struct FAndroidXRCubemapConfiguration
{
    GENERATED_BODY()

    //The resolution of the cubemap to create
    UPROPERTY(BlueprintReadWrite)
    int Resolution{};

    //The format for the cubemap to create
    UPROPERTY(BlueprintReadWrite)
    EAndroidXRCubemapColorFormat ColorFormat{};

    //Whether to reproject the cubemap data
    UPROPERTY(BlueprintReadWrite)
    bool bShouldReproject{};
};

/**
* The cubemap light estimate data
*/
USTRUCT(BlueprintType)
struct FAndroidXRCubemapLightData
{
    GENERATED_BODY()

    //The format of the cubemap light estimate data
    UPROPERTY(BlueprintReadOnly)
    EAndroidXRCubemapColorFormat CubemapFormat{};

    //The resolution of the cubemap light estimate data
    UPROPERTY(BlueprintReadOnly)
    int CubemapResolution{};

    //The state of the cubemap light estimate data
    UPROPERTY(BlueprintReadOnly)
    EAndroidXRLightEstimationState State{};

    //The right face image data
    UPROPERTY(BlueprintReadOnly)
    TArray<FLinearColor> ImageRight{};

    //The left face image data
    UPROPERTY(BlueprintReadOnly)
    TArray<FLinearColor> ImageLeft{};

    //The top face image data
    UPROPERTY(BlueprintReadOnly)
    TArray<FLinearColor> ImageTop{};

    //The bottom face image data
    UPROPERTY(BlueprintReadOnly)
    TArray<FLinearColor> ImageBottom{};

    //The front face image data
    UPROPERTY(BlueprintReadOnly)
    TArray<FLinearColor> ImageFront{};

    //The back face image data
    UPROPERTY(BlueprintReadOnly)
    TArray<FLinearColor> ImageBack{};

    //The rotation of the cubemap
    UPROPERTY(BlueprintReadOnly)
    FQuat Rotation{};

    //The exposure time of the cubemap
    UPROPERTY(BlueprintReadOnly)
    FTimespan CenterExposureTime{};
};