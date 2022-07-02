#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FixedPoint.hpp"

namespace AO {

enum class BellSize : s16
{
    eBig_0 = 0,
    eMedium_1 = 1,
    eSmall_2 = 2,
};

class Bells final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Bells(BellSize bellSize, FP xpos, FP ypos, FP scale);

    virtual void VUpdate() override;

    void PlaySounds();
    bool CanSmash();
    void Ring();

    u16 field_E8_bSmashing = 0;
    enum class Sounds : u16
    {
        eLowPitch_0 = 0,
        eMediumPitch_1 = 1,
        eHighPitch_2 = 2
    };
    Sounds field_EA_sound = Sounds::eLowPitch_0;
    s32 field_EC_bDoScreenWave = 0;
    s8 field_F0_sound_pitch_factor = 0;
    s32 field_F4_sound_cooldown_timer = 0;
};
ALIVE_ASSERT_SIZEOF(Bells, 0xF8);

} // namespace AO
