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

#include "AndroidXRSpatialEntitiesModuleBase.h"
#include <IOpenXRHMDModule.h>
#include <IXRTrackingSystem.h>
#include <IHeadMountedDisplay.h>
#include "IOpenXRHMD.h"

DEFINE_LOG_CATEGORY(LogAndroidXRSpatialEntities);

#define LOG_OPENXR_ERROR(Tag, FunctionName, Result) UE_LOG(LogAndroidXRSpatialEntities, Error, TEXT("%s %s failed with error %s (%d)"), TEXT(#Tag), TEXT(#FunctionName),  OpenXRResultToString(Result), Result);
#define CHECK_OPENXR_STATUS_AND_RETURN(Tag, FunctionName, Result, ReturnType) if(!XR_UNQUALIFIED_SUCCESS(Result)){ LOG_OPENXR_ERROR(Tag, FunctionName, Result); return ReturnType; }
#define CHECK_OPENXR_STATUS_AND_RETURN_BOOL(Tag, FunctionName, Result) CHECK_OPENXR_STATUS_AND_RETURN(Tag, FunctionName, Result, false);
#define LOG_IF_OPENXR_ERROR(Tag, FunctionName, Result) if(!XR_UNQUALIFIED_SUCCESS(Result)){ LOG_OPENXR_ERROR(Tag, FunctionName, Result);}

void FAndroidXRSpatialCapabilityModule::Start()
{
    if(bWaitingForSpatialContextCreation || bSpatialContextCreated)
    {
        return;
    }
    bWaitingForSpatialContextCreation = true;
    SubmitPreSpatialContextFutures(PreSpatialContextFutures);
    if(PreSpatialContextFutures.ShouldPoll())
    {
        //if there are tasks to be completed:
        PreSpatialContextFutures.OnAllFuturesCompleted.BindLambda([this]()
        {
            CreateSpatialContext();
        });
    }
    else
    {
        CreateSpatialContext();
    }
}

void FAndroidXRSpatialCapabilityModule::Stop()
{
    PreSpatialContextDestroy();
    ActiveFutures.CancelAll();
    DestroySpatialContext();
}

void FAndroidXRSpatialCapabilityModule::StartupModule()
{
    RegisterOpenXRExtensionModularFeature();
}

void FAndroidXRSpatialCapabilityModule::ShutdownModule()
{
    UnregisterOpenXRExtensionModularFeature();
}

void FAndroidXRSpatialCapabilityModule::PostCreateInstance(XrInstance InInstance)
{
    Instance = InInstance;
    ResolveOpenXRFunctions(Instance,
                           {
                               SPATIALENTITIES_FUNCTIONS(RESOLVE_OPENXR_FUNC)
                           },
                           [](const char* UnresolvableFunctionName, XrResult Result)
    {
        UE_LOG(LogAndroidXRSpatialEntities, Error,
               TEXT("Unable to resolve function pointer %s (error %s)"),
               UTF8_TO_TCHAR(UnresolvableFunctionName),
               OpenXRResultToString(Result));
    });
    ActiveFutures.Initialize(Instance, xrPollFutureEXT, xrCancelFutureEXT);
    PreSpatialContextFutures.Initialize(Instance, xrPollFutureEXT, xrCancelFutureEXT);
}

void FAndroidXRSpatialCapabilityModule::PostCreateSession(XrSession InSession)
{
    Session = InSession;
    GetCapabilitiesInfo();
}

bool FAndroidXRSpatialCapabilityModule::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
    OutExtensions.Add(XR_EXT_FUTURE_EXTENSION_NAME);
    OutExtensions.Add(XR_EXT_SPATIAL_ENTITY_EXTENSION_NAME);
    GetCapabilityExtensions(OutExtensions);
    return true;
}

void FAndroidXRSpatialCapabilityModule::PostSyncActions(XrSession InSession)
{
    if(PreSpatialContextFutures.ShouldPoll())
    {
        PreSpatialContextFutures.Poll();
        return;
    }
    ActiveFutures.Poll();
    UpdateTrackedEntities();
}

