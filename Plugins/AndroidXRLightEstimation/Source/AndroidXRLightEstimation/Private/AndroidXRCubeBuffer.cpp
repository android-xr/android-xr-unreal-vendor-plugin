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

#include "AndroidXRCubeBuffer.h"

const TArray<uint8>& FAndroidXRCubeBuffer::GetImageFromIndex(EFaceIndex FaceIndex)
{
    switch(FaceIndex)
    {
        case EFaceIndex::Left:
            return LeftImage();
        case EFaceIndex::Right:
            return RightImage();
        case EFaceIndex::Top:
            return TopImage();
        case EFaceIndex::Bottom:
            return BottomImage();
        case EFaceIndex::Front:
            return FrontImage();
        case EFaceIndex::Back:
            return BackImage();
        default:
            break;
    }
    return LeftImage();
}

int32 FAndroidXRCubeBuffer::GetCubemapPixelSize(EAndroidXRCubemapColorFormat CubeFormat)
{
    switch(CubeFormat)
    {
        case EAndroidXRCubemapColorFormat::R32G32B32:
            return sizeof(float) * 3;
        case EAndroidXRCubemapColorFormat::R32G32B32A32:
            return sizeof(float) * 4;
        case EAndroidXRCubemapColorFormat::R16G16B16A16:
            return sizeof(uint16_t) * 4;
        default:
            break;
    }
    return 0;
}

FAndroidXRCubeBuffer::FAndroidXRCubeBuffer()
{
    ImageData.SetNum(6);
}

void FAndroidXRCubeBuffer::Resize(EAndroidXRCubemapColorFormat ColorFormat, int Resolution)
{
    auto PixelSize = GetCubemapPixelSize(ColorFormat);
    for(auto CubeFace = 0; CubeFace < 6; CubeFace++)
    {
        ImageData[CubeFace].Reset();
        ImageData[CubeFace].SetNum(PixelSize * static_cast<uint32_t>(Resolution) * static_cast<uint32_t>(Resolution));
    }
}

TArray<uint8_t>& FAndroidXRCubeBuffer::FrontImage()
{
    return ImageData[static_cast<int>(EFaceIndex::Front)];
}

TArray<uint8_t>& FAndroidXRCubeBuffer::BackImage()
{
    return ImageData[static_cast<int>(EFaceIndex::Back)];
}

TArray<uint8_t>& FAndroidXRCubeBuffer::TopImage()
{
    return ImageData[static_cast<int>(EFaceIndex::Top)];
}

TArray<uint8_t>& FAndroidXRCubeBuffer::BottomImage()
{
    return ImageData[static_cast<int>(EFaceIndex::Bottom)];
}

TArray<uint8_t>& FAndroidXRCubeBuffer::LeftImage()
{
    return ImageData[static_cast<int>(EFaceIndex::Left)];
}

TArray<uint8_t>& FAndroidXRCubeBuffer::RightImage()
{
    return ImageData[static_cast<int>(EFaceIndex::Right)];
}

uint32_t FAndroidXRCubeBuffer::GetImageSize()
{
    return static_cast<uint32_t>(ImageData[0].Num());
}

void FAndroidXRCubeBuffer::GetNativeCubemapData(XrCubemapLightingDataANDROID& CubemapData)
{
    CubemapData =
    {
        .type = XR_TYPE_CUBEMAP_LIGHTING_DATA_ANDROID,
        .imageBufferSize = GetImageSize(),
        .imageBufferRight = RightImage().GetData(),
        .imageBufferLeft = LeftImage().GetData(),
        .imageBufferTop = TopImage().GetData(),
        .imageBufferBottom = BottomImage().GetData(),
        .imageBufferFront = FrontImage().GetData(),
        .imageBufferBack = BackImage().GetData(),
    };
}

template<typename T>
void ConvertToLinearColor(const T& Input, FLinearColor& Output)
{
}

