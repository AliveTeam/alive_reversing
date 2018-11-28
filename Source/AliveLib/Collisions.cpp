#include "stdafx.h"
#include "Collisions.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "PathData.hpp"
#include "Psx.hpp"
#include "DebugHelpers.hpp"

ALIVE_VAR(1, 0x5C1128, Collisions*, sCollisions_DArray_5C1128, nullptr);

Collisions* Collisions::ctor_418930(const CollisionInfo* pCollisionInfo, const BYTE* pPathRes)
{
    field_8_item_count = pCollisionInfo->field_10_num_collision_items;
    field_4_current_item_count = static_cast<WORD>(pCollisionInfo->field_10_num_collision_items);

    // Up to 40 dynamic collisions, slam doors, trap doors, lift platforms etc.
    field_C_max_count = pCollisionInfo->field_10_num_collision_items + 40;

    // Allocate memory for collisions array
    field_0_pArray = reinterpret_cast<PathLine*>(malloc_non_zero_4954F0(field_C_max_count * sizeof(PathLine)));

    // Copy collision line data out of Path resource
    memcpy(field_0_pArray, &pPathRes[pCollisionInfo->field_C_collision_offset], field_4_current_item_count * sizeof(PathLine));

    // Init dynamic collisions positions to zeros
    for (int i = field_4_current_item_count; i < field_C_max_count; i++)
    {
        field_0_pArray[i].field_0_x1 = 0;
        field_0_pArray[i].field_2_y1 = 0;
        field_0_pArray[i].field_4_x2 = 0;
        field_0_pArray[i].field_6_y2 = 0;
    }
    return this;
}

void Collisions::dtor_4189F0()
{
    Mem_Free_495560(field_0_pArray);
}

void CC Collisions::Factory_4188A0(const CollisionInfo* pCollisionInfo, const BYTE* pPathRes)
{
    sCollisions_DArray_5C1128 = alive_new<Collisions>();
    if (sCollisions_DArray_5C1128)
    {
        sCollisions_DArray_5C1128->ctor_418930(pCollisionInfo, pPathRes);
    }
}

// 24:8 fixed type.. I guess 16:16 wasn't good enough for collision detection
class Fixed_24_8
{
public:
    Fixed_24_8()
        : fpValue(0)
    {

    }

    explicit Fixed_24_8(int val)
    {
        fpValue = val * 256;
    }

    explicit Fixed_24_8(FP fixed16_16)
    {
        fpValue = fixed16_16.fpValue / 256;
    }

    int GetExponent() const
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

    int fpValue;
};

inline bool operator > (const Fixed_24_8& lhs, const Fixed_24_8& rhs)
{
    return lhs.fpValue > rhs.fpValue;
}

inline bool operator < (const Fixed_24_8& lhs, const Fixed_24_8& rhs)
{
    return lhs.fpValue < rhs.fpValue;
}

inline Fixed_24_8 operator - (const Fixed_24_8& lhs, const Fixed_24_8& rhs)
{
    Fixed_24_8 r;
    r.fpValue = lhs.fpValue - rhs.fpValue;
    return r;
}

inline Fixed_24_8 operator + (const Fixed_24_8& lhs, const Fixed_24_8& rhs)
{
    Fixed_24_8 r;
    r.fpValue = lhs.fpValue + rhs.fpValue;
    return r;
}

inline Fixed_24_8 operator * (const Fixed_24_8& lhs, const Fixed_24_8& rhs)
{
    // Whenever we see alldiv/allmul __PAIR__ and odd 64bit math its likely something like the following.

    // Multiply as 64bit numbers to get more precision in the results
    unsigned long long left = lhs.fpValue;
    unsigned long long right = rhs.fpValue;
    unsigned long long mult = (left * right) / 256;

    Fixed_24_8 r;
    r.fpValue = static_cast<int>(mult);
    return r;
}

