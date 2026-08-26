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
#include "AndroidXRPassthroughMeshSubsystem.h"
#include "AndroidXRPassthroughMeshBlueprintFunctionLibrary.h"
#include "AndroidXRPassthroughMeshComponent.generated.h"

/**
* AndroidXRPassthrough static mesh component
*/
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ANDROIDXRPASSTHROUGHMESH_API UAndroidXRPassthroughMeshComponent : public UStaticMeshComponent, public IAndroidXRTrackedDataListener
{
    GENERATED_BODY()
public:

    UAndroidXRPassthroughMeshComponent();

    /**
    * The opacity of the passthrough mesh
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AndroidXR|Passthrough Mesh")
    float Opacity{ 1.0f };

    /**
    * Updates the opacity of the passthrough mesh
    * @param[in] The new opacity of the passthrough mesh
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Passthrough Mesh")
    void SetPassthroughOpacity(float InOpacity);

    /**
    * Updates the passthrough static mesh
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Passthrough Mesh")
    void UpdatePassthroughMesh();

    void BeginPlay() override;
    void EndPlay(EEndPlayReason::Type EndplayReason) override;
    void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UFUNCTION()
    void CreatePassthroughLayer();
    UFUNCTION()
    void DestroyPassthroughLayer();
    void CreateMeshInfoFromStaticMesh();

    FAndroidXRPassthroughLayer PassthroughLayer{};
    FAndroidXRPassthroughLayerMeshInfo MeshInfo{};
    bool bLayerCreated{};
};
