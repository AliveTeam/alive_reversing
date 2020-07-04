#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"


START_NS_AO

struct Path_InvisibleSwitch : public Path_TLV
{
    __int16 field_18_id;
    __int16 field_1A_action;
    unsigned __int16 field_1C_delay;
    __int16 field_1E_set_off_alarm;
    __int16 field_20_scale;
    __int16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_InvisibleSwitch, 0x24);

class InvisibleSwitch : public BaseGameObject
{
public:
    EXPORT InvisibleSwitch* ctor_4334E0(Path_InvisibleSwitch* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_433540();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT InvisibleSwitch* Vdtor_433740(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_433700();

    __int16 field_10_id;
    __int16 field_12_action;
    int field_14_tlvInfo;
    int field_18_delay_timer;
    int field_1C_delay;
    PSX_Point field_20_top_left;
    PSX_Point field_24_bottom_right;
    unsigned __int16 field_28_state;
    __int16 field_2A_set_off_alarm;
    __int16 field_2C_scale;
    __int16 field_2E_pad;
};
ALIVE_ASSERT_SIZEOF(InvisibleSwitch, 0x30);


END_NS_AO

