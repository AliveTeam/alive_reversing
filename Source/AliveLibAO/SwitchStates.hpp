#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

struct SwitchStates
{
    char mData[256];
};

void SwitchStates_Set(unsigned __int16 idx, char value);
short SwitchStates_Get(unsigned __int16 idx);

enum class SwitchOp : __int16
{
    eSetTrue_0 = 0,
    eSetFalse_1 = 1,
    eToggle_2 = 2,
    eIncrement_3 = 3,
    eDecrement_4 = 4,
};

EXPORT void CC SwitchStates_Do_Operation_436A10(signed __int16 idx, SwitchOp operation);

void SwitchStates_Add(unsigned __int16 idx, char value);

ALIVE_VAR_EXTERN(SwitchStates, sSwitchStates_505568);

END_NS_AO
