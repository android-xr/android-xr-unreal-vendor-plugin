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
#include <CoreMinimal.h>
#include <OpenXRCore.h>
#include "openxr_delta.h"
#include "AndroidXRTypes.generated.h"

/**
 * Types of trackables.
 */
UENUM(BlueprintType)
enum class EAndroidXRTrackableType : uint8
{
    // Indicates that the trackable is not valid.
    NotValid,
    // Indicates that the trackable is a plane.
    Plane,
    // Indicates that the trackable is the depth buffer.
    Depth
};

/**
 * Tracking state.
 */
UENUM(BlueprintType)
enum class EAndroidXRTrackingState : uint8
{
    // Indicates that the trackable or anchor tracking is paused but may be resumed in the future.
    Paused,
    // Tracking has stopped on this Trackable and will never be resumed.
    Stopped,
    // The object is currently tracked and its pose is current.
    Tracking
};

/**
 * Plane type (generally indicates orientation and facing).
 */
UENUM(BlueprintType)
enum class EAndroidXRPlaneType : uint8
{
    // A horizontal plane facing downward (for example a ceiling).
    HorizontalDownwardFacing,
    // A horizontal plane facing upward (for example a floor or tabletop).
    HorizontalUpwardFacing,
    // A vertical plane (for example a wall).
    Vertical,
    // A plane with an arbitrary orientation.
    Arbitrary
};

/**
 * Semantic plane label.
 */
UENUM(BlueprintType)
enum class EAndroidXRPlaneLabel : uint8
{
    // It was not possible to label the plane
    Unknown,
    // The plane is a wall.
    Wall,
    // The plane is a floor.
    Floor,
    // The plane is a ceiling.
    Ceiling,
    // The plane is a table.
    Table
};

/**
 * Contains opaque pointer to XrTrackableANDROID.
 */
USTRUCT(BlueprintType)
struct FAndroidXRTrackable
{
    GENERATED_BODY()

    XrTrackableANDROID Trackable{};
};

/**
 * Raycast hit.
 */
USTRUCT(BlueprintType)
struct ANDROIDXR_API FAndroidXRRaycastHit
{
    GENERATED_BODY()

    /**
     * The trackable that was hit.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|Raycast")
    FAndroidXRTrackable Trackable{};

    /**
     * The transform of the intersection.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|Raycast")
    FTransform Transform;

    /**
     * The type of the trackable that was hit.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|Raycast")
    EAndroidXRTrackableType TrackableType{};
};

/**
 * Contains opaque pointer to XrTrackableTrackerANDROID.
 */
USTRUCT(BlueprintType)
struct ANDROIDXR_API FAndroidXRTrackableTracker
{
    GENERATED_BODY()

    XrTrackableTrackerANDROID Tracker{};
};

/**
 * Trackable plane.
 */
USTRUCT(BlueprintType)
struct ANDROIDXR_API FAndroidXRTrackablePlane
{
    GENERATED_BODY()

    // Tracking state
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|Trackables")
    EAndroidXRTrackingState TrackingState{};

    // Current pose
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|Trackables")
    FTransform Transform{};

    // Extents (half width/height)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|Trackables")
    FVector2D Extents{};

    // Plane type
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|Trackables")
    EAndroidXRPlaneType PlaneType{};

    // Semantic plane label
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|Trackables")
    EAndroidXRPlaneLabel PlaneLabel{};

    // Plane that subsumes this plane (if any)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|Trackables")
    FAndroidXRTrackable SubsumedByPlane;

    // Last updated time
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|Trackables")
    FTimespan LastUpdatedTime{};

    // Plane polygon vertices.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|Trackables")
    TArray<FVector2D> Vertices;
};

/**
 * Passthrough camera state
 */
UENUM(BlueprintType)
enum class EAndroidXRPassthroughCameraState : uint8
{
    Disabled,
    Initializing,
    Ready,
    Error
};

/**
 * Contains opaque pointer to an XrSpace
 */
USTRUCT(BlueprintType)
struct FAndroidXRSpace
{
    GENERATED_BODY()