inline Fixed_24_8 operator / (const Fixed_24_8& lhs, const Fixed_24_8& rhs)
{
    Fixed_24_8 r;
    r.fpValue = lhs.fpValue / (rhs.GetExponent());
    return r;
}

EXPORT PSX_RECT* __stdcall Rect_Clear_418040(PSX_RECT* pRect)
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

signed __int16 Collisions::Raycast_Real_417A60(FP /*X1*/, FP /*Y1*/, FP /*X2*/, FP /*Y2*/, PathLine** /*ppLine*/, FP * /*hitX*/, FP * /*hitY*/, unsigned int /*modeMask*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

signed __int16 Collisions::Raycast_Impl(FP X1_16_16, FP Y1_16_16, FP X2_16_16, FP Y2_16_16, PathLine** ppLine, FP * hitX, FP * hitY, unsigned int modeMask)
{
    // NOTE: The local static k256_dword_5BC034 is omitted since its actually just a constant of 256

    // The following was a huge help in figuring this out:
    // https://stackoverflow.com/questions/35473936/find-whether-two-line-segments-intersect-or-not-in-c

    Fixed_24_8 x1(X1_16_16);
    Fixed_24_8 x2(X2_16_16);
    Fixed_24_8 y1(Y1_16_16);
    Fixed_24_8 y2(Y2_16_16);

    int minX = min(x1, x2).GetExponent();
    int maxX = max(x1, x2).GetExponent();

    int minY = min(y1, y2).GetExponent();
    int maxY = max(y1, y2).GetExponent();

    Fixed_24_8 xDiff = x2 - x1;
    Fixed_24_8 yDiff = y2 - y1;

    Fixed_24_8 nearestMatch(2); // 512

    Fixed_24_8 epslion;
    epslion.fpValue = 256; // 0.99

    PathLine* pNearestMatch = nullptr;

    for (int i = 0; i < field_C_max_count; i++)
    {
        PathLine* pLine = &field_0_pArray[i];
        if (!(1 << (pLine->field_8_type % 32) & modeMask))
        {
            // Not a match on type
            continue;
        }

        if (min(pLine->field_0_x1, pLine->field_4_x2) > maxX)
        {
            continue;
        }

        if (max(pLine->field_0_x1, pLine->field_4_x2) < minX)
        {
            continue;
        }

        if (min(pLine->field_2_y1, pLine->field_6_y2) > maxY)
        {
            continue;
        }

        if (max(pLine->field_2_y1, pLine->field_6_y2) < minY)
        {
            continue;
        }

        Fixed_24_8 xDiffCurrent(pLine->field_4_x2 - pLine->field_0_x1);
        Fixed_24_8 yDiffCurrent(pLine->field_6_y2 - pLine->field_2_y1);

        Fixed_24_8 det = (xDiffCurrent * yDiff) - (xDiff * yDiffCurrent);
        if (det.Abs() < epslion)
        {
            continue;
        }

        Fixed_24_8 unknown1 =
            xDiffCurrent * (Fixed_24_8(pLine->field_2_y1) - y1) -
            yDiffCurrent * (Fixed_24_8(pLine->field_0_x1) - x1);

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

        Fixed_24_8 unknown2 =
            xDiff * (Fixed_24_8(pLine->field_2_y1) - y1) -
            yDiff * (Fixed_24_8(pLine->field_0_x1) - x1);

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
        int xh = x1.fpValue + ((xDiff.fpValue * nearestMatch.fpValue) / 256);
        int yh = y1.fpValue + ((yDiff.fpValue * nearestMatch.fpValue) / 256);

        *hitX = FP_FromRaw(xh << 8);
        *hitY = FP_FromRaw(yh << 8);

        *ppLine = pNearestMatch;

#if DEVELOPER_MODE
        DebugAddRaycast({ X1_16_16,Y1_16_16,X2_16_16,Y2_16_16,*hitX,*hitY, *ppLine, modeMask });
#endif

        return TRUE;
    }

    *ppLine = nullptr;


#if DEVELOPER_MODE
    DebugAddRaycast({ X1_16_16,Y1_16_16,X2_16_16,Y2_16_16,*hitX,*hitY, *ppLine, modeMask });
#endif

    return FALSE;
}

signed __int16 Collisions::Raycast_417A60(FP X1_16_16, FP Y1_16_16, FP X2_16_16, FP Y2_16_16, PathLine** ppLine, FP* hitX, FP* hitY, unsigned int modeMask)
{
    PathLine* pLineImpl = nullptr;
    FP implX = {};
    FP implY = {};
    signed __int16 ret_impl = Raycast_Impl(X1_16_16, Y1_16_16, X2_16_16, Y2_16_16, &pLineImpl, &implX, &implY, modeMask);

    if (IsAlive())
    {
        PathLine* pLineReal = nullptr;
        FP realX = {};
        FP realY = {};
        signed __int16 ret_real = Raycast_Real_417A60(X1_16_16, Y1_16_16, X2_16_16, Y2_16_16, &pLineReal, &realX, &realY, modeMask);

        // TODO: Remove when this is fully re-tested and known to be working
        if (ret_impl == ret_real)
        {
            if (pLineImpl != pLineReal)
            {
                ALIVE_FATAL("Line mismatch");
            }

            if (implX.fpValue != realX.fpValue)
            {
                ALIVE_FATAL("X mismatch");
            }

            if (implY.fpValue != realY.fpValue)
            {
                ALIVE_FATAL("Y mismatch");
            }
        }
        else
        {
            ALIVE_FATAL("ret mismatch");
        }
    }

    *ppLine = pLineImpl;
    *hitX = implX;
    *hitY = implY;

    return ret_impl;
}

PathLine* Collisions::Add_Dynamic_Collision_Line_417FA0(__int16 x1, __int16 y1, __int16 x2, __int16 y2, char mode)
{
    bool freeItemFound = false;
    int idx = field_8_item_count;
    while (idx < field_C_max_count)
    {
        PathLine* pIter = &field_0_pArray[idx];
        if (!pIter->field_0_x1 && !pIter->field_4_x2 && !pIter->field_2_y1 && !pIter->field_6_y2)
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
    pAddedLine->field_0_x1 = x1;
    pAddedLine->field_2_y1 = y1;
    pAddedLine->field_4_x2 = x2;
    pAddedLine->field_6_y2 = y2;
    pAddedLine->field_8_type = mode;
    pAddedLine->field_C_next = -1;
    pAddedLine->field_A_previous = -1;
    return pAddedLine;
}

PathLine* Collisions::Get_Line_At_Idx_418070(__int16 idx)
{
    if (idx == -1)
    {
        return nullptr;
    }
    return &field_0_pArray[idx];
}

const int kNearLineTollerance = 8;

PathLine* Collisions::PreviousLine_4180A0(PathLine* pLine)
{
    if (pLine->field_A_previous != -1)
    {
        return &field_0_pArray[pLine->field_A_previous];
    }

    if (!field_C_max_count)
    {
        return nullptr;
    }

    for (int i = 0; i < field_C_max_count; i++)
    {
        int xDiff = 0;
        if (pLine->field_0_x1 - field_0_pArray[i].field_4_x2 >= 0)
        {
            xDiff = pLine->field_0_x1 - field_0_pArray[i].field_4_x2;
        }
        else
        {
            xDiff = field_0_pArray[i].field_4_x2 - pLine->field_0_x1;
        }

        if (xDiff <= kNearLineTollerance)
        {
            int yDiff = 0;
            if (pLine->field_2_y1 - field_0_pArray[i].field_6_y2 >= 0)
            {
                yDiff = pLine->field_2_y1 - field_0_pArray[i].field_6_y2;
            }
            else
            {
                yDiff = field_0_pArray[i].field_6_y2 - pLine->field_2_y1;
            }

            if (yDiff <= kNearLineTollerance && (1 << field_0_pArray[i].field_8_type % 32) & (1 << pLine->field_8_type % 32))
            {
                return &field_0_pArray[i];
            }
        }
    }
    return nullptr;
}

PathLine* Collisions::NextLine_418180(PathLine* pLine)
{
    if (pLine->field_C_next != -1)
    {
        return &field_0_pArray[pLine->field_C_next];
    }

    if (!field_C_max_count)
    {
        return 0;
    }
    
    for (int i = 0; i < field_C_max_count; i++)
    {
        int xDiff = 0;
        if (pLine->field_4_x2 - field_0_pArray[i].field_0_x1 >= 0)
        {
            xDiff = pLine->field_4_x2 - field_0_pArray[i].field_0_x1;
        }
        else
        {
            xDiff = field_0_pArray[i].field_0_x1 - pLine->field_4_x2;
        }

        if (xDiff <= kNearLineTollerance)
        {
            int yDiff = 0;
            if (pLine->field_6_y2 - field_0_pArray[i].field_2_y1 >= 0)
            {
                yDiff = pLine->field_6_y2 - field_0_pArray[i].field_2_y1;
            }
            else
            {
                yDiff = field_0_pArray[i].field_2_y1 - pLine->field_6_y2;
            }

            if (yDiff <= kNearLineTollerance && (1 << field_0_pArray[i].field_8_type % 32) & (1 << pLine->field_8_type % 32))
            {
                return &field_0_pArray[i];
            }
        }
    }
    return nullptr;
}

namespace Test
{
    void CollisionTests()
    {
        Collisions c;
        FP x1 = {};
        FP y1 = {};
        FP x2 = {};
        FP y2 = {};
        PathLine* line = nullptr;
        FP x = {};
        FP y = {};

        x1.fpValue = 0x03d99a00;
        y1.fpValue = 0x014b028f;
        x2.fpValue = 0x03d99a00;
        y2.fpValue = 0x023b028f;

        PathLine test = 
        {
            0x02bc,
            0x01cc,
            0x0659,
            0x01cc,
            0,
            -1,
            -1,
            -1,
            -1,
            0x039d
        };

        c.field_0_pArray = reinterpret_cast<PathLine*>(malloc_non_zero_4954F0(1 * sizeof(PathLine)));
        memcpy(c.field_0_pArray, &test, 1 * sizeof(PathLine));
        c.field_4_current_item_count = 1;
        c.field_C_max_count = 1;

        signed int ret = c.Raycast_Impl(x1, y1, x2, y2, &line, &x, &y, 0xF);
        ASSERT_EQ(TRUE, ret);

        FP expectedX = {};
        expectedX.fpValue = 0x03d99a00;

        FP expectedY = {};
        expectedY.fpValue = 0x01cb7200;

        ASSERT_EQ(TRUE, ret);
        ASSERT_EQ(expectedX, x);
        ASSERT_EQ(expectedY, y);

        ASSERT_TRUE(memcmp(&test, line, sizeof(PathLine)) == 0);
    }
}

PathLine* PathLine::MoveOnLine_418260(FP* pXPos, FP* pYPos, FP distToMove)
{
    const FP xpos = *pXPos;
    const FP ypos = *pYPos;

    const FP xDiff = FP_FromInteger(field_4_x2 - field_0_x1);
    const FP yDiff = FP_FromInteger(field_6_y2 - field_2_y1);

    FP xPosRet = {};
    FP yPosRet = {};

    FP distXOff = {};

    if (yDiff == FP_FromInteger(0))
    {
        if (xDiff >= FP_FromInteger(0))
        {
            xPosRet = distToMove + xpos;
            yPosRet = ypos;
            distXOff = xPosRet;
        }
        else
        {
            xPosRet = xpos - distToMove;
            yPosRet = ypos;
            distXOff = xPosRet;
        }
        goto LABEL_30;
    }

    if (xDiff == FP_FromInteger(0))
    {
        FP distanceY = {};
        if (yDiff >= FP_FromInteger(0))
        {
            distanceY = distToMove + ypos;

            if (distanceY > FP_FromInteger(field_6_y2))
            {
                PathLine* pNextLine = sCollisions_DArray_5C1128->NextLine_418180(this);
                if (!pNextLine)
                {
                    return 0;
                }
                *pXPos = FP_FromInteger(pNextLine->field_0_x1);
                *pYPos = FP_FromInteger(pNextLine->field_2_y1);
                return pNextLine->MoveOnLine_418260(pXPos, pYPos, distanceY - (FP_FromInteger(field_6_y2)));
            }

            if (distanceY < FP_FromInteger(field_2_y1))
            {
                PathLine* pPreviousLine = sCollisions_DArray_5C1128->PreviousLine_4180A0(this);
                if (!pPreviousLine)
                {
                    return 0;
                }
                *pXPos = FP_FromInteger(pPreviousLine->field_4_x2);
                *pYPos = FP_FromInteger(pPreviousLine->field_6_y2);
                return pPreviousLine->MoveOnLine_418260(pXPos, pYPos, distanceY - (FP_FromInteger(field_2_y1)));
            }
        }
        else
        {
            distanceY = ypos - distToMove;
            if (distanceY < FP_FromInteger(field_6_y2))
            {
                PathLine* pNextLine = sCollisions_DArray_5C1128->NextLine_418180(this);
                if (!pNextLine)
                {
                    return 0;
                }
                *pXPos = FP_FromInteger(pNextLine->field_0_x1);
                *pYPos = FP_FromInteger(pNextLine->field_2_y1);
                return pNextLine->MoveOnLine_418260(pXPos, pYPos, (FP_FromInteger(field_6_y2)) - distanceY);
            }

            if (distanceY > FP_FromInteger(field_2_y1))
            {
                PathLine* pPreviousLine = sCollisions_DArray_5C1128->PreviousLine_4180A0(this);
                if (!pPreviousLine)
                {
                    return 0;
                }
                *pXPos = FP_FromInteger(pPreviousLine->field_4_x2);
                *pYPos = FP_FromInteger(pPreviousLine->field_6_y2);
                return pPreviousLine->MoveOnLine_418260(pXPos, pYPos, (FP_FromInteger(field_2_y1)) - distanceY);
            }
        }
        *pYPos = distanceY;
        return this;
    }

    FP yDiff_2 = yDiff;
    if (yDiff < FP_FromInteger(0))
    {
        yDiff_2 = FP_FromInteger(-1 * (field_6_y2 - field_2_y1));
    }

    FP xDiff_2 = xDiff;
    if (xDiff < FP_FromInteger(0))
    {
        xDiff_2 = -xDiff;
    }

    FP yDiffSquared;
    FP xDiffSquared;
    FP squareRoot;

    if (yDiff_2 + xDiff_2 <= FP_FromInteger(180))
    {
        yDiffSquared = yDiff * yDiff;
        xDiffSquared = xDiff * xDiff;
        squareRoot = Math_SquareRoot_FP_496E90(static_cast<short>(FP_GetExponent(yDiffSquared + xDiffSquared)));
    }
    else
    {
        squareRoot = FP_FromInteger(Math_SquareRoot_Int_496E70(FP_GetExponent(xDiff)* FP_GetExponent(xDiff) +
            FP_GetExponent(yDiff) * FP_GetExponent(yDiff)));
    }

    // Round up to 1
    if ((squareRoot / FP_FromInteger(1)) == FP_FromInteger(0))
    {
        squareRoot = FP_FromInteger(1);
    }

    const FP op2g = FP_FromInteger(1) / squareRoot;
    const FP v26 = xDiff * op2g;
    distXOff = (distToMove * v26) + *pXPos;
    const FP v27 = yDiff * op2g;
    const FP v28 = distToMove * v27;
    xPosRet = distXOff;
    yPosRet = v28 + *pYPos;

LABEL_30:
    const FP lineX2_fp = FP_FromInteger(this->field_4_x2);
    
    int op2b = 0;
    if (xPosRet - lineX2_fp <= FP_FromInteger(0))
    {
        op2b = (xPosRet - lineX2_fp >= FP_FromInteger(0)) - 1;
    }
    else
    {
        op2b = 1;
    }

    const int v30 = this->field_4_x2 - this->field_0_x1;
    int v31 = 0;
    if (v30 <= 0)
    {
        v31 = (v30 >= 0) - 1;
    }
    else
    {
        v31 = 1;
    }
    
    if (op2b == v31)
    {
        PathLine* pNextLine = sCollisions_DArray_5C1128->NextLine_418180(this);
        if (!pNextLine)
        {
            return 0;
        }

        const FP v32 = (FP_FromInteger(field_6_y2) - ypos) * (FP_FromInteger(field_6_y2) - ypos);
        const FP v33 = (FP_FromInteger(field_4_x2) - xpos) * (FP_FromInteger(field_4_x2) - xpos);
        const FP v34 = Math_SquareRoot_FP_496E90(static_cast<short>(FP_GetExponent(v32 + v33)));
        const FP v35 = (yPosRet - ypos) * (yPosRet - ypos);
        const FP v36 = (distXOff - xpos) * (distXOff - xpos);
        const FP v37 = Math_SquareRoot_FP_496E90(static_cast<short>(FP_GetExponent(v35 + v36)));

        *pXPos = FP_FromInteger(pNextLine->field_0_x1);
        *pYPos = FP_FromInteger(pNextLine->field_2_y1);
        return pNextLine->MoveOnLine_418260(pXPos, pYPos, v37 - v34);
    }

    const FP line_x1_fp = FP_FromInteger(field_0_x1);

    int op2d = 0;
    if (xPosRet - line_x1_fp <= FP_FromInteger(0))
    {
        op2d = (xPosRet - line_x1_fp >= FP_FromInteger(0)) - 1;
    }
    else
    {
        op2d = 1;
    }

    const int line_x_diff2 = this->field_0_x1 - this->field_4_x2;
    int v40 = 0;
    if (line_x_diff2 <= 0)
    {
        v40 = (line_x_diff2 >= 0) - 1;
    }
    else
    {
        v40 = 1;
    }

    if (op2d == v40)
    {
        PathLine* pPreviousLine = sCollisions_DArray_5C1128->PreviousLine_4180A0(this);
        if (!pPreviousLine)
        {
            return 0;
        }

        const FP a4b = (yPosRet - ypos) * (yPosRet - ypos);
        const FP v41 = (distXOff - xpos) * (distXOff - xpos);
        const FP a4c = Math_SquareRoot_FP_496E90(static_cast<short>(FP_GetExponent(a4b + v41)));
        const FP v42 = (FP_FromInteger(field_2_y1) - ypos) * (FP_FromInteger(field_2_y1) - ypos);
        const FP v43 = (FP_FromInteger(field_0_x1) - xpos) * (FP_FromInteger(field_0_x1) - xpos);
        const FP v44 = Math_SquareRoot_FP_496E90(static_cast<short>(FP_GetExponent(v42 + v43)));

        *pXPos = FP_FromInteger(pPreviousLine->field_4_x2);
        *pYPos = FP_FromInteger(pPreviousLine->field_6_y2);
        return pPreviousLine->MoveOnLine_418260(pXPos, pYPos, v44 - a4c);
    }

    *pXPos = xPosRet;
    *pYPos = yPosRet;

    return this;
}
