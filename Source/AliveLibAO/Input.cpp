#include "stdafx_ao.h"
#include "Input.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "../AliveLibAE/Input.hpp"
#include "BitField.hpp"
#include "Sys.hpp"

namespace AO {

ALIVE_VAR(1, 0x5009E8, InputObject, sInputObject_5009E8, {});
ALIVE_VAR(1, 0x5076B8, u16, sCurrentControllerIndex_5076B8, 0);
ALIVE_VAR(1, 0x508A60, s32, sJoystickEnabled_508A60, 0);
ALIVE_VAR(1, 0x9F7710, u8, sInputEnabled_9F7710, 0);
ALIVE_VAR(1, 0xA8A604, DWORD, sLastPressedKey_A8A604, 0);

const InputCommands sInputKey_Right_4C6590 = eRight;
const InputCommands sInputKey_Left_4C6594 = eLeft;
const InputCommands sInputKey_Up_4C6598 = eUp;
const InputCommands sInputKey_Down_4C659C = eDown;
const InputCommands sInputKey_Hop_4C65A0 = eHop;
const InputCommands sInputKey_DoAction_4C65A4 = eDoAction;
const InputCommands sInputKey_Run_4C65A8 = eRun;
const InputCommands sInputKey_Sneak_4C65AC = eSneak;
const InputCommands sInputKey_FartRoll_4C65B0 = eCrouchOrRoll;
const InputCommands sInputKey_ThrowItem_4C65B4 = eThrowItem;

const InputCommands sInputKey_LeftGameSpeakEnabler_4C65B8 = eLeftGamespeak;
const InputCommands sInputKey_GameSpeak1_4C65C8 = eHop;
const InputCommands sInputKey_GameSpeak2_4C65BC = eDoAction;
const InputCommands sInputKey_GameSpeak3_4C65C0 = eThrowItem;
const InputCommands sInputKey_GameSpeak4_4C65C4 = eCrouchOrRoll;

const InputCommands sInputKey_RightGameSpeakEnabler_4C65DC = eRightGameSpeak;
const InputCommands sInputKey_GameSpeak5_4C65EC = eCrouchOrRoll;
const InputCommands sInputKey_GameSpeak6_4C65E8 = eHop;
const InputCommands sInputKey_GameSpeak7_4C65E4 = eThrowItem;
const InputCommands sInputKey_GameSpeak8_4C65E0 = eDoAction;

EXPORT void InputObject::InitPad_4331A0(u32 /*padCount*/)
{
    for (PSX_Pad& pad : field_0_pads)
    {
        pad = {};
    }

    field_20_demo_playing &= ~1u;

    Input_Pads_Reset_4FA960();
    Input_EnableInput_4EDDD0();
}

ALIVE_ARY(1, 0x507778, u8, 64, sPad1Buffer_507778, {});
ALIVE_ARY(1, 0x507738, u8, 64, sPad2Buffer_507738, {});

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
        value.Set(AO::InputCommands::eDoAction);
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
        value.Set(AO::InputCommands::eCrouchOrRoll);
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

const s8* AEInputCommandToAEInputString(::InputCommands::Enum input_command)
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


static BitField32<::InputCommands::Enum> MakeAEInputBits(DWORD bits)
{
    BitField32<::InputCommands::Enum> r;
    r.Raw().all = bits;
    return r;
}

static BitField32<AO::InputCommands> MakeAOInputBits(DWORD bits)
{
    BitField32<AO::InputCommands> r;
    r.Raw().all = bits;
    return r;
}

EXPORT void InputObject::Update_433250()
{
    const u8 byte_4BB428[16] = { 0u, 64u, 0u, 32u, 192u, 0u, 224u, 0u, 128u, 96u, 0u, 0u, 160u, 0u, 0u, 0u };

    for (s32 i = 0; i < 2; i++)
    {
        field_0_pads[i].field_A_prev_dir = field_0_pads[i].field_2_dir;
        field_0_pads[i].field_B = field_0_pads[i].field_3;
        field_0_pads[i].field_4_previously_pressed = field_0_pads[i].field_0_pressed;
    }

    // Do AE input reading
    ::Input().Update_45F040();

    // Convert from AE bit flags to AO bit flags
    field_0_pads[0].field_0_pressed = static_cast<u16>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().field_0_pads[0].field_0_pressed)).Raw().all);

    // TODO: This one probably needs its own conversion
    field_0_pads[0].field_2_dir = ::Input().field_0_pads[0].field_4_dir;

    field_0_pads[0].field_4_previously_pressed = static_cast<u16>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().field_0_pads[0].field_8_previous)).Raw().all);
    field_0_pads[0].field_6_held = static_cast<u16>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().field_0_pads[0].field_C_held)).Raw().all);
    field_0_pads[0].field_8_released = static_cast<u16>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().field_0_pads[0].field_10_released)).Raw().all);

    // Handle demo input (AO impl)
    if (field_20_demo_playing & 1)
    {
        // Stop if any button on any pad is pressed
        if (field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed)
        {
            field_20_demo_playing &= ~1u;
            return;
        }

        if (static_cast<s32>(gnFrameCount_507670) >= field_28_command_duration)
        {
            const DWORD command = (*field_18_demo_res)[field_1C_demo_command_index++];
            field_24_command = command >> 16;
            field_28_command_duration = gnFrameCount_507670 + command & 0xFFFF;

            // End demo/quit command
            if (command & 0x8000)
            {
                field_20_demo_playing &= ~1u;
            }
        }

        // Will do nothing if we hit the end command..
        if (field_20_demo_playing & 1)
        {
            field_0_pads[0].field_0_pressed = static_cast<u16>(field_24_command);
        }

        for (s32 i = 0; i < 2; i++)
        {
            field_0_pads[i].field_8_released = ~field_0_pads[i].field_0_pressed & field_0_pads[i].field_4_previously_pressed;
            field_0_pads[i].field_6_held = ~field_0_pads[i].field_4_previously_pressed & field_0_pads[i].field_0_pressed;
            field_0_pads[i].field_2_dir = byte_4BB428[(field_0_pads[i].field_0_pressed >> 12) & 0xF];
            field_0_pads[i].field_3 = byte_4BB428[(field_0_pads[i].field_0_pressed >> 4) & 0xF];
        }

        return;
    }

    // Original AO impl
    //for (s32 i = 0; i < 2; i++)
    //{
    //    field_0_pads[i].field_A_prev_dir = field_0_pads[i].field_2_dir;
    //    field_0_pads[i].field_B = field_0_pads[i].field_3;
    //    field_0_pads[i].field_4_previously_pressed = field_0_pads[i].field_0_pressed;
    //}

    //if (sPad1Buffer_507778[0])
    //{
    //    field_0_pads[0].field_0_pressed = 0;
    //}
    //else
    //{
    //    field_0_pads[0].field_0_pressed = ~(sPad1Buffer_507778[3] + (sPad1Buffer_507778[2] << 8));
    //}

    //if (sPad2Buffer_507738[0])
    //{
    //    field_0_pads[1].field_0_pressed = 0;
    //}
    //else
    //{
    //    field_0_pads[1].field_0_pressed = ~(sPad2Buffer_507738[3] + (sPad2Buffer_507738[2] << 8));
    //}

    //if (field_20_demo_playing & 1)
    //{
    //    // Stop if any button on any pad is pressed
    //    if (field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed)
    //    {
    //        field_20_demo_playing &= ~1u;
    //        return;
    //    }

    //    if (static_cast<s32>(gnFrameCount_507670) >= field_28_command_duration)
    //    {
    //        const DWORD command = (*field_18_demo_res)[field_1C_demo_command_index++];
    //        field_24_command = command >> 16;
    //        field_28_command_duration = gnFrameCount_507670 + command & 0xFFFF;

    //        // End demo/quit command
    //        if (command & 0x8000)
    //        {
    //            field_20_demo_playing &= ~1u;
    //        }
    //    }

    //    // Will do nothing if we hit the end command..
    //    if (field_20_demo_playing & 1)
    //    {
    //        field_0_pads[0].field_0_pressed = static_cast<u16>(field_24_command);
    //    }
    //}

    //for (s32 i = 0; i < 2; i++)
    //{
    //    field_0_pads[i].field_8_released = ~field_0_pads[i].field_0_pressed & field_0_pads[i].field_4_previously_pressed;
    //    field_0_pads[i].field_6_held = ~field_0_pads[i].field_4_previously_pressed & field_0_pads[i].field_0_pressed;
    //    field_0_pads[i].field_2_dir = byte_4BB428[(field_0_pads[i].field_0_pressed >> 12) & 0xF];
    //    field_0_pads[i].field_3 = byte_4BB428[(field_0_pads[i].field_0_pressed >> 4) & 0xF];
    //}
}

