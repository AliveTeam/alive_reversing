#include "stdafx.h"
#include "Font.hpp"
#include "Function.hpp"
#include "DebugHelpers.hpp"
#include "FixedPoint.hpp"
#include "VRam.hpp"

void Font_ForceLink()
{

}

/*
 Notes for when coming across crazy width conversions:
 If it looks like this:

 (40 * x + 11) / 23

 Then its probably x *= 0.575;
 If it looks like:

 v17 = (signed int)(40 * x + 11 + ((unsigned __int64)(-1307163959i64 * (40 * x + 11)) >> 32)) >> 4;
 x = (v17 >> 31) + v17;

 then do x /= 0.575;
*/

EXPORT void CC static_font1context_dtor_433360()
{
    sFont1Context_5BC5C8.dtor_433510();
}

EXPORT void CC static_font1context_init_433300()
{
    //static_font1context_ctor_nullsub_47();
    atexit(static_font1context_dtor_433360);
}

EXPORT void CC static_font2context_dtor_4333E0()
{
    sFont2Context_5BC5D8.dtor_433510();
}

EXPORT void CC static_font2context_init_433380()
{
    //static_font2context_ctor_nullsub_48();
    atexit(static_font2context_dtor_4333E0);
}

ALIVE_VAR(1, 0x5bc5c8, Font_Context, sFont1Context_5BC5C8, {});
ALIVE_VAR(1, 0x5BC5D8, Font_Context, sFont2Context_5BC5D8, {});

ALIVE_VAR(1, 0x5c9304, char, sDisableFontFlicker_5C9304, 0);

ALIVE_VAR(1, 0x5ca4b4, byte, sFontDrawScreenSpace_5CA4B4, 0);

ALIVE_VAR(1, 0x5BC5E8, __int16, sFontType2LoadCount_5BC5E8, 0);

static std::vector<std::vector<BYTE>> sLoadedAtlas;

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

void Font::dtor_433540()
{
    PSX_Point palPoint = { field_28_palette_rect.x,field_28_palette_rect.y };
    Pal_free_483390(palPoint, field_28_palette_rect.w);
    field_28_palette_rect.x = 0;
    ResourceManager::FreeResource_49C330(field_20_fnt_poly_block_ptr);
}

int Font::DrawString_4337D0(int **ot, const char *text, int x, __int16 y, char abr, int bSemiTrans, int a2, int otLayer, char r, char g, char b, int polyOffset, FP scale, int a15, __int16 colorRandomRange)
{
    if (!sFontDrawScreenSpace_5CA4B4)
    {
        x /= 0.575; // 368 to 640. Convert world space to screen space coords.
    }

    int characterRenderCount = 0;
    const int maxRenderX = a15 / 0.575;
    short offsetX = static_cast<short>(x);
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

        const auto widthScaled = static_cast<signed int>(charWidth * scale.GetDouble());
        const auto heightScaled = static_cast<signed int>(charHeight * scale.GetDouble());

        PolyFT4_Init_4F8870(poly);
        Poly_Set_SemiTrans_4F8A60(&poly->field_0_header, bSemiTrans);
        Poly_Set_unknown_4F8A20(&poly->field_0_header, a2);

        poly->field_0_header.field_8_r0 = r + Math_RandomRange_496AB0(-colorRandomRange, colorRandomRange);
        poly->field_0_header.field_9_g0 = g + Math_RandomRange_496AB0(-colorRandomRange, colorRandomRange);
        poly->field_0_header.field_A_b0 = b + Math_RandomRange_496AB0(-colorRandomRange, colorRandomRange);
        poly->field_1A_tpage = static_cast<short>(tpage);
        poly->field_12_clut = static_cast<short>(clut);

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

        offsetX += widthScaled + static_cast<signed int>(field_34_font_context->field_8_atlas_array[0].field_2_width * scale.GetExponent());

        poly += 2;
    }

    pScreenManager_5BB5F4->InvalidateRect_40EC90(x, y - 1, offsetX, y + 24, pScreenManager_5BB5F4->field_3A);

    return polyOffset + characterRenderCount;
}

