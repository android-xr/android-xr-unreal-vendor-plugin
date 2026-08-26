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

#include "AndroidXR.h"
#include "OpenXRCore.h"
#include "AndroidXRTypeConversions.h"
#include <IOpenXRHMDModule.h>
#include <IXRTrackingSystem.h>
#include <IHeadMountedDisplay.h>
#include "OpenXRBlueprintFunctionLibrary.h"
#include "AndroidXRBlueprintFunctionLibrary.h"
#include "AndroidXREventProxy.h"

#if PLATFORM_ANDROID
#include <android_native_app_glue.h>
extern struct android_app* GNativeAndroidApp;
#endif

DEFINE_LOG_CATEGORY(LogAndroidXR);

template<typename T> void SetConsoleVariable(const char* Name, T Value)
{
    auto* ConsoleVariable =
        IConsoleManager::Get().FindConsoleVariable(UTF8_TO_TCHAR(Name));
    if (ConsoleVariable)
    {
      ConsoleVariable->Set(Value);
    }
    else
    {
        UE_LOG(LogAndroidXR, Error,
            TEXT("Could not find cvar %s"),
            UTF8_TO_TCHAR(Name));
    }
}

FAndroidXR::FAndroidXR()
: LoaderHandle(nullptr)
{
}

void FAndroidXR::StartupModule()
{
    auto* ConsoleVariable = IConsoleManager::Get().
        FindConsoleVariable(TEXT("xr.DisableOpenXROnAndroidWithoutOculus"));
    if (ConsoleVariable)
    {
        ConsoleVariable->Set(false);
    }

    IAndroidXRModule::StartupModule();
    RegisterOpenXRExtensionModularFeature();
}

void FAndroidXR::ShutdownModule()
{
    UnregisterOpenXRExtensionModularFeature();
    IAndroidXRModule::ShutdownModule();

    if (LoaderHandle)
    {
        FPlatformProcess::FreeDllHandle(LoaderHandle);
        LoaderHandle = nullptr;
    }
}

bool FAndroidXR::GetCustomLoader(PFN_xrGetInstanceProcAddr* OutGetProcAddr)
{
#if PLATFORM_ANDROID
    // Use a custom loader (via GetInstanceProcAddr) for the OpenXR plugin.
    // We need this because Epic still uses Meta's custom openxr loader with the
    // name libopenxr_loader.so and that doesnt work on any platform other than
    // Quest. We therefore package the Khronos openxr loader with the name
    // libandroidxr_openxr_loader.so, get PFN_xrGetInstanceProcAddr from that
    // lib, and provide the func ptr to the main OpenXR plugin via this func.
    // The impl of this func, other than the loader name, is copied from
    // FOpenXRHMDModule::GetDefaultLoader() in
    // Engine/Plugins/Runtime/OpenXR/Source/OpenXRHMD/Private/OpenXRHMDModule.cpp.
    FString LoaderName = "libandroidxr_openxr_loader.so";
    LoaderHandle = FPlatformProcess::GetDllHandle(*LoaderName);

    if (!LoaderHandle)
    {
        UE_LOG(LogAndroidXR, Log, TEXT("Failed to find OpenXR runtime loader."));
        return false;
    }

    *OutGetProcAddr =
        (PFN_xrGetInstanceProcAddr)FPlatformProcess::GetDllExport(
            LoaderHandle, TEXT("xrGetInstanceProcAddr"));

    PFN_xrInitializeLoaderKHR xrInitializeLoaderKHR;
    (*OutGetProcAddr)(XR_NULL_HANDLE,
                      "xrInitializeLoaderKHR",
                      (PFN_xrVoidFunction*)&xrInitializeLoaderKHR);
    if (xrInitializeLoaderKHR == nullptr)
    {
        UE_LOG(LogAndroidXR, Error,
            TEXT("Unable to load OpenXR Android xrInitializeLoaderKHR"));
        return false;
    }

    XrLoaderInitInfoAndroidKHR LoaderInitializeInfoAndroid;
    LoaderInitializeInfoAndroid.type = XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR;
    LoaderInitializeInfoAndroid.next = NULL;
    LoaderInitializeInfoAndroid.applicationVM = GNativeAndroidApp->activity->vm;
    LoaderInitializeInfoAndroid.applicationContext =
        GNativeAndroidApp->activity->clazz;
    XR_ENSURE(xrInitializeLoaderKHR(
        (XrLoaderInitInfoBaseHeaderKHR*)&LoaderInitializeInfoAndroid));
    return true;
#else
    // Use custom loader only on android.
    return false;
#endif
}
void FAndroidXR::PostCreateInstance(XrInstance InInstance)
{
    Instance = InInstance;

    TArray<TTuple<const char *, PFN_xrVoidFunction*>> Functions;

    if (IOpenXRHMDModule::Get().IsExtensionEnabled(XR_ANDROID_PASSTHROUGH_CAMERA_STATE_EXTENSION_NAME))
    {
        Functions.Append({ PASSTHROUGHCAMERA_FUNCTIONS(RESOLVE_OPENXR_FUNC) });
    }
    if (IOpenXRHMDModule::Get().IsExtensionEnabled(XR_EXT_PERFORMANCE_SETTINGS_EXTENSION_NAME))
    {
        Functions.Append({ PERFORMANCESETTINGS_FUNCTIONS(RESOLVE_OPENXR_FUNC) });
    }
    if (IOpenXRHMDModule::Get().IsExtensionEnabled(XR_FB_DISPLAY_REFRESH_RATE_EXTENSION_NAME))
    {
        Functions.Append({ DISPLAYREFRESHRATE_FUNCTIONS(RESOLVE_OPENXR_FUNC) });
    }
#if PLATFORM_ANDROID
    if (IOpenXRHMDModule::Get().IsExtensionEnabled(XR_KHR_ANDROID_THREAD_SETTINGS_EXTENSION_NAME))
    {
        Functions.Append({ APPLICATIONTHREAD_FUNCTIONS(RESOLVE_OPENXR_FUNC) });
    }
#endif // PLATFORM_ANDROID
    if (IOpenXRHMDModule::Get().IsExtensionEnabled(XR_EXT_DEBUG_UTILS_EXTENSION_NAME))
    {
        Functions.Append({ DEBUGUTILS_FUNCTIONS(RESOLVE_OPENXR_FUNC) });
    }

    ResolveOpenXRFunctions(Instance,
        Functions,
        [](const char* UnresolvableFunctionName, XrResult Result)
        {
            UE_LOG(LogAndroidXR, Error,
                TEXT("Unable to resolve function pointer %s (error %s)"),
                UTF8_TO_TCHAR(UnresolvableFunctionName),
                OpenXRResultToString(Result));
        });
}

