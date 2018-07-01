#pragma once

#include "FunctionFwd.hpp"

struct FmvInfo 
{
    const char* field_0_pName;
    unsigned __int16 field_4_id;
    __int16 field_6_flags;
    unsigned __int16 field_8;
    __int16 field_A_volume; // usually 127 ?
};
ALIVE_ASSERT_SIZEOF(FmvInfo, 0xC);

struct CollisionInfo
{
    void* field_0_fn_ptr;
    __int16 field_4_left;
    __int16 field_6_right;
    __int16 field_8_top;
    __int16 field_A_bottom;
    unsigned int field_C_collision_offset;
    unsigned int field_10_num_collision_items;
    unsigned int field_14_grid_width;
    unsigned int field_18_grid_height;
};
ALIVE_ASSERT_SIZEOF(CollisionInfo, 0x1C);

struct PathData
{
    __int16 field_0_bLeft;
    __int16 field_2_bRight;
    __int16 field_4_bTop;
    __int16 field_6_bBottom;
    __int16 field_A_grid_width;
    __int16 field_C_grid_height;
    __int16 field_E_width;
    __int16 field_10_height;
    unsigned int field_12_object_offset;
    unsigned int field_16_object_indextable_offset;
    __int16 field_1A_abe_start_xpos;
    __int16 field_1C_abe_start_ypos;
    void* field_1E_object_funcs[256];
};
ALIVE_ASSERT_SIZEOF(PathData, 0x41C);

struct PathBlyRec
{
    const char *field_0_blyName;
    const PathData *field_4_pPathData;
    const CollisionInfo *field_8_pCollisionData;
    WORD field_C;
    WORD field_E;
};

struct SoundBlockInfo
{
    const char *field_0_vab_header_name;
    const char *field_4_vab_body_name;
    int field_8_vab_id;
    int field_C_pVabHeader;
};

struct PathRoot
{
    const PathBlyRec* field_0_pBlyArrayPtr;
    FmvInfo* field_4_pFmvArray;
    SoundBlockInfo* field_8_pMusicInfo;
    const char* field_C_bsq_file_name;
    __int16 field_10_reverb;
    __int16 field_14_bg_music_id;
    const char* field_18_lvl_name;
    __int16 field_1A_num_paths;
    __int16 field_1C_unused;
    int field_1E;
    const char* field_22_lvl_name_cd;
    int field_26;
    const char* field_2A_ovl_name_cd;
    int field_2E;
    const char* field_32_mov_name_cd;
    const char* field_36_idx_name;
    const char* field_3A_bnd_name;
};

struct PathRootContainer
{
    PathRoot paths[17];
};

EXPORT const PathBlyRec* CC Path_Get_Bly_Record_460F30(unsigned __int16 lvlId, unsigned __int16 pathId);

EXPORT FmvInfo* CC Path_Get_FMV_Record_460F70(unsigned __int16 lvlId, unsigned __int16 fmvId);

EXPORT void CC Path_Format_CameraName_460FB0(char* pStrBuffer, __int16 levelId, __int16 pathId, __int16 cameraId);
