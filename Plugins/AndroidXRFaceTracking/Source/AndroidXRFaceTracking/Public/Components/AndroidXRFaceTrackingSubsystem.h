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
#include "AndroidXRTrackingSubsystem.h"
#include "AndroidXRTypes.h"
#include "AndroidXRFaceTrackingSubsystem.generated.h"

/**
* AndroidXR Face Tracking Subsystem
*/
UCLASS()
class ANDROIDXRFACETRACKING_API UAndroidXRFaceTrackingSubsystem : public UAndroidXRTrackingSubsystem, public IAndroidXRTrackedDataProvider<FAndroidXRFaceState, bool>
{
    GENERATED_BODY()
public:

    /**
    * Gets the blend shapes of a facial expression
    * @param[out] The face tracking state and facial expressions
    * @param[out] The face calibration status
    * @return Whether the state was able to be obtained successfully
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Face Tracking")
    bool GetTrackedData(FAndroidXRFaceState& TrackedData, bool& CalibrationStatus) override;

protected:
    void CreateTracker() override;
    void DestroyTracker() override;
    void GetRequiredPermissions(TArray<FString>& Permissions) override;

private:
    FAndroidXRFaceTracker FaceTracker{};
};