#include "stdafx_ao.h"
#include "Function.hpp"
#include "DeathBirdParticle.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Dove.hpp"
#include "Math.hpp"
#include "Sfx.hpp"

namespace AO {

void DeathBirdParticle::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::DeathFlare_2));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::DeathFlare_1));
}

DeathBirdParticle::DeathBirdParticle(FP xpos, FP ypos, s32 startTimer, bool playSound, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eDeathBird);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::DeathFlare_1));

    if (mBaseGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
        GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
        SetSpriteScale(scale);
        GetAnimation().SetSpriteScale(scale);

        if (scale <= FP_FromDouble(0.5))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
        }
        else
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_Above_FG1_39);
        }

        mXPos = xpos;
        mYPos = ypos;
        mRandom = Math_NextRandom();
        mStartTimer = startTimer;
        mState = States::eAnimateDeathFlares_0;
        mPlaySound = playSound;
    }
}

void DeathBirdParticle::VUpdate()
{
    switch (mState)
    {
        case States::eAnimateDeathFlares_0:
            if (static_cast<s32>(sGnFrame) > mStartTimer)
            {
                // Death "star"
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::DeathFlare_2));
                mState = States::eTransformStarsToDoves_1;
            }
            break;

        case States::eTransformStarsToDoves_1:
            // Has the Death "star" finished animating?
            if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                // Yes so magic it into a dove
                auto pDove = relive_new Dove(
                    AnimId::Dove_Flying,
                    mXPos,
                    mYPos - FP_FromInteger(15),
                    GetSpriteScale());
                if (pDove)
                {
                    if (pDove->GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                    {
                        pDove->mXPos += FP_FromInteger(8);
                    }
                    else
                    {
                        pDove->mXPos -= FP_FromInteger(8);
                    }

                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);

                    pDove->SetSpriteScale(GetSpriteScale());
                    if (mPlaySound)
                    {
                        SfxPlayMono(relive::SoundEffects::AbeDove, 0);
                    }
                }
            }
            break;
    }

    mXPos += FP_FromInteger(2) * Math_Sine(mRandom);
    mYPos -= FP_FromInteger(2);
    mRandom += 5;
}

} // namespace AO
