#include "stdafx_ao.h"
#include "WinMain.hpp"
#include "Function.hpp"
#include "bmp.hpp"

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

EXPORT void Game_Run_4373D0()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Game_SetExitCallBack_48E040(void*)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Sys_SetWindowProc_Filter_48E950(void*)
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
