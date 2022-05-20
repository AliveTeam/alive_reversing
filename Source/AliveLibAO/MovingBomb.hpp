#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

class Path_MovingBomb final : public Path_TLV
{
public:
    s16 field_18_speed;
    s16 field_1A_switch_id;
    Choice_short field_1C_bTriggered_by_alarm;
    Scale_short field_1E_scale;
    s16 field_20_max_rise;
    s16 field_22_disabled_resources;
    s16 field_24_start_speed;
    Choice_short field_26_persist_offscreen;
};
ALIVE_ASSERT_SIZEOF(Path_MovingBomb, 0x28);

struct Path_MovingBombStopper final : public Path_TLV
{
    s16 field_18_min_delay;
    s16 field_1A_max_delay;
};

class MovingBomb final : public BaseAliveGameObject
{
public:
    EXPORT MovingBomb* ctor_43AFE0(Path_MovingBomb* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_43B2C0();

    

    EXPORT BaseGameObject* Vdtor_43BCC0(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_43BC90();

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT s16 VTakeDamage_43BB60(BaseGameObject* pFrom);

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_43B910(PrimHeader** ppOt);

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    EXPORT void VOnThrowableHit_43B930(BaseGameObject* pFrom);

    EXPORT s16 HitObject_43B970();

    EXPORT void FollowLine_43BA40();

    virtual void VUpdate() override;

    EXPORT void VUpdate_43B440();

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
    States field_10C_state;
    s16 field_10E_pad;
    s32 field_110_tlvInfo;
    s32 field_114_timer;
    FP field_118_speed;
    u16 field_11C_switch_id;
    s16 field_11E_max;
    s16 field_120_min;
    s16 field_122_pad;
    s32 field_124_sound_channels;
    s16 field_128_disabled_resources;
    Choice_short field_12A_persist_offscreen;
};
ALIVE_ASSERT_SIZEOF(MovingBomb, 0x12C);

} // namespace AO
