#pragma once

#include "FunctionFwd.hpp"

namespace AO {

struct SwitchStates
{
    char mData[256];
};

void SwitchStates_Set(u16 idx, char value);
s16 SwitchStates_Get(u16 idx);

enum class SwitchOp : s16
{
    eSetTrue_0 = 0,
    eSetFalse_1 = 1,
    eToggle_2 = 2,
    eIncrement_3 = 3,
    eDecrement_4 = 4,
};

EXPORT void CC SwitchStates_Do_Operation_436A10(s16 idx, SwitchOp operation);

void SwitchStates_Add(u16 idx, char value);
void SwitchStates_ClearAll();

ALIVE_VAR_EXTERN(SwitchStates, sSwitchStates_505568);

}
