#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

struct SwitchStates
{
    char mData[256];
};

void SwitchStates_Set(unsigned __int16 idx, char value);
int SwitchStates_Get(unsigned __int16 idx);

ALIVE_VAR_EXTERN(SwitchStates, sSwitchStates_505568);

END_NS_AO
