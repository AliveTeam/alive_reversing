#include "stdafx.h"
#include "PsxRender.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "Sound/Midi.hpp"
#include "Sound/PsxSpuApi.hpp"
#include "Primitives.hpp"
#include "Game.hpp"
#include "Error.hpp"
#include "Compression.hpp"
#include <gmock/gmock.h>


struct OtUnknown
{
    int** field_0_pOtStart;
    int** field_4;
    int** field_8_pOt_End;
};

ALIVE_ARY(1, 0xBD0D88, OtUnknown, 32, sOt_Stack_BD0D88, {});
ALIVE_VAR(1, 0xBD0C08, int, sOtIdxRollOver_BD0C08, 0);


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

ALIVE_ARY(1, 0xC19160, float, 4096, sPsxEmu_float_table_C19160, {});
ALIVE_ARY(1, 0xC1D5C0, int, 4096, sPsxEmu_fixed_point_table_C1D5C0, {});

struct Render_Unknown
{
    int field_0_x; // 16:16 fixed ??
    int field_4_y;
    int field_8;
    int field_C;
    int field_10_float; // float ?
    int field_14_u; // float ?
    int field_18_v; // float ?
    int field_1C_GShadeR;
    int field_20_GShadeG;
    int field_24_GShadeB;
};
ALIVE_ASSERT_SIZEOF(Render_Unknown, 0x28);

ALIVE_VAR(1, 0xbd3350, WORD, sPoly_fill_colour_BD3350, 0);

ALIVE_VAR(1, 0xbd3320, Render_Unknown, left_side_BD3320, {});
ALIVE_VAR(1, 0xbd3200, Render_Unknown, slope_1_BD3200, {});

ALIVE_VAR(1, 0xbd32a0, Render_Unknown, right_side_BD32A0, {});
ALIVE_VAR(1, 0xbd32e0, Render_Unknown, slope_2_BD32E0, {});

struct Psx_Test
{
    __int16 r[32][32];
    __int16 g[32][32];
    __int16 b[32][32];
};
ALIVE_ASSERT_SIZEOF(Psx_Test, 0x1800); // 3072 words

ALIVE_ARY(1, 0xC215E0, Psx_Test, 4, sPsx_abr_lut_C215E0, {});


struct OT_Vert
{
    int field_0_x0; // Note actually __int16 1.3.12 FP
    int field_4_y0; // Note actually __int16 1.3.12 FP
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
    char field_D_bClip;
    char field_E;
    char field_F;
    __int16 field_10_tpage;
    __int16 field_12_clut;
    OT_Vert field_14_verts[9]; // TODO: Should be 9 ??
};
ALIVE_ASSERT_SIZEOF(OT_Prim, 380); // could be up to 380

ALIVE_ARY(1, 0xBD0C0C, BYTE, 380, byte_BD0C0C, {});

ALIVE_VAR(1, 0x578330, OT_Prim*, off_578330, reinterpret_cast<OT_Prim*>(&byte_BD0C0C[0]));

ALIVE_VAR(1, 0xbd3264, OT_Vert *, pVerts_dword_BD3264, nullptr);
ALIVE_VAR(1, 0xbd3270, WORD *, pClut_src_BD3270, nullptr);
ALIVE_VAR(1, 0xbd32c8, WORD *, pTPage_src_BD32C8, nullptr);

ALIVE_VAR(1, 0xbd3308, __int16*, r_lut_dword_BD3308, nullptr);
ALIVE_VAR(1, 0xbd32d8, __int16*, g_lut_dword_BD32D8, nullptr);
ALIVE_VAR(1, 0xbd3348, __int16*, b_lut_dword_BD3348, nullptr);

ALIVE_VAR(1, 0xbd32cc, BYTE*, rTable_dword_BD32CC, nullptr);
ALIVE_VAR(1, 0xbd3358, BYTE*, gTable_dword_BD3358, nullptr);
ALIVE_VAR(1, 0xbd334c, BYTE*, bTable_dword_BD334C, nullptr);

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

void Psx_Render_Float_Table_Init()
{
    for (int i = 0; i < 4096; i++)
    {
        if (i == 0)
        {
            sPsxEmu_float_table_C19160[i] = 0;
            sPsxEmu_fixed_point_table_C1D5C0[i] = 0;
        }
        else
        {
            sPsxEmu_float_table_C19160[i] = 1.0f / static_cast<float>(i);
            sPsxEmu_fixed_point_table_C1D5C0[i] = 0x10000 / (i);
        }
    }
}

EXPORT int CC PSX_poly_helper_fixed_point_scale_517FA0(int fixedPoint, int scaleFactor)
{
    return (static_cast<signed __int64>(fixedPoint) * scaleFactor) / 0x10000;
}

template<class T>
static inline void WriteSinglePixelOfXLine(WORD* pVRam, int xLeft, int xRight, T writePixel)
{
    WORD* pStart = &pVRam[xLeft];
    WORD* pEnd = &pVRam[xRight];
    while (pStart < pEnd)
    {
        writePixel(pStart);
        ++pStart;
    }
}

EXPORT void CC PSX_EMU_Render_Polys_Textured_Blending_Opqaue_51CCA0(WORD* pVRam, int ySize)
{
    const unsigned int pitch = (unsigned int)spBitmap_C2D038->field_10_locked_pitch / sizeof(WORD);

    const Render_Unknown* pLeft = &left_side_BD3320;
    const Render_Unknown* pRight = &right_side_BD32A0;

    for (int i = 0; i < ySize; i++)
    {
        if (pLeft->field_0_x > pRight->field_0_x)
        {
            std::swap(pLeft, pRight);
        }
        const int x_right = pRight->field_0_x >> 16;
        const int x_left = pLeft->field_0_x >> 16;
        const int x_diff = x_right - x_left;

        if (x_diff > 0)
        {
            int x_diff_m1 = x_right - x_left - 1;
            // Prevent divide by zero
            if (x_diff_m1 <= 0)
            {
                x_diff_m1 = 1;
            }

            int kFixedPointFactor = 0;
            switch (sTexture_mode_BD0F14)
            {
            case TextureModes::e16Bit:
                kFixedPointFactor = 1024;
                break;
            case TextureModes::e8Bit:
                kFixedPointFactor = 2048;
                break;
            case TextureModes::e4Bit:
                kFixedPointFactor = 4096;
                break;
            }

            const int u_diff = (signed int)(pRight->field_14_u - pLeft->field_14_u) / x_diff_m1;
            int u_pos = pLeft->field_14_u;

            const int v_diff = ((signed int)(pRight->field_18_v - pLeft->field_18_v) / x_diff_m1) * kFixedPointFactor;
            int v_pos = pLeft->field_18_v * kFixedPointFactor;

            if (sTexture_mode_BD0F14 == TextureModes::e8Bit)
            {
                WriteSinglePixelOfXLine(pVRam, x_left, x_right, [&](WORD* pOut) 
                {
                    const WORD clut_pixel = pClut_src_BD3270[*((unsigned __int8 *)pTPage_src_BD32C8 + ((signed int)(u_pos + (v_pos & 0x1FE00000)) >> 10))];
                    if (clut_pixel)
                    {
                        *pOut = clut_pixel;
                    }
                    u_pos += u_diff;
                    v_pos += v_diff;
                });
            }
            else if (sTexture_mode_BD0F14 == TextureModes::e16Bit)
            {
                const int k255_s20 = 255 << (20);

                if (sActiveTPage_578318 >= 0)
                {
                    WriteSinglePixelOfXLine(pVRam, x_left, x_right, [&](WORD* pOut)
                    {
                        const WORD tpage_pixel = pTPage_src_BD32C8[(u_pos + (k255_s20 & v_pos)) >> 10];
                        if (tpage_pixel)
                        {
                            *pOut = tpage_pixel;
                        }
                        u_pos += u_diff;
                        v_pos += v_diff;
                    });
                }
                else
                {
                    if (x_diff == 1)
                    {
                        pVRam[x_left] = pTPage_src_BD32C8[(pLeft->field_14_u + (k255_s20 & (pLeft->field_18_v << 10))) >> 10];
                    }
                    else
                    {
                        WriteSinglePixelOfXLine(pVRam, x_left, x_right, [&](WORD* pOut)
                        {
                            const unsigned int tpage_idx = (u_pos + (k255_s20 & v_pos)) >> 10;
                            *pOut = pTPage_src_BD32C8[tpage_idx];
                            u_pos += u_diff;
                            v_pos += v_diff;
                        });
                    }
                }
            }
            else if (sTexture_mode_BD0F14 == TextureModes::e4Bit)
            {
                WriteSinglePixelOfXLine(pVRam, x_left, x_right, [&](WORD* pOut)
                {
                    const unsigned int tpage_nibbles = *((unsigned __int8 *)pTPage_src_BD32C8 + ((signed int)(u_pos + (v_pos & 0x3FC00000)) >> 11));
                    
                    unsigned int nibble = 0;
                    if (u_pos & 0x400)
                    {
                        nibble = tpage_nibbles >> 4;
                    }
                    else
                    {
                        nibble = tpage_nibbles & 0xF;
                    }
                    
                    const WORD clut_pixel = pClut_src_BD3270[nibble];
                    if (clut_pixel)
                    {
                        *pOut = clut_pixel;
                    }

                    u_pos += u_diff;
                    v_pos += v_diff;
                });
            }
        }

        left_side_BD3320.field_0_x += slope_1_BD3200.field_0_x;
        left_side_BD3320.field_14_u += slope_1_BD3200.field_14_u;
        left_side_BD3320.field_18_v += slope_1_BD3200.field_18_v;

        right_side_BD32A0.field_0_x += slope_2_BD32E0.field_0_x;
        right_side_BD32A0.field_14_u += slope_2_BD32E0.field_14_u;
        right_side_BD32A0.field_18_v += slope_2_BD32E0.field_18_v;

        pVRam += pitch;
    }
}

// TODO: Refactor/remove duplication
EXPORT void CC PSX_EMU_Render_Polys_Textured_NoBlending_Opaque_51E140(WORD* pVRam, int ySize)
{
    const unsigned int pitch = (unsigned int)spBitmap_C2D038->field_10_locked_pitch / sizeof(WORD);
    Render_Unknown* pLeft = &left_side_BD3320;
    Render_Unknown* pRight = &right_side_BD32A0;

    for (int i = 0; i < ySize; i++)
    {
        if (pLeft->field_0_x > pRight->field_0_x)
        {
            std::swap(pLeft, pRight);
        }

        const int x_right = pRight->field_0_x >> 16;
        const int x_left = pLeft->field_0_x >> 16;

        const int x_diff = x_right - x_left;
        if (x_diff > 0)
        {
            DWORD u_left = pLeft->field_14_u;
            const DWORD left_v = pLeft->field_18_v;
            int x_diff_m1 = x_diff - 1;
            if (x_diff_m1 <= 0)
            {
                x_diff_m1 = 1;
            }
            const int u_diff = (signed int)(pRight->field_14_u - u_left) / x_diff_m1;
            const int v_diff = (signed int)(pRight->field_18_v - left_v) / x_diff_m1;

            if (sTexture_mode_BD0F14 == TextureModes::e8Bit)
            {
                DWORD left_v_fixed = left_v << 11;
                WORD* pStart = &pVRam[x_left];
                for (WORD* pEnd = &pVRam[x_right]; pStart < pEnd; left_v_fixed += v_diff << 11)
                {
                    const int clut_idx = *((unsigned __int8 *)pTPage_src_BD32C8 + ((signed int)(u_left + (left_v_fixed & 0x1FE00000)) >> 10));
                    if (pClut_src_BD3270[clut_idx])
                    {
                        *pStart =
                            b_lut_dword_BD3348[pClut_src_BD3270[clut_idx] & 0x1F]
                            | r_lut_dword_BD3308[(pClut_src_BD3270[clut_idx] >> 11) & 0x1F]
                            | g_lut_dword_BD32D8[(pClut_src_BD3270[clut_idx] >> 6) & 0x1F];
                    }
                    ++pStart;
                    u_left += u_diff;
                }
            }
            else if (sTexture_mode_BD0F14 == TextureModes::e16Bit)
            {
                const int k_255_sr10_p10 = 255 << (10 + 10);

                if (sActiveTPage_578318 >= 0)
                {
                    const DWORD v_left = pLeft->field_18_v;

                    WORD* pStart = &pVRam[x_left];
                    WORD* pEnd2 = &pVRam[x_right];
                    DWORD v_current = v_left << 10;
                    int v_pos = v_diff << 10;
                    while (pStart < pEnd2)
                    {
                        if (pTPage_src_BD32C8[(u_left + (k_255_sr10_p10 & v_current)) >> 10])
                        {
                            *pStart =
                                b_lut_dword_BD3348[(pTPage_src_BD32C8[(u_left + (k_255_sr10_p10 & v_current)) >> 10] & 0x1F)]
                                | r_lut_dword_BD3308[(pTPage_src_BD32C8[(u_left + (k_255_sr10_p10 & v_current)) >> 10] >> 11) & 0x1F]
                                | g_lut_dword_BD32D8[(pTPage_src_BD32C8[(u_left + (k_255_sr10_p10 & v_current)) >> 10] >> 6) & 0x1F];
                        }
                        u_left += u_diff;
                        ++pStart;
                        v_current += v_pos;
                    }
                }
                else
                {
                    WORD* pStart = &pVRam[x_left];
                    if (x_diff == 1)
                    {
                        const DWORD v_left = pLeft->field_18_v;
                        const WORD tpage_pixel = pTPage_src_BD32C8[(u_left + (k_255_sr10_p10 & (v_left << 10))) >> 10];
                        *pStart =
                            b_lut_dword_BD3348[tpage_pixel & 0x1F]
                            | r_lut_dword_BD3308[(tpage_pixel >> 11) & 0x1F]
                            | g_lut_dword_BD32D8[(tpage_pixel >> 6) & 0x1F];
                    }
                    else
                    {
                        const DWORD v_left = pLeft->field_18_v;
                        DWORD v_diff_fixed = v_left << 10;
                        const int u_diff_fixed = v_diff << 10;
                        WORD* pEnd = &pVRam[x_right];
                        if (pStart < pEnd)
                        {
                            while (pStart < pEnd)
                            {
                                ++pStart;
                                const WORD tpage_pixel = pTPage_src_BD32C8[(u_left + (k_255_sr10_p10 & v_diff_fixed)) >> 10];
                                const WORD pixel_value =
                                    b_lut_dword_BD3348[tpage_pixel & 0x1F]
                                    | r_lut_dword_BD3308[(tpage_pixel >> 11) & 0x1F]
                                    | g_lut_dword_BD32D8[(tpage_pixel >> 6) & 0x1F];
                                v_diff_fixed += u_diff_fixed;
                                *(pStart - 1) = pixel_value;
                                u_left += u_diff;
                            }
                        }
                    }
                }
            }
            else if (sTexture_mode_BD0F14 == TextureModes::e4Bit)
            {
                const DWORD v_left = pLeft->field_18_v;
                DWORD v_fixed_1 = v_left << 12;
                WORD* pStart = &pVRam[x_left];

                int v_fixed = v_diff << 12;
                const int _v_fixed_2 = v_fixed;
                while (pStart < &pVRam[x_right])
                {
                    const unsigned int tpage_pixel_1 = *((unsigned __int8 *)pTPage_src_BD32C8 + ((signed int)(u_left + (v_fixed_1 & 0x3FC00000)) >> 11));
                    unsigned int nibble = 0;
                    if (u_left & 0x400)
                    {
                        nibble = tpage_pixel_1 >> 4;
                    }
                    else
                    {
                        nibble = tpage_pixel_1 & 0xF;
                    }
                    const WORD clut_pixel = pClut_src_BD3270[nibble];
                    if (clut_pixel)
                    {
                        v_fixed = _v_fixed_2;
                        *pStart =
                            b_lut_dword_BD3348[(clut_pixel & 0x1F)]
                            | r_lut_dword_BD3308[(clut_pixel >> 11) & 0x1F]
                            | g_lut_dword_BD32D8[(clut_pixel >> 6) & 0x1F];
                    }
                    ++pStart;
                    u_left += u_diff;
                    v_fixed_1 += v_fixed;
                }
            }
        }

        left_side_BD3320.field_0_x += slope_1_BD3200.field_0_x;
        left_side_BD3320.field_14_u += slope_1_BD3200.field_14_u;
        left_side_BD3320.field_18_v += slope_1_BD3200.field_18_v;

        right_side_BD32A0.field_0_x += slope_2_BD32E0.field_0_x;
        right_side_BD32A0.field_14_u += slope_2_BD32E0.field_14_u;
        right_side_BD32A0.field_18_v += slope_2_BD32E0.field_18_v;

        pVRam += pitch;
    }
}

