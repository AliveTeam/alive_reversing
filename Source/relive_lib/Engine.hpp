#pragma once

#include "GameType.hpp"

class Engine final
{
public:
    Engine(GameType gameType, const char_type* pCommandLine);
    void Run();

private:
    void CmdLineRenderInit(const char_type* pCommandLine);

    GameType mGameType = GameType::eAe;
    const char_type* mCommandLine = nullptr;
};
