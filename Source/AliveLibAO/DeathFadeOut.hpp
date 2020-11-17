#pragma once

#include "FunctionFwd.hpp"
#include "EffectBase.hpp"

namespace AO {

class DeathFadeOut : public EffectBase
{
public:
    EXPORT DeathFadeOut* ctor_419DB0(__int16 layer, __int16 direction, __int16 destroyOnDone, int speed, __int16 abr);

    EXPORT void Init_419E40(__int16 layer, __int16 direction, __int16 destroyOnDone, int speed);

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    EXPORT void VUpdate_419E90();

    virtual void VRender(int** ppOt) override;

    EXPORT void VRender_419ED0(int** ppOt);

    BaseGameObject* VDestructor(signed int flags) override;

    __int16 field_68_current_fade_rgb;
    __int16 field_6A_speed;
    __int16 field_6C_direction;
    __int16 field_6E_bDone;
    __int16 field_70_destroy_on_done;
    __int16 field_72;
};
ALIVE_ASSERT_SIZEOF(DeathFadeOut, 0x74);

}

