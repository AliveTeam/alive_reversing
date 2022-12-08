#include "stdafx_ao.h"
#include "HoneyDrip.hpp"
#include "Sfx.hpp"
#include "../AliveLibAE/stdlib.hpp"

namespace AO {

HoneyDrip::HoneyDrip(FP xpos, FP ypos)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mRGB.SetRGB(128, 128, 128);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Honey_Drip));
    Animation_Init(GetAnimRes(AnimId::Honey_Drip));
    mYPos = ypos;
    mXPos = xpos;

    GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_22);

    mSoundPlaying = false;
}

void HoneyDrip::VUpdate()
{
    mXPos += mVelX;
    mYPos += mVelY;

    if (GetAnimation().GetCurrentFrame() == 7 && !mSoundPlaying)
    {
        SfxPlayMono(relive::SoundEffects::HoneyDrip, 0);
        mSoundPlaying = true;
    }

    if (GetAnimation().GetIsLastFrame())
    {
        SetDead(true);
    }
}

} // namespace AO