    XrSpace Space{};
};

/**
 * Performance metrics counter flags
 */
UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor="true"))
enum class EAndroidXRPerformanceMetricsCounterFlags : uint8
{
    None = 0x00 UMETA(Hidden),
    // Indicates any of the values in FAndroidXRPerformanceMetricsCounter is valid
    AnyValueValid = 0x01,
    // Indicates the IntValue in FAndroidXRPerformanceMetricsCounter is valid
    IntValueValid = 0x02,
    // Indicates the FloatValue in FAndroidXRPerformanceMetricsCounter is valid
    FloatValueValid = 0x04
};

/**
 * Performance metrics counter unit
 */
UENUM(BlueprintType)
enum class EAndroidXRPerformanceMetricsCounterUnit : uint8
{
    // The performance counter unit is generic (unspecified)
    Generic,
    // The performance counter unit is a percentage
    Percentage,
    // The performance counter unit is millisecond
    Millisecond,
    // The performance counter unit is byte
    Bytes,
    // The performance counter unit is hertz (Hz)
    Hertz
};

/**
 * Performance metrics counter
 */
USTRUCT(BlueprintType)
struct FAndroidXRPerformanceMetricsCounter
{
    GENERATED_BODY()

    // Flags
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|PerformanceMetrics", meta=(Bitmask, BitmaskEnum="/Script/AndroidXRPerformanceMetrics.EAndroidXRPerformanceMetricsCounterFlags"))
    int32 Flags{};

    // Unit
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|PerformanceMetrics")
    EAndroidXRPerformanceMetricsCounterUnit Unit{};

    // Integer value
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|PerformanceMetrics")
    int64 IntValue{};

    // Float value
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndroidXR|PerformanceMetrics")
    float FloatValue{};
};

/**
 * Depth camera resolutions
 */
UENUM(BlueprintType)
enum class EAndroidXRDepthCameraResolution : uint8
{
    // The resolution of the depth and confidence images is 80x80.
    Resolution80 UMETA(DisplayName="80x80"),
    // The resolution of the depth and confidence images is 160x160.
    Resolution160 UMETA(DisplayName="160x160"),
    // The resolution of the depth and confidence images is 320x320.
    Resolution320 UMETA(DisplayName="320x320")
};

/**
 * Flags used to control depth swapchain image creation
 */
UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor="true"))
enum class EAndroidXRDepthSwapchainCreateFlag : uint8
{
    None = 0x00 UMETA(Hidden),

    // Indicates the swapchain should provide smooth depth imagess.
    SmoothDepthImage = 0x01,
    // Indicates the swapchain should provide smooth depth confidence imagess.
    SmoothConfidenceImage = 0x02,
    // Indicates the swapchain should provide raw depth imagess.
    RawDepthImage = 0x04,
    // Indicates the swapchain should provide raw depth confidence imagess.
    RawConfidenceImage = 0x08,
};

/**
 * Contains opaque pointer to an XrDepthSwapchainANDROID
 */
 USTRUCT(BlueprintType)
struct FAndroidXRDepthSwapchain
{
    GENERATED_BODY()

    XrDepthSwapchainANDROID DepthSwapchain{};
};

/**
 * Contains opaque pointer to an XrDepthSwapchainImageANDROID
 */
 USTRUCT(BlueprintType)
struct FAndroidXRDepthSwapchainImage
{
    GENERATED_BODY()

    XrDepthSwapchainImageANDROID SwapchainImage{};
};

/**
 * Field of view
 */
USTRUCT(BlueprintType)
struct FAndroidXRFieldOfView
{
    GENERATED_BODY()

    // Left angle
    UPROPERTY(BlueprintReadOnly)
    float AngleLeft{};

    // Right angle
    UPROPERTY(BlueprintReadOnly)
    float AngleRight{};

    // Up angle
    UPROPERTY(BlueprintReadOnly)
    float AngleUp{};

    // Down angle
    UPROPERTY(BlueprintReadOnly)
    float AngleDown{};
};

/**
 * Depth view (essentially an eye's view)
 */
USTRUCT(BlueprintType)
struct FAndroidXRDepthView
{
    GENERATED_BODY()

    // Field of view
    UPROPERTY(BlueprintReadOnly)
    FAndroidXRFieldOfView FieldOfView;

    // Current pose of this view
    UPROPERTY(BlueprintReadOnly)
    FTransform Transform;
};

/**
 * Depth acquire result
 */
USTRUCT(BlueprintType)
struct FAndroidXRDepthAcquireResult
{
    GENERATED_BODY()

