#include "stdafx_ao.h"
#include "Function.hpp"
#include "Honey.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

namespace AO {

void Honey::VUpdate()
{
    // Empty
}

Honey::Honey(FP xpos, FP ypos)
{
    mBaseGameObjectTypeId = Types::eHoney_47;

    field_C4_b = 128;
    field_C2_g = 128;
    field_C0_r = 128;

    const AnimRecord& rec = AO::AnimRec(AnimId::Honey);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_AC_ypos = ypos;
    field_A8_xpos = xpos;
}

void Honey::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    gMap.TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
}

} // namespace AO
