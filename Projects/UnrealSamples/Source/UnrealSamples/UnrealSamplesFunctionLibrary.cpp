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

#include "UnrealSamplesFunctionLibrary.h"
#include "Engine/AssetManager.h"
#include <OpenXRBlueprintFunctionLibrary.h>

TArray<FString> UUnrealSamplesFunctionLibrary::GetAllMapNames()
{
    TArray<FString> MapNames;

    const auto& AssetRegistryModule =
        FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
    TArray<FAssetData> WorldAssets;
    AssetRegistryModule.Get().
        GetAssetsByClass(UWorld::StaticClass()->GetClassPathName(), WorldAssets);
    for (const auto& WorldAsset : WorldAssets)
    {
        if (WorldAsset.PackagePath.ToString().StartsWith("/Game"))
        {
            MapNames.Add(WorldAsset.AssetName.ToString());
        }
    }
    return MapNames;
}

FString UUnrealSamplesFunctionLibrary::GetCurrentMapName()
{
    auto MapName = GWorld->GetMapName();
    MapName.RemoveFromStart(GWorld->StreamingLevelsPrefix);
    return MapName;
}

float UUnrealSamplesFunctionLibrary::GetAverageFPS()
{
    extern ENGINE_API float GAverageFPS;
    return GAverageFPS;
}

bool UUnrealSamplesFunctionLibrary::IsPlatformAndroid()
{
#if PLATFORM_ANDROID
    return true;
#else
    return false;
#endif
}

void UUnrealSamplesFunctionLibrary::SetEnvironmentBlendMode(int BlendMode)
{
#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 8)
    EOpenXREnvironmentBlendMode NewBlendMode = (EOpenXREnvironmentBlendMode)BlendMode;
    UOpenXRBlueprintFunctionLibrary::SetEnvironmentBlendMode(NewBlendMode);
#else
    UOpenXRBlueprintFunctionLibrary::SetEnvironmentBlendMode(BlendMode);
#endif
}
