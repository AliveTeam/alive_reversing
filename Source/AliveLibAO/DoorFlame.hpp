#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

class FireBackgroundGlow;
class FlameSparks;

class DoorFlame final : public BaseAnimatedWithPhysicsGameObject
{
public:
    DoorFlame(relive::Path_DoorFlame* pTlv, const Guid& tlvId);
    ~DoorFlame();

    virtual void VStopAudio() override;
    virtual void VUpdate() override;

    Guid mTlvInfo;
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
