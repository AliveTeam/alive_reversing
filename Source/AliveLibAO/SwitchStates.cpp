#include "stdafx_ao.h"
#include "SwitchStates.hpp"
#include "Function.hpp"

namespace AO {

ALIVE_VAR(1, 0x505568, SwitchStates, sSwitchStates_505568, {});

void SwitchStates_Set(u16 idx, s8 value)
{
    sSwitchStates_505568.mData[idx] = value;
}

s16 SwitchStates_Get(u16 idx)
{
    if (idx > UCHAR_MAX)
    {
        LOG_WARNING("switch id value is " << idx << " and exceeds " << UCHAR_MAX << ". if you see this in a custom level consider lowering the switch id value.");
        return 0;
    }

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

void SwitchStates_Add(u16 idx, s8 value)
{
    sSwitchStates_505568.mData[idx] += value;
}


void SwitchStates_ClearAll()
{
    sSwitchStates_505568 = {};
}

EXPORT void CC SwitchStates_Do_Operation_436A10(s16 idx, SwitchOp operation)
{
    if (idx >= 2)
    {
        switch (operation)
        {
            case SwitchOp::eSetTrue_0:
                SwitchStates_Set(idx, 1);
                break;

            case SwitchOp::eSetFalse_1:
                SwitchStates_Set(idx, 0);
                break;

            case SwitchOp::eToggle_2:
                if (SwitchStates_Get(idx))
                {
                    // Its on, so turn off
                    SwitchStates_Set(idx, 0);
                }
                else
                {
                    // Its off, so turn on
                    SwitchStates_Set(idx, 1);
                }
                break;

            case SwitchOp::eIncrement_3:
                SwitchStates_Add(idx, 1);
                break;

            case SwitchOp::eDecrement_4:
                SwitchStates_Add(idx, -1);
                break;

            default:
                return;
        }
    }
}

} // namespace AO