EXPORT void CC InputObject::Shutdown_433230()
{
}


void InputObject::SetDemoRes_433470(DWORD** ppDemoRes)
{
    field_1C_demo_command_index = 2051;
    field_18_demo_res = ppDemoRes;
    field_20_demo_playing |= 1u;
    field_28_command_duration = 0;
}

s32 InputObject::IsDemoPlaying_4334A0()
{
    return field_20_demo_playing & 1;
}

static s32 PadIndexToInt(InputObject::PadIndex idx)
{
    switch (idx)
    {
    case InputObject::PadIndex::Active:
        return sCurrentControllerIndex_5076B8;

    case InputObject::PadIndex::Second:
        return 1;

    case InputObject::PadIndex::First:
    default:
        return 0;
    }
}

InputObject::PadIndex InputObject::CurrentController() const
{
    if (sCurrentControllerIndex_5076B8 == 0)
    {
        return PadIndex::First;
    }
    return PadIndex::Second;
}


void InputObject::SetCurrentController(PadIndex padIdx)
{
    sCurrentControllerIndex_5076B8 = padIdx == InputObject::PadIndex::First ? 0 : 1;
}


bool InputObject::JoyStickEnabled() const
{
    return Input_JoyStickEnabled();
}

bool InputObject::IsAnyPressed(DWORD command) const
{
    return IsAnyPressed(PadIndex::Active, command);
}

