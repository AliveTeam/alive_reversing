#include "stdafx.h"
#include "VGA.hpp"
#include "bmp.hpp"
#include "DDraw.hpp"
#include "Function.hpp"
#include "Error.hpp"
#include "Sys.hpp"

void VGA_ForceLink() {}


ALIVE_VAR(1, 0xBD0BFA, bool, sVGA_own_surfaces_BD0BFA, false);
ALIVE_VAR(1, 0xBC0BB8, bool, sVGA_Inited_BC0BB8, false);
ALIVE_VAR(1, 0xBD0BF8, bool, sVGA_IsWindowMode_BD0BF8, false);

ALIVE_VAR(1, 0xBD0BD0, Bitmap, sVGA_Bmp0_BD0BD0, {});
ALIVE_VAR(1, 0xBD2A20, Bitmap, sVGA_bmp_primary_BD2A20, {});
ALIVE_VAR(1, 0xBD2A40, Bitmap, sVGA_bmp_back_buffer_BD2A40, {});


ALIVE_VAR(1, 0xBD0BBC, char, sVGA_BD0BBC, 0);
ALIVE_VAR(1, 0xBD0BF9, char, sVGA_bpp_BD0BF9, 0);
ALIVE_VAR(1, 0xBD0BEC, WORD, sVGA_height_BD0BEC, 0);
ALIVE_VAR(1, 0xBD0BC4, WORD, sVGA_width_BD0BC4, 0);

ALIVE_VAR(1, 0xBD0BF0, int, sbVga_LockedType_BD0BF0, 0); // TODO: Enum
ALIVE_VAR(1, 0xBD0BC8, HDC, sVga_HDC_BD0BC8, 0);
ALIVE_VAR(1, 0xBD0BC0, int, sVga_LockPType_BD0BC0, 0);
ALIVE_VAR(1, 0xBD0BF4, LPVOID, sVgaLockBuffer_BD0BF4, 0);

#if USE_SDL2
EXPORT signed int CC VGA_FullScreenSet_4F31F0(bool /*bFullScreen*/)
{
    //  NOT_IMPLEMENTED();
    LOG_INFO("Stub"); // Can't be empty func otherwise NOT_IMPLEMENT'ed searcher will look into the next function
    return 0;
}

EXPORT void CC VGA_Shutdown_4F3170()
{
    if (sDD_primary_surface_BBC3C8)
    {
        if (!sVGA_own_surfaces_BD0BFA)
        {
            sDD_primary_surface_BBC3C8 = nullptr;
            sDD_surface_backbuffer_BBC3CC = nullptr;
        }

        if (sVGA_Bmp0_BD0BD0.field_0_pSurface)
        {
            Bmp_Free_4F1950(&sVGA_Bmp0_BD0BD0);
        }

        //DD_Shutdown_4F0790(1);
        sDD_primary_surface_BBC3C8 = nullptr;
        sDD_surface_backbuffer_BBC3CC = nullptr;
    }

    sVGA_Inited_BC0BB8 = false;

    memset(&sVGA_bmp_primary_BD2A20, 0, sizeof(sVGA_bmp_primary_BD2A20));
    memset(&sVGA_bmp_back_buffer_BD2A40, 0, sizeof(sVGA_bmp_back_buffer_BD2A40));
}

EXPORT bool VGA_IsWindowMode_4F31E0()
{
    return sVGA_IsWindowMode_BD0BF8;
}

EXPORT Bitmap* VGA_GetBitmap_4F3F00()
{
    return &sVGA_bmp_primary_BD2A20;
}

EXPORT int VGA_GetPixelFormat_4F3EE0()
{
    return sVGA_bmp_primary_BD2A20.field_15_pixel_format;
}

EXPORT int CC VGA_Convert_Colour_4F4DB0(int r, int g, int b)
{
    return Bmp_Convert_Colour_4F17D0(VGA_GetBitmap_4F3F00(), r, g, b);
}

