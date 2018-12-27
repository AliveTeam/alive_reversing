#pragma once

#include "FunctionFwd.hpp"
#include "EffectBase.hpp"

class DeathFadeOut : public EffectBase
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(int** pOrderingTable) override;

    EXPORT DeathFadeOut* ctor_427030(__int16 layer, __int16 direction, __int16 destroyOnDone, short speed, char abr);
    EXPORT void Init_427140(__int16 layer, __int16 direction, __int16 destroyOnDone, short speed);
private:
    EXPORT void dtor_427120();
    EXPORT DeathFadeOut* vdtor_4270F0(signed int flags);
    EXPORT void Update_4271C0();
    EXPORT void vRender_427210(int** pOt);
private:
    __int16 field_78_current_fade_rgb;
    __int16 field_7A_speed;
    __int16 field_7C_direction;
public:
    __int16 field_7E_bDone;
private:
    __int16 field_80_destroy_on_done;
    __int16 field_82;
};
ALIVE_ASSERT_SIZEOF(DeathFadeOut, 0x84);
