#pragma once

#include "../AliveLibCommon/Types.hpp"

enum class GameType : s32
{
    eAo = 0,
    eAe = 1,
};

GameType GetGameType();
void SetGameType(GameType type);
