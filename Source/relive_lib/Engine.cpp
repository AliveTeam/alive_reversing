#include "Engine.hpp"
#include "GameType.hpp"
#include "data_conversion/data_conversion_ui.hpp"
#include "PsxDisplay.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "BaseGameAutoPlayer.hpp"
#include "Sys.hpp"

#include "CommandLineParser.hpp"
#include "Renderer/IRenderer.hpp"
#include "Function.hpp"

#include "../relive_lib/Sys.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "../AliveLibAE/Input.hpp"
#include "../relive_lib/Psx.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../relive_lib/Sound/Sound.hpp" // for shut down func
#include "AmbientSound.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../AliveLibAE/Map.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../AliveLibAE/PauseMenu.hpp"
#include "../AliveLibAE/GameSpeak.hpp"
#include "../AliveLibAE/DDCheat.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include <fstream>
#include "../relive_lib/Events.hpp"
#include "../AliveLibAE/Abe.hpp"
#include "../AliveLibAE/MusicController.hpp"
#include "../relive_lib/GameObjects/CheatController.hpp"
#include "../AliveLibAE/Slurg.hpp"
#include "../AliveLibAE/PathDataExtensions.hpp"
#include "../AliveLibAE/GameAutoPlayer.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/ShadowZone.hpp"
#include "../relive_lib/ResourceManagerWrapper.hpp"
#include "../AliveLibAE/GameEnderController.hpp"
#include "../AliveLibAE/ColourfulMeter.hpp"
#include "../relive_lib/GameObjects/GasCountDown.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/GameObjects/PlatformBase.hpp"


#include "../relive_lib/Function.hpp"
#include "../AliveLibAE/Input.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../AliveLibAO/DDCheat.hpp"
#include "../relive_lib/Psx.hpp"
#include "../relive_lib/Sys.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../AliveLibAO/Map.hpp"
#include "../AliveLibAO/GameSpeak.hpp"
#include "../relive_lib/GameObjects/CheatController.hpp"
#include "../AliveLibAO/DDCheat.hpp"
#include "../AliveLibAO/MusicController.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "../AliveLibAO/Input.hpp"
#include "../AliveLibAO/Midi.hpp"
#include "../AliveLibAO/PauseMenu.hpp"
#include "../AliveLibAO/Abe.hpp"
#include "../relive_lib/GameObjects/ShadowZone.hpp"
#include "../AliveLibAO/CameraSwapper.hpp"
#include "AmbientSound.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/Events.hpp"
#include "../AliveLibAO/Sound.hpp"
#include "../relive_lib/Engine.hpp"
#include "../AliveLibAO/PathDataExtensions.hpp"
#include "../AliveLibAO/GameAutoPlayer.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "../relive_lib/GameObjects/GasCountDown.hpp"
#include "../relive_lib/GameObjects/PlatformBase.hpp"
#include "../AliveLibAO/GameEnderController.hpp"
#include "../relive_lib/Mods.hpp"
#include "data_conversion/file_system.hpp"
#include <FatalError.hpp>

u32 sGnFrame = 0;
bool gBreakGameLoop = false;
s16 gNumCamSwappers = 0;
bool gSkipGameObjectUpdates = false;
bool gDDCheatOn = false;
u16 gAttract = 0;


static bool sCommandLine_ShowFps;

Engine::Engine(GameType gameType, FileSystem& fs, CommandLineParser& clp)
    : mGameType(gameType)
    , mFs(fs)
    , mClp(clp)
{

    mIpcInterface = relive::MakeIpcInterface();
    mIpcInterface->Listen([&](relive::PacketTypes packetType, const std::vector<unsigned char>& buffer)
    {
        // Process IPC packets (usually comes from level editor) on this worker thread, send to main
        // thread via an SDL message which will end up in Sys_PumpMessages
        LOG_INFO("On ipc packet type %d len %zu", static_cast<u8>(packetType), buffer.size());
        if (packetType == relive::PacketTypes::LevelPathJsonChanged)
        {

            SDL_Event e;
            SDL_zero(e);
            e.type = Sys_BaseUserEventNumber();
            u8* tmp = new u8[buffer.size()];
            memcpy(tmp, buffer.data(), buffer.size());
            e.user.data1 = tmp;
            e.user.data2 = reinterpret_cast<void*>(buffer.size());
            SDL_PushEvent(&e);
        }
    });
}

Engine::~Engine()
{
    TRACE_ENTRYEXIT;
    mIpcInterface.reset();
}


