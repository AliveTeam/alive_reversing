#include "stdafx.h"
#include "Collisions.hpp"
#include "../AliveLibCommon/Sys_common.hpp"
#include "../relive_lib/Math.hpp"

ALIVE_VAR(1, 0x5C1128, Collisions*, sCollisions, nullptr);

Collisions::~Collisions()
{
    relive_delete[] field_0_pArray;
}

Collisions::Collisions(LineFormat lineFormat, const CollisionInfo* pCollisionInfo, const u8* pPathRes)
{
    field_8_item_count = pCollisionInfo->field_10_num_collision_items;
    field_4_current_item_count = static_cast<u16>(pCollisionInfo->field_10_num_collision_items);

    // Up to 40 dynamic collisions, slam doors, trap doors, lift platforms etc.
    field_C_max_count = pCollisionInfo->field_10_num_collision_items + (lineFormat == LineFormat::Ao ? 20 : 40);

    // Allocate memory for collisions array
    field_0_pArray = relive_new PathLine[field_C_max_count];

    if (lineFormat == LineFormat::Ao)
    {
        ConvertLineTypes<const PathLineAO*>(pCollisionInfo, pPathRes);
    }
    else
    {
        ConvertLineTypes<const PathLineAE*>(pCollisionInfo, pPathRes);
    }
}

PathLine* Collisions::Add_Dynamic_Collision_Line(s16 x1, s16 y1, s16 x2, s16 y2, eLineTypes mode)
{
    bool freeItemFound = false;
    s32 idx = field_8_item_count;
    while (idx < field_C_max_count)
    {
        PathLine* pIter = &field_0_pArray[idx];
        if (!pIter->mRect.x && !pIter->mRect.w && !pIter->mRect.y && !pIter->mRect.h)
        {
            freeItemFound = true;
            break;
        }
        idx++;
    }

    if (!freeItemFound)
    {
        idx--;
    }

    PathLine* pAddedLine = &field_0_pArray[idx];
    pAddedLine->mRect.x = x1;
    pAddedLine->mRect.y = y1;
    pAddedLine->mRect.w = x2;
    pAddedLine->mRect.h = y2;
    pAddedLine->mLineType = mode;
    pAddedLine->field_C_next = -1;
    pAddedLine->field_A_previous = -1;
    return pAddedLine;
}

// 24:8 fixed type.. I guess 16:16 wasn't good enough for collision detection
class Fixed_24_8 final
{
public:
    Fixed_24_8()
        : fpValue(0)
    {
    }

    explicit Fixed_24_8(s32 val)
    {
        fpValue = val * 256;
    }

    explicit Fixed_24_8(FP fixed16_16)
    {
        fpValue = fixed16_16.fpValue / 256;
    }

    s32 GetExponent() const
    {
        return fpValue / 256;
    }

    Fixed_24_8 Abs() const
    {
        Fixed_24_8 r;
        if (fpValue > 0)
        {
            r.fpValue = fpValue;
        }
        else
        {
            r.fpValue = -fpValue;
        }
        return r;
    }

    s32 fpValue;
};

inline bool operator>(const Fixed_24_8& lhs, const Fixed_24_8& rhs)
{
    return lhs.fpValue > rhs.fpValue;
}

inline bool operator<(const Fixed_24_8& lhs, const Fixed_24_8& rhs)
{
    return lhs.fpValue < rhs.fpValue;
}

inline Fixed_24_8 operator-(const Fixed_24_8& lhs, const Fixed_24_8& rhs)
{
    Fixed_24_8 r;
    r.fpValue = lhs.fpValue - rhs.fpValue;
    return r;
}

inline Fixed_24_8 operator+(const Fixed_24_8& lhs, const Fixed_24_8& rhs)
{
    Fixed_24_8 r;
    r.fpValue = lhs.fpValue + rhs.fpValue;
    return r;
}

inline Fixed_24_8 operator*(const Fixed_24_8& lhs, const Fixed_24_8& rhs)
{
    // Whenever we see alldiv/allmul __PAIR__ and odd 64bit math its likely something like the following.

    // Multiply as 64bit numbers to get more precision in the results
    u64 left = lhs.fpValue;
    u64 right = rhs.fpValue;
    u64 mult = (left * right) / 256;

    Fixed_24_8 r;
    r.fpValue = static_cast<s32>(mult);
    return r;
}

inline Fixed_24_8 operator/(const Fixed_24_8& lhs, const Fixed_24_8& rhs)
{
    Fixed_24_8 r;
    r.fpValue = lhs.fpValue / (rhs.GetExponent());
    return r;
}

