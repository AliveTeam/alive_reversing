#include "stdafx.h"
#include "LvlArchive.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include <assert.h>

const static s32 kSectorSize = 2048;

ALIVE_VAR(1, 0x5CA4B0, Bool32, sbEnable_PCOpen_5CA4B0, FALSE);
ALIVE_VAR(1, 0x5BC218, s32, sWrappingFileIdx_5BC218, 0);
ALIVE_VAR(1, 0x551D28, s32, sTotalOpenedFilesCount_551D28, 3); // Starts at 3.. for some reason
ALIVE_ARY(1, 0x5BC220, LvlFileRecord, 32, sOpenFileNames_5BC220, {});

ALIVE_VAR(1, 0x5BC520, LvlArchive, sLvlArchive_5BC520, {});
ALIVE_VAR(1, 0x5C3110, LvlArchive, stru_5C3110, {});

EXPORT void CC static_lvl_destruct_4803B0()
{
    stru_5C3110.Free_433130();
}

EXPORT void CC static_lvl_init_480350()
{
    //static_lvl_ctor_nullsub_49();
    atexit(static_lvl_destruct_4803B0);
}

EXPORT void CC LvlArchive_static_dtor_432E60()
{
    sLvlArchive_5BC520.Free_433130();
}

EXPORT void CC LvlArchive_Static_init_432E00()
{
    atexit(LvlArchive_static_dtor_432E60);
}

EXPORT s32 CC File_pc_open_4FA2C0(const char_type* /*fileName*/, s32 /*mode*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT s32 CC File_seek_4FA490(s32 /*hFile*/, s32 /*distance*/, s32 /*method*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT s32 CC File_close_4FA530(s32 /*hFile*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

s32 LvlArchive::Read_File_433070(const char_type* pFileName, void* pBuffer)
{
    return Read_File_4330A0(Find_File_Record_433160(pFileName), pBuffer);
}

s32 LvlArchive::Read_File_4330A0(LvlFileRecord* hFile, void* pBuffer)
{
    if (!hFile || !pBuffer)
    {
        return 0;
    }

    CdlLOC cdLoc = {};
    PSX_Pos_To_CdLoc_4FADD0(field_4_cd_pos + hFile->field_C_start_sector, &cdLoc);
    PSX_CD_File_Seek_4FB1E0(2, &cdLoc);

    s32 bOK = PSX_CD_File_Read_4FB210(hFile->field_10_num_sectors, pBuffer);
    if (PSX_CD_FileIOWait_4FB260(0) == -1)
    {
        bOK = 0;
    }
    return bOK;
}

s32 LvlArchive::Free_433130()
{
    // Strangely the emulated CD file isn't closed, but the next CD open file will close it anyway..
    if (field_0_0x2800_res)
    {
        ResourceManager::FreeResource_49C330(field_0_0x2800_res);
        field_0_0x2800_res = nullptr;
    }
    return 0;
}

s32 LvlArchive::Open_Archive_432E80(const char_type* fileName)
{
    // Allocate space for LVL archive header
    field_0_0x2800_res = ResourceManager::Allocate_New_Block_49BFB0(kSectorSize * 5, ResourceManager::BlockAllocMethod::eFirstMatching);

    // Open the LVL file

#if BEHAVIOUR_CHANGE_SUB_DATA_FOLDERS
    char_type subdirPath[256];
    strcpy(subdirPath, "levels");
    strcat(subdirPath, fileName);
    s32 hFile = PSX_CD_OpenFile_4FAE80(subdirPath, 1);

    if (!hFile)
    {
        hFile = PSX_CD_OpenFile_4FAE80(fileName, 1);
    }
#else
    s32 hFile = PSX_CD_OpenFile_4FAE80(fileName, 1);
#endif

    if (!hFile)
    {
        return 0;
    }

    // Not sure why any of this is required, doesn't really do anything.
    CdlLOC cdLoc = {};
    PSX_Pos_To_CdLoc_4FADD0(0, &cdLoc);
    field_4_cd_pos = PSX_CdLoc_To_Pos_4FAE40(&cdLoc);
    PSX_CD_File_Seek_4FB1E0(2, &cdLoc);

    // Read the header
    ResourceManager::Header* pResHeader = ResourceManager::Get_Header_49C410(field_0_0x2800_res);

    // OG BUG: Header assumed to be 5 sectors, if its bigger then we are doomed
    s32 bOk = PSX_CD_File_Read_4FB210(5, pResHeader);
    if (PSX_CD_FileIOWait_4FB260(0) == -1)
    {
        bOk = 0;
    }

    // Set ref count to 1 so ResourceManager won't kill it
    pResHeader->field_4_ref_count = 1;
    return bOk;
}

LvlFileRecord* LvlArchive::Find_File_Record_433160(const char_type* pFileName)
{
    const u32 fileNameLen = static_cast<u32>(strlen(pFileName) + 1);

    const bool notEnoughSpaceForFileExt = (static_cast<s32>(fileNameLen) - 1) < 4;
    if (notEnoughSpaceForFileExt || _strcmpi(&pFileName[fileNameLen - 5], ".STR") != 0) // Check its not a STR file
    {
        if (sbEnable_PCOpen_5CA4B0)
        {
            const s32 hFile = File_pc_open_4FA2C0(pFileName, 0);
            if (hFile >= 0)
            {
                const s32 idx = sWrappingFileIdx_5BC218++ & 31;
                strcpy(sOpenFileNames_5BC220[idx].field_0_file_name, pFileName);
                sOpenFileNames_5BC220[idx].field_C_start_sector = 0;
                sOpenFileNames_5BC220[idx].field_14_file_size = File_seek_4FA490(hFile, 0, 2);
                sOpenFileNames_5BC220[idx].field_10_num_sectors = (u32)(sOpenFileNames_5BC220[idx].field_14_file_size + kSectorSize - 1) >> 11;
                File_close_4FA530(hFile);
                return &sOpenFileNames_5BC220[idx];
            }
            return nullptr;
        }
    }
    else
    {
        ++sTotalOpenedFilesCount_551D28;
    }

    auto pHeader = reinterpret_cast<LvlHeader_Sub*>(*field_0_0x2800_res);
    if (!pHeader->field_0_num_files)
    {
        LOG_ERROR("Couldn't find " << pFileName << " in LVL");
        assert(false);
        return nullptr;
    }

    s32 fileRecordIndex = 0;
    while (strncmp(pHeader->field_10_file_recs[fileRecordIndex].field_0_file_name, pFileName, ALIVE_COUNTOF(LvlFileRecord::field_0_file_name)))
    {
        fileRecordIndex++;
        if (fileRecordIndex >= pHeader->field_0_num_files)
        {
            LOG_ERROR("Couldn't find " << pFileName << " in LVL");
            //assert(false);
            return nullptr;
        }
    }
    return &pHeader->field_10_file_recs[fileRecordIndex];
}
