#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

class Path_MovingBomb : public Path_TLV
{
public:
    s16 field_18_speed;
    s16 field_1A_id;
    s16 field_1C_bStart_type_triggered_by_alarm;
    s16 field_1E_scale;
    s16 field_20_max_rise;
    s16 field_22_disabled_resources;
    s16 field_24_start_speed;
    s16 field_26_persist_offscreen;
};
ALIVE_ASSERT_SIZEOF(Path_MovingBomb, 0x28);

struct Path_MovingBombStopper : public Path_TLV
{
    s16 field_18_min_delay;
    s16 field_1A_max_delay;
};

class MovingBomb : public BaseAliveGameObject
{
public:
    EXPORT MovingBomb* ctor_43AFE0(Path_MovingBomb* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_43B2C0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* Vdtor_43BCC0(signed int flags);

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
    int field_110_tlvInfo;
    int field_114_timer;
    FP field_118_speed;
    u16 field_11C_id;
    s16 field_11E_max;
    s16 field_120_min;
    s16 field_122_pad;
    int field_124_sound_channels;
    s16 field_128_disabled_resources;
    s16 field_12A_persist_offscreen;
};
ALIVE_ASSERT_SIZEOF(MovingBomb, 0x12C);

}
