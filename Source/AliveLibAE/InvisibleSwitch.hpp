#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Psx.hpp"
#include "../relive_lib/SwitchStates.hpp"

class InvisibleSwitch final : public BaseGameObject
{
public:
    InvisibleSwitch(relive::Path_InvisibleSwitch* pTlv, const Guid& tlvId);
    ~InvisibleSwitch();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s16 field_20_switch_id = 0;
    relive::reliveSwitchOp field_22_action = relive::reliveSwitchOp::eSetTrue;
    Guid field_24_tlvInfo;
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
    relive::reliveChoice field_3A_set_off_alarm = relive::reliveChoice::eNo;
    relive::Path_InvisibleSwitch::InvisibleSwitchScale field_3C_scale = relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf;
};
