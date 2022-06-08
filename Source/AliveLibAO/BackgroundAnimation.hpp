#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FixedPoint.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

enum class BgAnimSounds : s16
{
    eNone_m1 = -1,
    eNone_0 = 0,
    eFire_1 = 1,
    eFireIdx_40 = 40
};

struct Path_BackgroundAnimation final : public Path_TLV
{
    u16 field_18_animation_id;
    Choice_short field_1A_is_semi_trans;
    TPageAbr field_1C_semi_trans_mode;
    // pad
    BgAnimSounds field_1E_sound_effect;
};
ALIVE_ASSERT_SIZEOF(Path_BackgroundAnimation, 0x20);

class BackgroundAnimation final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BackgroundAnimation(Path_BackgroundAnimation* pTlv, s32 tlvInfo);
    ~BackgroundAnimation();

    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual void VUpdate() override;

    u8** field_E4_res = nullptr;
    s16 field_E8_xpos = 0;
    s16 field_EA_ypos = 0;
    s16 field_EC_w = 0;
    s16 field_EE_h = 0;
    s32 field_F0_tlvInfo = 0;
    FP field_F8_animXPos = FP_FromInteger(0);
    FP field_FC_animYPos = FP_FromInteger(0);
    BgAnimSounds field_100_sound_effect = BgAnimSounds::eNone_0;
    s32 field_104_sound_channels_mask = 0;
};
ALIVE_ASSERT_SIZEOF(BackgroundAnimation, 0x108);

} // namespace AO
