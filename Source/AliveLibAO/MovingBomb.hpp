#pragma once

#include "BaseAliveGameObject.hpp"

namespace relive {
struct Path_MovingBomb;
}

namespace AO {

class MovingBomb final : public BaseAliveGameObject
{
public:
    MovingBomb(relive::Path_MovingBomb* pTlv, const Guid& tlvId);
    ~MovingBomb();

    virtual void VScreenChanged() override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VUpdate() override;

    s16 HitObject();
    void FollowLine();

    enum class States : s16
    {
        eTriggeredByAlarm_0 = 0,
        eTriggeredBySwitch_1 = 1,
        eMoving_2 = 2,
        eStopMoving_3 = 3,
        eWaitABit_4 = 4,
        eToMoving_5 = 5,
        eBlowingUp_6 = 6,
        eKillMovingBomb_7 = 7
    };
    States mState = States::eTriggeredByAlarm_0;
    Guid mTlvId;
    s32 mTimer = 0;
    FP mSpeed = {};
    u16 mStartMovingSwitchId = 0;
    s16 mMinStopTime = 0;
    s16 mMaxStopTime = 0;
    s32 mChannelMask = 0;
    bool mPersistOffscreen = false;
};

} // namespace AO
