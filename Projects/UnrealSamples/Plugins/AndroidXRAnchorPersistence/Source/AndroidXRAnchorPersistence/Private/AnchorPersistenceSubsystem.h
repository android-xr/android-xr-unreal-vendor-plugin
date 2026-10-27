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
#pragma warning(disable:4146)

#include "CoreMinimal.h"
#include "AndroidXRAnchorPersistenceTypes.h"
#include "Tickable.h"
#include "AnchorPersistenceSubsystem.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogAndroidXRAnchorPersistenceSubsystem, Log, All);

UCLASS()
class ANDROIDXRANCHORPERSISTENCE_API UAnchorPersistenceSubsystem : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	UAnchorPersistenceSubsystem();
	void PersistAnchorAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FAndroidXRSpace& AnchorSpace, FOnAnchorPersisted& OnAnchorPersisted, FOnAnchorAsyncActionFailed& OnPersistAnchorFailed);
	void CreatePersistedAnchorSpaceAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FGuid& PersistedId, const FAndroidXRSpace& AnchorSpace, FOnPersistedAnchorSpaceCreated& OnPersistedAnchorCreated, FOnAnchorAsyncActionFailed& OnPersistAnchorCreateFailed);
	void UnpersistAnchorAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FGuid& PersistedId, FOnAnchorUnPersisted& OnAnchorUnPersisted, FOnAnchorAsyncActionFailed& OnAnchorUnpersistFailed);
	void Tick(float DeltaTime) override;
	TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UAnchorPersistenceSubsystem, STATGROUP_Tickables);
	}

	void SetOpenXRFunctions(PFN_xrPersistAnchorANDROID PersistFunction, PFN_xrUnpersistAnchorANDROID UnpersistFunction, PFN_xrCreatePersistedAnchorSpaceANDROID CreatePersistSpaceFunction, PFN_xrDestroySpace DestroySpaceFunction);

	bool IsTickable() const override;

	void ShouldRunAnchorPersistence(bool bShouldRun);

private:

	void PersistAnchors();
	void CreatePersistedAnchorSpaces();
	void UnpersistAnchors();

	bool bShouldRunAnchorSubsystem;
	PFN_xrPersistAnchorANDROID xrPersistAnchorAndroid{};
	PFN_xrUnpersistAnchorANDROID xrUnpersistAnchorANDROID{};
	PFN_xrCreatePersistedAnchorSpaceANDROID xrCreatePersistedAnchorSpaceANDROID{};
	PFN_xrDestroySpace xrDestroySpace{};

	typedef struct FAnchorPersistInfoBase
	{
		FAndroidXRAnchorPersistence AnchorPersistence{};
		FAndroidXRSpace AnchorSpace{};
		FGuid PersistedId{};
		FOnAnchorAsyncActionFailed OnAsycActionFailed{};
	}FAnchorPersistInfoBase;

	typedef struct FAnchorPersistInfo : public FAnchorPersistInfoBase
	{
		FOnAnchorPersisted OnAnchorPersisted{};
	}FAnchorPersistInfo;

	typedef struct FPersistAnchorCreateInfo : public FAnchorPersistInfoBase
	{
		FOnPersistedAnchorSpaceCreated OnPersistedAnchorCreated{};
	}FPersistAnchorCreateInfo;

	typedef struct FUnPersistAnchorInfo : public FAnchorPersistInfoBase
	{
		FOnAnchorUnPersisted OnAnchorUnPersisted{};
	}FUnPersistAnchorInfo;

	TMap<XrSpace, FAnchorPersistInfo> AnchorsToPersist{};
	TMap<FGuid, FPersistAnchorCreateInfo> PersistedAnchorsToCreate{};
	TMap<FGuid, FUnPersistAnchorInfo> AnchorsToUnPersist{};
};
