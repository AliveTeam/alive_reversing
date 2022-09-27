#include "stdafx.h"
#include "bmp.hpp"
#include "Error.hpp"
#include "../AliveLibCommon/Sys_common.hpp"

ALIVE_VAR(1, 0xBC0BB4, u8, gVGA_force_sys_memory_surfaces_BC0BB4, 0);

#if USE_SDL2

s32 BMP_Blt_4F1E50(Bitmap* pDstBmp, s32 xPos, s32 yPos, Bitmap* pSrcBmp, LPRECT pRect, s32 flags)
{
    if (flags != 0)
    {
        ALIVE_FATAL("Flags not supported");
    }

    SDL_Rect destRect = {xPos, yPos, 0, 0};
    const SDL_Rect srcRect = {pRect->left, pRect->right, pRect->top, pRect->bottom};
    if (SDL_BlitSurface(pSrcBmp->field_0_pSurface, &srcRect, pDstBmp->field_0_pSurface, &destRect) != 0)
    {
        return 0;
    }
    return -1;
}

s32 BMP_ClearRect(Bitmap* pBmp, const RECT* pRect, u32 fillColour)
{
    const SDL_Rect clearRect = {pRect->left, pRect->right, pRect->top, pRect->bottom};
    if (SDL_FillRect(pBmp->field_0_pSurface, &clearRect, fillColour /*SDL_MapRGB(pBmp->field_0_pSurface->format, 255, 0, 0)*/) != 0)
    {
        return -1;
    }
    return 0;
}

void BMP_unlock_4F2100(Bitmap* pBmp)
{
    if (pBmp->field_4_pLockedPixels && pBmp->field_0_pSurface)
    {
        SDL_UnlockSurface(pBmp->field_0_pSurface);
        pBmp->field_4_pLockedPixels = nullptr;
    }
}

void BMP_Release_DC_4F21A0(Bitmap* pBmp, HDC hdc)
{
    if (pBmp && hdc)
    {
        // No impl required
    }
}

HDC BMP_Get_DC_4F2150(Bitmap* /*pBmp*/)
{
    // No impl required
    return nullptr;
}

LONG BMP_Get_Font_Height_4F21F0(Bitmap* /*pBmp*/)
{
    return 10;
}

s32 BMP_New_convert_BPP_4F1CC0(s32 bpp)
{
    s32 converted = 0;
    switch (bpp)
    {
        case 1:
            converted = 1;
            break;
        case 2:
            converted = 2;
            break;
        case 4:
            converted = 4;
            break;
        case 8:
            converted = 8;
            break;
        case 15:
        case 16:
        case 115:
        case 116:
            converted = 16;
            break;
        case 24:
            converted = 24;
            break;
        case 32:
            converted = 32;
            break;
        default:
            converted = -1;
            break;
    }
    return converted;
}

s32 Bmp_Convert_Colour_4F17D0(Bitmap* pBmp, s32 r, s32 g, s32 b)
{
    s32 converted = 0;
    switch (pBmp->field_15_pixel_format)
    {
        case 0xF:
            converted = (b >> 3) | 4 * ((g & 0xF8) | 32 * (r & 0xF8));
            break;
        case 0x10:
            converted = (b >> 3) | 8 * ((g & 0xFC) | 32 * (r & 0xF8));
            break;
        case 0x18:
        case 0x20:
            converted = b | ((g | (r << 8)) << 8);
            break;
        case 0x73:
            converted = (r >> 3) | 4 * ((g & 0xF8) | 32 * (b & 0xF8));
            break;
        case 0x74:
            converted = (r >> 3) | 8 * ((g & 0xFC) | 32 * (b & 0xF8));
            break;
        case 0x7C:
        case 0x84:
            converted = r | ((g | (b << 8)) << 8);
            break;
        default:
            converted = 0;
            break;
    }
    return converted;
}

void Bmp_Free_4F1950(Bitmap* pBmp)
{
    if (pBmp && pBmp->field_0_pSurface)
    {
        if (pBmp->field_4_pLockedPixels)
        {
            BMP_unlock_4F2100(pBmp);
        }
        SDL_FreeSurface(pBmp->field_0_pSurface);
        memset(pBmp, 0, sizeof(Bitmap));
    }
}

