#include "stdafx.h"
#include "DeathGas.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"

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

ALIVE_VAR(1, 0x5BC600, Data_FP, xData_5BC600, {});
ALIVE_ARY(1, 0x5BC6C8, Prim_SetTPage, 2, gGasTPages_5BC6C8, {});
ALIVE_VAR(1, 0x5BC6E8, GasPolys, gasPolys_5BC6E8, {});
ALIVE_VAR(1, 0x5BD0E8, Data_Byte, sbyte_3_5BD0E8, {});
ALIVE_VAR(1, 0x5BD11C, Data_FP, yData_5BD11C, {});
ALIVE_VAR(1, 0x5BD1E4, Data_Byte, sbyte_1_5BD1E4, {});
ALIVE_VAR(1, 0x5BD218, Data_Byte, sbyte_2_5BD218, {});
ALIVE_VAR(1, 0x5BD24C, s32, gDeathGasCount_5BD24C, 0);

DeathGas::DeathGas(Layer layer, s32 amount)
    : BaseGameObject(TRUE, 0)
{
    gDeathGasCount_5BD24C++;

    SetType(ReliveTypes::eMainMenuTransistion); // wot moment
    gObjListDrawables->Push_Back(this);
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    field_26_flag = 0;

    Init_SetTPage(&gGasTPages_5BC6C8[0], 0, 1, PSX_getTPage(TPageMode::e16Bit_2, TPageAbr::eBlend_1, 0, 0));
    Init_SetTPage(&gGasTPages_5BC6C8[1], 0, 1, PSX_getTPage(TPageMode::e16Bit_2, TPageAbr::eBlend_1, 0, 0));

    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < 2; j++)
        {
            for (s32 k = 0; k < 4; k++)
            {
                for (s32 l = 0; l < 4; l++)
                {
                    Poly_G4* pPoly = &gasPolys_5BC6E8.polys[i][k][l][j];
                    PolyG4_Init_4F88B0(pPoly);
                    Poly_Set_SemiTrans(&pPoly->mBase.header, TRUE);
                }
            }
        }
    }

    field_28_layer = layer;
    field_20_total = 0;
    field_22_unused = 0;

    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < 5; j++)
        {
            for (s32 k = 0; k < 5; k++)
            {
                sbyte_1_5BD1E4.data[i][j][k] = Math_NextRandom();
                sbyte_2_5BD218.data[i][j][k] = (Math_NextRandom() & 3) + 2;
            }
        }
    }

    field_24_amount = static_cast<s16>(amount);
}

DeathGas::~DeathGas()
{
    gObjListDrawables->Remove_Item(this);
    gDeathGasCount_5BD24C--;
}

void DeathGas::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mOverlayId != gMap.GetOverlayId() || !sActiveHero)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void DeathGas::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!field_26_flag)
    {
        field_20_total += field_24_amount;

        if (field_20_total > 255)
        {
            field_20_total = 255;
        }
    }
}

