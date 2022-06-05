#include "GameType.hpp"

static GameType sGameType = GameType::eAe;

GameType GetGameType()
{
    return sGameType;
}

void SetGameType(GameType type)
{
    sGameType = type;
}
