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
#include "AndroidXRLightEstimationTypes.h"

namespace AndroidXR
{
    inline EAndroidXRLightEstimationState Convert(XrLightEstimateStateANDROID NativeType)
    {
        switch(NativeType)
        {
            case XrLightEstimateStateANDROID::XR_LIGHT_ESTIMATE_STATE_VALID_ANDROID: return EAndroidXRLightEstimationState::Valid;
            case XrLightEstimateStateANDROID::XR_LIGHT_ESTIMATE_STATE_INVALID_ANDROID: return EAndroidXRLightEstimationState::Invalid;
        }
        return EAndroidXRLightEstimationState::Invalid;
    }

    inline XrCubemapLightingColorFormatANDROID Convert(EAndroidXRCubemapColorFormat UnrealType)
    {
        switch(UnrealType)
        {
            case EAndroidXRCubemapColorFormat::R32G32B32:
                return XrCubemapLightingColorFormatANDROID::XR_CUBEMAP_LIGHTING_COLOR_FORMAT_R32G32B32_SFLOAT_ANDROID;
            case EAndroidXRCubemapColorFormat::R32G32B32A32:
                return XrCubemapLightingColorFormatANDROID::XR_CUBEMAP_LIGHTING_COLOR_FORMAT_R32G32B32A32_SFLOAT_ANDROID;
            case EAndroidXRCubemapColorFormat::R16G16B16A16:
                return XrCubemapLightingColorFormatANDROID::XR_CUBEMAP_LIGHTING_COLOR_FORMAT_R16G16B16A16_SFLOAT_ANDROID;
            default:
                break;
        }
        return XrCubemapLightingColorFormatANDROID::XR_CUBEMAP_LIGHTING_COLOR_FORMAT_R32G32B32A32_SFLOAT_ANDROID;
    }

    inline EAndroidXRCubemapColorFormat Convert(XrCubemapLightingColorFormatANDROID NativeType)
    {
        switch(NativeType)
        {
            case XR_CUBEMAP_LIGHTING_COLOR_FORMAT_R32G32B32_SFLOAT_ANDROID:
                return EAndroidXRCubemapColorFormat::R32G32B32;
            case XR_CUBEMAP_LIGHTING_COLOR_FORMAT_R32G32B32A32_SFLOAT_ANDROID:
                return EAndroidXRCubemapColorFormat::R32G32B32A32;
            case XR_CUBEMAP_LIGHTING_COLOR_FORMAT_R16G16B16A16_SFLOAT_ANDROID:
                return EAndroidXRCubemapColorFormat::R16G16B16A16;
            default:
                break;
        }
        return EAndroidXRCubemapColorFormat::R32G32B32A32;
    }
}