s32 BMP_New_4F1990(Bitmap* pBitmap, s32 width, s32 height, s32 pixelFormat, s32 createFlags)
{
    if (!pBitmap || !width || !height)
    {
        return -1;
    }

    memset(pBitmap, 0, sizeof(Bitmap));

    const s32 bpp = BMP_New_convert_BPP_4F1CC0(pixelFormat);
    if (bpp < 0)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 170, -1, "BMP_New: bits per pixel not supported");
        return -1;
    }

    u32 bMask = 0;
    u32 rMask = 0;
    u32 gMask = 0;

    switch (bpp)
    {
        case 15:
            rMask = 0x7C00;
            gMask = 0x3E0;
            bMask = 0x1F;
            break;
        case 16:
            rMask = 0xF800;
            gMask = 0x7E0;
            bMask = 0x1F;
            break;
        case 24:
        case 32:
            rMask = 0xFF0000;
            gMask = 0xFF00;
            bMask = 0xFF;
            break;
        case 115:
            rMask = 0x1F;
            gMask = 0x3E0;
            bMask = 0x7C00;
            break;
        case 116:
            rMask = 0x1F;
            gMask = 0x7E0;
            bMask = 0xF800;
            break;
        default:
            ALIVE_FATAL("bbp not supported");
    }

    pBitmap->field_0_pSurface = SDL_CreateRGBSurface(0, width, height, bpp, rMask, gMask, bMask, 0x0);
    pBitmap->field_C_height = height;
    pBitmap->field_8_width = width;
    pBitmap->field_14_bpp = static_cast<s8>(bpp);
    SDL_LockSurface(pBitmap->field_0_pSurface);
    pBitmap->field_10_locked_pitch = pBitmap->field_0_pSurface->pitch;
    SDL_UnlockSurface(pBitmap->field_0_pSurface);
    pBitmap->field_15_pixel_format = static_cast<s8>(pixelFormat);
    pBitmap->field_18_create_flags = createFlags;

    return 0;
}

LPVOID BMP_Lock_4F1FF0(Bitmap* pBitmap)
{
    // Already locked or we don't have a surface
    if (pBitmap->field_4_pLockedPixels || !pBitmap->field_0_pSurface)
    {
        return pBitmap->field_4_pLockedPixels;
    }

    SDL_LockSurface(pBitmap->field_0_pSurface);
    pBitmap->field_10_locked_pitch = pBitmap->field_0_pSurface->pitch;
    pBitmap->field_4_pLockedPixels = pBitmap->field_0_pSurface->pixels;
    return pBitmap->field_4_pLockedPixels;
}

