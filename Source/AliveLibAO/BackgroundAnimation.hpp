#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "FixedPoint.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

class BackgroundAnimation final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BackgroundAnimation(relive::Path_BackgroundAnimation* pTlv, const Guid& tlvId);
    ~BackgroundAnimation();

    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual void VUpdate() override;

    //u8** field_E4_res = nullptr;
    s16 field_E8_xpos = 0;
    s16 field_EA_ypos = 0;
    s16 field_EC_w = 0;
    s16 field_EE_h = 0;
    Guid mTlvInfo;
    FP field_F8_animXPos = {};
    FP field_FC_animYPos = {};
    relive::Path_BackgroundAnimation::BgAnimSounds mSoundEffect = relive::Path_BackgroundAnimation::BgAnimSounds::eNone;
    s32 field_104_sound_channels_mask = 0;
};
ALIVE_ASSERT_SIZEOF(BackgroundAnimation, 0x108);

} // namespace AO
