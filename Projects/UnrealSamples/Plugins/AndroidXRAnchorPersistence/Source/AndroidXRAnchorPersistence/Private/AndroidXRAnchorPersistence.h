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
#include "IAndroidXRAnchorPersistenceModule.h"
#include "IOpenXRExtensionPlugin.h"

#include <AndroidXRHelpers.h>

DECLARE_LOG_CATEGORY_EXTERN(LogAndroidXRAnchorPersistence, Log, All);

class UAnchorPersistenceSubsystem;

class FAndroidXRAnchorPersistenceModule : public IAndroidXRAnchorPersistenceModule, public IOpenXRExtensionPlugin
{
public:
    FAndroidXRAnchorPersistenceModule();

    /** IModuleInterface */
    void StartupModule() override;
    void ShutdownModule() override;

    /** IOpenXRExtensionPlugin */
    FString GetDisplayName() override
    {
        return FString(TEXT("AndroidXRAnchorPersistence"));
    }

    void PostCreateInstance(XrInstance InInstance) override;
    void PostCreateSession(XrSession InSession) override;
    bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;

    bool GetAnchorPersistenceSupported() const override;
    bool EnumerateSupportedPersistenceAnchorTypes(TArray<EAndroidXRTrackableType>& SupportedTypes) override;
    bool CreateAnchorPersistence(FAndroidXRAnchorPersistence& AnchorPersistence) override;
    bool DestroyAnchorPersistence(FAndroidXRAnchorPersistence& AnchorPersistence) override;
    bool GetAnchorPersistState(const FAndroidXRAnchorPersistence& AnchorPersistence,
        const FGuid &PersistedID,
        EAndroidXRAnchorPersistState& PersistState) override;
    bool CreatePersistedAnchorSpaceAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FGuid& PersistedId, const FAndroidXRSpace& AnchorSpace, FOnPersistedAnchorSpaceCreated& OnPersistedAnchorSpaceCreated, FOnAnchorAsyncActionFailed& OnPersistedAnchorCreateFailed) override;
    bool EnumeratePersistedAnchors(const FAndroidXRAnchorPersistence& AnchorPersistence,
        TArray<FGuid>& PersistedAnchors) override;
    bool UnpersistAnchorAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FGuid& PersistedId, FOnAnchorUnPersisted& OnAnchorUnPersisted, FOnAnchorAsyncActionFailed& OnUnpersistFailed) override;
    bool PersistAnchorAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FAndroidXRSpace& AnchorSpace, FOnAnchorPersisted& OnAnchorPersisted, FOnAnchorAsyncActionFailed& OnPersistAnchorFailed) override;
private:
    XrInstance Instance{};
    XrSession Session{};

    UPROPERTY()
    TObjectPtr<UAnchorPersistenceSubsystem> AnchorPersistenceSubsystem{};

    #define ANDROIDXRANCHORPERSISTENCE_FUNCTIONS(HelperMacro) \
        HelperMacro(xrEnumerateSupportedPersistenceAnchorTypesANDROID) \
        HelperMacro(xrCreateDeviceAnchorPersistenceANDROID) \
        HelperMacro(xrDestroyDeviceAnchorPersistenceANDROID) \
        HelperMacro(xrPersistAnchorANDROID) \
        HelperMacro(xrGetAnchorPersistStateANDROID) \
        HelperMacro(xrCreatePersistedAnchorSpaceANDROID) \
        HelperMacro(xrEnumeratePersistedAnchorsANDROID) \
        HelperMacro(xrUnpersistAnchorANDROID)

    ANDROIDXRANCHORPERSISTENCE_FUNCTIONS(DECLARE_OPENXR_FUNC);
};