const u8 sBitmapFont[] = {
    0x42,
    0x4D,
    0x00,
    0x04,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x3E,
    0x00,
    0x00,
    0x00,
    0x28,
    0x00,
    0x00,
    0x00,
    0xF1,
    0x02,
    0x00,
    0x00,
    0x0A,
    0x00,
    0x00,
    0x00,
    0x01,
    0x00,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xC2,
    0x03,
    0x00,
    0x00,
    0x12,
    0x0B,
    0x00,
    0x00,
    0x12,
    0x0B,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x80,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xF7,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xF7,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xF9,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x80,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xE3,
    0xFF,
    0xFF,
    0xCF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xEF,
    0xFB,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xE3,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xC9,
    0x00,
    0xF7,
    0xFF,
    0xEB,
    0xF7,
    0xFF,
    0xC5,
    0xFF,
    0xFB,
    0xEF,
    0xF7,
    0xF7,
    0xFB,
    0xFF,
    0xF3,
    0xBF,
    0xE3,
    0xE3,
    0xC1,
    0xE3,
    0xF1,
    0xE3,
    0xE3,
    0xEF,
    0xE3,
    0xE3,
    0xFF,
    0xFB,
    0xFB,
    0xFF,
    0xDF,
    0xF7,
    0xE3,
    0xDD,
    0xC3,
    0xE3,
    0xC3,
    0xC1,
    0xDF,
    0xE3,
    0xDD,
    0xE3,
    0xE7,
    0xDD,
    0xE1,
    0xBE,
    0xDD,
    0xE3,
    0xEF,
    0xE3,
    0xDD,
    0xE3,
    0xF7,
    0xE3,
    0xF7,
    0xEB,
    0xDD,
    0xF7,
    0xC1,
    0xE3,
    0xFE,
    0xE3,
    0xFF,
    0xFF,
    0xFF,
    0xE2,
    0xD3,
    0xE3,
    0xF2,
    0xE3,
    0xEF,
    0xFD,
    0xDD,
    0xF7,
    0xF7,
    0xDB,
    0xF7,
    0xBE,
    0xED,
    0xE3,
    0xEF,
    0xFB,
    0xDF,
    0xC7,
    0xF7,
    0xE5,
    0xF7,
    0xDD,
    0xDD,
    0xFD,
    0xC3,
    0xF3,
    0xF7,
    0xCF,
    0xFF,
    0xCD,
    0x00,
    0xFF,
    0xFF,
    0xEB,
    0xC3,
    0xD9,
    0xBB,
    0xFF,
    0xF7,
    0xF7,
    0xB6,
    0xF7,
    0xF3,
    0xFF,
    0xF3,
    0xDF,
    0xDD,
    0xF7,
    0xEF,
    0xDD,
    0xFB,
    0xDD,
    0xDD,
    0xEF,
    0xDD,
    0xDD,
    0xF3,
    0xF3,
    0xF7,
    0xFF,
    0xEF,
    0xFF,
    0xDF,
    0xDD,
    0xDD,
    0xDD,
    0xDD,
    0xDF,
    0xDF,
    0xDD,
    0xDD,
    0xF7,
    0xDB,
    0xDD,
    0xEF,
    0xBE,
    0xDD,
    0xDD,
    0xEF,
    0xDD,
    0xDB,
    0xDD,
    0xF7,
    0xDD,
    0xF7,
    0xEB,
    0xDD,
    0xF7,
    0xDF,
    0xEF,
    0xFD,
    0xFB,
    0xFF,
    0xFF,
    0xFF,
    0xDD,
    0xED,
    0xDF,
    0xED,
    0xDF,
    0xEF,
    0xE1,
    0xDD,
    0xF7,
    0xF7,
    0xD7,
    0xF7,
    0xBE,
    0xED,
    0xDD,
    0xE3,
    0xE3,
    0xDF,
    0xFB,
    0xF7,
    0xDB,
    0xEB,
    0xAA,
    0xEB,
    0xF1,
    0xEF,
    0xEF,
    0xF7,
    0xF7,
    0xFF,
    0x80,
    0x00,
    0xF7,
    0xFF,
    0x80,
    0xF5,
    0xE9,
    0xB9,
    0xFF,
    0xEF,
    0xFB,
    0xD5,
    0xF7,
    0xF3,
    0xFF,
    0xFF,
    0xEF,
    0xDD,
    0xF7,
    0xF7,
    0xFD,
    0xFB,
    0xFD,
    0xDD,
    0xEF,
    0xDD,
    0xFD,
    0xF3,
    0xF3,
    0xEF,
    0x80,
    0xF7,
    0xF7,
    0xD1,
    0xDD,
    0xDD,
    0xDF,
    0xDD,
    0xDF,
    0xDF,
    0xDD,
    0xDD,
    0xF7,
    0xDB,
    0xDB,
    0xEF,
    0xBE,
    0xD9,
    0xDD,
    0xEF,
    0xDD,
    0xD7,
    0xFD,
    0xF7,
    0xDD,
    0xEB,
    0xD5,
    0xEB,
    0xF7,
    0xEF,
    0xEF,
    0xFB,
    0xFB,
    0xFF,
    0xFF,
    0xFF,
    0xDD,
    0xED,
    0xDF,
    0xED,
    0xC1,
    0xEF,
    0xDD,
    0xDD,
    0xF7,
    0xF7,
    0xCF,
    0xF7,
    0xB6,
    0xED,
    0xDD,
    0xED,
    0xDB,
    0xDF,
    0xE7,
    0xF7,
    0xDB,
    0xDD,
    0xB6,
    0xF7,
    0xED,
    0xF7,
    0xEF,
    0xF7,
    0xF7,
    0xF9,
    0x80,
    0x00,
    0xF7,
    0xFF,
    0xEB,
    0xE3,
    0xF7,
    0xD7,
    0xFF,
    0xEF,
    0xFB,
    0xE3,
    0x80,
    0xFF,
    0x80,
    0xFF,
    0xF7,
    0xD5,
    0xF7,
    0xFB,
    0xF3,
    0xC1,
    0xC3,
    0xC3,
    0xF7,
    0xE3,
    0xE1,
    0xFF,
    0xFF,
    0xDF,
    0xFF,
    0xFB,
    0xFB,
    0xD5,
    0xC1,
    0xC3,
    0xDF,
    0xDD,
    0xC3,
    0xC1,
    0xD1,
    0xC1,
    0xF7,
    0xFB,
    0xC7,
    0xEF,
    0xB6,
    0xD5,
    0xDD,
    0xE3,
    0xDD,
    0xC3,
    0xE3,
    0xF7,
    0xDD,
    0xEB,
    0xD5,
    0xF7,
    0xF7,
    0xF7,
    0xEF,
    0xF7,
    0xFB,
    0xFF,
    0xFF,
    0xFF,
    0xE1,
    0xED,
    0xDF,
    0xED,
    0xDD,
    0xC7,
    0xDD,
    0xCD,
    0xF7,
    0xF7,
    0xD7,
    0xF7,
    0xB6,
    0xED,
    0xDD,
    0xED,
    0xDB,
    0xCF,
    0xDF,
    0xF7,
    0xDB,
    0xDD,
    0xBE,
    0xEB,
    0xED,
    0xFB,
    0xDF,
    0xF7,
    0xFB,
    0xB6,
    0x80,
    0x00,
    0xF7,
    0xFF,
    0x80,
    0xD7,
    0xCB,
    0xEF,
    0xFF,
    0xEF,
    0xFB,
    0xD5,
    0xF7,
    0xFF,
    0xFF,
    0xFF,
    0xFB,
    0xDD,
    0xF7,
    0xFD,
    0xFD,
    0xDB,
    0xDF,
    0xDF,
    0xFB,
    0xDD,
    0xDD,
    0xF3,
    0xF3,
    0xEF,
    0x80,
    0xF7,
    0xFD,
    0xD1,
    0xDD,
    0xDD,
    0xDF,
    0xDD,
    0xDF,
    0xDF,
    0xDF,
    0xDD,
    0xF7,
    0xFB,
    0xDB,
    0xEF,
    0xAA,
    0xD5,
    0xDD,
    0xED,
    0xDD,
    0xDD,
    0xDF,
    0xF7,
    0xDD,
    0xDD,
    0xBE,
    0xEB,
    0xEB,
    0xFB,
    0xEF,
    0xEF,
    0xFB,
    0xDD,
    0xFF,
    0xFF,
    0xFD,
    0xE3,
    0xE3,
    0xF1,
    0xE3,
    0xEF,
    0xE2,
    0xD3,
    0xFF,
    0xFF,
    0xDB,
    0xF7,
    0x49,
    0xD3,
    0xE3,
    0xD3,
    0xE5,
    0xD3,
    0xE3,
    0xE3,
    0xDB,
    0xDD,
    0xBE,
    0xDD,
    0xED,
    0xC3,
    0xEF,
    0xF7,
    0xF7,
    0xCF,
    0x80,
    0x00,
    0xF7,
    0xEB,
    0xEB,
    0xE1,
    0xCD,
    0xD7,
    0xF7,
    0xF7,
    0xF7,
    0xB6,
    0xF7,
    0xFF,
    0xFF,
    0xFF,
    0xFD,
    0xDD,
    0xE7,
    0xDD,
    0xDD,
    0xEB,
    0xDF,
    0xDD,
    0xFD,
    0xDD,
    0xDD,
    0xF3,
    0xF3,
    0xF7,
    0xFF,
    0xEF,
    0xDD,
    0xDD,
    0xDD,
    0xDD,
    0xDD,
    0xDD,
    0xDF,
    0xDF,
    0xDD,
    0xDD,
    0xF7,
    0xFB,
    0xDD,
    0xEF,
    0x9C,
    0xCD,
    0xDD,
    0xED,
    0xDD,
    0xDD,
    0xDD,
    0xF7,
    0xDD,
    0xDD,
    0xBE,
    0xDD,
    0xDD,
    0xFD,
    0xEF,
    0xDF,
    0xFB,
    0xEB,
    0xFF,
    0xF7,
    0xE3,
    0xEF,
    0xFF,
    0xFD,
    0xFF,
    0xED,
    0xFF,
    0xDF,
    0xF7,
    0xF7,
    0xDF,
    0xF7,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xF7,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xEF,
    0xF7,
    0xF7,
    0xFF,
    0x80,
    0x00,
    0xF7,
    0xEB,
    0xEB,
    0xF7,
    0xFF,
    0xE7,
    0xF7,
    0xFB,
    0xEF,
    0xF7,
    0xF7,
    0xFF,
    0xFF,
    0xFF,
    0xFE,
    0xE3,
    0xF7,
    0xE3,
    0xE3,
    0xF3,
    0xC1,
    0xE3,
    0xC1,
    0xE3,
    0xE3,
    0xFF,
    0xFF,
    0xFB,
    0xFF,
    0xDF,
    0xE3,
    0xE3,
    0xE3,
    0xC3,
    0xE3,
    0xC3,
    0xC1,
    0xC1,
    0xE3,
    0xDD,
    0xE3,
    0xF1,
    0xDD,
    0xEF,
    0xBE,
    0xDD,
    0xE3,
    0xE3,
    0xE3,
    0xC3,
    0xE3,
    0xC1,
    0xDD,
    0xDD,
    0xBE,
    0xDD,
    0xDD,
    0xC1,
    0xE3,
    0xBF,
    0xE3,
    0xF7,
    0xFF,
    0xEF,
    0xFF,
    0xEF,
    0xFF,
    0xFD,
    0xFF,
    0xF3,
    0xFF,
    0xDF,
    0xFF,
    0xFF,
    0xDF,
    0xE7,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xF3,
    0xF7,
    0xCF,
    0xFF,
    0x80,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x80,
    0x00,
    0x00,
    0x00,
};

