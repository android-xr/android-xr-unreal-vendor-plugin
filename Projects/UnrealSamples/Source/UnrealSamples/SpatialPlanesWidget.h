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
#include "AndroidXRSpatialPlaneTrackingTypes.h"
#include "SpatialPlaneActor.h"
#include "SpatialPlanesWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlaneRenderFilterChanged, const FSpatialPlaneRenderFilter&, RenderFilter);

UCLASS()
class UNREALSAMPLES_API USpatialPlanesWidget : public UUserWidget
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UCheckBox> RenderBoundsCheckbox{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UCheckBox> RenderPolygonsCheckbox{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UCheckBox> RenderMeshCheckbox{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UCheckBox> AlignmentFilter{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
    TObjectPtr<UComboBoxString> AlignmentComboBox{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UCheckBox> LabelFilter{};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UComboBoxString> LabelComboBox{};

    UPROPERTY(BlueprintAssignable)
    FPlaneRenderFilterChanged OnRenderFilterChanged;

    void NativeConstruct() override;
private:
    TMap<int, ESpatialPlaneAlignment> AlignmentOptionTable{};
    TMap<int, ESpatialPlaneSemanticLabel> SemanticLabelOptionTable{};

    UFUNCTION()
    void LabelFilterChangedHandler(bool bEnableFilter);

    UFUNCTION()
    void AlignmentFilterChangedHandler(bool bEnableFilter);

    UFUNCTION()
    void BoundsRenderChangedHandler(bool bEnableFilter);

    UFUNCTION()
    void MeshRenderChangedHandler(bool bEnableFilter);

    UFUNCTION()
    void PolygonRenderChangedHandler(bool bEnableFilter);

    void NotifyRenderFilterChanged();

    FSpatialPlaneRenderFilter RenderFilter{};
};
