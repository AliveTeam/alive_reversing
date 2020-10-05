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

struct LvlHeader_Sub
{
    int field_0_num_files;
    int field_4_unknown1;
    int field_8_unknown2;
    int field_C_unknown3;
    LvlFileRecord field_10_file_recs[1]; // TODO: Strictly UB on >= 1 access
};

struct LvlHeader
{
    int field_0_first_file_offset;
    int field_4_ref_count;
    int field_8_magic;
    int field_C_id;
    LvlHeader_Sub field_10_sub;
};


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
