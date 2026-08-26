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
#include "AndroidXRFaceTrackingSubsystem.h"
#include "AndroidXRFaceTrackingComponent.generated.h"

/**
* AndroidXR Face Tracking Component
*/
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ANDROIDXRFACETRACKING_API UAndroidXRFaceTrackingComponent : public USkeletalMeshComponent, public IAndroidXRTrackedDataListener
{
    GENERATED_BODY()
public:
    UAndroidXRFaceTrackingComponent();
    /**
    * The face tracking state and facial expressions
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Face Tracking")
    FAndroidXRFaceState FaceState{};

    /**
    * The face calibration status
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Face Tracking")
    bool bFaceCalibrationStatus{};

    void BeginPlay() override;
    void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    TMap<int, FString> ParameterStringTable{};
};
