#include "stdafx.h"
#include "Game.hpp"
#include "Sys.hpp"
#include "VGA.hpp"
#include "bmp.hpp"
#include "Input.hpp"
#include "Error.hpp"
#include "Psx.hpp"
#include "DynamicArray.hpp"
#include <timeapi.h>

#define GAME_IMPL true

using TExitGameCallBack = std::add_pointer<void CC()>::type;

ALIVE_VAR(1, 0xBBFB00, TExitGameCallBack, sGame_OnExitCallback_BBFB00, nullptr);

// Timer
ALIVE_VAR(1, 0xBBB9D4, DWORD, sTimer_period_BBB9D4, 0);

// I/O
ALIVE_VAR(1, 0xBD2A5C, BOOL, sIOSyncReads_BD2A5C, FALSE);
ALIVE_VAR(1, 0xBBC558, DWORD, sIoThreadId_BBC558, 0);
ALIVE_VAR(1, 0xBBC55C, HANDLE, sIoThreadHandle_BBC55C, nullptr);

ALIVE_FUNC_NOT_IMPL(0x4F25A0, DWORD WINAPI(LPVOID lpThreadParameter), FS_IOThread_4F25A0);
ALIVE_FUNC_NOT_IMPL(0x494EA0, void CC(const char *pCmdLineNotUsed, const char *pCommandLine), Main_ParseCommandLineArguments_494EA0);
ALIVE_FUNC_NOT_IMPL(0x494A40, LRESULT CC(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam), Sys_WindowMessageHandler_494A40);
ALIVE_FUNC_NOT_IMPL(0x466D40, void CC(), Game_Run_466D40);
ALIVE_FUNC_NOT_IMPL(0x4FA010, void CC(), PSX_EMU_VideoDeAlloc_4FA010);
ALIVE_FUNC_NOT_IMPL(0x4EDEC0, int __cdecl (UINT uDelay, void* callBack), CreateTimer_4EDEC0);
ALIVE_FUNC_NOT_IMPL(0x4EFD50, void (), SND_Close_4EFD50);


void Game_ForceLink() { }

void CC Game_SetExitCallBack_4F2BA0(TExitGameCallBack callBack)
{
    sGame_OnExitCallback_BBFB00 = callBack;
}
ALIVE_FUNC_IMPLEX(0x4F2BA0, Game_SetExitCallBack_4F2BA0, GAME_IMPL);

void CC Game_ExitGame_4954B0()
{
    PSX_EMU_VideoDeAlloc_4FA010();
}
ALIVE_FUNC_IMPLEX(0x4954B0, Game_ExitGame_4954B0, GAME_IMPL);

void CC IO_Stop_ASync_IO_Thread_4F26B0()
{
    if (sIoThreadHandle_BBC55C)
    {
        ::CloseHandle(sIoThreadHandle_BBC55C);
        sIoThreadHandle_BBC55C = nullptr;
    }
}
ALIVE_FUNC_IMPLEX(0x4F26B0, IO_Stop_ASync_IO_Thread_4F26B0, GAME_IMPL);

void CC Game_Shutdown_4F2C30()
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
ALIVE_FUNC_IMPLEX(0x4F2C30, Game_Shutdown_4F2C30, GAME_IMPL);

signed int TMR_Init_4EDE20()
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
ALIVE_FUNC_IMPLEX(0x4EDE20, TMR_Init_4EDE20, GAME_IMPL);

signed int CC Init_Input_Timer_And_IO_4F2BF0(bool forceSystemMemorySurfaces)
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
            FS_IOThread_4F25A0.Ptr(),
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
ALIVE_FUNC_IMPLEX(0x4F2BF0, Init_Input_Timer_And_IO_4F2BF0, GAME_IMPL);

void CC Game_Main_4949F0()
{
    // Inits
    Init_Input_Timer_And_IO_4F2BF0(false);
    
    Main_ParseCommandLineArguments_494EA0(Sys_GetCommandLine_4EE176(), Sys_GetCommandLine_4EE176());
    Game_SetExitCallBack_4F2BA0(Game_ExitGame_4954B0);
    Sys_SetWindowProc_Filter_4EE197(Sys_WindowMessageHandler_494A40.Ptr());
    
    // Only returns once the engine is shutting down
    Game_Run_466D40();

    if (sGame_OnExitCallback_BBFB00)
    {
        sGame_OnExitCallback_BBFB00();
        sGame_OnExitCallback_BBFB00 = nullptr;
    }

    Game_Shutdown_4F2C30();
}
ALIVE_FUNC_IMPLEX(0x4949F0, Game_Main_4949F0, GAME_IMPL);

class BaseGameObject;

struct BaseGameObject_VTable
{
    void(__thiscall *field_0)(BaseGameObject *, signed int);
    void(__thiscall *field_4)(BaseGameObject *);
    void(__thiscall *field_8_render)(BaseGameObject *, int *);
    void *field_C;
    void *field_10;
    void *field_14_get_save_state;
};

union BaseGameObject_VTable_Union
{
    BaseGameObject_VTable VBaseGameObject;
};

