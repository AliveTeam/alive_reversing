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
#include <timeapi.h>

void Game_ForceLink() { }

using TExitGameCallBack = std::add_pointer<void CC()>::type;

ALIVE_VAR(1, 0xBBFB00, TExitGameCallBack, sGame_OnExitCallback_BBFB00, nullptr);

// Timer
ALIVE_VAR(1, 0xBBB9D4, DWORD, sTimer_period_BBB9D4, 0);

// I/O
ALIVE_VAR(1, 0xBD2A5C, BOOL, sIOSyncReads_BD2A5C, FALSE);
ALIVE_VAR(1, 0xBBC558, DWORD, sIoThreadId_BBC558, 0);
ALIVE_VAR(1, 0xBBC55C, HANDLE, sIoThreadHandle_BBC55C, nullptr);

EXPORT DWORD WINAPI FS_IOThread_4F25A0(LPVOID /*lpThreadParameter*/)
{
    NOT_IMPLEMENTED(0x4F25A0);
    return 0;
}

EXPORT void CC Main_ParseCommandLineArguments_494EA0(const char* /*pCmdLineNotUsed*/, const char* /*pCommandLine*/)
{
    NOT_IMPLEMENTED(0x494EA0);
}

EXPORT LRESULT CC Sys_WindowMessageHandler_494A40(HWND /*hWnd*/, UINT /*msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    NOT_IMPLEMENTED(0x494A40);
    return 0;
}

EXPORT void CC PSX_EMU_VideoDeAlloc_4FA010()
{
    NOT_IMPLEMENTED(0x4FA010);
}

EXPORT int CC CreateTimer_4EDEC0(UINT /*uDelay*/, void* /*callBack*/)
{
    NOT_IMPLEMENTED(0x4EDEC0);
    return 0;
}

EXPORT void CC Game_Run_466D40()
{
    NOT_IMPLEMENTED(0x466D40);
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

class BaseGameObject
{
public:
    // Order must match VTable
    virtual void VDestructor(signed int) = 0; // Not an actual dtor because the generated compiler code has the param to determine if heap allocated or not
    virtual void VUpdate();
    virtual void VRender(int* pOrderingTable);
    EXPORT virtual void vsub_4DC0A0();
    virtual void vnullsub_4DC0F0();
    virtual int GetSaveState_4DC110(BYTE* pSaveBuffer);
public:
    __int16 field_4_typeId;
    __int16 field_6_flags;
    int field_8_flags_ex;
    int field_C_objectId;
    DynamicArray field_10_resources_array;
    int field_1C_update_delay;
};
ALIVE_ASSERT_SIZEOF(BaseGameObject, 0x20);

void BaseGameObject::VUpdate()
{
    // Empty 0x4DC080
}

void BaseGameObject::VRender(int* /*pOrderingTable*/)
{
    // Empty 0x4DBF80
}

void BaseGameObject::vsub_4DC0A0()
{
    NOT_IMPLEMENTED(0x4DC0A0);
}

void BaseGameObject::vnullsub_4DC0F0()
{
    // Empty 0x4DC0F0
}

int BaseGameObject::GetSaveState_4DC110(BYTE* /*pSaveBuffer*/)
{
    return 0;
}

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

    EXPORT void PSX_Display_Render_OT_41DDF0();
};

void PsxDisplay::PSX_Display_Render_OT_41DDF0()
{
    NOT_IMPLEMENTED(0x41DDF0);
}

class FG1 : public BaseGameObject
{
public:
    // TODO
};

class ScreenManager : public BaseGameObject
{
public:
    // TODO
};

class ResourceManager : public BaseGameObject
{
public:
    // TODO

    EXPORT void Shutdown_465610();
    EXPORT void sub_465590(int a1);
};
ALIVE_VAR(1, 0x5C1BB0, ResourceManager*, pResourceManager_5C1BB0, nullptr);

void ResourceManager::Shutdown_465610()
{
    NOT_IMPLEMENTED(0x465610);
}

void ResourceManager::sub_465590(int /*a1*/)
{
    NOT_IMPLEMENTED(0x465590);
}

ALIVE_VAR(1, 0x5C1130, PsxDisplay, gPsxDisplay_5C1130, {});

