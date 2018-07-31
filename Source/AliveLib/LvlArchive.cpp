#include "stdafx.h"
#include "LvlArchive.hpp"
#include "Function.hpp"
#include "Psx.hpp"

const static int kSectorSize = 2048;

ALIVE_VAR(1, 0x5CA4B0, BOOL, sbEnable_PCOpen_5CA4B0, FALSE);
ALIVE_VAR(1, 0x5BC218, int, sWrappingFileIdx_5BC218, 0);
ALIVE_VAR(1, 0x551D28, int, sTotalOpenedFilesCount_551D28, 3); // Starts at 3.. for some reason
ALIVE_ARY(1, 0x5BC220, LvlFileRecord, 32, sOpenFileNames_5BC220, {});

ALIVE_VAR(1, 0x5BC520, LvlArchive, sLvlArchive_5BC520, {});
ALIVE_VAR(1, 0x5C3110, LvlArchive, stru_5C3110, {});

EXPORT int CC File_pc_open_4FA2C0(const char* fileName, int mode)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC File_seek_4FA490(int hFile, int distance, int method)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC File_close_4FA530(int hFile)
{
    NOT_IMPLEMENTED();
    return 0;
}

int LvlArchive::Read_File_433070(const char* pFileName, void* pBuffer)
{
    return Read_File_4330A0(Find_File_Record_433160(pFileName), pBuffer);
}

int LvlArchive::Read_File_4330A0(LvlFileRecord* hFile, void* pBuffer)
{
    if (!hFile || !pBuffer)
    {
        return 0;
    }

    CdlLOC cdLoc = {};
    PSX_Pos_To_CdLoc_4FADD0(field_4 + hFile->field_C_start_sector, &cdLoc);
    PSX_CD_File_Seek_4FB1E0(2, &cdLoc);

    int bOK = PSX_CD_File_Read_4FB210(hFile->field_10_num_sectors, pBuffer);
    if (PSX_CD_FileIOWait_4FB260(0) == -1)
    {
        bOK = 0;
    }
    return bOK;
}

int LvlArchive::Free_433130()
{
    // Strangely the emulated CD file isn't closed, but the next CD open file will close it anyway..
    if (field_0_0x2800_res.Valid())
    {
        ResourceManager::FreeResource_49C330(field_0_0x2800_res.RawResource());
        field_0_0x2800_res.Clear();
    }
    return 0;
}

int LvlArchive::Open_Archive_432E80(const char* fileName)
{
    // Allocate space for LVL archive header
    field_0_0x2800_res = ResourceManager::Allocate_New_Block_49BFB0_T<LvlHeader_Sub*>(kSectorSize * 5, ResourceManager::BlockAllocMethod::eFirstMatching);

    // Open the LVL file
    int hFile = PSX_CD_OpenFile_4FAE80(fileName, 1);
    if (!hFile)
    {
        return 0;
    }

    // Not sure why any of this is required, doesn't really do anything.
    CdlLOC cdLoc = {};
    PSX_Pos_To_CdLoc_4FADD0(0, &cdLoc);
    field_4= PSX_CdLoc_To_Pos_4FAE40(&cdLoc);
    PSX_CD_File_Seek_4FB1E0(2, &cdLoc);

    // Read the header
    ResourceManager::Header* pResHeader = field_0_0x2800_res.GetHeader();
    int bOk = PSX_CD_File_Read_4FB210(5, pResHeader);
    if (PSX_CD_FileIOWait_4FB260(0) == -1)
    {
        bOk = 0;
    }

    // Set ref count to 1 so ResourceManager won't kill it
    pResHeader->field_4_ref_count = 1;
    return bOk;
}

LvlFileRecord* LvlArchive::Find_File_Record_433160(const char* pFileName)
{
    const unsigned int fileNameLen = strlen(pFileName) + 1;

    const bool notEnoughSpaceForFileExt = (static_cast<signed int>(fileNameLen) - 1) < 4;
    if (notEnoughSpaceForFileExt || _strcmpi(&pFileName[fileNameLen - 5], ".STR") != 0) // Check its not a STR file
    {
        if (sbEnable_PCOpen_5CA4B0)
        {
            const int hFile = File_pc_open_4FA2C0(pFileName, 0);
            if (hFile >= 0)
            {
                const int idx = sWrappingFileIdx_5BC218++ & 31;
                strcpy(sOpenFileNames_5BC220[idx].field_0_file_name, pFileName);
                sOpenFileNames_5BC220[idx].field_C_start_sector = 0;
                sOpenFileNames_5BC220[idx].field_14_file_size = File_seek_4FA490(hFile, 0, 2);
                sOpenFileNames_5BC220[idx].field_10_num_sectors = (unsigned int)(sOpenFileNames_5BC220[idx].field_14_file_size + kSectorSize - 1) >> 11;
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

    if (!field_0_0x2800_res->field_0_num_files)
    {
        return nullptr;
    }

    int fileRecordIndex = 0;
    while (strncmp(field_0_0x2800_res->field_10_file_recs[fileRecordIndex].field_0_file_name, pFileName, _countof(LvlFileRecord::field_0_file_name)))
    {
        fileRecordIndex++;
        if (fileRecordIndex >= field_0_0x2800_res->field_0_num_files)
        {
            LOG_ERROR("Couldn't find " << pFileName << " in LVL");
            return nullptr;
        }
    }
    return &field_0_0x2800_res->field_10_file_recs[fileRecordIndex];
}
