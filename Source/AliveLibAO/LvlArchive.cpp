#include "stdafx_ao.h"
#include "LvlArchive.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"

START_NS_AO

ALIVE_VAR(1, 0x4FFD60, LvlArchive, sLvlArchive_4FFD60, {});
ALIVE_VAR(1, 0x507C90, LvlArchive, stru_507C90, {});

const static int kSectorSize = 2048;

EXPORT void CC LvlArchive::ctor_static_41BBA0()
{
    atexit(dtor_static_41BBB0);
}

EXPORT void CC LvlArchive::dtor_static_41BBB0()
{
    if (sLvlArchive_4FFD60.field_0_0x2800_res)
    {
        ResourceManager::FreeResource_455550(sLvlArchive_4FFD60.field_0_0x2800_res);
        sLvlArchive_4FFD60.field_0_0x2800_res = 0;
    }
}

EXPORT void CC LvlArchive::ctor_static_443E70()
{
    atexit(dtor_static_443E80);
}


EXPORT void CC LvlArchive::dtor_static_443E80()
{
    stru_507C90.Free_41BEB0();
}

EXPORT void LvlArchive::OpenArchive_41BC60(int pos)
{
    // Allocate space for LVL archive header
    field_0_0x2800_res = ResourceManager::Allocate_New_Block_454FE0(kSectorSize * 5, ResourceManager::eFirstMatching);

    // Header data is populated later by the read, pointing to same buffer here
    ResourceManager::Header* pHeader = ResourceManager::Get_Header_455620(field_0_0x2800_res);

    field_4_cd_pos = pos;

    int retryCounter = 0;
    CdlLOC loc = {};
    PSX_Pos_To_CdLoc_49B340(pos, &loc);
    int bOk = 0;
    do
    {
        retryCounter++; // Left over from psx - try to re read CD on failure?
        if (retryCounter > 32)
        {
            LOG_ERROR("Exit in OpenArchive_41BC60");
            exit(-32);
        }
        PSX_CD_File_Seek_49B670(2, &loc);

        // OG BUG: Header assumed to be 5 sectors, if its bigger then we are doomed
        bOk = PSX_CD_File_Read_49B8B0(5, pHeader);
        if (PSX_CD_FileIOWait_49B900(0) == -1)
        {
            bOk = 0;
        }
    } while (!bOk);

    // Set ref count to 1 so ResourceManager won't kill it
    pHeader->field_4_ref_count = 1;
}

EXPORT __int16 LvlArchive::Free_41BEB0()
{
    if (field_0_0x2800_res)
    {
        ResourceManager::FreeResource_455550(field_0_0x2800_res);
        field_0_0x2800_res = nullptr;
    }
    return 0;
}

EXPORT LvlFileRecord* LvlArchive::Find_File_Record_41BED0(const char* pFileName)
{
    // NOTE: PcOpen branches removed

    auto pHeader = reinterpret_cast<LvlHeader_Sub*>(*field_0_0x2800_res);
    if (pHeader->field_0_num_files == 0)
    {
        LOG_ERROR("Couldn't find " << pFileName << " in LVL because the LVL is empty");
        assert(false);
        return nullptr;
    }

    const int total = pHeader->field_0_num_files;
    for (int i = 0; i < total; i++)
    {
        if (strncmp(pHeader->field_10_file_recs[i].field_0_file_name, pFileName, ALIVE_COUNTOF(LvlFileRecord::field_0_file_name)) == 0)
        {
             return &pHeader->field_10_file_recs[i];
        }
    }

    LOG_WARNING("Couldn't find " << pFileName << " in LVL");
    return nullptr;
}

EXPORT __int16 LvlArchive::Read_File_41BE40(const LvlFileRecord* pFileRec, void* pBuffer)
{
    if (!pFileRec || !pBuffer)
    {
        return 0;
    }

    CdlLOC loc = {};
    PSX_Pos_To_CdLoc_49B340(pFileRec->field_C_start_sector + field_4_cd_pos, &loc);
    PSX_CD_File_Seek_49B670(2, &loc);
    __int16 ret = static_cast<short>(PSX_CD_File_Read_49B8B0(pFileRec->field_10_num_sectors, pBuffer));
    if (PSX_CD_FileIOWait_49B900(0) == -1)
    {
        ret = 0;
    }
    return ret;
}

END_NS_AO