void FAndroidXRSpatialCapabilityModule::OnEvent(XrSession InSession, const XrEventDataBaseHeader* InHeader)
{
    if(InHeader && InHeader->type == XR_TYPE_EVENT_DATA_SPATIAL_DISCOVERY_RECOMMENDED_EXT && bSpatialContextCreated)
    {
        auto DiscoveryData = reinterpret_cast<const XrEventDataSpatialDiscoveryRecommendedEXT*>(InHeader);
        if(DiscoveryData->spatialContext == SpatialContext && ShouldAutoDiscoverEntities())
        {
            CreateDiscoverySnapshotRequest();
        }
    }
}

bool FAndroidXRSpatialCapabilityModule::GetSpatialBuffer(XrSpatialSnapshotEXT Snapshot, FAndroidXRSpatialBuffer& Buffer)
{
    if(Buffer.Buffer.bufferId == 0)
    {
        return false;
    }
    XrResult Result;
    XrSpatialBufferGetInfoEXT GetInfo{
        .type = XR_TYPE_SPATIAL_BUFFER_GET_INFO_EXT,
        .bufferId = Buffer.Buffer.bufferId
    };
    uint32_t BufferSize{};
    switch(Buffer.Buffer.bufferType)
    {
        case XrSpatialBufferTypeEXT::XR_SPATIAL_BUFFER_TYPE_FLOAT_EXT:
        {
            Result = xrGetSpatialBufferFloatEXT(Snapshot, &GetInfo, BufferSize, &BufferSize, nullptr);
            CHECK_OPENXR_STATUS_AND_RETURN_BOOL(GetSpatialBuffer, xrGetSpatialBufferFloatEXT, Result);
            Buffer.AllocateBuffer(BufferSize);
            Result = xrGetSpatialBufferFloatEXT(Snapshot, &GetInfo, BufferSize, &BufferSize, Buffer.GetNativeBuffer<float>());
            CHECK_OPENXR_STATUS_AND_RETURN_BOOL(GetSpatialBuffer, xrGetSpatialBufferFloatEXT, Result);
            break;
        }
        case XrSpatialBufferTypeEXT::XR_SPATIAL_BUFFER_TYPE_STRING_EXT:
        {
            Result = xrGetSpatialBufferStringEXT(Snapshot, &GetInfo, BufferSize, &BufferSize, nullptr);
            CHECK_OPENXR_STATUS_AND_RETURN_BOOL(GetSpatialBuffer, xrGetSpatialBufferStringEXT, Result);
            Buffer.AllocateBuffer(BufferSize);
            Result = xrGetSpatialBufferStringEXT(Snapshot, &GetInfo, BufferSize, &BufferSize, Buffer.GetNativeBuffer<char>());
            CHECK_OPENXR_STATUS_AND_RETURN_BOOL(GetSpatialBuffer, xrGetSpatialBufferStringEXT, Result);
            break;
        }
        case XrSpatialBufferTypeEXT::XR_SPATIAL_BUFFER_TYPE_VECTOR2F_EXT:
        {
            Result = xrGetSpatialBufferVector2fEXT(Snapshot, &GetInfo, BufferSize, &BufferSize, nullptr);
            CHECK_OPENXR_STATUS_AND_RETURN_BOOL(GetSpatialBuffer, xrGetSpatialBufferVector2fEXT, Result);
            Buffer.AllocateBuffer(BufferSize);
            Result = xrGetSpatialBufferVector2fEXT(Snapshot, &GetInfo, BufferSize, &BufferSize, Buffer.GetNativeBuffer<XrVector2f>());
            CHECK_OPENXR_STATUS_AND_RETURN_BOOL(GetSpatialBuffer, xrGetSpatialBufferVector2fEXT, Result);
            break;
        }
        case XrSpatialBufferTypeEXT::XR_SPATIAL_BUFFER_TYPE_VECTOR3F_EXT:
        {
            Result = xrGetSpatialBufferVector3fEXT(Snapshot, &GetInfo, BufferSize, &BufferSize, nullptr);
            CHECK_OPENXR_STATUS_AND_RETURN_BOOL(GetSpatialBuffer, xrGetSpatialBufferVector3fEXT, Result);
            Buffer.AllocateBuffer(BufferSize);
            Result = xrGetSpatialBufferVector3fEXT(Snapshot, &GetInfo, BufferSize, &BufferSize, Buffer.GetNativeBuffer<XrVector3f>());
            CHECK_OPENXR_STATUS_AND_RETURN_BOOL(GetSpatialBuffer, xrGetSpatialBufferVector3fEXT, Result);
            break;
        }
        case XrSpatialBufferTypeEXT::XR_SPATIAL_BUFFER_TYPE_UINT8_EXT:
        {
            Result = xrGetSpatialBufferUint8EXT(Snapshot, &GetInfo, BufferSize, &BufferSize, nullptr);
            CHECK_OPENXR_STATUS_AND_RETURN_BOOL(GetSpatialBuffer, xrGetSpatialBufferUint8fEXT, Result);
            Buffer.AllocateBuffer(BufferSize);
            Result = xrGetSpatialBufferUint8EXT(Snapshot, &GetInfo, BufferSize, &BufferSize, Buffer.GetNativeBuffer<uint8_t>());
            CHECK_OPENXR_STATUS_AND_RETURN_BOOL(GetSpatialBuffer, xrGetSpatialBufferUint8fEXT, Result);
            break;
        }
        case XrSpatialBufferTypeEXT::XR_SPATIAL_BUFFER_TYPE_UINT16_EXT:
        {
            Result = xrGetSpatialBufferUint16EXT(Snapshot, &GetInfo, BufferSize, &BufferSize, nullptr);
            CHECK_OPENXR_STATUS_AND_RETURN_BOOL(GetSpatialBuffer, xrGetSpatialBufferUint16fEXT, Result);
            Buffer.AllocateBuffer(BufferSize);
            Result = xrGetSpatialBufferUint16EXT(Snapshot, &GetInfo, BufferSize, &BufferSize, Buffer.GetNativeBuffer<uint16_t>());
            CHECK_OPENXR_STATUS_AND_RETURN_BOOL(GetSpatialBuffer, xrGetSpatialBufferUint16fEXT, Result);
            break;
        }
        case XrSpatialBufferTypeEXT::XR_SPATIAL_BUFFER_TYPE_UINT32_EXT:
        {
            Result = xrGetSpatialBufferUint32EXT(Snapshot, &GetInfo, BufferSize, &BufferSize, nullptr);
            CHECK_OPENXR_STATUS_AND_RETURN_BOOL(GetSpatialBuffer, xrGetSpatialBufferUint32fEXT, Result);
            Buffer.AllocateBuffer(BufferSize);
            Result = xrGetSpatialBufferUint32EXT(Snapshot, &GetInfo, BufferSize, &BufferSize, Buffer.GetNativeBuffer<uint32_t>());
            CHECK_OPENXR_STATUS_AND_RETURN_BOOL(GetSpatialBuffer, xrGetSpatialBufferUint32fEXT, Result);
            break;
        }
        default:
            break;
    }
    return true;
}

