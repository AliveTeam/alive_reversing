#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_MeatSaw final : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchMinTimeOff;
    s16 mSwitchMaxTimeOff;
    s16 mMaxRiseTime;
    s16 mSwitchId;
    enum class Type : s16
    {
        eAutomaticPersistOffscreen_0 = 0,
        eAutomatic_1 = 1,
        eSwitchId_2 = 2
    };
    Type mType;
    s16 mSpeed;
    enum class StartState : s16
    {
        eOff_0 = 0,
        eOn_1 = 1
    };
    StartState mStartState;
    s16 mOffSpeed;
    s16 mAutomaticMinTimeOff;
    s16 mAutomaticMaxTimeOff;
    s16 mInitialPosition;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MeatSaw, 0x30);

enum class MeatSawStates : s16
{
    eIdle_0 = 0,
    eGoingDown_1 = 1,
    eGoingUp_2 = 2
};

class MeatSaw final : public BaseAnimatedWithPhysicsGameObject
{
public:
    MeatSaw(Path_MeatSaw* pTlv, s32 tlvInfo);
    ~MeatSaw();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void GrindUpObjects_439CD0();

    MeatSawStates field_E4_state = MeatSawStates::eIdle_0;
    u16 field_E6_max_rise_time = 0;
    s16 field_E8_speed2 = 0;
    s16 field_EA_speed1 = 0;
    s16 field_EC_off_speed = 0;
    u16 field_EE_switch_id = 0;
    s16 field_F0_switch_value = 0;
    s16 field_F2_switch_value = 0;
    s16 field_F4 = 0;
    s16 field_F6_switch_min_time_off = 0;
    s16 field_F8_switch_max_time_off = 0;
    s16 field_FA_automatic_min_time_off = 0;
    s16 field_FC_automatic_max_time_off = 0;
    s32 field_100_tlvInfo = 0;
    s32 field_104_idle_timer = 0;
    s32 field_108_SFX_timer = 0;
    s32 field_10C_FrameCount = 0;
    Animation field_110_anim;

    enum flags_1A8
    {
        eBit1_ResetOffscreen = 0x1,
        eBit2_SwitchIdMeatSaw = 0x2,
        eBit3_AutomaticMeatSawIsDown = 0x4
    };
    BitField16<flags_1A8> field_1A8_flags = {};
};
ALIVE_ASSERT_SIZEOF(MeatSaw, 0x1AC);


} // namespace AO
