#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"

namespace AO {

struct Path_TimerTrigger final : public Path_TLV
{
    s16 field_18_source_switch_id;
    u16 field_1A_trigger_interval;
    s16 field_1C_id1;
    s16 field_1C_id2;
    s16 field_1C_id3;
    s16 field_1C_id4;
};
ALIVE_ASSERT_SIZEOF(Path_TimerTrigger, 0x24);

class TimerTrigger final : public BaseGameObject
{
public:
    TimerTrigger(Path_TimerTrigger* pTlv, s32 tlvInfo);
    ~TimerTrigger();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_479DB0();

    virtual void VUpdate() override;

    void ToggleAllIds();
    EXPORT void VUpdate_479C40();

    u16 field_10_source_switch_id;

    enum class State : s16
    {
        eWaitForEnabled_0 = 0,
        eWaitForFirstTrigger_1 = 1,
        eCheckForStartAgain_2 = 2,
        eWaitForSecondTrigger_3 = 3,
    };
    State field_12_state;
    s16 field_14_ids[4];
    s32 field_1C_tlvInfo;
    s32 field_20_trigger_interval_timer;
    s32 field_24_trigger_interval;
    s16 field_28_starting_switch_state;
    s16 field_2A_pad;
};
ALIVE_ASSERT_SIZEOF(TimerTrigger, 0x2C);


} // namespace AO
