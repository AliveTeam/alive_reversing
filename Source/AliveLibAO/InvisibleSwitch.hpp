#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"
#include "SwitchStates.hpp"

namespace AO {

enum class SwitchOp : s16;

enum class InvisibleSwitchScale : s16
{
    eHalf_0 = 0,
    eFull_1 = 1,
    eAny_2 = 2,
};

struct Path_InvisibleSwitch final : public Path_TLV
{
    s16 field_18_switch_id;
    SwitchOp field_1A_action;
    u16 field_1C_delay;
    Choice_short field_1E_set_off_alarm;
    InvisibleSwitchScale field_20_scale;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_InvisibleSwitch, 0x24);

class InvisibleSwitch final : public BaseGameObject
{
public:
    InvisibleSwitch(Path_InvisibleSwitch* pTlv, s32 tlvInfo);
    ~InvisibleSwitch();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s16 field_10_switch_id = 0;
    SwitchOp field_12_action = SwitchOp::eSetTrue_0;
    s32 field_14_tlvInfo = 0;
    s32 field_18_delay_timer = 0;
    s32 field_1C_delay = 0;
    PSX_Point field_20_top_left = {};
    PSX_Point field_24_bottom_right = {};
    enum class States : u16
    {
        eWaitForTrigger_0 = 0,
        eWaitForDelayTimer_1 = 1,
    };
    States field_28_state = States::eWaitForTrigger_0;
    Choice_short field_2A_set_off_alarm = Choice_short::eNo_0;
    InvisibleSwitchScale field_2C_scale = InvisibleSwitchScale::eHalf_0;
};
ALIVE_ASSERT_SIZEOF(InvisibleSwitch, 0x30);


} // namespace AO
