#ifndef XR_SPATIAL_ENTITIES_H_
#define XR_SPATIAL_ENTITIES_H_ 1

/*
** Copyright 2017-2025 The Khronos Group Inc.
**
** SPDX-License-Identifier: Apache-2.0 OR MIT
*/

/*
** This header is generated from the Khronos OpenXR XML API Registry.
**
*/


#ifdef __cplusplus
extern "C" {
#endif
#ifndef XR_EXT_spatial_entity
    // XR_EXT_spatial_entity is a preprocessor guard. Do not pass it to API calls.
#define XR_EXT_spatial_entity 1

#define XR_NULL_SPATIAL_ENTITY_ID_EXT 0

#define XR_NULL_SPATIAL_BUFFER_ID_EXT 0

    XR_DEFINE_ATOM(XrSpatialEntityIdEXT)
        XR_DEFINE_ATOM(XrSpatialBufferIdEXT)
        XR_DEFINE_HANDLE(XrSpatialEntityEXT)
        XR_DEFINE_HANDLE(XrSpatialContextEXT)
        XR_DEFINE_HANDLE(XrSpatialSnapshotEXT)
#define XR_EXT_spatial_entity_SPEC_VERSION 1
#define XR_EXT_SPATIAL_ENTITY_EXTENSION_NAME "XR_EXT_spatial_entity"

#define   XR_TYPE_SPATIAL_CAPABILITY_COMPONENT_TYPES_EXT ((XrStructureType) 1000740000)
#define   XR_TYPE_SPATIAL_CONTEXT_CREATE_INFO_EXT ((XrStructureType) 1000740001)
#define   XR_TYPE_CREATE_SPATIAL_CONTEXT_COMPLETION_EXT ((XrStructureType) 1000740002)
#define   XR_TYPE_SPATIAL_DISCOVERY_SNAPSHOT_CREATE_INFO_EXT ((XrStructureType) 1000740003)
#define   XR_TYPE_CREATE_SPATIAL_DISCOVERY_SNAPSHOT_COMPLETION_INFO_EXT ((XrStructureType) 1000740004)
#define   XR_TYPE_CREATE_SPATIAL_DISCOVERY_SNAPSHOT_COMPLETION_EXT ((XrStructureType) 1000740005)
#define   XR_TYPE_SPATIAL_COMPONENT_DATA_QUERY_CONDITION_EXT ((XrStructureType) 1000740006)
#define   XR_TYPE_SPATIAL_COMPONENT_DATA_QUERY_RESULT_EXT ((XrStructureType) 1000740007)
#define   XR_TYPE_SPATIAL_BUFFER_GET_INFO_EXT ((XrStructureType) 1000740008)
#define   XR_TYPE_SPATIAL_COMPONENT_BOUNDED_2D_LIST_EXT ((XrStructureType) 1000740009)
#define   XR_TYPE_SPATIAL_COMPONENT_BOUNDED_3D_LIST_EXT ((XrStructureType) 1000740010)
#define   XR_TYPE_SPATIAL_COMPONENT_PARENT_LIST_EXT ((XrStructureType) 1000740011)
#define   XR_TYPE_SPATIAL_COMPONENT_MESH_3D_LIST_EXT ((XrStructureType) 1000740012)
#define   XR_TYPE_SPATIAL_ENTITY_FROM_ID_CREATE_INFO_EXT ((XrStructureType) 1000740013)
#define   XR_TYPE_SPATIAL_UPDATE_SNAPSHOT_CREATE_INFO_EXT ((XrStructureType) 1000740014)
#define   XR_TYPE_EVENT_DATA_SPATIAL_DISCOVERY_RECOMMENDED_EXT ((XrStructureType) 1000740015)
#define   XR_TYPE_SPATIAL_FILTER_TRACKING_STATE_EXT ((XrStructureType) 1000740016)
#define   XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_PLANE_TRACKING_EXT ((XrStructureType) 1000741000)
#define   XR_TYPE_SPATIAL_COMPONENT_PLANE_ALIGNMENT_LIST_EXT ((XrStructureType) 1000741001)
#define   XR_TYPE_SPATIAL_COMPONENT_MESH_2D_LIST_EXT ((XrStructureType) 1000741002)
#define   XR_TYPE_SPATIAL_COMPONENT_POLYGON_2D_LIST_EXT ((XrStructureType) 1000741003)
#define   XR_TYPE_SPATIAL_COMPONENT_PLANE_SEMANTIC_LABEL_LIST_EXT ((XrStructureType) 1000741004)
#define   XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_QR_CODE_EXT ((XrStructureType) 1000743000)
#define   XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_MICRO_QR_CODE_EXT ((XrStructureType) 1000743001)
#define   XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_ARUCO_MARKER_EXT ((XrStructureType) 1000743002)
#define   XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_APRIL_TAG_EXT ((XrStructureType) 1000743003)
#define   XR_TYPE_SPATIAL_MARKER_SIZE_EXT ((XrStructureType) 1000743004)
#define   XR_TYPE_SPATIAL_MARKER_STATIC_OPTIMIZATION_EXT ((XrStructureType) 1000743005)
#define   XR_TYPE_SPATIAL_COMPONENT_MARKER_LIST_EXT ((XrStructureType) 1000743006)
#define   XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_ANCHOR_EXT ((XrStructureType) 1000762000)
#define   XR_TYPE_SPATIAL_COMPONENT_ANCHOR_LIST_EXT ((XrStructureType) 1000762001)
#define   XR_TYPE_SPATIAL_ANCHOR_CREATE_INFO_EXT ((XrStructureType) 1000762002)
#define   XR_TYPE_SPATIAL_PERSISTENCE_CONTEXT_CREATE_INFO_EXT ((XrStructureType) 1000763000)
#define   XR_TYPE_CREATE_SPATIAL_PERSISTENCE_CONTEXT_COMPLETION_EXT ((XrStructureType) 1000763001)
#define   XR_TYPE_SPATIAL_CONTEXT_PERSISTENCE_CONFIG_EXT ((XrStructureType) 1000763002)
#define   XR_TYPE_SPATIAL_DISCOVERY_PERSISTENCE_UUID_FILTER_EXT ((XrStructureType) 1000763003)
#define   XR_TYPE_SPATIAL_COMPONENT_PERSISTENCE_LIST_EXT ((XrStructureType) 1000763004)
#define   XR_TYPE_SPATIAL_ENTITY_PERSIST_INFO_EXT ((XrStructureType) 1000781000)
#define   XR_TYPE_PERSIST_SPATIAL_ENTITY_COMPLETION_EXT ((XrStructureType) 1000781001)
#define   XR_TYPE_SPATIAL_ENTITY_UNPERSIST_INFO_EXT ((XrStructureType) 1000781002)
#define   XR_TYPE_UNPERSIST_SPATIAL_ENTITY_COMPLETION_EXT ((XrStructureType) 1000781003)
#define   XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_OBJECT_TRACKING_ANDROID ((XrStructureType) 1000785000)
#define   XR_TYPE_SPATIAL_COMPONENT_OBJECT_SEMANTIC_LABEL_LIST_ANDROID ((XrStructureType) 1000785001)
#define   XR_TYPE_SPATIAL_CAPABILITY_CONFIGURATION_DEPTH_RAYCAST_ANDROID ((XrStructureType) 1000786000)
#define   XR_TYPE_SPATIAL_RAYCAST_INFO_ANDROID ((XrStructureType) 1000786001)
#define   XR_TYPE_SPATIAL_COMPONENT_RAYCAST_RESULT_LIST_ANDROID ((XrStructureType) 1000786002)
#define   XR_TYPE_SPATIAL_RAYCAST_SNAPSHOT_CREATE_INFO_ANDROID ((XrStructureType) 1000786003)
#define   XR_TYPE_SPATIAL_ANCHOR_PARENT_ANDROID ((XrStructureType) 1000790000)
#define   XR_TYPE_SPATIAL_DISCOVERY_UNIQUE_ENTITIES_FILTER_ANDROID ((XrStructureType) 1000791001)
#define   XR_TYPE_SPATIAL_COMPONENT_SUBSUMED_BY_LIST_ANDROID ((XrStructureType) 1000791002)
#define   XR_TYPE_EVENT_DATA_SPATIAL_DISCOVERY_RECOMMENDED_EXT ((XrStructureType) 1000740015)


    typedef enum XrSpatialCapabilityEXT {
        XR_SPATIAL_CAPABILITY_PLANE_TRACKING_EXT = 1000741000,
        XR_SPATIAL_CAPABILITY_MARKER_TRACKING_QR_CODE_EXT = 1000743000,
        XR_SPATIAL_CAPABILITY_MARKER_TRACKING_MICRO_QR_CODE_EXT = 1000743001,
        XR_SPATIAL_CAPABILITY_MARKER_TRACKING_ARUCO_MARKER_EXT = 1000743002,
        XR_SPATIAL_CAPABILITY_MARKER_TRACKING_APRIL_TAG_EXT = 1000743003,
        XR_SPATIAL_CAPABILITY_ANCHOR_EXT = 1000762000,
        XR_SPATIAL_CAPABILITY_OBJECT_TRACKING_ANDROID = 1000785000,
        XR_SPATIAL_CAPABILITY_DEPTH_RAYCAST_ANDROID = 1000786000,
        XR_SPATIAL_CAPABILITY_MAX_ENUM_EXT = 0x7FFFFFFF
    } XrSpatialCapabilityEXT;

    typedef enum XrSpatialCapabilityFeatureEXT {
        XR_SPATIAL_CAPABILITY_FEATURE_MARKER_TRACKING_FIXED_SIZE_MARKERS_EXT = 1000743000,
        XR_SPATIAL_CAPABILITY_FEATURE_MARKER_TRACKING_STATIC_MARKERS_EXT = 1000743001,
        XR_SPATIAL_CAPABILITY_FEATURE_MAX_ENUM_EXT = 0x7FFFFFFF
    } XrSpatialCapabilityFeatureEXT;

    typedef enum XrSpatialComponentTypeEXT {
        XR_SPATIAL_COMPONENT_TYPE_BOUNDED_2D_EXT = 1,
        XR_SPATIAL_COMPONENT_TYPE_BOUNDED_3D_EXT = 2,
        XR_SPATIAL_COMPONENT_TYPE_PARENT_EXT = 3,
        XR_SPATIAL_COMPONENT_TYPE_MESH_3D_EXT = 4,
        XR_SPATIAL_COMPONENT_TYPE_PLANE_ALIGNMENT_EXT = 1000741000,
        XR_SPATIAL_COMPONENT_TYPE_MESH_2D_EXT = 1000741001,
        XR_SPATIAL_COMPONENT_TYPE_POLYGON_2D_EXT = 1000741002,
        XR_SPATIAL_COMPONENT_TYPE_PLANE_SEMANTIC_LABEL_EXT = 1000741003,
        XR_SPATIAL_COMPONENT_TYPE_MARKER_EXT = 1000743000,
        XR_SPATIAL_COMPONENT_TYPE_ANCHOR_EXT = 1000762000,
        XR_SPATIAL_COMPONENT_TYPE_PERSISTENCE_EXT = 1000763000,
        XR_SPATIAL_COMPONENT_TYPE_OBJECT_SEMANTIC_LABEL_ANDROID = 1000785000,
        XR_SPATIAL_COMPONENT_TYPE_RAYCAST_RESULT_ANDROID = 1000786000,
        XR_SPATIAL_COMPONENT_TYPE_SUBSUMED_BY_ANDROID = 1000791000,
        XR_SPATIAL_COMPONENT_TYPE_MAX_ENUM_EXT = 0x7FFFFFFF
    } XrSpatialComponentTypeEXT;

    typedef enum XrSpatialEntityTrackingStateEXT {
        XR_SPATIAL_ENTITY_TRACKING_STATE_STOPPED_EXT = 1,
        XR_SPATIAL_ENTITY_TRACKING_STATE_PAUSED_EXT = 2,
        XR_SPATIAL_ENTITY_TRACKING_STATE_TRACKING_EXT = 3,
        XR_SPATIAL_ENTITY_TRACKING_STATE_MAX_ENUM_EXT = 0x7FFFFFFF
    } XrSpatialEntityTrackingStateEXT;

    typedef enum XrSpatialBufferTypeEXT {
        XR_SPATIAL_BUFFER_TYPE_UNKNOWN_EXT = 0,
        XR_SPATIAL_BUFFER_TYPE_STRING_EXT = 1,
        XR_SPATIAL_BUFFER_TYPE_UINT8_EXT = 2,
        XR_SPATIAL_BUFFER_TYPE_UINT16_EXT = 3,
        XR_SPATIAL_BUFFER_TYPE_UINT32_EXT = 4,
        XR_SPATIAL_BUFFER_TYPE_FLOAT_EXT = 5,
        XR_SPATIAL_BUFFER_TYPE_VECTOR2F_EXT = 6,
        XR_SPATIAL_BUFFER_TYPE_VECTOR3F_EXT = 7,
        XR_SPATIAL_BUFFER_TYPE_MAX_ENUM_EXT = 0x7FFFFFFF
    } XrSpatialBufferTypeEXT;
    typedef struct XrSpatialCapabilityComponentTypesEXT {
        XrStructureType               type;
        void* XR_MAY_ALIAS            next;
        uint32_t                      componentTypeCapacityInput;
        uint32_t                      componentTypeCountOutput;
        XrSpatialComponentTypeEXT* componentTypes;
    } XrSpatialCapabilityComponentTypesEXT;

    typedef struct XR_MAY_ALIAS XrSpatialCapabilityConfigurationBaseHeaderEXT {
        XrStructureType                     type;
        const void* XR_MAY_ALIAS            next;
        XrSpatialCapabilityEXT              capability;
        uint32_t                            enabledComponentCount;
        const XrSpatialComponentTypeEXT* enabledComponents;
    } XrSpatialCapabilityConfigurationBaseHeaderEXT;

    typedef struct XrSpatialContextCreateInfoEXT {
        XrStructureType                                                type;
        const void* XR_MAY_ALIAS                                       next;
        uint32_t                                                       capabilityConfigCount;
        const XrSpatialCapabilityConfigurationBaseHeaderEXT* const* capabilityConfigs;
    } XrSpatialContextCreateInfoEXT;

    typedef struct XrCreateSpatialContextCompletionEXT {
        XrStructureType        type;
        void* XR_MAY_ALIAS     next;
        XrResult               futureResult;
        XrSpatialContextEXT    spatialContext;
    } XrCreateSpatialContextCompletionEXT;

    typedef struct XrSpatialDiscoverySnapshotCreateInfoEXT {
        XrStructureType                     type;
        const void* XR_MAY_ALIAS            next;
        uint32_t                            componentTypeCount;
        const XrSpatialComponentTypeEXT* componentTypes;
    } XrSpatialDiscoverySnapshotCreateInfoEXT;

    typedef struct XrCreateSpatialDiscoverySnapshotCompletionInfoEXT {
        XrStructureType             type;
        const void* XR_MAY_ALIAS    next;
        XrSpace                     baseSpace;
        XrTime                      time;
        XrFutureEXT                 future;
    } XrCreateSpatialDiscoverySnapshotCompletionInfoEXT;

    typedef struct XrCreateSpatialDiscoverySnapshotCompletionEXT {
        XrStructureType         type;
        void* XR_MAY_ALIAS      next;
        XrResult                futureResult;
        XrSpatialSnapshotEXT    snapshot;
    } XrCreateSpatialDiscoverySnapshotCompletionEXT;

    typedef struct XrSpatialComponentDataQueryConditionEXT {
        XrStructureType                     type;
        const void* XR_MAY_ALIAS            next;
        uint32_t                            componentTypeCount;
        const XrSpatialComponentTypeEXT* componentTypes;
    } XrSpatialComponentDataQueryConditionEXT;

    typedef struct XrSpatialComponentDataQueryResultEXT {
        XrStructureType                     type;
        void* XR_MAY_ALIAS                  next;
        uint32_t                            entityIdCapacityInput;
        uint32_t                            entityIdCountOutput;
        XrSpatialEntityIdEXT* entityIds;
        uint32_t                            entityStateCapacityInput;
        uint32_t                            entityStateCountOutput;
        XrSpatialEntityTrackingStateEXT* entityStates;
    } XrSpatialComponentDataQueryResultEXT;

    typedef struct XrSpatialBufferEXT {
        XrSpatialBufferIdEXT      bufferId;
        XrSpatialBufferTypeEXT    bufferType;
    } XrSpatialBufferEXT;

    typedef struct XrSpatialBufferGetInfoEXT {
        XrStructureType             type;
        const void* XR_MAY_ALIAS    next;
        XrSpatialBufferIdEXT        bufferId;
    } XrSpatialBufferGetInfoEXT;

    typedef struct XrSpatialBounded2DDataEXT {
        XrPosef        center;
        XrExtent2Df    extents;
    } XrSpatialBounded2DDataEXT;

    // XrSpatialComponentBounded2DListEXT extends XrSpatialComponentDataQueryResultEXT
    typedef struct XrSpatialComponentBounded2DListEXT {
        XrStructureType               type;
        void* XR_MAY_ALIAS            next;
        uint32_t                      boundCount;
        XrSpatialBounded2DDataEXT* bounds;
    } XrSpatialComponentBounded2DListEXT;

    // XrSpatialComponentBounded3DListEXT extends XrSpatialComponentDataQueryResultEXT
    typedef struct XrSpatialComponentBounded3DListEXT {
        XrStructureType       type;
        void* XR_MAY_ALIAS    next;
        uint32_t              boundCount;
        XrBoxf* bounds;
    } XrSpatialComponentBounded3DListEXT;

    // XrSpatialComponentParentListEXT extends XrSpatialComponentDataQueryResultEXT
    typedef struct XrSpatialComponentParentListEXT {
        XrStructureType          type;
        void* XR_MAY_ALIAS       next;
        uint32_t                 parentCount;
        XrSpatialEntityIdEXT* parents;
    } XrSpatialComponentParentListEXT;

    typedef struct XrSpatialMeshDataEXT {
        XrPosef               origin;
        XrSpatialBufferEXT    vertexBuffer;
        XrSpatialBufferEXT    indexBuffer;
    } XrSpatialMeshDataEXT;

    // XrSpatialComponentMesh3DListEXT extends XrSpatialComponentDataQueryResultEXT
    typedef struct XrSpatialComponentMesh3DListEXT {
        XrStructureType          type;
        void* XR_MAY_ALIAS       next;
        uint32_t                 meshCount;
        XrSpatialMeshDataEXT* meshes;
    } XrSpatialComponentMesh3DListEXT;

    typedef struct XrSpatialEntityFromIdCreateInfoEXT {
        XrStructureType             type;
        const void* XR_MAY_ALIAS    next;
        XrSpatialEntityIdEXT        entityId;
    } XrSpatialEntityFromIdCreateInfoEXT;

    typedef struct XrSpatialUpdateSnapshotCreateInfoEXT {
        XrStructureType                     type;
        const void* XR_MAY_ALIAS            next;
        uint32_t                            entityCount;
        const XrSpatialEntityEXT* entities;
        uint32_t                            componentTypeCount;
        const XrSpatialComponentTypeEXT* componentTypes;
        XrSpace                             baseSpace;
        XrTime                              time;
    } XrSpatialUpdateSnapshotCreateInfoEXT;

    typedef struct XrEventDataSpatialDiscoveryRecommendedEXT {
        XrStructureType             type;
        const void* XR_MAY_ALIAS    next;
        XrSpatialContextEXT         spatialContext;
    } XrEventDataSpatialDiscoveryRecommendedEXT;

    // XrSpatialFilterTrackingStateEXT extends XrSpatialDiscoverySnapshotCreateInfoEXT,XrSpatialComponentDataQueryConditionEXT
    typedef struct XrSpatialFilterTrackingStateEXT {
        XrStructureType                    type;
        const void* XR_MAY_ALIAS           next;
        XrSpatialEntityTrackingStateEXT    trackingState;
    } XrSpatialFilterTrackingStateEXT;

    typedef XrResult(XRAPI_PTR* PFN_xrEnumerateSpatialCapabilitiesEXT)(XrInstance instance, XrSystemId systemId, uint32_t capabilityCapacityInput, uint32_t* capabilityCountOutput, XrSpatialCapabilityEXT* capabilities);
    typedef XrResult(XRAPI_PTR* PFN_xrEnumerateSpatialCapabilityComponentTypesEXT)(XrInstance instance, XrSystemId systemId, XrSpatialCapabilityEXT capability, XrSpatialCapabilityComponentTypesEXT* capabilityComponents);
    typedef XrResult(XRAPI_PTR* PFN_xrEnumerateSpatialCapabilityFeaturesEXT)(XrInstance instance, XrSystemId systemId, XrSpatialCapabilityEXT capability, uint32_t capabilityFeatureCapacityInput, uint32_t* capabilityFeatureCountOutput, XrSpatialCapabilityFeatureEXT* capabilityFeatures);
    typedef XrResult(XRAPI_PTR* PFN_xrCreateSpatialContextAsyncEXT)(XrSession session, const XrSpatialContextCreateInfoEXT* createInfo, XrFutureEXT* future);
    typedef XrResult(XRAPI_PTR* PFN_xrCreateSpatialContextCompleteEXT)(XrSession session, XrFutureEXT future, XrCreateSpatialContextCompletionEXT* completion);
    typedef XrResult(XRAPI_PTR* PFN_xrDestroySpatialContextEXT)(XrSpatialContextEXT spatialContext);
    typedef XrResult(XRAPI_PTR* PFN_xrCreateSpatialDiscoverySnapshotAsyncEXT)(XrSpatialContextEXT spatialContext, const XrSpatialDiscoverySnapshotCreateInfoEXT* createInfo, XrFutureEXT* future);
    typedef XrResult(XRAPI_PTR* PFN_xrCreateSpatialDiscoverySnapshotCompleteEXT)(XrSpatialContextEXT spatialContext, const XrCreateSpatialDiscoverySnapshotCompletionInfoEXT* createSnapshotCompletionInfo, XrCreateSpatialDiscoverySnapshotCompletionEXT* completion);
    typedef XrResult(XRAPI_PTR* PFN_xrQuerySpatialComponentDataEXT)(XrSpatialSnapshotEXT snapshot, const XrSpatialComponentDataQueryConditionEXT* queryCondition, XrSpatialComponentDataQueryResultEXT* queryResult);
    typedef XrResult(XRAPI_PTR* PFN_xrDestroySpatialSnapshotEXT)(XrSpatialSnapshotEXT snapshot);
    typedef XrResult(XRAPI_PTR* PFN_xrCreateSpatialEntityFromIdEXT)(XrSpatialContextEXT spatialContext, const XrSpatialEntityFromIdCreateInfoEXT* createInfo, XrSpatialEntityEXT* spatialEntity);
    typedef XrResult(XRAPI_PTR* PFN_xrDestroySpatialEntityEXT)(XrSpatialEntityEXT spatialEntity);
    typedef XrResult(XRAPI_PTR* PFN_xrCreateSpatialUpdateSnapshotEXT)(XrSpatialContextEXT spatialContext, const XrSpatialUpdateSnapshotCreateInfoEXT* createInfo, XrSpatialSnapshotEXT* snapshot);
    typedef XrResult(XRAPI_PTR* PFN_xrGetSpatialBufferStringEXT)(XrSpatialSnapshotEXT snapshot, const XrSpatialBufferGetInfoEXT* info, uint32_t bufferCapacityInput, uint32_t* bufferCountOutput, char* buffer);
    typedef XrResult(XRAPI_PTR* PFN_xrGetSpatialBufferUint8EXT)(XrSpatialSnapshotEXT snapshot, const XrSpatialBufferGetInfoEXT* info, uint32_t bufferCapacityInput, uint32_t* bufferCountOutput, uint8_t* buffer);
    typedef XrResult(XRAPI_PTR* PFN_xrGetSpatialBufferUint16EXT)(XrSpatialSnapshotEXT snapshot, const XrSpatialBufferGetInfoEXT* info, uint32_t bufferCapacityInput, uint32_t* bufferCountOutput, uint16_t* buffer);
    typedef XrResult(XRAPI_PTR* PFN_xrGetSpatialBufferUint32EXT)(XrSpatialSnapshotEXT snapshot, const XrSpatialBufferGetInfoEXT* info, uint32_t bufferCapacityInput, uint32_t* bufferCountOutput, uint32_t* buffer);
    typedef XrResult(XRAPI_PTR* PFN_xrGetSpatialBufferFloatEXT)(XrSpatialSnapshotEXT snapshot, const XrSpatialBufferGetInfoEXT* info, uint32_t bufferCapacityInput, uint32_t* bufferCountOutput, float* buffer);
    typedef XrResult(XRAPI_PTR* PFN_xrGetSpatialBufferVector2fEXT)(XrSpatialSnapshotEXT snapshot, const XrSpatialBufferGetInfoEXT* info, uint32_t bufferCapacityInput, uint32_t* bufferCountOutput, XrVector2f* buffer);
    typedef XrResult(XRAPI_PTR* PFN_xrGetSpatialBufferVector3fEXT)(XrSpatialSnapshotEXT snapshot, const XrSpatialBufferGetInfoEXT* info, uint32_t bufferCapacityInput, uint32_t* bufferCountOutput, XrVector3f* buffer);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
    XRAPI_ATTR XrResult XRAPI_CALL xrEnumerateSpatialCapabilitiesEXT(
        XrInstance                                  instance,
        XrSystemId                                  systemId,
        uint32_t                                    capabilityCapacityInput,
        uint32_t* capabilityCountOutput,
        XrSpatialCapabilityEXT* capabilities);

    XRAPI_ATTR XrResult XRAPI_CALL xrEnumerateSpatialCapabilityComponentTypesEXT(
        XrInstance                                  instance,
        XrSystemId                                  systemId,
        XrSpatialCapabilityEXT                      capability,
        XrSpatialCapabilityComponentTypesEXT* capabilityComponents);

    XRAPI_ATTR XrResult XRAPI_CALL xrEnumerateSpatialCapabilityFeaturesEXT(
        XrInstance                                  instance,
        XrSystemId                                  systemId,
        XrSpatialCapabilityEXT                      capability,
        uint32_t                                    capabilityFeatureCapacityInput,
        uint32_t* capabilityFeatureCountOutput,
        XrSpatialCapabilityFeatureEXT* capabilityFeatures);

    XRAPI_ATTR XrResult XRAPI_CALL xrCreateSpatialContextAsyncEXT(
        XrSession                                   session,
        const XrSpatialContextCreateInfoEXT* createInfo,
        XrFutureEXT* future);

    XRAPI_ATTR XrResult XRAPI_CALL xrCreateSpatialContextCompleteEXT(
        XrSession                                   session,
        XrFutureEXT                                 future,
        XrCreateSpatialContextCompletionEXT* completion);

    XRAPI_ATTR XrResult XRAPI_CALL xrDestroySpatialContextEXT(
        XrSpatialContextEXT                         spatialContext);

    XRAPI_ATTR XrResult XRAPI_CALL xrCreateSpatialDiscoverySnapshotAsyncEXT(
        XrSpatialContextEXT                         spatialContext,
        const XrSpatialDiscoverySnapshotCreateInfoEXT* createInfo,
        XrFutureEXT* future);

    XRAPI_ATTR XrResult XRAPI_CALL xrCreateSpatialDiscoverySnapshotCompleteEXT(
        XrSpatialContextEXT                         spatialContext,
        const XrCreateSpatialDiscoverySnapshotCompletionInfoEXT* createSnapshotCompletionInfo,
        XrCreateSpatialDiscoverySnapshotCompletionEXT* completion);

    XRAPI_ATTR XrResult XRAPI_CALL xrQuerySpatialComponentDataEXT(
        XrSpatialSnapshotEXT                        snapshot,
        const XrSpatialComponentDataQueryConditionEXT* queryCondition,
        XrSpatialComponentDataQueryResultEXT* queryResult);

    XRAPI_ATTR XrResult XRAPI_CALL xrDestroySpatialSnapshotEXT(
        XrSpatialSnapshotEXT                        snapshot);

    XRAPI_ATTR XrResult XRAPI_CALL xrCreateSpatialEntityFromIdEXT(
        XrSpatialContextEXT                         spatialContext,
        const XrSpatialEntityFromIdCreateInfoEXT* createInfo,
        XrSpatialEntityEXT* spatialEntity);

    XRAPI_ATTR XrResult XRAPI_CALL xrDestroySpatialEntityEXT(
        XrSpatialEntityEXT                          spatialEntity);

    XRAPI_ATTR XrResult XRAPI_CALL xrCreateSpatialUpdateSnapshotEXT(
        XrSpatialContextEXT                         spatialContext,
        const XrSpatialUpdateSnapshotCreateInfoEXT* createInfo,
        XrSpatialSnapshotEXT* snapshot);

    XRAPI_ATTR XrResult XRAPI_CALL xrGetSpatialBufferStringEXT(
        XrSpatialSnapshotEXT                        snapshot,
        const XrSpatialBufferGetInfoEXT* info,
        uint32_t                                    bufferCapacityInput,
        uint32_t* bufferCountOutput,
        char* buffer);

    XRAPI_ATTR XrResult XRAPI_CALL xrGetSpatialBufferUint8EXT(
        XrSpatialSnapshotEXT                        snapshot,
        const XrSpatialBufferGetInfoEXT* info,
        uint32_t                                    bufferCapacityInput,
        uint32_t* bufferCountOutput,
        uint8_t* buffer);

    XRAPI_ATTR XrResult XRAPI_CALL xrGetSpatialBufferUint16EXT(
        XrSpatialSnapshotEXT                        snapshot,
        const XrSpatialBufferGetInfoEXT* info,
        uint32_t                                    bufferCapacityInput,
        uint32_t* bufferCountOutput,
        uint16_t* buffer);

    XRAPI_ATTR XrResult XRAPI_CALL xrGetSpatialBufferUint32EXT(
        XrSpatialSnapshotEXT                        snapshot,
        const XrSpatialBufferGetInfoEXT* info,
        uint32_t                                    bufferCapacityInput,
        uint32_t* bufferCountOutput,
        uint32_t* buffer);

    XRAPI_ATTR XrResult XRAPI_CALL xrGetSpatialBufferFloatEXT(
        XrSpatialSnapshotEXT                        snapshot,
        const XrSpatialBufferGetInfoEXT* info,
        uint32_t                                    bufferCapacityInput,
        uint32_t* bufferCountOutput,
        float* buffer);

    XRAPI_ATTR XrResult XRAPI_CALL xrGetSpatialBufferVector2fEXT(
        XrSpatialSnapshotEXT                        snapshot,
        const XrSpatialBufferGetInfoEXT* info,
        uint32_t                                    bufferCapacityInput,
        uint32_t* bufferCountOutput,
        XrVector2f* buffer);

    XRAPI_ATTR XrResult XRAPI_CALL xrGetSpatialBufferVector3fEXT(
        XrSpatialSnapshotEXT                        snapshot,
        const XrSpatialBufferGetInfoEXT* info,
        uint32_t                                    bufferCapacityInput,
        uint32_t* bufferCountOutput,
        XrVector3f* buffer);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */
#endif

    // XR_EXT_spatial_plane_tracking is a preprocessor guard. Do not pass it to API calls.
#define XR_EXT_spatial_plane_tracking 1
#define XR_EXT_spatial_plane_tracking_SPEC_VERSION 1
#define XR_EXT_SPATIAL_PLANE_TRACKING_EXTENSION_NAME "XR_EXT_spatial_plane_tracking"

    typedef enum XrSpatialPlaneAlignmentEXT {
        XR_SPATIAL_PLANE_ALIGNMENT_HORIZONTAL_UPWARD_EXT = 0,
        XR_SPATIAL_PLANE_ALIGNMENT_HORIZONTAL_DOWNWARD_EXT = 1,
        XR_SPATIAL_PLANE_ALIGNMENT_VERTICAL_EXT = 2,
        XR_SPATIAL_PLANE_ALIGNMENT_ARBITRARY_EXT = 3,
        XR_SPATIAL_PLANE_ALIGNMENT_MAX_ENUM_EXT = 0x7FFFFFFF
    } XrSpatialPlaneAlignmentEXT;

    typedef enum XrSpatialPlaneSemanticLabelEXT {
        XR_SPATIAL_PLANE_SEMANTIC_LABEL_UNCATEGORIZED_EXT = 1,
        XR_SPATIAL_PLANE_SEMANTIC_LABEL_FLOOR_EXT = 2,
        XR_SPATIAL_PLANE_SEMANTIC_LABEL_WALL_EXT = 3,
        XR_SPATIAL_PLANE_SEMANTIC_LABEL_CEILING_EXT = 4,
        XR_SPATIAL_PLANE_SEMANTIC_LABEL_TABLE_EXT = 5,
        XR_SPATIAL_PLANE_SEMANTIC_LABEL_MAX_ENUM_EXT = 0x7FFFFFFF
    } XrSpatialPlaneSemanticLabelEXT;
    typedef struct XrSpatialCapabilityConfigurationPlaneTrackingEXT {
        XrStructureType                     type;
        const void* XR_MAY_ALIAS            next;
        XrSpatialCapabilityEXT              capability;
        uint32_t                            enabledComponentCount;
        const XrSpatialComponentTypeEXT* enabledComponents;
    } XrSpatialCapabilityConfigurationPlaneTrackingEXT;

    // XrSpatialComponentPlaneAlignmentListEXT extends XrSpatialComponentDataQueryResultEXT
    typedef struct XrSpatialComponentPlaneAlignmentListEXT {
        XrStructureType                type;
        void* XR_MAY_ALIAS             next;
        uint32_t                       planeAlignmentCount;
        XrSpatialPlaneAlignmentEXT* planeAlignments;
    } XrSpatialComponentPlaneAlignmentListEXT;

    // XrSpatialComponentMesh2DListEXT extends XrSpatialComponentDataQueryResultEXT
    typedef struct XrSpatialComponentMesh2DListEXT {
        XrStructureType          type;
        void* XR_MAY_ALIAS       next;
        uint32_t                 meshCount;
        XrSpatialMeshDataEXT* meshes;
    } XrSpatialComponentMesh2DListEXT;

    typedef struct XrSpatialPolygon2DDataEXT {
        XrPosef               origin;
        XrSpatialBufferEXT    vertexBuffer;
    } XrSpatialPolygon2DDataEXT;

    // XrSpatialComponentPolygon2DListEXT extends XrSpatialComponentDataQueryResultEXT
    typedef struct XrSpatialComponentPolygon2DListEXT {
        XrStructureType               type;
        void* XR_MAY_ALIAS            next;
        uint32_t                      polygonCount;
        XrSpatialPolygon2DDataEXT* polygons;
    } XrSpatialComponentPolygon2DListEXT;

    // XrSpatialComponentPlaneSemanticLabelListEXT extends XrSpatialComponentDataQueryResultEXT
    typedef struct XrSpatialComponentPlaneSemanticLabelListEXT {
        XrStructureType                    type;
        void* XR_MAY_ALIAS                 next;
        uint32_t                           semanticLabelCount;
        XrSpatialPlaneSemanticLabelEXT* semanticLabels;
    } XrSpatialComponentPlaneSemanticLabelListEXT;



    // XR_EXT_spatial_marker_tracking is a preprocessor guard. Do not pass it to API calls.
#define XR_EXT_spatial_marker_tracking 1
#define XR_EXT_spatial_marker_tracking_SPEC_VERSION 1
#define XR_EXT_SPATIAL_MARKER_TRACKING_EXTENSION_NAME "XR_EXT_spatial_marker_tracking"

    typedef enum XrSpatialMarkerArucoDictEXT {
        XR_SPATIAL_MARKER_ARUCO_DICT_4X4_50_EXT = 1,
        XR_SPATIAL_MARKER_ARUCO_DICT_4X4_100_EXT = 2,
        XR_SPATIAL_MARKER_ARUCO_DICT_4X4_250_EXT = 3,
        XR_SPATIAL_MARKER_ARUCO_DICT_4X4_1000_EXT = 4,
        XR_SPATIAL_MARKER_ARUCO_DICT_5X5_50_EXT = 5,
        XR_SPATIAL_MARKER_ARUCO_DICT_5X5_100_EXT = 6,
        XR_SPATIAL_MARKER_ARUCO_DICT_5X5_250_EXT = 7,
        XR_SPATIAL_MARKER_ARUCO_DICT_5X5_1000_EXT = 8,
        XR_SPATIAL_MARKER_ARUCO_DICT_6X6_50_EXT = 9,
        XR_SPATIAL_MARKER_ARUCO_DICT_6X6_100_EXT = 10,
        XR_SPATIAL_MARKER_ARUCO_DICT_6X6_250_EXT = 11,
        XR_SPATIAL_MARKER_ARUCO_DICT_6X6_1000_EXT = 12,
        XR_SPATIAL_MARKER_ARUCO_DICT_7X7_50_EXT = 13,
        XR_SPATIAL_MARKER_ARUCO_DICT_7X7_100_EXT = 14,
        XR_SPATIAL_MARKER_ARUCO_DICT_7X7_250_EXT = 15,
        XR_SPATIAL_MARKER_ARUCO_DICT_7X7_1000_EXT = 16,
        XR_SPATIAL_MARKER_ARUCO_DICT_MAX_ENUM_EXT = 0x7FFFFFFF
    } XrSpatialMarkerArucoDictEXT;

    typedef enum XrSpatialMarkerAprilTagDictEXT {
        XR_SPATIAL_MARKER_APRIL_TAG_DICT_16H5_EXT = 1,
        XR_SPATIAL_MARKER_APRIL_TAG_DICT_25H9_EXT = 2,
        XR_SPATIAL_MARKER_APRIL_TAG_DICT_36H10_EXT = 3,
        XR_SPATIAL_MARKER_APRIL_TAG_DICT_36H11_EXT = 4,
        XR_SPATIAL_MARKER_APRIL_TAG_DICT_MAX_ENUM_EXT = 0x7FFFFFFF
    } XrSpatialMarkerAprilTagDictEXT;
    typedef struct XrSpatialCapabilityConfigurationQrCodeEXT {
        XrStructureType                     type;
        const void* XR_MAY_ALIAS            next;
        XrSpatialCapabilityEXT              capability;
        uint32_t                            enabledComponentCount;
        const XrSpatialComponentTypeEXT* enabledComponents;
    } XrSpatialCapabilityConfigurationQrCodeEXT;

    typedef struct XrSpatialCapabilityConfigurationMicroQrCodeEXT {
        XrStructureType                     type;
        const void* XR_MAY_ALIAS            next;
        XrSpatialCapabilityEXT              capability;
        uint32_t                            enabledComponentCount;
        const XrSpatialComponentTypeEXT* enabledComponents;
    } XrSpatialCapabilityConfigurationMicroQrCodeEXT;

    typedef struct XrSpatialCapabilityConfigurationArucoMarkerEXT {
        XrStructureType                     type;
        const void* XR_MAY_ALIAS            next;
        XrSpatialCapabilityEXT              capability;
        uint32_t                            enabledComponentCount;
        const XrSpatialComponentTypeEXT* enabledComponents;
        XrSpatialMarkerArucoDictEXT         arUcoDict;
    } XrSpatialCapabilityConfigurationArucoMarkerEXT;

    typedef struct XrSpatialCapabilityConfigurationAprilTagEXT {
        XrStructureType                     type;
        const void* XR_MAY_ALIAS            next;
        XrSpatialCapabilityEXT              capability;
        uint32_t                            enabledComponentCount;
        const XrSpatialComponentTypeEXT* enabledComponents;
        XrSpatialMarkerAprilTagDictEXT      aprilDict;
    } XrSpatialCapabilityConfigurationAprilTagEXT;

    // XrSpatialMarkerSizeEXT extends XrSpatialCapabilityConfigurationArucoMarkerEXT,XrSpatialCapabilityConfigurationAprilTagEXT,XrSpatialCapabilityConfigurationQrCodeEXT,XrSpatialCapabilityConfigurationMicroQrCodeEXT
    typedef struct XrSpatialMarkerSizeEXT {
        XrStructureType             type;
        const void* XR_MAY_ALIAS    next;
        float                       markerSideLength;
    } XrSpatialMarkerSizeEXT;

    // XrSpatialMarkerStaticOptimizationEXT extends XrSpatialCapabilityConfigurationArucoMarkerEXT,XrSpatialCapabilityConfigurationAprilTagEXT,XrSpatialCapabilityConfigurationQrCodeEXT,XrSpatialCapabilityConfigurationMicroQrCodeEXT
    typedef struct XrSpatialMarkerStaticOptimizationEXT {
        XrStructureType             type;
        const void* XR_MAY_ALIAS    next;
        XrBool32                    optimizeForStaticMarker;
    } XrSpatialMarkerStaticOptimizationEXT;

    typedef struct XrSpatialMarkerDataEXT {
        XrSpatialCapabilityEXT    capability;
        uint32_t                  markerId;
        XrSpatialBufferEXT        data;
    } XrSpatialMarkerDataEXT;

    // XrSpatialComponentMarkerListEXT extends XrSpatialComponentDataQueryResultEXT
    typedef struct XrSpatialComponentMarkerListEXT {
        XrStructureType            type;
        void* XR_MAY_ALIAS         next;
        uint32_t                   markerCount;
        XrSpatialMarkerDataEXT* markers;
    } XrSpatialComponentMarkerListEXT;



    // XR_LOGITECH_mx_ink_stylus_interaction is a preprocessor guard. Do not pass it to API calls.
#define XR_LOGITECH_mx_ink_stylus_interaction 1
#define XR_LOGITECH_mx_ink_stylus_interaction_SPEC_VERSION 1
#define XR_LOGITECH_MX_INK_STYLUS_INTERACTION_EXTENSION_NAME "XR_LOGITECH_mx_ink_stylus_interaction"


// XR_EXT_spatial_anchor is a preprocessor guard. Do not pass it to API calls.
#define XR_EXT_spatial_anchor 1
#define XR_EXT_spatial_anchor_SPEC_VERSION 1
#define XR_EXT_SPATIAL_ANCHOR_EXTENSION_NAME "XR_EXT_spatial_anchor"
    typedef struct XrSpatialCapabilityConfigurationAnchorEXT {
        XrStructureType                     type;
        const void* XR_MAY_ALIAS            next;
        XrSpatialCapabilityEXT              capability;
        uint32_t                            enabledComponentCount;
        const XrSpatialComponentTypeEXT* enabledComponents;
    } XrSpatialCapabilityConfigurationAnchorEXT;

    // XrSpatialComponentAnchorListEXT extends XrSpatialComponentDataQueryResultEXT
    typedef struct XrSpatialComponentAnchorListEXT {
        XrStructureType       type;
        void* XR_MAY_ALIAS    next;
        uint32_t              locationCount;
        XrPosef* locations;
    } XrSpatialComponentAnchorListEXT;

    typedef struct XrSpatialAnchorCreateInfoEXT {
        XrStructureType             type;
        const void* XR_MAY_ALIAS    next;
        XrSpace                     baseSpace;
        XrTime                      time;
        XrPosef                     pose;
    } XrSpatialAnchorCreateInfoEXT;

    typedef XrResult(XRAPI_PTR* PFN_xrCreateSpatialAnchorEXT)(XrSpatialContextEXT spatialContext, const XrSpatialAnchorCreateInfoEXT* createInfo, XrSpatialEntityIdEXT* anchorEntityId, XrSpatialEntityEXT* anchorEntity);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
    XRAPI_ATTR XrResult XRAPI_CALL xrCreateSpatialAnchorEXT(
        XrSpatialContextEXT                         spatialContext,
        const XrSpatialAnchorCreateInfoEXT* createInfo,
        XrSpatialEntityIdEXT* anchorEntityId,
        XrSpatialEntityEXT* anchorEntity);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */


    // XR_EXT_spatial_persistence is a preprocessor guard. Do not pass it to API calls.
#define XR_EXT_spatial_persistence 1
    XR_DEFINE_HANDLE(XrSpatialPersistenceContextEXT)
#define XR_EXT_spatial_persistence_SPEC_VERSION 1
#define XR_EXT_SPATIAL_PERSISTENCE_EXTENSION_NAME "XR_EXT_spatial_persistence"

        typedef enum XrSpatialPersistenceScopeEXT {
        XR_SPATIAL_PERSISTENCE_SCOPE_SYSTEM_MANAGED_EXT = 1,
        XR_SPATIAL_PERSISTENCE_SCOPE_LOCAL_ANCHORS_EXT = 1000781000,
        XR_SPATIAL_PERSISTENCE_SCOPE_MAX_ENUM_EXT = 0x7FFFFFFF
    } XrSpatialPersistenceScopeEXT;

    typedef enum XrSpatialPersistenceContextResultEXT {
        XR_SPATIAL_PERSISTENCE_CONTEXT_RESULT_SUCCESS_EXT = 0,
        XR_SPATIAL_PERSISTENCE_CONTEXT_RESULT_ENTITY_NOT_TRACKING_EXT = -1000781001,
        XR_SPATIAL_PERSISTENCE_CONTEXT_RESULT_PERSIST_UUID_NOT_FOUND_EXT = -1000781002,
        XR_SPATIAL_PERSISTENCE_CONTEXT_RESULT_MAX_ENUM_EXT = 0x7FFFFFFF
    } XrSpatialPersistenceContextResultEXT;

    typedef enum XrSpatialPersistenceStateEXT {
        XR_SPATIAL_PERSISTENCE_STATE_LOADED_EXT = 1,
        XR_SPATIAL_PERSISTENCE_STATE_NOT_FOUND_EXT = 2,
        XR_SPATIAL_PERSISTENCE_STATE_MAX_ENUM_EXT = 0x7FFFFFFF
    } XrSpatialPersistenceStateEXT;
    typedef struct XrSpatialPersistenceContextCreateInfoEXT {
        XrStructureType                 type;
        const void* XR_MAY_ALIAS        next;
        XrSpatialPersistenceScopeEXT    scope;
    } XrSpatialPersistenceContextCreateInfoEXT;

    typedef struct XrCreateSpatialPersistenceContextCompletionEXT {
        XrStructureType                         type;
        void* XR_MAY_ALIAS                      next;
        XrResult                                futureResult;
        XrSpatialPersistenceContextResultEXT    createResult;
        XrSpatialPersistenceContextEXT          persistenceContext;
    } XrCreateSpatialPersistenceContextCompletionEXT;

    // XrSpatialContextPersistenceConfigEXT extends XrSpatialContextCreateInfoEXT
    typedef struct XrSpatialContextPersistenceConfigEXT {
        XrStructureType                          type;
        const void* XR_MAY_ALIAS                 next;
        uint32_t                                 persistenceContextCount;
        const XrSpatialPersistenceContextEXT* persistenceContexts;
    } XrSpatialContextPersistenceConfigEXT;

    // XrSpatialDiscoveryPersistenceUuidFilterEXT extends XrSpatialDiscoverySnapshotCreateInfoEXT,XrSpatialComponentDataQueryConditionEXT
    typedef struct XrSpatialDiscoveryPersistenceUuidFilterEXT {
        XrStructureType             type;
        const void* XR_MAY_ALIAS    next;
        uint32_t                    persistedUuidCount;
        const XrUuid* persistedUuids;
    } XrSpatialDiscoveryPersistenceUuidFilterEXT;

    typedef struct XrSpatialPersistenceDataEXT {
        XrUuid                          persistUuid;
        XrSpatialPersistenceStateEXT    persistState;
    } XrSpatialPersistenceDataEXT;

    // XrSpatialComponentPersistenceListEXT extends XrSpatialComponentDataQueryResultEXT
    typedef struct XrSpatialComponentPersistenceListEXT {
        XrStructureType                 type;
        void* XR_MAY_ALIAS              next;
        uint32_t                        persistDataCount;
        XrSpatialPersistenceDataEXT* persistData;
    } XrSpatialComponentPersistenceListEXT;

    typedef XrResult(XRAPI_PTR* PFN_xrEnumerateSpatialPersistenceScopesEXT)(XrInstance instance, XrSystemId systemId, uint32_t persistenceScopeCapacityInput, uint32_t* persistenceScopeCountOutput, XrSpatialPersistenceScopeEXT* persistenceScopes);
    typedef XrResult(XRAPI_PTR* PFN_xrCreateSpatialPersistenceContextAsyncEXT)(XrSession session, const XrSpatialPersistenceContextCreateInfoEXT* createInfo, XrFutureEXT* future);
    typedef XrResult(XRAPI_PTR* PFN_xrCreateSpatialPersistenceContextCompleteEXT)(XrSession session, XrFutureEXT future, XrCreateSpatialPersistenceContextCompletionEXT* completion);
    typedef XrResult(XRAPI_PTR* PFN_xrDestroySpatialPersistenceContextEXT)(XrSpatialPersistenceContextEXT persistenceContext);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
    XRAPI_ATTR XrResult XRAPI_CALL xrEnumerateSpatialPersistenceScopesEXT(
        XrInstance                                  instance,
        XrSystemId                                  systemId,
        uint32_t                                    persistenceScopeCapacityInput,
        uint32_t* persistenceScopeCountOutput,
        XrSpatialPersistenceScopeEXT* persistenceScopes);

    XRAPI_ATTR XrResult XRAPI_CALL xrCreateSpatialPersistenceContextAsyncEXT(
        XrSession                                   session,
        const XrSpatialPersistenceContextCreateInfoEXT* createInfo,
        XrFutureEXT* future);

    XRAPI_ATTR XrResult XRAPI_CALL xrCreateSpatialPersistenceContextCompleteEXT(
        XrSession                                   session,
        XrFutureEXT                                 future,
        XrCreateSpatialPersistenceContextCompletionEXT* completion);

    XRAPI_ATTR XrResult XRAPI_CALL xrDestroySpatialPersistenceContextEXT(
        XrSpatialPersistenceContextEXT              persistenceContext);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */


    // XR_EXT_spatial_persistence_operations is a preprocessor guard. Do not pass it to API calls.
#define XR_EXT_spatial_persistence_operations 1
#define XR_EXT_spatial_persistence_operations_SPEC_VERSION 1
#define XR_EXT_SPATIAL_PERSISTENCE_OPERATIONS_EXTENSION_NAME "XR_EXT_spatial_persistence_operations"
    typedef struct XrSpatialEntityPersistInfoEXT {
        XrStructureType             type;
        const void* XR_MAY_ALIAS    next;
        XrSpatialContextEXT         spatialContext;
        XrSpatialEntityIdEXT        spatialEntityId;
    } XrSpatialEntityPersistInfoEXT;

    typedef struct XrPersistSpatialEntityCompletionEXT {
        XrStructureType                         type;
        void* XR_MAY_ALIAS                      next;
        XrResult                                futureResult;
        XrSpatialPersistenceContextResultEXT    persistResult;
        XrUuid                                  persistUuid;
    } XrPersistSpatialEntityCompletionEXT;

    typedef struct XrSpatialEntityUnpersistInfoEXT {
        XrStructureType             type;
        const void* XR_MAY_ALIAS    next;
        XrUuid                      persistUuid;
    } XrSpatialEntityUnpersistInfoEXT;

    typedef struct XrUnpersistSpatialEntityCompletionEXT {
        XrStructureType                         type;
        void* XR_MAY_ALIAS                      next;
        XrResult                                futureResult;
        XrSpatialPersistenceContextResultEXT    unpersistResult;
    } XrUnpersistSpatialEntityCompletionEXT;

    typedef XrResult(XRAPI_PTR* PFN_xrPersistSpatialEntityAsyncEXT)(XrSpatialPersistenceContextEXT persistenceContext, const XrSpatialEntityPersistInfoEXT* persistInfo, XrFutureEXT* future);
    typedef XrResult(XRAPI_PTR* PFN_xrPersistSpatialEntityCompleteEXT)(XrSpatialPersistenceContextEXT persistenceContext, XrFutureEXT future, XrPersistSpatialEntityCompletionEXT* completion);
    typedef XrResult(XRAPI_PTR* PFN_xrUnpersistSpatialEntityAsyncEXT)(XrSpatialPersistenceContextEXT persistenceContext, const XrSpatialEntityUnpersistInfoEXT* unpersistInfo, XrFutureEXT* future);
    typedef XrResult(XRAPI_PTR* PFN_xrUnpersistSpatialEntityCompleteEXT)(XrSpatialPersistenceContextEXT persistenceContext, XrFutureEXT future, XrUnpersistSpatialEntityCompletionEXT* completion);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
    XRAPI_ATTR XrResult XRAPI_CALL xrPersistSpatialEntityAsyncEXT(
        XrSpatialPersistenceContextEXT              persistenceContext,
        const XrSpatialEntityPersistInfoEXT* persistInfo,
        XrFutureEXT* future);

    XRAPI_ATTR XrResult XRAPI_CALL xrPersistSpatialEntityCompleteEXT(
        XrSpatialPersistenceContextEXT              persistenceContext,
        XrFutureEXT                                 future,
        XrPersistSpatialEntityCompletionEXT* completion);

    XRAPI_ATTR XrResult XRAPI_CALL xrUnpersistSpatialEntityAsyncEXT(
        XrSpatialPersistenceContextEXT              persistenceContext,
        const XrSpatialEntityUnpersistInfoEXT* unpersistInfo,
        XrFutureEXT* future);

    XRAPI_ATTR XrResult XRAPI_CALL xrUnpersistSpatialEntityCompleteEXT(
        XrSpatialPersistenceContextEXT              persistenceContext,
        XrFutureEXT                                 future,
        XrUnpersistSpatialEntityCompletionEXT* completion);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */


    // XR_ANDROID_spatial_object_tracking is a preprocessor guard. Do not pass it to API calls.
#define XR_ANDROID_spatial_object_tracking 1
#define XR_ANDROID_spatial_object_tracking_SPEC_VERSION 1
#define XR_ANDROID_SPATIAL_OBJECT_TRACKING_EXTENSION_NAME "XR_ANDROID_spatial_object_tracking"

    typedef enum XrSpatialObjectSemanticLabelANDROID {
        XR_SPATIAL_OBJECT_SEMANTIC_LABEL_UNCATEGORIZED_ANDROID = 0,
        XR_SPATIAL_OBJECT_SEMANTIC_LABEL_KEYBOARD_ANDROID = 1,
        XR_SPATIAL_OBJECT_SEMANTIC_LABEL_MOUSE_ANDROID = 2,
        XR_SPATIAL_OBJECT_SEMANTIC_LABEL_LAPTOP_BASE_ANDROID = 3,
        XR_SPATIAL_OBJECT_SEMANTIC_LABEL_MAX_ENUM_ANDROID = 0x7FFFFFFF
    } XrSpatialObjectSemanticLabelANDROID;
    typedef struct XrSpatialCapabilityConfigurationObjectTrackingANDROID {
        XrStructureType                               type;
        const void* XR_MAY_ALIAS                      next;
        XrSpatialCapabilityEXT                        capability;
        uint32_t                                      enabledComponentCount;
        const XrSpatialComponentTypeEXT* enabledComponents;
        uint32_t                                      activeSemanticLabelCount;
        const XrSpatialObjectSemanticLabelANDROID* activeSemanticLabels;
    } XrSpatialCapabilityConfigurationObjectTrackingANDROID;

    // XrSpatialComponentObjectSemanticLabelListANDROID extends XrSpatialComponentDataQueryResultEXT
    typedef struct XrSpatialComponentObjectSemanticLabelListANDROID {
        XrStructureType                         type;
        void* XR_MAY_ALIAS                      next;
        uint32_t                                semanticLabelCount;
        XrSpatialObjectSemanticLabelANDROID* semanticLabels;
    } XrSpatialComponentObjectSemanticLabelListANDROID;



    // XR_ANDROID_spatial_discovery_raycast is a preprocessor guard. Do not pass it to API calls.
#define XR_ANDROID_spatial_discovery_raycast 1
#define XR_ANDROID_spatial_discovery_raycast_SPEC_VERSION 1
#define XR_ANDROID_SPATIAL_DISCOVERY_RAYCAST_EXTENSION_NAME "XR_ANDROID_spatial_discovery_raycast"
    typedef struct XrSpatialRaycastResultDataANDROID {
        XrPosef    hitPose;
        float      distanceSquared;
    } XrSpatialRaycastResultDataANDROID;

    typedef struct XrSpatialCapabilityConfigurationDepthRaycastANDROID {
        XrStructureType                     type;
        const void* XR_MAY_ALIAS            next;
        XrSpatialCapabilityEXT              capability;
        uint32_t                            enabledComponentCount;
        const XrSpatialComponentTypeEXT* enabledComponents;
    } XrSpatialCapabilityConfigurationDepthRaycastANDROID;

    // XrSpatialRaycastInfoANDROID extends XrSpatialDiscoverySnapshotCreateInfoEXT
    typedef struct XrSpatialRaycastInfoANDROID {
        XrStructureType             type;
        const void* XR_MAY_ALIAS    next;
        XrSpace                     space;
        XrTime                      time;
        XrVector3f                  origin;
        XrVector3f                  direction;
        float                       maxDistance;
    } XrSpatialRaycastInfoANDROID;

    // XrSpatialComponentRaycastResultListANDROID extends XrSpatialComponentDataQueryResultEXT
    typedef struct XrSpatialComponentRaycastResultListANDROID {
        XrStructureType                       type;
        void* XR_MAY_ALIAS                    next;
        uint32_t                              raycastResultCount;
        XrSpatialRaycastResultDataANDROID* raycastResults;
    } XrSpatialComponentRaycastResultListANDROID;

    typedef struct XrSpatialRaycastSnapshotCreateInfoANDROID {
        XrStructureType                       type;
        const void* XR_MAY_ALIAS              next;
        uint32_t                              componentTypeCount;
        const XrSpatialComponentTypeEXT* componentTypes;
        const XrSpatialRaycastInfoANDROID* raycastInfo;
    } XrSpatialRaycastSnapshotCreateInfoANDROID;

    typedef XrResult(XRAPI_PTR* PFN_xrCreateSpatialRaycastSnapshotANDROID)(XrSpatialContextEXT spatialContext, const XrSpatialRaycastSnapshotCreateInfoANDROID* createInfo, XrSpatialSnapshotEXT* snapshot);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
    XRAPI_ATTR XrResult XRAPI_CALL xrCreateSpatialRaycastSnapshotANDROID(
        XrSpatialContextEXT                         spatialContext,
        const XrSpatialRaycastSnapshotCreateInfoANDROID* createInfo,
        XrSpatialSnapshotEXT* snapshot);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */


    // XR_ANDROID_spatial_entity_bound_anchor is a preprocessor guard. Do not pass it to API calls.
#define XR_ANDROID_spatial_entity_bound_anchor 1
#define XR_ANDROID_spatial_entity_bound_anchor_SPEC_VERSION 1
#define XR_ANDROID_SPATIAL_ENTITY_BOUND_ANCHOR_EXTENSION_NAME "XR_ANDROID_spatial_entity_bound_anchor"
// XrSpatialAnchorParentANDROID extends XrSpatialAnchorCreateInfoEXT
    typedef struct XrSpatialAnchorParentANDROID {
        XrStructureType             type;
        const void* XR_MAY_ALIAS    next;
        XrSpatialEntityIdEXT        parentId;
    } XrSpatialAnchorParentANDROID;

    typedef XrResult(XRAPI_PTR* PFN_xrEnumerateSpatialAnchorAttachableComponentsANDROID)(XrInstance instance, XrSystemId systemId, uint32_t attachableComponentCapacityInput, uint32_t* attachableComponentCountOutput, XrSpatialComponentTypeEXT* attachableComponents);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
    XRAPI_ATTR XrResult XRAPI_CALL xrEnumerateSpatialAnchorAttachableComponentsANDROID(
        XrInstance                                  instance,
        XrSystemId                                  systemId,
        uint32_t                                    attachableComponentCapacityInput,
        uint32_t* attachableComponentCountOutput,
        XrSpatialComponentTypeEXT* attachableComponents);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */


    // XR_ANDROID_spatial_component_subsumed_by is a preprocessor guard. Do not pass it to API calls.
#define XR_ANDROID_spatial_component_subsumed_by 1
#define XR_ANDROID_spatial_component_subsumed_by_SPEC_VERSION 1
#define XR_ANDROID_SPATIAL_COMPONENT_SUBSUMED_BY_EXTENSION_NAME "XR_ANDROID_spatial_component_subsumed_by"
// XrSpatialDiscoveryUniqueEntitiesFilterANDROID extends XrSpatialDiscoverySnapshotCreateInfoEXT
    typedef struct XrSpatialDiscoveryUniqueEntitiesFilterANDROID {
        XrStructureType             type;
        const void* XR_MAY_ALIAS    next;
    } XrSpatialDiscoveryUniqueEntitiesFilterANDROID;

    // XrSpatialComponentSubsumedByListANDROID extends XrSpatialComponentDataQueryResultEXT
    typedef struct XrSpatialComponentSubsumedByListANDROID {
        XrStructureType          type;
        void* XR_MAY_ALIAS       next;
        uint32_t                 subsumedUniqueIdCount;
        XrSpatialEntityIdEXT* subsumedUniqueIds;
    } XrSpatialComponentSubsumedByListANDROID;




#ifdef __cplusplus
}
#endif

#endif