#include "stdafx_ao.h"
#include "Function.hpp"
#include "SnoozeParticle.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "Game.hpp"

START_NS_AO

const short word_4CF8E0[39] =
{
    1,
    0,
    0,
    1,
    0,
    0,
    1,
    0,
    1,
    0,
    1,
    1,
    1,
    0,
    1,
    0,
    1,
    0,
    0,
    1,
    0,
    0,
    0,
    -1,
    0,
    -1,
    0,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    0,
    -1,
    0,
    0,
    0,
    0
};

BaseGameObject* SnoozeParticle::VDestructor(signed int flags)
{
    return Vdtor_464CE0(flags);
}

BaseGameObject* SnoozeParticle::dtor_4644A0()
{
    SetVTable(this, 0x4BCA18);

    if (field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
    {
        gObjList_drawables_504618->Remove_Item(this);
    }
    return dtor_487DF0();
}

void SnoozeParticle::VScreenChanged_464CD0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void SnoozeParticle::VScreenChanged()
{
    VScreenChanged_464CD0();
}

SnoozeParticle* SnoozeParticle::ctor_464320(FP xpos, FP ypos, __int16 layer, FP scale)
{
    ctor_487E10(1);

    field_6_flags.Set(Options::eDrawable_Bit4);

    SetVTable(this, 0x4BCA18);

    field_4_typeId = Types::eSnoozParticle_87;
    gObjList_drawables_504618->Push_Back(this);

    field_10_x_start = xpos;
    field_14_y_start = ypos;
    field_18_x = xpos;
    field_1C_y = ypos;

    field_24_dy = FP_FromDouble(0.15);
    field_20_dx = FP_FromInteger(1);

    field_24_dy = ((field_24_dy * FP_FromInteger(Math_NextRandom())) / FP_FromInteger(256));
    field_24_dy += FP_FromDouble(0.35);
    field_24_dy = field_24_dy * FP_FromInteger(-1);

    field_30_layer = layer;

    field_28_scale = (scale * FP_FromDouble(0.4));

    field_2C_scale_dx = FP_FromDouble(0.30);
    field_2C_scale_dx = field_2C_scale_dx / (FP_FromInteger(20) / -field_24_dy);

    field_32_r = 0;
    field_34_g = 0;
    field_36_b = 0;
    field_1D4_state = 0;
    field_3A_count_down = 1;
    field_38_idx = Math_NextRandom() % 36;
    field_20_dx = FP_FromInteger(word_4CF8E0[field_38_idx]);
    field_38_idx++;
    return this;
}

SnoozeParticle* SnoozeParticle::Vdtor_464CE0(signed int flags)
{
    dtor_4644A0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void SnoozeParticle::VUpdate()
{
    VUpdate_464500();
}

void SnoozeParticle::VUpdate_464500()
{
    NOT_IMPLEMENTED();
}

void SnoozeParticle::VRender(int** ppOt)
{
    VRender_464620(ppOt);
}

void SnoozeParticle::VRender_464620(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

END_NS_AO
