#include "stdafx.h"
#include "SwitchStates.hpp"
#include "Function.hpp"

ALIVE_ARY(1, 0x5c1a28, char, 256, sSwitchStates_5C1A28, {});

void SwitchStates_ForceLink() {}

EXPORT void CC SwitchStates_SetRange_465FA0(unsigned __int16 start, unsigned __int16 end)
{
    if (start <= end)
    {
        memset(&sSwitchStates_5C1A28[start], 0, end - start + 1);
    }
}

EXPORT void CC SwitchStates_Set_465FF0(unsigned __int16 idx, char value)
{
    sSwitchStates_5C1A28[idx] = value;
}

EXPORT int CC SwitchStates_Get_466020(unsigned __int16 idx)
{
    if (idx == 0)
    {
        return 0;
    }
    
    if (idx == 1)
    {
        return 1;
    }

    return sSwitchStates_5C1A28[idx];
}

EXPORT void CC SwitchStates_Add_466060(unsigned __int16 idx, char value)
{
    sSwitchStates_5C1A28[idx] += value;
}

EXPORT void CC SwitchStates_Do_Operation_465F00(signed __int16 idx, SwitchOp operation)
{
    if (idx >= 2)
    {
        switch (operation)
        {
        case SwitchOp::eSetTrue:
            SwitchStates_Set_465FF0(idx, 1);
            break;

        case SwitchOp::eSetFalse:
            SwitchStates_Set_465FF0(idx, 0);
            break;

        case SwitchOp::eToggle:
            if (SwitchStates_Get_466020(idx))
            {
                // Its on, so turn off
                SwitchStates_Set_465FF0(idx, 0);
            }
            else
            {
                // Its off, so turn on
                SwitchStates_Set_465FF0(idx, 1);
            }
            break;

        case SwitchOp::eIncrement:
            SwitchStates_Add_466060(idx, 1);
            break;

        case SwitchOp::eDecrement:
            SwitchStates_Add_466060(idx, -1);
            break;
        }
    }
}
