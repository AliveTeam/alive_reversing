#include "stdafx_ao.h"
#include "Function.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "CameraSwapper.hpp"
#include "Math.hpp"

START_NS_AO

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

void ThrowableTotalIndicator::VRender(int** ppOt)
{
    VRender_41B810(ppOt);
}

void ThrowableTotalIndicator::VRender_41B810(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
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

END_NS_AO

