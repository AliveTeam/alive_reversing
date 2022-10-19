#include "stdafx_ao.h"
#include "Function.hpp"
#include "DeathGas.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Primitives.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"

namespace AO {

struct GasPolys final
{
    Poly_G4 polys[2][4][4][2];
};

struct Data_Byte final
{
    u8 data[2][5][5];
};

ALIVE_ARY(1, 0x5008E8, Prim_SetTPage, 2, gGasTPages_5008E8, {});

ALIVE_VAR(1, 0x4FFDE8, GasPolys, prims_dword_4FFDE8, {});

ALIVE_VAR(1, 0x4FFDB0, Data_Byte, sbyte_1_4FFDB0, {});
ALIVE_VAR(1, 0x5008B0, Data_Byte, sbyte_2_5008B0, {});
ALIVE_VAR(1, 0x5009D0, s32, gDeathGasCount_5009D0, 0);

DeathGas* DeathGas::ctor_41CF40(Layer layer, s16 amount)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BAF08);

    gDeathGasCount_5009D0++;

    field_4_typeId = Types::eDeathFadeOut_80; // wot moment
    gObjList_drawables_504618->Push_Back(this);
    field_6_flags.Set(Options::eDrawable_Bit4);
    field_16_flag = 0;

    for (s32 i = 0; i < 2; i++)
    {
        Init_SetTPage_495FB0(&gGasTPages_5008E8[i], 0, 1, PSX_getTPage_4965D0(TPageMode::e16Bit_2, TPageAbr::eBlend_1, 0, 0));
    }

    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < 2; j++)
        {
            for (s32 k = 0; k < 4; k++)
            {
                for (s32 l = 0; l < 4; l++)
                {
                    Poly_G4* pPoly = &prims_dword_4FFDE8.polys[i][k][l][j];
                    PolyG4_Init_498890(pPoly);
                    Poly_Set_SemiTrans_498A40(&pPoly->mBase.header, TRUE);
                }
            }
        }
    }

    field_18_layer = layer;
    field_10_total = 0;
    field_12_unused = 0;

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

    field_14_amount = amount;
    return this;
}

BaseGameObject* DeathGas::VDestructor(s32 flags)
{
    return Vdtor_41D740(flags);
}

BaseGameObject* DeathGas::dtor_41D0E0()
{
    SetVTable(this, 0x4BAF08);
    gObjList_drawables_504618->Remove_Item(this);
    gDeathGasCount_5009D0--;
    return dtor_487DF0();
}

void DeathGas::VScreenChanged_41D700()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path || !sActiveHero_507678)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void DeathGas::VScreenChanged()
{
    VScreenChanged_41D700();
}

void DeathGas::VUpdate_41D150()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (!field_16_flag)
    {
        field_10_total += field_14_amount;
        if (field_10_total > 255)
        {
            field_10_total = 255;
        }
    }
}

void DeathGas::VUpdate()
{
    VUpdate_41D150();
}

