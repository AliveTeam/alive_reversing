#include "stdafx.h"
#include "Leaf.hpp"
#include "Function.hpp"
#include "Sfx.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x563aa4, BYTE, sLeafRandIdx_563AA4, 8);

Leaf* Leaf::ctor_4E3120(FP xpos, FP ypos, FP xVel, FP yVel, FP scale)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x548004); // vTbl_Leaf_548004

    field_D0_r = 100;
    field_D2_g = 100;
    field_D4_b = 100;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kWellLeafResID);
    AnimHeader* pHeader = reinterpret_cast<AnimHeader*>(*ppRes);

    Animation_Init_424E10(
        pHeader->field_4_frame_table_offset,
        pHeader->field_0_max_w,
        pHeader->field_2_max_h,
        ppRes,
        1,
        1);

    field_CC_sprite_scale = scale;

    if (scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = 27;
        field_D6_scale = 1;
    }
    else
    {
        field_20_animation.field_C_render_layer = 8;
        field_D6_scale = 0;
    }

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;

    field_C4_velx = xVel * field_CC_sprite_scale;
    field_C8_vely = yVel * field_CC_sprite_scale;

    sLeafRandIdx_563AA4++;

    field_F4_bHitSomething &= ~1u;

    short randLeftVol = Math_RandomRange_496AB0(19, 24);
    if (field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        randLeftVol -= 7;
    }

    const short randRightVol = Math_RandomRange_496AB0(-900, -700);
    SFX_Play_46FBA0(SoundEffect::Leaf_22, (3 * randLeftVol) / 4, randRightVol);
    field_1C_update_delay = 1;

    return this;
}

BaseGameObject* Leaf::VDestructor(signed int flags)
{
    return vdtor_4E32E0(flags);
}

void Leaf::VUpdate()
{
    vUpdate_4E3330();
}

void Leaf::VScreenChanged()
{
    vScreenChanged_4E35B0();
}

void Leaf::vUpdate_4E3330()
{
    field_C8_vely += FP_FromDouble(0.5);

    field_C4_velx = field_C4_velx * FP_FromDouble(0.8);
    field_C8_vely = field_C8_vely * FP_FromDouble(0.8);

    const int randX = (sRandomBytes_546744[sLeafRandIdx_563AA4++] - 127);
    field_C4_velx += (field_CC_sprite_scale * (FP_FromInteger(randX) / FP_FromInteger(64)));

    const int randY = (sRandomBytes_546744[sLeafRandIdx_563AA4++] - 127);
    field_C8_vely += (field_CC_sprite_scale * (FP_FromInteger(randY) / FP_FromInteger(64)));

    const FP x2 = field_C4_velx + field_B8_xpos;
    const FP y2 = field_C8_vely + field_BC_ypos;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        x2,
        y2,
        &pLine,
        &hitX,
        &hitY,
        eLineTypes::eFlyingSligCeiling_17); // TODO: Must be another set of constants, this makes no sense ??

    // Hit the floor, die but only if in background..
    if (bCollision && field_CC_sprite_scale == FP_FromDouble(0.5) && pLine->field_8_type == eLineTypes::eFloor_0)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return;
    }

    if (field_F4_bHitSomething & 1 || !bCollision ||
        ((field_CC_sprite_scale != FP_FromDouble(0.5) || pLine->field_8_type != eLineTypes::eBackGroundFloor_4) &&
        (field_CC_sprite_scale != FP_FromDouble(1.0) || pLine->field_8_type != eLineTypes::eFloor_0)))
    {
        field_B8_xpos = x2;
        field_BC_ypos = y2;
    }
    else
    {
        field_C4_velx = FP_FromInteger(0);
        field_C8_vely = FP_FromInteger(0);

        field_F4_bHitSomething |= 1u;

        field_B8_xpos = hitX;
        field_BC_ypos = hitY;
    }

    // Out of the camera, die
    if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos,
        0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Leaf::vScreenChanged_4E35B0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void Leaf::dtor_4E3310()
{
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

Leaf* Leaf::vdtor_4E32E0(signed int flags)
{
    dtor_4E3310();

    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}
