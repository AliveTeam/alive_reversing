#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"

// TODO: Can be made into a template when all usages are reversed.
void DynamicArray_ForceLink();

class DynamicArray
{
public:
    EXPORT DynamicArray* ctor_40CA60(s16 startingSize);
    EXPORT DynamicArray* ctor_40C9E0(s16 startingSize);
    EXPORT void dtor_40CAD0();

    EXPORT s16 Expand_40CBE0(s16 expandSize);
    bool IsEmpty() const
    {
        return field_4_used_size == 0;
    }
    s16 Size() const
    {
        return field_4_used_size;
    }

    s32 RemoveAt(s32 idx)
    {
        field_4_used_size--;

        // Overwrite the items to remove with the item from the end
        field_0_array[idx] = field_0_array[field_4_used_size];

        return idx - 1;
    }

public:
    EXPORT s16 Push_Back_40CAF0(void* pValue);

protected:
    EXPORT s16 Remove_Item_40CB60(void* pItemToRemove);
    void** field_0_array;

public:
    s16 field_4_used_size;

private:
    s16 field_6_max_size;
    s16 field_8_expand_size;
    // padding

    friend class DynamicArrayIter;
};
ALIVE_ASSERT_SIZEOF(DynamicArray, 0xC);

// Typed wrapper for DynamicArray
template <class T>
class DynamicArrayT final : public DynamicArray
{
public:
    s16 Push_Back(T* pValue)
    {
        return Push_Back_40CAF0(pValue);
    }

    s16 Remove_Item(T* pItemToRemove)
    {
        return Remove_Item_40CB60(pItemToRemove);
    }

    T* ItemAt(s32 idx)
    {
        return reinterpret_cast<T*>(field_0_array[idx]);
    }

    void SetAt(s32 idx, T* itemToSet)
    {
        field_0_array[idx] = itemToSet;
    }
};

class DynamicArrayIter final
{
public:
    EXPORT void Remove_At_Iter_40CCA0();

    DynamicArray* field_0_pDynamicArray;
    s16 field_4_idx;
    // padding
};
ALIVE_ASSERT_SIZEOF(DynamicArrayIter, 0x8);