    // The index of the image that was acquired
    UPROPERTY(BlueprintReadOnly)
    int32 AcquiredIndex{};

    // Timespan acquired
    UPROPERTY(BlueprintReadOnly)
    FTimespan ExposureTimespan;

    // View details (array will be of size 2)
    UPROPERTY(BlueprintReadOnly)
    TArray<FAndroidXRDepthView> Views;
};

/**
 * Perf settings domain
 */
UENUM(BlueprintType)
enum class EAndroidXRPerfSettingsDomain : uint8
{
    // CPU domain
    CPU,
    // GPU domain
    GPU
};

/**
 * Perf settings subdomain
 */
UENUM(BlueprintType)
enum class EAndroidXRPerfSettingsSubDomain : uint8
{
    // Compositing subdomain
    Compositing,
    // Rendering subdomain
    Rendering,
    // Thermal subdomain
    Thermal
};

/**
 * Perf settings level
 */
UENUM(BlueprintType)
enum class EAndroidXRPerfSettingsLevel : uint8
{
    // Used by the application to indicate that it enters a non-XR section
    // (head-locked / static screen), during which power savings are to be
    // prioritized
    PowerSavings,
    // Used by the application to indicate that it enters a low and stable
    // complexity section, during which reducing power is more important
    // than occasional late rendering frames
    SustainedLow,
    // Used by the application to indicate that it enters a high or dynamic
    // complexity section, during which the XR Runtime strives for
    // consistent XR compositing and frame rendering within a thermally
    // sustainable range
    SustainedHigh,
    // Used to indicate that the application enters a section with very high
    // complexity, during which the XR Runtime is allowed to step up beyond
    // the thermally sustainable range
    Boost
};

/**
 * Perf settings notification level
 */
UENUM(BlueprintType)
enum class EAndroidXRPerfSettingsNotificationLevel : uint8
{
    // Notifies that the sub-domain has reached a level where no further actions
    // other than currently applied are necessary.
    Normal,
    // Notifies that the sub-domain has reached an early warning level where the
    // application should start proactive mitigation actions with the goal to
    // return to the normal level
    Warning,
    // Notifies that the sub-domain has reached a critical level with significant
    // performance degradation
    Impaired
};

/**
 * Thread types
 */
UENUM(BlueprintType)
enum class EAndroidXRAndroidThreadType : uint8
{
    // Hints the XR runtime that the thread is doing time critical CPU tasks
    ApplicationMain,
    // Hints the XR runtime that the thread is doing background CPU tasks
    ApplicationWorker,
    // Hints the XR runtime that the thread is doing time critical graphics
    // device tasks
    RendererMain,
    // Hints the XR runtime that the thread is doing background graphics
    // device tasks
    RendererWorker
};

/**
 * Hand interaction types
 */
UENUM(BlueprintType)
enum class EOpenXRHandInteractionType : uint8
{
    Aim,
    Grip,
    Pinch,
    Poke,
    Palm,
    GripSurface,
    Count UMETA(Hidden)
};

/**
 * Environment Blend Mode Types
 */
UENUM(BlueprintType)
enum class EAndroidXREnvironmentBlendMode : uint8
{
    None = 0,
    //The composition layers will be displayed with no view of the physical world behind them.
    Opaque = 1,
    //The composition layers will be additively blended with the real world behind the display.
    Additive = 2,
    //The composition layers will be alpha-blended with the real world behind the display.
    AlphaBlend = 3,
};

// Broadcast via the AndroidXREventProxy in the event of
// XR_TYPE_EVENT_DATA_PERF_SETTINGS_EXT
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAndroidXRPerfSettingsDynamicDelegate,
    EAndroidXRPerfSettingsDomain, Domain,
    EAndroidXRPerfSettingsSubDomain, SubDomain,
    EAndroidXRPerfSettingsNotificationLevel, FromLevel,
    EAndroidXRPerfSettingsNotificationLevel, ToLevel);

// Broadcast via the AndroidXREventProxy in the event of
// XR_TYPE_EVENT_DATA_DISPLAY_REFRESH_RATE_CHANGED_FB
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAndroidXRDisplayRefreshRateChangedDynamicDelegate,
    float, FromRate,
    float, ToRate);

