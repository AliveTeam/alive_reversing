#include "stdafx.h"
#include "DynamicArray.hpp"

DynamicArray::DynamicArray(s32 startingSize)
{
    if (startingSize == 0)
    {
        startingSize = 1;
    }

    field_0_array = new (std::nothrow) void*[startingSize]; // TODO: Needs to be strongly typed
    field_4_used_size = 0;
    field_6_max_size = 0;

    if (field_0_array)
    {
        field_6_max_size = static_cast<s16>(startingSize);
        field_8_expand_size = 8;
    }
}

DynamicArray::~DynamicArray()
{
    delete[] field_0_array;
}

s16 DynamicArray::Push_Back(void* pValue)
{
    if (!field_0_array)
    {
        return 0;
    }

    // If we have no more elements then expand the array
    if (field_4_used_size == field_6_max_size)
    {
        if (!Expand(field_8_expand_size))
        {
            return 0;
        }
    }

    field_0_array[field_4_used_size++] = pValue;
    return 1;
}

void DynamicArray::Remove_Item(void* pItemToRemove)
{
    for (s32 idx = 0; idx < field_4_used_size; idx++)
    {
        void* pCurrentItem = field_0_array[idx];
        if (!pCurrentItem)
        {
            break;
        }

        if (pCurrentItem == pItemToRemove)
        {
            RemoveAt(idx);
            break;
        }
    }
}

s16 DynamicArray::Expand(s16 expandSize)
{
    // Calculate new size and allocate buffer
    const s16 newSize = field_6_max_size + expandSize;
    void** pNewBuffer = new (std::nothrow) void*[newSize];
    if (!pNewBuffer)
    {
        return 0;
    }

    // Copy in the used elements into the new buffer
    memcpy(pNewBuffer, field_0_array, field_4_used_size * sizeof(void*));

    // Free the old array buffer
    // OG BUG: Null checked here but not at the memcpy
    if (field_0_array)
    {
        delete[] field_0_array;
    }

    // Update max size and array pointer
    field_6_max_size = newSize;
    field_0_array = pNewBuffer;

    return 1;
}