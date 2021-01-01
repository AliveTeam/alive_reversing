#pragma once

#include "FunctionFwd.hpp"

typedef int(*t_InputCallback)();

EXPORT unsigned __int8 CC Input_GetInputEnabled_4EDDE0();
EXPORT void CC Input_EnableInput_4EDDD0();
EXPORT void CC Input_InitKeyStateArray_4EDD60();
EXPORT void Input_SaveSettingsIni_492840();
EXPORT void CC Input_DisableInputForPauseMenuAndDebug_4EDDC0();
EXPORT void CC Input_Init_491BC0();
EXPORT void CC Input_SetKeyState_4EDD80(int key, char bIsDown);
EXPORT bool CC Input_IsVKPressed_4EDD40(int key);
EXPORT DWORD Input_GetLastPressedKey_492610();
EXPORT void Input_Reset_492660();
EXPORT unsigned int Input_IsChanting_45F260();
EXPORT void CC Input_Pads_Reset_4FA960();
EXPORT void CC Input_Init_Names_491870();
EXPORT const char* CC Input_GetButtonString_492530(const char* idx, int controllerType);

EXPORT bool Input_JoyStickEnabled();
EXPORT void Input_SetJoyStickEnabled(bool enabled);

enum InputCommands : unsigned int
{
    eUp =           1u << 0,  // 0x1
    eDown =         1u << 1,  // 0x2
    eLeft =         1u << 2,  // 0x4
    eRight =        1u << 3,  // 0x8
    eRun =          1u << 4,  // 0x10
    eDoAction =     1u << 5,  // Pick up rock, pull lever etc 0x20
    eSneak =        1u << 6,  // 0x40
    eThrowItem =    1u << 7,  // Or I say I dunno if no items 0x80
    eHop =          1u << 8,  // 0x100
    eFartOrRoll =   1u << 9,  // (Only roll in AO) 0x200
    eGameSpeak1 =   1u << 10, // Hello 0x400
    eGameSpeak2 =   1u << 11, // (Follow Me) 0x800
    eGameSpeak3 =   1u << 12, // Wait 0x1000
    eGameSpeak4 =   1u << 13, // (Work) (Whistle 1) 0x2000
    eGameSpeak5 =   1u << 14, // (Anger) 0x4000
    eGameSpeak6 =   1u << 15, // (All ya) (Fart) 0x8000
    eGameSpeak7 =   1u << 16, // (Sympathy) (Whistle 2) 0x10000
    eGameSpeak8 =   1u << 17, // (Stop it) (Laugh) 0x20000
    eChant =        1u << 18, // 0x40000
    ePause =        1u << 19, // Or enter 0x80000
    eUnPause_OrConfirm =      1u << 20, // Or/and back 0x100000
    eBack =         1u << 21, // 0x200000
    eCheatMode =    1u << 22, // 0x400000
    eSpeak1   =    1u << 23, // 0x800000
    eSpeak2 =      1u << 24, // 0x1000000
    // 0x2000000    = nothing
    // 0x4000000    = nothing
    // 0x8000000    = nothing
    // 0x10000000   = nothing
    ePageUp =       1u << 29, // 0x20000000
    ePageDown =     1u << 30, // 0x40000000
    eConfigure   = 1u << 31, // 0x80000000
};

extern const InputCommands sInputKey_Right_5550D0;
extern const InputCommands sInputKey_Left_5550D4;
extern const InputCommands sInputKey_Up_5550D8;
extern const InputCommands sInputKey_Down_5550DC;
extern const InputCommands sInputKey_Hop_5550E0;
extern const InputCommands sInputKey_DoAction_5550E4;
extern const InputCommands sInputKey_Run_5550E8;
extern const InputCommands sInputKey_Sneak_5550EC;
extern const InputCommands sInputKey_FartRoll_5550F0;
extern const InputCommands sInputKey_ThrowItem_5550F4;
extern const InputCommands sInputKey_GameSpeak2_5550F8;
extern const InputCommands sInputKey_GameSpeak4_5550FC;
extern const InputCommands sInputKey_GameSpeak3_555100;
extern const InputCommands sInputKey_GameSpeak1_555104;
extern const InputCommands sInputKey_GameSpeak6_555108;
extern const InputCommands sInputKey_GameSpeak5_55510C;
extern const InputCommands sInputKey_GameSpeak8_555110;
extern const InputCommands sInputKey_GameSpeak7_555114;

extern const InputCommands sInputKey_Chant;

EXPORT int CC Input_Remap_492680(InputCommands inputCmd);
EXPORT void CC Input_ResetBinding_4925A0(int input_command, int bIsGamePad);
EXPORT int CC Input_Read_Pad_4FA9C0(int padNum);

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
    eDisableAutoRun = 0x1,
    eHasDPad = 0x2
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
    bool IsReleased(DWORD keys);
};
ALIVE_ASSERT_SIZEOF(InputObject, 0x44);

EXPORT InputObject& Input();

ALIVE_VAR_EXTERN(InputObject, sInputObject_5BD4E0);
ALIVE_VAR_EXTERN(unsigned __int16, sCurrentControllerIndex_5C1BBE);
ALIVE_VAR_EXTERN(DWORD, sLastPressedKey_BD30A0);
ALIVE_VAR_EXTERN(int, sIsAKeyDown_BD309C);
ALIVE_VAR_EXTERN(__int16, bLongerTimeoutToNextDemo_5C1B9A);
ALIVE_VAR_EXTERN(int, sJoystickEnabled_5C9F70);
