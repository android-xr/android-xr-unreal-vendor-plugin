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

enum class EFaceIndex
{
    Front,
    Back,
    Top,
    Bottom,
    Left,
    Right
};

typedef struct FAndroidXRCubeBuffer
{
    FAndroidXRCubeBuffer();
    void Resize(EAndroidXRCubemapColorFormat ColorFormat, int Resolution);

    TArray<uint8_t>& FrontImage();
    TArray<uint8_t>& BackImage();
    TArray<uint8_t>& TopImage();
    TArray<uint8_t>& BottomImage();
    TArray<uint8_t>& LeftImage();
    TArray<uint8_t>& RightImage();

    uint32_t GetImageSize();
    void GetNativeCubemapData(XrCubemapLightingDataANDROID& CubemapData);
    static void GetColorBuffer(int FaceIndex, const TArray<uint8_t>* Buffer, int CubemapResolution, EAndroidXRCubemapColorFormat CubemapFormat, TArray<FLinearColor>& OutColor);
    const TArray<uint8>& GetImageFromIndex(EFaceIndex FaceIndex);
    static int32 GetCubemapPixelSize(EAndroidXRCubemapColorFormat CubeFormat);

private:
    TArray<TArray<uint8_t>> ImageData{};
}FAndroidXRCubeBuffer;