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
#include "AndroidXRFuture.h"
#include "AndroidXRSpatialEntitiesTypes.h"
#include "AndroidXRSpatialBuffer.h"
#include "AndroidXRSpatialEntitiesConversions.h"
#include "IOpenXRExtensionPlugin.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAndroidXRSpatialEntities, Log, All);

class ANDROIDXRSPATIALENTITIES_API FAndroidXRSpatialCapabilityModule : public IModuleInterface, public IOpenXRExtensionPlugin
{
public:
    void StartupModule() override;
    void ShutdownModule() override;

    void PostCreateInstance(XrInstance InInstance) override;
    void PostCreateSession(XrSession InSession) override;
    bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
    void PostSyncActions(XrSession InSession) override;
    void OnEvent(XrSession InSession, const XrEventDataBaseHeader* InHeader) override;
    bool GetSpatialBuffer(XrSpatialSnapshotEXT Snapshot, FAndroidXRSpatialBuffer& SpatialBuffer);

    void UpdateTrackedEntities();

    void Start();
    void Stop();
    bool IsReady() const
    {
        return bSpatialContextCreated;
    }

    bool CreateDiscoverySnapshotRequest();
    bool GetSupportedCapabilities(TArray<ESpatialCapability>& SupportedCapabilities);
    bool GetCapabilityInfo(ESpatialCapability Capability, FAndroidXRSpatialCapabilityInfo& CapabilityInfo);

    TMulticastDelegate<void()> OnSpatialContextCreated{};
    TMulticastDelegate<void()> OnSpatialContextDestroyed{};
protected:
    virtual void CreateSpatialContext();
    virtual void UpdateEnabledComponents() = 0;
    virtual void GetCapabilityConfigurations(TArray<XrSpatialCapabilityConfigurationBaseHeaderEXT*>& Headers) = 0;
    virtual FSpatialComponentList* GetSpatialComponentList(XrSpatialComponentTypeEXT ComponentType) = 0;
    virtual void EntityAdded(const FSpatialEntityInfo& EntityInfo) = 0;
    virtual void EntityUpdated(const FSpatialEntityInfo& EntityInfo) = 0;
    virtual void EntityRemoved(const FSpatialEntityInfo& EntityInfo) = 0;
    virtual void GetCapabilityExtensions(TArray<const ANSICHAR*>& Extensions) = 0;

    virtual void SubmitPreSpatialContextFutures(FAndroidXRFutures& Futures)
    {
    }
    virtual void PreSpatialContextCreate(XrSpatialContextCreateInfoEXT& CreateInfo)
    {
    }
    virtual void DestroySpatialContext();
    virtual void OnCapabilitiesInfoObtained()
    {
    }

    virtual void PreSpatialContextDestroy()
    {

    }

    virtual void SpatialContextDestroyed()
    {
    }

    virtual bool ShouldAutoDiscoverEntities()
    {
        return true;
    }

    FAndroidXRFutures ActiveFutures{};
    bool bWaitingForSpatialContextCreation{};
    bool bSpatialContextCreated{};
    bool bDiscoveryInProgress{};

    XrSpatialContextEXT SpatialContext{};
    XrSession Session{};
    XrInstance Instance{};

    FNativeSpatialCapabilitiesInfo CapabilitiesInfo{};

    TMap<XrSpatialEntityIdEXT, XrSpatialEntityEXT> TrackedEntities{};
    void GetCapabilitiesInfo();

    void OnSpatialContextFutureComplete(XrFutureEXT Future, bool bWasCancelled);
    void OnSpatialSnapshotDiscoveryComplete(XrFutureEXT Future, bool bWasCancelled);

    void QuerySpatialData(XrSpatialSnapshotEXT Snapshot, bool bShouldUpdate);

    //Common components:
    TFSpatialComponentList<XrSpatialComponentBounded2DListEXT, XrSpatialBounded2DDataEXT> Bounds2D{ XR_TYPE_SPATIAL_COMPONENT_BOUNDED_2D_LIST_EXT };
    TFSpatialComponentList<XrSpatialComponentBounded3DListEXT, XrBoxf> Bounds3D{ XR_TYPE_SPATIAL_COMPONENT_BOUNDED_3D_LIST_EXT };
    TFSpatialComponentList<XrSpatialComponentParentListEXT, XrSpatialEntityIdEXT> Parents{ XR_TYPE_SPATIAL_COMPONENT_PARENT_LIST_EXT };
    TFSpatialComponentList<XrSpatialComponentMesh3DListEXT, XrSpatialMeshDataEXT> Meshes3D{ XR_TYPE_SPATIAL_COMPONENT_MESH_3D_LIST_EXT };

