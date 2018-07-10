#include "stdafx.h"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "LvlArchive.hpp"

ALIVE_VAR(1, 0x5C1BB0, ResourceManager*, pResourceManager_5C1BB0, nullptr);
ALIVE_VAR(1, 0xab4a04, int, sManagedMemoryUsedSize_AB4A04, 0);

void ResourceManager::VDestructor(signed int flags)
{
    dtor_4649B0(flags);
}

void ResourceManager::dtor_4649B0(signed int flags)
{
    dtor_4649E0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void ResourceManager::dtor_4649E0()
{
    NOT_IMPLEMENTED();
}

ResourceManager::ResourceManager()
{
    Ctor_464910();
}

void ResourceManager::Ctor_464910()
{
    BaseGameObject_ctor_4DBFA0(1, 0);

    field_20_files_dArray.ctor_40CA60(3);
    field_48_dArray.ctor_40CA60(3);

    field_6_flags |= eBit08 | eUpdatableExtra; // TODO: Check me
    SetVTable(this, 0x545EBC); // vTbl_ResourceManager_545EBC

    field_4_typeId = Types::eResourceManager;
    field_2C_pFileItem = 0;
    field_30_start_sector = 0;
    field_34_num_sectors = 0;
    field_38_ppRes = {};
    field_3C_pLoadingHeader = 0;
    field_42_state = State_Wait_For_Load_Request;
    field_40_seek_attempts = 0;
}

void ResourceManager::Shutdown_465610()
{
    // Clear out every file in the files array
    auto pFilesArray = &field_20_files_dArray;
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

EXPORT void CC Game_ShowLoadingIcon_482D80()
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x5C1BAA, short, word_5C1BAA, 0);
ALIVE_VAR(1, 0x5C1BAC, int, dword_5C1BAC, 0);

void ResourceManager::LoadingLoop_465590(__int16 bShowLoadingIcon)
{
    while (!field_20_files_dArray.IsEmpty())
    {
        sub_494580();
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

signed __int16 CC ResourceManager::LoadResourceFile_49C170(const char* pFileName, Camera* pCamera)
{
    pResourceManager_5C1BB0->LoadResourceFile_465460(pFileName, pCamera, nullptr, nullptr, pCamera != nullptr);
    pResourceManager_5C1BB0->LoadingLoop_465590(0);
    return 1;
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
    pFileRecord->field_4 = 0;
    pFileRecord->field_8 = 0;
    pFileRecord->field_C = 0;

    auto pFilePart = alive_new<ResourceManager_FilePartRecord_18>();
    // TODO: De-inline this ctor
    pFilePart->field_8_pCamera = pCam;
    pFilePart->field_C_fn_arg_pCamera = a4;
    pFilePart->field_10_pFn = pFn;
    pFilePart->field_0_type = 0;
    pFilePart->field_4_id = 0;
    pFilePart->field_14 = a6;
    // Add the part to the file
    pFileRecord->field_10_file_sections_dArray.Push_Back(pFilePart);

    // Add the file to the array
    field_20_files_dArray.Push_Back(pFileRecord);
}

ALIVE_VAR(1, 0x5C1B96, short, word_5C1B96, 0);

EXPORT void __stdcall sub_465BC0(int a1)
{
    NOT_IMPLEMENTED();
}

void ResourceManager::vLoadFile_StateMachine_464A70()
{
    switch (field_42_state)
    {
    case State_Wait_For_Load_Request:
        PSX_CD_File_Seek_4FB1E0(1, 0); // NOTE: Mode 1 does nothing
        // NOTE: Pruned branches here from stub that was hard coded to return 0
        if (!field_20_files_dArray.IsEmpty())
        {
            field_2C_pFileItem = field_20_files_dArray.ItemAt(0);

            LvlFileRecord* pLvlFileRec1 = sLvlArchive_5BC520.Find_File_Record_433160(field_2C_pFileItem->field_0_fileName);
            field_34_num_sectors = pLvlFileRec1->field_10_num_sectors;
            field_30_start_sector = pLvlFileRec1->field_C_start_sector + sLvlArchive_5BC520.field_4[0];
            PSX_Pos_To_CdLoc_4FADD0(field_30_start_sector, &field_44_cdLoc);

            word_5C1B96 = 1;
            field_42_state = State_Allocate_Memory_For_File;
        }
        break;

    case State_Allocate_Memory_For_File:
        field_38_ppRes = ResourceManager::Allocate_New_Block_49BFB0_T<void*>(field_34_num_sectors << 11, 0);
        if (field_38_ppRes.Valid())
        {
            field_3C_pLoadingHeader = field_38_ppRes.GetHeader();
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
            int attempts = field_40_seek_attempts;
            if (attempts < 20u)
            {
                field_40_seek_attempts = attempts + 1;
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
            // Note: Skipping State_Wait_For_Read_Complete till next tick
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
        ResourceManager::sub_49C1C0(field_38_ppRes, &field_48_dArray);
        field_42_state = State_Load_Completed;
        break;

    case State_Load_Completed:
        word_5C1B96 = 0;
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
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0xAB49F4, short, sResources_Pending_Loading_AB49F4, 0);

void CC ResourceManager::Decrement_Pending_Count_49C610()
{
    --sResources_Pending_Loading_AB49F4;
}

void CC ResourceManager::Increment_Pending_Count_49C5F0()
{
    ++sResources_Pending_Loading_AB49F4;
}

void CC ResourceManager::Reclaim_Memory_49C470(int size)
{
    NOT_IMPLEMENTED();
}

signed __int16 CC ResourceManager::sub_49C1C0(Handle<void*> ppRes, DynamicArray* pArray)
{
    NOT_IMPLEMENTED();
    return 0;
}

void* CC ResourceManager::GetLoadedResource_49C2A0(DWORD type, int resourceID, unsigned __int16 addUseCount, __int16 a4)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

signed __int16 CC ResourceManager::FreeResource_49C330(BaseHandle handle)
{
    if (!handle.Valid())
    {
        return 1;
    }
    return FreeResource_Impl_49C360(handle.ToRawHandle());
}

signed __int16 CC ResourceManager::FreeResource_Impl_49C360(RawHandle handle)
{
    if (handle.Valid())
    {
        Header* pHeader = handle.GetHeader();
        if (pHeader->field_4_ref_count)
        {
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

BYTE** CC ResourceManager::Allocate_New_Block_49BFB0(int sizeBytes, int allocMethod)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

EXPORT void ResourceManager::ResourceManager_FileRecord_1C::dtor_464EA0()
{
    field_10_file_sections_dArray.dtor_40CAD0();
}
