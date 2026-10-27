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

#include "AndroidXRAnchorPersistenceTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AndroidXRAnchorPersistenceBlueprintFunctionLibrary.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAnchorPersistedDynamic, const FAndroidXRSpace&, AnchorSpace, const FGuid&, PersistedId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnchorUnpersistedDynamic);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPersistedAnchorSpaceCreatedDynamic, const FAndroidXRSpace&, AnchorSpace, const FGuid&, PersistedId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAsyncActionFailedDynamic);

UCLASS(ClassGroup=AndroidXRAnchorPersistence)
class ANDROIDXRANCHORPERSISTENCE_API UAndroidXRAnchorPersistenceBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    /**
     * Determines whether or not an FAndroidXRAnchorPersistence is valid.
     * @param[in] AnchorPersistence AnchorPersistence to be verified.
     * @return True if the anchor persistence is valid.
     */
    UFUNCTION(BlueprintPure, Category="AndroidXR|Anchor Persistence Function Library")
    static bool IsValidAnchorPersistence(const FAndroidXRAnchorPersistence& AnchorPersistence);

    /**
     * Determines whether or not anchor persistence is supported.
     * @return True if anchor persistence is supported.
     */
    UFUNCTION(BlueprintPure, Category="AndroidXR|Anchor Persistence Function Library")
    static bool GetAnchorPersistenceSupported();

    /**
     * Enumerates the types of trackables which support persistent anchors.
     * @param[out] SupportedTypes The trackable types which support persistent anchors.
     * @return True if the underlying API call was successful.
     */
    UFUNCTION(BlueprintCallable, Category="AndroidXR|Anchor Persistence Function Library")
    static bool EnumerateSupportedPersistenceAnchorTypes(TArray<EAndroidXRTrackableType>& SupportedTypes);

    /**
     * Creates an anchor persistence.
     * @param[out] AnchorPersistence The anchor persistence that was created.
     * @return True if the underlying API call was successful.
     */
    UFUNCTION(BlueprintCallable, Category="AndroidXR|Anchor Persistence Function Library")
    static bool CreateAnchorPersistence(FAndroidXRAnchorPersistence& AnchorPersistence);

    /**
     * Destroys an anchor persistence.
     * @param[in, out] AnchorPersistence The anchor persistence to be destroyed.
     * @return True if the underlying API call was successful.
     */
    UFUNCTION(BlueprintCallable, Category="AndroidXR|Anchor Persistence Function Library")
    static bool DestroyAnchorPersistence(UPARAM(ref) FAndroidXRAnchorPersistence& AnchorPersistence);

    /**
     * Gets the persist state of an anchor.
     * @param[in] AnchorPersistence The anchor persistence with which the anchor is associated.
     * @param[in] PersistedID The persisted ID of the anchor.
     * @param[out] PersistState The persist state of the anchor.
     * @return True if the underlying API call was successful.
     */
    UFUNCTION(BlueprintCallable, Category="AndroidXR|Anchor Persistence Function Library")
    static bool GetAnchorPersistState(const FAndroidXRAnchorPersistence& AnchorPersistence,
        const FGuid &PersistedID,
        EAndroidXRAnchorPersistState& PersistState);

    /**
     * Enumerates the persisted anchors associated with an anchor persistence.
     * @param[in] AnchorPersistence The anchor persistence to be queried.
     * @param[out] PersistedAnchors The persisted anchors associated with the provided
     * anchor persistence.
     * @return True if the underlying API call was successful.
     */
    UFUNCTION(BlueprintCallable, Category="AndroidXR|Anchor Persistence Function Library")
    static bool EnumeratePersistedAnchors(const FAndroidXRAnchorPersistence& AnchorPersistence,
        TArray<FGuid>& PersistedAnchors);
};

UCLASS(ClassGroup = AndroidXRAnchorPersistence)
class ANDROIDXRANCHORPERSISTENCE_API UAndroidXRAnchorPersistenceAsyncAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable, meta=(DisplayPriority=200))
    FOnAsyncActionFailedDynamic OnAsyncActionFailed{};
