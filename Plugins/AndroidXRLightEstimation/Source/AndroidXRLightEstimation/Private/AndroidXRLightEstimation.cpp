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

#include "AndroidXRLightEstimation.h"
#include <IOpenXRHMDModule.h>
#include <IXRTrackingSystem.h>
#include <IHeadMountedDisplay.h>
#include "IOpenXRHMD.h"
#include "AndroidXRLightEstimationConversions.h"

DEFINE_LOG_CATEGORY(LogAndroidXRLightEstimation);

#define LOG_IF_OPENXR_ERROR_AND_RETURN(Tag, Function, Result) if(!XR_UNQUALIFIED_SUCCESS(Result)){ UE_LOG(LogAndroidXRLightEstimation, Error, TEXT("%s : %s failed with error %s"), TEXT(#Tag), TEXT(#Function), OpenXRResultToString(Result)); return false;}
#define RETURN_IF_FUNCTION_NOT_FOUND(Function) if(!Function){ UE_LOG(LogAndroidXRLightEstimation, Error, TEXT("Unable to locate function %s"), TEXT(#Function)); return false;}

FAndroidXRLightEstimation::FAndroidXRLightEstimation()
{
}

void FAndroidXRLightEstimation::StartupModule()
{
    IAndroidXRLightEstimationModule::StartupModule();
    RegisterOpenXRExtensionModularFeature();
}

void FAndroidXRLightEstimation::ShutdownModule()
{
    UnregisterOpenXRExtensionModularFeature();
    IAndroidXRLightEstimationModule::ShutdownModule();
}

bool FAndroidXRLightEstimation::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
    OutExtensions.Add(XR_ANDROID_LIGHT_ESTIMATION_EXTENSION_NAME);
    OutExtensions.Add(XR_ANDROID_LIGHT_ESTIMATION_CUBEMAP_EXTENSION_NAME);
    return true;
}

void FAndroidXRLightEstimation::PostCreateInstance(XrInstance InInstance)
{
    Instance = InInstance;

    ResolveOpenXRFunctions(Instance,
                           {
                               LIGHTESTIMATION_FUNCTIONS(RESOLVE_OPENXR_FUNC)
                           },
                           [](const char* UnresolvableFunctionName, XrResult Result)
    {
        UE_LOG(LogAndroidXRLightEstimation, Error,
               TEXT("Unable to resolve function pointer %s (error %s)"),
               UTF8_TO_TCHAR(UnresolvableFunctionName),
               OpenXRResultToString(Result));
    });
}

void FAndroidXRLightEstimation::PostCreateSession(XrSession InSession)
{
    Session = InSession;
}

bool FAndroidXRLightEstimation::IsLightEstimationSupported() const
{
    if(xrGetSystemProperties == nullptr)
    {
        return false;
    }
    auto SystemId = IOpenXRHMDModule::Get().GetSystemId();
    XrSystemLightEstimationPropertiesANDROID LightEstimationProperties
    {
        .type = XR_TYPE_SYSTEM_LIGHT_ESTIMATION_PROPERTIES_ANDROID
    };
    XrSystemProperties Properties
    {
        .type = XR_TYPE_SYSTEM_PROPERTIES,
        .next = &LightEstimationProperties
    };
    auto Result = xrGetSystemProperties(Instance, SystemId, &Properties);
    LOG_IF_OPENXR_ERROR_AND_RETURN(IsLightEstimationSupported, xrGetSystemProperties, Result);
    return !!LightEstimationProperties.supportsLightEstimation;
}

bool FAndroidXRLightEstimation::CreateLightEstimator(FAndroidXRLightEstimator& LightEstimator) const
{
    XrLightEstimatorCreateInfoANDROID CreateInfo
    {
        .type = XR_TYPE_LIGHT_ESTIMATOR_CREATE_INFO_ANDROID
    };
    auto Result = xrCreateLightEstimatorANDROID(Session, &CreateInfo, &LightEstimator.LightEstimator);
    LOG_IF_OPENXR_ERROR_AND_RETURN(CreateLightEstimator, xrCreateLightEstimatorANDROID, Result);
    return true;
}

bool FAndroidXRLightEstimation::DestroyLightEstimator(FAndroidXRLightEstimator& LightEstimator) const
{
    auto Result = xrDestroyLightEstimatorANDROID(LightEstimator.LightEstimator);
    LOG_IF_OPENXR_ERROR_AND_RETURN(DestroyLightEstimator, xrDestroyLightEstimatorANDROID, Result);
    LightEstimator.LightEstimator = {};
    return true;
}