bool InputObject::IsAnyPressed(PadIndex padIx, DWORD command) const
{
    return (field_0_pads[PadIndexToInt(padIx)].field_0_pressed & command) != 0;
}

bool InputObject::IsAnyHeld(DWORD command) const
{
    return IsAnyHeld(PadIndex::Active, command);
}

bool InputObject::IsAnyHeld(PadIndex padIx, DWORD command) const
{
    return (field_0_pads[PadIndexToInt(padIx)].field_6_held & command) != 0;
}

bool InputObject::IsAnyReleased(DWORD command) const
{
    return IsAnyReleased(PadIndex::Active, command);
}

bool InputObject::IsAnyReleased(PadIndex padIx, DWORD command) const
{
    return (field_0_pads[PadIndexToInt(padIx)].field_8_released & command) != 0;
}

u8 InputObject::Dir() const
{
    return sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_2_dir >> 5;
}


bool InputObject::IsAllPressed(DWORD commands) const
{
    return IsAllPressed(PadIndex::Active, commands);
}

bool InputObject::IsAllPressed(PadIndex padIx, DWORD commands) const
{
    return (field_0_pads[PadIndexToInt(padIx)].field_0_pressed & commands) == commands;
}


bool InputObject::IsAllHeld(DWORD commands) const
{
    return (field_0_pads[sCurrentControllerIndex_5076B8].field_6_held & commands) == commands;
}

u16 InputObject::Pressed() const
{
    return Pressed(PadIndex::Active);
}