template<>
void ConvertToLinearColor(const FVector4f& Input, FLinearColor& Output)
{
    Output.R = Input.X;
    Output.G = Input.Y;
    Output.B = Input.Z;
    Output.A = Input.W;
}

template<>
void ConvertToLinearColor(const FVector3f& Input, FLinearColor& Output)
{
    Output.R = Input.X;
    Output.G = Input.Y;
    Output.B = Input.Z;
    Output.A = 1;
}

template<>
void ConvertToLinearColor(const TStaticArray<FFloat16, 4>& Input, FLinearColor& Output)
{
    Output.R = Input[0];
    Output.G = Input[1];
    Output.B = Input[2];
    Output.A = Input[3];
}

int Get1DIndex(int Row, int Col, int RowLength)
{
    return (Row * RowLength) + Col;
}

template<typename T>
void FillColorBuffer(EFaceIndex FaceIndex, const TArrayView<T>& InputBuffer, TArray<FLinearColor>& OutputBuffer, int CubemapResolution)
{
    auto Height = CubemapResolution - 1;
    for(auto Row = 0; Row < CubemapResolution; Row++)
    {
        for(auto Col = 0; Col < CubemapResolution; Col++)
        {
            auto SrcRow = 0;
            auto SrcCol = 0;
            switch(FaceIndex)
            {
                case EFaceIndex::Front:
                {
                    SrcRow = Col;
                    SrcCol = Row;
                    break;
                }
                case EFaceIndex::Left:
                {
                    SrcRow = Row;
                    SrcCol = Height - Col;
                    break;
                }
                case EFaceIndex::Right:
                {
                    SrcRow = Height - Row;
                    SrcCol = Col;
                    break;
                }
                case EFaceIndex::Back:
                case EFaceIndex::Top:
                case EFaceIndex::Bottom:
                {
                    SrcRow = Height - Col;
                    SrcCol = Height - Row;
                    break;
                }
                default:
                    break;
            }
            auto OutputPixelIndex = OutputBuffer.AddDefaulted();
            ConvertToLinearColor(InputBuffer[Get1DIndex(SrcRow, SrcCol, CubemapResolution)], OutputBuffer[OutputPixelIndex]);
        }
    }
}

void FAndroidXRCubeBuffer::GetColorBuffer(int FaceIndex, const TArray<uint8_t>* Buffer, int CubemapResolution, EAndroidXRCubemapColorFormat CubemapFormat, TArray<FLinearColor>& OutColor)
{
    auto FaceIndexValue = static_cast<EFaceIndex>(FaceIndex);
    auto PixelSize = GetCubemapPixelSize(CubemapFormat);
    auto PixelsPerRow = CubemapResolution;
    OutColor.Reset();
    switch(CubemapFormat)
    {
        case EAndroidXRCubemapColorFormat::R32G32B32:
        {
            TArrayView<const FVector3f> Input{ reinterpret_cast<const FVector3f*>(Buffer->GetData()), PixelsPerRow * CubemapResolution };
            FillColorBuffer(FaceIndexValue, Input, OutColor, CubemapResolution);
            break;
        }
        case EAndroidXRCubemapColorFormat::R32G32B32A32:
        {
            TArrayView<const FVector4f> Input{ reinterpret_cast<const FVector4f*>(Buffer->GetData()), PixelsPerRow * CubemapResolution };
            FillColorBuffer(FaceIndexValue, Input, OutColor, CubemapResolution);
            break;
        }
        case EAndroidXRCubemapColorFormat::R16G16B16A16:
        {
            TArrayView<const TStaticArray<FFloat16, 4>> Input{ reinterpret_cast<const TStaticArray<FFloat16, 4>*>(Buffer->GetData()), PixelsPerRow * CubemapResolution };
            FillColorBuffer(FaceIndexValue, Input, OutColor, CubemapResolution);
            break;
        }
        default:
            break;
    }
}
