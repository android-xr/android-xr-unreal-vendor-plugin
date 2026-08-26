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

#include "AndroidXRSpatialAnchorsModule.h"
#include <IOpenXRHMDModule.h>
#include <IXRTrackingSystem.h>
#include <IHeadMountedDisplay.h>
#include "IOpenXRHMD.h"
#include "AndroidXRSpatialEntitiesConversions.h"
#include "AndroidXRSpatialAnchorsConversions.h"

DEFINE_LOG_CATEGORY(LogAndroidXRSpatialAnchors);

#define LOG_OPENXR_ERROR(Tag, FunctionName, Result) UE_LOG(LogAndroidXRSpatialAnchors, Error, TEXT("%s %s failed with error %s (%d)"), TEXT(#Tag), TEXT(#FunctionName),  OpenXRResultToString(Result), Result);
#define CHECK_OPENXR_STATUS_AND_RETURN(Tag, FunctionName, Result, ReturnType) if(!XR_UNQUALIFIED_SUCCESS(Result)){ LOG_OPENXR_ERROR(Tag, FunctionName, Result); return ReturnType; }
#define CHECK_OPENXR_STATUS_AND_RETURN_BOOL(Tag, FunctionName, Result) CHECK_OPENXR_STATUS_AND_RETURN(Tag, FunctionName, Result, false);
#define LOG_IF_OPENXR_ERROR(Tag, FunctionName, Result) if(!XR_UNQUALIFIED_SUCCESS(Result)){ LOG_OPENXR_ERROR(Tag, FunctionName, Result);}

void FAndroidXRSpatialAnchorsModule::PostCreateInstance(XrInstance InInstance)
{
    IAndroidXRSpatialAnchorsModule::PostCreateInstance(InInstance);
    ResolveOpenXRFunctions(Instance,
                           {
                               SPATIALENTITIESANCHORS_FUNCTIONS(RESOLVE_OPENXR_FUNC)
                           },
                           [](const char* UnresolvableFunctionName, XrResult Result)
    {
        UE_LOG(LogAndroidXRSpatialAnchors, Error,
               TEXT("Unable to resolve function pointer %s (error %s)"),
               UTF8_TO_TCHAR(UnresolvableFunctionName),
               OpenXRResultToString(Result));
    });
}

void FAndroidXRSpatialAnchorsModule::ApplyToEntity(TObjectPtr<UAndroidXRSpatialAnchorEntity> AnchorEntity, const FSpatialEntityInfo& EntityInfo)
{
    if(!AnchorEntity)
    {
        return;
    }
    auto WorldToMeters = GEngine->XRSystem->GetWorldToMetersScale();

    XrPosef UpdatedPose{};
    if(AnchorsList.Get(EntityInfo.ComponentIndex, UpdatedPose))
    {
        AnchorEntity->AnchorTransform = ToFTransform(UpdatedPose, WorldToMeters);
    }
    XrSpatialPersistenceDataEXT PersistenceData{};
    if(PersistenceList.Get(EntityInfo.ComponentIndex, PersistenceData) && !AnchorEntity->bIsPersisted)
    {
        AnchorEntity->bIsPersisted = true;
        AnchorEntity->PersistedId = ToFGuid(PersistenceData.persistUuid);
        AnchorEntity->PersistState = AndroidXR::Convert(PersistenceData.persistState);
    }
}

void FAndroidXRSpatialAnchorsModule::GetCapabilityExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
    OutExtensions.Add(XR_EXT_SPATIAL_ANCHOR_EXTENSION_NAME);
    OutExtensions.Add(XR_EXT_SPATIAL_PERSISTENCE_EXTENSION_NAME);
    OutExtensions.Add(XR_EXT_SPATIAL_PERSISTENCE_OPERATIONS_EXTENSION_NAME);
}

void FAndroidXRSpatialAnchorsModule::UpdateEnabledComponents()
{
    if(CapabilitiesInfo.IsComponentSupported(XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_ANCHOR_EXT, XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_ANCHOR_EXT))
    {
        EnabledComponents.Add(XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_ANCHOR_EXT);
    }
    if(bSpatialPersistenceSupported)
    {
        EnabledComponents.Add(XrSpatialComponentTypeEXT::XR_SPATIAL_COMPONENT_TYPE_PERSISTENCE_EXT);
    }
}

