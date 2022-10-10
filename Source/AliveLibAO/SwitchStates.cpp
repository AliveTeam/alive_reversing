#include "stdafx_ao.h"
#include "SwitchStates.hpp"
#include "Function.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

SwitchStates sSwitchStates_505568 = {};

void SwitchStates_Set(u16 idx, s8 value)
{
    sSwitchStates_505568.mData[idx] = value;
}

s16 SwitchStates_Get(u16 idx)
{
    if (idx >= ALIVE_COUNTOF(sSwitchStates_505568.mData))
    {
        LOG_WARNING("switch id value is " << idx << " and exceeds 255. if you see this in a custom level consider lowering the switch id value.");
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

void SwitchStates_Do_Operation(s16 idx, relive::reliveSwitchOp operation)
{
    if (idx >= 2)
    {
        switch (operation)
        {
            case relive::reliveSwitchOp::eSetTrue:
                SwitchStates_Set(idx, 1);
                break;

            case relive::reliveSwitchOp::eSetFalse:
                SwitchStates_Set(idx, 0);
                break;

            case relive::reliveSwitchOp::eToggle:
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

            case relive::reliveSwitchOp::eIncrement:
                SwitchStates_Add(idx, 1);
                break;

            case relive::reliveSwitchOp::eDecrement:
                SwitchStates_Add(idx, -1);
                break;

            default:
                return;
        }
    }
}

} // namespace AO
