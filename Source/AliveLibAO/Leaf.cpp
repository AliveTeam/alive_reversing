#include "stdafx_ao.h"
#include "Function.hpp"
#include "Leaf.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Collisions.hpp"
#include "Map.hpp"

START_NS_AO

ALIVE_VAR(1, 0x4D148C, BYTE, sLeafRandIdx_4D148C, 8);

void Leaf::VUpdate_48B650()
{
    field_B8_vely += FP_FromDouble(0.5);

    field_B4_velx = field_B4_velx * FP_FromDouble(0.8);
    field_B8_vely = field_B8_vely * FP_FromDouble(0.8);

    const int randX = sRandomBytes_4BBE30[sLeafRandIdx_4D148C++] - 127;
    field_B4_velx += field_BC_sprite_scale * (FP_FromInteger(randX) / FP_FromInteger(64));

    const int randY = sRandomBytes_4BBE30[sLeafRandIdx_4D148C++] - 127;
    field_B8_vely += (field_BC_sprite_scale * (FP_FromInteger(randY) / FP_FromInteger(64)));

    const FP x2 = field_B4_velx + field_A8_xpos;
    const FP y2 = field_B8_vely + field_AC_ypos;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = sCollisions_DArray_504C6C->RayCast_40C410(
        field_A8_xpos,
        field_AC_ypos,
        x2,
        y2,
        &pLine,
        &hitX,
        &hitY,
        17);

    // Hit the floor, die but only if in background..
    if (bCollision && field_BC_sprite_scale == FP_FromDouble(0.5) && pLine->field_8_type == 0)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return;
    }

    if (field_E4_bHitSomething & 1 || !bCollision ||
            ((field_BC_sprite_scale != FP_FromDouble(0.5) || pLine->field_8_type != 4) &&
            (field_BC_sprite_scale != FP_FromInteger(1) || pLine->field_8_type != 0)))
    {
        field_A8_xpos = x2;
        field_AC_ypos = y2;
    }
    else
    {
        field_B4_velx = FP_FromInteger(0);
        field_B8_vely = FP_FromInteger(0);
        
        field_E4_bHitSomething |= 1;

        field_A8_xpos = hitX;
        field_AC_ypos = hitY;
    }

    // Out of the camera, die
    if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Leaf::VUpdate()
{
    VUpdate_48B650();
}

void Leaf::VScreenChanged_48B860()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void Leaf::VScreenChanged()
{
    VScreenChanged_48B860();
}

Leaf* Leaf::ctor_48B4F0(FP xpos, FP ypos, FP xVel, FP yVel, FP scale)
{
    ctor_417C10();
    SetVTable(this, 0x4BD768);

    field_C0_r = 100;
    field_C2_g = 100;
    field_C4_b = 100;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 341, 1, 0);
    if (!ppRes)
    {
        return this;
    }

    auto pHeader = reinterpret_cast<AnimHeader*>(*ppRes);
    Animation_Init_417FD0(
        pHeader->field_4_frame_table_offset,
        pHeader->field_0_max_w,
        pHeader->field_2_max_h,
        ppRes,
        1);

    field_BC_sprite_scale = scale;
    if (field_BC_sprite_scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = 27;
        field_C6_scale = 1;
    }
    else
    {
        field_10_anim.field_C_layer = 8;
        field_C6_scale = 0;
    }

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;

    field_B4_velx = xVel * field_BC_sprite_scale;
    field_B8_vely = yVel * field_BC_sprite_scale;

    sLeafRandIdx_4D148C++;

    field_E4_bHitSomething &= ~1u;

    short randLeftVol = Math_RandomRange_450F20(19, 24);
    if (field_BC_sprite_scale == FP_FromDouble(0.4)) // ??
    {
        randLeftVol -= 7;
    }

    const short randRightVol = Math_RandomRange_450F20(-900, -700);
    SFX_Play_43AE60(SoundEffect::Leaf_27, (3 * randLeftVol) / 4, randRightVol, nullptr);

    return this;
}

BaseGameObject* Leaf::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO

