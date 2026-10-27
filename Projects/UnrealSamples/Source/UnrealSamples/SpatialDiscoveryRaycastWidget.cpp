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
 #include "SpatialDiscoveryRaycastWidget.h"

void USpatialDiscoveryRaycastWidget::NativeConstruct()
{
    StartTrackingButton->OnClicked.RemoveAll(this);
    StartTrackingButton->OnClicked.AddUniqueDynamic(this, &USpatialDiscoveryRaycastWidget::StartClickedHandler);

    StopTrackingButton->OnClicked.RemoveAll(this);
    StopTrackingButton->OnClicked.AddUniqueDynamic(this, &USpatialDiscoveryRaycastWidget::StopClickedHandler);

    RayLengthSlider->OnValueChanged.RemoveAll(this);
    RayLengthSlider->OnValueChanged.AddUniqueDynamic(this, &USpatialDiscoveryRaycastWidget::RayLengthChangedHandler);

    EnableAllWidgets(false);
    UUserWidget::NativeConstruct();
}


void USpatialDiscoveryRaycastWidget::StartClickedHandler()
{
    EnableAllWidgets(false);
    OnStartClicked.Broadcast();
}

void USpatialDiscoveryRaycastWidget::StopClickedHandler()
{
    EnableAllWidgets(false);
    OnStopClicked.Broadcast();
}

void USpatialDiscoveryRaycastWidget::RayLengthChangedHandler(float Length)
{
    OnRayLengthChanged.Broadcast(Length);
}

void USpatialDiscoveryRaycastWidget::EnableAllWidgets(bool bShouldEnable)
{
    StartTrackingButton->SetIsEnabled(bShouldEnable);
    StopTrackingButton->SetIsEnabled(bShouldEnable);
    RayLengthSlider->SetIsEnabled(bShouldEnable);
}

void USpatialDiscoveryRaycastWidget::SetRaycastSupported(bool bRaycastSupported)
{
    RaycastSupportedCheckBox->SetIsChecked(bRaycastSupported);
    StartTrackingButton->SetIsEnabled(bRaycastSupported);
}

void USpatialDiscoveryRaycastWidget::TrackingStarted()
{
    EnableAllWidgets(true);
    StartTrackingButton->SetIsEnabled(false);
}

void USpatialDiscoveryRaycastWidget::TrackingStopped()
{
    EnableAllWidgets(false);
    StartTrackingButton->SetIsEnabled(true);
}