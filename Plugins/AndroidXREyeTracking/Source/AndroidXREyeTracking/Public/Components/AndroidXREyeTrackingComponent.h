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
#include "AndroidXREyeTrackingSubsystem.h"
#include "AndroidXREyeTrackingComponent.generated.h"

/**
* AndroidXR Eye Tracking scene component. The component will be positioned at the pose of the selected eye source
*/
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ANDROIDXREYETRACKING_API UAndroidXREyeTrackingComponent : public USceneComponent, public IAndroidXRTrackedDataListener
{
    GENERATED_BODY()
public:

    UAndroidXREyeTrackingComponent();
    /**
    * Whether to update the transform of the component with the eye transform
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Eye Tracking")
    bool bUpdateTransform{ true };

    /**
    * The forward offset from the eye position for the component
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Eye Tracking")
    float ForwardOffset{ 250.0f };

    /**
    * The location offset of the component from the eye position
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Eye Tracking")
    FVector LocationOffSet{};

    /**
    * Whether to match the component's rotation with the eye rotation
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Eye Tracking")
    bool bMatchEyeRotation{};

    /**
    * The eye to update the component's transform with
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidXR|Eye Tracking")
    EAndroidXREyeIndex EyeSource{};

    /**
    * The eye data obtained from the AndroidXREyeTracking subsystem
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Eye Tracking")
    FAndroidXREyes EyesState{};

    void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
