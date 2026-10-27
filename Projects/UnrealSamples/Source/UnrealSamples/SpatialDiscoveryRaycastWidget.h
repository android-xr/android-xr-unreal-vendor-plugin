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
#include "SpatialDiscoveryRaycastWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpatialRaycastLengthChanged, float, RayLength);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpatialDiscoveryRaycastButtonClicked);
UCLASS()
class UNREALSAMPLES_API USpatialDiscoveryRaycastWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void NativeConstruct() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UButton> StartTrackingButton{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UButton> StopTrackingButton{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<USlider> RayLengthSlider{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UCheckBox> RaycastSupportedCheckBox{};

    UPROPERTY(BlueprintAssignable)
    FSpatialDiscoveryRaycastButtonClicked OnStartClicked{};

    UPROPERTY(BlueprintAssignable)
    FSpatialDiscoveryRaycastButtonClicked OnStopClicked{};

    UPROPERTY(BlueprintAssignable)
    FSpatialRaycastLengthChanged OnRayLengthChanged{};

    UFUNCTION(BlueprintCallable)
    void SetRaycastSupported(bool bRaycastSupported);

    UFUNCTION(BlueprintCallable)
    void TrackingStarted();

    UFUNCTION(BlueprintCallable)
    void TrackingStopped();

private:
    UFUNCTION()
    void StartClickedHandler();

    UFUNCTION()
    void StopClickedHandler();

    UFUNCTION()
    void RayLengthChangedHandler(float Length);

    void EnableAllWidgets(bool bShouldEnable);
};
