#include "stdafx_ao.h"
#include "Input.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "../AliveLibAE/Input.hpp"
#include "BitField.hpp"
#include "BaseGameAutoPlayer.hpp"

namespace AO {

InputObject sInputObject = {};
u16 sCurrentControllerIndex = 0;
s32 sJoystickEnabled = 0;
u8 sInputEnabled = 0;
u32 sLastPressedKey = 0;

const InputCommands sInputKey_Right = eRight;
const InputCommands sInputKey_Left = eLeft;
const InputCommands sInputKey_Up = eUp;
const InputCommands sInputKey_Down = eDown;
const InputCommands sInputKey_Hop = eHop;
const InputCommands sInputKey_DoAction = eDoAction;
const InputCommands sInputKey_Run = eRun;
const InputCommands sInputKey_Sneak = eSneak;
const InputCommands sInputKey_FartRoll = eCrouchOrRoll;
const InputCommands sInputKey_ThrowItem = eThrowItem;

const InputCommands sInputKey_LeftGameSpeakEnabler = eLeftGamespeak;
const InputCommands sInputKey_GameSpeak1 = eHop;
const InputCommands sInputKey_GameSpeak2 = eDoAction;
const InputCommands sInputKey_GameSpeak3 = eThrowItem;
const InputCommands sInputKey_GameSpeak4 = eCrouchOrRoll;

const InputCommands sInputKey_RightGameSpeakEnabler = eRightGameSpeak;
const InputCommands sInputKey_GameSpeak5 = eCrouchOrRoll;
const InputCommands sInputKey_GameSpeak6 = eHop;
const InputCommands sInputKey_GameSpeak7 = eThrowItem;
const InputCommands sInputKey_GameSpeak8 = eDoAction;

void InputObject::InitPad(u32 /*padCount*/)
{
    for (PSX_Pad& pad : mPads)
    {
        pad = {};
    }

    field_20_demo_playing &= ~1u;

    Input_Pads_Reset_4FA960();
    Input_EnableInput_4EDDD0();
}

u8 sPad1Buffer_507778[64] = {};
u8 sPad2Buffer_507738[64] = {};

static void ConvertAEGamespeakAEtoAOGamespeak(BitField32<AO::InputCommands>& value, const BitField32<::InputCommands::Enum>& aeInput)
{
    if (aeInput.Get(::InputCommands::Enum::eGameSpeak1))
    {
        value.Set(AO::InputCommands::eLeftGamespeak);
        value.Set(AO::InputCommands::eHop);
    }
    else if (aeInput.Get(::InputCommands::Enum::eGameSpeak2))
    {
        value.Set(AO::InputCommands::eLeftGamespeak);
        value.Set(AO::InputCommands::eDoAction);
    }
    else if (aeInput.Get(::InputCommands::Enum::eGameSpeak3))
    {
        value.Set(AO::InputCommands::eLeftGamespeak);
        value.Set(AO::InputCommands::eCrouchOrRoll);
    }
    else if (aeInput.Get(::InputCommands::Enum::eGameSpeak4))
    {
        value.Set(AO::InputCommands::eLeftGamespeak);
        value.Set(AO::InputCommands::eThrowItem);
    }
    else if (aeInput.Get(::InputCommands::Enum::eGameSpeak5))
    {
        value.Set(AO::InputCommands::eRightGameSpeak);
        value.Set(AO::InputCommands::eCrouchOrRoll);
    }
    else if (aeInput.Get(::InputCommands::Enum::eGameSpeak6))
    {
        value.Set(AO::InputCommands::eRightGameSpeak);
        value.Set(AO::InputCommands::eHop);
    }
    else if (aeInput.Get(::InputCommands::Enum::eGameSpeak7))
    {
        value.Set(AO::InputCommands::eRightGameSpeak);
        value.Set(AO::InputCommands::eThrowItem);
    }
    else if (aeInput.Get(::InputCommands::Enum::eGameSpeak8))
    {
        value.Set(AO::InputCommands::eRightGameSpeak);
        value.Set(AO::InputCommands::eDoAction);
    }
    else if (aeInput.Get(::InputCommands::Enum::eBack))
    {
        value.Set(AO::InputCommands::eBack);
    }
    else if (aeInput.Get(::InputCommands::Enum::eCheatMode))
    {
        value.Set(AO::InputCommands::eCheatMode);
    }
}

static BitField32<AO::InputCommands> AEInputCommandsToAOInputCommands(const BitField32<::InputCommands::Enum>& aeInput)
{
    BitField32<AO::InputCommands> r;
    r.Set(AO::InputCommands::eUp, aeInput.Get(::InputCommands::Enum::eUp));
    r.Set(AO::InputCommands::eRight, aeInput.Get(::InputCommands::Enum::eRight));
    r.Set(AO::InputCommands::eDown, aeInput.Get(::InputCommands::Enum::eDown));
    r.Set(AO::InputCommands::eLeft, aeInput.Get(::InputCommands::Enum::eLeft));
    r.Set(AO::InputCommands::eRun, aeInput.Get(::InputCommands::Enum::eRun));
    r.Set(AO::InputCommands::eHop, aeInput.Get(::InputCommands::Enum::eHop));
    r.Set(AO::InputCommands::eSneak, aeInput.Get(::InputCommands::Enum::eSneak));
    r.Set(AO::InputCommands::eThrowItem, aeInput.Get(::InputCommands::Enum::eThrowItem));
    r.Set(AO::InputCommands::eCrouchOrRoll, aeInput.Get(::InputCommands::Enum::eFartOrRoll));
    r.Set(AO::InputCommands::eDoAction, aeInput.Get(::InputCommands::Enum::eDoAction));
    r.Set(AO::InputCommands::eBack, aeInput.Get(::InputCommands::Enum::eBack));
    r.Set(AO::InputCommands::eUnPause_OrConfirm, aeInput.Get(::InputCommands::Enum::eUnPause_OrConfirm));
    r.Set(AO::InputCommands::ePause, aeInput.Get(::InputCommands::Enum::ePause));
    ConvertAEGamespeakAEtoAOGamespeak(r, aeInput);
    return r;
}

static BitField32<::InputCommands::Enum> AOInputCommandsToAEInputCommands(const BitField32<AO::InputCommands>& aoInput)
{
    BitField32<::InputCommands::Enum> r;
    r.Set(::InputCommands::Enum::eUp, aoInput.Get(AO::InputCommands::eUp));
    r.Set(::InputCommands::Enum::eRight, aoInput.Get(AO::InputCommands::eRight));
    r.Set(::InputCommands::Enum::eDown, aoInput.Get(AO::InputCommands::eDown));
    r.Set(::InputCommands::Enum::eLeft, aoInput.Get(AO::InputCommands::eLeft));
    r.Set(::InputCommands::Enum::eRun, aoInput.Get(AO::InputCommands::eRun));
    r.Set(::InputCommands::Enum::eHop, aoInput.Get(AO::InputCommands::eHop));
    r.Set(::InputCommands::Enum::eSneak, aoInput.Get(AO::InputCommands::eSneak));
    r.Set(::InputCommands::Enum::eThrowItem, aoInput.Get(AO::InputCommands::eThrowItem));
    r.Set(::InputCommands::Enum::eFartOrRoll, aoInput.Get(AO::InputCommands::eCrouchOrRoll));
    r.Set(::InputCommands::Enum::eDoAction, aoInput.Get(AO::InputCommands::eDoAction));
    r.Set(::InputCommands::Enum::eBack, aoInput.Get(AO::InputCommands::eBack));
    r.Set(::InputCommands::Enum::eUnPause_OrConfirm, aoInput.Get(AO::InputCommands::eUnPause_OrConfirm));
    r.Set(::InputCommands::Enum::ePause, aoInput.Get(AO::InputCommands::ePause));

    // OG issue - LCD screens says hold alt + shift which is wrong
    r.Set(::InputCommands::Enum::eChant, aoInput.Get(AO::InputCommands::eLeftGamespeak) && aoInput.Get(AO::InputCommands::eRightGameSpeak));

    r.Set(::InputCommands::Enum::eGameSpeak1, aoInput.Get(AO::InputCommands::eLeftGamespeak) && aoInput.Get(AO::InputCommands::eHop));
    r.Set(::InputCommands::Enum::eGameSpeak2, aoInput.Get(AO::InputCommands::eLeftGamespeak) && aoInput.Get(AO::InputCommands::eDoAction));
    r.Set(::InputCommands::Enum::eGameSpeak3, aoInput.Get(AO::InputCommands::eLeftGamespeak) && aoInput.Get(AO::InputCommands::eCrouchOrRoll));
    r.Set(::InputCommands::Enum::eGameSpeak4, aoInput.Get(AO::InputCommands::eLeftGamespeak) && aoInput.Get(AO::InputCommands::eThrowItem));
    r.Set(::InputCommands::Enum::eGameSpeak5, aoInput.Get(AO::InputCommands::eRightGameSpeak) && aoInput.Get(AO::InputCommands::eDoAction));
    r.Set(::InputCommands::Enum::eGameSpeak6, aoInput.Get(AO::InputCommands::eRightGameSpeak) && aoInput.Get(AO::InputCommands::eHop));
    r.Set(::InputCommands::Enum::eGameSpeak7, aoInput.Get(AO::InputCommands::eRightGameSpeak) && aoInput.Get(AO::InputCommands::eThrowItem));
    r.Set(::InputCommands::Enum::eGameSpeak8, aoInput.Get(AO::InputCommands::eRightGameSpeak) && aoInput.Get(AO::InputCommands::eCrouchOrRoll));
    r.Set(::InputCommands::Enum::eCheatMode, aoInput.Get(AO::InputCommands::eCheatMode));

    // needed for remapping Speak I and Speak II on controllers
    r.Set(::InputCommands::Enum::eSpeak1, aoInput.Get(AO::InputCommands::eLeftGamespeak));
    r.Set(::InputCommands::Enum::eSpeak2, aoInput.Get(AO::InputCommands::eRightGameSpeak));

    return r;
}

static const char_type* AEInputCommandToAEInputString(::InputCommands::Enum input_command)
{
    if (input_command & ::InputCommands::Enum::eUp)
    {
        return kUp;
    }

    if (input_command & ::InputCommands::Enum::eDown)
    {
        return kDown;
    }

    if (input_command & ::InputCommands::Enum::eLeft)
    {
        return kLeft;
    }

    if (input_command & ::InputCommands::Enum::eRight)
    {
        return kRight;
    }

    if (input_command & ::InputCommands::Enum::eRun)
    {
        return kRun;
    }

    if (input_command & ::InputCommands::Enum::eHop)
    {
        return kJump;
    }

    if (input_command & ::InputCommands::Enum::eSneak)
    {
        return kSneak;
    }

    if (input_command & ::InputCommands::Enum::eThrowItem)
    {
        return kThrow;
    }

    if (input_command & ::InputCommands::Enum::eFartOrRoll)
    {
        return kFart; // TODO: wrong ?
    }

    if (input_command & ::InputCommands::Enum::eDoAction)
    {
        return kAction;
    }

    if (input_command & ::InputCommands::Enum::eChant)
    {
        return kChant;
    }

    if (input_command & ::InputCommands::Enum::eSpeak1)
    {
        return kSpeak1;
    }

    if (input_command & ::InputCommands::Enum::eSpeak2)
    {
        return kSpeak2;
    }

    if (input_command & ::InputCommands::Enum::eUnPause_OrConfirm)
    {
        return kConfirm;
    }

    if (input_command & ::InputCommands::Enum::eBack)
    {
        return kBack;
    }

    // TODO: Game speaks
    /*
#define kAllYa "\x10"
#define kSorry "\x11"
#define kStopIt "\x12"

#define kFart "\x0a"
#define kHello "\x0b"
#define kFollowMe "\x0c"
#define kWait "\x0d"
#define kWork "\x0e"
#define kAnger "\x0f"

#define kHoistZTurn "\x1b"
#define kDPad "\x1a"
*/

    return kUp;
}


static BitField32<::InputCommands::Enum> MakeAEInputBits(u32 bits)
{
    BitField32<::InputCommands::Enum> r;
    r.Raw().all = bits;
    return r;
}

static BitField32<AO::InputCommands> MakeAOInputBits(u32 bits)
{
    BitField32<AO::InputCommands> r;
    r.Raw().all = bits;
    return r;
}

void InputObject::Update(BaseGameAutoPlayer& gameAutoPlayer)
{
    const u8 byte_4BB428[16] = {0u, 64u, 0u, 32u, 192u, 0u, 224u, 0u, 128u, 96u, 0u, 0u, 160u, 0u, 0u, 0u};

    for (s32 i = 0; i < 2; i++)
    {
        mPads[i].mPreviousDir = mPads[i].mDir;
        mPads[i].field_B = mPads[i].field_3;
        mPads[i].mPreviousInput = mPads[i].mPressed;
    }

    // Do AE input reading
    ::Input().Update(gameAutoPlayer);

    // Convert from AE bit flags to AO bit flags
    mPads[0].mPressed = static_cast<u16>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().mPads[0].mPressed)).Raw().all);

    // TODO: This one probably needs its own conversion
    mPads[0].mDir = ::Input().mPads[0].mDir;

    mPads[0].mPreviousInput = static_cast<u16>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().mPads[0].mPreviousInput)).Raw().all);
    mPads[0].mHeld = static_cast<u16>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().mPads[0].mHeld)).Raw().all);
    mPads[0].mReleased = static_cast<u16>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().mPads[0].mReleased)).Raw().all);

    // Handle demo input (AO impl)
    if (field_20_demo_playing & 1)
    {
        // Stop if any button on any pad is pressed
        if (mPads[sCurrentControllerIndex].mPressed)
        {
            field_20_demo_playing &= ~1u;
            return;
        }

        if (static_cast<s32>(sGnFrame) >= field_28_command_duration)
        {
            const u32 command = (*field_18_demo_res)[field_1C_demo_command_index++];
            field_24_command = command >> 16;
            field_28_command_duration = sGnFrame + (command & 0xFFFF);

            // End demo/quit command
            if (command & 0x8000)
            {
                field_20_demo_playing &= ~1u;
            }
        }

        // Will do nothing if we hit the end command..
        if (field_20_demo_playing & 1)
        {
            mPads[0].mPressed = static_cast<u16>(field_24_command);
        }

        for (s32 i = 0; i < 2; i++)
        {
            mPads[i].mReleased = ~mPads[i].mPressed & mPads[i].mPreviousInput;
            mPads[i].mHeld = ~mPads[i].mPreviousInput & mPads[i].mPressed;
            mPads[i].mDir = byte_4BB428[(mPads[i].mPressed >> 12) & 0xF];
            mPads[i].field_3 = byte_4BB428[(mPads[i].mPressed >> 4) & 0xF];
        }

        return;
    }

    // Original AO impl
    //for (s32 i = 0; i < 2; i++)
    //{
    //    mPads[i].mPreviousDir = mPads[i].mDir;
    //    mPads[i].field_B = mPads[i].field_3;
    //    mPads[i].mPreviousInput = mPads[i].mPressed;
    //}

    //if (sPad1Buffer_507778[0])
    //{
    //    mPads[0].mPressed = 0;
    //}
    //else
    //{
    //    mPads[0].mPressed = ~(sPad1Buffer_507778[3] + (sPad1Buffer_507778[2] << 8));
    //}

    //if (sPad2Buffer_507738[0])
    //{
    //    mPads[1].mPressed = 0;
    //}
    //else
    //{
    //    mPads[1].mPressed = ~(sPad2Buffer_507738[3] + (sPad2Buffer_507738[2] << 8));
    //}

    //if (field_20_demo_playing & 1)
    //{
    //    // Stop if any button on any pad is pressed
    //    if (mPads[sCurrentControllerIndex].mPressed)
    //    {
    //        field_20_demo_playing &= ~1u;
    //        return;
    //    }

    //    if (static_cast<s32>(sGnFrame) >= field_28_command_duration)
    //    {
    //        const u32 command = (*field_18_demo_res)[field_1C_demo_command_index++];
    //        field_24_command = command >> 16;
    //        field_28_command_duration = sGnFrame + command & 0xFFFF;

    //        // End demo/quit command
    //        if (command & 0x8000)
    //        {
    //            field_20_demo_playing &= ~1u;
    //        }
    //    }

    //    // Will do nothing if we hit the end command..
    //    if (field_20_demo_playing & 1)
    //    {
    //        mPads[0].mPressed = static_cast<u16>(field_24_command);
    //    }
    //}

    //for (s32 i = 0; i < 2; i++)
    //{
    //    mPads[i].mReleased = ~mPads[i].mPressed & mPads[i].mPreviousInput;
    //    mPads[i].mHeld = ~mPads[i].mPreviousInput & mPads[i].mPressed;
    //    mPads[i].mDir = byte_4BB428[(mPads[i].mPressed >> 12) & 0xF];
    //    mPads[i].field_3 = byte_4BB428[(mPads[i].mPressed >> 4) & 0xF];
    //}
}

