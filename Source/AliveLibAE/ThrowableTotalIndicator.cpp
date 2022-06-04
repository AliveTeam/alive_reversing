#include "stdafx.h"
#include "ThrowableTotalIndicator.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x5c112c, u16, bThrowableIndicatorExists_5C112C, 0);

const s16 kNum_0_551994[17] = {
    4,
    -3, -4, 3, -4,
    3, -3, 3, 3,
    3, 4, -3, 4,
    -3, 3, -3, -3};

const s16 kNum_1_5519B8[5] = {
    1,
    2, -4, 2, 4};

const s16 kNum_2_5519C4[21] = {
    5,
    -5, -4, 5, -4,
    5, -3, 5, -1,
    5, 0, -5, 0,
    -5, 1, -5, 3,
    -5, 4, 5, 4};

const s16 kNum_3_5519F0[17] = {
    4,
    -5,
    -4,
    5,
    -4,
    5,
    -3,
    5,
    3,
    5,
    4,
    -5,
    4,
    -4,
    0,
    4,
    0,
};

const s16 kNum_4_551A14[13] = {
    3,
    -5, -4, -5, -1,
    -5, 0, 4, 0,
    5, -4, 5, 4};

const s16 kNum_5_551A30[21] = {
    5,
    5, -4, -5, -4,
    -5, -3, -5, -1,
    -5, 0, 5, 0,
    5, 1, 5, 3,
    5, 4, -5, 4};

const s16 kNum_6_551A5C[21] = {
    5,
    5, -4, -5, -4, -5,
    -3, -5, 3, -5, 4,
    5, 4, 5, 3, 5,
    1, 5, 0, -4, 0};

const s16 kNum_7_551A88[9] = {
    2,
    -5, -4, 5, -4,
    5, -3, 0, 4};

const s16 kNum_8_551A9C[21] = {
    5,
    -5, -4, 5, -4,
    5, -3, 5, 3,
    5, 4, -5, 4,
    -5, 3, -5, -3,
    -4, 0, 4, 0};

const s16 kNum_9_551AC8[17] = {
    4,
    5, 4, 5, -3,
    5, -4, -5, -4,
    -5, -3, -5, -1,
    -5, 0, 4, 0};

const s16 kInfinity_551AEC[25] = {
    6,
    -3, -2, -5, 0,
    -5, 1, -3, 3,
    -2, 3, 2, -2,
    3, -2, 5, 0,
    5, 1, 3, 3,
    2, 3, -2, -2};

const s16* kNumbersArray_551B20[11] = {
    kNum_0_551994,
    kNum_1_5519B8,
    kNum_2_5519C4,
    kNum_3_5519F0,
    kNum_4_551A14,
    kNum_5_551A30,
    kNum_6_551A5C,
    kNum_7_551A88,
    kNum_8_551A9C,
    kNum_9_551AC8,
    kInfinity_551AEC};

ThrowableTotalIndicator::ThrowableTotalIndicator(FP xpos, FP ypos, Layer layer, FP /*scale*/, s32 count, bool bFade)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    SetType(ReliveTypes::eThrowableTotalIndicator);

    gObjListDrawables->Push_Back(this);

    field_20_xpos = xpos;
    field_24_ypos = ypos;

    field_28_cur_xpos = xpos;
    field_2C_cur_ypos = ypos;

    field_30_xspeed = FP_FromInteger(0);

    field_18E_bFade = bFade;

    if (field_18E_bFade)
    {
        field_34_yspeed = FP_FromDouble(-0.7);
    }
    else
    {
        field_34_yspeed = FP_FromInteger(0);
    }

    field_38_scale = FP_FromInteger(1); // OG bug - should be using scale ??
    field_3C_scale_speed = FP_FromInteger(0);
    field_40_layer = layer;

    field_42_r = 0;
    field_44_g = 0;
    field_46_b = 0;

    if (field_18E_bFade)
    {
        field_18C_state = ThrowableTotalIndicatorState::eFading_1;
    }
    else
    {
        field_18C_state = ThrowableTotalIndicatorState::eCreated_0;
    }

    if (count < 0 || count > 9)
    {
        field_48_num_to_show = 10;
    }
    else
    {
        field_48_num_to_show = static_cast<s16>(count);
    }

    if (bFade)
    {
        bThrowableIndicatorExists_5C112C++;
    }
}

ThrowableTotalIndicator::~ThrowableTotalIndicator()
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
    {
        gObjListDrawables->Remove_Item(this);
    }

    if (field_18E_bFade)
    {
        bThrowableIndicatorExists_5C112C--;
    }
}

