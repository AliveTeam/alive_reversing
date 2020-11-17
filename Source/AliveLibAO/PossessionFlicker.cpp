#include "stdafx_ao.h"
#include "Function.hpp"
#include "PossessionFlicker.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

void PossessionFlicker::VUpdate()
{
    VUpdate_41A9B0();
}

void PossessionFlicker::VScreenChanged_41AA40()
{
    if (field_10_pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        field_10_pObj->field_C_refCount--;
        field_10_pObj = nullptr;
        field_6_flags.Set(Options::eDead_Bit3);
    }
}

void PossessionFlicker::VScreenChanged()
{
    VScreenChanged_41AA40();
}

PossessionFlicker* PossessionFlicker::vdtor_41AA70(signed int flags)
{
    dtor_41A940();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* PossessionFlicker::VDestructor(signed int flags)
{
    return vdtor_41AA70(flags);
}

BaseGameObject* PossessionFlicker::dtor_41A940()
{
    SetVTable(this, 0x4BAC58);
    if (field_10_pObj)
    {
        field_10_pObj->field_10_anim.field_B_render_mode = 0;

        field_10_pObj->field_C0_r = field_1E_old_r;
        field_10_pObj->field_C2_g = field_20_old_g;
        field_10_pObj->field_C4_b = field_22_old_b;

        field_10_pObj->field_C_refCount--;
    }
    return dtor_487DF0();
}

PossessionFlicker* PossessionFlicker::ctor_41A8C0(BaseAliveGameObject* pToApplyFlicker, int duration, __int16 r, __int16 g, __int16 b)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BAC58);

    field_10_pObj = pToApplyFlicker;
    field_10_pObj->field_C_refCount++;

    field_18_r = r;
    field_1A_g = g;
    field_1C_b = b;

    field_1E_old_r = pToApplyFlicker->field_C0_r;
    field_20_old_g = pToApplyFlicker->field_C2_g;
    field_22_old_b = pToApplyFlicker->field_C4_b;

    pToApplyFlicker->field_10_anim.field_B_render_mode = 1;

    field_14_time_to_flicker = gnFrameCount_507670 + duration;

    return this;
}


void PossessionFlicker::VUpdate_41A9B0()
{
    bool bFlicker = false;
    if (field_10_pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        field_10_pObj->field_C_refCount--;
        field_10_pObj = nullptr;
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        bFlicker = false;
    }
    else
    {
        bFlicker = true;
    }

    if (bFlicker)
    {
        if (gnFrameCount_507670 % 2)
        {
            field_10_pObj->field_C0_r = field_1E_old_r;
            field_10_pObj->field_C2_g = field_20_old_g;
            field_10_pObj->field_C4_b = field_22_old_b;
        }
        else
        {
            field_10_pObj->field_C0_r = field_18_r;
            field_10_pObj->field_C2_g = field_1A_g;
            field_10_pObj->field_C4_b = field_1C_b;
        }

        if (static_cast<int>(gnFrameCount_507670) > field_14_time_to_flicker)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

}