void InputObject::Shutdown()
{
}


void InputObject::SetDemoRes(u32** ppDemoRes)
{
    field_1C_demo_command_index = 2051;
    field_18_demo_res = ppDemoRes;
    field_20_demo_playing |= 1u;
    field_28_command_duration = 0;
}

s32 InputObject::IsDemoPlaying()
{
    return field_20_demo_playing & 1;
}

static s32 PadIndexToInt(InputObject::PadIndex idx)
{
    switch (idx)
    {
        case InputObject::PadIndex::Active:
            return sCurrentControllerIndex;

        case InputObject::PadIndex::Second:
            return 1;

        case InputObject::PadIndex::First:
        default:
            return 0;
    }
}

InputObject::PadIndex InputObject::CurrentController() const
{
    if (sCurrentControllerIndex == 0)
    {
        return PadIndex::First;
    }
    return PadIndex::Second;
}


void InputObject::SetCurrentController(PadIndex padIdx)
{
    sCurrentControllerIndex = padIdx == InputObject::PadIndex::First ? 0 : 1;
}


bool InputObject::JoyStickEnabled() const
{
    return Input_JoyStickEnabled();
}

bool InputObject::IsAnyPressed(u32 command) const
{
    return IsAnyPressed(PadIndex::Active, command);
}

