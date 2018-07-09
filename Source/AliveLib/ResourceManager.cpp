#include "stdafx.h"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

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

    field_4_typeId = 79;
    field_2C_pFileItem = 0;
    field_30_start_sector = 0;
    field_34_num_sectors = 0;
    field_38_ppRes = 0;
    field_3C_pLoadingHeader = 0;
    field_42_state = 0;
    field_40_seek_attempts = 0;
}

void ResourceManager::Shutdown_465610()
{
    NOT_IMPLEMENTED();
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