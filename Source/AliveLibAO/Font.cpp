#include "stdafx_ao.h"
#include "Font.hpp"
#include "Function.hpp"
#include "../AliveLibCommon/Primitives_common.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Math.hpp"
#include "../AliveLibCommon/FatalError.hpp"

namespace AO {

s16 sDisableFontFlicker_5080E4 = 0;
u8 sFontDrawScreenSpace_508BF4 = 0;

static const Font_AtlasEntry sPauseMenuFontAtlas[116] = {
    {0, 0, 2, 0},
    {0, 0, 9, 0},
    {43, 80, 6, 23, "!"},
    {182, 0, 11, 10},
    {182, 0, 11, 10},
    {182, 0, 11, 10},
    {182, 0, 11, 10},
    {182, 0, 11, 10},
    {85, 96, 7, 8, ","},
    {52, 79, 11, 23, "("},
    {64, 79, 11, 23, ")"},
    {186, 0, 11, 10},
    {0, 79, 23, 17, "+"},
    {86, 79, 7, 23, ","},
    {75, 79, 10, 11, "_"}, // or minus?
    {94, 79, 6, 23, "."},
    {149, 79, 17, 23, "/"},
    {193, 51, 17, 23, "0"},
    {32, 51, 17, 23, "1"},
    {50, 51, 17, 23, "2"},
    {69, 51, 17, 23, "3"},
    {87, 51, 17, 23, "4"},
    {106, 51, 17, 23, "5"},
    {124, 51, 17, 23, "6"},
    {142, 51, 17, 23, "7"},
    {158, 51, 17, 23, "8"},
    {176, 51, 17, 23, "9"},
    {110, 79, 7, 22, ":"},
    {102, 79, 7, 22, ";"},
    {61, 79, 10, 23, ")"},
    {25, 79, 15, 16, "="},
    {73, 79, 10, 23, "-"},
    {118, 79, 14, 23, "?"},
    {182, 0, 11, 10},
    {9, 0, 17, 23, "Big_A"},
    {26, 0, 15, 23, "Big_B"},
    {41, 0, 15, 23, "Big_C"},
    {56, 0, 16, 23, "Big_D"},
    {73, 0, 12, 23, "Big_E"},
    {86, 0, 12, 23, "Big_F"},
    {98, 0, 15, 23, "Big_G"},
    {114, 0, 16, 23, "Big_H"},
    {132, 0, 7, 23, "Big_I"},
    {139, 0, 14, 23, "Big_J"},
    {154, 0, 15, 23, "Big_K"},
    {170, 0, 12, 23, "Big_L"},
    {0, 25, 21, 23, "Big_M"},
    {21, 25, 16, 23, "Big_N"},
    {37, 25, 18, 23, "Big_O"},
    {56, 25, 14, 23, "Big_P"},
    {71, 25, 18, 23, "Big_Q"},
    {90, 25, 14, 23, "Big_R"},
    {106, 25, 14, 23, "Big_S"},
    {121, 25, 15, 23, "Big_T"},
    {137, 25, 16, 23, "Big_U"},
    {153, 25, 17, 23, "Big_V"},
    {170, 25, 20, 23, "Big_W"},
    {191, 25, 15, 23, "Big_X"},
    {0, 50, 17, 23, "Big_Y"},
    {17, 50, 13, 23, "Big_Z"},
    {61, 79, 10, 23, ")"},
    {131, 79, 17, 23, "\\"},
    {73, 79, 10, 23, "-"},
    {186, 0, 11, 10},
    {86, 79, 27, 10},
    {186, 0, 7, 11},
    {0, 110, 16, 22, "Medium_A"},
    {17, 110, 13, 22, "Medium_B"},
    {31, 110, 13, 22, "Medium_C"},
    {44, 110, 15, 22, "Medium_D"},
    {59, 110, 11, 22, "Medium_E"},
    {71, 110, 11, 22, "Medium_F"},
    {83, 110, 13, 22, "Medium_G"},
    {97, 110, 14, 22, "Medium_H"},
    {113, 110, 7, 22, "Medium_I"},
    {120, 110, 11, 22, "Medium_J"},
    {134, 110, 13, 22, "Medium_K"},
    {149, 110, 11, 22, "Medium_L"},
    {0, 137, 17, 22, "Medium_M"},
    {19, 137, 15, 22, "Medium_N"},
    {34, 137, 17, 22, "Medium_O"},
    {52, 137, 13, 22, "Medium_P"},
    {66, 137, 17, 22, "Medium_Q"},
    {84, 137, 13, 22, "Medium_R"},
    {97, 137, 12, 22, "Medium_S"},
    {111, 137, 14, 22, "Medium_T"},
    {125, 137, 14, 22, "Medium_U"},
    {141, 137, 15, 22, "Medium_V"},
    {157, 137, 17, 22, "Medium_W"},
    {1, 164, 13, 22, "Medium_X"},
    {16, 164, 15, 22, "Medium_Y"},
    {32, 164, 12, 22, "Medium_Z"},
    {0, 186, 36, 22, "Button_A"},
    {39, 186, 36, 22, "Button_B"},
    {78, 186, 36, 22, "Button_C"},
    {117, 186, 36, 22, "Button_D"},
    {156, 186, 36, 22, "Button_E"},
    {0, 210, 36, 22, "Button_F"},
    {39, 210, 36, 22, "Button_G"},
    {78, 210, 36, 22, "Button_H"},
    {189, 124, 30, 19, "Dpad_Up"},
    {221, 124, 30, 20, "Dpad_Down"},
    {224, 143, 25, 21, "Dpad_Left"},
    {193, 143, 25, 21, "Dpad_Right"},
    {162, 110, 19, 22, "Or"},
    {211, 0, 44, 26, "Dpad"},
    {196, 164, 16, 22, "0"},
    {48, 164, 16, 22, "1"},
    {64, 164, 16, 22, "2"},
    {81, 164, 16, 22, "3"},
    {98, 164, 16, 22, "4"},
    {115, 164, 16, 22, "5"},
    {131, 164, 16, 22, "6"},
    {148, 164, 16, 22, "7"},
    {163, 164, 16, 22, "8"},
    {180, 164, 16, 22, "9"}};

static const Font_AtlasEntry sLcdFontAtlas[104] = {
    {0, 0, 2, 0},
    {0, 0, 14, 0},
    {105, 42, 14, 13, "!"},
    {105, 70, 14, 4, "Double_Prime"},
    {0, 0, 14, 0},
    {75, 70, 14, 13, "$"},
    {0, 0, 14, 0},
    {0, 0, 14, 0},
    {105, 70, 8, 5, "Prime"},
    {45, 70, 14, 13, "("},
    {60, 70, 14, 13, ")"},
    {105, 84, 14, 0},
    {75, 84, 14, 13, "+"},
    {45, 42, 14, 13, ","},
    {60, 84, 14, 13, "-"},
    {30, 42, 14, 13, "."},
    {0, 0, 14, 0},
    {0, 56, 14, 13, "0"},
    {15, 56, 14, 13, "1"},
    {30, 56, 14, 13, "2"},
    {45, 56, 14, 13, "3"},
    {60, 56, 14, 13, "4"},
    {75, 56, 14, 13, "5"},
    {90, 56, 14, 13, "6"},
    {105, 56, 14, 13, "7"},
    {0, 70, 14, 13, "8"},
    {15, 70, 14, 13, "9"},
    {60, 42, 14, 13, ":"},
    {75, 42, 14, 13, ";"},
    {45, 70, 14, 13, "("},
    {30, 70, 14, 13, "="},
    {60, 70, 14, 13, ")"},
    {90, 42, 14, 13, "?"},
    {0, 0, 14, 0},
    {0, 0, 14, 13, "A"},
    {15, 0, 14, 13, "B"},
    {30, 0, 14, 13, "C"},
    {45, 0, 14, 13, "D"},
    {60, 0, 14, 13, "E"},
    {75, 0, 14, 13, "F"},
    {90, 0, 14, 13, "G"},
    {105, 0, 14, 13, "H"},
    {0, 14, 14, 13, "I"},
    {15, 14, 14, 13, "J"},
    {30, 14, 14, 13, "K"},
    {45, 14, 14, 13, "L"},
    {60, 14, 14, 13, "M"},
    {75, 14, 14, 13, "N"},
    {90, 14, 14, 13, "O"},
    {105, 14, 14, 13, "P"},
    {0, 28, 14, 13, "Q"},
    {15, 28, 14, 13, "R"},
    {30, 28, 14, 13, "S"},
    {45, 28, 14, 13, "T"},
    {60, 28, 14, 13, "U"},
    {75, 28, 14, 13, "V"},
    {90, 28, 14, 13, "W"},
    {105, 28, 14, 13, "X"},
    {0, 42, 14, 13, "Y"},
    {15, 42, 14, 13, "Z"},
    {45, 70, 14, 13, "("},
    {90, 70, 14, 13, "\\"},
    {60, 70, 14, 13, ")"},
    {0, 0, 14, 0},
    {0, 0, 14, 0},
    {105, 70, 7, 4, "Prime"},
    {0, 0, 14, 13, "A"},
    {15, 0, 14, 13, "B"},
    {30, 0, 14, 13, "C"},
    {45, 0, 14, 13, "D"},
    {60, 0, 14, 13, "E"},
    {75, 0, 14, 13, "F"},
    {90, 0, 14, 13, "G"},
    {105, 0, 14, 13, "H"},
    {0, 14, 14, 13, "I"},
    {15, 14, 14, 13, "J"},
    {30, 14, 14, 13, "K"},
    {45, 14, 14, 13, "L"},
    {60, 14, 14, 13, "M"},
    {75, 14, 14, 13, "N"},
    {90, 14, 14, 13, "O"},
    {105, 14, 14, 13, "P"},
    {0, 28, 14, 13, "Q"},
    {15, 28, 14, 13, "R"},
    {30, 28, 14, 13, "S"},
    {45, 28, 14, 13, "T"},
    {60, 28, 14, 13, "U"},
    {75, 28, 14, 13, "V"},
    {90, 28, 14, 13, "W"},
    {105, 28, 14, 13, "X"},
    {0, 42, 14, 13, "Y"},
    {15, 42, 14, 13, "Z"},
    {0, 98, 20, 15, "Button_A"},
    {20, 98, 20, 15, "Button_B"},
    {40, 98, 20, 15, "Button_C"},
    {60, 98, 20, 15, "Button_D"},
    {80, 98, 20, 15, "Button_E"},
    {0, 115, 20, 15, "Button_F"},
    {20, 115, 20, 15, "Button_G"},
    {40, 115, 20, 15, "Button_H"},
    {0, 132, 20, 15, "Button_Up"},
    {20, 132, 20, 15, "Button_Down"},
    {40, 132, 20, 15, "Button_Left"},
    {60, 132, 20, 15, "Button_Right"}};


void AliveFont::Load(s32 maxCharLength, const PalResource& pal, FontContext* fontContext)
{
    // TODO: Use pal
    field_34_font_context = fontContext;
    field_34_font_context->field_C_resource_id.mCurPal = pal.mPal;
    field_30_poly_count = maxCharLength;
    field_24_fnt_poly_array = relive_new Poly_FT4[maxCharLength * 2];
}

AliveFont::~AliveFont()
{
    relive_delete[] field_24_fnt_poly_array;
}

s32 AliveFont::DrawString(PrimHeader** ppOt, const char_type* text, s16 x, s16 y, TPageAbr abr, s32 bSemiTrans, s32 blendMode, Layer layer, u8 r, u8 g, u8 b, s32 polyOffset, FP scale, s32 maxRenderWidth, s32 colorRandomRange)
{
    if (!sFontDrawScreenSpace_508BF4)
    {
        x = PsxToPCX(x, 11);
    }

    s32 characterRenderCount = 0;
    const s32 maxRenderX = PsxToPCX(maxRenderWidth, 11);
    s16 offsetX = x;
    s32 charInfoIndex = 0;
    auto poly = &field_24_fnt_poly_array[gPsxDisplay.mBufferIndex + (2 * polyOffset)];

    //const s32 tpage = PSX_getTPage(TPageMode::e4Bit_0, abr, field_34_font_context->mRect.x & ~63, field_34_font_context->mRect.y);
   // const s32 clut = PSX_getClut(field_28_palette_rect.x, field_28_palette_rect.y);

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

        // TODO: Recalc when atlas is converted
        const s8 texture_u = static_cast<s8>(atlasEntry->x);
        const s8 texture_v = static_cast<s8>(atlasEntry->field_1_y);

        const s16 widthScaled = static_cast<s16>(charWidth * FP_GetDouble(scale));
        const s16 heightScaled = static_cast<s16>(charHeight * FP_GetDouble(scale));

        PolyFT4_Init(poly);

        SetPrimExtraPointerHack(poly, nullptr);

        Poly_Set_SemiTrans(&poly->mBase.header, bSemiTrans);
        Poly_Set_Blending(&poly->mBase.header, blendMode);

        SetRGB0(
            poly,
            static_cast<u8>(r + Math_RandomRange(static_cast<s16>(-colorRandomRange), static_cast<s16>(colorRandomRange))),
            static_cast<u8>(g + Math_RandomRange(static_cast<s16>(-colorRandomRange), static_cast<s16>(colorRandomRange))),
            static_cast<u8>(b + Math_RandomRange(static_cast<s16>(-colorRandomRange), static_cast<s16>(colorRandomRange))));

       // SetTPage(poly, static_cast<s16>(tpage));
        //SetClut(poly, static_cast<s16>(clut));

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

        // TPage blend mode start
        u16 tpageEmptyBlend = GetTPage(poly) & 0xFFCF;
        u16 blendModeBit = ((u16) abr) << 4;
        SetTPage(poly, tpageEmptyBlend | blendModeBit);
        
        // Its over, for now
        //poly->mFont = field_34_font_context;
        // TPage blend mode start

        OrderingTable_Add(OtLayer(ppOt, layer), &poly->mBase.header);

        ++characterRenderCount;

        offsetX += widthScaled + FP_GetExponent(FP_FromInteger(field_34_font_context->field_8_atlas_array[0].field_2_width) * scale);

        poly += 2;
    }

