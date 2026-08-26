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
#include "SpatialAnchorWidget.generated.h"

UENUM(BlueprintType)
enum class ESpatialAnchorOperationMode : uint8
{
    Create,
    Delete,
    Persist,
    Unpersist
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpatialAnchorOperationModeChagned, ESpatialAnchorOperationMode, Operation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpatialWidgetButtonClicked);
UCLASS()
class UNREALSAMPLES_API USpatialAnchorWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void NativeConstruct() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UComboBoxString> OperationModeComboBox{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UButton> LoadPersistedButton{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UButton> StartTrackingButton{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UButton> StopTrackingButton{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UButton> ExecuteButton{};

    UPROPERTY(BlueprintAssignable)
    FSpatialAnchorOperationModeChagned OnSpatialOperationChanged{};

    UPROPERTY(BlueprintAssignable)
    FSpatialWidgetButtonClicked OnStartClicked{};

    UPROPERTY(BlueprintAssignable)
    FSpatialWidgetButtonClicked OnStopClicked{};

    UPROPERTY(BlueprintAssignable)
    FSpatialWidgetButtonClicked OnLoadPersistedAnchorsClicked{};

private:
    UFUNCTION()
    void StartClickedHandler();

    UFUNCTION()
    void StopClickedHandler();

    UFUNCTION()
    void ExecuteClickedHandler();

    UFUNCTION()
    void LoadPersistedClickedHandler();

};
