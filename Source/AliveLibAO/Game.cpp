#include "stdafx_ao.h"
#include "Game.hpp"
#include "logger.hpp"
#include "Function.hpp"
#include "FixedPoint.hpp"
#include "BaseGameObject.hpp"
#include "SwitchStates.hpp"
#include "DDCheat.hpp"
#include "Io.hpp"
#include "Psx.hpp"
#include "Sys.hpp"
#include "DynamicArray.hpp"
#include "BaseAliveGameObject.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "PsxDisplay.hpp"
#include "Map.hpp"
#include "GameSpeak.hpp"
#include "CheatController.hpp"
#include "DDCheat.hpp"
#include "MusicController.hpp"
#include "VGA.hpp"
#include "Input.hpp"
#include "Midi.hpp"
#include "PauseMenu.hpp"
#include "Abe.hpp"
#include "ShadowZone.hpp"
#include "CameraSwapper.hpp"
#include "AmbientSound.hpp"
#include "PsxRender.hpp"
#include "ScreenManager.hpp"
#include "Error.hpp"
#include "Events.hpp"
#include "Sound.hpp"
#include "../AliveLibAE/Game.hpp"
#include "AddPointer.hpp"
#include "PathDataExtensions.hpp"
#include "GameAutoPlayer.hpp"

