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
                mAnim.Set_Animation_Data(6284, 0);
                field_E5_state = States::eTransformStarsToDoves_1;
            }
            break;

        case States::eTransformStarsToDoves_1:
            // Has the Death "star" finished animating?
            if (mAnim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                const AnimRecord& doveRec = AO::AnimRec(AnimId::Dove_Flying);

                // Yes so magic it into a dove
                auto pDove = ao_new<Dove>(
                    doveRec.mFrameTableOffset,
                    doveRec.mMaxW,
                    doveRec.mMaxH,
                    doveRec.mResourceId,
                    mXPos,
                    mYPos - FP_FromInteger(15),
                    mSpriteScale);
                if (pDove)
                {
                    if (pDove->mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        pDove->mXPos += FP_FromInteger(8);
                    }
                    else
                    {
                        pDove->mXPos -= FP_FromInteger(8);
                    }

                    mGameObjectFlags.Set(BaseGameObject::eDead);

                    pDove->mSpriteScale = mSpriteScale;
                    if (field_EC_bPlaySound)
                    {
                        SFX_Play_Mono(SoundEffect::AbeDove_18, 0, 0);
                    }
                }
            }
            break;
    }

    mXPos += FP_FromInteger(2) * Math_Sine_451110(field_E4_random);
    mYPos -= FP_FromInteger(2);
    field_E4_random += 5;
}

DeathBirdParticle::DeathBirdParticle(FP xpos, FP ypos, s32 start, s32 bPlaySound, FP scale)
{
    mTypeId = Types::eDeathBird_38;

    const AnimRecord& rec = AO::AnimRec(AnimId::DeathFlare_1);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    if (mGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        mApplyShadows &= ~1u;
        mAnim.mRenderMode = TPageAbr::eBlend_1;
        mSpriteScale = scale;
        mAnim.field_14_scale = scale;
        if (scale <= FP_FromDouble(0.5))
        {
            mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }
        else
        {
            mAnim.mRenderLayer = Layer::eLayer_Above_FG1_39;
        }

        mXPos = xpos;
        mYPos = ypos;
        field_E4_random = Math_NextRandom();
        field_E8_start = start;
        field_E5_state = States::eAnimateDeathFlares_0;
        field_EC_bPlaySound = static_cast<s16>(bPlaySound);
    }
}

} // namespace AO
