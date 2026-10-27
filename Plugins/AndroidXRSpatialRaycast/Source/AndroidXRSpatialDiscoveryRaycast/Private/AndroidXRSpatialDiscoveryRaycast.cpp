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

#include "AndroidXRSpatialDiscoveryRaycast.h"
#include <IXRTrackingSystem.h>
#include <IHeadMountedDisplay.h>
#include "IOpenXRHMD.h"
#include "IAndroidXRSpatialPlaneTrackingModule.h"

DEFINE_LOG_CATEGORY(LogAndroidXRSpatialDiscoveryRaycast)

#define LOG_IF_OPENXR_ERROR_AND_RETURN(Tag, Function, Result, ReturnVal) if(!XR_UNQUALIFIED_SUCCESS(Result)){ UE_LOG(LogAndroidXRSpatialDiscoveryRaycast, Error, TEXT("%s : %s failed with error %s"), TEXT(#Tag), TEXT(#Function), OpenXRResultToString(Result)); return ReturnVal;}

void FAndroidXRSpatialDiscoveryRaycast::StartupModule()
{
    IAndroidXRSpatialDiscoveryRaycastModule::StartupModule();
    RegisterOpenXRExtensionModularFeature();
}

void FAndroidXRSpatialDiscoveryRaycast::ShutdownModule()
{
    UnregisterOpenXRExtensionModularFeature();
    IAndroidXRSpatialDiscoveryRaycastModule::ShutdownModule();
}

void FAndroidXRSpatialDiscoveryRaycast::PostCreateInstance(XrInstance InInstance)
{
    Instance = InInstance;

    ResolveOpenXRFunctions(Instance,
                           {
                               RAYCASTDISCOVERY_FUNCTIONS(RESOLVE_OPENXR_FUNC)
                           },
                           [](const char* UnresolvableFunctionName, XrResult Result)
    {
        UE_LOG(LogAndroidXRSpatialDiscoveryRaycast, Error,
               TEXT("Unable to resolve function pointer %s (error %s)"),
               UTF8_TO_TCHAR(UnresolvableFunctionName),
               OpenXRResultToString(Result));
    });
}

bool FAndroidXRSpatialDiscoveryRaycast::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
    OutExtensions.Add(XR_ANDROID_SPATIAL_DISCOVERY_RAYCAST_EXTENSION_NAME);
    return true;
}

bool FAndroidXRSpatialDiscoveryRaycast::IsRaycastDiscoverySupported(ESpatialCapability Capability) const
{
    return IAndroidXRSpatialPlaneTrackingModule::IsAvailable() && IAndroidXRSpatialPlaneTrackingModule::Get().GetNativeCapabilitiesInfo().IsComponentSupported(AndroidXR::Convert(Capability), XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_RAYCAST_RESULT_ANDROID);
}

bool FAndroidXRSpatialDiscoveryRaycast::RaycastForEntities(const FSpatialContext& SpatialContext, const FSpatialRaycastConfig& RaycastConfig, TArray<FSpatialRaycastEntityResult>& FoundEntities)
{
    auto WorldToMetersScale = GEngine->XRSystem.Get()->GetWorldToMetersScale();
    auto Handler = [&FoundEntities](XrSpatialEntityIdEXT EntityId, FSpatialRaycastHitInfo& RaycastData)
    {
        FoundEntities.Add(
            FSpatialRaycastEntityResult
            {
                .HitInfo = RaycastData,
                .EntityId = AndroidXR::Convert(EntityId)
            }
        );
    };
    if(!FindEntitiesByRaycast(SpatialContext, RaycastConfig, Handler))
    {
        return false;
    }
    return true;
}

bool FAndroidXRSpatialDiscoveryRaycast::RaycastForPlanes(const FSpatialRaycastConfig& RaycastConfig, TArray<FSpatialRaycastPlaneResult>& FoundPlanes)
{
    if(!IAndroidXRSpatialPlaneTrackingModule::IsAvailable() || !IAndroidXRSpatialPlaneTrackingModule::Get().IsReady())
    {
        UE_LOG(LogAndroidXRSpatialDiscoveryRaycast, Error, TEXT("Plane tracking module is not ready"));
        return false;
    }
    auto PlaneContext = IAndroidXRSpatialPlaneTrackingModule::Get().GetSpatialContext();
    auto Handler = [&FoundPlanes](XrSpatialEntityIdEXT EntityId, FSpatialRaycastHitInfo& RaycastData)
    {
        auto PlaneEntity = IAndroidXRSpatialPlaneTrackingModule::Get().GetSpatialEntity(EntityId);
        if(PlaneEntity)
        {
            FoundPlanes.Add(FSpatialRaycastPlaneResult
                            {
                                .HitInfo = RaycastData,
                                .HitPlane = PlaneEntity
                            });
        }
    };
    if(!FindEntitiesByRaycast(PlaneContext, RaycastConfig, Handler))
    {
        return false;
    }
    return true;
}

