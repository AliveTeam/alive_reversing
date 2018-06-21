#pragma once

#include "Function.hpp"

// TODO: Can be made into a template when all usages are reversed.
void DynamicArray_ForceLink();

// Typed wrapper for DynamicArray
template<class T>
class DynamicArrayT : public DynamicArray
{
public:
    signed __int16 Push_Back(T* pValue)
    {
        Push_Back_40CAF0(pValue);
    }

    signed __int16 Remove_Item(T* pItemToRemove)
    {
        Remove_Item_40CB60(pItemToRemove);
    }

    T* ItemAt(int idx)
    {
        return reinterpret_cast<T*>(field_0_array[idx]);
    }
};

class DynamicArray
{
public:
    EXPORT DynamicArray* ctor_40CA60(signed __int16 startingSize);
    EXPORT DynamicArray* ctor_40C9E0(signed __int16 startingSize);
    EXPORT void dtor_40CAD0();

    EXPORT signed __int16 Expand_40CBE0(__int16 expandSize);
    bool IsEmpty() const {  return field_4_used_size == 0; }
    __int16 Size() const {  return field_4_used_size; }
protected:
    EXPORT signed __int16 Push_Back_40CAF0(void* pValue);
    EXPORT signed __int16 Remove_Item_40CB60(void* pItemToRemove);
    void** field_0_array;
private:
    __int16 field_4_used_size;
    __int16 field_6_max_size;
    __int16 field_8_expand_size;
    // padding

    friend class DynamicArrayIter;
};
ALIVE_ASSERT_SIZEOF(DynamicArray, 0xC);

class DynamicArrayIter
{
public:
    EXPORT void Remove_At_Iter_40CCA0();

    DynamicArray* field_0_pDynamicArray;
    __int16 field_4_idx;
    // padding
};
ALIVE_ASSERT_SIZEOF(DynamicArrayIter, 0x8);