void FAndroidXRSpatialAnchorsModule::SubmitPreSpatialContextFutures(FAndroidXRFutures& Futures)
{
    if(bSpatialPersistenceSupported)
    {
        XrSpatialPersistenceContextCreateInfoEXT CreateInfo
        {
            .type = XR_TYPE_SPATIAL_PERSISTENCE_CONTEXT_CREATE_INFO_EXT,
            .scope = XrSpatialPersistenceScopeEXT::XR_SPATIAL_PERSISTENCE_SCOPE_LOCAL_ANCHORS_EXT
        };
        XrFutureEXT Future{};
        //if persistence is supported, then create persistence spatial context:
        auto Result = xrCreateSpatialPersistenceContextAsyncEXT(Session, &CreateInfo, &Future);
        CHECK_OPENXR_STATUS_AND_RETURN(SubmitPreSpatialContextFutures, xrCreateSpatialPersistenceContextAsyncEXT, Result, );
        Futures.Submit(Future, [this](XrFutureEXT Future, bool bWasCancelled)
        {
            OnSpatialPersistenceContextCreateComplete(Future, bWasCancelled);
        });
    }
}

void FAndroidXRSpatialAnchorsModule::PreSpatialContextCreate(XrSpatialContextCreateInfoEXT& CreateInfo)
{
    if(!bSpatialPersistenceContextCreated)
    {
        return;
    }
    PersistenceConfig.persistenceContextCount = 1;
    PersistenceConfig.persistenceContexts = &PersistenceContext;

    CreateInfo.next = &PersistenceConfig;
}

void FAndroidXRSpatialAnchorsModule::PreSpatialContextDestroy()
{
    if(bSpatialPersistenceContextCreated && PersistenceContext != XR_NULL_HANDLE)
    {
        xrDestroySpatialPersistenceContextEXT(PersistenceContext);
        PersistenceContext = XR_NULL_HANDLE;
        bSpatialPersistenceContextCreated = false;
    }
}

void FAndroidXRSpatialAnchorsModule::GetCapabilityConfigurations(TArray<XrSpatialCapabilityConfigurationBaseHeaderEXT*>& Headers)
{
    AnchorConfiguration.capability = XrSpatialCapabilityEXT::XR_SPATIAL_CAPABILITY_ANCHOR_EXT;
    AnchorConfiguration.enabledComponentCount = static_cast<uint32_t>(EnabledComponents.Num());
    AnchorConfiguration.enabledComponents = EnabledComponents.GetData();
    Headers.Add(reinterpret_cast<XrSpatialCapabilityConfigurationBaseHeaderEXT*>(&AnchorConfiguration));
}

FSpatialComponentList* FAndroidXRSpatialAnchorsModule::GetSpatialComponentList(XrSpatialComponentTypeEXT ComponentType)
{
    switch(ComponentType)
    {
        case XR_SPATIAL_COMPONENT_TYPE_ANCHOR_EXT:
        {
            return &AnchorsList;
        }
        case XR_SPATIAL_COMPONENT_TYPE_PERSISTENCE_EXT:
        {
            return &PersistenceList;
        }
        default:
            break;
    }
    return nullptr;
}

void FAndroidXRSpatialAnchorsModule::OnCapabilitiesInfoObtained()
{
    auto HMD = GEngine->XRSystem.Get()->GetIOpenXRHMD();
    uint32_t ScopeCount{};
    auto Result = xrEnumerateSpatialPersistenceScopesEXT(Instance, HMD->GetSystem(), ScopeCount, &ScopeCount, nullptr);
    CHECK_OPENXR_STATUS_AND_RETURN(OnCapabilitiesInfoObtained, xrEnumerateSpatialPersistenceScopesEXT, Result, );
    TArray<XrSpatialPersistenceScopeEXT> Scopes{};
    Scopes.SetNum(ScopeCount);
    Result = xrEnumerateSpatialPersistenceScopesEXT(Instance, HMD->GetSystem(), ScopeCount, &ScopeCount, Scopes.GetData());
    CHECK_OPENXR_STATUS_AND_RETURN(OnCapabilitiesInfoObtained, xrEnumerateSpatialPersistenceScopesEXT, Result, );
    bSpatialPersistenceSupported = Scopes.Contains(XrSpatialPersistenceScopeEXT::XR_SPATIAL_PERSISTENCE_SCOPE_LOCAL_ANCHORS_EXT);
}

