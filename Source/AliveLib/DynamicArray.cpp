#include "stdafx.h"
#include "DynamicArray.hpp"

#define DYNAMIC_ARRAY_IMPL false

void* CC malloc_non_zero_4954F0(size_t size)
{
    if (size == 0)
    {
        size = 1;
    }
    return malloc(size);
}
ALIVE_FUNC_IMPLEX(0x4954F0, malloc_non_zero_4954F0, DYNAMIC_ARRAY_IMPL);

void CC Mem_Free_495540(void* ptr)
{
    free(ptr);
}
ALIVE_FUNC_IMPLEX(0x495540, Mem_Free_495540, DYNAMIC_ARRAY_IMPL);

DynamicArray* DynamicArray::Ctor_40CA60(signed __int16 startingSize)
{
    if (startingSize == 0)
    {
        startingSize = 1;
    }

    field_0_array = malloc_non_zero_4954F0(startingSize * sizeof(void*));
    field_4_used_size = 0;
    field_6_max_size = 0;

    if (field_0_array)
    {
        field_6_max_size = startingSize;
        field_8_expand_size = 8;
    }

    return this;
}
// TODO: Make ALIVE_FUNC_IMPLEX work with __thiscall
//ALIVE_FUNC_IMPLEX(0x40CA60, &DynamicArray::Ctor_40CA60, DYNAMIC_ARRAY_IMPL);
