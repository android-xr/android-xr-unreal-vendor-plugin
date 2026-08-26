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

#include "SpatialAnchorWidget.h"
#include "UnrealSamplesWidgetHelpers.h"
void USpatialAnchorWidget::NativeConstruct()
{
    OperationModeComboBox->ClearOptions();
    AddEnumToComboBox(OperationModeComboBox, StaticEnum<ESpatialAnchorOperationMode>());

    StartTrackingButton->OnClicked.RemoveAll(this);
    StartTrackingButton->OnClicked.AddUniqueDynamic(this, &USpatialAnchorWidget::StartClickedHandler);

    StopTrackingButton->OnClicked.RemoveAll(this);
    StopTrackingButton->OnClicked.AddUniqueDynamic(this, &USpatialAnchorWidget::StopClickedHandler);

    LoadPersistedButton->OnClicked.RemoveAll(this);
    LoadPersistedButton->OnClicked.AddUniqueDynamic(this, &USpatialAnchorWidget::LoadPersistedClickedHandler);

    ExecuteButton->OnClicked.RemoveAll(this);
    ExecuteButton->OnClicked.AddUniqueDynamic(this, &USpatialAnchorWidget::ExecuteClickedHandler);
}

void USpatialAnchorWidget::StartClickedHandler()
{
    OnStartClicked.Broadcast();
}

void USpatialAnchorWidget::StopClickedHandler()
{
    OnStopClicked.Broadcast();
}

void USpatialAnchorWidget::ExecuteClickedHandler()
{
    auto SelectedOperation = GetValueFromComboBox<ESpatialAnchorOperationMode>(OperationModeComboBox);
    OnSpatialOperationChanged.Broadcast(SelectedOperation);
}

void USpatialAnchorWidget::LoadPersistedClickedHandler()
{
    OnLoadPersistedAnchorsClicked.Broadcast();
}
