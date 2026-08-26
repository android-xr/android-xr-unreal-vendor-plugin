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

#include "Components/AndroidXRFaceTrackingComponent.h"

UAndroidXRFaceTrackingComponent::UAndroidXRFaceTrackingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bAutoActivate = true;
}

void UAndroidXRFaceTrackingComponent::BeginPlay()
{
    Super::BeginPlay();
    auto EnumPtr = StaticEnum<EAndroidXRFaceParameterIndices>();
    if(!EnumPtr)
    {
        return;
    }

    auto EnumCount = EnumPtr->ContainsExistingMax() ? EnumPtr->NumEnums() - 1 : EnumPtr->NumEnums();
    for(auto Index = 0; Index < EnumCount; Index++)
    {
        ParameterStringTable.Add(Index, EnumPtr->GetNameStringByValue(Index));
    }
}

void UAndroidXRFaceTrackingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(ParameterStringTable.IsEmpty())
    {
        return;
    }
    if(!CanFetchTrackedData<UAndroidXRFaceTrackingSubsystem>() || !GetTrackedData<UAndroidXRFaceTrackingSubsystem, FAndroidXRFaceState, bool>(FaceState, bFaceCalibrationStatus))
    {
        return;
    }
    const auto& Parameters = FaceState.Parameters;
    for(auto Index = 0; Index < Parameters.Num(); Index++)
    {
        auto ParameterName = ParameterStringTable[Index];
        SetMorphTarget(*ParameterName, Parameters[Index]);
    }
}