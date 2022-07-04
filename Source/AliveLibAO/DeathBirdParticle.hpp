#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class DeathBirdParticle final : public BaseAnimatedWithPhysicsGameObject
{
public:
    DeathBirdParticle(FP xpos, FP ypos, s32 start, s32 playSound, FP scale);

    virtual void VUpdate() override;

    s8 field_E4_random = 0;
    enum class States : s8
    {
        eAnimateDeathFlares_0 = 0,
        eTransformStarsToDoves_1 = 1
    };
    States field_E5_state = States::eAnimateDeathFlares_0;
    s32 field_E8_start = 0;
    s16 field_EC_bPlaySound = 0;
};
ALIVE_ASSERT_SIZEOF(DeathBirdParticle, 0xF0);

} // namespace AO
