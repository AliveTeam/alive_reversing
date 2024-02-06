#include "stdafx_ao.h"
#include "Game.hpp"
#include "../relive_lib/Function.hpp"
#include "../AliveLibAE/Input.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/Psx.hpp"
#include "../relive_lib/Sys.hpp"
#include "../AliveLibAE/Io.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Map.hpp"
#include "GameSpeak.hpp"
#include "CheatController.hpp"
#include "DDCheat.hpp"
#include "MusicController.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "Input.hpp"
#include "Midi.hpp"
#include "PauseMenu.hpp"
#include "Abe.hpp"
#include "../relive_lib/GameObjects/ShadowZone.hpp"
#include "CameraSwapper.hpp"
#include "AmbientSound.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/Events.hpp"
#include "Sound.hpp"
#include "../AliveLibAE/Game.hpp"
#include "PathDataExtensions.hpp"
#include "GameAutoPlayer.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "../AliveLibAE/GasCountDown.hpp"

// Note: Using AE var
//bool gDDCheatOn = false;

namespace AO {

bool gbKillUnsavedMudsDone = false;

// TODO: Move to game ender controller for AO sync
s16 gRestartRuptureFarmsKilledMuds = 0;
s16 gRestartRuptureFarmsSavedMuds = 0;

s16 sBreakGameLoop = 0;
s16 gAttract = 0;


void Init_GameStates()
{
    gKilledMudokons = gRestartRuptureFarmsKilledMuds;
    gRescuedMudokons = gRestartRuptureFarmsSavedMuds;

    gDeathGasOn = false;
    gDeathGasTimer = 0;

    gSwitchStates = {};
    gbKillUnsavedMudsDone = false;
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

    // TODO: The switch state clearing is done in Init_GameStates in AE
    // check this is not an AO bug
    SwitchStates_ClearRange(0, 255);
}

void Game_Init_LoadingIcon()
{
    ResourceManagerWrapper::PendAnimation(AnimId::Loading_Icon2);
    ResourceManagerWrapper::LoadingLoop2();

    /*
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kLoadingAOResID, 1, 0);
    if (!ppRes)
    {
        ResourceManager::LoadResourceFile_455270("LOADING.BAN", nullptr);
        ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kLoadingAOResID, 1, 0);
    }
    ResourceManager::Set_Header_Flags_4557D0(ppRes, ResourceManager::ResourceHeaderFlags::eNeverFree);
    */
}

void Game_Free_LoadingIcon()
{
    /*
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kLoadingAOResID, 0, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }
    */
}

void Game_Shutdown()
{
    Input_DisableInput();
    SND_SsQuit();
    IO_Stop_ASync_IO_Thread_4F26B0();
    VGA_Shutdown();
}


void Game_Loop()
{
    sBreakGameLoop = 0;
    bool bPauseMenuObjectFound = false;
    while (!gBaseGameObjects->IsEmpty())
    {
        GetGameAutoPlayer().SyncPoint(SyncPoints::MainLoopStart);

        EventsResetActive();

        // Update objects
        GetGameAutoPlayer().SyncPoint(SyncPoints::ObjectsUpdateStart);
        for (s32 baseObjIdx = 0; baseObjIdx < gBaseGameObjects->Size(); baseObjIdx++)
        {
            BaseGameObject* pBaseGameObject = gBaseGameObjects->ItemAt(baseObjIdx);

            if (!pBaseGameObject)
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

            if (pObj->GetDead() && pObj->mBaseGameObjectRefCount == 0)
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

        if (sBreakGameLoop)
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
    Input().InitPad(1);

    gBaseGameObjects = relive_new DynamicArrayT<BaseGameObject>(90);

    BaseAnimatedWithPhysicsGameObject::MakeArray(); // Makes drawables

    AnimationBase::CreateAnimationArray();

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

    gMap.Init(EReliveLevelIds::eMenu, 1, 10, CameraSwapEffects::eInstantChange_0, 0, 0);

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
    relive_delete gBaseAliveGameObjects;

    MusicController::Shutdown();

    SND_Reset_Ambiance();
    SND_Shutdown();
    InputObject::Shutdown();
}


void Game_Main()
{
    // Only returns once the engine is shutting down
    Game_Run();

    Game_Shutdown();
}

} // namespace AO
