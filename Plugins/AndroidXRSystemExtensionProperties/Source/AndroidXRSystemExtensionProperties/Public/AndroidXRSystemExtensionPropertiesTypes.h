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

#include <CoreMinimal.h>
#include <OpenXRCore.h>
#include "openxr_delta.h"
#include "AndroidXRTypes.h"
#include "AndroidXRSystemExtensionPropertiesTypes.generated.h"

/**
 * System Extension Properties.
 */
USTRUCT(BlueprintType)
struct FAndroidXRSystemExtensionInformation
{
    GENERATED_BODY()

    // Extension name.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AndroidXR|System Extension Properties")
    FString ExtensionName{};

    // Extension version.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AndroidXR|System Extension Properties")
    int32 ExtensionVersion{};

    // Indicates whether the extension is currently supported by the system.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|System Extension Properties")
    bool bIsSupported{};
};

UDELEGATE(BlueprintCallable)
// Broadcast via the AndroidXRSystemExtensionProperties in the event of
// XR_TYPE_EVENT_DATA_SYSTEM_PROPERTIES_CHANGED_ANDROID
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAndroidXRSystemPropertiesChangedDynamicDelegate,
    const TArray<FAndroidXRSystemExtensionInformation>&, ExtensionProperties);