#include "stdafx_ao.h"
#include "Font.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "VRam.hpp"
#include "FixedPoint.hpp"
#include "Sys_common.hpp"
#include "Primitives.hpp"
#include "PsxDisplay.hpp"
#include "FunctionFwd.hpp"
#include "ScreenManager.hpp"
#include "Math.hpp"
#include "../AliveLibAE/Renderer/IRenderer.hpp"

namespace AO {

ALIVE_VAR(1, 0x4FFD68, FontContext, sFontContext_4FFD68, {});

ALIVE_VAR(1, 0x5080E4, s16, sDisableFontFlicker_5080E4, 0);
ALIVE_VAR(1, 0x508BF4, u8, sFontDrawScreenSpace_508BF4, 0);

const Font_AtlasEntry sFont1Atlas_4C56E8[116] = {
    {0, 0, 2, 0},
    {0, 0, 9, 0},
    {43, 80, 6, 23},
    {182, 0, 11, 10},
    {182, 0, 11, 10},
    {182, 0, 11, 10},
    {182, 0, 11, 10},
    {182, 0, 11, 10},
    {85, 96, 7, 8},
    {52, 79, 11, 23},
    {64, 79, 11, 23},
    {186, 0, 11, 10},
    {0, 79, 23, 17},
    {86, 79, 7, 23},
    {75, 79, 10, 11},
    {94, 79, 6, 23},
    {149, 79, 17, 23},
    {193, 51, 17, 23},
    {32, 51, 17, 23},
    {50, 51, 17, 23},
    {69, 51, 17, 23},
    {87, 51, 17, 23},
    {106, 51, 17, 23},
    {124, 51, 17, 23},
    {142, 51, 17, 23},
    {158, 51, 17, 23},
    {176, 51, 17, 23},
    {110, 79, 7, 22},
    {102, 79, 7, 22},
    {61, 79, 10, 23},
    {25, 79, 15, 16},
    {73, 79, 10, 23},
    {118, 79, 14, 23},
    {182, 0, 11, 10},
    {9, 0, 17, 23},
    {26, 0, 15, 23},
    {41, 0, 15, 23},
    {56, 0, 16, 23},
    {73, 0, 12, 23},
    {86, 0, 12, 23},
    {98, 0, 15, 23},
    {114, 0, 16, 23},
    {132, 0, 7, 23},
    {139, 0, 14, 23},
    {154, 0, 15, 23},
    {170, 0, 12, 23},
    {0, 25, 21, 23},
    {21, 25, 16, 23},
    {37, 25, 18, 23},
    {56, 25, 14, 23},
    {71, 25, 18, 23},
    {90, 25, 14, 23},
    {106, 25, 14, 23},
    {121, 25, 15, 23},
    {137, 25, 16, 23},
    {153, 25, 17, 23},
    {170, 25, 20, 23},
    {191, 25, 15, 23},
    {0, 50, 17, 23},
    {17, 50, 13, 23},
    {61, 79, 10, 23},
    {131, 79, 17, 23},
    {73, 79, 10, 23},
    {186, 0, 11, 10},
    {86, 79, 27, 10},
    {186, 0, 7, 11},
    {0, 110, 16, 22},
    {17, 110, 13, 22},
    {31, 110, 13, 22},
    {44, 110, 15, 22},
    {59, 110, 11, 22},
    {71, 110, 11, 22},
    {83, 110, 13, 22},
    {97, 110, 14, 22},
    {113, 110, 7, 22},
    {120, 110, 11, 22},
    {134, 110, 13, 22},
    {149, 110, 11, 22},
    {0, 137, 17, 22},
    {19, 137, 15, 22},
    {34, 137, 17, 22},
    {52, 137, 13, 22},
    {66, 137, 17, 22},
    {84, 137, 13, 22},
    {97, 137, 12, 22},
    {111, 137, 14, 22},
    {125, 137, 14, 22},
    {141, 137, 15, 22},
    {157, 137, 17, 22},
    {1, 164, 13, 22},
    {16, 164, 15, 22},
    {32, 164, 12, 22},
    {0, 186, 36, 22},
    {39, 186, 36, 22},
    {78, 186, 36, 22},
    {117, 186, 36, 22},
    {156, 186, 36, 22},
    {0, 210, 36, 22},
    {39, 210, 36, 22},
    {78, 210, 36, 22},
    {189, 124, 30, 19},
    {221, 124, 30, 20},
    {224, 143, 25, 21},
    {193, 143, 25, 21},
    {162, 110, 19, 22},
    {211, 0, 44, 26},
    {196, 164, 16, 22},
    {48, 164, 16, 22},
    {64, 164, 16, 22},
    {81, 164, 16, 22},
    {98, 164, 16, 22},
    {115, 164, 16, 22},
    {131, 164, 16, 22},
    {148, 164, 16, 22},
    {163, 164, 16, 22},
    {180, 164, 16, 22}};

const Font_AtlasEntry sFont2Atlas_4C58B8[104] = {
    {0, 0, 2, 0},
    {0, 0, 14, 0},
    {105, 42, 14, 13},
    {105, 70, 14, 4},
    {0, 0, 14, 0},
    {75, 70, 14, 13},
    {0, 0, 14, 0},
    {0, 0, 14, 0},
    {105, 70, 8, 5},
    {45, 70, 14, 13},
    {60, 70, 14, 13},
    {105, 84, 14, 0},
    {75, 84, 14, 13},
    {45, 42, 14, 13},
    {60, 84, 14, 13},
    {30, 42, 14, 13},
    {0, 0, 14, 0},
    {0, 56, 14, 13},
    {15, 56, 14, 13},
    {30, 56, 14, 13},
    {45, 56, 14, 13},
    {60, 56, 14, 13},
    {75, 56, 14, 13},
    {90, 56, 14, 13},
    {105, 56, 14, 13},
    {0, 70, 14, 13},
    {15, 70, 14, 13},
    {60, 42, 14, 13},
    {75, 42, 14, 13},
    {45, 70, 14, 13},
    {30, 70, 14, 13},
    {60, 70, 14, 13},
    {90, 42, 14, 13},
    {0, 0, 14, 0},
    {0, 0, 14, 13},
    {15, 0, 14, 13},
    {30, 0, 14, 13},
    {45, 0, 14, 13},
    {60, 0, 14, 13},
    {75, 0, 14, 13},
    {90, 0, 14, 13},
    {105, 0, 14, 13},
    {0, 14, 14, 13},
    {15, 14, 14, 13},
    {30, 14, 14, 13},
    {45, 14, 14, 13},
    {60, 14, 14, 13},
    {75, 14, 14, 13},
    {90, 14, 14, 13},
    {105, 14, 14, 13},
    {0, 28, 14, 13},
    {15, 28, 14, 13},
    {30, 28, 14, 13},
    {45, 28, 14, 13},
    {60, 28, 14, 13},
    {75, 28, 14, 13},
    {90, 28, 14, 13},
    {105, 28, 14, 13},
    {0, 42, 14, 13},
    {15, 42, 14, 13},
    {45, 70, 14, 13},
    {90, 70, 14, 13},
    {60, 70, 14, 13},
    {0, 0, 14, 0},
    {0, 0, 14, 0},
    {105, 70, 7, 4},
    {0, 0, 14, 13},
    {15, 0, 14, 13},
    {30, 0, 14, 13},
    {45, 0, 14, 13},
    {60, 0, 14, 13},
    {75, 0, 14, 13},
    {90, 0, 14, 13},
    {105, 0, 14, 13},
    {0, 14, 14, 13},
    {15, 14, 14, 13},
    {30, 14, 14, 13},
    {45, 14, 14, 13},
    {60, 14, 14, 13},
    {75, 14, 14, 13},
    {90, 14, 14, 13},
    {105, 14, 14, 13},
    {0, 28, 14, 13},
    {15, 28, 14, 13},
    {30, 28, 14, 13},
    {45, 28, 14, 13},
    {60, 28, 14, 13},
    {75, 28, 14, 13},
    {90, 28, 14, 13},
    {105, 28, 14, 13},
    {0, 42, 14, 13},
    {15, 42, 14, 13},
    {0, 98, 20, 15},
    {20, 98, 20, 15},
    {40, 98, 20, 15},
    {60, 98, 20, 15},
    {80, 98, 20, 15},
    {0, 115, 20, 15},
    {20, 115, 20, 15},
    {40, 115, 20, 15},
    {0, 132, 20, 15},
    {20, 132, 20, 15},
    {40, 132, 20, 15},
    {60, 132, 20, 15}};


void CC FontContext::static_ctor_41C010()
{
    atexit(static_dtor_41C020);
}

void CC FontContext::static_dtor_41C020()
{
    sFontContext_4FFD68.dtor_41C110();
}

void FontContext::LoadFontType_41C040(s16 resourceID)
{
    field_C_resource_id = resourceID;
    auto loadedResource = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Font, resourceID, 1, 0);
    auto fontFile = reinterpret_cast<File_Font*>(*loadedResource);