int Font::MeasureWidth_433700(const char * text)
{
    int result = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        const char c = text[i];
        int charIndex = 0;

        if (c <= 32 || c > 175)
        {
            if (c < 7 || c > 31)
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

    if (!sFontDrawScreenSpace_5CA4B4)
    {
        result *= 0.575; // Convert screen space to world space.
    }

    return result;
}

// Measures the width of a string with scale applied.
int Font::MeasureWidth_4336C0(const char * text, FP scale)
{
    FP ret = (FP(MeasureWidth_433700(text)) * scale) + FP_FromDouble(0.5);
    return ret.GetExponent();
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

    if (!sFontDrawScreenSpace_5CA4B4)
    {
        result *= 0.575; // Convert screen space to world space.
    }

    return result;
}

// Wasn't too sure what to call this. Returns the char offset of where the text is cut off. (left and right region)
const char * Font::SliceText_433BD0(const char * text, int left, FP scale, int right)
{
    NOT_IMPLEMENTED();
}

void Font_Context::LoadFontType_433400(int resourceID)
{
    auto loadedResource = reinterpret_cast<BYTE **>(ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Font, resourceID, 1u, 0));
    auto fontFile = reinterpret_cast<File_Font*>(*loadedResource);
    
    field_C_resource_id = resourceID;

    Vram_alloc_4956C0(fontFile->field_0_width, fontFile->field_2_height, fontFile->field_4_color_depth, &field_0_rect);
    PSX_RECT rect = { field_0_rect.x, field_0_rect.y, fontFile->field_0_width / 4, fontFile->field_2_height };

    if (fontFile->field_4_color_depth == 16)
    {
        PSX_LoadImage16_4F5E20(&rect, fontFile->field_28_pixel_buffer);
    }
    else
    {
        PSX_LoadImage_4F5FB0(&rect, fontFile->field_28_pixel_buffer);
    }
    
    // Free our loaded font resource as its now in vram
    ResourceManager::FreeResource_49C330(loadedResource);

    switch (resourceID)
    {
    case 1:
        field_8_atlas_array = sFont1Atlas_551D34;
        break;
    case 2:
        field_8_atlas_array = sFont2Atlas_551FD8;
        break;
    default:
        ALIVE_FATAL("Unknown font resource ID !!!");
        break;
    }
}

void Font_Context::dtor_433510()
{
    if (field_0_rect.x)
        Vram_free_495A60(*(DWORD *)&field_0_rect.x, *(DWORD *)&field_0_rect.w);
}

bool Font_Context::LoadFontTypeFromFile(const char * fontPath, const char * atlasPath, char * pPaletteOut)
{
    auto debugFont = FS::ReadFile(fontPath);
    auto debugFontAtlas = FS::ReadFile(atlasPath);

    if (!debugFont.size() || !debugFontAtlas.size())
    {
        LOG_ERROR("Could not load custom font!");
        return false;
    }

    sLoadedAtlas.push_back(debugFontAtlas);

    LoadFontTypeCustom(reinterpret_cast<File_Font*>(debugFont.data()), reinterpret_cast<Font_AtlasEntry*>(sLoadedAtlas.back().data()), pPaletteOut);
    return true;
}

void Font_Context::LoadFontTypeCustom(File_Font * fontFile, Font_AtlasEntry * fontAtlas, char * pPaletteOut)
{
    // Give custom fonts a constant resource id for now.
    field_C_resource_id = 0xBEEF;

    Vram_alloc_4956C0(fontFile->field_0_width, fontFile->field_2_height, fontFile->field_4_color_depth, &field_0_rect);
    PSX_RECT rect = { field_0_rect.x, field_0_rect.y, fontFile->field_0_width / 4, fontFile->field_2_height };

    if (pPaletteOut)
    {
        memcpy(pPaletteOut, fontFile->field_8_palette, fontFile->field_6_palette_size * 2);
    }

    if (fontFile->field_4_color_depth == 16)
    {
        PSX_LoadImage16_4F5E20(&rect, fontFile->field_28_pixel_buffer);
    }
    else
    {
        PSX_LoadImage_4F5FB0(&rect, fontFile->field_28_pixel_buffer);
    }

    field_8_atlas_array = fontAtlas;
}

