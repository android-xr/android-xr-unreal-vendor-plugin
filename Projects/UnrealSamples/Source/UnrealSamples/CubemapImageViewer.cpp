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

#include "CubemapImageViewer.h"
#include "Components/ComboBoxString.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "RenderResource.h"
#include "RHIResources.h"
#include "Engine/TextureCube.h"
#include "AndroidXRLightEstimationBlueprintFunctionLibrary.h"

void UCubemapImageViewer::OnSelectedFaceChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    SelectedFace = FaceSelectionComboBox->GetSelectedIndex();
}

void UCubemapImageViewer::InitializeCubemap(const FAndroidXRCubemapEstimator& CubemapEstimator)
{
    CubemapTexture = UTexture2D::CreateTransient(CubemapEstimator.CubemapResolution, CubemapEstimator.CubemapResolution, EPixelFormat::PF_A32B32G32R32F);
    if(CubemapTexture)
    {
        CubemapTexture->UpdateResource();
    }
    MaterialInstance = UMaterialInstanceDynamic::Create(CubemapViewerMaterial, this);
    MaterialInstance->SetTextureParameterValue("CubemapTexture", CubemapTexture);
    if(CubeFaceImage)
    {
        CubeFaceImage->SetBrushFromMaterial(MaterialInstance);
    }
}

void UCubemapImageViewer::ResetCubemap()
{
    CubemapTexture->ConditionalBeginDestroy();
    MaterialInstance->ConditionalBeginDestroy();
}

void UCubemapImageViewer::UpdateCubemap(UTextureCube* Cube)
{
    if(!Cube || !CubemapTexture)
    {
        return;
    }
    auto SourceResouce = Cube->GetResource();
    auto DestResource = CubemapTexture->GetResource();
    const auto& SourceMips = Cube->GetPlatformData()->Mips[0];
    auto& DestMips = CubemapTexture->GetPlatformData()->Mips[0];
    auto SourceStride = SourceMips.SizeX * SourceMips.SizeY * GPixelFormats[EPixelFormat::PF_A32B32G32R32F].BlockBytes;
    auto SourceData = reinterpret_cast<const uint8_t*>(SourceMips.BulkData.LockReadOnly());
    auto SourceStart = SourceData + (SelectedFace * SourceStride);
    auto DestData = DestMips.BulkData.Lock(LOCK_READ_WRITE);
    FMemory::Memcpy(DestData, SourceStart, SourceStride);
    SourceMips.BulkData.Unlock();
    DestMips.BulkData.Unlock();
    CubemapTexture->UpdateResource();
}

void UCubemapImageViewer::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}

void UCubemapImageViewer::NativeConstruct()
{
    Super::NativeConstruct();
    FaceSelectionComboBox->ClearOptions();
    FaceSelectionComboBox->OnSelectionChanged.RemoveAll(this);
    FaceSelectionComboBox->OnSelectionChanged.AddDynamic(this, &UCubemapImageViewer::OnSelectedFaceChanged);
    auto EnumPtr = StaticEnum<ECubemapFace>();
    for(auto FaceIndex = 0; FaceIndex < 6; FaceIndex++)
    {
        FaceSelectionComboBox->AddOption(FString::Printf(TEXT("%s"), *EnumPtr->GetNameStringByIndex(FaceIndex)));
    }
    FaceSelectionComboBox->SetSelectedIndex(0);
}