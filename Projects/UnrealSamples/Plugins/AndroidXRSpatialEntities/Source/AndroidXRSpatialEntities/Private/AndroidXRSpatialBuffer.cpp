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
#include "AndroidXRSpatialBuffer.h"

FAndroidXRSpatialBuffer::FAndroidXRSpatialBuffer(XrSpatialBufferEXT InBuffer)
{
    Reset(InBuffer);
}

void FAndroidXRSpatialBuffer::AllocateBuffer(int32 ElementCount)
{
    BufferBlock.Reset();
    BufferBlock.SetNum(ElementCount * DataTypeSize);
    DataCount = ElementCount;
}

void FAndroidXRSpatialBuffer::Reset(XrSpatialBufferEXT InBuffer)
{
    Buffer = InBuffer;
    switch(Buffer.bufferType)
    {
        case XrSpatialBufferTypeEXT::XR_SPATIAL_BUFFER_TYPE_STRING_EXT:
        {
            DataTypeSize = sizeof(char);
            break;
        }
        case XrSpatialBufferTypeEXT::XR_SPATIAL_BUFFER_TYPE_FLOAT_EXT:
        {
            DataTypeSize = sizeof(float);
            break;
        }
        case XrSpatialBufferTypeEXT::XR_SPATIAL_BUFFER_TYPE_VECTOR2F_EXT:
        {
            DataTypeSize = sizeof(XrVector2f);
            break;
        }
        case XrSpatialBufferTypeEXT::XR_SPATIAL_BUFFER_TYPE_VECTOR3F_EXT:
        {
            DataTypeSize = sizeof(XrVector3f);
            break;
        }
        case XrSpatialBufferTypeEXT::XR_SPATIAL_BUFFER_TYPE_UINT8_EXT:
        {
            DataTypeSize = sizeof(uint8_t);
            break;
        }
        case XrSpatialBufferTypeEXT::XR_SPATIAL_BUFFER_TYPE_UINT16_EXT: {
            DataTypeSize = sizeof(uint16_t);
            break;
        }
        case XrSpatialBufferTypeEXT::XR_SPATIAL_BUFFER_TYPE_UINT32_EXT: {
            DataTypeSize = sizeof(uint32_t);
            break;
        }
        default:
            break;
    }
}