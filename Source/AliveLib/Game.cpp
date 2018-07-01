#include "stdafx.h"
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
#include "ScreenManager.hpp"
#include "Animation.hpp"
#include "stdlib.hpp"
#include <timeapi.h>
#include "GameSpeak.hpp"
#include "PathData.hpp"
#include "DDCheat.hpp"

void Game_ForceLink() { }

using TExitGameCallBack = std::add_pointer<void CC()>::type;

ALIVE_VAR(1, 0xBBFB00, TExitGameCallBack, sGame_OnExitCallback_BBFB00, nullptr);

ALIVE_VAR(1, 0x5C1B84, unsigned int, sGnFrame_5C1B84, 0);

// Timer
ALIVE_VAR(1, 0xBBB9D4, DWORD, sTimer_period_BBB9D4, 0);

// I/O
ALIVE_VAR(1, 0xBD2A5C, BOOL, sIOSyncReads_BD2A5C, FALSE);
ALIVE_VAR(1, 0xBBC558, DWORD, sIoThreadId_BBC558, 0);
ALIVE_VAR(1, 0xBBC55C, HANDLE, sIoThreadHandle_BBC55C, nullptr);

EXPORT DWORD WINAPI FS_IOThread_4F25A0(LPVOID /*lpThreadParameter*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT bool CC Is_Cd_Rom_Drive_495470(CHAR driveLetter)
{
    CHAR RootPathName[4] = {};
    strcpy(RootPathName, "C:\\");
    RootPathName[0] = driveLetter;
    return ::GetDriveTypeA(RootPathName) == DRIVE_CDROM;
}

EXPORT void CC Main_ParseCommandLineArguments_494EA0(const char* /*pCmdLineNotUsed*/, const char* /*pCommandLine*/)
{
    NOT_IMPLEMENTED();
}

