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
#include "PermissionsRequester.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPermissionsGranted, const UPermissionsRequester*, PermissionRequester);

UCLASS(BlueprintType)
class UNREALSAMPLES_API UPermissionsRequester : public UObject
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintAssignable)
    FOnPermissionsGranted OnPermissionsGranted;

    UFUNCTION()
    void OnPermissionGrantedHandler(const TArray<FString>& Permissions, const TArray<bool>& Status);

    UFUNCTION(BlueprintCallable)
    bool IsPermissionGranted(const FString& Permission);

    UFUNCTION(BlueprintCallable)
    bool AreAllPermissionsGranted(const TArray<FString>& Permissions);

    UFUNCTION(BlueprintCallable)
    void RequestPermissions(const TArray<FString>& Permissions);

private:
    UPROPERTY()
    TMap<FString, bool> PermissionsTable{};

    FDelegateHandle PermissionHandle;
};