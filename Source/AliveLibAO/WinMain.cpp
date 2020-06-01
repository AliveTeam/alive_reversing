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

START_NS_AO

void WinMain_ForceLink()
{

}

void Static_Inits_AO()
{

}


struct PSX_Pad
{
    unsigned __int16 field_0_pressed;
    unsigned __int8 field_2;
    char field_3;
    __int16 field_4;
    unsigned __int16 field_6_held;
    int field_8;
};
ALIVE_ASSERT_SIZEOF(PSX_Pad, 0xC);

class InputObject
{
public:
    EXPORT void InitPad_4331A0(unsigned int /*padCount*/)
    {
        NOT_IMPLEMENTED();
    }

    EXPORT void Update_433250()
    {
        NOT_IMPLEMENTED();
    }

    static EXPORT void CC Shutdown_433230()
    {
        NOT_IMPLEMENTED();
    }

    PSX_Pad field_0_pads[2];
    BYTE** field_18_demo_res;
    int field_1C_demo_command_index;
    __int16 field_20_demo_playing;
    __int16 field_22;
    int field_24_command;
    int field_28_command_duration;
    int field_2C;
};
ALIVE_ASSERT_SIZEOF(InputObject, 0x30);

class GameSpeak : public BaseGameObject
{
public:
    EXPORT GameSpeak* ctor_40F990()
    {
        NOT_IMPLEMENTED();
        return this;
    }

    EXPORT GameSpeak* Vdtor_40FB50(signed int /*flags*/)
    {
        NOT_IMPLEMENTED();
        return this;
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return Vdtor_40FB50(flags);
    }

    __int16 field_10;
    __int16 field_12;
    int field_14;
    int field_18_idx;
    char field_1C[32];
};
ALIVE_ASSERT_SIZEOF(GameSpeak, 0x3C);

class CheatController : public BaseGameObject
{
public:
    EXPORT CheatController* ctor_40FBF0()
    {
        NOT_IMPLEMENTED();
        return this;
    }

    EXPORT CheatController* Vdtor_40FCD0(signed int /*flags*/)
    {
        NOT_IMPLEMENTED();
        return this;
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return Vdtor_40FCD0(flags);
    }

    __int16 field_10;
    __int16 field_12;
};
ALIVE_ASSERT_SIZEOF(CheatController, 0x14);

class DDCheat : public BaseGameObject
{
public:
    static EXPORT void ClearProperties_4095B0()
    {
        NOT_IMPLEMENTED();
    }

    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
};
ALIVE_ASSERT_SIZEOF(DDCheat, 0x28);

struct PSX_Display_Buffer
{
    PSX_DRAWENV field_0_draw_env;
    PSX_DISPENV field_5C_disp_env;
    int* field_70_ot_buffer[256];
};
ALIVE_ASSERT_SIZEOF(PSX_Display_Buffer, 0x470);

struct PSX_Display_Params;

class PsxDisplay
{
public:

    EXPORT PsxDisplay* ctor_40DAB0(PSX_Display_Params* /*pParams*/)
    {
        NOT_IMPLEMENTED();
        return this;
    }

    EXPORT void PSX_Display_Render_OT_40DD20()
    {
        NOT_IMPLEMENTED();
    }

    unsigned __int16 field_0_width;
    unsigned __int16 field_2_height;
    __int16 field_4_bpp;
    __int16 field_6_max_buffers;
    unsigned __int16 field_8_buffer_size;
    unsigned __int16 field_A_buffer_index;
    PSX_Display_Buffer field_C_drawEnv[2];
};
ALIVE_ASSERT_SIZEOF(PsxDisplay, 0x8EC);

struct PSX_Display_Params
{
    __int16 field_0_width;
    __int16 field_2_height;
    __int16 field_4_bpp;
    __int16 field_6_max_buffers;
    __int16 field_8_buffer_size;
    __int16 field_A_k32;
    __int16 field_C_k1;
    __int16 field_E;
};
ALIVE_ASSERT_SIZEOF(PSX_Display_Params, 0x10);

ALIVE_VAR(1, 0x9F771C, HINSTANCE, sInstance_9F771C, 0);
ALIVE_VAR(1, 0x9F7784, HINSTANCE, sPrevInstance_9F7784, 0);
ALIVE_VAR(1, 0x9F772C, int, sCmdShow_9F772C, 0);
ALIVE_VAR(1, 0x9F7718, LPSTR, sCommandLine_9F7718, 0);
ALIVE_VAR(1, 0x9F7734, int, sExitCode_9F7734, 0);
ALIVE_VAR(1, 0xA8A4C0, Bitmap, sVGA_Bmp1_A8A4C0, {});
ALIVE_VAR(1, 0x9F7724, HWND, gHwnd_9F7724, nullptr);
ALIVE_VAR(1, 0x9F6650, HWND, hWnd_9F6650, nullptr);
ALIVE_VAR(1, 0x508BF8, char, gDDCheatMode_508BF8, 0);
ALIVE_VAR(1, 0x508BFC, char, byte_508BFC, 0);



