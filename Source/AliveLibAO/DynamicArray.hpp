#pragma once

#include "FunctionFwd.hpp"
#include "Types.hpp"

namespace AO {

class DynamicArray
{
public:
    EXPORT DynamicArray* ctor_4043E0(s16 startingSize);
    EXPORT void dtor_404440();

protected:
    EXPORT s16 Push_Back_404450(void* item);
    EXPORT s16 Remove_Item_404520(void* item);

public:
    void** field_0_array;

public:
    s16 field_4_used_size;

private:
    s16 field_6_max_size;
    s16 field_8_expand_size;
    s16 field_A_pad;
};

ALIVE_ASSERT_SIZEOF(DynamicArray, 0xC);


// Typed wrapper for DynamicArray
template <class T>
class DynamicArrayT final : public DynamicArray
{
public:
    s16 Push_Back(T* pValue)
    {
        return Push_Back_404450(pValue);
    }

    s16 Remove_Item(T* pItemToRemove)
    {
        return Remove_Item_404520(pItemToRemove);
    }

    T* ItemAt(s32 idx)
    {
        if (idx < 0 || idx >= field_4_used_size)
        {
            LOG_ERROR(idx << " is out of bounds max= " << field_4_used_size);
        }
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

} // namespace AO