void BMP_Draw_String_4F2230(Bitmap* pBmp, s32 x, s32 y, u32 /*fgColour*/, s32 /*bgColour*/, LPCSTR lpString)
{
    SDL_Surface* pFontBmp = SDL_LoadBMP_RW(SDL_RWFromConstMem(sBitmapFont, sizeof(sBitmapFont)), SDL_TRUE);

    s32 xpos = 0;
    const s32 kLetterWidth = 8;
    const s32 kLetterHeight = 10;
    for (u32 i = 0; i < strlen(lpString); i++)
    {
        s8 letter = lpString[i];

        SDL_Rect dstRect = {x + xpos, y, kLetterWidth, kLetterHeight};

        // Don't do anything with spaces
        if (letter == ' ')
        {
            xpos += kLetterWidth;
            SDL_FillRect(pBmp->field_0_pSurface, &dstRect, 0x0000);
            continue;
        }

        // And don't do anything with chars not in the atlas
        if (letter < '!')
        {
            continue;
        }
        else if (letter >= 'z')
        {
            continue;
        }

        // Re-base to zero
        letter = letter - '!';

        SDL_Rect srcRect = {kLetterWidth * letter, 0, kLetterWidth, kLetterHeight};

        if (SDL_BlitSurface(pFontBmp, &srcRect, pBmp->field_0_pSurface, &dstRect) != 0)
        {
            const char_type* e = SDL_GetError();
            LOG_ERROR(e);
            abort();
        }
        xpos += kLetterWidth;
    }

    SDL_FreeSurface(pFontBmp);
}

