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
#include "AndroidXRTrackingSubsystem.h"
#include "AndroidXRSpatialTrackingSubsystem.h"
#include "AndroidXRSpatialDiscoveryRaycastTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AndroidXRSpatialDepthRaycastSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDepthRaycastCompleted, const FSpatialRaycastHitInfo&, RaycastHitInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDepthRaycastFailed);

/**
* The base class for depth async operations (has to be a base class due to how blueprint async nodes display only one delegate per class definition in its output node)
*/
UCLASS(ClassGroup = AndroidXRSpatialDepthRaycast)
class ANDROIDXRSPATIALDEPTHRAYCAST_API UAndroidXRSpatialDepthRaycastAsyncActionBase : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
public:

    /**
    * The callback for when the depth raycast fails
    */
    UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Depth Raycast")
    FOnDepthRaycastFailed OnDepthRaycastFailed;
};

/**
* The asynchronous operation for depth raycast
*/
UCLASS(ClassGroup = AndroidXRSpatialDepthRaycast)
class ANDROIDXRSPATIALDEPTHRAYCAST_API UAndroidXRSpatialDepthRaycastAsyncAction : public UAndroidXRSpatialDepthRaycastAsyncActionBase
{
    GENERATED_BODY()
public:
    void Activate() override;

    /**
    * Perfroms a depth raycast asynchronously
    * @param[in] The raycast config to perform depth raycast with
    * @return Returns the depth raycast async operation node
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Depth Raycast", meta = (BlueprintInternalUseOnly = "true"))
    static UAndroidXRSpatialDepthRaycastAsyncAction* SpatialDepthRaycastAsync(const FSpatialRaycastConfig& InRaycastConfig);

    /**
    * The callback for when depth raycast is completed
    */
    UPROPERTY(BlueprintAssignable, Category = "AndroidXR|Depth Raycast")
    FOnDepthRaycastCompleted OnDepthRaycastCompleted;

private:
    UFUNCTION()
    void DepthRaycastCompletedHandler(const FSpatialRaycastHitInfo& RaycastHitInfo);

    UFUNCTION()
    void DepthRaycastFailedHandler();
    FSpatialRaycastConfig RaycastConfig{};
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDepthRaycastFinishedMulticast, const TArray<FSpatialRaycastHitInfo>&, DepthResults);

UCLASS()
class ANDROIDXRSPATIALDEPTHRAYCAST_API UAndroidXRSpatialDepthRaycastSubsystem : public UAndroidXRSpatialEntitiesSubsystem
{
    GENERATED_BODY()
public:

    /**
    * Checks for depth raycast support
    * @return Returns true if the runtime supports depth raycast
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Depth Raycast")
    bool IsDepthRaycastSupported() const;

    /**
    * Checks if the depth raycast can be performed (if a previous depth raycast request is being processed, this will return false)
    * @return Returns true if the runtime can perform a depth raycast
    */
    UFUNCTION(BlueprintCallable, Category = "AndroidXR|Spatial Depth Raycast")
    bool CanPerformDepthRaycast() const;

    /**
    * Perform a depth raycast asynchronously
    * @param[in] The raycast config to perform depth raycast with
    * @param[in] The callback to execute when the operation finishes
    * @param[in] The callback for when the operation fails
    * @return True if the underlying API call was successful
    */
    bool DepthRaycastAsync(const FSpatialRaycastConfig& RaycastConfig, TDelegate<void(const FSpatialRaycastHitInfo&)> DepthRaycastCompletedCallback, TDelegate<void()> OnFailedCallback);
protected:

    void GetRequiredPermissions(TArray<FString>& Permissions) override;
    FAndroidXRSpatialCapabilityModule* GetCapabilityModule() override;
};
