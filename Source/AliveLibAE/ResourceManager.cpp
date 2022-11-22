#include "stdafx.h"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Particle.hpp"
#include "PsxRender.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Sys.hpp"
#include "GameAutoPlayer.hpp"
#include "../relive_lib/Camera.hpp"

ResourceManager* pResourceManager_5C1BB0 = nullptr;

u32 sManagedMemoryUsedSize_AB4A04 = 0;
u32 sPeakedManagedMemUsage_AB4A08 = 0;

s16 bHideLoadingIcon_5C1BAA = 0;
s32 loading_ticks_5C1BAC = 0;
s16 sResources_Pending_Loading_AB49F4 = 0;
s16 sAllocationFailed_AB4A0C = 0;

ResourceManager::ResourceHeapItem* sFirstLinkedListItem_5D29EC = nullptr;
ResourceManager::ResourceHeapItem* sSecondLinkedListItem_5D29E8 = nullptr;

const u32 kResHeapSize = 5120000;
u8 sResourceHeap_5D29F4[kResHeapSize] = {}; // Huge 5.4 MB static resource buffer

const u32 kLinkedListArraySize = 375;
ResourceManager::ResourceHeapItem sResourceLinkedList_5D1E30[kLinkedListArraySize] = {};

u8* spResourceHeapEnd_AB49F8 = nullptr;

// TODO: Move to own file
void sub_465BC0(s32 /*a1*/)
{
    
}

// TODO: Move to own file
void Game_ShowLoadingIcon_482D80()
{
    AnimResource ppLoadingAnimRes = ResourceManagerWrapper::LoadAnimation(AnimId::Loading_Icon2);
    PSX_Display_Buffer dispBuffer = {};

    Particle* pParticle = relive_new Particle(FP_FromInteger(0), FP_FromInteger(0), ppLoadingAnimRes);

    // TODO: May need to clear all other low word bits ?
    pParticle->GetAnimation().mFlags.Clear(AnimFlags::eSemiTrans);
    pParticle->GetAnimation().mFlags.Set(AnimFlags::eBlending);

    pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_0);

    // This was doing something odd with OT index.. I think its trying to simulate f64 buffering by
    // using other parts of the OT while another part is drawn, but it was bugged because it cleared the other anyway
    // on PC it seems fine to just always start at zero.
    PSX_ClearOTag(dispBuffer.mOrderingTable, 43);
    pParticle->GetAnimation().VRender(320, 220, dispBuffer.mOrderingTable, 0, 0);
    PSX_DrawOTag(dispBuffer.mOrderingTable);

    PSX_ClearOTag(dispBuffer.mOrderingTable, 43);
    pParticle->GetAnimation().VRender(320, 640 - 164, dispBuffer.mOrderingTable, 0, 0);
    PSX_DrawOTag(dispBuffer.mOrderingTable);

    PSX_SetDefDispEnv_4F55A0(&dispBuffer.mDisplayEnv);
    PSX_PutDispEnv_4F5890();
    pParticle->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    bHideLoadingIcon_5C1BAA = 1;
}

ResourceManager::ResourceManager()
    : BaseGameObject(true, 0)
    , field_20_files_pending_loading(3)
    , field_48_dArray(3)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    mBaseGameObjectFlags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);

    SetType(ReliveTypes::eResourceManager);
    field_2C_pFileItem = nullptr;
    field_30_start_sector = 0;
    field_34_num_sectors = 0;
    field_38_ppRes = {};
    field_3C_pLoadingHeader = nullptr;
    field_42_state = State_Wait_For_Load_Request;
    field_40_seek_attempts = 0;
}

ResourceManager::~ResourceManager()
{
    
}


void ResourceManager::LoadingLoop_465590(s16 )
{
    GetGameAutoPlayer().DisableRecorder();

    /*
    while (!field_20_files_pending_loading.IsEmpty())
    {
        SYS_EventsPump();
        VUpdate(); // vLoadFile_StateMachine_464A70 - process loading of files
        PSX_VSync_4F6170(0);
        const s32 ticks = loading_ticks_5C1BAC++ + 1;
        if (bShowLoadingIcon && !bHideLoadingIcon_5C1BAA && ticks > 180)
        {
            // Render everything in the ordering table including the loading icon
            Game_ShowLoadingIcon_482D80();
        }
    }*/

    GetGameAutoPlayer().EnableRecorder();
}


void ResourceManager::VUpdate()
{

}

void ResourceManager::VScreenChanged_464EC0()
{
    // Empty
}

void ResourceManager::VScreenChanged()
{
    VScreenChanged_464EC0();
}


s32 ResourceManager::SEQ_HashName_49BE30(const char_type* seqFileName)
{
    return ResourceManagerWrapper::SEQ_HashName(seqFileName);
}
