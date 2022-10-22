#include "stdafx.h"
#include "Font.hpp"
#include "Function.hpp"
#include "FixedPoint.hpp"
#include "../AliveLibCommon/FatalError.hpp"

/*
 Notes for when coming across crazy width conversions:
 If it looks like this:

 (40 * x + 11) / 23

 Then its probably x *= 0.575;
 If it looks like:

 v17 = (s32)(40 * x + 11 + ((s64)(-1307163959i64 * (40 * x + 11)) >> 32)) >> 4;
 x = (v17 >> 31) + v17;

 then do x /= 0.575;
*/

s8 sDisableFontFlicker = 0;

u8 sFontDrawScreenSpace = 0;

// OG bug: many entries seem to be misaligned
Font_AtlasEntry sPauseMenuFontAtlas[169] =
          {{0u, 0u, 2u, 0u},
           {0u, 0u, 9u, 0u},
           {43u, 80u, 6u, 23u, "!"},
           {182u, 0u, 11u, 10u},
           {182u, 0u, 11u, 10u},
           {182u, 0u, 11u, 10u},
           {182u, 0u, 11u, 10u},
           {182u, 0u, 11u, 10u},
           {85u, 96u, 7u, 8u, ","},
           {52u, 79u, 11u, 23u, "("},
           {64u, 79u, 11u, 23u, ")"},
           {186u, 0u, 11u, 10u},
           {0u, 79u, 23u, 17u, "+"},
           {86u, 79u, 7u, 23u},
           {75u, 79u, 10u, 11u},
           {94u, 79u, 6u, 23u},
           {0u, 0u, 9u, 0u},
           {193u, 51u, 17u, 23u, "0"},
           {32u, 51u, 17u, 23u, "1"},
           {50u, 51u, 17u, 23u, "2"},
           {69u, 51u, 17u, 23u, "3"},
           {87u, 51u, 17u, 23u, "4"},
           {106u, 51u, 17u, 23u, "5"},
           {124u, 51u, 17u, 23u, "6"},
           {142u, 51u, 17u, 23u, "7"},
           {158u, 51u, 17u, 23u, "8"},
           {176u, 51u, 17u, 23u, "9"},
           {110u, 79u, 7u, 22u, ":"},
           {102u, 79u, 7u, 22u, ";"},
           {61u, 79u, 10u, 23u, ")"},
           {25u, 79u, 15u, 16u, "="},
           {73u, 79u, 10u, 23u, "-"},
           {118u, 79u, 14u, 23u, "?"},
           {182u, 0u, 11u, 10u},
           {9u, 0u, 17u, 23u, "Big_A"},
           {26u, 0u, 15u, 23u, "Big_B"},
           {41u, 0u, 15u, 23u, "Big_C"},
           {56u, 0u, 16u, 23u, "Big_D"},
           {73u, 0u, 12u, 23u, "Big_E"},
           {86u, 0u, 12u, 23u, "Big_F"},
           {98u, 0u, 15u, 23u, "Big_G"},
           {114u, 0u, 16u, 23u, "Big_H"},
           {132u, 0u, 7u, 23u, "Big_I"},
           {139u, 0u, 14u, 23u, "Big_J"},
           {154u, 0u, 15u, 23u, "Big_K"},
           {170u, 0u, 12u, 23u, "Big_L"},
           {0u, 25u, 21u, 23u, "Big_M"},
           {21u, 25u, 16u, 23u, "Big_N"},
           {37u, 25u, 18u, 23u, "Big_O"},
           {56u, 25u, 14u, 23u, "Big_P"},
           {71u, 25u, 18u, 23u, "Big_Q"},
           {90u, 25u, 14u, 23u, "Big_R"},
           {106u, 25u, 14u, 23u, "Big_S"},
           {121u, 25u, 15u, 23u, "Big_T"},
           {137u, 25u, 16u, 23u, "Big_U"},
           {153u, 25u, 17u, 23u, "Big_V"},
           {170u, 25u, 20u, 23u, "Big_W"},
           {191u, 25u, 15u, 23u, "Big_X"},
           {0u, 50u, 17u, 23u, "Big_Y"},
           {17u, 50u, 13u, 23u, "Big_Z"},
           {61u, 79u, 10u, 23u, ")"},
           {0u, 0u, 9u, 0u},
           {73u, 79u, 10u, 23u, "-"},
           {186u, 0u, 11u, 10u},
           {86u, 79u, 27u, 10u},
           {186u, 0u, 7u, 11u},
           {0u, 108u, 16u, 24u, "Medium_A"},
           {17u, 108u, 13u, 24u, "Medium_B"},
           {31u, 108u, 13u, 24u, "Medium_C"},
           {44u, 108u, 15u, 24u, "Medium_D"},
           {59u, 108u, 11u, 24u, "Medium_E"},
           {71u, 108u, 11u, 24u, "Medium_F"},
           {83u, 108u, 13u, 24u, "Medium_G"},
           {97u, 108u, 14u, 24u, "Medium_H"},
           {113u, 108u, 7u, 24u, "Medium_I"},
           {120u, 108u, 11u, 24u, "Medium_J"},
           {134u, 108u, 13u, 24u, "Medium_K"},
           {149u, 108u, 11u, 24u, "Medium_L"},
           {0u, 135u, 17u, 24u, "Medium_M"},
           {19u, 135u, 15u, 24u, "Medium_N"},
           {34u, 135u, 17u, 24u, "Medium_O"},
           {52u, 135u, 13u, 24u, "Medium_P"},
           {66u, 135u, 17u, 24u, "Medium_Q"},
           {84u, 135u, 13u, 24u, "Medium_R"},
           {97u, 135u, 12u, 24u, "Medium_S"},
           {111u, 135u, 14u, 24u, "Medium_T"},
           {125u, 135u, 14u, 24u, "Medium_U"},
           {141u, 135u, 15u, 24u, "Medium_V"},
           {157u, 135u, 17u, 24u, "Medium_W"},
           {1u, 163u, 13u, 24u, "Medium_X"},
           {16u, 163u, 15u, 24u, "Medium_Y"},
           {32u, 163u, 12u, 24u, "Medium_Z"},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {134u, 79u, 16u, 9u},
           {0u, 0u, 9u, 0u},
           {227u, 211u, 12u, 24u},
           {239u, 187u, 14u, 24u},
           {233u, 79u, 11u, 24u, "E_Acute"},
           {201u, 79u, 14u, 24u},
           {217u, 79u, 14u, 24u},
           {187u, 79u, 14u, 24u},
           {0u, 110u, 16u, 22u},
           {227u, 211u, 12u, 24u},
           {211u, 52u, 11u, 24u},
           {222u, 52u, 11u, 24u},
           {244u, 79u, 11u, 24u},
           {213u, 28u, 10u, 24u},
           {175u, 136u, 8u, 24u},
           {240u, 51u, 7u, 24u},
           {218u, 79u, 17u, 24u},
           {0u, 110u, 16u, 22u},
           {233u, 79u, 11u, 24u},
           {223u, 234u, 19u, 20u},
           {223u, 234u, 19u, 20u},
           {214u, 162u, 17u, 24u},
           {230u, 162u, 17u, 24u},
           {239u, 27u, 16u, 24u},
           {225u, 187u, 14u, 24u},
           {210u, 187u, 14u, 24u},
           {16u, 164u, 15u, 22u},
           {230u, 162u, 17u, 24u},
           {239u, 187u, 14u, 24u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {170u, 79u, 16u, 24u, "A_Acute"},
           {234u, 51u, 7u, 24u, "I_Acute"},
           {223u, 27u, 16u, 24u, "O_Acute"},
           {197u, 187u, 14u, 24u, "U_Acute"},
           {238u, 211u, 15u, 24u, "N_Tilde"},
           {238u, 211u, 16u, 24u}, // same as above with less width
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {197u, 212u, 14u, 24u, "Inverted_Question_Mark"},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {197u, 0u, 7u, 22u, "Inverted_Exclamation_Point"},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 186u, 36u, 22u, "Button_A"},
           {39u, 186u, 36u, 22u, "Button_B"},
           {78u, 186u, 36u, 22u, "Button_C"},
           {117u, 186u, 36u, 22u, "Button_D"},
           {156u, 186u, 36u, 22u, "Button_E"},
           {0u, 210u, 36u, 22u, "Button_F"},
           {39u, 210u, 36u, 22u, "Button_G"},
           {78u, 210u, 36u, 22u, "Button_H"},
           {189u, 124u, 30u, 19u, "Dpoad_Up"},
           {221u, 124u, 30u, 20u, "Dpad_Down"},
           {224u, 143u, 25u, 21u, "Dpad_Left"},
           {193u, 143u, 25u, 21u, "Dpad_Right"},
           {162u, 110u, 19u, 18u},
           {211u, 0u, 44u, 26u, "Dpad"},
           {196u, 162u, 16u, 24u, "0"},
           {48u, 162u, 16u, 24u, "1"},
           {64u, 162u, 16u, 24u, "2"},
           {81u, 162u, 16u, 24u, "3"},
           {98u, 162u, 16u, 24u, "4"},
           {115u, 162u, 16u, 24u, "5"},
           {131u, 162u, 16u, 24u, "6"},
           {148u, 162u, 16u, 24u, "7"},
           {163u, 162u, 16u, 24u, "8"},
           {180u, 162u, 16u, 24u, "9"}};

