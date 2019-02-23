#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "Psx.hpp"

enum class SwitchOp : __int16;

struct Path_InvisibleSwitch : public Path_TLV
{
    __int16 field_10_id;
    SwitchOp field_12_action;
    __int16 field_14_delay;
    __int16 field_16_set_off_alarm;
    __int16 field_18_scale;
    __int16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_InvisibleSwitch, 0x1C);

class InvisibleSwitch : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT InvisibleSwitch* ctor_45FA70(Path_InvisibleSwitch* pTlv, DWORD tlvInfo);
private:
    EXPORT void dtor_45FB20();
    EXPORT InvisibleSwitch* vdtor_45FAF0(signed int flags);
    EXPORT void vUpdate_45FBA0();
    EXPORT void vScreenChanged_45FD80();
private:
    __int16 field_20_id;
    SwitchOp field_22_action;
    int field_24_tlvInfo;
    int field_28_delay_timer;
    int field_2C_delay;
    PSX_Point field_30_top_left;
    PSX_Point field_34_bottom_right;
    enum class States : __int16
    {
        eState_WaitForTrigger_0 = 0,
        eState_WaitForDelayTimer_1 = 1,
    };
    States field_38_state;
    __int16 field_3A_set_off_alarm;
    __int16 field_3C_scale;
    __int16 field_3E_pad;
};
ALIVE_ASSERT_SIZEOF(InvisibleSwitch, 0x40);
