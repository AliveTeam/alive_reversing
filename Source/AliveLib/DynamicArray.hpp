#pragma once

#include "Function.hpp"

// TODO: Can be made into a template when all usages are reversed.
void DynamicArray_ForceLink();

class DynamicArray
{
public:
    DynamicArray* ctor_40CA60(signed __int16 startingSize);
    DynamicArray* ctor_40C9E0(signed __int16 startingSize);
    void dtor_40CAD0();

    signed __int16 Push_Back_40CAF0(void* pValue);
    signed __int16 Remove_Item_40CB60(void* pItemToRemove);
    signed __int16 Expand_40CBE0(__int16 expandSize);

    void** field_0_array;
    __int16 field_4_used_size;
    __int16 field_6_max_size;
    __int16 field_8_expand_size;
    // padding
};
ALIVE_ASSERT_SIZEOF(DynamicArray, 0xC);

class DynamicArrayIter
{
public:
    void Remove_At_Iter_40CCA0();

    DynamicArray* field_0_pDynamicArray;
    __int16 field_4_idx;
    // padding
};
ALIVE_ASSERT_SIZEOF(DynamicArrayIter, 0x8);
