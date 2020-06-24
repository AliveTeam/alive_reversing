#include "stdafx_ao.h"
#include "Function.hpp"
#include "Sparks.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

START_NS_AO

Sparks* Sparks::ctor_40A3A0(FP xpos, FP ypos, FP scale)
{
    ctor_417C10();
    SetVTable(this, 0x4BA358);
    field_4_typeId = Types::eSpark_15;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 312, 1, 0);
    Animation_Init_417FD0(1532, 38, 21, ppRes, 1);

    field_10_anim.field_A_b = 80;
    field_10_anim.field_9_g = 80;
    field_10_anim.field_8_r = 80;

    field_CC &= ~1u;

    field_10_anim.field_C_layer = 37;
    field_10_anim.field_B_render_mode = 1;

    field_BC_sprite_scale = scale * ((FP_FromInteger(Math_NextRandom() % 6) / FP_FromInteger(10)) + FP_FromDouble(0.2));
    field_EA_random = Math_RandomRange_450F20(0, 16);

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;

    field_B4_velx = FP_FromInteger(Math_RandomRange_450F20(-8, 8));
    field_B8_vely = FP_FromInteger(Math_RandomRange_450F20(-6, -3));
    return this;
}

void Sparks::VUpdate()
{
    VUpdate_40A4F0();
}

void Sparks::VUpdate_40A4F0()
{
    if (field_EA_random > 0)
    {
        field_EA_random--;
    }

    if (field_EA_random == 0)
    {
        field_10_anim.Set_Animation_Data_402A40(1492, 0);
        field_EA_random = -1;
    }

    field_B8_vely += FP_FromDouble(0.8);

    field_B4_velx = field_B4_velx * FP_FromDouble(0.9);
    field_B8_vely = field_B8_vely * FP_FromDouble(0.9);

    field_B4_velx += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
    field_B8_vely += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);

    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;

    if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0))
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void Sparks::VScreenChanged()
{
    VScreenChanged_40A640();
}

void Sparks::VScreenChanged_40A640()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

BaseGameObject* Sparks::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO

