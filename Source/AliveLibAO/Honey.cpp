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
    mBaseGameObjectTypeId = ReliveTypes::eHoney;

    mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(128, 128, 128);

    const AnimRecord rec = AO::AnimRec(AnimId::Honey);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::Honey, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
}

void Honey::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    gMap.TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
}

} // namespace AO