bool FAndroidXRLightEstimation::GetDirectionalLightData(const FAndroidXRLightEstimator& LightEstimator, FAndroidXRDirectionalLightData& DirectionalLightData)
{
    auto WorldToMetersScale = GEngine->XRSystem.Get()->GetWorldToMetersScale();

    XrDirectionalLightANDROID DirectionalLight
    {
        .type = XR_TYPE_DIRECTIONAL_LIGHT_ANDROID
    };

    auto Result = GetLightEstimateData(LightEstimator, &DirectionalLight);
    LOG_IF_OPENXR_ERROR_AND_RETURN(GetDirectionalLightData, xrGetLightEstimateANDROID, Result);

    DirectionalLightData.State = AndroidXR::Convert(DirectionalLight.state);
    DirectionalLightData.Direction = ToFVector(DirectionalLight.direction, WorldToMetersScale);
    DirectionalLightData.Color = FLinearColor{ DirectionalLight.intensity.x, DirectionalLight.intensity.y, DirectionalLight.intensity.z, 1 };
    return true;
}

bool FAndroidXRLightEstimation::GetAmbientLightData(const FAndroidXRLightEstimator& LightEstimator, FAndroidXRAmbientLightData& AmbientLightData)
{
    XrAmbientLightANDROID AmbientLight
    {
        .type = XR_TYPE_AMBIENT_LIGHT_ANDROID
    };
    auto Result = GetLightEstimateData(LightEstimator, &AmbientLight);
    LOG_IF_OPENXR_ERROR_AND_RETURN(GetAmbientLightData, xrGetLightEstimateANDROID, Result);

    AmbientLightData.State = AndroidXR::Convert(AmbientLight.state);
    AmbientLightData.Color = FLinearColor{ AmbientLight.intensity.x,AmbientLight.intensity.y, AmbientLight.intensity.z, 1 };
    AmbientLightData.ColorCorrection = FLinearColor{ AmbientLight.colorCorrection.x, AmbientLight.colorCorrection.y, AmbientLight.colorCorrection.z, 1 };

    return true;
}

bool FAndroidXRLightEstimation::GetSphericalHarmonicsData(const FAndroidXRLightEstimator& LightEstimator, EAndroidXRSphericalHarmonicsKind HarmonicsKind, FAndroidXRSphericalHarmonicsData& SphericalHarmonicsData)
{
    XrSphericalHarmonicsANDROID SphericalHarmonics
    {
        .type = XR_TYPE_SPHERICAL_HARMONICS_ANDROID
    };
    auto Result = GetLightEstimateData(LightEstimator, &SphericalHarmonics);
    LOG_IF_OPENXR_ERROR_AND_RETURN(GetSphericalHarmonicsData, xrGetLightEstimateANDROID, Result);

    SphericalHarmonicsData.State = AndroidXR::Convert(SphericalHarmonics.state);
    SphericalHarmonicsData.HarmonicsKind = HarmonicsKind;

    auto& InputHarmonics = SphericalHarmonics.coefficients;
    auto& Harmonics = SphericalHarmonicsData.Harmonics;
    auto& R = Harmonics.R;
    auto& G = Harmonics.G;
    auto& B = Harmonics.B;
    for(auto Index = 0; Index < 9; Index++)
    {
        R.Add(InputHarmonics[Index][0]);
        G.Add(InputHarmonics[Index][1]);
        B.Add(InputHarmonics[Index][2]);
    }
    return true;
}

bool FAndroidXRLightEstimation::IsCubemapLightEstimationSupported() const
{
    if(xrGetSystemProperties == nullptr)
    {
        return false;
    }
    auto SystemId = IOpenXRHMDModule::Get().GetSystemId();
    XrSystemCubemapLightEstimationPropertiesANDROID CubemapProperties
    {
        .type = XR_TYPE_SYSTEM_CUBEMAP_LIGHT_ESTIMATION_PROPERTIES_ANDROID
    };
    XrSystemProperties Properties
    {
        .type = XR_TYPE_SYSTEM_PROPERTIES,
        .next = &CubemapProperties
    };
    auto Result = xrGetSystemProperties(Instance, SystemId, &Properties);
    LOG_IF_OPENXR_ERROR_AND_RETURN(IsLightEstimationDataTypeSupported, xrGetSystemProperties, Result);
    return !!CubemapProperties.supportsCubemapLightEstimation;
}

