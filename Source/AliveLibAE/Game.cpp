#include "stdafx.h"
#include "Game.hpp"
#include "../relive_lib/Sys.hpp"
#include "VGA.hpp"
#include "Input.hpp"
#include "../relive_lib/Psx.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../relive_lib/Sound/Sound.hpp" // for shut down func
#include "AmbientSound.hpp"
#include "ResourceManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Map.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "stdlib.hpp"
#include "PauseMenu.hpp"
#include "GameSpeak.hpp"
#include "DDCheat.hpp"
#include "Io.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include <fstream>
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "CheatController.hpp"
#include "Slurg.hpp"
#include "PathDataExtensions.hpp"
#include "GameAutoPlayer.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/ShadowZone.hpp"
#include "../relive_lib/ResourceManagerWrapper.hpp"
#include "GameEnderController.hpp"
#include "ColourfulMeter.hpp"
#include "GasCountDown.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Collisions.hpp"

u32 sGnFrame = 0;

// Arrays of things
DynamicArrayT<BaseGameObject>* gPlatformsArray = nullptr;

bool gBreakGameLoop = false;
s16 gNumCamSwappers = 0;
bool gSkipGameObjectUpdates = false;

bool sCommandLine_ShowFps = false;
bool gDDCheatOn = false;

// Fps calcs
f64 sFps_55EFDC = 0.0;
s32 sFrameDiff_5CA4DC = 0;
s32 sFrameCount_5CA300 = 0;

u16 gAttract = 0;

Abe* gAbe = nullptr;


void DestroyObjects()
{
    ResourceManagerWrapper::LoadingLoop(false);
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

f64 Calculate_FPS_495250(s32 frameCount)
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

void DrawFps_4952F0(s32 /*x*/, s32 /*y*/, f32 fps)
{
    char_type strBuffer[125] = {};
    sprintf(strBuffer, "%02.1f fps ", static_cast<f64>(fps));
    //BMP_Draw_String_4F2230(pBmp, x, y, strBuffer);
}

void Draw_Debug_Strings_4F2800()
{
    // TODO
}

s32 Game_End_Frame(u32 flags)
{
    if (flags & 1)
    {
        gTurnOffRendering = false;
        return 0;
    }

    const f64 fps = Calculate_FPS_495250(sFrameCount_5CA300);
    if (sCommandLine_ShowFps)
    {
        DrawFps_4952F0(10, 10, static_cast<f32>(fps));
    }

    Draw_Debug_Strings_4F2800();
    ++sFrameCount_5CA300;

    if (Sys_PumpMessages())
    {
        exit(0);
    }
    return 0;
}

void Init_GameStates()
{
    gKilledMudokons = gFeeco_Restart_KilledMudCount;
    gRescuedMudokons = gFeecoRestart_SavedMudCount;

    gGasOn = false;
    gGasTimer = 0;

    gbDrawMeterCountDown = false;
    gTotalMeterBars = 0;

    gAbeInvincible = false;

    SwitchStates_ClearRange(2u, 255u);
}

void Init_Sound_DynamicArrays_And_Others()
{
    DebugFont_Init();

    gPauseMenu = nullptr;
    gAbe = nullptr;
    sControlledCharacter = nullptr;
    gNumCamSwappers = 0;
    sGnFrame = 0;

    gPlatformsArray = relive_new DynamicArrayT<BaseGameObject>(20); // For trap doors/dynamic platforms

    ShadowZone::MakeArray();

    gBaseAliveGameObjects = relive_new DynamicArrayT<IBaseAliveGameObject>(20);

    SND_Init();
    SND_Init_Ambiance();
    MusicController::Create();

    Init_GameStates(); // Init other vars + switch states
}

void SYS_EventsPump()
{
    if (Sys_PumpMessages())
    {
        exit(0);
    }
}

void Game_Init_LoadingIcon()
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

void Game_Free_LoadingIcon()
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
    IO_Stop_ASync_IO_Thread_4F26B0();
    VGA_Shutdown();
}


void Game_Loop()
{
    gBreakGameLoop = false;
    bool bPauseMenuObjectFound = false;
    while (!gBaseGameObjects->IsEmpty())
    {
        GetGameAutoPlayer().SyncPoint(SyncPoints::MainLoopStart);

        EventsResetActive();
        Slurg::Clear_Slurg_Step_Watch_Points();
        gSkipGameObjectUpdates = false;

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

        DebugFont_Flush();
        gScreenManager->VRender(gPsxDisplay.mDrawEnv.mOrderingTable);
        SYS_EventsPump(); // Exit checking?

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

            if (pObj->GetDead() && !pObj->GetCantKill())
            {
                idx = gBaseGameObjects->RemoveAt(idx);
                delete pObj;
            }
        }

        GetGameAutoPlayer().SyncPoint(SyncPoints::RenderEnd);

        if (bPauseMenuObjectFound && gPauseMenu)
        {
            gPauseMenu->VUpdate();
        }

        bPauseMenuObjectFound = false;

        gMap.ScreenChange();
        Input().Update(GetGameAutoPlayer());

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

        if (pObjToKill->mBaseGameObjectRefCount == 0)
        {
            i = gBaseGameObjects->RemoveAt(i);
            delete pObjToKill;
        }
    }
}

void DDCheat_Allocate()
{
    relive_new DDCheat();
}

void Game_Run()
{
    // Begin start up
    SYS_EventsPump();

    gAttract = 0;

    SYS_EventsPump();

    gPsxDisplay.Init();
    Input_Pads_Reset_4FA960(); // starts card/pads on psx ver
    Input_EnableInput_4EDDD0();

    gBaseGameObjects = relive_new DynamicArrayT<BaseGameObject>(90);

    BaseAnimatedWithPhysicsGameObject::MakeArray(); // Makes drawables

    AnimationBase::CreateAnimationArray();

    // AO doesn't have an instance, only statics
    relive_new ResourceManager();

    if (GetGameAutoPlayer().IsPlaying())
    {
        // temp de-sync fix
        Init_Sound_DynamicArrays_And_Others();
        Input_Init();
    }
    else
    {
        // NOTE: We need to call Input_Init() before Init_Sound_DynamicArrays_And_Others() because of gLatencyHack
        // which can be configured from the ini
        Input_Init();
        Init_Sound_DynamicArrays_And_Others();
    }

    // Not technically needed yet but will de-sync if not instantiated here
    CamResource nullCamRes;
    gScreenManager = relive_new ScreenManager(nullCamRes, &gMap.mCameraOffset);

    gMap.Init(EReliveLevelIds::eMenu, 1, 25, CameraSwapEffects::eInstantChange_0, 0, 0);

    DDCheat_Allocate();

    gEventSystem = relive_new GameSpeak();

    gCheatController = relive_new CheatController();

    Game_Init_LoadingIcon();

    // Main loop start
    Game_Loop();

    // Shut down start
    Game_Free_LoadingIcon();

    DDCheat::ClearProperties();

    gMap.Shutdown();

    AnimationBase::FreeAnimationArray();
    BaseAnimatedWithPhysicsGameObject::FreeArray();
    relive_delete gBaseGameObjects;
    relive_delete gPlatformsArray;
    ShadowZone::FreeArray();
    relive_delete gBaseAliveGameObjects;
    relive_delete gCollisions;

    gMusicController = nullptr; // Note: OG bug - should have been set to nullptr after shutdown call?
    MusicController::Shutdown();

    SND_Reset_Ambiance();
    SND_Shutdown();
    Input().ShutDown_45F020();
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

void Game_Main()
{
    // Only returns once the engine is shutting down
    Game_Run();

    Game_Shutdown();
}
