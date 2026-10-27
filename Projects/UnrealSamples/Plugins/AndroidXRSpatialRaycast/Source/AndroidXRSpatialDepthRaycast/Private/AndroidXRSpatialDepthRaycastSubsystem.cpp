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

#include "AndroidXRSpatialDepthRaycastSubsystem.h"
#include "IAndroidXRSpatialDepthRaycastModule.h"

bool UAndroidXRSpatialDepthRaycastSubsystem::IsDepthRaycastSupported() const
{
    return IAndroidXRSpatialDepthRaycastModule::IsAvailable() && IAndroidXRSpatialDepthRaycastModule::Get().IsDepthRaycastSupported();
}

void UAndroidXRSpatialDepthRaycastSubsystem::GetRequiredPermissions(TArray<FString>& Permissions)
{
    Permissions.Add(TEXT("android.permission.SCENE_UNDERSTANDING_COARSE"));
    Permissions.Add(TEXT("android.permission.SCENE_UNDERSTANDING_FINE"));
}

FAndroidXRSpatialCapabilityModule* UAndroidXRSpatialDepthRaycastSubsystem::GetCapabilityModule()
{
    if(!IAndroidXRSpatialDepthRaycastModule::IsAvailable())
    {
        return nullptr;
    }
    return &IAndroidXRSpatialDepthRaycastModule::Get();
}

bool UAndroidXRSpatialDepthRaycastSubsystem::DepthRaycastAsync(const FSpatialRaycastConfig& RaycastConfig, TDelegate<void(const FSpatialRaycastHitInfo&)> DepthRaycastCompletedCallback, TDelegate<void()> OnFailedCallback)
{
    if(!IAndroidXRSpatialDepthRaycastModule::IsAvailable())
    {
        OnFailedCallback.ExecuteIfBound();
        return false;
    }
    return IAndroidXRSpatialDepthRaycastModule::Get().DepthRaycastAsync(RaycastConfig, DepthRaycastCompletedCallback, OnFailedCallback);
}

bool UAndroidXRSpatialDepthRaycastSubsystem::CanPerformDepthRaycast() const
{
    return IAndroidXRSpatialDepthRaycastModule::IsAvailable() && IAndroidXRSpatialDepthRaycastModule::Get().CanPerformDepthRaycast();
}

void UAndroidXRSpatialDepthRaycastAsyncAction::Activate()
{
    auto Subsystem = GEngine->GetEngineSubsystem<UAndroidXRSpatialDepthRaycastSubsystem>();
    if(!Subsystem)
    {
        OnDepthRaycastFailed.Broadcast();
        return;
    }
    TDelegate<void(const FSpatialRaycastHitInfo&)> Handler{};
    Handler.BindUObject(this, &UAndroidXRSpatialDepthRaycastAsyncAction::DepthRaycastCompletedHandler);

    TDelegate<void()> FailedHandler{};
    FailedHandler.BindUObject(this, &UAndroidXRSpatialDepthRaycastAsyncAction::DepthRaycastFailedHandler);
    if(!Subsystem->DepthRaycastAsync(RaycastConfig, Handler, FailedHandler))
    {
        DepthRaycastFailedHandler();
    }
}

UAndroidXRSpatialDepthRaycastAsyncAction* UAndroidXRSpatialDepthRaycastAsyncAction::SpatialDepthRaycastAsync(const FSpatialRaycastConfig& InRaycastConfig)
{
    auto DepthRaycastAction = NewObject<UAndroidXRSpatialDepthRaycastAsyncAction>();
    DepthRaycastAction->RaycastConfig = InRaycastConfig;
    return DepthRaycastAction;
}

void UAndroidXRSpatialDepthRaycastAsyncAction::DepthRaycastCompletedHandler(const FSpatialRaycastHitInfo& RaycastHitInfo)
{
    OnDepthRaycastCompleted.Broadcast(RaycastHitInfo);
}

void UAndroidXRSpatialDepthRaycastAsyncAction::DepthRaycastFailedHandler()
{
    OnDepthRaycastFailed.Broadcast();
}