Font_AtlasEntry sLcdFontAtlas[169] =
          {{0u, 0u, 2u, 0u},
           {0u, 0u, 14u, 0u},
           {105u, 42u, 14u, 13u, "!"},
           {105u, 70u, 14u, 4u, "Double_Prime"},
           {0u, 0u, 14u, 0u},
           {75u, 70u, 14u, 13u, "Ash"}, // looks like AE
           {0u, 0u, 14u, 0u},
           {0u, 0u, 14u, 0u},
           {105u, 70u, 8u, 5u, "Acute_Accent_More_WH"},
           {45u, 70u, 14u, 13u, "("},
           {60u, 70u, 14u, 13u, ")"},
           {105u, 84u, 14u, 0u},
           {75u, 84u, 14u, 13u, "+"},
           {45u, 42u, 14u, 13u, ","},
           {60u, 84u, 14u, 13u, "-"},
           {30u, 42u, 14u, 13u, "."},
           {0u, 0u, 14u, 0u},
           {0u, 56u, 14u, 13u, "0"},
           {15u, 56u, 14u, 13u, "1"},
           {30u, 56u, 14u, 13u, "2"},
           {45u, 56u, 14u, 13u, "3"},
           {60u, 56u, 14u, 13u, "4"},
           {75u, 56u, 14u, 13u, "5"},
           {90u, 56u, 14u, 13u, "6"},
           {105u, 56u, 14u, 13u, "7"},
           {0u, 70u, 14u, 13u, "8"},
           {15u, 70u, 14u, 13u, "9"},
           {60u, 42u, 14u, 13u, ":"},
           {75u, 42u, 14u, 13u, ";"},
           {45u, 70u, 14u, 13u, "("},
           {30u, 70u, 14u, 13u, "="},
           {60u, 70u, 14u, 13u, ")"},
           {90u, 42u, 14u, 13u, "?"},
           {0u, 0u, 14u, 0u},
           {0u, 0u, 14u, 13u, "A"},
           {15u, 0u, 14u, 13u, "B"},
           {30u, 0u, 14u, 13u, "C"},
           {45u, 0u, 14u, 13u, "D"},
           {60u, 0u, 14u, 13u, "E"},
           {75u, 0u, 14u, 13u, "F"},
           {90u, 0u, 14u, 13u, "G"},
           {105u, 0u, 14u, 13u, "H"},
           {0u, 14u, 14u, 13u, "I"},
           {15u, 14u, 14u, 13u, "J"},
           {30u, 14u, 14u, 13u, "K"},
           {45u, 14u, 14u, 13u, "L"},
           {60u, 14u, 14u, 13u, "M"},
           {75u, 14u, 14u, 13u, "N"},
           {90u, 14u, 14u, 13u, "O"},
           {105u, 14u, 14u, 13u, "P"},
           {0u, 28u, 14u, 13u, "Q"},
           {15u, 28u, 14u, 13u, "R"},
           {30u, 28u, 14u, 13u, "S"},
           {45u, 28u, 14u, 13u, "T"},
           {60u, 28u, 14u, 13u, "U"},
           {75u, 28u, 14u, 13u, "V"},
           {90u, 28u, 14u, 13u, "W"},
           {105u, 28u, 14u, 13u, "X"},
           {0u, 42u, 14u, 13u, "Y"},
           {15u, 42u, 14u, 13u, "Z"},
           {45u, 70u, 14u, 13u, "("},
           {90u, 70u, 14u, 13u, "\\"},
           {60u, 70u, 14u, 13u, ")"},
           {0u, 0u, 14u, 0u},
           {0u, 0u, 14u, 0u},
           {105u, 70u, 7u, 4u, "Acute_Accent"},
           {0u, 0u, 14u, 13u, "A"},
           {15u, 0u, 14u, 13u, "B"},
           {30u, 0u, 14u, 13u, "C"},
           {45u, 0u, 14u, 13u, "D"},
           {60u, 0u, 14u, 13u, "E"},
           {75u, 0u, 14u, 13u, "F"},
           {90u, 0u, 14u, 13u, "G"},
           {105u, 0u, 14u, 13u, "H"},
           {0u, 14u, 14u, 13u, "I"},
           {15u, 14u, 14u, 13u, "J"},
           {30u, 14u, 14u, 13u, "K"},
           {45u, 14u, 14u, 13u, "L"},
           {60u, 14u, 14u, 13u, "M"},
           {75u, 14u, 14u, 13u, "N"},
           {90u, 14u, 14u, 13u, "O"},
           {105u, 14u, 14u, 13u, "P"},
           {0u, 28u, 14u, 13u, "Q"},
           {15u, 28u, 14u, 13u, "R"},
           {30u, 28u, 14u, 13u, "S"},
           {45u, 28u, 14u, 13u, "T"},
           {60u, 28u, 14u, 13u, "U"},
           {75u, 28u, 14u, 13u, "V"},
           {90u, 28u, 14u, 13u, "W"},
           {105u, 28u, 14u, 13u, "X"},
           {0u, 42u, 14u, 13u, "Y"},
           {15u, 42u, 14u, 13u, "Z"},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 0u, 9u, 0u},
           {0u, 98u, 20u, 15u, "Button_A"},
           {20u, 98u, 20u, 15u, "Button_B"},
           {40u, 98u, 20u, 15u, "Button_C"},
           {60u, 98u, 20u, 15u, "Button_D"},
           {80u, 98u, 20u, 15u, "Button_E"},
           {0u, 115u, 20u, 15u, "Button_F"},
           {20u, 115u, 20u, 15u, "Button_G"},
           {40u, 115u, 20u, 15u, "Button_H"},
           {0u, 132u, 20u, 15u, "Button_Up"},
           {20u, 132u, 20u, 15u, "Button_Down"},
           {40u, 132u, 20u, 15u, "Button_Left"},
           {60u, 132u, 20u, 15u, "Button_Right"},
           {0u, 0u, 0u, 0u},
           {0u, 0u, 0u, 0u},
           {0u, 0u, 0u, 0u},
           {0u, 0u, 0u, 0u},
           {0u, 0u, 0u, 0u},
           {0u, 0u, 0u, 0u},
           {0u, 0u, 0u, 0u},
           {0u, 0u, 0u, 0u},
           {0u, 0u, 0u, 0u},
           {0u, 0u, 0u, 0u},
           {0u, 0u, 0u, 0u},
           {0u, 0u, 0u, 0u}};

