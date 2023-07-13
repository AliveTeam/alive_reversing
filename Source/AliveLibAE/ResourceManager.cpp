#include "stdafx.h"
#include "ResourceManager.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Psx.hpp"
#include "stdlib.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "PsxRender.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/BaseGameAutoPlayer.hpp"

void Game_ShowLoadingIcon()
{
    AnimResource ppLoadingAnimRes = ResourceManagerWrapper::LoadAnimation(AnimId::Loading_Icon2);
    PSX_Display_Buffer dispBuffer = {};

    auto pParticle = relive_new Particle(FP_FromInteger(0), FP_FromInteger(0), ppLoadingAnimRes);
    if (pParticle)
    {
        pParticle->GetAnimation().SetSemiTrans(false);
        pParticle->GetAnimation().SetBlending(true);

        pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_0);

        pParticle->GetAnimation().VRender(320, 220, dispBuffer.mOrderingTable, 0, 0);
        PSX_DrawOTag(dispBuffer.mOrderingTable);

        PSX_PutDispEnv_4F5890();
        pParticle->SetDead(true);
        ResourceManagerWrapper::bHideLoadingIcon = 1;
    }
}

ResourceManager::ResourceManager()
    : BaseGameObject(true, 0)
{
    SetSurviveDeathReset(true);
    SetUpdateDuringCamSwap(true);

    SetType(ReliveTypes::eResourceManager);
}

void ResourceManager::VUpdate()
{
    // Empty
}

void ResourceManager::VScreenChanged()
{
    // Empty
}
