#include "stdafx.h"
#include "WinAPISupport.hpp"
#include "Game.hpp"
#include "Sys.hpp"
#include "VGA.hpp"
#include "bmp.hpp"
#include "Input.hpp"
#include "Error.hpp"
#include "Psx.hpp"
#include "DynamicArray.hpp"
#include "Sound.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "PsxDisplay.hpp"
#include "Map.hpp"
#include "ScreenManager.hpp"
#include "Animation.hpp"
#include "stdlib.hpp"
#include "PauseMenu.hpp"
#include "GameSpeak.hpp"
#include "PathData.hpp"
#include "DDCheat.hpp"
#include "QuikSave.hpp"
#include "Io.hpp"
#include "LvlArchive.hpp"
#include "DDraw.hpp"
#include "Midi.hpp"
#include <atomic>
#include <fstream>
#include "DebugHelpers.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "PathData.hpp"
#include "MusicController.hpp"
#include <gmock/gmock.h>
#include "CheatController.hpp"
#include "FG1.hpp"
#include "PsxRender.hpp"
#include "Slurg.hpp"
#include "Movie.hpp"
#include "Masher.hpp"

void Game_ForceLink() { }

using TExitGameCallBack = std::add_pointer<void CC()>::type;

ALIVE_VAR(1, 0xBBFB00, TExitGameCallBack, sGame_OnExitCallback_BBFB00, nullptr);

ALIVE_VAR(1, 0x5C1B84, unsigned int, sGnFrame_5C1B84, 0);

// Timer
ALIVE_VAR(1, 0xBBB9D4, DWORD, sTimer_period_BBB9D4, 0);

// I/O
ALIVE_VAR(1, 0xBD2A5C, BOOL, sIOSyncReads_BD2A5C, FALSE);
ALIVE_VAR(1, 0xBBC55C, HANDLE, sIoThreadHandle_BBC55C, nullptr);

// Arrays of things
ALIVE_VAR(1, 0x5C1B78, DynamicArrayT<BaseGameObject>*, ObjList_5C1B78, nullptr);
ALIVE_VAR(1, 0x5BD4D8, DynamicArray*, ObjList_5BD4D8, nullptr);
ALIVE_VAR(1, 0x5C1B80, DynamicArrayT<ShadowZone>*, sShadowZone_dArray_5C1B80, nullptr);

ALIVE_VAR(1, 0x5C2FE0, short, sBreakGameLoop_5C2FE0, 0);
ALIVE_VAR(1, 0x5C1B66, short, sNum_CamSwappers_5C1B66, 0);
ALIVE_VAR(1, 0x5C2F78, int, dword_5C2F78, 0);
ALIVE_VAR(1, 0x5C2FA0, short, bSkipGameObjectUpdates_5C2FA0, 0);

ALIVE_ARY(1, 0x5CA488, char, 30, sCdRomDrives_5CA488, {});
ALIVE_VAR(1, 0x5CA4D4, int, dword_5CA4D4, 0);
ALIVE_VAR(1, 0x55EF90, int, k1_dword_55EF90, 1);
ALIVE_VAR(1, 0x55EF88, bool, byte_55EF88, true);
ALIVE_VAR(1, 0x5CA4D0, bool, sCommandLine_ShowFps_5CA4D0, false);
ALIVE_VAR(1, 0x5CA4B5, bool, sCommandLine_DDCheatEnabled_5CA4B5, false);
ALIVE_VAR(1, 0x5CA4D2, bool, byte_5CA4D2, false);
ALIVE_VAR(1, 0x5CA4E0, int, dword_5CA4E0, 0);

// Fps calcs
ALIVE_VAR(1, 0xBD0F08, char, byte_BD0F08, 0);
ALIVE_VAR(1, 0x55EFDC, double, sFps_55EFDC, 0.0);
ALIVE_VAR(1, 0x5CA4DC, int, sFrameDiff_5CA4DC, 0);
ALIVE_VAR(1, 0xC2D03C, int, sNumRenderedPrims_C2D03C, 0);
ALIVE_VAR(1, 0x5CA300, int, sFrameCount_5CA300, 0);

