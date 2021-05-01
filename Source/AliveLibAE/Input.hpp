#pragma once

#include "FunctionFwd.hpp"

typedef s32 (*t_InputCallback)();

EXPORT u8 CC Input_GetInputEnabled_4EDDE0();
EXPORT void CC Input_EnableInput_4EDDD0();
EXPORT void CC Input_InitKeyStateArray_4EDD60();
EXPORT void Input_SaveSettingsIni_Common(bool isAo);
EXPORT void Input_SaveSettingsIni_492840();
EXPORT void CC Input_DisableInputForPauseMenuAndDebug_4EDDC0();
EXPORT void CC Input_Init_491BC0();
EXPORT void CC Input_SetKeyState_4EDD80(s32 key, s8 bIsDown);
EXPORT bool CC Input_IsVKPressed_4EDD40(s32 key);
EXPORT u32 Input_GetLastPressedKey_492610();
EXPORT void Input_Reset_492660();
EXPORT u32 Input_IsChanting_45F260();
EXPORT void CC Input_Pads_Reset_4FA960();
EXPORT void CC Input_Init_Names_491870();
EXPORT const char_type* CC Input_GetButtonString_492530(const char_type* idx, s32 controllerType);

EXPORT bool Input_JoyStickEnabled();
EXPORT void Input_SetJoyStickEnabled(bool enabled);
EXPORT bool Input_JoyStickAvailable();

namespace InputCommands {

enum Enum : u32
{
    eUp = 1u << 0,                 // 0x1
    eDown = 1u << 1,               // 0x2
    eLeft = 1u << 2,               // 0x4
    eRight = 1u << 3,              // 0x8
    eRun = 1u << 4,                // 0x10
    eDoAction = 1u << 5,           // Pick up rock, pull lever etc 0x20
    eSneak = 1u << 6,              // 0x40
    eThrowItem = 1u << 7,          // Or I say I dunno if no items 0x80
    eHop = 1u << 8,                // 0x100
    eFartOrRoll = 1u << 9,         // (Only roll in AO) 0x200
    eGameSpeak1 = 1u << 10,        // Hello 0x400
    eGameSpeak2 = 1u << 11,        // (Follow Me) 0x800
    eGameSpeak3 = 1u << 12,        // Wait 0x1000
    eGameSpeak4 = 1u << 13,        // (Work) (Whistle 1) 0x2000
    eGameSpeak5 = 1u << 14,        // (Anger) 0x4000
    eGameSpeak6 = 1u << 15,        // (All ya) (Fart) 0x8000
    eGameSpeak7 = 1u << 16,        // (Sympathy) (Whistle 2) 0x10000
    eGameSpeak8 = 1u << 17,        // (Stop it) (Laugh) 0x20000
    eChant = 1u << 18,             // 0x40000
    ePause = 1u << 19,             // Or enter 0x80000
    eUnPause_OrConfirm = 1u << 20, // Or/and back 0x100000
    eBack = 1u << 21,              // 0x200000
    eCheatMode = 1u << 22,         // 0x400000
    eSpeak1 = 1u << 23,            // 0x800000
    eSpeak2 = 1u << 24,            // 0x1000000
    // 0x2000000    = nothing
    // 0x4000000    = nothing
    // 0x8000000    = nothing
    // 0x10000000   = nothing
    ePageUp = 1u << 29,    // 0x20000000
    ePageDown = 1u << 30,  // 0x40000000
    eConfigure = 1u << 31, // 0x80000000
};

} // namespace InputCommands

