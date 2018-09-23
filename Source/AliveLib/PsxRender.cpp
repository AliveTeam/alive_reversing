#include "stdafx.h"
#include "PsxRender.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "Midi.hpp"
#include "Primitives.hpp"
#include "Game.hpp"
#include "Error.hpp"
#include <gmock/gmock.h>


struct OtUnknown
{
    int** field_0_pOtStart;
    int** field_4;
    int** field_8_pOt_End;
};

ALIVE_ARY(1, 0xBD0D88, OtUnknown, 32, sOt_Stack_BD0D88, {});
ALIVE_VAR(1, 0xBD0C08, int, sOtIdxRollOver_BD0C08, 0);

ALIVE_VAR(1, 0xC2D03C, int, dword_C2D03C, 0);


ALIVE_VAR(1, 0x578318, short, sActiveTPage_578318, -1);
ALIVE_VAR(1, 0xbd0f0c, DWORD, sTexture_page_x_BD0F0C, 0);
ALIVE_VAR(1, 0xbd0f10, DWORD, sTexture_page_y_BD0F10, 0);
ALIVE_VAR(1, 0xbd0f14, DWORD, sTexture_mode_BD0F14, 0);
ALIVE_VAR(1, 0x57831c, DWORD, dword_57831C, 10);
ALIVE_VAR(1, 0xBD0F18, DWORD, sTexture_page_abr_BD0F18, 0);
ALIVE_VAR(1, 0xbd0f1c, WORD *, sTPage_src_ptr_BD0F1C, nullptr);

ALIVE_VAR(1, 0xBD2A04, DWORD, sTile_r_BD2A04, 0);
ALIVE_VAR(1, 0xBD2A00, DWORD, sTile_g_BD2A00, 0);
ALIVE_VAR(1, 0xBD29FC, DWORD, sTile_b_BD29FC, 0);

enum TextureModes
{
    e4Bit = 0,
    e8Bit = 1,
    e16Bit = 2
};

enum BlendModes // Or SemiTransparency rates
{
    eBlendMode_0 = 0, // 0.5xB + 0.5xF
    eBlendMode_1 = 1, // 1.0xB + 1.0xF
    eBlendMode_2 = 2, // 1.0xB - 1.0xF
    eBlendMode_3 = 3, // 1.0xB + 0.25xF
};

