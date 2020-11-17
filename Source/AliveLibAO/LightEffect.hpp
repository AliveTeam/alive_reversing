#pragma once

#include "FunctionFwd.hpp"
#include "DoorLight.hpp"

namespace AO {

class LightEffect : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT LightEffect* ctor_4064C0(Path_LightEffect* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_406770();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT LightEffect* Vdtor_406800(signed int flags);


    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_4067E0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_406610();

    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;

    int field_E4_tlvInfo;
    __int16 field_E8;
    __int16 field_EA;
    int field_EC_rnd1;
    int field_F0_rnd2;
    int field_F4_rnd3;
    int field_F8_rnd4;
};
ALIVE_ASSERT_SIZEOF(LightEffect, 0xFC);

}