class BaseGameObject
{
public:
    BaseGameObject_VTable_Union *field_0_VTbl;
    __int16 field_4_typeId;
    __int16 field_6_flags;
    int field_8_flags_ex;
    int field_C_objectId;
    DynamicArray field_10_resources_array;
    int field_1C_update_delay;
};

struct PSX_DISPENV
{
    PSX_RECT disp;
    PSX_RECT screen;
    char isinter;
    char isrgb24;
    char pad0;
    char pad1;
};

struct PSX_DR_ENV
{
    int field_0_tag;
    int field_4_code[15];
};

struct PSX_DRAWENV
{
    PSX_RECT field_0_clip;
    __int16 field_8_ofs[2];
    PSX_RECT field_C_tw;
    unsigned __int16 field_14_tpage;
    char field_16_dtd;
    char field_17_dfe;
    char field_18_isbg;
    char field_19_r0;
    char field_1A_g0;
    char field_1B_b0;
    PSX_DR_ENV field_1C_dr_env;
};


class PSX_Display_Buffer
{
public:
    PSX_DRAWENV field_0_draw_env;
    PSX_DISPENV field_5C_disp_env;
    int field_70_ot_buffer[256];
};

class PsxDisplay
{
public:
    unsigned __int16 field_0_width;
    __int16 field_2_height;
    __int16 field_4;
    __int16 field_6_bpp;
    __int16 field_8_max_buffers;
    unsigned __int16 field_A_buffer_size;
    unsigned __int16 field_C_buffer_index;
    __int16 field_E;
    PSX_Display_Buffer field_10_drawEnv[2];

    void PSX_Display_Render_OT_41DDF0();
};

void PsxDisplay::PSX_Display_Render_OT_41DDF0()
{
    // TODO
}

struct FG1
{
    BaseGameObject field_0_mBase;
    // TODO
};

ALIVE_VAR(1, 0x5C1130, PsxDisplay, gPsxDisplay_5C1130, {});

ALIVE_VAR(1, 0xBB47C4, DynamicArray*, gBaseGameObject_list_BB47C4, nullptr);
ALIVE_VAR(1, 0x5C1A24, DynamicArray*, gObjList_animations_5C1A24, nullptr);
ALIVE_VAR(1, 0x5C1124, DynamicArray*, gObjList_drawables_5C1124, nullptr);
ALIVE_VAR(1, 0x5D1E28, DynamicArray*, gFG1List_5D1E28, nullptr);



ALIVE_VAR(1, 0x5C2FE0, short, sBreakGameLoop_5C2FE0, 0);
ALIVE_VAR(1, 0x5C1B66, short, word_5C1B66, 0);
ALIVE_VAR(1, 0x5C2F78, int, dword_5C2F78, 0);
ALIVE_VAR(1, 0x5C2FA0, short, word_5C2FA0, 0);
ALIVE_VAR(1, 0x5C9300, BaseGameObject*, pPauseMenu_5C9300, nullptr);


ALIVE_FUNC_NOT_IMPL(0x422DA0, void(), sub_422DA0);
ALIVE_FUNC_NOT_IMPL(0x449A90, void(), sub_449A90);
ALIVE_FUNC_NOT_IMPL(0x40AC20, void __cdecl (DynamicArray *pAnimations), AnimateAllAnimations_40AC20);



