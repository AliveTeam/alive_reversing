#include "stdafx_ao.h"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "Particle.hpp"
#include "stdlib.hpp"
#include "PsxDisplay.hpp"
#include "Psx.hpp"
#include "PsxRender.hpp"

START_NS_AO

ALIVE_VAR(1, 0x5076A0, short, bHideLoadingIcon_5076A0, 0);
ALIVE_VAR(1, 0x5076A4, int, loading_ticks_5076A4, 0);

void CC ResourceManager::CancelPendingResourcesFor_41EA60(BaseGameObject* /*pObj*/)
{
    NOT_IMPLEMENTED();
}

void CC ResourceManager::Reclaim_Memory_455660(DWORD /*sizeToReclaim*/)
{
    NOT_IMPLEMENTED();
}

__int16 CC ResourceManager::FreeResource_455550(BYTE** /*ppRes*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

void CC ResourceManager::LoadResource_446C90(const char* /*pFileName*/, int /*type*/, int /*resourceId*/, __int16 /*loadMode*/, __int16 /*bDontLoad*/)
{
    NOT_IMPLEMENTED();
}

ResourceManager::Header* CC ResourceManager::Get_Header_455620(BYTE** ppRes)
{
    return reinterpret_cast<Header*>((*ppRes - sizeof(Header)));
}

EXPORT void CC ResourceManager::Free_Resources_For_Camera_447170(Camera* /*pCamera*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC ResourceManager::Free_Resource_Of_Type_455810(int /*type*/)
{
    NOT_IMPLEMENTED();
}

EXPORT __int16 CC ResourceManager::LoadResourceFile_455270(const char* /*filename*/, Camera* /*pCam*/, int /*allocMethod*/)
{
    NOT_IMPLEMENTED();
    return 0;
}


EXPORT BYTE** CC ResourceManager::Allocate_New_Locked_Resource_454F80(int /*type*/, int /*id*/, int /*size*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

EXPORT void CC ResourceManager::Set_Header_Flags_4557D0(BYTE** /*ppRes*/, __int16 /*flags*/)
{
    NOT_IMPLEMENTED();
}

EXPORT ResourceManager::ResourceManager_FileRecord* CC ResourceManager::LoadResourceFile_4551E0(const char* /*pFileName*/, TLoaderFn /*fnOnLoad*/, Camera* /*pCamera1*/, Camera* /*pCamera2*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

EXPORT void CC ResourceManager::LoadingLoop_41EAD0(__int16 /*bShowLoadingIcon*/)
{
    NOT_IMPLEMENTED();
}

BYTE** CC ResourceManager::GetLoadedResource_4554F0(int /*type*/, int /*resourceId*/, __int16 /*addUseCount*/, __int16 /*bLock*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void CC Game_ShowLoadingIcon_445EB0()
{
    BYTE** ppLoadingAnimRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 8010, 1, 0);
    if (ppLoadingAnimRes)
    {
        auto pParticle = ao_new<Particle>();
        if (pParticle)
        {
            pParticle->ctor_478880(FP_FromInteger(0), FP_FromInteger(0), 652, 50, 38, ppLoadingAnimRes);
        }

        pParticle->field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        pParticle->field_10_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

        pParticle->field_10_anim.field_C_layer = 0;

        int* local_ot[42] = {};
        PSX_DRAWENV drawEnv = {};

        PSX_SetDefDrawEnv_495EF0(&drawEnv, 0, 0, 640, 240);
        PSX_PutDrawEnv_495DD0(&drawEnv);
        PSX_DrawSync_496750(0);
        PSX_ClearOTag_496760(local_ot, 42);

        pParticle->field_10_anim.vRender(320, 220, local_ot, 0, 0);

        PSX_DrawOTag_4969F0(local_ot);
        PSX_DrawSync_496750(0);

        PSX_ClearOTag_496760(local_ot, 42);

        pParticle->field_10_anim.vRender(320, gPsxDisplay_504C78.field_2_height + 220, local_ot, 0, 0);

        PSX_DrawOTag_4969F0(local_ot);
        PSX_DrawSync_496750(0);

        PSX_DISPENV dispEnv = {};
        PSX_SetDefDispEnv_4959D0(&dispEnv, 0, 0, 640, 240);
        PSX_PutDispEnv_495CE0(&dispEnv);
        pParticle->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        bHideLoadingIcon_5076A0 = TRUE;
    }
}

void CC ResourceManager::LoadResourcesFromList_446E80(const char* /*pFileName*/, ResourcesToLoadList* /*list*/, __int16 /*loadMode*/, __int16)
{
    NOT_IMPLEMENTED();
}

END_NS_AO



