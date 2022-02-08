#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

enum class SwitchOp : s16;

struct Path_MultiSwitchController final : public Path_TLV
{
    s16 field_10_output_switch_id;
    SwitchOp field_12_action;
    u16 field_14_on_off_delay;
    u16 field_16_input_switch_id1;
    u16 field_18_input_switch_id2;
    u16 field_1A_input_switch_id3;
    u16 field_1C_input_switch_id4;
    u16 field_1E_input_switch_id5;
    u16 field_20_input_switch_id6;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MultiSwitchController, 0x24);

class MultiSwitchController final : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    EXPORT MultiSwitchController* ctor_4D60E0(Path_MultiSwitchController* pTlv, u32 tlvInfo);

private:
    EXPORT MultiSwitchController* vdtor_4D61A0(s32 flags);
    EXPORT void dtor_4D61D0();
    EXPORT void vScreenChanged_4D6860();
    EXPORT void vUpdate_4D6250();
    EXPORT void WaitingForAllOn_4D6290();
    EXPORT void WaitingForAllOff_4D65B0();

private:
    u16 field_20_input_switch_id1;
    u16 field_22_input_switch_id2;
    u16 field_24_input_switch_id3;
    u16 field_26_input_switch_id4;
    u16 field_28_input_switch_id5;
    u16 field_2A_input_switch_id6;
    u16 field_2C_output_switch_id;
    SwitchOp field_2E_action;
    s32 field_30_tlvInfo;
    s32 field_34_last_switch_on_time;
    s32 field_38_all_switches_on_or_off_time;
    s32 field_3C_on_off_delay;
    enum Flags_40
    {
        eBit1_is_on = 0x1,
    };
    BitField16<Flags_40> field_40_flags;
    //s16 field_42; // pad
};
ALIVE_ASSERT_SIZEOF(MultiSwitchController, 0x44);