static f64 sFps_55EFDC = 0.0;
static s32 sFrameDiff_5CA4DC = 0;
static s32 sFrameCount_5CA300 = 0;

static f64 Calculate_FPS_495250(s32 frameCount)
{
    static u32 sLastTime_5CA338 = SYS_GetTicks() - 500;
    const u32 curTime = SYS_GetTicks();
    const s32 timeDiff = curTime - sLastTime_5CA338;

    if (static_cast<s32>((curTime - sLastTime_5CA338)) < 500)
    {
        return sFps_55EFDC;
    }

    const s32 diffFrames = frameCount - sFrameDiff_5CA4DC;
    sFps_55EFDC = static_cast<f64>(diffFrames) * 1000.0 / static_cast<f64>(timeDiff);

    sLastTime_5CA338 = curTime;
    sFrameDiff_5CA4DC = frameCount;
    return sFps_55EFDC;
}

static void DrawFps_4952F0(f32 fps)
{
    char_type strBuffer[125] = {};
    snprintf(strBuffer, sizeof(strBuffer), "%02.1f fps ", static_cast<f64>(fps));
    gPsxDisplay.mDebugFont.DebugFont_Printf(0, "%s", strBuffer);
}


// Called wherever Sys_PumpMessages() signals the user confirmed they want to quit. A hard
// exit(0) here previously left FMV conversion jobs still writing to their final output file
static s32 Game_End_Frame(u32 flags, BaseMap* pMap)
{
    if (flags & 1)
    {
        gTurnOffRendering = false;
        return 0;
    }

    const f64 fps = Calculate_FPS_495250(sFrameCount_5CA300);
    if (sCommandLine_ShowFps)
    {
        DrawFps_4952F0(static_cast<f32>(fps));
    }

    ++sFrameCount_5CA300;

    // A background FMV conversion (DataConversionUI) never overlaps with this callback actually
    // firing - it only runs during Game_Loop, well after DataConversionUI's own loop (which pumps
    // messages itself - see Engine::Run()) has already finished - so there's nothing here that
    // ever needs to know about cancelling one.
    if (Sys_PumpMessages(pMap))
    {
        exit(0);
    }
    return 0;
}


void Engine::CmdLineRenderInit(const std::string& activeModName)
{
#if FORCE_DDCHEAT
    gDDCheatOn = true;
#else
    gDDCheatOn = mClp.SwitchExists("-ddcheat") || mClp.SwitchExists("-it_is_me_your_father");
#endif

    IRenderer::Renderers rendererToCreate = IRenderer::Renderers::Sdl3;
    LOG_INFO("Default renderer is Sdl3");

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
            rendererToCreate = IRenderer::Renderers::Sdl3;
        }
    }


    if (mGameType == GameType::eAe)
    {
        VGA_CreateRenderer(rendererToCreate, WindowTitleAE(activeModName));
    }
    else
    {
        VGA_CreateRenderer(rendererToCreate, WindowTitleAO(activeModName));
    }

    PSX_EMU_SetCallBack_4F9430([this](u32 flags) { return Game_End_Frame(flags, mMap.get()); });
}


// QuickSave load/Restart path calls this
void DestroyObjects(ResourceManagerWrapper& resMan)
{
    resMan.LoadingLoop(false);
    for (s32 iterations = 0; iterations < 2; iterations++)
    {
        for (s32 idx = 0;idx < gBaseGameObjects->Size(); idx++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(idx);
            if (!pObj)
            {
                break;
            }

            if (!pObj->GetSurviveDeathReset())
            {
                idx = gBaseGameObjects->RemoveAt(idx);

                delete pObj;
            }
        }
    }
}

void SYS_EventsPump(BaseMap* pMap)
{
    if (Sys_PumpMessages(pMap))
    {
        exit(0);
    }
}

u32 SYS_GetTicks()
{
    // Using this instead of SDL_GetTicks resolves a weird x64 issue on windows where
    // the tick returned is a lot faster on some machines.
    return static_cast<u32>(SDL_GetPerformanceCounter() / (SDL_GetPerformanceFrequency() / 1000));
}

void Alive_Show_ErrorMsg(const char_type* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char_type buf[2048] = {};
    vsnprintf(buf, sizeof(buf) - 1, fmt, args);
    va_end(args);

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ("R.E.L.I.V.E. " + BuildString()).c_str(), buf, nullptr);
}