void DeathGas::VRender(PrimHeader** ppOt)
{
    field_22_unused += 2;

    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < 5; j++)
        {
            for (s32 k = 0; k < 5; k++)
            {
                xData_5BC600.data[i][j][k] = FP_FromInteger(10) * Math_Cosine_496CD0(sbyte_1_5BD1E4.data[i][j][k]);
                yData_5BD11C.data[i][j][k] = FP_FromInteger(10) * Math_Sine_496DD0(sbyte_1_5BD1E4.data[i][j][k]);
                sbyte_1_5BD1E4.data[i][j][k] += sbyte_2_5BD218.data[i][j][k];
            }
        }
    }

    for (s32 i = 0; i < 5; i++)
    {
        for (s32 j = 0; j < 5; j++)
        {
            const FP cosVal = Math_Cosine_496CD0(sbyte_1_5BD1E4.data[0][i][j]);
            const FP sinVal = Math_Sine_496DD0(sbyte_1_5BD1E4.data[0][i][j]);
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
            sbyte_3_5BD0E8.data[0][i][j] = static_cast<u8>(tableVal);
        }
    }

    // Seems like a hacked copy paste job of the above with the sin value not being used and cos inverted ??
    for (s32 i = 0; i < 5; i++)
    {
        for (s32 j = 0; j < 5; j++)
        {
            const FP cosVal = Math_Cosine_496CD0(sbyte_1_5BD1E4.data[1][i][j]);
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
            sbyte_3_5BD0E8.data[1][i][j] = static_cast<u8>(tableVal);
        }
    }

    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < 4; j++)
        {
            for (s32 k = 0; k < 4; k++)
            {
                Poly_G4* pPoly = &gasPolys_5BC6E8.polys[i][j][k][gPsxDisplay_5C1130.field_C_buffer_index];

                SetRGB0(pPoly, 0, sbyte_3_5BD0E8.data[i][j][k], 0);
                SetRGB1(pPoly, 0, sbyte_3_5BD0E8.data[i][j][k + 1], 0);

                if (j == 3)
                {
                    SetRGB2(pPoly, 0, 0, 0);
                    SetRGB3(pPoly, 0, 0, 0);
                }
                else
                {
                    SetRGB2(pPoly, 0, sbyte_3_5BD0E8.data[i][j + 1][k], 0);
                    SetRGB3(pPoly, 0, sbyte_3_5BD0E8.data[i][j + 1][k + 1], 0);
                }

                const s32 heightBase = (gPsxDisplay_5C1130.field_2_height + 56) / 4;

                const s32 height0 = ((j + 0) * heightBase) - 28 + (8 * k);
                const s32 height1 = ((j + 1) * heightBase) - 28 + (8 * k);

                const s32 width0 = ((gPsxDisplay_5C1130.field_0_width + 32) / 4) * (k + 0) - 16;
                const s32 width1 = ((gPsxDisplay_5C1130.field_0_width + 32) / 4) * (k + 1) - 16;


                s32 x0 = FP_GetExponent(xData_5BC600.data[i][j][k]);
                s32 y0 = FP_GetExponent(yData_5BD11C.data[i][j][k]);

                s32 x1 = FP_GetExponent(xData_5BC600.data[i][j][k + 1]);
                s32 y1 = FP_GetExponent(yData_5BD11C.data[i][j][k + 1]);

                s32 x2 = FP_GetExponent(xData_5BC600.data[i][j + 1][k]);
                s32 y2 = FP_GetExponent(yData_5BD11C.data[i][j + 1][k]);

                s32 x3 = FP_GetExponent(xData_5BC600.data[i][j + 1][k + 1]);
                s32 y3 = FP_GetExponent(yData_5BD11C.data[i][j + 1][k + 1]);

                x0 += width0;
                y0 += height0;

                x1 += width1;
                y1 += height0 + 8;

                x2 += width0;
                y2 += height1;

                x3 += width1;
                y3 += height1 + 8;

                const s32 yVal = (gPsxDisplay_5C1130.field_2_height + 28) * (255 - field_20_total) / 255;

                SetXY0(pPoly, static_cast<s16>(x0), static_cast<s16>(y0 - yVal));
                SetXY1(pPoly, static_cast<s16>(x1), static_cast<s16>(y1 - yVal));
                SetXY2(pPoly, static_cast<s16>(x2), static_cast<s16>(y2 - yVal));
                SetXY3(pPoly, static_cast<s16>(x3), static_cast<s16>(y3 - yVal));

                OrderingTable_Add(OtLayer(ppOt, field_28_layer), &pPoly->mBase.header);
            }
        }
    }

    OrderingTable_Add(OtLayer(ppOt, field_28_layer), &gGasTPages_5BC6C8[gPsxDisplay_5C1130.field_C_buffer_index].mBase);
    pScreenManager->InvalidateRectCurrentIdx(0, 0, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height);

    if (field_20_total >= 255)
    {
        field_26_flag = 1;
    }
}