void CC Game_Loop_467230()
{
    dword_5C2F78 = 0;
    sBreakGameLoop_5C2FE0 = 0;
    bool isPauseMenuInObjList_3 = false;
    while (gBaseGameObject_list_BB47C4->field_4_used_size > 0)
    {
        sub_422DA0();
        sub_449A90();
        word_5C2FA0 = 0;

        for (int baseObjIdx = 0; baseObjIdx < gBaseGameObject_list_BB47C4->field_4_used_size; baseObjIdx++)
        {
            BaseGameObject* pBaseGameObject = (BaseGameObject *)gBaseGameObject_list_BB47C4->field_0_array[baseObjIdx];

            if (!pBaseGameObject || word_5C2FA0)
            {
                break;
            }

            if (pBaseGameObject->field_6_flags & 2 && !(pBaseGameObject->field_6_flags & 4) && (!word_5C1B66 || pBaseGameObject->field_6_flags & 0x200))
            {
                if (pBaseGameObject->field_1C_update_delay <= 0)
                {
                    if (pBaseGameObject == pPauseMenu_5C9300)
                    {
                        isPauseMenuInObjList_3 = true;
                    }
                    else
                    {
                        pBaseGameObject->field_0_VTbl->VBaseGameObject.field_4(pBaseGameObject);
                    }
                }
                else
                {
                    pBaseGameObject->field_1C_update_delay--;
                }
            }
        }

        if (word_5C1B66 <= 0)
        {
            AnimateAllAnimations_40AC20(gObjList_animations_5C1A24);
        }

        int* pOtBuffer = gPsxDisplay_5C1130.field_10_drawEnv[gPsxDisplay_5C1130.field_C_buffer_index].field_70_ot_buffer;
        for (int i=0; i < gObjList_drawables_5C1124->field_4_used_size; i++)
        {
            BaseGameObject* v10 = (BaseGameObject *)gObjList_drawables_5C1124->field_0_array[i];
            if (!v10)
            {
                break;
            }

            if (v10->field_6_flags & 4)
            {
                v10->field_6_flags = v10->field_6_flags & 0xFBFF;
            }
            else if (v10->field_6_flags & 8)
            {
                // TODO
                //HIBYTE(v10->field_6_flags) |= 4u;
                v10->field_0_VTbl->VBaseGameObject.field_8_render(v10, pOtBuffer);
            }
        }

        int* pOtBuffer2 = gPsxDisplay_5C1130.field_10_drawEnv[gPsxDisplay_5C1130.field_C_buffer_index].field_70_ot_buffer;
        for (int i=0; i < gFG1List_5D1E28->field_4_used_size; i++)
        {
            FG1* pFG1 = (FG1 *)gFG1List_5D1E28->field_0_array[i];
            if (!pFG1)
            {
                break;
            }

            if (pFG1->field_0_mBase.field_6_flags & 4)
            {
                pFG1->field_0_mBase.field_6_flags = pFG1->field_0_mBase.field_6_flags & 0xFBFF;
            }
            else if (pFG1->field_0_mBase.field_6_flags & 8)
            {
                // TODO
                //HIBYTE(pFG1->field_0_mBase.field_6_flags) |= 4u;
                pFG1->field_0_mBase.field_0_VTbl->VBaseGameObject.field_8_render(&pFG1->field_0_mBase, pOtBuffer2);
            }
        }
        
        /*
        Font::sub_4DD050();
        PSX_DrawSync_4F6280(0);

        pScreenManager_5BB5F4->field_0_mBase.field_0_VTbl->VBaseGameObject.field_8_render(
            &pScreenManager_5BB5F4->field_0_mBase,
            gPsxDisplay_5C1130.field_10_drawEnv[gPsxDisplay_5C1130.field_C_buffer_index].field_70_ot_buffer);

        sub_494580();
        gPsxDisplay_5C1130.PSX_Display_Render_OT_41DDF0();
        */

        /*
        v21 = gBaseGameObject_list_BB47C4;
        v22 = 0;
        *(DWORD *)&pRect.x = gBaseGameObject_list_BB47C4;
        pRect.w = 0;
        while (v22 < v21->field_4_used_size)
        {
            v23 = v22++;
            v20 = (BaseGameObject *)v21->field_0_array[v23];
            pRect.w = v22;
            if (!v20)
                break;
            v24 = v20->field_6_flags;
            if (v24 & 4 && !(v24 & 0x400))
            {
                DynamicArrayIter::Remove_At_Iter_40CCA0((DynamicArrayIter *)&pRect);
                v20->field_0_VTbl->VBaseGameObject.field_0(v20, 1);
                v22 = pRect.w;
                v21 = *(DynamicArray **)&pRect.x;
            }
        }

        if (isPauseMenuInObjList_3 && pPauseMenu_5C9300)
        {
            pPauseMenu_5C9300->field_0_VTbl->VBaseGameObject.field_4(pPauseMenu_5C9300);
        }

        isPauseMenuInObjList_3 = 0;

        Map::sub_480B80(&gMap_5C3030, v20);
        Input_update_45F040(&sInputObject_5BD4E0);

        if (!word_5C1B66)
        {
            ++sGnFrame_5C1B84;
        }

        if (sBreakGameLoop_5C2FE0)
        {
            break;
        }
        
        if (byte_5CA4D2)
        {
            ResourceManager::sub_465590(pResourceManager_5C1BB0, v25, 0);
        }
        */
    }

    /*
    pRect.x = 0;
    pRect.y = 0;
    pRect.w = 640;
    pRect.h = 240;
    PSX_ClearImage_4F5BD0(&pRect, 0, 0, 0);
    PSX_DrawSync_4F6280(0);
    PSX_VSync_4F6170(0);
    ResourceManager::Shutdown_465610(pResourceManager_5C1BB0);
    for (i = gBaseGameObject_list_BB47C4; gBaseGameObject_list_BB47C4->field_4_used_size > 0; pRect.w = 0)
    {
        v27 = 0;
        *(DWORD *)&pRect.x = i;
        pRect.w = 0;
        while (v27 < i->field_4_used_size)
        {
            v28 = (BaseGameObject *)i->field_0_array[v27];
            pRect.w = v27 + 1;
            if (!v28)
            {
                break;
            }
            DynamicArrayIter::Remove_At_Iter_40CCA0((DynamicArrayIter *)&pRect);
            v28->field_0_VTbl->VBaseGameObject.field_0(v28, 1);
            v27 = pRect.w;
            i = *(DynamicArray **)&pRect.x;
        }
        i = gBaseGameObject_list_BB47C4;
    }
    */
}
ALIVE_FUNC_IMPLEX(0x467230, Game_Loop_467230, false); // TODO: Complete impl
