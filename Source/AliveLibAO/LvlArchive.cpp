#include "stdafx_ao.h"
#include "LvlArchive.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "../AliveLibAE/Psx.hpp" // AE lib hack
#include <assert.h>

namespace AO {

ALIVE_VAR(1, 0x4FFD60, LvlArchive, sLvlArchive_4FFD60, {});
ALIVE_VAR(1, 0x507C90, LvlArchive, stru_507C90, {});

const static s32 kSectorSize = 2048;

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

static s32 ReadFirstSector(s32 pos, u8* pSector)
{
    pos = 0; // AE lib hack

    CdlLOC loc = {};
    PSX_Pos_To_CdLoc_49B340(pos, &loc);

    PSX_CD_File_Seek_49B670(2, &loc);

    s32 bOk = PSX_CD_File_Read_49B8B0(1, pSector);
    if (PSX_CD_FileIOWait_49B900(0) == -1)
    {
        bOk = 0;
    }
    return bOk;
}

EXPORT void LvlArchive::OpenArchive(const char_type* fileName, s32 pos)
{
    // HACK: Added so that AE PSX emu lib works as we don't have a mapping of CDPos <> FileName in the AE emu
    // (it was a stupid idea so I guess they removed it in the next iteration)
    pos = PSX_CD_OpenFile(fileName, 1);

    // Read the fixed sized header
    u8 sector[2048] = {};
    if (!ReadFirstSector(pos, &sector[0]))
    {
        LOG_ERROR("Failed to read first 2048 bytes of " << fileName);
        exit(-32);
    }

    const auto pLvlHeader = reinterpret_cast<const LvlHeader*>(&sector[0]);

    // Allocate space for LVL archive header
    field_0_0x2800_res = ResourceManager::Allocate_New_Block_454FE0(kSectorSize * pLvlHeader->field_10_sub.field_4_header_size_in_sectors, ResourceManager::eFirstMatching);
    if (pLvlHeader->field_10_sub.field_4_header_size_in_sectors != 5)
    {
        LOG_INFO("Header size in sectors is " << pLvlHeader->field_10_sub.field_4_header_size_in_sectors);
    }

    // Header data is populated later by the read, pointing to same buffer here
    ResourceManager::Header* pHeader = ResourceManager::Get_Header_455620(field_0_0x2800_res);

    field_4_cd_pos = pos;

    s32 retryCounter = 0;
    CdlLOC loc = {};

    pos = 0; // AE lib hack

    PSX_Pos_To_CdLoc_49B340(pos, &loc);

    field_4_cd_pos = PSX_CdLoc_To_Pos_4FAE40(&loc); // AE lib hack

    s32 bOk = 0;
    do
    {
        retryCounter++; // Left over from psx - try to re read CD on failure?
        if (retryCounter > 32)
        {
            LOG_ERROR("Exit in OpenArchive_41BC60");
            exit(-32);
        }
        PSX_CD_File_Seek_49B670(2, &loc);

        bOk = PSX_CD_File_Read_49B8B0(pLvlHeader->field_10_sub.field_4_header_size_in_sectors, pHeader);
        if (PSX_CD_FileIOWait_49B900(0) == -1)
        {
            bOk = 0;
        }
    }
    while (!bOk);

    // Set ref count to 1 so ResourceManager won't kill it
    pHeader->field_4_ref_count = 1;
}

EXPORT void LvlArchive::OpenArchive_41BC60(s32 pos)
{
    OpenArchive(nullptr, pos);
}

EXPORT s16 LvlArchive::Free_41BEB0()
{
    if (field_0_0x2800_res)
    {
        ResourceManager::FreeResource_455550(field_0_0x2800_res);
        field_0_0x2800_res = nullptr;
    }
    return 0;
}

EXPORT LvlFileRecord* LvlArchive::Find_File_Record_41BED0(const char_type* pFileName)
{
    // NOTE: PcOpen branches removed

    auto pHeader = reinterpret_cast<LvlHeader_Sub*>(*field_0_0x2800_res);
    if (pHeader->field_0_num_files == 0)
    {
        LOG_ERROR("Couldn't find " << pFileName << " in LVL because the LVL is empty");
        assert(false);
        return nullptr;
    }

    const s32 total = pHeader->field_0_num_files;
    for (s32 i = 0; i < total; i++)
    {
        if (strncmp(pHeader->field_10_file_recs[i].field_0_file_name, pFileName, ALIVE_COUNTOF(LvlFileRecord::field_0_file_name)) == 0)
        {
            return &pHeader->field_10_file_recs[i];
        }
    }

    LOG_WARNING("Couldn't find " << pFileName << " in LVL");
    return nullptr;
}

EXPORT s16 LvlArchive::Read_File_41BE40(const LvlFileRecord* pFileRec, void* pBuffer)
{
    if (!pFileRec || !pBuffer)
    {
        return 0;
    }

    CdlLOC loc = {};
    PSX_Pos_To_CdLoc_49B340(pFileRec->field_C_start_sector + field_4_cd_pos, &loc);
    PSX_CD_File_Seek_49B670(2, &loc);
    s16 ret = static_cast<s16>(PSX_CD_File_Read_49B8B0(pFileRec->field_10_num_sectors, pBuffer));
    if (PSX_CD_FileIOWait_49B900(0) == -1)
    {
        ret = 0;
    }
    return ret;
}

} // namespace AO
