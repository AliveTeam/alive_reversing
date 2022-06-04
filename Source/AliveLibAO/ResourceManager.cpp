#include "stdafx_ao.h"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "Particle.hpp"
#include "stdlib.hpp"
#include "PsxDisplay.hpp"
#include "Psx.hpp"
#include "PsxRender.hpp"
#include "ScreenManager.hpp"
#include "Game.hpp"
#include "LvlArchive.hpp"
#include "Map.hpp"
#include "Sys.hpp"

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
        : BaseGameObject(1)
    {
        gFilesPending_507714++;

        mBaseGameObjectFlags.Set(Options::eSurviveDeathReset_Bit9);
        mBaseGameObjectFlags.Set(Options::eUpdateDuringCamSwap_Bit10);

        field_14_fn = pFn;
        field_18_fn_arg = fnArg;
        field_10_size = size;

        mBaseGameObjectTypeId = ReliveTypes::eLoadingFile;
        field_1C_pCamera = pArray;

        PSX_Pos_To_CdLoc_49B340(pos, &field_2A_cdLoc);

        field_28_state = 0;
    }

    ~LoadingFile()
    {
        gFilesPending_507714--;

        if (field_28_state != 0)
        {
            if (field_28_state != 7)
            {
                bLoadingAFile_50768C = 0;
            }
        }
    }

    void DestroyOnState0_41EA50()
    {
        if (field_28_state == 0)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }

    virtual void VUpdate() override
    {
        VUpdate_41E900();
    }

    void VUpdate_41E900()
    {
        switch (field_28_state)
        {
            case 0:
                if (!bLoadingAFile_50768C)
                {
                    field_20_ppRes = ResourceManager::Allocate_New_Block_454FE0(field_10_size << 11, ResourceManager::eFirstMatching);
                    if (field_20_ppRes)
                    {
                        ResourceManager::Header* pHeader = ResourceManager::Get_Header_455620(field_20_ppRes);
                        field_24_readBuffer = pHeader;
                        pHeader->field_8_type = ResourceManager::Resource_Pend;
                        ResourceManager::Increment_Pending_Count_4557A0();
                        bLoadingAFile_50768C = 1;
                        field_28_state = 1;
                    }
                    else
                    {
                        ResourceManager::Reclaim_Memory_455660(200000u);
                    }
                }
                break;

            case 1:
                if (PSX_CD_File_Seek_49B670(2, &field_2A_cdLoc))
                {
                    field_28_state = 2;
                }
                break;

            case 2:
                if (PSX_CD_File_Read_49B8B0(field_10_size, field_24_readBuffer))
                {
                    field_28_state = 3;
                    const s32 ioRet = PSX_CD_FileIOWait_49B900(1);
                    if (ioRet <= 0)
                    {
                        field_28_state = ioRet != -1 ? 4 : 1;
                    }
                    break;
                }
                break;

            case 3:
            {
                const s32 ioRet = PSX_CD_FileIOWait_49B900(1);
                if (ioRet <= 0)
                {
                    field_28_state = ioRet != -1 ? 4 : 1;
                }
                break;
            }

            case 4:
                ResourceManager::Move_Resources_To_DArray_455430(
                    field_20_ppRes,
                    &field_1C_pCamera->field_0_array);
                field_28_state = 5;
                break;

            case 5:
                if (field_14_fn)
                {
                    field_14_fn(field_18_fn_arg);
                }
                field_28_state = 6;
                bLoadingAFile_50768C = 0;
                break;

            case 6:
                ResourceManager::Decrement_Pending_Count_4557B0();
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                field_28_state = 7;
                break;

            default:
                return;
        }
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
    s16 field_2E_pad;
};
ALIVE_ASSERT_SIZEOF(LoadingFile, 0x30);

