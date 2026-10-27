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
#include "Components/ComboBoxString.h"

inline void AddEnumToComboBox(TObjectPtr<UComboBoxString>& ComboBox, UEnum* EnumPtr)
{
    if(!EnumPtr || !ComboBox)
    {
        return;
    }
    ComboBox->ClearOptions();
    auto NumEnums = EnumPtr->ContainsExistingMax() ? EnumPtr->NumEnums() - 1 : EnumPtr->NumEnums();
    for(auto Index = 0; Index < NumEnums; Index++)
    {
        ComboBox->AddOption(EnumPtr->GetNameStringByIndex(Index));
    }
    ComboBox->SetSelectedIndex(0);
}

template<typename TEnumType>
inline TEnumType GetValueFromComboBox(TObjectPtr<UComboBoxString>& ComboBox)
{
    auto EnumPtr = StaticEnum<TEnumType>();
    if(!EnumPtr)
    {
        return static_cast<TEnumType>(0);
    }
    return static_cast<TEnumType>(EnumPtr->GetValueByNameString(ComboBox->GetSelectedOption()));
}