void Engine::Init_GameStates()
{
    if (GetGameType() == GameType::eAo)
    {
        gKilledMudokons = AO::GameEnderController::gRestartRuptureFarmsKilledMuds;
        gRescuedMudokons = AO::GameEnderController::gRestartRuptureFarmsSavedMuds;
    }
    else
    {
        gKilledMudokons = gFeeco_Restart_KilledMudCount;
        gRescuedMudokons = gFeecoRestart_SavedMudCount;
    }

    gDeathGasOn = false; // GasCountDown
    gDeathGasTimer = 0;

    gbDrawMeterCountDown = false; // ColourfulMeter
    gTotalMeterBars = 0;

    gAbeInvincible = false; // Abe

    SwitchStates_ClearRange(0, 255);
}

void Engine::Init_Sound_DynamicArrays_And_Others()
{
    gPauseMenu = nullptr; // PauseMenu
    gAbe = nullptr;
    AO::gPauseMenu = nullptr;
    AO::gAbe = nullptr;
    sControlledCharacter = nullptr;
    gNumCamSwappers = 0; // TODO: Move
    sGnFrame = 0;

    PlatformBase::MakeArray();
    ShadowZone::MakeArray();

    gBaseAliveGameObjects = relive_new DynamicArrayT<BaseAliveGameObject>(20);

    if (mGameType == GameType::eAe)
    {
        SND_Init();
        SND_Init_Ambiance();
        MusicController::Create(*mResMan, *mMap);
    }
    else
    {
        AO::SND_Init();
        SND_Init_Ambiance();
        AO::MusicController::Create(*mResMan, *mMap);
    }
    Init_GameStates(); // Init other vars + switch states

}

static void Game_Init_LoadingIcon()
{
    /*
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kLoadingResID, 1u, 0);
    if (!ppRes)
    {
        ResourceManager::LoadResourceFile_49C170("LOADING.BAN", nullptr);
        ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kLoadingResID, 1u, 0);
    }
    ResourceManager::Set_Header_Flags_49C650(ppRes, ResourceManager::ResourceHeaderFlags::eNeverFree);
    */
}

static void Game_Free_LoadingIcon()
{
    //gLoadingResource.Clear();
    /*
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kLoadingResID, 0, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_49C330(ppRes);
    }*/
}


void Game_Shutdown()
{
    Input_DisableInputForPauseMenuAndDebug_4EDDC0();
    GetSoundAPI().mSND_SsQuit();
    VGA_Shutdown();
}