bool FAndroidXRSpatialDiscoveryRaycast::FindEntitiesByRaycast(const FSpatialContext& SpatialContext, const FSpatialRaycastConfig& RaycastConfig, TFunctionRef<void(XrSpatialEntityIdEXT, FSpatialRaycastHitInfo&)> IdHandler)
{
    auto HMD = GEngine->XRSystem.Get()->GetIOpenXRHMD();
    auto WorldToMetersScale = GEngine->XRSystem.Get()->GetWorldToMetersScale();
    XrSpatialRaycastInfoANDROID RaycastInfo
    {
        .type = XR_TYPE_SPATIAL_RAYCAST_INFO_ANDROID,
        .space = HMD->GetTrackingSpace(),
        .time = HMD->GetDisplayTime(),
        .origin = ToXrVector(RaycastConfig.RayOrigin, WorldToMetersScale),
        .direction = ToXrVector(RaycastConfig.RayDirection, WorldToMetersScale),
        .maxDistance = RaycastConfig.RayLength / WorldToMetersScale
    };

    XrSpatialRaycastSnapshotCreateInfoANDROID CreateInfo
    {
        .type = XR_TYPE_SPATIAL_RAYCAST_SNAPSHOT_CREATE_INFO_ANDROID,
        .raycastInfo = &RaycastInfo,
    };

    XrSpatialSnapshotEXT Snapshot{XR_NULL_HANDLE};
    auto Result = xrCreateSpatialRaycastSnapshotANDROID(SpatialContext.NativeContext, &CreateInfo, &Snapshot);

    ON_SCOPE_EXIT
    {
       if(Snapshot == XR_NULL_HANDLE)
       {
           return;
       }
       Result = xrDestroySpatialSnapshotEXT(Snapshot);
       if(!XR_UNQUALIFIED_SUCCESS(Result))
       {
          LOG_IF_OPENXR_ERROR_AND_RETURN(FindEntitiesByRaycast, xrDestroySpatialSnapshotEXT, Result,);
       }
    };

    LOG_IF_OPENXR_ERROR_AND_RETURN(RaycastForEntities, xrCreateSpatialRaycastSnapshotANDROID, Result, false);

    TArray<XrSpatialComponentTypeEXT> QueryComponents
    {
        XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_RAYCAST_RESULT_ANDROID
    };
    XrSpatialComponentDataQueryConditionEXT QueryCondition
    {
        .type = XR_TYPE_SPATIAL_COMPONENT_DATA_QUERY_CONDITION_EXT,
        .componentTypeCount = 1,
        .componentTypes = QueryComponents.GetData()
    };

    XrSpatialComponentDataQueryResultEXT QueryResult
    {
        .type = XR_TYPE_SPATIAL_COMPONENT_DATA_QUERY_RESULT_EXT
    };

    Result = xrQuerySpatialComponentDataEXT(Snapshot, &QueryCondition, &QueryResult);
    LOG_IF_OPENXR_ERROR_AND_RETURN(RaycastForEntities, xrQuerySpatialComponentDataEXT_FetchCount, Result, false);
    TArray<XrSpatialEntityIdEXT> FoundEntities{};
    FoundEntities.SetNum(QueryResult.entityIdCountOutput);

    TArray<XrSpatialEntityTrackingStateEXT> EntityTrackingStates{};
    EntityTrackingStates.SetNum(QueryResult.entityStateCountOutput);

    QueryResult.entityIdCapacityInput = QueryResult.entityIdCountOutput;
    QueryResult.entityStateCapacityInput = QueryResult.entityStateCountOutput;
    QueryResult.entityIds = FoundEntities.GetData();
    QueryResult.entityStates = EntityTrackingStates.GetData();

    QueryCondition.componentTypes = QueryComponents.GetData();
    QueryCondition.componentTypeCount = 1;

    RaycastComponents.Allocate(FoundEntities.Num());
    QueryResult.next = RaycastComponents.GetNativeHeader();

    Result = xrQuerySpatialComponentDataEXT(Snapshot, &QueryCondition, &QueryResult);
    LOG_IF_OPENXR_ERROR_AND_RETURN(RaycastForEntities, xrQuerySpatialComponentDataEXT, Result, false);
    XrSpatialRaycastResultDataANDROID RaycastData{};
    FSpatialRaycastHitInfo HitInfo{};
    for(auto EntityIndex = 0; EntityIndex < FoundEntities.Num(); EntityIndex++)
    {
        if(RaycastComponents.Get(EntityIndex, RaycastData))
        {
            HitInfo.DistanceSquared = RaycastData.distanceSquared;
            HitInfo.HitTransform = ToFTransform(RaycastData.hitPose, WorldToMetersScale);
            IdHandler(FoundEntities[EntityIndex], HitInfo);
        }
    }

    Result = xrDestroySpatialSnapshotEXT(Snapshot);
    LOG_IF_OPENXR_ERROR_AND_RETURN(RaycastForEntities, xrDestroySpatialSnapshotEXT, Result, false);
    Snapshot = XR_NULL_HANDLE;
    return true;
}

IMPLEMENT_MODULE(FAndroidXRSpatialDiscoveryRaycast, AndroidXRSpatialDiscoveryRaycast)