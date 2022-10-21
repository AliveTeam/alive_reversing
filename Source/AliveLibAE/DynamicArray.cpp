#include "stdafx.h"
#include "DynamicArray.hpp"
#include "stdlib.hpp"
#include "Function.hpp"

void DynamicArray_ForceLink()
{ }

DynamicArray* DynamicArray::ctor_40CA60(s16 startingSize)
{
    if (startingSize == 0)
    {
        startingSize = 1;
    }

    field_0_array = reinterpret_cast<void**>(ae_malloc_non_zero_4954F0(startingSize * sizeof(void*)));
    field_4_used_size = 0;
    field_6_max_size = 0;

    if (field_0_array)
    {
        field_6_max_size = startingSize;
        field_8_expand_size = 8;
    }

    return this;
}

DynamicArray* DynamicArray::ctor_40C9E0(s16 startingSize)
{
    ctor_40CA60(startingSize);
    for (s32 i = 0; i < startingSize; i++)
    {
        Push_Back_40CAF0(nullptr);
    }
    return this;
}

void DynamicArray::dtor_40CAD0()
{
    ae_non_zero_free_495560(field_0_array);
}

s16 DynamicArray::Push_Back_40CAF0(void* pValue)
{
    if (!field_0_array)
    {
        return 0;
    }

    // If we have no more elements then expand the array
    if (field_4_used_size == field_6_max_size)
    {
        if (!Expand_40CBE0(field_8_expand_size))
        {
            return 0;
        }
    }

    field_0_array[field_4_used_size++] = pValue;
    return 1;
}

s16 DynamicArray::Remove_Item_40CB60(void* pItemToRemove)
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
            return 1;
        }
    }
    return 0;
}

s16 DynamicArray::Expand_40CBE0(s16 expandSize)
{
    // Calculate new size and allocate buffer
    const s16 newSize = field_6_max_size + expandSize;
    void** pNewBuffer = reinterpret_cast<void**>(ae_malloc_non_zero_4954F0(newSize * sizeof(void*)));
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
        ae_non_zero_free_495560(field_0_array);
    }

    // Update max size and array pointer
    field_6_max_size = newSize;
    field_0_array = pNewBuffer;

    return 1;
}

void DynamicArrayIter::Remove_At_Iter_40CCA0()
{
    field_4_idx--;
    field_0_pDynamicArray->field_4_used_size--;

    // Overwrite the items to remove with the item from the end
    field_0_pDynamicArray->field_0_array[field_4_idx] = field_0_pDynamicArray->field_0_array[field_0_pDynamicArray->field_4_used_size];
}
