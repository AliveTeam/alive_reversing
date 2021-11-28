#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"

namespace AO {

struct LvlFileRecord final
{
    char_type field_0_file_name[12];
    s32 field_C_start_sector;
    s32 field_10_num_sectors;
    s32 field_14_file_size;
};
ALIVE_ASSERT_SIZEOF(LvlFileRecord, 0x18);

struct LvlHeader_Sub final
{
    s32 field_0_num_files;
    s32 field_4_unknown1;
    s32 field_8_unknown2;
    s32 field_C_unknown3;
    LvlFileRecord field_10_file_recs[1]; // TODO: Strictly UB on >= 1 access
};

struct LvlHeader final
{
    s32 field_0_first_file_offset;
    s32 field_4_ref_count;
    s32 field_8_magic;
    s32 field_C_id;
    LvlHeader_Sub field_10_sub;
};


class LvlArchive
{
public:
    EXPORT static void CC ctor_static_41BBA0();

    EXPORT static void CC dtor_static_41BBB0();

    EXPORT static void CC ctor_static_443E70();

    EXPORT static void CC dtor_static_443E80();

    EXPORT void OpenArchive(const char_type* fileName, s32 pos);

    EXPORT void OpenArchive_41BC60(s32 pos);

    EXPORT s16 Free_41BEB0();

    EXPORT LvlFileRecord* Find_File_Record_41BED0(const char_type* pFileName);

    EXPORT s16 Read_File_41BE40(const LvlFileRecord* pFileRec, void* pBuffer);

private:
    u8** field_0_0x2800_res;

public:
    s32 field_4_cd_pos;
};
ALIVE_ASSERT_SIZEOF(LvlArchive, 0x8);

ALIVE_VAR_EXTERN(LvlArchive, sLvlArchive_4FFD60);
ALIVE_VAR_EXTERN(LvlArchive, stru_507C90);

} // namespace AO