#else
HRESULT BMP_New_create_surface_4F1C60(DDSURFACEDESC* pSurfaceDesc, LPDIRECTDRAWSURFACE* ppSurface)
{
    *ppSurface = nullptr;

    HRESULT hr = sDDraw_BBC3D4->CreateSurface(pSurfaceDesc, ppSurface, nullptr);

    if (hr == DDERR_INVALIDPARAMS)
    {
        const u32 oldCaps = pSurfaceDesc->ddsCaps.dwCaps;
        if (!(oldCaps & DDSCAPS_SYSTEMMEMORY))
        {
            const u32 tempCaps = (pSurfaceDesc->ddsCaps.dwCaps & ~DDSCAPS_VIDEOMEMORY) | DDSCAPS_SYSTEMMEMORY;
            pSurfaceDesc->ddsCaps.dwCaps = tempCaps;
            hr = sDDraw_BBC3D4->CreateSurface(pSurfaceDesc, ppSurface, nullptr);
            pSurfaceDesc->ddsCaps.dwCaps = oldCaps;
        }
    }
    return hr;
}

s32 BMP_Blt_4F1E50(Bitmap* pDstBmp, s32 xPos, s32 yPos, Bitmap* pSrcBmp, LPRECT pRect, s32 flags)
{
    HRESULT hr = S_OK;
    bool addError = true;
    for (;;)
    {
        hr = pDstBmp->field_0_pSurface->BltFast(
            xPos,
            yPos,
            pSrcBmp->field_0_pSurface,
            pRect,
            flags);

        if (SUCCEEDED(hr))
        {
            return 0;
        }

        if (hr == DDERR_SURFACELOST)
        {
            if (FAILED(pSrcBmp->field_0_pSurface->Restore()) && FAILED(pDstBmp->field_0_pSurface->Restore()))
            {
                addError = true;
            }
            else
            {
                addError = false;
            }
            break;
        }

        if (hr != DDERR_WASSTILLDRAWING && hr != DDERR_WRONGMODE)
        {
            addError = true;
            break;
        }
    }

    if (addError)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 307, -1, DX_HR_To_String_4F4EC0(hr));
    }
    return -1;
}

