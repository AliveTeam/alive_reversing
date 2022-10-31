#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive
{
    struct Path_DoorFlame;
}

class DoorFlame final : public BaseAnimatedWithPhysicsGameObject
{
public:
    DoorFlame(relive::Path_DoorFlame* pTlv, const Guid& tlvId);
    ~DoorFlame();

    virtual void VUpdate() override;
    virtual void VStopAudio() override;
    virtual void VScreenChanged() override;

private:
    Guid mTlvInfo;
    s16 mSwitchId = 0;
    s16 mFrameCount = 0;
    enum class States : s16
    {
        eDisabled_0 = 0,
        eEnabled_1 = 1,
    };
    States mState = States::eDisabled_0;
    s16 mRandom = 0;
    s32 mSoundsMask = 0;
    Guid mFireBackgroundGlowId;
    Guid mFlameSparksId;
};
