#include "stdafx.h"
#include "DynamicArray.hpp"

void DynamicArray_ForceLink() { }

EXPORT void* CC malloc_non_zero_4954F0(size_t size)
{
    NOT_IMPLEMENTED;
    if (size == 0)
    {
        size = 1;
    }
    return malloc(size);
}

EXPORT void CC Mem_Free_495540(void* ptr)
{
    NOT_IMPLEMENTED;
    free(ptr);
}

EXPORT void CC Mem_Free_495560(void *ptr)
{
    NOT_IMPLEMENTED;
    free(ptr);
}

DynamicArray* DynamicArray::ctor_40CA60(signed __int16 startingSize)
{
    if (startingSize == 0)
    {
        startingSize = 1;
    }

    field_0_array = reinterpret_cast<void**>(malloc_non_zero_4954F0(startingSize * sizeof(void*)));
    field_4_used_size = 0;
    field_6_max_size = 0;

    if (field_0_array)
    {
        field_6_max_size = startingSize;
        field_8_expand_size = 8;
    }

    return this;
}

DynamicArray* DynamicArray::ctor_40C9E0(signed __int16 startingSize)
{
    ctor_40CA60(startingSize);
    for (int i = 0; i < startingSize; i++)
    {
        Push_Back_40CAF0(nullptr);
    }
    return this;
}

void DynamicArray::dtor_40CAD0()
{
    Mem_Free_495560(field_0_array);
}

signed __int16 DynamicArray::Push_Back_40CAF0(void* pValue)
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

signed __int16 DynamicArray::Remove_Item_40CB60(void* pItemToRemove)
{
    DynamicArrayIter arrayIter;
    arrayIter.field_0_pDynamicArray = this;
    arrayIter.field_4_idx = 0;
    for (__int16 idx = 0; idx < field_4_used_size; idx++)
    {
        void* pCurrentItem = field_0_array[idx];
        arrayIter.field_4_idx = idx + 1;

        if (!pCurrentItem)
        {
            break;
        }

        if (pCurrentItem == pItemToRemove)
        {
            arrayIter.Remove_At_Iter_40CCA0();
            return 1;
        }
    }
    return 0;
}

signed __int16 DynamicArray::Expand_40CBE0(__int16 expandSize)
{
    // Calculate new size and allocate buffer
    const __int16 newSize = field_6_max_size + expandSize;
    void** pNewBuffer = reinterpret_cast<void**>(malloc_non_zero_4954F0(newSize * sizeof(void*)));
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
        Mem_Free_495560(field_0_array);
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
