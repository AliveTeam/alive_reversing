#include "stdafx_ao.h"
#include "Game.hpp"
#include "Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "SwitchStates.hpp"
#include "DDCheat.hpp"
#include "../AliveLibAE/Psx.hpp"
#include "../AliveLibAE/Sys.hpp"
#include "../AliveLibAE/Io.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "ResourceManager.hpp"
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
#include "../relive_lib/ShadowZone.hpp"
#include "CameraSwapper.hpp"
#include "AmbientSound.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/Events.hpp"
#include "Sound.hpp"
#include "../AliveLibAE/Game.hpp"
#include "AddPointer.hpp"
#include "PathDataExtensions.hpp"
#include "GameAutoPlayer.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "GasCountDown.hpp"

namespace AO {

s16 gbKillUnsavedMudsDone_5076CC = 0;

// TODO: Move to game ender controller for AO sync
s16 gRestartRuptureFarmsKilledMuds_5076C4 = 0;
s16 gRestartRuptureFarmsSavedMuds_5076C8 = 0;

s16 gOldKilledMuds_5076D0 = 0;
s16 gOldSavedMuds_5076D4 = 0;

s16 sBreakGameLoop = 0;
s16 gAttract = 0;

s8 gDDCheatOn = 0;

s32 Game_End_Frame_4505D0(u32 bSkip)
{
    return Game_End_Frame_4950F0(bSkip);
}

static void Main_ParseCommandLineArguments(const char_type* pCommandLine)
{
    IO_Init_494230();

    std::string windowTitle = WindowTitleAO();

    if (GetGameAutoPlayer().IsRecording())
    {
        windowTitle += " [Recording]";
    }
    else if (GetGameAutoPlayer().IsPlaying())
    {
        windowTitle += " [AutoPlay]";
    }

    Sys_WindowClass_Register(windowTitle.c_str(), 32, 64, 640, 480);

    Sys_Set_Hwnd(Sys_GetWindowHandle());

    if (pCommandLine)
    {
        if (_strcmpi(pCommandLine, "-it_is_me_your_father") == 0)
        {
            gDDCheatOn = 1;
        }
        // Force DDCheat
#if FORCE_DDCHEAT
        gDDCheatOn = true;
#endif
    }

    VGA_CreateRenderer();

    PSX_EMU_SetCallBack_4F9430(Game_End_Frame_4505D0);
}

void Init_GameStates()
{
    sKilledMudokons = gRestartRuptureFarmsKilledMuds_5076C4;
    sRescuedMudokons = gRestartRuptureFarmsSavedMuds_5076C8;

    gGasOn = 0;
    sGasTimer = 0;

    gSwitchStates = {};
    gOldKilledMuds_5076D0 = 0;
    gOldSavedMuds_5076D4 = 0;
    gbKillUnsavedMudsDone_5076CC = 0;
}


void Init_Sound_DynamicArrays_And_Others()
{
    DebugFont_Init();

    gPauseMenu = nullptr;
    sActiveHero = nullptr;
    sControlledCharacter = nullptr;
    gNumCamSwappers = 0;
    sGnFrame = 0;

    gPlatformsArray = relive_new DynamicArrayT<BaseGameObject>(20); // For trap doors/dynamic platforms

    ShadowZone::MakeArray();

    gBaseAliveGameObjects = relive_new DynamicArrayT<IBaseAliveGameObject>(20);

    SND_Init_476E40();
    SND_Init_Ambiance();
    MusicController::Create();

    Init_GameStates(); // Init other vars + switch states

    // TODO: The switch state clearing is done in Init_GameStates in AE
    // check this is not an AO bug
    SwitchStates_ClearAll();
}

void Game_Init_LoadingIcon()
{
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

            if (pBaseGameObject->mBaseGameObjectFlags.Get(BaseGameObject::eUpdatable_Bit2)
			    && !pBaseGameObject->mBaseGameObjectFlags.Get(BaseGameObject::eDead) 
                && (gNumCamSwappers == 0 || pBaseGameObject->mBaseGameObjectFlags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
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
        PrimHeader** ppOt = gPsxDisplay.mDrawEnvs[gPsxDisplay.mBufferIndex].mOrderingTable;

        // Render objects
        GetGameAutoPlayer().SyncPoint(SyncPoints::DrawAllStart);
        for (s32 i = 0; i < gObjListDrawables->Size(); i++)
        {
            BaseGameObject* pDrawable = gObjListDrawables->ItemAt(i);
            if (!pDrawable)
            {
                break;
            }

            if (pDrawable->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                pDrawable->mBaseGameObjectFlags.Clear(BaseGameObject::eCantKill_Bit11);
            }
            else if (pDrawable->mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
            {
                pDrawable->mBaseGameObjectFlags.Set(BaseGameObject::eCantKill_Bit11);
                pDrawable->VRender(ppOt);
            }
        }
        GetGameAutoPlayer().SyncPoint(SyncPoints::DrawAllEnd);

        DebugFont_Flush();
        pScreenManager->VRender(ppOt);
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

            if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead) && pObj->mBaseGameObjectRefCount == 0)
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

    PSX_VSync_4F6170(0);

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

    BaseAnimatedWithPhysicsGameObject::MakeArray();

    AnimationBase::CreateAnimationArray();

    Init_Sound_DynamicArrays_And_Others();
    Input_Init();

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

    MusicController::Shutdown();

    SND_Reset_Ambiance();
    SND_Shutdown_476EC0();
    InputObject::Shutdown();
}


void Game_Main(const char_type* pCommandLine)
{
    GetGameAutoPlayer().ParseCommandLine(pCommandLine);

    Main_ParseCommandLineArguments(pCommandLine);

    // Only returns once the engine is shutting down
    Game_Run();

    Game_Shutdown();
}

} // namespace AO