namespace AO {

DynamicArrayT<BaseGameObject>* gLoadingFiles = nullptr;

// TODO: Move these few funcs to correct location
#ifdef _WIN32
EXPORT s32 CC Add_Dirty_Area_48D910(s32, s32, s32, s32)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT s32 MessageBox_48E3F0(const char_type* /*pTitle*/, s32 /*lineNumber*/, const char_type* /*pMsg*/, ...)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT s32 CC Sys_WindowMessageHandler_4503B0(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT ret = 0;

    switch (msg)
    {
        case WM_PAINT:
        {
            RECT rect = {};
            PAINTSTRUCT paint = {};
            BeginPaint(hWnd, &paint);
            GetClientRect(hWnd, &rect);
            PatBlt(paint.hdc, 0, 0, rect.right, rect.bottom, BLACKNESS); // use pal 0
            EndPaint(hWnd, &paint);
            Add_Dirty_Area_48D910(0, 0, 640, 240);
        }
            return 1;

        case WM_CLOSE:
            return (MessageBoxA(hWnd, "Do you really want to quit ?", "Abe's Oddysee", MB_DEFBUTTON2 | MB_ICONQUESTION | MB_YESNO) == IDNO) ? -1 : 0;

        case WM_KEYDOWN:
            if (wParam == VK_F1)
            {
                MessageBox_48E3F0(
                    "About Abe",
                    -1,
                    "Oddworld Abe's Oddysee 2.0\nPC version by Digital Dialect\n\nBuild date: %s %s\n",
                    "Oct 22 1997",
                    "14:32:52");
                Input_InitKeyStateArray_48E5F0();
            }
            Input_SetKeyState_48E610(static_cast<s32>(wParam), 1);
            return 0;

        case WM_KEYUP:
            Input_SetKeyState_48E610(static_cast<s32>(wParam), 0);
            break;

        case WM_SETCURSOR:
        {
            static auto hCursor = LoadCursor(nullptr, IDC_ARROW);
            SetCursor(hCursor);
        }
            return -1;

    #ifndef BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE
        case WM_NCLBUTTONDOWN:
            // Prevent window being moved when click + dragged
            return -1;
    #endif

        case WM_ACTIVATE:
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
        case WM_ENTERMENULOOP:
        case WM_EXITMENULOOP:
        case WM_ENTERSIZEMOVE:
        case WM_EXITSIZEMOVE:
            Input_InitKeyStateArray_48E5F0();
            break;

        case WM_INITMENUPOPUP:
            // TODO: Constants for wParam
            if ((u32) lParam >> 16)
            {
                return -1;
            }
            break;

        case WM_SYSKEYDOWN:
            // TODO: Constants for wParam
            if (wParam == 18 || wParam == 32)
            {
                ret = -1;
            }
            Input_SetKeyState_48E610(static_cast<s32>(wParam), 1);
            break;

        case WM_SYSKEYUP:
            // TODO: Constants for wParam
            if (wParam == 18 || wParam == 32)
            {
                ret = -1;
            }
            Input_SetKeyState_48E610(static_cast<s32>(wParam), 0);
            break;

        case WM_TIMER:
            return 1;
        default:
            return static_cast<s32>(ret);
    }
    return static_cast<s32>(ret);
}
using TFilter = AddPointer_t<s32 CC(HWND, UINT, WPARAM, LPARAM)>;

EXPORT void CC Sys_SetWindowProc_Filter_48E950(TFilter)
{
    NOT_IMPLEMENTED();
}

#endif


ALIVE_VAR(1, 0x507670, u32, gnFrameCount_507670, 0);
ALIVE_VAR(1, 0x504618, DynamicArrayT<BaseGameObject>*, gObjList_drawables_504618, nullptr);

ALIVE_VAR(1, 0x50766C, DynamicArrayT<BaseGameObject>*, ObjListPlatforms_50766C, nullptr);

void Game_ForceLink()
{
}

ALIVE_VAR(1, 0x5076CC, s16, gbKillUnsavedMudsDone_5076CC, 0);

// TODO: Move to game ender controller for AO sync
ALIVE_VAR(1, 0x5076C4, s16, gRestartRuptureFarmsKilledMuds_5076C4, 0);
ALIVE_VAR(1, 0x5076C8, s16, gRestartRuptureFarmsSavedMuds_5076C8, 0);

ALIVE_VAR(1, 0x5076D0, s16, gOldKilledMuds_5076D0, 0);
ALIVE_VAR(1, 0x5076D4, s16, gOldSavedMuds_5076D4, 0);

ALIVE_VAR(1, 0x507B78, s16, sBreakGameLoop_507B78, 0);
ALIVE_VAR(1, 0x507698, s16, gAttract_507698, 0);
ALIVE_VAR(1, 0x507B0C, s32, gTimeOut_NotUsed_507B0C, 0);
ALIVE_VAR(1, 0x507B10, s32, gFileOffset_NotUsed_507B10, 0);
ALIVE_VAR(1, 0x505564, DynamicArrayT<AnimationBase>*, gObjList_animations_505564, nullptr);

ALIVE_VAR(1, 0x508BF8, s8, gDDCheatMode_508BF8, 0);
ALIVE_VAR(1, 0x508BFC, s8, byte_508BFC, 0);

ALIVE_ARY(1, 0x4CECC8, s8, 3, gDriveLetter_4CECC8, {'D', ':', '0'});



EXPORT s32 CC Game_End_Frame_4505D0(u32 bSkip)
{
    return Game_End_Frame_4950F0(bSkip);
}

static void Main_ParseCommandLineArguments()
{
    IO_Init_48E1A0(0);

    // TODO: I guess some code got removed that picked the CD-ROM drive like in AE since this
    // doesn't really make any sense anymore.
    char_type cdDrivePath[3] = {};
    cdDrivePath[0] = gDriveLetter_4CECC8[0];
    cdDrivePath[1] = gDriveLetter_4CECC8[1];
    if (gDriveLetter_4CECC8[0] > 'Z')
    {
        cdDrivePath[0] = 'C';
    }

    PSX_EMU_Set_Cd_Emulation_Paths_49B000(".", cdDrivePath, nullptr);

    std::string windowTitle = WindowTitleAO();

    if (GetGameAutoPlayer().IsRecording())
    {
        windowTitle += " [Recording]";
    }
    else if (GetGameAutoPlayer().IsPlaying())
    {
        windowTitle += " [AutoPlay]";
    }

    Sys_WindowClass_Register_48E9E0("ABE_WINCLASS", windowTitle.c_str(), 32, 64, 640, 480);

    Sys_Set_Hwnd_48E340(Sys_GetWindowHandle_48E930());

    const LPSTR pCmdLine = Sys_GetCommandLine_48E920();
    if (pCmdLine)
    {
        if (_strcmpi(pCmdLine, "-it_is_me_your_father") == 0)
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
#if FORCE_DDCHEAT
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

    PSX_EMU_Init_49A1D0(false);
    PSX_EMU_VideoAlloc_49A2B0();
    PSX_EMU_SetCallBack_499920(1, Game_End_Frame_4505D0);
    //Main_Set_HWND_499900(Sys_GetWindowHandle_48E930()); // Note: Set global is never read
}

EXPORT void CC Init_GameStates_41CEC0()
{
    sKilledMudokons_5076BC = gRestartRuptureFarmsKilledMuds_5076C4;
    sRescuedMudokons_5076C0 = gRestartRuptureFarmsSavedMuds_5076C8;
    sSwitchStates_505568 = {};
    gOldKilledMuds_5076D0 = 0;
    gOldSavedMuds_5076D4 = 0;
    gbKillUnsavedMudsDone_5076CC = 0;
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
    ObjList_5009E0->ctor_4043E0(10); // not used in AE

    sShadowZone_dArray_507B08 = ao_new<DynamicArrayT<ShadowZone>>();
    sShadowZone_dArray_507B08->ctor_4043E0(4);

    gBaseAliveGameObjects_4FC8A0 = ao_new<DynamicArrayT<BaseAliveGameObject>>();
    gBaseAliveGameObjects_4FC8A0->ctor_4043E0(20);

    gLoadingFiles = ao_new<DynamicArrayT<BaseGameObject>>();
    gLoadingFiles->ctor_4043E0(20); // TODO: Leaked on purpose for now

    ResourceManager::Init_454DA0();
    SND_Init_476E40();
    SND_Init_Ambiance_4765C0();
    MusicController::Create_4436C0();

    Init_GameStates_41CEC0(); // Note: inlined

    // TODO: The switch state clearing is done in Init_GameStates in AE
    // check this is not an AO bug
    SwitchStates_ClearAll();
}

EXPORT void CC Game_Init_LoadingIcon_445E30()
{
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kLoadingAOResID, 1, 0);
    if (!ppRes)
    {
        ResourceManager::LoadResourceFile_455270("LOADING.BAN", nullptr);
        ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kLoadingAOResID, 1, 0);
    }
    ResourceManager::Set_Header_Flags_4557D0(ppRes, ResourceManager::ResourceHeaderFlags::eNeverFree);
}

