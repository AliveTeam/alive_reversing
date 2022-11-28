#pragma once

#include "BaseAliveGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

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

} // namespace AO