    vram_alloc_450B20(fontFile->field_0_width, fontFile->field_2_height, fontFile->field_4_color_depth, &field_0_rect);

    const PSX_RECT rect = {field_0_rect.x, field_0_rect.y, static_cast<s16>(fontFile->field_0_width / 4), fontFile->field_2_height};

    IRenderer::GetRenderer()->Upload(fontFile->field_4_color_depth == 16 ? IRenderer::BitDepth::e16Bit : IRenderer::BitDepth::e4Bit, rect, fontFile->field_28_pixel_buffer);

    // Free our loaded font resource as its now in vram
    ResourceManager::FreeResource_455550(loadedResource);

    switch (resourceID)
    {
        case 1:
            field_8_atlas_array = sFont1Atlas_4C56E8;
            break;
        case 2:
            field_8_atlas_array = sFont2Atlas_4C58B8;
            break;
        default:
            ALIVE_FATAL("Unknown font resource ID !!!");
            break;
    }
}

void FontContext::dtor_41C110()
{
    if (field_0_rect.x > 0)
    {
        Vram_free_450CE0(
            {field_0_rect.x, field_0_rect.y},
            {field_0_rect.w, field_0_rect.h});
    }
}

AliveFont* AliveFont::ctor_41C170(s32 maxCharLength, const u8* palette, FontContext* fontContext)
{
    field_34_font_context = fontContext;

    IRenderer::PalRecord rec;
    rec.depth = 16;
    if (!IRenderer::GetRenderer()->PalAlloc(rec))
    {
        LOG_ERROR("PalAlloc failed");
    }

    IRenderer::GetRenderer()->PalSetData(rec, palette);

    field_28_palette_rect.x = rec.x;
    field_28_palette_rect.y = rec.y;
    field_28_palette_rect.w = rec.depth;
    field_28_palette_rect.h = 1;

    field_30_poly_count = maxCharLength;
    field_20_fnt_poly_block_ptr = ResourceManager::Allocate_New_Locked_Resource_454F80(
        ResourceManager::Resource_FntP,
        fontContext->field_C_resource_id,
        sizeof(Poly_FT4) * 2 * maxCharLength);
    field_24_fnt_poly_array = reinterpret_cast<Poly_FT4*>(*field_20_fnt_poly_block_ptr);
    return this;
}