ALIVE_VAR(1, 0x5C1B94, short, word_5C1B94, 0);
ALIVE_VAR(1, 0x5C2A4C, Abe, stru_5C2A4C, {});
ALIVE_VAR(1, 0x554D5C, Abe*, spAbe_554D5C, &stru_5C2A4C);


ALIVE_VAR(1, 0x5C2F6C, DWORD, dword_5C2F6C, 0);
ALIVE_VAR(1, 0x5C1BA0, WORD, word_5C1BA0, 0);
ALIVE_VAR(1, 0x5C2F70, DWORD, dword_5C2F70, 0);



ALIVE_VAR(1, 0x5c1b68, Abe *, sActiveHero_5C1B68, 0);

static bool gDebugHelpersEnabled = false;

FP CC ScaleToGridSize_4498B0(FP scaleFP)
{
    if (scaleFP == FP_FromDouble(0.5))
    {
        return FP_FromInteger(13); // 25/2
    }
    if (scaleFP == FP_FromDouble(1.0))
    {
        return FP_FromInteger(25);
    }
    return FP_FromInteger(0);
}


EXPORT bool CC Is_Cd_Rom_Drive_495470(CHAR driveLetter)
{
    CHAR RootPathName[4] = {};
    strcpy(RootPathName, "C:\\");
    RootPathName[0] = driveLetter;
    return ::GetDriveTypeA(RootPathName) == DRIVE_CDROM;
}

void DestroyObjects_4A1F20()
{
    pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
    for (int iterations =0; iterations < 2; iterations++)
    {
        short idx = 0;
        
      

        while (idx < gBaseGameObject_list_BB47C4->Size())
        {
            BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(idx);
            idx++;

            if (!pObj)
            {
                break;
            }

            if (!(pObj->field_6_flags.Get(BaseGameObject::eSurviveDeathReset)))
            {
                DynamicArrayIter iter;
                iter.field_0_pDynamicArray = gBaseGameObject_list_BB47C4;
                iter.field_4_idx = idx;
                iter.Remove_At_Iter_40CCA0();

                pObj->VDestructor(1);

                // Don't go forwards as we just removed an item otherwise we'd miss one
                idx = iter.field_4_idx;
            }
        }
    }

}

EXPORT double CC Calculate_FPS_495250(int frameCount)
{
    static DWORD sLastTime_5CA338 = timeGetTime() - 500;
    const DWORD curTime = timeGetTime();
    const int timeDiff = curTime - sLastTime_5CA338;

    if (static_cast<signed int>((curTime - sLastTime_5CA338)) < 500)
    {
        return sFps_55EFDC;
    }

    const int diffFrames = frameCount - sFrameDiff_5CA4DC;
    sFps_55EFDC = static_cast<double>(diffFrames) * 1000.0 / static_cast<double>(timeDiff);

    sLastTime_5CA338 = curTime;
    sFrameDiff_5CA4DC = frameCount;
    return sFps_55EFDC;
}

EXPORT void CC DrawFps_4952F0(Bitmap* pBmp, int x, int y, float fps)
{
    char strBuffer[125] = {};
    sprintf(strBuffer, "%02.1f fps ", fps);
    sNumRenderedPrims_C2D03C = 0;
    BMP_Draw_String_4F2230(pBmp, x, y, 0xFF80FFu, 1, strBuffer);
    Add_Dirty_Area_4ED970(0, 0, 97, 32);
}

EXPORT void __cdecl sub_4FBA20()
{
    NOT_IMPLEMENTED();
}

EXPORT void __cdecl CheckShiftCapslock_4953B0()
{
    NOT_IMPLEMENTED();
}

EXPORT void __cdecl Draw_Debug_Strings_4F2800()
{
    NOT_IMPLEMENTED();
}


