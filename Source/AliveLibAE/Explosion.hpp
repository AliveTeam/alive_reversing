#pragma once

#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

class Explosion final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Explosion(FP xpos, FP ypos, FP scale, bool bSmall);

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

private:
    EXPORT void DealBlastDamage_4A1BD0(PSX_RECT* pRect);

private:
    s16 field_F4_bSmall;
    //s16 field_F6_pad;
    FP field_F8_scale;
    FP field_FC_explosion_size;
};
ALIVE_ASSERT_SIZEOF(Explosion, 0x100);
