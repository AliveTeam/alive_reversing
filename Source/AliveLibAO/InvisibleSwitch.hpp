#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "SwitchStates.hpp"
#include "../relive_lib/BaseGameObject.hpp"

namespace AO {

enum class SwitchOp : s16;

class InvisibleSwitch final : public ::BaseGameObject
{
public:
    InvisibleSwitch(relive::Path_InvisibleSwitch* pTlv, const Guid& tlvId);
    ~InvisibleSwitch();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s16 field_10_switch_id = 0;
    relive::reliveSwitchOp field_12_action = relive::reliveSwitchOp::eSetTrue;
    Guid field_14_tlvInfo;
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
    relive::reliveChoice mSetOffAlarm = relive::reliveChoice::eNo;
    relive::Path_InvisibleSwitch::InvisibleSwitchScale field_2C_scale = relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull;
};


} // namespace AO
