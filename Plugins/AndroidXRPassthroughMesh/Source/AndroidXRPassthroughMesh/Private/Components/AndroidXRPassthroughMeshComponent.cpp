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

#include "Components/AndroidXRPassthroughMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

UAndroidXRPassthroughMeshComponent::UAndroidXRPassthroughMeshComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bAutoActivate = true;
}

void UAndroidXRPassthroughMeshComponent::BeginPlay()
{
    Super::BeginPlay();
    auto Subsystem = GEngine->GetEngineSubsystem<UAndroidXRPassthroughMeshSubsystem>();
    if(!Subsystem)
    {
        return;
    }
    if(Subsystem->IsTracking())
    {
        CreatePassthroughLayer();
    }
    else
    {
        Subsystem->OnTrackingStarted.AddUniqueDynamic(this, &UAndroidXRPassthroughMeshComponent::CreatePassthroughLayer);
    }
}

void UAndroidXRPassthroughMeshComponent::EndPlay(EEndPlayReason::Type EndplayReason)
{
    Super::EndPlay(EndplayReason);
    DestroyPassthroughLayer();
    auto Subsystem = GEngine->GetEngineSubsystem<UAndroidXRPassthroughMeshSubsystem>();
    if(!Subsystem)
    {
        return;
    }
    Subsystem->OnTrackingStarted.RemoveDynamic(this, &UAndroidXRPassthroughMeshComponent::CreatePassthroughLayer);
}

void UAndroidXRPassthroughMeshComponent::CreatePassthroughLayer()
{
    if(bLayerCreated)
    {
        return;
    }
    CreateMeshInfoFromStaticMesh();
    bLayerCreated = UAndroidXRPassthroughMeshBlueprintFunctionLibrary::CreatePassthroughLayer(MeshInfo, PassthroughLayer);
}

void UAndroidXRPassthroughMeshComponent::DestroyPassthroughLayer()
{
    UAndroidXRPassthroughMeshBlueprintFunctionLibrary::DestroyPassthroughLayer(PassthroughLayer);
    bLayerCreated = false;
}

void UAndroidXRPassthroughMeshComponent::CreateMeshInfoFromStaticMesh()
{
    MeshInfo.Transform = GetComponentTransform();
    MeshInfo.Opacity = Opacity;
    TArray<FVector> Normals{};
    TArray<FVector2D> UVs{};
    TArray<FProcMeshTangent> Tangents{};
    UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(GetStaticMesh(), 0, 0, MeshInfo.Vertices, MeshInfo.Indices, Normals, UVs, Tangents);
}

void UAndroidXRPassthroughMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(!bLayerCreated)
    {
        return;
    }
    UAndroidXRPassthroughMeshBlueprintFunctionLibrary::UpdatePassthroughLayerTransform(PassthroughLayer, GetComponentTransform());
}

void UAndroidXRPassthroughMeshComponent::SetPassthroughOpacity(float InOpacity)
{
    if(!bLayerCreated)
    {
        return;
    }
    Opacity = InOpacity;
    UAndroidXRPassthroughMeshBlueprintFunctionLibrary::UpdatePassthroughLayerOpacity(PassthroughLayer, Opacity);
}

void UAndroidXRPassthroughMeshComponent::UpdatePassthroughMesh()
{
    if(!bLayerCreated)
    {
        return;
    }
    CreateMeshInfoFromStaticMesh();
    UAndroidXRPassthroughMeshBlueprintFunctionLibrary::UpdatePassthroughLayerMesh(PassthroughLayer, MeshInfo.Vertices, MeshInfo.Indices);
}