EXPORT signed int CC VGA_ClearRect_4F4CF0(RECT* pRect, DWORD fillColour)
{
    return BMP_ClearRect_4F1EE0(VGA_GetBitmap_4F3F00(), pRect, fillColour);
}

static SDL_Renderer* gRenderer = nullptr;

EXPORT void CC VGA_CopyToFront_4F3730(Bitmap* pBmp, RECT* pRect, int /*screenMode*/)
{
    SDL_Rect copyRect = {};
    if (pRect)
    {
        copyRect = { pRect->left, pRect->top, pRect->right, pRect->bottom };
    }

    SDL_Rect* pCopyRect = pRect ? &copyRect : nullptr;

    SDL_UnlockSurface(pBmp->field_0_pSurface);
    SDL_UnlockSurface(sVGA_bmp_primary_BD2A20.field_0_pSurface);

    if (SDL_BlitSurface(pBmp->field_0_pSurface, pCopyRect, sVGA_bmp_primary_BD2A20.field_0_pSurface, nullptr) == 0)
    {
        // TODO: This will really murder performance
        SDL_Texture* pTexture = SDL_CreateTextureFromSurface(gRenderer, pBmp->field_0_pSurface);
        if (pTexture)
        {
            SDL_RenderCopy(gRenderer, pTexture, pCopyRect, nullptr);
            SDL_DestroyTexture(pTexture);
        }
        else
        {
            LOG_ERROR("Create texture failure");
        }
    }
    else
    {
        LOG_ERROR("Blt failure");
    }
   
    SDL_RenderPresent(gRenderer);
}

EXPORT void CC VGA_CopyToFront_4F3710(Bitmap* pBmp, RECT* pRect)
{
    VGA_CopyToFront_4F3730(pBmp, pRect, 0);
}

EXPORT void CC VGA_CopyToFront_4F3EB0(Bitmap* pBmp, RECT* pRect, unsigned __int8 screenMode)
{
    VGA_CopyToFront_4F3730(pBmp, pRect, screenMode);
}

