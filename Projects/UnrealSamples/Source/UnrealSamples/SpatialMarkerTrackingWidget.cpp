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


#include "SpatialMarkerTrackingWidget.h"
#include "UnrealSamplesWidgetHelpers.h"
#include "SpatialAnchorWidget.h"

void EnableMarkerTypeAndFeatures(TObjectPtr<USpatialMarkerConfigWidget> Widget, const FAndroidXRSpatialCapabilityInfo& CapabilityInfo)
{
    Widget->SetIsEnabled(CapabilityInfo.bIsCapabilitySupported);
    auto& Features = CapabilityInfo.SupportedFeatures;
    auto bFixedLengthMarkerSupported = Features.Contains(ESpatialCapabilityFeature::FixedSizeMarkers);
    auto bStaticMarkerSupported = Features.Contains(ESpatialCapabilityFeature::StaticMarkers);
    Widget->OptimizeForFixedLengthMarker->SetIsEnabled(bFixedLengthMarkerSupported);
    Widget->OptimizeForStaticMarker->SetIsEnabled(bStaticMarkerSupported);
    Widget->MarkerLengthSlider->SetIsEnabled(bFixedLengthMarkerSupported);
}

void ApplySetting(FSpatialMarkerTrackingFeature& FeatureConfig, TObjectPtr<USpatialMarkerConfigWidget> Widget)
{
    FeatureConfig.bEnableMarkerType = Widget->EnableMarkerType->IsChecked();
    FeatureConfig.bOptimizeForMarkerSize = Widget->OptimizeForFixedLengthMarker->IsChecked();
    FeatureConfig.bOptimizeForStaticMarker = Widget->OptimizeForStaticMarker->IsChecked();
    FeatureConfig.MarkerSideLength = Widget->MarkerLengthSlider->GetValue();
}

void SetNameForMarkerWidget(TObjectPtr<USpatialMarkerConfigWidget> Widget, const FString& Text)
{
    Widget->MarkerTypeLabel->SetText(FText::FromString(Text));
}

void USpatialMarkerTrackingWidget::NativeConstruct()
{
    SetNameForMarkerWidget(QRCodeConfig, TEXT("QR Code"));
    SetNameForMarkerWidget(MicroQRCodeConfig, TEXT("Micro QR Code"));
    SetNameForMarkerWidget(ArucoCodeConfig->MarkerConfig, TEXT("Aruco Marker"));
    SetNameForMarkerWidget(AprilTagConfig->MarkerConfig, TEXT("April Tag"));

    AprilTagConfig->LabelText->SetText(FText::FromString(TEXT("April Dict")));
    ArucoCodeConfig->LabelText->SetText(FText::FromString(TEXT("Aruco Dict")));

    StartTrackingButton->OnClicked.RemoveAll(this);
    StartTrackingButton->OnClicked.AddUniqueDynamic(this, &USpatialMarkerTrackingWidget::StartClickedHandler);

    StopTrackingButton->OnClicked.RemoveAll(this);
    StopTrackingButton->OnClicked.AddUniqueDynamic(this, &USpatialMarkerTrackingWidget::StopClickedHandler);

    ForceUpdateButton->OnClicked.RemoveAll(this);
    ForceUpdateButton->OnClicked.AddUniqueDynamic(this, &USpatialMarkerTrackingWidget::ForceUpdateClickedHandler);

    ArucoCodeConfig->ComboBox->ClearOptions();
    AddEnumToComboBox(ArucoCodeConfig->ComboBox, StaticEnum<ESpatialMarkerArucoDict>());
    AddEnumToComboBox(AprilTagConfig->ComboBox, StaticEnum<ESpatialMarkerAprilTagDict>());

    SetIsEnabled(false);
}

void USpatialMarkerTrackingWidget::InitializeMarkerConfig(const TMap<ESpatialCapability, FAndroidXRSpatialCapabilityInfo>& CapabilityInfoTable)
{
    SetIsEnabled(true);
    QRCodeConfig->SetIsEnabled(false);
    MicroQRCodeConfig->SetIsEnabled(false);
    AprilTagConfig->SetIsEnabled(false);
    ArucoCodeConfig->SetIsEnabled(false);

    for(auto& [Capability, CapabilityInfo] : CapabilityInfoTable)
    {
        switch(Capability)
        {
            case ESpatialCapability::MarkerTracking_QR_Code:
            {
                EnableMarkerTypeAndFeatures(QRCodeConfig, CapabilityInfo);
                break;
            }
            case ESpatialCapability::MarkerTracking_Micro_QR_Code:
            {
                EnableMarkerTypeAndFeatures(MicroQRCodeConfig, CapabilityInfo);
                break;
            }
            case ESpatialCapability::MarkerTracking_Aruco:
            {
                ArucoCodeConfig->SetIsEnabled(CapabilityInfo.bIsCapabilitySupported);
                EnableMarkerTypeAndFeatures(ArucoCodeConfig->MarkerConfig, CapabilityInfo);
                break;
            }
            case ESpatialCapability::MarkerTracking_April_Tag:
            {
                AprilTagConfig->SetIsEnabled(CapabilityInfo.bIsCapabilitySupported);
                EnableMarkerTypeAndFeatures(AprilTagConfig->MarkerConfig, CapabilityInfo);
                break;
            }
            default:
                break;
        }
    }
}

void USpatialMarkerTrackingWidget::StartClickedHandler()
{
    StartTrackingButton->SetIsEnabled(false);
    StopTrackingButton->SetIsEnabled(true);
    ForceUpdateButton->SetIsEnabled(true);
    FSpatialMarkerTrackingConfig Config{};
    ApplySetting(Config.QRCodeSettings, QRCodeConfig);
    ApplySetting(Config.MicroQRCodeSettings, MicroQRCodeConfig);
    ApplySetting(Config.ArucoMarkerSettings, ArucoCodeConfig->MarkerConfig);
    ApplySetting(Config.AprilTagSettings, AprilTagConfig->MarkerConfig);

    Config.AprilTagSettings.AprilTagDict = GetValueFromComboBox<ESpatialMarkerAprilTagDict>(AprilTagConfig->ComboBox);
    Config.ArucoMarkerSettings.ArucoDict = GetValueFromComboBox<ESpatialMarkerArucoDict>(ArucoCodeConfig->ComboBox);
    OnStartTrackingClicked.Broadcast(Config);
}

void USpatialMarkerTrackingWidget::StopClickedHandler()
{
    StartTrackingButton->SetIsEnabled(true);
    StopTrackingButton->SetIsEnabled(false);
    ForceUpdateButton->SetIsEnabled(false);
    OnStopTrackingClicked.Broadcast();
}

void USpatialMarkerTrackingWidget::ForceUpdateClickedHandler()
{
    OnForceUpdateClicked.Broadcast();
}
