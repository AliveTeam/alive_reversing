#include "stdafx.h"
#include "DeathBirdParticle.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Dove.hpp"
#include "stdlib.hpp"
#include "Sfx.hpp"

DeathBirdParticle::DeathBirdParticle(FP xpos, FP ypos, s32 start, bool bPlaySound, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eDeathBird_62);

    const AnimRecord& rec = AnimRec(AnimId::DeathFlare_1);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1u);

    if (mBaseGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        mApplyShadows &= ~1u;
        field_20_animation.mRenderMode = TPageAbr::eBlend_1;
        field_CC_sprite_scale = scale;
        field_20_animation.field_14_scale = scale;

        if (scale <= FP_FromDouble(0.5))
        {
            field_20_animation.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }
        else
        {
            field_20_animation.mRenderLayer = Layer::eLayer_Above_FG1_39;
        }

        mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
        mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
        field_F4_random = Math_NextRandom();
        field_F8_start = start;
        field_F5_state = States::eAnimateDeathFlares_0;
        field_FC_bPlaySound = bPlaySound;
    }
}

void DeathBirdParticle::VUpdate()
{
    switch (field_F5_state)
    {
        case States::eAnimateDeathFlares_0:
            if (static_cast<s32>(sGnFrame_5C1B84) > field_F8_start)
            {
                // Death "star"
                field_20_animation.Set_Animation_Data(9912, 0);
                field_F5_state = States::eTransformStarsToDoves_1;
            }
            break;

        case States::eTransformStarsToDoves_1:
            // Has the Death "star" finished animating?
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                // Yes so magic it into a dove
                auto pDove = ae_new<Dove>(
                    5516, // TODO: Hard coded frame table
                    41,
                    20,
                    60,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(15),
                    field_CC_sprite_scale);

                if (pDove->field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    pDove->mBaseAnimatedWithPhysicsGameObject_XPos += FP_FromInteger(8);
                }
                else
                {
                    pDove->mBaseAnimatedWithPhysicsGameObject_XPos -= FP_FromInteger(8);
                }

                pDove->field_CC_sprite_scale = field_CC_sprite_scale;
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);

                if (field_FC_bPlaySound)
                {
                    SFX_Play_Mono(SoundEffect::AbeDove_15, 0);
                }
            }
            break;
    }

    mBaseAnimatedWithPhysicsGameObject_XPos += FP_FromInteger(2) * Math_Sine_496DD0(field_F4_random);
    mBaseAnimatedWithPhysicsGameObject_YPos -= FP_FromInteger(2);
    field_F4_random += 5;
}
