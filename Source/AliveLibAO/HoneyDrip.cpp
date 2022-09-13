#include "stdafx_ao.h"
#include "Function.hpp"
#include "HoneyDrip.hpp"
#include "ResourceManager.hpp"
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

    mAnim.mRenderLayer = Layer::eLayer_BeforeWell_22;

    mSoundPlaying = false;
}

void HoneyDrip::VUpdate()
{
    mXPos += mVelX;
    mYPos += mVelY;

    if (mAnim.mCurrentFrame == 7 && !mSoundPlaying)
    {
        SfxPlayMono(relive::SoundEffects::HoneyDrip, 0);
        mSoundPlaying = true;
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
