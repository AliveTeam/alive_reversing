#include "stdafx_ao.h"
#include "Function.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "CameraSwapper.hpp"
#include "PauseMenu.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"
#include "Math.hpp"

namespace AO {

ALIVE_VAR(1, 0x504C70, WORD, bThrowableIndicatorExists_504C70, 0);

void ThrowableTotalIndicator::VScreenChanged()
{
    VScreenChanged_41BB70();
}

BaseGameObject* ThrowableTotalIndicator::Vdtor_41BB80(signed int flags)
{
    dtor_41B610();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* ThrowableTotalIndicator::VDestructor(signed int flags)
{
    return Vdtor_41BB80(flags);
}

BaseGameObject* ThrowableTotalIndicator::dtor_41B610()
{
    SetVTable(this, 0x4BAED8);

    if (field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
    {
        gObjList_drawables_504618->Remove_Item(this);
    }

    if (field_19E_bFade)
    {
        bThrowableIndicatorExists_504C70--;
    }

    return dtor_487DF0();
}

void ThrowableTotalIndicator::VScreenChanged_41BB70()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void ThrowableTotalIndicator::VUpdate()
{
    VUpdate_41B690();
}

void ThrowableTotalIndicator::VUpdate_41B690()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (sNumCamSwappers_507668 != 0)
    {
        return;
    }

    switch (field_19C_state)
    {
    case ThrowableTotalIndicatorState::eCreated_0:
    {
        field_18_cur_xpos = field_10_xpos - (FP_FromInteger(12) * Math_Sine_451110(static_cast<BYTE>(2 * gnFrameCount_507670)));
        field_1C_cur_ypos = (FP_FromInteger(12) * Math_Cosine_4510A0(static_cast<BYTE>(2 * gnFrameCount_507670))) + field_14_ypos;

        const short rgb = FP_GetExponent(FP_FromInteger(48) * Math_Sine_451110(static_cast<BYTE>(3 * gnFrameCount_507670))) + 80;

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
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
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

void ThrowableTotalIndicator::VRender(PrimHeader** ppOt)
{
    VRender_41B810(ppOt);
}

const __int16 kNum_0[17] =
{
    4,
    -3, -4, 3, -4,
    3, -3, 3, 3,
    3, 4, -3, 4,
    -3, 3, -3, -3
};

const __int16 kNum_1[5] =
{
    1,
    2, -4, 2, 4
};

const __int16 kNum_2[21] =
{
    5,
    -5, -4, 5, -4,
    5, -3, 5, -1,
    5, 0, -5, 0,
    -5, 1, -5, 3,
    -5, 4, 5, 4
};

const __int16 kNum_3[17] =
{
    4,
    -5, -4, 5, -4,
    5, -3, 5, 3,
    5, 4, -5, 4,
    -4, 0, 4, 0,
};

const __int16 kNum_4[13] =
{
    3,
    -5, -4, -5, -1,
    -5, 0, 4, 0,
    5, -4, 5, 4
};

const __int16 kNum_5[21] =
{
    5,
    5, -4, -5, -4,
    -5, -3, -5, -1,
    -5, 0, 5, 0,
    5, 1, 5, 3,
    5, 4, -5, 4
};

const __int16 kNum_6[21] =
{
    5,
    5, -4, -5, -4, -5,
    -3, -5, 3, -5, 4,
    5, 4, 5, 3, 5,
    1, 5, 0, -4, 0
};

const __int16 kNum_7[9] =
{
    2,
    -5, -4, 5, -4,
    5, -3, 0, 4
};

const __int16 kNum_8[21] =
{
    5,
    -5, -4, 5, -4,
    5, -3, 5, 3,
    5, 4, -5, 4,
    -5, 3, -5, -3,
    -4, 0, 4, 0
};

const __int16 kNum_9[17] =
{
    4,
    5, 4, 5, -3,
    5, -4, -5, -4,
    -5, -3, -5, -1,
    -5, 0, 4, 0
};

const __int16 kInfinity[25] =
{
    6,
    -3, -2, -5, 0,
    -5, 1, -3, 3,
    -2, 3, 2, -2,
    3, -2, 5, 0,
    5, 1, 3, 3,
    2, 3, -2, -2
};

const __int16 kCheckpoint[36] =
{
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
    0
};

const __int16 *kNumbersArray_4C56A8[12] =
{
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
    kCheckpoint
};

void ThrowableTotalIndicator::VRender_41B810(PrimHeader** ppOt)
{
    if (*kNumbersArray_4C56A8[field_38_num_to_show] <= 0)
    {
        return;
    }

    const FP_Point* camPos = pScreenManager_4FF7C8->field_10_pCamPos;
    const FP camX = FP_FromInteger(FP_GetExponent(camPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos)));
    const FP camY = FP_FromInteger(FP_GetExponent(camPos->field_4_y - FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos)));

    short xpos = 0;
    short ypos = 0;

    for (short counter = 0; counter < kNumbersArray_4C56A8[field_38_num_to_show][0]; counter++)
    {
        xpos = FP_GetExponent(field_18_cur_xpos - camX);
        ypos = FP_GetExponent(field_1C_cur_ypos - camY);

        const FP x0 = FP_FromInteger(kNumbersArray_4C56A8[field_38_num_to_show][(4 * counter) + 1]) * field_28_scale;
        const FP y0 = FP_FromInteger(kNumbersArray_4C56A8[field_38_num_to_show][(4 * counter) + 2]) * field_28_scale;
        const FP x1 = FP_FromInteger(kNumbersArray_4C56A8[field_38_num_to_show][(4 * counter) + 3]) * field_28_scale;
        const FP y1 = FP_FromInteger(kNumbersArray_4C56A8[field_38_num_to_show][(4 * counter) + 4]) * field_28_scale;

        short primBaseX = 0;
        short primVertX = 0;
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

        Line_F2* pLine = &field_3C_lines[gPsxDisplay_504C78.field_A_buffer_index][counter];
        Line_F2_Init(pLine);

        SetXY0(pLine, primBaseX + FP_GetExponent(x0), ypos + FP_GetExponent(y0));
        SetXY1(pLine, primVertX + FP_GetExponent(x1), ypos + FP_GetExponent(y1));
        SetRGB0(pLine,
            static_cast<BYTE>(field_32_r),
            static_cast<BYTE>(field_34_g),
            static_cast<BYTE>(field_36_b)
        );
        Poly_Set_SemiTrans_498A40(&pLine->mBase.header, 1);
        OrderingTable_Add_498A80(&ppOt[field_30_layer], &pLine->mBase.header);
    }

    Init_SetTPage_495FB0(&field_17C_tPage[gPsxDisplay_504C78.field_A_buffer_index], 1, 0, PSX_getTPage_4965D0(0, 1, 0, 0));
    OrderingTable_Add_498A80(&ppOt[field_30_layer], &field_17C_tPage->mBase);

    pScreenManager_4FF7C8->InvalidateRect_406E40(
        PsxToPCX(xpos - 31),
        ypos - 21,
        PsxToPCX(xpos + 31),
        ypos + 31,
        pScreenManager_4FF7C8->field_2E_idx
    );
}

ThrowableTotalIndicator* ThrowableTotalIndicator::ctor_41B520(FP xpos, FP ypos, __int16 layer, FP /*scale*/, __int16 count, __int16 bFade)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BAED8);

    field_6_flags.Set(Options::eDrawable_Bit4);
    field_4_typeId = Types::eThrowableTotalIndicator_35;

    gObjList_drawables_504618->Push_Back(this);

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

    field_28_scale = FP_FromInteger(1);  // OG bug - should be using scale ??
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
        field_38_num_to_show = count;
    }

    if (bFade)
    {
        bThrowableIndicatorExists_504C70++;
    }

    return this;
}

}

