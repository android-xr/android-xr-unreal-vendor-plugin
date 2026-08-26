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
#include "AndroidXRGlobalPassthroughDimmingEventProxy.generated.h"

UDELEGATE(BlueprintCallable)
// Broadcast via the AndroidXRGlobalPassthroughDimmingEventProxy in the event of
// XR_TYPE_EVENT_DATA_GLOBAL_DIMMING_LEVEL_CHANGED_ANDROID
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAndroidXRGlobalDimmingLevelChangedDynamicDelegate,
    float, DimmingLevel);

UCLASS(NotBlueprintable)
class ANDROIDXRGLOBALPASSTHROUGHDIMMING_API UAndroidXRGlobalPassthroughDimmingEventProxy : public UObject
{
    GENERATED_BODY()
public:
    /**
     * Triggered when the dimming level is changed by either apps or physical buttons on devices.
     * @param[out] DimmingLevel The current dimming level.
     */
    UPROPERTY(BlueprintAssignable, Category = "AndroidXR| Global Passthrough Dimming")
    FAndroidXRGlobalDimmingLevelChangedDynamicDelegate OnGlobalDimmingLevelChanged;

    static inline UAndroidXRGlobalPassthroughDimmingEventProxy* EventProxy{};

    static UAndroidXRGlobalPassthroughDimmingEventProxy* GetInstance();
};