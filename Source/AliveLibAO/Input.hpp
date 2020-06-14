#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

struct PSX_Pad
{
    unsigned __int16 field_0_pressed;
    unsigned __int8 field_2;
    char field_3;
    __int16 field_4;
    unsigned __int16 field_6_held;
    int field_8;
};
ALIVE_ASSERT_SIZEOF(PSX_Pad, 0xC);

class InputObject
{
public:
    EXPORT void InitPad_4331A0(unsigned int padCount);

    EXPORT void Update_433250();

    static EXPORT void CC Shutdown_433230();

    PSX_Pad field_0_pads[2];
    BYTE** field_18_demo_res;
    int field_1C_demo_command_index;
    __int16 field_20_demo_playing;
    __int16 field_22;
    int field_24_command;
    int field_28_command_duration;
    int field_2C;
};
ALIVE_ASSERT_SIZEOF(InputObject, 0x30);

END_NS_AO
