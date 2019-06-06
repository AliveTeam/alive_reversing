#include "stdafx.h"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "LvlArchive.hpp"
#include "ScreenManager.hpp" // Camera
#include "Particle.hpp"
#include "PsxRender.hpp"
#include "PsxDisplay.hpp"

ALIVE_VAR(1, 0x5C1BB0, ResourceManager*, pResourceManager_5C1BB0, nullptr);

ALIVE_VAR(1, 0xab4a04, DWORD, sManagedMemoryUsedSize_AB4A04, 0);
ALIVE_VAR(1, 0xab4a08, DWORD, sPeakedManagedMemUsage_AB4A08, 0);

ALIVE_VAR(1, 0x5C1B96, short, sbLoadingInProgress_5C1B96, 0);
ALIVE_VAR(1, 0x5C1BAA, short, word_5C1BAA, 0);
ALIVE_VAR(1, 0x5C1BAC, int, dword_5C1BAC, 0);
ALIVE_VAR(1, 0xAB49F4, short, sResources_Pending_Loading_AB49F4, 0);
ALIVE_VAR(1, 0xAB4A0C, short, sAllocationFailed_AB4A0C, 0);

ALIVE_VAR(1, 0x5D29EC, ResourceManager::ResourceHeapItem*, sFirstLinkedListItem_5D29EC, nullptr);
ALIVE_VAR(1, 0x5D29E8, ResourceManager::ResourceHeapItem*, sSecondLinkedListItem_5D29E8, nullptr);

// TODO: Move to own file
EXPORT void CCSTD sub_465BC0(int /*a1*/)
{
    NOT_IMPLEMENTED();
}

// TODO: Move to own file
EXPORT void CC Game_ShowLoadingIcon_482D80()
{
    BYTE** ppLoadingAnimRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kLoadingResID, FALSE, FALSE);
    if (!ppLoadingAnimRes)
    {
        return;
    }
    PSX_Display_Buffer dispBuffer = {};

    Particle* pParticle = alive_new<Particle>();
    pParticle->ctor_4CC4C0(FP_FromInteger(0), FP_FromInteger(0), 900, 50, 38, ppLoadingAnimRes);

    // TODO: May need to clear all other low word bits ?
    pParticle->field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
    pParticle->field_20_animation.field_4_flags.Set(AnimFlags::eBit16_bBlending);

    pParticle->field_20_animation.field_C_render_layer = 0;
    PSX_SetDefDrawEnv_4F5AA0(&dispBuffer.field_0_draw_env, 0, 0, 640, 240);
    PSX_PutDrawEnv_4F5980(&dispBuffer.field_0_draw_env);
    PSX_DrawSync_4F6280(0);

    // This was doing something odd with OT index.. I think its trying to simulate double buffering by
    // using other parts of the OT while another part is drawn, but it was bugged because it cleared the other anyway
    // on PC it seems fine to just always start at zero.
    PSX_ClearOTag_4F6290(dispBuffer.field_70_ot_buffer, 43);
    pParticle->field_20_animation.vRender_40B820(320, 220, dispBuffer.field_70_ot_buffer, 0, 0);
    PSX_DrawOTag_4F6540(dispBuffer.field_70_ot_buffer);
    PSX_DrawSync_4F6280(0);

    PSX_ClearOTag_4F6290(dispBuffer.field_70_ot_buffer, 43);
    pParticle->field_20_animation.vRender_40B820(320, 640 - 164, dispBuffer.field_70_ot_buffer, 0, 0);
    PSX_DrawOTag_4F6540(dispBuffer.field_70_ot_buffer);
    PSX_DrawSync_4F6280(0);

    PSX_SetDefDispEnv_4F55A0(&dispBuffer.field_5C_disp_env, 0, 0, 640, 240);
    PSX_PutDispEnv_4F5890(&dispBuffer.field_5C_disp_env);
    pParticle->field_6_flags.Set(BaseGameObject::eDead);
    word_5C1BAA = 1;
}

ResourceManager* ResourceManager::ctor_464910()
{
    BaseGameObject_ctor_4DBFA0(1, 0);

    field_20_files_pending_loading.ctor_40CA60(3);
    field_48_dArray.ctor_40CA60(3);

    field_6_flags.Set(BaseGameObject::eSurviveDeathReset);
    field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap);
    SetVTable(this, 0x545EBC); // vTbl_ResourceManager_545EBC

    field_4_typeId = Types::eResourceManager_70;
    field_2C_pFileItem = nullptr;
    field_30_start_sector = 0;
    field_34_num_sectors = 0;
    field_38_ppRes = {};
    field_3C_pLoadingHeader = nullptr;
    field_42_state = State_Wait_For_Load_Request;
    field_40_seek_attempts = 0;
    return this;
}

