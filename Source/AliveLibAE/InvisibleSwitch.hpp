#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "Psx.hpp"

enum class SwitchOp : s16;

enum class InvisibleSwitchScale : s16
{
    eHalf_0 = 0,
    eFull_1 = 1,
    eAny_2 = 2,
};

struct Path_InvisibleSwitch final : public Path_TLV
{
    s16 field_10_id;
    SwitchOp field_12_action;
    s16 field_14_delay;
    Choice_short field_16_set_off_alarm;
    InvisibleSwitchScale field_18_scale;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_InvisibleSwitch, 0x1C);

class InvisibleSwitch : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT InvisibleSwitch* ctor_45FA70(Path_InvisibleSwitch* pTlv, u32 tlvInfo);

private:
    EXPORT void dtor_45FB20();
    EXPORT InvisibleSwitch* vdtor_45FAF0(s32 flags);
    EXPORT void vUpdate_45FBA0();
    EXPORT void vScreenChanged_45FD80();

private:
    s16 field_20_id;
    SwitchOp field_22_action;
    s32 field_24_tlvInfo;
    s32 field_28_delay_timer;
    s32 field_2C_delay;
    PSX_Point field_30_top_left;
    PSX_Point field_34_bottom_right;
    enum class States : s16
    {
        eState_WaitForTrigger_0 = 0,
        eState_WaitForDelayTimer_1 = 1,
    };
    States field_38_state;
    Choice_short field_3A_set_off_alarm;
    InvisibleSwitchScale field_3C_scale;
    s16 field_3E_pad;
};
ALIVE_ASSERT_SIZEOF(InvisibleSwitch, 0x40);
