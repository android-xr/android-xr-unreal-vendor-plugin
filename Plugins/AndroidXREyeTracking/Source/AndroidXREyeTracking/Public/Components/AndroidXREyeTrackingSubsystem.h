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
#include "AndroidXREyeTrackingTypes.h"
#include "AndroidXREyeTrackingSubsystem.generated.h"

/**
* AndroidXR Eye Tracking Subsystem
*/
UCLASS()
class ANDROIDXREYETRACKING_API UAndroidXREyeTrackingSubsystem : public UAndroidXRTrackingSubsystem, public IAndroidXRTrackedDataProvider<FAndroidXREyes>
{
    GENERATED_BODY()
public:

    /**
    * Get the tracked eye data
    * @params[out] The eye data obtained from the eye tracking subsystem
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Eye Tracking")
    bool GetTrackedData(FAndroidXREyes& FineEyeData) override;

    /**
    * Set the eye tracking mode
    * @params[in] Whether to use fine mode for fetching the eye data
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Eye Tracking")
    void SetEyeTrackingMode(bool InIsFineMode);

protected:
    void CreateTracker() override;
    void DestroyTracker() override;
    void GetRequiredPermissions(TArray<FString>& Permissions) override;

private:
    FAndroidXREyeTracker EyeTracker{};
    bool bIsFineMode{ true };
};
