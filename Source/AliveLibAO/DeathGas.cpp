#include "stdafx_ao.h"
#include "DeathGas.hpp"
#include "Map.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Abe.hpp"
#include "../relive_lib/Events.hpp"
#include "Math.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/PsxDisplay.hpp"

namespace AO {
struct Data_FP final
{
    FP data[2][5][5];
};

struct GasPolys final
{
    Poly_G4 polys[2][4][4][2];
};

struct Data_Byte final
{
    u8 data[2][5][5];
};

Data_FP xData_500908 = {};
Data_FP yData_5007E8 = {};

GasPolys prims_dword_4FFDE8 = {};
Data_Byte sbyte_3_4FFD78 = {};

Data_Byte sbyte_1_4FFDB0 = {};
Data_Byte sbyte_2_5008B0 = {};
static s32 sDeathGasCount = 0;

DeathGas::DeathGas(Layer layer, s32 amount)
    : BaseGameObject(true, 0)
{
    sDeathGasCount++;

    SetType(ReliveTypes::eFade); // wot moment
    gObjListDrawables->Push_Back(this);
    SetDrawable(true);
    mDone = false;


    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < 2; j++)
        {
            for (s32 k = 0; k < 4; k++)
            {
                for (s32 l = 0; l < 4; l++)
                {
                    Poly_G4* pPoly = &prims_dword_4FFDE8.polys[i][k][l][j];
                    pPoly->SetSemiTransparent( true);
                    pPoly->SetBlendMode(relive::TBlendModes::eBlend_1);
                }
            }
        }
    }

    mLayer = layer;
    mTotal = 0;

    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < 5; j++)
        {
            for (s32 k = 0; k < 5; k++)
            {
                sbyte_1_4FFDB0.data[i][j][k] = Math_NextRandom();
                sbyte_2_5008B0.data[i][j][k] = (Math_NextRandom() & 3) + 2;
            }
        }
    }

    mAmount = static_cast<s16>(amount);
}

DeathGas::~DeathGas()
{
    gObjListDrawables->Remove_Item(this);
    sDeathGasCount--;
}

void DeathGas::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged() || !sActiveHero)
    {
        SetDead(true);
    }
}

void DeathGas::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (!mDone)
    {
        mTotal += mAmount;

        if (mTotal > 255)
        {
            mTotal = 255;
        }
    }
}

void DeathGas::VRender(BasePrimitive** ppOt)
{
    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < 5; j++)
        {
            for (s32 k = 0; k < 5; k++)
            {
                xData_500908.data[i][j][k] = FP_FromInteger(10) * Math_Cosine(sbyte_1_4FFDB0.data[i][j][k]);
                yData_5007E8.data[i][j][k] = FP_FromInteger(10) * Math_Sine(sbyte_1_4FFDB0.data[i][j][k]);
                sbyte_1_4FFDB0.data[i][j][k] += sbyte_2_5008B0.data[i][j][k];
            }
        }
    }

    for (s32 i = 0; i < 5; i++)
    {
        for (s32 j = 0; j < 5; j++)
        {
            const FP cosVal = Math_Cosine(sbyte_1_4FFDB0.data[0][i][j]);
            const FP sinVal = Math_Sine(sbyte_1_4FFDB0.data[0][i][j]);
            const s16 cosMul10 = FP_GetExponent(FP_FromInteger(20) * cosVal);
            const s16 sinMul10 = FP_GetExponent(FP_FromInteger(20) * sinVal);

            s32 tableVal = 0;
            if ((i & 1) && (j & 1))
            {
                tableVal = std::abs(cosMul10) + 20;
            }
            else if (i & 1)
            {
                tableVal = std::abs(sinMul10) + 20;
            }
            else
            {
                tableVal = 20;
            }
            sbyte_3_4FFD78.data[0][i][j] = static_cast<u8>(tableVal);
        }
    }

    // Seems like a hacked copy paste job of the above with the sin value not being used and cos inverted ??
    for (s32 i = 0; i < 5; i++)
    {
        for (s32 j = 0; j < 5; j++)
        {
            const FP cosVal = Math_Cosine(sbyte_1_4FFDB0.data[1][i][j]);
            const s16 cosMul10 = FP_GetExponent(FP_FromInteger(20) * cosVal);

            s32 tableVal = 0;
            if (!(i & 1) && (j & 1))
            {
                tableVal = std::abs(cosMul10) + 20;
            }
            else
            {
                tableVal = 20;
            }
            sbyte_3_4FFD78.data[1][i][j] = static_cast<u8>(tableVal);
        }
    }

    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < 4; j++)
        {
            for (s32 k = 0; k < 4; k++)
            {
                Poly_G4* pPoly = &prims_dword_4FFDE8.polys[i][j][k][gPsxDisplay.mBufferIndex];

                pPoly->SetRGB0(0, sbyte_3_4FFD78.data[i][j][k], 0);
                pPoly->SetRGB1(0, sbyte_3_4FFD78.data[i][j][k + 1], 0);

                if (j == 3)
                {
                    pPoly->SetRGB2(0, 0, 0);
                    pPoly->SetRGB3(0, 0, 0);
                }
                else
                {
                    pPoly->SetRGB2(0, sbyte_3_4FFD78.data[i][j + 1][k], 0);
                    pPoly->SetRGB3(0, sbyte_3_4FFD78.data[i][j + 1][k + 1], 0);
                }

                const s32 heightBase = (gPsxDisplay.mHeight + 56) / 4;

                const s32 height0 = ((j + 0) * heightBase) - 28 + (8 * k);
                const s32 height1 = ((j + 1) * heightBase) - 28 + (8 * k);

                const s32 width0 = ((gPsxDisplay.mWidth + 32) / 4) * (k + 0) - 16;
                const s32 width1 = ((gPsxDisplay.mWidth + 32) / 4) * (k + 1) - 16;


                s32 x0 = FP_GetExponent(xData_500908.data[i][j][k]);
                s32 y0 = FP_GetExponent(yData_5007E8.data[i][j][k]);

                s32 x1 = FP_GetExponent(xData_500908.data[i][j][k + 1]);
                s32 y1 = FP_GetExponent(yData_5007E8.data[i][j][k + 1]);

                s32 x2 = FP_GetExponent(xData_500908.data[i][j + 1][k]);
                s32 y2 = FP_GetExponent(yData_5007E8.data[i][j + 1][k]);

                s32 x3 = FP_GetExponent(xData_500908.data[i][j + 1][k + 1]);
                s32 y3 = FP_GetExponent(yData_5007E8.data[i][j + 1][k + 1]);

                x0 += width0;
                y0 += height0;

                x1 += width1;
                y1 += height0 + 8;

                x2 += width0;
                y2 += height1;

                x3 += width1;
                y3 += height1 + 8;

                const s32 yVal = (gPsxDisplay.mHeight + 28) * (255 - mTotal) / 255;

                pPoly->SetXY0( static_cast<s16>(x0), static_cast<s16>(y0 - yVal));
                pPoly->SetXY1( static_cast<s16>(x1), static_cast<s16>(y1 - yVal));
                pPoly->SetXY2( static_cast<s16>(x2), static_cast<s16>(y2 - yVal));
                pPoly->SetXY3( static_cast<s16>(x3), static_cast<s16>(y3 - yVal));

                OrderingTable_Add(OtLayer(ppOt, mLayer), pPoly);
            }
        }
    }

    if (mTotal >= 255)
    {
        mDone = true;
    }
}

} // namespace AO
