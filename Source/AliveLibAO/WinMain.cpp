#include "stdafx_ao.h"
#include "WinMain.hpp"
#include "Function.hpp"
#include "bmp.hpp"
#include "Psx_common.hpp"
#include "stdlib.hpp"
#include "BaseGameObject.hpp"
#include "DynamicArray.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "Animation.hpp"
#include "FixedPoint_common.hpp"
#include "ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "Psx.hpp"
#include "Alarm.hpp"
#include "BaseAliveGameObject.hpp"
#include "PsxDisplay.hpp"
#include "AmbientSound.hpp"
#include "MusicController.hpp"
#include "GameSpeak.hpp"
#include "Input.hpp"
#include "DDCheat.hpp"
#include "CheatController.hpp"
#include "RollingBallShaker.hpp"
#include "Blood.hpp"
#include "Paramite.hpp"
#include "Scrab.hpp"
#include "Slog.hpp"
#include "Mudokon.hpp"
#include "SlingMudokon.hpp"
#include "CameraSwapper.hpp"
#include "HintFly.hpp"
#include "ZapLine.hpp"
#include "ScreenWave.hpp"
#include "Dove.hpp"
#include "LvlArchive.hpp"
#include "Font.hpp"
#include "GameEnderController.hpp"
#include "Spark.hpp"
#include "Events.hpp"
#include "../AliveLibCommon/config.h"
#include "Sfx.hpp"
#include "ShadowZone.hpp"
#include "PauseMenu.hpp"
#include "SwitchStates.hpp"
#include "Midi.hpp"
#include "Error.hpp"
#include "VGA.hpp"
#include "Sys.hpp"
#include "PsxRender.hpp"
#include "Psx.hpp"

START_NS_AO

void WinMain_ForceLink()
{

}

void Static_Inits_AO()
{
    Dove_static_ctor_40EE10();
    LvlArchive::ctor_static_41BBA0();
    FontContext::static_ctor_41C010();
    Map::ctor_static_443E10();
    LvlArchive::ctor_static_443E70();
}

void ForceLinks()
{
    RollingBallShaker_ForceLink();
    Blood_ForceLink();
    Paramite_ForceLink();
    Scrab_ForceLink();
    Slog_ForceLink();
    Mud_ForceLink();
    SlingMud_ForceLink();
    HintFly_ForceLink();
    ZapLine_ForceLink();
    Forcelink_ScreenWave();
    GameEnderController_ForceLink();
    Spark_ForceLink();
}

ALIVE_VAR(1, 0x9F771C, HINSTANCE, sInstance_9F771C, 0);
ALIVE_VAR(1, 0x9F7784, HINSTANCE, sPrevInstance_9F7784, 0);
ALIVE_VAR(1, 0x9F772C, int, sCmdShow_9F772C, 0);
ALIVE_VAR(1, 0x9F7718, LPSTR, sCommandLine_9F7718, 0);
ALIVE_VAR(1, 0x9F7734, int, sExitCode_9F7734, 0);
ALIVE_VAR(1, 0xA8A4C0, Bitmap, sVGA_Bmp1_A8A4C0, {});
ALIVE_VAR(1, 0x9F7724, HWND, gHwnd_9F7724, nullptr);
ALIVE_VAR(1, 0x508BF8, char, gDDCheatMode_508BF8, 0);
ALIVE_VAR(1, 0x508BFC, char, byte_508BFC, 0);


