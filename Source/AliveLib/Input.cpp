#include "stdafx.h"
#include "Input.hpp"
#include <windows.h>
#include "logger.hpp"
#include "Function.hpp"
#include "easylogging++.h"
#include "Game.hpp"

#define INPUT_IMPL true

// -- Variables -- //

ALIVE_ARY(1, 0xBD2F60, unsigned char, 256, sInputKeyStates_BD2F60, {});

ALIVE_VAR(1, 0x5C2EF4, int, sJoystickEnabled_5C2EF4, 0);
ALIVE_VAR(1, 0x5C2EFC, int, sJoystickNumButtons_5C2EFC, 0);
ALIVE_VAR(1, 0x5C2F00, int, sJoystickID_5C2F00, 0);
ALIVE_VAR(1, 0xBBB9D0, BYTE, sInputEnabled_BBB9D0, 0);

ALIVE_VAR(1, 0x5BD4E0, InputObject, sInputObject_5BD4E0, {});
ALIVE_VAR(1, 0x5C1BBE, unsigned __int16, sCurrentControllerIndex_5C1BBE, 0);
ALIVE_VAR(1, 0x5C1B9A, __int16, word_5C1B9A, 0);

// -- Functions -- //

EXPORT unsigned __int8 CC Input_GetInputEnabled_4EDDE0()
{
    return sInputEnabled_BBB9D0 != 0;
}

EXPORT void CC Input_EnableInput_4EDDD0()
{
    sInputEnabled_BBB9D0 = TRUE;
}

EXPORT void CC Input_DisableInput_4EDDC0()
{
    sInputEnabled_BBB9D0 = FALSE;
}

EXPORT void CC Input_Init_491BC0()
{
    NOT_IMPLEMENTED();
}

// Zeros the input key state array.
EXPORT void CC Input_InitKeyStateArray_4EDD60()
{
    memset(sInputKeyStates_BD2F60, 0, 256u);
}

// Returns true if a key was just pressed down.
EXPORT bool CC Input_IsVKPressed_4EDD40(int key)
{
    unsigned char keyState = sInputKeyStates_BD2F60[key];

    if (!keyState)
    {
        return false;
    }

    sInputKeyStates_BD2F60[key] = keyState & 0x80;

    return true;
}

EXPORT void CC InputGetJoystickState_460280(float* /*X1*/, float* /*Y1*/, float* /*X2*/, float* /*Y2*/, DWORD* /*Buttons*/)
{
    NOT_IMPLEMENTED();
}



EXPORT int CC sub_4FA9C0(int /*padNum*/)
{

    NOT_IMPLEMENTED();
    return 0;
}

int InputObject::Is_Demo_Playing_45F220()
{
    return field_38_bDemoPlaying & 1;
}

void InputObject::UnsetDemoPlaying_45F240()
{
    field_38_bDemoPlaying &= ~1;
}

void InputObject::SetDemoResource_45F1E0(DWORD** pDemoRes)
{
    field_34_demo_command_index = 2;
    field_30_pDemoRes = pDemoRes;
    field_38_bDemoPlaying |= 1u;
    field_40_command_duration = 0;
}

