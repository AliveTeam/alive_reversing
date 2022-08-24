#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

class FireBackgroundGlow;
class FlameSparks;

struct Path_DoorFlame final : public Path_TLV
{
    s16 mSwitchId;
    enum class Scale : s16
    {
        eFull_0 = 0,
        eHalf_1 = 1,
        eHalf_2 = 2
    };
    Scale mScale;

    enum class Colour : s16
    {
        default_0 = 0,
        red_1 = 1,
        green_2 = 2,
        blue_3 = 3
    };
    Colour mColour;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_DoorFlame, 0x20);

class DoorFlame final : public BaseAnimatedWithPhysicsGameObject
{
public:
    DoorFlame(relive::Path_DoorFlame* pTlv, s32 tlvInfo);
    ~DoorFlame();

    virtual void VStopAudio() override;
    virtual void VUpdate() override;

    s32 mTlvInfo = 0;
    s16 mSwitchId = 0;
    s16 mFrameCount = 0;
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
ALIVE_ASSERT_SIZEOF(DoorFlame, 0x100);

} // namespace AO