EXPORT int CC PSX_EMU_Render_Polys_2_51CCA0(int /*a1*/, int /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int* CC PSX_EMU_Render_Polys_2_51E140(int /*a1*/, int /*a2*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

EXPORT int CC PSX_EMU_Render_Polys_2_51D890(int /*a1*/, int /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT unsigned int CC PSX_EMU_Render_Polys_2_51C4C0(int /*a2*/, int /*a3*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC PSX_EMU_Render_Polys_2_51C6E0(int /*a1*/, int /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int* CC PSX_EMU_Render_Polys_2_51D2B0(int /*a1*/, int /*a2*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

EXPORT int* CC PSX_EMU_Render_Polys_2_51E890(int /*a1*/, int /*a2*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

EXPORT int CC PSX_EMU_Render_Polys_2_51DC90(int /*a1*/, int /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC PSX_EMU_Render_Polys_2_51C590(int /*a1*/, int /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int* CC PSX_EMU_Render_Polys_2_51C8D0(int /*a1*/, int /*size*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

ALIVE_VAR(1, 0xC2D04C, decltype(&PSX_EMU_Render_SPRT_51EF90), pPSX_EMU_Render_SPRT_51EF90_C2D04C, nullptr);
ALIVE_VAR(1, 0xBD3364, decltype(&PSX_EMU_Render_Polys_2_51CCA0), pPSX_EMU_51CCA0_BD3364, nullptr);
ALIVE_VAR(1, 0xBD328C, decltype(&PSX_EMU_Render_Polys_2_51E140), pPSX_EMU_51E140_BD328C, nullptr);
ALIVE_VAR(1, 0xBD3360, decltype(&PSX_EMU_Render_Polys_2_51D890), pPSX_EMU_51D890_BD3360, nullptr);
ALIVE_VAR(1, 0xBD32D4, decltype(&PSX_EMU_Render_Polys_2_51C4C0), pPSX_EMU_51C4C0_BD32D4, nullptr);
ALIVE_VAR(1, 0xBD32D0, decltype(&PSX_EMU_Render_Polys_2_51C6E0), pPSX_EMU_51C6E0_BD32D0, nullptr);
ALIVE_VAR(1, 0xBD327C, decltype(&PSX_EMU_Render_Polys_2_51D2B0), pPSX_EMU_51D2B0_BD327C, nullptr);
ALIVE_VAR(1, 0xBD326C, decltype(&PSX_EMU_Render_Polys_2_51E890), pPSX_EMU_51E890_BD326C, nullptr);
ALIVE_VAR(1, 0xBD3274, decltype(&PSX_EMU_Render_Polys_2_51DC90), pPSX_EMU_51DC90_BD3274, nullptr);
ALIVE_VAR(1, 0xBD3354, decltype(&PSX_EMU_Render_Polys_2_51C590), pPSX_EMU_51C590_BD3354, nullptr);
ALIVE_VAR(1, 0xBD335C, decltype(&PSX_EMU_Render_Polys_2_51C8D0), pPSX_EMU_51C8D0_BD335C, nullptr);

ALIVE_VAR(1, 0xc215c0, DWORD, sSemiTransShift_C215C0, 0);
ALIVE_VAR(1, 0xc215c4, DWORD, sRedShift_C215C4, 0);
ALIVE_VAR(1, 0xc1d180, DWORD, sGreenShift_C1D180, 0);
ALIVE_VAR(1, 0xc19140, DWORD, sBlueShift_C19140, 0);

struct Psx_Test
{
    __int16 r[32][32];
    __int16 g[32][32];
    __int16 b[32][32];
};
ALIVE_ASSERT_SIZEOF(Psx_Test, 0x1800); // 3072 words

ALIVE_ARY(1, 0xC215E0, Psx_Test, 4, sPsx_abr_lut_C215E0, {});

struct Psx_Data
{
    BYTE field_0[32];
};
ALIVE_ASSERT_SIZEOF(Psx_Data, 32);

ALIVE_ARY(1, 0xC1D1C0, Psx_Data, 32, stru_C1D1C0, {});
ALIVE_VAR(1, 0xC146C0, Psx_Test, stru_C146C0, {});


static void CalculateBlendingModesLUT()
{
    const int redShift = sRedShift_C215C4;
    const int greenShift = sGreenShift_C1D180;
    for (short i = 0; i < 32; i++)
    {
        for (short j = 0; j < 32; j++)
        {
            // 0.5xB + 0.5xF
            short value1 = (j + i) / 2;
            sPsx_abr_lut_C215E0[eBlendMode_0].r[i][j] = value1 << redShift;
            sPsx_abr_lut_C215E0[eBlendMode_0].g[i][j] = value1 << greenShift;
            sPsx_abr_lut_C215E0[eBlendMode_0].b[i][j] = value1;

            // 1.0xB + 1.0xF
            short value2 = i + j;
            if (value2 > 31)
            {
                value2 = 31;
            }
            sPsx_abr_lut_C215E0[eBlendMode_1].r[i][j] = value2 << redShift;
            sPsx_abr_lut_C215E0[eBlendMode_1].g[i][j] = value2 << greenShift;
            sPsx_abr_lut_C215E0[eBlendMode_1].b[i][j] = value2;

            // 1.0xB - 1.0xF
            short value3 = j - i;
            if (value3 < 0)
            {
                value3 = 0;
            }
            sPsx_abr_lut_C215E0[eBlendMode_2].r[i][j] = value3 << redShift;
            sPsx_abr_lut_C215E0[eBlendMode_2].g[i][j] = value3 << greenShift;
            sPsx_abr_lut_C215E0[eBlendMode_2].b[i][j] = value3;

            // 1.0xB + 0.25xF
            short value4 = j + (i / 4);
            if (value4 > 31)
            {
                value4 = 31;
            }
            sPsx_abr_lut_C215E0[eBlendMode_3].r[i][j] = value4 << redShift;
            sPsx_abr_lut_C215E0[eBlendMode_3].g[i][j] = value4 << greenShift;
            sPsx_abr_lut_C215E0[eBlendMode_3].b[i][j] = value4;
        }
    }
}
EXPORT int CC PSX_EMU_SetDispType_4F9960(int dispType)
{
    if (dispType != 2)
    {
        Error_PushErrorRecord_4F2920(
            "C:\\abe2\\code\\PSXEmu\\PSXEMU.C",
            288,
            -1,
            "PSX_EMU_SetDispType_4F9960 only implemented for type 2");
        return -1;
    }

    sVGA_DisplayType_BD1468 = dispType;

    sRedShift_C215C4 = 11;
    sGreenShift_C1D180 = 6;
    sBlueShift_C19140 = 0;
    sSemiTransShift_C215C0 = 5;

    pPSX_EMU_Render_SPRT_51EF90_C2D04C = PSX_EMU_Render_SPRT_51EF90;
    pPSX_EMU_51CCA0_BD3364 = PSX_EMU_Render_Polys_2_51CCA0;
    pPSX_EMU_51E140_BD328C = PSX_EMU_Render_Polys_2_51E140;
    pPSX_EMU_51D890_BD3360 = PSX_EMU_Render_Polys_2_51D890;
    pPSX_EMU_51C4C0_BD32D4 = PSX_EMU_Render_Polys_2_51C4C0;
    pPSX_EMU_51C6E0_BD32D0 = PSX_EMU_Render_Polys_2_51C6E0;
    pPSX_EMU_51D2B0_BD327C = PSX_EMU_Render_Polys_2_51D2B0;
    pPSX_EMU_51E890_BD326C = PSX_EMU_Render_Polys_2_51E890;
    pPSX_EMU_51DC90_BD3274 = PSX_EMU_Render_Polys_2_51DC90;
    pPSX_EMU_51C590_BD3354 = PSX_EMU_Render_Polys_2_51C590;
    pPSX_EMU_51C8D0_BD335C = PSX_EMU_Render_Polys_2_51C8D0;

    /*
    // NOTE: Never used and empty funcs, so skipped.
    dword_BD3290 = (int)PSX_EMU_nullsub_20;
    dword_BD3280 = (int)PSX_EMU_nullsub_22;
    dword_BD3288 = (int)PSX_EMU_nullsub_24;
    dword_BD3268 = (int)PSX_EMU_nullsub_21;
    dword_BD3284 = (int)PSX_EMU_nullsub_23;
    dword_BD3278 = (int)PSX_EMU_nullsub_25;
    */

    const int redShift = sRedShift_C215C4;
    const int greenShift = sGreenShift_C1D180;

    // TODO: Its unknown what this is calculating
    for (short i = 0; i < 32; i++)
    {
        short iPlus_iExp = 0;
        short iExp = 0;
        for (short j = 0; j < 32; j++)
        {
            short value = iPlus_iExp / 16;
            if (value > 31)
            {
                value = 31;
            }

            stru_C1D1C0[i].field_0[j] = static_cast<BYTE>(value);

            stru_C146C0.r[i][j] = value << redShift;
            stru_C146C0.g[i][j] = value << greenShift;
            stru_C146C0.b[i][j] = value;

            iPlus_iExp = i + iExp;
            iExp += i;
        }
    }

    CalculateBlendingModesLUT();

    return 0;
}


EXPORT void CC PSX_Pal_Conversion_4F98D0(WORD* pDataToConvert, WORD* pConverted, unsigned int size)
{
    if (sVGA_DisplayType_BD1468 == 5)
    {
        return;
    }

    for (unsigned int i = 0; i < size; i++)
    {
        pConverted[i] = ((pDataToConvert[i] >> 15) << sSemiTransShift_C215C0)
                      | ((pDataToConvert[i] & 31) << sRedShift_C215C4)
                      | (((pDataToConvert[i] >> 5) & 31) << sGreenShift_C1D180)
                      | (((pDataToConvert[i] >> 10) & 31) << sBlueShift_C19140);
    }
}

template <typename T>
T clip(const T& n, const T& lower, const T& upper) 
{
    return max(lower, min(n, upper));
}

// Note: Assumes bounds checked before hand
static void VRam_Rect_Fill(WORD* pVRamIter, int rect_w, int rect_h, int pitch_words, WORD fill_colour)
{
    if (rect_h - 1 >= 0)
    {
        for (int y = 0; y < rect_h; y++)
        {
            for (int x = 0; x < rect_w; x++)
            {
                pVRamIter[x] = fill_colour;
            }

            pVRamIter += pitch_words;
        }
    }
}

EXPORT signed int CC PSX_ClearImage_4F5BD0(PSX_RECT* pRect, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b)
{
    if (!BMP_Lock_4F1FF0(&sPsxVram_C1D160))
    {
        return 0;
    }

    // Max bound check
    if (pRect->x >= 1024 || pRect->y >= 512)
    {
        return 0;
    }

    // Min bound check
    if (pRect->w + pRect->x - 1 < 0 || pRect->h + pRect->y - 1 < 0)
    {
        return 0;
    }

    int rect_x1 = pRect->x;
    int rect_y1 = pRect->y;
    int rect_right = pRect->w + rect_x1 - 1;
    int rect_bottom = pRect->h + rect_y1 - 1;

    rect_x1 = clip(rect_x1, 0, 1023);

    if (rect_right > 1023)
    {
        rect_right = 1023;
    }

    rect_y1 = clip(rect_y1, 0, 511);

    if (rect_bottom > 511)
    {
        rect_bottom = 511;
    }

    const WORD colour_value = 
        ((1 << sSemiTransShift_C215C0) // TODO: Might be something else
        | (static_cast<unsigned int>(r) >> 3 << sRedShift_C215C4)
        | (static_cast<unsigned int>(g) >> 3 << sGreenShift_C1D180)
        | (static_cast<unsigned int>(b) >> 3 << sBlueShift_C19140));

    const int pitch_words = sPsxVram_C1D160.field_10_locked_pitch / sizeof(WORD);

    WORD* pVram = reinterpret_cast<WORD*>(sPsxVram_C1D160.field_4_pLockedPixels) + sizeof(WORD) * (rect_x1 + (rect_y1 * pitch_words));

    const int rect_h = rect_bottom - rect_y1 + 1;
    const int rect_w = rect_right - rect_x1 + 1;

    VRam_Rect_Fill(pVram, rect_w, rect_h, pitch_words, colour_value);

    BMP_unlock_4F2100(&sPsxVram_C1D160);
    return 1;
}

EXPORT void CC PSX_ClearOTag_4F6290(int** otBuffer, int otBufferSize)
{
    PSX_OrderingTable_4F62C0(otBuffer, otBufferSize);

    // Set each element to point to the next
    int i = 0;
    for (i = 0; i < otBufferSize - 1; i++)
    {
        otBuffer[i] = reinterpret_cast<int*>(&otBuffer[i + 1]);
    }

    // Terminate the list
    otBuffer[i] = reinterpret_cast<int*>(0xFFFFFFFF);
}

EXPORT void CC PSX_OrderingTable_4F62C0(int** otBuffer, int otBufferSize)
{
    int otIdx = 0;
    for (otIdx = 0; otIdx < 32; otIdx++)
    {
        if (otBuffer == sOt_Stack_BD0D88[otIdx].field_0_pOtStart)
        {
            break;
        }
    }

    if (otIdx == 32)
    {
        sOtIdxRollOver_BD0C08 = (sOtIdxRollOver_BD0C08 & 31);
        otIdx = sOtIdxRollOver_BD0C08;
    }

    sOt_Stack_BD0D88[otIdx].field_0_pOtStart = otBuffer;
    sOt_Stack_BD0D88[otIdx].field_4 = otBuffer;
    sOt_Stack_BD0D88[otIdx].field_8_pOt_End = &otBuffer[otBufferSize];
}

EXPORT signed int CC PSX_OT_Idx_From_Ptr_4F6A40(int** ot)
{
    for (int i = 0; i < 32; i++)
    {
        OtUnknown* pItem = &sOt_Stack_BD0D88[i];
        if (ot >= pItem->field_4 && ot <= pItem->field_8_pOt_End)
        {
            return i;
        }
    }
    return -1;
}

EXPORT void __cdecl PSX_4F6ED0(WORD* /*pVRam*/, int /*width*/, int /*height*/, int /*r*/, int /*g*/, int /*b*/, int /*pitch*/)
{
    NOT_IMPLEMENTED();
}

static int sLast_TILE_r_578328 = 0;
static int sLast_TILE_g_C3D0E0 = 0;
static int sLast_TILE_b_C3D0DC = 0;
static DWORD sLast_Tile_abr_57832C = 0;
ALIVE_ARY(1, 0xC2D080, short, 16384, word_C2D080, {});

void PSX_Render_TILE_Blended_Large_Impl(WORD *pVRam, int width, int height, int r, int g, int b, int pitch)
{
    // Rebuild if cached copy is invalid
    if (r != sLast_TILE_r_578328
        || g != sLast_TILE_g_C3D0E0
        || b != sLast_TILE_b_C3D0DC
        || sTexture_page_abr_BD0F18 != sLast_Tile_abr_57832C)
    {  
        // NOTE: if (spBitmap_C2D038->field_15_pixel_format == 15) {} case removed as only 16 supported

        short* pAbr_R = &sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18].r[r][0];
        short* pAbr_G = &sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18].g[g][0];
        short* pAbr_B = &sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18].b[b][0];

        // TODO: Figure out what this look up table actually is
        for (int i = 0; i < 16384; i++)
        {
            word_C2D080[i] = pAbr_B[(4 * i) & 31] | pAbr_R[(((i / 64)) / 8) & 31]  | pAbr_G[((4 * i) / 64) & 31];
        }

        sLast_TILE_r_578328 = r;
        sLast_TILE_g_C3D0E0 = g;
        sLast_TILE_b_C3D0DC = b;
        sLast_Tile_abr_57832C = sTexture_page_abr_BD0F18;
    }

    if (height - 1 >= 0)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                // Index into the look up table using the vram limited value as the index
                // which in turn is used as the output
                const WORD v1 = (pVRam[x] >> 2) & 0x3FFF;
                pVRam[x] = word_C2D080[v1];
            }

            pVRam += pitch;
        }
    }
}

EXPORT void CC PSX_Render_TILE_Blended_Large_4F6D00(WORD *pVRam, int width, int height, int r, int g, int b, int pitch)
{
    PSX_Render_TILE_Blended_Large_Impl(pVRam, width, height, r, g, b, pitch);
}