bool FAndroidXRLightEstimation::EnumerateSupportedCubemapFormats(TArray<EAndroidXRCubemapColorFormat>& Formats) const
{
    auto SystemId = IOpenXRHMDModule::Get().GetSystemId();
    uint32_t FormatCount;
    auto Result = xrEnumerateCubemapLightingColorFormatsANDROID(Instance, SystemId, 0, &FormatCount, nullptr);
    LOG_IF_OPENXR_ERROR_AND_RETURN(EnumerateSupportedCubemapFormats, xrEnumerateCubemapLightingColorFormatsANDROID, Result);
    TArray<XrCubemapLightingColorFormatANDROID> NativeFormats{};
    NativeFormats.SetNum(FormatCount);

    Result = xrEnumerateCubemapLightingColorFormatsANDROID(Instance, SystemId, FormatCount, &FormatCount, NativeFormats.GetData());
    LOG_IF_OPENXR_ERROR_AND_RETURN(EnumerateSupportedCubemapFormats, xrEnumerateCubemapLightingColorFormatsANDROID, Result);
    Formats.Reset();
    Algo::Transform(NativeFormats, Formats, [](XrCubemapLightingColorFormatANDROID Format)
    {
        return AndroidXR::Convert(Format);
    });
    return true;
}

bool FAndroidXRLightEstimation::EnumerateSupportedCubemapResolutions(TArray<int>& Resolutions) const
{
    auto SystemId = IOpenXRHMDModule::Get().GetSystemId();
    uint32_t ResolutionCount;
    auto Result = xrEnumerateCubemapLightingResolutionsANDROID(Instance, SystemId, 0, &ResolutionCount, nullptr);
    LOG_IF_OPENXR_ERROR_AND_RETURN(EnumerateSupportedCubemapFormats, xrEnumerateCubemapLightingResolutionsANDROID, Result);
    TArray<uint32_t> NativeResolutions{};
    NativeResolutions.SetNum(ResolutionCount);
    Result = xrEnumerateCubemapLightingResolutionsANDROID(Instance, SystemId, ResolutionCount, &ResolutionCount, NativeResolutions.GetData());
    LOG_IF_OPENXR_ERROR_AND_RETURN(EnumerateSupportedCubemapFormats, xrEnumerateCubemapLightingResolutionsANDROID, Result);
    Resolutions.Reset();
    Algo::Transform(NativeResolutions, Resolutions, [](uint32_t Resolution)
    {
        return static_cast<int>(Resolution);
    });
    return true;
}

bool FAndroidXRLightEstimation::CreateCubemapLightEstimator(FAndroidXRCubemapConfiguration& Config, FAndroidXRCubemapEstimator& CubemapEstimator) const
{
    XrCubemapLightEstimatorCreateInfoANDROID CubemapCreateInfo
    {
        .type = XR_TYPE_CUBEMAP_LIGHT_ESTIMATOR_CREATE_INFO_ANDROID,
        .cubemapResolution = static_cast<uint32_t>(Config.Resolution),
        .colorFormat = AndroidXR::Convert(Config.ColorFormat),
        .reproject = Config.bShouldReproject
    };

    XrLightEstimatorCreateInfoANDROID CreateInfo
    {
        .type = XR_TYPE_LIGHT_ESTIMATOR_CREATE_INFO_ANDROID,
        .next = &CubemapCreateInfo
    };

    auto Result = xrCreateLightEstimatorANDROID(Session, &CreateInfo, &CubemapEstimator.LightEstimator.LightEstimator);
    LOG_IF_OPENXR_ERROR_AND_RETURN(CreateCubemapLightEstimator, xrCreateLightEstimatorANDROID, Result);
    CubemapEstimator.ColorFormat = Config.ColorFormat;
    CubemapEstimator.CubemapResolution = Config.Resolution;
    return true;
}

bool FAndroidXRLightEstimation::DestroyCubemapEstimator(FAndroidXRCubemapEstimator& CubemapEstimator) const
{
    return DestroyLightEstimator(CubemapEstimator.LightEstimator);
}

