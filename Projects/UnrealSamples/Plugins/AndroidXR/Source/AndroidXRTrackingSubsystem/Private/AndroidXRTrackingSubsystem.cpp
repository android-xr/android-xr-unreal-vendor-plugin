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

#include "AndroidXRTrackingSubsystem.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AndroidPermissionFunctionLibrary.h"
#include "AndroidPermissionCallbackProxy.h"

void UAndroidXRTrackingSubsystem::StartTracking()
{
    if(IsTracking())
    {
        //if tracker was already created, fire off the on tracking started event
        TrackerCreated();
        return;
    }
    if(bWaitingForPermissions)
    {
        return;
    }
    if(!bPermissionsGranted)
    {
        bWaitingForPermissions = true;
        TArray<FString> Permissions{};
        GetRequiredPermissions(Permissions);
        RequestPermissions(Permissions);
    }
    else
    {
        CreateTracker();
    }
}

void UAndroidXRTrackingSubsystem::StopTracking()
{
    if(!IsTracking())
    {
        //if already stopped fire the destroyed tracker event
        TrackerDestroyed();
        return;
    }
    DestroyTracker();
}

bool UAndroidXRTrackingSubsystem::IsTracking() const
{
    return !bWaitingForPermissions && bIsTracking;
}

void UAndroidXRTrackingSubsystem::TrackerCreated()
{
    OnTrackingStarted.Broadcast();
    bIsTracking = true;
}

void UAndroidXRTrackingSubsystem::TrackerDestroyed()
{
    OnTrackingStopped.Broadcast();
    bIsTracking = false;
}

void UAndroidXRTrackingSubsystem::RequestPermissions(TArray<FString>& Permissions)
{
#if PLATFORM_ANDROID
    auto PermissionsProxy = UAndroidPermissionFunctionLibrary::AcquirePermissions(Permissions);
    if(PermissionsProxy)
    {
        PermissionsProxy->OnPermissionsGrantedDelegate.AddUObject(this, &UAndroidXRTrackingSubsystem::OnPermissionsGranted);
    }
#else
    TArray<bool> Status{};
    Status.Init(true, Permissions.Num());
    OnPermissionsGranted(Permissions, Status);
    bPermissionsGranted = true;
#endif
}

void UAndroidXRTrackingSubsystem::OnPermissionsGranted(const TArray<FString>& Permissions, const TArray<bool>& Status)
{
    if(bPermissionsGranted)
    {
        return;
    }
    bWaitingForPermissions = false;
    bPermissionsGranted = true;
    for(auto Index = 0; Index < Permissions.Num(); Index++)
    {
        if(!Status[Index])
        {
            bPermissionsGranted = false;
            break;
        }
    }
    if(!bPermissionsGranted)
    {
        return;
    }
    CreateTracker();
}


IMPLEMENT_MODULE(FDefaultModuleImpl, AndroidXRTrackingSubsystem);