EXPORT void CC PSX_Render_TILE_4F6A70(const PSX_RECT* pRect, const PrimHeader* pPrim)
{
    if (!spBitmap_C2D038->field_4_pLockedPixels)
    {
        return;
    }

    if (pRect->x > sPSX_EMU_DrawEnvState_C3D080.field_0_clip.w + sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x - 1)
    {
        return;
    }

    if (pRect->y > sPSX_EMU_DrawEnvState_C3D080.field_0_clip.h + sPSX_EMU_DrawEnvState_C3D080.field_0_clip.y - 1)
    {
        return;
    }

    int rect_right_clipped = pRect->w + pRect->x - 1;
    if (rect_right_clipped < sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x)
    {
        return;
    }

    const int rect_bottom = pRect->h + pRect->y - 1;
    if (rect_bottom < sPSX_EMU_DrawEnvState_C3D080.field_0_clip.y)
    {
        return;
    }

    int clipped_x = pRect->x;
    if (pRect->x <= sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x)
    {
        clipped_x = sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x;
    }

    int clipped_y = sPSX_EMU_DrawEnvState_C3D080.field_0_clip.y;
    if (pRect->y > sPSX_EMU_DrawEnvState_C3D080.field_0_clip.y)
    {
        clipped_y = pRect->y;
    }

    if (rect_right_clipped >= sPSX_EMU_DrawEnvState_C3D080.field_0_clip.w + sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x - 1)
    {
        rect_right_clipped = sPSX_EMU_DrawEnvState_C3D080.field_0_clip.w + sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x - 1;
    }

    int rect_bottom_clipped = rect_bottom;
    if (rect_bottom >= sPSX_EMU_DrawEnvState_C3D080.field_0_clip.h + sPSX_EMU_DrawEnvState_C3D080.field_0_clip.y - 1)
    {
        rect_bottom_clipped = sPSX_EMU_DrawEnvState_C3D080.field_0_clip.h + sPSX_EMU_DrawEnvState_C3D080.field_0_clip.y - 1;
    }

    const int rect_w = rect_right_clipped - clipped_x + 1;
    const int rect_h = rect_bottom_clipped - clipped_y + 1;

    const BYTE r0 = pPrim->rgb_code.r;
    const BYTE b0 = pPrim->rgb_code.b;
    const BYTE g0 = pPrim->rgb_code.g;

    const DWORD r0_S3 = r0 >> 3;
    const DWORD g0_S3 = g0 >> 3;
    const DWORD b0_S3 = b0 >> 3;

    const int width_pitch = spBitmap_C2D038->field_10_locked_pitch / sizeof(WORD);
    WORD* pVRamDst = reinterpret_cast<WORD*>(spBitmap_C2D038->field_4_pLockedPixels) + (clipped_x + (clipped_y * width_pitch));

    if (!(pPrim->rgb_code.code_or_pad & 2)) // blending flag
    {
        const WORD fill_colour = static_cast<WORD>((r0_S3 << sRedShift_C215C4) | (g0_S3 << sGreenShift_C1D180) | (b0_S3 << sBlueShift_C19140));
        VRam_Rect_Fill(pVRamDst, rect_w, rect_h, width_pitch, fill_colour);
        return;
    }

    if (r0_S3 == 0 && g0_S3 == 0 && b0_S3 == 0)
    {
        return;
    }

    if (dword_5CA4D4) // Some DDFast option
    {
        if (sTexture_page_abr_BD0F18 == BlendModes::eBlendMode_1)
        {
            sTile_r_BD2A04 |= r0;
            sTile_g_BD2A00 |= g0;
            sTile_b_BD29FC |= b0;
            return;
        }

        if (sTexture_page_abr_BD0F18 == BlendModes::eBlendMode_2)
        {
            // RGB 666 / 24bit ?
            sTile_r_BD2A04 |= (r0 >> 1) & 63;
            sTile_g_BD2A00 |= (g0 >> 1) & 63;
            sTile_b_BD29FC |= (b0 >> 1) & 63;
            return;
        }
    }

    // Some optimization case - for type 2 blending and white colour the result must be black
    if (sTexture_page_abr_BD0F18 == BlendModes::eBlendMode_2 && r0_S3 >= 31 && g0_S3 >= 31 && b0_S3 >= 31)
    {
        VRam_Rect_Fill(pVRamDst, rect_w, rect_h, width_pitch, 0);
        return;
    }

    // NOTE: if (rect_w * rect_h < 16384){} optimization case removed
    PSX_Render_TILE_Blended_Large_4F6D00(pVRamDst, rect_w, rect_h, r0_S3, g0_S3, b0_S3, width_pitch);
}

struct OT_Vert
{
    int field_0_x0;
    int field_4_y0;
    int field_8;
    int field_C;
    int field_10;
    int field_14_u;
    int field_18_v;
    int field_1C_r;
    int field_20_g;
    int field_24_b;
};
ALIVE_ASSERT_SIZEOF(OT_Vert, 0x28);

struct OT_Prim
{
    int field_0;
    int field_4;
    BYTE field_8_r;
    BYTE field_9_g;
    BYTE field_A_b;
    BYTE field_B_flags;
    char field_C_vert_count;
    char field_D;
    char field_E;
    char field_F;
    __int16 field_10_tpage;
    __int16 field_12_clut;
    OT_Vert field_14_verts[4];
};
ALIVE_ASSERT_SIZEOF(OT_Prim, 180); // could be up to 380

ALIVE_ARY(1, 0x0, BYTE, 380, byte_BD0C0C, {});

ALIVE_VAR(1, 0x578330, OT_Prim*, off_578330, reinterpret_cast<OT_Prim*>(&byte_BD0C0C[0]));

EXPORT void CC PSX_poly_helper_50CC70(OT_Prim* /*pOt*/, int /*width*/, int /*height*/, int /*unknown*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_poly_helper_517990(OT_Prim* /*pOt*/, int /*width*/, int /*height*/, int /*unknown*/, int /*x0*/, int /*y0*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_poly_helper_517880(OT_Prim* /*pOt*/, int /*width*/, int /*height*/, int /*unknown*/)
{
    NOT_IMPLEMENTED();
}

