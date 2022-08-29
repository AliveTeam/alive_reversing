#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

class Path_MovingBomb final : public Path_TLV
{
public:
    s16 mSpeed;
    s16 mStartMovingSwitchId;
    Choice_short mTriggeredByAlarm;
    Scale_short mScale;
    s16 field_20_max_rise; // seems unused
    s16 mDisabledResources;
    s16 mStartSpeed;
    Choice_short mPersistOffscreen;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MovingBomb, 0x28);

struct Path_MovingBombStopper final : public Path_TLV
{
    s16 mMinDelay;
    s16 mMaxDelay;
};
// TODO: size

class MovingBomb final : public BaseAliveGameObject
{
public:
    MovingBomb(relive::Path_MovingBomb* pTlv, const Guid& tlvId);
    ~MovingBomb();

    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VRender(PrimHeader** ppOt) override;
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
    States field_10C_state = States::eTriggeredByAlarm_0;
    Guid field_110_tlvInfo;
    s32 field_114_timer = 0;
    FP field_118_speed = {};
    u16 field_11C_switch_id = 0;
    s16 field_11E_min = 0;
    s16 field_120_max = 0;
    s32 field_124_sound_channels = 0;
    s16 field_128_disabled_resources = 0;
    relive::reliveChoice field_12A_persist_offscreen = relive::reliveChoice::eNo;
};
ALIVE_ASSERT_SIZEOF(MovingBomb, 0x12C);

} // namespace AO
