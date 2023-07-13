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

        // This was doing something odd with OT index.. I think its trying to simulate double buffering by
        // using other parts of the OT while another part is drawn, but it was bugged because it cleared the other anyway
        // on PC it seems fine to just always start at zero.
        pParticle->GetAnimation().VRender(320, 220, dispBuffer.mOrderingTable, 0, 0);
        PSX_DrawOTag(dispBuffer.mOrderingTable);

        pParticle->GetAnimation().VRender(320, 640 - 164, dispBuffer.mOrderingTable, 0, 0);
        PSX_DrawOTag(dispBuffer.mOrderingTable);

        PSX_SetDefDispEnv_4F55A0(&dispBuffer.mDisplayEnv);
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

//void ResourceManager::LoadingLoop(s16 )
//{
    //GetGameAutoPlayer().DisableRecorder();

    /*
    while (!field_20_files_pending_loading.IsEmpty())
    {
        SYS_EventsPump();
        VUpdate(); // vLoadFile_StateMachine_464A70 - process loading of files
        PSX_VSync(0);
        const s32 ticks = loading_ticks++ + 1;
        if (bShowLoadingIcon && !bHideLoadingIcon && ticks > 180)
        {
            // Render everything in the ordering table including the loading icon
            Game_ShowLoadingIcon();
        }
    }*/

    //GetGameAutoPlayer().EnableRecorder();
//}


void ResourceManager::VUpdate()
{
    // Empty
}

void ResourceManager::VScreenChanged()
{
    // Empty
}
