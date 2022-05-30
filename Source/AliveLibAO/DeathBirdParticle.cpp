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

void DeathBirdParticle::VUpdate()
{
    switch (field_E5_state)
    {
        case States::eAnimateDeathFlares_0:
            if (static_cast<s32>(gnFrameCount_507670) > field_E8_start)
            {
                // Death "star"
                field_10_anim.Set_Animation_Data(6284, 0);
                field_E5_state = States::eTransformStarsToDoves_1;
            }
            break;

        case States::eTransformStarsToDoves_1:
            // Has the Death "star" finished animating?
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                const AnimRecord& doveRec = AO::AnimRec(AnimId::Dove_Flying);

                // Yes so magic it into a dove
                auto pDove = ao_new<Dove>(
                    doveRec.mFrameTableOffset,
                    doveRec.mMaxW,
                    doveRec.mMaxH,
                    doveRec.mResourceId,
                    field_A8_xpos,
                    field_AC_ypos - FP_FromInteger(15),
                    field_BC_sprite_scale);
                if (pDove)
                {
                    if (pDove->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        pDove->field_A8_xpos += FP_FromInteger(8);
                    }
                    else
                    {
                        pDove->field_A8_xpos -= FP_FromInteger(8);
                    }

                    mFlags.Set(BaseGameObject::eDead);

                    pDove->field_BC_sprite_scale = field_BC_sprite_scale;
                    if (field_EC_bPlaySound)
                    {
                        SFX_Play_43AD70(SoundEffect::AbeDove_18, 0, 0);
                    }
                }
            }
            break;
    }

    field_A8_xpos += FP_FromInteger(2) * Math_Sine_451110(field_E4_random);
    field_AC_ypos -= FP_FromInteger(2);
    field_E4_random += 5;
}

DeathBirdParticle::DeathBirdParticle(FP xpos, FP ypos, s32 start, s32 bPlaySound, FP scale)
{
    field_4_typeId = Types::eDeathBird_38;

    const AnimRecord& rec = AO::AnimRec(AnimId::DeathFlare_1);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    if (mFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        mFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        field_CC_bApplyShadows &= ~1u;
        field_10_anim.field_B_render_mode = TPageAbr::eBlend_1;
        field_BC_sprite_scale = scale;
        field_10_anim.field_14_scale = scale;
        if (scale <= FP_FromDouble(0.5))
        {
            field_10_anim.field_C_layer = Layer::eLayer_Foreground_Half_17;
        }
        else
        {
            field_10_anim.field_C_layer = Layer::eLayer_Above_FG1_39;
        }

        field_A8_xpos = xpos;
        field_AC_ypos = ypos;
        field_E4_random = Math_NextRandom();
        field_E8_start = start;
        field_E5_state = States::eAnimateDeathFlares_0;
        field_EC_bPlaySound = static_cast<s16>(bPlaySound);
    }
}

} // namespace AO
