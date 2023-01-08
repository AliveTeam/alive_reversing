#include "stdafx_ao.h"
#include "Input.hpp"
#include "Game.hpp"
#include "../AliveLibAE/Input.hpp"
#include "../relive_lib/BitField.hpp"
#include "../relive_lib/BaseGameAutoPlayer.hpp"

namespace AO {

InputObject sInputObject = {};
u16 sCurrentControllerIndex = 0;
u8 sInputEnabled = 0;
u32 sLastPressedKey = 0;

void InputObject::InitPad(u32 /*padCount*/)
{
    for (PSX_Pad& pad : mPads)
    {
        pad = {};
    }

    mbDemoPlaying &= ~1u;

    Input_Pads_Reset_4FA960();
    Input_EnableInput_4EDDD0();
}

u8 sPad1Buffer_507778[64] = {};
u8 sPad2Buffer_507738[64] = {};

static void ConvertAEGamespeakAEtoAOGamespeak(BitField32<AO::InputCommands>& value, const BitField32<::InputCommands>& aeInput)
{
    if (aeInput.Get(::InputCommands::eGameSpeak1))
    {
        value.Set(AO::InputCommands::eLeftGameSpeak);
        value.Set(AO::InputCommands::eHop);
    }
    else if (aeInput.Get(::InputCommands::eGameSpeak2))
    {
        value.Set(AO::InputCommands::eLeftGameSpeak);
        value.Set(AO::InputCommands::eDoAction);
    }
    else if (aeInput.Get(::InputCommands::eGameSpeak3))
    {
        value.Set(AO::InputCommands::eLeftGameSpeak);
        value.Set(AO::InputCommands::eCrouchOrRoll);
    }
    else if (aeInput.Get(::InputCommands::eGameSpeak4))
    {
        value.Set(AO::InputCommands::eLeftGameSpeak);
        value.Set(AO::InputCommands::eThrowItem);
    }
    else if (aeInput.Get(::InputCommands::eGameSpeak5))
    {
        value.Set(AO::InputCommands::eRightGameSpeak);
        value.Set(AO::InputCommands::eCrouchOrRoll);
    }
    else if (aeInput.Get(::InputCommands::eGameSpeak6))
    {
        value.Set(AO::InputCommands::eRightGameSpeak);
        value.Set(AO::InputCommands::eHop);
    }
    else if (aeInput.Get(::InputCommands::eGameSpeak7))
    {
        value.Set(AO::InputCommands::eRightGameSpeak);
        value.Set(AO::InputCommands::eThrowItem);
    }
    else if (aeInput.Get(::InputCommands::eGameSpeak8))
    {
        value.Set(AO::InputCommands::eRightGameSpeak);
        value.Set(AO::InputCommands::eDoAction);
    }
    else if (aeInput.Get(::InputCommands::eBack))
    {
        value.Set(AO::InputCommands::eBack);
    }
    else if (aeInput.Get(::InputCommands::eCheatMode))
    {
        value.Set(AO::InputCommands::eCheatMode);
    }
}

static BitField32<AO::InputCommands> AEInputCommandsToAOInputCommands(const BitField32<::InputCommands>& aeInput)
{
    BitField32<AO::InputCommands> r;
    r.Set(AO::InputCommands::eUp, aeInput.Get(::InputCommands::eUp));
    r.Set(AO::InputCommands::eRight, aeInput.Get(::InputCommands::eRight));
    r.Set(AO::InputCommands::eDown, aeInput.Get(::InputCommands::eDown));
    r.Set(AO::InputCommands::eLeft, aeInput.Get(::InputCommands::eLeft));
    r.Set(AO::InputCommands::eRun, aeInput.Get(::InputCommands::eRun));
    r.Set(AO::InputCommands::eHop, aeInput.Get(::InputCommands::eHop));
    r.Set(AO::InputCommands::eSneak, aeInput.Get(::InputCommands::eSneak));
    r.Set(AO::InputCommands::eThrowItem, aeInput.Get(::InputCommands::eThrowItem));
    r.Set(AO::InputCommands::eCrouchOrRoll, aeInput.Get(::InputCommands::eFartOrRoll));
    r.Set(AO::InputCommands::eDoAction, aeInput.Get(::InputCommands::eDoAction));
    r.Set(AO::InputCommands::eBack, aeInput.Get(::InputCommands::eBack));
    r.Set(AO::InputCommands::eUnPause_OrConfirm, aeInput.Get(::InputCommands::eUnPause_OrConfirm));
    r.Set(AO::InputCommands::ePause, aeInput.Get(::InputCommands::ePause));
    ConvertAEGamespeakAEtoAOGamespeak(r, aeInput);
    return r;
}

static BitField32<::InputCommands> AOInputCommandsToAEInputCommands(const BitField32<AO::InputCommands>& aoInput)
{
    BitField32<::InputCommands> r;
    r.Set(::InputCommands::eUp, aoInput.Get(AO::InputCommands::eUp));
    r.Set(::InputCommands::eRight, aoInput.Get(AO::InputCommands::eRight));
    r.Set(::InputCommands::eDown, aoInput.Get(AO::InputCommands::eDown));
    r.Set(::InputCommands::eLeft, aoInput.Get(AO::InputCommands::eLeft));
    r.Set(::InputCommands::eRun, aoInput.Get(AO::InputCommands::eRun));
    r.Set(::InputCommands::eHop, aoInput.Get(AO::InputCommands::eHop));
    r.Set(::InputCommands::eSneak, aoInput.Get(AO::InputCommands::eSneak));
    r.Set(::InputCommands::eThrowItem, aoInput.Get(AO::InputCommands::eThrowItem));
    r.Set(::InputCommands::eFartOrRoll, aoInput.Get(AO::InputCommands::eCrouchOrRoll));
    r.Set(::InputCommands::eDoAction, aoInput.Get(AO::InputCommands::eDoAction));
    r.Set(::InputCommands::eBack, aoInput.Get(AO::InputCommands::eBack));
    r.Set(::InputCommands::eUnPause_OrConfirm, aoInput.Get(AO::InputCommands::eUnPause_OrConfirm));
    r.Set(::InputCommands::ePause, aoInput.Get(AO::InputCommands::ePause));

    // OG issue - LCD screens says hold alt + shift which is wrong
    r.Set(::InputCommands::eChant, aoInput.Get(AO::InputCommands::eLeftGameSpeak) && aoInput.Get(AO::InputCommands::eRightGameSpeak));

    r.Set(::InputCommands::eGameSpeak1, aoInput.Get(AO::InputCommands::eLeftGameSpeak) && aoInput.Get(AO::InputCommands::eHop));
    r.Set(::InputCommands::eGameSpeak2, aoInput.Get(AO::InputCommands::eLeftGameSpeak) && aoInput.Get(AO::InputCommands::eDoAction));
    r.Set(::InputCommands::eGameSpeak3, aoInput.Get(AO::InputCommands::eLeftGameSpeak) && aoInput.Get(AO::InputCommands::eCrouchOrRoll));
    r.Set(::InputCommands::eGameSpeak4, aoInput.Get(AO::InputCommands::eLeftGameSpeak) && aoInput.Get(AO::InputCommands::eThrowItem));
    r.Set(::InputCommands::eGameSpeak5, aoInput.Get(AO::InputCommands::eRightGameSpeak) && aoInput.Get(AO::InputCommands::eDoAction));
    r.Set(::InputCommands::eGameSpeak6, aoInput.Get(AO::InputCommands::eRightGameSpeak) && aoInput.Get(AO::InputCommands::eHop));
    r.Set(::InputCommands::eGameSpeak7, aoInput.Get(AO::InputCommands::eRightGameSpeak) && aoInput.Get(AO::InputCommands::eThrowItem));
    r.Set(::InputCommands::eGameSpeak8, aoInput.Get(AO::InputCommands::eRightGameSpeak) && aoInput.Get(AO::InputCommands::eCrouchOrRoll));
    r.Set(::InputCommands::eCheatMode, aoInput.Get(AO::InputCommands::eCheatMode));

    // needed for remapping Speak I and Speak II on controllers
    r.Set(::InputCommands::eLeftGameSpeak, aoInput.Get(AO::InputCommands::eLeftGameSpeak));
    r.Set(::InputCommands::eRightGameSpeak, aoInput.Get(AO::InputCommands::eRightGameSpeak));

    return r;
}

static const char_type* AEInputCommandToAEInputString(::InputCommands input_command)
{
    if (input_command & ::InputCommands::eUp)
    {
        return kUp;
    }

    if (input_command & ::InputCommands::eDown)
    {
        return kDown;
    }

    if (input_command & ::InputCommands::eLeft)
    {
        return kLeft;
    }

    if (input_command & ::InputCommands::eRight)
    {
        return kRight;
    }

    if (input_command & ::InputCommands::eRun)
    {
        return kRun;
    }

    if (input_command & ::InputCommands::eHop)
    {
        return kJump;
    }

    if (input_command & ::InputCommands::eSneak)
    {
        return kSneak;
    }

    if (input_command & ::InputCommands::eThrowItem)
    {
        return kThrow;
    }

    if (input_command & ::InputCommands::eFartOrRoll)
    {
        return kFart; // TODO: wrong ?
    }

    if (input_command & ::InputCommands::eDoAction)
    {
        return kAction;
    }

    if (input_command & ::InputCommands::eChant)
    {
        return kChant;
    }

    if (input_command & ::InputCommands::eLeftGameSpeak)
    {
        return kSpeak1;
    }

    if (input_command & ::InputCommands::eRightGameSpeak)
    {
        return kSpeak2;
    }

    if (input_command & ::InputCommands::eUnPause_OrConfirm)
    {
        return kConfirm;
    }

    if (input_command & ::InputCommands::eBack)
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


static BitField32<::InputCommands> MakeAEInputBits(u32 bits)
{
    BitField32<::InputCommands> r;
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
        mPads[i].mPreviousInput = mPads[i].mRawInput;
    }

    // Do AE input reading
    ::Input().Update(gameAutoPlayer);

    // Convert from AE bit flags to AO bit flags
    mPads[0].mRawInput = static_cast<u16>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().mPads[0].mRawInput)).Raw().all);

    // TODO: This one probably needs its own conversion
    mPads[0].mDir = ::Input().mPads[0].mDir;

    mPads[0].mPreviousInput = static_cast<u16>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().mPads[0].mPreviousInput)).Raw().all);
    mPads[0].mPressed = static_cast<u16>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().mPads[0].mPressed)).Raw().all);
    mPads[0].mReleased = static_cast<u16>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().mPads[0].mReleased)).Raw().all);

    // Handle demo input (AO impl)
    if (mbDemoPlaying & 1)
    {
        // Stop if any button on any pad is pressed
        if (mPads[sCurrentControllerIndex].mRawInput)
        {
            mbDemoPlaying &= ~1u;
            return;
        }

        if (static_cast<s32>(sGnFrame) >= mCommandDuration)
        {
            const u32 command = (*mpDemoRes)[mDemoCommandIndex++];
            mCommand = command >> 16;
            mCommandDuration = sGnFrame + (command & 0xFFFF);

            // End demo/quit command
            if (command & 0x8000)
            {
                mbDemoPlaying &= ~1u;
            }
        }

        // Will do nothing if we hit the end command..
        if (mbDemoPlaying & 1)
        {
            mPads[0].mRawInput = static_cast<u16>(mCommand);
        }

        for (s32 i = 0; i < 2; i++)
        {
            mPads[i].mReleased = ~mPads[i].mRawInput & mPads[i].mPreviousInput;
            mPads[i].mPressed = ~mPads[i].mPreviousInput & mPads[i].mRawInput;
            mPads[i].mDir = byte_4BB428[(mPads[i].mRawInput >> 12) & 0xF];
            mPads[i].field_3 = byte_4BB428[(mPads[i].mRawInput >> 4) & 0xF];
        }

        return;
    }
}