EXPORT signed int CC VGA_DisplaySet_4F32C0(unsigned __int16 width, unsigned __int16 height, unsigned __int8 bpp, unsigned __int8 backbufferCount, TSurfaceType** ppSurface)
{
    // TODO: Window sub classing for VGA_WindowSubClass_4F2F50 removed as it only exists to support 8 bpp mode.

    if (sVGA_Inited_BC0BB8)
    {
        VGA_Shutdown_4F3170();
    }

    if (ppSurface)
    {
        sVGA_own_surfaces_BD0BFA = false;
        backbufferCount = 1;
    }
    else
    {
        sVGA_own_surfaces_BD0BFA = true;
    }

    sVGA_BD0BBC = backbufferCount;
    sVGA_bpp_BD0BF9 = bpp;
    memset(&sVGA_bmp_primary_BD2A20, 0, sizeof(sVGA_bmp_primary_BD2A20));
    memset(&sVGA_bmp_back_buffer_BD2A40, 0, sizeof(sVGA_bmp_back_buffer_BD2A40));
    sVGA_height_BD0BEC = height;
    sVGA_width_BD0BC4 = width;

    if (!sVGA_IsWindowMode_BD0BF8)
    {
        /*
        RECT rect = {};
        ::SetWindowPos(Sys_GetHWnd_4F2C70(), HWND_TOPMOST, 0, 0, width, height, 0x204u); // TODO: SDK constants
        ::GetClientRect(Sys_GetHWnd_4F2C70(), &rect);
        if (width != rect.right || height != rect.bottom)
        {
            ::SetWindowPos(Sys_GetHWnd_4F2C70(), HWND_TOPMOST, 0, 0, width - rect.right + width, height - rect.bottom + height, 0x204u); // TODO: SDK constants
        }
        */
    }

    if (sVGA_own_surfaces_BD0BFA /*|| DD_Init_4F0840(backbufferCount)*/)
    {


        // Create primary surface
        sVGA_bmp_primary_BD2A20.field_0_pSurface = SDL_CreateRGBSurface(0, width, height, bpp, 0x7c00, 0x03e0, 0x001f, 0x0); // TODO
//        sVGA_bmp_primary_BD2A20.field_0_pSurface = SDL_GetWindowSurface(Sys_GetHWnd_4F2C70());

        sVGA_bmp_primary_BD2A20.field_8_width = width;
        sVGA_bmp_primary_BD2A20.field_10_locked_pitch = sVGA_bmp_primary_BD2A20.field_0_pSurface->pitch; // TODO: Probably wrong ?
        sVGA_bmp_primary_BD2A20.field_C_height = height;
        sVGA_bmp_primary_BD2A20.field_14_bpp = static_cast<char>(bpp);
        sVGA_bmp_primary_BD2A20.field_18_create_flags = 2;

        // TODO: Create back buffer surface

        /*
        memcpy(&sVGA_bmp_back_buffer_BD2A40, &sVGA_bmp_primary_BD2A20, sizeof(sVGA_bmp_back_buffer_BD2A40));
        sVGA_bmp_back_buffer_BD2A40.field_0_pSurface = sDD_surface_backbuffer_BBC3CC;
        */

        sVGA_Inited_BC0BB8 = 1;

        gRenderer = SDL_CreateRenderer(Sys_GetHWnd_4F2C70(), -1, 0);
        //  gRenderer = SDL_CreateSoftwareRenderer(sVGA_bmp_primary_BD2A20.field_0_pSurface);
        if (!gRenderer)
        {
            ALIVE_FATAL("Render create failed");
        }
        SDL_RenderClear(gRenderer);

        switch (sVGA_bmp_primary_BD2A20.field_0_pSurface->format->BitsPerPixel)
        {
        case 1u:
            sVGA_bmp_primary_BD2A20.field_15_pixel_format = 1;
            return 0;
        case 2u:
            sVGA_bmp_primary_BD2A20.field_15_pixel_format = 2;
            return 0;
        case 4u:
            sVGA_bmp_primary_BD2A20.field_15_pixel_format = 4;
            return 0;
        case 8u:
            sVGA_bmp_primary_BD2A20.field_15_pixel_format = 8;
            return 0;
        case 15u:
        case 16u:
            sVGA_bmp_primary_BD2A20.field_15_pixel_format = 16;
            return 0;
        case 32u:
            sVGA_bmp_primary_BD2A20.field_15_pixel_format = 32;
            return 0;
        default:
            break;
        }
    }

  
    return 0;
}

EXPORT void VGA_BuffUnlockPtr_4F2FB0()
{
    // TODO
}

EXPORT LPVOID CC VGA_BuffLockPtr_4F30A0(int /*always3*/)
{
    // TODO
    return nullptr;
}

#else

#if BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE
EXPORT signed int CC VGA_FullScreenSet_4F31F0(bool /*bFullScreen*/)
{
  //  NOT_IMPLEMENTED();
    LOG_INFO("Stub"); // Can't be empty func otherwise NOT_IMPLEMENT'ed searcher will look into the next function
    return 0;
}
#endif


EXPORT void CC VGA_Shutdown_4F3170()
{
    if (sDD_primary_surface_BBC3C8)
    {
        if (!sVGA_own_surfaces_BD0BFA)
        {
            sDD_primary_surface_BBC3C8 = nullptr;
            sDD_surface_backbuffer_BBC3CC = nullptr;
        }

        if (sVGA_Bmp0_BD0BD0.field_0_pSurface)
        {
            Bmp_Free_4F1950(&sVGA_Bmp0_BD0BD0);
        }

        DD_Shutdown_4F0790(1);
        sDD_primary_surface_BBC3C8 = nullptr;
        sDD_surface_backbuffer_BBC3CC = nullptr;
    }
    
    sVGA_Inited_BC0BB8 = false;

    memset(&sVGA_bmp_primary_BD2A20, 0, sizeof(sVGA_bmp_primary_BD2A20));
    memset(&sVGA_bmp_back_buffer_BD2A40, 0, sizeof(sVGA_bmp_back_buffer_BD2A40));
}