FORCEINLINE FGuid ToFGuid(const XrUuidEXT& XrUuid)
{
    auto GuidBits = reinterpret_cast<const uint32*>(XrUuid.data);
    return FGuid(GuidBits[0], GuidBits[1], GuidBits[2], GuidBits[3]);
}

FORCEINLINE XrUuidEXT ToXrUuid(const FGuid& Guid)
{
    XrUuidEXT XrUuid;
    FMemory::Memcpy(XrUuid.data, reinterpret_cast
            <const uint8*>(&Guid.A), sizeof(XrUuid.data));
    return XrUuid;
}


#pragma region FaceTracking
/**
 * Identifies the different states of the face tracker.
 */
UENUM(BlueprintType)
enum class EAndroidXRFaceTrackingState : uint8
{
    // Indicates that face tracking is paused but may be resumed in the future.
    Paused,
    // Tracking has stopped but the client still has an active face tracker.
    Stopped,
    // The face is tracked and its pose is current.
    Tracked
};

/**
 * The blend shapes defined by the face tracking extension.
 * Each parameter in this enum is an index into a blend shape array whose values are type of float and the runtime normalizes to 1 - 0
 */
UENUM(BlueprintType)
enum class EAndroidXRFaceParameterIndices : uint8
{
    // The left brow lowerer blendshape parameter.
    Brow_Lowerer_L = 0,
    // The right brow lowerer blendshape parameter.
    Brow_Lowerer_R = 1,
    // The left cheek puff blendshape parameter.
    Cheek_Puff_L = 2,
    // The right cheek puff blendshape parameter.
    Cheek_Puff_R = 3,
    // The left cheek raiser blendshape parameter.
    Cheek_Raiser_L = 4,
    // The right cheek raiser blendshape parameter.
    Cheek_Raiser_R = 5,
    // The left cheek suck blendshape parameter.
    Cheek_Suck_L = 6,
    // The right cheek suck blendshape parameter.
    Cheek_Suck_R = 7,
    // The bottom chin raiser blendshape parameter.
    Chin_Raiser_B = 8,
    // The top chin raiser blendshape parameter.
    Chin_Raiser_T = 9,
    // The left dimpler blendshape parameter.
    Dimpler_L = 10,
    // The right dimpler lowerer blendshape parameter.
    Dimpler_R = 11,
    // The left eyes closed blendshape parameter.
    Eyes_Closed_L = 12,
    // The right eyes closed blendshape parameter.
    Eyes_Closed_R = 13,
    // The left eyes look down blendshape parameter.
    Eyes_Look_Down_L = 14,
    // The right eyes look down blendshape parameter.
    Eyes_Look_Down_R = 15,
    // The left look left blendshape parameter.
    Eyes_Look_Left_L = 16,
    // The left look right blendshape parameter.
    Eyes_Look_Left_R = 17,
    // The right look left blendshape parameter.
    Eyes_Look_Right_L = 18,
    // The right look right blendshape parameter.
    Eyes_Look_Right_R = 19,
    // The left eyes look up blendshape parameter.
    Eyes_Look_Up_L = 20,
    // The right eyes look up blendshape parameter.
    Eyes_Look_Up_R = 21,
    // The left inner brow raiser blendshape parameter.
    Inner_Brow_Raiser_L = 22,
    // The right inner brow raiser blendshape parameter.
    Inner_Brow_Raiser_R = 23,
    // The jaw drop blendshape parameter.
    Jaw_Drop = 24,
    // The left jaw sideways blendshape parameter.
    Jaw_Sideways_Left = 25,
    // The right jaw sideways blendshape parameter.
    Jaw_Sideways_Right = 26,
    // The jaw thrust blendshape parameter.
    Jaw_Thrust = 27,
    // The left lid tightener blendshape parameter.
    Lid_Tightener_L = 28,
    // The right lid tightener blendshape parameter.
    Lid_Tightener_R = 29,
    // The left corner lip depressor blendshape parameter.
    Lip_Corner_Depressor_L = 30,
    // The right corner lip depressor blendshape parameter.
    Lip_Corner_Depressor_R = 31,
    // The left corner lip puller blendshape parameter.
    Lip_Corner_Puller_L = 32,
    // The right corner lip puller blendshape parameter.
    Lip_Corner_Puller_R = 33,
    // The left bottom lip funneler blendshape parameter.
    Lip_Funneler_LB = 34,
    // The left top lip funneler blendshape parameter.
    Lip_Funneler_LT = 35,
    // The right bottom lip funneler blendshape parameter.
    Lip_Funneler_RB = 36,
    // The right top lip funneler blendshape parameter.
    Lip_Funneler_RT = 37,
    // The left lip pressor blendshape parameter.
    Lip_Pressor_L = 38,
    // The right lip pressor blendshape parameter.
    Lip_Pressor_R = 39,
    // The left lip pucker blendshape parameter.
    Lip_Pucker_L = 40,
    // The right lip pucker blendshape parameter.
    Lip_Pucker_R = 41,
    // The left lip stretcher blendshape parameter.
    Lip_Stretcher_L = 42,
    // The right lip stretcher blendshape parameter.
    Lip_Stretcher_R = 43,
    // The left bottom lip suck blendshape parameter.
    Lip_Suck_LB = 44,
    // The left top lip suck blendshape parameter.
    Lip_Suck_LT = 45,
    // The right bottom lip suck blendshape parameter.
    Lip_Suck_RB = 46,
    // The right top lip suck blendshape parameter.
    Lip_Suck_RT = 47,
    // The left lip tightener blendshape parameter.
    Lip_Tightener_L = 48,
    // The right lip tightener blendshape parameter.
    Lip_Tightener_R = 49,
    // The lips toward blendshape parameter.
    Lips_Toward = 50,
    // The left lower lip depressor blendshape parameter.
    Lower_Lip_Depressor_L = 51,
    // The right lower lip depressor blendshape parameter.
    Lower_Lip_Depressor_R = 52,
    // The mouth move left blendshape parameter.
    Mouth_Left = 53,
    // The mouth move right blendshape parameter.
    Mouth_Right = 54,
    // The left nose wrinkler blendshape parameter.
    Nose_Wrinkler_L = 55,
    // The right nose wrinkler blendshape parameter.
    Nose_Wrinkler_R = 56,
    // The left outer brow raiser blendshape parameter.
    Outer_Brow_Raiser_L = 57,
    // The right outer brow raiser blendshape parameter.
    Outer_Brow_Raiser_R = 58,
    // The left lid raiser blendshape parameter.
    Upper_Lid_Raiser_L = 59,
    // The right lid raiser blendshape parameter.
    Upper_Lid_Raiser_R = 60,
    // The left lip raiser blendshape parameter.
    Upper_Lip_Raiser_L = 61,
    // The right lip raiser blendshape parameter.
    Upper_Lip_Raiser_R = 62,
    // The tongue out blendshape parameter.
    Tongue_Out = 63,
    // The tongue left puller blendshape parameter.
    Tongue_Left = 64,
    // The right right puller blendshape parameter.
    Tongue_Right = 65,
    // The right up puller blendshape parameter.
    Tongue_Up = 66,
    // The right down puller blendshape parameter.
    Tongue_Down = 67,
};

