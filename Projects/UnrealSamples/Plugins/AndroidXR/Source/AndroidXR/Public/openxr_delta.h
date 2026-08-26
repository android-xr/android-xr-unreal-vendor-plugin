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

// This file encapsulates the delta between the snapshotted version of
// openxr.h included with Unreal and the most current release; it also
// collates the xr_android headers packaged with the plugin.

#if PLATFORM_ANDROID
#define XR_USE_PLATFORM_ANDROID 1
#include <openxr/openxr_platform.h>
#endif

#include "third_party/xr_android/xr_android_trackables.h"
#include "third_party/xr_android/xr_android_raycast.h"
#include "third_party/xr_android/xr_android_passthrough_camera_state.h"
#include "third_party/xr_android/xr_android_performance_metrics.h"
#include "third_party/xr_android/xr_android_recommended_resolution.h"
#include "third_party/xr_android/xr_android_device_anchor_persistence.h"
#include "third_party/xr_android/xr_android_depth_texture.h"
#include "third_party/xr_android/xr_android_face_tracking.h"
#include "third_party/xr_android/xr_android_eye_tracking.h"
#include "third_party/xr_android/xr_android_scene_meshing.h"
#include "third_party/xr_android/xr_android_composition_layer_passthrough_mesh.h"
#include "third_party/xr_android/xr_android_trackables_object.h"
#include "third_party/xr_android/xr_android_global_passthrough_dimming.h"
#include "third_party/xr_android/xr_spatial_entities.h"
#include "third_party/xr_android/xr_android_enumerate_system_extension_properties.h"
#include "third_party/xr_android/xr_android_light_estimation.h"
#include "third_party/xr_android/xr_android_light_estimation_cubemap.h"