EXPORT bool VGA_IsWindowMode_4F31E0()
{
    return sVGA_IsWindowMode_BD0BF8;
}

EXPORT Bitmap* VGA_GetBitmap_4F3F00()
{
    return &sVGA_bmp_primary_BD2A20;
}

EXPORT int VGA_GetPixelFormat_4F3EE0()
{
    return sVGA_bmp_primary_BD2A20.field_15_pixel_format;
}

EXPORT int CC VGA_Convert_Colour_4F4DB0(int r, int g, int b)
{
    return Bmp_Convert_Colour_4F17D0(VGA_GetBitmap_4F3F00(), r, g, b);
}

EXPORT signed int CC VGA_ClearRect_4F4CF0(RECT* pRect, DWORD fillColour)
{
    return BMP_ClearRect_4F1EE0(VGA_GetBitmap_4F3F00(), pRect, fillColour);
}

EXPORT void CC VGA_CopyToFront_4F3730(Bitmap* pBmp, RECT* pRect, int screenMode)
{
    Bitmap *pBitmapToUse; // ebp
    int srcWidth; // ebx
    char bpp; // cl
    int v7 = 0; // eax
    unsigned __int16 *v22; // esi
    char *v23; // edx
    int v24; // ecx
    char *v25; // eax
    unsigned int v26; // edi
    unsigned __int16 v27; // ax
    int v28; // ebx
    char v29; // bp
    char v30; // bl
    unsigned int v31; // edi
    unsigned __int16 v32; // ax
    char v34; // [esp+10h] [ebp-438h]
    char v35; // [esp+10h] [ebp-438h]
    char v37; // [esp+14h] [ebp-434h]
    char v38; // [esp+14h] [ebp-434h]
    int v40; // [esp+18h] [ebp-430h]
    int srcWidth2; // [esp+1Ch] [ebp-42Ch]
    int v45; // [esp+20h] [ebp-428h]
    LONG srcX; // [esp+24h] [ebp-424h]
    char v47; // [esp+24h] [ebp-424h]
    int height; // [esp+28h] [ebp-420h]
    int v50; // [esp+2Ch] [ebp-41Ch]
    char *v52; // [esp+30h] [ebp-418h]
    LONG srcY; // [esp+34h] [ebp-414h]
    RECT rect; // [esp+38h] [ebp-410h]

    if (sVGA_bmp_primary_BD2A20.field_8_width == 0)
    {
        return;
    }

    if (pRect)
    {
        pBitmapToUse = pBmp;
        srcX = pRect->left;
        srcY = pRect->top;
        srcWidth = pRect->right - pRect->left;
        srcWidth2 = pRect->right - pRect->left;
        height = pRect->bottom - srcY;
    }
    else
    {
        pBitmapToUse = pBmp;
        srcX = 0;
        srcY = 0;
        srcWidth = pBmp->field_8_width;
        srcWidth2 = pBmp->field_8_width;
        height = pBmp->field_C_height;
    }

    if (pBitmapToUse && pBitmapToUse->field_0_pSurface)
    {
        bpp = sVGA_bmp_primary_BD2A20.field_14_bpp;
        if (pBitmapToUse->field_14_bpp == sVGA_bmp_primary_BD2A20.field_14_bpp)
        {
            DD_render_back_buffer_4F0D90(pBitmapToUse->field_0_pSurface, pRect, screenMode);
            if (sVGA_Bmp0_BD0BD0.field_0_pSurface)
            {
                Bmp_Free_4F1950(&sVGA_Bmp0_BD0BD0);
            }
            return;
        }

        // TODO: We never hit this point so it can't be cleaned up or debugged

        if (pBitmapToUse->field_14_bpp != 16)
        {
            return;
        }

        if (srcWidth != sVGA_Bmp0_BD0BD0.field_8_width || height != sVGA_Bmp0_BD0BD0.field_C_height)
        {
            if (sVGA_Bmp0_BD0BD0.field_0_pSurface)
            {
                Bmp_Free_4F1950(&sVGA_Bmp0_BD0BD0);
                bpp = sVGA_bmp_primary_BD2A20.field_14_bpp;
            }

            switch (bpp)
            {
            case 8:
                v7 = 8;
                break;
            case 24:
                v7 = 24;
                break;
            case 32:
                v7 = 32;
                break;
            default:
                //v7 = v42;
                break;
            }

            if (BMP_New_4F1990(&sVGA_Bmp0_BD0BD0, srcWidth, height, v7, 1))
            {
                Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\VGA.C", 452, -1, "VGA_CopyToFront: BMP_New FAILED!");
                return;
            }
            bpp = sVGA_bmp_primary_BD2A20.field_14_bpp;
        }

        if (bpp == 8)
        {
            // TODO: Mode not supported
        }

        if (bpp != 24 && bpp != 32)
        {
            goto LABEL_87;
        }

        if (!BMP_Lock_4F1FF0(pBitmapToUse))
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\VGA.C", 546, -1, "VGA_CopyToFront: BMP_LockPtr 1 FAILED!");
            return;
        }

        if (!BMP_Lock_4F1FF0(&sVGA_Bmp0_BD0BD0))
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\VGA.C", 552, -1, "VGA_CopyToFront: BMP_LockPtr 2 FAILED!");
            BMP_unlock_4F2100(pBitmapToUse);
            return;
        }

        v50 = ((unsigned int)pBitmapToUse->field_10_locked_pitch >> 1) - srcWidth;
        v22 = (unsigned __int16 *)((char *)pBitmapToUse->field_4_pLockedPixels
            + 2 * (srcX + ((unsigned int)(srcY * pBitmapToUse->field_10_locked_pitch) >> 1)));

        if (sVGA_bmp_primary_BD2A20.field_14_bpp == 32)
        {
            v40 = 4 * srcWidth2;
        }
        else
        {
            v40 = 3 * srcWidth2;
        }

        v23 = (char *)sVGA_Bmp0_BD0BD0.field_4_pLockedPixels;
        v24 = sVGA_Bmp0_BD0BD0.field_10_locked_pitch - v40;
        v25 = (char *)sVGA_Bmp0_BD0BD0.field_4_pLockedPixels + height * sVGA_Bmp0_BD0BD0.field_10_locked_pitch;
        v45 = sVGA_Bmp0_BD0BD0.field_10_locked_pitch - v40;
        v52 = (char *)sVGA_Bmp0_BD0BD0.field_4_pLockedPixels + height * sVGA_Bmp0_BD0BD0.field_10_locked_pitch;
        if (sVGA_bmp_primary_BD2A20.field_14_bpp == 32)
        {
            if (pBitmapToUse->field_15_pixel_format == 15)
            {
                v47 = 9;
                v37 = 6;
            }
            else
            {
                if (pBitmapToUse->field_15_pixel_format != 16)
                {
                    v47 = 0;
                    v37 = 0;
                    v34 = 0;
                    goto LABEL_68;
                }
                v47 = 8;
                v37 = 5;
            }
            v34 = 3;
        LABEL_68:
            if (sVGA_Bmp0_BD0BD0.field_4_pLockedPixels < v25)
            {
                do
                {
                    v26 = (unsigned int)&v23[v40];
                    if (v23 < &v23[v40])
                    {
                        do
                        {
                            v27 = *v22;
                            v23 += 4;
                            v28 = *v22 << v34;
                            ++v22;
                            *((DWORD *)v23 - 1) = (v27 << v37) & 0xFF00 | (v27 << v47) & 0xFF0000 | (unsigned __int8)v28;
                        } while ((unsigned int)v23 < v26);
                        v24 = v45;
                        v25 = v52;
                        pBitmapToUse = pBmp;
                    }
                    v23 += v24;
                    v22 += v50;
                } while (v23 < v25);
            }
            goto LABEL_86;
        }
        if (pBitmapToUse->field_15_pixel_format == 15)
        {
            v29 = 8;
            v30 = 3;
        }
        else
        {
            if (pBitmapToUse->field_15_pixel_format == 16)
            {
                v29 = 7;
                v38 = 2;
                v35 = 3;
                goto LABEL_80;
            }
            v29 = 0;
            v30 = 0;
        }
        v38 = v30;
        v35 = v30;
    LABEL_80:
        if (sVGA_Bmp0_BD0BD0.field_4_pLockedPixels < v25)
        {
            do
            {
                v31 = (unsigned int)&v23[v40];
                if (v23 < &v23[v40])
                {
                    do
                    {
                        v32 = *v22;
                        v23 += 3;
                        ++v22;
                        *(v23 - 3) = static_cast<char>((unsigned int)v32 >> v29);
                        *(v23 - 2) = static_cast<char>((unsigned int)v32 >> v38);
                        *(v23 - 1) = (BYTE)v32 << v35;
                    } while ((unsigned int)v23 < v31);
                    v24 = v45;
                    v25 = v52;
                }
                v23 += v24;
                v22 += v50;
            } while (v23 < v25);
        }
        pBitmapToUse = pBmp;
    LABEL_86:
        srcWidth = srcWidth2;
    LABEL_87:
        BMP_unlock_4F2100(&sVGA_Bmp0_BD0BD0);
        BMP_unlock_4F2100(pBitmapToUse);
        rect.left = 0;
        rect.top = 0;
        rect.bottom = height;
        rect.right = srcWidth;
        DD_render_back_buffer_4F0D90(sVGA_Bmp0_BD0BD0.field_0_pSurface, &rect, screenMode);
        return;
    }
}

