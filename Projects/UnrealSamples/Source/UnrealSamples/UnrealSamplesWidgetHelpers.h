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