static inline WORD Calc_Abr_Pixel(const Psx_Test& abr_lut, WORD vram_pixel, WORD clut_pixel)
{
    return abr_lut.b[clut_pixel & 0x1F][vram_pixel & 0x1F]
        | abr_lut.r[(clut_pixel >> 11) & 0x1F][(vram_pixel >> 11) & 0x1F]
        | abr_lut.g[(clut_pixel >> 6) & 0x1F][(vram_pixel >> 6) & 0x1F];
}

EXPORT void CC PSX_EMU_Render_Polys_Textured_NoBlending_SemiTrans_51E890(WORD* pVRam, int ySize)
{
    const unsigned int pitch = (unsigned int)spBitmap_C2D038->field_10_locked_pitch / sizeof(WORD);

    const Render_Unknown* pLeft = &left_side_BD3320;
    const Render_Unknown* pRight = &right_side_BD32A0;
    
    const Psx_Test& abr_lut = sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18];

    for (int i = 0; i < ySize; i++)
    {
        if (pLeft->field_0_x > pRight->field_0_x)
        {
            std::swap(pLeft, pRight);
        }
        const int x_right = pRight->field_0_x >> 16;
        const int x_left = pLeft->field_0_x >> 16;
        const int x_diff = x_right - x_left;

        if (x_diff > 0)
        {
            int x_diff_m1 = x_right - x_left - 1;
            // Prevent divide by zero
            if (x_diff_m1 <= 0)
            {
                x_diff_m1 = 1;
            }

            int kFixedPointFactor = 0;
            switch (sTexture_mode_BD0F14)
            {
            case TextureModes::e16Bit:
                kFixedPointFactor = 1024;
                break;
            case TextureModes::e8Bit:
                kFixedPointFactor = 2048;
                break;
            case TextureModes::e4Bit:
                kFixedPointFactor = 4096;
                break;
            }

            const int u_diff = (signed int)(pRight->field_14_u - pLeft->field_14_u) / x_diff_m1;
            int u_pos = pLeft->field_14_u;

            const int v_diff = ((signed int)(pRight->field_18_v - pLeft->field_18_v) / x_diff_m1) * kFixedPointFactor;
            int v_pos = pLeft->field_18_v * kFixedPointFactor;

            if (sTexture_mode_BD0F14 == TextureModes::e8Bit)
            {
                WriteSinglePixelOfXLine(pVRam, x_left, x_right, [&](WORD* pOut)
                {
                    const WORD clut_pixel = pClut_src_BD3270[*((unsigned __int8 *)pTPage_src_BD32C8 + ((signed int)(u_pos + (v_pos & 0x1FE00000)) >> 10))];
                    if (clut_pixel)
                    {
                        WORD rLut = r_lut_dword_BD3308[(clut_pixel >> 11) & 0x1F];
                        WORD gLut = g_lut_dword_BD32D8[(clut_pixel >> 6) & 0x1F];
                        WORD bLut = b_lut_dword_BD3348[(clut_pixel & 0x1F)];

                        WORD pixel_value = rLut | gLut | bLut;

                        if (clut_pixel & 0x20)
                        {
                            pixel_value = Calc_Abr_Pixel(abr_lut, *pOut, pixel_value);
                        }
                        *pOut = pixel_value;
                    }
                    u_pos += u_diff;
                    v_pos += v_diff;
                });
            }
            else if (sTexture_mode_BD0F14 == TextureModes::e16Bit)
            {
                WriteSinglePixelOfXLine(pVRam, x_left, x_right, [&](WORD* pOut)
                {
                    const WORD texture_pixel = pTPage_src_BD32C8[(u_pos + (v_pos & 0x0ff00000)) >> 10];
                    if (texture_pixel)
                    {
                        const WORD rLut = r_lut_dword_BD3308[(texture_pixel >> 11) & 0x1F];
                        const WORD gLut = g_lut_dword_BD32D8[(texture_pixel >> 6) & 0x1F];
                        const WORD bLut = b_lut_dword_BD3348[texture_pixel & 0x1F];

                        WORD pixel = rLut | gLut | bLut;
                        if (texture_pixel & 0x20)
                        {
                            pixel = Calc_Abr_Pixel(abr_lut, *pOut, pixel);
                        }

                        *pOut = pixel;
                    }

                    u_pos += u_diff;
                    v_pos += v_diff;
                });
            }
            else if (sTexture_mode_BD0F14 == TextureModes::e4Bit)
            {
                WriteSinglePixelOfXLine(pVRam, x_left, x_right, [&](WORD* pOut)
                {
                    const WORD nibbles = *((unsigned __int8 *)pTPage_src_BD32C8 + ((signed int)(u_pos + (v_pos & 0x3FC00000)) >> 11));
                    WORD nibble = 0;
                    if (u_pos & 0x400)
                    {
                        nibble = nibbles >> 4;
                    }
                    else
                    {
                        nibble = nibbles & 0xF;
                    }
                    const WORD clut_pixel_1 = pClut_src_BD3270[nibble];

                    if (clut_pixel_1)
                    {
                        WORD rLut = r_lut_dword_BD3308[(clut_pixel_1 >> 11) & 0x1F];
                        WORD gLut = g_lut_dword_BD32D8[(clut_pixel_1 >> 6) & 0x1F];
                        WORD bLut = b_lut_dword_BD3348[(clut_pixel_1 & 0x1F)];

                        WORD pixel4 = rLut | gLut | bLut;

                        if (clut_pixel_1 & 0x20)
                        {
                            pixel4 = Calc_Abr_Pixel(abr_lut, *pOut, pixel4);
                        }

                        *pOut = pixel4;
                    }
                    
                    u_pos += u_diff;
                    v_pos += v_diff;
                });
            }
        }

        left_side_BD3320.field_0_x += slope_1_BD3200.field_0_x;
        left_side_BD3320.field_14_u += slope_1_BD3200.field_14_u;
        left_side_BD3320.field_18_v += slope_1_BD3200.field_18_v;

        right_side_BD32A0.field_0_x += slope_2_BD32E0.field_0_x;
        right_side_BD32A0.field_14_u += slope_2_BD32E0.field_14_u;
        right_side_BD32A0.field_18_v += slope_2_BD32E0.field_18_v;

        pVRam += pitch;
    }
}

EXPORT void CC PSX_EMU_Render_Polys_Textured_Unknown_Opqaue_51D890(WORD* /*a1*/, int /*a2*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_EMU_Render_Polys_FShaded_NoTexture_Opqaue_51C4C0(WORD* pVram, int ySize)
{
    Render_Unknown* pLeft = &left_side_BD3320;
    Render_Unknown* pRight = &right_side_BD32A0;

    const unsigned int width_pitch = ((unsigned int)spBitmap_C2D038->field_10_locked_pitch) / sizeof(WORD);
    for (int i = 0; i < ySize; i++)
    {
        if (pLeft->field_0_x > pRight->field_0_x)
        {
            Render_Unknown* pTmpLeft = pLeft;
            pLeft = pRight;
            pRight = pTmpLeft;
        }

        WORD* pDst = &pVram[pLeft->field_0_x >> 16];
        WORD* pEnd = &pVram[pRight->field_0_x >> 16];
        while (pDst < pEnd)
        {
            *pDst = sPoly_fill_colour_BD3350;
            pDst++;
        }

        pVram = &pVram[width_pitch];
        left_side_BD3320.field_0_x += slope_1_BD3200.field_0_x;
        right_side_BD32A0.field_0_x += slope_2_BD32E0.field_0_x;
    }
}

EXPORT void CC PSX_EMU_Render_Polys_FShaded_NoTexture_SemiTrans_51C590(WORD* pVRam, int ySize)
{
    const auto lut_b = &sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18].b[(sPoly_fill_colour_BD3350 & 0x1F)][0];
    const auto lut_r = &sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18].r[(sPoly_fill_colour_BD3350 >> 11) & 0x1F][0];
    const auto lut_g = &sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18].g[(sPoly_fill_colour_BD3350 >> 6) & 0x1F][0];

    const unsigned int pitch = (unsigned int)spBitmap_C2D038->field_10_locked_pitch / sizeof(WORD);
    const Render_Unknown* pLeft1 = &left_side_BD3320;
    const Render_Unknown* pRight1 = &right_side_BD32A0;

    for (int i = 0; i < ySize; i++)
    {
        if (pLeft1->field_0_x > pRight1->field_0_x)
        {
            const Render_Unknown* pLeft1Temp = pLeft1;
            pLeft1 = pRight1;
            pRight1 = pLeft1Temp;
        }

        WORD* pStart = &pVRam[pLeft1->field_0_x >> 16];
        WORD* pEnd =   &pVRam[pRight1->field_0_x >> 16];
        while (pStart < pEnd)
        {
            const WORD vram_pixel = *pStart;
            *pStart = lut_b[vram_pixel & 0x1F] | lut_r[(vram_pixel >> 11) & 0x1F] | lut_g[(vram_pixel >> 6) & 0x1F];
            ++pStart;
        }

        left_side_BD3320.field_0_x += slope_1_BD3200.field_0_x;
        right_side_BD32A0.field_0_x += slope_2_BD32E0.field_0_x;
        pVRam += pitch;
    }
}

EXPORT void CC PSX_EMU_Render_Polys_Textured_Blending_SemiTrans_51D2B0(WORD* pVram, int ySize)
{
    const Render_Unknown* pRight = &right_side_BD32A0;
    const Render_Unknown* pLeft = &left_side_BD3320;

    const unsigned int pitch = (unsigned int)spBitmap_C2D038->field_10_locked_pitch / sizeof(WORD);
    const Psx_Test& abr_lut = sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18];
    for (int i = 0; i < ySize; i++)
    {
        if (pLeft->field_0_x > pRight->field_0_x)
        {
            std::swap(pLeft, pRight);
        }
        const int x_right = pRight->field_0_x >> 16;
        const int x_left = pLeft->field_0_x >> 16;
        if (x_right - x_left > 0)
        {
            int x_diff_m1 = x_right - x_left - 1;
            if (x_diff_m1 <= 0)
            {
                x_diff_m1 = 1;
            }

            const int u_diff = (signed int)(pRight->field_14_u - pLeft->field_14_u) / x_diff_m1;
            DWORD u_pos = pLeft->field_14_u;

            WORD* pStart = &pVram[x_left];
            WORD* pEnd = &pVram[x_right];

            if (sTexture_mode_BD0F14 == TextureModes::e8Bit)
            {
                const int v_diff = ((signed int)(pRight->field_18_v - pLeft->field_18_v) / x_diff_m1) * 2048;
                DWORD v_pos = pLeft->field_18_v * 2048;
                while (pStart < pEnd)
                {
                    // TODO: Refactor
                    const int clut_idx = *((unsigned __int8 *)pTPage_src_BD32C8 + ((signed int)(u_pos + (v_pos & 0x1FE00000)) >> 10));
                    if (pClut_src_BD3270[clut_idx] != 0) // Black pixels are transparent
                    {
                        if (pClut_src_BD3270[clut_idx] & 0x20) // unknown flag
                        {
                            *pStart = Calc_Abr_Pixel(abr_lut, *pStart, pClut_src_BD3270[clut_idx]);
                        }
                        else
                        {
                            *pStart = pClut_src_BD3270[clut_idx];
                        }
                    }
                    u_pos += u_diff;
                    v_pos += v_diff;
                    ++pStart;
                }
            }
            else if (sTexture_mode_BD0F14 == TextureModes::e16Bit)
            {
                int v_diff = ((signed int)(pRight->field_18_v - pLeft->field_18_v) / x_diff_m1) * 1024;
                DWORD v_pos = pLeft->field_18_v * 1024;

                while (pStart < pEnd)
                {
                    // TODO: Refactor
                    const DWORD tpage_idx = (u_pos + (v_pos & 0x0ff00000)) / 1024;
                    if (pTPage_src_BD32C8[tpage_idx] != 0) // Black pixels are transparent
                    {
                        *pStart = Calc_Abr_Pixel(abr_lut, *pStart, pTPage_src_BD32C8[tpage_idx]);
                    }
                    ++pStart;
                    u_pos += u_diff;
                    v_pos += v_diff;
                }
            }
            else if (sTexture_mode_BD0F14 == TextureModes::e4Bit)
            {
                int v_diff = ((signed int)(pRight->field_18_v - pLeft->field_18_v) / x_diff_m1) * 4096;
                DWORD v_pos = pLeft->field_18_v * 4096;
                while (pStart < pEnd)
                {
                    // TODO: Refactor
                    const int clut_idx = (*((unsigned __int8 *)pTPage_src_BD32C8 + ((signed int)(u_pos + (v_pos & 0x3FC00000)) >> 11)) >> (BYTE1(u_pos) & 4)) & 0xF;
                    if (pClut_src_BD3270[clut_idx] != 0) // Black pixels are transparent
                    {
                        *pStart = Calc_Abr_Pixel(abr_lut, *pStart, pClut_src_BD3270[clut_idx]);
                    }
                    ++pStart;
                    u_pos += u_diff;
                    v_pos += v_diff;
                }
            }
        }

        left_side_BD3320.field_0_x += slope_1_BD3200.field_0_x;
        left_side_BD3320.field_14_u += slope_1_BD3200.field_14_u;
        left_side_BD3320.field_18_v += slope_1_BD3200.field_18_v;

        right_side_BD32A0.field_18_v += slope_2_BD32E0.field_18_v;
        right_side_BD32A0.field_0_x += slope_2_BD32E0.field_0_x;
        right_side_BD32A0.field_14_u += slope_2_BD32E0.field_14_u;

        pVram += pitch;
    }
}

EXPORT void CC PSX_EMU_Render_Polys_Textured_Unknown_SemiTrans_51DC90(WORD* /*a1*/, int /*a2*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_EMU_Render_Polys_GShaded_NoTexture_Opqaue_51C6E0(WORD* pVRam, int ySize)
{
    const Render_Unknown* pLeft = &left_side_BD3320;
    const Render_Unknown* pRight = &right_side_BD32A0;
    const unsigned int pitch = (unsigned int)spBitmap_C2D038->field_10_locked_pitch / sizeof(WORD);

    for (int i = 0; i < ySize; i++)
    {
        if (pLeft->field_0_x > pRight->field_0_x)
        {
            const Render_Unknown* leftTemp = pLeft;
            pLeft = pRight;
            pRight = leftTemp;
        }

        const int xdiff_f = (pRight->field_0_x >> 16) - (pLeft->field_0_x >> 16);
        if (xdiff_f > 0)
        {
            int shade_r = pLeft->field_1C_GShadeR;
            int shade_g = pLeft->field_20_GShadeG;
            int shade_b = pLeft->field_24_GShadeB;

            const int r_scaled = PSX_poly_helper_fixed_point_scale_517FA0(pRight->field_1C_GShadeR - shade_r, sPsxEmu_fixed_point_table_C1D5C0[xdiff_f + 1]);
            const int g_scaled = PSX_poly_helper_fixed_point_scale_517FA0(pRight->field_20_GShadeG - shade_g, sPsxEmu_fixed_point_table_C1D5C0[xdiff_f + 1]);
            const int b_scaled = PSX_poly_helper_fixed_point_scale_517FA0(pRight->field_24_GShadeB - shade_b, sPsxEmu_fixed_point_table_C1D5C0[xdiff_f + 1]);
            
            WORD* pStart = &pVRam[pLeft->field_0_x >> 16];
            WORD* pEnd = &pVRam[pRight->field_0_x >> 16];
            while (pStart < pEnd)
            {
                *pStart = ((shade_b >> 16) & 0x1F) | ((shade_g >> 10) & 0x7C0) | ((shade_r >> 5) & 0xF800);
                ++pStart;
                shade_r += r_scaled;
                shade_g += g_scaled;
                shade_b += b_scaled;
            }
        }

        left_side_BD3320.field_0_x += slope_1_BD3200.field_0_x;
        left_side_BD3320.field_1C_GShadeR += slope_1_BD3200.field_1C_GShadeR;
        left_side_BD3320.field_20_GShadeG += slope_1_BD3200.field_20_GShadeG;
        left_side_BD3320.field_24_GShadeB += slope_1_BD3200.field_24_GShadeB;

        right_side_BD32A0.field_0_x += slope_2_BD32E0.field_0_x;
        right_side_BD32A0.field_1C_GShadeR += slope_2_BD32E0.field_1C_GShadeR;
        right_side_BD32A0.field_20_GShadeG += slope_2_BD32E0.field_20_GShadeG;
        right_side_BD32A0.field_24_GShadeB += slope_2_BD32E0.field_24_GShadeB;

        pVRam += pitch;
    }
}

EXPORT void CC PSX_EMU_Render_Polys_GShaded_NoTexture_SemiTrans_51C8D0(WORD* pVram, int yCount)
{
    const unsigned int pitch = (unsigned int)spBitmap_C2D038->field_10_locked_pitch / sizeof(WORD);

    const auto r_table = sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18].r;
    const auto g_table = sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18].g;
    const auto b_table = sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18].b;

    const Render_Unknown* pLeft = &left_side_BD3320;
    const Render_Unknown* pRight = &right_side_BD32A0;

    for (int i = 0; i < yCount; i++)
    {
        if (pLeft->field_0_x > pRight->field_0_x)
        {
            const Render_Unknown* leftTemp = pLeft;
            pLeft = pRight;
            pRight = leftTemp;
        }

        const int xdiff_f = (pRight->field_0_x >> 16) - (pLeft->field_0_x >> 16);
        if (xdiff_f > 0)
        {
            int shade_r = pLeft->field_1C_GShadeR;
            int shade_g = pLeft->field_20_GShadeG;
            int shade_b = pLeft->field_24_GShadeB;

            const int r_scaled = PSX_poly_helper_fixed_point_scale_517FA0(pRight->field_1C_GShadeR - shade_r, sPsxEmu_fixed_point_table_C1D5C0[xdiff_f + 1]);
            const int g_scaled = PSX_poly_helper_fixed_point_scale_517FA0(pRight->field_20_GShadeG - shade_g, sPsxEmu_fixed_point_table_C1D5C0[xdiff_f + 1]);
            const int b_scaled = PSX_poly_helper_fixed_point_scale_517FA0(pRight->field_24_GShadeB - shade_b, sPsxEmu_fixed_point_table_C1D5C0[xdiff_f + 1]);

            WORD* pStart = &pVram[pLeft->field_0_x  >> 16];
            WORD* pEnd =   &pVram[pRight->field_0_x >> 16];

            while (pStart < pEnd)
            {
                const WORD existingPixel = *pStart;
                *pStart =
                      (unsigned __int16)b_table[(shade_b >> 16) & 0x1F][existingPixel & 0x1F]
                    | (unsigned __int16)r_table[(shade_r >> 16) & 0x1F][(existingPixel >> 11) & 0x1F]
                    | (unsigned __int16)g_table[(shade_g >> 16) & 0x1F][(existingPixel >> 6) & 0x1F];

                shade_r += r_scaled;
                shade_g += g_scaled;
                shade_b += b_scaled;

                pStart++;
            }
        }

        left_side_BD3320.field_0_x += slope_1_BD3200.field_0_x;
        left_side_BD3320.field_1C_GShadeR += slope_1_BD3200.field_1C_GShadeR;
        left_side_BD3320.field_20_GShadeG += slope_1_BD3200.field_20_GShadeG;
        left_side_BD3320.field_24_GShadeB += slope_1_BD3200.field_24_GShadeB;

        right_side_BD32A0.field_0_x += slope_2_BD32E0.field_0_x;
        right_side_BD32A0.field_24_GShadeB += slope_2_BD32E0.field_24_GShadeB;
        right_side_BD32A0.field_1C_GShadeR += slope_2_BD32E0.field_1C_GShadeR;
        right_side_BD32A0.field_20_GShadeG += slope_2_BD32E0.field_20_GShadeG;

        pVram += pitch;
    }
}

ALIVE_VAR(1, 0xC2D04C, decltype(&PSX_EMU_Render_SPRT_51EF90), pPSX_EMU_Render_SPRT_51EF90_C2D04C, nullptr);
ALIVE_VAR(1, 0xBD3364, decltype(&PSX_EMU_Render_Polys_Textured_Blending_Opqaue_51CCA0), pPSX_EMU_51CCA0_BD3364, nullptr);
ALIVE_VAR(1, 0xBD328C, decltype(&PSX_EMU_Render_Polys_Textured_NoBlending_Opaque_51E140), pPSX_EMU_51E140_BD328C, nullptr);
ALIVE_VAR(1, 0xBD3360, decltype(&PSX_EMU_Render_Polys_Textured_Unknown_Opqaue_51D890), pPSX_EMU_51D890_BD3360, nullptr);
ALIVE_VAR(1, 0xBD32D4, decltype(&PSX_EMU_Render_Polys_FShaded_NoTexture_Opqaue_51C4C0), pPSX_EMU_51C4C0_BD32D4, nullptr);
ALIVE_VAR(1, 0xBD32D0, decltype(&PSX_EMU_Render_Polys_GShaded_NoTexture_Opqaue_51C6E0), pPSX_EMU_51C6E0_BD32D0, nullptr);
ALIVE_VAR(1, 0xBD327C, decltype(&PSX_EMU_Render_Polys_Textured_Blending_SemiTrans_51D2B0), pPSX_EMU_51D2B0_BD327C, nullptr);
ALIVE_VAR(1, 0xBD326C, decltype(&PSX_EMU_Render_Polys_Textured_NoBlending_SemiTrans_51E890), pPSX_EMU_51E890_BD326C, nullptr);
ALIVE_VAR(1, 0xBD3274, decltype(&PSX_EMU_Render_Polys_Textured_Unknown_SemiTrans_51DC90), pPSX_EMU_51DC90_BD3274, nullptr);
ALIVE_VAR(1, 0xBD3354, decltype(&PSX_EMU_Render_Polys_FShaded_NoTexture_SemiTrans_51C590), pPSX_EMU_51C590_BD3354, nullptr);
ALIVE_VAR(1, 0xBD335C, decltype(&PSX_EMU_Render_Polys_GShaded_NoTexture_SemiTrans_51C8D0), pPSX_EMU_51C8D0_BD335C, nullptr);

ALIVE_VAR(1, 0xc215c0, DWORD, sSemiTransShift_C215C0, 0);
ALIVE_VAR(1, 0xc215c4, DWORD, sRedShift_C215C4, 0);
ALIVE_VAR(1, 0xc1d180, DWORD, sGreenShift_C1D180, 0);
ALIVE_VAR(1, 0xc19140, DWORD, sBlueShift_C19140, 0);

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
    pPSX_EMU_51CCA0_BD3364 = PSX_EMU_Render_Polys_Textured_Blending_Opqaue_51CCA0;
    pPSX_EMU_51E140_BD328C = PSX_EMU_Render_Polys_Textured_NoBlending_Opaque_51E140;
    pPSX_EMU_51D890_BD3360 = PSX_EMU_Render_Polys_Textured_Unknown_Opqaue_51D890;
    pPSX_EMU_51C4C0_BD32D4 = PSX_EMU_Render_Polys_FShaded_NoTexture_Opqaue_51C4C0;
    pPSX_EMU_51C6E0_BD32D0 = PSX_EMU_Render_Polys_GShaded_NoTexture_Opqaue_51C6E0;
    pPSX_EMU_51D2B0_BD327C = PSX_EMU_Render_Polys_Textured_Blending_SemiTrans_51D2B0;
    pPSX_EMU_51E890_BD326C = PSX_EMU_Render_Polys_Textured_NoBlending_SemiTrans_51E890;
    pPSX_EMU_51DC90_BD3274 = PSX_EMU_Render_Polys_Textured_Unknown_SemiTrans_51DC90;
    pPSX_EMU_51C590_BD3354 = PSX_EMU_Render_Polys_FShaded_NoTexture_SemiTrans_51C590;
    pPSX_EMU_51C8D0_BD335C = PSX_EMU_Render_Polys_GShaded_NoTexture_SemiTrans_51C8D0;

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


EXPORT void CC PSX_Pal_Conversion_4F98D0(const WORD* pDataToConvert, WORD* pConverted, unsigned int size)
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

EXPORT void CC Add_Dirty_Area_4ED970(int /*x*/, int /*y*/, int /*w*/, int /*h*/)
{
    NOT_IMPLEMENTED();
}

