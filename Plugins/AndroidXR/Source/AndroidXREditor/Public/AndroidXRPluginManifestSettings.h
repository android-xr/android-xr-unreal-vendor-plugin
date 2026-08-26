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
#include "AndroidXRPluginManifestSettings.generated.h"

USTRUCT()
struct FAndroidXRPluginManifest
{
	GENERATED_BODY()

	UPROPERTY()
	FName PluginName{};

	UPROPERTY()
	TArray<FName> HardwareFeatures{};

	UPROPERTY()
	int SpatialSDKLevel{};
};

UCLASS(Config = AndroidXR, DefaultConfig)
class ANDROIDXREDITOR_API UAndroidXRPluginManifestSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(Config)
	TArray<FAndroidXRPluginManifest> PluginFeatures{};

	TArray<FName> GetRegisteredFeatures() const;
};


UCLASS(Config = Engine, DefaultConfig)
class ANDROIDXREDITOR_API UAndroidXRRuntimeManifestSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(GlobalConfig, EditAnywhere)
	bool bEnablePermissions{};

	UPROPERTY(GlobalConfig, EditAnywhere, meta = (GetOptions = "GetRegisteredHardwareFeatures"))
	TArray<FName> RequiredHardwareFeatures{};

	UPROPERTY(GlobalConfig, EditAnywhere)
	bool bRequireSpatialSDK{};

	UFUNCTION()
	TArray<FName> GetRegisteredHardwareFeatures();
};