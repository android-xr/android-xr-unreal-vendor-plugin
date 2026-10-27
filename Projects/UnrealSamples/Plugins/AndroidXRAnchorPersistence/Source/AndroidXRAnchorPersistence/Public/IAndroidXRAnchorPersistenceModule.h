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

#include "Modules/ModuleManager.h"
#include "AndroidXRAnchorPersistenceTypes.h"

/**
 * The public interface to this module.  In most cases, this interface is only
 * public to sibling modules within this plugin.
 */

class ANDROIDXRANCHORPERSISTENCE_API IAndroidXRAnchorPersistenceModule : public IModuleInterface
{
public:
    /**
     * Singleton-like access to this module's interface.  This is just for
     * convenience! Beware of calling this during the shutdown phase, though. Your
     * module might have been unloaded already.
     *
     * @return Returns singleton instance, loading the module on demand if needed
     */
    static inline IAndroidXRAnchorPersistenceModule& Get()
    {
        return FModuleManager::LoadModuleChecked<IAndroidXRAnchorPersistenceModule>("AndroidXRAnchorPersistence");
    }

    /**
     * Checks to see if this module is loaded and ready.  It is only valid to call
     * Get() if IsAvailable() returns true.
     *
     * @return True if the module is loaded and ready to use
     */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("AndroidXRAnchorPersistence");
    }

    virtual bool GetAnchorPersistenceSupported() const = 0;
    virtual bool EnumerateSupportedPersistenceAnchorTypes(TArray<EAndroidXRTrackableType>& SupportedTypes) = 0;
    virtual bool CreateAnchorPersistence(FAndroidXRAnchorPersistence& AnchorPersistence) = 0;
    virtual bool DestroyAnchorPersistence(FAndroidXRAnchorPersistence& AnchorPersistence) = 0;
    virtual bool PersistAnchorAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FAndroidXRSpace& AnchorSpace, FOnAnchorPersisted& OnAnchorPersisted,FOnAnchorAsyncActionFailed& OnPersistAnchorFailed) = 0;
    virtual bool GetAnchorPersistState(const FAndroidXRAnchorPersistence& AnchorPersistence,
        const FGuid &PersistedID,
        EAndroidXRAnchorPersistState& PersistState) = 0;
    virtual bool CreatePersistedAnchorSpaceAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FGuid& PersistedId, const FAndroidXRSpace& AnchorSpace, FOnPersistedAnchorSpaceCreated& OnPersistedAnchorSpaceCreated, FOnAnchorAsyncActionFailed& OnPersistedAnchorCreateFailed) = 0;
    virtual bool EnumeratePersistedAnchors(const FAndroidXRAnchorPersistence& AnchorPersistence,
        TArray<FGuid>& PersistedAnchors) = 0;
    virtual bool UnpersistAnchorAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FGuid& PersistedId, FOnAnchorUnPersisted& OnAnchorUnPersisted, FOnAnchorAsyncActionFailed& OnUnpersistFailed) = 0;
};