protected:
    void GetAsyncActionFailedCallback(FOnAnchorAsyncActionFailed & ActionFailedCallback);
    FAndroidXRAnchorPersistence AnchorPersistence{};
    FAndroidXRSpace AnchorSpace{};
    FGuid PersistedId{};

private:
    void OnActionFailedCallback();
};

UCLASS(ClassGroup = AndroidXRAnchorPersistence)
class ANDROIDXRANCHORPERSISTENCE_API UPersistAnchor : public UAndroidXRAnchorPersistenceAsyncAction
{
    GENERATED_BODY()
public:

    /**
    * Persists an anchor asynchronously.
    * @param[in] AnchorPersistence The anchor persistence with which the anchor should
    * be associated.
    * @param[in] AnchorSpace The anchor space.
    * @param[out] PersistedID The persisted ID of the anchor.
    * @return A blueprint latent node that fires off the OnAnchorPersisted event when the operation has finished successfully.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Anchor Persistence Function Library", meta=(BlueprintInternalUseOnly = "true"))
    static UPersistAnchor* PersistAnchorAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FAndroidXRSpace& AnchorSpace);

    void Activate() override;

    UPROPERTY(BlueprintAssignable)
    FOnAnchorPersistedDynamic OnAnchorPersisted;

private:
    UFUNCTION()
    void PersistAnchorCallback(const FAndroidXRSpace& PersistedAnchorSpace, const FGuid& PersistedAnchorId);
    FAndroidXRAnchorPersistence AnchorPersistence{};
    FAndroidXRSpace AnchorSpace{};
    FGuid PersistedId{};
};

UCLASS(ClassGroup = AndroidXRAnchorPersistence)
class ANDROIDXRANCHORPERSISTENCE_API UUnpersistAnchor : public UAndroidXRAnchorPersistenceAsyncAction
{
    GENERATED_BODY()
public:
    /**
    * Unpersists a persisted anchor asynchronously.
    * @param[in] AnchorPersistence The anchor persistence from which the specified anchor
    * is to be unpersisted.
    * @param[in] PersistedID The persisted ID of the anchor to be unpersisted.
    * @return A blueprint latent node that fires off the OnAnchorUnpersisted event when the operation has finished successfully.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Anchor Persistence Function Library")
    static UUnpersistAnchor* UnpersistAnchorAsync(const FAndroidXRAnchorPersistence& AnchorPersistence,const FGuid& PersistedId);

    void Activate() override;

    UPROPERTY(BlueprintAssignable)
    FOnAnchorUnpersistedDynamic OnAnchorUnpersisted;

private:
    UFUNCTION()
    void UnpersistAnchorCallback();
    FAndroidXRAnchorPersistence AnchorPersistence{};
    FAndroidXRSpace AnchorSpace{};
    FGuid PersistedId{};
};

UCLASS(ClassGroup = AndroidXRAnchorPersistence)
class ANDROIDXRANCHORPERSISTENCE_API UCreatePersistedAnchorSpace : public UAndroidXRAnchorPersistenceAsyncAction
{
    GENERATED_BODY()
public:
    /**
    * Creates a space for a persisted anchor asynchronously.
    * @param[in] AnchorPersistence The anchor persistence with which the anchor is associated.
    * @param[in] PersistedID The persisted ID of the anchor.
    * @param[out] AnchorSpace The space created for the anchor.
    * @return A blueprint latent node that fires off the OnPersistedAnchorSpaceCreated event when the operation has finished successfully.
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Anchor Persistence Function Library")
    static UCreatePersistedAnchorSpace* CreatePersistedAnchorSpaceAsync(const FAndroidXRAnchorPersistence& AnchorPersistence,const FGuid& PersistedId, const FAndroidXRSpace& AnchorSpace);

    void Activate() override;

    UPROPERTY(BlueprintAssignable)
    FOnPersistedAnchorSpaceCreatedDynamic OnPersistedAnchorSpaceCreated;

private:
    UFUNCTION()
    void PersistedAnchorCreatedCallback(const FAndroidXRSpace& CreatedAnchorSpace, const FGuid& PersistedAnchorId);
};