ALIVE_VAR(1, 0xBB47C4, DynamicArrayT<BaseGameObject>*, gBaseGameObject_list_BB47C4, nullptr);
ALIVE_VAR(1, 0x5C1A24, DynamicArray*, gObjList_animations_5C1A24, nullptr);
ALIVE_VAR(1, 0x5C1124, DynamicArrayT<BaseGameObject>*, gObjList_drawables_5C1124, nullptr);
ALIVE_VAR(1, 0x5D1E28, DynamicArrayT<FG1>*, gFG1List_5D1E28, nullptr);

ALIVE_VAR(1, 0x5BB5F4, ScreenManager*, pScreenManager_5BB5F4, nullptr);


ALIVE_VAR(1, 0x5C2FE0, short, sBreakGameLoop_5C2FE0, 0);
ALIVE_VAR(1, 0x5C1B66, short, word_5C1B66, 0);
ALIVE_VAR(1, 0x5C2F78, int, dword_5C2F78, 0);
ALIVE_VAR(1, 0x5C2FA0, short, word_5C2FA0, 0);
ALIVE_VAR(1, 0x5C9300, BaseGameObject*, pPauseMenu_5C9300, nullptr);

EXPORT void CC sub_422DA0()
{
    NOT_IMPLEMENTED(0x422DA0);
}

EXPORT void CC sub_449A90()
{
    NOT_IMPLEMENTED(0x449A90);
}

EXPORT void CC AnimateAllAnimations_40AC20(DynamicArray* /*pAnimations*/)
{
    NOT_IMPLEMENTED(0x40AC20);
}

struct InputPadObject
{
    DWORD field_0_pressed;
    BYTE field_4_dir;
    BYTE field_5;
    WORD field_6_padding; // Not confirmed
    DWORD field_8_previous;
    DWORD field_C_held;
    DWORD field_10_released;
    DWORD field_14_padding; // Not confirmed
};
ALIVE_ASSERT_SIZEOF(InputPadObject, 0x18);

enum PsxButtonBits : unsigned int
{
    eL2 = 1 << 0,
    eR2 = 1 << 1,
    eL1 = 1 << 2,
    eR1 = 1 << 3,
    eTriangle = 1 << 4,
    eCircle = 1 << 5,
    eCross = 1 << 6,
    eSquare = 1 << 7,
    eSelect = 1 << 8,
    // As seen in LibEtc.h of PSYQ.. don't think these can ever be used.
    // PADi 9 ?
    // PADj 10 ?
    eStart = 1 << 11,
    eDPadUp = 1 << 12,
    eDPadRight = 1 << 13,
    eDPadDown = 1 << 14,
    eDPadLeft = 1 << 15,
};

enum InputCommands : unsigned int
{
    eUp =           1 << 0,
    eDown =         1 << 1,
    eLeft =         1 << 2,
    eRight =        1 << 3,
    eRun =          1 << 4,
    eDoAction =     1 << 5,  // Pick up rock, pull lever etc
    eSneak =        1 << 6,
    eThrowItem =    1 << 7,  // Or I say I dunno if no items
    eHop =          1 << 8,
    eFartOrRoll =   1 << 9,  // (Only roll in AO)
    eGameSpeak1 =   1 << 10, // Hello
    eGameSpeak2 =   1 << 11, // (Follow Me)
    eGameSpeak3 =   1 << 12, // Wait
    eGameSpeak4 =   1 << 13, // (Work) (Whistle 1)
    eGameSpeak5 =   1 << 14, // (Anger)
    eGameSpeak6 =   1 << 15, // (All ya) (Fart)
    eGameSpeak7 =   1 << 16, // (Sympathy) (Whistle 2)
    eGameSpeak8 =   1 << 17, // (Stop it) (Laugh)
    eChant =        1 << 18, 
    ePause =        1 << 19, // Or enter
    eUnPause =      1 << 20, // Or/and back
    // 0x200000     = nothing
    eCheatMode =    1 << 22,
    // 0x800000     = nothing
    // 0x1000000    = nothing
    // 0x2000000    = nothing
    // 0x4000000    = nothing
    // 0x8000000    = nothing
    // 0x10000000   = nothing
    // 0x20000000   = nothing
    // 0x40000000   = nothing
    // 0x80000000   = nothing
};