void FAndroidXRSpatialCapabilityModule::UpdateTrackedEntities()
{
    if(TrackedEntities.IsEmpty())
    {
        return;
    }
    auto HMD = GEngine->XRSystem.Get()->GetIOpenXRHMD();
    TArray<XrSpatialEntityEXT> Entities{};
    TrackedEntities.GenerateValueArray(Entities);
    XrSpatialUpdateSnapshotCreateInfoEXT UpdateCreateInfo
    {
        .type = XR_TYPE_SPATIAL_UPDATE_SNAPSHOT_CREATE_INFO_EXT,
        .entityCount = static_cast<uint32_t>(Entities.Num()),
        .entities = Entities.GetData(),
        .componentTypeCount = static_cast<uint32_t>(EnabledComponents.Num()),
        .componentTypes = EnabledComponents.GetData(),
        .baseSpace = HMD->GetTrackingSpace(),
        .time = HMD->GetDisplayTime()
    };
    XrSpatialSnapshotEXT UpdateSnapshot{};
    auto Result = xrCreateSpatialUpdateSnapshotEXT(SpatialContext, &UpdateCreateInfo, &UpdateSnapshot);
    CHECK_OPENXR_STATUS_AND_RETURN(UpdateDeviceLocations, xrCreateSpatialUpdateSnapshotEXT, Result, );
    QuerySpatialData(UpdateSnapshot, true);
    Result = xrDestroySpatialSnapshotEXT(UpdateSnapshot);
    CHECK_OPENXR_STATUS_AND_RETURN(UpdateDeviceLocations, xrDestroySpatialSnapshotEXT, Result, );
}

