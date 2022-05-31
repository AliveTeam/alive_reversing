#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

class DeathBirdParticle final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    virtual void VUpdate() override;

    DeathBirdParticle(FP xpos, FP ypos, s32 start, bool playSound, FP scale);

private:
    s8 field_F4_random;
    enum class States : s8
    {
        eAnimateDeathFlares_0 = 0,
        eTransformStarsToDoves_1 = 1
    };
    States field_F5_state;
    // 2 byte pad
    s32 field_F8_start;
    s16 field_FC_bPlaySound;
    // 2 byte pad
};
ALIVE_ASSERT_SIZEOF(DeathBirdParticle, 0x100);