AliveFont::AliveFont()
{
}

AliveFont::AliveFont(s32 maxCharLength, const PalResource& pal, FontContext* fontContext)
{
    Load(maxCharLength, pal, fontContext);
}

void AliveFont::Load(s32 maxCharLength, const PalResource& pal, FontContext* fontContext)
{
    field_34_FontContext = fontContext;
    field_34_FontContext->field_C_resource_id.mCurPal = pal.mPal;
    field_30_poly_count = maxCharLength;
    field_24_fnt_poly_array = relive_new Poly_FT4[maxCharLength * 2];
}

AliveFont::~AliveFont()
{
    relive_delete[] field_24_fnt_poly_array;
}

s32 AliveFont::DrawString(PrimHeader** ppOt, const char_type* text, s32 x, s16 y, TPageAbr abr, s32 bSemiTrans, s32 blendMode, Layer layer, u8 r, u8 g, u8 b, s32 polyOffset, FP scale, s32 maxRenderWidth, s16 colorRandomRange)
{
    if (!sFontDrawScreenSpace)
    {
        x = static_cast<s32>(x / 0.575); // 368 to 640. Convert world space to screen space coords.
    }

    s32 characterRenderCount = 0;
    const s32 maxRenderX = static_cast<s32>(maxRenderWidth / 0.575);
    s16 offsetX = static_cast<s16>(x);
    s32 charInfoIndex = 0;
    auto poly = &field_24_fnt_poly_array[gPsxDisplay.mBufferIndex + (2 * polyOffset)];

    for (u32 i = 0; i < strlen(text); i++)
    {
        if (offsetX >= maxRenderX)
        {
            break;
        }

        const u8 c = text[i];
        if (c <= 0x20u || c > 0xAFu)
        {
            if (c < 7u || c > 0x1Fu)
            {
                offsetX += field_34_FontContext->field_8_atlas_array[1].mWidth;
                continue;
            }
            charInfoIndex = c + 137;
        }
        else
        {
            charInfoIndex = c - 31;
        }

        const auto fContext = field_34_FontContext;
        const auto atlasEntry = &fContext->field_8_atlas_array[charInfoIndex];

        const s8 charWidth = atlasEntry->mWidth;
        const auto charHeight = atlasEntry->mHeight;

        // TODO: Recalc when atlas is converted
        const s8 texture_u = static_cast<s8>(atlasEntry->x);
        const s8 texture_v = static_cast<s8>(atlasEntry->mY);

        const s16 widthScaled = static_cast<s16>(charWidth * FP_GetDouble(scale));
        const s16 heightScaled = static_cast<s16>(charHeight * FP_GetDouble(scale));

        PolyFT4_Init(poly);

        SetPrimExtraPointerHack(poly, nullptr);

        Poly_Set_SemiTrans(&poly->mBase.header, bSemiTrans);
        Poly_Set_Blending(&poly->mBase.header, blendMode);

        SetRGB0(
            poly,
            static_cast<u8>(r + Math_RandomRange(-colorRandomRange, colorRandomRange)),
            static_cast<u8>(g + Math_RandomRange(-colorRandomRange, colorRandomRange)),
            static_cast<u8>(b + Math_RandomRange(-colorRandomRange, colorRandomRange)));

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
        poly->mFont = field_34_FontContext;
        // TPage blend mode start

        OrderingTable_Add(OtLayer(ppOt, layer), &poly->mBase.header);

        ++characterRenderCount;

        offsetX += widthScaled + static_cast<s16>(field_34_FontContext->field_8_atlas_array[0].mWidth * FP_GetExponent(scale));

        poly += 2;
    }

    return polyOffset + characterRenderCount;
}

