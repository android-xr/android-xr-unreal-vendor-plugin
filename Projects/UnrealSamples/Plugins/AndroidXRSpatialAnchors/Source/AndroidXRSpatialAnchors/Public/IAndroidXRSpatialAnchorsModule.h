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
#include "AndroidXRSpatialAnchorEntity.h"
#include "AndroidXRSpatialAnchorTypes.h"
#include "AndroidXRSpatialEntitiesModuleBase.h"

class ANDROIDXRSPATIALANCHORS_API IAndroidXRSpatialAnchorsModule : public TIAndroidXRSpatialCapabilityModule<UAndroidXRSpatialAnchorEntity>
{
public:
    /**
    * Singleton-like access to this module's interface.  This is just for
    * convenience! Beware of calling this during the shutdown phase, though. Your
    * module might have been unloaded already.
    *
    * @return Returns singleton instance, loading the module on demand if needed
    */
    static inline IAndroidXRSpatialAnchorsModule& Get()
    {
        return FModuleManager::LoadModuleChecked<IAndroidXRSpatialAnchorsModule>("AndroidXRSpatialAnchors");
    }

    /**
     * Checks to see if this module is loaded and ready.  It is only valid to call
     * Get() if IsAvailable() returns true.
     *
     * @return True if the module is loaded and ready to use
     */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("AndroidXRSpatialAnchors");
    }

    /**
    * Creates a spatial anchor at the specified transform
    * @param[in] The transform at which to create the anchor
    * @param[out] The created anchor
    * @return Whether the underlying API call was successful
    */
    virtual bool CreateSpatialAnchor(const FTransform& AnchorTransform, UAndroidXRSpatialAnchorEntity*& CreatedAnchor) = 0;

    /**
    * Destroys a spatial anchor
    * @param[in] The anchor to destroy
    * @return Whether the underlying API call was successful
    */
    virtual bool DestroySpatialAnchor(UAndroidXRSpatialAnchorEntity* AnchorToDestroy) = 0;

    /**
    * Persists a spatial anchor
    * @param[in] The anchor to persist
    * @param[in] The callback for when the anchor is persisted
    * @param[in] The callback for when the operation fails
    * @return Whether the underlying API call was successful
    */
    virtual bool PersistAnchor(UAndroidXRSpatialAnchorEntity* AnchorToPersist, FAnchorPersistOperationDelegate OnPersistedCallback, FAnchorPersistOperationFailedDelegate OnFailedCallback) = 0;

    /**
    * Unpersists a spatial anchor
    * @param[in] The anchor to unpersist
    * @param[in] The callback for when the anchor is unpersisted
    * @param[in] The callback for when the operation fails
    * @return Whether the underlying API call was successful
    */
    virtual bool UnPersistAnchor(UAndroidXRSpatialAnchorEntity* AnchorToUnpersist, FAnchorPersistOperationDelegate OnUnpersistedCallback, FAnchorPersistOperationFailedDelegate OnFailedCallback) = 0;

    /**
    * Loads all the peristed anchors
    * @return Whether the underlying API call was successful
    */
    virtual bool LoadPersistedAnchors() = 0;
};