ALIVE_VAR(1, 0xBBC458, DDBLTFX, blt_fx_stru_BBC458, {});

s32 BMP_ClearRect(Bitmap* pBmp, const RECT* pRect, u32 fillColour)
{
    RECT rect = *pRect;

    if (rect.left < 0)
    {
        rect.left = 0;
    }

    if (rect.left > pBmp->field_8_width)
    {
        rect.left = pBmp->field_8_width;
    }

    if (rect.right < pRect->left)
    {
        rect.right = pRect->left;
    }

    if (rect.right > pBmp->field_C_height)
    {
        rect.right = pBmp->field_C_height;
    }

    if (rect.top < 0)
    {
        rect.top = 0;
    }

    if (rect.top > pBmp->field_C_height)
    {
        rect.top = pBmp->field_C_height;
    }

    if (rect.bottom < rect.top)
    {
        rect.bottom = rect.top;
    }

    if (rect.bottom > pBmp->field_C_height)
    {
        rect.bottom = pBmp->field_C_height;
    }

    blt_fx_stru_BBC458.dwSize = sizeof(DDBLTFX);
    blt_fx_stru_BBC458.dwFillColor = fillColour;

    HRESULT hr = S_OK;
    do
    {
        hr = pBmp->field_0_pSurface->Blt(&rect, 0, &rect, DDBLT_COLORFILL | DDBLT_WAIT, &blt_fx_stru_BBC458);
        if (FAILED(hr))
        {
            if (hr != DDERR_SURFACELOST)
            {
                continue;
            }

            hr = pBmp->field_0_pSurface->Restore();

            if (FAILED(hr))
            {
                continue;
            }
        }
        return 0;
    }
    while (hr == DDERR_WASSTILLDRAWING || hr == DDERR_WRONGMODE);

    Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 340, -1, DX_HR_To_String_4F4EC0(hr));
    return -1;
}

void BMP_unlock_4F2100(Bitmap* pBmp)
{
    if (pBmp->field_4_pLockedPixels && pBmp->field_0_pSurface)
    {
        const HRESULT hr = pBmp->field_0_pSurface->Unlock(pBmp->field_4_pLockedPixels);
        if (FAILED(hr))
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 393, -1, DX_HR_To_String_4F4EC0(hr));
        }
        pBmp->field_4_pLockedPixels = nullptr;
    }
}

void BMP_Release_DC_4F21A0(Bitmap* pBmp, HDC hdc)
{
    if (pBmp && hdc)
    {
        const HRESULT hr = pBmp->field_0_pSurface->ReleaseDC(hdc);
        if (FAILED(hr))
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 431, -1, DX_HR_To_String_4F4EC0(hr));
        }
    }
}

HDC BMP_Get_DC_4F2150(Bitmap* pBmp)
{
    if (!pBmp || pBmp->field_4_pLockedPixels || !pBmp->field_0_pSurface)
    {
        return nullptr;
    }

    HDC dc = nullptr;
    const HRESULT hr = pBmp->field_0_pSurface->GetDC(&dc);
    if (FAILED(hr))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 412, -1, DX_HR_To_String_4F4EC0(hr));
        return nullptr;
    }

    return dc;
}

LONG BMP_Get_Font_Height_4F21F0(Bitmap* pBmp)
{
    HDC dc = BMP_Get_DC_4F2150(pBmp);
    TEXTMETRICA tm = {};
    GetTextMetricsA(dc, &tm);
    BMP_Release_DC_4F21A0(pBmp, dc);

    // Invert if negative
    LONG textHeight = tm.tmHeight;
    if (tm.tmHeight < 0)
    {
        textHeight = -tm.tmHeight;
    }

    return textHeight;
}

