#pragma once

#include "FunctionFwd.hpp"
#include "ResourceManager.hpp"

EXPORT void CC LvlArchive_Static_init_432E00();
EXPORT void CC static_lvl_init_480350();

struct LvlFileRecord
{
    char field_0_file_name[12];
    int field_C_start_sector;
    int field_10_num_sectors;
    int field_14_file_size;
};

struct LvlHeader_Sub
{
    int field_0_num_files;
    int field_4_header_size_in_sectors;
    int field_8_unknown2;
    int field_C_unknown3;
    LvlFileRecord field_10_file_recs[1]; // TODO: Strictly UB on >= 1 access
};

struct LvlHeader
{
    // TODO: Up to field_C is really a ResourceManager::Header
    int field_0_first_file_offset;
    int field_4_ref_count;
    int field_8_magic;
    int field_C_id;
    LvlHeader_Sub field_10_sub;
};


class LvlArchive
{
public:
    EXPORT int Open_Archive_432E80(const char* fileName);
    EXPORT LvlFileRecord* Find_File_Record_433160(const char* pFileName);
    EXPORT int Read_File_433070(const char* pFileName, void* pBuffer);
    EXPORT int Read_File_4330A0(LvlFileRecord* hFile, void* pBuffer);
    EXPORT int Free_433130();
private:
    BYTE** field_0_0x2800_res;
public:
    DWORD field_4_cd_pos;
};
ALIVE_ASSERT_SIZEOF(LvlArchive, 0x8);

ALIVE_VAR_EXTERN(LvlArchive, sLvlArchive_5BC520);
ALIVE_VAR_EXTERN(LvlArchive, stru_5C3110);
ALIVE_VAR_EXTERN(BOOL, sbEnable_PCOpen_5CA4B0);
