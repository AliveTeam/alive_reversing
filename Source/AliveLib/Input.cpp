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

// -- Functions -- //

// Zeros the input key state array.
void CC Input_InitKeyStateArray_4EDD60()
{
	memset(sInputKeyStates_BD2F60, 0, 256u);
}
ALIVE_FUNC_IMPLEX(0x4EDD60, Input_InitKeyStateArray_4EDD60, INPUT_IMPL);


// Returns true if a key was just pressed down.
bool CC Abe_IsVKPressed_4EDD40(int key)
{
	unsigned char keyState = sInputKeyStates_BD2F60[key];

	if (!keyState)
	{
		return false;
	}

	sInputKeyStates_BD2F60[key] = keyState & 0x80;

	return true;
}
ALIVE_FUNC_IMPLEX(0x4EDD40, Abe_IsVKPressed_4EDD40, INPUT_IMPL);


// Unimplemented
ALIVE_FUNC_NOT_IMPL(0x460280, void CC(float *X1, float *Y1, float *X2, float *Y2, DWORD *Buttons), InputGetJoystickState_460280);