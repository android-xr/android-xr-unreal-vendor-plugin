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
#include "AndroidXRStreamingConsts.generated.h"

UENUM()
enum class EAndroidXRStringKey : uint8
{
    NoTargetSelected,
    InvalidTarget,
    NoCompatibleDevicesFound,
    SeachingForDevices,
    TargetSelectedMessage,
    StreamingErrorText,
    StreamingNotificationTitle,
    ClientStartingMessage,
    ClientStartedSuccessMessage,
    ClientStartFailedMessage,
    ClientStoppingMessage,
    ClientStoppedSuccessMessage,
    ClientStopFailMessage,
    StreamingClientRunning,
    ConnectedDevicesText,
    RefreshButtonText,
    ResetButtonText,
};

UENUM()
enum class EAndroidXRClientStreamingMode : uint8
{
    USB,
    Wifi
};

class ANDROIDXRSTREAMINGEDITOR_API FAndroidXRStreamingConsts
{
public:
    FAndroidXRStreamingConsts();

    static TSharedRef<FAndroidXRStreamingConsts> Get();

    inline static FText GetText(EAndroidXRStringKey Key)
    {
        return FAndroidXRStreamingConsts::Get()->GetTextInternal(Key);
    }

    template<typename ...ArgType>
    inline static FText GetText(EAndroidXRStringKey Key, ArgType... Args)
    {
        return FAndroidXRStreamingConsts::Get()->GetTextInternal(Key, Args...);
    }

private:
    FText GetTextInternal(EAndroidXRStringKey Key);
    template<typename... ArgType>
    FText GetTextInternal(EAndroidXRStringKey Key, ArgType... Args);

    inline static TSharedPtr<FAndroidXRStreamingConsts> Instance{};
};

template<typename ...ArgType>
inline FText FAndroidXRStreamingConsts::GetTextInternal(EAndroidXRStringKey Key, ArgType ...Args)
{
    return FText::Format(GetTextInternal(Key), FText::FromString(Args)...);
}