bool FAndroidXRSpatialAnchorsModule::CreateSpatialAnchor(const FTransform& AnchorTransform,UAndroidXRSpatialAnchorEntity*& CreatedAnchor)
{
    if(!bSpatialContextCreated)
    {
        return false;
    }
    auto HMD = GEngine->XRSystem.Get()->GetIOpenXRHMD();
    auto WorldToMeters = GEngine->XRSystem->GetWorldToMetersScale();
    XrSpatialAnchorCreateInfoEXT CreateInfo
    {
        .type = XR_TYPE_SPATIAL_ANCHOR_CREATE_INFO_EXT,
        .baseSpace = HMD->GetTrackingSpace(),
        .time = HMD->GetDisplayTime(),
        .pose = ToXrPose(AnchorTransform, WorldToMeters)
    };
    XrSpatialEntityIdEXT AnchorId{};
    XrSpatialEntityEXT Anchor{};
    auto Result = xrCreateSpatialAnchorEXT(SpatialContext, &CreateInfo, &AnchorId, &Anchor);
    CHECK_OPENXR_STATUS_AND_RETURN_BOOL(CreateSpatialAnchor, xrCreateSpatialAnchorEXT, Result);
    auto NewAnchor = CreateNewEntityFromId(AnchorId, Anchor);
    NewAnchor->AnchorTransform = AnchorTransform;
    NewAnchor->EntityId = AndroidXR::Convert(AnchorId);
    NewAnchor->AnchorId = NewAnchor->EntityId;
    CreatedAnchor = NewAnchor;
    return true;
}

bool FAndroidXRSpatialAnchorsModule::DestroySpatialAnchor(UAndroidXRSpatialAnchorEntity* AnchorToDestroy)
{
    if(!AnchorToDestroy || !bSpatialContextCreated)
    {
        return false;
    }
    auto AnchorId = AndroidXR::Convert(AnchorToDestroy->EntityId);
    auto AnchorEntity = TrackedEntities.Find(AnchorId);
    if(!AnchorEntity)
    {
        return false;
    }
    auto Result = xrDestroySpatialEntityEXT(*AnchorEntity);
    CHECK_OPENXR_STATUS_AND_RETURN_BOOL(DestroySpatialAnchor, xrDestroySpatialEntityEXT, Result);
    RemoveEntityById(AnchorId);
    return true;
}

