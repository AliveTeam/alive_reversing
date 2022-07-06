#pragma once

#include "../AliveLibCommon/Function.hpp"

class BaseGameAutoPlayer;

namespace AO {

struct PSX_Pad final
{
    u16 mPressed;
    u8 mDir;
    s8 field_3;
    s16 mPreviousInput;
    u16 mHeld;
    s16 mReleased;
    s8 mPreviousDir;
    s8 field_B;
};
ALIVE_ASSERT_SIZEOF(PSX_Pad, 0xC);

// TODO: most of these are gonna be wrong bar the ones in abe.cpp
enum InputCommands : u32
{
    eRightGameSpeak = 1u << 0,    // 0x1
    eSneak = 1u << 1,             // 0x2
    eLeftGamespeak = 1u << 2,     // 0x4
    eRun = 1u << 3,               // 0x8
    eHop = 1u << 4,               // 0x10
    eThrowItem = 1u << 5,         // 0x20
    eCrouchOrRoll = 1u << 6,      // 0x40
    eDoAction = 1u << 7,          // 0x80
    eCheatMode = 1u << 8,         // 0x100
    eUnPause_OrConfirm = 1u << 9, // 0x200 - not present/unused in OG!!
    ePause = 1u << 10,            // 0x400 - not present/unused in OG!!
    eBack = 1u << 11,             // 0x800
    eUp = 1u << 12,               // 0x1000
    eRight = 1u << 13,            // 0x2000
    eDown = 1u << 14,             // 0x4000
    eLeft = 1u << 15,             // 0x8000
    // ---
    // anything else below this won't fit into 16-bit variables
    // ---
    //eGameSpeak7 = 1u << 16, // 0x10000
    //eGameSpeak8 = 1u << 17, // 0x20000
    //eChant = 1u << 18, // 0x40000
    //ePause = 1u << 19, // Or enter
    //eUnPause_OrConfirm = 1u << 20, // 0x100000 Or/and back
    //eBack = 1u << 21, // 0x200000
    //eCheatMode = 1u << 22,
    //eSpeak1 = 1u << 23, // 0x800000
    //eSpeak2 = 1u << 24, // 0x1000000    = nothing
    // 0x2000000    = nothing
    // 0x4000000    = nothing
    // 0x8000000    = nothing
    // 0x10000000   = nothing
    // 0x20000000   = nothing
    // 0x40000000   = nothing
    //e0x80000000 = 1u << 31,
};

extern const InputCommands sInputKey_Right;
extern const InputCommands sInputKey_Left;
extern const InputCommands sInputKey_Up;
extern const InputCommands sInputKey_Down;
extern const InputCommands sInputKey_Hop;
extern const InputCommands sInputKey_DoAction;
extern const InputCommands sInputKey_Run;
extern const InputCommands sInputKey_Sneak;
extern const InputCommands sInputKey_FartRoll;
extern const InputCommands sInputKey_ThrowItem;

extern const InputCommands sInputKey_LeftGameSpeakEnabler;
extern const InputCommands sInputKey_GameSpeak1;
extern const InputCommands sInputKey_GameSpeak2;
extern const InputCommands sInputKey_GameSpeak3;
extern const InputCommands sInputKey_GameSpeak4;

extern const InputCommands sInputKey_RightGameSpeakEnabler;
extern const InputCommands sInputKey_GameSpeak5;
extern const InputCommands sInputKey_GameSpeak6;
extern const InputCommands sInputKey_GameSpeak7;
extern const InputCommands sInputKey_GameSpeak8;

const InputCommands sInputKey_Chant = static_cast<InputCommands>(eRightGameSpeak | eLeftGamespeak);

#define kAO_Esc "\x06"

#define kAO_Speak1 "\x08"

#define kAO_Speak2 "\x09"

#define kAO_Up "\x10"
#define kAO_Down "\x11"
#define kAO_Left "\x12"
#define kAO_Right "\x13"
#define kAO_Or "\x14"
#define kAO_DirectionalButtons "\x15"
#define kAO_ConfirmContinue "\x16"

#define kAO_Run "\x0a"
#define kAO_Sneak "\x0b"
#define kAO_Jump_Or_Hello "\x0c"
#define kAO_Action "\x0d"
#define kAO_Throw "\x0e"
#define kAO_Crouch "\x0f"


bool Input_IsChanting();
void Input_InitKeyStateArray();
void Input_SetKeyState(s32 key, s8 bIsDown);



void Input_GetCurrentKeyStates();

s8 Input_IsVKPressed(s32 key);

void Input_Init();

void Input_DisableInput();

const char_type* Input_GetButtonString(InputCommands inputCommand, bool forceKeyboardLookupIfGamepadFails = true);
const char_type* Input_GetButtonString_44F1C0(InputCommands inputCommand);

s32 Input_Remap(InputCommands inputCmd);

s32 Input_SaveSettingsIni();

s8 Input_GetLastPressedKey();

void Input_Reset();

bool Input_JoyStickEnabled();
void Input_SetJoyStickEnabled(bool enabled);
bool Input_JoyStickAvailable();


class InputObject final
{
public:
    void InitPad(u32 padCount);

    // BC break of Update_433250
    void Update(BaseGameAutoPlayer& gameAutoPlayer);

    static void Shutdown();

    void SetDemoRes(u32** ppDemoRes);

    s32 IsDemoPlaying();

    PSX_Pad mPads[2] = {};
    u32** field_18_demo_res = nullptr;
    s32 field_1C_demo_command_index = 0;
    s16 field_20_demo_playing = 0;
    s16 field_22 = 0;
    s32 field_24_command = 0;
    s32 field_28_command_duration = 0;
    s32 field_2C = 0;

    // These use the active pad
    bool IsAnyPressed(u32 command) const;
    bool IsAnyHeld(u32 command) const;
    bool IsAnyReleased(u32 command) const;
    u8 Dir() const;
    bool IsAllPressed(u32 commands) const;
    bool IsAllHeld(u32 commands) const;

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
    bool IsAnyPressed(PadIndex padIx, u32 command) const;
    bool IsAnyHeld(PadIndex padIx, u32 command) const;
    bool IsAnyReleased(PadIndex padIx, u32 command) const;
    bool IsAllPressed(PadIndex padIx, u32 commands) const;

    u16 Pressed(PadIndex padIx) const;

    // These use the active pad
    u16 Pressed() const;
    u16 Held() const;
    u16 Released() const;

    u16 Held(PadIndex padIx) const;

    u32 Input_Read_Pad(u32 padIdx);
};
ALIVE_ASSERT_SIZEOF(InputObject, 0x30);

InputObject& Input();

} // namespace AO
