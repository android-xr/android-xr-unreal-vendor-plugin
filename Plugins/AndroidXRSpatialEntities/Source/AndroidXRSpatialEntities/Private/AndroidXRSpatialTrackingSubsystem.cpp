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

#include "AndroidXRSpatialTrackingSubsystem.h"

void UAndroidXRSpatialEntitiesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    UAndroidXRTrackingSubsystem::Initialize(Collection);
    CapabilityModule = GetCapabilityModule();
    if(!CapabilityModule)
    {
        return;
    }
    CapabilityModule->OnSpatialContextCreated.AddUObject(this, &UAndroidXRSpatialEntitiesSubsystem::SpatialContextCreatedHandler);
    CapabilityModule->OnSpatialContextDestroyed.AddUObject(this, &UAndroidXRSpatialEntitiesSubsystem::SpatialContextDestroyedHandler);
}

void UAndroidXRSpatialEntitiesSubsystem::CreateTracker()
{
    if(!CapabilityModule)
    {
        return;
    }
    BindEntityEvents();
    CapabilityModule->Start();
}

void UAndroidXRSpatialEntitiesSubsystem::DestroyTracker()
{
    if(!CapabilityModule)
    {
        return;
    }
    UnbindEntityEvents();
    CapabilityModule->Stop();
}

void UAndroidXRSpatialEntitiesSubsystem::SpatialContextCreatedHandler()
{
    TrackerCreated();
}

void UAndroidXRSpatialEntitiesSubsystem::SpatialContextDestroyedHandler()
{
    TrackerDestroyed();
}

bool UAndroidXRSpatialEntitiesSubsystem::GetSupportedCapabilities(TArray<ESpatialCapability>& Capabilities)
{
    if(!CapabilityModule)
    {
        return false;
    }
    return CapabilityModule->GetSupportedCapabilities(Capabilities);
}

bool UAndroidXRSpatialEntitiesSubsystem::GetCapabilityInfo(ESpatialCapability Capability, FAndroidXRSpatialCapabilityInfo& CapabilityInfo)
{
    if(!CapabilityModule)
    {
        return false;
    }
    return CapabilityModule->GetCapabilityInfo(Capability, CapabilityInfo);
}

bool UAndroidXRSpatialEntitiesSubsystem::CreateDiscoverySnapshotRequest()
{
    if(!CapabilityModule)
    {
        return false;
    }
    return CapabilityModule->CreateDiscoverySnapshotRequest();
}
