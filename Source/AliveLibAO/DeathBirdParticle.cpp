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
                mAnim.Set_Animation_Data(AnimId::DeathFlare_2, nullptr);
                field_E5_state = States::eTransformStarsToDoves_1;
            }
            break;

        case States::eTransformStarsToDoves_1:
            // Has the Death "star" finished animating?
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                // Yes so magic it into a dove
                auto pDove = relive_new Dove(
                    AnimId::Dove_Flying,
                    mXPos,
                    mYPos - FP_FromInteger(15),
                    mSpriteScale);
                if (pDove)
                {
                    if (pDove->mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        pDove->mXPos += FP_FromInteger(8);
                    }
                    else
                    {
                        pDove->mXPos -= FP_FromInteger(8);
                    }

                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);

                    pDove->mSpriteScale = mSpriteScale;
                    if (field_EC_bPlaySound)
                    {
                        SfxPlayMono(SoundEffect::AbeDove_18, 0, 0);
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
    mBaseGameObjectTypeId = ReliveTypes::eDeathBird;

    const AnimRecord rec = AO::AnimRec(AnimId::DeathFlare_1);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::DeathFlare_1, ppRes);

    if (mBaseGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
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