ALIVE_ARY(1, 0x551D34, Font_AtlasEntry, 169, sFont1Atlas_551D34, 
{
    { 0u, 0u, 2u, 0u },
    { 0u, 0u, 9u, 0u },
    { 43u, 80u, 6u, 23u },
    { 182u, 0u, 11u, 10u },
    { 182u, 0u, 11u, 10u },
    { 182u, 0u, 11u, 10u },
    { 182u, 0u, 11u, 10u },
    { 182u, 0u, 11u, 10u },
    { 85u, 96u, 7u, 8u },
    { 52u, 79u, 11u, 23u },
    { 64u, 79u, 11u, 23u },
    { 186u, 0u, 11u, 10u },
    { 0u, 79u, 23u, 17u },
    { 86u, 79u, 7u, 23u },
    { 75u, 79u, 10u, 11u },
    { 94u, 79u, 6u, 23u },
    { 0u, 0u, 9u, 0u },
    { 193u, 51u, 17u, 23u },
    { 32u, 51u, 17u, 23u },
    { 50u, 51u, 17u, 23u },
    { 69u, 51u, 17u, 23u },
    { 87u, 51u, 17u, 23u },
    { 106u, 51u, 17u, 23u },
    { 124u, 51u, 17u, 23u },
    { 142u, 51u, 17u, 23u },
    { 158u, 51u, 17u, 23u },
    { 176u, 51u, 17u, 23u },
    { 110u, 79u, 7u, 22u },
    { 102u, 79u, 7u, 22u },
    { 61u, 79u, 10u, 23u },
    { 25u, 79u, 15u, 16u },
    { 73u, 79u, 10u, 23u },
    { 118u, 79u, 14u, 23u },
    { 182u, 0u, 11u, 10u },
    { 9u, 0u, 17u, 23u },
    { 26u, 0u, 15u, 23u },
    { 41u, 0u, 15u, 23u },
    { 56u, 0u, 16u, 23u },
    { 73u, 0u, 12u, 23u },
    { 86u, 0u, 12u, 23u },
    { 98u, 0u, 15u, 23u },
    { 114u, 0u, 16u, 23u },
    { 132u, 0u, 7u, 23u },
    { 139u, 0u, 14u, 23u },
    { 154u, 0u, 15u, 23u },
    { 170u, 0u, 12u, 23u },
    { 0u, 25u, 21u, 23u },
    { 21u, 25u, 16u, 23u },
    { 37u, 25u, 18u, 23u },
    { 56u, 25u, 14u, 23u },
    { 71u, 25u, 18u, 23u },
    { 90u, 25u, 14u, 23u },
    { 106u, 25u, 14u, 23u },
    { 121u, 25u, 15u, 23u },
    { 137u, 25u, 16u, 23u },
    { 153u, 25u, 17u, 23u },
    { 170u, 25u, 20u, 23u },
    { 191u, 25u, 15u, 23u },
    { 0u, 50u, 17u, 23u },
    { 17u, 50u, 13u, 23u },
    { 61u, 79u, 10u, 23u },
    { 0u, 0u, 9u, 0u },
    { 73u, 79u, 10u, 23u },
    { 186u, 0u, 11u, 10u },
    { 86u, 79u, 27u, 10u },
    { 186u, 0u, 7u, 11u },
    { 0u, 108u, 16u, 24u },
    { 17u, 108u, 13u, 24u },
    { 31u, 108u, 13u, 24u },
    { 44u, 108u, 15u, 24u },
    { 59u, 108u, 11u, 24u },
    { 71u, 108u, 11u, 24u },
    { 83u, 108u, 13u, 24u },
    { 97u, 108u, 14u, 24u },
    { 113u, 108u, 7u, 24u },
    { 120u, 108u, 11u, 24u },
    { 134u, 108u, 13u, 24u },
    { 149u, 108u, 11u, 24u },
    { 0u, 135u, 17u, 24u },
    { 19u, 135u, 15u, 24u },
    { 34u, 135u, 17u, 24u },
    { 52u, 135u, 13u, 24u },
    { 66u, 135u, 17u, 24u },
    { 84u, 135u, 13u, 24u },
    { 97u, 135u, 12u, 24u },
    { 111u, 135u, 14u, 24u },
    { 125u, 135u, 14u, 24u },
    { 141u, 135u, 15u, 24u },
    { 157u, 135u, 17u, 24u },
    { 1u, 163u, 13u, 24u },
    { 16u, 163u, 15u, 24u },
    { 32u, 163u, 12u, 24u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 134u, 79u, 16u, 9u },
    { 0u, 0u, 9u, 0u },
    { 227u, 211u, 12u, 24u },
    { 239u, 187u, 14u, 24u },
    { 233u, 79u, 11u, 24u },
    { 201u, 79u, 14u, 24u },
    { 217u, 79u, 14u, 24u },
    { 187u, 79u, 14u, 24u },
    { 0u, 110u, 16u, 22u },
    { 227u, 211u, 12u, 24u },
    { 211u, 52u, 11u, 24u },
    { 222u, 52u, 11u, 24u },
    { 244u, 79u, 11u, 24u },
    { 213u, 28u, 10u, 24u },
    { 175u, 136u, 8u, 24u },
    { 240u, 51u, 7u, 24u },
    { 218u, 79u, 17u, 24u },
    { 0u, 110u, 16u, 22u },
    { 233u, 79u, 11u, 24u },
    { 223u, 234u, 19u, 20u },
    { 223u, 234u, 19u, 20u },
    { 214u, 162u, 17u, 24u },
    { 230u, 162u, 17u, 24u },
    { 239u, 27u, 16u, 24u },
    { 225u, 187u, 14u, 24u },
    { 210u, 187u, 14u, 24u },
    { 16u, 164u, 15u, 22u },
    { 230u, 162u, 17u, 24u },
    { 239u, 187u, 14u, 24u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 170u, 79u, 16u, 24u },
    { 234u, 51u, 7u, 24u },
    { 223u, 27u, 16u, 24u },
    { 197u, 187u, 14u, 24u },
    { 238u, 211u, 15u, 24u },
    { 238u, 211u, 16u, 24u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 197u, 212u, 14u, 24u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 197u, 0u, 7u, 22u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 186u, 36u, 22u },
    { 39u, 186u, 36u, 22u },
    { 78u, 186u, 36u, 22u },
    { 117u, 186u, 36u, 22u },
    { 156u, 186u, 36u, 22u },
    { 0u, 210u, 36u, 22u },
    { 39u, 210u, 36u, 22u },
    { 78u, 210u, 36u, 22u },
    { 189u, 124u, 30u, 19u },
    { 221u, 124u, 30u, 20u },
    { 224u, 143u, 25u, 21u },
    { 193u, 143u, 25u, 21u },
    { 162u, 110u, 19u, 18u },
    { 211u, 0u, 44u, 26u },
    { 196u, 162u, 16u, 24u },
    { 48u, 162u, 16u, 24u },
    { 64u, 162u, 16u, 24u },
    { 81u, 162u, 16u, 24u },
    { 98u, 162u, 16u, 24u },
    { 115u, 162u, 16u, 24u },
    { 131u, 162u, 16u, 24u },
    { 148u, 162u, 16u, 24u },
    { 163u, 162u, 16u, 24u },
    { 180u, 162u, 16u, 24u }
});

