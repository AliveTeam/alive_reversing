#include "stdafx_ao.h"
#include "Function.hpp"
#include "HoneyDrip.hpp"
#include "ResourceManager.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"

namespace AO {

HoneyDrip::HoneyDrip(FP xpos, FP ypos)
{
    mBaseAnimatedWithPhysicsGameObject_RGB.b = 128;
    mBaseAnimatedWithPhysicsGameObject_RGB.g = 128;
    mBaseAnimatedWithPhysicsGameObject_RGB.r = 128;

    const AnimRecord rec = AO::AnimRec(AnimId::Honey_Drip);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;

    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeWell_22;

    field_E4_bSoundPlaying = FALSE;
}

void HoneyDrip::VUpdate()
{
    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 7 && !field_E4_bSoundPlaying)
    {
        SFX_Play_Mono(SoundEffect::HoneyDrip_26, 0, 0);
        field_E4_bSoundPlaying = TRUE;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