EXPORT int CC Game_End_Frame_4950F0(DWORD flags)
{
    if (flags & 1)
    {
        byte_BD0F20 = 0;
        return 0;
    }

    const BYTE oldShowVRam = sPsxEMU_show_vram_BD1465;
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

    const double fps = Calculate_FPS_495250(sFrameCount_5CA300);
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
            static_cast<float>(fps));
    }

    Draw_Debug_Strings_4F2800();
    ++sFrameCount_5CA300;

    if (Sys_PumpMessages_4EE4F4())
    {
        byte_BD0F08 = 1;
        exit(0);
    }
    sub_4FBA20();
    return 0;
}

EXPORT void CC VLC_Tables_Init_496720()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Main_ParseCommandLineArguments_494EA0(const char* /*pCmdLineNotUsed*/, const char* pCommandLine)
{
    //nullsub_2(); // Note: Pruned
    IO_Init_494230();

    // Default the CD drive to C:
    char strDrive[3] = {};
    strcpy(strDrive, "C:");

    // Collect up all CD rom drives
    int pos = 0;
    for (char drive = 'D'; drive < 'Z'; drive++)
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
#if BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE
    #ifdef BUILD_NUMBER
        // Automated AppVeyor build title
        const char* kWindowTitle = "Oddworld Abe's Exoddus (AV Build: " BUILD_NUMBER ")";
    #else
        // Local build title
        const char* kWindowTitle = "Oddworld Abe's Exoddus";
    #endif
#else
    // Original build title
    const char* kWindowTitle = "Oddworld Abe's Exoddus";
#endif

    Sys_WindowClass_Register_4EE22F("ABE_WINCLASS", kWindowTitle, 32, 64, 640, 480);
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
            sCommandLine_NoFrameSkip_5CA4D1 = true;
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

EXPORT int CC CreateTimer_4EDEC0(UINT /*uDelay*/, void* /*callBack*/)
{
    NOT_IMPLEMENTED();
    return 0;
}


ALIVE_VAR(1, 0x5C1A24, DynamicArrayT<Animation>*, gObjList_animations_5C1A24, nullptr);
ALIVE_VAR(1, 0x5C1124, DynamicArrayT<BaseGameObject>*, gObjList_drawables_5C1124, nullptr);

EXPORT void CC Init_GameStates_43BF40()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC static_init_set_default_hero_43EC10()
{
    sActiveHero_5C1B68 = spAbe_554D5C;
}

EXPORT void CC Init_Sound_DynamicArrays_And_Others_43BDB0()
{
    DebugFont_Init_4DCF40();
    word_5C1B94 = 1; // Used in dead overlay stuff, CD number ??
    //Overlays_Init_43BFC0(); // Note: Pointless because never used in PC
    pPauseMenu_5C9300 = nullptr;
    sActiveHero_5C1B68 = spAbe_554D5C;
    sControlledCharacter_5C1B8C = 0;
    sNum_CamSwappers_5C1B66 = 0;
    sGnFrame_5C1B84 = 0;
    sbLoadingInProgress_5C1B96 = 0;

    ObjList_5C1B78 = alive_new<DynamicArrayT<BaseGameObject>>(); // For trap doors/dynamic platforms?
    ObjList_5C1B78->ctor_40CA60(20);

    ObjList_5BD4D8 = alive_new<DynamicArray>();
    ObjList_5BD4D8->ctor_40CA60(10); // Never seems to be used?

    sShadowZone_dArray_5C1B80 = alive_new<DynamicArrayT<ShadowZone>>();
    sShadowZone_dArray_5C1B80->ctor_40CA60(4);

    gBaseAliveGameObjects_5C1B7C = alive_new<DynamicArrayT<BaseAliveGameObject>>();
    gBaseAliveGameObjects_5C1B7C->ctor_40CA60(20);

    ResourceManager::Init_49BCE0();
    SND_Init_4CA1F0();
    SND_Init_Ambiance_4CB480();
    MusicController::Create_47FC40();
    Init_GameStates_43BF40(); // Init other vars + switch states
}

EXPORT void CC SYS_EventsPump_494580()
{
    if (Sys_PumpMessages_4EE4F4())
    {
        byte_BD0F08 = 1;
        exit(0);
    }
    sub_4FBA20();
}

EXPORT void CC DDCheat_Allocate_415320()
{
    auto pDDCheat = alive_new<DDCheat>();
    pDDCheat->ctor_4153C0();
}

EXPORT void CC Game_Loop_467230();


EXPORT void CC Game_Init_LoadingIcon_482CD0()
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kLoadingResID, 1u, 0);
    if (!ppRes)
    {
        ResourceManager::LoadResourceFile_49C170("LOADING.BAN", nullptr);
        ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kLoadingResID, 1u, 0);
    }
    ResourceManager::Set_Header_Flags_49C650(ppRes, ResourceManager::ResourceHeaderFlags::eNeverFree);
}

