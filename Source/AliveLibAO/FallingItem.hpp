#pragma once

#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include "../relive_lib/AnimResources.hpp"

namespace relive {
struct Path_FallingItem;
}

namespace AO {

struct FallingItem_Data final
{
    AnimId mFallingAnimId;
    AnimId mWaitingAnimId;
};

class FallingItem final : public ::BaseAliveGameObject
{
public:
    FallingItem(relive::Path_FallingItem* pTlv, const Guid& tlvId);
    ~FallingItem();

    void LoadAnimations();
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

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
    u16 mSwitchId = 0;
    s16 mFallInterval = 0;
    s16 mMaxFallingItems = 0;
    s16 mRemainingFallingItems = 0;
    bool mResetSwitchIdAfterUse = false;
    FP mTlvXPos = {};
    FP mTlvYPos = {};
    FP mStartYPos = {};
    s32 mFallIntervalTimer = 0;
    bool mDoAirStreamSound = false;
    s32 mAirStreamSndChannels = 0;
    s32 mCreatedGnFrame = 0;
};

} // namespace AO
