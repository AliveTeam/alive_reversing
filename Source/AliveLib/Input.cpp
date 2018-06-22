#include "stdafx.h"
#include "Input.hpp"
#include <windows.h>
#include "logger.hpp"
#include "Function.hpp"
#include "easylogging++.h"

#define INPUT_IMPL true

// -- Variables -- //

ALIVE_ARY(1, 0xBD2F60, unsigned char, 256, sInputKeyStates_BD2F60, {});

ALIVE_VAR(1, 0x5C2EF4, int, sJoystickEnabled_5C2EF4, 0);
ALIVE_VAR(1, 0x5C2EFC, int, sJoystickNumButtons_5C2EFC, 0);
ALIVE_VAR(1, 0x5C2F00, int, sJoystickID_5C2F00, 0);
ALIVE_VAR(1, 0xBBB9D0, BYTE, sInputEnabled_BBB9D0, 0);


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
    NOT_IMPLEMENTED(0x460280);
}
