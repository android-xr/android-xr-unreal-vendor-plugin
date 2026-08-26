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
#include <AndroidXRTypes.h>

 /**
 * An abstraction over a XrSpatialBuffer
 */
class ANDROIDXRSPATIALENTITIES_API FAndroidXRSpatialBuffer
{
public:
    FAndroidXRSpatialBuffer() = default;
    FAndroidXRSpatialBuffer(XrSpatialBufferEXT InBuffer);
    void AllocateBuffer(int32 ElementCount);

    template<typename T>
    T* GetNativeBuffer();

    template<typename T>
    void FillData(TArray<T>& Array);

    XrSpatialBufferEXT Buffer{};

    void Reset(XrSpatialBufferEXT InBuffer);
private:
    TArray<uint8> BufferBlock{};
    int32 DataTypeSize;
    int32 DataCount;
};

template<typename T>
T* FAndroidXRSpatialBuffer::GetNativeBuffer()
{
    return reinterpret_cast<T*>(BufferBlock.GetData());
}

template<typename T>
inline void FAndroidXRSpatialBuffer::FillData(TArray<T>& Array)
{
    Array.Reset();
    Array.Append(reinterpret_cast<T*>(BufferBlock.GetData()), DataCount);
}