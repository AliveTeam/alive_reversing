#include "stdafx_ao.h"
#include "Function.hpp"
#include "HoneyDrip.hpp"
#include "ResourceManager.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"

namespace AO {

HoneyDrip::HoneyDrip(FP xpos, FP ypos)
{
    field_C4_b = 128;
    field_C2_g = 128;
    field_C0_r = 128;

    const AnimRecord& rec = AO::AnimRec(AnimId::Honey_Drip);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;

    field_10_anim.mRenderLayer = Layer::eLayer_BeforeWell_22;

    field_E4_bSoundPlaying = FALSE;
}

void HoneyDrip::VUpdate()
{
    mBaseAnimatedWithPhysicsGameObject_XPos += field_B4_velx;
    mBaseAnimatedWithPhysicsGameObject_YPos += field_B8_vely;

    if (field_10_anim.field_92_current_frame == 7 && !field_E4_bSoundPlaying)
    {
        SFX_Play_Mono(SoundEffect::HoneyDrip_26, 0, 0);
        field_E4_bSoundPlaying = TRUE;
    }

    if (field_10_anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
