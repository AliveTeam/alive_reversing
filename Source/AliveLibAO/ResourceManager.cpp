#include "stdafx_ao.h"
#include "ResourceManager.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Psx.hpp"
#include "../AliveLibAE/PsxRender.hpp"
#include "GameAutoPlayer.hpp"

namespace AO {

void Game_ShowLoadingIcon()
{
    AnimResource res = ResourceManagerWrapper::LoadAnimation(AnimId::Loading_Icon2);
    auto pParticle = relive_new Particle(FP_FromInteger(0), FP_FromInteger(0), res);
    if (pParticle)
    {
        pParticle->GetAnimation().SetSemiTrans(false);
        pParticle->GetAnimation().SetBlending(true);

        pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_0);

        OrderingTable local_ot;

        pParticle->GetAnimation().VRender(320, 220, local_ot, 0, 0);
        PSX_DrawOTag(local_ot);

        PSX_PutDispEnv_4F5890();
        pParticle->SetDead(true);
        ResourceManagerWrapper::bHideLoadingIcon = true;
    }
}

} // namespace AO