BaseGameObject* ResourceManager::vdtor_4649B0(signed int flags)
{
    dtor_4649E0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void ResourceManager::dtor_4649E0()
{
    SetVTable(this, 0x545EBC); // vTbl_ResourceManager_545EBC
    Shutdown_465610();
    field_48_dArray.dtor_40CAD0();
    field_20_files_pending_loading.dtor_40CAD0();
    BaseGameObject_dtor_4DBEC0();
}

void ResourceManager::vLoadFile_StateMachine_464A70()
{
    switch (field_42_state)
    {
    case State_Wait_For_Load_Request:
        // NOTE: Pruned branches here from stub that was hard coded to return 0
        if (!field_20_files_pending_loading.IsEmpty())
        {
            field_2C_pFileItem = field_20_files_pending_loading.ItemAt(0);

            LvlFileRecord* pLvlFileRec1 = sLvlArchive_5BC520.Find_File_Record_433160(field_2C_pFileItem->field_0_fileName);
            field_34_num_sectors = pLvlFileRec1->field_10_num_sectors;
            field_30_start_sector = pLvlFileRec1->field_C_start_sector + sLvlArchive_5BC520.field_4;
            PSX_Pos_To_CdLoc_4FADD0(field_30_start_sector, &field_44_cdLoc);

            sbLoadingInProgress_5C1B96 = 1;
            field_42_state = State_Allocate_Memory_For_File;
        }
        break;

    case State_Allocate_Memory_For_File:
        field_38_ppRes = ResourceManager::Allocate_New_Block_49BFB0(field_34_num_sectors << 11, BlockAllocMethod::eFirstMatching);
        if (field_38_ppRes)
        {
            field_3C_pLoadingHeader = Get_Header_49C410(field_38_ppRes);
            field_3C_pLoadingHeader->field_8_type = Resource_Pend;
            ResourceManager::Increment_Pending_Count_49C5F0();
            field_42_state = State_Seek_To_File;
        }
        else
        {
            // Failed to allocate, free some memory and loop around for another go
            ResourceManager::Reclaim_Memory_49C470(200000);
        }
        break;

    case State_Seek_To_File:
        if (!PSX_CD_File_Seek_4FB1E0(2, &field_44_cdLoc))
        {
            if (field_40_seek_attempts < 20u)
            {
                field_40_seek_attempts++;
                return;
            }

            sub_465BC0(0); // Crashes if forced to be called.. ? Seems to display the can't find demo/fmv message

            while (!PSX_CD_File_Seek_4FB1E0(2, &field_44_cdLoc))
            {
                // Do nothing
            }
        }
        field_42_state = State_Read_Sectors_ASync;
        field_40_seek_attempts = 0;
        break;

    case State_Read_Sectors_ASync:
        if (PSX_CD_File_Read_4FB210(field_34_num_sectors, field_3C_pLoadingHeader))
        {
            field_42_state = State_Wait_For_Read_Complete;
            const int bWaitRet = PSX_CD_FileIOWait_4FB260(1);
            if (bWaitRet <= 0)
            {
                field_42_state = bWaitRet != -1 ? State_File_Read_Completed : State_Seek_To_File;
            }
        }
        else
        {
            field_42_state = State_Seek_To_File;
        }
        break;

    case State_Wait_For_Read_Complete:
    {
        const int bWaitRet = PSX_CD_FileIOWait_4FB260(1);
        if (bWaitRet <= 0)
        {
            field_42_state = bWaitRet != -1 ? State_File_Read_Completed : State_Seek_To_File;
        }
    }
    break;

    case State_File_Read_Completed:
        Move_Resources_To_DArray_49C1C0(field_38_ppRes, &field_48_dArray);
        field_42_state = State_Load_Completed;
        break;

    case State_Load_Completed:
        sbLoadingInProgress_5C1B96 = 0;
        OnResourceLoaded_464CE0();
        field_48_dArray.field_4_used_size = 0; // TODO: Needs to be private
        Decrement_Pending_Count_49C610();
        field_42_state = State_Wait_For_Load_Request;
        break;

    default:
        return;
    }
}

void ResourceManager::OnResourceLoaded_464CE0()
{
    // Iterate every section in the loaded file
    DynamicArrayIter fileSectionsArrayIter = {};
    fileSectionsArrayIter.field_0_pDynamicArray = &field_2C_pFileItem->field_10_file_sections_dArray;
    fileSectionsArrayIter.field_4_idx = 0;
    while (fileSectionsArrayIter.field_4_idx < field_2C_pFileItem->field_10_file_sections_dArray.Size())
    {
        ResourceManager_FilePartRecord_18* pFilePart = field_2C_pFileItem->field_10_file_sections_dArray.ItemAt(fileSectionsArrayIter.field_4_idx);
        fileSectionsArrayIter.field_4_idx++;

        if (!pFilePart)
        {
            break;
        }

        if (pFilePart->field_0_type || pFilePart->field_4_id)
        {
            // Find matching file part
            BYTE** ppRes = nullptr;
            for (int i=0; i< field_48_dArray.Size(); i++)
            {
                if (field_48_dArray.ItemAt(i))
                {
                    Header* pHeader = Get_Header_49C410(field_48_dArray.ItemAt(i));
                    if (pHeader->field_C_id == pFilePart->field_4_id && pHeader->field_8_type == pFilePart->field_0_type)
                    {
                        ppRes = field_48_dArray.ItemAt(i);
                        break;
                    }
                }
            }

            if (!ppRes)
            {
                LOG_ERROR("File part not found id " << pFilePart->field_4_id << " type " << pFilePart->field_0_type);
                ALIVE_FATAL("File part not found");
            }

            if (pFilePart->field_14_bAddUseCount)
            {
                Inc_Ref_Count_49C310(ppRes);
            }

            if (pFilePart->field_8_pCamera)
            {
                pFilePart->field_8_pCamera->field_0.Push_Back_40CAF0(ppRes);
            }

            if (pFilePart->field_10_pFn)
            {
                pFilePart->field_10_pFn(pFilePart->field_C_fn_arg_pCamera);
            }
        }
        else
        {
            if (pFilePart->field_14_bAddUseCount || pFilePart->field_8_pCamera)
            {
                for (int i = 0; i < field_48_dArray.Size(); i++)
                {
                    BYTE** pItem2 = field_48_dArray.ItemAt(i);
                    if (!pItem2)
                    {
                        break;
                    }

                    if (pFilePart->field_14_bAddUseCount)
                    {
                        Inc_Ref_Count_49C310(pItem2);
                    }

                    if (pFilePart->field_8_pCamera)
                    {
                        pFilePart->field_8_pCamera->field_0.Push_Back_40CAF0(pItem2);
                    }
                }
            }

            if (pFilePart->field_10_pFn)
            {
                pFilePart->field_10_pFn(pFilePart->field_C_fn_arg_pCamera);
            }
        }
        fileSectionsArrayIter.Remove_At_Iter_40CCA0();
        Mem_Free_495540(pFilePart);
    }

    // Remove from pending files
    field_20_files_pending_loading.Remove_Item(field_2C_pFileItem);

    // Free/destruct the removed item
    Mem_Free_495560(field_2C_pFileItem->field_0_fileName);
    if (field_2C_pFileItem)
    {
        field_2C_pFileItem->dtor_464EA0();
        Mem_Free_495540(field_2C_pFileItem);
    }
    field_2C_pFileItem = nullptr;
}

void ResourceManager::ResourceManager_FileRecord_1C::dtor_464EA0()
{
    field_10_file_sections_dArray.dtor_40CAD0();
}

void ResourceManager::LoadResource_464EE0(const char* pFileItem, DWORD type, DWORD resourceID, Camera* pCamera, Camera* pFnArg, ResourceManager::TLoaderFn pFn, __int16 bAddUseCount)
{
    BYTE** pLoadedRes = GetLoadedResource_49C2A0(type, resourceID, 1, 0);
    if (pLoadedRes)
    {
        if (pCamera)
        {
            pCamera->field_0.Push_Back_40CAF0(pLoadedRes);
        }

        if (pFn)
        {
            pFn(pFnArg);
        }
        return;
    }

    for (int i=0; i<field_20_files_pending_loading.Size(); i++)
    {
        ResourceManager_FileRecord_1C* pFileRec = field_20_files_pending_loading.ItemAt(i);
        if (!pFileRec)
        {
            break;
        }

        ResourcesToLoadList* pResourceToLoadList = pFileRec->field_4_pResourcesToLoadList;
        if (pResourceToLoadList)
        {
            if (pResourceToLoadList->field_0_count > 0)
            {
                bool found = false;
                for (int j = 0; j < pResourceToLoadList->field_0_count; j++)
                {
                    if (type == pResourceToLoadList->field_4_items[j].field_0_type && resourceID == pResourceToLoadList->field_4_items[j].field_4_res_id)
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    auto pNewFilePart = alive_new<ResourceManager_FilePartRecord_18>();
                    pNewFilePart->field_0_type = type;
                    pNewFilePart->field_10_pFn = pFn;
                    pNewFilePart->field_8_pCamera = pCamera;
                    pNewFilePart->field_14_bAddUseCount = bAddUseCount;
                    pNewFilePart->field_4_id = resourceID;
                    pNewFilePart->field_C_fn_arg_pCamera = pFnArg;
                    pFileRec->field_10_file_sections_dArray.Push_Back(pNewFilePart);
                    return;
                }
            }
        }
        else if (type == pFileRec->field_8_type && resourceID == pFileRec->field_C_id)
        {
            auto pNewFilePart = alive_new<ResourceManager_FilePartRecord_18>();
            pNewFilePart->field_0_type = type;
            pNewFilePart->field_10_pFn = pFn;
            pNewFilePart->field_8_pCamera = pCamera;
            pNewFilePart->field_14_bAddUseCount = bAddUseCount;
            pNewFilePart->field_4_id = resourceID;
            pNewFilePart->field_C_fn_arg_pCamera = pFnArg;
            pFileRec->field_10_file_sections_dArray.Push_Back(pNewFilePart);
            return;
        }
    }

    auto pNewFileRec = alive_new<ResourceManager_FileRecord_1C>();
    pNewFileRec->field_10_file_sections_dArray.ctor_40CA60(3);
    pNewFileRec->field_0_fileName = reinterpret_cast<char*>(malloc_non_zero_4954F0(strlen(pFileItem) + 1));
    strcpy(pNewFileRec->field_0_fileName, pFileItem);
    pNewFileRec->field_4_pResourcesToLoadList = 0;
    pNewFileRec->field_8_type = type;
    pNewFileRec->field_C_id = resourceID;

    auto pNewFilePart1 = alive_new<ResourceManager_FilePartRecord_18>();
    pNewFilePart1->field_8_pCamera = pCamera;
    pNewFilePart1->field_C_fn_arg_pCamera = pFnArg;
    pNewFilePart1->field_10_pFn = pFn;
    pNewFilePart1->field_0_type = type;
    pNewFilePart1->field_4_id = resourceID;
    pNewFilePart1->field_14_bAddUseCount = bAddUseCount;
    pNewFileRec->field_10_file_sections_dArray.Push_Back(pNewFilePart1);
    field_20_files_pending_loading.Push_Back(pNewFileRec);
}

void ResourceManager::LoadResourcesFromList_465150(const char* pFileName, ResourceManager::ResourcesToLoadList* pTypeAndIdList, Camera* pCamera, Camera* pFnArg, ResourceManager::TLoaderFn pFn, __int16 addUseCount)
{
    // Already loaded flag ??
    if (!(pTypeAndIdList->field_0_count & ~0x80000000))
    {
        return;
    }

    // Check if all resources are already loaded
    bool allResourcesLoaded = true;
    for (int i = 0; i < pTypeAndIdList->field_0_count; i++)
    {
        if (!GetLoadedResource_49C2A0(
            pTypeAndIdList->field_4_items[i].field_0_type,
            pTypeAndIdList->field_4_items[i].field_4_res_id,
            0, 0))
        {
            // A resource we need is missing
            allResourcesLoaded = false;
            break;
        }
    }

    // All resources that we required are already loaded
    if (allResourcesLoaded && pCamera) // mlg: Check if pCamera is null, so we can spawn objects easier.
    {
        for (int i = 0; i < pTypeAndIdList->field_0_count; i++)
        {
            // Add to array optionally bumping the ref count
            pCamera->field_0.Push_Back_40CAF0(GetLoadedResource_49C2A0(
                pTypeAndIdList->field_4_items[i].field_0_type,
                pTypeAndIdList->field_4_items[i].field_4_res_id,
                addUseCount,
                0));
        }
        return;
    }

    // Check if we already have a file record or not
    ResourceManager_FileRecord_1C* pFoundFileRecord = nullptr;
    for (int fileIdx1=0; fileIdx1 < field_20_files_pending_loading.Size(); fileIdx1++)
    {
        ResourceManager_FileRecord_1C* pFileRec = field_20_files_pending_loading.ItemAt(fileIdx1);
        if (!pFileRec)
        {
            break;
        }

        if (strcmp(pFileName, pFileRec->field_0_fileName) == 0)
        {
            pFoundFileRecord = pFileRec;
            break;
        }
    }

    // Create a new record or use the one we found
    auto pNewFileRec = pFoundFileRecord ? pFoundFileRecord : alive_new<ResourceManager_FileRecord_1C>();
    if (!pFoundFileRecord)
    {
        // Only do ctor stuff if we created a new record

        // TODO: De-inline ctor
        pNewFileRec->field_10_file_sections_dArray.ctor_40CA60(3);
        pNewFileRec->field_0_fileName = reinterpret_cast<char*>(malloc_non_zero_4954F0(strlen(pFileName) + 1));
        strcpy(pNewFileRec->field_0_fileName, pFileName);
        pNewFileRec->field_4_pResourcesToLoadList = pTypeAndIdList;
        pNewFileRec->field_8_type = 0;
        pNewFileRec->field_C_id = 0;
    }

    // Create a file part record for each item
    for (int i = 0; i < pTypeAndIdList->field_0_count; i++)
    {
        auto pNewFilePart = alive_new<ResourceManager_FilePartRecord_18>();
        pNewFilePart->field_0_type = pTypeAndIdList->field_4_items[i].field_0_type;
        pNewFilePart->field_4_id = pTypeAndIdList->field_4_items[i].field_4_res_id;
        pNewFilePart->field_8_pCamera = pCamera;
        pNewFilePart->field_10_pFn = pFn;
        pNewFilePart->field_C_fn_arg_pCamera = pFnArg;
        pNewFilePart->field_14_bAddUseCount = addUseCount;
        pNewFileRec->field_10_file_sections_dArray.Push_Back(pNewFilePart);
    }

    // Only add to array if we created it
    if (!pFoundFileRecord)
    {
        field_20_files_pending_loading.Push_Back(pNewFileRec);
    }
}

void ResourceManager::LoadResourceFile_465460(const char* filename, Camera* pCam, Camera* a4, ResourceManager::TLoaderFn pFn, __int16 a6)
{
    auto pFileRecord = alive_new<ResourceManager_FileRecord_1C>();
    if (pFileRecord)
    {
        // TODO: De-inline this ctor
        pFileRecord->field_10_file_sections_dArray.ctor_40CA60(3);
    }

    pFileRecord->field_0_fileName = reinterpret_cast<char*>(malloc_non_zero_4954F0(strlen(filename) + 1));
    strcpy(pFileRecord->field_0_fileName, filename);
    pFileRecord->field_4_pResourcesToLoadList = 0;
    pFileRecord->field_8_type = 0;
    pFileRecord->field_C_id = 0;

    auto pFilePart = alive_new<ResourceManager_FilePartRecord_18>();
    // TODO: De-inline this ctor
    pFilePart->field_8_pCamera = pCam;
    pFilePart->field_C_fn_arg_pCamera = a4;
    pFilePart->field_10_pFn = pFn;
    pFilePart->field_0_type = 0;
    pFilePart->field_4_id = 0;
    pFilePart->field_14_bAddUseCount = a6;
    // Add the part to the file
    pFileRecord->field_10_file_sections_dArray.Push_Back(pFilePart);

    // Add the file to the array
    field_20_files_pending_loading.Push_Back(pFileRecord);
}

void ResourceManager::LoadingLoop_465590(__int16 bShowLoadingIcon)
{
    while (!field_20_files_pending_loading.IsEmpty())
    {
        SYS_EventsPump_494580();
        VUpdate();  // vLoadFile_StateMachine_464A70 - process loading of files
        PSX_VSync_4F6170(0);
        const int ticks = dword_5C1BAC++ + 1;
        if (bShowLoadingIcon && !word_5C1BAA && ticks > 180)
        {
            // Render everything in the ordering table including the loading icon
            Game_ShowLoadingIcon_482D80();
        }
    }
}

void ResourceManager::Shutdown_465610()
{
    // Clear out every file in the files array
    auto pFilesArray = &field_20_files_pending_loading;
    __int16 fileIdx = 0;
    DynamicArrayIter iter;
    iter.field_0_pDynamicArray = pFilesArray;
    iter.field_4_idx = 0;
    while (fileIdx < pFilesArray->Size())
    {
        // Iterate and clear out the file parts in this item
        ResourceManager_FileRecord_1C* pFileRec = pFilesArray->ItemAt(fileIdx);
        iter.field_4_idx = fileIdx + 1;
        if (!pFileRec)
        {
            break;
        }

        auto pFileSectionsArray = &pFileRec->field_10_file_sections_dArray;
        __int16 fileSectionIdx = 0;
        DynamicArrayIter fileSectionsIter;
        fileSectionsIter.field_0_pDynamicArray = &pFileRec->field_10_file_sections_dArray;
        fileSectionsIter.field_4_idx = 0;

        while (fileSectionIdx < pFileSectionsArray->Size())
        {
            ResourceManager_FilePartRecord_18* pFileSection = pFileSectionsArray->ItemAt(fileSectionIdx);
            fileSectionsIter.field_4_idx = fileSectionIdx + 1;
            if (!pFileSection)
            {
                break;
            }
            fileSectionsIter.Remove_At_Iter_40CCA0();
            Mem_Free_495540(pFileSection);

            fileSectionIdx = fileSectionsIter.field_4_idx;
        }

        iter.Remove_At_Iter_40CCA0();
        Mem_Free_495560(pFileRec->field_0_fileName);

        pFileRec->dtor_464EA0();
        Mem_Free_495540(pFileRec);

        fileIdx = iter.field_4_idx;
    }
}

void ResourceManager::Free_Resources_For_Camera_4656F0(const Camera* pCamera)
{
    DynamicArrayIter filesIter;
    filesIter.field_0_pDynamicArray = &field_20_files_pending_loading;
    filesIter.field_4_idx = 0;
    while (filesIter.field_4_idx < filesIter.field_0_pDynamicArray->field_4_used_size)
    {
        ResourceManager_FileRecord_1C* pFileItem = field_20_files_pending_loading.ItemAt(filesIter.field_4_idx);
        filesIter.field_4_idx++;

        if (!pFileItem)
        {
            break;
        }

        if (pFileItem != field_2C_pFileItem)
        {
            DynamicArrayIter filePartsIter;
            filePartsIter.field_0_pDynamicArray = &pFileItem->field_10_file_sections_dArray;
            filePartsIter.field_4_idx = 0;
            while (filePartsIter.field_4_idx < pFileItem->field_10_file_sections_dArray.Size())
            {
                ResourceManager_FilePartRecord_18* pFilePartItem = pFileItem->field_10_file_sections_dArray.ItemAt(filePartsIter.field_4_idx);
                filePartsIter.field_4_idx++;
                if (!pFilePartItem)
                {
                    break;
                }

                if (pFilePartItem->field_C_fn_arg_pCamera == pCamera)
                {
                    filePartsIter.Remove_At_Iter_40CCA0();
                    Mem_Free_495540(pFilePartItem);
                }
            }

            if (pFileItem->field_10_file_sections_dArray.IsEmpty())
            {
                filesIter.Remove_At_Iter_40CCA0();
                Mem_Free_495560(pFileItem->field_0_fileName);
                pFileItem->dtor_464EA0();
                Mem_Free_495540(pFileItem);
            }
        }
    }
}

BaseGameObject* ResourceManager::VDestructor(signed int flags)
{
    return vdtor_4649B0(flags);
}

void ResourceManager::VUpdate()
{
    vLoadFile_StateMachine_464A70();
}

EXPORT void ResourceManager::VScreenChanged_464EC0()
{
    // Empty
}

void ResourceManager::VScreenChanged()
{
    VScreenChanged_464EC0();
}

ResourceManager::ResourceManager()
{

}

const DWORD kResHeapSize = 5120000;
ALIVE_ARY(1, 0x5D29F4, BYTE, kResHeapSize, sResourceHeap_5D29F4, {}); // Huge 5.4 MB static resource buffer

const DWORD kLinkedListArraySize = 375;
ALIVE_ARY(1, 0x5D1E30, ResourceManager::ResourceHeapItem, kLinkedListArraySize, sResourceLinkedList_5D1E30, {});

ALIVE_VAR(1, 0xAB49F8, BYTE*, spResourceHeapEnd_AB49F8, nullptr);

void CC ResourceManager::Init_49BCE0()
{
    for (int i = 1; i < kLinkedListArraySize - 1; i++)
    {
        sResourceLinkedList_5D1E30[i].field_0_ptr = nullptr;
        sResourceLinkedList_5D1E30[i].field_4_pNext = &sResourceLinkedList_5D1E30[i + 1];
    }

    sResourceLinkedList_5D1E30[kLinkedListArraySize - 1].field_4_pNext = nullptr;

    sResourceLinkedList_5D1E30[0].field_0_ptr = &sResourceHeap_5D29F4[sizeof(Header)];
    sResourceLinkedList_5D1E30[0].field_4_pNext = nullptr;

    Header* pHeader = Get_Header_49C410(&sResourceLinkedList_5D1E30[0].field_0_ptr);
    pHeader->field_0_size = kResHeapSize;
    pHeader->field_8_type = Resource_Free;

    sFirstLinkedListItem_5D29EC = &sResourceLinkedList_5D1E30[0];
    sSecondLinkedListItem_5D29E8 = &sResourceLinkedList_5D1E30[1];

    // TODO: Check this is correct
    spResourceHeapEnd_AB49F8 = &sResourceHeap_5D29F4[kResHeapSize - 1];
}

ResourceManager::ResourceHeapItem* CC ResourceManager::Push_List_Item_49BD70()
{
    auto old = sSecondLinkedListItem_5D29E8;
    sSecondLinkedListItem_5D29E8 = sSecondLinkedListItem_5D29E8->field_4_pNext;
    return old;
}

void CC ResourceManager::Pop_List_Item_49BD90(ResourceManager::ResourceHeapItem* pListItem)
{
    pListItem->field_0_ptr = nullptr;
    pListItem->field_4_pNext = sSecondLinkedListItem_5D29E8; // point to the current
    sSecondLinkedListItem_5D29E8 = pListItem; // set current to old
}

ResourceManager::ResourceHeapItem* CC ResourceManager::Split_block_49BDC0(ResourceManager::ResourceHeapItem* pItem, int size)
{
    Header* pToSplit = Get_Header_49C410(&pItem->field_0_ptr);
    const unsigned int sizeForNewRes = pToSplit->field_0_size - size;
    if (sizeForNewRes >= sizeof(Header))
    {
        ResourceHeapItem* pNewListItem = ResourceManager::Push_List_Item_49BD70();
        pNewListItem->field_4_pNext = pItem->field_4_pNext; // New item points to old
        pItem->field_4_pNext = pNewListItem; // Old item points to new

        pNewListItem->field_0_ptr = pItem->field_0_ptr + size; // Point the split point

        // Init header of split item
        Header* pHeader = Get_Header_49C410(&pNewListItem->field_0_ptr);
        pHeader->field_0_size = sizeForNewRes;
        pHeader->field_8_type = Resource_Free;
        pHeader->field_4_ref_count = 0;
        pHeader->field_C_id = 0;

        // Update old size
        pToSplit->field_0_size = size;
    }

    return pItem;
}

int CC ResourceManager::SEQ_HashName_49BE30(const char* seqFileName)
{
    // Clamp max len
    size_t seqFileNameLength = strlen(seqFileName) - 1;
    if (seqFileNameLength > 8)
    {
        seqFileNameLength = 8;
    }

    // Iterate each char to calculate hash
    DWORD hashId = 0;
    for (size_t index = 0; index < seqFileNameLength; index++)
    {
        char letter = seqFileName[index];
        if (letter == '.')
        {
            break;
        }

        const DWORD temp = 10 * hashId;
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

BYTE** ResourceManager::Alloc_New_Resource_Impl(DWORD type, DWORD id, DWORD size, bool locked, ResourceManager::BlockAllocMethod allocType)
{
    BYTE** ppNewRes = Allocate_New_Block_49BFB0(size + sizeof(Header), allocType);
    if (!ppNewRes)
    {
        // Failed, try to reclaim some memory and try again.
        Reclaim_Memory_49C470(0);
        ppNewRes = Allocate_New_Block_49BFB0(size + sizeof(Header), allocType);
    }

    if (ppNewRes)
    {
        Header* pHeader = Get_Header_49C410(ppNewRes);
        pHeader->field_8_type = type;
        pHeader->field_C_id = id;
        pHeader->field_4_ref_count = 1;
        pHeader->field_6_flags = locked ? ResourceHeaderFlags::eLocked : 0;
    }

    return ppNewRes;
}

BYTE** CC ResourceManager::Alloc_New_Resource_49BED0(DWORD type, DWORD id, DWORD size)
{
    return Alloc_New_Resource_Impl(type, id, size, false, BlockAllocMethod::eFirstMatching);
}

BYTE** CC ResourceManager::Allocate_New_Locked_Resource_49BF40(DWORD type, DWORD id, DWORD size)
{
    return Alloc_New_Resource_Impl(type, id, size, true, BlockAllocMethod::eLastMatching);
}

BYTE** CC ResourceManager::Allocate_New_Block_49BFB0(int sizeBytes, BlockAllocMethod allocMethod)
{
    ResourceHeapItem* pListItem = sFirstLinkedListItem_5D29EC;
    ResourceHeapItem* pHeapMem = nullptr;
    const unsigned int size = (sizeBytes + 3) & ~3u; // Rounding ??
    Header* pHeaderToUse = nullptr;
    while (pListItem)
    {
        // Is it a free block?
        Header* pResHeader = Get_Header_49C410(&pListItem->field_0_ptr);
        if (pResHeader->field_8_type == Resource_Free)
        {
            // Keep going till we hit a block that isn't free
            for (ResourceHeapItem* i = pListItem->field_4_pNext; i; i = pListItem->field_4_pNext)
            {
                Header* pHeader = Get_Header_49C410(&i->field_0_ptr);
                if (pHeader->field_8_type != Resource_Free)
                {
                    break;
                }

                // Combine up the free blocks
                pResHeader->field_0_size += pHeader->field_0_size;
                pListItem->field_4_pNext = i->field_4_pNext;
                Pop_List_Item_49BD90(i);
            }

            // Size will be bigger now that we've freed at least 1 resource
            if (pResHeader->field_0_size >= size)
            {
                switch (allocMethod)
                {
                case BlockAllocMethod::eFirstMatching:
                    // Use first matching item
                    sManagedMemoryUsedSize_AB4A04 += size;
                    if (sManagedMemoryUsedSize_AB4A04 >= sPeakedManagedMemUsage_AB4A08)
                    {
                        sPeakedManagedMemUsage_AB4A08 = sManagedMemoryUsedSize_AB4A04;
                    }
                    return &Split_block_49BDC0(pListItem, size)->field_0_ptr;
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
        sAllocationFailed_AB4A0C = 1;
        return nullptr;
    }

    sManagedMemoryUsedSize_AB4A04 += size;
    if (sManagedMemoryUsedSize_AB4A04 >= sPeakedManagedMemUsage_AB4A08)
    {
        sPeakedManagedMemUsage_AB4A08 = sManagedMemoryUsedSize_AB4A04;
    }

    switch (allocMethod)
    {
    // Note: eFirstMatching case not possible here as pHeapMem case would have early returned
    case BlockAllocMethod::eNearestMatching:
        return &ResourceManager::Split_block_49BDC0(pHeapMem, size)->field_0_ptr;

    case BlockAllocMethod::eLastMatching:
        if (pHeaderToUse->field_0_size - size >= sizeof(Header))
        {
            return &Split_block_49BDC0(pHeapMem, pHeaderToUse->field_0_size - size)->field_4_pNext->field_0_ptr;
        }
        else
        {
            // No need to split as the size must be exactly the size of a resource header
            return &pHeapMem->field_0_ptr;
        }
        break;

    default: return nullptr;
    }

    // Should be impossible to get here
    return nullptr;
}

int CC ResourceManager::LoadResourceFile_49C130(const char* filename, TLoaderFn pFn, Camera* a4, Camera* pCamera)
{
    pResourceManager_5C1BB0->LoadResourceFile_465460(filename, pCamera, a4, pFn, pCamera != nullptr);
    return 0;
}

signed __int16 CC ResourceManager::LoadResourceFile_49C170(const char* pFileName, Camera* pCamera)
{
    pResourceManager_5C1BB0->LoadResourceFile_465460(pFileName, pCamera, nullptr, nullptr, pCamera != nullptr);
    pResourceManager_5C1BB0->LoadingLoop_465590(0);
    return 1;
}

signed __int16 CC ResourceManager::Move_Resources_To_DArray_49C1C0(BYTE** ppRes, DynamicArray* pArray)
{
    ResourceHeapItem* pItemToAdd = (ResourceHeapItem *)ppRes;
    Header* pHeader = Get_Header_49C410(&pItemToAdd->field_0_ptr);
    if (pHeader->field_8_type != Resource_End)
    {
        // While we're not at the end resource and the pointer/size are a multiple of 4? (how can they not be when they are all allocated aligned??)
        while (pHeader->field_8_type != Resource_Pend && pHeader->field_0_size && !(pHeader->field_0_size & 3))
        {
            if (pArray)
            {
                pArray->Push_Back_40CAF0(pItemToAdd);
            }

            pHeader = (Header *)((char *)pHeader + pHeader->field_0_size);
            
            // Out of heap space
            if (pHeader->field_0_size >= kResHeapSize)
            {
                return 1;
            }

            ResourceHeapItem* pNewListItem = ResourceManager::Push_List_Item_49BD70();
            pNewListItem->field_4_pNext = pItemToAdd->field_4_pNext;
            pItemToAdd->field_4_pNext = pNewListItem;
            pNewListItem->field_0_ptr = (BYTE*)&pHeader[1];// point after header
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
            pHeader->field_0_size = static_cast<DWORD>(pItemToAdd->field_4_pNext->field_0_ptr - (BYTE*)pHeader - sizeof(Header));
        }
        else
        {
            // Isn't a next item so use ptr to end of heap - location of current res
            // TODO: 64bit warning
            pHeader->field_0_size = static_cast<DWORD>(spResourceHeapEnd_AB49F8 - (BYTE *)pHeader);
        }
        sManagedMemoryUsedSize_AB4A04 -= pHeader->field_0_size;
    }
    return 1;
}

BYTE** CC ResourceManager::GetLoadedResource_49C2A0(DWORD type, DWORD resourceID, unsigned __int16 addUseCount, unsigned __int16 bLock)
{
    // Iterate all list items
    ResourceHeapItem* pListIter = sFirstLinkedListItem_5D29EC;
    while (pListIter)
    {
        // Find something that matches the type and resource ID
        Header* pResHeader = Get_Header_49C410(&pListIter->field_0_ptr);
        if (pResHeader->field_8_type == type && pResHeader->field_C_id == resourceID)
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

void CC ResourceManager::Inc_Ref_Count_49C310(BYTE** ppRes)
{
    Get_Header_49C410(ppRes)->field_4_ref_count++;
}

signed __int16 CC ResourceManager::FreeResource_49C330(BYTE** handle)
{
    if (!handle)
    {
        return 1;
    }
    return FreeResource_Impl_49C360(*handle);
}

signed __int16 CC ResourceManager::FreeResource_Impl_49C360(BYTE* handle)
{
    if (handle)
    {
        Header* pHeader = Get_Header_49C410(&handle);
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
            sManagedMemoryUsedSize_AB4A04 -= pHeader->field_0_size;
        }
    }
    return 1;
}

ResourceManager::Header* CC ResourceManager::Get_Header_49C410(BYTE** ppRes)
{
    return reinterpret_cast<Header*>((*ppRes - sizeof(Header)));
}

void CC ResourceManager::Reclaim_Memory_49C470(unsigned int sizeToReclaim)
{
    if (sResources_Pending_Loading_AB49F4 != 0)
    {
        return;
    }

    // If we failed to allocate a block or no size was passed then attempt to reclaim the whole heap
    if (sAllocationFailed_AB4A0C || sizeToReclaim == 0)
    {
        sizeToReclaim = kResHeapSize;
        sAllocationFailed_AB4A0C = 0;
    }

    ResourceHeapItem* pListItem = sFirstLinkedListItem_5D29EC;
    ResourceHeapItem* pToUpdate = nullptr;

    while (pListItem)
    {
        Header* pCurrentHeader = Get_Header_49C410(&pListItem->field_0_ptr);
        if (pCurrentHeader->field_8_type == Resource_Free)
        {
            ResourceHeapItem* pNext = pListItem->field_4_pNext;
            if (!pNext)
            {
                return;
            }

            Header* pNextHeader = Get_Header_49C410(&pNext->field_0_ptr);
            if (pNextHeader->field_8_type == Resource_Free)
            {
                // Next block is also free, so we can merge them together
                ResourceHeapItem* pToRemove = pListItem->field_4_pNext;
                pCurrentHeader->field_0_size += pNextHeader->field_0_size;
                pListItem->field_4_pNext = pNext->field_4_pNext;
                Pop_List_Item_49BD90(pToRemove);
            }
            else
            {
                unsigned int sizeToMove = 0;
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
                    const DWORD savedSize = pCurrentHeader->field_0_size;
                    BYTE* pDataStart = pNext->field_0_ptr - sizeof(Header);
                    if (sizeToMove > 0)
                    {
                        const size_t offset = (char *)pCurrentHeader - (char *)pNextHeader;
                        memmove(pDataStart + offset, pDataStart, sizeToMove);
                    }

                    // Get resource header after the current one
                    Header* pNextResHeader = (Header *)((char *)pCurrentHeader + pCurrentHeader->field_0_size);
                    pNextResHeader->field_0_size = savedSize;
                    pNextResHeader->field_8_type = Resource_Free;
                    
                    pNext->field_0_ptr = (BYTE*)&pCurrentHeader[1]; // Data starts after header
                    pListItem->field_0_ptr = (BYTE*)&pNextResHeader[1]; // Data starts after header
                    pListItem->field_4_pNext = pNext->field_4_pNext;
                    pNext->field_4_pNext = pListItem;

                    if (pToUpdate)
                    {
                        pToUpdate->field_4_pNext = pNext;
                    }
                    else
                    {
                        sFirstLinkedListItem_5D29EC = pNext;
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

void CC ResourceManager::Increment_Pending_Count_49C5F0()
{
    ++sResources_Pending_Loading_AB49F4;
}
void CC ResourceManager::Decrement_Pending_Count_49C610()
{
    --sResources_Pending_Loading_AB49F4;
}

void CC ResourceManager::Set_Header_Flags_49C650(BYTE** ppRes, __int16 flags)
{
    Get_Header_49C410(ppRes)->field_6_flags |= flags;
}

void CC ResourceManager::Free_Resource_Of_Type_49C6B0(DWORD type)
{
    ResourceHeapItem* pListItem = sFirstLinkedListItem_5D29EC;
    while (pListItem)
    {
        // Free it if the type matches and its not flagged as never free
        Header* pHeader = Get_Header_49C410(&pListItem->field_0_ptr);
        if (pHeader->field_8_type == type && !(pHeader->field_6_flags & ResourceHeaderFlags::eNeverFree))
        {
            pHeader->field_8_type = Resource_Free;
            pHeader->field_6_flags = 0;
            pHeader->field_4_ref_count = 0;

            sManagedMemoryUsedSize_AB4A04 -= pHeader->field_0_size;
        }
        pListItem = pListItem->field_4_pNext;
    }
}

void CC ResourceManager::NoEffect_49C700()
{
     // NOTE: Does nothing because the real func just seems to try to tally 
     // up some sort of stat that is never used.
}