EXPORT OT_Prim* CC PSX_Render_Convert_Polys_To_Internal_Format_4F7110(void* pData, int xoff, int yoff)
{
    PrimAny any;
    any.mVoid = pData;

    OT_Prim* pConverted = off_578330;
    pConverted->field_E = 0;
    pConverted->field_D = 1;
    pConverted->field_8_r = any.mPrimHeader->rgb_code.r;
    pConverted->field_9_g = any.mPrimHeader->rgb_code.g;
    pConverted->field_A_b = any.mPrimHeader->rgb_code.b;
    pConverted->field_B_flags = any.mPrimHeader->rgb_code.code_or_pad;

    const int kPrimType = PSX_Prim_Code_Without_Blending_Or_SemiTransparency(any.mPrimHeader->rgb_code.code_or_pad);
    if (kPrimType == ePolyGT4)
    {
        Poly_GT4* pPoly = any.mPolyGT4;
        pConverted->field_C_vert_count = 4;

        pConverted->field_10_tpage = GetTPage(pPoly);
        pConverted->field_12_clut = GetClut(pPoly);

        pConverted->field_14_verts[0].field_0_x0 = 16 * X0(pPoly);
        pConverted->field_14_verts[0].field_4_y0 = 16 * Y0(pPoly);
        pConverted->field_14_verts[0].field_8 = 0;
        pConverted->field_14_verts[0].field_1C_r = R0(pPoly);
        pConverted->field_14_verts[0].field_20_g = G0(pPoly);
        pConverted->field_14_verts[0].field_24_b = B0(pPoly);
        pConverted->field_14_verts[0].field_14_u = U0(pPoly);
        pConverted->field_14_verts[0].field_18_v = V0(pPoly);

        pConverted->field_14_verts[1].field_0_x0 = 16 * X1(pPoly);
        pConverted->field_14_verts[1].field_4_y0 = 16 * Y1(pPoly);
        pConverted->field_14_verts[1].field_8 = 0;
        pConverted->field_14_verts[1].field_1C_r = R1(pPoly);
        pConverted->field_14_verts[1].field_20_g = G1(pPoly);
        pConverted->field_14_verts[1].field_24_b = B1(pPoly);
        pConverted->field_14_verts[1].field_14_u = U1(pPoly);
        pConverted->field_14_verts[1].field_18_v = V1(pPoly);

        pConverted->field_14_verts[2].field_0_x0 = 16 * X3(pPoly);
        pConverted->field_14_verts[2].field_4_y0 = 16 * Y3(pPoly);
        pConverted->field_14_verts[2].field_8 = 0;
        pConverted->field_14_verts[2].field_1C_r = R3(pPoly);
        pConverted->field_14_verts[2].field_20_g = G3(pPoly);
        pConverted->field_14_verts[2].field_24_b = B3(pPoly);
        pConverted->field_14_verts[2].field_14_u = U3(pPoly);
        pConverted->field_14_verts[2].field_18_v = V3(pPoly);

        pConverted->field_14_verts[3].field_0_x0 = 16 * X2(pPoly);
        pConverted->field_14_verts[3].field_4_y0 = 16 * Y2(pPoly);
        pConverted->field_14_verts[3].field_8 = 0;
        pConverted->field_14_verts[3].field_1C_r = R2(pPoly);
        pConverted->field_14_verts[3].field_20_g = G2(pPoly);
        pConverted->field_14_verts[3].field_24_b = B2(pPoly);
        pConverted->field_14_verts[3].field_14_u = U2(pPoly);
        pConverted->field_14_verts[3].field_18_v = V2(pPoly);
        return pConverted;
    }
    else if (kPrimType == ePolyG4)
    {
        Poly_G4* pPoly = any.mPolyG4;
        pConverted->field_C_vert_count = 4;

        pConverted->field_14_verts[0].field_0_x0 = 16 * X0(pPoly);
        pConverted->field_14_verts[0].field_4_y0 = 16 * Y0(pPoly);
        pConverted->field_14_verts[0].field_8 = 0;
        pConverted->field_14_verts[0].field_1C_r = R0(pPoly) << 13;
        pConverted->field_14_verts[0].field_20_g = G0(pPoly) << 13;
        pConverted->field_14_verts[0].field_24_b = B0(pPoly) << 13;

        pConverted->field_14_verts[1].field_0_x0 = 16 * X1(pPoly);
        pConverted->field_14_verts[1].field_4_y0 = 16 * Y1(pPoly);
        pConverted->field_14_verts[1].field_8 = 0;
        pConverted->field_14_verts[1].field_1C_r = R1(pPoly) << 13;
        pConverted->field_14_verts[1].field_20_g = G1(pPoly) << 13;
        pConverted->field_14_verts[1].field_24_b = B1(pPoly) << 13;

        pConverted->field_14_verts[2].field_0_x0 = 16 * X3(pPoly);
        pConverted->field_14_verts[2].field_4_y0 = 16 * Y3(pPoly);
        pConverted->field_14_verts[2].field_8 = 0;
        pConverted->field_14_verts[2].field_1C_r = R3(pPoly) << 13;
        pConverted->field_14_verts[2].field_20_g = G3(pPoly) << 13;
        pConverted->field_14_verts[2].field_24_b = B3(pPoly) << 13;

        pConverted->field_14_verts[3].field_0_x0 = 16 * X2(pPoly);
        pConverted->field_14_verts[3].field_4_y0 = 16 * Y2(pPoly);
        pConverted->field_14_verts[3].field_8 = 0;
        pConverted->field_14_verts[3].field_1C_r = R2(pPoly) << 13;
        pConverted->field_14_verts[3].field_20_g = G2(pPoly) << 13;
        pConverted->field_14_verts[3].field_24_b = B2(pPoly) << 13;
        return pConverted;
    }
    else if (kPrimType == ePolyFT4)
    {
        Poly_FT4* pPoly = any.mPolyFT4;
        pConverted->field_C_vert_count = 4;

        pConverted->field_10_tpage = GetTPage(pPoly);
        pConverted->field_12_clut = GetClut(pPoly);

        pConverted->field_14_verts[0].field_0_x0 = 16 * X0(pPoly);
        pConverted->field_14_verts[0].field_4_y0 = 16 * Y0(pPoly);
        pConverted->field_14_verts[0].field_8 = 0;
        pConverted->field_14_verts[0].field_14_u = U0(pPoly);
        pConverted->field_14_verts[0].field_18_v = V0(pPoly);

        pConverted->field_14_verts[1].field_0_x0 = 16 * X1(pPoly);
        pConverted->field_14_verts[1].field_4_y0 = 16 * Y1(pPoly);
        pConverted->field_14_verts[1].field_8 = 0;
        pConverted->field_14_verts[1].field_14_u = U1(pPoly);
        pConverted->field_14_verts[1].field_18_v = V1(pPoly);

        pConverted->field_14_verts[2].field_0_x0 = 16 * X3(pPoly);
        pConverted->field_14_verts[2].field_4_y0 = 16 * Y3(pPoly);
        pConverted->field_14_verts[2].field_8 = 0;
        pConverted->field_14_verts[2].field_14_u = U3(pPoly);
        pConverted->field_14_verts[2].field_18_v = V3(pPoly);

        pConverted->field_14_verts[3].field_0_x0 = 16 * X2(pPoly);
        pConverted->field_14_verts[3].field_4_y0 = 16 * Y2(pPoly);
        pConverted->field_14_verts[3].field_8 = 0;
        pConverted->field_14_verts[3].field_14_u = U2(pPoly);
        pConverted->field_14_verts[3].field_18_v = V2(pPoly);

        if (!(pConverted->field_B_flags & 1) // Blending enabled?
            && (pConverted->field_8_r & 248) == 128
            && (pConverted->field_9_g & 248) == 128
            && (pConverted->field_A_b & 248) == 128)
        {
            pConverted->field_B_flags |= 1;
        }

        // TODO: Figure out what this extra 4 byte of data in the reserved fields is being used for
        const DWORD unknown = pPoly->mVerts[1].mUv.tpage_clut_pad + (pPoly->mVerts[2].mUv.tpage_clut_pad << 16);
        if (unknown)
        {
            const int xoffConverted = 16 * xoff;
            if (xoffConverted > 0)
            {
                pConverted->field_14_verts[0].field_0_x0 += xoffConverted;
                pConverted->field_14_verts[1].field_0_x0 += xoffConverted;
                pConverted->field_14_verts[2].field_0_x0 += xoffConverted;
                pConverted->field_14_verts[3].field_0_x0 += xoffConverted;
            }

            const int yoffConverted = 16 * yoff;
            if (yoffConverted > 0)
            {
                pConverted->field_14_verts[0].field_4_y0 += yoffConverted;
                pConverted->field_14_verts[1].field_4_y0 += yoffConverted;
                pConverted->field_14_verts[2].field_4_y0 += yoffConverted;
                pConverted->field_14_verts[3].field_4_y0 += yoffConverted;
            }

            PSX_TPage_Change_4F6430(pConverted->field_10_tpage);

            const int tPageSemiTransRate = (pConverted->field_10_tpage >> 7) & 3;
            switch (tPageSemiTransRate)
            {
            case eBlendMode_0: // 0.5xB + 0.5xF
                PSX_poly_helper_517880(pConverted, 
                    pConverted->field_14_verts[2].field_0_x0 - pConverted->field_14_verts[0].field_0_x0,
                    pConverted->field_14_verts[2].field_4_y0 - pConverted->field_14_verts[0].field_4_y0,
                    unknown);
                break;

            case eBlendMode_1: // 1.0xB + 1.0xF
                PSX_poly_helper_50CC70(pConverted,
                    pConverted->field_14_verts[2].field_0_x0 - pConverted->field_14_verts[0].field_0_x0,
                    pConverted->field_14_verts[2].field_4_y0 - pConverted->field_14_verts[0].field_4_y0,
                    unknown);
                break;

            case eBlendMode_2: // 1.0xB - 1.0xF
                PSX_poly_helper_517990(pConverted,
                    pConverted->field_14_verts[2].field_0_x0 - pConverted->field_14_verts[0].field_0_x0,
                    pConverted->field_14_verts[2].field_4_y0 - pConverted->field_14_verts[0].field_4_y0,
                    unknown,
                    pConverted->field_14_verts[0].field_0_x0 , pConverted->field_14_verts[0].field_4_y0);
                break;

            case eBlendMode_3: // 1.0xB + 0.25xF
                break;
            }
            return nullptr;
        }
        return pConverted;
    }
    else if (kPrimType == ePolyF4)
    {
        Poly_F4* pPoly = any.mPolyF4;
        pConverted->field_C_vert_count = 4;

        pConverted->field_14_verts[0].field_1C_r = R0(pPoly) << 13;
        pConverted->field_14_verts[0].field_20_g = G0(pPoly) << 13;
        pConverted->field_14_verts[0].field_24_b = B0(pPoly) << 13;

        pConverted->field_14_verts[0].field_0_x0 = 16 * X0(pPoly);
        pConverted->field_14_verts[0].field_4_y0 = 16 * Y0(pPoly);
        pConverted->field_14_verts[0].field_8 = 0;

        pConverted->field_14_verts[1].field_0_x0 = 16 * X1(pPoly);
        pConverted->field_14_verts[1].field_4_y0 = 16 * Y1(pPoly);
        pConverted->field_14_verts[1].field_8 = 0;

        pConverted->field_14_verts[2].field_0_x0 = 16 * X3(pPoly);
        pConverted->field_14_verts[2].field_4_y0 = 16 * Y3(pPoly);
        pConverted->field_14_verts[2].field_8 = 0;

        pConverted->field_14_verts[3].field_0_x0 = 16 * X2(pPoly);
        pConverted->field_14_verts[3].field_4_y0 = 16 * Y2(pPoly);
        pConverted->field_14_verts[3].field_8 = 0;
        return pConverted;
    }
    else if (kPrimType == ePolyGT3)
    {
        Poly_GT3* pPoly = any.mPolyGT3;
        pConverted->field_C_vert_count = 3;

        pConverted->field_10_tpage = GetTPage(pPoly);
        pConverted->field_12_clut = GetClut(pPoly);

        pConverted->field_14_verts[0].field_0_x0 = 16 * X0(pPoly);
        pConverted->field_14_verts[0].field_4_y0 = 16 * Y0(pPoly);
        pConverted->field_14_verts[0].field_8 = 0;
        pConverted->field_14_verts[0].field_1C_r = R0(pPoly);
        pConverted->field_14_verts[0].field_20_g = G0(pPoly);
        pConverted->field_14_verts[0].field_24_b = B0(pPoly);
        pConverted->field_14_verts[0].field_14_u = U0(pPoly);
        pConverted->field_14_verts[0].field_18_v = V0(pPoly);

        pConverted->field_14_verts[1].field_0_x0 = 16 * X1(pPoly);
        pConverted->field_14_verts[1].field_4_y0 = 16 * Y1(pPoly);
        pConverted->field_14_verts[1].field_8 = 0;
        pConverted->field_14_verts[1].field_1C_r = R1(pPoly);
        pConverted->field_14_verts[1].field_20_g = G1(pPoly);
        pConverted->field_14_verts[1].field_24_b = B1(pPoly);
        pConverted->field_14_verts[1].field_14_u = U1(pPoly);
        pConverted->field_14_verts[1].field_18_v = V1(pPoly);

        pConverted->field_14_verts[2].field_0_x0 = 16 * X2(pPoly);
        pConverted->field_14_verts[2].field_4_y0 = 16 * Y2(pPoly);
        pConverted->field_14_verts[2].field_8 = 0;
        pConverted->field_14_verts[2].field_1C_r = R2(pPoly);
        pConverted->field_14_verts[2].field_20_g = G2(pPoly);
        pConverted->field_14_verts[2].field_24_b = B2(pPoly);
        pConverted->field_14_verts[2].field_14_u = U2(pPoly);
        pConverted->field_14_verts[2].field_18_v = V2(pPoly);
        return pConverted;
    }
    else if (kPrimType == ePolyG3)
    {
        Poly_G3* pPoly = any.mPolyG3;
        pConverted->field_C_vert_count = 3;

        pConverted->field_14_verts[0].field_0_x0 = 16 * X0(pPoly);
        pConverted->field_14_verts[0].field_4_y0 = 16 * Y0(pPoly);
        pConverted->field_14_verts[0].field_8 = 0;
        pConverted->field_14_verts[0].field_1C_r = R0(pPoly) << 13;
        pConverted->field_14_verts[0].field_20_g = G0(pPoly) << 13;
        pConverted->field_14_verts[0].field_24_b = B0(pPoly) << 13;

        pConverted->field_14_verts[1].field_0_x0 = 16 * X1(pPoly);
        pConverted->field_14_verts[1].field_4_y0 = 16 * Y1(pPoly);
        pConverted->field_14_verts[1].field_8 = 0;
        pConverted->field_14_verts[1].field_1C_r = R1(pPoly) << 13;
        pConverted->field_14_verts[1].field_20_g = G1(pPoly) << 13;
        pConverted->field_14_verts[1].field_24_b = B1(pPoly) << 13;

        pConverted->field_14_verts[2].field_0_x0 = 16 * X2(pPoly);
        pConverted->field_14_verts[2].field_4_y0 = 16 * Y2(pPoly);
        pConverted->field_14_verts[2].field_8 = 0;
        pConverted->field_14_verts[2].field_1C_r = R2(pPoly) << 13;
        pConverted->field_14_verts[2].field_20_g = G2(pPoly) << 13;
        pConverted->field_14_verts[2].field_24_b = B2(pPoly) << 13;
        return pConverted;
    }
    else if (kPrimType == ePolyFT3)
    {
        Poly_FT3* pPoly = any.mPolyFT3;
        pConverted->field_C_vert_count = 3;

        pConverted->field_10_tpage = GetTPage(pPoly);
        pConverted->field_12_clut = GetClut(pPoly);

        pConverted->field_14_verts[0].field_0_x0 = 16 * X0(pPoly);
        pConverted->field_14_verts[0].field_4_y0 = 16 * Y0(pPoly);
        pConverted->field_14_verts[0].field_8 = 0;
        pConverted->field_14_verts[0].field_14_u = U0(pPoly);
        pConverted->field_14_verts[0].field_18_v = V0(pPoly);

        pConverted->field_14_verts[1].field_0_x0 = 16 * X1(pPoly);
        pConverted->field_14_verts[1].field_4_y0 = 16 * Y1(pPoly);
        pConverted->field_14_verts[1].field_8 = 0;
        pConverted->field_14_verts[1].field_14_u = U1(pPoly);
        pConverted->field_14_verts[1].field_18_v = V1(pPoly);

        pConverted->field_14_verts[2].field_0_x0 = 16 * X2(pPoly);
        pConverted->field_14_verts[2].field_4_y0 = 16 * Y2(pPoly);
        pConverted->field_14_verts[2].field_8 = 0;
        pConverted->field_14_verts[2].field_14_u = U2(pPoly);
        pConverted->field_14_verts[2].field_18_v = V2(pPoly);

        if (!(pConverted->field_B_flags & 1) // Blending enabled?
            && (pConverted->field_8_r & 248) == 128
            && (pConverted->field_9_g & 248) == 128
            && (pConverted->field_A_b & 248) == 128)
        {
            pConverted->field_B_flags |= 1;
        }

        return pConverted;
    }
    else if (kPrimType == ePolyF3)
    {
        Poly_F3* pPoly = any.mPolyF3;
        pConverted->field_C_vert_count = 3;

        pConverted->field_14_verts[0].field_1C_r = R0(pPoly) << 13;
        pConverted->field_14_verts[0].field_20_g = G0(pPoly) << 13;
        pConverted->field_14_verts[0].field_24_b = B0(pPoly) << 13;

        pConverted->field_14_verts[0].field_0_x0 = 16 * X0(pPoly);
        pConverted->field_14_verts[0].field_4_y0 = 16 * Y0(pPoly);
        pConverted->field_14_verts[0].field_8 = 0;

        pConverted->field_14_verts[1].field_0_x0 = 16 * X1(pPoly);
        pConverted->field_14_verts[1].field_4_y0 = 16 * Y1(pPoly);
        pConverted->field_14_verts[1].field_8 = 0;

        pConverted->field_14_verts[2].field_0_x0 = 16 * X2(pPoly);
        pConverted->field_14_verts[2].field_4_y0 = 16 * Y2(pPoly);
        pConverted->field_14_verts[2].field_8 = 0;
        
        return pConverted;
    }
    else
    {
        return nullptr;
    }
}

