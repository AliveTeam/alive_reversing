#include "stdafx_ao.h"
#include "Input.hpp"
#include "Function.hpp"
#include "Game.hpp"

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

EXPORT void InputObject::Update_433250()
{
    NOT_IMPLEMENTED();

    // TODO: Try to mostly use the AE code - this changed quite a bit between games though

    /*
    pPadIter = &field_0_pads[0].field_2;
    gnFrame = 2;
    do
    {
        *((_WORD*)pPadIter + 1) = *((_WORD*)pPadIter - 1);
        pPadIter[8] = *pPadIter;
        pPadIter[9] = pPadIter[1];
        pPadIter += 0xC;
        --gnFrame;
    } while (gnFrame);
    */

    /*
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
    */

    if (field_20_demo_playing & 1)
    {
        // Stop if any button on any pad is pressed
        if (field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed)
        {
            //field_20_demo_playing &= ~1u;
            //return;
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

    /*
    pad_field4 = &field_0_pads[0].field_4;
    k2Counter = 2;
    do
    {
        LOWORD(gnFrame) = *pad_field4;
        LOWORD(duration) = ~*(pad_field4 - 2);
        v8 = duration & gnFrame;
        pad_field4[2] = v8;
        LOWORD(duration) = *pad_field4;
        LOWORD(v8) = *(pad_field4 - 2);
        pad_field4 += 6;
        LOWORD(duration) = ~(_WORD)duration;
        gnFrame = duration & v8;
        *(pad_field4 - 5) = gnFrame;
        *((_BYTE*)pad_field4 - 14) = byte_4BB428[(unsigned int)(unsigned __int16)*(pad_field4 - 8) >> 12];
        duration = (unsigned __int8)(*((_BYTE*)pad_field4 - 16) >> 4);
        --k2Counter;
        *((_BYTE*)pad_field4 - 13) = byte_4BB428[duration];
    } while (k2Counter);
    */
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

bool InputObject::IsPressed(DWORD command)
{
    return (field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed & command) != 0;
}

bool InputObject::IsHeld(DWORD command)
{
    return (field_0_pads[sCurrentControllerIndex_5076B8].field_6_held & command) != 0;
}

bool InputObject::IsReleased(DWORD command)
{
     return (field_0_pads[sCurrentControllerIndex_5076B8].field_8_released & command) != 0;
}

BOOL CC Input_IsChanting_4334C0()
{
    return (sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed & sInputKey_Chant) == sInputKey_Chant;
}

void CC Input_SetKeyState_48E610(int /*key*/, char /*bIsDown*/)
{
    NOT_IMPLEMENTED();
}

EXPORT char CC Input_IsVKPressed_48E5D0(int /*key*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC Input_Init_44EB60()
{
    NOT_IMPLEMENTED();
}


EXPORT void Input_DisableInput_48E690()
{
    NOT_IMPLEMENTED();
}

EXPORT void Input_GetCurrentKeyStates_48E630()
{
    NOT_IMPLEMENTED();
}

void Input_InitKeyStateArray_48E5F0()
{
    NOT_IMPLEMENTED();
}

}
