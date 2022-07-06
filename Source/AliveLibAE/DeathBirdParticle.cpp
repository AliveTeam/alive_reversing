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
    SetType(ReliveTypes::eDeathBird);

    const AnimRecord& rec = AnimRec(AnimId::DeathFlare_1);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
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
            if (static_cast<s32>(sGnFrame) > field_F8_start)
            {
                // Death "star"
                mAnim.Set_Animation_Data(AnimId::DeathFlare_2, nullptr);
                field_F5_state = States::eTransformStarsToDoves_1;
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

                if (pDove->mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    pDove->mXPos += FP_FromInteger(8);
                }
                else
                {
                    pDove->mXPos -= FP_FromInteger(8);
                }

                pDove->mSpriteScale = mSpriteScale;
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);

                if (field_FC_bPlaySound)
                {
                    SfxPlayMono(SoundEffect::AbeDove_15, 0);
                }
            }
            break;
    }

    mXPos += FP_FromInteger(2) * Math_Sine_496DD0(field_F4_random);
    mYPos -= FP_FromInteger(2);
    field_F4_random += 5;
}
