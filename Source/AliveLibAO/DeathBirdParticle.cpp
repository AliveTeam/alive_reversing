#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "DeathBirdParticle.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/GameObjects/Dove.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "../relive_lib/FixedPoint.hpp"

namespace AO {

void DeathBirdParticle::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::DeathFlare_2));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::DeathFlare_1));
}

DeathBirdParticle::DeathBirdParticle(FP xpos, FP ypos, s32 startTimer, bool playSound, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0),
      mStartTimer(startTimer),
      mPlaySound(playSound)
{
    SetType(ReliveTypes::eDeathBird);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::DeathFlare_1));

    if (GetListAddFailed())
    {
        SetDead(true);
    }
    else
    {
        SetApplyShadowZoneColour(false);
        GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);
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
            if (GetAnimation().GetIsLastFrame())
            {
                // Yes so magic it into a dove
                auto pDove = relive_new Dove(
                    AnimId::Dove_Flying,
                    mXPos,
                    mYPos - FP_FromInteger(15),
                    GetSpriteScale());

                if (pDove->GetAnimation().GetFlipX())
                {
                    pDove->mXPos += FP_FromInteger(8);
                }
                else
                {
                    pDove->mXPos -= FP_FromInteger(8);
                }

                SetDead(true);
                pDove->SetSpriteScale(GetSpriteScale());

                if (mPlaySound)
                {
                    SfxPlayMono(relive::SoundEffects::AbeDove, 0);
                }
            }
            break;
    }

    mXPos += FP_FromInteger(2) * Math_Sine(mRandom);
    mYPos -= FP_FromInteger(2);
    mRandom += 5;
}

} // namespace AO
