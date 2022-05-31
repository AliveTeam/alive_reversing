#pragma once

#include "../AliveLibCommon/Function.hpp"

void SwitchStates_ForceLink();

void SwitchStates_SetRange_465FA0(u16 start, u16 end);
void SwitchStates_Set_465FF0(u16 idx, s8 value);
s32 SwitchStates_Get_466020(u16 idx);
void SwitchStates_Add_466060(u16 idx, s8 value);

enum class SwitchOp : s16
{
    eSetTrue_0 = 0,
    eSetFalse_1 = 1,
    eToggle_2 = 2,
    eIncrement_3 = 3,
    eDecrement_4 = 4,
};

void SwitchStates_Do_Operation_465F00(s16 idx, SwitchOp operation);

struct SwitchStates final
{
    s8 mData[256];
};

ALIVE_VAR_EXTERN(SwitchStates, sSwitchStates_5C1A28);
