#include "stdafx_ao.h"
#include "BaseGameObject.hpp"
#include "DynamicArray.hpp"
#include "Function.hpp"

EXPORT void CC Remove_Item_417350(void*)
{
    NOT_IMPLEMENTED();
}

START_NS_AO

ALIVE_VAR(1, 0x9F2DF0, DynamicArray*, gBaseGameObject_list_9F2DF0, nullptr);

BaseGameObject* BaseGameObject::ctor_487E10(__int16 arraySize)
{
    SetVTable(this, 0x4BD488); // vTable_BaseGameObject_4BD488

    if (!this) // Compiler code or hack
    {
        return this;
    }

    field_8_update_delay = 0;
    field_4_typeId = 0;
    field_C_bCanKill = 0;

    field_6_flags &= ~0x3F5u;
    field_6_flags |= 2;
    
    if (!arraySize)
    {
        return this;
    }

    if (!gBaseGameObject_list_9F2DF0->Push_Back_404450(this))
    {
        field_6_flags |= 1u;
    }

    return this;
}

void BaseGameObject::dtor_487DF0()
{
    SetVTable(this, 0x4BD488); // vTable_BaseGameObject_4BD488
    Remove_Item_417350(this);
}

END_NS_AO