EXPORT void CC PSX_Render_Polys_2_4F7960(OT_Prim* /*a1*/, int /*a2*/, int /*a3*/)
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0xbd30e4, int, sScreenXOffSet_BD30E4, 0);
ALIVE_VAR(1, 0xbd30a4, int, sScreenYOffset_BD30A4, 0);

EXPORT void CC PSX_Render_Line_Prim_4F7D90(void* /*pOtPrim*/, int /*offX*/, int /*offY*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_84_4F7B80(int /*a1*/, int /*a2*/, int /*a3*/, int /*a4*/, int /*a5*/)
{
    NOT_IMPLEMENTED();
}

static void DrawOTag_Render_SPRT(PrimAny& any, __int16 drawEnv_of0, __int16 drawEnv_of1, short width, short height)
{
    BYTE b = 0;
    BYTE g = 0;
    BYTE r = 0;

    // Blending disabled bit
    if (any.mPrimHeader->rgb_code.code_or_pad & 1)
    {
        r = 128;
        g = 128;
        b = 128;
    }
    else
    {
        r = any.mPrimHeader->rgb_code.r;
        g = any.mPrimHeader->rgb_code.g;
        b = any.mPrimHeader->rgb_code.b;
    }

    const short x0 = drawEnv_of0 + any.mSprt->mBase.vert.x;
    const short y0 = drawEnv_of1 + any.mSprt->mBase.vert.y;
    const short u0 = any.mSprt->mUv.u;
    const short v0 = any.mSprt->mUv.v;

    pPSX_EMU_Render_SPRT_51EF90_C2D04C(
        x0, y0,
        u0, v0,
        r,
        g,
        b,
        width,
        height,
        any.mSprt->mUv.tpage_clut_pad,
        any.mPrimHeader->rgb_code.code_or_pad & 2); // Semi transparency bit
}

static void DrawOTag_Render_TILE(PrimAny& any, short x, short y, short w, short h)
{
    PSX_RECT rect = { x,y,w,h };
    PSX_Render_TILE_4F6A70(&rect, &any.mTile->mBase.header);
}

static void DrawOTag_HandlePrimRendering(PrimAny& any, __int16 drawEnv_of0, __int16 drawEnv_of1)
{
    switch (PSX_Prim_Code_Without_Blending_Or_SemiTransparency(any.mPrimHeader->rgb_code.code_or_pad))
    {
    case PrimTypeCodes::eSprt:
        DrawOTag_Render_SPRT(any, drawEnv_of0, drawEnv_of1, any.mSprt->field_14_w, any.mSprt->field_16_h);
        break;

    case PrimTypeCodes::eSprt8:
        DrawOTag_Render_SPRT(any, drawEnv_of0, drawEnv_of1, 8, 8);
        break;

    case PrimTypeCodes::eSprt16:
        DrawOTag_Render_SPRT(any, drawEnv_of0, drawEnv_of1, 16, 16);
        break;

    case PrimTypeCodes::eTile:
        DrawOTag_Render_TILE(any, drawEnv_of0 + any.mSprt->mBase.vert.x, drawEnv_of1 + any.mSprt->mBase.vert.y, any.mTile->field_14_w, any.mTile->field_16_h);
        break;

    case PrimTypeCodes::eTile1:
        DrawOTag_Render_TILE(any, drawEnv_of0 + any.mSprt->mBase.vert.x, drawEnv_of1 + any.mSprt->mBase.vert.y, 1, 1);
        break;

    case PrimTypeCodes::eTile8:
        DrawOTag_Render_TILE(any, drawEnv_of0 + any.mSprt->mBase.vert.x, drawEnv_of1 + any.mSprt->mBase.vert.y, 8, 8);
        break;

    case PrimTypeCodes::eTile16:
        DrawOTag_Render_TILE(any, drawEnv_of0 + any.mSprt->mBase.vert.x, drawEnv_of1 + any.mSprt->mBase.vert.y, 16, 16);
        break;

    case PrimTypeCodes::eLineF2:
    case PrimTypeCodes::eLineF3:
    case PrimTypeCodes::eLineF4:
    case PrimTypeCodes::eLineG2:
    case PrimTypeCodes::eLineG3:
    case PrimTypeCodes::eLineG4:
        PSX_Render_Line_Prim_4F7D90(any.mVoid, drawEnv_of0, drawEnv_of1);
        break;

    case PrimTypeCodes::ePolyF3:
    case PrimTypeCodes::ePolyFT3:
    case PrimTypeCodes::ePolyG3:
    case PrimTypeCodes::ePolyGT3:
    case PrimTypeCodes::ePolyF4:
    case PrimTypeCodes::ePolyFT4:
    case PrimTypeCodes::ePolyG4:
    case PrimTypeCodes::ePolyGT4:
        {
            // I think this works by func 1 populating some data structure and then func 2 does the actual rendering
            OT_Prim* pPolyBuffer = PSX_Render_Convert_Polys_To_Internal_Format_4F7110(any.mVoid, drawEnv_of0, drawEnv_of1);
            if (pPolyBuffer)
            {
                PSX_Render_Polys_2_4F7960(pPolyBuffer, drawEnv_of0, drawEnv_of1);
            }
        }
        break;
    }
}

static bool DrawOTagImpl(int** pOT, __int16 drawEnv_of0, __int16 drawEnv_of1)
{
    sScreenXOffSet_BD30E4 = 0;
    sScreenYOffset_BD30A4 = 0;
    sActiveTPage_578318 = -1;

    int otIdx = PSX_OT_Idx_From_Ptr_4F6A40(pOT);
    if (otIdx < 0)
    {
        return false;
    }

    int** pLastOtItem = sOt_Stack_BD0D88[otIdx].field_4;
    int** pOtEnd = sOt_Stack_BD0D88[otIdx].field_8_pOt_End;
    int** pOtItem = pOT;
    while (pOtItem)
    {
        if (pOtItem == (int **)-1)
        {
            break;
        }

        MIDI_UpdatePlayer_4FDC80();

        PrimAny any;
        any.mVoid = pOtItem;

        if (pOtItem < pLastOtItem || pOtItem >= pOtEnd) // Must actually be start otherwise check makes no sense ??
        {
            int itemToDrawType = any.mPrimHeader->rgb_code.code_or_pad;
            switch (itemToDrawType)
            {
            case 2: // TODO: unknown
                PSX_Render_TILE_4F6A70(nullptr, &any.mTile->mBase.header);
                break;

            case PrimTypeCodes::eSetTPage:
                PSX_TPage_Change_4F6430(static_cast<short>(any.mSetTPage->field_C_tpage));
                break;

            case PrimTypeCodes::ePrimClipper:
                sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x = any.mPrimClipper->field_C_x;
                sPSX_EMU_DrawEnvState_C3D080.field_0_clip.y = any.mPrimClipper->field_E_y;
                sPSX_EMU_DrawEnvState_C3D080.field_0_clip.w = any.mPrimHeader->header.mRect.w;
                sPSX_EMU_DrawEnvState_C3D080.field_0_clip.h = any.mPrimHeader->header.mRect.h;

                PSX_SetDrawEnv_Impl_4FE420(
                    16 * any.mPrimClipper->field_C_x,
                    16 * any.mPrimClipper->field_E_y,
                    (16 * (any.mPrimClipper->field_C_x + any.mPrimHeader->header.mRect.w)) - 16,
                    (16 * (any.mPrimClipper->field_E_y + any.mPrimHeader->header.mRect.h)) - 16,
                    1000 / 2,
                    nullptr);
                break;
            case PrimTypeCodes::eScreenOffset:
                // NOTE: Conditional on dword_55EF94 removed as it is constant 1
                sScreenXOffSet_BD30E4 = any.mScreenOffset->field_C_xoff * 2;
                sScreenYOffset_BD30A4 = any.mScreenOffset->field_E_yoff;
                break;

            case 0x83: // TODO: unknown
                // Unlock because move image will lock + unlock again
                BMP_unlock_4F2100(&sPsxVram_C1D160);
                PSX_MoveImage_4F5D50((PSX_RECT *)(pOtItem + 5), (int)pOtItem[3], (int)pOtItem[4]);

                // Hence lock again after move image
                if (BMP_Lock_4F1FF0(&sPsxVram_C1D160))
                {
                    break;
                }

                if (sPsxEmu_EndFrameFnPtr_C1D17C)
                {
                    sPsxEmu_EndFrameFnPtr_C1D17C(1);
                }
                return true;

            case 0x84: // TODO: unknown
                // Appears for gas..
                PSX_84_4F7B80((int)pOtItem[3], (int)pOtItem[4], (int)pOtItem[5], (int)pOtItem[6], (int)pOtItem[7]);
                break;
            default:
                DrawOTag_HandlePrimRendering(any, drawEnv_of0, drawEnv_of1);
                break;
            }
        }

        // To the next item
        pOtItem = (int **)any.mPrimHeader->tag;
    }

    return false;
}

