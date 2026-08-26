
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

#include "PermissionsRequester.h"
#include "AndroidPermissionFunctionLibrary.h"
#include "AndroidPermissionCallbackProxy.h"
#include "UnrealSamplesFunctionLibrary.h"

void UPermissionsRequester::OnPermissionGrantedHandler(const TArray<FString>& Permissions, const TArray<bool>& Status)
{
    if(UUnrealSamplesFunctionLibrary::IsPlatformAndroid())
    {
        auto Proxy = UAndroidPermissionCallbackProxy::GetInstance();
        if(Proxy)
        {
            Proxy->OnPermissionsGrantedDelegate.Remove(PermissionHandle);
        }
    }
    PermissionsTable.Reset();
    for (auto i = 0; i < Permissions.Num(); i++)
    {
        PermissionsTable.Add(Permissions[i], Status[i]);
    }
    OnPermissionsGranted.Broadcast(this);
}

bool UPermissionsRequester::IsPermissionGranted(const FString& Permission)
{
    auto PermissionPtr = PermissionsTable.Find(Permission);
    return PermissionPtr && *PermissionPtr;
}

bool UPermissionsRequester::AreAllPermissionsGranted(const TArray<FString>& Permissions)
{
    for (const auto& Permission : Permissions)
    {
        if (!IsPermissionGranted(Permission))
        {
            return false;
        }
    }
    return true;
}

void UPermissionsRequester::RequestPermissions(const TArray<FString>& Permissions)
{
    if (UUnrealSamplesFunctionLibrary::IsPlatformAndroid())
    {
        auto Proxy = UAndroidPermissionCallbackProxy::GetInstance();
        if(Proxy)
        {
            PermissionHandle = Proxy->OnPermissionsGrantedDelegate.AddUObject(this, &UPermissionsRequester::OnPermissionGrantedHandler);
            UAndroidPermissionFunctionLibrary::AcquirePermissions(Permissions);
        }
    }
    else
    {
        TArray<bool> Status{};
        Status.Init(true, Permissions.Num());
        OnPermissionGrantedHandler(Permissions, Status);
    }
}
