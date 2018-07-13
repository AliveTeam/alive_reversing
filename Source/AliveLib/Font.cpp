#include "stdafx.h"
#include "Font.hpp"
#include "Function.hpp"

void Font_ForceLink()
{

}

ALIVE_VAR(1, 0x5bc5c8, Font_Context, sFont1Context_5BC5C8, {});
ALIVE_VAR(1, 0x5BC5D8, Font_Context, sFont2Context_5BC5D8, {});

ALIVE_VAR(1, 0x5c9304, char, sDisableFontFlicker_5C9304, 0);

ALIVE_VAR(1, 0x5ca4b4, byte, byte_5CA4B4, 0);

EXPORT signed __int16 __cdecl Pal_Allocate_483110(PSX_RECT *a1, unsigned int paletteColorCount)
{
    NOT_IMPLEMENTED();
}

Font::Font()
{
}

Font::Font(int maxCharLength, BYTE *palette, Font_Context *fontContext)
{
    ctor_433590(maxCharLength, palette, fontContext);
}

void Font::ctor_433590(int maxCharLength, BYTE *palette, Font_Context *fontContext)
{
    field_34_font_context = fontContext;
    Pal_Allocate_483110(&field_28_palette_rect, 0x10u);
    PSX_RECT rect = { field_28_palette_rect.x , field_28_palette_rect.y, 16, 1 };
    PSX_LoadImage16_4F5E20(&rect, palette);
    field_30_poly_count = maxCharLength;
    field_20_fnt_poly_block_ptr = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_FntP, fontContext->field_C_resource_id, sizeof(Poly_FT4) * 2 * maxCharLength);
    field_24_fnt_poly_array = reinterpret_cast<Poly_FT4*>(*field_20_fnt_poly_block_ptr);
}

