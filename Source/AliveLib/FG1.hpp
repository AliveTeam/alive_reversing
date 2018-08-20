#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "DynamicArray.hpp"

class FG1 : public BaseGameObject
{
public:
    EXPORT void ctor_499FC0(BYTE** pFg1Res);

    virtual void VDestructor(signed int flags) override;
    EXPORT void vdtor_49A1E0(signed int flags);
    EXPORT void dtor_49A540();

    int field_20;
    __int16 field_24_level_id;
    __int16 field_26_path_id;
    __int16 field_28;
    __int16 field_2A;
    BYTE** field_2C_ptr;
    int field_30;
};
ALIVE_ASSERT_SIZEOF(FG1, 0x34);

ALIVE_VAR_EXTERN(DynamicArrayT<FG1>*, gFG1List_5D1E28);