EXPORT void CC Game_Free_LoadingIcon_482D40()
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kLoadingResID, 0, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_49C330(ppRes);
    }
}

#if DEVELOPER_MODE
extern bool gBootToLoadScreen;
#endif

EXPORT void CC Game_Run_466D40()
{
    // Begin start up
    SYS_EventsPump_494580();
    dword_5C2F6C = 6000;
    word_5C1BA0 = 0;
    dword_5C2F70 = 34;
    SYS_EventsPump_494580();

    PSX_ResetCallBack_4FAA20();
    gPsxDisplay_5C1130.ctor_41DC30();
    PSX_CdInit_4FB2C0();
    PSX_CdSetDebug_4FB330(0);
    Input_Pads_Reset_4FA960(); // starts card/pads on psx ver

    gBaseGameObject_list_BB47C4 = alive_new<DynamicArrayT<BaseGameObject>>();
    gBaseGameObject_list_BB47C4->ctor_40CA60(50);

    gObjList_drawables_5C1124 = alive_new<DynamicArrayT<BaseGameObject>>();
    gObjList_drawables_5C1124->ctor_40CA60(30);

    gFG1List_5D1E28 = alive_new<DynamicArrayT<FG1>>();
    gFG1List_5D1E28->ctor_40CA60(4);


    gObjList_animations_5C1A24 = alive_new<DynamicArrayT<Animation>>();
    gObjList_animations_5C1A24->ctor_40CA60(30);

    pResourceManager_5C1BB0 = alive_new<ResourceManager>();
    pResourceManager_5C1BB0->ctor_464910();

    Init_Sound_DynamicArrays_And_Others_43BDB0();

    Camera camera;
    camera.ctor_480DD0();

    sLvlArchive_5BC520.Open_Archive_432E80(CdLvlName(LevelIds::eMenu_0));
    ResourceManager::LoadResourceFile_49C170("STP01C25.CAM", &camera);

    camera.field_C_pCamRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Bits, 125, 1u, 0);
    gMap_5C3030.field_24_camera_offset.field_4_y = FP_FromInteger(0);
    gMap_5C3030.field_24_camera_offset.field_0_x = FP_FromInteger(0);

    pScreenManager_5BB5F4 = alive_new<ScreenManager>();
    pScreenManager_5BB5F4->ctor_40E3E0(camera.field_C_pCamRes, &gMap_5C3030.field_24_camera_offset);

    pScreenManager_5BB5F4->DecompressToVRam_40EF60((unsigned __int16 **)camera.field_C_pCamRes);
    pScreenManager_5BB5F4->MoveImage_40EB70();

    sLvlArchive_5BC520.Free_433130();

    camera.dtor_480E00();

    Input_Init_491BC0();
    short cameraId = 25;
#if DEVELOPER_MODE
    if (GetKeyState(VK_LSHIFT) >= 0)
    {
        gBootToLoadScreen = true;
        cameraId = 1;
    }
