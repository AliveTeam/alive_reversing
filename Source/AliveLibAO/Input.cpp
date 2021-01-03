#include "stdafx_ao.h"
#include "Input.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "../AliveLibAE/Input.hpp"
#include "BitField.hpp"
#include "Sys.hpp"

namespace AO {

ALIVE_VAR(1, 0x5009E8, InputObject, sInputObject_5009E8, {});
ALIVE_VAR(1, 0x5076B8, unsigned __int16, sCurrentControllerIndex_5076B8, 0);
ALIVE_VAR(1, 0x508A60, int, sJoystickEnabled_508A60, 0);
ALIVE_VAR(1, 0x9F7710, BYTE, sInputEnabled_9F7710, 0);
ALIVE_VAR(1, 0xA8A604, DWORD, sLastPressedKey_A8A604, 0);

const InputCommands sInputKey_Right_4C6590 = eRight;
const InputCommands sInputKey_Left_4C6594 = eLeft;
const InputCommands sInputKey_Up_4C6598 = eUp;
const InputCommands sInputKey_Down_4C659C = eDown;
const InputCommands sInputKey_Hop_4C65A0 = eHop;
const InputCommands sInputKey_DoAction_4C65A4 = eDoAction;
const InputCommands sInputKey_Run_4C65A8 = eRun;
const InputCommands sInputKey_Sneak_4C65AC = eSneak;
const InputCommands sInputKey_FartRoll_4C65B0 = eFartOrRoll;
const InputCommands sInputKey_ThrowItem_4C65B4 = eThrowItem;

const InputCommands sInputKey_LeftGameSpeakEnabler_4C65B8 = eLeftGamespeak;
const InputCommands sInputKey_GameSpeak1_4C65C8 = eHop;
const InputCommands sInputKey_GameSpeak2_4C65BC = eDoAction;
const InputCommands sInputKey_GameSpeak3_4C65C0 = eThrowItem;
const InputCommands sInputKey_GameSpeak4_4C65C4 = eFartOrRoll;

const InputCommands sInputKey_RightGameSpeakEnabler_4C65DC = eRightGameSpeak;
const InputCommands sInputKey_GameSpeak5_4C65EC = eFartOrRoll;
const InputCommands sInputKey_GameSpeak6_4C65E8 = eHop;
const InputCommands sInputKey_GameSpeak7_4C65E4 = eThrowItem;
const InputCommands sInputKey_GameSpeak8_4C65E0 = eDoAction;

EXPORT void InputObject::InitPad_4331A0(unsigned int /*padCount*/)
{
    NOT_IMPLEMENTED();

    for (PSX_Pad& pad : field_0_pads)
    {
        pad = {};
    }

    field_20_demo_playing &= ~1u;

    Input_Pads_Reset_4FA960();
    Input_EnableInput_4EDDD0();
}

ALIVE_ARY(1, 0x507778, BYTE, 64, sPad1Buffer_507778, {});
ALIVE_ARY(1, 0x507738, BYTE, 64, sPad2Buffer_507738, {});

static void ConvertAEGamespeakAEtoAOGamespeak(BitField32<InputCommands>& value, const BitField32<::InputCommands>& aeInput)
{
    if (aeInput.Get(::InputCommands::eGameSpeak1))
    {
        value.Set(InputCommands::eLeftGamespeak);
        value.Set(InputCommands::eHop);
    }
    else if (aeInput.Get(::InputCommands::eGameSpeak2))
    {
        value.Set(InputCommands::eLeftGamespeak);
        value.Set(InputCommands::eDoAction);
    }
    else if (aeInput.Get(::InputCommands::eGameSpeak3))
    {
        value.Set(InputCommands::eLeftGamespeak);
        value.Set(InputCommands::eFartOrRoll);
    }
    else if (aeInput.Get(::InputCommands::eGameSpeak4))
    {
        value.Set(InputCommands::eLeftGamespeak);
        value.Set(InputCommands::eThrowItem);
    }
    else if (aeInput.Get(::InputCommands::eGameSpeak5))
    {
        value.Set(InputCommands::eRightGameSpeak);
        value.Set(InputCommands::eDoAction);
    }
    else if (aeInput.Get(::InputCommands::eGameSpeak6))
    {
        value.Set(InputCommands::eRightGameSpeak);
        value.Set(InputCommands::eHop);
    }
    else if (aeInput.Get(::InputCommands::eGameSpeak7))
    {
        value.Set(InputCommands::eRightGameSpeak);
        value.Set(InputCommands::eThrowItem);
    }
    else if (aeInput.Get(::InputCommands::eGameSpeak8))
    {
        value.Set(InputCommands::eRightGameSpeak);
        value.Set(InputCommands::eFartOrRoll);
    }
    else if (aeInput.Get(::InputCommands::eBack))
    {
        value.Set(InputCommands::eBack);
    }
    else if (aeInput.Get(::InputCommands::eCheatMode))
    {
        value.Set(InputCommands::eCheatMode);
    }
}

static BitField32<InputCommands> AEInputCommandsToAOInputCommands(const BitField32<::InputCommands>& aeInput)
{
    BitField32<InputCommands> r;
    r.Set(InputCommands::eUp, aeInput.Get(::InputCommands::eUp));
    r.Set(InputCommands::eRight, aeInput.Get(::InputCommands::eRight));
    r.Set(InputCommands::eDown, aeInput.Get(::InputCommands::eDown));
    r.Set(InputCommands::eLeft, aeInput.Get(::InputCommands::eLeft));
    r.Set(InputCommands::eRun, aeInput.Get(::InputCommands::eRun));
    r.Set(InputCommands::eHop, aeInput.Get(::InputCommands::eHop));
    r.Set(InputCommands::eSneak, aeInput.Get(::InputCommands::eSneak));
    r.Set(InputCommands::eThrowItem, aeInput.Get(::InputCommands::eThrowItem));
    r.Set(InputCommands::eFartOrRoll, aeInput.Get(::InputCommands::eFartOrRoll));
    r.Set(InputCommands::eDoAction, aeInput.Get(::InputCommands::eDoAction));
    r.Set(InputCommands::eBack, aeInput.Get(::InputCommands::eBack));
    ConvertAEGamespeakAEtoAOGamespeak(r, aeInput);
    return r;
}

static BitField32<::InputCommands> AOInputCommandsToAEInputCommands(const BitField32<InputCommands>& aoInput)
{
     BitField32<::InputCommands> r;
     r.Set(::InputCommands::eUp, aoInput.Get(InputCommands::eUp));
     r.Set(::InputCommands::eRight, aoInput.Get(InputCommands::eRight));
     r.Set(::InputCommands::eDown, aoInput.Get(InputCommands::eDown));
     r.Set(::InputCommands::eLeft, aoInput.Get(InputCommands::eLeft));
     r.Set(::InputCommands::eRun, aoInput.Get(InputCommands::eRun));
     r.Set(::InputCommands::eHop, aoInput.Get(InputCommands::eHop));
     r.Set(::InputCommands::eSneak, aoInput.Get(InputCommands::eSneak));
     r.Set(::InputCommands::eThrowItem, aoInput.Get(InputCommands::eThrowItem));
     r.Set(::InputCommands::eFartOrRoll, aoInput.Get(InputCommands::eFartOrRoll));
     r.Set(::InputCommands::eDoAction, aoInput.Get(InputCommands::eDoAction));
     r.Set(::InputCommands::eBack, aoInput.Get(InputCommands::eBack));

     // OG issue - LCD screens says hold alt + shift which is wrong
     r.Set(::InputCommands::eChant, aoInput.Get(InputCommands::eLeftGamespeak) && aoInput.Get(InputCommands::eRightGameSpeak));

     r.Set(::InputCommands::eGameSpeak1, aoInput.Get(InputCommands::eLeftGamespeak) && aoInput.Get(InputCommands::eHop));
     r.Set(::InputCommands::eGameSpeak2, aoInput.Get(InputCommands::eLeftGamespeak) && aoInput.Get(InputCommands::eDoAction));
     r.Set(::InputCommands::eGameSpeak3, aoInput.Get(InputCommands::eLeftGamespeak) && aoInput.Get(InputCommands::eFartOrRoll));
     r.Set(::InputCommands::eGameSpeak4, aoInput.Get(InputCommands::eLeftGamespeak) && aoInput.Get(InputCommands::eThrowItem));
     r.Set(::InputCommands::eGameSpeak5, aoInput.Get(InputCommands::eRightGameSpeak) && aoInput.Get(InputCommands::eDoAction));
     r.Set(::InputCommands::eGameSpeak6, aoInput.Get(InputCommands::eRightGameSpeak) && aoInput.Get(InputCommands::eHop));
     r.Set(::InputCommands::eGameSpeak7, aoInput.Get(InputCommands::eRightGameSpeak) && aoInput.Get(InputCommands::eThrowItem));
     r.Set(::InputCommands::eGameSpeak8, aoInput.Get(InputCommands::eRightGameSpeak) && aoInput.Get(InputCommands::eFartOrRoll));
     r.Set(::InputCommands::eCheatMode, aoInput.Get(InputCommands::eCheatMode));

     return r;
}

const char* AEInputCommandToAEInputString(::InputCommands input_command)
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