Bool32 Collisions::Raycast(FP X1_16_16, FP Y1_16_16, FP X2_16_16, FP Y2_16_16, PathLine** ppLine, FP* hitX, FP* hitY, const CollisionMask& modeMask)
{
    // NOTE: The local static k256_dword_5BC034 is omitted since its actually just a constant of 256

    // The following was a huge help in figuring this out:
    // https://stackoverflow.com/questions/35473936/find-whether-two-line-segments-intersect-or-not-in-c

    Fixed_24_8 x1(X1_16_16);
    Fixed_24_8 x2(X2_16_16);
    Fixed_24_8 y1(Y1_16_16);
    Fixed_24_8 y2(Y2_16_16);

    s32 minX = std::min(x1, x2).GetExponent();
    s32 maxX = std::max(x1, x2).GetExponent();

    s32 minY = std::min(y1, y2).GetExponent();
    s32 maxY = std::max(y1, y2).GetExponent();

    Fixed_24_8 xDiff = x2 - x1;
    Fixed_24_8 yDiff = y2 - y1;

    Fixed_24_8 nearestMatch(2); // 512

    Fixed_24_8 epslion;
    epslion.fpValue = 256; // 0.99

    PathLine* pNearestMatch = nullptr;

    for (s32 i = 0; i < field_C_max_count; i++)
    {
        PathLine* pLine = &field_0_pArray[i];
        if (!(1 << (pLine->mLineType % 32) & modeMask.Mask()))
        {
            // Not a match on type
            continue;
        }

        if (std::min(pLine->mRect.x, pLine->mRect.w) > maxX)
        {
            continue;
        }

        if (std::max(pLine->mRect.x, pLine->mRect.w) < minX)
        {
            continue;
        }

        if (std::min(pLine->mRect.y, pLine->mRect.h) > maxY)
        {
            continue;
        }

        if (std::max(pLine->mRect.y, pLine->mRect.h) < minY)
        {
            continue;
        }

        Fixed_24_8 xDiffCurrent(pLine->mRect.w - pLine->mRect.x);
        Fixed_24_8 yDiffCurrent(pLine->mRect.h - pLine->mRect.y);

        Fixed_24_8 det = (xDiffCurrent * yDiff) - (xDiff * yDiffCurrent);
        if (det.Abs() < epslion)
        {
            continue;
        }

        Fixed_24_8 unknown1 = xDiffCurrent * (Fixed_24_8(pLine->mRect.y) - y1) - yDiffCurrent * (Fixed_24_8(pLine->mRect.x) - x1);

        if (det > Fixed_24_8(0))
        {
            if (unknown1 < Fixed_24_8(0))
            {
                continue;
            }

            if (unknown1 > det)
            {
                continue;
            }
        }
        else
        {
            if (unknown1 > Fixed_24_8(0))
            {
                continue;
            }

            if (unknown1 < det)
            {
                continue;
            }
        }

        Fixed_24_8 unknown2 = xDiff * (Fixed_24_8(pLine->mRect.y) - y1) - yDiff * (Fixed_24_8(pLine->mRect.x) - x1);

        if (det > Fixed_24_8(0))
        {
            if (unknown2 < Fixed_24_8(0))
            {
                continue;
            }

            if (unknown2 > det)
            {
                continue;
            }
        }
        else
        {
            if (unknown2 > Fixed_24_8(0))
            {
                continue;
            }

            if (unknown2 < det)
            {
                continue;
            }
        }

        unknown1 = unknown1 / det;

        if (unknown1 < nearestMatch)
        {
            nearestMatch = unknown1;
            pNearestMatch = pLine;
        }
    }

    if (nearestMatch < Fixed_24_8(2))
    {
        // TODO: This needs cleaning up
        s32 xh = x1.fpValue + ((xDiff.fpValue * nearestMatch.fpValue) / 256);
        s32 yh = y1.fpValue + ((yDiff.fpValue * nearestMatch.fpValue) / 256);

        *hitX = FP_FromRaw(xh << 8);
        *hitY = FP_FromRaw(yh << 8);

        *ppLine = pNearestMatch;

#if DEVELOPER_MODE
        //DebugAddRaycast({X1_16_16, Y1_16_16, X2_16_16, Y2_16_16, *hitX, *hitY, *ppLine, modeMask});
#endif

        return TRUE;
    }

    *ppLine = nullptr;


#if DEVELOPER_MODE
    //DebugAddRaycast({X1_16_16, Y1_16_16, X2_16_16, Y2_16_16, *hitX, *hitY, *ppLine, modeMask});
#endif

    return FALSE;
}

