#include "Engine.hpp"
#include "data_conversion/data_conversion_ui.hpp"
#include "../AliveLibAE/Game.hpp"
#include "../AliveLibAO/Game.hpp"
#include "PsxDisplay.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "../AliveLibCommon/BaseGameAutoPlayer.hpp"

void SYS_EventsPump();
BaseGameAutoPlayer& GetGameAutoPlayer();
void Main_ParseCommandLineArguments(const char_type* pCommandLine);

namespace AO {
void Main_ParseCommandLineArguments(const char_type* pCommandLine);
}

Engine::Engine(GameType gameType, const char_type* pCommandLine)
    : mGameType(gameType)
    , mCommandLine(pCommandLine)
{

}

void Engine::Run()
{
    // TODO: HACK mini loop till Game.cpp is merged
    if (mGameType == GameType::eAe)
    {
        GetGameAutoPlayer().ParseCommandLine(mCommandLine);
        Main_ParseCommandLineArguments(mCommandLine);
    }
    else
    {
        GetGameAutoPlayer().ParseCommandLine(mCommandLine);
        AO::Main_ParseCommandLineArguments(mCommandLine);
    }

    gPsxDisplay.Init();
    DataConversionUI dcu(mGameType);
    if (dcu.ConversionRequired())
    {
        do
        {
            dcu.VUpdate();

            PrimHeader** ppOt = gPsxDisplay.mDrawEnvs[gPsxDisplay.mBufferIndex].mOrderingTable;
            dcu.VRender(ppOt);

            SYS_EventsPump();
            gPsxDisplay.RenderOrderingTable();
        }
        while (!dcu.mBaseGameObjectFlags.Get(BaseGameObject::eDead));
    }
    else
    {
        LOG_INFO("Data is up to date, skip conversion");
    }

    if (mGameType == GameType::eAe)
    {
        LOG_INFO("AE standalone starting...");
        Game_Main();
    }
    else
    {
        LOG_INFO("AO standalone starting...");
        AO::Game_Main();
    }
}
