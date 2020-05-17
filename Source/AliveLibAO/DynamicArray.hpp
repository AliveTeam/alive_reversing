#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

class DynamicArray
{
public:
    EXPORT DynamicArray* ctor_4043E0(__int16 startingSize);
    EXPORT void dtor_404440();
public:
    EXPORT __int16 Push_Back_404450(void *item);
    EXPORT __int16 Remove_Item_404520(void *item);
public:
    void** field_0_array;
public:
    __int16 field_4_used_size;
private:
    __int16 field_6_max_size;
    __int16 field_8_expand_size;
    __int16 field_A_pad;
};

ALIVE_ASSERT_SIZEOF(DynamicArray, 0xC);

END_NS_AO
