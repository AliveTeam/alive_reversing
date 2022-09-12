#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "FixedPoint.hpp"

namespace AO {

enum class BellSize : s16
{
    eBig = 0,
    eMedium = 1,
    eSmall = 2,
};

class Bells final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Bells(BellSize bellSize, FP xpos, FP ypos, FP scale);

    void LoadAnimations();

    virtual void VUpdate() override;

    void PlaySounds();
    bool CanSmash();
    void Ring();

    bool mSmashing = false;
    enum class BellPitch : u16
    {
        eLowPitch = 0,
        eMediumPitch = 1,
        eHighPitch = 2
    };
    BellPitch mBellPitch = BellPitch::eLowPitch;
    bool mDoScreenWave = false;
    s8 mSoundPitchFactor = 0;
    s32 mSoundCooldownTimer = 0;
};
ALIVE_ASSERT_SIZEOF(Bells, 0xF8);

} // namespace AO
