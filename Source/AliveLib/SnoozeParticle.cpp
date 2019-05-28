#include "stdafx.h"
#include "SnoozeParticle.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

const __int16 word_560408[39] =
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
    -3
};


SnoozeParticle* SnoozeParticle::ctor_4B06F0(FP xpos, FP ypos, __int16 layer, FP scale)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    field_6_flags.Set(BaseGameObject::eDrawable);
    
    SetVTable(this, 0x5472FC);

    field_4_typeId = Types::eSnoozParticle_124;
    gObjList_drawables_5C1124->Push_Back(this);

    field_20_x_start = xpos;
    field_24_y_start = ypos;
    field_28_x = xpos;
    field_2C_y = ypos;

    // Interesting calc.. ??
    field_34_dy = (FP_FromDouble(0.15) * FP_FromInteger(Math_NextRandom())) / FP_FromInteger(256);
    field_34_dy += FP_FromDouble(0.35);
    field_34_dy = (field_34_dy + FP_FromDouble(0.35)) * FP_FromInteger(-1);

    field_40_layer = layer;

    field_38_scale = scale * FP_FromDouble(0.4);

    field_3C_scale_dx = FP_FromDouble(0.30);
    field_3C_scale_dx = field_3C_scale_dx / (FP_FromInteger(20) / -field_34_dy);

    field_42_r = 0;
    field_44_g = 0;
    field_46_b = 0;
    field_1E4_state = 0;
    field_4A_count_down = 1;
    field_48_idx = Math_NextRandom() % 36;
    field_30_dx = FP_FromDouble(word_560408[field_48_idx]);
    field_48_idx++;
    return this;
}

void SnoozeParticle::dtor_4B0900()
{
    SetVTable(this, 0x5472FC);
    if (field_6_flags.Get(BaseGameObject::eDrawable))
    {
        gObjList_drawables_5C1124->Remove_Item(this);
    }
    BaseGameObject_dtor_4DBEC0();
}

SnoozeParticle* SnoozeParticle::vdtor_4B08D0(signed int flags)
{
    dtor_4B0900();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void SnoozeParticle::vScreenChanged_4B1300()
{
    field_6_flags.Set(BaseGameObject::eDead);
}
