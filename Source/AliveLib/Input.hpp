#pragma once

#include "FunctionFwd.hpp"

typedef int(*t_InputCallback)();

EXPORT unsigned __int8 CC Input_GetInputEnabled_4EDDE0();
EXPORT void CC Input_EnableInput_4EDDD0();
EXPORT void CC Input_InitKeyStateArray_4EDD60();
EXPORT void Input_SaveSettingsIni_492840();
EXPORT void CC Input_DisableInput_4EDDC0();
EXPORT void CC Input_Init_491BC0();
EXPORT void CC Input_SetKeyState_4EDD80(int key, char bIsDown);
EXPORT bool CC Input_IsVKPressed_4EDD40(int key);
EXPORT DWORD Input_GetLastPressedKey_492610();
EXPORT void Input_Reset_492660();
EXPORT unsigned int Input_IsChanting_45F260();
EXPORT void CC Input_Pads_Reset_4FA960();

enum InputCommands : unsigned int
{
    eUp =           1u << 0, // 0x1
    eDown =         1u << 1, // 0x2
    eLeft =         1u << 2, // 0x4
    eRight =        1u << 3, // 0x8
    eRun =          1u << 4,
    eDoAction =     1u << 5,  // Pick up rock, pull lever etc
    eSneak =        1u << 6,
    eThrowItem =    1u << 7,  // Or I say I dunno if no items
    eHop =          1u << 8,
    eFartOrRoll =   1u << 9,  // (Only roll in AO)
    eGameSpeak1 =   1u << 10, // Hello 0x400
    eGameSpeak2 =   1u << 11, // (Follow Me) 0x800
    eGameSpeak3 =   1u << 12, // Wait 0x1000
    eGameSpeak4 =   1u << 13, // (Work) (Whistle 1) 0x2000
    eGameSpeak5 =   1u << 14, // (Anger) 0x4000
    eGameSpeak6 =   1u << 15, // (All ya) (Fart) 0x8000
    eGameSpeak7 =   1u << 16, // (Sympathy) (Whistle 2) 0x10000
    eGameSpeak8 =   1u << 17, // (Stop it) (Laugh) 0x20000
    eChant =        1u << 18, 
    ePause =        1u << 19, // Or enter
    eUnPause =      1u << 20, // Or/and back
    eBack =         1u << 21,
    eCheatMode =    1u << 22,
    // 0x800000     = nothing
    // 0x1000000    = nothing
    // 0x2000000    = nothing
    // 0x4000000    = nothing
    // 0x8000000    = nothing
    // 0x10000000   = nothing
    // 0x20000000   = nothing
    // 0x40000000   = nothing
    e0x80000000   = 1u << 31,
};

// Not to be confused for PsxButtonBits.
// This is used internally in input functions.
// 0 Square
// 1 Cross
// 2 Circle
// 3 Triangle
// 4 L1
// 5 R1
// 6 L2
// 7 R2
// 8 Back
// 9 Start
enum InputButtonBits : unsigned int
{
    eInputSquare = 1 << 0,
    eInputCross = 1 << 1,
    eInputCircle = 1 << 2,
    eInputTriangle = 1 << 3,
    eInputL1 = 1 << 4,
    eInputR1 = 1 << 5,
    eInputL2 = 1 << 6,
    eInputR2 = 1 << 7,
    eInputSelect = 1 << 8,
    eInputStart = 1 << 9,
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
    InputCommands command;
};

enum GamepadOptionFlags
{
    eAutoRun = 0x1,
    eHasDPad = 0x2,
    eUnknown_3 = 0x4,
    eUnknown_4 = 0x8,
};

class InputObject
{
public:
    EXPORT int Is_Demo_Playing_45F220();
    EXPORT void UnsetDemoPlaying_45F240();
    EXPORT void SetDemoResource_45F1E0(DWORD** pDemoRes);
    EXPORT void Update_45F040();
    EXPORT static DWORD CC Command_To_Raw_45EE40(DWORD cmd);
    EXPORT static char CC Raw_To_Command_45EF70(int a1);
    EXPORT void CC ShutDown_45F020();
public:
    InputPadObject field_0_pads[2];
    DWORD** field_30_pDemoRes;
    DWORD field_34_demo_command_index;
    WORD field_38_bDemoPlaying;
    WORD field_3A_pad_idx;
    DWORD field_3C_command;
    DWORD field_40_command_duration;
    bool isPressed(DWORD command);
    bool isHeld(DWORD command);
};
ALIVE_ASSERT_SIZEOF(InputObject, 0x44);

ALIVE_VAR_EXTERN(InputObject, sInputObject_5BD4E0);
ALIVE_VAR_EXTERN(unsigned __int16, sCurrentControllerIndex_5C1BBE);
ALIVE_VAR_EXTERN(DWORD, sLastPressedKey_BD30A0);
ALIVE_VAR_EXTERN(int, sIsAKeyDown_BD309C);
ALIVE_VAR_EXTERN(__int16, word_5C1B9A);
ALIVE_VAR_EXTERN(int, sJoystickEnabled_5C9F70);
