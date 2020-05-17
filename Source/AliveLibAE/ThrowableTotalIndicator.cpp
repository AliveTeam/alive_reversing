#include "stdafx.h"
#include "ThrowableTotalIndicator.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x5c112c, WORD, bThrowableIndicatorExists_5C112C, 0);

const __int16 kNum_0_551994[17] =
{
    4,
   -3,  -4,   3,   -4,
    3,  -3,   3,    3,
    3,   4,  -3,    4,
   -3,   3,  -3,   -3
};

const __int16 kNum_1_5519B8[5] =
{ 
    1, 
    2, -4, 2, 4
};

const __int16 kNum_2_5519C4[21] =
{
    5,
    -5,  -4,   5,   -4,
    5,   -3,   5,   -1,
    5,    0,  -5,    0,
    -5,   1,  -5,    3,
    -5,   4,   5,    4
};

const __int16 kNum_3_5519F0[17] =
{ 
    4, 
   -5,  -4,  5, -4,
    5,  -3,  5,  3,
    5,   4, -5,  4,
   -4,   0,  4,  0,
};

const __int16 kNum_4_551A14[13] =
{ 
     3,
    -5, -4, -5, -1,
    -5,  0,  4,  0,
     5, -4,  5,  4
};

const __int16 kNum_5_551A30[21] =
{
    5,
    5,   -4,  -5,  -4,
   -5,   -3,  -5,  -1,
   -5,    0,   5,   0,
    5,    1,   5,   3,
    5,    4,  -5,   4
};

const __int16 kNum_6_551A5C[21] =
{
    5,
    5,   -4,   -5,   -4,   -5,
   -3,   -5,    3,   -5,    4,
    5,    4,    5,    3,    5,
    1,    5,    0,   -4,    0
};

const __int16 kNum_7_551A88[9] =
{ 
    2, 
   -5, -4, 5, -4,
    5, -3, 0,  4
};

const __int16 kNum_8_551A9C[21] =
{
    5,
   -5,  -4,   5,  -4,
    5,  -3,   5,   3,
    5,   4,  -5,   4,
   -5,   3,  -5,  -3,
   -4,   0,   4,   0
};

const __int16 kNum_9_551AC8[17] =
{
    4,
    5,   4,   5,  -3,
    5,  -4,  -5,  -4,
   -5,  -3,  -5,  -1,
   -5,   0,   4,   0
};

const __int16 kInfinity_551AEC[25] =
{
   6,
  -3,  -2,  -5,  0,
  -5,   1,  -3,  3,
  -2,   3,   2, -2,
   3,  -2,   5,  0,
   5,   1,   3,  3,
   2,   3,  -2,  -2
};

const __int16* kNumbersArray_551B20[10] =
{
    kNum_0_551994,
    kNum_1_5519B8,
    kNum_2_5519C4,
    kNum_3_5519F0,
    kNum_4_551A14,
    kNum_5_551A30,
    kNum_6_551A5C,
    kNum_7_551A88,
    kNum_8_551A9C,
    kNum_9_551AC8
};

ThrowableTotalIndicator* ThrowableTotalIndicator::ctor_431CB0(FP xpos, FP ypos, __int16 layer, FP /*scale*/, __int16 count, __int16 bFade)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x544FE4);

    field_6_flags.Set(BaseGameObject::eDrawable_Bit4);
    field_4_typeId = Types::eThrowableTotalIndicator_53;

    gObjList_drawables_5C1124->Push_Back(this);

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
        field_18C_state = 1;
    }
    else
    {
        field_18C_state = 0;
    }

    if (count < 0 || count > 9)
    {
        field_48_num_to_show = 10;
    }
    else
    {
        field_48_num_to_show = count;
    }

    if (bFade)
    {
        bThrowableIndicatorExists_5C112C++;
    }

    return this;
}

BaseGameObject* ThrowableTotalIndicator::VDestructor(signed int flags)
{
    return vdtor_431DE0(flags);
}

void ThrowableTotalIndicator::VScreenChanged()
{
    vScreenChanged_4323E0();
}

void ThrowableTotalIndicator::VUpdate()
{
    vUpdate_431EA0();
}

void ThrowableTotalIndicator::VRender(int** pOrderingTable)
{
    vRender_432070(pOrderingTable);
}

