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
#include "Blueprint/UserWidget.h"
#include "AndroidXRTypes.h"
#include "AndroidXRLightEstimationTypes.h"
#include "CubemapImageViewer.generated.h"

class UComboBoxString;
class UImage;
class UTextBlock;

UENUM()
enum ECubemapFace : uint8
{
    Front,
    Back,
    Right,
    Left,
    Up,
    Down
};

UCLASS()
class UNREALSAMPLES_API UCubemapImageViewer : public UUserWidget
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UImage> CubeFaceImage{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UComboBoxString> FaceSelectionComboBox{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UMaterialInterface> CubemapViewerMaterial;

    UFUNCTION()
    void OnSelectedFaceChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION(BlueprintCallable)
    void InitializeCubemap(const FAndroidXRCubemapEstimator& CubemapEstimator);

    UFUNCTION(BlueprintCallable)
    void ResetCubemap();

    UFUNCTION(BlueprintCallable)
    void UpdateCubemap(UTextureCube* Cube);

protected:
    void NativeOnInitialized() override;
    void NativeConstruct() override;
    int SelectedFace = 0;

    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;

    UPROPERTY()
    TObjectPtr<UTexture2D> CubemapTexture;
};
