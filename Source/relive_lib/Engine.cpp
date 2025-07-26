#include "Engine.hpp"
#include "data_conversion/data_conversion_ui.hpp"
#include "../AliveLibAE/Game.hpp"
#include "../AliveLibAO/Game.hpp"
#include "PsxDisplay.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "BaseGameAutoPlayer.hpp"
#include "Sys.hpp"

#include "../AliveLibAE/Io.hpp"
#include "CommandLineParser.hpp"
#include "Renderer/IRenderer.hpp"
#include "Function.hpp"

// TODO: Remove after merge
extern bool sCommandLine_ShowFps;

Engine::Engine(GameType gameType, FileSystem& fs, CommandLineParser& clp)
    : mGameType(gameType)
    , mFs(fs)
    , mClp(clp)
{

}

void Engine::CmdLineRenderInit()
{
    IO_Init_494230();

    sCommandLine_ShowFps = mClp.SwitchExists("-ddfps");
    gCommandLine_NoFrameSkip = mClp.SwitchExists("-ddnoskip");

#if FORCE_DDCHEAT
    gDDCheatOn = true;
#else
    gDDCheatOn = mClp.SwitchExists("-ddcheat") || mClp.SwitchExists("-it_is_me_your_father");
#endif

    IRenderer::Renderers rendererToCreate = IRenderer::Renderers::Sdl2;
    LOG_INFO("Default renderer is Sdl2");

    std::string renderer;
    if (mClp.ExtractNamePairArgument(renderer, "-renderer="))
    {
        if (strcmpi(renderer.c_str(), "gl") == 0 || strcmpi(renderer.c_str(), "gl3") == 0 || strcmpi(renderer.c_str(), "opengl") == 0 || strcmpi(renderer.c_str(), "opengl3") == 0)
        {
            LOG_INFO("Command line set renderer to opengl3");
            rendererToCreate = IRenderer::Renderers::OpenGL;
        }

        if (strcmpi(renderer.c_str(), "sdl") == 0)
        {
            LOG_INFO("Command line set renderer to sdl");
            rendererToCreate = IRenderer::Renderers::Sdl2;
        }
    }

    if (mGameType == GameType::eAe)
    {
        VGA_CreateRenderer(rendererToCreate, WindowTitleAE());
    }
    else
    {
        VGA_CreateRenderer(rendererToCreate, WindowTitleAO());
    }

    PSX_EMU_SetCallBack_4F9430(Game_End_Frame);
}

void Engine::Run()
{
    gPsxDisplay.Init();

    GetGameAutoPlayer().ProcessCommandLine(mClp);
    CmdLineRenderInit();

    // Another hack till refactor branch replaces master
    GetGameAutoPlayer().Pause(true);
    GetGameAutoPlayer().DisableRecorder();

    // Moved from PsxDisplay init to prevent desync
    PSX_PutDispEnv_4F5890();

    // TODO: HACK mini loop till Game.cpp is merged
    DataConversionUI dcu(mGameType);
    if (dcu.ConversionRequired())
    {
        do
        {
            dcu.VUpdate();

            dcu.VRender(gPsxDisplay.mDrawEnv.mOrderingTable);

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
    GetGameAutoPlayer().EnableRecorder();

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