u16 InputObject::Pressed(PadIndex padIx) const
{
    return field_0_pads[PadIndexToInt(padIx)].field_0_pressed;
}

u16 InputObject::Held() const
{
    return Held(PadIndex::Active);
}

u16 InputObject::Held(PadIndex padIx) const
{
    return sInputObject_5009E8.field_0_pads[PadIndexToInt(padIx)].field_6_held;
}

u16 InputObject::Released() const
{
    return sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_8_released;
}

BOOL CC Input_IsChanting_4334C0()
{
    return Input_IsChanting_45F260();
}

void CC Input_SetKeyState_48E610(s32 key, s8 bIsDown)
{
    Input_SetKeyState_4EDD80(key, bIsDown);
}

EXPORT s8 CC Input_IsVKPressed_48E5D0(s32 key)
{
    return Input_IsVKPressed_4EDD40(key);
}

// from the MainMenu class
ALIVE_VAR_EXTERN(s32, gJoystickAvailable_5079A4);

EXPORT void CC Input_Init_44EB60()
{
    Input_Init_491BC0();

    if (Input_JoyStickAvailable())
    {
        gJoystickAvailable_5079A4 = 1;
    }
}

EXPORT void Input_DisableInput_48E690()
{
    Input_DisableInputForPauseMenuAndDebug_4EDDC0();
}

// Only really needed for SHIFT being pressed for the AO ddcheat, may as well just remove the requirement
// for shift to be pressed because it is a pain anyway.
EXPORT void Input_GetCurrentKeyStates_48E630()
{
}

void Input_InitKeyStateArray_48E5F0()
{
    Input_InitKeyStateArray_4EDD60();
}

EXPORT const s8* CC Input_GetButtonString_44F1C0(InputCommands input_command, bool forceKeyboardLookupIfGamepadFails)
{
    const auto aeBits = static_cast<::InputCommands::Enum>(AOInputCommandsToAEInputCommands(MakeAOInputBits(input_command)).Raw().all);
    s32 controller_type = forceKeyboardLookupIfGamepadFails ? 2 : 1;
    return ::Input_GetButtonString_492530(
        AEInputCommandToAEInputString(aeBits),
        Input_JoyStickEnabled() ? controller_type : 0
    );
}

EXPORT s32 CC Input_Remap_44F300(InputCommands inputCmd)
{
    return Input_Remap_492680(static_cast<::InputCommands::Enum>(AOInputCommandsToAEInputCommands(MakeAOInputBits(inputCmd)).Raw().all));
}

EXPORT s8 Input_GetLastPressedKey_44F2C0()
{
    // AE impl
    return static_cast<s8>(::Input_GetLastPressedKey_492610());

    // AO impl
    //if (!Sys_IsAnyKeyDown_48E6C0())
    //{
    //    return 0;
    //}

    //const s8 result = static_cast<s8>(sLastPressedKey_A8A604);
    //sIsAKeyDown_A8A600 = 0;
    //sLastPressedKey_A8A604 = 0;
    //return result;
}

EXPORT s32 Input_Enable_48E6A0()
{
    // AE impl
    ::Input_EnableInput_4EDDD0();
    return 0;

    // AO impl
    //sInputEnabled_9F7710 = 1;
    //return 0;
}

EXPORT void Input_Reset_44F2F0()
{
    // Funcs below call AE impls in standalone
    Input_Enable_48E6A0();
    Input_InitKeyStateArray_48E5F0();
}


bool Input_JoyStickEnabled()
{
    // Use AE var
    return ::Input_JoyStickEnabled();

    // Use AO var
    //return sJoystickEnabled_508A60 ? true : false;
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
    //sJoystickEnabled_508A60 = enabled;
}

ALIVE_VAR(1, 0x508A64, DWORD, dword_508A64, 0);

EXPORT s32 CC Input_SaveSettingsIni_44F460()
{
    // Call AE func both in standalone and DLL
    Input_SaveSettingsIni_Common(true);
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
    return sInputObject_5009E8;
}

}