bool InputObject::IsAnyPressed(PadIndex padIx, u32 command) const
{
    return (mPads[PadIndexToInt(padIx)].mPressed & command) != 0;
}

bool InputObject::IsAnyHeld(u32 command) const
{
    return IsAnyHeld(PadIndex::Active, command);
}

bool InputObject::IsAnyHeld(PadIndex padIx, u32 command) const
{
    return (mPads[PadIndexToInt(padIx)].mHeld & command) != 0;
}

bool InputObject::IsAnyReleased(u32 command) const
{
    return IsAnyReleased(PadIndex::Active, command);
}

bool InputObject::IsAnyReleased(PadIndex padIx, u32 command) const
{
    return (mPads[PadIndexToInt(padIx)].mReleased & command) != 0;
}

u8 InputObject::Dir() const
{
    return Input().mPads[sCurrentControllerIndex].mDir >> 5;
}


bool InputObject::IsAllPressed(u32 commands) const
{
    return IsAllPressed(PadIndex::Active, commands);
}

bool InputObject::IsAllPressed(PadIndex padIx, u32 commands) const
{
    return (mPads[PadIndexToInt(padIx)].mPressed & commands) == commands;
}


bool InputObject::IsAllHeld(u32 commands) const
{
    return (mPads[sCurrentControllerIndex].mHeld & commands) == commands;
}