void Game_Loop(BaseMap& map)
{
    gBreakGameLoop = false;
    bool bPauseMenuObjectFound = false;
    while (!gBaseGameObjects->IsEmpty())
    {
        GetGameAutoPlayer().SyncPoint(SyncPoints::MainLoopStart);

        EventsResetActive();
        Slurg::Clear_Slurg_Step_Watch_Points();
        gSkipGameObjectUpdates = false; // Used by quick save

        // Update objects
        GetGameAutoPlayer().SyncPoint(SyncPoints::ObjectsUpdateStart);
        for (s32 baseObjIdx = 0; baseObjIdx < gBaseGameObjects->Size(); baseObjIdx++)
        {
            BaseGameObject* pBaseGameObject = gBaseGameObjects->ItemAt(baseObjIdx);

            if (!pBaseGameObject || gSkipGameObjectUpdates)
            {
                break;
            }

            if (pBaseGameObject->GetUpdatable()
			    && !pBaseGameObject->GetDead() 
                && (gNumCamSwappers == 0 || pBaseGameObject->GetUpdateDuringCamSwap()))
            {
                const s32 updateDelay = pBaseGameObject->UpdateDelay();
                if (updateDelay <= 0)
                {
                    if (pBaseGameObject == gPauseMenu)
                    {
                        bPauseMenuObjectFound = true;
                    }
                    else
                    {
                        pBaseGameObject->VUpdate();
                    }
                }
                else
                {
                    pBaseGameObject->SetUpdateDelay(updateDelay - 1);
                }
            }
        }
        GetGameAutoPlayer().SyncPoint(SyncPoints::ObjectsUpdateEnd);

        // Animate everything
        if (gNumCamSwappers <= 0)
        {
            GetGameAutoPlayer().SyncPoint(SyncPoints::AnimateAll);
            AnimationBase::AnimateAll(AnimationBase::gAnimations);
        }

        // Render objects
        GetGameAutoPlayer().SyncPoint(SyncPoints::DrawAllStart);
        for (s32 i = 0; i < gObjListDrawables->Size(); i++)
        {
            BaseGameObject* pDrawable = gObjListDrawables->ItemAt(i);
            if (!pDrawable)
            {
                break;
            }

            if (pDrawable->GetDead())
            {
                pDrawable->SetCantKill(false);
            }
            else if (pDrawable->GetDrawable())
            {
                pDrawable->SetCantKill(true);
                pDrawable->VRender(gPsxDisplay.mDrawEnv.mOrderingTable);
            }
        }
        GetGameAutoPlayer().SyncPoint(SyncPoints::DrawAllEnd);

        gPsxDisplay.mDebugFont.DebugFont_Flush();
        gScreenManager->VRender(gPsxDisplay.mDrawEnv.mOrderingTable);
        SYS_EventsPump(&map); // Exit checking?

        GetGameAutoPlayer().SyncPoint(SyncPoints::RenderOT);
        gPsxDisplay.RenderOrderingTable();
        
        GetGameAutoPlayer().SyncPoint(SyncPoints::RenderStart);

        // Destroy objects with certain flags
        for (s32 idx = 0; idx < gBaseGameObjects->Size(); idx++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(idx);
            if (!pObj)
            {
                break;
            }

            if (pObj->GetDead() && !pObj->GetCantKill() && pObj->mChaseCounter == 0)
            {
                idx = gBaseGameObjects->RemoveAt(idx);
                relive_delete pObj;
            }
        }

        GetGameAutoPlayer().SyncPoint(SyncPoints::RenderEnd);

        if (bPauseMenuObjectFound && gPauseMenu)
        {
            gPauseMenu->VUpdate();
        }

        bPauseMenuObjectFound = false;

        map.ScreenChange();

        if (GetGameType() == GameType::eAe)
        {
            Input().Update(GetGameAutoPlayer());
        }
        else
        {
            AO::Input().Update(GetGameAutoPlayer());
        }

        if (gNumCamSwappers == 0)
        {
            GetGameAutoPlayer().SyncPoint(SyncPoints::IncrementFrame);
            sGnFrame++;
        }

        if (gBreakGameLoop)
        {
            GetGameAutoPlayer().SyncPoint(SyncPoints::MainLoopExit);
            break;
        }

        GetGameAutoPlayer().ValidateObjectStates();

    } // Main loop end

    PSX_VSync(VSyncMode::UncappedFps);

    // Destroy all game objects
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObjToKill = gBaseGameObjects->ItemAt(i);
        if (!pObjToKill)
        {
            break;
        }

        if (pObjToKill->GetDead())
        {
            i = gBaseGameObjects->RemoveAt(i);
            relive_delete pObjToKill;
        }
    }
}

void Engine::Game_Run(EReliveLevelIds startLevel, s32 startPath, s32 startCamera)
{
    // Begin start up
    SYS_EventsPump(mMap.get());

    gAttract = 0;
 
    AO::Input().InitPad(1);

    gBaseGameObjects = relive_new DynamicArrayT<BaseGameObject>(90);

    BaseAnimatedWithPhysicsGameObject::MakeArray(); // Makes drawables

    AnimationBase::CreateAnimationArray();

    if (mGameType == GameType::eAe)
    {
        Input_Init();
    }
    else
    {
        AO::Input_Init();
    }

    Init_Sound_DynamicArrays_And_Others();

    if (mGameType == GameType::eAe)
    {
        relive_new DDCheat(*mResMan, *mMap);
        gEventSystem = relive_new GameSpeak(*mResMan, *mMap);
    }
    else
    {
        relive_new AO::DDCheat(*mResMan, *mMap);
        AO::gEventSystem = relive_new AO::GameSpeak(*mResMan, *mMap);
    }
    gCheatController = relive_new CheatController(*mResMan, *mMap);

    Game_Init_LoadingIcon();

    mMap->Init(startLevel, static_cast<s16>(startPath), static_cast<s16>(startCamera), CameraSwapEffects::eInstantChange_0, {}, 0);

    // Main loop start
    Game_Loop(*mMap);

    // Shut down start
    Game_Free_LoadingIcon();

    mMap->Shutdown();
    mMap.reset();

    if (mGameType == GameType::eAe)
    {
        DDCheat::ClearProperties();
    }
    else
    {
        AO::DDCheat::ClearProperties();
    }

    AnimationBase::FreeAnimationArray();
    BaseAnimatedWithPhysicsGameObject::FreeArray();
    relive_delete gBaseGameObjects;
    PlatformBase::FreeArray();
    ShadowZone::FreeArray();
    relive_delete gBaseAliveGameObjects;
    relive_delete gCollisions;

    if (mGameType == GameType::eAe)
    {
        MusicController::Shutdown();
    }
    else
    {
        AO::MusicController::Shutdown();
    }

    SND_Reset_Ambiance();
    SND_Shutdown();
    Input().ShutDown_45F020();
}