#endif
    gMap_5C3030.Init_4803F0(LevelIds::eMenu_0, 1, cameraId, CameraSwapEffects::eEffect0_InstantChange, 0, 0);

    DDCheat_Allocate_415320();
    pEventSystem_5BC11C = alive_new<GameSpeak>();
    pEventSystem_5BC11C->ctor_421820();

    pCheatController_5BC120 = alive_new<CheatController>();
    pCheatController_5BC120->ctor_421BD0();

    Game_Init_LoadingIcon_482CD0();

    // Main loop start
    Game_Loop_467230();

    // Shut down start
    Game_Free_LoadingIcon_482D40();
    DDCheat::ClearProperties_415390();
    gMap_5C3030.Shutdown_4804E0();

    if (gObjList_animations_5C1A24)
    {
        gObjList_animations_5C1A24->dtor_40CAD0();
        Mem_Free_495540(gObjList_animations_5C1A24);
    }

    if (gObjList_drawables_5C1124)
    {
        gObjList_drawables_5C1124->dtor_40CAD0();
        Mem_Free_495540(gObjList_drawables_5C1124);
    }

    if (gFG1List_5D1E28)
    {
        gFG1List_5D1E28->dtor_40CAD0();
        Mem_Free_495540(gFG1List_5D1E28);
    }

    if (gBaseGameObject_list_BB47C4)
    {
        gBaseGameObject_list_BB47C4->dtor_40CAD0();
        Mem_Free_495540(gBaseGameObject_list_BB47C4);
    }

    if (ObjList_5C1B78)
    {
        ObjList_5C1B78->dtor_40CAD0();
        Mem_Free_495540(ObjList_5C1B78);
    }

    if (ObjList_5BD4D8)
    {
        ObjList_5BD4D8->dtor_40CAD0();
        Mem_Free_495540(ObjList_5BD4D8);
    }

    if (sShadowZone_dArray_5C1B80)
    {
        sShadowZone_dArray_5C1B80->dtor_40CAD0();
        Mem_Free_495540(sShadowZone_dArray_5C1B80);
    }

    if (gBaseAliveGameObjects_5C1B7C)
    {
        gBaseAliveGameObjects_5C1B7C->dtor_40CAD0();
        Mem_Free_495540(gBaseAliveGameObjects_5C1B7C);
    }

    if (sCollisions_DArray_5C1128)
    {
        sCollisions_DArray_5C1128->dtor_4189F0();
        Mem_Free_495540(sCollisions_DArray_5C1128);
    }

    pMusicController_5C3020 = nullptr; // Note: OG bug - should have been set to nullptr after shutdown call?
    MusicController::Shutdown_47FD20();

    SND_Reset_Ambiance_4CB4B0();
    SND_Shutdown_4CA280();
    PSX_CdControlB_4FB320(8, 0, 0);
    PSX_ResetCallBack_4FAA20();
    PSX_StopCallBack_4FAA30();
    sInputObject_5BD4E0.ShutDown_45F020();
    PSX_ResetGraph_4F8800(0);
}

EXPORT void CC Game_SetExitCallBack_4F2BA0(TExitGameCallBack callBack)
{
    sGame_OnExitCallback_BBFB00 = callBack;
}

EXPORT void CC Game_ExitGame_4954B0()
{
    PSX_EMU_VideoDeAlloc_4FA010();
}

EXPORT void CC IO_Stop_ASync_IO_Thread_4F26B0()
{
    if (sIoThreadHandle_BBC55C)
    {
        ::CloseHandle(sIoThreadHandle_BBC55C);
        sIoThreadHandle_BBC55C = nullptr;
    }
}

EXPORT void CC Game_Shutdown_4F2C30()
{
    if (sGame_OnExitCallback_BBFB00)
    {
        sGame_OnExitCallback_BBFB00();
        sGame_OnExitCallback_BBFB00 = nullptr;
    }

    CreateTimer_4EDEC0(0, nullptr); // Creates a timer that calls a call back which is always null, therefore seems like dead code?
    Input_DisableInput_4EDDC0();
    //SND_MCI_Close_4F0060(nullptr); // TODO: Seems like more dead code because the mci is never set?
    SND_SsQuit_4EFD50();
    IO_Stop_ASync_IO_Thread_4F26B0();
    VGA_Shutdown_4F3170();
}

EXPORT signed int TMR_Init_4EDE20()
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