void FAndroidXR::PostCreateSession(XrSession InSession)
{
    Session = InSession;
}

bool FAndroidXR::GetOptionalExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
    OutExtensions.Add(XR_ANDROID_PASSTHROUGH_CAMERA_STATE_EXTENSION_NAME);
    OutExtensions.Add(XR_EXT_PERFORMANCE_SETTINGS_EXTENSION_NAME);
    OutExtensions.Add(XR_FB_DISPLAY_REFRESH_RATE_EXTENSION_NAME);
#if PLATFORM_ANDROID
    OutExtensions.Add(XR_KHR_ANDROID_THREAD_SETTINGS_EXTENSION_NAME);
#endif
    OutExtensions.Add(XR_EXT_DEBUG_UTILS_EXTENSION_NAME);
    return true;
}

void FAndroidXR::OnEvent(XrSession InSession, const XrEventDataBaseHeader *InHeader)
{
    switch (InHeader->type)
    {
        case XR_TYPE_EVENT_DATA_PERF_SETTINGS_EXT:
        {
            const auto EventDataPerfSettings =
                reinterpret_cast<const XrEventDataPerfSettingsEXT*>(InHeader);
            UAndroidXRBlueprintFunctionLibrary::GetEventProxy()->
                OnPerfSettingsDelegate.Broadcast(
                    AndroidXR::Convert(EventDataPerfSettings->domain),
                    AndroidXR::Convert(EventDataPerfSettings->subDomain),
                    AndroidXR::Convert(EventDataPerfSettings->fromLevel),
                    AndroidXR::Convert(EventDataPerfSettings->toLevel));
            break;
        }
        case XR_TYPE_EVENT_DATA_DISPLAY_REFRESH_RATE_CHANGED_FB:
        {
            const auto EventDataRefreshRateChanged =
                reinterpret_cast<const XrEventDataDisplayRefreshRateChangedFB*>(InHeader);
            UAndroidXRBlueprintFunctionLibrary::GetEventProxy()->
                OnRefreshRateChangedDelegate.Broadcast(
                    EventDataRefreshRateChanged->fromDisplayRefreshRate,
                    EventDataRefreshRateChanged->toDisplayRefreshRate);
            break;
        }
        default:
            break;
    }
}

