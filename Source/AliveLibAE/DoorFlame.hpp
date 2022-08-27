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
    DoorFlame(relive::Path_DoorFlame* pTlv, s32 tlvInfo);
    ~DoorFlame();

    virtual void VUpdate() override;
    virtual void VStopAudio() override;
    virtual void VScreenChanged() override;

private:
    s32 mTlvInfo = 0;
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
    s32 mFireBackgroundGlowId = 0;
    s32 mFlameSparksId = 0;
};
ALIVE_ASSERT_SIZEOF(DoorFlame, 0x110);
