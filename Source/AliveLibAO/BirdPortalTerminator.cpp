#include "stdafx_ao.h"
#include "BirdPortalTerminator.hpp"

namespace AO {

void BirdPortalTerminator::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BirdPortal_TerminatorIdle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BirdPortal_TerminatorShrink));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BirdPortal_TerminatorGrow));
}

BirdPortalTerminator::BirdPortalTerminator(FP xpos, FP ypos, FP scale, relive::Path_BirdPortal::PortalType /*portalType*/)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eClawOrBirdPortalTerminator);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::BirdPortal_TerminatorGrow));

    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);
    SetSpriteScale(scale);
    if (scale == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_Above_FG1_39);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_Above_FG1_Half_20);
    }

    SetApplyShadowZoneColour(false);

    mYPos = ypos;
    mXPos = xpos;

    mRGB.SetRGB(255, 128, 64);
}

void BirdPortalTerminator::VScreenChanged()
{
    // Staying alive
}

void BirdPortalTerminator::Fadeout()
{
    const s16 r = mRGB.r;
    mRGB.r = (r >> 1) + (r >> 2);

    const s16 g = mRGB.g;
    mRGB.g = (g >> 1) + (g >> 2);

    const s16 b = mRGB.b;
    mRGB.b = (b >> 1) + (b >> 2);
}

} // namespace AO
