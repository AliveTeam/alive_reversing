#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Animation.hpp"

namespace relive
{
    struct Path_Hoist;
}

struct HoistRockParticle final
{
    s16 mState;
    FP mXPos;
    FP mYPos;
    FP mVelY;
    Animation mAnim;
};

class HoistRocksEffect final : public BaseGameObject
{
public:
    HoistRocksEffect(relive::Path_Hoist* pTlv, const Guid& tlvId);
    ~HoistRocksEffect();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    void LoadAnimations();

private:
    s16 mTlvXPos = 0;
    s16 mTlvYPos = 0;
    Guid mTlvId;
    s32 mTimer = 0;
    FP mSpriteScale = {};
    HoistRockParticle mRocks[4] = {};
};
