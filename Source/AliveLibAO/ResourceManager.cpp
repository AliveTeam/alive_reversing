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
        local_ot.Clear();
 
        pParticle->GetAnimation().VRender(320, 220, local_ot, 0, 0);
        PSX_DrawOTag(local_ot);

        PSX_PutDispEnv_4F5890();
        pParticle->SetDead(true);
        ResourceManagerWrapper::bHideLoadingIcon = true;
    }
}

//void ResourceManager::LoadingLoop(s16)
//{
    //GetGameAutoPlayer().DisableRecorder();

    /*
    while (gFilesPending_507714 > 0)
    {
        SYS_EventsPump();

        for (s32 i = 0; i < gLoadingFiles->Size(); i++)
        {
            BaseGameObject* pObjIter = gLoadingFiles->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            if (!pObjIter->GetDead())
            {
                pObjIter->VUpdate();
            }

            if (pObjIter->GetDead())
            {
                i = gLoadingFiles->RemoveAt(i);
                relive_delete pObjIter;
            }
        }

        Odd_Sleep_48DD90(16u);
        PSX_VSync(0);

        loading_ticks++;

        if (bShowLoadingIcon)
        {
            if (!bHideLoadingIcon && loading_ticks > 180)
            {
                Game_ShowLoadingIcon();
            }
        }
    }
    */

    //GetGameAutoPlayer().EnableRecorder();
//}

} // namespace AO
