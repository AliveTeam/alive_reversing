#include "stdafx.h"
#include "SwitchStates.hpp"
#include "Function.hpp"

ALIVE_VAR(1, 0x5c1a28, SwitchStates, sSwitchStates_5C1A28, {});

void SwitchStates_ForceLink() {}

EXPORT void CC SwitchStates_SetRange_465FA0(u16 start, u16 end)
{
    if (start <= end)
    {
        memset(&sSwitchStates_5C1A28.mData[start], 0, end - start + 1);
    }
}

EXPORT void CC SwitchStates_Set_465FF0(u16 idx, char value)
{
    sSwitchStates_5C1A28.mData[idx] = value;
}

EXPORT s32 CC SwitchStates_Get_466020(u16 idx)
{
    if (idx == 0)
    {
        return 0;
    }
    
    if (idx == 1)
    {
        return 1;
    }

    return sSwitchStates_5C1A28.mData[idx];
}

EXPORT void CC SwitchStates_Add_466060(u16 idx, char value)
{
    sSwitchStates_5C1A28.mData[idx] += value;
}

EXPORT void CC SwitchStates_Do_Operation_465F00(s16 idx, SwitchOp operation)
{
    if (idx >= 2)
    {
        switch (operation)
        {
        case SwitchOp::eSetTrue_0:
            SwitchStates_Set_465FF0(idx, 1);
            break;

        case SwitchOp::eSetFalse_1:
            SwitchStates_Set_465FF0(idx, 0);
            break;

        case SwitchOp::eToggle_2:
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

        case SwitchOp::eIncrement_3:
            SwitchStates_Add_466060(idx, 1);
            break;

        case SwitchOp::eDecrement_4:
            SwitchStates_Add_466060(idx, -1);
            break;
        }
    }
}