PathLine* Collisions::Get_Line_At_Idx(s16 idx)
{
    if (idx == -1)
    {
        return nullptr;
    }
    return &field_0_pArray[idx];
}

const s32 kNearLineTollerance = 8;

PathLine* Collisions::PreviousLine(PathLine* pLine)
{
    if (pLine->field_A_previous != -1)
    {
        return &field_0_pArray[pLine->field_A_previous];
    }

    if (field_C_max_count == 0)
    {
        return nullptr;
    }

    for (s32 i = 0; i < field_C_max_count; i++)
    {
        if (abs(pLine->mRect.x - field_0_pArray[i].mRect.w) <= kNearLineTollerance && abs(pLine->mRect.y - field_0_pArray[i].mRect.h) <= kNearLineTollerance)
        {
            if ((1 << field_0_pArray[i].mLineType % 32) & (1 << pLine->mLineType % 32))
            {
                return &field_0_pArray[i];
            }
        }
    }
    return nullptr;
}

PathLine* Collisions::NextLine(PathLine* pLine)
{
    if (pLine->field_C_next != -1)
    {
        return &field_0_pArray[pLine->field_C_next];
    }

    if (field_C_max_count == 0)
    {
        return 0;
    }

    for (s32 i = 0; i < field_C_max_count; i++)
    {
        if (abs(pLine->mRect.w - field_0_pArray[i].mRect.x) <= kNearLineTollerance && abs(pLine->mRect.h - field_0_pArray[i].mRect.y) <= kNearLineTollerance)
        {
            if ((1 << field_0_pArray[i].mLineType % 32) & (1 << pLine->mLineType % 32))
            {
                return &field_0_pArray[i];
            }
        }
    }
    return nullptr;
}

PSX_RECT* Rect_Clear(PSX_RECT* pRect)
{
    if (!pRect)
    {
        return nullptr;
    }
    pRect->x = 0;
    pRect->w = 0;
    pRect->y = 0;
    pRect->h = 0;
    return pRect;
}

static s32 ClampMinus1To1(s32 value)
{
    if (value < 0)
    {
        return -1;
    }
    else if (value == 0)
    {
        return 0;
    }
    return 1;
}

static s32 ClampMinus1To1(FP value)
{
    if (value < FP_FromInteger(0))
    {
        return -1;
    }
    else if (value == FP_FromInteger(0))
    {
        return 0;
    }
    return 1;
}

