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
#include "Map.hpp"
#include "ScreenManager.hpp"
#include "Animation.hpp"
#include "stdlib.hpp"
#include <timeapi.h>
#include "GameSpeak.hpp"
#include "PathData.hpp"
#include "DDCheat.hpp"
#include "Quicksave.hpp"

#include <fstream>

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


EXPORT int CC Game_End_Frame_4950F0(float fps)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC sub_496720()
{
    NOT_IMPLEMENTED();
}

EXPORT void IO_Init_494230()
{
    NOT_IMPLEMENTED();
}

ALIVE_ARY(1, 0x5CA488, char, 30, sCdRomDrives_5CA488, {});
ALIVE_VAR(1, 0x5CA4D4, int, dword_5CA4D4, 0);
ALIVE_VAR(1, 0x55EF90, int, dword_55EF90, 1);
ALIVE_VAR(1, 0x55EF88, bool, byte_55EF88, true);
ALIVE_VAR(1, 0x5CA4D0, bool, sCommandLine_ShowFps_5CA4D0, false);
ALIVE_VAR(1, 0x5CA4B5, bool, sCommandLine_DDCheatEnabled_5CA4B5, false);
ALIVE_VAR(1, 0x5CA4D2, bool, byte_5CA4D2, false);
ALIVE_VAR(1, 0x5CA4E0, int, dword_5CA4E0, 0);

EXPORT void CC sub_4ED960(int a1)
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
    Sys_WindowClass_Register_4EE22F("ABE_WINCLASS", "Oddworld Abe's Exoddus", 32, 64, 640, 480);
    Sys_Set_Hwnd_4F2C50(Sys_GetWindowHandle_4EE180());

    dword_5CA4D4 = 0;
    dword_55EF90 = 1;
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
    }

    if (dword_5CA4E0 == 1)
    {
        sub_4ED960(1);
        PSX_EMU_Set_screen_mode_4F9420(1);
    }
    else if (dword_5CA4E0 == 2)
    {
        sub_4ED960(0);
        PSX_EMU_Set_screen_mode_4F9420(0);
    }
    else
    {
        sub_4ED960(2);
        PSX_EMU_Set_screen_mode_4F9420(2);
    }

    Init_VGA_AndPsxVram_494690();
    PSX_EMU_Init_4F9CD0(false);
    PSX_EMU_VideoAlloc_4F9D70();
    PSX_EMU_SetCallBack_4F9430(1, Game_End_Frame_4950F0);
    //Main_Set_HWND_4F9410(Sys_GetWindowHandle_4EE180()); // Note: Set but never read
    sub_496720();
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

    // LOAD DEBUG SAVE //
    // If debug.sav exists, load it before game start.
    // Makes debugging in game stuff a lot faster.
    std::ifstream debugSave("debug.sav");
    if (!debugSave.fail())
    {
        debugSave.read((char*)&sActiveQuicksaveData_BAF7F8, sizeof(sActiveQuicksaveData_BAF7F8));
        Quicksave_LoadActive_4C9170();
        debugSave.close();
    }
    /////////////////////////
    
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

EXPORT void CC sub_422DA0()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC sub_449A90()
{
    NOT_IMPLEMENTED();
}

struct PrimHeader
{
    int field_0_tag;
    char field_4_num_longs;
    char field_5_unknown;
    __int16 field_6_pad0;
    BYTE field_8_r0;
    BYTE field_9_g0;
    BYTE field_A_b0;
    BYTE field_B_code;
};
ALIVE_ASSERT_SIZEOF(PrimHeader, 0xC);

struct Poly_G3
{
    PrimHeader field_0_header;

    __int16 field_C_x0;
    __int16 field_E_y0;

    BYTE field_10_r1;
    BYTE field_11_g1;
    BYTE field_12_b1;
    char field_13_pad2;
    __int16 field_14_x1;
    __int16 field_16_y1;

