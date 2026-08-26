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
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "AndroidXRSpatialMarkerTrackingTypes.h"
#include "SpatialMarkerTrackingWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpatialMarkerTrackingStartClicked, const FSpatialMarkerTrackingConfig&, Config);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpatialMarkerTrackingStopClicked);

UCLASS()
class UNREALSAMPLES_API USpatialMarkerConfigWidget : public UUserWidget
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> MarkerTypeLabel{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UCheckBox> EnableMarkerType{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UCheckBox> OptimizeForStaticMarker{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UCheckBox> OptimizeForFixedLengthMarker{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<USlider> MarkerLengthSlider{};
};

UCLASS()
class UNREALSAMPLES_API USpatialMarkerConfigWidgetWithComboBox : public UUserWidget
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<USpatialMarkerConfigWidget> MarkerConfig{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> LabelText{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UComboBoxString> ComboBox{};
};

UCLASS()
class UNREALSAMPLES_API USpatialMarkerTrackingWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void NativeConstruct() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<USpatialMarkerConfigWidget> QRCodeConfig{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<USpatialMarkerConfigWidget> MicroQRCodeConfig{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<USpatialMarkerConfigWidgetWithComboBox> ArucoCodeConfig{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<USpatialMarkerConfigWidgetWithComboBox> AprilTagConfig{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
    TObjectPtr<UButton> StartTrackingButton{};


    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UButton> StopTrackingButton{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UButton> ForceUpdateButton{};

    UPROPERTY(BlueprintAssignable)
    FSpatialMarkerTrackingStartClicked OnStartTrackingClicked{};

    UPROPERTY(BlueprintAssignable)
    FSpatialMarkerTrackingStopClicked OnStopTrackingClicked{};

    UPROPERTY(BlueprintAssignable)
    FSpatialMarkerTrackingStopClicked OnForceUpdateClicked{};

    UFUNCTION(BlueprintCallable)
    void InitializeMarkerConfig(const TMap<ESpatialCapability, FAndroidXRSpatialCapabilityInfo>& CapabilityInfoTable);

private:
    UFUNCTION()
    void StartClickedHandler();

    UFUNCTION()
    void StopClickedHandler();

    UFUNCTION()
    void ForceUpdateClickedHandler();
};
