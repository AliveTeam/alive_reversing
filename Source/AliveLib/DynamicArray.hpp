#pragma once

#include "Function.hpp"

// TODO: Can be made into a template when all usages are reversed.

class DynamicArray
{
public:
    DynamicArray* Ctor_40CA60(signed __int16 startingSize);

    void* field_0_array;
    __int16 field_4_used_size;
    __int16 field_6_max_size;
    __int16 field_8_expand_size;
    // padding
};
ALIVE_ASSERT_SIZEOF(DynamicArray, 0xC);

class DynamicArrayIter
{
public:
    DynamicArray* field_0_pDynamicArray;
    __int16 field_4_idx;
    // padding
};
ALIVE_ASSERT_SIZEOF(DynamicArrayIter, 0x8);
