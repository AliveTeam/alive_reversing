#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "SwitchStates.hpp"

struct Path_MultiSwitchController final : public Path_TLV
{
    s16 mOutputSwitchId;
    SwitchOp mAction;
    u16 mOnOffDelay;
    u16 mInputSwitchId1;
    u16 mInputSwitchId2;
    u16 mInputSwitchId3;
    u16 mInputSwitchId4;
    u16 mInputSwitchId5;
    u16 mInputSwitchId6;
    s16 padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MultiSwitchController, 0x24);

class MultiSwitchController final : public BaseGameObject
{
public:
    MultiSwitchController(relive::Path_MultiSwitchController* pTlv, const TLVUniqueId& tlvId);
    ~MultiSwitchController();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    void WaitingForAllOn();
    void WaitingForAllOff();

private:
    u16 mInputSwitchId1 = 0;
    u16 mInputSwitchId2 = 0;
    u16 mInputSwitchId3 = 0;
    u16 mInputSwitchId4 = 0;
    u16 mInputSwitchId5 = 0;
    u16 mInputSwitchId6 = 0;
    u16 mOutputSwitchId = 0;
    relive::reliveSwitchOp mAction = relive::reliveSwitchOp::eSetTrue;
    s32 mTlvInfo = 0;
    s32 field_34_last_switch_on_time = 0;
    s32 field_38_all_switches_on_or_off_time = 0;
    s32 mOnOffDelay = 0;
    enum Flags_40
    {
        eBit1_is_on = 0x1,
    };
    BitField16<Flags_40> field_40_flags = {};
};
ALIVE_ASSERT_SIZEOF(MultiSwitchController, 0x44);
