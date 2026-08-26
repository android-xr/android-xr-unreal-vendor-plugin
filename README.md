# Android XR Plugins for Unreal

1.  AndroidXR: required for all Android XR Extensions' features.
2.  Depth Texture
3.  Device Anchor Persistence
4.  Eyetracking
5.  Facetracking
6.  Passthrough Mesh
7.  Performance Metrics
8.  Scene Meshing
9.  Streaming
10. Trackables Core
11. Trackables Object
12. Hand Interaction
13. Hand Interaction Input Device
14. Hand Tracking Mesh
15. Global Passthrough Dimming
16. Light Estimation
17. Recommended Resolution
18. Spatial Entities
19. Spatial Marker Tracking
20. Spatial Plane Tracking
21. Spatial Anchors
22. System Extension Properties

## Supported Development Platforms
- Windows
- Mac

Linux is not supported.

## Supported Unreal Engine Versions
- 5.6

## How to use

1.  Ensure your Unreal Engine project is closed.
2.  Locate the main directory of your Unreal Engine project.
3.  If one doesn't exist, create a new folder named Plugins in the root of your project directory.
4.  Each downloaded plugin should have its' own folder which should be placed in the Plugins folder.
5.  Launch your project again. Unreal Engine should detect the new plugins and prompt you if necessary.
6.  Go to **Edit > Plugins** and ensure the newly installed plugins are listed under the "Installed" category and are enabled.
7.  Go to **Edit > Project Settings > Platforms > Android** If there is a red box that states the project is not configured for the Android platform, click the **Configure Now** button and **Accept SDK License** if needed.
8.  Package your project targeting **Android** platform.
