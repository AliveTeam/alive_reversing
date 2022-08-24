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
    u16 mAnimId;
    Choice_short mIsSemiTrans;
    TPageAbr mSemiTransMode;
    // pad
    BgAnimSounds mSoundEffect;
};
ALIVE_ASSERT_SIZEOF(Path_BackgroundAnimation, 0x20);
// TODO: size

class BackgroundAnimation final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BackgroundAnimation(relive::Path_BackgroundAnimation* pTlv, s32 tlvInfo);
    ~BackgroundAnimation();

    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual void VUpdate() override;

    u8** field_E4_res = nullptr;
    s16 field_E8_xpos = 0;
    s16 field_EA_ypos = 0;
    s16 field_EC_w = 0;
    s16 field_EE_h = 0;
    s32 mTlvInfo = 0;
    FP field_F8_animXPos = {};
    FP field_FC_animYPos = {};
    relive::Path_BackgroundAnimation::BgAnimSounds mSoundEffect = relive::Path_BackgroundAnimation::BgAnimSounds::eNone_0;
    s32 field_104_sound_channels_mask = 0;
};
ALIVE_ASSERT_SIZEOF(BackgroundAnimation, 0x108);

} // namespace AO
