#pragma once

#include "../AliveLibCommon/Function.hpp"

// TODO: Can be made into a template when all usages are reversed.
class DynamicArray
{
public:
    explicit DynamicArray(s32 startingSize);
    ~DynamicArray();

    DynamicArray(const DynamicArray& rhs) = delete;
    DynamicArray& operator=(const DynamicArray& rhs) const = delete;

    s16 Expand(s16 expandSize);

    bool IsEmpty() const
    {
        return field_4_used_size == 0;
    }

    s16 Size() const
    {
        return field_4_used_size;
    }

public:
    s16 Push_Back(void* pValue);

protected:
    s16 Remove_Item(void* pItemToRemove);
    void** field_0_array = nullptr;

public:
    s16 field_4_used_size = 0;

private:
    s16 field_6_max_size = 0;
    s16 field_8_expand_size = 0;
    // padding

    friend class DynamicArrayIter;
};
ALIVE_ASSERT_SIZEOF(DynamicArray, 0xC);

// Typed wrapper for DynamicArray
template <class T>
class DynamicArrayT final : public DynamicArray
{
public:
    using DynamicArray::DynamicArray;

    s16 Push_Back(T* pValue)
    {
        return DynamicArray::Push_Back(pValue);
    }

    s16 Remove_Item(T* pItemToRemove)
    {
        return DynamicArray::Remove_Item(pItemToRemove);
    }

    T* ItemAt(s32 idx)
    {
        return reinterpret_cast<T*>(field_0_array[idx]);
    }

    void SetAt(s32 idx, T* itemToSet)
    {
        field_0_array[idx] = itemToSet;
    }

    s32 Size() const
    {
        return field_4_used_size;
    }

    bool Empty() const
    {
        return field_4_used_size == 0;
    }

    s32 RemoveAt(s32 idx)
    {
        field_4_used_size--;

        // Overwrite the items to remove with the item from the end
        field_0_array[idx] = field_0_array[field_4_used_size];

        return idx - 1;
    }
};

class DynamicArrayIter final
{
public:
    void Remove_At_Iter();

    DynamicArray* field_0_pDynamicArray;
    s16 field_4_idx;
    // padding
};
ALIVE_ASSERT_SIZEOF(DynamicArrayIter, 0x8);
