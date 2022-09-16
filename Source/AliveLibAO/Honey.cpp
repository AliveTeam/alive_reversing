#include "stdafx_ao.h"
#include "Function.hpp"
#include "Honey.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "../AliveLibAE/stdlib.hpp"

namespace AO {

void Honey::VUpdate()
{
    // Empty
}

Honey::Honey(FP xpos, FP ypos)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eHoney);

    mRGB.SetRGB(128, 128, 128);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Honey));
    Animation_Init(GetAnimRes(AnimId::Honey));

    mYPos = ypos;
    mXPos = xpos;
}

void Honey::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

} // namespace AO
