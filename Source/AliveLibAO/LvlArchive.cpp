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

EXPORT __int16 LvlArchive::Read_File_41BE40(const LvlFileRecord* /*pFileRec*/, void* /*pBuffer*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

END_NS_AO
