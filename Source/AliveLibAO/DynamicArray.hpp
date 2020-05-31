#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

class DynamicArray
{
public:
    EXPORT DynamicArray* ctor_4043E0(__int16 startingSize);
    EXPORT void dtor_404440();
protected:
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


// Typed wrapper for DynamicArray
template<class T>
class DynamicArrayT : public DynamicArray
{
public:
    __int16 Push_Back(T* pValue)
    {
        return Push_Back_404450(pValue);
    }

    __int16 Remove_Item(T* pItemToRemove)
    {
        return Remove_Item_404520(pItemToRemove);
    }

    T* ItemAt(int idx)
    {
        return reinterpret_cast<T*>(field_0_array[idx]);
    }

    void SetAt(int idx, T* itemToSet)
    {
        field_0_array[idx] = itemToSet;
    }
};

END_NS_AO