void FAndroidXRSpatialCapabilityModule::GetCapabilitiesInfo()
{
    auto HMD = GEngine->XRSystem.Get()->GetIOpenXRHMD();

    uint32_t CapabilityCount{};
    auto Result = xrEnumerateSpatialCapabilitiesEXT(Instance, HMD->GetSystem(), CapabilityCount, &CapabilityCount, nullptr);
    CHECK_OPENXR_STATUS_AND_RETURN(GetCapabilitiesInfo, xrEnumerateSpatialCapabilitiesEXT, Result, );
    TArray<XrSpatialCapabilityEXT> Capabilities{};
    Capabilities.SetNum(CapabilityCount);
    Result = xrEnumerateSpatialCapabilitiesEXT(Instance, HMD->GetSystem(), CapabilityCount, &CapabilityCount, Capabilities.GetData());
    CHECK_OPENXR_STATUS_AND_RETURN(GetCapabilitiesInfo, xrEnumerateSpatialCapabilitiesEXT, Result, );
    for(auto Capability : Capabilities)
    {
        auto& CapabilityInfo = CapabilitiesInfo.Capabilities.Add(Capability);
        CapabilityInfo.bIsSupported = true;
        TArray<XrSpatialComponentTypeEXT> Components{};
        uint32_t ComponentCount{};

        XrSpatialCapabilityComponentTypesEXT ComponentTypes
        {
            .type = XR_TYPE_SPATIAL_CAPABILITY_COMPONENT_TYPES_EXT
        };
        Result = xrEnumerateSpatialCapabilityComponentTypesEXT(Instance, HMD->GetSystem(), Capability, &ComponentTypes);
        CHECK_OPENXR_STATUS_AND_RETURN(GetCapabilitiesInfo, xrEnumerateSpatialCapabilityComponentTypesEXT, Result, );

        Components.SetNum(ComponentTypes.componentTypeCountOutput);
        ComponentTypes.componentTypeCapacityInput = ComponentTypes.componentTypeCountOutput;
        ComponentTypes.componentTypes = Components.GetData();

        Result = xrEnumerateSpatialCapabilityComponentTypesEXT(Instance, HMD->GetSystem(), Capability, &ComponentTypes);
        CHECK_OPENXR_STATUS_AND_RETURN(GetCapabilitiesInfo, xrEnumerateSpatialCapabilityComponentTypesEXT, Result, );

        for(auto Component : Components)
        {
            CapabilityInfo.SupportedComponents.Add(Component);
        }
        uint32_t FeatureCount{};
        TArray<XrSpatialCapabilityFeatureEXT> Features{};
        Result = xrEnumerateSpatialCapabilityFeaturesEXT(Instance, HMD->GetSystem(), Capability, FeatureCount, &FeatureCount, Features.GetData());
        CHECK_OPENXR_STATUS_AND_RETURN(GetCapabilitiesInfo, xrEnumerateSpatialCapabilityFeaturesEXT, Result, );

        Features.SetNum(FeatureCount);
        Result = xrEnumerateSpatialCapabilityFeaturesEXT(Instance, HMD->GetSystem(), Capability, FeatureCount, &FeatureCount, Features.GetData());
        CHECK_OPENXR_STATUS_AND_RETURN(GetCapabilitiesInfo, xrEnumerateSpatialCapabilityFeaturesEXT, Result, );
        for(auto Feature : Features)
        {
            CapabilityInfo.SupportedFeatures.Add(Feature);
        }
    }
    OnCapabilitiesInfoObtained();
}

bool FAndroidXRSpatialCapabilityModule::GetSupportedCapabilities(TArray<ESpatialCapability>& SupportedCapabilities)
{
    if(CapabilitiesInfo.Capabilities.IsEmpty())
    {
        return false;
    }
    SupportedCapabilities.Reset();
    Algo::Transform(CapabilitiesInfo.Capabilities, SupportedCapabilities, [](const auto& Entry)
    {
        return AndroidXR::Convert(Entry.Key);
    });
    return true;
}

