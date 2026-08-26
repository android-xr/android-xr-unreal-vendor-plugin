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
#include "AndroidXRTypes.h"
#include "CoreMinimal.h"
#include "AndroidXRFuture.h"
#include "AndroidXRSpatialBuffer.h"
#include "AndroidXRSpatialEntitiesTypes.generated.h"

/**
 * Spatial capabilities define a runtime�s abilities to discover entities that have a guaranteed set of components on them
 */
UENUM(BlueprintType)
enum class ESpatialCapability : uint8 {
    // Plane tracking
    PlaneTracking = 0,
    //Capability to be able to detect and track QR codes
    MarkerTracking_QR_Code,
    //Capability to be able to detect and track Micro QR codes
    MarkerTracking_Micro_QR_Code,
    //Capability to be able to detect and track Aruco Markers
    MarkerTracking_Aruco,
    //Capability to be able to detect and track AprilTags
    MarkerTracking_April_Tag,
    //Capability to be able to create spatial anchors
    Anchor,
    //Capability to track objects
    ObjectTracking,
    //Capability to perform a depth raycast against entities
    DepthRaycast,
};

/**
* Some capabilities have parameters exposed to the application to configure how the component data is computed by the runtime. These dimensions of parameterization/configurability are known as capability features
*/
UENUM(BlueprintType)
enum class ESpatialCapabilityFeature : uint8 {
    //Capability feature to allow applications to specify the size for the markers
    FixedSizeMarkers,
    //Capability feature to allow applications to specify if markers are static
    StaticMarkers,
};

/**
* A spatial entity has one or more components which provide data or behaviors for that entity
*/
UENUM(BlueprintType)
enum class ESpatialComponentType : uint8 {
    None,
    //Component that provides the 2D bounds for a spatial entity
    Bounded2D,
    //Component that provides the 3D bounds for a spatial entity
    Bounded3D,
    //Component that provides the FSpatialEntityId of the parent for a spatial entity
    Parent,
    //Component that provides a 3D mesh for a spatial entity
    Mesh3D,
    //Component that provides the plane alignment enum for a spatial entity
    PlaneAlignment,
    //Component that provides a 2D mesh for a spatial entityzs
    Mesh2D,
    //Component that provides a 2D boundary polygon for a spatial entity
    Polygon2D,
    //Component that provides a semantic label for a plane
    PlaneSemanticLabel,
    //A component describing the marker type
    Marker,
    //Component that provides the location for an anchor
    Anchor,
    //Component that provides the persisted UUID for a spatial entity
    Persistence,
    //Component that provides a semantic label for an object
    ObjectSemanticLabel,
    //Component that provides the result of a raycast
    RaycastResult,
    //The comonent that the entity is subsumed by
    SubsumedByAndroid
};

/**
* The possible spatial entity tracking states
*/
UENUM(BlueprintType)
enum class ESpatialEntityTrackingState : uint8 {
    //The runtime has stopped tracking this entity and will never resume tracking it
    Stopped,
    //The runtime has paused tracking this entity but may resume tracking it in the future
    Paused,
    //The runtime is currently tracking this entity and its component data is valid
    Tracking
};

USTRUCT(BlueprintType)
struct ANDROIDXRSPATIALENTITIES_API FAndroidXRSpatialCapabilityInfo
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Entities")
    bool bIsCapabilitySupported{};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Entities")
    TSet<ESpatialComponentType> SupportedComponents{};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AndroidXR|Spatial Entities")
    TSet<ESpatialCapabilityFeature> SupportedFeatures{};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpatialContextCreatedDelegate);

#pragma region NativeTypes

//Component lists:
typedef struct FSpatialComponentList
{
    virtual XrBaseOutStructure* GetNativeHeader() = 0;
    virtual void Allocate(uint32 Size) = 0;
    virtual ~FSpatialComponentList() = default;
}FSpatialComponentList;


