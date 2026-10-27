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

#include "AndroidXRStreamingRuntime.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

DEFINE_LOG_CATEGORY(LogAndroidXRStreamingRuntime)

UAndroidXRStreamingRuntime::UAndroidXRStreamingRuntime()
{
	if (FetchAllAvailableRuntimes())
	{
		auto AndroidRuntimeName = AndroidXRRuntimeName;
		auto AndroidXRRuntimePtr = Algo::FindByPredicate(AvailableRuntimes, [AndroidRuntimeName](const FOpenXRRuntimeInfo& Runtime) {
			return Runtime.Name == AndroidRuntimeName;
			});
		if (AndroidXRRuntimePtr)
		{
			bIsAvailable = true;
			AndroidXRStreamingRuntimeInfo = *AndroidXRRuntimePtr;
			Name = AndroidXRStreamingRuntimeInfo.Name;
			LibraryPath = AndroidXRStreamingRuntimeInfo.LibraryPath;
			RegistryKey = AndroidXRStreamingRuntimeInfo.RegistryKey;
		}
		if (FetchActiveRuntime())
		{
			auto ActiveRuntime = ActiveRuntimeInfo.Name.Len() > 0 ? ActiveRuntimeInfo.Name : ActiveRuntimeInfo.RegistryKey;
			bIsActive = ActiveRuntimeInfo.Name == AndroidRuntimeName;
		}
	}
}

void UAndroidXRStreamingRuntime::EnableRuntime()
{
	if (!bIsAvailable || bIsActive)
	{
		return;
	}
	bIsActive = true;
	FPlatformMisc::SetEnvironmentVar(*OpenXRRuntimeEnvVariable, *RegistryKey);
	FetchActiveRuntime();
}

void UAndroidXRStreamingRuntime::DisableRuntime()
{
	bIsActive = false;
	FPlatformMisc::SetEnvironmentVar(*OpenXRRuntimeEnvVariable, TEXT(""));
	FetchActiveRuntime();
}

bool UAndroidXRStreamingRuntime::FetchAllAvailableRuntimes()
{
	AvailableRuntimes.Reset();
	bool bSuccess = false;
	auto AvailableRuntimesKey = FString::Printf(TEXT("%s\\%s"), *OpenXRRuntimeRegistryKey, *AvailableRuntimesRegistryKey);
	for (int32 RegistryIndex = 0; RegistryIndex < 2 && !bSuccess; ++RegistryIndex)
	{
		HKEY Key = 0;
		const uint32 RegFlags = (RegistryIndex == 0) ? KEY_WOW64_32KEY : KEY_WOW64_64KEY;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, *AvailableRuntimesKey, 0, KEY_READ | RegFlags, &Key) == ERROR_SUCCESS)
		{
			DWORD RuntimeCount{};
			DWORD MaxNameLength{};

			auto Result = RegQueryInfoKey(Key, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &RuntimeCount, &MaxNameLength, nullptr, nullptr, nullptr);
			MaxNameLength++;
			auto CurrentLength = MaxNameLength;
			if (Result == ERROR_SUCCESS)
			{
				TArray<wchar_t> NameBuffer{};
				NameBuffer.SetNum(MaxNameLength);
				for (DWORD i = 0; i < RuntimeCount; i++)
				{
					Result = RegEnumValue(Key, i, NameBuffer.GetData(), &CurrentLength, nullptr, nullptr, nullptr, nullptr);
					if (Result == ERROR_SUCCESS)
					{
						FString CurrentRuntime = FString::ConstructFromPtrSize(NameBuffer.GetData(), CurrentLength);
						FOpenXRRuntimeInfo RuntimeInfo{};
						FetchRuntimeDetails(CurrentRuntime, RuntimeInfo);
						AvailableRuntimes.Add(RuntimeInfo);
					}
					CurrentLength = MaxNameLength;
				}
				/*
				* Was causing parent loop exit after querying the 32-bit registry keys,
				* ignoring the 64-bit runtime libraries (e.g. the Android XR runtime) found in the KEY_WOW64_64KEY pass
				*/
				//bSuccess = true;
			}
			RegCloseKey(Key);
		}
		else bSuccess = true;
	}
	return !AvailableRuntimes.IsEmpty();
}

bool UAndroidXRStreamingRuntime::FetchActiveRuntime()
{
	//There are two ways to set the runtime. First is to check the environment variable, this takes priority over the registry entry
	auto ActiveRuntimeEntry = FPlatformMisc::GetEnvironmentVariable(*OpenXRRuntimeEnvVariable);
	if (ActiveRuntimeEntry.Len() == 0)
	{
		if (!FPlatformMisc::QueryRegKey(HKEY_LOCAL_MACHINE, *OpenXRRuntimeRegistryKey, *ActiveRuntimeRegistryKey, ActiveRuntimeEntry))
		{
			return false;
		}
	}
	auto RuntimePtr = Algo::FindByPredicate(AvailableRuntimes, [ActiveRuntimeEntry](const FOpenXRRuntimeInfo& RuntimeInfo) {
		return RuntimeInfo.RegistryKey == ActiveRuntimeEntry;
		});
	if (RuntimePtr)
	{
		ActiveRuntimeInfo = *RuntimePtr;
		return true;
	}
	UE_LOG(LogAndroidXRStreamingRuntime, Error, TEXT("Unable to fetch the active OpenXR runtime"));
	return false;
}

void UAndroidXRStreamingRuntime::FetchRuntimeDetails(const FString& Runtime, FOpenXRRuntimeInfo& RuntimeInfo)
{
	FString FileStr{};
	RuntimeInfo.RegistryKey = Runtime;
	if (FFileHelper::LoadFileToString(FileStr, *Runtime))
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(FileStr);
		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			auto RuntimeVal = JsonObject->GetField(TEXTVIEW("runtime"), EJson::Object);
			auto RuntimeObj = RuntimeVal->AsObject();
			RuntimeObj->TryGetStringField(TEXTVIEW("name"), RuntimeInfo.Name);
			RuntimeObj->TryGetStringField(TEXTVIEW("library_path"), RuntimeInfo.LibraryPath);
		}
	}
}

