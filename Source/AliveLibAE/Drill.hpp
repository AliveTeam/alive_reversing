#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "BitField.hpp"
#include "FunctionFwd.hpp"

enum class DrillDirection : s16
{
    eDown_0 = 0,
    eRight_1 = 1,
    eLeft_2 = 2,
};

enum class DrillBehavior : s16
{
    eNotInteractable_0 = 0,
    eToggle_1 = 1,
    eUse_2 = 2
};

struct Path_Drill_Data final
{
    Scale_short field_10_scale;
    s16 field_12_min_off_time;
    s16 field_14_max_off_time;
    s16 field_16_id;
    DrillBehavior field_18_behavior;
    s16 field_1A_speed;
    Choice_short field_1C_bStart_state_on;
    s16 field_1E_off_speed;
    s16 field_20_min_off_time_speed_change;
    s16 field_22_max_off_time_speed_change;
    Choice_short field_24_bStart_position_bottom;
    DrillDirection field_26_direction;
};
ALIVE_ASSERT_SIZEOF(Path_Drill_Data, 0x18);

enum class DrillStates : s16
{
    State_0_Restart_Cycle = 0,
    State_1_Going_Down = 1,
    State_2_GoingUp = 2,
};

struct Drill_State final
{
    s16 field_0;
    //s16 field_2_padding;
    s32 field_4;
    s32 field_8_tlvInfo;
    s32 field_C_off_timer;
    DrillStates field_10_state;
    s16 field_12_xyoff;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Drill_State, 0x14);


struct Path_Drill final : public Path_TLV
{
    Path_Drill_Data field_10_data;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Drill, 0x28);

class Drill final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Drill* ctor_4200D0(Path_Drill* pTlv, u32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    EXPORT static s32 CC CreateFromSaveState_421600(const u8* pData);

private:
    EXPORT void vUpdate_420C50();
    EXPORT void dtor_420B60();
    EXPORT Drill* vdtor_4206A0(s32 flags);
    EXPORT void vScreenChanged_4214B0();
    EXPORT void vRender_4213D0(PrimHeader** ppOt);
    EXPORT void vStopAudio_4215C0();
    EXPORT s32 vGetSaveState_4217B0(u8* /*pSaveBuffer*/);
    EXPORT void EmitSparks_4206D0();
    EXPORT s16 DamageTouchingObjects_421060();

private:
    DrillStates field_F4_state;
    s16 field_F6_width;
    s16 field_F8_id;
    DrillDirection field_FA_direction;
    s16 field_FC_min_off_time;
    s16 field_FE_max_off_time;
    s16 field_100_min_off_time_speed_change;
    s16 field_102_max_off_time_speed_change;
    u32 field_104_tlv;
    s32 field_108_off_timer;
    s32 field_10C_audio_channels_mask;
    FP field_110_xPos;
    FP field_114_yPos;
    FP field_118_speed;
    FP field_11C_speed2;
    FP field_120_off_speed;
    FP field_124_xyoff;
    enum Flags
    {
        eBit1_StartOff = 0x1,
        eBit2_ToggleStartState_StartOn = 0x2,
        eBit3_UseId = 0x4,
        eBit4_Toggle = 0x8,
        eBit5_SpeedChanged = 0x10,
        eBit6_StartPosIsBottom = 0x20,
    };
    BitField16<Flags> field_128_flags;
    s16 field_12A_pad;
};
ALIVE_ASSERT_SIZEOF(Drill, 0x12C);
