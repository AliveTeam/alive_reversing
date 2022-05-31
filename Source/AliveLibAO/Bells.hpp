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
    Bool32 CanSmash();
    void Ring();

    s32 field_D4_padding;
    s32 field_D8_padding;
    s32 field_DC_padding;
    s32 field_E0_padding;
    s32 field_E4_padding;

    u16 field_E8_bSmashing;
    enum class Sounds : u16
    {
        eLowPitch_0 = 0,
        eMediumPitch_1 = 1,
        eHighPitch_2 = 2
    };
    Sounds field_EA_sound;
    s32 field_EC_bDoScreenWave;
    s8 field_F0_sound_pitch_factor;
    s8 field_F1_padding;
    s8 field_F2_padding;
    s8 field_F3_padding;
    s32 field_F4_sound_cooldown_timer;
};
ALIVE_ASSERT_SIZEOF(Bells, 0xF8);

} // namespace AO