void InputObject::Update_45F040()
{
    const unsigned char byte_545A4C[20] =
    {
        0, // left?
        64, // up?
        192, // down?
        0,
        128, // right?
        96,
        160,
        128,
        0,
        32,
        224,
        0,
        0,
        64,
        192,
        0,
        0,
        0,
        0,
        0
    };

    field_0_pads[0].field_8_previous = field_0_pads[0].field_0_pressed;
    field_0_pads[0].field_0_pressed = sub_4FA9C0(0);

    if (Is_Demo_Playing_45F220())
    {
        // Stop if any button on any pad is pressed
        if (field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
        {
            word_5C1B9A = 0;
            UnsetDemoPlaying_45F240();
            return;
        }

        if (sGnFrame_5C1B84 >= field_40_command_duration)
        {
            const DWORD command = (*field_30_pDemoRes)[field_34_demo_command_index++];
            field_3C_command = command >> 16;
            field_40_command_duration = sGnFrame_5C1B84 + command & 0xFFFF;

            // End demo/quit command
            if (command & 0x8000)
            {
                UnsetDemoPlaying_45F240();
            }
        }

        // Will do nothing if we hit the end command..
        if (Is_Demo_Playing_45F220())
        {
            field_0_pads[0].field_0_pressed = Command_To_Raw_404354(field_3C_command);
        }
    }

    field_0_pads[0].field_10_released = field_0_pads[0].field_8_previous & ~field_0_pads[0].field_0_pressed;
    field_0_pads[0].field_C_held = field_0_pads[0].field_0_pressed & ~field_0_pads[0].field_8_previous;
    field_0_pads[0].field_4_dir = byte_545A4C[field_0_pads[0].field_0_pressed & 0xF];

    field_0_pads[1].field_8_previous = field_0_pads[1].field_0_pressed;
    field_0_pads[1].field_0_pressed = sub_4FA9C0(1);
    field_0_pads[1].field_10_released = field_0_pads[1].field_8_previous & ~field_0_pads[1].field_0_pressed;
    field_0_pads[1].field_C_held = field_0_pads[1].field_0_pressed & ~field_0_pads[1].field_8_previous;
    field_0_pads[1].field_4_dir = byte_545A4C[field_0_pads[1].field_0_pressed & 0xF];
}

DWORD CC InputObject::Command_To_Raw_404354(DWORD cmd)
{
    unsigned int shoulderButtonsPressedCount = 0;

    if (cmd & PsxButtonBits::eL2)
    {
        ++shoulderButtonsPressedCount;
    }

    if (cmd & PsxButtonBits::eR2)
    {
        ++shoulderButtonsPressedCount;
    }

    if (cmd & PsxButtonBits::eL1)
    {
        ++shoulderButtonsPressedCount;
    }

    if (cmd & PsxButtonBits::eR1)
    {
        ++shoulderButtonsPressedCount;
    }

    if (shoulderButtonsPressedCount > 1) // Any 2 shoulder button combo = chanting
    {
        return InputCommands::eChant;
    }

    DWORD rawInput = 0;
    if (cmd & PsxButtonBits::eDPadUp)
    {
        rawInput |= InputCommands::eUp;
    }

    if (cmd & PsxButtonBits::eDPadRight)
    {
        rawInput |= InputCommands::eRight;
    }

    if (cmd & PsxButtonBits::eDPadDown)
    {
        rawInput |= InputCommands::eDown;
    }

    if (cmd & eDPadLeft)
    {
        rawInput |= InputCommands::eLeft;
    }

    if (cmd & PsxButtonBits::eR1)
    {
        rawInput |= InputCommands::eRun;
    }

    if (cmd & PsxButtonBits::eR2)
    {
        rawInput |= InputCommands::eSneak;
    }

    if (cmd & PsxButtonBits::eL1)
    {
        if (cmd & PsxButtonBits::eTriangle)
        {
            rawInput |= InputCommands::eGameSpeak1;
        }

        if (cmd & PsxButtonBits::eCircle)
        {
            rawInput |= InputCommands::eGameSpeak4;
        }

        if (cmd & PsxButtonBits::eCross)
        {
            rawInput |= InputCommands::eGameSpeak3;
        }

        if (cmd & PsxButtonBits::eSquare)
        {
            rawInput |= InputCommands::eGameSpeak2;
        }
    }
    else if (cmd & PsxButtonBits::eL2)
    {
        if (cmd & PsxButtonBits::eTriangle)
        {
            rawInput |= InputCommands::eGameSpeak6;
        }

        if (cmd & PsxButtonBits::eCircle)
        {
            rawInput |= InputCommands::eGameSpeak7;
        }

        if (cmd & PsxButtonBits::eCross)
        {
            rawInput |= InputCommands::eGameSpeak5;
        }

        if (cmd & PsxButtonBits::eSquare)
        {
            rawInput |= InputCommands::eGameSpeak8;
        }
    }
    else // No shoulder buttons
    {
        if (cmd & PsxButtonBits::eTriangle)
        {
            rawInput |= InputCommands::eHop;
        }

        if (cmd & PsxButtonBits::eCircle)
        {
            rawInput |= InputCommands::eThrowItem;
        }

        if (cmd & PsxButtonBits::eCross)
        {
            rawInput |= InputCommands::eFartOrRoll;
        }

        if (cmd & PsxButtonBits::eSquare)
        {
            rawInput |= InputCommands::eDoAction;
        }
    }

    return rawInput;
}

void CC InputObject::ShutDown_45F020()
{
    // Empty
}
