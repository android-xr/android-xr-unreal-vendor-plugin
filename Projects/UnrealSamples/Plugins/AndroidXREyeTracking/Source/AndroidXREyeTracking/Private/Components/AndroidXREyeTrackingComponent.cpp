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

#include "Components/AndroidXREyeTrackingComponent.h"

UAndroidXREyeTrackingComponent::UAndroidXREyeTrackingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bAutoActivate = true;
}

void UAndroidXREyeTrackingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(!CanFetchTrackedData<UAndroidXREyeTrackingSubsystem>())
    {
        return;
    }
    auto Subsystem = GEngine->GetEngineSubsystem<UAndroidXREyeTrackingSubsystem>();
    if(!Subsystem)
    {
        return;
    }
    if(!GetTrackedData<UAndroidXREyeTrackingSubsystem, FAndroidXREyes>(EyesState))
    {
        return;
    }
    if(!EyesState.Eyes.IsValidIndex(static_cast<int32>(EyeSource)))
    {
        return;
    }
    auto& EyeData = EyesState.Eyes[static_cast<uint32>(EyeSource)];
    if(EyeData.EyeState == EAndroidXREyeState::Invalid || !bUpdateTransform)
    {
        return;
    }
    auto EyePosition = EyeData.Transform.GetLocation();
    auto EyeRotation = EyeData.Transform.GetRotation();
    EyePosition += LocationOffSet + (ForwardOffset * EyeRotation.RotateVector(FVector::ForwardVector));

    FTransform FinalTransform = GetComponentTransform();;
    if(bMatchEyeRotation)
    {
        FinalTransform.SetRotation(EyeRotation);
    }
    FinalTransform.SetLocation(EyePosition);
    SetWorldTransform(FinalTransform);
}