bool FAndroidXRSpatialCapabilityModule::GetCapabilityInfo(ESpatialCapability Capability, FAndroidXRSpatialCapabilityInfo& CapabilityInfo)
{
    CapabilityInfo.bIsCapabilitySupported = false;
    if(CapabilitiesInfo.Capabilities.IsEmpty() || !CapabilitiesInfo.IsCapabilitySupported(AndroidXR::Convert(Capability)))
    {
        return false;
    }
    auto& NativeCapabilityInfo = CapabilitiesInfo.Capabilities[AndroidXR::Convert(Capability)];
    Algo::Transform(NativeCapabilityInfo.SupportedFeatures, CapabilityInfo.SupportedFeatures, [](XrSpatialCapabilityFeatureEXT Feature)
    {
        return AndroidXR::Convert(Feature);
    });
    Algo::Transform(NativeCapabilityInfo.SupportedComponents, CapabilityInfo.SupportedComponents, [](XrSpatialComponentTypeEXT Component)
    {
        return AndroidXR::Convert(Component);
    });
    CapabilityInfo.bIsCapabilitySupported = true;
    return true;
}

void FAndroidXRSpatialCapabilityModule::CreateSpatialContext()
{
    XrSpatialContextCreateInfoEXT CreateInfo
    {
        .type = XR_TYPE_SPATIAL_CONTEXT_CREATE_INFO_EXT
    };
    UpdateEnabledComponents();
    TArray<XrSpatialCapabilityConfigurationBaseHeaderEXT*> ConfigurationHeaders{};
    GetCapabilityConfigurations(ConfigurationHeaders);
    CreateInfo.capabilityConfigs = ConfigurationHeaders.GetData();
    CreateInfo.capabilityConfigCount = static_cast<uint32_t>(ConfigurationHeaders.Num());
    XrFutureEXT Future{};
    PreSpatialContextCreate(CreateInfo);
    auto Result = xrCreateSpatialContextAsyncEXT(Session, &CreateInfo, &Future);

    CHECK_OPENXR_STATUS_AND_RETURN(CreateSpatialContext, xrCreateSpatialContextAsyncEXT, Result, );
    ActiveFutures.Submit(Future, [this](XrFutureEXT Future, bool bWasCancelled)
    {
        OnSpatialContextFutureComplete(Future, bWasCancelled);
    });
}

void FAndroidXRSpatialCapabilityModule::DestroySpatialContext()
{
    if(!bSpatialContextCreated)
    {
        return;
    }

    for(auto& [EntityId, Entity] : TrackedEntities)
    {
        auto Result = xrDestroySpatialEntityEXT(Entity);
        LOG_IF_OPENXR_ERROR(Stop, xrDestroySpatialEntityEXT, Result);
    }
    TrackedEntities.Reset();
    auto Result = xrDestroySpatialContextEXT(SpatialContext);
    LOG_IF_OPENXR_ERROR(Stop, xrDestroySpatialContextEXT, Result);
    SpatialContextDestroyed();
    SpatialContext = XR_NULL_HANDLE;
    bWaitingForSpatialContextCreation = false;
    bSpatialContextCreated = false;
    ActiveFutures.CancelAll();
    OnSpatialContextDestroyed.Broadcast();
}

void FAndroidXRSpatialCapabilityModule::OnSpatialContextFutureComplete(XrFutureEXT Future, bool bWasCancelled)
{
    if(bWasCancelled)
    {
        return;
    }
    XrCreateSpatialContextCompletionEXT Completion{
        .type = XR_TYPE_CREATE_SPATIAL_CONTEXT_COMPLETION_EXT,
    };

    auto Result = xrCreateSpatialContextCompleteEXT(Session, Future, &Completion);
    CHECK_OPENXR_STATUS_AND_RETURN(OnSpatialContextFutureComplete, xrCreateSpatialContextCompleteEXT, Result, );
    CHECK_OPENXR_STATUS_AND_RETURN(OnSpatialContextFutureComplete, xrCreateSpatialContextCompleteEXT, Completion.futureResult, );

    bSpatialContextCreated = true;
    SpatialContext = Completion.spatialContext;
    TrackedEntities.Reset();
    OnSpatialContextCreated.Broadcast();
}