    if (input_command & ::InputCommands::eSpeak1)
    {
        return kSpeak1;
    }

    if (input_command & ::InputCommands::eSpeak2)
    {
        return kSpeak2;
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


static BitField32<::InputCommands> MakeAEInputBits(DWORD bits)
{
    BitField32<::InputCommands> r;
    r.Raw().all = bits;
    return r;
}

static BitField32<InputCommands> MakeAOInputBits(DWORD bits)
{
    BitField32<InputCommands> r;
    r.Raw().all = bits;
    return r;
}

EXPORT void InputObject::Update_433250()
{
    const BYTE byte_4BB428[16] = { 0u, 64u, 0u, 32u, 192u, 0u, 224u, 0u, 128u, 96u, 0u, 0u, 160u, 0u, 0u, 0u };

    if (!RunningAsInjectedDll())
    {
        for (int i = 0; i < 2; i++)
        {
            field_0_pads[i].field_A_prev_dir = field_0_pads[i].field_2_dir;
            field_0_pads[i].field_B = field_0_pads[i].field_3;
            field_0_pads[i].field_4_previously_pressed = field_0_pads[i].field_0_pressed;
        }

        // Do AE input reading
        ::Input().Update_45F040();

        // Convert from AE bit flags to AO bit flags
        field_0_pads[0].field_0_pressed = static_cast<unsigned short>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().field_0_pads[0].field_0_pressed)).Raw().all);

