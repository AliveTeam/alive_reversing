#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

struct Path_MovingBomb final : public Path_TLV
{
    u16 mSpeed;
    s16 mStartMovingSwitchId;
    Choice_short mTriggeredByAlarm;
    Scale_short mScale;
    s16 field_18_max_rise; // seems unused
    s16 mDisabledResources;
    u16 mStartSpeed;
    Choice_short mPersistOffscreen;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MovingBomb, 0x20);

struct Path_MovingBombStopper final : public Path_TLV
{
    s16 mMinDelay;
    s16 mMaxDelay;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MovingBombStopper, 0x14);

class MovingBomb final : public BaseAliveGameObject
{
public:
    MovingBomb(Path_MovingBomb* pTlv, s32 tlvInfo);
    ~MovingBomb();
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
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
    States field_118_state = States::eTriggeredByAlarm_0;
    s32 field_11C_tlvInfo = 0;
    s32 field_120_timer = 0;
    FP field_124_speed = {};
    s16 field_128_start_moving_switch_id = 0;
    s16 field_12A_min = 0;
    s16 field_12C_max = 0;
    s32 field_130_sound_channels = 0;
    s16 field_134_disabled_resources = 0;
    Choice_short field_136_persist_offscreen = Choice_short::eNo_0;
};
ALIVE_ASSERT_SIZEOF(MovingBomb, 0x138);
