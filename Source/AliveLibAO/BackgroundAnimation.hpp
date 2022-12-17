#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

class BackgroundAnimation final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BackgroundAnimation(relive::Path_BackgroundAnimation* pTlv, const Guid& tlvId);
    ~BackgroundAnimation();

    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual void VUpdate() override;

private:
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

} // namespace AO
