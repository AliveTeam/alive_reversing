#pragma once

#include "../relive_lib/Function.hpp"

typedef s32 (*t_InputCallback)();

u8 Input_GetInputEnabled_4EDDE0();
void Input_EnableInput_4EDDD0();
void Input_InitKeyStateArray_4EDD60();
void Input_SaveSettingsIni_Common();
void Input_SaveSettingsIni_492840();
void Input_DisableInputForPauseMenuAndDebug_4EDDC0();
void Input_Init();
void Input_SetKeyState(s32 key, s8 bIsDown);
bool Input_IsVKPressed_4EDD40(s32 key);
u32 Input_GetLastPressedKey_492610();
void Input_Reset_492660();
u32 Input_IsChanting();
void Input_Pads_Reset_4FA960();
void Input_Init_Names_491870();
const char_type* Input_GetButtonString_492530(const char_type* idx, s32 controllerType);

bool Input_JoyStickEnabled();
void Input_SetJoyStickEnabled(bool enabled);
bool Input_JoyStickAvailable();

struct PSX_Pad final
{
    u32 mRawInput;
    u8 mDir;
    u8 field_5;
    u16 field_6_padding; // Not confirmed
    u32 mPreviousInput;
    u32 mPressed;
    u32 mReleased;
    u32 field_14_padding; // Not confirmed
};
ALIVE_ASSERT_SIZEOF(PSX_Pad, 0x18);

enum InputCommands : u32
{
    eUp = 1u << 0,                 // 0x1
    eDown = 1u << 1,               // 0x2
    eLeft = 1u << 2,               // 0x4
    eRight = 1u << 3,              // 0x8
    eRun = 1u << 4,                // 0x10
    eDoAction = 1u << 5,           // Pick up rock, pull lever etc 0x20
    eSneak = 1u << 6,              // 0x40
    eThrowItem = 1u << 7,          // Or I mSay I dunno if no items 0x80
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
    eLeftGameSpeak = 1u << 23,            // 0x800000
    eRightGameSpeak = 1u << 24,            // 0x1000000
    // 0x2000000    = nothing
    // 0x4000000    = nothing
    // 0x8000000    = nothing
    eDelete = 1u << 28,    // 0x10000000
    ePageUp = 1u << 29,    // 0x20000000
    ePageDown = 1u << 30,  // 0x40000000
    eConfigure = 1u << 31, // 0x80000000
};

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

s32 Input_Remap_492680(InputCommands inputCmd);
void Input_ResetBinding_4925A0(s32 input_command, s32 bIsGamePad);
s32 Input_Read_Pad(s32 padNum);


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
    InputCommands command;
};

enum GamepadOptionFlags
{
    eDisableAutoRun = 0x1,
    eHasDPad = 0x2
};

class BaseGameAutoPlayer;

class InputObject final
{
public:
    s32 Is_Demo_Playing_45F220();
    void UnsetDemoPlaying_45F240();
    void SetDemoResource_45F1E0(u32** pDemoRes);
    void Update(BaseGameAutoPlayer& gameAutoPlayer);
    static u32 PsxButtonsToKeyboardInput_45EE40(u32 cmd);
    static s8 KeyboardInputToPsxButtons_45EF70(s32 cmd);
    void ShutDown_45F020();

public:
    PSX_Pad mPads[2] = {};
    u32** mpDemoRes = nullptr;
    u32 mDemoCommandIndex = 0;
    u16 mbDemoPlaying = 0;
    u16 field_3A_pad_idx = 0;
    u32 mCommand = 0;
    u32 mCommandDuration = 0;
    bool IsAnyHeld(u32 command);
    bool IsAnyPressed(u32 command);
    bool IsAnyReleased(u32 keys);

    u32 GetHeld();
    u32 GetPressed();
    u32 GetReleased();
};

InputObject& Input();

extern InputObject sInputObject;
extern u16 sCurrentControllerIndex;
extern u32 sLastPressedKey;
extern s32 sIsAKeyDown;
extern s16 bLongerTimeoutToNextDemo;
extern s32 sJoystickEnabled;