s32 BMP_New_convert_BPP_4F1CC0(s32 bpp)
{
    s32 converted = 0;
    switch (bpp)
    {
        case 1:
            converted = 1;
            break;
        case 2:
            converted = 2;
            break;
        case 4:
            converted = 4;
            break;
        case 8:
            converted = 8;
            break;
        case 15:
        case 16:
        case 115:
        case 116:
            converted = 16;
            break;
        case 24:
            converted = 24;
            break;
        case 32:
            converted = 32;
            break;
        default:
            converted = -1;
            break;
    }
    return converted;
}

s32 Bmp_Convert_Colour_4F17D0(Bitmap* pBmp, s32 r, s32 g, s32 b)
{
    s32 converted = 0;
    switch (pBmp->field_15_pixel_format)
    {
        case 0xF:
            converted = (b >> 3) | 4 * (g & 0xF8 | 32 * (r & 0xF8));
            break;
        case 0x10:
            converted = (b >> 3) | 8 * (g & 0xFC | 32 * (r & 0xF8));
            break;
        case 0x18:
        case 0x20:
            converted = b | ((g | (r << 8)) << 8);
            break;
        case 0x73:
            converted = (r >> 3) | 4 * (g & 0xF8 | 32 * (b & 0xF8));
            break;
        case 0x74:
            converted = (r >> 3) | 8 * (g & 0xFC | 32 * (b & 0xF8));
            break;
        case 0x7C:
        case 0x84:
            converted = r | ((g | (b << 8)) << 8);
            break;
        default:
            converted = 0;
            break;
    }
    return converted;
}

void Bmp_Free_4F1950(Bitmap* pBmp)
{
    if (pBmp && pBmp->field_0_pSurface)
    {
        if (pBmp->field_4_pLockedPixels)
        {
            BMP_unlock_4F2100(pBmp);
        }
        pBmp->field_0_pSurface->Release();
        memset(pBmp, 0, sizeof(Bitmap));
    }
}

s32 BMP_New_4F1990(Bitmap* pBitmap, s32 width, s32 height, s32 pixelFormat, s32 createFlags)
{
    if (!pBitmap || !width || !height)
    {
        return -1;
    }

    memset(pBitmap, 0, sizeof(Bitmap));

    const s32 bpp = BMP_New_convert_BPP_4F1CC0(pixelFormat);
    if (bpp < 0)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 170, -1, "BMP_New: bits per pixel not supported");
        return -1;
    }

    u32 bMask = 0;
    u32 rMask = 0;
    u32 gMask = 0;
    u32 flags = 0;

    switch (bpp)
    {
        case 1:
            flags = DDPF_PALETTEINDEXED1; // 0x800
            break;
        case 2:
            flags = DDPF_PALETTEINDEXED2; // 0x1000
            break;
        case 4:
            flags = DDPF_PALETTEINDEXED4; // 0x8
            break;
        case 8:
            flags = DDPF_PALETTEINDEXED8; // 0x20
            break;
        case 15:
            rMask = 0x7C00;
            gMask = 0x3E0;
            bMask = 0x1F;
            break;
        case 16:
            rMask = 0xF800;
            gMask = 0x7E0;
            bMask = 0x1F;
            break;
        case 24:
        case 32:
            rMask = 0xFF0000;
            gMask = 0xFF00;
            bMask = 0xFF;
            break;
        case 115:
            rMask = 0x1F;
            gMask = 0x3E0;
            bMask = 0x7C00;
            break;
        case 116:
            rMask = 0x1F;
            gMask = 0x7E0;
            bMask = 0xF800;
            break;
        default:
            break;
    }

    DDSURFACEDESC pSurfaceDesc = {};
    pSurfaceDesc.dwSize = sizeof(DDSURFACEDESC);
    pSurfaceDesc.ddpfPixelFormat.dwBBitMask = bMask;
    pSurfaceDesc.ddpfPixelFormat.dwFlags = flags | DDPF_RGB; // 0x40
    pSurfaceDesc.ddckCKSrcBlt.dwColorSpaceHighValue = 0;
    pSurfaceDesc.ddckCKSrcBlt.dwColorSpaceLowValue = 0;

    pSurfaceDesc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    pSurfaceDesc.ddpfPixelFormat.dwRBitMask = rMask;
    pSurfaceDesc.ddpfPixelFormat.dwGBitMask = gMask;
    pSurfaceDesc.ddpfPixelFormat.dwRGBBitCount = bpp;

    pSurfaceDesc.dwFlags = DDSD_CAPS | DDSD_PIXELFORMAT | DDSD_CKSRCBLT | DDSD_WIDTH | DDSD_HEIGHT; // 0x11007;
    pSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;                                           // 0x40

    if (gVGA_force_sys_memory_surfaces_BC0BB4)
    {
        pSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN; // 0x840
    }
    else if (createFlags & 1)
    {
        pSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN; // 0x840
    }
    else if (createFlags & 2)
    {
        pSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_OFFSCREENPLAIN; // 0x4040
    }

    pSurfaceDesc.dwWidth = width;
    pSurfaceDesc.dwHeight = height;

    if (BMP_New_create_surface_4F1C60(&pSurfaceDesc, &pBitmap->field_0_pSurface))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 230, -1, "BMP_New: can't create the surface");
        return -1;
    }
    pBitmap->field_10_locked_pitch = pSurfaceDesc.lPitch;

    if (pBitmap->field_0_pSurface->GetSurfaceDesc(&pSurfaceDesc))
    {
        pBitmap->field_10_locked_pitch = 0;
        Error_NullPrint_4F28C0("BMP: GetSurfaceDesc failed on new BMP_t");
    }
    else
    {
        pBitmap->field_10_locked_pitch = pSurfaceDesc.lPitch;
    }

    pBitmap->field_C_height = height;
    pBitmap->field_14_bpp = static_cast<s8>(pSurfaceDesc.ddpfPixelFormat.dwRGBBitCount);
    pBitmap->field_8_width = width;
    pBitmap->field_15_pixel_format = static_cast<s8>(pixelFormat);
    pBitmap->field_18_create_flags = createFlags;
    return 0;
}

