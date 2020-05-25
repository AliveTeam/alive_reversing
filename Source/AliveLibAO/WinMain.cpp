#include "stdafx_ao.h"
#include "WinMain.hpp"
#include "Function.hpp"

START_NS_AO

void WinMain_ForceLink()
{

}

void Static_Inits_AO()
{

}


ALIVE_VAR(1, 0x9F771C, HINSTANCE, sInstance_9F771C, 0);
ALIVE_VAR(1, 0x9F7784, HINSTANCE, sPrevInstance_9F7784, 0);
ALIVE_VAR(1, 0x9F772C, int, sCmdShow_9F772C, 0);
ALIVE_VAR(1, 0x9F7718, LPSTR, sCommandLine_9F7718, 0);
ALIVE_VAR(1, 0x9F7734, int, sExitCode_9F7734, 0);

/*
static Init_VGA_AndPsxVram()
{
    VGA_FullScreenSet_490160(1);
    VGA_DisplaySet_490230(640u, 480u, 16, 1u, 0);
    rect.left = 0;
    rect.top = 0;
    rect.right = 640;
    rect.bottom = 480;
    BMP_48F810(&sVGA_Bmp1_A8A4C0, &rect, 0);
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
        SYS_WarningMessage_48E470("This program requires a high-color display mode of 32768 or 65536 colors at 640x480 resolution.");
        Error_ShowErrorStackToUser_48DF10(0);
        exit(0);
        return;
    }
}

static Main_ParseCommandLineArguments()
{
    IO_Init_48E1A0(0);
    v0 = GetCommandLine_48E920();
    v1 = GetCommandLine_48E920();
    LOBYTE(cdDrivePath[1]) = byte_4CECCA;
    cdDrivePath[0] = gDriveLetter_4CECC8;
    if (gDriveLetter_4CECC8 > 'Z')
    {
        LOBYTE(cdDrivePath[0]) = 'C';
    }
    PSX_EMU_Set_Cd_Emulation_Paths_49B000(".", cdDrivePath, 0);
    Sys_WindowClass_Register_48E9E0("ABE_WINCLASS", "Abe's Oddworld Oddysee 2.0", 32, 64, 640, 480);
    v2 = Sys_GetWindowHandle_48E930();
    Sys_Set_Hwnd_48E340(v2);
    if (v0)
    {
        if (!memcmp(v0, "-it_is_me_your_father", 22u))
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
    if (!v1)
    {
        goto LABEL_26;
    }

    if (!_strcmpi(v1, "-interline"))
    {
        PSX_DispEnv_Set_48D900(1);
        PSX_EMU_Set_screen_mode_499910(1);
        byte_508BFC = 0;
        goto LABEL_15;
    }

    if (!_strcmpi(v1, "-vstretch"))
    {
        PSX_DispEnv_Set_48D900(0);
        PSX_EMU_Set_screen_mode_499910(0);
        byte_508BFC = 0;
        goto LABEL_15;
    }

    if (_strcmpi(v1, "-vdouble"))
    {
    LABEL_26:
        PSX_DispEnv_Set_48D900(2);
        PSX_EMU_Set_screen_mode_499910(2);
    }
    else
    {
        PSX_DispEnv_Set_48D900(0);
        PSX_EMU_Set_screen_mode_499910(0);
        byte_508BFC = 1;
    }
LABEL_15:
   
    Init_VGA_AndPsxVram();

    PSX_EMU_Init_49A1D0(0);
    PSX_EMU_VideoAlloc_49A2B0();
    PSX_EMU_SetCallBack_499920(1, Game_End_Frame_4505D0);
    Main_Set_HWND_499900(Sys_GetWindowHandle_48E930());
}
*/

EXPORT void Game_Main_450050()
{
    NOT_IMPLEMENTED();
    /*
    Main_ParseCommandLineArguments();
    Game_SetExitCallBack_48E040(Game_ExitGame_450730);
    Sys_SetWindowProc_Filter_48E950(Sys_WindowMessageHandler_4503B0);
    Game_Run_4373D0();

    // TODO: AE inlined calls here
    Errors_Display_48E050();
    */
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