ALIVE_ARY(1, 0x551FD8, Font_AtlasEntry, 169, sFont2Atlas_551FD8,
{
    { 0u, 0u, 2u, 0u },
    { 0u, 0u, 14u, 0u },
    { 105u, 42u, 14u, 13u },
    { 105u, 70u, 14u, 4u },
    { 0u, 0u, 14u, 0u },
    { 75u, 70u, 14u, 13u },
    { 0u, 0u, 14u, 0u },
    { 0u, 0u, 14u, 0u },
    { 105u, 70u, 8u, 5u },
    { 45u, 70u, 14u, 13u },
    { 60u, 70u, 14u, 13u },
    { 105u, 84u, 14u, 0u },
    { 75u, 84u, 14u, 13u },
    { 45u, 42u, 14u, 13u },
    { 60u, 84u, 14u, 13u },
    { 30u, 42u, 14u, 13u },
    { 0u, 0u, 14u, 0u },
    { 0u, 56u, 14u, 13u },
    { 15u, 56u, 14u, 13u },
    { 30u, 56u, 14u, 13u },
    { 45u, 56u, 14u, 13u },
    { 60u, 56u, 14u, 13u },
    { 75u, 56u, 14u, 13u },
    { 90u, 56u, 14u, 13u },
    { 105u, 56u, 14u, 13u },
    { 0u, 70u, 14u, 13u },
    { 15u, 70u, 14u, 13u },
    { 60u, 42u, 14u, 13u },
    { 75u, 42u, 14u, 13u },
    { 45u, 70u, 14u, 13u },
    { 30u, 70u, 14u, 13u },
    { 60u, 70u, 14u, 13u },
    { 90u, 42u, 14u, 13u },
    { 0u, 0u, 14u, 0u },
    { 0u, 0u, 14u, 13u },
    { 15u, 0u, 14u, 13u },
    { 30u, 0u, 14u, 13u },
    { 45u, 0u, 14u, 13u },
    { 60u, 0u, 14u, 13u },
    { 75u, 0u, 14u, 13u },
    { 90u, 0u, 14u, 13u },
    { 105u, 0u, 14u, 13u },
    { 0u, 14u, 14u, 13u },
    { 15u, 14u, 14u, 13u },
    { 30u, 14u, 14u, 13u },
    { 45u, 14u, 14u, 13u },
    { 60u, 14u, 14u, 13u },
    { 75u, 14u, 14u, 13u },
    { 90u, 14u, 14u, 13u },
    { 105u, 14u, 14u, 13u },
    { 0u, 28u, 14u, 13u },
    { 15u, 28u, 14u, 13u },
    { 30u, 28u, 14u, 13u },
    { 45u, 28u, 14u, 13u },
    { 60u, 28u, 14u, 13u },
    { 75u, 28u, 14u, 13u },
    { 90u, 28u, 14u, 13u },
    { 105u, 28u, 14u, 13u },
    { 0u, 42u, 14u, 13u },
    { 15u, 42u, 14u, 13u },
    { 45u, 70u, 14u, 13u },
    { 90u, 70u, 14u, 13u },
    { 60u, 70u, 14u, 13u },
    { 0u, 0u, 14u, 0u },
    { 0u, 0u, 14u, 0u },
    { 105u, 70u, 7u, 4u },
    { 0u, 0u, 14u, 13u },
    { 15u, 0u, 14u, 13u },
    { 30u, 0u, 14u, 13u },
    { 45u, 0u, 14u, 13u },
    { 60u, 0u, 14u, 13u },
    { 75u, 0u, 14u, 13u },
    { 90u, 0u, 14u, 13u },
    { 105u, 0u, 14u, 13u },
    { 0u, 14u, 14u, 13u },
    { 15u, 14u, 14u, 13u },
    { 30u, 14u, 14u, 13u },
    { 45u, 14u, 14u, 13u },
    { 60u, 14u, 14u, 13u },
    { 75u, 14u, 14u, 13u },
    { 90u, 14u, 14u, 13u },
    { 105u, 14u, 14u, 13u },
    { 0u, 28u, 14u, 13u },
    { 15u, 28u, 14u, 13u },
    { 30u, 28u, 14u, 13u },
    { 45u, 28u, 14u, 13u },
    { 60u, 28u, 14u, 13u },
    { 75u, 28u, 14u, 13u },
    { 90u, 28u, 14u, 13u },
    { 105u, 28u, 14u, 13u },
    { 0u, 42u, 14u, 13u },
    { 15u, 42u, 14u, 13u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 0u, 9u, 0u },
    { 0u, 98u, 20u, 15u },
    { 20u, 98u, 20u, 15u },
    { 40u, 98u, 20u, 15u },
    { 60u, 98u, 20u, 15u },
    { 80u, 98u, 20u, 15u },
    { 0u, 115u, 20u, 15u },
    { 20u, 115u, 20u, 15u },
    { 40u, 115u, 20u, 15u },
    { 0u, 132u, 20u, 15u },
    { 20u, 132u, 20u, 15u },
    { 40u, 132u, 20u, 15u },
    { 60u, 132u, 20u, 15u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
});