void InputObject::Shutdown()
{
}


void InputObject::SetDemoRes(u32** ppDemoRes)
{
    mDemoCommandIndex = 2051;
    mpDemoRes = ppDemoRes;
    mbDemoPlaying |= 1u;
    mCommandDuration = 0;
}

s32 InputObject::IsDemoPlaying()
{
    return mbDemoPlaying & 1;
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


bool InputObject::IsJoyStickEnabled() const
{
    return ::Input().IsJoyStickEnabled();
}

void InputObject::SetJoyStickEnabled(bool enabled) const
{
    return ::Input().SetJoyStickEnabled(enabled);
}

bool InputObject::IsAnyHeld(u32 command) const
{
    return IsAnyHeld(PadIndex::Active, command);
}

bool InputObject::IsAnyHeld(PadIndex padIx, u32 command) const
{
    return (mPads[PadIndexToInt(padIx)].mRawInput & command) != 0;
}

bool InputObject::IsAnyPressed(u32 command) const
{
    return IsAnyPressed(PadIndex::Active, command);
}

bool InputObject::IsAnyPressed(PadIndex padIx, u32 command) const
{
    return (mPads[PadIndexToInt(padIx)].mPressed & command) != 0;
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

u16 InputObject::GetHeld() const
{
    return GetHeld(PadIndex::Active);
}

u16 InputObject::GetHeld(PadIndex padIx) const
{
    return mPads[PadIndexToInt(padIx)].mRawInput;
}

u16 InputObject::GetPressed() const
{
    return GetPressed(PadIndex::Active);
}

u16 InputObject::GetPressed(PadIndex padIx) const
{
    return Input().mPads[PadIndexToInt(padIx)].mPressed;
}

u32 InputObject::Input_Read_Pad(u32 padIdx)
{
    return ::Input_Read_Pad(padIdx);
}

u16 InputObject::GetReleased() const
{
    return Input().mPads[sCurrentControllerIndex].mReleased;
}

bool Input_IsChanting()
{
    return ::Input_IsChanting();
}

void Input_SetKeyState(s32 key, s8 bIsDown)
{
    ::Input_SetKeyState(key, bIsDown);
}

s8 Input_IsVKPressed(s32 key)
{
    return Input_IsVKPressed_4EDD40(key);
}

// from the MainMenu class
extern s32 gJoystickAvailable;

void Input_Init()
{
    ::Input_Init();

    if (Input().IsJoyStickAvailable())
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
    const auto aeBits = static_cast<::InputCommands>(AOInputCommandsToAEInputCommands(MakeAOInputBits(inputCommand)).Raw().all);
    s32 controller_type = forceKeyboardLookupIfGamepadFails ? 2 : 1;

    return ::Input_GetButtonString_492530(
        AEInputCommandToAEInputString(aeBits),
        Input().IsJoyStickEnabled() ? controller_type : 0);
}

const char_type* Input_GetButtonString_44F1C0(InputCommands inputCommand)
{
    return Input_GetButtonString(inputCommand);
}

s32 Input_Remap(InputCommands inputCmd)
{
    return Input_Remap_492680(static_cast<::InputCommands>(AOInputCommandsToAEInputCommands(MakeAOInputBits(inputCmd)).Raw().all));
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

bool InputObject::IsJoyStickAvailable()
{
    return ::Input().IsJoyStickAvailable();
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
    //if (Input().JoyStickEnabled())
    //{
    //    fprintf(iniFileHandle, "controller = Game Pad\n");

    //}
    //else
    //{
    //    fprintf(iniFileHandle, "controller = Keyboard\n");
    //}
    //
    //const auto oldJoystickEnabled = Input().JoyStickEnabled();

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
