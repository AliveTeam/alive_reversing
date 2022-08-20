#include "stdafx_ao.h"
#include "Function.hpp"
#include "HoneyDrip.hpp"
#include "ResourceManager.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"

namespace AO {

HoneyDrip::HoneyDrip(FP xpos, FP ypos)
{
    mRGB.SetRGB(128, 128, 128);

    const AnimRecord rec = AO::AnimRec(AnimId::Honey_Drip);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::Honey_Drip, ppRes);
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
        SfxPlayMono(SoundEffect::HoneyDrip_26, 0, 0);
        mSoundPlaying = true;
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