EXPORT void CC PSX_DrawOTag_4F6540(int** pOT)
{
    if (!sPsxEmu_EndFrameFnPtr_C1D17C || !sPsxEmu_EndFrameFnPtr_C1D17C(0))
    {
        if (byte_BD0F20 || !BMP_Lock_4F1FF0(&sPsxVram_C1D160))
        {
            if (sPsxEmu_EndFrameFnPtr_C1D17C)
            {
                sPsxEmu_EndFrameFnPtr_C1D17C(1);
            }
            return;
        }

        __int16 drawEnv_of0 = 0;
        __int16 drawEnv_of1 = 0;

        if (bDontUseXYOffsetInRender_BD1464)
        {
            if (!BMP_Lock_4F1FF0(&stru_C1D1A0))
            {
                BMP_unlock_4F2100(&sPsxVram_C1D160);
                if (sPsxEmu_EndFrameFnPtr_C1D17C)
                {
                    sPsxEmu_EndFrameFnPtr_C1D17C(1);
                }
                return;
            }
            spBitmap_C2D038 = &stru_C1D1A0;
            drawEnv_of1 = 0;
            drawEnv_of0 = 0;
        }
        else
        {
            spBitmap_C2D038 = &sPsxVram_C1D160;
            drawEnv_of0 = sPSX_EMU_DrawEnvState_C3D080.field_8_ofs[0];
            drawEnv_of1 = sPSX_EMU_DrawEnvState_C3D080.field_8_ofs[1];
        }

        if (DrawOTagImpl(pOT, drawEnv_of0, drawEnv_of1))
        {
            return;
        }

        if (bDontUseXYOffsetInRender_BD1464)
        {
            BMP_unlock_4F2100(&stru_C1D1A0);
        }

        BMP_unlock_4F2100(&sPsxVram_C1D160);
        if (sPsxEmu_EndFrameFnPtr_C1D17C)
        {
            sPsxEmu_EndFrameFnPtr_C1D17C(1);
        }
        return;
    }
}

EXPORT void CC PSX_TPage_Change_4F6430(__int16 tPage)
{
    if (sActiveTPage_578318 != tPage)
    {
        sActiveTPage_578318 = tPage;

        // NOTE: Branch guarded by PSX_Ret_0_4F60D0 removed

        // Extract parts of the tpage
        sTexture_page_x_BD0F0C = (tPage & 0xF) << 6;
        sTexture_page_y_BD0F10 = 16 * (tPage & 0x10) + (((unsigned int)tPage >> 2) & 0x200);

        sTexture_mode_BD0F14 = ((unsigned int)tPage >> 7) & 3;
        sTexture_page_abr_BD0F18 = ((unsigned int)tPage >> 5) & 3;

        if (sTexture_mode_BD0F14 == (TextureModes::e16Bit | TextureModes::e8Bit))
        {
            // 0, 1 and 2 are valid values. So if for some reason 1 and 2 are set then default to 2
            sTexture_mode_BD0F14 = TextureModes::e16Bit;
        }

        // NOTE: Branch guarded by dword_BD2250[tPage & 31] removed as it is never written

        dword_57831C = 10;
        sTPage_src_ptr_BD0F1C = reinterpret_cast<WORD*>(sPsxVram_C1D160.field_4_pLockedPixels) + (sTexture_page_x_BD0F0C + (sTexture_page_y_BD0F10 * 1024));
    }
}

EXPORT bool CC PSX_Rects_intersect_point_4FA100(const PSX_RECT* pScreen, const PSX_RECT* pToRender, PSX_RECT* pOverlapRect, int* uStart, int* vStart)
{
    const bool bOverlaps = PSX_Rects_overlap_4FA0B0(pScreen, pToRender) ? true : false;
    if (bOverlaps)
    {
        int biggestRight = pToRender->x + pToRender->w;
        if (biggestRight > pScreen->x + pScreen->w)
        {
            biggestRight = pScreen->x + pScreen->w;
        }

        const __int16 overlap_w = static_cast<short>(biggestRight - pToRender->x);
        pOverlapRect->w = overlap_w;
        if (pToRender->x >= pScreen->x)
        {
            pOverlapRect->x = pToRender->x;
        }
        else
        {
            pOverlapRect->x = pScreen->x;
            pOverlapRect->w = pToRender->x + overlap_w - pScreen->x;
            *uStart += pScreen->x - pToRender->x;
        }


        int biggestBottom = pToRender->y + pToRender->h;
        if (biggestBottom > pScreen->y + pScreen->h)
        {
            biggestBottom = pScreen->y + pScreen->h;
        }

        const __int16 overlap_h = static_cast<short>(biggestBottom - pToRender->y);
        pOverlapRect->h = overlap_h;
        if (pToRender->y >= pScreen->y)
        {
            pOverlapRect->y = pToRender->y;
        }
        else
        {
            pOverlapRect->y = pScreen->y;
            pOverlapRect->h = pToRender->y + overlap_h - pScreen->y;
            *vStart += pScreen->y - pToRender->y;
        }
    }
    return bOverlaps;
}

