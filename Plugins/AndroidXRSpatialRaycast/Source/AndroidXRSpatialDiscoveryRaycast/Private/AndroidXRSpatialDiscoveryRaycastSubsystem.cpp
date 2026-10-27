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

#include "AndroidXRSpatialDiscoveryRaycastSubsystem.h"
#include "IAndroidXRSpatialDiscoveryRaycastModule.h"

void UAndroidXRSpatialDiscoveryRaycastSubsystem::GetRequiredPermissions(TArray<FString>& Permissions)
{
    Permissions.Add(TEXT("android.permission.SCENE_UNDERSTANDING_COARSE"));
    Permissions.Add(TEXT("android.permission.SCENE_UNDERSTANDING_FINE"));
}

bool UAndroidXRSpatialDiscoveryRaycastSubsystem::RaycastForEntities(const FSpatialContext& SpatialContext, const FSpatialRaycastConfig& RaycastConfig, TArray<FSpatialRaycastEntityResult>& FoundEntities)
{
    return IAndroidXRSpatialDiscoveryRaycastModule::IsAvailable() && IAndroidXRSpatialDiscoveryRaycastModule::Get().RaycastForEntities(SpatialContext, RaycastConfig, FoundEntities);
}

bool UAndroidXRSpatialDiscoveryRaycastSubsystem::RaycastForPlanes(const FSpatialRaycastConfig& RaycastConfig, TArray<FSpatialRaycastPlaneResult>& FoundPlanes)
{
    return IAndroidXRSpatialDiscoveryRaycastModule::IsAvailable() && IAndroidXRSpatialDiscoveryRaycastModule::Get().RaycastForPlanes(RaycastConfig, FoundPlanes);
}

bool UAndroidXRSpatialDiscoveryRaycastSubsystem::IsRaycastDiscoverySupported(ESpatialCapability Capability) const
{
    return IAndroidXRSpatialDiscoveryRaycastModule::IsAvailable() && IAndroidXRSpatialDiscoveryRaycastModule::Get().IsRaycastDiscoverySupported(Capability);
}