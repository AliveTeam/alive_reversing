#include "stdafx_ao.h"
#include "DynamicArray.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

START_NS_AO

DynamicArray* DynamicArray::ctor_4043E0(__int16 startingSize)
{
    const auto sizeInBytes = startingSize * sizeof(void*);
    field_0_array = reinterpret_cast<void**>(alloc_450740(sizeInBytes));

    field_4_used_size = 0;
    field_6_max_size = 0;

    if (field_0_array)
    {
        memset(field_0_array, 0, sizeInBytes);
        field_4_used_size = 0;
        field_6_max_size = startingSize;
        field_8_expand_size = 10;
    }

    return this;
}

void DynamicArray::dtor_404440()
{
    ao_delete_free_450770(field_0_array);
}

__int16 DynamicArray::Push_Back_404450(void *item)
{
    if (!item || !field_0_array)
    {
        return 0;
    }

    if (field_4_used_size == field_6_max_size)
    {
        // Inlined expand call?
        const __int16 newSizeElements = field_6_max_size + field_8_expand_size;
        const auto newArraySizeBytes = newSizeElements * sizeof(void*);
        auto pNewArray = reinterpret_cast<void**>(alloc_450740(newArraySizeBytes));
        if (!pNewArray)
        {
            return 0;
        }

        // Clear new memory + copy in old array
        memset(pNewArray, 0, newArraySizeBytes);
        memcpy(pNewArray, field_0_array, field_4_used_size * sizeof(void*));

        // Free old array + assign new values
        ao_delete_free_450770(field_0_array);
        field_0_array = pNewArray;
        field_6_max_size = newSizeElements;
    }

    field_0_array[field_4_used_size++] = item;
    return 1;
}

__int16 DynamicArray::Remove_Item_404520(void *item)
{
    // TODO: FIX ME
    NOT_IMPLEMENTED();

    int counter = 0;
    void* pCurItem = nullptr;
    for (;;)
    {
        if (counter < field_4_used_size)
        {
            counter++;
            pCurItem = field_0_array[counter];
        }
        else
        {
            pCurItem = nullptr;
        }

        if (!pCurItem)
        {
            break;
        }

        if (pCurItem == item)
        {
            field_4_used_size--;
            field_0_array[counter - 1] = field_0_array[field_4_used_size];
            field_0_array[field_4_used_size] = nullptr;
            return 1;
        }
    }
    return 0;
}

END_NS_AO
