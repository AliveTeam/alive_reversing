#include "stdafx.h"
#include "AddPointer.hpp"
#include "Game.hpp"
#include "Sys.hpp"
#include "VGA.hpp"
#include "../relive_lib/bmp.hpp"
#include "Input.hpp"
#include "Psx.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "Sound/Sound.hpp" // for shut down func
#include "AmbientSound.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Map.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "stdlib.hpp"
#include "PauseMenu.hpp"
#include "GameSpeak.hpp"
#include "PathData.hpp"
#include "DDCheat.hpp"
#include "Io.hpp"
#include "Sound/Midi.hpp"
#include <atomic>
#include <fstream>
#include "DebugHelpers.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "PathData.hpp"
#include "MusicController.hpp"
#include <gmock/gmock.h>
#include "CheatController.hpp"
#include "FG1.hpp"
#include "PsxRender.hpp"
#include "Slurg.hpp"
#include "PathDataExtensions.hpp"
#include "GameAutoPlayer.hpp"
#include "Function.hpp"
#include "../relive_lib/ShadowZone.hpp"
#include "../relive_lib/ResourceManagerWrapper.hpp"
#include <string>
#include "Camera.hpp"

using TExitGameCallBack = AddPointer_t<void CC()>;

ALIVE_VAR(1, 0xBBFB00, TExitGameCallBack, sGame_OnExitCallback_BBFB00, nullptr);

ALIVE_VAR(1, 0x5C1B84, u32, sGnFrame, 0);

// Timer
ALIVE_VAR(1, 0xBBB9D4, u32, sTimer_period_BBB9D4, 0);

// Arrays of things
ALIVE_VAR(1, 0x5C1B78, DynamicArrayT<BaseGameObject>*, ObjList_5C1B78, nullptr);
ALIVE_VAR(1, 0x5BD4D8, DynamicArray*, ObjList_5BD4D8, nullptr);

ALIVE_VAR(1, 0x5C2FE0, s16, sBreakGameLoop_5C2FE0, 0);
ALIVE_VAR(1, 0x5C1B66, s16, sNum_CamSwappers_5C1B66, 0);
ALIVE_VAR(1, 0x5C2F78, s32, dword_5C2F78, 0);
ALIVE_VAR(1, 0x5C2FA0, s16, bSkipGameObjectUpdates_5C2FA0, 0);

ALIVE_ARY(1, 0x5CA488, s8, 30, sCdRomDrives_5CA488, {});
ALIVE_VAR(1, 0x5CA4D4, s32, dword_5CA4D4, 0);
ALIVE_VAR(1, 0x55EF90, s32, k1_dword_55EF90, 1);
ALIVE_VAR(1, 0x55EF88, bool, byte_55EF88, true);
ALIVE_VAR(1, 0x5CA4D0, bool, sCommandLine_ShowFps_5CA4D0, false);
ALIVE_VAR(1, 0x5CA4B5, bool, sCommandLine_DDCheatEnabled_5CA4B5, false);
ALIVE_VAR(1, 0x5CA4D2, bool, byte_5CA4D2, false);
ALIVE_VAR(1, 0x5CA4E0, s32, dword_5CA4E0, 0);

// Fps calcs
ALIVE_VAR(1, 0xBD0F08, s8, bQuitting_BD0F08, 0);
ALIVE_VAR(1, 0x55EFDC, f64, sFps_55EFDC, 0.0);
ALIVE_VAR(1, 0x5CA4DC, s32, sFrameDiff_5CA4DC, 0);
ALIVE_VAR(1, 0xC2D03C, s32, sNumRenderedPrims_C2D03C, 0);
ALIVE_VAR(1, 0x5CA300, s32, sFrameCount_5CA300, 0);

ALIVE_VAR(1, 0x5C1B94, s16, word_5C1B94, 0);

ALIVE_VAR(1, 0x5C2F6C, u32, dword_5C2F6C, 0);
ALIVE_VAR(1, 0x5C1BA0, u16, gAttract_5C1BA0, 0);
ALIVE_VAR(1, 0x5C2F70, u32, dword_5C2F70, 0);



ALIVE_VAR(1, 0x5c1b68, Abe*, sActiveHero, 0);

bool gDebugHelpersEnabled = false;

#include "GameEnderController.hpp"
#include "ColourfulMeter.hpp"
#include "GasCountDown.hpp"