int Font::DrawString_4337D0(int **ot, char *text, int x, __int16 y, char abr, int bSemiTrans, int a2, int otLayer, char r, char g, char b, int polyOffset, signed int scale, int a15, __int16 colorRandomRange)
{
    if (!byte_5CA4B4) // Todo: Figure this insane shit out.
    {
        auto v17 = ((signed int)(40 * x + 11 + ((unsigned __int64)(-1307163959i64 * (40 * x + 11)) >> 32)) >> 4);
        x = (v17 >> 31) + v17;
    }

    int characterRenderCount = 0;
    const int maxRenderX = (40 * a15 + 11) / 23;
    int offsetX = x;
    int charInfoIndex = 0;
    auto poly = &field_24_fnt_poly_array[gPsxDisplay_5C1130.field_C_buffer_index + (2 * polyOffset)];

    int tpage = PSX_getTPage_4F60E0(0, abr, field_34_font_context->field_0_rect.x & 0xFFC0, field_34_font_context->field_0_rect.y & 0xFF00);
    int clut = PSX_getClut_4F6350(field_28_palette_rect.x, field_28_palette_rect.y);

    for (int i = 0; i < strlen(text); i++)
    {
        if (offsetX >= maxRenderX)
            break;

        const char c = text[i];

        if (c <= 0x20u || c > 0xAFu)
        {
            if (c < 7u || c > 0x1Fu)
            {
                offsetX += field_34_font_context->field_8_atlas_array[1].field_2_width;
                continue;
            }
            charInfoIndex = c + 137;
        }
        else
        {
            charInfoIndex = c - 31;
        }

        const auto fContext = field_34_font_context;
        const auto atlasEntry = &fContext->field_8_atlas_array[charInfoIndex];

        const auto charWidth = atlasEntry->field_2_width;
        const auto charHeight = atlasEntry->field_3_height;
        const auto texture_u = atlasEntry->field_0_x + (4 * (fContext->field_0_rect.x & 0x3F));
        const auto texture_v = atlasEntry->field_1_y + LOBYTE(fContext->field_0_rect.y);

        const auto widthScaled = (signed int)Math_FixedPoint_Multiply_496C50(charWidth << 16, scale) / 0x10000;
        const auto heightScaled = (signed int)Math_FixedPoint_Multiply_496C50(charHeight << 16, scale) / 0x10000;

        PolyFT4_Init_4F8870(poly);
        Poly_Set_SemiTrans_4F8A60(&poly->field_0_header, bSemiTrans);
        Poly_Set_unknown_4F8A20(&poly->field_0_header, a2);

        poly->field_0_header.field_8_r0 = r + Math_RandomRange_496AB0(-colorRandomRange, colorRandomRange);
        poly->field_0_header.field_9_g0 = g + Math_RandomRange_496AB0(-colorRandomRange, colorRandomRange);
        poly->field_0_header.field_A_b0 = b + Math_RandomRange_496AB0(-colorRandomRange, colorRandomRange);
        poly->field_1A_tpage = tpage;
        poly->field_12_clut = clut;

        // Padding
        poly->field_22_pad1 = 0;
        poly->field_2A_pad2 = 0;

        // P0
        poly->field_C_x0 = offsetX;
        poly->field_E_y0 = y;
        poly->field_10_u0 = texture_u;
        poly->field_11_v0 = texture_v;

        // P1
        poly->field_14_x1 = offsetX + widthScaled;
        poly->field_16_y1 = y;
        poly->field_18_u1 = texture_u + charWidth;
        poly->field_19_v1 = texture_v;

        // P2
        poly->field_1C_x2 = offsetX;
        poly->field_1E_y2 = y + heightScaled;
        poly->field_20_u2 = texture_u;
        poly->field_21_v2 = texture_v + charHeight;

        // P3
        poly->field_24_x3 = offsetX + widthScaled;
        poly->field_26_y3 = y + heightScaled;
        poly->field_28_u3 = texture_u + charWidth;
        poly->field_29_v3 = texture_v + charHeight;

        OrderingTable_Add_4F8AA0(&ot[otLayer], poly);

        ++characterRenderCount;

        offsetX += widthScaled + (Math_FixedPoint_Multiply_496C50(field_34_font_context->field_8_atlas_array[0].field_2_width << 16, scale) / 0x10000);

        poly += 2;
    }

    pScreenManager_5BB5F4->InvalidateRect_40EC90(
        (signed __int16)x,
        (signed __int16)(y - 1),
        (signed __int16)offsetX,
        (signed __int16)(y + 24),
        pScreenManager_5BB5F4->field_3A);

    return polyOffset + characterRenderCount;
}

int Font::MeasureWidth_433700(char * text)
{
    int result = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        const char c = text[i];
        int charIndex = 0;

        if (c <= 0x20u || c > 0xAFu)
        {
            if (c < 7u || c > 0x1Fu)
            {
                result += field_34_font_context->field_8_atlas_array[1].field_2_width;
                continue;
            }
            else
            {
                charIndex = c + 137;
            }
        }
        else
        {
            charIndex = c - 31;
        }

        result += field_34_font_context->field_8_atlas_array[0].field_2_width;
        result += field_34_font_context->field_8_atlas_array[charIndex].field_2_width;
    }

    if (!byte_5CA4B4)
        result = (23 * result + 20) / 40;

    return result;
}

// Measures the width of a string with scale applied.
int Font::MeasureWidth_4336C0(char * text, signed int fp_scale)
{
    return (Math_FixedPoint_Multiply_496C50(Font::MeasureWidth_433700(text) << 16, fp_scale) + 0x8000) >> 16;
}

// Measures the width of a single character.
int Font::MeasureWidth_433630(unsigned char character)
{
    int result = 0;
    int charIndex = 0;

    if (character <= 0x20u || character > 0xAFu)
    {
        if (character < 7u || character > 0x1Fu)
        {
            return field_34_font_context->field_8_atlas_array[1].field_2_width;
        }
        charIndex = character + 137;
    }
    else
    {
        charIndex = character - 31;
    }
    result = field_34_font_context->field_8_atlas_array[charIndex].field_2_width;

    if (!byte_5CA4B4)
    {
        result = (23 * result + 20) / 40;
    }

    return result;
}