EXPORT u32 AliveFont::MeasureWidth_41C2B0(const s8* text)
{
    s32 result = 0;

    for (u32 i = 0; i < strlen(text); i++)
    {
        const s8 c = text[i];
        s32 charIndex = 0;

        if (c <= 32 || static_cast<u8>(c) > 175)
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

    if (!sFontDrawScreenSpace_508BF4)
    {
        result -= field_34_font_context->field_8_atlas_array[0].field_2_width;
        result = PCToPsxX(result, 20);
    }

    return result;
}

EXPORT s32 AliveFont::MeasureWidth_41C200(s8 character)
{
    s32 result = 0;
    s32 charIndex = 0;

    if (character <= 32 || character > 175)
    {
        if (character < 8 || character > 31)
        {
            return field_34_font_context->field_8_atlas_array[1].field_2_width;
        }
        charIndex = character + 84;
    }
    else
    {
        charIndex = character - 31;
    }
    result = field_34_font_context->field_8_atlas_array[charIndex].field_2_width;

    if (!sFontDrawScreenSpace_508BF4)
    {
        result = static_cast<s32>(result * 0.575);
    }

    return result;
}


s32 AliveFont::MeasureWidth_41C280(const s8* text, FP scale)
{
    const FP width = FP_FromInteger(MeasureWidth_41C2B0(text));
    return FP_GetExponent((width * scale) + FP_FromDouble(0.5));
}

EXPORT s32 AliveFont::DrawString_41C360(PrimHeader** ppOt, const s8* text, s16 x, s16 y, TPageAbr abr, s32 bSemiTrans, s32 blendMode, Layer layer, u8 r, u8 g, u8 b, s32 polyOffset, FP scale, s32 maxRenderWidth, s32 colorRandomRange)
{
    if (!sFontDrawScreenSpace_508BF4)
    {
        x = PsxToPCX(x, 11);
    }

    s32 characterRenderCount = 0;
    const s32 maxRenderX = PsxToPCX(maxRenderWidth, 11);
    s16 offsetX = x;
    s32 charInfoIndex = 0;
    auto poly = &field_24_fnt_poly_array[gPsxDisplay_504C78.field_A_buffer_index + (2 * polyOffset)];

    const s32 tpage = PSX_getTPage_4965D0(TPageMode::e4Bit_0, abr, field_34_font_context->field_0_rect.x & ~63, field_34_font_context->field_0_rect.y);
    const s32 clut = PSX_getClut_496840(field_28_palette_rect.x, field_28_palette_rect.y);

    for (u32 i = 0; i < strlen(text); i++)
    {
        if (offsetX >= maxRenderX)
        {
            break;
        }

        const u8 c = text[i];

        if (c <= 32 || c > 175)
        {
            if (c < 8 || c > 31)
            {
                offsetX += field_34_font_context->field_8_atlas_array[0].field_2_width + field_34_font_context->field_8_atlas_array[1].field_2_width;
                continue;
            }
            charInfoIndex = c + 84;
        }
        else
        {
            charInfoIndex = c - 31;
        }

        const auto fContext = field_34_font_context;
        const auto atlasEntry = &fContext->field_8_atlas_array[charInfoIndex];

        const s8 charWidth = atlasEntry->field_2_width;
        const auto charHeight = atlasEntry->field_3_height;
        const s8 texture_u = static_cast<s8>(atlasEntry->field_0_x + (4 * (fContext->field_0_rect.x & 0x3F)));
        const s8 texture_v = static_cast<s8>(atlasEntry->field_1_y + LOBYTE(fContext->field_0_rect.y));

        const s16 widthScaled = static_cast<s16>(charWidth * FP_GetDouble(scale));
        const s16 heightScaled = static_cast<s16>(charHeight * FP_GetDouble(scale));

        PolyFT4_Init(poly);

        SetPrimExtraPointerHack(poly, nullptr);

        Poly_Set_SemiTrans_498A40(&poly->mBase.header, bSemiTrans);
        Poly_Set_Blending_498A00(&poly->mBase.header, blendMode);

        SetRGB0(
            poly,
            static_cast<u8>(r + Math_RandomRange_450F20(static_cast<s16>(-colorRandomRange), static_cast<s16>(colorRandomRange))),
            static_cast<u8>(g + Math_RandomRange_450F20(static_cast<s16>(-colorRandomRange), static_cast<s16>(colorRandomRange))),
            static_cast<u8>(b + Math_RandomRange_450F20(static_cast<s16>(-colorRandomRange), static_cast<s16>(colorRandomRange))));

        SetTPage(poly, static_cast<s16>(tpage));
        SetClut(poly, static_cast<s16>(clut));

        // Padding
        poly->mVerts[1].mUv.tpage_clut_pad = 0;
        poly->mVerts[2].mUv.tpage_clut_pad = 0;

        // P0
        SetXY0(poly, offsetX, y);
        SetUV0(poly, texture_u, texture_v);

        // P1
        SetXY1(poly, offsetX + widthScaled, y);
        SetUV1(poly, texture_u + charWidth, texture_v);

        // P2
        SetXY2(poly, offsetX, y + heightScaled);
        SetUV2(poly, texture_u, texture_v + charHeight);

        // P3
        SetXY3(poly, offsetX + widthScaled, y + heightScaled);
        SetUV3(poly, texture_u + charWidth, texture_v + charHeight);

        OrderingTable_Add_498A80(OtLayer(ppOt, layer), &poly->mBase.header);

        ++characterRenderCount;

        offsetX += widthScaled + FP_GetExponent(FP_FromInteger(field_34_font_context->field_8_atlas_array[0].field_2_width) * scale);

        poly += 2;
    }

    pScreenManager_4FF7C8->InvalidateRect_406E40(x, y - 1, offsetX, y + 24, pScreenManager_4FF7C8->field_2E_idx);

    return polyOffset + characterRenderCount;
}

void AliveFont::dtor_41C130()
{
    IRenderer::GetRenderer()->PalFree(IRenderer::PalRecord{field_28_palette_rect.x, field_28_palette_rect.y, field_28_palette_rect.w});
    field_28_palette_rect.x = 0;
    ResourceManager::FreeResource_455550(field_20_fnt_poly_block_ptr);
}

const s8* AliveFont::SliceText_41C6C0(const s8* text, s32 left, FP scale, s32 right)
{
    s32 xOff = 0;
    s32 rightWorldSpace = PsxToPCX(right, 11);

    if (sFontDrawScreenSpace_508BF4)
    {
        xOff = left;
    }
    else
    {
        xOff = PsxToPCX(left, 11);
    }

    for (const s8* strPtr = text; *strPtr; strPtr++)
    {
        s32 atlasIdx = 0;
        s8 character = *strPtr;
        if (xOff >= rightWorldSpace)
            break;

        if (character <= 32 || character > 122)
        {
            atlasIdx = character < 8 || character > 31 ? 1 : character + 84;
        }
        else
        {
            atlasIdx = character - 31;
        }

        xOff += static_cast<s32>(field_34_font_context->field_8_atlas_array[atlasIdx].field_2_width * FP_GetDouble(scale)) + field_34_font_context->field_8_atlas_array->field_2_width;
        text = strPtr;
    }

    return text;
}

} // namespace AO