// TODO: Can be refactored MUCH further
EXPORT void CC PSX_EMU_Render_SPRT_4bit_51F0E0(const PSX_RECT* pRect, int u, int v, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, WORD clut, char bSemiTrans)
{
    const int tpagey = sTexture_page_y_BD0F10 + v;
    const unsigned int rect_w = pRect->w;
    const WORD* pClutSrc1 = (WORD *)((char *)sPsxVram_C1D160.field_4_pLockedPixels  + 32 * ((clut & 63) + ((unsigned int)clut >> 6 << 6)));
    const WORD* pClutSrc2 = pClutSrc1;
    
    Psx_Test* pAbrLut = nullptr;
    if (bSemiTrans)
    {
        pAbrLut = &sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18];
    }

    const int texture_remainder_pitch = 2048 - (rect_w  / 2);
    const unsigned int pitch = spBitmap_C2D038->field_10_locked_pitch / 2;
    const int vram_remainder_pitch = pitch - pRect->w;
    const BYTE* pTexture_4bit_src1 = (BYTE *)sPsxVram_C1D160.field_4_pLockedPixels + 2 * (sTexture_page_x_BD0F0C + (u / 4) + (tpagey / 1024));
    const BYTE* pTexture_4bit_src2 = pTexture_4bit_src1;
    WORD* pVram_start = (WORD *)((char *)spBitmap_C2D038->field_4_pLockedPixels + 2 * (pRect->x + pitch * pRect->y));
    WORD* pVram_end = &pVram_start[(pRect->w - 1) + pitch * (pRect->h - 1)];
    WORD* pVram_end2 = pVram_end;
    if (r != 128 || g != 128 || b != 128)
    {
        if (bSemiTrans)
        {
            const Psx_Data* pLut_r = &stru_C1D1C0[r >> 3];
            const Psx_Data* pLut_g = &stru_C1D1C0[g >> 3];
            const Psx_Data* pLut_b = &stru_C1D1C0[b >> 3];
            while (pVram_start < pVram_end)
            {
                WORD* pLineEnd1 = &pVram_start[pRect->w];
                WORD* pLineEnd2 = &pVram_start[pRect->w];
                while (pVram_start < pLineEnd1)
                {
                    const int clut_4_bit_idx1 = *pTexture_4bit_src1 & 0xF;
                    if (pClutSrc1[clut_4_bit_idx1])
                    {
                        const WORD clut_pixel = pClutSrc1[clut_4_bit_idx1];
                        const WORD vram_pixel = *pVram_start;
                        pClutSrc1 = pClutSrc2;
                        pTexture_4bit_src1 = pTexture_4bit_src2;
                        pLineEnd1 = pLineEnd2;

                        const BYTE lutR = pLut_r->field_0[clut_pixel >> 11];
                        const BYTE lutG = pLut_g->field_0[(clut_pixel >> 6) & 31];
                        const BYTE lutB = pLut_b->field_0[clut_pixel & 31];

                        *pVram_start =
                            pAbrLut->r[lutR][vram_pixel >> 11]
                            | pAbrLut->g[lutG][((vram_pixel >> 6) & 31)]
                            | pAbrLut->b[lutB][vram_pixel & 31];
                    }
                    ++pVram_start;

                    if (pVram_start >= pLineEnd1)
                    {
                        break;
                    }

                    const unsigned int clut_4_bit_idx2 = (*pTexture_4bit_src1++) >> 4;
                    const WORD clut_pixel2 = pClutSrc1[clut_4_bit_idx2];
                    const WORD vram_pixel2 = *pVram_start;
                    pTexture_4bit_src2 = pTexture_4bit_src1;
                    if (clut_pixel2)
                    {
                        const BYTE lutR = pLut_r->field_0[clut_pixel2 >> 11];
                        const BYTE lutG = pLut_g->field_0[(clut_pixel2 >> 6) & 31];
                        const BYTE lutB = pLut_b->field_0[clut_pixel2 & 31];

                        *pVram_start =
                            pAbrLut->r[lutR][vram_pixel2 >> 11]
                            | pAbrLut->g[lutG][((vram_pixel2 >> 6) & 31)]
                            | pAbrLut->b[lutB][vram_pixel2 & 31];
                    }
                    ++pVram_start;
                }
                pTexture_4bit_src1 += texture_remainder_pitch;
                pVram_start += vram_remainder_pitch;
                pTexture_4bit_src2 = pTexture_4bit_src1;
            }
        }
        else
        {
            const __int16* g_lut = stru_C146C0.g[g >> 3];
            const __int16* r_lut = stru_C146C0.r[r >> 3];
            const __int16* b_lut = stru_C146C0.b[b >> 3];

            while (pVram_start < pVram_end)
            {
                WORD* pLineEnd3 = &pVram_start[pRect->w];
                WORD* pLineEnd4 = &pVram_start[pRect->w];
                while (pVram_start < pLineEnd3)
                {
                    if (pClutSrc1[*pTexture_4bit_src1 & 0xF])
                    {
                        *pVram_start =
                              b_lut[pClutSrc1[*pTexture_4bit_src1 & 0xF] & 31]
                            | r_lut[pClutSrc1[*pTexture_4bit_src1 & 0xF] >> 11]
                            | g_lut[(pClutSrc1[*pTexture_4bit_src1 & 0xF] >> 6) & 31];
                        pLineEnd3 = pLineEnd4;
                    }
                    ++pVram_start;

                    if (pVram_start >= pLineEnd3)
                    {
                        break;
                    }

                    const unsigned int clut_4_bit_idx3 = (*pTexture_4bit_src1++) >> 4;
                    const WORD clut_pixel3 = pClutSrc1[clut_4_bit_idx3];
                    if (clut_pixel3)
                    {
                        *pVram_start =
                              r_lut[clut_pixel3 >> 11]
                            | g_lut[(clut_pixel3 >> 6) & 31]
                            | b_lut[clut_pixel3 & 31];

                        pLineEnd3 = pLineEnd4;
                    }
                    ++pVram_start;
                }

                pVram_end = pVram_end2;
                pTexture_4bit_src1 += texture_remainder_pitch;
                pVram_start += vram_remainder_pitch;
            }
        }
    }
    else if (bSemiTrans)
    {
        while (pVram_start < pVram_end)
        {
            WORD* pLineEnd5 = &pVram_start[pRect->w];
            while (pVram_start < pLineEnd5)
            {
                const WORD clut_4_bit_idx4 = pClutSrc1[*pTexture_4bit_src1 & 0xF];
                if (clut_4_bit_idx4)
                {
                    pClutSrc1 = pClutSrc2;
                    pTexture_4bit_src1 = pTexture_4bit_src2;

                    const WORD vram_pixel2 = *pVram_start;

                    *pVram_start =
                        pAbrLut->r[vram_pixel2 >> 11][clut_4_bit_idx4 >> 11]
                        | pAbrLut->g[((vram_pixel2 >> 6) & 31)][(clut_4_bit_idx4 >> 6) & 31]
                        | pAbrLut->b[vram_pixel2 & 31][clut_4_bit_idx4 & 31];

                }
                ++pVram_start;

                if (pVram_start >= pLineEnd5)
                {
                    break;
                }

                const unsigned int clut_4_bit_idx5 = (*pTexture_4bit_src1++) >> 4;
                const WORD clut_pixel5 = pClutSrc1[clut_4_bit_idx5];
                pTexture_4bit_src2 = pTexture_4bit_src1;
                if (clut_pixel5)
                {
                    pClutSrc1 = pClutSrc2;

                    const WORD vram_pixel2 = *pVram_start;

                    *pVram_start =
                        pAbrLut->r[vram_pixel2 >> 11][clut_pixel5 >> 11]
                        | pAbrLut->g[((vram_pixel2 >> 6) & 31)][(clut_pixel5 >> 6) & 31]
                        | pAbrLut->b[vram_pixel2 & 31][clut_pixel5 & 31];

                }
                ++pVram_start;
            }
            pTexture_4bit_src1 += texture_remainder_pitch;
            pVram_start += vram_remainder_pitch;
            pTexture_4bit_src2 = pTexture_4bit_src1;
        }
    }
    else
    {
        while (pVram_start < pVram_end)
        {
            WORD* pLineEnd = &pVram_start[pRect->w];
            while (pVram_start < pLineEnd)
            {
                const WORD clut_pixel_from_nibble1 = pClutSrc1[*pTexture_4bit_src1 & 0xF];
                if (clut_pixel_from_nibble1)
                {
                    *pVram_start = clut_pixel_from_nibble1;
                }
                ++pVram_start;

                if (pVram_start >= pLineEnd)
                {
                    break;
                }

                const WORD clut_pixel_from_nibble2 = pClutSrc1[(*pTexture_4bit_src1++) >> 4];
                if (clut_pixel_from_nibble2)
                {
                    *pVram_start = clut_pixel_from_nibble2;
                }
                ++pVram_start;
            }
            pVram_start += vram_remainder_pitch;
            pTexture_4bit_src1 += texture_remainder_pitch;
        }
    }
}

// TODO: Can be refactored further
EXPORT void CC PSX_EMU_Render_SPRT_8bit_51F660(const PSX_RECT* pRect, int u, int v, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, unsigned __int16 clut, char bSemiTrans)
{
    const int tpagex = sTexture_page_x_BD0F0C + (u / 2);
    const int tpagey = sTexture_page_y_BD0F10 + v;
    BYTE* pTexture_8bit_src = (unsigned __int8 *)sPsxVram_C1D160.field_4_pLockedPixels + 2 * (tpagex + (tpagey * 1024));
    const int texture_8bit_remainder = 2048 - pRect->w;

    const WORD* pClutSrc = (WORD *)((char *)sPsxVram_C1D160.field_4_pLockedPixels + 32 * ((clut & 63) + (((unsigned int)clut / 64) * 64)));

    const unsigned int pitch = (unsigned int)spBitmap_C2D038->field_10_locked_pitch / 2;
    const int vram_dst_remainder = pitch - pRect->w;
    unsigned __int16* pVram_dst = (unsigned __int16 *)((char *)spBitmap_C2D038->field_4_pLockedPixels + 2 * (pRect->x + pitch * pRect->y));
    WORD* pVram_End = &pVram_dst[(pRect->w - 1) + pitch * (pRect->h - 1)];

    if (r != 128 || g != 128 || b != 128)
    {
        if (bSemiTrans)
        {
            Psx_Test* pAbr_Lut = &sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18];
            while (pVram_dst < pVram_End)
            {
                WORD* pLineEnd = &pVram_dst[pRect->w];
                while (pVram_dst < pLineEnd)
                {
                    const int vram_8bit_idx = *pTexture_8bit_src;
                    if (pClutSrc[vram_8bit_idx])
                    {
                        const WORD clut_pixel = pClutSrc[vram_8bit_idx];

                        const BYTE g_lut_idx = stru_C1D1C0[g >> 3].field_0[(clut_pixel >> 6) & 31];
                        const BYTE r_lut_idx = stru_C1D1C0[r >> 3].field_0[(clut_pixel >> 11) & 31];
                        const BYTE b_lut_idx = stru_C1D1C0[b >> 3].field_0[(clut_pixel) & 31];

                        const WORD vram_pixel = *pVram_dst;
                        *pVram_dst =
                              pAbr_Lut->r[r_lut_idx][(vram_pixel >> 11) & 31]
                            | pAbr_Lut->g[g_lut_idx][(vram_pixel >> 6) & 31]
                            | pAbr_Lut->b[b_lut_idx][(vram_pixel) & 31];
                    }
                    pVram_dst++;
                    pTexture_8bit_src++;
                }
                pVram_dst += vram_dst_remainder;
                pTexture_8bit_src += texture_8bit_remainder;
            }
        }
        else
        {
            while (pVram_dst < pVram_End)
            {
                WORD* pLineEnd = &pVram_dst[pRect->w];
                while (pVram_dst < pLineEnd)
                {
                    const WORD vram_8bit_idx = *pTexture_8bit_src;
                    const WORD clut_pixel = pClutSrc[vram_8bit_idx];
                    if (clut_pixel)
                    {
                        *pVram_dst =
                              stru_C146C0.b[b >> 3][clut_pixel & 31]
                            | stru_C146C0.r[r >> 3][clut_pixel >> 11]
                            | stru_C146C0.g[g >> 3][(clut_pixel >> 6) & 31];
                    }
                    pVram_dst++;
                    pTexture_8bit_src++;
                }
                pTexture_8bit_src += texture_8bit_remainder;
                pVram_dst += vram_dst_remainder;
            }
        }
    }
    else if (bSemiTrans)
    {
        Psx_Test* pAbr_Lut = &sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18];
        while (pVram_dst < pVram_End)
        {
            WORD* pLineEnd = &pVram_dst[pRect->w];
            while (pVram_dst < pLineEnd)
            {
                const WORD vram_8bit_idx = *pTexture_8bit_src;
                const WORD clut_pixel = pClutSrc[vram_8bit_idx];

                if (clut_pixel)
                {
                    // Semi trans flag
                    if (clut_pixel & 0x20) 
                    {
                        const BYTE r_lut_idx = (clut_pixel >> 11);
                        const BYTE g_lut_idx = (((clut_pixel >> 6) & 31));
                        const BYTE b_lut_idx = ((clut_pixel & 31));

                        const WORD vram_pixel = *pVram_dst;
                        *pVram_dst =
                              pAbr_Lut->r[r_lut_idx][(vram_pixel >> 11)]
                            | pAbr_Lut->g[g_lut_idx][((vram_pixel >> 6) & 31)]
                            | pAbr_Lut->b[b_lut_idx][(vram_pixel & 31)];
                    }
                    else
                    {
                        *pVram_dst = clut_pixel;
                    }
                }
                else
                {
                    // Pure black pixels skipped
                }

                pVram_dst++;
                pTexture_8bit_src++;
            }
            pVram_dst += vram_dst_remainder;
            pTexture_8bit_src += texture_8bit_remainder;
        }
    }
    else
    {
        while (pVram_dst < pVram_End)
        {
            WORD* pLineEnd = &pVram_dst[pRect->w];
            while (pVram_dst < pLineEnd)
            {
                const WORD vram_8bit_idx = *pTexture_8bit_src;
                if (pClutSrc[vram_8bit_idx])
                {
                    *pVram_dst = pClutSrc[vram_8bit_idx];
                }
                pTexture_8bit_src++;
                pVram_dst++;
            }
            pTexture_8bit_src += texture_8bit_remainder;
            pVram_dst += vram_dst_remainder;
        }
    }
}

