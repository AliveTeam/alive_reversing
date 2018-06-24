#include "stdafx.h"
#include "VGA.hpp"
#include "bmp.hpp"
#include "DDraw.hpp"
#include "Function.hpp"
#include "Error.hpp"
#include "Sys.hpp"

void VGA_ForceLink() {}

#ifdef BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE
EXPORT signed int CC VGA_FullScreenSet_4F31F0(char /*bFullScreen*/)
{
    LOG_INFO("Stub"); // Can't be empty func otherwise NOT_IMPLEMENT'ed searcher will look into the next function
    return 0;
}
#endif

ALIVE_VAR(1, 0xBD0BFA, bool, sVGA_own_surfaces_BD0BFA, false);
ALIVE_VAR(1, 0xBC0BB8, bool, sVGA_Inited_BC0BB8, false);
ALIVE_VAR(1, 0xBD0BF8, bool, sVGA_IsWindowMode_BD0BF8, false);

ALIVE_VAR(1, 0xBD0BD0, Bitmap, sVGA_Bmp0_BD0BD0, {});
ALIVE_VAR(1, 0xBD2A20, Bitmap, sVGA_Bmp1_BD2A20, {});
ALIVE_VAR(1, 0xBD2A40, Bitmap, sVGA_Bmp2_BD2A40, {});


ALIVE_VAR(1, 0xBD0BBC, char, sVGA_BD0BBC, 0);
ALIVE_VAR(1, 0xBD0BF9, char, sVGA_bpp_BD0BF9, 0);
ALIVE_VAR(1, 0xBD0BEC, WORD, sVGA_height_BD0BEC, 0);
ALIVE_VAR(1, 0xBD0BC4, WORD, sVGA_width_BD0BC4, 0);

EXPORT void CC VGA_Shutdown_4F3170()
{
    if (sDD_Surface1_BBC3C8)
    {
        if (!sVGA_own_surfaces_BD0BFA)
        {
            sDD_Surface1_BBC3C8 = nullptr;
            sDD_Surface2_BBC3CC = nullptr;
        }

        if (sVGA_Bmp0_BD0BD0.field_0_pSurface)
        {
            Bmp_Free_4F1950(&sVGA_Bmp0_BD0BD0);
        }

        DD_Shutdown_4F0790(1);
        sDD_Surface1_BBC3C8 = nullptr;
        sDD_Surface2_BBC3CC = nullptr;
    }
    
    sVGA_Inited_BC0BB8 = false;

    memset(&sVGA_Bmp1_BD2A20, 0, sizeof(sVGA_Bmp1_BD2A20));
    memset(&sVGA_Bmp2_BD2A40, 0, sizeof(sVGA_Bmp2_BD2A40));
}

EXPORT bool VGA_IsWindowMode_4F31E0()
{
    return sVGA_IsWindowMode_BD0BF8;
}

EXPORT Bitmap* VGA_GetBitmap_4F3F00()
{
    return &sVGA_Bmp1_BD2A20;
}

EXPORT int VGA_GetPixelFormat_4F3EE0()
{
    return sVGA_Bmp1_BD2A20.field_15_pixel_format;
}

EXPORT int CC VGA_Convert_Colour_4F4DB0(int r, int g, int b)
{
    return Bmp_Convert_Colour_4F17D0(VGA_GetBitmap_4F3F00(), r, g, b);
}

EXPORT signed int CC VGA_ClearRect_4F4CF0(RECT* pRect, DWORD fillColour)
{
    return BMP_ClearRect_4F1EE0(VGA_GetBitmap_4F3F00(), pRect, fillColour);
}

