#include "stdafx_ao.h"
#include "Function.hpp"
#include "DeathBirdParticle.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Dove.hpp"
#include "Math.hpp"
#include "Sfx.hpp"

namespace AO {

void DeathBirdParticle::VUpdate_41DA60()
{
    switch (field_E5_state)
    {
    case States::eAnimateDeathFlares_0:
        if (static_cast<int>(gnFrameCount_507670) > field_E8_start)
        {
            // Death "star"
            field_10_anim.Set_Animation_Data_402A40(6284, 0);
            field_E5_state = States::eTransformStarsToDoves_1;
        }
        break;

    case States::eTransformStarsToDoves_1:
        // Has the Death "star" finished animating?
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            // Yes so magic it into a dove
            auto pDove = ao_new<Dove>();
            if (pDove)
            {
                pDove->ctor_40EFF0(
                    4988,
                    41,
                    20,
                    60,
                    field_A8_xpos,
                    field_AC_ypos - FP_FromInteger(15),
                    field_BC_sprite_scale);
            }

            if (pDove->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                pDove->field_A8_xpos += FP_FromInteger(8);
            }
            else
            {
                pDove->field_A8_xpos -= FP_FromInteger(8);
            }

            field_6_flags.Set(BaseGameObject::eDead_Bit3);

            pDove->field_BC_sprite_scale = field_BC_sprite_scale;
            if (field_EC_bPlaySound)
            {
                SFX_Play_43AD70(SoundEffect::AbeDove_18, 0, 0);
            }
        }
        break;
    }

    field_A8_xpos += FP_FromInteger(2) * Math_Sine_451110(field_E4_random);
    field_AC_ypos -= FP_FromInteger(2);
    field_E4_random += 5;
}

void DeathBirdParticle::VUpdate()
{
    VUpdate_41DA60();
}

DeathBirdParticle* DeathBirdParticle::ctor_41D950(FP xpos, FP ypos, int start, __int16 bPlaySound, FP scale)
{
    ctor_417C10();

    SetVTable(this, 0x4BAF38);
    field_4_typeId = Types::eDeathBird_38;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kDeathFlareResID, 1, 0);
    Animation_Init_417FD0(6312, 70, 43, ppRes, 1);

    if (field_6_flags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else
    {
        field_CC_bApplyShadows &= ~1u;
        field_10_anim.field_B_render_mode = 1;
        field_BC_sprite_scale = scale;
        field_10_anim.field_14_scale = scale;
        if (scale <= FP_FromDouble(0.5))
        {
            field_10_anim.field_C_layer = Layer::eLayer_17;
        }
        else
        {
            field_10_anim.field_C_layer = Layer::eLayer_39;
        }

        field_A8_xpos = xpos;
        field_AC_ypos = ypos;
        field_E4_random = Math_NextRandom();
        field_E8_start = start;
        field_E5_state = States::eAnimateDeathFlares_0;
        field_EC_bPlaySound = bPlaySound;
    }

    return this;
}

BaseGameObject* DeathBirdParticle::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

}

