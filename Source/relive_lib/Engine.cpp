#include "Engine.hpp"
#include "data_conversion/data_conversion_ui.hpp"
#include "../AliveLibAE/Game.hpp"
#include "../AliveLibAO/Game.hpp"
#include "PsxDisplay.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "../relive_lib/BaseGameAutoPlayer.hpp"

void SYS_EventsPump();

void Main_ParseCommandLineArguments(const char_type* pCommandLine);

namespace AO {
void Main_ParseCommandLineArguments(const char_type* pCommandLine);
}

Engine::Engine(GameType gameType, const char_type* pCommandLine)
    : mGameType(gameType)
    , mCommandLine(pCommandLine)
{

}

void PSX_PutDispEnv_4F5890();

void Engine::Run()
{
    // TODO: HACK mini loop till Game.cpp is merged

    gPsxDisplay.Init();

    GetGameAutoPlayer().ParseCommandLine(mCommandLine);
    if (mGameType == GameType::eAe)
    {
        Main_ParseCommandLineArguments(mCommandLine);
    }
    else
    {
        AO::Main_ParseCommandLineArguments(mCommandLine);
    }

    // Moved from PsxDisplay init to prevent desync
    PSX_PutDispEnv_4F5890();

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
        while (!dcu.GetDead());
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