PathLine* PathLine::MoveOnLine(FP* pXPos, FP* pYPos, const FP distToMove)
{
    const FP xpos = *pXPos;
    const FP ypos = *pYPos;

    const FP xDiff = FP_FromInteger(mRect.w - mRect.x);
    const FP yDiff = FP_FromInteger(mRect.h - mRect.y);

    FP xPosRet = {};
    FP yPosRet = {};

    if (yDiff == FP_FromInteger(0))
    {
        if (xDiff >= FP_FromInteger(0))
        {
            xPosRet = distToMove + xpos;
            yPosRet = ypos;
        }
        else
        {
            xPosRet = xpos - distToMove;
            yPosRet = ypos;
        }
    }
    else if (xDiff == FP_FromInteger(0))
    {
        if (yDiff >= FP_FromInteger(0))
        {
            yPosRet = distToMove + ypos;

            if (yPosRet > FP_FromInteger(mRect.h))
            {
                PathLine* pNextLine = sCollisions->NextLine(this);
                if (!pNextLine)
                {
                    return nullptr;
                }
                *pXPos = FP_FromInteger(pNextLine->mRect.x);
                *pYPos = FP_FromInteger(pNextLine->mRect.y);
                return pNextLine->MoveOnLine(pXPos, pYPos, yPosRet - (FP_FromInteger(mRect.h)));
            }

            if (yPosRet < FP_FromInteger(mRect.y))
            {
                PathLine* pPreviousLine = sCollisions->PreviousLine(this);
                if (!pPreviousLine)
                {
                    return nullptr;
                }
                *pXPos = FP_FromInteger(pPreviousLine->mRect.w);
                *pYPos = FP_FromInteger(pPreviousLine->mRect.h);
                return pPreviousLine->MoveOnLine(pXPos, pYPos, yPosRet - (FP_FromInteger(mRect.y)));
            }
        }
        else
        {
            yPosRet = ypos - distToMove;
            if (yPosRet < FP_FromInteger(mRect.h))
            {
                PathLine* pNextLine = sCollisions->NextLine(this);
                if (!pNextLine)
                {
                    return nullptr;
                }
                *pXPos = FP_FromInteger(pNextLine->mRect.x);
                *pYPos = FP_FromInteger(pNextLine->mRect.y);
                return pNextLine->MoveOnLine(pXPos, pYPos, (FP_FromInteger(mRect.h)) - yPosRet);
            }

            if (yPosRet > FP_FromInteger(mRect.y))
            {
                PathLine* pPreviousLine = sCollisions->PreviousLine(this);
                if (!pPreviousLine)
                {
                    return nullptr;
                }
                *pXPos = FP_FromInteger(pPreviousLine->mRect.w);
                *pYPos = FP_FromInteger(pPreviousLine->mRect.h);
                return pPreviousLine->MoveOnLine(pXPos, pYPos, (FP_FromInteger(mRect.y)) - yPosRet);
            }
        }
        *pYPos = yPosRet;
        return this;
    }
    else
    {
        const FP yDiff_2 = FP_Abs(FP_FromInteger(mRect.h - mRect.y));
        const FP xDiff_2 = FP_Abs(xDiff);

        FP squareRoot = {};

        if (yDiff_2 + xDiff_2 <= FP_FromInteger(180))
        {
            squareRoot = Math_SquareRoot_FP_Wrapper(
                (yDiff * yDiff) + (xDiff * xDiff));
        }
        else
        {
            squareRoot = FP_FromInteger(Math_SquareRoot_Int_496E70(
                FP_GetExponent(xDiff) * FP_GetExponent(xDiff) + FP_GetExponent(yDiff) * FP_GetExponent(yDiff)));
        }

        // Round up to 1 to prevent divide by zero
        if ((squareRoot / FP_FromInteger(1)) == FP_FromInteger(0))
        {
            squareRoot = FP_FromInteger(1);
        }

        squareRoot = FP_FromInteger(1) / squareRoot;
        xPosRet = (distToMove * (xDiff * squareRoot)) + (*pXPos);
        yPosRet = (distToMove * (yDiff * squareRoot)) + (*pYPos);
    }

    const s32 xCalc1 = ClampMinus1To1(xPosRet - FP_FromInteger(mRect.w));
    const s32 xCalc2 = ClampMinus1To1(mRect.w - mRect.x);

    if (xCalc1 == xCalc2)
    {
        PathLine* pNextLine = sCollisions->NextLine(this);
        if (!pNextLine)
        {
            return nullptr;
        }

        const FP root1 = Math_SquareRoot_FP_Wrapper(
            (FP_FromInteger(mRect.h) - ypos) * (FP_FromInteger(mRect.h) - ypos) + (FP_FromInteger(mRect.w) - xpos) * (FP_FromInteger(mRect.w) - xpos));

        const FP root2 = Math_SquareRoot_FP_Wrapper(
            ((yPosRet - ypos) * (yPosRet - ypos)) + ((xPosRet - xpos) * (xPosRet - xpos)));

        *pXPos = FP_FromInteger(pNextLine->mRect.x);
        *pYPos = FP_FromInteger(pNextLine->mRect.y);
        return pNextLine->MoveOnLine(pXPos, pYPos, root2 - root1);
    }


    const s32 yCalc1 = ClampMinus1To1(xPosRet - FP_FromInteger(mRect.x));
    const s32 yCalc2 = ClampMinus1To1(mRect.x - mRect.w);

    if (yCalc1 == yCalc2)
    {
        PathLine* pPreviousLine = sCollisions->PreviousLine(this);
        if (!pPreviousLine)
        {
            return nullptr;
        }

        const FP root1 = Math_SquareRoot_FP_Wrapper(
            (yPosRet - ypos) * (yPosRet - ypos) + (xPosRet - xpos) * (xPosRet - xpos));

        const FP root2 = Math_SquareRoot_FP_Wrapper(
            (FP_FromInteger(mRect.y) - ypos) * (FP_FromInteger(mRect.y) - ypos)
            + (FP_FromInteger(mRect.x) - xpos) * (FP_FromInteger(mRect.x) - xpos));

        *pXPos = FP_FromInteger(pPreviousLine->mRect.w);
        *pYPos = FP_FromInteger(pPreviousLine->mRect.h);
        return pPreviousLine->MoveOnLine(pXPos, pYPos, root2 - root1);
    }

    *pXPos = xPosRet;
    *pYPos = yPosRet;

    return this;
}
