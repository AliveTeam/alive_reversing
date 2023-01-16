#include "Engine.hpp"
#include "data_conversion/data_conversion_ui.hpp"
#include "../AliveLibAE/Game.hpp"
#include "../AliveLibAO/Game.hpp"
#include "PsxDisplay.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "BaseGameAutoPlayer.hpp"
#include "Sys.hpp"

#include "../AliveLibAE/Io.hpp"

// TODO: Remove after merge
extern bool sCommandLine_ShowFps;

Engine::Engine(GameType gameType, const char_type* pCommandLine)
    : mGameType(gameType)
    , mCommandLine(pCommandLine)
{

}

void Engine::CmdLineRenderInit(const char_type* pCommandLine)
{
    IO_Init_494230();

    if (pCommandLine)
    {
        if (strstr(pCommandLine, "-ddfps"))
        {
            sCommandLine_ShowFps = true;
        }

        if (strstr(pCommandLine, "-ddnoskip"))
        {
            gCommandLine_NoFrameSkip = true;
        }

        if (strstr(pCommandLine, "-ddcheat") || _strcmpi(pCommandLine, "-it_is_me_your_father") == 0)
        {
            gDDCheatOn = true;
        }
    }

#if FORCE_DDCHEAT
    gDDCheatOn = true;
#endif

    if (mGameType == GameType::eAe)
    {
        VGA_CreateRenderer(WindowTitleAE());
    }
    else
    {
        VGA_CreateRenderer(WindowTitleAO());
    }

    PSX_EMU_SetCallBack_4F9430(Game_End_Frame);
}

void Engine::Run()
{
    gPsxDisplay.Init();

    GetGameAutoPlayer().ParseCommandLine(mCommandLine);
    CmdLineRenderInit(mCommandLine);

    // Moved from PsxDisplay init to prevent desync
    PSX_PutDispEnv_4F5890();

    // Another hack till refactor branch replaces master
    GetGameAutoPlayer().Pause(true);

    // TODO: HACK mini loop till Game.cpp is merged
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

    GetGameAutoPlayer().Pause(false);

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
