#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "FixedPoint.hpp"

struct SlapLockWhirlWind_State final
{
    AETypes mType;
    s16 mSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SlapLockWhirlWind_State, 0x4);

class SlapLockWhirlWind final : public BaseGameObject
{
public:
    SlapLockWhirlWind(s16 doorNumber, s16 switchId, FP xpos, FP ypos, FP scale);
    
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    static s32 CreateFromSaveState(const u8* pBuffer);

    s16 SwitchId() const;

private:
    FP mDoorX = {};
    FP mDoorY = {};
    FP mDoorSpriteScale = {};
    s32 mOrbWhirlWindId = 0;
    s16 mState = 0;
    s32 mTimer = 0;
    s16 mSwitchId = 0;
};
ALIVE_ASSERT_SIZEOF(SlapLockWhirlWind, 0x48);
