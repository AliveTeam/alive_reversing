#include "stdafx_ao.h"
#include "Honey.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Path.hpp"

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
    SetDead(true);
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

} // namespace AO
