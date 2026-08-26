# Changelog

All notable changes to the Android XR Extensions for Unreal will be added
here. It includes changes that affect public APIs, samples and runtime
behaviors.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this package adheres to
[Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-08-03

Official **1.0.0** release of Android XR Extensions for Unreal (`com.google.xr.extensions`), removing Developer Preview / Beta designations across all plugins.

### Added
- **Spatial Entities & Component Architecture**:
  - `AndroidXRSpatialEntities` (`XR_EXT_spatial_entity`) - Core spatial entities and extensible component architecture.
  - `AndroidXRSpatialPlaneTracking` (`XR_EXT_spatial_plane_tracking`) - Spatial plane tracking component.
  - `AndroidXRSpatialMarkerTracking` (`XR_EXT_spatial_marker_tracking`) - Spatial marker and QR code tracking component (includes Aura device fix).
  - `AndroidXRSpatialAnchors` (`XR_EXT_spatial_anchors`) - Spatial anchor components with room-mapping persistence and unpersistence support.
  - `AndroidXRTrackingSubsystem` - Unified tracking subsystem and scene components management.
- **Light Estimation & Cubemaps**:
  - `AndroidXRLightEstimation` (`XR_ANDROID_light_estimation`, `XR_ANDROID_light_estimation_cubemap`) - Dynamic scene environment light estimation and lighting cubemap generation.
- **Global Passthrough Dimming**:
  - `AndroidXRGlobalPassthroughDimming` (`XR_ANDROID_global_passthrough_dimming`) - Runtime global passthrough dimming control.
- **System Extension Properties**:
  - `AndroidXRSystemExtensionProperties` (`XR_ANDROID_enumerate_system_extension_properties`) - Support for enumerating runtime system extension properties.
- **AndroidXR Streaming**:
  - `AndroidXRStreaming` - Project settings and runtime support for AndroidXR remote streaming.

### Updated & Fixed
- Completed runtime implementation and Blueprint integration for `XR_ANDROID_recommended_resolution`.
- Restored removal of out-of-view spatial entities transitioning to `PAUSED_EXT` or `STOPPED_EXT` tracking states to prevent infinite trackable accumulation.
- Fixed timeout handling and return values for spatial anchor persistence and unpersistence.
- Bug fixes across Scene Meshing, Eye Tracking, and Blueprint UI samples.

## [0.9.0] - 2026-05-19

This is the first release of **Android XR Extensions for Unreal
<com.google.xr.extensions> v0.9.0** developer preview package. This version
supports the following features:

- XR_EXT_hand_interaction
- XR_FB_hand_tracking_mesh
- XR_ANDROID_composition_layer_passthrough_mesh
- XR_ANDROID_eye_tracking
- XR_ANDROID_depth_texture
- XR_ANDROID_device_anchor_persistence
- XR_ANDROID_face_tracking
- XR_ANDROID_passthrough_camera_state
- XR_ANDROID_performance_metrics
- XR_ANDROID_raycast
- XR_ANDROID_recommended_resolution
- XR_ANDROID_scene_mesh
- XR_ANDROID_trackables
- XR_ANDROID_trackables_object