    BYTE field_18_r2;
    BYTE field_19_g2;
    BYTE field_1A_b2;
    char field_1B_pad3;
    __int16 field_1C_x2;
    __int16 field_1E_y2;
};
ALIVE_ASSERT_SIZEOF(Poly_G3, 0x20);

struct Poly_F4
{
    PrimHeader field_0_header;
    __int16 field_C_x0;
    __int16 field_E_y0;
    __int16 field_10_x1;
    __int16 field_12_y1;
    __int16 field_14_x2;
    __int16 field_16_y2;
    __int16 field_18_x3;
    __int16 field_1A_y3;
};
ALIVE_ASSERT_SIZEOF(Poly_F4, 0x1C);

struct Poly_G4
{
    PrimHeader field_0_header;

    __int16 field_C_x0;
    __int16 field_E_y0;

    BYTE field_10_r1;
    BYTE field_11_g1;
    BYTE field_12_b1;
    char field_13_pad2;
    __int16 field_14_x1;
    __int16 field_16_y1;

    BYTE field_18_r2;
    BYTE field_19_g2;
    BYTE field_1A_b2;
    char field_1B_pad3;
    __int16 field_1C_x2;
    __int16 field_1E_y2;

    BYTE field_20_r3;
    BYTE field_21_g3;
    BYTE field_22_b3;
    char field_23_pad4;
    __int16 field_24_x3;
    __int16 field_26_y3;
};
ALIVE_ASSERT_SIZEOF(Poly_G4, 0x28);


ALIVE_VAR(1, 0xBD146C, BYTE, byte_BD146C, 0);

EXPORT void CC PolyG3_Init_4F8890(Poly_G3* pPoly)
{
    pPoly->field_0_header.field_4_num_longs = 6;
    pPoly->field_0_header.field_5_unknown = byte_BD146C;
    pPoly->field_0_header.field_B_code = 0x30;
}

EXPORT void CC PolyG4_Init_4F88B0(Poly_G4* pPoly)
{
    pPoly->field_0_header.field_4_num_longs = 8;
    pPoly->field_0_header.field_5_unknown = byte_BD146C;
    pPoly->field_0_header.field_B_code = 0x38;
}

EXPORT void CC PolyF4_Init_4F8830(Poly_F4* pPoly)
{
    pPoly->field_0_header.field_4_num_longs = 5;
    pPoly->field_0_header.field_5_unknown = byte_BD146C;
    pPoly->field_0_header.field_B_code = 0x28;
}

EXPORT void CC Poly_Set_unknown_4F8A20(PrimHeader* pPrim, int bFlag1)
{
    pPrim->field_5_unknown = byte_BD146C;
    if (bFlag1)
    {
        pPrim->field_B_code = pPrim->field_B_code | 1;
    }
    else
    {
        pPrim->field_B_code = pPrim->field_B_code & ~1;
    }
}

EXPORT void CC Poly_Set_SemiTrans_4F8A60(PrimHeader* pPrim, int bSemiTrans)
{
    pPrim->field_5_unknown = byte_BD146C;
    if (bSemiTrans)
    {
        pPrim->field_B_code = pPrim->field_B_code | 2;
    }
    else
    {
        pPrim->field_B_code = pPrim->field_B_code & ~2;
    }
}



EXPORT void CC OrderingTable_Add_4F8AA0(int** pOt, void* pItem)
{
    NOT_IMPLEMENTED();
}

class RenderTest : public BaseGameObject
{
public:
    RenderTest()
    {
        // Don't hack the vtable else our virtuals won't get called and we can't hack the correct one back since we don't know the address of our vtable.
        DisableVTableHack disableHack;

        ctor_4DBFA0(1, 1);

        InitTestRender();
        
        field_6_flags |= 0x8;

        gObjList_drawables_5C1124->Push_Back(this);
    }

    virtual void VDestructor(signed int flags) override
    {
        Destruct();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
    }

