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

unsigned __int8 CC Input_GetInputEnabled_4EDDE0()
{
    return sInputEnabled_BBB9D0 != 0;
}
ALIVE_FUNC_IMPLEX(0x4EDDE0, Input_GetInputEnabled_4EDDE0, INPUT_IMPL);

void CC Input_EnableInput_4EDDD0()
{
    sInputEnabled_BBB9D0 = 1;
}
ALIVE_FUNC_IMPLEX(0x4EDDD0, Input_EnableInput_4EDDD0, INPUT_IMPL);

void CC Input_DisableInput_4EDDC0()
{
    sInputEnabled_BBB9D0 = 0;
}
ALIVE_FUNC_IMPLEX(0x4EDDC0, Input_DisableInput_4EDDC0, INPUT_IMPL);

// Zeros the input key state array.
void CC Input_InitKeyStateArray_4EDD60()
{
	memset(sInputKeyStates_BD2F60, 0, 256u);
}
ALIVE_FUNC_IMPLEX(0x4EDD60, Input_InitKeyStateArray_4EDD60, INPUT_IMPL);


// Returns true if a key was just pressed down.
bool CC Input_IsVKPressed_4EDD40(int key)
{
	unsigned char keyState = sInputKeyStates_BD2F60[key];

	if (!keyState)
	{
		return false;
	}

	sInputKeyStates_BD2F60[key] = keyState & 0x80;

	return true;
}
ALIVE_FUNC_IMPLEX(0x4EDD40, Input_IsVKPressed_4EDD40, INPUT_IMPL);


// Unimplemented
ALIVE_FUNC_NOT_IMPL(0x460280, void CC(float *X1, float *Y1, float *X2, float *Y2, DWORD *Buttons), InputGetJoystickState_460280);