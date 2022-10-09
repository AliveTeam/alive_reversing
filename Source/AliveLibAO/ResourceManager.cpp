#include "stdafx_ao.h"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "../relive_lib/Particle.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Psx.hpp"
#include "PsxRender.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "Sys.hpp"
#include "Camera.hpp"
#include "GameAutoPlayer.hpp"

namespace AO {

ALIVE_VAR(1, 0x5009E0, DynamicArrayT<ResourceManager::ResourceManager_FileRecord>*, ObjList_5009E0, nullptr);

ALIVE_VAR(1, 0x9F0E48, u32, sManagedMemoryUsedSize_9F0E48, 0);
ALIVE_VAR(1, 0x9F0E4C, u32, sPeakedManagedMemUsage_9F0E4C, 0);

ALIVE_VAR(1, 0x5076A0, s16, bHideLoadingIcon_5076A0, 0);
ALIVE_VAR(1, 0x5076A4, s32, loading_ticks_5076A4, 0);
ALIVE_VAR(1, 0x9F0E38, s16, sResources_Pending_Loading_9F0E38, 0);
ALIVE_VAR(1, 0x9F0E50, s16, sAllocationFailed_9F0E50, 0);



ALIVE_VAR(1, 0x50EE2C, ResourceManager::ResourceHeapItem*, sFirstLinkedListItem_50EE2C, nullptr);
ALIVE_VAR(1, 0x50EE28, ResourceManager::ResourceHeapItem*, sSecondLinkedListItem_50EE28, nullptr);

const u32 kResHeapSize = 5120000;
ALIVE_ARY(1, 0x50EE38, u8, kResHeapSize, sResourceHeap_50EE38, {}); // Huge 5.4 MB static resource buffer

const u32 kLinkedListArraySize = 375;
ALIVE_ARY(1, 0x50E270, ResourceManager::ResourceHeapItem, kLinkedListArraySize, sResourceLinkedList_50E270, {});

ALIVE_VAR(1, 0x50EE30, u8*, spResourceHeapStart_50EE30, nullptr);
ALIVE_VAR(1, 0x9F0E3C, u8*, spResourceHeapEnd_9F0E3C, nullptr);

// TODO: move to correct location
void Odd_Sleep_48DD90(u32 /*dwMilliseconds*/)
{
    
}

ALIVE_VAR(1, 0x507714, s32, gFilesPending_507714, 0);
ALIVE_VAR(1, 0x50768C, s16, bLoadingAFile_50768C, 0);

class LoadingFile final : public BaseGameObject
{
public:
    LoadingFile(s32 pos, s32 size, TLoaderFn pFn, void* fnArg, Camera* pArray)
        : BaseGameObject(FALSE, 0) // DON'T add to BGE list
    {

        gFilesPending_507714++;
        
        mBaseGameObjectFlags.Set(Options::eSurviveDeathReset_Bit9);
        mBaseGameObjectFlags.Set(Options::eUpdateDuringCamSwap_Bit10);

        field_14_fn = pFn;
        field_18_fn_arg = fnArg;
        field_10_size = size;

        SetType(ReliveTypes::eLoadingFile);
        field_1C_pCamera = pArray;

        PSX_Pos_To_CdLoc_49B340(pos, &field_2A_cdLoc);

        field_28_state = 0;

        gLoadingFiles->Push_Back(this);

        ALIVE_FATAL("Its over for the res manager");
    }

    ~LoadingFile()
    {
      
    }

    void DestroyOnState0_41EA50()
    {
       
    }

    virtual void VUpdate() override
    {

    }

    virtual void VScreenChanged() override
    {
        // Stay alive
    }

    s32 field_10_size;
    TLoaderFn field_14_fn;
    void* field_18_fn_arg;
    Camera* field_1C_pCamera;
    u8** field_20_ppRes;
    void* field_24_readBuffer;
    s16 field_28_state;
    CdlLOC field_2A_cdLoc;
};
ALIVE_ASSERT_SIZEOF(LoadingFile, 0x30);

void Game_ShowLoadingIcon_445EB0()
{
    AnimResource res = ResourceManagerWrapper::LoadAnimation(AnimId::Loading_Icon2);
    auto pParticle = relive_new Particle(FP_FromInteger(0), FP_FromInteger(0), res);
    if (pParticle)
    {
        pParticle->mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        pParticle->mAnim.mFlags.Set(AnimFlags::eBit16_bBlending);

        pParticle->mAnim.mRenderLayer = Layer::eLayer_0;

        PrimHeader* local_ot[42] = {};
        PSX_DRAWENV drawEnv = {};

        PSX_SetDefDrawEnv_495EF0(&drawEnv, 0, 0, 640, 240);
        PSX_PutDrawEnv_495DD0(&drawEnv);
        PSX_DrawSync_496750(0);
        PSX_ClearOTag_496760(local_ot, 42);

        pParticle->mAnim.VRender(320, 220, local_ot, 0, 0);

        PSX_DrawOTag_4969F0(local_ot);
        PSX_DrawSync_496750(0);

        PSX_ClearOTag_496760(local_ot, 42);

        pParticle->mAnim.VRender(320, gPsxDisplay.mHeight + 220, local_ot, 0, 0);

        PSX_DrawOTag_4969F0(local_ot);
        PSX_DrawSync_496750(0);

        PSX_DISPENV dispEnv = {};
        PSX_SetDefDispEnv_4959D0(&dispEnv, 0, 0, 640, 240);
        PSX_PutDispEnv_495CE0(&dispEnv);
        pParticle->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        bHideLoadingIcon_5076A0 = TRUE;
    }
}

void ResourceManager::LoadingLoop_41EAD0(s16)
{
    GetGameAutoPlayer().DisableRecorder();

    /*
    while (gFilesPending_507714 > 0)
    {
        SYS_EventsPump_44FF90();

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
        PSX_VSync_496620(0);

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