ALIVE_VAR(1, 0xBBC3E8, DDSURFACEDESC, sBmpSurfaceDesc_BBC3E8, {});

LPVOID BMP_Lock_4F1FF0(Bitmap* pBitmap)
{
    // Already locked or we don't have a surface
    if (pBitmap->field_4_pLockedPixels || !pBitmap->field_0_pSurface)
    {
        return pBitmap->field_4_pLockedPixels;
    }

    // Try to lock
    sBmpSurfaceDesc_BBC3E8.dwSize = sizeof(DDSURFACEDESC);
    HRESULT hr = pBitmap->field_0_pSurface->Lock(nullptr, &sBmpSurfaceDesc_BBC3E8, DDLOCK_WAIT, 0);
    if (hr == DDERR_SURFACELOST)
    {
        // Surface is gone, restore and try again
        hr = pBitmap->field_0_pSurface->Restore();
        if (SUCCEEDED(hr))
        {
            hr = pBitmap->field_0_pSurface->Lock(nullptr, &sBmpSurfaceDesc_BBC3E8, DDLOCK_WAIT, 0);
        }
    }

    if (SUCCEEDED(hr))
    {
        // OK locked
        pBitmap->field_10_locked_pitch = sBmpSurfaceDesc_BBC3E8.lPitch;
        pBitmap->field_4_pLockedPixels = sBmpSurfaceDesc_BBC3E8.lpSurface;
        return pBitmap->field_4_pLockedPixels;
    }
    else if (hr != DDERR_WRONGMODE)
    {
        // Push an error for everything bar wrong mode
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 363, -1, DX_HR_To_String_4F4EC0(hr));
    }

    return nullptr;
}

void BMP_Draw_String_4F2230(Bitmap* pBmp, s32 x, s32 y, u32 fgColour, s32 bgColour, LPCSTR lpString)
{
    const HDC dc = BMP_Get_DC_4F2150(pBmp);
    if (bgColour)
    {
        ::SetBkColor(dc, 0);
    }

    ::SetBkMode(dc, (bgColour != 0) + 1);

    u32 colour = 0;
    colour |= (BYTE0(fgColour) << 16);
    colour |= (BYTE1(fgColour) << 8);
    colour |= (BYTE2(fgColour) << 0);

    ::SetTextColor(dc, colour);
    ::TextOutA(dc, x, y, lpString, strlen(lpString));
    BMP_Release_DC_4F21A0(pBmp, dc);
}

#endif