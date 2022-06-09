#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Path.hpp"
#include "Psx.hpp"
#include "SwitchStates.hpp"

enum class InvisibleSwitchScale : s16
{
    eHalf_0 = 0,
    eFull_1 = 1,
    eAny_2 = 2,
};

struct Path_InvisibleSwitch final : public Path_TLV
{
    s16 field_10_switch_id;
    SwitchOp field_12_action;
    s16 field_14_delay;
    Choice_short field_16_set_off_alarm;
    InvisibleSwitchScale field_18_scale;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_InvisibleSwitch, 0x1C);

class InvisibleSwitch final : public BaseGameObject
{
public:
    InvisibleSwitch(Path_InvisibleSwitch* pTlv, u32 tlvInfo);
    ~InvisibleSwitch();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s16 field_20_switch_id = 0;
    SwitchOp field_22_action = SwitchOp::eSetTrue_0;
    s32 field_24_tlvInfo = 0;
    s32 field_28_delay_timer = 0;
    s32 field_2C_delay = 0;
    PSX_Point field_30_top_left = {};
    PSX_Point field_34_bottom_right = {};
    enum class States : s16
    {
        eWaitForTrigger_0 = 0,
        eWaitForDelayTimer_1 = 1,
    };
    States field_38_state = States::eWaitForTrigger_0;
    Choice_short field_3A_set_off_alarm = Choice_short::eNo_0;
    InvisibleSwitchScale field_3C_scale = InvisibleSwitchScale::eHalf_0;
};
ALIVE_ASSERT_SIZEOF(InvisibleSwitch, 0x40);