u16 InputObject::Pressed() const
{
    return Pressed(PadIndex::Active);
}

u16 InputObject::Pressed(PadIndex padIx) const
{
    return mPads[PadIndexToInt(padIx)].mPressed;
}

u16 InputObject::Held() const
{
    return Held(PadIndex::Active);
}

u16 InputObject::Held(PadIndex padIx) const
{
    return Input().mPads[PadIndexToInt(padIx)].mHeld;
}

u32 InputObject::Input_Read_Pad(u32 padIdx)
{
    return Input_Read_Pad_4FA9C0(padIdx);
}

u16 InputObject::Released() const
{
    return Input().mPads[sCurrentControllerIndex].mReleased;
}

bool Input_IsChanting()
{
    return Input_IsChanting_45F260();
}

void Input_SetKeyState(s32 key, s8 bIsDown)
{
    Input_SetKeyState_4EDD80(key, bIsDown);
}

s8 Input_IsVKPressed(s32 key)
{
    return Input_IsVKPressed_4EDD40(key);
}

// from the MainMenu class
extern s32 gJoystickAvailable;

void Input_Init()
{
    Input_Init_491BC0();

    if (Input_JoyStickAvailable())
    {
        gJoystickAvailable = 1;
    }
}

void Input_DisableInput()
{
    Input_DisableInputForPauseMenuAndDebug_4EDDC0();
}