EXPORT void CC VGA_CopyToFront_4F3730(Bitmap* /*pBmp*/, RECT* /*pRect*/, int /*screenMode*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC VGA_CopyToFront_4F3EB0(Bitmap* pBmp, RECT* pRect, unsigned __int8 screenMode)
{
    VGA_CopyToFront_4F3730(pBmp, pRect, screenMode);
}

EXPORT void CC VGA_CopyToFront_4F3710(Bitmap* pBmp, RECT* pRect)
{
    VGA_CopyToFront_4F3730(pBmp, pRect, 0);
}

EXPORT signed int CC VGA_DisplaySet_4F32C0(unsigned __int16 width, unsigned __int16 height, unsigned __int8 bpp, unsigned __int8 a4, IDirectDrawSurface** ppSurface)
{
    signed int result = 0;
   
    // TODO: Window sub classing for VGA_WindowSubClass_4F2F50 removed as it only exists to support 8 bpp mode.

    if (sVGA_Inited_BC0BB8)
    {
        VGA_Shutdown_4F3170();
    }

    if (DD_Init_4F02D0(Sys_GetHWnd_4F2C70(), sVGA_IsWindowMode_BD0BF8, gVGA_force_sys_memory_surfaces_BC0BB4))
    {
        if (ppSurface)
        {
            sVGA_own_surfaces_BD0BFA = false;
            sDD_Surface1_BBC3C8 = *ppSurface;
            sDD_Surface2_BBC3CC = *ppSurface;
            a4 = 1;
        }
        else
        {
            sVGA_own_surfaces_BD0BFA = true;
        }
        
        sVGA_BD0BBC = a4;
        sVGA_bpp_BD0BF9 = bpp;
        memset(&sVGA_Bmp1_BD2A20, 0, sizeof(sVGA_Bmp1_BD2A20));
        memset(&sVGA_Bmp2_BD2A40, 0, sizeof(sVGA_Bmp2_BD2A40));
        sVGA_height_BD0BEC = height;
        sVGA_width_BD0BC4 = width;

        if (DD_Enable_4F0380(Sys_GetHWnd_4F2C70(), width, height, bpp, a4, 0))
        {
            if (!sVGA_IsWindowMode_BD0BF8)
            {
                RECT rect = {};
                ::SetWindowPos(Sys_GetHWnd_4F2C70(), HWND_TOPMOST, 0, 0, width, height, 0x204u); // TODO: SDK constants
                ::GetClientRect(Sys_GetHWnd_4F2C70(), &rect);
                if (width != rect.right || height != rect.bottom)
                {
                    ::SetWindowPos(Sys_GetHWnd_4F2C70(), HWND_TOPMOST, 0, 0, width - rect.right + width, height - rect.bottom + height, 0x204u); // TODO: SDK constants
                }
            }

            if (!sVGA_own_surfaces_BD0BFA || DD_Init_4F0840(a4))
            {
                DDSURFACEDESC surfaceDesc = {};
                surfaceDesc.dwSize = sizeof(DDSURFACEDESC);
                if (sDD_Surface1_BBC3C8->GetSurfaceDesc(&surfaceDesc))
                {
                    Error_PushErrorRecord_4F2920(
                        "C:\\abe2\\code\\POS\\VGA.C",
                        368,
                        -1,
                        "VGA_DisplaySet: problem calling IDirectDrawSurface_GetSurfaceDesc");
                }
                else
                {
                    sVGA_Bmp1_BD2A20.field_0_pSurface = sDD_Surface1_BBC3C8;
                    sVGA_Bmp1_BD2A20.field_8_width = surfaceDesc.dwWidth;
                    sVGA_Bmp1_BD2A20.field_10_locked_pitch = surfaceDesc.lPitch;
                    sVGA_Bmp1_BD2A20.field_C_height = surfaceDesc.dwHeight;
                    sVGA_Bmp1_BD2A20.field_14_bpp = static_cast<char>(surfaceDesc.ddpfPixelFormat.dwRGBBitCount);
                    sVGA_Bmp1_BD2A20.field_18_create_flags = 2;
                    memcpy(&sVGA_Bmp2_BD2A40, &sVGA_Bmp1_BD2A20, sizeof(sVGA_Bmp2_BD2A40));
                    sVGA_Bmp2_BD2A40.field_0_pSurface = sDD_Surface2_BBC3CC;
                    sVGA_Inited_BC0BB8 = 1;

                    // TODO: Refactor to own function, change if/else chain to early outs
                    switch (surfaceDesc.ddpfPixelFormat.dwRGBBitCount)
                    {
                    case 1u:
                        sVGA_Bmp1_BD2A20.field_15_pixel_format = 1;
                        return 0;
                    case 2u:
                        sVGA_Bmp1_BD2A20.field_15_pixel_format = 2;
                        return 0;
                    case 4u:
                        sVGA_Bmp1_BD2A20.field_15_pixel_format = 4;
                        return 0;
                    case 8u:
                        sVGA_Bmp1_BD2A20.field_15_pixel_format = 8;
                        return 0;
                    case 16u:
                        if (surfaceDesc.ddpfPixelFormat.dwRBitMask == 63488)
                        {
                            if (surfaceDesc.ddpfPixelFormat.dwGBitMask == 2016 && surfaceDesc.ddpfPixelFormat.dwBBitMask == 31)
                            {
                                sVGA_Bmp1_BD2A20.field_15_pixel_format = 16;
                                return 0;
                            }
                            break;
                        }
                        if (surfaceDesc.ddpfPixelFormat.dwRBitMask == 31)
                        {
                            if (surfaceDesc.ddpfPixelFormat.dwGBitMask == 2016)
                            {
                                if (surfaceDesc.ddpfPixelFormat.dwBBitMask != 63488)
                                    break;
                                sVGA_Bmp1_BD2A20.field_15_pixel_format = 116;
                                return 0;
                            }
                        }
                        else
                        {
                            if (surfaceDesc.ddpfPixelFormat.dwRBitMask == 31744)
                            {
                                if (surfaceDesc.ddpfPixelFormat.dwGBitMask == 992 && surfaceDesc.ddpfPixelFormat.dwBBitMask == 31)
                                {
                                    sVGA_Bmp1_BD2A20.field_15_pixel_format = 15;
                                    return 0;
                                }
                                break;
                            }
                            if (surfaceDesc.ddpfPixelFormat.dwRBitMask != 31)
                                break;
                        }
                        if (surfaceDesc.ddpfPixelFormat.dwGBitMask == 992 && surfaceDesc.ddpfPixelFormat.dwBBitMask == 31744)
                        {
                            sVGA_Bmp1_BD2A20.field_15_pixel_format = 115;
                            return 0;
                        }
                        break;
                    case 24u:
                        sVGA_Bmp1_BD2A20.field_15_pixel_format = 24;
                        return 0;
                    case 32u:
                        sVGA_Bmp1_BD2A20.field_15_pixel_format = 32;
                        return 0;
                    default:
                        break;
                    }
                }
                result = 0;
            }
            else
            {
                result = -1;
            }
        }
        else
        {
            result = -1;
        }
    }
    else
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\VGA.C", 273, -1, "VGA_DisplaySet(): DDInit failed");
        result = -1;
    }
    return result;
}

ALIVE_VAR(1, 0xBD0BF0, int, sbVga_LockedType_BD0BF0, 0); // TODO: Enum
ALIVE_VAR(1, 0xBD0BC8, HDC, sVga_HDC_BD0BC8, 0);
ALIVE_VAR(1, 0xBD0BC0, int, sVga_LockPType_BD0BC0, 0);
ALIVE_VAR(1, 0xBD0BF4, LPVOID, sVgaLockBuffer_BD0BF4, 0);


EXPORT void VGA_BuffUnlockPtr_4F2FB0()
{
    if (sbVga_LockedType_BD0BF0)
    {
        Bitmap* pBmpToUnlock = &sVGA_Bmp1_BD2A20;
        if (sVga_LockPType_BD0BC0 != 3)
        {
            pBmpToUnlock = &sVGA_Bmp2_BD2A40;
        }

        if (sbVga_LockedType_BD0BF0 == 1)
        {
            BMP_unlock_4F2100(pBmpToUnlock);
            sVgaLockBuffer_BD0BF4 = nullptr;
        }
        else if (sbVga_LockedType_BD0BF0 == 2)
        {
            BMP_Release_DC_4F21A0(pBmpToUnlock, sVga_HDC_BD0BC8);
            sVga_HDC_BD0BC8 = 0;
            sbVga_LockedType_BD0BF0 = 0;
            return;
        }
        sbVga_LockedType_BD0BF0 = 0;
    }
}


EXPORT LPVOID CC VGA_BuffLockPtr_4F30A0(int always3)
{
    LPVOID pLockedBuffer = sVgaLockBuffer_BD0BF4;
    if (!pLockedBuffer)
    {
        if (sVga_HDC_BD0BC8)
        {
            return nullptr;
        }

        if (always3 == 3)
        {
            if (!sVGA_IsWindowMode_BD0BF8)
            {
                RECT rect = ClientToScreenConvert(Sys_GetHWnd_4F2C70());
                if (rect.left < 0 || rect.top < 0)
                {
                    return nullptr;
                }
            }
            pLockedBuffer = BMP_Lock_4F1FF0(&sVGA_Bmp1_BD2A20);
        }
        else
        {
            if (always3 != 4)
            {
                Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\VGA.C", 179, -1, "BuffLockPtr wrong PTYPE");
                return 0;
            }
            pLockedBuffer = BMP_Lock_4F1FF0(&sVGA_Bmp2_BD2A40);
        }
        sVgaLockBuffer_BD0BF4 = pLockedBuffer;
        if (pLockedBuffer)
        {
            sbVga_LockedType_BD0BF0 = 1;
            sVga_LockPType_BD0BC0 = always3;
        }
    }
    return pLockedBuffer;
}