EXPORT LRESULT CC Sys_WindowMessageHandler_494A40(HWND /*hWnd*/, UINT /*msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC CreateTimer_4EDEC0(UINT /*uDelay*/, void* /*callBack*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

class FG1 : public BaseGameObject
{
public:
    // TODO
};

ALIVE_VAR(1, 0x5C1A24, DynamicArrayT<Animation>*, gObjList_animations_5C1A24, nullptr);
ALIVE_VAR(1, 0x5C1124, DynamicArrayT<BaseGameObject>*, gObjList_drawables_5C1124, nullptr);
ALIVE_VAR(1, 0x5D1E28, DynamicArrayT<FG1>*, gFG1List_5D1E28, nullptr);

EXPORT void CC Init_Sound_DynamicArrays_And_Others_43BDB0()
{
    NOT_IMPLEMENTED();
}

EXPORT void __stdcall sub_45F000(int )
{
    NOT_IMPLEMENTED();
}

EXPORT void CC sub_494580()
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x5C2F6C, DWORD, dword_5C2F6C, 0);
ALIVE_VAR(1, 0x5C1BA0, WORD, word_5C1BA0, 0);
ALIVE_VAR(1, 0x5C2F70, DWORD, dword_5C2F70, 0);

class LvlArchive
{
public:
    EXPORT __int16 sub_432E80(const char* fileName);
    EXPORT __int16 sub_433130();
private:
    DWORD field_0_0x2800_res;
    DWORD field_4[41];
};
ALIVE_ASSERT_SIZEOF(LvlArchive, 0xA8);

__int16 LvlArchive::sub_432E80(const char* fileName)
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 LvlArchive::sub_433130()
{
    NOT_IMPLEMENTED();
    return 0;
}

ALIVE_VAR(1, 0x5BC520, LvlArchive, sLvlArchive_5BC520, {});


class Map
{
public:
  
    unsigned __int16 sCurrentLevelId_5C3030;
    unsigned __int16 sCurrentPathId_5C3032;
    unsigned __int16 sCurrentCamId_5C3034;

    __int16 field_6;
    __int16 field_8;

    unsigned __int16 field_A_5C303A_levelId;
    __int16 field_C_5C303C_pathId;
    __int16 field_E_cameraId;

    __int16 field_10[7];

    __int16 field_1E_5C304E;
    __int16 field_20_5C3050;
    __int16 field_22_5C3052;

    int field_24_5C3054;
    int field_28_5C3058;
    int field_2C_5C305C;

    EXPORT void sub_480B80();
    EXPORT int sub_4803F0(__int16 a2, __int16 a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7);
    EXPORT void sub_4804E0();
};
ALIVE_ASSERT_SIZEOF(Map, 0x30);

void Map::sub_480B80()
{
    NOT_IMPLEMENTED();
}

int Map::sub_4803F0(__int16 a2, __int16 a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7)
{
    NOT_IMPLEMENTED();
}

void Map::sub_4804E0()
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x5C3030, Map, gMap_5C3030, {});

EXPORT void CC DDCheat_Allocate_415320()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Game_Loop_467230();

class CheatController : public BaseGameObject
{
public:
    virtual void VDestructor(signed int flags) override;
    EXPORT void dtor_421C10(signed int flags);

    CheatController();
    EXPORT void ctor_421BD0();
 
    __int16 field_20;
    __int16 field_22;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
};
ALIVE_ASSERT_SIZEOF(CheatController, 0x40);

ALIVE_VAR(1, 0x5BC120, CheatController*, pCheatController_5BC120, nullptr);

void CheatController::VDestructor(signed int flags)
{
    dtor_421C10(flags);
}

void CheatController::dtor_421C10(signed int flags)
{
    NOT_IMPLEMENTED();
}


CheatController::CheatController()
{
    ctor_421BD0();
}

void CheatController::ctor_421BD0()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Game_Init_LoadingIcon_482CD0()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Game_Free_LoadingIcon_482D40()
{
    NOT_IMPLEMENTED();
}

class MusicController
{
public:
    static EXPORT void CC Shutdown_47FD20();
};

ALIVE_VAR(1, 0x5C3020, MusicController*, pMusicController_5C3020, nullptr);

void CC MusicController::Shutdown_47FD20()
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x5C1B78, DynamicArray*, ObjList_5C1B78, nullptr);
ALIVE_VAR(1, 0x5BD4D8, DynamicArray*, ObjList_5BD4D8, nullptr);
ALIVE_VAR(1, 0x5C1B7C, DynamicArray*, gBaseAliveGameObjects_5C1B7C, nullptr);
ALIVE_VAR(1, 0x5C1B80, DynamicArray*, sShadow_dArray_5C1B80, nullptr);

class Collisions
{
public:
    EXPORT void dtor_4189F0();
private:
    void* field_0_pArray;
    int field_4;
    int field_8;
    int field_C;
};
ALIVE_ASSERT_SIZEOF(Collisions, 0x10);

void Collisions::dtor_4189F0()
{
    Mem_Free_495560(field_0_pArray);
}

ALIVE_VAR(1, 0x5C1128, Collisions*, sCollisions_DArray_5C1128, nullptr);



EXPORT void CC Game_Run_466D40()
{
    // Begin start up
    sub_494580();
    dword_5C2F6C = 6000;
    word_5C1BA0 = 0;
    dword_5C2F70 = 34;
    sub_494580();

    PSX_ResetCallBack_4FAA20();
    gPsxDisplay_5C1130.ctor_41DC30();
    PSX_CdInit_4FB2C0();
    PSX_CdSetDebug_4FB330(0);
    sub_45F000(1); // starts card/pads on psx ver

    gBaseGameObject_list_BB47C4 = alive_new<DynamicArrayT<BaseGameObject>>(50);
    gObjList_drawables_5C1124 = alive_new<DynamicArrayT<BaseGameObject>>(30);
    gFG1List_5D1E28 = alive_new<DynamicArrayT<FG1>>(4);
    gObjList_animations_5C1A24 = alive_new<DynamicArrayT<Animation>>(30);
    pResourceManager_5C1BB0 = alive_new<ResourceManager>();

    Init_Sound_DynamicArrays_And_Others_43BDB0();

    Camera camera; // ctor_480DD0

    sLvlArchive_5BC520.sub_432E80(CdLvlName(0));
    ResourceManager::LoadResourceFile_49C170("STP01C25.CAM", &camera);

    camera.field_C_pCamRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Bits, 125, 1u, 0);
    gMap_5C3030.field_28_5C3058 = 0;
    gMap_5C3030.field_24_5C3054 = 0;

    pScreenManager_5BB5F4 = alive_new<ScreenManager>((int)camera.field_C_pCamRes, (int)&gMap_5C3030.field_24_5C3054); // ctor_40E3E0
    pScreenManager_5BB5F4->sub_cam_vlc_40EF60((unsigned __int16 **)camera.field_C_pCamRes);
    pScreenManager_5BB5F4->MoveImage_40EB70();

    sLvlArchive_5BC520.sub_433130();

    camera.dtor_480E00();

    Input_Init_491BC0();
    gMap_5C3030.sub_4803F0(0, 1, 25, 0, 0, 0);

    DDCheat_Allocate_415320();
    pEventSystem_5BC11C = alive_new<GameSpeak>(); // ctor_421820

    pCheatController_5BC120 = alive_new<CheatController>(); // ctor_421BD0
    
    Game_Init_LoadingIcon_482CD0();
    
    // Main loop start
    Game_Loop_467230();

    // Shut down start
    Game_Free_LoadingIcon_482D40();
    DDCheat::sub_415390();
    gMap_5C3030.sub_4804E0();

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

    if (sShadow_dArray_5C1B80)
    {
        sShadow_dArray_5C1B80->dtor_40CAD0();
        Mem_Free_495540(sShadow_dArray_5C1B80);
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

    SND_Clear_4CB4B0();
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
    SND_Close_4EFD50(); // TODO: This appears to terminate the game, re-impl function to figure out why
    IO_Stop_ASync_IO_Thread_4F26B0();
    VGA_Shutdown_4F3170();
}

EXPORT signed int TMR_Init_4EDE20()
{
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
    Path_Get_Bly_Record_460F30(0, 0);

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



ALIVE_VAR(1, 0x5C2FE0, short, sBreakGameLoop_5C2FE0, 0);
ALIVE_VAR(1, 0x5C1B66, short, word_5C1B66, 0);
ALIVE_VAR(1, 0x5C2F78, int, dword_5C2F78, 0);
ALIVE_VAR(1, 0x5C2FA0, short, word_5C2FA0, 0);
ALIVE_VAR(1, 0x5C9300, BaseGameObject*, pPauseMenu_5C9300, nullptr);
ALIVE_VAR(1, 0x5CA4D2, BYTE, byte_5CA4D2, 0);

EXPORT void CC sub_422DA0()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC sub_449A90()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Game_Loop_467230()
{
    dword_5C2F78 = 0;
    sBreakGameLoop_5C2FE0 = 0;
    bool bPauseMenuObjectFound = false;
    while (!gBaseGameObject_list_BB47C4->IsEmpty())
    {
        sub_422DA0();
        sub_449A90();
        word_5C2FA0 = 0;

        // Update objects
        for (int baseObjIdx = 0; baseObjIdx < gBaseGameObject_list_BB47C4->Size(); baseObjIdx++)
        {
            BaseGameObject* pBaseGameObject = gBaseGameObject_list_BB47C4->ItemAt(baseObjIdx);

            if (!pBaseGameObject || word_5C2FA0)
            {
                break;
            }

            // bit 00 = 0x001 = BaseGameObject couldn't add to list / it unsets inverse of 0xF80A / 0b1111100000001010
            // bit 01 = 0x002 = do update ?
            // bit 02 = 0x004 = dead
            // bit 03 = 0x008 = render
            // bit 04 = 0x010 = set by BaseAnimatedWithPhysicsGameObject
            // bit 05 = 0x020 = set by BaseAliveGameObject
            // bit 06 = 0x040 = set by ChantSuppressor::ctor_466350 / MovingBomb::ctor_46FD40 - explodable?
            // bit 07 = 0x080 = set by Uxb::ctor_4DE9A0 = pressable ?
         
            // bit 08 = 0x100 = ?
            // bit 09 = 0x200 = still update in some circumstance ? when word_5C1B66 is 0
            // bit 10 = 0x400 = can never be removed
            // bit 11 = 0x800 = ?
            // bit 12 = 0x1000 = ?

            if (pBaseGameObject->field_6_flags & 2 && !(pBaseGameObject->field_6_flags & 4) && (!word_5C1B66 || pBaseGameObject->field_6_flags & 0x200))
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
        if (word_5C1B66 <= 0)
        {
            Animation::AnimateAll_40AC20(gObjList_animations_5C1A24);
        }

        /*
        static int hack = 0;
        hack++;
        if (hack == 100)
        {
            static BackgroundAnimation_Params params = {};
            params.field_10_res_id = 8001;
            params.field_8_xpos = 90;
            params.field_A_ypos = 90;
            params.field_C_width = 50;
            params.field_E_height = 50;
            params.field_1A_layer = 1;
            ResourceManager::LoadResourceFile_49C170("STDOOR.BAN", 0);
            Factory_BackgroundAnimation_4D84F0(&params, 4, 4, 4);
        }
        */

        int** pOtBuffer = gPsxDisplay_5C1130.field_10_drawEnv[gPsxDisplay_5C1130.field_C_buffer_index].field_70_ot_buffer;
        
        // Render objects
        for (int i=0; i < gObjList_drawables_5C1124->Size(); i++)
        {
            BaseGameObject* pObj = gObjList_drawables_5C1124->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->field_6_flags & 4)
            {
                pObj->field_6_flags = pObj->field_6_flags & ~0x400;
            }
            else if (pObj->field_6_flags & 8)
            {
                pObj->field_6_flags |= 0x400;
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

            if (pFG1->field_6_flags & 4)
            {
                pFG1->field_6_flags = pFG1->field_6_flags & ~0x400;
            }
            else if (pFG1->field_6_flags & 8)
            {
                pFG1->field_6_flags |= 0x400;
                pFG1->VRender(pOtBuffer);
            }
        }
        
        DebugFont_Flush_4DD050();
        PSX_DrawSync_4F6280(0);
        pScreenManager_5BB5F4->VRender(pOtBuffer);
        sub_494580(); // Exit checking?
        
        gPsxDisplay_5C1130.PSX_Display_Render_OT_41DDF0();
        
        // Destroy objects with certain flags
        for (short idx = 0; idx < gBaseGameObject_list_BB47C4->Size(); idx++)
        {
            BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(idx);
            if (!pObj)
            {
                break;
            }

            const int flags = pObj->field_6_flags;
            if (flags & 4 && !(flags & 0x400))
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

        if (!word_5C1B66)
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
            pResourceManager_5C1BB0->sub_465590(0);
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