// Only really needed for SHIFT being pressed for the AO ddcheat, may as well just remove the requirement
// for shift to be pressed because it is a pain anyway.
void Input_GetCurrentKeyStates()
{
}

void Input_InitKeyStateArray()
{
    Input_InitKeyStateArray_4EDD60();
}

const char_type* Input_GetButtonString(InputCommands inputCommand, bool forceKeyboardLookupIfGamepadFails)
{
    const auto aeBits = static_cast<::InputCommands::Enum>(AOInputCommandsToAEInputCommands(MakeAOInputBits(inputCommand)).Raw().all);
    s32 controller_type = forceKeyboardLookupIfGamepadFails ? 2 : 1;

    return ::Input_GetButtonString_492530(
        AEInputCommandToAEInputString(aeBits),
        Input_JoyStickEnabled() ? controller_type : 0);
}

const char_type* Input_GetButtonString_44F1C0(InputCommands inputCommand)
{
    return Input_GetButtonString(inputCommand);
}

s32 Input_Remap(InputCommands inputCmd)
{
    return Input_Remap_492680(static_cast<::InputCommands::Enum>(AOInputCommandsToAEInputCommands(MakeAOInputBits(inputCmd)).Raw().all));
}

s8 Input_GetLastPressedKey()
{
    // AE impl
    return static_cast<s8>(::Input_GetLastPressedKey_492610());

    // AO impl
    //if (!Sys_IsAnyKeyDown())
    //{
    //    return 0;
    //}

    //const s8 result = static_cast<s8>(sLastPressedKey);
    //sIsAKeyDown = 0;
    //sLastPressedKey = 0;
    //return result;
}