void ThrowableTotalIndicator::dtor_431E10()
{
    SetVTable(this, 0x544FE4);

    if (field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
    {
        gObjList_drawables_5C1124->Remove_Item(this);
    }

    if (field_18E_bFade)
    {
        bThrowableIndicatorExists_5C112C--;
    }

    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* ThrowableTotalIndicator::vdtor_431DE0(signed int flags)
{
    dtor_431E10();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void ThrowableTotalIndicator::vScreenChanged_4323E0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void ThrowableTotalIndicator::vUpdate_431EA0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (sNum_CamSwappers_5C1B66 != 0)
    {
        return;
    }

    switch (field_18C_state)
    {
    case 0:
        {
            field_28_cur_xpos = field_20_xpos - (FP_FromInteger(12) * Math_Sine_496DD0(static_cast<BYTE>(2 * sGnFrame_5C1B84)));
            field_2C_cur_ypos = (FP_FromInteger(12) * Math_Cosine_496CD0(static_cast<BYTE>(2 * sGnFrame_5C1B84))) + field_24_ypos;

            const short rgb = FP_GetExponent(FP_FromInteger(48) * Math_Sine_496DD0(static_cast<BYTE>(3 * sGnFrame_5C1B84))) + 80;

            field_42_r = rgb;
            field_44_g = rgb;
            field_46_b = rgb;
        }
        break;

    case 1:
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
            field_18C_state = 2;
        }
        break;

    case 2:
        if (field_42_r < 7 && field_44_g < 7 && field_46_b < 7)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
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

void ThrowableTotalIndicator::vRender_432070(int** pOt)
{
    if (*kNumbersArray_551B20[field_48_num_to_show] <= 0)
    {
        return;
    }

    const FP camX = FP_NoFractional(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
    const FP camY = FP_NoFractional(pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);

    short xpos = 0;
    short ypos = 0;

    for (short counter = 0; counter < kNumbersArray_551B20[field_48_num_to_show][0]; counter++)
    {
        // return static_cast<T>(((40 * x) + 11) / 23);
        // Can't use PsxToPCX as this deals with FP's and it adds 11 before the divide ??

        xpos = FP_GetExponent(((field_28_cur_xpos - camX) * FP_FromInteger(40) + FP_FromInteger(11)) / FP_FromInteger(23));
        ypos = FP_GetExponent(field_2C_cur_ypos - camY);
        const FP x0 = FP_FromInteger(kNumbersArray_551B20[field_48_num_to_show][(4 * counter) + 1]) * field_38_scale;
        const FP y0 = FP_FromInteger(kNumbersArray_551B20[field_48_num_to_show][(4 * counter) + 2]) * field_38_scale;
        const FP x1 = FP_FromInteger(kNumbersArray_551B20[field_48_num_to_show][(4 * counter) + 3]) * field_38_scale;
        const FP y1 = FP_FromInteger(kNumbersArray_551B20[field_48_num_to_show][(4 * counter) + 4]) * field_38_scale;
        Line_G2* pLine = &field_4C_prims[gPsxDisplay_5C1130.field_C_buffer_index][counter];

        LineG2_Init(pLine);

        SetXY0(pLine, xpos + FP_GetExponent(x0), ypos + FP_GetExponent(y0));
        SetXY1(pLine, xpos + FP_GetExponent(x1), ypos + FP_GetExponent(y1));

        SetRGB0(pLine, field_42_r & 0xFF, field_44_g & 0xFF, field_46_b & 0xFF);
        SetRGB1(pLine, field_42_r & 0xFF, field_44_g & 0xFF, field_46_b & 0xFF);

        Poly_Set_SemiTrans_4F8A60(&pLine->mBase.header, TRUE);
        OrderingTable_Add_4F8AA0(&pOt[field_40_layer], &pLine->mBase.header);
    }

    Prim_SetTPage* pTPage = &field_16C_tPage[gPsxDisplay_5C1130.field_C_buffer_index];
    Init_SetTPage_4F5B60(pTPage, 1, 0, PSX_getTPage_4F60E0(0, 1, 0, 0));

    OrderingTable_Add_4F8AA0(&pOt[field_40_layer], &pTPage->mBase);

    pScreenManager_5BB5F4->InvalidateRect_40EC90(
        xpos - 8,
        ypos - 8,
        xpos + 8,
        ypos + 8,
        pScreenManager_5BB5F4->field_3A_idx);
}
