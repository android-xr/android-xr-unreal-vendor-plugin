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
#include "AndroidXREyeTrackingTypes.generated.h"

/**
* The index of the eye
*/
UENUM(BlueprintType)
enum class EAndroidXREyeIndex : uint8
{
    // Left Eye Index
    Left = 0,
    // Right Eye Index
    Right = 1
};

/**
* The state of the eye data
*/
UENUM(BlueprintType)
enum class EAndroidXREyeState : uint8
{
    // The eye data is invalid
    Invalid = 0,
    // The eye is gazing
    Gazing = 1,
    // The eye is shut
    Shut = 2
};

/**
* The eye tracking mode
*/
UENUM(BlueprintType)
enum class EAndroidXREyeTrackingMode : uint8
{
    // Eyes are not being tracked
    Not_Tracking = 0,
    // The right eye is being tracked
    Right = 1,
    // The left eye is being tracked
    Left = 2,
    // Both eyes are being tracked
    Both = 3
};

/**
* Represents an eye tracker
*/
USTRUCT(BlueprintType)
struct FAndroidXREyeTracker
{
    GENERATED_BODY()

    // The native eye tracker
    XrEyeTrackerANDROID EyeTracker{ XR_NULL_HANDLE };
};

/**
* An eye data obtained from eye tracking
*/
USTRUCT(BlueprintType)
struct FAndroidXREye
{
    GENERATED_BODY()

    // The state of the eye
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AndroidXR|Eye Tracking")
    EAndroidXREyeState EyeState{};

    // The transform of the eye
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AndroidXR|Eye Tracking")
    FTransform Transform{};
};

/**
* The eyes data obtained from eye tracking
*/
USTRUCT(BlueprintType)
struct FAndroidXREyes
{
    GENERATED_BODY()

    // All the obtained eye data
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AndroidXR|Eye Tracking")
    TArray<FAndroidXREye> Eyes{};

    // The tracking mode of the eyes
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AndroidXR|Eye Tracking")
    EAndroidXREyeTrackingMode Mode{};
};