DeathGas* DeathGas::Vdtor_41D740(s32 flags)
{
    dtor_41D0E0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void DeathGas::VRender(PrimHeader** ppOt)
{
    VRender_41D190(ppOt);
}

struct Data_FP final
{
    FP data[2][5][5];
};
ALIVE_VAR(1, 0x500908, Data_FP, xData_500908, {});
ALIVE_VAR(1, 0x5007E8, Data_FP, yData_5007E8, {});
ALIVE_VAR(1, 0x4FFD78, Data_Byte, sbyte_3_4FFD78, {});

void DeathGas::VRender_41D190(PrimHeader** ppOt)
{
    field_12_unused += 2;

    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < 5; j++)
        {
            for (s32 k = 0; k < 5; k++)
            {
                xData_500908.data[i][j][k] = FP_FromInteger(10) * Math_Cosine_4510A0(sbyte_1_4FFDB0.data[i][j][k]);
                yData_5007E8.data[i][j][k] = FP_FromInteger(10) * Math_Sine_451110(sbyte_1_4FFDB0.data[i][j][k]);
                sbyte_1_4FFDB0.data[i][j][k] += sbyte_2_5008B0.data[i][j][k];
            }
        }
    }

    for (s32 i = 0; i < 5; i++)
    {
        for (s32 j = 0; j < 5; j++)
        {
            const FP cosVal = Math_Cosine_4510A0(sbyte_1_4FFDB0.data[0][i][j]);
            const FP sinVal = Math_Sine_451110(sbyte_1_4FFDB0.data[0][i][j]);
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
            const FP cosVal = Math_Cosine_4510A0(sbyte_1_4FFDB0.data[1][i][j]);
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
                Poly_G4* pPoly = &prims_dword_4FFDE8.polys[i][j][k][gPsxDisplay_504C78.field_A_buffer_index];

                SetRGB0(pPoly, 0, sbyte_3_4FFD78.data[i][j][k], 0);
                SetRGB1(pPoly, 0, sbyte_3_4FFD78.data[i][j][k + 1], 0);

                if (j == 3)
                {
                    SetRGB2(pPoly, 0, 0, 0);
                    SetRGB3(pPoly, 0, 0, 0);
                }
                else
                {
                    SetRGB2(pPoly, 0, sbyte_3_4FFD78.data[i][j + 1][k], 0);
                    SetRGB3(pPoly, 0, sbyte_3_4FFD78.data[i][j + 1][k + 1], 0);
                }

                const s32 heightBase = (gPsxDisplay_504C78.field_2_height + 56) / 4;

                const s32 height0 = ((j + 0) * heightBase) - 28 + (8 * k);
                const s32 height1 = ((j + 1) * heightBase) - 28 + (8 * k);

                const s32 width0 = ((gPsxDisplay_504C78.field_0_width + 32) / 4) * (k + 0) - 16;
                const s32 width1 = ((gPsxDisplay_504C78.field_0_width + 32) / 4) * (k + 1) - 16;


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

                const s32 yVal = (gPsxDisplay_504C78.field_2_height + 28) * (255 - field_10_total) / 255;

                SetXY0(pPoly, static_cast<s16>(x0), static_cast<s16>(y0 - yVal));
                SetXY1(pPoly, static_cast<s16>(x1), static_cast<s16>(y1 - yVal));
                SetXY2(pPoly, static_cast<s16>(x2), static_cast<s16>(y2 - yVal));
                SetXY3(pPoly, static_cast<s16>(x3), static_cast<s16>(y3 - yVal));

                OrderingTable_Add_498A80(OtLayer(ppOt, field_18_layer), &pPoly->mBase.header);

                /*
                v32 = gPsxDisplay_504C78.field_2_height;
                v33 = gPsxDisplay_504C78.field_0_width + 32;
                *((_BYTE*)pPrim + 34) = 0;
                v34 = v33 / 4;
                v35 = v33 / 4 * v22 - 16;
                v36 = (v32 + 56) / 4;
                v37 = (s32)a2a + v36 * v21 - 28;
                v38 = k5Counter_inner_b + 1;
                v39 = v34 * (k5Counter_inner_b + 1) - 16;
                v40 = (s32)a2a + v36 * v56 - 28;
                v41 = (255 - total) * (v32 + 28) / 255;
                *((_WORD*)pPrim + 6) = v35 + xData_500908[0][0][v26] / 0x10000;
                *((_WORD*)pPrim + 7) = v37 + yData_5007E8[0][0][v26] / 0x10000 - v41;
                *((_WORD*)pPrim + 10) = v39 + xData_500908[0][0][v26 + 1] / 0x10000;
                *((_WORD*)pPrim + 11) = v37 + 8 + yData_5007E8[0][0][v26 + 1] / 0x10000 - v41;
                *((_WORD*)pPrim + 14) = v35
                    + ((((xData_500908[0][1][v26] >> 31) & 0xFFFFu) + xData_500908[0][1][v26]) >> 16);
                *((_WORD*)pPrim + 15) = v40 + yData_5007E8[0][1][v26] / 0x10000 - v41;
                *((_WORD*)pPrim + 18) = v39 + xData_500908[0][1][v26 + 1] / 0x10000;
                *((_WORD*)pPrim + 19) = v40 + 8 + yData_5007E8[0][1][v26 + 1] / 0x10000 - v41;
                OrderingTable_Add_498A80(ppOt_layer, pPrim);
                v22 = v38;
                ++v26;
                v14 = __OFSUB__((_WORD)a2a + 8, 32);
                v13 = (s16)((_WORD)a2a - 24) < 0;
                k5Counter_inner_b = v38;
                a2a += 2;
                v49 += 2;
                ++k2Countera;
                if (!(v13 ^ v14))
                    break;
                v21 = k5Counter_outter_b;
                v24 = k2Countera;
                v25 = v49;
                */
            }
        }
    }

    OrderingTable_Add_498A80(OtLayer(ppOt, field_18_layer), &gGasTPages_5008E8[gPsxDisplay_504C78.field_A_buffer_index].mBase);

    pScreenManager_4FF7C8->InvalidateRect_406CC0(
        0,
        0,
        gPsxDisplay_504C78.field_0_width,
        gPsxDisplay_504C78.field_2_height);

    if (field_10_total >= 255)
    {
        field_16_flag = 1;
    }
}

} // namespace AO