s32 Input_Enable_48E6A0()
{
    // AE impl
    ::Input_EnableInput_4EDDD0();
    return 0;

    // AO impl
    //sInputEnabled = 1;
    //return 0;
}

void Input_Reset()
{
    // Funcs below call AE impls in standalone
    Input_Enable_48E6A0();
    Input_InitKeyStateArray();
}


bool Input_JoyStickEnabled()
{
    // Use AE var
    return ::Input_JoyStickEnabled();

    // Use AO var
    //return sJoystickEnabled ? true : false;
}

bool Input_JoyStickAvailable()
{
    return ::Input_JoyStickAvailable();
}

void Input_SetJoyStickEnabled(bool enabled)
{
    // Use AE var
    return ::Input_SetJoyStickEnabled(enabled);

    // Use AO var
    //sJoystickEnabled = enabled;
}

u32 dword_508A64 = 0;

s32 Input_SaveSettingsIni()
{
    // Call AE func both in standalone and DLL
    Input_SaveSettingsIni_Common();
    return 1;

    // AO approach
    //FILE* iniFileHandle = fopen("abe.ini", "w");
    //if (!iniFileHandle)
    //{
    //    return 0;
    //}

    //dword_508A64 = 1;
    //fprintf(iniFileHandle, "[Control Layout]\n");
    //if (Input_JoyStickEnabled())
    //{
    //    fprintf(iniFileHandle, "controller = Game Pad\n");

    //}
    //else
    //{
    //    fprintf(iniFileHandle, "controller = Keyboard\n");
    //}
    //
    //const auto oldJoystickEnabled = Input_JoyStickEnabled();

    //Input_SetJoyStickEnabled(false);

    //fprintf(iniFileHandle, "[Keyboard]\n");
    //fprintf(iniFileHandle, "run = %s\n", Input_GetButtonString_44F1C0(InputCommands::eRun));
    //fprintf(iniFileHandle, "sneak = %s\n", Input_GetButtonString_44F1C0(InputCommands::eSneak));
    //fprintf(iniFileHandle, "jump = %s\n", Input_GetButtonString_44F1C0(InputCommands::eHop));
    //fprintf(iniFileHandle, "action = %s\n", Input_GetButtonString_44F1C0(InputCommands::eDoAction));
    //fprintf(iniFileHandle, "throw = %s\n", Input_GetButtonString_44F1C0(InputCommands::eThrowItem));
    //fprintf(iniFileHandle, "crouch = %s\n", Input_GetButtonString_44F1C0(InputCommands::eCrouchOrRoll));

    //Input_SetJoyStickEnabled(true);

    //fprintf(iniFileHandle, "[Game Pad]\n");
    //fprintf(iniFileHandle, "run = %s\n", Input_GetButtonString_44F1C0(InputCommands::eRun));
    //fprintf(iniFileHandle, "sneak = %s\n", Input_GetButtonString_44F1C0(InputCommands::eSneak));
    //fprintf(iniFileHandle, "jump = %s\n", Input_GetButtonString_44F1C0(InputCommands::eHop));
    //fprintf(iniFileHandle, "action = %s\n", Input_GetButtonString_44F1C0(InputCommands::eDoAction));
    //fprintf(iniFileHandle, "throw = %s\n", Input_GetButtonString_44F1C0(InputCommands::eThrowItem));
    //fprintf(iniFileHandle, "crouch = %s\n", Input_GetButtonString_44F1C0(InputCommands::eCrouchOrRoll));

    //Input_SetJoyStickEnabled(oldJoystickEnabled);

    //fclose(iniFileHandle);
    //dword_508A64 = 0;
    //return 1;
}

InputObject& Input()
{
    return sInputObject;
}

} // namespace AO
