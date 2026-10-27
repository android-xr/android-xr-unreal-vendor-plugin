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
#include "IOpenXRExtensionPlugin.h"
#include "IAndroidXRSpatialDiscoveryRaycastModule.h"
#include <AndroidXRHelpers.h>

DECLARE_LOG_CATEGORY_EXTERN(LogAndroidXRSpatialDiscoveryRaycast, Log, All);

class ANDROIDXRSPATIALDISCOVERYRAYCAST_API FAndroidXRSpatialDiscoveryRaycast : public IOpenXRExtensionPlugin, public IAndroidXRSpatialDiscoveryRaycastModule
{
public:
    /** IModuleInterface */
    void StartupModule() override;
    void ShutdownModule() override;

    /** IOpenXRExtensionPlugin */
    FString GetDisplayName() override
    {
        return FString(TEXT("AndroidXRSpatialDiscoveryRaycast"));
    }

    void PostCreateInstance(XrInstance InInstance) override;
    bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
    bool IsRaycastDiscoverySupported(ESpatialCapability Capability) const override;
    bool RaycastForEntities(const FSpatialContext& SpatialContext, const FSpatialRaycastConfig& RaycastConfig, TArray<FSpatialRaycastEntityResult>& FoundEntities) override;
    bool RaycastForPlanes(const FSpatialRaycastConfig& RaycastConfig, TArray<FSpatialRaycastPlaneResult>& FoundPlanes) override;

private:
    XrSession Session{};
    XrInstance Instance{};
    TFSpatialComponentList<XrSpatialComponentRaycastResultListANDROID, XrSpatialRaycastResultDataANDROID> RaycastComponents{ XR_TYPE_SPATIAL_COMPONENT_RAYCAST_RESULT_LIST_ANDROID };

    bool FindEntitiesByRaycast(const FSpatialContext& SpatialContext, const FSpatialRaycastConfig& RaycastConfig,TFunctionRef<void(XrSpatialEntityIdEXT,FSpatialRaycastHitInfo&)> IdHandler);

#define RAYCASTDISCOVERY_FUNCTIONS(HelperMacro) \
        HelperMacro(xrCreateSpatialRaycastSnapshotANDROID) \
        HelperMacro(xrQuerySpatialComponentDataEXT) \
        HelperMacro(xrDestroySpatialSnapshotEXT)

    RAYCASTDISCOVERY_FUNCTIONS(DECLARE_OPENXR_FUNC);
};