class InputObject
{
public:
    EXPORT int Is_Demo_Playing_45F220();
    EXPORT void UnsetDemoPlaying_45F240();
    EXPORT void SetDemoResource_45F1E0(DWORD** pDemoRes);
    EXPORT void Update_45F040();
    EXPORT static DWORD CC Command_To_Raw_404354(DWORD cmd);
private:
    InputPadObject field_0_pads[2];
    DWORD** field_30_pDemoRes;
    DWORD field_34_demo_command_index;
    WORD field_38_bDemoPlaying;
    WORD field_3A_pad_idx;
    DWORD field_3C_command;
    DWORD field_40_command_duration;
};
ALIVE_ASSERT_SIZEOF(InputObject, 0x44);

ALIVE_VAR(1, 0x5BD4E0, InputObject, sInputObject_5BD4E0, {});

int InputObject::Is_Demo_Playing_45F220()
{
    return field_38_bDemoPlaying & 1;
}

void InputObject::UnsetDemoPlaying_45F240()
{
    field_38_bDemoPlaying &= ~1;
}

void InputObject::SetDemoResource_45F1E0(DWORD** pDemoRes)
{
    field_34_demo_command_index = 2;
    field_30_pDemoRes = pDemoRes;
    field_38_bDemoPlaying |= 1u;
    field_40_command_duration = 0;
}

ALIVE_VAR(1, 0x5C1BBE, unsigned __int16, sCurrentControllerIndex_5C1BBE, 0);
ALIVE_VAR(1, 0x5C1B84, unsigned int, sGnFrame_5C1B84, 0);
ALIVE_VAR(1, 0x5C1B9A, __int16, word_5C1B9A, 0);
ALIVE_VAR(1, 0x5CA4D2, BYTE, byte_5CA4D2, 0);


EXPORT int CC sub_4FA9C0(int /*padNum*/)
{
    return 0;
}

