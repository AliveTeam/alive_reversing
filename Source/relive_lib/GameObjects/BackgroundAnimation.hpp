#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../data_conversion/relive_tlvs.hpp"

class BackgroundAnimation final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BackgroundAnimation(relive::Path_BackgroundAnimation* pTlv, const Guid& tlvId);
    ~BackgroundAnimation();

    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual void VUpdate() override;

private:
    Guid mTlvId;
    FP mObjectXPos = {};
    FP mObjectYPos = {};
    relive::Path_BackgroundAnimation::BgAnimSounds mSoundEffect = relive::Path_BackgroundAnimation::BgAnimSounds::eNone;
    s32 mSoundChannelsMask = 0;
};
