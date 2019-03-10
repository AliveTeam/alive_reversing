#include "stdafx.h"
#include "DeathBirdParticle.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Dove.hpp"
#include "stdlib.hpp"
#include "Sfx.hpp"

BaseGameObject* DeathBirdParticle::VDestructor(signed int flags)
{
    return vdtor_43EE20(flags);
}

void DeathBirdParticle::VUpdate()
{
    Update_43EE70();
}

DeathBirdParticle* DeathBirdParticle::ctor_43ECB0(FP xpos, FP ypos, int start, __int16 playSound, FP scale)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(this, 0x545298); // vTbl_DeathBirdParticle_00545298
    field_4_typeId = BaseGameObject::Types::eDeathBird_62;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kDeathFlareResID);
    Animation_Init_424E10(9940, 122, 43, ppRes, 1, 1u);

    if (field_6_flags.Get(BaseGameObject::eListAddFailed))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    else
    {
        field_DC_bApplyShadows &= ~1u;
        field_20_animation.field_B_render_mode = 1;
        field_CC_sprite_scale = scale;
        field_20_animation.field_14_scale = scale;

        if (scale <= FP_FromDouble(0.5))
        {
            field_20_animation.field_C_render_layer = 17;
        }
        else
        {
            field_20_animation.field_C_render_layer = 39;
        }

        field_B8_xpos = xpos;
        field_BC_ypos = ypos;
        field_F4_random = Math_NextRandom();
        field_F8_start = start;
        field_F5_state = 0;
        field_FC_playSound = playSound;
    }

    return this;
}

void DeathBirdParticle::Update_43EE70()
{
    switch (field_F5_state)
    {
    case 0:
        if (static_cast<int>(sGnFrame_5C1B84) > field_F8_start)
        {
            // Death "star"
            field_20_animation.Set_Animation_Data_409C80(9912, 0);
            field_F5_state = 1;
        }
        break;

    case 1:
        // Has the Death "star" finished animating?
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            // Yes so magic it into a dove
            auto pDove = alive_new<Dove>();
            if (pDove)
            {
                pDove->ctor_41F660(
                    5516,
                    41,
                    20,
                    60,
                    field_B8_xpos,
                    field_BC_ypos - FP_FromInteger(15),
                    field_CC_sprite_scale);
            }

            if (pDove->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                pDove->field_B8_xpos += FP_FromInteger(8);
            }
            else
            {
                pDove->field_B8_xpos -= FP_FromInteger(8);
            }

            pDove->field_CC_sprite_scale = field_CC_sprite_scale;
            field_6_flags.Set(BaseGameObject::eDead);

            if (field_FC_playSound)
            {
                SFX_Play_46FA90(0xFu, 0);
            }
        }

        break;
    }

    field_B8_xpos += FP_FromInteger(2) * Math_Sine_496DD0(field_F4_random);
    field_BC_ypos -= FP_FromInteger(2);
    field_F4_random += 5;
}

void DeathBirdParticle::dtor_43EE50()
{
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

DeathBirdParticle* DeathBirdParticle::vdtor_43EE20(signed int flags)
{
    dtor_43EE50();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}