bool FAndroidXRSpatialAnchorsModule::PersistAnchor(UAndroidXRSpatialAnchorEntity* AnchorToPersist, FAnchorPersistOperationDelegate OnPersistedCallback, FAnchorPersistOperationFailedDelegate OnFailedCallback)
{
    ESpatialAnchorPeristenceResult ValidationResult{};
    if(!IsPersistenceOperationValid(ValidationResult))
    {
        OnFailedCallback.Broadcast(ValidationResult);
        return false;
    }
    if(!AnchorToPersist || AnchorToPersist->bIsPersisted || !AnchorToPersist->AnchorId.IsValid())
    {
        OnFailedCallback.Broadcast(ESpatialAnchorPeristenceResult::InvalidEntity);
        return false;
    }
    auto NativeId = AndroidXR::Convert(AnchorToPersist->AnchorId);

    XrSpatialEntityPersistInfoEXT PersistInfo
    {
        .type = XR_TYPE_SPATIAL_ENTITY_PERSIST_INFO_EXT,
        .spatialContext = SpatialContext,
        .spatialEntityId = NativeId
    };
    XrFutureEXT PersistFuture{};
    auto Result = xrPersistSpatialEntityAsyncEXT(PersistenceContext, &PersistInfo, &PersistFuture);
    CHECK_OPENXR_STATUS_AND_RETURN_BOOL(PersistAnchor, xrPersistSpatialEntityAsyncEXT, Result);
    ActiveFutures.Submit(PersistFuture, [this, OnPersist = MoveTemp(OnPersistedCallback), OnFailed=MoveTemp(OnFailedCallback), AnchorToPersist](XrFutureEXT Future, bool bWasCancelled)
    {
        if(bWasCancelled)
        {
            OnFailed.Broadcast(ESpatialAnchorPeristenceResult::TimedOut);
            return;
        }
        //Persist operation completed:
        XrPersistSpatialEntityCompletionEXT Completion
        {
            .type = XR_TYPE_PERSIST_SPATIAL_ENTITY_COMPLETION_EXT,
        };
        auto Result = xrPersistSpatialEntityCompleteEXT(PersistenceContext, Future, &Completion);
        CHECK_OPENXR_STATUS_AND_RETURN(PersistAnchor, xrPersistSpatialEntityCompleteEXT, Result, );
        CHECK_OPENXR_STATUS_AND_RETURN(PersistAnchor, xrPersistSpatialEntityCompleteEXT, Completion.futureResult, );

        if(Completion.persistResult != XrSpatialPersistenceContextResultEXT::XR_SPATIAL_PERSISTENCE_CONTEXT_RESULT_SUCCESS_EXT)
        {
            UE_LOG(LogAndroidXRSpatialAnchors, Error, TEXT("Unable to persist anchor due to %d"), Completion.persistResult);
            OnFailed.Broadcast(AndroidXR::Convert(Completion.persistResult));
            return;
        }
        auto TrackedAnchor = Entities.Find(AndroidXR::Convert(AnchorToPersist->EntityId));
        if(!TrackedAnchor)
        {
            UE_LOG(LogAndroidXRSpatialAnchors, Error, TEXT("Unable to find anchor with the given id."));
            OnFailed.Broadcast(ESpatialAnchorPeristenceResult::PersistedIdNotFound);
            return;
        }
        (*TrackedAnchor)->bIsPersisted = true;
        (*TrackedAnchor)->PersistedId = ToFGuid(Completion.persistUuid);
        (*TrackedAnchor)->PersistState = ESpatialAnchorPersistState::Loaded;
        OnPersist.Broadcast(*TrackedAnchor);
    }, 30.0);
    return true;
}

bool FAndroidXRSpatialAnchorsModule::UnPersistAnchor(UAndroidXRSpatialAnchorEntity* AnchorToUnpersist, FAnchorPersistOperationDelegate OnUnpersistedCallback, FAnchorPersistOperationFailedDelegate OnFailedCallback)
{
    ESpatialAnchorPeristenceResult ValidationResult{};
    if(!IsPersistenceOperationValid(ValidationResult))
    {
        OnFailedCallback.Broadcast(ValidationResult);
        return false;
    }
    if(!AnchorToUnpersist || !AnchorToUnpersist->bIsPersisted || !AnchorToUnpersist->PersistedId.IsValid())
    {
        OnFailedCallback.Broadcast(ESpatialAnchorPeristenceResult::InvalidEntity);
        return false;
    }

    XrSpatialEntityUnpersistInfoEXT Info
    {
        .type = XR_TYPE_SPATIAL_ENTITY_UNPERSIST_INFO_EXT,
        .persistUuid = ToXrUuid(AnchorToUnpersist->PersistedId)
    };
    XrFutureEXT Future{};
    auto Result = xrUnpersistSpatialEntityAsyncEXT(PersistenceContext, &Info, &Future);
    CHECK_OPENXR_STATUS_AND_RETURN_BOOL(UnPersistAnchor, xrUnpersistSpatialEntityAsyncEXT, Result);
    ActiveFutures.Submit(Future, [OnUnpersist = MoveTemp(OnUnpersistedCallback), this,OnFailed = MoveTemp(OnFailedCallback), AnchorToUnpersist](XrFutureEXT Future, bool bWasCancelled)
    {
        if(bWasCancelled)
        {
            OnFailed.Broadcast(ESpatialAnchorPeristenceResult::TimedOut);
            return;
        }
        XrUnpersistSpatialEntityCompletionEXT Completion
        {
            .type = XR_TYPE_UNPERSIST_SPATIAL_ENTITY_COMPLETION_EXT
        };
        auto Result = xrUnpersistSpatialEntityCompleteEXT(PersistenceContext, Future, &Completion);
        CHECK_OPENXR_STATUS_AND_RETURN(UnPersistAnchor, xrUnpersistSpatialEntityCompleteEXT, Result, );
        CHECK_OPENXR_STATUS_AND_RETURN(UnPersistAnchor, xrUnpersistSpatialEntityCompleteEXT, Completion.futureResult, );
        if(Completion.unpersistResult != XrSpatialPersistenceContextResultEXT::XR_SPATIAL_PERSISTENCE_CONTEXT_RESULT_SUCCESS_EXT)
        {
            UE_LOG(LogAndroidXRSpatialAnchors, Error, TEXT("Unable to unpersist anchor due to %d"), Completion.unpersistResult);
            OnFailed.Broadcast(AndroidXR::Convert(Completion.unpersistResult));
            return;
        }

        auto TrackedAnchor = Entities.Find(AndroidXR::Convert(AnchorToUnpersist->EntityId));
        if(!TrackedAnchor)
        {
            OnFailed.Broadcast(ESpatialAnchorPeristenceResult::PersistedIdNotFound);
            return;
        }
        (*TrackedAnchor)->bIsPersisted = false;
        (*TrackedAnchor)->PersistedId.Invalidate();
        (*TrackedAnchor)->PersistState = ESpatialAnchorPersistState::NotFound;
        OnUnpersist.Broadcast(*TrackedAnchor);
    }, 30.0);
    return true;
}