void FAndroidXRSpatialCapabilityModule::OnSpatialSnapshotDiscoveryComplete(XrFutureEXT Future, bool bWasCancelled)
{
    bDiscoveryInProgress = false;
    if(bWasCancelled)
    {
        return;
    }
    auto HMD = GEngine->XRSystem.Get()->GetIOpenXRHMD();

    XrCreateSpatialDiscoverySnapshotCompletionInfoEXT CompletionInfo
    {
        .type = XR_TYPE_CREATE_SPATIAL_DISCOVERY_SNAPSHOT_COMPLETION_INFO_EXT,
        .baseSpace = HMD->GetTrackingSpace(),
        .time = HMD->GetDisplayTime(),
        .future = Future
    };
    XrCreateSpatialDiscoverySnapshotCompletionEXT Completion
    {
        .type = XR_TYPE_CREATE_SPATIAL_DISCOVERY_SNAPSHOT_COMPLETION_EXT
    };

    auto Result = xrCreateSpatialDiscoverySnapshotCompleteEXT(SpatialContext, &CompletionInfo, &Completion);
    CHECK_OPENXR_STATUS_AND_RETURN(OnSpatialSnapshotDiscoveryComplete, xrCreateSpatialDiscoverySnapshotCompleteEXT, Result, );
    CHECK_OPENXR_STATUS_AND_RETURN(OnSpatialSnapshotDiscoveryComplete, xrCreateSpatialDiscoverySnapshotCompleteEXT_FutureResult, Completion.futureResult, );
    QuerySpatialData(Completion.snapshot, false);
    Result = xrDestroySpatialSnapshotEXT(Completion.snapshot);
    LOG_IF_OPENXR_ERROR(OnSpatialSnapshotDiscoveryComplete, xrDestroySpatialSnapshotEXT, Result);
}

