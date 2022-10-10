#pragma once

#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

class Explosion final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Explosion(FP xpos, FP ypos, FP scale, bool bSmall);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    void DealBlastDamage(PSX_RECT* pRect);

private:
    s16 field_F4_bSmall = 0;
    FP field_F8_scale = {};
    FP field_FC_explosion_size = {};
};