UENUM(BlueprintType)
enum class EAndroidXRFaceTrackingConfidenceRegions : uint8
{
    // Confidence corresponding to the lower region.
    Lower = 0,
    // Confidence corresponding to the left upper region.
    Left_Upper = 1,
    // Confidence corresponding to the right upper region.
    Right_Upper = 2,
};

/**
 * Contains opaque pointer to XrFaceTrackerANDROID.
 */
USTRUCT(BlueprintType)
struct FAndroidXRFaceTracker
{
    GENERATED_BODY()

    XrFaceTrackerANDROID FaceTracker = XR_NULL_HANDLE;
};

/**
 * The face tracking state and the facial expression blend shapes
 */
USTRUCT(BlueprintType)
struct FAndroidXRFaceState
{
    GENERATED_BODY()
    // Whether the data is valid for the current frame or not
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool IsValid{};

    // The validaty status of the face tracking
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EAndroidXRFaceTrackingState FaceTrackingState{};

    // The time at which the expressions were tracked
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FTimespan SampleTime{};

    // The weights of facial expression blend shapes
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<float> Parameters{};

    // The confidence values for regions
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<float> ConfidenceRegions{};
};

#pragma endregion

/**
 * Scene mesh semantic label set
 */
UENUM(BlueprintType)
enum class EAndroidXRSceneMeshSemanticLabelSet : uint8
{
    // No semantic labels are needed for the mesh
    None,
    // Semantic label information is needed for the mesh
    Default
};

