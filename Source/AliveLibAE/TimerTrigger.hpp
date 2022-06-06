#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

struct Path_TimerTrigger final : public Path_TLV
{
    s16 field_10_input_switch_id;
    u16 field_12_trigger_interval;
    s16 field_14_output_switch_id1;
    s16 field_16_output_switch_id2;
    s16 field_18_output_switch_id3;
    s16 field_1A_output_switch_id4;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TimerTrigger, 0x1C);

enum class TimerTriggerStates : s16
{
    eWaitForEnabled_0 = 0,
    eWaitForFirstTrigger_1 = 1,
    eCheckForStartAgain_2 = 2,
    eWaitForSecondTrigger_3 = 3,
};

struct TimerTrigger_State final
{
    AETypes field_0_type;
    s32 field_4_tlvInfo;
    s32 field_8_delay_timer_base;
    TimerTriggerStates field_C_state;
    s16 field_E_starting_switch_state;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(TimerTrigger_State, 0x10);

class TimerTrigger final : public BaseGameObject
{
public:
    TimerTrigger(Path_TimerTrigger* pTlv, s32 tlvInfo);
    ~TimerTrigger();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pData);

private:
    void ToggleAllIds();

private:
    s16 field_20_input_switch_id;
    TimerTriggerStates field_22_state;
    s16 field_24_output_switch_ids[4];
    s32 field_2C_tlvInfo;
    s32 field_30_trigger_interval_timer;
    s32 field_34_trigger_interval;
    s16 field_38_starting_switch_state;
};
ALIVE_ASSERT_SIZEOF(TimerTrigger, 0x3C);
