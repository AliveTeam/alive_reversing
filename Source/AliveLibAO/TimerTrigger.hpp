#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseGameObject.hpp"

namespace AO {

struct Path_TimerTrigger final : public Path_TLV
{
    s16 field_18_input_switch_id;
    u16 field_1A_trigger_interval;
    s16 field_1C_output_switch_id1;
    s16 field_1C_output_switch_id2;
    s16 field_1C_output_switch_id3;
    s16 field_1C_output_switch_id4;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TimerTrigger, 0x24);

class TimerTrigger final : public BaseGameObject
{
public:
    TimerTrigger(Path_TimerTrigger* pTlv, s32 tlvInfo);
    ~TimerTrigger();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    void ToggleAllIds();

    u16 field_10_input_switch_id = 0;

    enum class State : s16
    {
        eWaitForEnabled_0 = 0,
        eWaitForFirstTrigger_1 = 1,
        eCheckForStartAgain_2 = 2,
        eWaitForSecondTrigger_3 = 3,
    };
    State field_12_state = State::eWaitForEnabled_0;
    s16 field_14_ids[4] = {};
    s32 field_1C_tlvInfo = 0;
    s32 field_20_trigger_interval_timer = 0;
    s32 field_24_trigger_interval = 0;
    s16 field_28_starting_switch_state = 0;
};
ALIVE_ASSERT_SIZEOF(TimerTrigger, 0x2C);


} // namespace AO