bool FAndroidXRSpatialAnchorsModule::LoadPersistedAnchors()
{
    ESpatialAnchorPeristenceResult ValidationResult{};
    if(!IsPersistenceOperationValid(ValidationResult))
    {
        return false;
    }
    CreateDiscoverySnapshotRequest();
    return true;
}

void FAndroidXRSpatialAnchorsModule::OnSpatialPersistenceContextCreateComplete(XrFutureEXT Future, bool bWasCancelled)
{
    if(bWasCancelled)
    {
        return;
    }
    XrCreateSpatialPersistenceContextCompletionEXT Completion
    {
        .type = XR_TYPE_CREATE_SPATIAL_PERSISTENCE_CONTEXT_COMPLETION_EXT,
    };
    auto Result = xrCreateSpatialPersistenceContextCompleteEXT(Session, Future, &Completion);
    CHECK_OPENXR_STATUS_AND_RETURN(OnSpatialPersistenceContextCreateComplete, xrCreateSpatialPersistenceContextCompleteEXT, Result, );
    CHECK_OPENXR_STATUS_AND_RETURN(OnSpatialPersistenceContextCreateComplete, xrCreateSpatialPersistenceContextCompleteEXT, Completion.futureResult, );
    if(Completion.createResult != XrSpatialPersistenceContextResultEXT::XR_SPATIAL_PERSISTENCE_CONTEXT_RESULT_SUCCESS_EXT)
    {
        UE_LOG(LogAndroidXRSpatialAnchors, Error, TEXT("Persistence context creation failed due to %d"), Completion.createResult);
        return;
    }
    bSpatialPersistenceContextCreated = true;
    PersistenceContext = Completion.persistenceContext;
}
bool FAndroidXRSpatialAnchorsModule::IsPersistenceOperationValid(ESpatialAnchorPeristenceResult& OutResult)
{
    if(!bSpatialPersistenceSupported)
    {
        OutResult = ESpatialAnchorPeristenceResult::PersistenceUnsupported;
        return false;
    }
    if(!bSpatialContextCreated)
    {
        OutResult = ESpatialAnchorPeristenceResult::InvalidSpatialContext;
        return false;
    }
    if(!bSpatialPersistenceContextCreated)
    {
        OutResult = ESpatialAnchorPeristenceResult::InvalidPersistenceContext;
        return false;
    }

    return true;
}
IMPLEMENT_MODULE(FAndroidXRSpatialAnchorsModule, AndroidXRSpatialAnchors)