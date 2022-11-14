#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class DeathBirdParticle final : public BaseAnimatedWithPhysicsGameObject
{
public:
    DeathBirdParticle(FP xpos, FP ypos, s32 startTimer, bool playSound, FP scale);

    void LoadAnimations();

    virtual void VUpdate() override;

private:
    s8 mRandom = 0;
    enum class States : s8
    {
        eAnimateDeathFlares_0 = 0,
        eTransformStarsToDoves_1 = 1
    };
    States mState = States::eAnimateDeathFlares_0;
    s32 mStartTimer = 0;
    bool mPlaySound = false;
};

} // namespace AO