EXPORT int CC VGA_FullScreenSet_490160(bool /*bFullScreen*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT signed int CC VGA_DisplaySet_490230(unsigned __int16 /*width*/, unsigned __int16 /*height*/, char /*bpp*/, unsigned __int8 /*backbufferCount*/, int* /*ppSurface*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int Error_WarningMessageBox_48E470(const char* /*pWarningMsg*/, ...)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC Error_ShowErrorStackToUser_48DF10(char /*bDisplayAsOneMessage*/)
{
    NOT_IMPLEMENTED();
}

EXPORT int CC VGA_GetPixelFormat_490E60()
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT signed int CC PSXEMU_SetDispType_499E60(int /*dispType*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT signed int CC BMP_ClearRect_48F810(Bitmap* /*pBmp*/, RECT* /*pRect*/, int /*fillColour*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC IO_Init_48E1A0(char /*bUnknown*/)
{
    // TODO: This has been totally changed in AE
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT signed int CC PSX_EMU_Set_Cd_Emulation_Paths_49B000(const char* /*pPath1*/, const char* /*pPath2*/, const char* /*pPath3*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC Sys_WindowClass_Register_48E9E0(LPCSTR /*lpClassName*/, LPCSTR /*lpWindowName*/, int /*X*/, int /*Y*/, int /*nWidth*/, int /*nHeight*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC PSX_EMU_Init_49A1D0(char /*bUnknown*/)
{
    NOT_IMPLEMENTED();
}

EXPORT signed int PSX_EMU_VideoAlloc_49A2B0()
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC Game_End_Frame_4505D0(char /*bSkip*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void Input_GetCurrentKeyStates_48E630()
{
    NOT_IMPLEMENTED();
}

EXPORT signed int CC PSX_EMU_SetCallBack_499920(int /*a1*/, int(CC* /*a2*/)(char))
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT char CC Input_IsVKPressed_48E5D0(int /*key*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC PSX_EMU_Set_screen_mode_499910(char /*a1*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_DispEnv_Set_48D900(int /*arg_0*/)
{
    NOT_IMPLEMENTED();
}

EXPORT LPSTR CC GetCommandLine_48E920()
{
    return sCommandLine_9F7718;
}

EXPORT HWND CC Sys_GetWindowHandle_48E930()
{
    return gHwnd_9F7724;
}

EXPORT void CC Sys_Set_Hwnd_48E340(HWND hwnd)
{
    hWnd_9F6650 = hwnd;
    // Note byte_9F5240 omited
}

static void Init_VGA_AndPsxVram()
{
    VGA_FullScreenSet_490160(1);
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
        PSXEMU_SetDispType_499E60(1);
        break;
    case 15:
        PSXEMU_SetDispType_499E60(4);
        break;
    case 16:
        PSXEMU_SetDispType_499E60(2);
        break;
    case 115:
        PSXEMU_SetDispType_499E60(5);
        break;
    case 116:
        PSXEMU_SetDispType_499E60(3);
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
    Sys_WindowClass_Register_48E9E0("ABE_WINCLASS", "Abe's Oddworld Oddysee 2.0", 32, 64, 640, 480); // Nice window title lol
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
            byte_508BFC = 0;;
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

EXPORT void PSX_EMU_VideoDeAlloc_49A550()
{
    NOT_IMPLEMENTED();
}

EXPORT int CC Sys_WindowMessageHandler_4503B0(HWND /*hWnd*/, UINT /*msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC Errors_Display_48E050()
{
    NOT_IMPLEMENTED();
}

EXPORT void SYS_EventsPump_44FF90()
{
    NOT_IMPLEMENTED();
}

EXPORT int DebugOut_495990(const char*, ...)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int PSX_ResetCallBack_49AFB0()
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC Init_Sound_DynamicArrays_And_Others_41CD20()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Input_Init_44EB60()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC DDCheat_Allocate_409560()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Game_Init_LoadingIcon_445E30()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Events_Reset_Active_417320()
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x507B78, short, sBreakGameLoop_507B78, 0);
ALIVE_VAR(1, 0x507668, short, sNumCamSwappers_507668, 0);

ALIVE_VAR(1, 0x505564, DynamicArrayT<AnimationBase>*, gObjList_animations_505564, nullptr);


struct DirtyBits
{
    __int16 field_0[20];
};
ALIVE_ASSERT_SIZEOF(DirtyBits, 0x28);

class ScreenManager : public BaseGameObject
{
public:

    FP_Point* field_10_pCamPos;
    __int16 field_14;
    unsigned __int16 field_16;
    int field_18;
    int field_1C;
    __int16 field_20;
    __int16 field_22;
    int field_24;
    int field_28;
    __int16 field_2C;
    unsigned __int16 field_2E_idx;
    unsigned __int16 field_30;
    unsigned __int16 field_32;
    __int16 field_34;
    __int16 field_36_flags;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    DirtyBits field_58[6];
};
ALIVE_ASSERT_SIZEOF(ScreenManager, 0x148);

ALIVE_VAR(1, 0x4FF7C8, ScreenManager*, pScreenManager_4FF7C8, nullptr);

EXPORT void CC DebugFont_Flush_487F50()
{
    NOT_IMPLEMENTED();
}

EXPORT int CC PSX_DrawSync_496750(int /*mode*/)
{
    return 0;
}

EXPORT int CC PSX_VSync_496620(int )
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC PSX_ClearImage_496020(const PSX_RECT* /*pRect*/, __int16 /*r*/, unsigned __int8 /*g*/, unsigned __int8 /*b*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

ALIVE_VAR(1, 0x507698, short, gAttract_507698, 0);
ALIVE_VAR(1, 0x507B0C, int, gTimeOut_NotUsed_507B0C, 0);
ALIVE_VAR(1, 0x507B10, int, gFileOffset_NotUsed_507B10, 0);
const PSX_Display_Params kDisplayParams = { 640, 240, 16, 1, 42, 32, 1, 0 };
ALIVE_VAR(1, 0x4BB830, PSX_Display_Params, gPsxDisplayParams_4BB830, kDisplayParams);
ALIVE_VAR(1, 0x504C78, PsxDisplay, gPsxDisplay_504C78, {});
ALIVE_VAR(1, 0x5009E8, InputObject, sInputObject_5009E8, {});

class BaseAliveGameObject;

class ResourceManager
{
public:
    static EXPORT void CC CancelPendingResourcesFor_41EA60(BaseAliveGameObject* /*pObj*/)
    {
        NOT_IMPLEMENTED();
    }
};


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

            if (pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3) && pObj->field_C_bCanKill == 0)
            {
                gBaseGameObject_list_9F2DF0->RemoveAt(i);
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
    ResourceManager::CancelPendingResourcesFor_41EA60(0);

    for (int i=0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObjToKill = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObjToKill)
        {
            break;
        }

        if (pObjToKill->field_C_bCanKill == 0)
        {
            gBaseGameObject_list_9F2DF0->RemoveAt(i);
            pObjToKill->VDestructor(1);
        }
    }
}