    TArray<XrSpatialComponentTypeEXT> EnabledComponents{};

#define SPATIALENTITIES_FUNCTIONS(HelperMacro) \
        HelperMacro(xrEnumerateSpatialCapabilitiesEXT) \
        HelperMacro(xrEnumerateSpatialCapabilityComponentTypesEXT) \
        HelperMacro(xrEnumerateSpatialCapabilityFeaturesEXT) \
        HelperMacro(xrCreateSpatialContextAsyncEXT) \
        HelperMacro(xrCreateSpatialContextCompleteEXT) \
        HelperMacro(xrDestroySpatialContextEXT) \
        HelperMacro(xrCreateSpatialDiscoverySnapshotAsyncEXT) \
        HelperMacro(xrCreateSpatialDiscoverySnapshotCompleteEXT) \
        HelperMacro(xrQuerySpatialComponentDataEXT) \
        HelperMacro(xrDestroySpatialSnapshotEXT) \
        HelperMacro(xrCreateSpatialEntityFromIdEXT) \
        HelperMacro(xrDestroySpatialEntityEXT) \
        HelperMacro(xrCreateSpatialUpdateSnapshotEXT) \
        HelperMacro(xrGetSpatialBufferStringEXT) \
        HelperMacro(xrGetSpatialBufferUint8EXT) \
        HelperMacro(xrGetSpatialBufferUint16EXT) \
        HelperMacro(xrGetSpatialBufferUint32EXT) \
        HelperMacro(xrGetSpatialBufferFloatEXT) \
        HelperMacro(xrGetSpatialBufferVector2fEXT) \
        HelperMacro(xrGetSpatialBufferVector3fEXT) \
        HelperMacro(xrPollFutureEXT) \
        HelperMacro(xrCancelFutureEXT)

    SPATIALENTITIES_FUNCTIONS(DECLARE_OPENXR_FUNC);
private:
    FAndroidXRFutures PreSpatialContextFutures{};
};

template<typename TSpatialEntity>
class TIAndroidXRSpatialCapabilityModule : public FAndroidXRSpatialCapabilityModule
{
public:
    TMulticastDelegate<void(TObjectPtr<TSpatialEntity>)> OnEntityAdded{};
    TMulticastDelegate<void(TObjectPtr<TSpatialEntity>)> OnEntityRemoved{};

    void SpatialContextDestroyed() override
    {
        for(auto& [_,Entity] :Entities)
        {
            Entity->RemoveFromRoot();
        }
        Entities.Reset();
    }

    void GetTrackedEntities(TArray<TSpatialEntity*>& OutEntities, bool bShouldUpdate)
    {
        if(bShouldUpdate)
        {
            UpdateTrackedEntities();
        }
        OutEntities.Reset();
        Algo::Transform(Entities, OutEntities, [](auto& Pair)
        {
            return Pair.Value;
        });
    }

    void EntityAdded(const FSpatialEntityInfo& EntityInfo) override
    {
        auto NewEntity = NewObject<TSpatialEntity>();
        NewEntity->AddToRoot();
        NewEntity->EntityId = AndroidXR::Convert(EntityInfo.EntityId);
        NewEntity->TrackingState = AndroidXR::Convert(EntityInfo.EntityTrackingState);
        ApplyToEntity(NewEntity, EntityInfo);
        Entities.Add(EntityInfo.EntityId, NewEntity);
        OnEntityAdded.Broadcast(NewEntity);
    }

    void EntityUpdated(const FSpatialEntityInfo& EntityInfo) override
    {
        auto Entry = Entities.Find(EntityInfo.EntityId);
        if(!Entry)
        {
            return;
        }
        ApplyToEntity(*Entry, EntityInfo);
    }

    void EntityRemoved(const FSpatialEntityInfo& EntityInfo) override
    {
        auto Entry = Entities.Find(EntityInfo.EntityId);
        if(!Entry)
        {
            return;
        }
        OnEntityRemoved.Broadcast(*Entry);
        Entities.Remove(EntityInfo.EntityId);
        (*Entry)->RemoveFromRoot();
    }

    TSpatialEntity* CreateNewEntityFromId(XrSpatialEntityIdEXT EntityId, XrSpatialEntityEXT SpatialEntity)
    {
        auto Entry = Entities.Find(EntityId);
        if(Entry)
        {
            return *Entry;
        }
        TrackedEntities.Add(EntityId, SpatialEntity);
        auto NewEntity = NewObject<TSpatialEntity>();
        NewEntity->AddToRoot();
        Entities.Add(EntityId, NewEntity);
        NewEntity->EntityId = AndroidXR::Convert(EntityId);
        return NewEntity;
    }

    void RemoveEntityById(XrSpatialEntityIdEXT EntityId)
    {
        auto Entry = Entities.Find(EntityId);
        if(!Entry)
        {
            return;
        }
        (*Entry)->RemoveFromRoot();
        Entities.Remove(EntityId);
        TrackedEntities.Remove(EntityId);
    }
protected:
    virtual void ApplyToEntity(TObjectPtr<TSpatialEntity> Entity, const FSpatialEntityInfo& EntityInfo) = 0;
    TMap<XrSpatialEntityIdEXT, TObjectPtr<TSpatialEntity>> Entities{};
};
