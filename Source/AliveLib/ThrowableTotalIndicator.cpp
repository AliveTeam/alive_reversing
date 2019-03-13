#include "stdafx.h"
#include "ThrowableTotalIndicator.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x5c112c, WORD, bThrowableIndicatorExists_5C112C, 0);

ThrowableTotalIndicator* ThrowableTotalIndicator::ctor_431CB0(FP xpos, FP ypos, __int16 layer, FP /*scale*/, __int16 count, __int16 bFade)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x544FE4);

    field_6_flags.Set(BaseGameObject::eDrawable);
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
        field_48_count = 10;
    }
    else
    {
        field_48_count = count;
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

void ThrowableTotalIndicator::dtor_431E10()
{
    SetVTable(this, 0x544FE4);

    if (field_6_flags.Get(BaseGameObject::eDrawable))
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
        Mem_Free_495540(this);
    }
    return this;
}

void ThrowableTotalIndicator::vScreenChanged_4323E0()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

void ThrowableTotalIndicator::vUpdate_431EA0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
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
            field_6_flags.Set(BaseGameObject::eDead);
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
