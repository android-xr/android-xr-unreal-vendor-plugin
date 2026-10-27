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
#include "AndroidXRViewportFeatheringTypes.generated.h"

/**
 * The trackables object
 */
USTRUCT(BlueprintType)
struct FAndroidXRViewportFeatheringInsetInfo
{
    GENERATED_BODY()

    //  The number of pixels on the left side of the view that is affected by feathering.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|Viewport Feathering")
    int32 InsetLeft{};

    //  The number of pixels on the right side of the view that is affected by feathering.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AndroidXR|Viewport Feathering")
    int32 InsetRight{};

    //  The number of pixels on the top side of the view that is affected by feathering.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AndroidXR|Viewport Feathering")
    int32 InsetTop{};

    //  The number of pixels on the bottom side of the view that is affected by feathering.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AndroidXR|Viewport Feathering")
    int32 InsetBottom{};
};