        // TODO: This one probably needs its own conversion
        field_0_pads[0].field_2_dir = ::Input().field_0_pads[0].field_4_dir;

        field_0_pads[0].field_4_previously_pressed = static_cast<unsigned short>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().field_0_pads[0].field_8_previous)).Raw().all);
        field_0_pads[0].field_6_held = static_cast<unsigned short>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().field_0_pads[0].field_C_held)).Raw().all);
        field_0_pads[0].field_8_released = static_cast<unsigned short>(AEInputCommandsToAOInputCommands(MakeAEInputBits(::Input().field_0_pads[0].field_10_released)).Raw().all);

        // Handle demo in put (AO impl)
        if (field_20_demo_playing & 1)
        {
            // Stop if any button on any pad is pressed
            if (field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed)
            {
                field_20_demo_playing &= ~1u;
                return;
            }

            if (static_cast<int>(gnFrameCount_507670) >= field_28_command_duration)
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
                field_0_pads[0].field_0_pressed = static_cast<unsigned short>(field_24_command);
            }
        }

        for (int i = 0; i < 2; i++)
        {
            field_0_pads[i].field_8_released = ~field_0_pads[i].field_0_pressed & field_0_pads[i].field_4_previously_pressed;
            field_0_pads[i].field_6_held = ~field_0_pads[i].field_4_previously_pressed & field_0_pads[i].field_0_pressed;
            field_0_pads[i].field_2_dir = byte_4BB428[(field_0_pads[i].field_0_pressed >> 12) & 0xF];
            field_0_pads[i].field_3 = byte_4BB428[(field_0_pads[i].field_0_pressed >> 4) & 0xF];
        }

        return;
    }

    // Original AO impl

    for (int i = 0; i < 2; i++)
    {
        field_0_pads[i].field_A_prev_dir = field_0_pads[i].field_2_dir;
        field_0_pads[i].field_B = field_0_pads[i].field_3;
        field_0_pads[i].field_4_previously_pressed = field_0_pads[i].field_0_pressed;
    }

    if (sPad1Buffer_507778[0])
    {
        field_0_pads[0].field_0_pressed = 0;
    }
    else
    {
        field_0_pads[0].field_0_pressed = ~(sPad1Buffer_507778[3] + (sPad1Buffer_507778[2] << 8));
    }

    if (sPad2Buffer_507738[0])
    {
        field_0_pads[1].field_0_pressed = 0;
    }
    else
    {
        field_0_pads[1].field_0_pressed = ~(sPad2Buffer_507738[3] + (sPad2Buffer_507738[2] << 8));
    }

    if (field_20_demo_playing & 1)
    {
        // Stop if any button on any pad is pressed
        if (field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed)
        {
            field_20_demo_playing &= ~1u;
            return;
        }

        if (static_cast<int>(gnFrameCount_507670) >= field_28_command_duration)
        {
            const DWORD command = (*field_18_demo_res)[field_1C_demo_command_index++];
            field_24_command = command >> 16;
            field_28_command_duration =  gnFrameCount_507670 + command & 0xFFFF;
           
            // End demo/quit command
            if (command & 0x8000)
            {
                field_20_demo_playing &= ~1u;
            }
        }

        // Will do nothing if we hit the end command..
        if (field_20_demo_playing & 1)
        {
            field_0_pads[0].field_0_pressed = static_cast<unsigned short>(field_24_command);
        }
    }

    for (int i = 0; i < 2; i++)
    {
        field_0_pads[i].field_8_released = ~field_0_pads[i].field_0_pressed & field_0_pads[i].field_4_previously_pressed;
        field_0_pads[i].field_6_held = ~field_0_pads[i].field_4_previously_pressed & field_0_pads[i].field_0_pressed;
        field_0_pads[i].field_2_dir = byte_4BB428[(field_0_pads[i].field_0_pressed >> 12) & 0xF];
        field_0_pads[i].field_3 = byte_4BB428[(field_0_pads[i].field_0_pressed >> 4) & 0xF];
    }
}