void Game_ShowLoadingIcon_445EB0()
{
    const AnimRecord& rec = AO::AnimRec(AnimId::Loading_Icon2);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    if (ppRes)
    {
        auto pParticle = ao_new<Particle>(FP_FromInteger(0), FP_FromInteger(0), rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes);
        if (pParticle)
        {
            pParticle->field_10_anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
            pParticle->field_10_anim.mAnimFlags.Set(AnimFlags::eBit16_bBlending);

            pParticle->field_10_anim.mRenderLayer = Layer::eLayer_0;

            PrimHeader* local_ot[42] = {};
            PSX_DRAWENV drawEnv = {};

            PSX_SetDefDrawEnv_495EF0(&drawEnv, 0, 0, 640, 240);
            PSX_PutDrawEnv_495DD0(&drawEnv);
            PSX_DrawSync_496750(0);
            PSX_ClearOTag_496760(local_ot, 42);

            pParticle->field_10_anim.VRender(320, 220, local_ot, 0, 0);

            PSX_DrawOTag_4969F0(local_ot);
            PSX_DrawSync_496750(0);

            PSX_ClearOTag_496760(local_ot, 42);

            pParticle->field_10_anim.VRender(320, gPsxDisplay_504C78.field_2_height + 220, local_ot, 0, 0);

            PSX_DrawOTag_4969F0(local_ot);
            PSX_DrawSync_496750(0);

            PSX_DISPENV dispEnv = {};
            PSX_SetDefDispEnv_4959D0(&dispEnv, 0, 0, 640, 240);
            PSX_PutDispEnv_495CE0(&dispEnv);
            pParticle->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            bHideLoadingIcon_5076A0 = TRUE;
        }
    }
}


void ResourceManager::On_Loaded_446C10(ResourceManager_FileRecord* pLoaded)
{
    for (s32 i = 0; i < pLoaded->field_10_file_sections_dArray.Size(); i++)
    {
        ResourceManager_FilePartRecord* pFilePart = pLoaded->field_10_file_sections_dArray.ItemAt(i);
        if (!pFilePart)
        {
            break;
        }

        u8** ppRes = ResourceManager::GetLoadedResource_4554F0(
            pFilePart->field_0_type,
            pFilePart->field_4_res_id,
            1,
            0);

        if (ppRes)
        {
            pFilePart->field_8_pCamera->field_0_array.Push_Back(ppRes);
        }

        ao_delete_free_447540(pFilePart);
    }

    // pLoaded is done with now, remove it
    ObjList_5009E0->Remove_Item(pLoaded);

    relive_delete pLoaded;
}

void ResourceManager::LoadResource_446C90(const char_type* pFileName, u32 type, u32 resourceId, LoadMode loadMode, s16 bDontLoad)
{
    if (bDontLoad)
    {
        return;
    }

    u8** ppExistingRes = ResourceManager::GetLoadedResource_4554F0(type, resourceId, 1, 0);
    if (ppExistingRes)
    {
        sCameraBeingLoaded_507C98->field_0_array.Push_Back(ppExistingRes);
        return;
    }

    if (loadMode == LoadMode::LoadResourceFromList_1)
    {
        for (s32 i = 0; i < ObjList_5009E0->Size(); i++)
        {
            ResourceManager_FileRecord* pExistingFileRec = ObjList_5009E0->ItemAt(i);
            if (!pExistingFileRec)
            {
                break;
            }

            ResourcesToLoadList* pListToLoad = pExistingFileRec->field_4_pResourcesToLoadList;
            bool found = false;
            if (pListToLoad)
            {
                if (pListToLoad->field_0_count > 0)
                {
                    for (s32 j = 0; j < pListToLoad->field_0_count; j++)
                    {
                        if (type == pListToLoad->field_4_items[j].field_0_type && resourceId == pListToLoad->field_4_items[j].field_4_res_id)
                        {
                            found = true;
                            break;
                        }
                    }
                }
            }
            else if (type == pExistingFileRec->field_8_type && resourceId == pExistingFileRec->field_C_resourceId)
            {
                found = true;
            }

            if (found)
            {
                auto pFilePart = ao_new<ResourceManager_FilePartRecord>();
                pFilePart->field_8_pCamera = sCameraBeingLoaded_507C98;
                pFilePart->field_0_type = type;
                pFilePart->field_4_res_id = resourceId;
                pExistingFileRec->field_10_file_sections_dArray.Push_Back(pFilePart);
                return;
            }
        }

        auto pFileRec = ao_new<ResourceManager_FileRecord>();
        if (pFileRec)
        {
            pFileRec->field_0_fileName = pFileName;
            pFileRec->field_4_pResourcesToLoadList = nullptr;
            pFileRec->field_8_type = type;
            pFileRec->field_C_resourceId = resourceId;

            auto pFilePart = ao_new<ResourceManager_FilePartRecord>();
            pFilePart->field_0_type = type;
            pFilePart->field_4_res_id = resourceId;
            pFilePart->field_8_pCamera = sCameraBeingLoaded_507C98;

            pFileRec->field_10_file_sections_dArray.Push_Back(pFilePart);

            pFileRec->field_1C_pGameObjFileRec = ResourceManager::LoadResourceFile(
                pFileName,
                ResourceManager::On_Loaded_446C10,
                pFileRec);
            ObjList_5009E0->Push_Back(pFileRec);
        }
    }
    else if (loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResourceFile_455270(pFileName, nullptr);
        u8** ppRes = ResourceManager::GetLoadedResource_4554F0(type, resourceId, 1, 0);
        if (ppRes)
        {
            sCameraBeingLoaded_507C98->field_0_array.Push_Back(ppRes);
        }
    }
}