EXPORT int CC IO_Init_48E1A0(char /*bUnknown*/)
{
    // TODO: This has been totally changed in AE
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC Game_End_Frame_4505D0(DWORD /*bSkip*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void Input_GetCurrentKeyStates_48E630()
{
    NOT_IMPLEMENTED();
}

EXPORT char CC Input_IsVKPressed_48E5D0(int /*key*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT LPSTR CC GetCommandLine_48E920()
{
    return sCommandLine_9F7718;
}

static void Init_VGA_AndPsxVram()
{
    VGA_FullScreenSet_490160(true);
    VGA_DisplaySet_490230(640u, 480u, 16, 1, 0);

    RECT rect = {};
    rect.left = 0;
    rect.top = 0;
    rect.right = 640;
    rect.bottom = 480;
    BMP_ClearRect_48F810(&sVGA_Bmp1_A8A4C0, &rect, 0);
    switch (VGA_GetPixelFormat_490E60())
    {
    case 8:
        PSX_EMU_SetDispType_499E60(1);
        break;
    case 15:
        PSX_EMU_SetDispType_499E60(4);
        break;
    case 16:
        PSX_EMU_SetDispType_499E60(2);
        break;
    case 115:
        PSX_EMU_SetDispType_499E60(5);
        break;
    case 116:
        PSX_EMU_SetDispType_499E60(3);
        break;
    default:
        Error_WarningMessageBox_48E470("This program requires a high-color display mode of 32768 or 65536 colors at 640x480 resolution.");
        Error_ShowErrorStackToUser_48DF10(0);
        exit(0);
        return;
    }
}

ALIVE_ARY(1, 0x4CECC8, char, 3, gDriveLetter_4CECC8, { 'D', ':', '0' });

static void Main_ParseCommandLineArguments()
{
    IO_Init_48E1A0(0);
  
    // TODO: I guess some code got removed that picked the CD-ROM drive like in AE since this 
    // doesn't really make any sense anymore.
    char cdDrivePath[3] = {};
    cdDrivePath[0] = gDriveLetter_4CECC8[0];
    cdDrivePath[1] = gDriveLetter_4CECC8[1];
    if (gDriveLetter_4CECC8[0] > 'Z')
    {
        cdDrivePath[0] = 'C';
    }

    PSX_EMU_Set_Cd_Emulation_Paths_49B000(".", cdDrivePath, 0);
#ifdef _WIN32
    Sys_WindowClass_Register_48E9E0("ABE_WINCLASS", "Abe's Oddworld Oddysee 2.0", 32, 64, 640, 480); // Nice window title lol
#endif
    Sys_Set_Hwnd_48E340(Sys_GetWindowHandle_48E930());

    const LPSTR pCmdLine = GetCommandLine_48E920();
    if (pCmdLine)
    {
        if (!memcmp(pCmdLine, "-it_is_me_your_father", 22u))
        {
            Input_GetCurrentKeyStates_48E630();
            if (Input_IsVKPressed_48E5D0(VK_SHIFT))
            {
                gDDCheatMode_508BF8 = 1;
                PSX_DispEnv_Set_48D900(2);
                PSX_EMU_Set_screen_mode_499910(2);
            }
        }
        // Force DDCheat
#if DEVELOPER_MODE
        gDDCheatMode_508BF8 = 1;
#endif
    }

    if (!pCmdLine)
    {
        PSX_DispEnv_Set_48D900(2);
        PSX_EMU_Set_screen_mode_499910(2);
    }
    else
    {
        if (_strcmpi(pCmdLine, "-interline") == 0)
        {
            PSX_DispEnv_Set_48D900(1);
            PSX_EMU_Set_screen_mode_499910(1);
            byte_508BFC = 0;
        }
        else if (_strcmpi(pCmdLine, "-vstretch") == 0)
        {
            PSX_DispEnv_Set_48D900(0);
            PSX_EMU_Set_screen_mode_499910(0);
            byte_508BFC = 0;
        }
        else if (_strcmpi(pCmdLine, "-vdouble") == 0)
        {
            PSX_DispEnv_Set_48D900(0);
            PSX_EMU_Set_screen_mode_499910(0);
            byte_508BFC = 1;
        }
        else
        {
            PSX_DispEnv_Set_48D900(2);
            PSX_EMU_Set_screen_mode_499910(2);
        }
    }

    Init_VGA_AndPsxVram();

    PSX_EMU_Init_49A1D0(0);
    PSX_EMU_VideoAlloc_49A2B0();
    PSX_EMU_SetCallBack_499920(1, Game_End_Frame_4505D0);
    //Main_Set_HWND_499900(Sys_GetWindowHandle_48E930()); // Note: Set global is never read
}

EXPORT int CC DebugFont_Init_487EC0()
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC Init_Sound_DynamicArrays_And_Others_41CD20()
{
    DebugFont_Init_487EC0();

    for (OverlayRecord& rec : sOverlayTable_4C5AA8.records)
    {
        CdlFILE cdFile = {};
        CdlFILE* pFile = PSX_CdSearchFile_49B930(&cdFile, rec.field_0_fileName);
        if (pFile)
        {
            rec.field_4_pos = PSX_CdLoc_To_Pos_49B3B0(&pFile->field_0_loc);
        }
    }

    pPauseMenu_5080E0 = nullptr;
    sActiveHero_507678 = nullptr;
    sControlledCharacter_50767C = nullptr;
    sNumCamSwappers_507668 = 0;
    gnFrameCount_507670 = 0;

    gFilesPending_507714 = 0;
    bLoadingAFile_50768C = 0;

    ObjListPlatforms_50766C = ao_new<DynamicArrayT<BaseGameObject>>();
    ObjListPlatforms_50766C->ctor_4043E0(20);

    ObjList_5009E0 = ao_new<DynamicArrayT<ResourceManager::ResourceManager_FileRecord>>();
    ObjList_5009E0->ctor_4043E0(10);  // not used in AE

    sShadowZone_dArray_507B08 = ao_new<DynamicArrayT<ShadowZone>>();
    sShadowZone_dArray_507B08->ctor_4043E0(4);

    gBaseAliveGameObjects_4FC8A0 = ao_new<DynamicArrayT<BaseAliveGameObject>>();
    gBaseAliveGameObjects_4FC8A0->ctor_4043E0(20);

    ResourceManager::Init_454DA0();
    SND_Init_476E40();
    SND_Init_Ambiance_4765C0();
    MusicController::Create_4436C0();

    Init_GameStates_41CEC0(); // Note: inlined

    // TODO: The switch state clearing is done in Init_GameStates in AE
    // check this is not an AO bug
    SwitchStates_ClearAll();
}

EXPORT void CC Input_Init_44EB60()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC DDCheat_Allocate_409560()
{
    auto pDDCheat = ao_new<DDCheat>();
    if (pDDCheat)
    {
        pDDCheat->ctor_4095D0();
    }
}

EXPORT void CC Game_Init_LoadingIcon_445E30()
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 8010, 1, 0);
    if (!ppRes)
    {
        ResourceManager::LoadResourceFile_455270("LOADING.BAN", nullptr);
        ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 8010, 1, 0);
    }
    ResourceManager::Set_Header_Flags_4557D0(ppRes, ResourceManager::ResourceHeaderFlags::eNeverFree);
}

ALIVE_VAR(1, 0x507B78, short, sBreakGameLoop_507B78, 0);

ALIVE_VAR(1, 0x505564, DynamicArrayT<AnimationBase>*, gObjList_animations_505564, nullptr);


EXPORT void CC DebugFont_Flush_487F50()
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x507698, short, gAttract_507698, 0);
ALIVE_VAR(1, 0x507B0C, int, gTimeOut_NotUsed_507B0C, 0);
ALIVE_VAR(1, 0x507B10, int, gFileOffset_NotUsed_507B10, 0);

class BaseAliveGameObject;

EXPORT void CC Game_Loop_437630()
{
    sBreakGameLoop_507B78 = 0;

    while (!sBreakGameLoop_507B78 && !gBaseGameObject_list_9F2DF0->Empty())
    {
        Events_Reset_Active_417320();

        // Update objects
        for (int i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
        {
            BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            if (pObjIter->field_6_flags.Get(BaseGameObject::eUpdatable_Bit2) && 
                !pObjIter->field_6_flags.Get(BaseGameObject::eDead_Bit3) && 
                (sNumCamSwappers_507668 == 0 || pObjIter->field_6_flags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
            {
                if (pObjIter->field_8_update_delay > 0)
                {
                    pObjIter->field_8_update_delay--;
                }
                else
                {
                    pObjIter->VUpdate();
                }
            }
        }

        // Animate everything
        if (sNumCamSwappers_507668 <= 0)
        {
            AnimationBase::AnimateAll_4034F0(gObjList_animations_505564);
        }

        // Render objects
        int** ppOt = gPsxDisplay_504C78.field_C_drawEnv[gPsxDisplay_504C78.field_A_buffer_index].field_70_ot_buffer;

        for (int i=0; i < gObjList_drawables_504618->Size(); i++)
        {
            BaseGameObject* pDrawable = gObjList_drawables_504618->ItemAt(i);
            if (!pDrawable)
            {
                break;
            }

            if (!pDrawable->field_6_flags.Get(BaseGameObject::eDead_Bit3) && pDrawable->field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
            {
                pDrawable->VRender(ppOt);
            }
        }

        DebugFont_Flush_487F50();
        PSX_DrawSync_496750(0);
        pScreenManager_4FF7C8->VRender(ppOt);
        SYS_EventsPump_44FF90();

        gPsxDisplay_504C78.PSX_Display_Render_OT_40DD20();

        // Destroy objects with certain flags
        for (int i=0; i< gBaseGameObject_list_9F2DF0->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3) && pObj->field_C_refCount == 0)
            {
                i = gBaseGameObject_list_9F2DF0->RemoveAt(i);
                pObj->VDestructor(1);
            }
        }

        gMap_507BA8.ScreenChange_4444D0();
        sInputObject_5009E8.Update_433250();

        if (sNumCamSwappers_507668 == 0)
        {
            gnFrameCount_507670++;
        }
    }  // Main loop end

    const PSX_RECT rect = { 0, 0, 368, 480 };
    PSX_ClearImage_496020(&rect, 0, 0, 0);
    PSX_DrawSync_496750(0);
    PSX_VSync_496620(0);
    ResourceManager::WaitForPendingResources_41EA60(0);

    for (int i=0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObjToKill = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObjToKill)
        {
            break;
        }

        if (pObjToKill->field_C_refCount == 0)
        {
            i = gBaseGameObject_list_9F2DF0->RemoveAt(i);
            pObjToKill->VDestructor(1);
        }
    }
}

EXPORT void CC Game_Free_LoadingIcon_445E80()
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 8010, 0, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }
}

EXPORT void CC SND_Shutdown_476EC0()
{
    NOT_IMPLEMENTED();
}

EXPORT void Game_Run_4373D0()
{
    SYS_EventsPump_44FF90();

    gAttract_507698 = 0;
    gTimeOut_NotUsed_507B0C = 6000;
    gFileOffset_NotUsed_507B10 = 34;

    DebugOut_495990("Abe's Oddysee Attract=%d Timeout=%d FileOffset=%d DA Track=NA\n", 0, 200, 34);
    SYS_EventsPump_44FF90();
    PSX_ResetCallBack_49AFB0();

    //Nop_49BAF0();
    //Nop_49BB50();

    gPsxDisplay_504C78.ctor_40DAB0(&gPsxDisplayParams_4BB830);
    sInputObject_5009E8.InitPad_4331A0(1);

    gBaseGameObject_list_9F2DF0 = ao_new<DynamicArrayT<BaseGameObject>>();
    gBaseGameObject_list_9F2DF0->ctor_4043E0(90);

    gObjList_drawables_504618 = ao_new<DynamicArrayT<BaseGameObject>>();
    gObjList_drawables_504618->ctor_4043E0(80);

    gObjList_animations_505564 = ao_new<DynamicArrayT<AnimationBase>>();
    gObjList_animations_505564->ctor_4043E0(80);

    Init_Sound_DynamicArrays_And_Others_41CD20();
    Input_Init_44EB60();

#if DEVELOPER_MODE
    // Boot directly to the "abe hello" screen
    gMap_507BA8.Init_443EE0(LevelIds::eMenu_0, 1, 1, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
#else
    // Normal copy right screen boot
    gMap_507BA8.Init_443EE0(LevelIds::eMenu_0, 1, 10, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
#endif

    DDCheat_Allocate_409560();

    pEventSystem_4FF954 = ao_new<GameSpeak>();
    pEventSystem_4FF954->ctor_40F990();

    pCheatController_4FF958 = ao_new<CheatController>();
    pCheatController_4FF958->ctor_40FBF0();

    Game_Init_LoadingIcon_445E30();
    Game_Loop_437630();
    Game_Free_LoadingIcon_445E80();

    DDCheat::ClearProperties_4095B0();

    gMap_507BA8.Shutdown_443F90();

    if (gObjList_animations_505564)
    {
        gObjList_animations_505564->dtor_404440();
        ao_delete_free_447540(gObjList_animations_505564);
    }

    if (gObjList_drawables_504618)
    {
        gObjList_drawables_504618->dtor_404440();
        ao_delete_free_447540(gObjList_drawables_504618);
    }

    if (gBaseGameObject_list_9F2DF0)
    {
        gBaseGameObject_list_9F2DF0->dtor_404440();
        ao_delete_free_447540(gBaseGameObject_list_9F2DF0);
    }

    MusicController::Shutdown_4437E0();
    SND_Reset_Ambiance_4765E0();
    SND_Shutdown_476EC0();
    PSX_CdControlB_49BB40(8, 0, 0);
    PSX_ResetCallBack_49AFB0();
    PSX_StopCallBack_49AFC0();
    InputObject::Shutdown_433230();
    PSX_ResetGraph_4987E0(3);

    DebugOut_495990("Abe's Oddysee Demo Done\n");
}

using TExitGameCB = std::add_pointer<void CC()>::type;

EXPORT void CC Game_SetExitCallBack_48E040(TExitGameCB)
{
    NOT_IMPLEMENTED();
}


EXPORT void CC Game_ExitGame_450730()
{
    PSX_EMU_VideoDeAlloc_49A550();
}


EXPORT void CC Game_Shutdown_48E050()
{
    NOT_IMPLEMENTED();
}

EXPORT void Game_Main_450050()
{
    Alarm_ForceLink();
    BaseAliveGameObject_ForceLink();

    Main_ParseCommandLineArguments();
    Game_SetExitCallBack_48E040(Game_ExitGame_450730);
#ifdef _WIN32
    // Only SDL2 supported in AO
    //Sys_SetWindowProc_Filter_48E950(Sys_WindowMessageHandler_4503B0);
#endif
    Game_Run_4373D0();

    // TODO: AE inlined calls here (pull AE's code into another func)
    Game_Shutdown_48E050();
}

EXPORT int CALLBACK WinMain_48EF50(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    ForceLinks();
    sInstance_9F771C = hInstance;
    sPrevInstance_9F7784 = hPrevInstance;
    sCmdShow_9F772C = nShowCmd;
    sCommandLine_9F7718 = lpCmdLine;
    Game_Main_450050();
    return sExitCode_9F7734;
}

END_NS_AO