EXPORT void CC Game_Free_LoadingIcon_445E80()
{
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kLoadingAOResID, 0, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }
}


using TExitGameCB = AddPointer_t<void CC()>;

ALIVE_VAR(1, 0x9F664C, TExitGameCB, sGame_OnExitCallback_9F664C, nullptr);

EXPORT void CC Game_SetExitCallBack_48E040(TExitGameCB)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Game_ExitGame_450730()
{
    PSX_EMU_VideoDeAlloc_49A550();
}


EXPORT s32 CC CreateTimer_48F030(s32, void*)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC Game_Shutdown_48E050()
{
    if (sGame_OnExitCallback_9F664C)
    {
        sGame_OnExitCallback_9F664C();
        sGame_OnExitCallback_9F664C = nullptr;
    }

    CreateTimer_48F030(0, nullptr); // Creates a timer that calls a call back which is always null, therefore seems like dead code?
    Input_DisableInput_48E690();
    //SND_MCI_Close_493C30(); // TODO: Seems like more dead code because the mci is never set?
    SND_SsQuit_4938E0();
    IO_Stop_ASync_IO_Thread_491A80();
    VGA_Shutdown_4900E0();
    Error_ShowErrorStackToUser_48DF10(true);
}


EXPORT void CC Game_Loop_437630()
{
    sBreakGameLoop_507B78 = 0;
    bool bPauseMenuObjectFound = false;
    while (!gBaseGameObject_list_9F2DF0->Empty())
    {
        GetGameAutoPlayer().SyncPoint(SyncPoints::MainLoopStart);

        Events_Reset_Active_417320();

        // Update objects
        GetGameAutoPlayer().SyncPoint(SyncPoints::ObjectsUpdateStart);
        for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
        {
            BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            if (pObjIter->field_6_flags.Get(BaseGameObject::eUpdatable_Bit2) && !pObjIter->field_6_flags.Get(BaseGameObject::eDead_Bit3) && (sNumCamSwappers_507668 == 0 || pObjIter->field_6_flags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
            {
                if (pObjIter->field_8_update_delay > 0)
                {
                    pObjIter->field_8_update_delay--;
                }
                else
                {
                    if (pObjIter == pPauseMenu_5080E0)
                    {
                        bPauseMenuObjectFound = true;
                    }
                    else
                    {
                        pObjIter->VUpdate();
                    }
                }
            }
        }

        for (s32 i = 0; i < gLoadingFiles->Size(); i++)
        {
            BaseGameObject* pObjIter = gLoadingFiles->ItemAt(i);
            if (pObjIter->field_6_flags.Get(BaseGameObject::eUpdatable_Bit2) && !pObjIter->field_6_flags.Get(BaseGameObject::eDead_Bit3) && (sNumCamSwappers_507668 == 0 || pObjIter->field_6_flags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
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

            if (pObjIter->field_6_flags.Get(BaseGameObject::eDead_Bit3) && pObjIter->field_C_refCount == 0)
            {
                i = gLoadingFiles->RemoveAt(i);
                pObjIter->VDestructor(1);
            }
        }

        GetGameAutoPlayer().SyncPoint(SyncPoints::ObjectsUpdateEnd);

        // Animate everything
        if (sNumCamSwappers_507668 <= 0)
        {
            GetGameAutoPlayer().SyncPoint(SyncPoints::AnimateAll);
            AnimationBase::AnimateAll_4034F0(gObjList_animations_505564);
        }

        // Render objects
        PrimHeader** ppOt = gPsxDisplay_504C78.field_C_drawEnv[gPsxDisplay_504C78.field_A_buffer_index].field_70_ot_buffer;

        GetGameAutoPlayer().SyncPoint(SyncPoints::DrawAllStart);
        for (s32 i = 0; i < gObjList_drawables_504618->Size(); i++)
        {
            BaseGameObject* pDrawable = gObjList_drawables_504618->ItemAt(i);
            if (!pDrawable)
            {
                break;
            }

            if (pDrawable->field_6_flags.Get(BaseGameObject::eDead_Bit3))
            {
                pDrawable->field_6_flags.Clear(BaseGameObject::eCantKill_Bit11);
            }
            else if (pDrawable->field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
            {
                pDrawable->field_6_flags.Set(BaseGameObject::eCantKill_Bit11);
                pDrawable->VRender(ppOt);
            }
        }
        GetGameAutoPlayer().SyncPoint(SyncPoints::DrawAllEnd);

        DebugFont_Flush_487F50();
        PSX_DrawSync_496750(0);
        pScreenManager_4FF7C8->VRender(ppOt);
        SYS_EventsPump_44FF90();

        GetGameAutoPlayer().SyncPoint(SyncPoints::RenderOT);
        gPsxDisplay_504C78.PSX_Display_Render_OT_40DD20();

        GetGameAutoPlayer().SyncPoint(SyncPoints::RenderStart);

        // Destroy objects with certain flags
        for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
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

        GetGameAutoPlayer().SyncPoint(SyncPoints::RenderEnd);

        if (bPauseMenuObjectFound && pPauseMenu_5080E0)
        {
            pPauseMenu_5080E0->VUpdate();
        }

        bPauseMenuObjectFound = false;

        gMap_507BA8.ScreenChange_4444D0();
        Input().Update(GetGameAutoPlayer());

        if (sNumCamSwappers_507668 == 0)
        {
            GetGameAutoPlayer().SyncPoint(SyncPoints::IncrementFrame);
            gnFrameCount_507670++;
        }

        if (sBreakGameLoop_507B78)
        {
            GetGameAutoPlayer().SyncPoint(SyncPoints::MainLoopExit);
            break;
        }

        GetGameAutoPlayer().ValidateObjectStates();

    } // Main loop end

    const PSX_RECT rect = {0, 0, 368, 480};
    PSX_ClearImage_496020(&rect, 0, 0, 0);
    PSX_DrawSync_496750(0);
    PSX_VSync_496620(0);
    ResourceManager::WaitForPendingResources_41EA60(0);

    for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
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

EXPORT void CC DDCheat_Allocate_409560()
{
    auto pDDCheat = ao_new<DDCheat>();
    if (pDDCheat)
    {
        pDDCheat->ctor_4095D0();
    }
}

EXPORT void Game_Run_4373D0()
{
    SYS_EventsPump_44FF90();

    gAttract_507698 = 0;
    gTimeOut_NotUsed_507B0C = 6000;
    gFileOffset_NotUsed_507B10 = 34;

    DDCheat::DebugStr_495990("Abe's Oddysee Attract=%d Timeout=%d FileOffset=%d DA Track=NA\n", 0, 200, 34);
    SYS_EventsPump_44FF90();
    PSX_ResetCallBack_49AFB0();

    //Nop_49BAF0();
    //Nop_49BB50();

    gPsxDisplay_504C78.ctor_40DAB0(&gPsxDisplayParams_4BB830);
    Input().InitPad_4331A0(1);

    gBaseGameObject_list_9F2DF0 = ao_new<DynamicArrayT<BaseGameObject>>();
    gBaseGameObject_list_9F2DF0->ctor_4043E0(90);

    gObjList_drawables_504618 = ao_new<DynamicArrayT<BaseGameObject>>();
    gObjList_drawables_504618->ctor_4043E0(80);

    gObjList_animations_505564 = ao_new<DynamicArrayT<AnimationBase>>();
    gObjList_animations_505564->ctor_4043E0(80);

    // NOTE: We need to call Input_Init() before Init_Sound_DynamicArrays_And_Others() because of gLatencyHack
    // which can be configured from the ini
    Input_Init_44EB60();

    Init_Sound_DynamicArrays_And_Others_41CD20();

    Path_Set_NewData_FromLvls();

#if DEVELOPER_MODE
    // Boot directly to the "abe hello" screen
    gMap_507BA8.Init_443EE0(LevelIds::eMenu_0, 1, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
#else
    // Normal copy right screen boot
    gMap_507BA8.Init_443EE0(LevelIds::eMenu_0, 1, 10, CameraSwapEffects::eInstantChange_0, 0, 0);
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

    DDCheat::DebugStr_495990("Abe's Oddysee Demo Done\n");
}


EXPORT void Game_Main_450050()
{
    BaseAliveGameObject_ForceLink();

    GetGameAutoPlayer().ParseCommandLine(Sys_GetCommandLine_48E920());

    Main_ParseCommandLineArguments();
    Game_SetExitCallBack_48E040(Game_ExitGame_450730);
#ifdef _WIN32
    // Only SDL2 supported in AO
    Sys_SetWindowProc_Filter_48E950(Sys_WindowMessageHandler_4503B0);
#endif
    Game_Run_4373D0();

    // TODO: AE inlined calls here (pull AE's code into another func)
    Game_Shutdown_48E050();
}

} // namespace AO
