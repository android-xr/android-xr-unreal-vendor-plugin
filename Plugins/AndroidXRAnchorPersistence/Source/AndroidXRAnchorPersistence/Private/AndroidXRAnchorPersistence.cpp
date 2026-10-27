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

#include "AndroidXRAnchorPersistence.h"
#include <IOpenXRHMDModule.h>
#include <IOpenXRHMD.h>
#include <IXRTrackingSystem.h>
#include "AnchorPersistenceSubsystem.h"
#include "AndroidXRTypeConversions.h"
#include "AndroidXRPersistenceConversions.h"

DEFINE_LOG_CATEGORY(LogAndroidXRAnchorPersistence);

FAndroidXRAnchorPersistenceModule::FAndroidXRAnchorPersistenceModule()
{
}

void FAndroidXRAnchorPersistenceModule::StartupModule()
{
    IAndroidXRAnchorPersistenceModule::StartupModule();
    RegisterOpenXRExtensionModularFeature();
}

void FAndroidXRAnchorPersistenceModule::ShutdownModule()
{
    UnregisterOpenXRExtensionModularFeature();
    IAndroidXRAnchorPersistenceModule::ShutdownModule();
}

void FAndroidXRAnchorPersistenceModule::PostCreateInstance(XrInstance InInstance)
{
    Instance = InInstance;

    ResolveOpenXRFunctions(Instance,
        {
            ANDROIDXRANCHORPERSISTENCE_FUNCTIONS(RESOLVE_OPENXR_FUNC)
        },
        [](const char* UnresolvableFunctionName, XrResult Result)
        {
            UE_LOG(LogAndroidXRAnchorPersistence, Error,
                TEXT("Unable to resolve function pointer %s (error %s)"),
                UTF8_TO_TCHAR(UnresolvableFunctionName),
                OpenXRResultToString(Result));
        });
    if (GetAnchorPersistenceSupported())
    {
        AnchorPersistenceSubsystem = NewObject<UAnchorPersistenceSubsystem>();
        //If Anchor persistence is supported , start the AnchorPersistenceSubsystem
        if (AnchorPersistenceSubsystem)
        {
            AnchorPersistenceSubsystem->SetOpenXRFunctions(xrPersistAnchorANDROID, xrUnpersistAnchorANDROID, xrCreatePersistedAnchorSpaceANDROID, xrDestroySpace);
            AnchorPersistenceSubsystem->ShouldRunAnchorPersistence(true);
        }
    }
}

void FAndroidXRAnchorPersistenceModule::PostCreateSession(XrSession InSession)
{
    Session = InSession;
}

bool FAndroidXRAnchorPersistenceModule::GetRequiredExtensions(TArray<const ANSICHAR *> &OutExtensions)
{
    OutExtensions.Add(XR_ANDROID_DEVICE_ANCHOR_PERSISTENCE_EXTENSION_NAME);
    return true;
}

bool FAndroidXRAnchorPersistenceModule::GetAnchorPersistenceSupported() const
{
    if (xrGetSystemProperties != nullptr)
    {
        XrSystemDeviceAnchorPersistencePropertiesANDROID AnchorPersistenceProperties
            { .type = XR_TYPE_SYSTEM_DEVICE_ANCHOR_PERSISTENCE_PROPERTIES_ANDROID };
        XrSystemProperties SystemProperties{
            .type = XR_TYPE_SYSTEM_PROPERTIES,
            .next = &AnchorPersistenceProperties };

        auto Result = xrGetSystemProperties(Instance,
            IOpenXRHMDModule::Get().GetSystemId(), &SystemProperties);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            return !!AnchorPersistenceProperties.supportsAnchorPersistence;
        }

        UE_LOG(LogAndroidXRAnchorPersistence, Error,
            TEXT("GetAnchorPersistenceSupported: "
                "xrGetSystemProperties failed with error %s"),
                OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXRAnchorPersistenceModule::EnumerateSupportedPersistenceAnchorTypes(TArray<EAndroidXRTrackableType> &SupportedTypes)
{
    if (xrEnumerateSupportedPersistenceAnchorTypesANDROID != nullptr)
    {
        uint32_t CapacityOutput;

        auto Result = xrEnumerateSupportedPersistenceAnchorTypesANDROID(Instance,
            IOpenXRHMDModule::Get().GetSystemId(),
            0,
            &CapacityOutput,
            nullptr);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            TArray<XrTrackableTypeANDROID> NativeTypes;
            NativeTypes.SetNum(CapacityOutput);

            Result = xrEnumerateSupportedPersistenceAnchorTypesANDROID(Instance,
                IOpenXRHMDModule::Get().GetSystemId(),
                NativeTypes.Num(),
                &CapacityOutput,
                NativeTypes.GetData());
            if (XR_UNQUALIFIED_SUCCESS(Result))
            {
                SupportedTypes.Reserve(NativeTypes.Num());
                for (const auto& NativeType : NativeTypes)
                {
                    SupportedTypes.Add(AndroidXR::Convert(NativeType));
                }
                return true;
            }
        }

        UE_LOG(LogAndroidXRAnchorPersistence, Error,
            TEXT("EnumerateSupportedPersistenceAnchorTypes: "
                "xrEnumerateSupportedPersistenceAnchorTypesANDROID failed with error %s"),
                OpenXRResultToString(Result));

    }
    return false;
}

bool FAndroidXRAnchorPersistenceModule::CreateAnchorPersistence(FAndroidXRAnchorPersistence&AnchorPersistence)
{
    if (xrCreateDeviceAnchorPersistenceANDROID != nullptr)
    {
        XrDeviceAnchorPersistenceCreateInfoANDROID CreateInfo
        {
            .type = XR_TYPE_DEVICE_ANCHOR_PERSISTENCE_CREATE_INFO_ANDROID,
        };
        XrDeviceAnchorPersistenceANDROID NativeAnchorPersistence{};
        auto Result = xrCreateDeviceAnchorPersistenceANDROID(Session,
            &CreateInfo, &NativeAnchorPersistence);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            AnchorPersistence.Persistence = NativeAnchorPersistence;
            return true;
        }

        UE_LOG(LogAndroidXRAnchorPersistence, Error,
            TEXT("CreateAnchorPersistence: "
                "xrCreateDeviceAnchorPersistenceANDROID failed with error %s"),
                OpenXRResultToString(Result));

    }
    return false;
}

