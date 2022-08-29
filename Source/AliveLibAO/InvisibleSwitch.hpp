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
    s16 mSwitchId;
    SwitchOp mAction;
    u16 mActivationDelay;
    Choice_short mSetOffAlarm;
    InvisibleSwitchScale mScale;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_InvisibleSwitch, 0x24);

class InvisibleSwitch final : public BaseGameObject
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
ALIVE_ASSERT_SIZEOF(InvisibleSwitch, 0x30);


} // namespace AO
