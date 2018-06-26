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

EXPORT void CC PSX_EMU_VideoDeAlloc_4FA010()
{
    NOT_IMPLEMENTED();
}

EXPORT int CC CreateTimer_4EDEC0(UINT /*uDelay*/, void* /*callBack*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

/*
template<typename T, typename... Args>
inline T* alive_new(Args&&... args)
{
    void* buffer = malloc_4954D0(sizeof(T));
    if (buffer)
    {
        return new (buffer) T(std::forward<Args>(args)...);
    }
    return nullptr;
}
*/

EXPORT void CC Game_Run_466D40()
{
    auto v0 = (DynamicArray *)malloc_4954D0(0xCu);
    v0->ctor_40C9E0(30);

    //alive_new<DynamicArray>();

    NOT_IMPLEMENTED();
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

class FG1 : public BaseGameObject
{
public:
    // TODO
};


ALIVE_VAR(1, 0xBB47C4, DynamicArrayT<BaseGameObject>*, gBaseGameObject_list_BB47C4, nullptr);
ALIVE_VAR(1, 0x5C1A24, DynamicArrayT<Animation>*, gObjList_animations_5C1A24, nullptr);
ALIVE_VAR(1, 0x5C1124, DynamicArrayT<BaseGameObject>*, gObjList_drawables_5C1124, nullptr);
ALIVE_VAR(1, 0x5D1E28, DynamicArrayT<FG1>*, gFG1List_5D1E28, nullptr);


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
};
ALIVE_ASSERT_SIZEOF(Map, 0x30);

void Map::sub_480B80()
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x5C3030, Map, gMap_5C3030, {});

EXPORT void CC sub_494580()
{
    NOT_IMPLEMENTED();
}

EXPORT __int16 __cdecl Factory_BackgroundAnimation_4D84F0(void *pParams, int a2, int a3, __int16 mode)
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

        int* pOtBuffer = gPsxDisplay_5C1130.field_10_drawEnv[gPsxDisplay_5C1130.field_C_buffer_index].field_70_ot_buffer;
        
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
