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

// TODO: most of these are gonna be wrong bar the ones in abe.cpp
enum InputCommands : unsigned int
{
    eGameSpeak3 = 1u << 0,  // 0x1
    eSneak = 1u << 1,  // 0x2
    eGameSpeak6 = 1u << 2,  // 0x4
    eRun = 1u << 3,  // 0x8
    eHop = 1u << 4,  // 0x10
    eThrowItem = 1u << 5,  // 0x20
    eFartOrRoll = 1u << 6, // 0x40
    eDoAction  = 1u << 7,  // 0x80
    eGameSpeak4 = 1u << 8,  // 0x100
    eGameSpeak5 = 1u << 9,  // 0x200
    eGameSpeak1 = 1u << 10, // 0x400
    eGameSpeak2 = 1u << 11, // 0x800
    eUp = 1u << 12, // 0x1000
    eRight = 1u << 13, // 0x2000
    eDown = 1u << 14, // 0x4000
    eLeft = 1u << 15, // 0x8000
    eGameSpeak7 = 1u << 16, // 0x10000
    eGameSpeak8 = 1u << 17, // 0x20000
    eChant = 1u << 18, // 0x40000
    ePause = 1u << 19, // Or enter
    eUnPause_OrConfirm = 1u << 20, // Or/and back
    eBack = 1u << 21, // 0x200000
    eCheatMode = 1u << 22,
    eSpeak1 = 1u << 23, // 0x800000
    eSpeak2 = 1u << 24, // 0x1000000    = nothing
    // 0x2000000    = nothing
    // 0x4000000    = nothing
    // 0x8000000    = nothing
    // 0x10000000   = nothing
    // 0x20000000   = nothing
    // 0x40000000   = nothing
    e0x80000000 = 1u << 31,
};

EXPORT BOOL CC Input_IsChanting_4334C0();

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

    bool isPressed(DWORD command);

    bool isHeld(DWORD command);
};
ALIVE_ASSERT_SIZEOF(InputObject, 0x30);

ALIVE_VAR_EXTERN(InputObject, sInputObject_5009E8);
ALIVE_VAR_EXTERN(unsigned __int16, sCurrentControllerIndex_5076B8);


END_NS_AO
