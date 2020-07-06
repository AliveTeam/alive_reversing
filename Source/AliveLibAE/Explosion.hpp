#pragma once

#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"

class Explosion : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Explosion* ctor_4A1200(FP xpos, FP ypos, FP scale, __int16 bSmall);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

private:

    EXPORT void DealBlastDamage_4A1BD0(PSX_RECT* pRect);

    EXPORT void vUpdate_4A1510();

    EXPORT void vScreenChanged_4A1EE0();

    EXPORT void dtor_4A14F0();

    EXPORT Explosion* vdtor_4A14C0(signed int flags);

private:
    __int16 field_F4_bSmall;
    //__int16 field_F6_pad;
    FP field_F8_scale;
    FP field_FC_explosion_size;
};
ALIVE_ASSERT_SIZEOF(Explosion, 0x100);
