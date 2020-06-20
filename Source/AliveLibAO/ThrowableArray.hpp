#pragma once
#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

START_NS_AO;

class ThrowableArray : public BaseGameObject
{
public:
    EXPORT BaseGameObject* Vdtor_454690(signed int flags);
    EXPORT void VUpdate_4542B0();
    EXPORT void vScreenChange_454300();

    EXPORT void sub_4540D0(char);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;

    __int16 field_10;
    __int16 field_12;
};
ALIVE_ASSERT_SIZEOF(ThrowableArray, 0x14);

ALIVE_VAR_EXTERN(ThrowableArray*, gpThrowableArray_50E26C);

END_NS_AO;
