#include "stdafx.h"
#include "BulletShell.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0xBAF7E0, short, sShellCount_BAF7E0, 0);

BulletShell* BulletShell::ctor_4AD340(FP xpos, FP ypos, __int16 direction, FP scale)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x54708C);

    sShellCount_BAF7E0++;

    if (sShellCount_BAF7E0 >= 11)
    {
        field_6_flags.Clear(BaseGameObject::eDrawable);
        field_6_flags.Set(BaseGameObject::eDead);
        return this;
    }
    else
    {
        field_4_typeId = Types::eNone_0;
        BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, kShellResID);
        Animation_Init_424E10(320, 5, 5u, ppRes, 1, 1);
        
        field_CC_sprite_scale = scale;

        if (scale == FP_FromInteger(1))
        {
            field_20_animation.field_C_render_layer = 36;
        }
        else
        {
            field_20_animation.field_C_render_layer = 17;
        }

        field_DC_bApplyShadows &= ~1u;
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, direction & 1);

        field_FC = 0;
        
        field_B8_xpos = xpos;
        field_BC_ypos = ypos;

        if (direction)
        {
            field_C4_velx = FP_FromInteger(Math_RandomRange_496AB0(-6, -3));
        }
        else
        {
            field_C4_velx = FP_FromInteger(Math_RandomRange_496AB0(3, 6));
        }
        field_C8_vely = FP_FromInteger(Math_RandomRange_496AB0(-4, -1));
        field_100 = FP_FromInteger(1);
    }
    return this;
}

BaseGameObject* BulletShell::VDestructor(signed int flags)
{
    return vdtor_4AD4F0(flags);
}

void BulletShell::VUpdate()
{
    vUpdate_4AD550();
}

BulletShell* BulletShell::vdtor_4AD4F0(signed int flags)
{
    dtor_4AD520();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void BulletShell::dtor_4AD520()
{
    SetVTable(this, 0x54708C);
    sShellCount_BAF7E0--;
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void BulletShell::vUpdate_4AD550()
{
    NOT_IMPLEMENTED();
}
