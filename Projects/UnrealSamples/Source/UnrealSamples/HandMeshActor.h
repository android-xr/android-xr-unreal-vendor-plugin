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
#include "GameFramework/Actor.h"
#include "HeadMountedDisplayTypes.h"
#include "HandMeshActor.generated.h"

class UPoseableMeshComponent;

UCLASS()
class UNREALSAMPLES_API AHandMeshActor : public AActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UPoseableMeshComponent> PoseableMeshComponent;

#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 8)
    FXRMotionControllerState MotionControllerData{};
#else
    FXRMotionControllerData MotionControllerData{};
#endif

    // NOTE that this material must have bUsedWithSkeletalMesh set to true
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UMaterialInterface> Material;

    // Set this to either Right or Left to get the joints
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn))
    EControllerHand ControllerHand = EControllerHand::Right;

    void BeginPlay() override;

public:
    AHandMeshActor();

    UFUNCTION(BlueprintCallable)
    void CreateHandMesh();

    UFUNCTION(BlueprintCallable)
    void SetHandMeshScale(float Scale);

    void Tick(float DeltaTime) override;
};
