#include "stdafx_ao.h"
#include "Input.hpp"
#include "Function.hpp"
#include "Game.hpp"
//#include "..\AliveLibAE\Input.hpp"

namespace AO {

ALIVE_VAR(1, 0x5009E8, InputObject, sInputObject_5009E8, {});
ALIVE_VAR(1, 0x5076B8, unsigned __int16, sCurrentControllerIndex_5076B8, 0);
ALIVE_VAR(1, 0x508A60, int, sJoystickEnabled_508A60, 0);

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

const InputCommands sInputKey_LeftGameSpeakEnabler_4C65B8 = eGameSpeak6;
const InputCommands sInputKey_GameSpeak1_4C65C8 = eHop;
const InputCommands sInputKey_GameSpeak2_4C65BC = eDoAction;
const InputCommands sInputKey_GameSpeak3_4C65C0 = eThrowItem;
const InputCommands sInputKey_GameSpeak4_4C65C4 = eFartOrRoll;

const InputCommands sInputKey_RightGameSpeakEnabler_4C65DC = eGameSpeak3;
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
}

ALIVE_ARY(1, 0x507778, BYTE, 64, sPad1Buffer_507778, {});
ALIVE_ARY(1, 0x507738, BYTE, 64, sPad2Buffer_507738, {});

EXPORT void InputObject::Update_433250()
{
    const BYTE byte_4BB428[16] = { 0u, 64u, 0u, 32u, 192u, 0u, 224u, 0u, 128u, 96u, 0u, 0u, 160u, 0u, 0u, 0u };

    for (int i = 0; i < 2; i++)
    {
        field_0_pads[i].field_A = field_0_pads[i].field_2_dir;
        field_0_pads[i].field_B = field_0_pads[i].field_3;
        field_0_pads[i].field_4_previously_pressed = field_0_pads[i].field_0_pressed;
    }

    if (sPad1Buffer_507778[0])                // can call Input_Read_Pad_49AF10 instead here to match AE
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
    return sJoystickEnabled_508A60;
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
    return sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_2_dir;
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
    NOT_IMPLEMENTED();
    //return Input_IsChanting_45F260();
    //return (sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed & sInputKey_Chant) == sInputKey_Chant;
    return 0;
}

void CC Input_SetKeyState_48E610(int /*key*/, char /*bIsDown*/)
{
    NOT_IMPLEMENTED();
    //Input_SetKeyState_4EDD80(key, bIsDown);
}

EXPORT char CC Input_IsVKPressed_48E5D0(int /*key*/)
{
    NOT_IMPLEMENTED();
    //return Input_IsVKPressed_4EDD40(key);
    return 0;
}

EXPORT void CC Input_Init_44EB60()
{
    NOT_IMPLEMENTED();
    //Input_Init_491BC0();
}

EXPORT void Input_DisableInput_48E690()
{
    AE_IMPLEMENTED();
    //Input_DisableInputForPauseMenuAndDebug_4EDDC0();
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
    //Input_InitKeyStateArray_4EDD60();
}

EXPORT const char* CC Input_GetButtonString_44F1C0(InputCommands /*input_command*/)
{
    NOT_IMPLEMENTED();
    return "lol"; // don't kill standalone
}

EXPORT int CC Input_Remap_44F300(InputCommands /*inputCmd*/)
{
    NOT_IMPLEMENTED();
    //return Input_Remap_492680(static_cast<::InputCommands>(inputCmd));
    return 0;
}

ALIVE_VAR(1, 0x508A64, DWORD, dword_508A64, 0);

EXPORT int CC Input_SaveSettingsIni_44F460()
{
    // Call AE func in standalone
    if (!RunningAsInjectedDll())
    {
        //Input_SaveSettingsIni_492840();
        return 1;
    }

    FILE* iniFileHandle = fopen("abe.ini", "w");
    if (!iniFileHandle)
    {
        return 0;
    }

    dword_508A64 = 1;
    fprintf(iniFileHandle, "[Control Layout]\n");
    if (sJoystickEnabled_508A60)
    {
        if (sJoystickEnabled_508A60 == 1)
        {
            fprintf(iniFileHandle, "controller = Game Pad\n");
        }
    }
    else
    {
        fprintf(iniFileHandle, "controller = Keyboard\n");
    }
    auto oldJoystickEnabled = sJoystickEnabled_508A60;

    sJoystickEnabled_508A60 = 0;

    fprintf(iniFileHandle, "[Keyboard]\n");
    fprintf(iniFileHandle, "run = %s\n", Input_GetButtonString_44F1C0(InputCommands::eRun));
    fprintf(iniFileHandle, "sneak = %s\n", Input_GetButtonString_44F1C0(InputCommands::eSneak));
    fprintf(iniFileHandle, "jump = %s\n", Input_GetButtonString_44F1C0(InputCommands::eHop));
    fprintf(iniFileHandle, "action = %s\n", Input_GetButtonString_44F1C0(InputCommands::eDoAction));
    fprintf(iniFileHandle, "throw = %s\n", Input_GetButtonString_44F1C0(InputCommands::eThrowItem));
    fprintf(iniFileHandle, "crouch = %s\n", Input_GetButtonString_44F1C0(InputCommands::eFartOrRoll));

    sJoystickEnabled_508A60 = 1;

    fprintf(iniFileHandle, "[Game Pad]\n");
    fprintf(iniFileHandle, "run = %s\n", Input_GetButtonString_44F1C0(InputCommands::eRun));
    fprintf(iniFileHandle, "sneak = %s\n", Input_GetButtonString_44F1C0(InputCommands::eSneak));
    fprintf(iniFileHandle, "jump = %s\n", Input_GetButtonString_44F1C0(InputCommands::eHop));
    fprintf(iniFileHandle, "action = %s\n", Input_GetButtonString_44F1C0(InputCommands::eDoAction));
    fprintf(iniFileHandle, "throw = %s\n", Input_GetButtonString_44F1C0(InputCommands::eThrowItem));
    fprintf(iniFileHandle, "crouch = %s\n", Input_GetButtonString_44F1C0(InputCommands::eFartOrRoll));

    sJoystickEnabled_508A60 = oldJoystickEnabled;

    fclose(iniFileHandle);
    dword_508A64 = 0;
    return 1;
}

InputObject& Input()
{
    return sInputObject_5009E8;
}

}