s32 AliveFont::MeasureTextWidth(const char_type* text)
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
                result += field_34_FontContext->field_8_atlas_array[1].mWidth;
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

        result += field_34_FontContext->field_8_atlas_array[0].mWidth;
        result += field_34_FontContext->field_8_atlas_array[charIndex].mWidth;
    }

    if (!sFontDrawScreenSpace)
    {
        result = static_cast<s32>(result * 0.575); // Convert screen space to world space.
    }

    return result;
}

// Measures the width of a string with scale applied.
s32 AliveFont::MeasureScaledTextWidth(const char_type* text, FP scale)
{
    FP ret = (FP_FromInteger(MeasureTextWidth(text)) * scale) + FP_FromDouble(0.5);
    return FP_GetExponent(ret);
}

// Measures the width of a single character.
s32 AliveFont::MeasureCharacterWidth(char_type character)
{
    s32 result = 0;
    s32 charIndex = 0;

    if (character <= 32u || character > 175u)
    {
        if (character < 7u || character > 31u)
        {
            return field_34_FontContext->field_8_atlas_array[1].mWidth;
        }
        charIndex = character + 137;
    }
    else
    {
        charIndex = character - 31;
    }
    result = field_34_FontContext->field_8_atlas_array[charIndex].mWidth;

    if (!sFontDrawScreenSpace)
    {
        result = static_cast<s32>(result * 0.575); // Convert screen space to world space.
    }

    return result;
}

// Wasn't too sure what to call this. Returns the s8 offset of where the text is cut off. (left and right region)
const char_type* AliveFont::SliceText(const char_type* text, s32 left, FP scale, s32 right)
{
    s32 xOff = 0;
    s32 rightWorldSpace = static_cast<s32>(right * 0.575);

    if (sFontDrawScreenSpace)
    {
        xOff = left;
    }
    else
    {
        xOff = static_cast<s32>(left / 0.575);
    }


    for (const char_type* strPtr = text; *strPtr; strPtr++)
    {
        s32 atlasIdx = 0;
        char_type character = *strPtr;
        if (xOff >= rightWorldSpace)
        {
            return strPtr;
        }

        if (character <= 0x20u || character > 0x7Au)
        {
            if (character < 7u || character > 0x1Fu)
            {
                xOff += field_34_FontContext->field_8_atlas_array[1].mWidth;
                continue;
            }
            atlasIdx = character + 137;
        }
        else
        {
            atlasIdx = character - 31;
        }

        xOff += static_cast<s32>(field_34_FontContext->field_8_atlas_array[atlasIdx].mWidth * FP_GetDouble(scale)) / 0x10000 + field_34_FontContext->field_8_atlas_array->mWidth;
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



