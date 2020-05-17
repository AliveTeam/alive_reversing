#pragma once

#include "FunctionFwd.hpp"

EXPORT void CC Remove_Item_417350(void*);

START_NS_AO

class BaseGameObject
{
public:
    EXPORT BaseGameObject* ctor_487E10(__int16 arraySize);
    EXPORT void dtor_487DF0();

    virtual BaseGameObject* VDestructor(signed int flags) = 0;

    virtual void VUpdate()
    {

    }

    virtual void VRender(int**)
    {

    }

    virtual void VScreenChanged()
    {

    }

    virtual void VUnknown()
    {

    }

public:
    __int16 field_4_typeId;
    unsigned __int16 field_6_flags;
    int field_8_update_delay;
    char field_C_bCanKill;
    char field_D;
    __int16 field_E;
};
ALIVE_ASSERT_SIZEOF(BaseGameObject, 0x10);

ALIVE_VAR_EXTERN(class DynamicArray*, gBaseGameObject_list_9F2DF0);

END_NS_AO
