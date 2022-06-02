#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

enum class WheelStates : s16
{
    eIdle_0 = 0,
    eTurning_1 = 1,
};


struct WorkWheel_SaveState final
{
    AETypes field_0_id;
    // pad
    s32 field_4_tlvInfo;
    s16 field_8_snd_counter;
    // pad
    WheelStates field_C_state;
    s16 field_E_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(WorkWheel_SaveState, 0x10);

struct Path_WorkWheel final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_switch_id;
    s16 field_14_activation_time;
    s16 field_16_off_time;
    Choice_short field_18_turn_off_when_stopped;
    s16 field_20_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WorkWheel, 0x1C);

class WorkWheel final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual void VStartTurning();
    virtual void VStopTurning(s16 bResetSwitch);

    WorkWheel(Path_WorkWheel* pTlv, s32 tlvInfo);
    ~WorkWheel();

public:
    static s32 CreateFromSaveState(const u8* pState);

private:
    s32 field_F4_tlv_info;
    s16 field_F8_switch_id;
    s16 field_FA_padding;
    WheelStates field_FC_state;
    s16 field_FE_activation_time;
    u16 field_100_on_counter;
    s16 field_102_off_time;
    Choice_short field_104_turn_off_when_stopped;
};
ALIVE_ASSERT_SIZEOF(WorkWheel, 0x108);
