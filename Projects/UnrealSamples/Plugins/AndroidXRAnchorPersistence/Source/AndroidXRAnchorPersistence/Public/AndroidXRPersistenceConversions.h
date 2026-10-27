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
namespace AndroidXR
{
    inline EAndroidXRAnchorPersistState Convert(XrAnchorPersistStateANDROID NativeType)
    {
        switch (NativeType)
        {
        case XR_ANCHOR_PERSIST_STATE_PERSIST_NOT_REQUESTED_ANDROID:
            return EAndroidXRAnchorPersistState::PersistNotRequested;
        case XR_ANCHOR_PERSIST_STATE_PERSIST_PENDING_ANDROID:
            return EAndroidXRAnchorPersistState::PersistPending;
        case XR_ANCHOR_PERSIST_STATE_PERSISTED_ANDROID:
            return EAndroidXRAnchorPersistState::Persisted;
        default:
            break;
        }
        return EAndroidXRAnchorPersistState::PersistNotRequested;
    }
    inline XrAnchorPersistStateANDROID Convert(EAndroidXRAnchorPersistState UnrealType)
    {
        switch (UnrealType)
        {
        case EAndroidXRAnchorPersistState::PersistNotRequested:
            return XR_ANCHOR_PERSIST_STATE_PERSIST_NOT_REQUESTED_ANDROID;
        case EAndroidXRAnchorPersistState::PersistPending:
            return XR_ANCHOR_PERSIST_STATE_PERSIST_PENDING_ANDROID;
        case EAndroidXRAnchorPersistState::Persisted:
            return XR_ANCHOR_PERSIST_STATE_PERSISTED_ANDROID;
        default:
            break;
        }
        return XR_ANCHOR_PERSIST_STATE_PERSIST_NOT_REQUESTED_ANDROID;
    }
}