EXPORT void CC InputObject::Shutdown_433230()
{
    NOT_IMPLEMENTED();
}


void InputObject::SetDemoRes_433470(DWORD** ppDemoRes)
{
    field_1C_demo_command_index = 2051;
    field_18_demo_res = ppDemoRes;
    field_20_demo_playing |= 1u;
    field_28_command_duration = 0;
}

int InputObject::IsDemoPlaying_4334A0()
{
    return field_20_demo_playing & 1;
}

static int PadIndexToInt(InputObject::PadIndex idx)
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

unsigned __int8 InputObject::Dir() const
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

unsigned __int16 InputObject::Pressed() const
{
    return Pressed(PadIndex::Active);
}

unsigned __int16 InputObject::Pressed(PadIndex padIx) const
{
    return field_0_pads[PadIndexToInt(padIx)].field_0_pressed;
}

unsigned short InputObject::Held() const
{
    return Held(PadIndex::Active);
}

unsigned short InputObject::Held(PadIndex padIx) const
{
    return sInputObject_5009E8.field_0_pads[PadIndexToInt(padIx)].field_6_held;
}

unsigned short InputObject::Released() const
{
    return sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_8_released;
}

BOOL CC Input_IsChanting_4334C0()
{
    AE_IMPLEMENTED();
    return Input_IsChanting_45F260();
}

void CC Input_SetKeyState_48E610(int key, char bIsDown)
{
    AE_IMPLEMENTED();
    Input_SetKeyState_4EDD80(key, bIsDown);
}

EXPORT char CC Input_IsVKPressed_48E5D0(int key)
{
    AE_IMPLEMENTED();
    return Input_IsVKPressed_4EDD40(key);
}

EXPORT void CC Input_Init_44EB60()
{
    AE_IMPLEMENTED();
    Input_Init_491BC0();
}

EXPORT void Input_DisableInput_48E690()
{
    AE_IMPLEMENTED();
    Input_DisableInputForPauseMenuAndDebug_4EDDC0();
}

// Only really needed for SHIFT being pressed for the AO ddcheat, may as well just remove the requirement
// for shift to be pressed because it is a pain anyway.
EXPORT void Input_GetCurrentKeyStates_48E630()
{
    NOT_IMPLEMENTED();
}

void Input_InitKeyStateArray_48E5F0()
{
    AE_IMPLEMENTED();
    Input_InitKeyStateArray_4EDD60();
}

EXPORT const char* CC Input_GetButtonString_44F1C0(InputCommands input_command)
{
    AE_IMPLEMENTED();

    const auto aeBits = static_cast<::InputCommands>(AOInputCommandsToAEInputCommands(MakeAOInputBits(input_command)).Raw().all);
    if (aeBits & ::InputCommands::eBack)
    {
        return "esc";
    }


    return ::Input_GetButtonString_492530(AEInputCommandToAEInputString(aeBits), Input_JoyStickEnabled() ? 1 : 0);
}

EXPORT int CC Input_Remap_44F300(InputCommands inputCmd)
{
    AE_IMPLEMENTED();

    return Input_Remap_492680(static_cast<::InputCommands>(AOInputCommandsToAEInputCommands(MakeAOInputBits(inputCmd)).Raw().all));
}

