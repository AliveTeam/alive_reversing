#include "stdafx_ao.h"
#include "LvlArchive.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"

START_NS_AO

ALIVE_VAR(1, 0x4FFD60, LvlArchive, sLvlArchive_4FFD60, {});
ALIVE_VAR(1, 0x507C90, LvlArchive, stru_507C90, {});


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

EXPORT void LvlArchive::OpenArchive_41BC60(int /*pos*/)
{
    NOT_IMPLEMENTED();
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

EXPORT LvlFileRecord* LvlArchive::Find_File_Record_41BED0(const char* /*pFileName*/)
{
    NOT_IMPLEMENTED();
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
