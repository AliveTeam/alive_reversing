#pragma once

#include "BaseAliveGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp" // reliveChoice only

class MovingBomb final : public BaseAliveGameObject
{
public:
    MovingBomb(relive::Path_MovingBomb* pTlv, const Guid& tlvId);
    ~MovingBomb();
    
    virtual void VUpdate() override;
    virtual void VRender(BasePrimitive** ppOt) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VScreenChanged() override;

private:
    void BlowUp();
    void FollowLine();
    s16 HitObject();

private:
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
    s16 mStartMovingSwitchId = 0;
    s16 mMinStopTime = 0;
    s16 mMaxStopTime = 0;
    s32 mChannelMask = 0;
    relive::reliveChoice mPersistOffscreen = relive::reliveChoice::eNo;
};
