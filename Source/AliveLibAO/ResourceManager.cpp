#include "stdafx_ao.h"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "../relive_lib/Particle.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../AliveLibAE/Psx.hpp"
#include "../AliveLibAE/PsxRender.hpp"
#include "GameAutoPlayer.hpp"

namespace AO {

s16 bHideLoadingIcon_5076A0 = 0;
s32 loading_ticks_5076A4 = 0;

void Game_ShowLoadingIcon_445EB0()
{
    AnimResource res = ResourceManagerWrapper::LoadAnimation(AnimId::Loading_Icon2);
    auto pParticle = relive_new Particle(FP_FromInteger(0), FP_FromInteger(0), res);
    if (pParticle)
    {
        pParticle->GetAnimation().mFlags.Clear(AnimFlags::eSemiTrans);
        pParticle->GetAnimation().mFlags.Set(AnimFlags::eBlending);

        pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_0);

        PrimHeader* local_ot[42] = {};
        PSX_ClearOTag(local_ot, 42);

        pParticle->GetAnimation().VRender(320, 220, local_ot, 0, 0);

        PSX_DrawOTag(local_ot);

        PSX_ClearOTag(local_ot, 42);

        pParticle->GetAnimation().VRender(320, gPsxDisplay.mHeight + 220, local_ot, 0, 0);

        PSX_DrawOTag(local_ot);

        PSX_PutDispEnv_4F5890();
        pParticle->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        bHideLoadingIcon_5076A0 = true;
    }
}

void ResourceManager::LoadingLoop(s16)
{
    GetGameAutoPlayer().DisableRecorder();

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

            if (!pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                pObjIter->VUpdate();
            }

            if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                i = gLoadingFiles->RemoveAt(i);
                relive_delete pObjIter;
            }
        }

        Odd_Sleep_48DD90(16u);
        PSX_VSync_4F6170(0);

        loading_ticks_5076A4++;

        if (bShowLoadingIcon)
        {
            if (!bHideLoadingIcon_5076A0 && loading_ticks_5076A4 > 180)
            {
                Game_ShowLoadingIcon_445EB0();
            }
        }
    }
    */

    GetGameAutoPlayer().EnableRecorder();
}


s32 ResourceManager::SEQ_HashName_454EA0(const char_type* seqFileName)
{
    return ResourceManagerWrapper::SEQ_HashName(seqFileName);
}

} // namespace AO
