#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

void SwitchStates_SetRange(u16 start, u16 end);
void SwitchStates_Set(u16 idx, s8 value);
s32 SwitchStates_Get(u16 idx);
void SwitchStates_Add(u16 idx, s8 value);

enum class SwitchOp : s16
{
    eSetTrue_0 = 0,
    eSetFalse_1 = 1,
    eToggle_2 = 2,
    eIncrement_3 = 3,
    eDecrement_4 = 4,
};

void SwitchStates_Do_Operation(s16 idx, relive::reliveSwitchOp operation);

struct SwitchStates final
{
    s8 mData[256];
};

ALIVE_VAR_EXTERN(SwitchStates, sSwitchStates_5C1A28);