/**
 * Scene mesh semantic label
 */
UENUM(BlueprintType)
enum class ESceneMeshSemanticLabel : uint8
{
    // This semantic indicates that the corresponding mesh element represents an unknown object.
    Other,
    // This semantic indicates that the corresponding mesh element represents a floor.
    Floor,
    // This semantic indicates that the corresponding mesh element represents a ceiling.
    Ceiling,
    // This semantic indicates that the corresponding mesh element represents a wall.
    Wall,
    // This semantic indicates that the corresponding mesh element represents a table.
    Table
};

/**
 * Scene mesh tracking state
 */
UENUM(BlueprintType)
enum class ESceneMeshTrackingState : uint8
{
    // The internal tracker is not yet ready to provide mesh data.
    Initializing,
    // The internal tracker is actively tracking.
    Tracking,
    // The internal tracker is waiting for valid measurements to integrate since the last mesh update.
    Waiting,
    // The internal tracker has not received valid measurements for multiple cycles and is in an error state.
    Error
};

/**
 * Contains opaque pointer to an XrSceneMeshingTrackerANDROID
 * Also stores whether or not the tracker was created with normals,
 * so we know whether or not to supply storage
 */
 USTRUCT(BlueprintType)
struct FAndroidXRSceneMeshingTracker
{
    GENERATED_BODY()
    // If the created tracker was created with normals
    bool bHasNormals{ };
    // The native mesh tracker
    XrSceneMeshingTrackerANDROID Tracker{};
};

/**
 * Contains opaque pointer to an XrSceneMeshSnapshotANDROID
 * Also stores whether or not the snapshot was created from
 * a tracker with normals enabled, so we don't have to also
 * pass the tracker around
 */
USTRUCT(BlueprintType)
struct FAndroidXRSceneMeshSnapshot
{
    GENERATED_BODY()
    // If the created tracker was created with normals
    bool bHasNormals{ };

    // The native mesh snapshot
    XrSceneMeshSnapshotANDROID Snapshot{};
};

/**
 * The Scene submesh state
 */
USTRUCT(BlueprintType)
struct FAndroidXRSceneSubmeshState
{
    GENERATED_BODY()

    // The Id of the Submesh
    UPROPERTY(BlueprintReadOnly)
    FGuid SubmeshId{};

    // The last updated timespan for the submesh
    UPROPERTY(BlueprintReadOnly)
    FTimespan LastUpdatedTimespan{};

    // The pose of the submesh in base space
    UPROPERTY(BlueprintReadOnly)
    FTransform PoseInBaseSpace{};

    // The extents of the submesh
    UPROPERTY(BlueprintReadOnly)
    FVector Extents{};
};

/**
* The submesh data
*/
USTRUCT(BlueprintType)
struct FAndroidXRSceneSubmeshData
{
    GENERATED_BODY()

    // The Id of the submesh
    UPROPERTY(BlueprintReadOnly)
    FGuid SubmeshId{};

    // The vertex positions of the submesh
    UPROPERTY(BlueprintReadOnly)
    TArray<FVector> VertexPositions{};

    // The vertex normals of the submesh
    UPROPERTY(BlueprintReadOnly)
    TArray<FVector> VertexNormals{};

    // The vertex semantics of the submesh
    UPROPERTY(BlueprintReadOnly)
    TArray<uint8> VertexSemantics{};

    // The indicies of the submesh
    UPROPERTY(BlueprintReadOnly)
    TArray<int32> Indexes{};
};

FORCEINLINE XrExtent3Df ToXrExtent3Df(FVector UnrealExtent, float Scale)
{
    // OpenXR extents are diameters
    Scale *= 2.0f;

    return
    {
        .width = static_cast<float>(UnrealExtent.Y * Scale),
        .height = static_cast<float>(UnrealExtent.Z * Scale),
        .depth = static_cast<float>(UnrealExtent.X * Scale)
    };
}

FORCEINLINE FVector ToFVector(XrExtent3Df NativeExtent, float Scale)
{
    // OpenXR extents are diameters
    Scale *= 0.5f;

    return FVector(NativeExtent.depth * Scale,
        NativeExtent.width * Scale, NativeExtent.height * Scale);
}