    return polyOffset + characterRenderCount;
}

u32 AliveFont::MeasureTextWidth(const char_type* text)
{
    s32 result = 0;

    for (u32 i = 0; i < strlen(text); i++)
    {
        const char_type c = text[i];
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

// Measures the width of a string with scale applied.
s32 AliveFont::MeasureScaledTextWidth(const char_type* text, FP scale)
{
    const FP width = FP_FromInteger(MeasureTextWidth(text));
    return FP_GetExponent((width * scale) + FP_FromDouble(0.5));
}

// Measures the width of a single character.
s32 AliveFont::MeasureCharacterWidth(char_type character)
{
    s32 result = 0;
    s32 charIndex = 0;

    if (character <= 32 /*|| character > 175*/) // character > 175 always false
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

// Wasn't too sure what to call this. Returns the s8 offset of where the text is cut off. (left and right region)
const char_type* AliveFont::SliceText(const char_type* text, s32 left, FP scale, s32 right)
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

    for (const char_type* strPtr = text; *strPtr; strPtr++)
    {
        s32 atlasIdx = 0;
        char_type character = *strPtr;
        if (xOff >= rightWorldSpace)
        {
            break;
        }

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

void FontContext::LoadFontType(FontType resourceID)
{
    FontResource fontRes = ResourceManagerWrapper::LoadFont(resourceID);
    field_C_resource_id = fontRes;

    // TODO: Will get moved to a json file in FontResource
    switch (resourceID)
    {
        case FontType::PauseMenu:
            field_8_atlas_array = sPauseMenuFontAtlas;
            break;
        case FontType::LcdFont:
            field_8_atlas_array = sLcdFontAtlas;
            break;
        default:
            ALIVE_FATAL("Unknown font resource ID !!!");
            break;
    }
}



} // namespace AO
