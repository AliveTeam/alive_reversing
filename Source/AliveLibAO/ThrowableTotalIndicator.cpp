#include "stdafx_ao.h"
#include "Function.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "CameraSwapper.hpp"
#include "PauseMenu.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Math.hpp"
#include "../relive_lib/Primitives.hpp"

namespace AO {

ALIVE_VAR(1, 0x504C70, u16, bThrowableIndicatorExists_504C70, 0);

ThrowableTotalIndicator::~ThrowableTotalIndicator()
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
    {
        gObjListDrawables->Remove_Item(this);
    }

    if (field_19E_bFade)
    {
        bThrowableIndicatorExists_504C70--;
    }
}

void ThrowableTotalIndicator::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void ThrowableTotalIndicator::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (sNumCamSwappers_507668 != 0)
    {
        return;
    }

    switch (field_19C_state)
    {
        case ThrowableTotalIndicatorState::eCreated_0:
        {
            field_18_cur_xpos = field_10_xpos - (FP_FromInteger(12) * Math_Sine_451110(static_cast<u8>(2 * sGnFrame)));
            field_1C_cur_ypos = (FP_FromInteger(12) * Math_Cosine_4510A0(static_cast<u8>(2 * sGnFrame))) + field_14_ypos;

            const s16 rgb = FP_GetExponent(FP_FromInteger(48) * Math_Sine_451110(static_cast<u8>(3 * sGnFrame))) + 80;

            field_32_r = rgb;
            field_34_g = rgb;
            field_36_b = rgb;
        }
        break;

        case ThrowableTotalIndicatorState::eFading_1:
            if (field_1C_cur_ypos >= (field_14_ypos - FP_FromInteger(20)))
            {
                if (field_32_r < 70 && field_34_g < 90 && field_36_b < 20)
                {
                    field_32_r += 14;
                    field_34_g += 18;
                    field_36_b += 4;
                }

                field_28_scale += field_2C_scale_speed;
                field_18_cur_xpos += field_20_xspeed;
                field_1C_cur_ypos += field_24_yspeed;
            }
            else
            {
                field_19C_state = ThrowableTotalIndicatorState::eVanishing_2;
            }
            break;

        case ThrowableTotalIndicatorState::eVanishing_2:
            if (field_32_r < 7 && field_34_g < 7 && field_36_b < 7)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            field_34_g -= 9;
            field_36_b -= 2;
            field_32_r -= 7;

            field_18_cur_xpos += field_20_xspeed;
            field_1C_cur_ypos += field_24_yspeed;
            break;
    }
}

const s16 kNum_0[17] = {
    4,
    -3, -4, 3, -4,
    3, -3, 3, 3,
    3, 4, -3, 4,
    -3, 3, -3, -3};

const s16 kNum_1[5] = {
    1,
    2, -4, 2, 4};

const s16 kNum_2[21] = {
    5,
    -5, -4, 5, -4,
    5, -3, 5, -1,
    5, 0, -5, 0,
    -5, 1, -5, 3,
    -5, 4, 5, 4};