void ResourceManager::LoadResourcesFromList_446E80(const char_type* pFileName, ResourcesToLoadList* pTypeAndIdList, LoadMode loadMode, s16 bDontLoad)
{
    // Debug_Print_Stub_48DD70("Requesting tag res %s\n", pFileName);

    if (bDontLoad)
    {
        return;
    }

    // Check if all resources are already loaded
    bool allResourcesLoaded = true;
    for (s32 i = 0; i < pTypeAndIdList->field_0_count; i++)
    {
        while (!ResourceManager::GetLoadedResource_4554F0(
            pTypeAndIdList->field_4_items[i].field_0_type,
            pTypeAndIdList->field_4_items[i].field_4_res_id,
            0,
            0))
        {
            // A resource we need is missing
            allResourcesLoaded = false;
            break;
        }
    }

    // All resources that we required are already loaded
    if (allResourcesLoaded)
    {
        for (s32 i = 0; i < pTypeAndIdList->field_0_count; i++)
        {
            sCameraBeingLoaded_507C98->field_0_array.Push_Back(GetLoadedResource_4554F0(
                pTypeAndIdList->field_4_items[i].field_0_type,
                pTypeAndIdList->field_4_items[i].field_4_res_id,
                1,
                0));
        }
        return;
    }

    if (loadMode == LoadMode::LoadResourceFromList_1)
    {
        for (s32 i = 0; i < ObjList_5009E0->Size(); i++)
        {
            ResourceManager_FileRecord* pFileRec = ObjList_5009E0->ItemAt(i);
            if (!pFileRec)
            {
                break;
            }

            if (!strcmp(pFileName, pFileRec->field_0_fileName))
            {
                if (pTypeAndIdList->field_0_count == 0)
                {
                    return;
                }

                for (s32 j = 0; j < pTypeAndIdList->field_0_count; j++)
                {
                    auto pPart = ao_new<ResourceManager_FilePartRecord>();
                    pPart->field_0_type = pTypeAndIdList->field_4_items[j].field_0_type;
                    pPart->field_4_res_id = pTypeAndIdList->field_4_items[j].field_4_res_id;
                    pPart->field_8_pCamera = sCameraBeingLoaded_507C98;
                    pFileRec->field_10_file_sections_dArray.Push_Back(pPart);
                }
                return;
            }
        }

        auto pNewFileRec = ao_new<ResourceManager_FileRecord>();
        pNewFileRec->field_0_fileName = pFileName;
        pNewFileRec->field_4_pResourcesToLoadList = pTypeAndIdList;
        pNewFileRec->field_8_type = 0;
        pNewFileRec->field_C_resourceId = 0;

        // Check if all resources are already loaded
        if ((pTypeAndIdList->field_0_count & ~0x80000000))
        {
            for (s32 j = 0; j < pTypeAndIdList->field_0_count; j++)
            {
                auto pNewFilePart = ao_new<ResourceManager_FilePartRecord>();
                pNewFilePart->field_0_type = pTypeAndIdList->field_4_items[j].field_0_type;
                pNewFilePart->field_4_res_id = pTypeAndIdList->field_4_items[j].field_4_res_id;
                pNewFilePart->field_8_pCamera = sCameraBeingLoaded_507C98;
                pNewFileRec->field_10_file_sections_dArray.Push_Back(pNewFilePart);
            }
        }

        pNewFileRec->field_1C_pGameObjFileRec = ResourceManager::LoadResourceFile(
            pFileName,
            ResourceManager::On_Loaded_446C10,
            pNewFileRec);
        ObjList_5009E0->Push_Back(pNewFileRec);
    }
    else if (loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResourceFile_455270(pFileName, nullptr);
        for (s32 j = 0; j < pTypeAndIdList->field_0_count; j++)
        {
            u8** ppLoadedRes = ResourceManager::GetLoadedResource_4554F0(
                pTypeAndIdList->field_4_items[j].field_0_type,
                pTypeAndIdList->field_4_items[j].field_4_res_id,
                1,
                0);

            if (ppLoadedRes)
            {
                sCameraBeingLoaded_507C98->field_0_array.Push_Back(ppLoadedRes);
            }
        }
    }
}

