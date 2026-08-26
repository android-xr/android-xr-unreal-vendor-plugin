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

#include "AndroidXRRecommendedResolutionTypes.h"
#include "AndroidXRRecommendedResolutionEventProxy.generated.h"

UCLASS(NotBlueprintable)
class ANDROIDXRRECOMMENDEDRESOLUTION_API UAndroidXRRecommendedResolutionEventProxy : public UObject
{
    GENERATED_BODY()
public:
    /**
     * Triggered when a new recommended resolution is provided by the runtime.
     * @param[out] RecommendedResolution The current recommended resolution.
     * @param[out] MaxResolution The maximum supported resolution.
     */
    UPROPERTY(BlueprintAssignable, Category="AndroidXR|Recommended Resolution Event")
    FAndroidXRRecommendedResolutionChangedDynamicDelegate OnRecommendedResolutionChanged;

    static inline UAndroidXRRecommendedResolutionEventProxy* EventProxy{};

    static UAndroidXRRecommendedResolutionEventProxy* GetInstance();
};