EXPORT char Input_GetLastPressedKey_44F2C0()
{
    // AE impl
    if (!RunningAsInjectedDll())
    {
        return static_cast<char>(::Input_GetLastPressedKey_492610());
    }

    // AO impl
    if (!Sys_IsAnyKeyDown_48E6C0())
    {
        return 0;
    }

    const char result = static_cast<char>(sLastPressedKey_A8A604);
    sIsAKeyDown_A8A600 = 0;
    sLastPressedKey_A8A604 = 0;
    return result;
}

EXPORT int Input_Enable_48E6A0()
{
    // AE impl
    if (!RunningAsInjectedDll())
    {
        ::Input_EnableInput_4EDDD0();
        return 0;
    }

    // AO impl
    sInputEnabled_9F7710 = 1;
    return 0;
}

EXPORT void Input_Reset_44F2F0()
{
    // Funcs below call AE impls in standalone
    Input_Enable_48E6A0();
    Input_InitKeyStateArray_48E5F0();
}


bool Input_JoyStickEnabled()
{
    if (RunningAsInjectedDll())
    {
        // Use AO var
        return sJoystickEnabled_508A60 ? true : false;
    }
    else
    {
        // Use AE var
        return ::Input_JoyStickEnabled();
    }
}

void Input_SetJoyStickEnabled(bool enabled)
{
    if (RunningAsInjectedDll())
    {
        // Use AO var
        sJoystickEnabled_508A60 = enabled;
    }
    else
    {
        // Use AE var
        return ::Input_SetJoyStickEnabled(enabled);
    }
}

ALIVE_VAR(1, 0x508A64, DWORD, dword_508A64, 0);

EXPORT int CC Input_SaveSettingsIni_44F460()
{
    // Call AE func in standalone
    if (!RunningAsInjectedDll())
    {
        Input_SaveSettingsIni_Common(true);
        return 1;
    }

    FILE* iniFileHandle = fopen("abe.ini", "w");
    if (!iniFileHandle)
    {
        return 0;
    }

    dword_508A64 = 1;
    fprintf(iniFileHandle, "[Control Layout]\n");
    if (Input_JoyStickEnabled())
    {
        fprintf(iniFileHandle, "controller = Game Pad\n");

    }
    else
    {
        fprintf(iniFileHandle, "controller = Keyboard\n");
    }
    
    const auto oldJoystickEnabled = Input_JoyStickEnabled();

    Input_SetJoyStickEnabled(false);

    fprintf(iniFileHandle, "[Keyboard]\n");
    fprintf(iniFileHandle, "run = %s\n", Input_GetButtonString_44F1C0(InputCommands::eRun));
    fprintf(iniFileHandle, "sneak = %s\n", Input_GetButtonString_44F1C0(InputCommands::eSneak));
    fprintf(iniFileHandle, "jump = %s\n", Input_GetButtonString_44F1C0(InputCommands::eHop));
    fprintf(iniFileHandle, "action = %s\n", Input_GetButtonString_44F1C0(InputCommands::eDoAction));
    fprintf(iniFileHandle, "throw = %s\n", Input_GetButtonString_44F1C0(InputCommands::eThrowItem));
    fprintf(iniFileHandle, "crouch = %s\n", Input_GetButtonString_44F1C0(InputCommands::eFartOrRoll));

    Input_SetJoyStickEnabled(true);

    fprintf(iniFileHandle, "[Game Pad]\n");
    fprintf(iniFileHandle, "run = %s\n", Input_GetButtonString_44F1C0(InputCommands::eRun));
    fprintf(iniFileHandle, "sneak = %s\n", Input_GetButtonString_44F1C0(InputCommands::eSneak));
    fprintf(iniFileHandle, "jump = %s\n", Input_GetButtonString_44F1C0(InputCommands::eHop));
    fprintf(iniFileHandle, "action = %s\n", Input_GetButtonString_44F1C0(InputCommands::eDoAction));
    fprintf(iniFileHandle, "throw = %s\n", Input_GetButtonString_44F1C0(InputCommands::eThrowItem));
    fprintf(iniFileHandle, "crouch = %s\n", Input_GetButtonString_44F1C0(InputCommands::eFartOrRoll));

    Input_SetJoyStickEnabled(oldJoystickEnabled);

    fclose(iniFileHandle);
    dword_508A64 = 0;
    return 1;
}

InputObject& Input()
{
    return sInputObject_5009E8;
}

}