void Init_GameStates_43BF40()
{
    sKilledMudokons_5C1BC0 = sFeeco_Restart_KilledMudCount_5C1BC6;
    sRescuedMudokons_5C1BC2 = sFeecoRestart_SavedMudCount_5C1BC8;

    // TODO: Check that the unused vars here really are unused
    //word_5C1BCA = 0;
    //word_5C1B= 0;
    //word_5C1BF0 = 0;

    gGasOn_5C1C00 = 0;
    sGasTimer_5C1BE8 = 0;

    gbDrawMeterCountDown_5C1BF8 = FALSE;
    gTotalMeterBars_5C1BFA = 0;

    gAbeBulletProof_5C1BDA = 0;

    SwitchStates_SetRange(2u, 255u);
}

bool Is_Cd_Rom_Drive_495470([[maybe_unused]] CHAR driveLetter)
{
#if _WIN32
    CHAR RootPathName[4] = {};
    strcpy(RootPathName, "C:\\");
    RootPathName[0] = driveLetter;
    return ::GetDriveTypeA(RootPathName) == DRIVE_CDROM;
#else
    return false;
#endif
}

void DestroyObjects_4A1F20()
{
    pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
    for (s32 iterations = 0; iterations < 2; iterations++)
    {
        s16 idx = 0;



        while (idx < gBaseGameObjects->Size())
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(idx);
            idx++;

            if (!pObj)
            {
                break;
            }

            if (!(pObj->mBaseGameObjectFlags.Get(BaseGameObject::eSurviveDeathReset_Bit9)))
            {
                DynamicArrayIter iter;
                iter.field_0_pDynamicArray = gBaseGameObjects;
                iter.field_4_idx = idx;
                iter.Remove_At_Iter();

                delete pObj;

                // Don't go forwards as we just removed an item otherwise we'd miss one
                idx = iter.field_4_idx;
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

void DrawFps_4952F0(Bitmap* pBmp, s32 x, s32 y, f32 fps)
{
    char_type strBuffer[125] = {};
    sprintf(strBuffer, "%02.1f fps ", fps);
    sNumRenderedPrims_C2D03C = 0;
    BMP_Draw_String_4F2230(pBmp, x, y, 0xFF80FFu, 1, strBuffer);
    Add_Dirty_Area_4ED970(0, 0, 97, 32);
}

void sub_4FBA20()
{
    
}

void CheckShiftCapslock_4953B0()
{
    
}

void Draw_Debug_Strings_4F2800()
{
    
}


s32 Game_End_Frame_4950F0(u32 flags)
{
    if (flags & 1)
    {
        turn_off_rendering_BD0F20 = 0;
        return 0;
    }

    /*
    const u8 oldShowVRam = sPsxEMU_show_vram_BD1465;
    if (sCommandLine_DDCheatEnabled_5CA4B5)
    {
        if (Input_IsVKPressed_4EDD40(VK_SCROLL))
        {
            sPsxEMU_show_vram_BD1465 = !sPsxEMU_show_vram_BD1465;
            while (Input_IsVKPressed_4EDD40(VK_SCROLL))
            {
                //nullsub_3();
                SYS_EventsPump_494580();
            }
        }
    }

    if (oldShowVRam && !sPsxEMU_show_vram_BD1465)
    {
        Add_Dirty_Area_4ED970(0, 0, 640, 240);
    }
    */

    const f64 fps = Calculate_FPS_495250(sFrameCount_5CA300);
    CheckShiftCapslock_4953B0();
    if (sCommandLine_ShowFps_5CA4D0)
    {
        Bitmap* pVram = spBitmap_C2D038;
        if (!spBitmap_C2D038)
        {
            pVram = &sPsxVram_C1D160;
        }

        DrawFps_4952F0(
            pVram,
            sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x + 4,
            sPSX_EMU_DrawEnvState_C3D080.field_0_clip.y + 4,
            static_cast<f32>(fps));
    }

    Draw_Debug_Strings_4F2800();
    ++sFrameCount_5CA300;

    if (Sys_PumpMessages_4EE4F4())
    {
        bQuitting_BD0F08 = 1;
        exit(0);
    }
    sub_4FBA20();
    return 0;
}

void VLC_Tables_Init_496720()
{
    
}

void Main_ParseCommandLineArguments_494EA0(const char_type* /*pCmdLineNotUsed*/, const char_type* pCommandLine)
{
    //nullsub_2(); // Note: Pruned
    IO_Init_494230();

    // Default the CD drive to C:
    char_type strDrive[3] = {};
    strcpy(strDrive, "C:");

    // Collect up all CD rom drives
    s32 pos = 0;
    for (s8 drive = 'D'; drive < 'Z'; drive++)
    {
        if (Is_Cd_Rom_Drive_495470(drive))
        {
            sCdRomDrives_5CA488[pos++] = drive;
        }
    }

    // Use the first found CD ROM drive as the game location
    if (sCdRomDrives_5CA488[0])
    {
        strDrive[0] = sCdRomDrives_5CA488[0];
    }

    PSX_EMU_Set_Cd_Emulation_Paths_4FAA70(".", strDrive, strDrive);

    std::string windowTitle = WindowTitleAE();
    
    if (GetGameAutoPlayer().IsRecording())
    {
        windowTitle += " [Recording]";
    }
    else if (GetGameAutoPlayer().IsPlaying())
    {
        windowTitle += " [AutoPlay]";
    }

    Sys_WindowClass_Register_4EE22F("ABE_WINCLASS", windowTitle.c_str(), 32, 64, 640, 480);
    Sys_Set_Hwnd_4F2C50(Sys_GetWindowHandle_4EE180());

    dword_5CA4D4 = 0;
    k1_dword_55EF90 = 1; // Global way to turn off semi trans rendering?
    byte_55EF88 = true;

    if (pCommandLine)
    {
        if (strstr(pCommandLine, "-ddfps"))
        {
            sCommandLine_ShowFps_5CA4D0 = true;
        }

        if (strstr(pCommandLine, "-ddnoskip"))
        {
            sCommandLine_NoFrameSkip = true;
        }

        if (strstr(pCommandLine, "-ddfast"))
        {
            byte_5CA4D2 = true;
            dword_5CA4D4 = 1;
            byte_55EF88 = false;
            dword_5CA4E0 = 2;
        }

        if (strstr(pCommandLine, "-ddfastest"))
        {
            dword_5CA4E0 = 1;
        }

        if (strstr(pCommandLine, "-ddcheat"))
        {
            sCommandLine_DDCheatEnabled_5CA4B5 = true;
        }

#if DEVELOPER_MODE
        if (strstr(pCommandLine, "-debug"))
        {
            gDebugHelpersEnabled = true;
        }
#endif
    }

    if (dword_5CA4E0 == 1)
    {
        PSX_DispEnv_Set_4ED960(1);
        PSX_EMU_Set_screen_mode_4F9420(1);
    }
    else if (dword_5CA4E0 == 2)
    {
        PSX_DispEnv_Set_4ED960(0);
        PSX_EMU_Set_screen_mode_4F9420(0);
    }
    else
    {
        PSX_DispEnv_Set_4ED960(2);
        PSX_EMU_Set_screen_mode_4F9420(2);
    }

    Init_VGA_AndPsxVram_494690();
    PSX_EMU_Init_4F9CD0(false);
    PSX_EMU_VideoAlloc_4F9D70();
    PSX_EMU_SetCallBack_4F9430(1, Game_End_Frame_4950F0);
    //Main_Set_HWND_4F9410(Sys_GetWindowHandle_4EE180()); // Note: Set but never read
    VLC_Tables_Init_496720();
}

s32 CreateTimer_4EDEC0(UINT /*uDelay*/, void* /*callBack*/)
{
    
    return 0;
}

void Init_Sound_DynamicArrays_And_Others_43BDB0()
{
    DebugFont_Init();
    word_5C1B94 = 1; // Used in dead overlay stuff, CD number ??
    //Overlays_Init_43BFC0(); // Note: Pointless because never used in PC
    pPauseMenu_5C9300 = nullptr;
    sActiveHero = nullptr;
    sControlledCharacter = 0;
    sNum_CamSwappers_5C1B66 = 0;
    sGnFrame = 0;
    sbLoadingInProgress_5C1B96 = 0;

    ObjList_5C1B78 = relive_new DynamicArrayT<BaseGameObject>(20); // For trap doors/dynamic platforms?

    ObjList_5BD4D8 = relive_new DynamicArray(10); // Never seems to be used?

    ShadowZone::MakeArray();

    gBaseAliveGameObjects = relive_new DynamicArrayT<BaseAliveGameObject>(20);

    ResourceManager::Init_49BCE0();
    SND_Init();
    SND_Init_Ambiance();
    MusicController::Create();
    Init_GameStates_43BF40(); // Init other vars + switch states
}

void SYS_EventsPump_494580()
{
    if (Sys_PumpMessages_4EE4F4())
    {
        bQuitting_BD0F08 = 1;
        exit(0);
    }
    sub_4FBA20();
}

void DDCheat_Allocate_415320()
{
    relive_new DDCheat();
}

void Game_Loop_467230();


//static AnimResource gLoadingResource;

void Game_Init_LoadingIcon_482CD0()
{
    //gLoadingResource = ResourceManagerWrapper::LoadAnimation(AnimId::Mudokon_FallLandDie);
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

void Game_Free_LoadingIcon_482D40()
{
    //gLoadingResource.Clear();
    /*
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kLoadingResID, 0, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_49C330(ppRes);
    }*/
}

#if DEVELOPER_MODE
extern bool gBootToLoadScreen;
#endif

void Game_Run_466D40()
{
    // Begin start up
    SYS_EventsPump_494580();
    dword_5C2F6C = 6000;
    gAttract_5C1BA0 = 0;
    dword_5C2F70 = 34;
    SYS_EventsPump_494580();

    PSX_ResetCallBack_4FAA20();
    gPsxDisplay.Init();
    PSX_CdInit_4FB2C0();
    PSX_CdSetDebug_4FB330(0);
    Input_Pads_Reset_4FA960(); // starts card/pads on psx ver

    gBaseGameObjects = relive_new DynamicArrayT<BaseGameObject>(50);

    gObjListDrawables = relive_new DynamicArrayT<BaseGameObject>(30);

    gFG1List_5D1E28 = relive_new DynamicArrayT<FG1>(4);

    AnimationBase::CreateAnimationArray();

    pResourceManager_5C1BB0 = relive_new ResourceManager();

    Init_Sound_DynamicArrays_And_Others_43BDB0();

    Camera camera;

    // Load the first camera we see on boot
    camera.field_C_pCamRes = ResourceManagerWrapper::LoadCam(EReliveLevelIds::eMenu, 1, 25);

    gMap.field_24_camera_offset.y = FP_FromInteger(0);
    gMap.field_24_camera_offset.x = FP_FromInteger(0);

    pScreenManager = relive_new ScreenManager(camera.field_C_pCamRes, &gMap.field_24_camera_offset);

    pScreenManager->DecompressCameraToVRam(camera.field_C_pCamRes);

    camera.Free();

    Input_Init_491BC0();
    s16 cameraId = 25;
#if DEVELOPER_MODE
    #if _WIN32
    if (GetKeyState(VK_LSHIFT) >= 0)
    {
        gBootToLoadScreen = true;
        cameraId = 1;
    }
    #endif
#endif

    gMap.Init(EReliveLevelIds::eMenu, 1, cameraId, CameraSwapEffects::eInstantChange_0, 0, 0);

    DDCheat_Allocate_415320();
    pEventSystem_5BC11C = relive_new GameSpeak();

    pCheatController_5BC120 = relive_new CheatController();

    if (gDebugHelpersEnabled)
    {
        DebugHelpers_Init(); // Custom helper code
    }

    Game_Init_LoadingIcon_482CD0();

    // Main loop start
    Game_Loop_467230();

    // Shut down start
    Game_Free_LoadingIcon_482D40();
    DDCheat::ClearProperties();
    gMap.Shutdown();

    AnimationBase::FreeAnimationArray();
    BaseAnimatedWithPhysicsGameObject::FreeArray();
    relive_delete gFG1List_5D1E28;
    relive_delete gBaseGameObjects;
    relive_delete ObjList_5C1B78;
    relive_delete ObjList_5BD4D8;
    ShadowZone::FreeArray();
    relive_delete gBaseAliveGameObjects;
    relive_delete sCollisions;

    pMusicController_5C3020 = nullptr; // Note: OG bug - should have been set to nullptr after shutdown call?
    MusicController::Shutdown();

    SND_Reset_Ambiance();
    SND_Shutdown();
    PSX_CdControlB_4FB320(8, 0, 0);
    PSX_ResetCallBack_4FAA20();
    PSX_StopCallBack_4FAA30();
    Input().ShutDown_45F020();
    PSX_ResetGraph_4F8800(0);
}

void Game_SetExitCallBack_4F2BA0(TExitGameCallBack callBack)
{
    sGame_OnExitCallback_BBFB00 = callBack;
}

void Game_ExitGame_4954B0()
{
    PSX_EMU_VideoDeAlloc_4FA010();
}

void Game_Shutdown_4F2C30()
{
    if (sGame_OnExitCallback_BBFB00)
    {
        sGame_OnExitCallback_BBFB00();
        sGame_OnExitCallback_BBFB00 = nullptr;
    }

    CreateTimer_4EDEC0(0, nullptr); // Creates a timer that calls a call back which is always null, therefore seems like dead code?
    Input_DisableInputForPauseMenuAndDebug_4EDDC0();
    //SND_MCI_Close_4F0060(nullptr); // TODO: Seems like more dead code because the mci is never set?
    GetSoundAPI().SND_SsQuit();
    IO_Stop_ASync_IO_Thread_4F26B0();
    VGA_Shutdown_4F3170();
}

s32 TMR_Init_4EDE20()
{
#if USE_SDL2
    return 0;
#else
    struct timecaps_tag ptc = {};
    if (::timeGetDevCaps(&ptc, sizeof(timecaps_tag)))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\TMR.C", 25, 0, "TMR_Init: timeGetDevCaps() failed !");
        return -1;
    }

    sTimer_period_BBB9D4 = ptc.wPeriodMin;
    // This makes timers as accurate as possible increasing cpu/power usage as a trade off
    ::timeBeginPeriod(ptc.wPeriodMin);
    return 0;
#endif
}

s32 Init_Input_Timer_And_IO_4F2BF0(bool forceSystemMemorySurfaces)
{
    static bool sbGameShutdownSet_BBC560 = false;
    if (!sbGameShutdownSet_BBC560)
    {
        // OG: Change - this gets called normally anyway, using atexit results in a f64 call that
        // will f64 free and use freed objects
        //atexit(Game_Shutdown_4F2C30);
        sbGameShutdownSet_BBC560 = 1;
        gVGA_force_sys_memory_surfaces_BC0BB4 = forceSystemMemorySurfaces;
    }

    Input_EnableInput_4EDDD0();
    Input_InitKeyStateArray_4EDD60();
    TMR_Init_4EDE20();

    if (!IO_CreateThread())
    {
        return -1;
    }

    if (strstr(Sys_GetCommandLine_4EE176(), "-syncread"))
    {
        sIOSyncReads_BD2A5C = TRUE;
    }
    else
    {
        sIOSyncReads_BD2A5C = FALSE;
    }
    return 0;
}

void Game_Main_4949F0()
{
    // Inits
    Init_Input_Timer_And_IO_4F2BF0(false);

    GetGameAutoPlayer().ParseCommandLine(Sys_GetCommandLine_4EE176());

    Main_ParseCommandLineArguments_494EA0(Sys_GetCommandLine_4EE176(), Sys_GetCommandLine_4EE176());

    Game_SetExitCallBack_4F2BA0(Game_ExitGame_4954B0);
#if _WIN32
    #if !USE_SDL2
    Sys_SetWindowProc_Filter_4EE197(Sys_WindowMessageHandler_494A40);
    #endif
#endif
    // Only returns once the engine is shutting down
    Game_Run_466D40();

    if (sGame_OnExitCallback_BBFB00)
    {
        sGame_OnExitCallback_BBFB00();
        sGame_OnExitCallback_BBFB00 = nullptr;
    }

    Game_Shutdown_4F2C30();
}

void Game_Loop_467230()
{
    dword_5C2F78 = 0;
    sBreakGameLoop_5C2FE0 = 0;
    bool bPauseMenuObjectFound = false;
    while (!gBaseGameObjects->IsEmpty())
    {
        GetGameAutoPlayer().SyncPoint(SyncPoints::MainLoopStart);

        EventsResetActive();
        Slurg::Clear_Slurg_Step_Watch_Points();
        bSkipGameObjectUpdates_5C2FA0 = 0;

        // Update objects
        GetGameAutoPlayer().SyncPoint(SyncPoints::ObjectsUpdateStart);
        for (s32 baseObjIdx = 0; baseObjIdx < gBaseGameObjects->Size(); baseObjIdx++)
        {
            BaseGameObject* pBaseGameObject = gBaseGameObjects->ItemAt(baseObjIdx);

            if (!pBaseGameObject || bSkipGameObjectUpdates_5C2FA0)
            {
                break;
            }

            if (pBaseGameObject->mBaseGameObjectFlags.Get(BaseGameObject::eUpdatable_Bit2)
                && pBaseGameObject->mBaseGameObjectFlags.Get(BaseGameObject::eDead) == false
                && (sNum_CamSwappers_5C1B66 == 0 || pBaseGameObject->mBaseGameObjectFlags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
            {
                const s32 updateDelay = pBaseGameObject->UpdateDelay();
                if (updateDelay <= 0)
                {
                    if (pBaseGameObject == pPauseMenu_5C9300)
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
        if (sNum_CamSwappers_5C1B66 <= 0)
        {
            GetGameAutoPlayer().SyncPoint(SyncPoints::AnimateAll);
            AnimationBase::AnimateAll(AnimationBase::gAnimations);
        }

        PrimHeader** ppOtBuffer = gPsxDisplay.mDrawEnvs[gPsxDisplay.mBufferIndex].mOrderingTable;

        // Render objects
        GetGameAutoPlayer().SyncPoint(SyncPoints::DrawAllStart);
        for (s32 i = 0; i < gObjListDrawables->Size(); i++)
        {
            BaseGameObject* pObj = gObjListDrawables->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                pObj->mBaseGameObjectFlags.Clear(BaseGameObject::eCantKill_Bit11);
            }
            else if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
            {
                pObj->mBaseGameObjectFlags.Set(BaseGameObject::eCantKill_Bit11);
                pObj->VRender(ppOtBuffer);
            }
        }
        GetGameAutoPlayer().SyncPoint(SyncPoints::DrawAllEnd);

        // Render FG1's
        for (s32 i = 0; i < gFG1List_5D1E28->Size(); i++)
        {
            FG1* pFG1 = gFG1List_5D1E28->ItemAt(i);
            if (!pFG1)
            {
                break;
            }

            if (pFG1->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                pFG1->mBaseGameObjectFlags.Clear(BaseGameObject::eCantKill_Bit11);
            }
            else if (pFG1->mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
            {
                pFG1->mBaseGameObjectFlags.Set(BaseGameObject::eCantKill_Bit11);
                pFG1->VRender(ppOtBuffer);
            }
        }

        DebugFont_Flush();
        PSX_DrawSync_4F6280(0);
        pScreenManager->VRender(ppOtBuffer);
        SYS_EventsPump_494580(); // Exit checking?

        GetGameAutoPlayer().SyncPoint(SyncPoints::RenderOT);
        gPsxDisplay.RenderOrderingTable();
        
        GetGameAutoPlayer().SyncPoint(SyncPoints::RenderStart);

        // Destroy objects with certain flags
        for (s16 idx = 0; idx < gBaseGameObjects->Size(); idx++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(idx);
            if (!pObj)
            {
                break;
            }

            if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead) && pObj->mBaseGameObjectFlags.Get(BaseGameObject::eCantKill_Bit11) == false)
            {
                DynamicArrayIter it;
                it.field_0_pDynamicArray = gBaseGameObjects;
                it.field_4_idx = idx + 1;

                it.Remove_At_Iter();
                delete pObj;
            }
        }

        GetGameAutoPlayer().SyncPoint(SyncPoints::RenderEnd);

        if (bPauseMenuObjectFound && pPauseMenu_5C9300)
        {
            pPauseMenu_5C9300->VUpdate();
        }

        bPauseMenuObjectFound = false;

        gMap.ScreenChange();
        Input().Update(GetGameAutoPlayer());

        if (sNum_CamSwappers_5C1B66 == 0)
        {
            GetGameAutoPlayer().SyncPoint(SyncPoints::IncrementFrame);
            sGnFrame++;
        }

        if (sBreakGameLoop_5C2FE0)
        {
            GetGameAutoPlayer().SyncPoint(SyncPoints::MainLoopExit);
            break;
        }

        // Enabled only for ddfast option
        if (byte_5CA4D2)
        {
            pResourceManager_5C1BB0->LoadingLoop_465590(0);
        }

        GetGameAutoPlayer().ValidateObjectStates();

    } // Main loop end

    // Clear the screen to black
    PSX_RECT rect = {};
    rect.x = 0;
    rect.y = 0;
    rect.w = 640;
    rect.h = 240;
    PSX_ClearImage_4F5BD0(&rect, 0, 0, 0);
    PSX_DrawSync_4F6280(0);
    PSX_VSync_4F6170(0);

    pResourceManager_5C1BB0->Shutdown_465610();

    // Destroy all game objects
    while (!gBaseGameObjects->IsEmpty())
    {
        DynamicArrayIter iter = {};
        iter.field_0_pDynamicArray = gBaseGameObjects;
        for (s16 idx = 0; idx < gBaseGameObjects->Size(); idx++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(idx);
            iter.field_4_idx = idx + 1;
            if (!pObj)
            {
                break;
            }
            iter.Remove_At_Iter();
            relive_delete pObj;
        }
    }
}
