#pragma once

#include "Types.hpp"
enum class EReliveLevelIds : s16;

s32 Path_GetTotalMuds(EReliveLevelIds lvlId, u32 pathNum);
s32 Path_BadEndingMuds(EReliveLevelIds lvlId, u32 pathNum);
s32 Path_GoodEndingMuds(EReliveLevelIds lvlId, u32 pathNum);

void Path_Set_NewData_FromLvls();