bool FAndroidXRLightEstimation::GetCubemapData(const FAndroidXRCubemapEstimator& CubemapEstimator, FAndroidXRCubemapLightData& CubemapData)
{
    using namespace UE::Tasks;
    CubeBuffer.Resize(CubemapEstimator.ColorFormat, CubemapEstimator.CubemapResolution);
    XrCubemapLightingDataANDROID Cubemap{};
    CubeBuffer.GetNativeCubemapData(Cubemap);
    auto Result = GetLightEstimateData(CubemapEstimator.LightEstimator, &Cubemap);
    LOG_IF_OPENXR_ERROR_AND_RETURN(GetCubemapData, xrGetLightEstimateANDROID, Result);

    CubemapData.CubemapFormat = CubemapEstimator.ColorFormat;
    CubemapData.CubemapResolution = CubemapEstimator.CubemapResolution;
    CubemapData.State = AndroidXR::Convert(Cubemap.state);
    CubemapData.CenterExposureTime = ToFTimespan(Cubemap.centerExposureTime);

    auto CubemapResolution = CubemapEstimator.CubemapResolution;
    auto CubemapFormat = CubemapEstimator.ColorFormat;

    TArray<const TArray<uint8_t>*> InputBuffers
    {
        &CubeBuffer.FrontImage(),
        &CubeBuffer.BackImage(),
        &CubeBuffer.TopImage(),
        &CubeBuffer.BottomImage(),
        &CubeBuffer.LeftImage(),
        &CubeBuffer.RightImage(),
    };

    TArray<TArray<FLinearColor>*> OutputBuffers
    {
        &CubemapData.ImageFront,
        &CubemapData.ImageBack,
        &CubemapData.ImageTop,
        &CubemapData.ImageBottom,
        &CubemapData.ImageLeft,
        &CubemapData.ImageRight,
    };

    ParallelFor(6, [InputBuffers, OutputBuffers, CubemapResolution, CubemapFormat](int32 Index)
    {
        FAndroidXRCubeBuffer::GetColorBuffer(Index, InputBuffers[Index], CubemapResolution, CubemapFormat, *OutputBuffers[Index]);
    });
    return true;
}

bool FAndroidXRLightEstimation::CreateCubemapTextureForLightEstimation(const FAndroidXRCubemapEstimator& CubemapEstimator, UTextureCube*& TextureCube) const
{
    auto PixelFormat = EPixelFormat::PF_A32B32G32R32F;
    TextureCube = UTextureCube::CreateTransient(CubemapEstimator.CubemapResolution, CubemapEstimator.CubemapResolution, PixelFormat);
    if(TextureCube)
    {
        TextureCube->UpdateResource();
        return true;
    }
    return false;
}

const TArray<FLinearColor>* GetFaceDataFromFaceIndex(const FAndroidXRCubemapLightData& CubemapData, int TextureCubeFaceIndex)
{
    switch(TextureCubeFaceIndex)
    {
        case 0: return &CubemapData.ImageFront;
        case 1: return &CubemapData.ImageBack;
        case 2: return &CubemapData.ImageRight;
        case 3: return &CubemapData.ImageLeft;
        case 4: return &CubemapData.ImageTop;
        case 5: return &CubemapData.ImageBottom;
        default:
            break;
    }
    return nullptr;
}

bool FAndroidXRLightEstimation::ApplyLightEstimationCubemap(const FAndroidXRCubemapLightData& CubemapData, UTextureCube* TextureCube) const
{
    if(TextureCube->GetPixelFormat() != EPixelFormat::PF_A32B32G32R32F)
    {
        return false;
    }
    auto PixelStride = GPixelFormats[EPixelFormat::PF_A32B32G32R32F].BlockBytes;
    auto& DestMips = TextureCube->GetPlatformData()->Mips[0];
    auto DestFaceStride = DestMips.SizeX * DestMips.SizeY * PixelStride;
    auto DestStart = reinterpret_cast<uint8_t*>(DestMips.BulkData.Lock(LOCK_READ_WRITE));
    auto Dest = DestStart;
    for(auto FaceIndex = 0; FaceIndex < 6; FaceIndex++)
    {
        auto SourceData = GetFaceDataFromFaceIndex(CubemapData, FaceIndex);
        if(!SourceData)
        {
            UE_LOG(LogAndroidXRLightEstimation, Error, TEXT("No face data was found for face index %d"), FaceIndex);
            continue;
        }
        check(SourceData->Num() * sizeof(FLinearColor) == DestFaceStride);
        FMemory::Memcpy(Dest, SourceData->GetData(), DestFaceStride);
        Dest += DestFaceStride;
    }
    DestMips.BulkData.Unlock();
    TextureCube->UpdateResource();
    return true;
}

XrResult FAndroidXRLightEstimation::GetLightEstimateData(const FAndroidXRLightEstimator& LightEstimator, void* LightEstimateDataToAttach)
{
    auto HMD = GEngine->XRSystem.Get()->GetIOpenXRHMD();
    LightEstimateGetInfo.space = HMD->GetTrackingSpace();
    LightEstimateGetInfo.time = HMD->GetDisplayTime();
    LightEstimate.next = LightEstimateDataToAttach;
    return xrGetLightEstimateANDROID(LightEstimator.LightEstimator, &LightEstimateGetInfo, &LightEstimate);
}

IMPLEMENT_MODULE(FAndroidXRLightEstimation, AndroidXRLightEstimation);