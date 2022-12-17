#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/FixedPoint.hpp"

struct SlapLockWhirlWindSaveState final
{
    ReliveTypes mType;
    s16 mSwitchId;
};

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
    Guid mOrbWhirlWindId;
    s16 mState = 0;
    s32 mTimer = 0;
    s16 mSwitchId = 0;
};
