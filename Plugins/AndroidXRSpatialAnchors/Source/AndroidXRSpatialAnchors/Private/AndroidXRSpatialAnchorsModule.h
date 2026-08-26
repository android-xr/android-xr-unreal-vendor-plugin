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
#include "AndroidXRSpatialEntitiesTypes.h"
#include "AndroidXRSpatialAnchorTypes.h"
#include "AndroidXRSpatialAnchorEntity.h"
#include "IAndroidXRSpatialAnchorsModule.h"
#include "AndroidXRSpatialEntitiesModuleBase.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAndroidXRSpatialAnchors, Log, All);

class ANDROIDXRSPATIALANCHORS_API FAndroidXRSpatialAnchorsModule : public IAndroidXRSpatialAnchorsModule
{
public:
    void PostCreateInstance(XrInstance InInstance) override;
    void ApplyToEntity(TObjectPtr<UAndroidXRSpatialAnchorEntity> AnchorEntity, const FSpatialEntityInfo& EntityInfo) override;

    void GetCapabilityExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
    void UpdateEnabledComponents() override;
    void SubmitPreSpatialContextFutures(FAndroidXRFutures& Futures) override;
    void PreSpatialContextCreate(XrSpatialContextCreateInfoEXT& CreateInfo) override;
    void GetCapabilityConfigurations(TArray<XrSpatialCapabilityConfigurationBaseHeaderEXT*>& Headers) override;
    FSpatialComponentList* GetSpatialComponentList(XrSpatialComponentTypeEXT ComponentType) override;
    void OnCapabilitiesInfoObtained() override;
    void PreSpatialContextDestroy() override;

    bool CreateSpatialAnchor(const FTransform& AnchorTransform, UAndroidXRSpatialAnchorEntity*& CreatedAnchor) override;
    bool DestroySpatialAnchor(UAndroidXRSpatialAnchorEntity* AnchorToDestroy) override;
    bool PersistAnchor(UAndroidXRSpatialAnchorEntity* AnchorToPersist, FAnchorPersistOperationDelegate OnPersistedCallback, FAnchorPersistOperationFailedDelegate OnFailedCallback) override;
    bool UnPersistAnchor(UAndroidXRSpatialAnchorEntity* AnchorToUnpersist, FAnchorPersistOperationDelegate OnUnpersistedCallback, FAnchorPersistOperationFailedDelegate OnFailedCallback) override;
    bool LoadPersistedAnchors() override;
protected:
    bool ShouldAutoDiscoverEntities() override
    {
        return false;
    }
private:

    void OnSpatialPersistenceContextCreateComplete(XrFutureEXT Future, bool bWasCancelled);
    TFSpatialComponentList<XrSpatialComponentAnchorListEXT, XrPosef> AnchorsList{ XR_TYPE_SPATIAL_COMPONENT_ANCHOR_LIST_EXT };
    TFSpatialComponentList< XrSpatialComponentPersistenceListEXT, XrSpatialPersistenceDataEXT> PersistenceList{ XR_TYPE_SPATIAL_COMPONENT_PERSISTENCE_LIST_EXT };
    bool bSpatialPersistenceSupported{};

    XrSpatialCapabilityConfigurationAnchorEXT AnchorConfiguration{ XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_ANCHOR_EXT };

    bool bSpatialPersistenceContextCreated{};
    XrSpatialPersistenceContextEXT PersistenceContext{};
    XrSpatialContextPersistenceConfigEXT PersistenceConfig{ XR_TYPE_SPATIAL_CONTEXT_PERSISTENCE_CONFIG_EXT };

    bool IsPersistenceOperationValid(ESpatialAnchorPeristenceResult& OutResult);

#define SPATIALENTITIESANCHORS_FUNCTIONS(HelperMacro) \
        HelperMacro(xrEnumerateSpatialPersistenceScopesEXT) \
        HelperMacro(xrCreateSpatialAnchorEXT) \
        HelperMacro(xrCreateSpatialPersistenceContextAsyncEXT) \
        HelperMacro(xrCreateSpatialPersistenceContextCompleteEXT) \
        HelperMacro(xrDestroySpatialPersistenceContextEXT) \
        HelperMacro(xrPersistSpatialEntityAsyncEXT) \
        HelperMacro(xrPersistSpatialEntityCompleteEXT) \
        HelperMacro(xrUnpersistSpatialEntityAsyncEXT) \
        HelperMacro(xrUnpersistSpatialEntityCompleteEXT)

    SPATIALENTITIESANCHORS_FUNCTIONS(DECLARE_OPENXR_FUNC);
};
