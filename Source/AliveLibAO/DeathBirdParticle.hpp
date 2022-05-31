#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class DeathBirdParticle final : public BaseAnimatedWithPhysicsGameObject
{
public:
    DeathBirdParticle(FP xpos, FP ypos, s32 start, s32 playSound, FP scale);

    virtual void VUpdate() override;

    s32 field_D4_padding[4];
    s8 field_E4_random;
    enum class States : s8
    {
        eAnimateDeathFlares_0 = 0,
        eTransformStarsToDoves_1 = 1
    };
    States field_E5_state;
    s16 field_E6_padding;
    s32 field_E8_start;
    s16 field_EC_bPlaySound;
    s16 field_EE_padding;
};
ALIVE_ASSERT_SIZEOF(DeathBirdParticle, 0xF0);

} // namespace AO