template<typename TNativeListType, typename TNativeElementType>
struct TFSpatialComponentList : public FSpatialComponentList
{
    XrBaseOutStructure* GetNativeHeader() override
    {
        return reinterpret_cast<XrBaseOutStructure*>(&NativeListType);
    }
    TArray<TNativeElementType> NativeBuffer{};

    void Allocate(uint32 Size) override
    {
        NativeBuffer.Reset();
        NativeBuffer.SetNum(Size);
        NativeListType = { XrType, nullptr, static_cast<uint32_t>(NativeBuffer.Num()), NativeBuffer.GetData() };
    }

    TNativeListType NativeListType{};
    XrStructureType XrType{};

    TFSpatialComponentList(XrStructureType InXrType)
        : NativeListType{ InXrType }, XrType{InXrType}
    {
    }

    const TNativeElementType& operator[](uint32 Index)
    {
        return NativeBuffer[Index];
    }

    TNativeElementType& operator[](uint32 Index) const
    {
        return NativeBuffer[Index];
    }

    bool IsValidIndex(uint32 Index) const
    {
        return NativeBuffer.IsValidIndex(Index);
    }

    bool Get(int Index, TNativeElementType& Data)
    {
        if(!IsValidIndex(Index))
        {
            return false;
        }
        Data = NativeBuffer[Index];
        return true;
    }
};

typedef struct FSpatialEntityInfo
{
    XrSpatialEntityIdEXT EntityId{};
    XrSpatialEntityTrackingStateEXT EntityTrackingState{};
    int ComponentIndex{};
    XrSpatialSnapshotEXT Snapshot{};

    TFSpatialComponentList<XrSpatialComponentBounded2DListEXT, XrSpatialBounded2DDataEXT>* Bounds{};
    TFSpatialComponentList<XrSpatialComponentParentListEXT, XrSpatialEntityIdEXT>* Parents{};
    TFSpatialComponentList<XrSpatialComponentMesh3DListEXT, XrSpatialMeshDataEXT>* Meshes3D{};
    TFSpatialComponentList<XrSpatialComponentBounded3DListEXT, XrBoxf>* Bounds3D{};
}FSpatialEntityInfo;


struct ANDROIDXRSPATIALENTITIES_API FNativeSpatialCapabilityInfo
{
    bool bIsSupported{};
    TSet<XrSpatialComponentTypeEXT> SupportedComponents{};
    TSet<XrSpatialCapabilityFeatureEXT> SupportedFeatures{};

    bool IsCapabilitySupported() const
    {
        return bIsSupported;
    }

    bool IsComponentSupported(XrSpatialComponentTypeEXT Component) const
    {
        return SupportedComponents.Contains(Component);
    }

    bool IsFeatureSupported(XrSpatialCapabilityFeatureEXT Feature) const
    {
        return SupportedFeatures.Contains(Feature);
    }
};

struct ANDROIDXRSPATIALENTITIES_API FNativeSpatialCapabilitiesInfo
{
    TMap<XrSpatialCapabilityEXT, FNativeSpatialCapabilityInfo> Capabilities{};
    bool IsCapabilitySupported(XrSpatialCapabilityEXT Capability) const
    {
        auto Entry = Capabilities.Find(Capability);
        if(!Entry)
        {
            return false;
        }
        return Entry->IsCapabilitySupported();
    }

    bool IsComponentSupported(XrSpatialCapabilityEXT Capability, XrSpatialComponentTypeEXT ComponentType) const
    {
        auto Entry = Capabilities.Find(Capability);
        if(!Entry)
        {
            return false;
        }
        return Entry->IsComponentSupported(ComponentType);
    }

    bool IsFeatureSupported(XrSpatialCapabilityEXT Capability, XrSpatialCapabilityFeatureEXT Feature) const
    {
        auto Entry = Capabilities.Find(Capability);
        if(!Entry)
        {
            return false;
        }
        return Entry->IsFeatureSupported(Feature);
    }
};
#pragma endregion