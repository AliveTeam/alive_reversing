#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

namespace AO {

class BaseAliveGameObject;

class PossessionFlicker : public BaseGameObject
{
public:
    EXPORT PossessionFlicker* ctor_41A8C0(BaseAliveGameObject* pToApplyFlicker, int duration, __int16 r, __int16 g, __int16 b);

    EXPORT BaseGameObject* dtor_41A940();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT PossessionFlicker* vdtor_41AA70(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41AA40();

    virtual void VUpdate() override;

    EXPORT void VUpdate_41A9B0();

    BaseAliveGameObject* field_10_pObj;
    int field_14_time_to_flicker;

    __int16 field_18_r;
    __int16 field_1A_g;
    __int16 field_1C_b;

    __int16 field_1E_old_r;
    __int16 field_20_old_g;
    __int16 field_22_old_b;
};
ALIVE_ASSERT_SIZEOF(PossessionFlicker, 0x24);

}