extern const InputCommands::Enum sInputKey_Right_5550D0;
extern const InputCommands::Enum sInputKey_Left_5550D4;
extern const InputCommands::Enum sInputKey_Up_5550D8;
extern const InputCommands::Enum sInputKey_Down_5550DC;
extern const InputCommands::Enum sInputKey_Hop_5550E0;
extern const InputCommands::Enum sInputKey_DoAction_5550E4;
extern const InputCommands::Enum sInputKey_Run_5550E8;
extern const InputCommands::Enum sInputKey_Sneak_5550EC;
extern const InputCommands::Enum sInputKey_FartRoll_5550F0;
extern const InputCommands::Enum sInputKey_ThrowItem_5550F4;
extern const InputCommands::Enum sInputKey_GameSpeak2_5550F8;
extern const InputCommands::Enum sInputKey_GameSpeak4_5550FC;
extern const InputCommands::Enum sInputKey_GameSpeak3_555100;
extern const InputCommands::Enum sInputKey_GameSpeak1_555104;
extern const InputCommands::Enum sInputKey_GameSpeak6_555108;
extern const InputCommands::Enum sInputKey_GameSpeak5_55510C;
extern const InputCommands::Enum sInputKey_GameSpeak8_555110;
extern const InputCommands::Enum sInputKey_GameSpeak7_555114;

extern const InputCommands::Enum sInputKey_Chant;


#define kUp "\x01"
#define kDown "\x02"
#define kLeft "\x03"
#define kRight "\x04"
#define kRun "\x05"
#define kAction "\x06"
#define kSneak "\x07"
#define kThrow "\x08"
#define kJump "\x09"
#define kAllYa "\x10"
#define kSorry "\x11"
#define kStopIt "\x12"
#define kChant "\x13"
#define kConfirm "\x15"
#define kBack "\x16"

#define kFart "\x0a"
#define kHello "\x0b"
#define kFollowMe "\x0c"
#define kWait "\x0d"
#define kWork "\x0e"
#define kAnger "\x0f"

#define kSpeak1 "\x18"
#define kSpeak2 "\x19"

#define kHoistZTurn "\x1b"
#define kDPad "\x1a"

EXPORT s32 CC Input_Remap_492680(InputCommands::Enum inputCmd);
EXPORT void CC Input_ResetBinding_4925A0(s32 input_command, s32 bIsGamePad);
EXPORT s32 CC Input_Read_Pad_4FA9C0(s32 padNum);

struct InputPadObject final
{
    u32 field_0_pressed;
    u8 field_4_dir;
    u8 field_5;
    u16 field_6_padding; // Not confirmed
    u32 field_8_previous;
    u32 field_C_held;
    u32 field_10_released;
    u32 field_14_padding; // Not confirmed
};
ALIVE_ASSERT_SIZEOF(InputPadObject, 0x18);

enum PsxButtonBits : u32
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

struct InputBinding final
{
    s32 key;
    InputCommands::Enum command;
};

enum GamepadOptionFlags
{
    eDisableAutoRun = 0x1,
    eHasDPad = 0x2
};

class InputObject final
{
public:
    EXPORT s32 Is_Demo_Playing_45F220();
    EXPORT void UnsetDemoPlaying_45F240();
    EXPORT void SetDemoResource_45F1E0(u32** pDemoRes);
    EXPORT void Update_45F040();
    EXPORT static u32 CC Command_To_Raw_45EE40(u32 cmd);
    EXPORT static s8 CC Raw_To_Command_45EF70(s32 a1);
    EXPORT void CC ShutDown_45F020();

public:
    InputPadObject field_0_pads[2];
    u32** field_30_pDemoRes;
    u32 field_34_demo_command_index;
    u16 field_38_bDemoPlaying;
    u16 field_3A_pad_idx;
    u32 field_3C_command;
    u32 field_40_command_duration;
    bool isPressed(u32 command);
    bool isHeld(u32 command);
    bool IsReleased(u32 keys);
};
ALIVE_ASSERT_SIZEOF(InputObject, 0x44);

EXPORT InputObject& Input();

ALIVE_VAR_EXTERN(InputObject, sInputObject_5BD4E0);
ALIVE_VAR_EXTERN(u16, sCurrentControllerIndex_5C1BBE);
ALIVE_VAR_EXTERN(u32, sLastPressedKey_BD30A0);
ALIVE_VAR_EXTERN(s32, sIsAKeyDown_BD309C);
ALIVE_VAR_EXTERN(s16, bLongerTimeoutToNextDemo_5C1B9A);
ALIVE_VAR_EXTERN(s32, sJoystickEnabled_5C9F70);