EXPORT void CC Game_Free_LoadingIcon_445E80()
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x4FF954, GameSpeak*, pEventSystem_4FF954, nullptr);
ALIVE_VAR(1, 0x4FF958, CheatController*, pCheatController_4FF958, nullptr);

EXPORT int CC PSX_ResetGraph_4987E0(int /*mode*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC PSX_StopCallBack_49AFC0()
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC PSX_CdControlB_49BB40(int, int, int)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC SND_Shutdown_476EC0()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC SND_Reset_Ambiance_4765E0()
{
    NOT_IMPLEMENTED();
}

class MusicController
{
public:
    // TODO: Class data

    static EXPORT void Shutdown_4437E0()
    {
        NOT_IMPLEMENTED();
    }
};


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

    gMap_507BA8.Init_443EE0(0, 1, 10, 0, 0, 0);

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

using TFilter = std::add_pointer<int CC(HWND, UINT, WPARAM, LPARAM)>::type;

EXPORT void CC Sys_SetWindowProc_Filter_48E950(TFilter)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Game_ExitGame_450730()
{
    PSX_EMU_VideoDeAlloc_49A550();
}

EXPORT void Game_Main_450050()
{
    Main_ParseCommandLineArguments();
    Game_SetExitCallBack_48E040(Game_ExitGame_450730);
    Sys_SetWindowProc_Filter_48E950(Sys_WindowMessageHandler_4503B0);
    Game_Run_4373D0();

    // TODO: AE inlined calls here (pull AE's code into another func)
    Errors_Display_48E050();
}

EXPORT int CALLBACK WinMain_48EF50(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    sInstance_9F771C = hInstance;
    sPrevInstance_9F7784 = hPrevInstance;
    sCmdShow_9F772C = nShowCmd;
    sCommandLine_9F7718 = lpCmdLine;
    Game_Main_450050();
    return sExitCode_9F7734;
}

END_NS_AO
