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
    NOT_IMPLEMENTED();
    return 0;
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

        result += field_34_font_context->field_8_atlas_array->field_2_width;
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
