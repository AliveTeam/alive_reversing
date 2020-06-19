#include "stdafx_ao.h"
#include "SwitchStates.hpp"
#include "Function.hpp"

START_NS_AO

ALIVE_VAR(1, 0x505568, SwitchStates, sSwitchStates_505568, {});

void SwitchStates_Set(unsigned __int16 idx, char value)
{
    sSwitchStates_505568.mData[idx] = value;
}

int SwitchStates_Get(unsigned __int16 idx)
{
    if (idx == 0)
    {
        return 0;
    }

    if (idx == 1)
    {
        return 1;
    }

    return sSwitchStates_505568.mData[idx];
}

END_NS_AO

