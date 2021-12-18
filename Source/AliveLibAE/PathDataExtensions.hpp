#pragma once

#include "Types.hpp"
enum class LevelIds : s16;

s32 Path_GetTotalMuds(LevelIds lvlId, u32 pathNum);
s32 Path_BadEndingMuds(LevelIds lvlId, u32 pathNum);
s32 Path_GoodEndingMuds(LevelIds lvlId, u32 pathNum);

void Path_Set_NewData_FromLvls();
