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

#include "Components/AndroidXREyeTrackingSubsystem.h"
#include "AndroidXREyeTrackingBlueprintFunctionLibrary.h"

void UAndroidXREyeTrackingSubsystem::CreateTracker()
{
    if(!UAndroidXREyeTrackingBlueprintFunctionLibrary::CreateEyeTracker(EyeTracker))
    {
        return;
    }
    TrackerCreated();
}

void UAndroidXREyeTrackingSubsystem::DestroyTracker()
{
    if(!UAndroidXREyeTrackingBlueprintFunctionLibrary::DestroyEyeTracker(EyeTracker))
    {
        return;
    }
    TrackerDestroyed();
}

void UAndroidXREyeTrackingSubsystem::GetRequiredPermissions(TArray<FString>& Permissions)
{
    Permissions.Add(TEXT("android.permission.EYE_TRACKING_COARSE"));
    Permissions.Add(TEXT("android.permission.EYE_TRACKING_FINE"));
}

bool UAndroidXREyeTrackingSubsystem::GetTrackedData(FAndroidXREyes& Eyes)
{
    if(bIsFineMode)
    {
        return UAndroidXREyeTrackingBlueprintFunctionLibrary::GetFineEyeTrackingInfo(EyeTracker, Eyes);
    }
    else
    {
        return UAndroidXREyeTrackingBlueprintFunctionLibrary::GetCoarseEyeTrackingInfo(EyeTracker, Eyes);
    }
}

void UAndroidXREyeTrackingSubsystem::SetEyeTrackingMode(bool InIsFineMode)
{
    bIsFineMode = InIsFineMode;
}