bool FAndroidXRAnchorPersistenceModule::DestroyAnchorPersistence(FAndroidXRAnchorPersistence&AnchorPersistence)
{
    if (xrDestroyDeviceAnchorPersistenceANDROID != nullptr)
    {
        auto Result = xrDestroyDeviceAnchorPersistenceANDROID(AnchorPersistence.Persistence);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            AnchorPersistence.Persistence = 0;
            return true;
        }

        UE_LOG(LogAndroidXRAnchorPersistence, Error,
            TEXT("DestroyAnchorPersistence: "
                "xrDestroyDeviceAnchorPersistenceANDROID failed with error %s"),
                OpenXRResultToString(Result));

    }
    return false;
}

bool FAndroidXRAnchorPersistenceModule::GetAnchorPersistState(const FAndroidXRAnchorPersistence&AnchorPersistence,
    const FGuid &PersistedID,
    EAndroidXRAnchorPersistState &PersistState)
{
    if (xrGetAnchorPersistStateANDROID != nullptr)
    {
        XrAnchorPersistStateANDROID NativePersistState;
        auto XrUuid = ToXrUuid(PersistedID);
        auto Result = xrGetAnchorPersistStateANDROID(AnchorPersistence.
            Persistence, &XrUuid, &NativePersistState);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            PersistState = AndroidXR::Convert(NativePersistState);
            return true;
        }

        UE_LOG(LogAndroidXRAnchorPersistence, Error,
            TEXT("GetAnchorPersistState: "
                "xrGetAnchorPersistStateANDROID failed with error %s"),
                OpenXRResultToString(Result));

    }
    return false;
}

bool FAndroidXRAnchorPersistenceModule::EnumeratePersistedAnchors(const FAndroidXRAnchorPersistence&AnchorPersistence,
    TArray<FGuid> &PersistedAnchors)
{
    if (xrEnumeratePersistedAnchorsANDROID != nullptr)
    {
        uint32_t CapacityOutput;
        auto Result = xrEnumeratePersistedAnchorsANDROID(AnchorPersistence.
            Persistence, 0, &CapacityOutput, nullptr);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            TArray<XrUuidEXT> NativeAnchorIds;
            NativeAnchorIds.SetNum(CapacityOutput);

            Result = xrEnumeratePersistedAnchorsANDROID(AnchorPersistence.
                Persistence,
                NativeAnchorIds.Num(),
                &CapacityOutput,
                NativeAnchorIds.GetData());
            if (XR_UNQUALIFIED_SUCCESS(Result))
            {
                PersistedAnchors.Reserve(NativeAnchorIds.Num());
                for (const auto& NativeAnchorId : NativeAnchorIds)
                {
                    PersistedAnchors.Add(ToFGuid(NativeAnchorId));
                }
                return true;
            }
        }

        UE_LOG(LogAndroidXRAnchorPersistence, Error,
            TEXT("EnumeratePersistedAnchors: "
                "xrEnumeratePersistedAnchorsANDROID failed with error %s"),
                OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXRAnchorPersistenceModule::PersistAnchorAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FAndroidXRSpace& AnchorSpace, FOnAnchorPersisted& OnAnchorPersisted, FOnAnchorAsyncActionFailed& OnPersistAnchorFailed)
{
    if (!AnchorPersistenceSubsystem)
    {
        return false;
    }
    AnchorPersistenceSubsystem->PersistAnchorAsync(AnchorPersistence, AnchorSpace, OnAnchorPersisted, OnPersistAnchorFailed);
    return true;
}

bool FAndroidXRAnchorPersistenceModule::UnpersistAnchorAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FGuid& PersistedId, FOnAnchorUnPersisted& OnAnchorUnPersisted, FOnAnchorAsyncActionFailed& OnAnchorUnpersistFailed)
{
    if (!AnchorPersistenceSubsystem)
    {
        return false;
    }
    AnchorPersistenceSubsystem->UnpersistAnchorAsync(AnchorPersistence, PersistedId, OnAnchorUnPersisted, OnAnchorUnpersistFailed);
    return true;
}


bool FAndroidXRAnchorPersistenceModule::CreatePersistedAnchorSpaceAsync(const FAndroidXRAnchorPersistence& AnchorPersistence, const FGuid& PersistedId, const FAndroidXRSpace& AnchorSpace, FOnPersistedAnchorSpaceCreated& OnPersistedAnchorSpaceCreated, FOnAnchorAsyncActionFailed& OnPersistedAnchorCreateFailed)
{
    if (!AnchorPersistenceSubsystem)
    {
        return false;
    }
    AnchorPersistenceSubsystem->CreatePersistedAnchorSpaceAsync(AnchorPersistence, PersistedId, AnchorSpace, OnPersistedAnchorSpaceCreated, OnPersistedAnchorCreateFailed);
    return true;
}

IMPLEMENT_MODULE(FAndroidXRAnchorPersistenceModule, AndroidXRAnchorPersistence);
