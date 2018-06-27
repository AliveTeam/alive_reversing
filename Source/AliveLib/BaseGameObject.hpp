#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"

class BaseGameObject
{
public:
    // Order must match VTable
    virtual void VDestructor(signed int) = 0; // Not an actual dtor because the generated compiler code has the param to determine if heap allocated or not
    virtual void VUpdate();
    virtual void VRender(int** pOrderingTable);
    EXPORT virtual void vsub_4DC0A0();
    virtual void vnullsub_4DC0F0();
    virtual int GetSaveState_4DC110(BYTE* pSaveBuffer);

    EXPORT void ctor_4DBFA0(__int16 bAddToObjectList, signed __int16 resourceArraySize);

public:
    __int16 field_4_typeId;
    __int16 field_6_flags;
    int field_8_flags_ex;
    int field_C_objectId;
    DynamicArray field_10_resources_array;
    int field_1C_update_delay;
};
ALIVE_ASSERT_SIZEOF(BaseGameObject, 0x20);

ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gBaseGameObject_list_BB47C4);

class Class_5C1B70
{
public:
    EXPORT void sub_449C10(int objCount, void* pGameObj);
};