    virtual void VRender(int** pOrderingTable) override
    {
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyG3.field_0_header.field_0_tag); // 30 being the "layer"
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyF4.field_0_header.field_0_tag);
        OrderingTable_Add_4F8AA0(&pOrderingTable[30], &mPolyG4.field_0_header.field_0_tag);

    }

    void Destruct()
    {
        gObjList_drawables_5C1124->Remove_Item(this);
    }

private:
    void InitTestRender()
    {
        {
            PolyG3_Init_4F8890(&mPolyG3);

            mPolyG3.field_0_header.field_8_r0 = 255;
            mPolyG3.field_0_header.field_9_g0 = 255;
            mPolyG3.field_0_header.field_A_b0 = 255;
            mPolyG3.field_C_x0 = 50;
            mPolyG3.field_E_y0 = 50;

            mPolyG3.field_10_r1 = 0;
            mPolyG3.field_11_g1 = 0;
            mPolyG3.field_12_b1 = 255;
            mPolyG3.field_14_x1 = 200;
            mPolyG3.field_16_y1 = 50;

            mPolyG3.field_18_r2 = 255;
            mPolyG3.field_19_g2 = 0;
            mPolyG3.field_1A_b2 = 255;
            mPolyG3.field_1C_x2 = 150;
            mPolyG3.field_1E_y2 = 100;
        }

        {
            PolyF4_Init_4F8830(&mPolyF4);
            mPolyF4.field_0_header.field_8_r0 = 255;
            mPolyF4.field_0_header.field_9_g0 = 255;
            mPolyF4.field_0_header.field_A_b0 = 255;

            struct XY { short x; short y; };
            XY points[4] =
            {
                { 150, 150 },
                { 150, 200 },
                { 200, 150 },
                { 200, 200 },
            };

            mPolyF4.field_C_x0 = points[0].x;
            mPolyF4.field_E_y0 = points[0].y;

            mPolyF4.field_10_x1 = points[1].x;
            mPolyF4.field_12_y1 = points[1].y;

            mPolyF4.field_14_x2 = points[2].x;
            mPolyF4.field_16_y2 = points[2].y;

            mPolyF4.field_18_x3 = points[3].x;
            mPolyF4.field_1A_y3 = points[3].y;
        }

        {
            PolyG4_Init_4F88B0(&mPolyG4);
            struct XY { short x; short y; };
            XY points[4] =
            {
                { 150 + 100, 150 + 10 },
                { 150 + 100, 200 + 10 },
                { 200 + 100, 150 + 10 },
                { 200 + 100, 200 + 10 },
            };

            mPolyG4.field_C_x0 = points[0].x;
            mPolyG4.field_E_y0 = points[0].y;

            mPolyG4.field_0_header.field_8_r0 = 255;
            mPolyG4.field_0_header.field_9_g0 = 0;
            mPolyG4.field_0_header.field_A_b0 = 0;


            mPolyG4.field_14_x1 = points[1].x;
            mPolyG4.field_16_y1 = points[1].y;

            mPolyG4.field_10_r1 = 0;
            mPolyG4.field_11_g1 = 0;
            mPolyG4.field_12_b1 = 255;
         

            mPolyG4.field_1C_x2 = points[2].x;
            mPolyG4.field_1E_y2 = points[2].y;

            mPolyG4.field_18_r2 = 0;
            mPolyG4.field_19_g2 = 255;
            mPolyG4.field_1A_b2 = 0;

            mPolyG4.field_24_x3 = points[3].x;
            mPolyG4.field_26_y3 = points[3].y;

            mPolyG4.field_20_r3 = 255;
            mPolyG4.field_21_g3 = 0;
            mPolyG4.field_22_b3 = 255;

        }
    }

    Poly_G3 mPolyG3 = {};
    Poly_G4 mPolyG4 = {};
    Poly_F4 mPolyF4 = {};
};

EXPORT void CC Game_Loop_467230()
{
    alive_new<RenderTest>(); // Will get nuked at LVL/Path change

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