const s16 kNum_3[17] = {
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

const s16 kNum_4[13] = {
    3,
    -5, -4, -5, -1,
    -5, 0, 4, 0,
    5, -4, 5, 4};

const s16 kNum_5[21] = {
    5,
    5, -4, -5, -4,
    -5, -3, -5, -1,
    -5, 0, 5, 0,
    5, 1, 5, 3,
    5, 4, -5, 4};

const s16 kNum_6[21] = {
    5,
    5, -4, -5, -4, -5,
    -3, -5, 3, -5, 4,
    5, 4, 5, 3, 5,
    1, 5, 0, -4, 0};

const s16 kNum_7[9] = {
    2,
    -5, -4, 5, -4,
    5, -3, 0, 4};

const s16 kNum_8[21] = {
    5,
    -5, -4, 5, -4,
    5, -3, 5, 3,
    5, 4, -5, 4,
    -5, 3, -5, -3,
    -4, 0, 4, 0};

const s16 kNum_9[17] = {
    4,
    5, 4, 5, -3,
    5, -4, -5, -4,
    -5, -3, -5, -1,
    -5, 0, 4, 0};

const s16 kInfinity[25] = {
    6,
    -3, -2, -5, 0,
    -5, 1, -3, 3,
    -2, 3, 2, -2,
    3, -2, 5, 0,
    5, 1, 3, 3,
    2, 3, -2, -2};

const s16 kCheckpoint[36] = {
    8, 0, -6, 1,
    -6,
    12,
    0,
    13,
    0,
    0,
    6,
    1,
    6,
    -11,
    0,
    -12,
    0,
    2,
    -5,
    11,
    -1,
    11,
    1,
    2,
    5,
    -1,
    5,
    -10,
    1,
    -10,
    -1,
    -1,
    -5,
    0,
    0,
    0};

const s16* kNumbersArray_4C56A8[12] = {
    kNum_0,
    kNum_1,
    kNum_2,
    kNum_3,
    kNum_4,
    kNum_5,
    kNum_6,
    kNum_7,
    kNum_8,
    kNum_9,
    kInfinity,
    kCheckpoint};

void ThrowableTotalIndicator::VRender(PrimHeader** ppOt)
{
    if (*kNumbersArray_4C56A8[field_38_num_to_show] <= 0)
    {
        return;
    }

    const FP_Point* camPos = pScreenManager->mCamPos;
    const FP camX = FP_FromInteger(FP_GetExponent(camPos->x - FP_FromInteger(pScreenManager->mCamXOff)));
    const FP camY = FP_FromInteger(FP_GetExponent(camPos->y - FP_FromInteger(pScreenManager->mCamYOff)));

    s16 xpos = 0;
    s16 ypos = 0;

    for (s16 counter = 0; counter < kNumbersArray_4C56A8[field_38_num_to_show][0]; counter++)
    {
        xpos = FP_GetExponent(field_18_cur_xpos - camX);
        ypos = FP_GetExponent(field_1C_cur_ypos - camY);

        const FP x0 = FP_FromInteger(kNumbersArray_4C56A8[field_38_num_to_show][(4 * counter) + 1]) * field_28_scale;
        const FP y0 = FP_FromInteger(kNumbersArray_4C56A8[field_38_num_to_show][(4 * counter) + 2]) * field_28_scale;
        const FP x1 = FP_FromInteger(kNumbersArray_4C56A8[field_38_num_to_show][(4 * counter) + 3]) * field_28_scale;
        const FP y1 = FP_FromInteger(kNumbersArray_4C56A8[field_38_num_to_show][(4 * counter) + 4]) * field_28_scale;

        s16 primBaseX = 0;
        s16 primVertX = 0;
        if (field_38_num_to_show == 11)
        {
            primBaseX = PsxToPCX(xpos);
            primVertX = PsxToPCX(xpos);
        }
        else
        {
            primBaseX = PsxToPCX(xpos, 11);
            primVertX = PsxToPCX(xpos, 11);
        }

        Line_F2* pLine = &field_3C_lines[gPsxDisplay.mBufferIndex][counter];
        Line_F2_Init(pLine);

        SetXY0(pLine, primBaseX + FP_GetExponent(x0), ypos + FP_GetExponent(y0));
        SetXY1(pLine, primVertX + FP_GetExponent(x1), ypos + FP_GetExponent(y1));
        SetRGB0(pLine,
                static_cast<u8>(field_32_r),
                static_cast<u8>(field_34_g),
                static_cast<u8>(field_36_b));
        Poly_Set_SemiTrans(&pLine->mBase.header, 1);
        OrderingTable_Add(OtLayer(ppOt, field_30_layer), &pLine->mBase.header);
    }

    Init_SetTPage(&field_17C_tPage[gPsxDisplay.mBufferIndex], 1, 0, PSX_getTPage(TPageMode::e4Bit_0, TPageAbr::eBlend_1, 0, 0));
    OrderingTable_Add(OtLayer(ppOt, field_30_layer), &field_17C_tPage->mBase);

    pScreenManager->InvalidateRectCurrentIdx(
        PsxToPCX(xpos - 31),
        ypos - 21,
        PsxToPCX(xpos + 31),
        ypos + 31);
}

ThrowableTotalIndicator::ThrowableTotalIndicator(FP xpos, FP ypos, Layer layer, FP /*scale*/, s32 count, bool bFade)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);
    mBaseGameObjectTypeId = ReliveTypes::eThrowableTotalIndicator;

    gObjListDrawables->Push_Back(this);

    field_10_xpos = xpos;
    field_14_ypos = ypos;

    field_18_cur_xpos = xpos;
    field_1C_cur_ypos = ypos;

    field_20_xspeed = FP_FromInteger(0);

    field_19E_bFade = bFade;

    if (bFade)
    {
        field_24_yspeed = FP_FromDouble(-0.7);
    }
    else
    {
        field_24_yspeed = FP_FromInteger(0);
    }

    field_28_scale = FP_FromInteger(1); // OG bug - should be using scale ??
    field_2C_scale_speed = FP_FromInteger(0);
    field_30_layer = layer;

    field_32_r = 0;
    field_34_g = 0;
    field_36_b = 0;

    if (bFade)
    {
        field_19C_state = ThrowableTotalIndicatorState::eFading_1;
    }
    else
    {
        field_19C_state = ThrowableTotalIndicatorState::eCreated_0;
    }

    if (count == -1)
    {
        field_38_num_to_show = 10;
    }
    else
    {
        field_38_num_to_show = static_cast<s16>(count);
    }

    if (bFade)
    {
        bThrowableIndicatorExists_504C70++;
    }
}

} // namespace AO