bool FAndroidXR::GetCameraPassthroughSupported()
{
    if (xrGetSystemProperties != nullptr)
    {
        XrSystemPassthroughCameraStatePropertiesANDROID CameraStateProperties
            { .type = XR_TYPE_SYSTEM_PASSTHROUGH_CAMERA_STATE_PROPERTIES_ANDROID };
        XrSystemProperties SystemProperties{
            .type = XR_TYPE_SYSTEM_PROPERTIES,
            .next = &CameraStateProperties };

        auto Result = xrGetSystemProperties(Instance,
            IOpenXRHMDModule::Get().GetSystemId(), &SystemProperties);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            return !!CameraStateProperties.supportsPassthroughCameraState;
        }

        UE_LOG(LogAndroidXR, Error,
            TEXT("GetCameraPassthroughSupported: "
                "xrGetSystemProperties failed with error %s"),
                OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXR::GetPassthroughCameraState(EAndroidXRPassthroughCameraState& CameraState)
{
    if (xrGetPassthroughCameraStateANDROID != nullptr)
    {
        if (xrGetPassthroughCameraStateANDROID != nullptr)
        {
            XrPassthroughCameraStateGetInfoANDROID CameraStateGetInfo{
                .type = XR_TYPE_PASSTHROUGH_CAMERA_STATE_GET_INFO_ANDROID
            };
            XrPassthroughCameraStateANDROID NativeCameraState;

            auto Result = xrGetPassthroughCameraStateANDROID(Session,
                &CameraStateGetInfo, &NativeCameraState);

            if (XR_UNQUALIFIED_SUCCESS(Result))
            {
                CameraState = AndroidXR::Convert(NativeCameraState);
                return true;
            }

            UE_LOG(LogAndroidXR, Error,
                TEXT("GetPassthroughCameraState: "
                    "xrGetPassthroughCameraStateANDROID failed with error %s"),
                    OpenXRResultToString(Result));
        }
    }
    return false;
}

bool FAndroidXR::PerfSettingsSetPerformanceLevel(EAndroidXRPerfSettingsDomain SettingsDomain,
    EAndroidXRPerfSettingsLevel SettingsLevel)
{
    if (xrPerfSettingsSetPerformanceLevelEXT != nullptr)
    {
        auto Result = xrPerfSettingsSetPerformanceLevelEXT(Session,
            AndroidXR::Convert(SettingsDomain),
            AndroidXR::Convert(SettingsLevel));
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            return true;
        }

        UE_LOG(LogAndroidXR, Error,
            TEXT("PerfSettingsSetPerformanceLevel: "
                "xrPerfSettingsSetPerformanceLevelEXT failed with error %s"),
                OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXR::EnumerateDisplayRefreshRates(TArray<float> &DisplayRefreshRates)
{
    if (xrEnumerateDisplayRefreshRatesFB != nullptr)
    {
        uint32 RefreshRateCount{};
        auto Result = xrEnumerateDisplayRefreshRatesFB(Session,
            0,
            &RefreshRateCount,
            nullptr);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            TArray<float> RefreshRates;
            RefreshRates.SetNum(RefreshRateCount);
            Result = xrEnumerateDisplayRefreshRatesFB(Session,
                RefreshRates.Num(),
                &RefreshRateCount,
                RefreshRates.GetData());
            if (XR_UNQUALIFIED_SUCCESS(Result))
            {
                DisplayRefreshRates = RefreshRates;
                return true;
            }
        }

        UE_LOG(LogAndroidXR, Error,
            TEXT("EnumerateDisplayRefreshRates: "
                "xrEnumerateDisplayRefreshRatesFB failed with error %s"),
                OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXR::GetDisplayRefreshRate(float &DisplayRefreshRate)
{
    if (xrGetDisplayRefreshRateFB != nullptr)
    {
        auto Result = xrGetDisplayRefreshRateFB(Session, &DisplayRefreshRate);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            return true;
        }

        UE_LOG(LogAndroidXR, Error,
            TEXT("GetDisplayRefreshRate: "
                "xrGetDisplayRefreshRateFB failed with error %s"),
                OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXR::RequestDisplayRefreshRate(float DisplayRefreshRate)
{
    if (xrRequestDisplayRefreshRateFB != nullptr)
    {
        auto Result = xrRequestDisplayRefreshRateFB(Session, DisplayRefreshRate);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            return true;
        }

        UE_LOG(LogAndroidXR, Error,
            TEXT("RequestDisplayRefreshRate: "
                "xrRequestDisplayRefreshRateFB failed with error %s"),
                OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXR::SetAndroidApplicationThread(EAndroidXRAndroidThreadType ThreadType, uint32 ThreadID)
{
#if PLATFORM_ANDROID
    if (xrSetAndroidApplicationThreadKHR != nullptr)
    {
        auto Result = xrSetAndroidApplicationThreadKHR(Session,
            AndroidXR::Convert(ThreadType),
            ThreadID);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            return true;
        }

        UE_LOG(LogAndroidXR, Error,
            TEXT("SetAndroidApplicationThread: "
                "xrSetAndroidApplicationThreadKHR failed to set %d with error %s"),
                AndroidXR::Convert(ThreadType),
                OpenXRResultToString(Result));
    }
#endif // PLATFORM_ANDROID
    return false;
}

bool FAndroidXR::SetDebugUtilsObjectName(XrObjectType ObjectType, uint64 ObjectHandle, FString ObjectName)
{
    if (xrSetDebugUtilsObjectNameEXT != nullptr)
    {
        auto StringConversion = StringCast<ANSICHAR>(*ObjectName);

        XrDebugUtilsObjectNameInfoEXT ObjectNameInfo
        {
            .type = XR_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
            .objectType = ObjectType,
            .objectHandle = ObjectHandle,
            .objectName = reinterpret_cast<const char*>(StringConversion.Get())
        };
        auto Result = xrSetDebugUtilsObjectNameEXT(Instance, &ObjectNameInfo);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            return true;
        }

        UE_LOG(LogAndroidXR, Error,
            TEXT("SetDebugUtilsObjectName: "
                "xrSetDebugUtilsObjectNameEXT failed with error %s"),
                OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXR::CreateDebugUtilsMessenger(const XrDebugUtilsMessengerCreateInfoEXT& CreateInfo,
    XrDebugUtilsMessengerEXT& Messenger)
{
    if (xrCreateDebugUtilsMessengerEXT != nullptr)
    {
        auto Result = xrCreateDebugUtilsMessengerEXT(Instance,
            &CreateInfo,
            &Messenger);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            return true;
        }

        UE_LOG(LogAndroidXR, Error,
            TEXT("CreateDebugUtilsMessenger: "
                "xrCreateDebugUtilsMessengerEXT failed with error %s"),
                OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXR::DestroyDebugUtilsMessenger(XrDebugUtilsMessengerEXT& Messenger)
{
    if (xrDestroyDebugUtilsMessengerEXT != nullptr)
    {
        auto Result = xrDestroyDebugUtilsMessengerEXT(Messenger);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            Messenger = XR_NULL_HANDLE;
            return true;
        }

        UE_LOG(LogAndroidXR, Error,
            TEXT("DestroyDebugUtilsMessenger: "
                "xrDestroyDebugUtilsMessengerEXT failed with error %s"),
                OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXR::SubmitDebugUtilsMessage(XrDebugUtilsMessageSeverityFlagsEXT SeverityFlags,
    XrDebugUtilsMessageTypeFlagsEXT TypeFlags,
    const XrDebugUtilsMessengerCallbackDataEXT& CallbackData)
{
    if (xrSubmitDebugUtilsMessageEXT != nullptr)
    {
        auto Result = xrSubmitDebugUtilsMessageEXT(Instance,
            SeverityFlags,
            TypeFlags,
            &CallbackData);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            return true;
        }

        UE_LOG(LogAndroidXR, Error,
            TEXT("SubmitDebugUtilsMessage: "
                "xrSubmitDebugUtilsMessageEXT failed with error %s"),
                OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXR::BeginDebugUtilsLabelRegion(const XrDebugUtilsLabelEXT& LabelInfo)
{
    if (xrSessionBeginDebugUtilsLabelRegionEXT != nullptr)
    {
        auto Result = xrSessionBeginDebugUtilsLabelRegionEXT(Session, &LabelInfo);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            return true;
        }

        UE_LOG(LogAndroidXR, Error,
            TEXT("BeginDebugUtilsLabelRegion: "
                "xrSessionBeginDebugUtilsLabelRegionEXT failed with error %s"),
                OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXR::EndDebugUtilsLabelRegion()
{
    if (xrSessionEndDebugUtilsLabelRegionEXT != nullptr)
    {
        auto Result = xrSessionEndDebugUtilsLabelRegionEXT(Session);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            return true;
        }

        UE_LOG(LogAndroidXR, Error,
            TEXT("EndDebugUtilsLabelRegion: "
                "xrSessionEndDebugUtilsLabelRegionEXT failed with error %s"),
                OpenXRResultToString(Result));
    }
    return false;
}

bool FAndroidXR::InsertDebugUtilsLabel(const XrDebugUtilsLabelEXT& LabelInfo)
{
    if (xrSessionInsertDebugUtilsLabelEXT != nullptr)
    {
        auto Result = xrSessionInsertDebugUtilsLabelEXT(Session, &LabelInfo);
        if (XR_UNQUALIFIED_SUCCESS(Result))
        {
            return true;
        }

        UE_LOG(LogAndroidXR, Error,
            TEXT("InsertDebugUtilsLabel: "
                "xrSessionInsertDebugUtilsLabelEXT failed with error %s"),
                OpenXRResultToString(Result));
    }
    return false;
}

IMPLEMENT_MODULE(FAndroidXR, AndroidXR);
