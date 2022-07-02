#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "ResourceManager.hpp"

struct LvlFileRecord final
{
    char_type field_0_file_name[12];
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
    s32 Open_Archive_432E80(const char_type* fileName);
    LvlFileRecord* Find_File_Record_433160(const char_type* pFileName);
    s32 Read_File_433070(const char_type* pFileName, void* pBuffer);
    s32 Read_File_4330A0(LvlFileRecord* hFile, void* pBuffer);
    s32 Free_433130();
    ~LvlArchive()
    {
        Free_433130();
    }

private:
    u8** field_0_0x2800_res = nullptr;

public:
    u32 field_4_cd_pos = 0;
};
ALIVE_ASSERT_SIZEOF(LvlArchive, 0x8);

ALIVE_VAR_EXTERN(LvlArchive, sLvlArchive_5BC520);
ALIVE_VAR_EXTERN(LvlArchive, stru_5C3110);
ALIVE_VAR_EXTERN(bool, sbEnable_PCOpen_5CA4B0);
