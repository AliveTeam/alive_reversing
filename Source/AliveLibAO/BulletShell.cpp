#include "stdafx_ao.h"
#include "Function.hpp"
#include "BulletShell.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Collisions.hpp"
#include "Sfx.hpp"

START_NS_AO

BulletShell* BulletShell::ctor_462790(FP xpos, FP ypos, __int16 direction, FP scale)
{
    ctor_417C10();
    SetVTable(this, 0x4BC968);

    field_4_typeId = Types::eNone_0;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 360, 1, 0);
    Animation_Init_417FD0(308, 6, 5, ppRes, 1);

    field_BC_sprite_scale = scale;

    if (scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = 36;
    }
    else
    {
        field_10_anim.field_C_layer = 17;
    }

    field_CC_bApplyShadows &= ~1u;
    field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, direction & 1);

    field_EC_hitCount = 0;

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;

    if (direction)
    {
        field_B4_velx = FP_FromInteger(Math_RandomRange_450F20(-6, -3));
    }
    else
    {
        field_B4_velx = FP_FromInteger(Math_RandomRange_450F20(3, 6));
    }
    field_B8_vely = FP_FromInteger(Math_RandomRange_450F20(-4, -1));
    field_F0_speed = FP_FromInteger(1);
    return this;
}

BaseGameObject* BulletShell::dtor_4628B0()
{
    SetVTable(this, 0x4BC968);
    return dtor_417D10();
}

BaseGameObject* BulletShell::VDestructor(signed int flags)
{
    return Vdtor_462A50(flags);
}

BulletShell* BulletShell::Vdtor_462A50(signed int flags)
{
    dtor_4628B0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void BulletShell::VUpdate()
{
    VUpdate_4628C0();
}


void BulletShell::VUpdate_4628C0()
{
    field_A8_xpos = field_B4_velx + field_A8_xpos;
    field_AC_ypos = field_B8_vely + field_AC_ypos;

    field_B8_vely += field_F0_speed;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast_40C410(
        field_A8_xpos,
        field_A8_xpos - field_B8_vely,
        field_A8_xpos,
        field_AC_ypos,
        &field_E4_pLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70) == 1)
    {
        const auto line_type = field_E4_pLine->field_8_type;
        if (!line_type || line_type == 4)
        {
            field_AC_ypos = hitY - FP_FromInteger(1);
            field_B8_vely = -(field_B8_vely * FP_FromDouble(0.3));
            field_B4_velx = (field_B4_velx * FP_FromDouble(0.3));

            if (field_B4_velx < FP_FromInteger(0) && field_B4_velx > FP_FromInteger(-1))
            {
                field_B4_velx = FP_FromInteger(-1);
            }
            else if (field_B4_velx > FP_FromInteger(0) && field_B4_velx < FP_FromInteger(1))
            {
                field_B4_velx = FP_FromInteger(1);
            }

            short volume = 19 * (3 - field_EC_hitCount);
            if (volume <= 19)
            {
                volume = 19;
            }
            SFX_Play_43AD70(SoundEffect::BulletShell_7, volume, 0);
            field_EC_hitCount++;
        }
    }

    if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_EC_hitCount >= 3)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

END_NS_AO