template <typename T>
T clip(const T& n, const T& lower, const T& upper) 
{
    return std::max(lower, std::min(n, upper));
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

EXPORT int CC PSX_ClearImage_4F5BD0(const PSX_RECT* pRect, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b)
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

    WORD* pVram = reinterpret_cast<WORD*>(sPsxVram_C1D160.field_4_pLockedPixels) + (rect_x1 + (rect_y1 * pitch_words));

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
        sOtIdxRollOver_BD0C08 = ((sOtIdxRollOver_BD0C08 + 1) & 31);
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

EXPORT void CC PSX_4F6ED0(WORD* /*pVRam*/, int /*width*/, int /*height*/, int /*r*/, int /*g*/, int /*b*/, int /*pitch*/)
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

static inline BYTE Decompress_Next_Type3(int& control_byte, unsigned int& dstIdx, const WORD*& pCompressed)
{
    if (control_byte == 0)
    {
        dstIdx = *(DWORD *)pCompressed;
        control_byte = 32;
        pCompressed += 2;
    }
    else if (control_byte == 14)
    {
        control_byte = 30;
        dstIdx |= *pCompressed << 14;
        ++pCompressed;
    }

    control_byte = control_byte - 6;
    const BYTE data = dstIdx & 0x3F;
    dstIdx = dstIdx >> 6;
    return data;
}


static inline BYTE Decompress_Next_Type6(int& control_byte, unsigned int& dstIdx, const WORD*& pCompressed)
{
    if (control_byte == 0)
    {
        dstIdx = *(DWORD *)pCompressed;
        control_byte = 32;
        pCompressed += 2;
    }

    control_byte = control_byte - 4;
    const BYTE data = dstIdx & 0xF;
    dstIdx = dstIdx >> 4;
    return data;
}

template<class TfnWritePixel, class TfnConvertPixel, class TfnDecompress>
static void Scaled_Poly_FT4_Inline_Texture_Render(
    int xpos_clip,
    int ypos_clip,
    int width_clip,
    int height_clip,
    int v_height,
    int u_width,
    int width,
    int height,
    WORD* pVramDst, 
    signed int vram_pitch, 
    const WORD* pCompressedIter,
    const WORD* pClut,
    int bytesToNextPixel,
    TfnWritePixel fnWritePixel,
    TfnConvertPixel fnConvertPixel,
    TfnDecompress fnDecompress)
{
    if (v_height <= 0 || width <=0 || height <= 0)
    {
        return;
    }

    const float texture_w_step = static_cast<float>(u_width) / static_cast<float>(width);
    const float texture_h_step = static_cast<float>(v_height) / static_cast<float>(height);

    int control_byte = 0;
    unsigned int dstIdx = 0;

    int yCounter = 0;
    float v_pos = 0.0f;

    if ( (xpos_clip % 2) !=0)
    {
        // TODO: HACK HACK, some xpos rendering is off by 1
        pVramDst = pVramDst - 1;
    }
  

    for (int v_height_counter = 0; v_height_counter < v_height; v_height_counter++)
    {
        if (yCounter >= height_clip)
        {
            return;
        }

        WORD* pVramIter = pVramDst;

        // Move to next vpos
        int yDuplicateCount = 0;
        while (v_height_counter == static_cast<int>(v_pos))
        {
            v_pos += texture_h_step;
            yDuplicateCount++;
            pVramDst += (vram_pitch / sizeof(WORD));
        }

        yCounter += yDuplicateCount;

        if (yDuplicateCount > yCounter - ypos_clip)
        {
            // Move the vram pointer to match the vpos
            pVramIter += (vram_pitch / sizeof(WORD)) * (ypos_clip + yDuplicateCount - yCounter);
            yDuplicateCount = yCounter - ypos_clip;
        }

        // Limit the number of y lines to write if its going out of bounds
        if (yCounter > height_clip)
        {
            yDuplicateCount += height_clip - yCounter;
        }

        if (yDuplicateCount > 0)
        {
            // Write scan lines
            if (u_width < 0)
            {
                break;
            }

            int width_clip_counter = 0;
            float u_pos = 0.0f;
            int u_width_counter = 0;
            while (u_width_counter <= u_width)
            {
                if (width_clip_counter >= width_clip)
                {
                    // Skip end of line clipped area
                    while (u_width_counter <= u_width)
                    {
                        const int blackPixelCount = fnDecompress(control_byte, dstIdx, pCompressedIter);
                        int runLengthCount = fnDecompress(control_byte, dstIdx, pCompressedIter);

                        u_width_counter += runLengthCount + blackPixelCount;

                        while (runLengthCount > 0)
                        {
                            fnDecompress(control_byte, dstIdx, pCompressedIter);
                            runLengthCount--;
                        }
                    }
                    break;
                }
                else
                {
                    const int blackLengthCount = fnDecompress(control_byte, dstIdx, pCompressedIter);
                    const int totalWidthBytes = blackLengthCount + u_width_counter;
                    u_width_counter = totalWidthBytes;

                    // Write black pixels
                    while (totalWidthBytes > static_cast<int>(u_pos))
                    {
                        if (bytesToNextPixel == 2)
                        {
                            // Left to right
                            pVramIter++;
                        }
                        else
                        {
                            // Right to left
                            pVramIter--;
                        }
                        u_pos += texture_w_step;
                        width_clip_counter++;
                    }

                    // Write non black pixels
                    BYTE runLengthCopyCount = fnDecompress(control_byte, dstIdx, pCompressedIter);
                    for (int numBytesToCopy = 0; numBytesToCopy < runLengthCopyCount; numBytesToCopy++)
                    {
                        WORD* pVramXOff = pVramIter;

                        BYTE decompressed_byte = fnDecompress(control_byte, dstIdx, pCompressedIter);

                        int width_to_write = 0;
                        while (u_width_counter == static_cast<int>(u_pos))
                        {
                            u_pos += texture_w_step;
                            width_to_write++;
                        }

                        pVramIter += (bytesToNextPixel / sizeof(WORD)) * width_to_write;
                        width_clip_counter += width_to_write;

                        if (width_to_write > width_clip_counter - xpos_clip)
                        {
                            // Move the vram pointer to match the upos
                            WORD* pVramTmp = pVramXOff + (bytesToNextPixel / sizeof(WORD)) * (xpos_clip + width_to_write - width_clip_counter);
                            width_to_write = width_clip_counter - xpos_clip;
                            pVramXOff = pVramTmp;
                        }

                        // Limit if going to go out of bounds
                        if (width_clip_counter > width_clip)
                        {
                            width_to_write += width_clip - width_clip_counter;
                        }

                        u_width_counter++;

                        // Write out the scanline to vram
                        if (width_to_write > 0)
                        {
                            const WORD clut_pixel = pClut[decompressed_byte]; // Note: Clut data is already "converted"
                            if (fnWritePixel(clut_pixel))
                            {
                                WORD* pDstVRamLine = pVramXOff;
                                for (int yLinesToWrite = 0; yLinesToWrite < yDuplicateCount; yLinesToWrite++)
                                {
                                    for (int widthPixelsToWrite = 0; widthPixelsToWrite < width_to_write; widthPixelsToWrite++)
                                    {
                                        const WORD converted_pixel = fnConvertPixel(clut_pixel, *pDstVRamLine);
                                        *pDstVRamLine = converted_pixel;
                                        pDstVRamLine += (bytesToNextPixel / sizeof(WORD));
                                    }
                                    pDstVRamLine = &pVramXOff[vram_pitch / sizeof(WORD)];
                                    pVramXOff = pDstVRamLine;
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            // Skip Y lines
            int u_skip_counter = 0;
            while (u_skip_counter <= u_width)
            {
                const int blackPixelCount = fnDecompress(control_byte, dstIdx, pCompressedIter) + u_skip_counter;
                int runLengthCount = fnDecompress(control_byte, dstIdx, pCompressedIter);

                u_skip_counter = runLengthCount + blackPixelCount;

                while (runLengthCount > 0)
                {
                    fnDecompress(control_byte, dstIdx, pCompressedIter);
                    runLengthCount--;
                }
            }
        }
    }
}

template<size_t clut_size, class TFnDecompress>
static void PSX_Render_Poly_FT4_Direct_Impl(bool isSemiTrans, OT_Prim* pPrim, int width, int height, const void* pData, TFnDecompress fnDecompress)
{
    WORD clut_local[clut_size] = {};

    const WORD* pClut = (WORD *)((char *)sPsxVram_C1D160.field_4_pLockedPixels + 32 * ((pPrim->field_12_clut & 0x3F) + (pPrim->field_12_clut >> 6 << 6)));

    const __int16* lut_r = &stru_C146C0.r[pPrim->field_8_r >> 3][0];
    const __int16* lut_g = &stru_C146C0.g[pPrim->field_9_g >> 3][0];
    const __int16* lut_b = &stru_C146C0.b[pPrim->field_A_b >> 3][0];

    int skipBlackPixels = 0;
    int unknown = 1;

    if (pPrim->field_B_flags & 1)
    {
        for (int i = 1; i < 64; i++)
        {
            if (isSemiTrans)
            {
                if (!(pClut[i] & 0x20))
                {
                    unknown = 0;
                }
            }
            else
            {
                if (pClut[i] == 0)
                {
                    skipBlackPixels = 1;
                    break;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < clut_size; i++)
        {
            if (isSemiTrans)
            {
                if (pClut[i] == 0)
                {
                    clut_local[i] = 0;
                }
                else if (pClut[i] == 0x20)
                {
                    clut_local[i] = 0x20;
                }
                else
                {
                    const WORD clut_entry = pClut[i];
                    const DWORD bitSet = (clut_entry & 0x20);
                    WORD clut_pixel = lut_b[clut_entry & 0x1F] | lut_r[(clut_entry >> 11) & 0x1F] | lut_g[(clut_entry >> 6) & 0x1F];
                    clut_pixel |= bitSet;
                    clut_local[i] = clut_pixel;

                    if (clut_pixel == 0x20)
                    {
                        if (sTexture_page_abr_BD0F18 == BlendModes::eBlendMode_0)
                        {
                            clut_local[i] = 0x861;
                        }
                    }

                    if (!bitSet)
                    {
                        unknown = i == 0;
                    }
                }
            }
            else
            {
                if (pClut[i] == 0)
                {
                    clut_local[i] = 0;
                    skipBlackPixels = i == 0;
                }
                else if (pClut[i] == 0x20)
                {
                    clut_local[i] = 0x20;
                }
                else
                {
                    const WORD clut_entry = pClut[i];
                    const WORD clut_pixel = lut_b[clut_entry & 0x1F] | lut_r[(clut_entry >> 11) & 0x1F] | lut_g[(clut_entry >> 6) & 0x1F];
                    clut_local[i] = clut_pixel | (clut_entry & 0x20);
                }
            }
        }

        // Use the local/converted clut
        pClut = clut_local;
    }

    if (sTexture_page_abr_BD0F18 != BlendModes::eBlendMode_1 /*|| !dword_5CA4D4*/)
    {
        unknown = 0;
    }

    int height_clip = height;
    int width_clip = width;

    int ypos_clip = 0;
    int xpos_clip = 0;

    const int polyXPos = pPrim->field_14_verts[0].field_0_x0 / 16;
    const int polyYPos = pPrim->field_14_verts[0].field_4_y0 / 16;

    if (polyXPos < (sPsx_drawenv_clipx_BDCD40 / 16))
    {
        xpos_clip = (sPsx_drawenv_clipx_BDCD40 / 16) - polyXPos;
    }

    if (polyYPos < (sPsx_drawenv_clipy_BDCD44 / 16))
    {
        ypos_clip = (sPsx_drawenv_clipy_BDCD44 / 16) - polyYPos;
    }

    if (width > (sPsx_drawenv_clipw_BDCD48 / 16) - polyXPos + 1)
    {
        width_clip = (sPsx_drawenv_clipw_BDCD48 / 16) - polyXPos + 1;
    }

    if (height > (sPsx_drawenv_cliph_BDCD4C / 16) - polyYPos + 1)
    {
        height_clip = (sPsx_drawenv_cliph_BDCD4C / 16) - polyYPos + 1;
    }

    WORD* pVramStartPos = (WORD *)((char *)spBitmap_C2D038->field_4_pLockedPixels + (sizeof(WORD) * polyXPos) + (polyYPos * spBitmap_C2D038->field_10_locked_pitch));

    int u_height = pPrim->field_14_verts[2].field_14_u - pPrim->field_14_verts[0].field_14_u;
    int v_width = pPrim->field_14_verts[2].field_18_v - pPrim->field_14_verts[0].field_18_v;

    int bytesToNextPixel = 0;
    int vramPitchBytes = 0;
    if (u_height <= 0)
    {
        // Swap them
        u_height = pPrim->field_14_verts[0].field_14_u - pPrim->field_14_verts[2].field_14_u;
        bytesToNextPixel = -static_cast<int>(sizeof(WORD));
        vramPitchBytes = 1024 * sizeof(WORD);

        pVramStartPos += width;

        const int tmpWidthClip = width - xpos_clip + 1;
        xpos_clip = width - width_clip + 1;
        width_clip = tmpWidthClip;
    }
    else
    {
        bytesToNextPixel = sizeof(WORD);
        vramPitchBytes = 1024 * sizeof(WORD);
    }

    if (v_width < 0)
    {
        // Swap them
        v_width = pPrim->field_14_verts[0].field_18_v - pPrim->field_14_verts[2].field_18_v;
        vramPitchBytes = -static_cast<int>(1024 * sizeof(WORD));

        pVramStartPos = &pVramStartPos[1024 * height];

        const int tmpHeightClip = height_clip - ypos_clip;
        ypos_clip = height - height_clip + 1;
        height_clip = ypos_clip + tmpHeightClip;
    }

    Psx_Test* pAbrLut = &sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18];

    auto fnShouldWritePixel = [&](WORD clut_value)
    {
        if (isSemiTrans)
        {
            return true;
        }
        else
        {
            if (!skipBlackPixels && clut_value == 0)
            {
                return false;
            }
            return true;
        }
    };

    auto fnConvertPixel = [&](WORD clut_value, WORD vram_value)
    {
        if (isSemiTrans)
        {
            // TODO: Some cases ignore 0x20 flag
            if (unknown && isSemiTrans && !skipBlackPixels)
            {
                return Calc_Abr_Pixel(*pAbrLut, vram_value, clut_value);
            }
            
            if (clut_value & 0x20)
            {
                return Calc_Abr_Pixel(*pAbrLut, vram_value, clut_value);
            }
            else
            {
                return clut_value;
            }
        }
        return clut_value;
    };

    WORD* pCompressedIter = ((WORD*)pData) + 2;                  // skip w/h to get to compressed data
    Scaled_Poly_FT4_Inline_Texture_Render(
        xpos_clip,
        ypos_clip,
        width_clip,
        height_clip,
        v_width,
        u_height,
        width,
        height,
        pVramStartPos,
        vramPitchBytes,
        pCompressedIter,
        pClut,
        bytesToNextPixel,
        fnShouldWritePixel,
        fnConvertPixel,
        fnDecompress
    );
}

EXPORT void CC PSX_Render_PolyFT4_4bit_SemiTrans_50DF30(OT_Prim* pPrim, int width, int height, const void* pCompressed)
{
    PSX_Render_Poly_FT4_Direct_Impl<16>(true, pPrim, width, height, pCompressed, Decompress_Next_Type6);
}

EXPORT void CC PSX_Render_PolyFT4_4bit_Opqaue_50CDB0(OT_Prim* pPrim, int width, int height, const void* pCompressed)
{
    PSX_Render_Poly_FT4_Direct_Impl<16>(false, pPrim, width, height, pCompressed, Decompress_Next_Type6);
}

EXPORT void CC PSX_Render_PolyFT4_8bit_SemiTrans_501B00(OT_Prim* pPrim, int width, int height, const void* pCompressed)
{
    PSX_Render_Poly_FT4_Direct_Impl<64>(true, pPrim, width, height, pCompressed, Decompress_Next_Type3);
}

EXPORT void CC PSX_Render_PolyFT4_8bit_Opaque_5006E0(OT_Prim* pPrim, int width, int height, const void* pData)
{
    PSX_Render_Poly_FT4_Direct_Impl<64>(false, pPrim, width, height, pData, Decompress_Next_Type3);
}

EXPORT void CC PSX_Render_PolyFT4_8bit_50CC70(OT_Prim* pOt, int width, int height, const void* pCompressedData)
{
    assert(sRedShift_C215C4 == 11); // Should be the only possible case
    if (pOt->field_B_flags & 2 && k1_dword_55EF90)
    {
        PSX_Render_PolyFT4_8bit_SemiTrans_501B00(pOt, width, height, pCompressedData);
    }
    else
    {
        PSX_Render_PolyFT4_8bit_Opaque_5006E0(pOt, width, height, pCompressedData);
    }
}

// Only possible to be used by FG1
EXPORT void CC PSX_Render_PolyFT4_16bit_517990(OT_Prim* pPrim, int width, int height, const DWORD* pFg1Data, int xoffset, int yoffset)
{
    int xConverted = pPrim->field_14_verts[0].field_0_x0 / 16;
    int yConverted = pPrim->field_14_verts[0].field_4_y0 / 16;

    const int clipx_div16 = sPsx_drawenv_clipx_BDCD40 / 16;
    const int clipy_div16 = sPsx_drawenv_clipy_BDCD44 / 16;

    // TODO: Needs more reversing/clean up to understand the coordinate space conversion
    int skipBits = 0;
    if (xConverted >= clipx_div16)
    {
        skipBits = 0;
    }
    else
    {
        skipBits = clipx_div16 - xConverted;
        width -= clipx_div16 - xConverted;
        xoffset += clipx_div16 - xConverted;
        xConverted = sPsx_drawenv_clipx_BDCD40 / 16;
    }

    if (width > sPsx_drawenv_clipw_BDCD48 / 16 - xConverted)
    {
        width = sPsx_drawenv_clipw_BDCD48 / 16 - xConverted;
    }

    int height_clipped = 0;
    if (yConverted >= clipy_div16)
    {
        height_clipped = height;
    }
    else
    {
        height_clipped = yConverted - clipy_div16 + height;
        yoffset += clipy_div16 - yConverted;
        yConverted = sPsx_drawenv_clipy_BDCD44 / 16;
    }

    if (height_clipped > sPsx_drawenv_cliph_BDCD4C / 16 - yConverted)
    {
        height_clipped = sPsx_drawenv_cliph_BDCD4C / 16 - yConverted;
    }

    if (height_clipped >= 0 && width >= 0)
    {
        const unsigned int pitch = spBitmap_C2D038->field_10_locked_pitch / sizeof(WORD);
        WORD* pDst = (WORD *)spBitmap_C2D038->field_4_pLockedPixels + (1 * xConverted) + (yConverted * pitch);
        WORD* pSrc = (WORD *)spBitmap_C2D038->field_4_pLockedPixels + (pitch * (yoffset + 272)) + xoffset;
       
        for (int yIter = 0; yIter < height_clipped; yIter++)
        {
            DWORD bitMask = (*pFg1Data) >> skipBits;
            size_t dst_off = pSrc - pDst;
            for (int xIter = 0; xIter < width; xIter++)
            {
                if (bitMask & 1)
                {
                    pDst[xIter] = pDst[dst_off + xIter];
                }
                bitMask >>= 1; // To next bit
            }

            pDst += pitch;
            pSrc += pitch;

            pFg1Data++;
        }
    }
}

EXPORT void CC PSX_Render_PolyFT4_4bit_517880(OT_Prim* pOt, int width, int height, const void* pCompressed)
{
    assert(sRedShift_C215C4 == 11); // Should be the only possible case
    if (pOt->field_B_flags & 2)
    {
        PSX_Render_PolyFT4_4bit_SemiTrans_50DF30(pOt, width, height, pCompressed);
    }
    else
    {
        PSX_Render_PolyFT4_4bit_Opqaue_50CDB0(pOt, width, height, pCompressed);
    }
}


// For 3 verticies the order is 0, 1, 2
// For 4 verticies the order is 0, 1, 3, 2
enum class EPolyType
{
    e4Point,
    e3Point,
};

template<class T>
static inline void Convert_Vertex_XY(OT_Vert* pConverted, T* pPoly, EPolyType polyType)
{
    pConverted[0].field_0_x0 = 16 * X0(pPoly);
    pConverted[0].field_4_y0 = 16 * Y0(pPoly);
    pConverted[0].field_8 = 0;

    pConverted[1].field_0_x0 = 16 * X1(pPoly);
    pConverted[1].field_4_y0 = 16 * Y1(pPoly);
    pConverted[1].field_8 = 0;

    if (polyType == EPolyType::e4Point)
    {
        pConverted[2].field_0_x0 = 16 * X3(pPoly);
        pConverted[2].field_4_y0 = 16 * Y3(pPoly);
        pConverted[2].field_8 = 0;

        pConverted[3].field_0_x0 = 16 * X2(pPoly);
        pConverted[3].field_4_y0 = 16 * Y2(pPoly);
        pConverted[3].field_8 = 0;
    }
    else
    {
        pConverted[2].field_0_x0 = 16 * X2(pPoly);
        pConverted[2].field_4_y0 = 16 * Y2(pPoly);
        pConverted[2].field_8 = 0;
    }
}

template<class T>
static inline void Convert_Vertex_RGB_Single(OT_Vert* pConverted, T* pPoly)
{
    pConverted[0].field_1C_r = R0(pPoly) << 13;
    pConverted[0].field_20_g = G0(pPoly) << 13;
    pConverted[0].field_24_b = B0(pPoly) << 13;
}

template<class T>
static inline void Convert_Vertex_RGB(OT_Vert* pConverted, T* pPoly, EPolyType polyType, bool hasTexture)
{
    const int shiftValue = hasTexture ? 0 : 13;

    pConverted[0].field_1C_r = R0(pPoly) << shiftValue;
    pConverted[0].field_20_g = G0(pPoly) << shiftValue;
    pConverted[0].field_24_b = B0(pPoly) << shiftValue;

    pConverted[1].field_1C_r = R1(pPoly) << shiftValue;
    pConverted[1].field_20_g = G1(pPoly) << shiftValue;
    pConverted[1].field_24_b = B1(pPoly) << shiftValue;

    if (polyType == EPolyType::e4Point)
    {
        pConverted[2].field_1C_r = R3(pPoly) << shiftValue;
        pConverted[2].field_20_g = G3(pPoly) << shiftValue;
        pConverted[2].field_24_b = B3(pPoly) << shiftValue;

        pConverted[3].field_1C_r = R2(pPoly) << shiftValue;
        pConverted[3].field_20_g = G2(pPoly) << shiftValue;
        pConverted[3].field_24_b = B2(pPoly) << shiftValue;
    }
    else
    {
        pConverted[2].field_1C_r = R2(pPoly) << shiftValue;
        pConverted[2].field_20_g = G2(pPoly) << shiftValue;
        pConverted[2].field_24_b = B2(pPoly) << shiftValue;
    }
}

template<class T>
static inline void Convert_Vertex_UV(OT_Vert* pConverted, T* pPoly, EPolyType polyType)
{
    pConverted[0].field_14_u = U0(pPoly);
    pConverted[0].field_18_v = V0(pPoly);

    pConverted[1].field_14_u = U1(pPoly);
    pConverted[1].field_18_v = V1(pPoly);

    if (polyType == EPolyType::e4Point)
    {
        pConverted[2].field_14_u = U3(pPoly);
        pConverted[2].field_18_v = V3(pPoly);

        pConverted[3].field_14_u = U2(pPoly);
        pConverted[3].field_18_v = V2(pPoly);
    }
    else
    {
        pConverted[2].field_14_u = U2(pPoly);
        pConverted[2].field_18_v = V2(pPoly);
    }
}

EXPORT OT_Prim* CC PSX_Render_Convert_Polys_To_Internal_Format_4F7110(void* pData, int xoff, int yoff)
{
    PrimAny any;
    any.mVoid = pData;

    OT_Prim* pConverted = off_578330;
    pConverted->field_E = 0;
    pConverted->field_D_bClip = 1;
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

        Convert_Vertex_XY(pConverted->field_14_verts, pPoly, EPolyType::e4Point);
        Convert_Vertex_RGB(pConverted->field_14_verts, pPoly, EPolyType::e4Point, true);
        Convert_Vertex_UV(pConverted->field_14_verts, pPoly, EPolyType::e4Point);

        return pConverted;
    }
    else if (kPrimType == ePolyG4)
    {
        Poly_G4* pPoly = any.mPolyG4;
        pConverted->field_C_vert_count = 4;

        Convert_Vertex_XY(pConverted->field_14_verts, pPoly, EPolyType::e4Point);
        Convert_Vertex_RGB(pConverted->field_14_verts, pPoly, EPolyType::e4Point, false);

        return pConverted;
    }
    else if (kPrimType == ePolyFT4)
    {
        Poly_FT4* pPoly = any.mPolyFT4;
        pConverted->field_C_vert_count = 4;

        pConverted->field_10_tpage = GetTPage(pPoly);
        pConverted->field_12_clut = GetClut(pPoly);

        Convert_Vertex_XY(pConverted->field_14_verts, pPoly, EPolyType::e4Point);
        Convert_Vertex_UV(pConverted->field_14_verts, pPoly, EPolyType::e4Point);

        if (!(pConverted->field_B_flags & 1) // Blending enabled?
            && (pConverted->field_8_r & 248) == 128
            && (pConverted->field_9_g & 248) == 128
            && (pConverted->field_A_b & 248) == 128)
        {
            pConverted->field_B_flags |= 1;
        }

        // TODO: 64bit - can't fit 64bits of data into 32
        // TODO: This seems to be set by Animation::vRender and its a pointer to type 3 or 6 compressed frame data
        // It can also be set by FG1 rendering in which case a its a pointer to FG1 data.
        
        const void* pAnimOrFG1Data = GetPrimExtraPointerHack(pPoly);
        if (pAnimOrFG1Data)
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

            const int bitDepth = (pConverted->field_10_tpage >> 7) & 3;
            switch (bitDepth)
            {
            case TextureModes::e4Bit:
                PSX_Render_PolyFT4_4bit_517880(pConverted,
                    (pConverted->field_14_verts[2].field_0_x0 - pConverted->field_14_verts[0].field_0_x0) / 16,
                    (pConverted->field_14_verts[2].field_4_y0 - pConverted->field_14_verts[0].field_4_y0) / 16,
                    pAnimOrFG1Data);
                break;

            case TextureModes::e8Bit:
                PSX_Render_PolyFT4_8bit_50CC70(pConverted,
                    (pConverted->field_14_verts[2].field_0_x0 - pConverted->field_14_verts[0].field_0_x0) / 16,
                    (pConverted->field_14_verts[2].field_4_y0 - pConverted->field_14_verts[0].field_4_y0) / 16,
                    pAnimOrFG1Data);
                break;

            case TextureModes::e16Bit:
                PSX_Render_PolyFT4_16bit_517990(pConverted,
                    (pConverted->field_14_verts[2].field_0_x0 - pConverted->field_14_verts[0].field_0_x0) / 16,
                    (pConverted->field_14_verts[2].field_4_y0 - pConverted->field_14_verts[0].field_4_y0) / 16,
                    reinterpret_cast<const DWORD*>(pAnimOrFG1Data),
                    pConverted->field_14_verts[0].field_0_x0 / 16 , pConverted->field_14_verts[0].field_4_y0 / 16);
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

        Convert_Vertex_XY(pConverted->field_14_verts, pPoly, EPolyType::e4Point);
        Convert_Vertex_RGB_Single(pConverted->field_14_verts, pPoly);
     
        return pConverted;
    }
    else if (kPrimType == ePolyGT3)
    {
        Poly_GT3* pPoly = any.mPolyGT3;
        pConverted->field_C_vert_count = 3;

        pConverted->field_10_tpage = GetTPage(pPoly);
        pConverted->field_12_clut = GetClut(pPoly);

        Convert_Vertex_XY(pConverted->field_14_verts, pPoly, EPolyType::e3Point);
        Convert_Vertex_RGB(pConverted->field_14_verts, pPoly, EPolyType::e3Point, true);
        Convert_Vertex_UV(pConverted->field_14_verts, pPoly, EPolyType::e3Point);

        return pConverted;
    }
    else if (kPrimType == ePolyG3)
    {
        Poly_G3* pPoly = any.mPolyG3;
        pConverted->field_C_vert_count = 3;

        Convert_Vertex_XY(pConverted->field_14_verts, pPoly, EPolyType::e3Point);
        Convert_Vertex_RGB(pConverted->field_14_verts, pPoly, EPolyType::e3Point, false);

        return pConverted;
    }
    else if (kPrimType == ePolyFT3)
    {
        Poly_FT3* pPoly = any.mPolyFT3;
        pConverted->field_C_vert_count = 3;

        pConverted->field_10_tpage = GetTPage(pPoly);
        pConverted->field_12_clut = GetClut(pPoly);

        Convert_Vertex_XY(pConverted->field_14_verts, pPoly, EPolyType::e3Point);
        Convert_Vertex_UV(pConverted->field_14_verts, pPoly, EPolyType::e3Point);

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

        Convert_Vertex_XY(pConverted->field_14_verts, pPoly, EPolyType::e3Point);
        Convert_Vertex_RGB_Single(pConverted->field_14_verts, pPoly);

        return pConverted;
    }
    else
    {
        return nullptr;
    }
}

EXPORT void CC PSX_poly_GShaded_NoTexture_517E60(Render_Unknown* pOrigin, Render_Unknown* pSlope, int idx1, int idx2)
{
    const OT_Vert* pV1 = &pVerts_dword_BD3264[idx1];
    const OT_Vert* pV2 = &pVerts_dword_BD3264[idx2];

    pSlope->field_1C_GShadeR = pV2->field_1C_r - pV1->field_1C_r;
    pSlope->field_20_GShadeG = pV2->field_20_g - pV1->field_20_g;
    pSlope->field_24_GShadeB = pV2->field_24_b - pV1->field_24_b;

    const int dx = (pV2->field_0_x0 - pV1->field_0_x0) << 16; // To FP 16:16
    const int dy = pV2->field_4_y0 - pV1->field_4_y0;

    pSlope->field_0_x = dx;
    if (dy > 0)
    {
        const int tableValue = sPsxEmu_fixed_point_table_C1D5C0[dy + 1];
        pSlope->field_0_x = PSX_poly_helper_fixed_point_scale_517FA0(dx, tableValue);
        pSlope->field_1C_GShadeR = PSX_poly_helper_fixed_point_scale_517FA0(pSlope->field_1C_GShadeR, tableValue * 16);
        pSlope->field_20_GShadeG = PSX_poly_helper_fixed_point_scale_517FA0(pSlope->field_20_GShadeG, tableValue * 16);
        pSlope->field_24_GShadeB = PSX_poly_helper_fixed_point_scale_517FA0(pSlope->field_24_GShadeB, tableValue * 16);

    }
    pSlope->field_4_y = dy;

    const int v1_y0_rounded = ((pV1->field_4_y0 + 15) & ~15) - pV1->field_4_y0;
    pOrigin->field_0_x = (pV1->field_0_x0 * 4096) + (v1_y0_rounded * pSlope->field_0_x) / 16;

    pOrigin->field_1C_GShadeR = pV1->field_1C_r + (v1_y0_rounded * pSlope->field_1C_GShadeR) / 16;
    pOrigin->field_20_GShadeG = pV1->field_20_g + (v1_y0_rounded * pSlope->field_20_GShadeG) / 16;
    pOrigin->field_24_GShadeB = pV1->field_24_b + (v1_y0_rounded * pSlope->field_24_GShadeB) / 16;
}


EXPORT void CC PSX_poly_FShaded_NoTexture_517DF0(Render_Unknown* pOrigin, Render_Unknown* pSlope, int idx1, int idx2)
{
    // Calculate slope between verts
    const int v1_x = pVerts_dword_BD3264[idx1].field_0_x0;
    const int v1_y = pVerts_dword_BD3264[idx1].field_4_y0;

    const int dx = (pVerts_dword_BD3264[idx2].field_0_x0 - v1_x) << 16; // to fixed point 16:16
    const int dy = (pVerts_dword_BD3264[idx2].field_4_y0 - v1_y);

    // Prevent divide by zero
    if (dy > 0)
    {
        // Amount to increment X by in pixels (fixed point)
        pSlope->field_0_x = dx / dy;
    }
    else
    {
        // Amount to increment X by in pixels (fixed point)
        pSlope->field_0_x = dx;
    }
    pSlope->field_4_y = dy;

    // Set starting x to PSX GTE fixed format?
    pOrigin->field_0_x = (pVerts_dword_BD3264[idx1].field_0_x0 * 4096);

    const int v1_y_rounded = ((v1_y + 15) & ~15) - v1_y;
    pOrigin->field_0_x += (v1_y_rounded * pSlope->field_0_x) / 16; // Div16 is conversion of fixed 16:16 ?
}

EXPORT void CC PSX_poly_Textured_517FC0(Render_Unknown* pOrigin, Render_Unknown* pSlope, int idx1, int idx2)
{
    const OT_Vert* pV1 = &pVerts_dword_BD3264[idx1];
    const OT_Vert* pV2 = &pVerts_dword_BD3264[idx2];

    pSlope->field_0_x = (pV2->field_0_x0 - pV1->field_0_x0) << 16;
    pSlope->field_14_u = (pV2->field_14_u - pV1->field_14_u) * 1024;
    pSlope->field_18_v = (pV2->field_18_v - pV1->field_18_v) * 1024;
    pSlope->field_4_y = pV2->field_4_y0 - pV1->field_4_y0;

    if (pSlope->field_18_v > 0)
    {
        pSlope->field_18_v += 1024;
    }
    else if (pSlope->field_18_v < 0)
    {
        pSlope->field_18_v -= 1024;
    }

    if (pSlope->field_4_y > 0)
    {
        pSlope->field_0_x = pSlope->field_0_x / pSlope->field_4_y;
        pSlope->field_14_u = (pSlope->field_14_u / pSlope->field_4_y) * 16;
        pSlope->field_18_v = (pSlope->field_18_v / pSlope->field_4_y) * 16;
    }

    const int v1_y_rounded = ((pV1->field_4_y0 + 15) & ~15) - pV1->field_4_y0;
    pOrigin->field_0_x = (pV1->field_0_x0 * 4096) + (v1_y_rounded * pSlope->field_0_x) / 16;
    pOrigin->field_14_u = (pV1->field_14_u * 1024) + (v1_y_rounded * pSlope->field_14_u) / 16;
    pOrigin->field_18_v = (pV1->field_18_v * 1024) + (v1_y_rounded * pSlope->field_18_v) / 16;
}

EXPORT void CC PSX_poly_Textured_Unknown_5180B0(Render_Unknown* /*pOrigin*/, Render_Unknown* /*pSlope*/, int /*idx1*/, int /*idx2*/)
{
    /*
    const OT_Vert* pV1 = &pVerts_dword_BD3264[idx1];
    const OT_Vert* pV2 = &pVerts_dword_BD3264[idx2];

    const int v1_x = pV1->field_0_x0;
    const int v2_y = pV1->field_4_y0;

    pSlope->field_0_x = (pV2->field_0_x0 - pV1->field_0_x0) << 16; // To FP 16:16
    pSlope->field_4_y = pV2->field_4_y0 - v2_y;
    if (pSlope->field_4_y > 0)
    {
        const float psx_fixed_to_float = sPsxEmu_float_table_C19160[pSlope->field_4_y];
        pSlope->field_0_x = static_cast<int>(pSlope->field_0_x * psx_fixed_to_float);
        pSlope->field_14_u = (pV2->field_14_u - pV1->field_14_u) * psx_fixed_to_float * 16.0f;
        pSlope->field_18_v = (pV2->field_18_v - pV1->field_18_v) * psx_fixed_to_float * 16.0f;
        pSlope->field_10_float = (pV2->field_10 - pV1->field_10) * psx_fixed_to_float * 16.0f;
    }

    const int yRounded = (pV1->field_4_y0 + 15) & ~15;
    pOrigin->field_0_x = (yRounded * pSlope->field_0_x) + (v1_x * 4096) / 16;

    const float yRounded_scaled = yRounded - (pV1->field_4_y0 / 16.0f); // * 0.0625f
    pOrigin->field_14_u = (yRounded_scaled * pSlope->field_14_u) + pV1->field_14_u;
    pOrigin->field_18_v = (yRounded_scaled * pSlope->field_18_v) + pV1->field_18_v;
    pOrigin->field_10_float = (yRounded_scaled * pSlope->field_10_float) + pV1->field_10;
    */
}

using TCalculateSlopes = decltype(&PSX_poly_FShaded_NoTexture_517DF0);
using TRenderScanLines = decltype(&PSX_EMU_Render_Polys_FShaded_NoTexture_Opqaue_51C4C0);

EXPORT void CC PSX_Render_Poly_Internal_Generic_517B10(OT_Prim* pPrim, TCalculateSlopes pCalcSlopes, TRenderScanLines pRenderScanLines)
{
    if (pPrim->field_C_vert_count < 3)
    {
        return;
    }

    // textured check
    if (pPrim->field_B_flags & 4)
    {
        // Set up CLUT source if not 16 bit direct mode
        if (sTexture_mode_BD0F14 != TextureModes::e16Bit)
        {
            // TODO: Refactor
            pClut_src_BD3270 = (WORD *)((char *)sPsxVram_C1D160.field_4_pLockedPixels + 32 * ((pPrim->field_12_clut & 63) + (pPrim->field_12_clut >> 6 << 6)));
        }

        pTPage_src_BD32C8 = sTPage_src_ptr_BD0F1C;

        if (pPrim->field_B_flags & 1)
        {
            // blending
            const int greenBlue = (16 << sGreenShift_C1D180) | (16 << sBlueShift_C19140);
            sPoly_fill_colour_BD3350 = static_cast<WORD>((16 << sRedShift_C215C4) | greenBlue);
        }
        else
        {
            // no blending

            unsigned int g_s3 = pPrim->field_9_g >> 3;
            unsigned int r_s3 = pPrim->field_8_r >> 3;
            unsigned int b_s3 = pPrim->field_A_b >> 3;

            r_lut_dword_BD3308 = stru_C146C0.r[r_s3];
            g_lut_dword_BD32D8 = stru_C146C0.g[g_s3];
            b_lut_dword_BD3348 = stru_C146C0.b[b_s3];

            rTable_dword_BD32CC = stru_C1D1C0[r_s3].field_0;
            gTable_dword_BD3358 = stru_C1D1C0[g_s3].field_0;
            bTable_dword_BD334C = stru_C1D1C0[b_s3].field_0;

            sPoly_fill_colour_BD3350 = ((WORD)r_s3 << sRedShift_C215C4) | ((WORD)g_s3 << sGreenShift_C1D180) | ((WORD)b_s3 << sBlueShift_C19140);
        }
    }
    else
    {
        // not textured
        unsigned int g_s3 = pPrim->field_9_g >> 3;
        unsigned int r_s3 = pPrim->field_8_r >> 3;
        unsigned int b_s3 = pPrim->field_A_b >> 3;

        sPoly_fill_colour_BD3350 = ((WORD)r_s3 << sRedShift_C215C4) | ((WORD)g_s3 << sGreenShift_C1D180) | ((WORD)b_s3 << sBlueShift_C19140);
    }

    int lowest_y_vert_idx = 0;
    int smallestYPos = 0x7FFFFFFF; // max int
    for (int i = 0; i < pPrim->field_C_vert_count; i++)
    {
        if (pPrim->field_14_verts[i].field_4_y0 < smallestYPos)
        {
            smallestYPos = pPrim->field_14_verts[i].field_4_y0;
            lowest_y_vert_idx = i;
        }
    }

    pVerts_dword_BD3264 = pPrim->field_14_verts;

    int bottom_pos = (smallestYPos + 15) / 16;

    int lowest_y_idx_forwards = lowest_y_vert_idx;
    int ypos_forwards = bottom_pos - 1;

    int lowest_y_idx_backwards = lowest_y_vert_idx;
    int ypos_backwards = bottom_pos - 1;

    int vertCounter = pPrim->field_C_vert_count;
    while (vertCounter > 0)
    {
        // Backwards loop around verts
        while (ypos_backwards <= bottom_pos)
        {
            if (vertCounter <= 0)
            {
                break;
            }
            --vertCounter;

            int vertIdx_2 = lowest_y_idx_backwards - 1;
            if (vertIdx_2 < 0)
            {
                vertIdx_2 = pPrim->field_C_vert_count - 1;
            }

            pCalcSlopes(&left_side_BD3320, &slope_1_BD3200, lowest_y_idx_backwards, vertIdx_2);
            ypos_backwards = (pPrim->field_14_verts[vertIdx_2].field_4_y0 + 15) / 16;
            lowest_y_idx_backwards = vertIdx_2;
        }

        // Forwards loop around verts
        while (ypos_forwards <= bottom_pos)
        {
            if (vertCounter <= 0)
            {
                break;
            }
            --vertCounter;

            int vertIdx_2 = lowest_y_idx_forwards + 1;
            if (vertIdx_2 >= pPrim->field_C_vert_count)
            {
                vertIdx_2 = 0;
            }

            pCalcSlopes(&right_side_BD32A0, &slope_2_BD32E0, lowest_y_idx_forwards, vertIdx_2);
            ypos_forwards = (pPrim->field_14_verts[vertIdx_2].field_4_y0 + 15) / 16;
            lowest_y_idx_forwards = vertIdx_2;
        }

        int clampedY = ypos_backwards;
        if (ypos_backwards >= ypos_forwards)
        {
            clampedY = ypos_forwards;
        }

        if (clampedY - bottom_pos > 0)
        {
            BYTE* pVram = ((BYTE*)spBitmap_C2D038->field_4_pLockedPixels) + (bottom_pos * spBitmap_C2D038->field_10_locked_pitch);
            pRenderScanLines((WORD*)pVram, clampedY - bottom_pos);
        }

        bottom_pos = clampedY;
    }
}

EXPORT OT_Prim* CC PSX_clip_polys_4FE710(OT_Prim* pOt);

EXPORT void CC PSX_Render_Internal_Format_Polygon_4F7960(OT_Prim* prim, int xoff, int yoff)
{
    if (!prim)
    {
        return;
    }


    // Temp increase clip w/h
    sPsx_drawenv_clipw_BDCD48 += 16;
    sPsx_drawenv_cliph_BDCD4C += 16;

    for (int i = 0; i < prim->field_C_vert_count; i++)
    {
        prim->field_14_verts[i].field_0_x0 += 16 * xoff;
        prim->field_14_verts[i].field_4_y0 += 16 * yoff;
    }

    if (prim->field_D_bClip)
    {
        prim = PSX_clip_polys_4FE710(prim);
    }

    if (prim)
    {
        const short oldTPage = sActiveTPage_578318;
        switch (PSX_Prim_Code_Without_Blending_Or_SemiTransparency(prim->field_B_flags))
        {
        case PrimTypeCodes::ePolyFT3:
        case PrimTypeCodes::ePolyGT3:
        case PrimTypeCodes::ePolyFT4:
        case PrimTypeCodes::ePolyGT4:
            PSX_TPage_Change_4F6430(prim->field_10_tpage);

            if (prim->field_E & 1) // TODO: Dead code? Haven't seen this get written... yet
            {
                // unknown flag, something to do with specific colours? Maybe optimization case?
                PSX_Render_Poly_Internal_Generic_517B10(prim, PSX_poly_Textured_Unknown_5180B0, 
                    (prim->field_B_flags & 2) ? 
                        PSX_EMU_Render_Polys_Textured_Unknown_SemiTrans_51DC90 : PSX_EMU_Render_Polys_Textured_Unknown_Opqaue_51D890);
            }
            else
            {
                if (prim->field_B_flags & 1)
                {
                    // Blending enabled
                    PSX_Render_Poly_Internal_Generic_517B10(prim, PSX_poly_Textured_517FC0, 
                        (prim->field_B_flags & 2) ? 
                            PSX_EMU_Render_Polys_Textured_Blending_SemiTrans_51D2B0 : PSX_EMU_Render_Polys_Textured_Blending_Opqaue_51CCA0);
                }
                else
                {
                    PSX_Render_Poly_Internal_Generic_517B10(prim, PSX_poly_Textured_517FC0, 
                        (prim->field_B_flags & 2) ? 
                            PSX_EMU_Render_Polys_Textured_NoBlending_SemiTrans_51E890 : PSX_EMU_Render_Polys_Textured_NoBlending_Opaque_51E140);
                }
            }

            // Restore old tpage
            PSX_TPage_Change_4F6430(oldTPage);
            break;

        case PrimTypeCodes::ePolyG3:
        case PrimTypeCodes::ePolyG4:
            // G shaded, not textured
            PSX_Render_Poly_Internal_Generic_517B10(prim, PSX_poly_GShaded_NoTexture_517E60, 
                (prim->field_B_flags & 2) ? 
                    PSX_EMU_Render_Polys_GShaded_NoTexture_SemiTrans_51C8D0 : PSX_EMU_Render_Polys_GShaded_NoTexture_Opqaue_51C6E0);
            break;

        case PrimTypeCodes::ePolyF3:
        case PrimTypeCodes::ePolyF4:
            // F shaded, not textured
            PSX_Render_Poly_Internal_Generic_517B10(prim, PSX_poly_FShaded_NoTexture_517DF0, 
                (prim->field_B_flags & 2) ? 
                    PSX_EMU_Render_Polys_FShaded_NoTexture_SemiTrans_51C590 : PSX_EMU_Render_Polys_FShaded_NoTexture_Opqaue_51C4C0);
            break;
        }
    }

    // Restore clip w/h
    sPsx_drawenv_clipw_BDCD48 -= 16;
    sPsx_drawenv_cliph_BDCD4C -= 16;
}

ALIVE_VAR(1, 0xbd30e4, int, sScreenXOffSet_BD30E4, 0);
ALIVE_VAR(1, 0xbd30a4, int, sScreenYOffset_BD30A4, 0);

enum LineSegmentClipEdges
{
    eLeft = 0x1,
    eRight = 0x2,
    eBottom = 0x4,
    eTop = 0x8,
};

EXPORT char CC PSX_Calc_LineSegment_Clip_Edges_4FE460(int x, int y)
{
    char result = 0;

    if (x < sPsx_drawenv_clipx_BDCD40)
    {
        result = LineSegmentClipEdges::eLeft;
    }
    else if (x > sPsx_drawenv_clipw_BDCD48)
    {
        result = LineSegmentClipEdges::eRight;
    }

    if (y < sPsx_drawenv_clipy_BDCD44)
    {
        result |= LineSegmentClipEdges::eBottom;
    }
    else if (y > sPsx_drawenv_cliph_BDCD4C)
    {
        result |= LineSegmentClipEdges::eTop;
    }

    return result;
}

EXPORT void CC PSX_Clip_LineSegment_Against_DrawEnv_4FE640(int* pX, int* pY, int xDiff, int yDiff, char clipEdges)
{
   // NOT_IMPLEMENTED();

    if (clipEdges & LineSegmentClipEdges::eLeft)
    {
        const int newY = yDiff * ((sPsx_drawenv_clipx_BDCD40 - *pX) / xDiff) + *pY;
        *pX = sPsx_drawenv_clipx_BDCD40;
        *pY = newY;
        clipEdges = PSX_Calc_LineSegment_Clip_Edges_4FE460(sPsx_drawenv_clipx_BDCD40, *pY);
    }
    else if (clipEdges & LineSegmentClipEdges::eRight)
    {
        const int newY = yDiff * ((sPsx_drawenv_clipw_BDCD48 - *pX) / xDiff) + *pY;
        *pX = sPsx_drawenv_clipw_BDCD48;
        *pY = newY;
        clipEdges = PSX_Calc_LineSegment_Clip_Edges_4FE460(sPsx_drawenv_clipw_BDCD48, *pY);
    }

    if (clipEdges & LineSegmentClipEdges::eBottom)
    {
        const int newX = xDiff * ((sPsx_drawenv_clipy_BDCD44 - *pY) / yDiff) + *pX;
        *pY = sPsx_drawenv_clipy_BDCD44;
        *pX = newX;
        PSX_Calc_LineSegment_Clip_Edges_4FE460(*pX, sPsx_drawenv_clipy_BDCD44);
    }
    else if (clipEdges & LineSegmentClipEdges::eTop)
    {
        const int newX = xDiff * ((sPsx_drawenv_cliph_BDCD4C - *pY) / yDiff) + *pX;
        *pY = sPsx_drawenv_cliph_BDCD4C;
        *pX = newX;
        PSX_Calc_LineSegment_Clip_Edges_4FE460(*pX, sPsx_drawenv_cliph_BDCD4C);
    }
}

EXPORT char CC PSX_Clip_Line_Segments_4FE4F0(int* pX0, int* pY0, int* pX1, int* pY1)
{
    const int y0_fixed = 16 * *pY0;
    const int x1_fixed = 16 * *pX1;
    const int x0_fixed = 16 * *pX0;
    const int y1_fixed = 16 * *pY1;

    *pX1 = x1_fixed;
    *pY0 = y0_fixed;
    *pY1 = y1_fixed;
    *pX0 = x0_fixed;
    
    int x_diff_fixed = x1_fixed - x0_fixed;
    int y_diff_fixed = y1_fixed - y0_fixed;
    
    char xy0_clip = PSX_Calc_LineSegment_Clip_Edges_4FE460(x0_fixed, y0_fixed);
    char xy1_clip = PSX_Calc_LineSegment_Clip_Edges_4FE460(x1_fixed, y1_fixed);
  
    if (xy1_clip & xy0_clip)
    {
        return 0;
    }

    while (1)
    {
        bool clip = true;
        if (xy0_clip)
        {
            PSX_Clip_LineSegment_Against_DrawEnv_4FE640(pX0, pY0, x_diff_fixed, y_diff_fixed, xy0_clip);
            if (!xy1_clip)
            {
                clip = false;
            }
        }
        else
        {
            if (!xy1_clip)
            {
                break;
            }
        }

        if (clip)
        {
            PSX_Clip_LineSegment_Against_DrawEnv_4FE640(pX1, pY1, x_diff_fixed, y_diff_fixed, xy1_clip);
        }

        xy0_clip = PSX_Calc_LineSegment_Clip_Edges_4FE460(*pX0, *pY0);
        xy1_clip = PSX_Calc_LineSegment_Clip_Edges_4FE460(*pX1, *pY1);
        if (xy1_clip & xy0_clip)
        {
            return 0;
        }
    }

    *pX0 /= 16;
    *pY0 /= 16;
    *pX1 /= 16;
    *pY1 /= 16;

    return 1;
}

EXPORT void CC PSX_EMU_Render_G_LineSegment_4F8250(void* pOtPrim1, signed int x0, int y0, int x1, int y1, BYTE r0, BYTE g0, BYTE b0, BYTE r1, BYTE g1, BYTE b1)
{
    if (!PSX_Clip_Line_Segments_4FE4F0(&x0, &y0, &x1, &y1))
    {
        return;
    }

    const unsigned int pitch = (unsigned int)spBitmap_C2D038->field_10_locked_pitch / sizeof(WORD);

    int rDiff = (r1 - r0) << 16;
    int gDiff = (g1 - g0) << 16;
    int bDiff = (b1 - b0) << 16;

    int r0_fixed = r0 << 16;
    int g0_fixed = g0 << 16;
    int b0_fixed = b0 << 16;

    int x_Diff = x1 - x0;
    int y_Diff = y1 - y0;
 
    OT_Prim* pOtPrim = (OT_Prim*)pOtPrim1;
    Psx_Test* abr_lut = nullptr;
    const bool bSemiTrans = (pOtPrim->field_B_flags & 2) != 0;
    if (bSemiTrans)
    {
        abr_lut = &sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18];
    }

    if (y_Diff < 0)
    {
        y_Diff = -y_Diff;
    }

    if (x_Diff < 0)
    {
        x_Diff = -x_Diff;
    }

    if (y_Diff <= x_Diff)
    {
        int x_max = x1;
        int y_max = y0;

        if (x0 > x1)
        {
            y_max = y1;
            std::swap(y0, y1);

            x_max = x0;
            std::swap(x0, x1);
        }

        int width = x_max - x0;
        if (width > 0)
        {
            y_Diff = ((y1 - y_max) << 16) / width;
            y_max = y0;
            rDiff /= width;
            gDiff /= width;
            bDiff /= width;
        }

        WORD* pVram = (WORD *)((char *)spBitmap_C2D038->field_4_pLockedPixels + 2 * x0);
        int yLine = (y_max << 16) + 0x7FFF;
        for (int i = 0; i <= width; i++)
        {
            if (bSemiTrans)
            {
                WORD* pVramSrcDst = &pVram[pitch * (yLine >> 16)];
                DWORD src = *pVramSrcDst;
                *pVramSrcDst =
                      abr_lut->b[(b0_fixed >> 19) & 0x1F][((src >> sBlueShift_C19140) & 0x1F)]
                    | abr_lut->g[(g0_fixed >> 19) & 0x1F][((src >> sGreenShift_C1D180) & 0x1F)]
                    | abr_lut->r[(r0_fixed >> 19) & 0x1F][((src >> sRedShift_C215C4) & 0x1F)];
            }
            else
            {
                const WORD pixel = (r0_fixed >> 19 << sRedShift_C215C4) | (g0_fixed >> 19 << sGreenShift_C1D180) | (b0_fixed >> 19 << sBlueShift_C19140);
                pVram[pitch * (yLine >> 16)] = pixel;
            }
            pVram++; // Move X across
            r0_fixed += rDiff;
            g0_fixed += gDiff;
            b0_fixed += bDiff;
            yLine += y_Diff;
        }
    }
    else
    {
        int yMax = y1;
        int y0_max = y0;

        if (y0 > y1)
        {
            yMax = y0;
            y0_max = y1;

            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        int height = yMax - y0_max;
        if (height > 0)
        {
            x_Diff = ((x1 - x0) << 16) / height;
            y0_max = y0;
            rDiff /= height;
            gDiff /= height;
            bDiff /= height;
        }

        WORD* pVRam = (WORD *)((char *)spBitmap_C2D038->field_4_pLockedPixels + 2 * y0_max * pitch);

        int xpos = (x0 << 16) + 0x7FFF;
        for (int i = 0; i <= height; i++)
        {
            if (bSemiTrans)
            {
                WORD* pVramSrcDst = &pVRam[xpos >> 16];
                DWORD src = *pVramSrcDst;
                *pVramSrcDst =
                      abr_lut->b[(b0_fixed >> 19) & 0x1F][((src >> sBlueShift_C19140) & 0x1F)]
                    | abr_lut->g[(g0_fixed >> 19) & 0x1F][((src >> sGreenShift_C1D180) & 0x1F)]
                    | abr_lut->r[(r0_fixed >> 19) & 0x1F][((src >> sRedShift_C215C4) & 0x1F)];
            }
            else
            {
                pVRam[xpos >> 16] = (r0_fixed >> 19 << sRedShift_C215C4) | (g0_fixed >> 19 << sGreenShift_C1D180) | (b0_fixed >> 19 << sBlueShift_C19140);
            }

            pVRam += pitch;

            r0_fixed += rDiff;
            b0_fixed += bDiff;
            g0_fixed += gDiff;

            xpos += x_Diff;
        }
    }
}

EXPORT void CC PSX_EMU_Render_F_LineSegment_4F80C0(int x0, int y0, int x1, int y1, int r, int g, int b)
{
    if (!PSX_Clip_Line_Segments_4FE4F0(&x0, &y0, &x1, &y1))
    {
        return;
    }

    const WORD g_s3 = static_cast<WORD>(g >> 3);
    const WORD r_s3 = static_cast<WORD>(r >> 3);
    const WORD b_s3 = static_cast<WORD>(b >> 3);
    const WORD fill_colour = (r_s3 << sRedShift_C215C4) | (g_s3 << sGreenShift_C1D180) | (b_s3 << sBlueShift_C19140);

    const unsigned int pitch = spBitmap_C2D038->field_10_locked_pitch / sizeof(WORD);

    int x1_max = x1;
    int y1_max = y1;
    int x0_max = x0;
    int y0_max = y0;

    int x_diff = x1 - x0;
    int y_diff = y1 - y0;

    if (y_diff < 0)
    {
        y_diff = y0 - y1;
    }

    if (x_diff < 0)
    {
        x_diff = x0 - x1;
    }

    if (y_diff <= x_diff)
    {
        // Direction of y matter, recalc in case previous < 0 check changed it
        y_diff = y1 - y0;

        // Horizontal segment
        if (x0 > x1)
        {
            // Swap points
            y1_max = y0;
            y0_max = y1;
            x1_max = x0;
            x0_max = x1;
            y1 = y0;
            y0 = y0_max;
            x1 = x0;
            x0 = x0_max;
        }
        const int width = x1_max - x0_max;
        if (width > 0)
        {
            y_diff = ((y1_max - y0_max) << 16) / width;
        }

        WORD* pVRam = reinterpret_cast<WORD*>(spBitmap_C2D038->field_4_pLockedPixels) + x0_max;
        int yPos = (y0_max << 16) + 0x7FFF; // +1 in fixed ?
        for (int i = 0; i <= width; i++)
        {
            pVRam[pitch * (yPos >> 16)] = fill_colour;
            pVRam++;
            yPos += y_diff;
        }
    }
    else
    {
        // Vertical segment
        if (y0 > y1)
        {
            // Swap points
            y1_max = y0;
            y0_max = y1;
            x1_max = x0;
            x0_max = x1;
            y1 = y0;
            y0 = y0_max;
            x1 = x0;
            x0 = x0_max;
        }
        int height = y1_max - y0_max;
        if (height > 0)
        {
            x_diff = ((x1_max - x0_max) << 16) / height;
        }

        WORD* pVRam = reinterpret_cast<WORD*>(spBitmap_C2D038->field_4_pLockedPixels) + (y0_max * pitch);
        int xpos = (x0_max << 16) + 0x7FFF; // +1 in fixed ?
        for (int i = 0; i <= height; i++)
        {
            pVRam[xpos >> 16] = fill_colour;
            pVRam += pitch;
            xpos += x_diff;
        }
    }
}

EXPORT void CC PSX_Render_Line_Prim_4F7D90(void* pOtPrim, int offX, int offY)
{
    PrimAny any;
    any.mVoid = pOtPrim;

    switch (PSX_Prim_Code_Without_Blending_Or_SemiTransparency(any.mPrimHeader->rgb_code.code_or_pad))
    {
    case PrimTypeCodes::eLineG2:
    {
        Line_G2* pLine = any.mLineG2;
        PSX_EMU_Render_G_LineSegment_4F8250(
            pOtPrim,
            offX + X0(pLine),
            offY + Y0(pLine),
            offX + X1(pLine),
            offY + Y1(pLine),
            R0(pLine), G0(pLine), B0(pLine),
            R1(pLine), G1(pLine), B1(pLine));
    }
    break;

    case PrimTypeCodes::eLineG3:
    {
        Line_G3* pLine = any.mLineG3;
        PSX_EMU_Render_G_LineSegment_4F8250(
            pOtPrim,
            offX + X0(pLine),
            offY + Y0(pLine),
            offX + X1(pLine),
            offY + Y1(pLine),
            R0(pLine), G0(pLine), B0(pLine),
            R1(pLine), G1(pLine), B1(pLine));

        PSX_EMU_Render_G_LineSegment_4F8250(
            pOtPrim,
            offX + X1(pLine),
            offY + Y1(pLine),
            offX + X2(pLine),
            offY + Y2(pLine),
            R1(pLine), G1(pLine), B1(pLine),
            R2(pLine), G2(pLine), B2(pLine));
    }
    break;

    case PrimTypeCodes::eLineG4:
    {
        Line_G4* pLine = any.mLineG4;
        PSX_EMU_Render_G_LineSegment_4F8250(
            pOtPrim,
            offX + X0(pLine),
            offY + Y0(pLine),
            offX + X1(pLine),
            offY + Y1(pLine),
            R0(pLine), G0(pLine), B0(pLine),
            R1(pLine), G1(pLine), B1(pLine));

        PSX_EMU_Render_G_LineSegment_4F8250(
            pOtPrim,
            offX + X1(pLine),
            offY + Y1(pLine),
            offX + X2(pLine),
            offY + Y2(pLine),
            R1(pLine), G1(pLine), B1(pLine),
            R2(pLine), G2(pLine), B2(pLine));

        PSX_EMU_Render_G_LineSegment_4F8250(
            pOtPrim,
            offX + X2(pLine),
            offY + Y2(pLine),
            offX + X3(pLine),
            offY + Y3(pLine),
            R2(pLine), G2(pLine), B2(pLine),
            R3(pLine), G3(pLine), B3(pLine));
    }
    break;

    case PrimTypeCodes::eLineF2:
    {
        Line_F2* pLine = any.mLineF2;
        PSX_EMU_Render_F_LineSegment_4F80C0(
            offX + X0(pLine),
            offY + Y0(pLine),
            offX + X1(pLine),
            offY + Y1(pLine), 
            R0(pLine), G0(pLine), B0(pLine));
    } 
    break;

    case PrimTypeCodes::eLineF3:
    {
        Line_F3* pLine = any.mLineF3;
        PSX_EMU_Render_F_LineSegment_4F80C0(
            offX + X0(pLine),
            offY + Y0(pLine),
            offX + X1(pLine),
            offY + Y1(pLine),
            R0(pLine), G0(pLine), B0(pLine));

        PSX_EMU_Render_F_LineSegment_4F80C0(
            offX + X1(pLine),
            offY + Y1(pLine),
            offX + X2(pLine),
            offY + Y2(pLine),
            R0(pLine), G0(pLine), B0(pLine));
    }
    break;

    case PrimTypeCodes::eLineF4:
    {
        Line_F4* pLine = any.mLineF4;
        PSX_EMU_Render_F_LineSegment_4F80C0(
            offX + X0(pLine),
            offY + Y0(pLine),
            offX + X1(pLine),
            offY + Y1(pLine),
            R0(pLine), G0(pLine), B0(pLine));

        PSX_EMU_Render_F_LineSegment_4F80C0(
            offX + X1(pLine),
            offY + Y1(pLine),
            offX + X2(pLine),
            offY + Y2(pLine),
            R0(pLine), G0(pLine), B0(pLine));

        PSX_EMU_Render_F_LineSegment_4F80C0(
            offX + X2(pLine),
            offY + Y2(pLine),
            offX + X3(pLine),
            offY + Y3(pLine),
            R0(pLine), G0(pLine), B0(pLine));
    }
    break;
    }
}

EXPORT void CC PSX_84_4F7B80(int xpos, int ypos, int width, int height, WORD* pData)
{
    const WORD pixel_mask = ~((1 << sRedShift_C215C4) | (1 << sGreenShift_C1D180) | (1 << sBlueShift_C19140) | (1 << sSemiTransShift_C215C0));
    const int pitchWords = spBitmap_C2D038->field_10_locked_pitch / sizeof(WORD);

    WORD* pDst = reinterpret_cast<WORD*>(sPsxVram_C1D160.field_4_pLockedPixels) + (xpos + (ypos * (pitchWords)));

    const int clipx_exp = sPsx_drawenv_clipx_BDCD40 / 16;
    const int clipw_exp = sPsx_drawenv_clipw_BDCD48 / 16;

    int xClipped = 0;
    if (xpos >= clipx_exp)
    {
        xClipped = 0;
    }
    else
    {
        xClipped = (clipx_exp - xpos) / 2;
    }

    const int width_count = width - xpos;

    int widthClipped = 0;
    if (xpos + width_count <= clipw_exp)
    {
        widthClipped = width_count;
    }
    else
    {
        widthClipped = clipw_exp - xpos;
    }

    const int wCountToWrite = widthClipped / 2;
    const int width_count_div4 = width_count / 4;

    WORD* pDstIter = &pDst[2 * xClipped];
    WORD* pSrc1 = &pData[xClipped / 2];
    WORD* pSrc2 = &pData[xClipped / 2];

    if (ypos < sPsx_drawenv_clipy_BDCD44 / 16)
    {
        ypos = sPsx_drawenv_clipy_BDCD44 / 16;
        pSrc2 = pSrc1;
    }

    int yCountToWrite = height - 1;
    if (yCountToWrite > sPsx_drawenv_cliph_BDCD4C / 16 + 1)
    {
        yCountToWrite = sPsx_drawenv_cliph_BDCD4C / 16 + 1;
    }

    for (int yCounter = ypos; yCounter < yCountToWrite; yCounter++)
    {
        int dst_idx = yCounter & 1;
        WORD* pSrcIter = pSrc2;
        WORD* pDstLineIter = &pDstIter[dst_idx];

        for (int xCounter = xClipped; xCounter < wCountToWrite; xCounter++)
        {
            if (*pSrcIter)
            {
                *pDstLineIter = (*pSrcIter + (unsigned int)(unsigned __int16)(pixel_mask & *pDstLineIter)) >> 1;
            }
            pDstLineIter += 2;
            pSrcIter += (xCounter & 1);
        }

        if (!dst_idx)
        {
            pSrc2 += width_count_div4;
        }

        pDstIter += pitchWords; // to next Y line
    }
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
            // This works by func 1 populating some data structure and then func 2 does the actual rendering
            // for POLY_FT4 it may return nullptr as it short circuits this logic and renders the polygon itself in some cases.
            OT_Prim* pPolyBuffer = PSX_Render_Convert_Polys_To_Internal_Format_4F7110(any.mVoid, drawEnv_of0, drawEnv_of1);
            if (pPolyBuffer)
            {
                PSX_Render_Internal_Format_Polygon_4F7960(pPolyBuffer, drawEnv_of0, drawEnv_of1);
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
        if (pOtItem == reinterpret_cast<int**>(0xFFFFFFFF))
        {
            break;
        }

        SsSeqCalledTbyT_4FDC80();

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
                PSX_MoveImage_4F5D50(any.mMoveImage->mpRect, any.mMoveImage->xPos, any.mMoveImage->yPos);

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

            case PrimTypeCodes::eGas:
                // Appears for gas..
                PSX_84_4F7B80(any.mGas->x, any.mGas->y, any.mGas->w, any.mGas->h, any.mGas->pData);
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
    
    Psx_Test* pAbrLut = nullptr;
    if (bSemiTrans)
    {
        pAbrLut = &sPsx_abr_lut_C215E0[sTexture_page_abr_BD0F18];
    }

    const unsigned int texture_remainder_pitch = 2048 - (rect_w  / 2);
    const unsigned int pitchWords = spBitmap_C2D038->field_10_locked_pitch / 2;
    const unsigned int vram_remainder_pitch = pitchWords - pRect->w;
    const BYTE* pTexture_4bit_src1 = (BYTE *)sPsxVram_C1D160.field_4_pLockedPixels + 2 * (sTexture_page_x_BD0F0C + (u / 4) + (tpagey / 1024));
    WORD* pVram_start = (WORD *)((char *)spBitmap_C2D038->field_4_pLockedPixels + 2 * (pRect->x + pitchWords * pRect->y));

    if (r != 128 || g != 128 || b != 128)
    {
        if (bSemiTrans)
        {
            const Psx_Data* pLut_r = &stru_C1D1C0[r >> 3];
            const Psx_Data* pLut_g = &stru_C1D1C0[g >> 3];
            const Psx_Data* pLut_b = &stru_C1D1C0[b >> 3];
            for (int y = 0; y < pRect->h; y++)
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
            }
        }
        else
        {
            const __int16* g_lut = stru_C146C0.g[g >> 3];
            const __int16* r_lut = stru_C146C0.r[r >> 3];
            const __int16* b_lut = stru_C146C0.b[b >> 3];

            for (int y = 0; y < pRect->h; y++)
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

                pTexture_4bit_src1 += texture_remainder_pitch;
                pVram_start += vram_remainder_pitch;
            }
        }
    }
    else if (bSemiTrans)
    {
        for (int y = 0; y < pRect->h; y++)
        {
            WORD* pLineEnd = &pVram_start[pRect->w];
            while (pVram_start < pLineEnd)
            {
                const WORD clut_4_bit_idx4 = pClutSrc1[*pTexture_4bit_src1 & 0xF];
                if (clut_4_bit_idx4)
                {
                    const WORD vram_pixel2 = *pVram_start;

                    *pVram_start = Calc_Abr_Pixel(*pAbrLut, vram_pixel2, clut_4_bit_idx4);
                }

                ++pVram_start;
                if (pVram_start >= pLineEnd)
                {
                    break;
                }

                const unsigned int clut_4_bit_idx5 = (*pTexture_4bit_src1) >> 4;
                const WORD clut_pixel5 = pClutSrc1[clut_4_bit_idx5];
                if (clut_pixel5)
                {
                    const WORD vram_pixel2 = *pVram_start;
                    *pVram_start = Calc_Abr_Pixel(*pAbrLut, vram_pixel2, clut_pixel5);
                }

                pTexture_4bit_src1++;
                ++pVram_start;
            }

            pTexture_4bit_src1 += texture_remainder_pitch;
            pVram_start += vram_remainder_pitch;
        }
    }
    else
    {
        for (int y = 0; y < pRect->h; y++)
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

ALIVE_ARY(1, 0xBD1D00, OT_Prim, 7, stru_BD1D00, {});

OT_Prim* PSX_clip_polys_impl(OT_Prim* pOt)
{
    OT_Prim primLocal = {};

    primLocal.field_0 = pOt->field_0;
    primLocal.field_4 = pOt->field_4;
    primLocal.field_8_r = pOt->field_8_r;
    primLocal.field_A_b = pOt->field_A_b;
    primLocal.field_9_g = pOt->field_9_g;
    primLocal.field_E = pOt->field_E;
    primLocal.field_B_flags = pOt->field_B_flags;
    primLocal.field_12_clut = pOt->field_12_clut;
    primLocal.field_10_tpage = pOt->field_10_tpage;

    // Copy stuff assigned above plus the first vertex, even though first vertex is random data at this point ??
    memcpy(stru_BD1D00, &primLocal, 60);

    OT_Prim* pLocal = &primLocal;
    OT_Prim* result = &primLocal;

    int idx = 0;
    while (1)
    {
        OT_Vert* pLastVert =  &pOt->field_14_verts[pOt->field_C_vert_count];
        OT_Vert* pLastVert1 = &pOt->field_14_verts[pOt->field_C_vert_count - 1];
        OT_Vert* pLastVert2 = &pOt->field_14_verts[pOt->field_C_vert_count - 1];
        
        bool bClip = false;
        switch (idx)
        {
        case 0:
            bClip = pLastVert1->field_0_x0 < sPsx_drawenv_clipx_BDCD40;
            break;

        case 1:
            bClip = pLastVert1->field_4_y0 < sPsx_drawenv_clipy_BDCD44;
            break;

        case 2:
            bClip = pLastVert1->field_0_x0 > sPsx_drawenv_clipw_BDCD48;
            break;

        case 3:
            bClip = pLastVert1->field_4_y0 > sPsx_drawenv_cliph_BDCD4C;
            break;
        }

        OT_Vert* pVerts = pOt->field_14_verts;
        result->field_C_vert_count = 0;

        if (pVerts < pLastVert)
        {
            while (1)
            {
                const int x0 = pVerts->field_0_x0;
                const int y0 = pVerts->field_4_y0;
                int x0_diff = pVerts->field_0_x0 - pLastVert1->field_0_x0;
                int y0_diff = pVerts->field_4_y0 - pLastVert1->field_4_y0;

                bool bWasClipped = false;
                float scaleFactor = 0.0f;
                OT_Vert* pTmpVert = nullptr;
                switch (idx)
                {
                case 0:
                    bWasClipped = pVerts->field_0_x0 < sPsx_drawenv_clipx_BDCD40;
                    if (bClip != bWasClipped)
                    {
                        if (pVerts->field_0_x0 == pLastVert1->field_0_x0)
                        {
                            x0_diff = 1;
                        }

                        scaleFactor = (float)(sPsx_drawenv_clipx_BDCD40 - pVerts->field_0_x0) / (float)x0_diff;
                        pTmpVert = &result->field_14_verts[result->field_C_vert_count++];
                        pTmpVert->field_4_y0 = static_cast<int>(((float)(y0_diff * scaleFactor) + (float)pVerts->field_4_y0));
                        pTmpVert->field_0_x0 = sPsx_drawenv_clipx_BDCD40;
                        result = pLocal;
                    }
                    break;

                case 1:
                    bWasClipped = pVerts->field_4_y0 < sPsx_drawenv_clipy_BDCD44;
                    if (bClip != bWasClipped)
                    {
                        if (pVerts->field_4_y0 == pLastVert1->field_4_y0)
                        {
                            y0_diff = 1;
                        }

                        scaleFactor = (float)(sPsx_drawenv_clipy_BDCD44 - pVerts->field_4_y0) / (float)y0_diff;
                        pTmpVert = &result->field_14_verts[result->field_C_vert_count++];
                        pTmpVert->field_0_x0 = static_cast<int>(((float)(x0_diff * scaleFactor) + (float)pVerts->field_0_x0));
                        pTmpVert->field_4_y0 = sPsx_drawenv_clipy_BDCD44;
                        result = pLocal;
                    }
                    break;

                case 2:
                    bWasClipped = pVerts->field_0_x0 > sPsx_drawenv_clipw_BDCD48;
                    if (bClip != bWasClipped)
                    {
                        if (pVerts->field_0_x0 == pLastVert1->field_0_x0)
                        {
                            x0_diff = 1;
                        }
                        
                        scaleFactor = (float)(sPsx_drawenv_clipw_BDCD48 - pVerts->field_0_x0) / (float)x0_diff;
                        pTmpVert = &result->field_14_verts[result->field_C_vert_count++];
                        pTmpVert->field_4_y0 = static_cast<int>(((float)(y0_diff * scaleFactor) + (float)pVerts->field_4_y0));
                        pTmpVert->field_0_x0 = sPsx_drawenv_clipw_BDCD48;
                        result = pLocal;
                    }
                    break;

                case 3:
                    bWasClipped = pVerts->field_4_y0 > sPsx_drawenv_cliph_BDCD4C;
                    if (bClip != bWasClipped)
                    {
                        if (pVerts->field_4_y0 == pLastVert1->field_4_y0)
                        {
                            y0_diff = 1;
                        }

                        scaleFactor = (float)(sPsx_drawenv_cliph_BDCD4C - pVerts->field_4_y0) / (float)y0_diff;
                        pTmpVert = &result->field_14_verts[result->field_C_vert_count++];
                        pTmpVert->field_0_x0 =  static_cast<int>((float)(x0_diff * scaleFactor) + (float)pVerts->field_0_x0);
                        pTmpVert->field_4_y0 = sPsx_drawenv_cliph_BDCD4C;
                        result = pLocal;
                    }
                    break;
                }

                if (bClip != bWasClipped)
                {
                    OT_Prim* pOtCopy = pOt;
                    if (pOt->field_B_flags & 4)
                    {
                        if (pOt->field_E & 1)
                        {
                            // TODO: Dead branch, u/v is never treated as floats, must have been a union in the real code?
                            LOG_ERROR("Never expected float flag to be set, bugs ahead");
                            pTmpVert->field_14_u = (int)((float)(pVerts->field_14_u - pLastVert2->field_14_u) * scaleFactor + (float)pVerts->field_14_u);
                            pTmpVert->field_18_v = (int)((float)(pVerts->field_18_v - pLastVert2->field_18_v) * scaleFactor + (float)pVerts->field_18_v);
                            pTmpVert->field_10 = (int)((float)(pVerts->field_10 - pLastVert2->field_10) * scaleFactor + (float)pVerts->field_10);
                        }
                        else
                        {
                            pTmpVert->field_14_u = (int)((float)(pVerts->field_14_u - pLastVert2->field_14_u) * scaleFactor + (float)pVerts->field_14_u);
                            pTmpVert->field_18_v = (int)((float)(pVerts->field_18_v - pLastVert2->field_18_v) * scaleFactor + (float)pVerts->field_18_v);
                            pOtCopy = pOt;
                        }
                    }

                    if (pOtCopy->field_B_flags & 0x10)
                    {
                        pTmpVert->field_1C_r = (int)((float)(pVerts->field_1C_r - pLastVert2->field_1C_r) * scaleFactor + (float)pVerts->field_1C_r);
                        pTmpVert->field_20_g = (int)((float)(pVerts->field_20_g - pLastVert2->field_20_g) * scaleFactor + (float)pVerts->field_20_g);
                        pTmpVert->field_24_b = (int)((float)(pVerts->field_24_b - pLastVert2->field_24_b) * scaleFactor + (float)pVerts->field_24_b);
                    }

                    pTmpVert->field_8 = (int)((float)(pVerts->field_8 - pLastVert2->field_8) * scaleFactor + (float)pVerts->field_8);
                    result = pLocal;
                }

                if (!bWasClipped)
                {
                    memcpy(
                        &result->field_14_verts[result->field_C_vert_count++],
                        pVerts,
                        sizeof(OT_Vert));
                }

                pLastVert2 = pVerts;
                pVerts++;
                bClip = bWasClipped;

                if (pVerts >= pLastVert)
                {
                    break;
                }

                pLastVert1 = pLastVert2;
            } // inner loop
        }

        if (idx > 0)
        {
            pLocal = pOt;
        }
        else
        {
            pLocal = stru_BD1D00;
        }

        ++idx;

        pOt = result;

        if (idx >= 4)
        {
            return result;
        }

        result = pLocal;
    } // outer loop
}

EXPORT OT_Prim* CC PSX_clip_polys_4FE710(OT_Prim* pOt)
{
    return PSX_clip_polys_impl(pOt);
}

namespace Test
{
    const BYTE kTestPal[] = 
    {
        0x00, 0x00, 0xC4, 0x98, 0x22, 0x80, 0x83, 0x90, 0x20,
        0x80, 0x00, 0x84, 0x42, 0x88, 0xC6, 0x98, 0xAD, 0x2D,
        0xE9, 0x14, 0xC7, 0x14, 0x44, 0x04, 0xE6, 0x1C, 0x43,
        0x04, 0x87, 0x08, 0x43, 0x00, 0xE5, 0x1C, 0x84, 0x0C,
        0x23, 0x00, 0x62, 0x0C, 0x21, 0x04, 0x42, 0x00, 0x49,
        0x25, 0xA5, 0x10, 0xE8, 0x18, 0x21, 0x00, 0x62, 0x08,
        0xA6, 0x10, 0x20, 0x00, 0x8C, 0x29, 0xE8, 0x10, 0x86,
        0x08, 0x83, 0x0C, 0x28, 0x21, 0x63, 0x08, 0x83, 0x08,
        0x63, 0x04, 0xE6, 0x14, 0x85, 0x0C, 0x84, 0x10, 0x6B,
        0x25, 0x21, 0x08, 0x08, 0x19, 0xC6, 0x14, 0x43, 0x08,
        0x42, 0x08, 0x4B, 0x21, 0x63, 0x0C, 0x06, 0x1D, 0x42,
        0x04, 0xC5, 0x14, 0x2A, 0x1D, 0x42, 0x0C, 0x22, 0x00,
        0xA3, 0x10, 0x09, 0x1D, 0x62, 0x10, 0x41, 0x04, 0xC8,
        0x10, 0x45, 0x04, 0x00, 0x04, 0x00, 0x04, 0x01, 0x00,
        0x11, 0x3A
    };

    static WORD RGB888toRGB565(unsigned int r, unsigned int g, unsigned int b)
    {
        return static_cast<WORD>((r >> 3 << 11) + (g >> 2 << 5) + (b >> 3));
    }

    const WORD kTestImagePal[16] =
    {
        RGB888toRGB565(237, 28,  36 ),
        RGB888toRGB565(255, 255, 255 ),
        RGB888toRGB565(127, 127, 127 ),
        RGB888toRGB565(63,  72,  204 ),
        RGB888toRGB565(34,  177, 76  ),
        RGB888toRGB565(255, 174, 201 ),
        RGB888toRGB565(255, 127, 39  ),
        RGB888toRGB565(163, 73,  164 ),
    };

    const BYTE kTestImg[4][8] =
    {
        { 0, 0, 1, 2, 2, 2, 1, 3 },
        { 0, 0, 1, 1, 2, 1, 1, 3 },
        { 4, 4, 5, 1, 2, 1, 6, 7 },
        { 4, 4, 1, 5, 2, 6, 1, 7 },
    };

    inline static BYTE AsByte(BYTE nibble1, BYTE nibble2)
    {
        return (nibble2 << 4) | nibble1;
    }

    // Pack kTestImg nibbles into bytes
    const BYTE kTestImage[4][4] =
    {
        { AsByte(kTestImg[0][0],kTestImg[0][1]), AsByte(kTestImg[0][2],kTestImg[0][3]), AsByte(kTestImg[0][4],kTestImg[0][5]), AsByte(kTestImg[0][6],kTestImg[0][7]) },
        { AsByte(kTestImg[1][0],kTestImg[1][1]), AsByte(kTestImg[1][2],kTestImg[1][3]), AsByte(kTestImg[1][4],kTestImg[1][5]), AsByte(kTestImg[1][6],kTestImg[1][7]) },
        { AsByte(kTestImg[2][0],kTestImg[2][1]), AsByte(kTestImg[2][2],kTestImg[2][3]), AsByte(kTestImg[2][4],kTestImg[2][5]), AsByte(kTestImg[2][6],kTestImg[2][7]) },
        { AsByte(kTestImg[3][0],kTestImg[3][1]), AsByte(kTestImg[3][2],kTestImg[3][3]), AsByte(kTestImg[3][4],kTestImg[3][5]), AsByte(kTestImg[3][6],kTestImg[3][7]) },
    };

    // Pack kTestImage bytes into a RLE compressed buffer
    const BYTE kTestImageCompressed[] =
    {
        8, 0,               // u16 width
        4, 0,               // u16 height
        AsByte(0, 8),       // (black pixel count, direct copy count) both are limited to max 16,16 since 4 bits each
        kTestImage[0][0],
        kTestImage[0][1],
        kTestImage[0][2],
        kTestImage[0][3],

        AsByte(0, 8),
        kTestImage[1][0],
        kTestImage[1][1],
        kTestImage[1][2],
        kTestImage[1][3],

        AsByte(0, 8),
        kTestImage[2][0],
        kTestImage[2][1],
        kTestImage[2][2],
        kTestImage[2][3],

        AsByte(0, 8),
        kTestImage[3][0],
        kTestImage[3][1],
        kTestImage[3][2],
        kTestImage[3][3],
    };

    WORD vramTest[512][1024] = {};

    BYTE kSinglePixel8BitImg[4] = 
    {
        0x0,
        0x0,
        0x0,
        0x0
    };

    void Test_PSX_8Bit_PolyFT4()
    {
        sPsxVram_C1D160.field_4_pLockedPixels = vramTest;
        sPsxVram_C1D160.field_10_locked_pitch = 2048;
        spBitmap_C2D038 = &sPsxVram_C1D160;

       
        OT_Prim otPrim = {};
        otPrim.field_12_clut = static_cast<short>(PSX_getClut_4F6350(0, 256));
        otPrim.field_14_verts[0].field_0_x0 = 0;
        otPrim.field_14_verts[0].field_4_y0 = 0;
        otPrim.field_14_verts[3].field_0_x0 = 8;
        otPrim.field_14_verts[3].field_4_y0 = 4;

        otPrim.field_14_verts[0].field_14_u = 0;
        otPrim.field_14_verts[2].field_14_u = 1;

        otPrim.field_14_verts[0].field_18_v = 0;
        otPrim.field_14_verts[2].field_18_v = 1;
        

        // TODO: Clut upload
        WORD* pClut = (WORD *)((char *)sPsxVram_C1D160.field_4_pLockedPixels + 32 * ((otPrim.field_12_clut & 0x3F) + (otPrim.field_12_clut >> 6 << 6)));
      
        otPrim.field_8_r = 127;
        otPrim.field_9_g = 127;
        otPrim.field_A_b = 127;

        PSX_EMU_SetDispType_4F9960(5);

        otPrim.field_B_flags |= 1;
        memset(vramTest, 0, sizeof(vramTest));
        memcpy(pClut, kTestPal, sizeof(kTestPal));

       // PSX_Render_PolyFT4_8bit_Opaque_5006E0(&otPrim, 1, 1, (DWORD*)&kSinglePixel8BitImg[0]);
        PSX_Render_PolyFT4_8bit_SemiTrans_501B00(&otPrim, 2, 2, (DWORD*)&kSinglePixel8BitImg[0]);
    }


    static void Test_PSX_4Bit_PolyFT4()
    {
        memset(vramTest, 0, sizeof(vramTest));

        BYTE decompressed[4][4] = {};
        CompressionType6Ae_Decompress_40A8A0(&kTestImageCompressed[0], &decompressed[0][0]);

        sPsxVram_C1D160.field_4_pLockedPixels = vramTest;
        sPsxVram_C1D160.field_10_locked_pitch = 2048;
        spBitmap_C2D038 = &sPsxVram_C1D160;

        // TODO: Clut upload

        OT_Prim otPrim = {};
        otPrim.field_14_verts[0].field_0_x0 = 0;
        otPrim.field_14_verts[0].field_4_y0 = 0;
        otPrim.field_14_verts[3].field_0_x0 = 8;
        otPrim.field_14_verts[3].field_4_y0 = 4;

        otPrim.field_8_r = 127;
        otPrim.field_9_g = 127;
        otPrim.field_A_b = 127;

        PSX_EMU_SetDispType_4F9960(5);

       // PSX_Render_PolyFT4_4bit_Opqaue_50CDB0(&otPrim, 8, 4, (DWORD*)&kTestImageCompressed[0]);
    }

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

            ASSERT_EQ(off_578330->field_D_bClip, 1);
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

            ASSERT_EQ(off_578330->field_D_bClip, 1);
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

    static void Test_PSX_poly_FShaded_NoTexture_517DF0()
    {
        {
            Render_Unknown origin = {};
            Render_Unknown slope = {};

            OT_Vert verts[2] = {};
            pVerts_dword_BD3264 = verts;

            verts[0].field_0_x0 = 1 * 16;
            verts[0].field_4_y0 = 1 * 16;

            verts[1].field_0_x0 = 640 * 16;
            verts[1].field_4_y0 = 480 * 16;

            PSX_poly_FShaded_NoTexture_517DF0(&origin, &slope, 0, 1);

            ASSERT_EQ(0x00015582, slope.field_0_x);
            ASSERT_EQ(0x00001df0, slope.field_4_y);
            ASSERT_EQ(0x00010000, origin.field_0_x);
            ASSERT_EQ(0x00000000, origin.field_4_y);
        }

        {
            Render_Unknown origin = {};
            Render_Unknown slope = {};

            OT_Vert verts[2] = {};
            pVerts_dword_BD3264 = verts;

            verts[0].field_0_x0 = 1 * 16;
            verts[0].field_4_y0 = 1;

            verts[1].field_0_x0 = 640 * 16;
            verts[1].field_4_y0 = 5;

            PSX_poly_FShaded_NoTexture_517DF0(&origin, &slope, 0, 1);

            ASSERT_EQ(167510016, slope.field_0_x);
            ASSERT_EQ(4, slope.field_4_y);
            ASSERT_EQ(-111329280, origin.field_0_x);
            ASSERT_EQ(0, origin.field_4_y);
        }
        pVerts_dword_BD3264 = nullptr;
    }

    static void Test_PSX_poly_helper_fixed_point_scale_517FA0()
    {
        ASSERT_EQ(172890, PSX_poly_helper_fixed_point_scale_517FA0(2088960, 339 * 16));
        ASSERT_EQ(2, PSX_poly_helper_fixed_point_scale_517FA0(0x10000, 2));
        ASSERT_EQ(2, PSX_poly_helper_fixed_point_scale_517FA0(0x12000, 2));
        ASSERT_EQ(2, PSX_poly_helper_fixed_point_scale_517FA0(0x10200, 2));
        ASSERT_EQ(2, PSX_poly_helper_fixed_point_scale_517FA0(0x12020, 2));
        ASSERT_EQ(2, PSX_poly_helper_fixed_point_scale_517FA0(0x10002, 2));
        ASSERT_EQ(4, PSX_poly_helper_fixed_point_scale_517FA0(0x20002, 2));
        ASSERT_EQ(0xFFFE0, PSX_poly_helper_fixed_point_scale_517FA0(0x7FFF0002, 32));
    }
    
    void PsxRenderTests()
    {
        Test_PSX_TPage_Change_4F6430();
        Test_PSX_Rects_intersect_point_4FA100();
        Test_PSX_Render_Convert_Polys_To_Internal_Format_4F7110();
        Test_PSX_poly_FShaded_NoTexture_517DF0();
        Test_PSX_poly_helper_fixed_point_scale_517FA0();
        Test_PSX_4Bit_PolyFT4();
        //Test_PSX_8Bit_PolyFT4();
    }
}

