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
#include "AndroidXRTypes.h"
#include "AndroidXRRecommendedResolutionTypes.generated.h"

UDELEGATE(BlueprintCallable)
// Broadcast via the AndroidXRRecommendedResolutionEventProxy in the event of
// XR_TYPE_EVENT_DATA_RECOMMENDED_RESOLUTION_CHANGED_ANDROID
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAndroidXRRecommendedResolutionChangedDynamicDelegate,
    FVector2D, RecommendedResolution,
    FVector2D, MaxResolution);