void Engine::Game_Main(EReliveLevelIds startLevel, s32 startPath, s32 startCamera)
{
    // Only returns once the engine is shutting down
    Game_Run(startLevel, startPath, startCamera);

    Game_Shutdown();
}

void Engine::Run()
{
    std::string activeModPath;
    std::string activeModDisplayName;

    std::string modName;
    if (mClp.ExtractNamePairArgument(modName, "-mod="))
    {
        LOG_INFO("Set active mod to be %s", modName.c_str());

        FileSystem::Path modsDir;
        modsDir.Append("relive_data").Append("mods");

        relive::Mods mods(mFs);
        mods.EnumerateMods(modsDir.GetPath());

        const relive::Mod* pMod = mods.FindByDirOrModName(modName);
        if (!pMod)
        {
            ALIVE_FATAL("Mod \"%s\" was set as the active mod on the command line but doesn't exist", modName.c_str());
        }

        const char_type* const expectedTargetGame = (mGameType == GameType::eAe) ? "AE" : "AO";
        if (strcmpi(pMod->mTargetGame.c_str(), expectedTargetGame) != 0)
        {
            ALIVE_FATAL("Mod \"%s\" targets \"%s\" but the active game is \"%s\"", modName.c_str(), pMod->mTargetGame.c_str(), expectedTargetGame);
        }

        activeModDisplayName = pMod->mName;
        activeModPath = modsDir.Append(pMod->mDirectory).GetPath();
    }

    mResMan = std::make_unique<ResourceManagerWrapper>(mFs, activeModPath);

    gPsxDisplay.Init(*mResMan);

    if (mGameType == GameType::eAe)
    {
        mMap = std::make_unique<Map>(*mResMan, mFactory);
    }
    else
    {
        mMap = std::make_unique<AO::Map>(*mResMan, mFactory);
    }
    GetGameAutoPlayer().ProcessCommandLine(mFs, mClp);

    sCommandLine_ShowFps = mClp.SwitchExists("-ddfps");
    gCommandLine_NoFrameSkip = mClp.SwitchExists("-ddnoskip");

    CmdLineRenderInit(activeModDisplayName);

    // Another hack till refactor branch replaces master
    GetGameAutoPlayer().Pause(true);
    GetGameAutoPlayer().DisableRecorder();

    // TODO: HACK mini loop till Game.cpp is merged
    DataConversionUI dcu(mGameType, *mResMan, *mMap);
    if (dcu.ConversionRequired())
    {
        do
        {
            dcu.VUpdate();

            dcu.VRender(gPsxDisplay.mDrawEnv.mOrderingTable);

            // Not the general SYS_EventsPump(mMap.get()) (which would just exit(0) immediately)
            // - dcu (and so any FMV conversion jobs it dispatched onto its own ThreadPool - see
            // fmv_converter.cpp) is only ever reachable from right here, so quitting while it's
            // still running is handled locally: ask it to cancel and give it a bounded window to
            // actually stop (FmvConv::Convert checks ThreadPool::IsCancelRequested() roughly once
            // per encoded frame and cleans up its own temp file) before exiting for real either
            // way, rather than a hard exit(0) leaving conversion jobs mid-write.
            if (Sys_PumpMessages(mMap.get()))
            {
                dcu.RequestCancel();

                constexpr u32 kMaxCancelWaitMs = 2000;
                const u32 waitStartTicks = SYS_GetTicks();
                while (dcu.AsyncTasksInProgress() && (SYS_GetTicks() - waitStartTicks) < kMaxCancelWaitMs)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }

                exit(0);
            }
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
        //Game_Main(EReliveLevelIds::eMines, 1, 4);
        Game_Main(EReliveLevelIds::eMenu, 1, 25);
    }
    else
    {
        LOG_INFO("AO standalone starting...");
        Game_Main(EReliveLevelIds::eMenu, 1, 10);
    }
}
