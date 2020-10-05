#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

struct LvlFileRecord
{
    char field_0_file_name[12];
    int field_C_start_sector;
    int field_10_num_sectors;
    int field_14_file_size;
};
ALIVE_ASSERT_SIZEOF(LvlFileRecord, 0x18);

class LvlArchive
{
public:
    EXPORT static void CC ctor_static_41BBA0();

    EXPORT static void CC dtor_static_41BBB0();

    EXPORT static void CC ctor_static_443E70();

    EXPORT static void CC dtor_static_443E80();

    EXPORT void OpenArchive_41BC60(int pos);

    EXPORT __int16 Free_41BEB0();

    EXPORT LvlFileRecord* Find_File_Record_41BED0(const char* pFileName);

    EXPORT __int16 Read_File_41BE40(const LvlFileRecord* pFileRec, void* pBuffer);

private:
    BYTE** field_0_0x2800_res;
public:
    int field_4_cd_pos;
};
ALIVE_ASSERT_SIZEOF(LvlArchive, 0x8);

ALIVE_VAR_EXTERN(LvlArchive, sLvlArchive_4FFD60);
ALIVE_VAR_EXTERN(LvlArchive, stru_507C90);

END_NS_AO
