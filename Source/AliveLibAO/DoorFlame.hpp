#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive
{
    struct Path_DoorFlame;
}

namespace AO {

class FireBackgroundGlow;
class FlameSparks;

class DoorFlame final : public BaseAnimatedWithPhysicsGameObject
{
public:
    DoorFlame(relive::Path_DoorFlame* pTlv, const Guid& tlvId);
    ~DoorFlame();

    virtual void VUpdate() override;
    virtual void VStopAudio() override;

private:
    Guid mTlvInfo;
    s16 mSwitchId = 0;
    u32 mFrameCount = 0;
    enum class States : u16
    {
        eDisabled_0 = 0,
        eEnabled_1 = 1
    };
    States mState = States::eDisabled_0;
    s16 mRandom = 0;
    s32 mSoundsMask = 0;
    FireBackgroundGlow* mFireBackgroundGlow = nullptr;
    FlameSparks* mFlameSparks = nullptr;
};

} // namespace AO