EXPORT signed int CC Init_Input_Timer_And_IO_4F2BF0(bool forceSystemMemorySurfaces)
{
    static bool sbGameShutdownSet_BBC560 = false;
    if (!sbGameShutdownSet_BBC560)
    {
        atexit(Game_Shutdown_4F2C30);
        sbGameShutdownSet_BBC560 = 1;
        gVGA_force_sys_memory_surfaces_BC0BB4 = forceSystemMemorySurfaces;
    }

    Input_EnableInput_4EDDD0();
    Input_InitKeyStateArray_4EDD60();
    TMR_Init_4EDE20();

    if (!sIoThreadHandle_BBC55C)
    {
        sIoThreadHandle_BBC55C = ::CreateThread(
            0,
            0x4000u,
            FS_IOThread_4F25A0,
            0,
            0,
            &sIoThreadId_BBC558);
        if (!sIoThreadHandle_BBC55C)
        {
            return -1;
        }
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

EXPORT void CC Game_Main_4949F0()
{
    // Inits
    Init_Input_Timer_And_IO_4F2BF0(false);
    
    Main_ParseCommandLineArguments_494EA0(Sys_GetCommandLine_4EE176(), Sys_GetCommandLine_4EE176());
    Game_SetExitCallBack_4F2BA0(Game_ExitGame_4954B0);
    Sys_SetWindowProc_Filter_4EE197(Sys_WindowMessageHandler_494A40);
    
    // Only returns once the engine is shutting down
    Game_Run_466D40();

    if (sGame_OnExitCallback_BBFB00)
    {
        sGame_OnExitCallback_BBFB00();
        sGame_OnExitCallback_BBFB00 = nullptr;
    }

    Game_Shutdown_4F2C30();
}

EXPORT void CC Game_Loop_467230()
{
    if (gDebugHelpersEnabled)
    {
        DebugHelpers_Init(); // Custom helper code
    }

    dword_5C2F78 = 0;
    sBreakGameLoop_5C2FE0 = 0;
    bool bPauseMenuObjectFound = false;
    while (!gBaseGameObject_list_BB47C4->IsEmpty())
    {
        Events_Reset_Active_422DA0();
        Slurg::Clear_Slurg_Step_Watch_Points_449A90();
        bSkipGameObjectUpdates_5C2FA0 = 0;

        // Update objects
        for (int baseObjIdx = 0; baseObjIdx < gBaseGameObject_list_BB47C4->Size(); baseObjIdx++)
        {
            BaseGameObject* pBaseGameObject = gBaseGameObject_list_BB47C4->ItemAt(baseObjIdx);

            if (!pBaseGameObject || bSkipGameObjectUpdates_5C2FA0)
            {
                break;
            }

            if (pBaseGameObject->field_6_flags.Get(BaseGameObject::eUpdatable)
                && pBaseGameObject->field_6_flags.Get(BaseGameObject::eDead) == false
                && (sNum_CamSwappers_5C1B66 == 0 || pBaseGameObject->field_6_flags.Get(BaseGameObject::eUpdateDuringCamSwap)))
            {
                if (pBaseGameObject->field_1C_update_delay <= 0)
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
                    pBaseGameObject->field_1C_update_delay--;
                }
            }
        }

        // Animate everything
        if (sNum_CamSwappers_5C1B66 <= 0)
        {
            Animation::AnimateAll_40AC20(gObjList_animations_5C1A24);
        }

        int** pOtBuffer = gPsxDisplay_5C1130.field_10_drawEnv[gPsxDisplay_5C1130.field_C_buffer_index].field_70_ot_buffer;
        
        // Render objects
        for (int i=0; i < gObjList_drawables_5C1124->Size(); i++)
        {
            BaseGameObject* pObj = gObjList_drawables_5C1124->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->field_6_flags.Get(BaseGameObject::eDead))
            {
                pObj->field_6_flags.Clear(BaseGameObject::eCantKill);
            }
            else if (pObj->field_6_flags.Get(BaseGameObject::eDrawable))
            {
                pObj->field_6_flags.Set(BaseGameObject::eCantKill);
                pObj->VRender(pOtBuffer);
            }
        }

        // Render FG1's
        for (int i=0; i < gFG1List_5D1E28->Size(); i++)
        {
            FG1* pFG1 = gFG1List_5D1E28->ItemAt(i);
            if (!pFG1)
            {
                break;
            }

            if (pFG1->field_6_flags.Get(BaseGameObject::eDead))
            {
                pFG1->field_6_flags.Clear(BaseGameObject::eCantKill);
            }
            else if (pFG1->field_6_flags.Get(BaseGameObject::eDrawable))
            {
                pFG1->field_6_flags.Set(BaseGameObject::eCantKill);
                pFG1->VRender(pOtBuffer);
            }
        }
        
        DebugFont_Flush_4DD050();
        PSX_DrawSync_4F6280(0);
        pScreenManager_5BB5F4->VRender(pOtBuffer);
        SYS_EventsPump_494580(); // Exit checking?

        gPsxDisplay_5C1130.PSX_Display_Render_OT_41DDF0();
        
        // Destroy objects with certain flags
        for (short idx = 0; idx < gBaseGameObject_list_BB47C4->Size(); idx++)
        {
            BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(idx);
            if (!pObj)
            {
                break;
            }

            if (pObj->field_6_flags.Get(BaseGameObject::eDead) && pObj->field_6_flags.Get(BaseGameObject::eCantKill) == false)
            {
                DynamicArrayIter it;
                it.field_0_pDynamicArray = gBaseGameObject_list_BB47C4;
                it.field_4_idx = idx + 1;

                it.Remove_At_Iter_40CCA0();
                pObj->VDestructor(1);
            }
        }

        if (bPauseMenuObjectFound && pPauseMenu_5C9300)
        {
            pPauseMenu_5C9300->VUpdate();
        }

        bPauseMenuObjectFound = false;

        gMap_5C3030.sub_480B80();
        sInputObject_5BD4E0.Update_45F040();

        if (sNum_CamSwappers_5C1B66 == 0)
        {
            sGnFrame_5C1B84++;
        }

        if (sBreakGameLoop_5C2FE0)
        {
            break;
        }
        
        // Enabled only for ddfast option
        if (byte_5CA4D2)
        {
            pResourceManager_5C1BB0->LoadingLoop_465590(0);
        }
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
    while (!gBaseGameObject_list_BB47C4->IsEmpty())
    {
        DynamicArrayIter iter = {};
        iter.field_0_pDynamicArray = gBaseGameObject_list_BB47C4;
        for (short idx =0; idx < gBaseGameObject_list_BB47C4->Size(); idx++)
        {
            BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(idx);
            iter.field_4_idx = idx + 1;
            if (!pObj)
            {
                break;
            }
            iter.Remove_At_Iter_40CCA0();
            pObj->VDestructor(1);
        }
    }
}


namespace Test
{
    static void Test_PSX_getTPage_4F60E0()
    {
        ASSERT_EQ(0, PSX_getTPage_4F60E0(0, 0, 0, 0));

        ASSERT_EQ(32, PSX_getTPage_4F60E0(0, 1, 0, 0));
        ASSERT_EQ(64, PSX_getTPage_4F60E0(0, 2, 0, 0));
        ASSERT_EQ(96, PSX_getTPage_4F60E0(0, 3, 0, 0));

        ASSERT_EQ(128, PSX_getTPage_4F60E0(1, 0, 0, 0));
        ASSERT_EQ(256, PSX_getTPage_4F60E0(2, 0, 0, 0));
        ASSERT_EQ(384, PSX_getTPage_4F60E0(3, 0, 0, 0));

        ASSERT_EQ(1, PSX_getTPage_4F60E0(0, 0, 64, 0));
        ASSERT_EQ(2, PSX_getTPage_4F60E0(0, 0, 64 * 2, 64));
        ASSERT_EQ(18, PSX_getTPage_4F60E0(0, 0, 64 * 2, 64 * 4));

    }

    void GameTests()
    {
        Test_PSX_getTPage_4F60E0();
    }
}
