#pragma once

#include "FunctionFwd.hpp"
#include "ResourceManager.hpp"

EXPORT void CC LvlArchive_Static_init_432E00();
EXPORT void CC static_lvl_init_480350();

struct LvlFileRecord final
{
    s8 field_0_file_name[12];
    s32 field_C_start_sector;
    s32 field_10_num_sectors;
    s32 field_14_file_size;
};

struct LvlHeader_Sub
{
    s32 field_0_num_files;
    s32 field_4_header_size_in_sectors;
    s32 field_8_unknown2;
    s32 field_C_unknown3;
    LvlFileRecord field_10_file_recs[1]; // TODO: Strictly UB on >= 1 access
};

struct LvlHeader final
{
    // TODO: Up to field_C is really a ResourceManager::Header
    s32 field_0_first_file_offset;
    s32 field_4_ref_count;
    s32 field_8_magic;
    s32 field_C_id;
    LvlHeader_Sub field_10_sub;
};


class LvlArchive final
{
public:
    EXPORT s32 Open_Archive_432E80(const s8* fileName);
    EXPORT LvlFileRecord* Find_File_Record_433160(const s8* pFileName);
    EXPORT s32 Read_File_433070(const s8* pFileName, void* pBuffer);
    EXPORT s32 Read_File_4330A0(LvlFileRecord* hFile, void* pBuffer);
    EXPORT s32 Free_433130();

private:
    u8** field_0_0x2800_res;

public:
    u32 field_4_cd_pos;
};
ALIVE_ASSERT_SIZEOF(LvlArchive, 0x8);

ALIVE_VAR_EXTERN(LvlArchive, sLvlArchive_5BC520);
ALIVE_VAR_EXTERN(LvlArchive, stru_5C3110);
ALIVE_VAR_EXTERN(BOOL, sbEnable_PCOpen_5CA4B0);