void ResourceManager::WaitForPendingResources_41EA60(BaseGameObject* pObj)
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->mBaseGameObjectTypeId == ReliveTypes::eLoadingFile)
        {
            auto pLoadingFile = static_cast<LoadingFile*>(pObjIter);
            if (!pObj || pObj == pLoadingFile->field_18_fn_arg)
            {
                while (pLoadingFile->field_28_state != 0)
                {
                    if (pLoadingFile->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
                    {
                        break;
                    }
                    pLoadingFile->VUpdate();
                }
                pLoadingFile->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
    }
}

void ResourceManager::LoadingLoop_41EAD0(s16 bShowLoadingIcon)
{
    while (gFilesPending_507714 > 0)
    {
        SYS_EventsPump_44FF90();

        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            if (pObjIter->mBaseGameObjectTypeId == ReliveTypes::eLoadingFile)
            {
                if (!pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
                {
                    pObjIter->VUpdate();
                }

                if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
                {
                    i = gBaseGameObjects->RemoveAt(i);
                    delete pObjIter;
                }
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
}

void ResourceManager::Free_Resources_For_Camera_447170(Camera* pCamera)
{
    for (s32 i = 0; i < ObjList_5009E0->Size(); i++)
    {
        ResourceManager_FileRecord* pObjIter = ObjList_5009E0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->field_1C_pGameObjFileRec->field_28_state == 0)
        {
            // Remove/free file parts that belong to the cameraa
            auto pFileSecsArray = &pObjIter->field_10_file_sections_dArray;
            for (s32 j = 0; j < pFileSecsArray->Size(); j++)
            {
                ResourceManager_FilePartRecord* pFilePartRecord = pFileSecsArray->ItemAt(j);
                if (!pFilePartRecord)
                {
                    break;
                }

                if (pFilePartRecord->field_8_pCamera == pCamera)
                {
                    j = pFileSecsArray->RemoveAt(j);

                    // Only delete the record we just removed
                    ao_delete_free_447540(pFilePartRecord);
                }
                else
                {
                    LOG_WARNING("OG bug fix 0x" << pFilePartRecord << " would have been deleted here!");
                }
            }

            // Free the containing record if its section array is now empty
            if (pObjIter->field_10_file_sections_dArray.Empty())
            {
                if (pObjIter->field_1C_pGameObjFileRec)
                {
                    pObjIter->field_1C_pGameObjFileRec->DestroyOnState0_41EA50();
                }

                i = ObjList_5009E0->RemoveAt(i);
                relive_delete pObjIter;
            }
        }
    }
}

s32 ResourceManager::SEQ_HashName_454EA0(const char_type* seqFileName)
{
    // Clamp max len
    size_t seqFileNameLength = strlen(seqFileName) - 1;
    if (seqFileNameLength > 8)
    {
        seqFileNameLength = 8;
    }

    // Iterate each s8 to calculate hash
    u32 hashId = 0;
    for (size_t index = 0; index < seqFileNameLength; index++)
    {
        s8 letter = seqFileName[index];
        if (letter == '.')
        {
            break;
        }

        const u32 temp = 10 * hashId;
        if (letter < '0' || letter > '9')
        {
            if (letter >= 'a')
            {
                if (letter <= 'z')
                {
                    letter -= ' ';
                }
            }
            hashId = letter % 10 + temp;
        }
        else
        {
            hashId = index || letter != '0' ? temp + letter - '0' : temp + 9;
        }
    }
    return hashId;
}

void ResourceManager::Init_454DA0()
{
    for (s32 i = 1; i < kLinkedListArraySize - 1; i++)
    {
        sResourceLinkedList_50E270[i].field_0_ptr = nullptr;
        sResourceLinkedList_50E270[i].field_4_pNext = &sResourceLinkedList_50E270[i + 1];
    }

    sResourceLinkedList_50E270[kLinkedListArraySize - 1].field_4_pNext = nullptr;

    sResourceLinkedList_50E270[0].field_0_ptr = &sResourceHeap_50EE38[sizeof(Header)];
    sResourceLinkedList_50E270[0].field_4_pNext = nullptr;

    Header* pHeader = Get_Header_455620(&sResourceLinkedList_50E270[0].field_0_ptr);
    pHeader->field_0_size = kResHeapSize;
    pHeader->field_8_type = Resource_Free;

    sFirstLinkedListItem_50EE2C = &sResourceLinkedList_50E270[0];
    sSecondLinkedListItem_50EE28 = &sResourceLinkedList_50E270[1];

    spResourceHeapStart_50EE30 = &sResourceHeap_50EE38[0];
    spResourceHeapEnd_9F0E3C = &sResourceHeap_50EE38[kResHeapSize - 1];
}

ResourceManager::ResourceHeapItem* ResourceManager::Push_List_Item()
{
    auto old = sSecondLinkedListItem_50EE28;
    sSecondLinkedListItem_50EE28 = sSecondLinkedListItem_50EE28->field_4_pNext;
    return old;
}


void ResourceManager::Pop_List_Item(ResourceHeapItem* pListItem)
{
    pListItem->field_0_ptr = nullptr;
    pListItem->field_4_pNext = sSecondLinkedListItem_50EE28; // point to the current
    sSecondLinkedListItem_50EE28 = pListItem;                // set current to old
}

ResourceManager::ResourceHeapItem* ResourceManager::Split_block(ResourceManager::ResourceHeapItem* pItem, s32 size)
{
    Header* pToSplit = Get_Header_455620(&pItem->field_0_ptr);
    const u32 sizeForNewRes = pToSplit->field_0_size - size;
    if (sizeForNewRes >= sizeof(Header))
    {
        ResourceHeapItem* pNewListItem = ResourceManager::Push_List_Item();
        pNewListItem->field_4_pNext = pItem->field_4_pNext; // New item points to old
        pItem->field_4_pNext = pNewListItem;                // Old item points to new

        pNewListItem->field_0_ptr = pItem->field_0_ptr + size; // Point the split point

        // Init header of split item
        Header* pHeader = Get_Header_455620(&pNewListItem->field_0_ptr);
        pHeader->field_0_size = sizeForNewRes;
        pHeader->field_8_type = Resource_Free;
        pHeader->field_4_ref_count = 0;
        pHeader->field_C_id = 0;

        // Update old size
        pToSplit->field_0_size = size;
    }

    return pItem;
}

LoadingFile* ResourceManager::LoadResourceFile_4551E0(const char_type* pFileName, TLoaderFn fnOnLoad, Camera* pCamera1, Camera* pCamera2)
{
    LvlFileRecord* pFileRec = sLvlArchive_4FFD60.Find_File_Record(pFileName);
    if (!pFileRec)
    {
        return nullptr;
    }

    return ao_new<LoadingFile>(
        sLvlArchive_4FFD60.field_4_cd_pos + pFileRec->field_C_start_sector,
        pFileRec->field_10_num_sectors,
        fnOnLoad,
        pCamera1,
        pCamera2);
}

u8** ResourceManager::Alloc_New_Resource_Impl(u32 type, u32 id, u32 size, bool locked, BlockAllocMethod allocType)
{
    u8** ppNewRes = Allocate_New_Block_454FE0(size + sizeof(Header), allocType);
    if (!ppNewRes)
    {
        // Failed, try to reclaim some memory and try again.
        Reclaim_Memory_455660(0);
        ppNewRes = Allocate_New_Block_454FE0(size + sizeof(Header), allocType);
    }

    if (ppNewRes)
    {
        Header* pHeader = Get_Header_455620(ppNewRes);
        pHeader->field_8_type = type;
        pHeader->field_C_id = id;
        pHeader->field_4_ref_count = 1;
        pHeader->field_6_flags = locked ? ResourceHeaderFlags::eLocked : 0;
    }
    return ppNewRes;
}

u8** ResourceManager::Alloc_New_Resource_454F20(u32 type, u32 id, u32 size)
{
    return Alloc_New_Resource_Impl(type, id, size, false, BlockAllocMethod::eFirstMatching);
}

u8** ResourceManager::Allocate_New_Locked_Resource(u32 type, u32 id, u32 size)
{
    return Alloc_New_Resource_Impl(type, id, size, true, BlockAllocMethod::eLastMatching);
}


u8** ResourceManager::Allocate_New_Block_454FE0(u32 sizeBytes, BlockAllocMethod allocMethod)
{
    ResourceHeapItem* pListItem = sFirstLinkedListItem_50EE2C;
    ResourceHeapItem* pHeapMem = nullptr;
    const u32 size = (sizeBytes + 3) & ~3u; // Rounding ??
    Header* pHeaderToUse = nullptr;
    while (pListItem)
    {
        // Is it a free block?
        Header* pResHeader = Get_Header_455620(&pListItem->field_0_ptr);
        if (pResHeader->field_8_type == Resource_Free)
        {
            // Keep going till we hit a block that isn't free
            for (ResourceHeapItem* i = pListItem->field_4_pNext; i; i = pListItem->field_4_pNext)
            {
                Header* pHeader = Get_Header_455620(&i->field_0_ptr);
                if (pHeader->field_8_type != Resource_Free)
                {
                    break;
                }

                // Combine up the free blocks
                pResHeader->field_0_size += pHeader->field_0_size;
                pListItem->field_4_pNext = i->field_4_pNext;
                Pop_List_Item(i);
            }

            // Size will be bigger now that we've freed at least 1 resource
            if (pResHeader->field_0_size >= size)
            {
                switch (allocMethod)
                {
                    case BlockAllocMethod::eFirstMatching:
                        // Use first matching item
                        sManagedMemoryUsedSize_9F0E48 += size;
                        if (sManagedMemoryUsedSize_9F0E48 >= sPeakedManagedMemUsage_9F0E4C)
                        {
                            sPeakedManagedMemUsage_9F0E4C = sManagedMemoryUsedSize_9F0E48;
                        }
                        return &Split_block(pListItem, size)->field_0_ptr;
                    case BlockAllocMethod::eNearestMatching:
                        // Find nearest matching item
                        if (pResHeader->field_0_size < pHeaderToUse->field_0_size)
                        {
                            pHeapMem = pListItem;
                            pHeaderToUse = pResHeader;
                        }
                        break;
                    case BlockAllocMethod::eLastMatching:
                        // Will always to set to the last most free item
                        pHeapMem = pListItem;
                        pHeaderToUse = pResHeader;
                        break;
                }
            }
        }

        pListItem = pListItem->field_4_pNext;
    }

    if (!pHeapMem)
    {
        // Allocation failure
        sAllocationFailed_9F0E50 = 1;
        return nullptr;
    }

    sManagedMemoryUsedSize_9F0E48 += size;
    if (sManagedMemoryUsedSize_9F0E48 >= sPeakedManagedMemUsage_9F0E4C)
    {
        sPeakedManagedMemUsage_9F0E4C = sManagedMemoryUsedSize_9F0E48;
    }

    switch (allocMethod)
    {
            // Note: eFirstMatching case not possible here as pHeapMem case would have early returned
        case BlockAllocMethod::eNearestMatching:
            return &ResourceManager::Split_block(pHeapMem, size)->field_0_ptr;

        case BlockAllocMethod::eLastMatching:
            if (pHeaderToUse->field_0_size - size >= sizeof(Header))
            {
                return &Split_block(pHeapMem, pHeaderToUse->field_0_size - size)->field_4_pNext->field_0_ptr;
            }
            else
            {
                // No need to split as the size must be exactly the size of a resource header
                return &pHeapMem->field_0_ptr;
            }
            break;

            // Should be impossible to get here
        default:
            return nullptr;
    }
}

s16 ResourceManager::LoadResourceFileWrapper(const char_type* filename, Camera* pCam)
{
    return LoadResourceFile_455270(filename, pCam, BlockAllocMethod::eFirstMatching);
}

s16 ResourceManager::LoadResourceFile_455270(const char_type* filename, Camera* pCam, BlockAllocMethod allocMethod)
{
    // Note: None gPcOpenEnabled_508BF0 block not impl as never used

    ResourceManager::LoadingLoop_41EAD0(0);

    LvlFileRecord* pFileRec = sLvlArchive_4FFD60.Find_File_Record(filename);
    if (!pFileRec)
    {
        return 0;
    }

    const s32 size = pFileRec->field_10_num_sectors << 11;
    u8** ppRes = ResourceManager::Allocate_New_Block_454FE0(size, allocMethod);
    if (!ppRes)
    {
        ResourceManager::Reclaim_Memory_455660(0);
        ppRes = ResourceManager::Allocate_New_Block_454FE0(size, allocMethod);
        if (!ppRes)
        {
            return 0;
        }
    }

    // NOTE: Not sure why this is done twice, perhaps the above memory compact can invalidate the ptr?
    pFileRec = sLvlArchive_4FFD60.Find_File_Record(filename);
    if (!pFileRec)
    {
        return 0;
    }

    if (!sLvlArchive_4FFD60.Read_File(pFileRec, Get_Header_455620(ppRes)))
    {
        FreeResource_455550(ppRes);
        return 0;
    }

    ResourceManager::Move_Resources_To_DArray_455430(ppRes, &pCam->field_0_array);
    return 1;
}

s16 ResourceManager::Move_Resources_To_DArray_455430(u8** ppRes, DynamicArrayT<u8*>* pArray)
{
    auto pItemToAdd = (ResourceHeapItem*) ppRes;
    Header* pHeader = Get_Header_455620(ppRes);
    if (pHeader->field_8_type != Resource_End)
    {
        while (pHeader->field_8_type != Resource_Pend
               && pHeader->field_0_size
               && !(pHeader->field_0_size & 3))
        {
            if (pArray)
            {
                pArray->Push_Back((u8**) pItemToAdd);
                pHeader->field_4_ref_count++;
            }

            pHeader = (Header*) ((s8*) pHeader + pHeader->field_0_size);

            // Out of heap space
            if (pHeader->field_0_size >= kResHeapSize)
            {
                return 1;
            }

            ResourceHeapItem* pNewListItem = Push_List_Item();
            pNewListItem->field_4_pNext = pItemToAdd->field_4_pNext;
            pItemToAdd->field_4_pNext = pNewListItem;
            pNewListItem->field_0_ptr = (u8*) &pHeader[1]; // point after header
            pItemToAdd = pNewListItem;

            // No more resources to add
            if (pHeader->field_8_type == Resource_End)
            {
                break;
            }
        }
    }

    if (pHeader)
    {
        pHeader->field_8_type = Resource_Free;
        if (pItemToAdd->field_4_pNext)
        {
            // Size of next item - location of current res
            // TODO 64bit warning
            pHeader->field_0_size = static_cast<u32>(pItemToAdd->field_4_pNext->field_0_ptr - (u8*) pHeader - sizeof(Header));
        }
        else
        {
            // Isn't a next item so use ptr to end of heap - location of current res
            // TODO: 64bit warning
            pHeader->field_0_size = static_cast<u32>(spResourceHeapEnd_9F0E3C - (u8*) pHeader);
        }

        sManagedMemoryUsedSize_9F0E48 -= pHeader->field_0_size;
    }

    return 1;
}

u8** ResourceManager::GetLoadedResource_4554F0(u32 type, u32 resourceId, s16 addUseCount, s16 bLock)
{
    // Iterate all list items
    ResourceHeapItem* pListIter = sFirstLinkedListItem_50EE2C;
    while (pListIter)
    {
        // Find something that matches the type and resource ID
        Header* pResHeader = Get_Header_455620(&pListIter->field_0_ptr);
        if (pResHeader->field_8_type == type && pResHeader->field_C_id == resourceId)
        {
            if (addUseCount)
            {
                pResHeader->field_4_ref_count++;
            }

            if (bLock)
            {
                pResHeader->field_6_flags |= ResourceHeaderFlags::eLocked;
            }

            return &pListIter->field_0_ptr;
        }

        pListIter = pListIter->field_4_pNext;
    }
    return nullptr;
}


void ResourceManager::CheckResourceIsLoaded(u32 type, AOResourceID resourceId)
{
    u8** ppRes = GetLoadedResource_4554F0(type, resourceId, FALSE, FALSE);
    if (!ppRes)
    {
        LOG_ERROR("Resource not loaded type " << type << " resource Id " << resourceId);
        ALIVE_FATAL("Resource not loaded");
    }
}

void ResourceManager::CheckResourceIsLoaded(u32 type, std::initializer_list<AOResourceID>& resourceIds)
{
    for (const auto& resourceId : resourceIds)
    {
        CheckResourceIsLoaded(type, resourceId);
    }
}

s16 ResourceManager::FreeResource_455550(u8** handle)
{
    // Note: Checks for ptrs of 0xCDCDCDCD and 0xDDDDDDDD removed
    // because these can only come from the MSVCRT debug runtimes
    if (!handle)
    {
        return 1;
    }
    return FreeResource_Impl_4555B0(*handle);
}


s16 ResourceManager::FreeResource_Impl_4555B0(u8* handle)
{
    if (handle)
    {
        Header* pHeader = Get_Header_455620(&handle);
        if (pHeader->field_4_ref_count)
        {
            // Decrement ref count, if its not zero then we can't free it yet
            pHeader->field_4_ref_count--;
            if (pHeader->field_4_ref_count > 0)
            {
                return 0;
            }
            pHeader->field_8_type = Resource_Free;
            pHeader->field_6_flags = 0;
            sManagedMemoryUsedSize_9F0E48 -= pHeader->field_0_size;
        }
    }
    return 1;
}

ResourceManager::Header* ResourceManager::Get_Header_455620(u8** ppRes)
{
    return reinterpret_cast<Header*>((*ppRes - sizeof(Header)));
}

void ResourceManager::Reclaim_Memory_455660(u32 sizeToReclaim)
{
    if (sResources_Pending_Loading_9F0E38 != 0)
    {
        return;
    }

    // If we failed to allocate a block or no size was passed then attempt to reclaim the whole heap
    if (sAllocationFailed_9F0E50 || sizeToReclaim == 0)
    {
        sizeToReclaim = kResHeapSize;
        sAllocationFailed_9F0E50 = 0;
    }

    ResourceHeapItem* pListItem = sFirstLinkedListItem_50EE2C;
    ResourceHeapItem* pToUpdate = nullptr;

    while (pListItem)
    {
        Header* pCurrentHeader = Get_Header_455620(&pListItem->field_0_ptr);
        if (pCurrentHeader->field_8_type == Resource_Free)
        {
            ResourceHeapItem* pNext = pListItem->field_4_pNext;
            if (!pNext)
            {
                return;
            }

            Header* pNextHeader = Get_Header_455620(&pNext->field_0_ptr);
            if (pNextHeader->field_8_type == Resource_Free)
            {
                // Next block is also free, so we can merge them together
                ResourceHeapItem* pToRemove = pListItem->field_4_pNext;
                pCurrentHeader->field_0_size += pNextHeader->field_0_size;
                pListItem->field_4_pNext = pNext->field_4_pNext;
                Pop_List_Item(pToRemove);
            }
            else
            {
                u32 sizeToMove = 0;
                if (pNextHeader->field_6_flags & ResourceHeaderFlags::eOnlyAHeader)
                {
                    sizeToMove = sizeof(Header);
                }
                else
                {
                    sizeToMove = pNextHeader->field_0_size;
                }

                if (pNextHeader->field_6_flags & ResourceHeaderFlags::eLocked || sizeToMove > sizeToReclaim)
                {
                    // Locked or trying to move more than requested, skip to next
                    pToUpdate = pListItem;
                    pListItem = pListItem->field_4_pNext;
                }
                else
                {
                    sizeToReclaim -= sizeToMove;
                    const u32 savedSize = pCurrentHeader->field_0_size;
                    u8* pDataStart = pNext->field_0_ptr - sizeof(Header);
                    if (sizeToMove > 0)
                    {
                        const size_t offset = (s8*) pCurrentHeader - (s8*) pNextHeader;
                        memmove(pDataStart + offset, pDataStart, sizeToMove);
                    }

                    // Get resource header after the current one
                    Header* pNextResHeader = (Header*) ((s8*) pCurrentHeader + pCurrentHeader->field_0_size);
                    pNextResHeader->field_0_size = savedSize;
                    pNextResHeader->field_8_type = Resource_Free;

                    pNext->field_0_ptr = (u8*) &pCurrentHeader[1];     // Data starts after header
                    pListItem->field_0_ptr = (u8*) &pNextResHeader[1]; // Data starts after header
                    pListItem->field_4_pNext = pNext->field_4_pNext;
                    pNext->field_4_pNext = pListItem;

                    if (pToUpdate)
                    {
                        pToUpdate->field_4_pNext = pNext;
                    }
                    else
                    {
                        sFirstLinkedListItem_50EE2C = pNext;
                    }
                    pToUpdate = pNext;
                }
            }
        }
        else
        {
            // Not a free block, so move to the next item
            pToUpdate = pListItem;
            pListItem = pListItem->field_4_pNext;
        }
    }
}

void ResourceManager::Increment_Pending_Count_4557A0()
{
    sResources_Pending_Loading_9F0E38++;
}

void ResourceManager::Decrement_Pending_Count_4557B0()
{
    sResources_Pending_Loading_9F0E38--;
}

void ResourceManager::Set_Header_Flags_4557D0(u8** ppRes, s16 flags)
{
    Get_Header_455620(ppRes)->field_6_flags |= flags;
}


void ResourceManager::Clear_Header_Flags_4557F0(u8** ppRes, s16 flags)
{
    Get_Header_455620(ppRes)->field_6_flags &= ~flags;
}

s32 ResourceManager::Is_Resources_Pending_4557C0()
{
    return sResources_Pending_Loading_9F0E38 > 0 ? 1 : 0;
}

void ResourceManager::Free_Resource_Of_Type_455810(u32 type)
{
    ResourceHeapItem* pListItem = sFirstLinkedListItem_50EE2C;
    while (pListItem)
    {
        // Free it if the type matches and its not flagged as never free
        Header* pHeader = Get_Header_455620(&pListItem->field_0_ptr);
        if (pHeader->field_8_type == type && !(pHeader->field_6_flags & ResourceHeaderFlags::eNeverFree))
        {
            pHeader->field_8_type = Resource_Free;
            pHeader->field_6_flags = 0;
            pHeader->field_4_ref_count = 0;

            sManagedMemoryUsedSize_9F0E48 -= pHeader->field_0_size;
        }
        pListItem = pListItem->field_4_pNext;
    }
}

} // namespace AO