EXPORT void CC PSX_EMU_Render_SPRT_16bit_51FA30(const PSX_RECT* pRect, int u, int v, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, int /*clut*/, char bSemiTrans)
{
    const int texture_row_width = (1 << dword_57831C) - pRect->w;
    const unsigned int line_pitch = (unsigned int)spBitmap_C2D038->field_10_locked_pitch >> 1;
    const unsigned int pitch_remainder = line_pitch - pRect->w;

    const WORD* pTexture_src = &sTPage_src_ptr_BD0F1C[u + (v << dword_57831C)];// dword_57831C = tpage width/bpp? becomes / 1024 ?
    WORD* pVram_start = reinterpret_cast<WORD*>(spBitmap_C2D038->field_4_pLockedPixels) + (pRect->x + (line_pitch * pRect->y));
    WORD* pVram_end = &pVram_start[(pRect->w - 1) + line_pitch * (pRect->h - 1)];

    if (!bSemiTrans && (r != 128 || g != 128 || b != 128))
    {
        while (pVram_start < pVram_end)
        {
            WORD* pLineEnd = &pVram_start[pRect->w];
            while (pVram_start < pLineEnd)
            {
                const WORD texture_pixel = *pTexture_src;
                if (texture_pixel)
                {
                    *pVram_start =
                        stru_C146C0.r[r >> 3][(texture_pixel >> 11) & 31]
                        | stru_C146C0.g[g >> 3][(texture_pixel >> 6) & 31]
                        | stru_C146C0.b[b >> 3][(texture_pixel & 31)];
                }
                pTexture_src++;
                pVram_start++;
            }
            pTexture_src += texture_row_width;
            pVram_start += pitch_remainder;
        }
        return;
    }

    if (bSemiTrans)
    {
        const auto pAbr_lut = &sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18];
        while (pVram_start < pVram_end)
        {
            WORD* pLineEnd = &pVram_start[pRect->w];
            while (pVram_start < pLineEnd)
            {
                const WORD texture_pixel = *pTexture_src;
                if (texture_pixel)
                {
                    const WORD vram_pixel = *pVram_start;

                    const BYTE rLut = stru_C1D1C0[r >> 3].field_0[texture_pixel >> 11];
                    const BYTE gLut = stru_C1D1C0[g >> 3].field_0[(texture_pixel >> 6) & 31];
                    const BYTE bLut = stru_C1D1C0[b >> 3].field_0[texture_pixel & 31];

                    *pVram_start =
                        pAbr_lut->r[rLut][(vram_pixel >> 11)]
                        | pAbr_lut->g[gLut][((vram_pixel >> 6) & 31)]
                        | pAbr_lut->b[bLut][(vram_pixel & 31)];
                }

                pTexture_src++;
                pVram_start++;
            }

            pVram_start += pitch_remainder;
            pTexture_src += texture_row_width;
        }
        return;
    }

    const int widthBytes = pRect->w + pitch_remainder;
    const int nextWriteOffset = widthBytes;
    const bool ignoreBlackPixels = sActiveTPage_578318 >= 0;

    // NOTE: Odd optimization case removed
    while (pVram_start < pVram_end)
    {
        for (int i = 0; i < pRect->w; i++)
        {
            // Skip black pixels these become see through
            const bool writePixel = ignoreBlackPixels == false || pTexture_src[i];
            if (writePixel)
            {
                pVram_start[i] = pTexture_src[i];
            }
        }
        pVram_start += nextWriteOffset;
        pTexture_src += nextWriteOffset;
    }
}

EXPORT void CC PSX_EMU_Render_SPRT_51EF90(__int16 x, __int16 y, int u, int v, BYTE r, BYTE g, BYTE b, __int16 w, __int16 h, WORD clut, int semiTrans)
{
    // Get the screen rect
    PSX_RECT screenRect  = {};
    if (bDontUseXYOffsetInRender_BD1464)
    {
        screenRect.y = 0;
        screenRect.x = 0;
        screenRect.w = sPSX_EMU_DrawEnvState_C3D080.field_0_clip.w;
        screenRect.h = sPSX_EMU_DrawEnvState_C3D080.field_0_clip.h;
    }
    else
    {
        screenRect = sPSX_EMU_DrawEnvState_C3D080.field_0_clip;
    }

    // Check for overlap
    const PSX_RECT toRenderRect = { x , y, w, h };
    PSX_RECT overlapRect = {};
    if (PSX_Rects_intersect_point_4FA100(&screenRect, &toRenderRect, &overlapRect, &u, &v))
    {
        // Render
        if (sTexture_mode_BD0F14 == TextureModes::e4Bit)
        {
            PSX_EMU_Render_SPRT_4bit_51F0E0(&overlapRect, u, v, r, g, b, clut, static_cast<char>(semiTrans));
        }
        else if (sTexture_mode_BD0F14 == TextureModes::e8Bit)
        {
            PSX_EMU_Render_SPRT_8bit_51F660(&overlapRect, u, v, r, g, b, clut, static_cast<char>(semiTrans));
        }
        else if (sTexture_mode_BD0F14 == TextureModes::e16Bit)
        {
            PSX_EMU_Render_SPRT_16bit_51FA30(&overlapRect, u, v, r, g, b, clut, static_cast<char>(semiTrans));
        }
    }
}

namespace Test
{
    static void Test_PSX_Rects_intersect_point_4FA100()
    {
        PSX_RECT r1 = { 0,0, 300, 150 };
        PSX_RECT r2 = { 10, 10, 50, 200 };
        PSX_RECT ro = {};
        int px = 0;
        int py = 0;
        ASSERT_EQ(1, PSX_Rects_intersect_point_4FA100(&r1, &r2, &ro, &px, &py));

        ASSERT_EQ(px, 0);

    }

    static void Test_PSX_TPage_Change_4F6430()
    {
        sActiveTPage_578318 = 0;
        sTexture_page_x_BD0F0C = 0;
        sTexture_page_y_BD0F10 = 0;
        sTexture_mode_BD0F14 = 0;
        dword_57831C = 0;

        for (DWORD i = 0; i < 3; i++)
        {
            int tpage = PSX_getTPage_4F60E0(static_cast<char>(i), static_cast<char>(3 - i), 64 * i, (i == 0u) ? 0u : 256u);
            PSX_TPage_Change_4F6430(static_cast<short>(tpage));

            ASSERT_EQ(sActiveTPage_578318, tpage);
            ASSERT_EQ(sTexture_page_x_BD0F0C, 64u * i);
            ASSERT_EQ(sTexture_page_y_BD0F10, (i == 0) ? 0u : 256u);
            ASSERT_EQ(sTexture_mode_BD0F14, i);
            ASSERT_EQ(sTexture_page_abr_BD0F18, 3 - i);
            ASSERT_EQ(dword_57831C, 10u);
        }
    }

    static void Test_PSX_Render_Convert_Polys_To_Internal_Format_4F7110()
    {
        {
            memset(off_578330, 1, 380);

            Poly_F3 polyF3 = {};
            PolyF3_Init(&polyF3);
            SetRGB0(&polyF3, 255, 0, 255);
            SetXY0(&polyF3, 20, 50);
            SetXY1(&polyF3, 20, 50 + 50);
            SetXY2(&polyF3, 20 + 50, 50 + 50);

            PSX_Render_Convert_Polys_To_Internal_Format_4F7110(&polyF3, 0, 0);

            ASSERT_EQ(off_578330->field_D, 1);
            ASSERT_EQ(off_578330->field_C_vert_count, 3);

            ASSERT_EQ(off_578330->field_8_r, R0(&polyF3));
            ASSERT_EQ(off_578330->field_9_g, G0(&polyF3));
            ASSERT_EQ(off_578330->field_A_b, B0(&polyF3));

            ASSERT_EQ(off_578330->field_14_verts[0].field_0_x0, 20 * 16);
            ASSERT_EQ(off_578330->field_14_verts[0].field_4_y0, 50 * 16);

            ASSERT_EQ(off_578330->field_14_verts[1].field_0_x0, 20 * 16);
            ASSERT_EQ(off_578330->field_14_verts[1].field_4_y0, (50 + 50) * 16);

            ASSERT_EQ(off_578330->field_14_verts[2].field_0_x0, (20 + 50) * 16);
            ASSERT_EQ(off_578330->field_14_verts[2].field_4_y0, (50 + 50) * 16);
        }

        {
            memset(off_578330, 0, 380);

            Poly_FT4 polyFT4 = {};
            PolyFT4_Init_4F8870(&polyFT4);
            SetRGB0(&polyFT4, 255, 0, 255);
            SetXY0(&polyFT4, 20, 50);
            SetXY1(&polyFT4, 20, 50 + 50);
            SetXY2(&polyFT4, 20 + 50, 50 + 50);
            SetXY3(&polyFT4, 20 + 50, 50);

            PSX_Render_Convert_Polys_To_Internal_Format_4F7110(&polyFT4, 0, 0);

            ASSERT_EQ(off_578330->field_D, 1);
            ASSERT_EQ(off_578330->field_C_vert_count, 4);

            ASSERT_EQ(off_578330->field_8_r, R0(&polyFT4));
            ASSERT_EQ(off_578330->field_9_g, G0(&polyFT4));
            ASSERT_EQ(off_578330->field_A_b, B0(&polyFT4));

            ASSERT_EQ(off_578330->field_14_verts[0].field_0_x0, 20 * 16);
            ASSERT_EQ(off_578330->field_14_verts[0].field_4_y0, 50 * 16);

            ASSERT_EQ(off_578330->field_14_verts[1].field_0_x0, 20 * 16);
            ASSERT_EQ(off_578330->field_14_verts[1].field_4_y0, (50 + 50) * 16);

            ASSERT_EQ(off_578330->field_14_verts[2].field_0_x0, (20 + 50) * 16);
            ASSERT_EQ(off_578330->field_14_verts[2].field_4_y0, (50) * 16);

            ASSERT_EQ(off_578330->field_14_verts[3].field_0_x0, (20 + 50) * 16);
            ASSERT_EQ(off_578330->field_14_verts[3].field_4_y0, (50 + 50) * 16);
        }
    }

    void PsxRenderTests()
    {
        Test_PSX_TPage_Change_4F6430();
        Test_PSX_Rects_intersect_point_4FA100();
        Test_PSX_Render_Convert_Polys_To_Internal_Format_4F7110();
    }
}
