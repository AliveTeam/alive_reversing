#pragma once

#include "FunctionFwd.hpp"

typedef int(*t_InputCallback)();

EXPORT unsigned __int8 CC Input_GetInputEnabled_4EDDE0();
EXPORT void CC Input_EnableInput_4EDDD0();
EXPORT void CC Input_InitKeyStateArray_4EDD60();
EXPORT void CC Input_DisableInput_4EDDC0();
EXPORT void CC Input_Init_491BC0();
EXPORT void CC Input_SetKeyState_4EDD80(int key, char bIsDown);
EXPORT bool CC Input_IsVKPressed_4EDD40(int key);
EXPORT char Input_ReadKey_492610();
EXPORT void Input_Reset_492660();

enum InputCommands : unsigned int
{
    eUp =           1 << 0,
    eDown =         1 << 1,
    eLeft =         1 << 2,
    eRight =        1 << 3,
    eRun =          1 << 4,
    eDoAction =     1 << 5,  // Pick up rock, pull lever etc
    eSneak =        1 << 6,
    eThrowItem =    1 << 7,  // Or I say I dunno if no items
    eHop =          1 << 8,
    eFartOrRoll =   1 << 9,  // (Only roll in AO)
    eGameSpeak1 =   1 << 10, // Hello
    eGameSpeak2 =   1 << 11, // (Follow Me)
    eGameSpeak3 =   1 << 12, // Wait
    eGameSpeak4 =   1 << 13, // (Work) (Whistle 1)
    eGameSpeak5 =   1 << 14, // (Anger)
    eGameSpeak6 =   1 << 15, // (All ya) (Fart)
    eGameSpeak7 =   1 << 16, // (Sympathy) (Whistle 2)
    eGameSpeak8 =   1 << 17, // (Stop it) (Laugh)
    eChant =        1 << 18, 
    ePause =        1 << 19, // Or enter
    eUnPause =      1 << 20, // Or/and back
    // 0x200000     = nothing
    eCheatMode =    1 << 22,
    // 0x800000     = nothing
    // 0x1000000    = nothing
    // 0x2000000    = nothing
    // 0x4000000    = nothing
    // 0x8000000    = nothing
    // 0x10000000   = nothing
    // 0x20000000   = nothing
    // 0x40000000   = nothing
    // 0x80000000   = nothing
};

struct InputPadObject
{
    DWORD field_0_pressed;
    BYTE field_4_dir;
    BYTE field_5;
    WORD field_6_padding; // Not confirmed
    DWORD field_8_previous;
    DWORD field_C_held;
    DWORD field_10_released;
    DWORD field_14_padding; // Not confirmed
};
ALIVE_ASSERT_SIZEOF(InputPadObject, 0x18);

enum PsxButtonBits : unsigned int
{
    eL2 = 1 << 0,
    eR2 = 1 << 1,
    eL1 = 1 << 2,
    eR1 = 1 << 3,
    eTriangle = 1 << 4,
    eCircle = 1 << 5,
    eCross = 1 << 6,
    eSquare = 1 << 7,
    eSelect = 1 << 8,
    // As seen in LibEtc.h of PSYQ.. don't think these can ever be used.
    // PADi 9 ?
    // PADj 10 ?
    eStart = 1 << 11,
    eDPadUp = 1 << 12,
    eDPadRight = 1 << 13,
    eDPadDown = 1 << 14,
    eDPadLeft = 1 << 15,
};

struct InputBinding
{
    int key;
    int command;
};

class InputObject
{
public:
    EXPORT int Is_Demo_Playing_45F220();
    EXPORT void UnsetDemoPlaying_45F240();
    EXPORT void SetDemoResource_45F1E0(DWORD** pDemoRes);
    EXPORT void Update_45F040();
    EXPORT static DWORD CC Command_To_Raw_45EE40(DWORD cmd);
    EXPORT void CC ShutDown_45F020();
public:
    InputPadObject field_0_pads[2];
    DWORD** field_30_pDemoRes;
    DWORD field_34_demo_command_index;
    WORD field_38_bDemoPlaying;
    WORD field_3A_pad_idx;
    DWORD field_3C_command;
    DWORD field_40_command_duration;
};
ALIVE_ASSERT_SIZEOF(InputObject, 0x44);

ALIVE_VAR_EXTERN(InputObject, sInputObject_5BD4E0);
ALIVE_VAR_EXTERN(unsigned __int16, sCurrentControllerIndex_5C1BBE);
ALIVE_VAR_EXTERN(BOOL, sLastPressedKey_BD30A0);
ALIVE_VAR_EXTERN(int, sIsAKeyDown_BD309C);
ALIVE_VAR_EXTERN(__int16, word_5C1B9A);