void InputObject::Update_45F040()
{
    const unsigned char byte_545A4C[20] =
    {
        0, // left?
        64, // up?
        192, // down?
        0,
        128, // right?
        96,
        160,
        128,
        0,
        32,
        224,
        0,
        0,
        64,
        192,
        0,
        0,
        0,
        0,
        0
    };

    field_0_pads[0].field_8_previous = field_0_pads[0].field_0_pressed;
    field_0_pads[0].field_0_pressed = sub_4FA9C0(0);

    if (Is_Demo_Playing_45F220())
    {
        // Stop if any button on any pad is pressed
        if (field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
        {
            word_5C1B9A = 0;
            UnsetDemoPlaying_45F240();
            return;
        }

        if (sGnFrame_5C1B84 >= field_40_command_duration)
        {
            const DWORD command = (*field_30_pDemoRes)[field_34_demo_command_index++];
            field_3C_command = command >> 16;
            field_40_command_duration = sGnFrame_5C1B84 + command & 0xFFFF;

            // End demo/quit command
            if (command & 0x8000)
            {
                UnsetDemoPlaying_45F240();
            }
        }

        // Will do nothing if we hit the end command..
        if (Is_Demo_Playing_45F220())
        {
            field_0_pads[0].field_0_pressed = Command_To_Raw_404354(field_3C_command);
        }
    }

    field_0_pads[0].field_10_released = field_0_pads[0].field_8_previous & ~field_0_pads[0].field_0_pressed;
    field_0_pads[0].field_C_held = field_0_pads[0].field_0_pressed & ~field_0_pads[0].field_8_previous;
    field_0_pads[0].field_4_dir = byte_545A4C[field_0_pads[0].field_0_pressed & 0xF];

    field_0_pads[1].field_8_previous = field_0_pads[1].field_0_pressed;
    field_0_pads[1].field_0_pressed = sub_4FA9C0(1);
    field_0_pads[1].field_10_released = field_0_pads[1].field_8_previous & ~field_0_pads[1].field_0_pressed;
    field_0_pads[1].field_C_held = field_0_pads[1].field_0_pressed & ~field_0_pads[1].field_8_previous;
    field_0_pads[1].field_4_dir = byte_545A4C[field_0_pads[1].field_0_pressed & 0xF];
}

DWORD CC InputObject::Command_To_Raw_404354(DWORD cmd)
{
    unsigned int shoulderButtonsPressedCount = 0;

    if (cmd & PsxButtonBits::eL2)
    {
        ++shoulderButtonsPressedCount;
    }

    if (cmd & PsxButtonBits::eR2)
    {
        ++shoulderButtonsPressedCount;
    }

    if (cmd & PsxButtonBits::eL1)
    {
        ++shoulderButtonsPressedCount;
    }

    if (cmd & PsxButtonBits::eR1)
    {
        ++shoulderButtonsPressedCount;
    }

    if (shoulderButtonsPressedCount > 1) // Any 2 shoulder button combo = chanting
    {
        return InputCommands::eChant;
    }

    DWORD rawInput = 0;
    if (cmd & PsxButtonBits::eDPadUp)
    {
        rawInput |= InputCommands::eUp;
    }

    if (cmd & PsxButtonBits::eDPadRight)
    {
        rawInput |= InputCommands::eRight;
    }

    if (cmd & PsxButtonBits::eDPadDown)
    {
        rawInput |= InputCommands::eDown;
    }

    if (cmd & eDPadLeft)
    {
        rawInput |= InputCommands::eLeft;
    }

    if (cmd & PsxButtonBits::eR1)
    {
        rawInput |= InputCommands::eRun;
    }

    if (cmd & PsxButtonBits::eR2)
    {
        rawInput |= InputCommands::eSneak;
    }

    if (cmd & PsxButtonBits::eL1)
    {
        if (cmd & PsxButtonBits::eTriangle)
        {
            rawInput |= InputCommands::eGameSpeak1;
        }

        if (cmd & PsxButtonBits::eCircle)
        {
            rawInput |= InputCommands::eGameSpeak4;
        }

        if (cmd & PsxButtonBits::eCross)
        {
            rawInput |= InputCommands::eGameSpeak3;
        }

        if (cmd & PsxButtonBits::eSquare)
        {
            rawInput |= InputCommands::eGameSpeak2;
        }
    }
    else if (cmd & PsxButtonBits::eL2)
    {
        if (cmd & PsxButtonBits::eTriangle)
        {
            rawInput |= InputCommands::eGameSpeak6;
        }

        if (cmd & PsxButtonBits::eCircle)
        {
            rawInput |= InputCommands::eGameSpeak7;
        }

        if (cmd & PsxButtonBits::eCross)
        {
            rawInput |= InputCommands::eGameSpeak5;
        }

        if (cmd & PsxButtonBits::eSquare)
        {
            rawInput |= InputCommands::eGameSpeak8;
        }
    }
    else // No shoulder buttons
    {
        if (cmd & PsxButtonBits::eTriangle)
        {
            rawInput |= InputCommands::eHop;
        }

        if (cmd & PsxButtonBits::eCircle)
        {
            rawInput |= InputCommands::eThrowItem;
        }

        if (cmd & PsxButtonBits::eCross)
        {
            rawInput |= InputCommands::eFartOrRoll;
        }

        if (cmd & PsxButtonBits::eSquare)
        {
            rawInput |= InputCommands::eDoAction;
        }
    }

    return rawInput;
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
    NOT_IMPLEMENTED(0x480B80);
}

ALIVE_VAR(1, 0x5C3030, Map, gMap_5C3030, {});

EXPORT void CC sub_494580()
{
    NOT_IMPLEMENTED(0x494580);
}

EXPORT signed int CC PSX_ClearImage_4F5BD0(PSX_RECT* /*pRect*/, unsigned __int8 /*r*/, unsigned __int8 /*g*/, __int16 /*b*/)
{
    NOT_IMPLEMENTED(0x4F5BD0);
    return 0;
}

EXPORT void CC Font_sub_4DD050()
{
    NOT_IMPLEMENTED(0x4DD050);
}

EXPORT int CC PSX_DrawSync_4F6280(int /*mode*/)
{
    return 0;
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
            AnimateAllAnimations_40AC20(gObjList_animations_5C1A24);
        }

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
        
        Font_sub_4DD050();
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