void FAndroidXRSpatialCapabilityModule::QuerySpatialData(XrSpatialSnapshotEXT Snapshot, bool bShouldUpdate)
{
    XrSpatialComponentDataQueryConditionEXT QueryCondition
    {
        .type = XR_TYPE_SPATIAL_COMPONENT_DATA_QUERY_CONDITION_EXT,
        .componentTypeCount = static_cast<uint32_t>(EnabledComponents.Num()),
        .componentTypes = EnabledComponents.GetData()
    };

    XrSpatialComponentDataQueryResultEXT QueryResult
    {
        .type = XR_TYPE_SPATIAL_COMPONENT_DATA_QUERY_RESULT_EXT
    };

    auto Result = xrQuerySpatialComponentDataEXT(Snapshot, &QueryCondition, &QueryResult);
    CHECK_OPENXR_STATUS_AND_RETURN(QuerySpatialData, xrQuerySpatialComponentDataEXT_FetchCount, Result, );

    TArray<XrSpatialEntityIdEXT> EntityIds{};
    EntityIds.SetNum(QueryResult.entityIdCountOutput);

    TArray<XrSpatialEntityTrackingStateEXT> EntityTrackingStates{};
    EntityTrackingStates.SetNum(QueryResult.entityStateCountOutput);

    QueryResult.entityIdCapacityInput = QueryResult.entityIdCountOutput;
    QueryResult.entityStateCapacityInput = QueryResult.entityStateCountOutput;
    QueryResult.entityIds = EntityIds.GetData();
    QueryResult.entityStates = EntityTrackingStates.GetData();

    QueryCondition.componentTypes = EnabledComponents.GetData();
    QueryCondition.componentTypeCount = static_cast<uint32_t>(EnabledComponents.Num());

    TArray<FSpatialComponentList*> ComponentList{};
    for(auto ComponentType : EnabledComponents)
    {
        switch(ComponentType)
        {
            case XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_BOUNDED_2D_EXT:
            {
                ComponentList.Add(&Bounds2D);
                break;
            }
            case XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_BOUNDED_3D_EXT:
            {
                ComponentList.Add(&Bounds3D);
                break;
            }
            case XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_PARENT_EXT:
            {
                ComponentList.Add(&Parents);
                break;
            }
            case XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_MESH_3D_EXT:
            {
                ComponentList.Add(&Meshes3D);
                break;
            }
            default:
            {
                auto ListToAdd = GetSpatialComponentList(ComponentType);
                if(ListToAdd)
                {
                    ComponentList.Add(ListToAdd);
                }
                break;
            }
        }
    }
    XrBaseOutStructure* ChainStart = reinterpret_cast<XrBaseOutStructure*>(&QueryResult);
    for(auto Component : ComponentList)
    {
        Component->Allocate(EntityIds.Num());
        ChainStart->next = Component->GetNativeHeader();
        ChainStart = ChainStart->next;
    }
    Result = xrQuerySpatialComponentDataEXT(Snapshot, &QueryCondition, &QueryResult);
    CHECK_OPENXR_STATUS_AND_RETURN(QuerySpatialData, xrQuerySpatialComponentDataEXT, Result, );

    FSpatialEntityInfo EntityInfo{};
    EntityInfo.Snapshot = Snapshot;
    EntityInfo.Bounds = &Bounds2D;
    EntityInfo.Bounds3D = &Bounds3D;
    EntityInfo.Parents = &Parents;
    EntityInfo.Meshes3D = &Meshes3D;

    for(auto Index = 0; Index < EntityIds.Num(); Index++)
    {
        auto& EntityId = EntityIds[Index];
        auto& EntityState = EntityTrackingStates[Index];

        EntityInfo.EntityId = EntityId;
        EntityInfo.EntityTrackingState = EntityState;
        EntityInfo.ComponentIndex = Index;

        auto Entry = TrackedEntities.Find(EntityId);
        if(Entry)
        {
            //If already being tracked:
            switch(EntityState)
            {
                case XrSpatialEntityTrackingStateEXT::XR_SPATIAL_ENTITY_TRACKING_STATE_PAUSED_EXT:
                case XrSpatialEntityTrackingStateEXT::XR_SPATIAL_ENTITY_TRACKING_STATE_STOPPED_EXT:
                {
                    auto Entity = *Entry;
                    Result = xrDestroySpatialEntityEXT(Entity);
                    if(!XR_UNQUALIFIED_SUCCESS(Result))
                    {
                        LOG_OPENXR_ERROR(QuerySpatialData, xrDestroySpatialEntityEXT, Result);
                    }
                    TrackedEntities.Remove(EntityId);
                    EntityRemoved(EntityInfo);
                    break;
                }
                case XrSpatialEntityTrackingStateEXT::XR_SPATIAL_ENTITY_TRACKING_STATE_TRACKING_EXT:
                {
                    if(bShouldUpdate)
                    {
                        EntityUpdated(EntityInfo);
                    }
                    break;
                }
                default:
                    break;
            }
        }
        else
        {
            if(EntityState != XrSpatialEntityTrackingStateEXT::XR_SPATIAL_ENTITY_TRACKING_STATE_TRACKING_EXT)
            {
                continue;
            }
            XrSpatialEntityFromIdCreateInfoEXT CreateInfo
            {
                .type = XR_TYPE_SPATIAL_ENTITY_FROM_ID_CREATE_INFO_EXT,
                .entityId = EntityId
            };
            XrSpatialEntityEXT Entity{};
            Result = xrCreateSpatialEntityFromIdEXT(SpatialContext, &CreateInfo, &Entity);
            if(!XR_UNQUALIFIED_SUCCESS(Result))
            {
                LOG_OPENXR_ERROR(QuerySpatialData, xrCreateSpatialEntityFromIdEXT, Result);
                continue;
            }
            TrackedEntities.Add(EntityId, Entity);
            EntityAdded(EntityInfo);
        }
    }
}

bool FAndroidXRSpatialCapabilityModule::CreateDiscoverySnapshotRequest()
{
    if(!bSpatialContextCreated || bDiscoveryInProgress)
    {
        return false;
    }
    XrSpatialDiscoverySnapshotCreateInfoEXT CreateInfo
    {
        .type = XR_TYPE_SPATIAL_DISCOVERY_SNAPSHOT_CREATE_INFO_EXT,
        .componentTypeCount = static_cast<uint32_t>(EnabledComponents.Num()),
        .componentTypes = EnabledComponents.GetData()
    };
    XrFutureEXT DiscoveryFuture{};
    auto Result = xrCreateSpatialDiscoverySnapshotAsyncEXT(SpatialContext, &CreateInfo, &DiscoveryFuture);
    if(XR_FAILED(Result))
    {
        return false;
    }
    bDiscoveryInProgress = true;
    ActiveFutures.Submit(DiscoveryFuture, [this](XrFutureEXT Future, bool bWasCancelled)
    {
        OnSpatialSnapshotDiscoveryComplete(Future, bWasCancelled);
    });
    return true;
}

IMPLEMENT_MODULE(FDefaultModuleImpl, AndroidXRSpatialEntities);
