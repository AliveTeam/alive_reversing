#pragma once

#include "FunctionFwd.hpp"

namespace AO {

struct PSX_Pad
{
    unsigned __int16 field_0_pressed;
    unsigned __int8 field_2_dir;
    char field_3;
    __int16 field_4_previously_pressed;
    unsigned __int16 field_6_held;
    __int16 field_8_released;
    __int8 field_A_prev_dir;
    __int8 field_B;
};
ALIVE_ASSERT_SIZEOF(PSX_Pad, 0xC);

// TODO: most of these are gonna be wrong bar the ones in abe.cpp
enum InputCommands : unsigned int
{
    eRightGameSpeak = 1u << 0,  // 0x1
    eSneak = 1u << 1,  // 0x2
    eLeftGamespeak = 1u << 2,  // 0x4
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

extern const InputCommands sInputKey_Right_4C6590;
extern const InputCommands sInputKey_Left_4C6594;
extern const InputCommands sInputKey_Up_4C6598;
extern const InputCommands sInputKey_Down_4C659C;
extern const InputCommands sInputKey_Hop_4C65A0;
extern const InputCommands sInputKey_DoAction_4C65A4;
extern const InputCommands sInputKey_Run_4C65A8;
extern const InputCommands sInputKey_Sneak_4C65AC;
extern const InputCommands sInputKey_FartRoll_4C65B0;
extern const InputCommands sInputKey_ThrowItem_4C65B4;

extern const InputCommands sInputKey_LeftGameSpeakEnabler_4C65B8;
extern const InputCommands sInputKey_GameSpeak1_4C65C8;
extern const InputCommands sInputKey_GameSpeak2_4C65BC;
extern const InputCommands sInputKey_GameSpeak3_4C65C0;
extern const InputCommands sInputKey_GameSpeak4_4C65C4;

extern const InputCommands sInputKey_RightGameSpeakEnabler_4C65DC;
extern const InputCommands sInputKey_GameSpeak5_4C65EC;
extern const InputCommands sInputKey_GameSpeak6_4C65E8;
extern const InputCommands sInputKey_GameSpeak7_4C65E4;
extern const InputCommands sInputKey_GameSpeak8_4C65E0;

const InputCommands sInputKey_Chant = static_cast<InputCommands>(eRightGameSpeak | eLeftGamespeak);

#define kAO_Esc "\x06"

#define kAO_Speak1 "\x08"

#define kAO_Speak2 "\x09"

#define kAO_Up "\x10"
#define kAO_Down "\x11"
#define kAO_Left "\x12"
#define kAO_Right "\x13"
#define kAO_MountZTurn "\x14"
#define kAO_15 "\x15"

#define kAO_Run "\x0a"
#define kAO_Sneak "\x0b"
#define kAO_Jump_Or_Hello "\x0c"
#define kAO_Action "\x0d"
#define kAO_Throw "\x0e"
#define kAO_Enter "\x0f"


EXPORT BOOL CC Input_IsChanting_4334C0();
EXPORT void Input_InitKeyStateArray_48E5F0();
EXPORT void CC Input_SetKeyState_48E610(int key, char bIsDown);



EXPORT void Input_GetCurrentKeyStates_48E630();

EXPORT char CC Input_IsVKPressed_48E5D0(int key);

EXPORT void CC Input_Init_44EB60();

EXPORT void Input_DisableInput_48E690();

EXPORT const char* CC Input_GetButtonString_44F1C0(InputCommands input_command);

EXPORT int CC Input_Remap_44F300(InputCommands inputCmd);

EXPORT int CC Input_SaveSettingsIni_44F460();

EXPORT char Input_GetLastPressedKey_44F2C0();

EXPORT void Input_Reset_44F2F0();

bool Input_JoyStickEnabled();
void Input_SetJoyStickEnabled(bool enabled);

class InputObject
{
public:
    EXPORT void InitPad_4331A0(unsigned int padCount);

    EXPORT void Update_433250();

    static EXPORT void CC Shutdown_433230();

    EXPORT void SetDemoRes_433470(DWORD** ppDemoRes);

    EXPORT int IsDemoPlaying_4334A0();

    PSX_Pad field_0_pads[2];
    DWORD** field_18_demo_res;
    int field_1C_demo_command_index;
    __int16 field_20_demo_playing;
    __int16 field_22;
    int field_24_command;
    int field_28_command_duration;
    int field_2C;

    // These use the active pad
    bool IsAnyPressed(DWORD command) const;
    bool IsAnyHeld(DWORD command) const;
    bool IsAnyReleased(DWORD command) const;
    unsigned __int8 Dir() const;
    bool IsAllPressed(DWORD commands) const;
    bool IsAllHeld(DWORD commands) const;

    enum class PadIndex
    {
        First,
        Second,
        Active,
    };

    PadIndex CurrentController() const;

    void SetCurrentController(PadIndex padIdx);

    bool JoyStickEnabled() const;

    // Check a specific pad
    bool IsAnyPressed(PadIndex padIx, DWORD command) const;
    bool IsAnyHeld(PadIndex padIx, DWORD command) const;
    bool IsAnyReleased(PadIndex padIx, DWORD command) const;
    bool IsAllPressed(PadIndex padIx, DWORD commands) const;

    unsigned __int16 Pressed(PadIndex padIx) const;

    // These use the active pad
    unsigned __int16 Pressed() const;
    unsigned short Held() const;
    unsigned short Released() const;

    unsigned short Held(PadIndex padIx) const;
};
ALIVE_ASSERT_SIZEOF(InputObject, 0x30);

InputObject& Input();

}
