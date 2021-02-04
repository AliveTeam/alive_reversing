#include "stdafx.h"
#include "Sparks.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

Sparks* Sparks::ctor_416390(FP xpos, FP ypos, FP scale)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x544534);
    field_4_typeId = Types::eSparks_22;

    const AnimRecord& rec = AnimRec(AnimId::Sparks);
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_DC_bApplyShadows &= ~1u;

    field_20_animation.field_C_render_layer = Layer::eLayer_37;
    field_20_animation.field_B_render_mode = 1;

    field_20_animation.field_A_b = 80;
    field_20_animation.field_9_g = 80;
    field_20_animation.field_8_r = 80;

    field_CC_sprite_scale = scale * (((FP_FromInteger(Math_NextRandom() % 6)) / FP_FromInteger(10)) + FP_FromDouble(0.7));
    field_FA_16_random = Math_RandomRange_496AB0(0, 16);

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;

    field_C4_velx = FP_FromInteger(Math_RandomRange_496AB0(-8, 8));
    field_C8_vely = FP_FromInteger(Math_RandomRange_496AB0(-6, -3));
    return this;
}

BaseGameObject* Sparks::VDestructor(signed int flags)
{
    return vdtor_416520(flags);
}

void Sparks::VUpdate()
{
    vUpdate_416570();
}

void Sparks::VScreenChanged()
{
    vScreenChanged_416720();
}

void Sparks::vUpdate_416570()
{
    if (field_FA_16_random > 0)
    {
        field_FA_16_random--;
    }

    if (field_FA_16_random == 0)
    {
		const AnimRecord& animRec = AnimRec(AnimId::Sparks);
        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
        field_FA_16_random = -1;
    }

    field_C8_vely += FP_FromDouble(0.8);

    field_C4_velx *= FP_FromDouble(0.9);
    field_C8_vely *= FP_FromDouble(0.9);

    field_C4_velx += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
    field_C8_vely += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);

    field_BC_ypos += field_C8_vely;
    field_B8_xpos += field_C4_velx;

    if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Sparks::vScreenChanged_416720()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void Sparks::dtor_416550()
{
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

Sparks* Sparks::vdtor_416520(signed int flags)
{
    dtor_416550();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}