EXPORT void CC VGA_CopyToFront_4F3EB0(Bitmap* pBmp, RECT* pRect, unsigned __int8 screenMode)
{
    VGA_CopyToFront_4F3730(pBmp, pRect, screenMode);
}

EXPORT void CC VGA_CopyToFront_4F3710(Bitmap* pBmp, RECT* pRect)
{
    VGA_CopyToFront_4F3730(pBmp, pRect, 0);
}

EXPORT signed int CC VGA_DisplaySet_4F32C0(unsigned __int16 width, unsigned __int16 height, unsigned __int8 bpp, unsigned __int8 backbufferCount, TSurfaceType** ppSurface)
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
            sDD_primary_surface_BBC3C8 = *ppSurface;
            sDD_surface_backbuffer_BBC3CC = *ppSurface;
            backbufferCount = 1;
        }
        else
        {
            sVGA_own_surfaces_BD0BFA = true;
        }
        
        sVGA_BD0BBC = backbufferCount;
        sVGA_bpp_BD0BF9 = bpp;
        memset(&sVGA_bmp_primary_BD2A20, 0, sizeof(sVGA_bmp_primary_BD2A20));
        memset(&sVGA_bmp_back_buffer_BD2A40, 0, sizeof(sVGA_bmp_back_buffer_BD2A40));
        sVGA_height_BD0BEC = height;
        sVGA_width_BD0BC4 = width;

        if (DD_Enable_4F0380(Sys_GetHWnd_4F2C70(), width, height, bpp, backbufferCount, 0))
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

            if (!sVGA_own_surfaces_BD0BFA || DD_Init_4F0840(backbufferCount))
            {
                DDSURFACEDESC surfaceDesc = {};
                surfaceDesc.dwSize = sizeof(DDSURFACEDESC);
                if (sDD_primary_surface_BBC3C8->GetSurfaceDesc(&surfaceDesc))
                {
                    Error_PushErrorRecord_4F2920(
                        "C:\\abe2\\code\\POS\\VGA.C",
                        368,
                        -1,
                        "VGA_DisplaySet: problem calling IDirectDrawSurface_GetSurfaceDesc");
                }
                else
                {
                    sVGA_bmp_primary_BD2A20.field_0_pSurface = sDD_primary_surface_BBC3C8;
                    sVGA_bmp_primary_BD2A20.field_8_width = surfaceDesc.dwWidth;
                    sVGA_bmp_primary_BD2A20.field_10_locked_pitch = surfaceDesc.lPitch;
                    sVGA_bmp_primary_BD2A20.field_C_height = surfaceDesc.dwHeight;
                    sVGA_bmp_primary_BD2A20.field_14_bpp = static_cast<char>(surfaceDesc.ddpfPixelFormat.dwRGBBitCount);
                    sVGA_bmp_primary_BD2A20.field_18_create_flags = 2;
                    memcpy(&sVGA_bmp_back_buffer_BD2A40, &sVGA_bmp_primary_BD2A20, sizeof(sVGA_bmp_back_buffer_BD2A40));
                    sVGA_bmp_back_buffer_BD2A40.field_0_pSurface = sDD_surface_backbuffer_BBC3CC;
                    sVGA_Inited_BC0BB8 = 1;

                    // TODO: Refactor to own function, change if/else chain to early outs
                    switch (surfaceDesc.ddpfPixelFormat.dwRGBBitCount)
                    {
                    case 1u:
                        sVGA_bmp_primary_BD2A20.field_15_pixel_format = 1;
                        return 0;
                    case 2u:
                        sVGA_bmp_primary_BD2A20.field_15_pixel_format = 2;
                        return 0;
                    case 4u:
                        sVGA_bmp_primary_BD2A20.field_15_pixel_format = 4;
                        return 0;
                    case 8u:
                        sVGA_bmp_primary_BD2A20.field_15_pixel_format = 8;
                        return 0;
                    case 16u:
                        if (surfaceDesc.ddpfPixelFormat.dwRBitMask == 63488)
                        {
                            if (surfaceDesc.ddpfPixelFormat.dwGBitMask == 2016 && surfaceDesc.ddpfPixelFormat.dwBBitMask == 31)
                            {
                                sVGA_bmp_primary_BD2A20.field_15_pixel_format = 16;
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
                                sVGA_bmp_primary_BD2A20.field_15_pixel_format = 116;
                                return 0;
                            }
                        }
                        else
                        {
                            if (surfaceDesc.ddpfPixelFormat.dwRBitMask == 31744)
                            {
                                if (surfaceDesc.ddpfPixelFormat.dwGBitMask == 992 && surfaceDesc.ddpfPixelFormat.dwBBitMask == 31)
                                {
                                    sVGA_bmp_primary_BD2A20.field_15_pixel_format = 15;
                                    return 0;
                                }
                                break;
                            }
                            if (surfaceDesc.ddpfPixelFormat.dwRBitMask != 31)
                                break;
                        }
                        if (surfaceDesc.ddpfPixelFormat.dwGBitMask == 992 && surfaceDesc.ddpfPixelFormat.dwBBitMask == 31744)
                        {
                            sVGA_bmp_primary_BD2A20.field_15_pixel_format = 115;
                            return 0;
                        }
                        break;
                    case 24u:
                        sVGA_bmp_primary_BD2A20.field_15_pixel_format = 24;
                        return 0;
                    case 32u:
                        sVGA_bmp_primary_BD2A20.field_15_pixel_format = 32;
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

EXPORT void VGA_BuffUnlockPtr_4F2FB0()
{
    if (sbVga_LockedType_BD0BF0)
    {
        Bitmap* pBmpToUnlock = &sVGA_bmp_primary_BD2A20;
        if (sVga_LockPType_BD0BC0 != 3)
        {
            pBmpToUnlock = &sVGA_bmp_back_buffer_BD2A40;
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
            pLockedBuffer = BMP_Lock_4F1FF0(&sVGA_bmp_primary_BD2A20);
        }
        else
        {
            if (always3 != 4)
            {
                Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\VGA.C", 179, -1, "BuffLockPtr wrong PTYPE");
                return 0;
            }
            pLockedBuffer = BMP_Lock_4F1FF0(&sVGA_bmp_back_buffer_BD2A40);
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

#endif
