#include "stdafx.h"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "LvlArchive.hpp"

ALIVE_VAR(1, 0x5C1BB0, ResourceManager*, pResourceManager_5C1BB0, nullptr);

ALIVE_VAR(1, 0xab4a04, int, sManagedMemoryUsedSize_AB4A04, 0);
ALIVE_VAR(1, 0xab4a08, int, sPeakedManagedMemUsage_AB4A08, 0);

ALIVE_VAR(1, 0x5C1B96, short, word_5C1B96, 0);
ALIVE_VAR(1, 0x5C1BAA, short, word_5C1BAA, 0);
ALIVE_VAR(1, 0x5C1BAC, int, dword_5C1BAC, 0);
ALIVE_VAR(1, 0xAB49F4, short, sResources_Pending_Loading_AB49F4, 0);
ALIVE_VAR(1, 0xAB4A0C, short, word_AB4A0C, 0);

ALIVE_VAR(1, 0x5D29EC, ResourceManager::ResourceHeapItem*, sFirstLinkedListItem_5D29EC, nullptr);
ALIVE_VAR(1, 0x5D29E8, ResourceManager::ResourceHeapItem*, sSecondLinkedListItem_5D29E8, nullptr);


void ResourceManager::Ctor_464910()
{
    BaseGameObject_ctor_4DBFA0(1, 0);

    field_20_files_dArray.ctor_40CA60(3);
    field_48_dArray.ctor_40CA60(3);

    field_6_flags |= eBit08 | eUpdatableExtra; // TODO: Check me
    SetVTable(this, 0x545EBC); // vTbl_ResourceManager_545EBC

    field_4_typeId = Types::eResourceManager;
    field_2C_pFileItem = nullptr;
    field_30_start_sector = 0;
    field_34_num_sectors = 0;
    field_38_ppRes = {};
    field_3C_pLoadingHeader = nullptr;
    field_42_state = State_Wait_For_Load_Request;
    field_40_seek_attempts = 0;
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

// TODO: Move to own file
EXPORT void __stdcall sub_465BC0(int a1)
{
    NOT_IMPLEMENTED();
}

// TODO: Move to own file
EXPORT void CC Game_ShowLoadingIcon_482D80()
{
    NOT_IMPLEMENTED();
}

void ResourceManager::vLoadFile_StateMachine_464A70()
{
    switch (field_42_state)
    {
    case State_Wait_For_Load_Request:
        PSX_CD_File_Seek_4FB1E0(1, nullptr); // NOTE: Mode 1 does nothing
                                             // NOTE: Pruned branches here from stub that was hard coded to return 0
        if (!field_20_files_dArray.IsEmpty())
        {
            field_2C_pFileItem = field_20_files_dArray.ItemAt(0);

            LvlFileRecord* pLvlFileRec1 = sLvlArchive_5BC520.Find_File_Record_433160(field_2C_pFileItem->field_0_fileName);
            field_34_num_sectors = pLvlFileRec1->field_10_num_sectors;
            field_30_start_sector = pLvlFileRec1->field_C_start_sector + sLvlArchive_5BC520.field_4;
            PSX_Pos_To_CdLoc_4FADD0(field_30_start_sector, &field_44_cdLoc);

            word_5C1B96 = 1;
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
        ResourceManager::Move_Resources_To_DArray_49C1C0(field_38_ppRes, &field_48_dArray);
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

void ResourceManager::ResourceManager_FileRecord_1C::dtor_464EA0()
{
    field_10_file_sections_dArray.dtor_40CAD0();
}

void ResourceManager::sub_464EE0(const char* pFileItem, DWORD type, DWORD resourceID, Camera* pCamera, Camera* pFnArg, ResourceManager::TLoaderFn pFn, __int16 bAddUseCount)
{
    NOT_IMPLEMENTED();
}

void ResourceManager::sub_465150(const char *pFileName, ResourceManager::ResourcesToLoadList* pTypeAndIdList, Camera* pCamera, Camera* pFnArg, ResourceManager::TLoaderFn pFn, __int16 addUseCount)
{
    NOT_IMPLEMENTED();
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
    field_20_files_dArray.Push_Back(pFileRecord);
}

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

void ResourceManager::Free_Resources_For_Camera_4656F0(const Camera* pCamera)
{
    DynamicArrayIter filesIter;
    filesIter.field_0_pDynamicArray = &field_20_files_dArray;
    filesIter.field_4_idx = 0;
    while (filesIter.field_4_idx < filesIter.field_0_pDynamicArray->field_4_used_size)
    {
        ResourceManager_FileRecord_1C* pFileItem = field_20_files_dArray.ItemAt(filesIter.field_4_idx);
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

void ResourceManager::VDestructor(signed int flags)
{
    dtor_4649B0(flags);
}

ResourceManager::ResourceManager()
{
    Ctor_464910();
}

void CC ResourceManager::Init_49BCE0()
{
    NOT_IMPLEMENTED();
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
    pListItem->field_4_pNext = sSecondLinkedListItem_5D29E8; // point to the currrent
    sSecondLinkedListItem_5D29E8 = pListItem; // set current to old
}

BYTE** CC ResourceManager::Split_block_49BDC0(ResourceManager::ResourceHeapItem* pItem, int size)
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

    // TODO: Confirm return type is correct
    return reinterpret_cast<BYTE**>(pItem);
}

int CC ResourceManager::SEQ_HashName_49BE30(const char* seqFileName)
{
    DWORD hashId = 0;

    size_t seqFileNameLength = strlen(seqFileName) - 1;
    if (seqFileNameLength > 8)
    {
        seqFileNameLength = 8;
    }

    size_t index = 0;
    if (seqFileNameLength)
    {
        do
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
                hashId = index || *seqFileName != '0' ? temp + letter - '0' : temp + 9;
            }
            ++index;
        } while (index < seqFileNameLength);
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
                    if (sManagedMemoryUsedSize_AB4A04 >= (unsigned int)sPeakedManagedMemUsage_AB4A08)
                    {
                        sPeakedManagedMemUsage_AB4A08 = sManagedMemoryUsedSize_AB4A04;
                    }
                    return Split_block_49BDC0(pListItem, size);
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
        return nullptr;
    }

    sManagedMemoryUsedSize_AB4A04 += size;
    if (sManagedMemoryUsedSize_AB4A04 >= (unsigned int)sPeakedManagedMemUsage_AB4A08)
    {
        sPeakedManagedMemUsage_AB4A08 = sManagedMemoryUsedSize_AB4A04;
    }

    switch (allocMethod)
    {
    // Note: eFirstMatching case not possible here as pHeapMem case would have early returned
    case BlockAllocMethod::eNearestMatching:
        return ResourceManager::Split_block_49BDC0(pHeapMem, size);

    case BlockAllocMethod::eLastMatching:
        if (pHeaderToUse->field_0_size - size >= sizeof(Header))
        {
            // TODO: Return type == ??
            return (BYTE **)Split_block_49BDC0(pHeapMem, pHeaderToUse->field_0_size - size)[1];
        }
        else
        {
            // No need to split as the size must be exactly the size of a resource header
            // TODO: Return type == ??
            return (BYTE **)pHeapMem;
        }
        break;
    }

    // Allocation failure
    word_AB4A0C = 1;
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
    NOT_IMPLEMENTED();
    return 0;
}

void* CC ResourceManager::GetLoadedResource_49C2A0(DWORD type, DWORD resourceID, unsigned __int16 addUseCount, unsigned __int16 bLock)
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

            return pListIter;
        }

        pListIter = pListIter->field_4_pNext;
    }
    return nullptr;
}

void CC ResourceManager::Inc_Ref_Count_49C310(BYTE **ppRes)
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
    return  BaseHandle(ppRes).GetHeader();
}

void CC ResourceManager::Reclaim_Memory_49C470(int size)
{
    NOT_IMPLEMENTED();
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
