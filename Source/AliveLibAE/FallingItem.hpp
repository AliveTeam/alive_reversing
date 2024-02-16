#pragma once

#include "BaseAliveGameObject.hpp"

namespace relive
{
    struct Path_FallingItem;
}

class FallingItem final : public BaseAliveGameObject
{
public:
    FallingItem(relive::Path_FallingItem* pTlv, const Guid& tlvId);
    FallingItem(s32 xpos, s32 ypos, s32 scale, s32 id, s32 fallInterval, s32 numItems, s32 bResetIdAfterUse);
    ~FallingItem();

    void LoadAnimations();
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VOnThrowableHit(BaseGameObject*) override
    {
        // FallingItem__vnullsub_75
    }

private:
    void DamageHitItems();

private:
    enum class State : s16
    {
        eWaitForIdEnable_0 = 0,
        eGoWaitForDelay_1 = 1,
        eWaitForFallDelay_2 = 2,
        eFalling_3 = 3,
        eSmashed_4 = 4
    };

public:
    Guid mTlvId;
    State mState = State::eWaitForIdEnable_0;
    s16 mSwitchId = 0;
    s16 mFallInterval = 0;
    s16 mMaxFallingItems = 0;
    s16 mRemainingFallingItems = 0;
    bool mResetSwitchIdAfterUse = false;
    FP mTlvXPos = {};
    FP mTlvYPos = {};
    FP mStartYPos = {};
    s32 mFallIntervalTimer = 0;
    bool mDoAirStreamSound = true;
    bool mHitDrillOrMineCar = false;
    s32 mAirStreamSndChannels = 0;
    s32 mCreatedGnFrame = 0;
};
