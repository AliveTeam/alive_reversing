#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

struct Path_DoorFlame final : public Path_TLV
{
    s16 mSwitchId;
    Scale_short mScale;
    s16 field_14_colour;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_DoorFlame, 0x18);

class DoorFlame final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    DoorFlame(Path_DoorFlame* pTlv, s32 tlvInfo);
    ~DoorFlame();

    virtual void VUpdate() override;
    virtual void VStopAudio() override;
    virtual void VScreenChanged() override;

private:
    s32 field_F4_tlvInfo = 0;
    s16 field_F8_switch_id = 0;
    s16 field_FA_frame_count = 0;
    enum class States : s16
    {
        eDisabled_0 = 0,
        eEnabled_1 = 1,
    };
    States field_FC_state = States::eDisabled_0;
    s16 field_FE_2_random = 0;
    s32 field_100_sounds_mask = 0;
    s32 field_108_fire_background_glow_id = 0;
    s32 field_10C_flame_sparks_id = 0;
};
ALIVE_ASSERT_SIZEOF(DoorFlame, 0x110);
