#include "stdafx_ao.h"
#include "Font.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Function.hpp"
#include "Math.hpp"
#include "../relive_lib/FatalError.hpp"

namespace AO {

AliveFont::AliveFont()
{
}

AliveFont::AliveFont(s32 maxCharLength, const PalResource& pal, FontContext* fontContext)
{
    Load(maxCharLength, pal, fontContext);
}

void AliveFont::Load(s32 maxCharLength, const PalResource& pal, FontContext* fontContext)
{
    mFontContext = fontContext;
    mFontContext->mFntResource.mCurPal = pal.mPal;
    mPolyCount = maxCharLength;
    mFntPolyArray = relive_new Poly_FT4[maxCharLength * 2];
}

AliveFont::~AliveFont()
{
    relive_delete[] mFntPolyArray;
}

s32 AliveFont::DrawString(OrderingTable& ot, const char_type* text, s32 x, s16 y, relive::TBlendModes blendMode, s32 bSemiTrans, bool disableBlending, Layer layer, u8 r, u8 g, u8 b, s32 polyOffset, FP scale, s32 maxRenderWidth, s16 colorRandomRange)
{
    if (!gFontDrawScreenSpace)
    {
        x = PsxToPCX(x, 11);
    }

    s32 characterRenderCount = 0;
    const s32 maxRenderX = PsxToPCX(maxRenderWidth, 11);
    s16 offsetX = static_cast<s16>(x);
    s32 charInfoIndex = 0;
    auto poly = &mFntPolyArray[gPsxDisplay.mBufferIndex + (2 * polyOffset)];

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
                offsetX += mFontContext->mAtlasArray[0].mWidth + mFontContext->mAtlasArray[1].mWidth;
                continue;
            }
            charInfoIndex = c + 84;
        }
        else
        {
            charInfoIndex = c - 31;
        }

        const auto fContext = mFontContext;
        const auto atlasEntry = &fContext->mAtlasArray[charInfoIndex];

        const s8 charWidth = atlasEntry->mWidth;
        const auto charHeight = atlasEntry->mHeight;

        // TODO: Recalc when atlas is converted
        const s8 texture_u = static_cast<s8>(atlasEntry->x);
        const s8 texture_v = static_cast<s8>(atlasEntry->mY);

        const s16 widthScaled = static_cast<s16>(charWidth * FP_GetDouble(scale));
        const s16 heightScaled = static_cast<s16>(charHeight * FP_GetDouble(scale));

        poly->SetSemiTransparent(bSemiTrans);
        poly->DisableBlending(disableBlending);

        poly->SetRGB0(
            static_cast<u8>(r + Math_RandomRange(-colorRandomRange, colorRandomRange)),
            static_cast<u8>(g + Math_RandomRange(-colorRandomRange, colorRandomRange)),
            static_cast<u8>(b + Math_RandomRange(-colorRandomRange, colorRandomRange)));

        // P0
        poly->SetXY0(offsetX, y);
        poly->SetUV0(texture_u, texture_v);

        // P1
        poly->SetXY1(offsetX + widthScaled, y);
        poly->SetUV1(texture_u + charWidth, texture_v);

        // P2
        poly->SetXY2(offsetX, y + heightScaled);
        poly->SetUV2(texture_u, texture_v + charHeight);

        // P3
        poly->SetXY3(offsetX + widthScaled, y + heightScaled);
        poly->SetUV3(texture_u + charWidth, texture_v + charHeight);

        poly->SetBlendMode(blendMode);

        poly->mFont = mFontContext;

        ot.Add(layer, poly);

        ++characterRenderCount;

        offsetX += widthScaled + FP_GetExponent(FP_FromInteger(mFontContext->mAtlasArray[0].mWidth) * scale);

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
                result += mFontContext->mAtlasArray[1].mWidth;
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

        result += mFontContext->mAtlasArray[0].mWidth;
        result += mFontContext->mAtlasArray[charIndex].mWidth;
    }

    if (!gFontDrawScreenSpace)
    {
        // sub 2??
        result -= mFontContext->mAtlasArray[0].mWidth;
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
            return mFontContext->mAtlasArray[1].mWidth;
        }
        charIndex = character + 84;
    }
    else
    {
        charIndex = character - 31;
    }
    result = mFontContext->mAtlasArray[charIndex].mWidth;

    if (!gFontDrawScreenSpace)
    {
        result = static_cast<s32>(result * 0.575); // Convert screen space to world space.
    }

    return result;
}

// Wasn't too sure what to call this. Returns the s8 offset of where the text is cut off. (left and right region)
const char_type* AliveFont::SliceText(const char_type* text, s32 left, FP scale, s32 right)
{
    s32 xOff = 0;
    s32 rightWorldSpace = PsxToPCX(right, 11);

    if (gFontDrawScreenSpace)
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
            if (character < 8 || character> 31)
            {
                xOff += mFontContext->mAtlasArray[1].mWidth;
                continue;
            }
            atlasIdx = character + 84;
        }
        else
        {
            atlasIdx = character - 31;
        }

        xOff += static_cast<s32>(mFontContext->mAtlasArray[atlasIdx].mWidth * FP_GetDouble(scale)) + mFontContext->mAtlasArray->mWidth;
    }

    return text;
}

} // namespace AO
