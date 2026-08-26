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
#include "AndroidXRTrackingSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTrackerCreated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTrackerDestroyed);

/**
* The base class for all AndroidXR tracking subsystems.
*/
UCLASS(Abstract)
class ANDROIDXRTRACKINGSUBSYSTEM_API UAndroidXRTrackingSubsystem : public UEngineSubsystem
{
    GENERATED_BODY()
public:

    /**
    * Start tracking the associated feature
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Tracking Subsystem")
    void StartTracking();

    /**
    * Stop tracking the associated feature
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Tracking Subsystem")
    void StopTracking();

    /**
    * Stop tracking the associated feature
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Tracking Subsystem")
    bool IsTracking() const;

    /**
    * The callback for when the system has started tracking
    */
    UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Tracking Subsystem")
    FOnTrackerCreated OnTrackingStarted{};

    /**
    * The callback for when the underlying tracker is destroyed
    */
    UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Tracking Subsystem")
    FOnTrackerDestroyed OnTrackingStopped{};

protected:

    void TrackerCreated();
    void TrackerDestroyed();

    virtual void CreateTracker()
    {
        TrackerCreated();
    }

    virtual void DestroyTracker()
    {
        TrackerDestroyed();
    }
    virtual void GetRequiredPermissions(TArray<FString>& Permissions)
    {
    }
private:
    void RequestPermissions(TArray<FString>& Permissions);
    void OnPermissionsGranted(const TArray<FString>& Permissions, const TArray<bool>& Status);
    bool bIsTracking{};
    bool bWaitingForPermissions{};
    bool bPermissionsGranted{};
};

template<typename ...TTrackedData>
class IAndroidXRTrackedDataProvider
{
public:
    virtual bool GetTrackedData(TTrackedData&... TrackedData) = 0;
};

class IAndroidXRTrackedDataListener
{
public:
    template<typename TTrackingSubsystem, typename ...TTrackedData>
    bool GetTrackedData(TTrackedData&... TrackedData)
    {
        auto Subsystem = GEngine->GetEngineSubsystem<TTrackingSubsystem>();
        if(!Subsystem)
        {
            return false;
        }
        return Subsystem->GetTrackedData(TrackedData...);
    }

    template<typename TTrackingSubsystem>
    bool CanFetchTrackedData()
    {
        auto Subsystem = GetSubsystem<TTrackingSubsystem>();
        if(!Subsystem)
        {
            return false;
        }
        return Subsystem->IsTracking();
    }
private:
    template<typename TTrackingSubsystem>
    TTrackingSubsystem* GetSubsystem()
    {
        return GEngine->GetEngineSubsystem<TTrackingSubsystem>();
    }
};