void ThrowableTotalIndicator::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void ThrowableTotalIndicator::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (sNum_CamSwappers_5C1B66 != 0)
    {
        return;
    }

    switch (field_18C_state)
    {
        case ThrowableTotalIndicatorState::eCreated_0:
        {
            field_28_cur_xpos = field_20_xpos - (FP_FromInteger(12) * Math_Sine_496DD0(static_cast<u8>(2 * sGnFrame)));
            field_2C_cur_ypos = (FP_FromInteger(12) * Math_Cosine_496CD0(static_cast<u8>(2 * sGnFrame))) + field_24_ypos;

            const s16 rgb = FP_GetExponent(FP_FromInteger(48) * Math_Sine_496DD0(static_cast<u8>(3 * sGnFrame))) + 80;

            field_42_r = rgb;
            field_44_g = rgb;
            field_46_b = rgb;
        }
        break;

        case ThrowableTotalIndicatorState::eFading_1:
            if (field_2C_cur_ypos >= field_24_ypos - FP_FromInteger(20))
            {
                if (field_42_r < 70 && field_44_g < 90 && field_46_b < 20)
                {
                    field_42_r += 14;
                    field_44_g += 18;
                    field_46_b += 4;
                }

                field_38_scale += field_3C_scale_speed;
                field_28_cur_xpos += field_30_xspeed;
                field_2C_cur_ypos += field_34_yspeed;
                return;
            }
            else
            {
                field_18C_state = ThrowableTotalIndicatorState::eVanishing_2;
            }
            break;

        case ThrowableTotalIndicatorState::eVanishing_2:
            if (field_42_r < 7 && field_44_g < 7 && field_46_b < 7)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            field_44_g -= 9;
            field_46_b -= 2;
            field_42_r -= 7;

            field_28_cur_xpos += field_30_xspeed;
            field_2C_cur_ypos += field_34_yspeed;
            break;
    }
}

void ThrowableTotalIndicator::VRender(PrimHeader** ppOt)
{
    if (*kNumbersArray_551B20[field_48_num_to_show] <= 0)
    {
        return;
    }

    const FP camX = FP_NoFractional(pScreenManager->field_20_pCamPos->field_0_x);
    const FP camY = FP_NoFractional(pScreenManager->field_20_pCamPos->field_4_y);

    s16 xpos = 0;
    s16 ypos = 0;

    for (s16 counter = 0; counter < kNumbersArray_551B20[field_48_num_to_show][0]; counter++)
    {
        // return static_cast<T>(((40 * x) + 11) / 23);
        // Can't use PsxToPCX as this deals with FP's and it adds 11 before the divide ??

        xpos = FP_GetExponent(((field_28_cur_xpos - camX) * FP_FromInteger(40) + FP_FromInteger(11)) / FP_FromInteger(23));
        ypos = FP_GetExponent(field_2C_cur_ypos - camY);
        const FP x0 = FP_FromInteger(kNumbersArray_551B20[field_48_num_to_show][(4 * counter) + 1]) * field_38_scale;
        const FP y0 = FP_FromInteger(kNumbersArray_551B20[field_48_num_to_show][(4 * counter) + 2]) * field_38_scale;
        const FP x1 = FP_FromInteger(kNumbersArray_551B20[field_48_num_to_show][(4 * counter) + 3]) * field_38_scale;
        const FP y1 = FP_FromInteger(kNumbersArray_551B20[field_48_num_to_show][(4 * counter) + 4]) * field_38_scale;
        Line_G2* pLine = &field_4C_lines[gPsxDisplay_5C1130.field_C_buffer_index][counter];

        LineG2_Init(pLine);

        SetXY0(pLine, xpos + FP_GetExponent(x0), ypos + FP_GetExponent(y0));
        SetXY1(pLine, xpos + FP_GetExponent(x1), ypos + FP_GetExponent(y1));

        SetRGB0(pLine, field_42_r & 0xFF, field_44_g & 0xFF, field_46_b & 0xFF);
        SetRGB1(pLine, field_42_r & 0xFF, field_44_g & 0xFF, field_46_b & 0xFF);

        Poly_Set_SemiTrans_4F8A60(&pLine->mBase.header, TRUE);
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_40_layer), &pLine->mBase.header);
    }

    Prim_SetTPage* pTPage = &field_16C_tPage[gPsxDisplay_5C1130.field_C_buffer_index];
    Init_SetTPage_4F5B60(pTPage, 1, 0, PSX_getTPage_4F60E0(TPageMode::e4Bit_0, TPageAbr::eBlend_1, 0, 0));

    OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_40_layer), &pTPage->mBase);

    pScreenManager->InvalidateRect_40EC90(
        xpos - 8,
        ypos - 8,
        xpos + 8,
        ypos + 8,
        pScreenManager->field_3A_idx);
}
