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
            if (static_cast<s32>(sGnFrame) > field_E8_start)
            {
                // Death "star"
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(6284, 0);
                field_E5_state = States::eTransformStarsToDoves_1;
            }
            break;

        case States::eTransformStarsToDoves_1:
            // Has the Death "star" finished animating?
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                const AnimRecord& doveRec = AO::AnimRec(AnimId::Dove_Flying);

                // Yes so magic it into a dove
                auto pDove = relive_new Dove(
                    doveRec.mFrameTableOffset,
                    doveRec.mMaxW,
                    doveRec.mMaxH,
                    doveRec.mResourceId,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(15),
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                if (pDove)
                {
                    if (pDove->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        pDove->mBaseAnimatedWithPhysicsGameObject_XPos += FP_FromInteger(8);
                    }
                    else
                    {
                        pDove->mBaseAnimatedWithPhysicsGameObject_XPos -= FP_FromInteger(8);
                    }

                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);

                    pDove->mBaseAnimatedWithPhysicsGameObject_SpriteScale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
                    if (field_EC_bPlaySound)
                    {
                        SFX_Play_Mono(SoundEffect::AbeDove_18, 0, 0);
                    }
                }
            }
            break;
    }

    mBaseAnimatedWithPhysicsGameObject_XPos += FP_FromInteger(2) * Math_Sine_451110(field_E4_random);
    mBaseAnimatedWithPhysicsGameObject_YPos -= FP_FromInteger(2);
    field_E4_random += 5;
}

DeathBirdParticle::DeathBirdParticle(FP xpos, FP ypos, s32 start, s32 bPlaySound, FP scale)
{
    mBaseGameObjectTypeId = ReliveTypes::eDeathBird;

    const AnimRecord& rec = AO::AnimRec(AnimId::DeathFlare_1);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    if (mBaseGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        mApplyShadows &= ~1u;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;
        mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = scale;
        if (scale <= FP_FromDouble(0.5))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Above_FG1_39;
        }

        mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
        mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
        field_E4_random = Math_NextRandom();
        field_E8_start = start;
        field_E5_state = States::eAnimateDeathFlares_0;
        field_EC_bPlaySound = static_cast<s16>(bPlaySound);
    }
}

} // namespace AO
