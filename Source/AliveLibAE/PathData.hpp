#pragma once

#include "FunctionFwd.hpp"
#include "Factory.hpp"
#include "Collisions.hpp"

enum class LevelIds : s16
{
    eNone = -1,
    eMenu_0 = 0,
    eMines_1 = 1,
    eNecrum_2 = 2,
    eMudomoVault_3 = 3,
    eMudancheeVault_4 = 4,
    eFeeCoDepot_5 = 5,
    eBarracks_6 = 6,
    eMudancheeVault_Ender_7 = 7,
    eBonewerkz_8 = 8,
    eBrewery_9 = 9,
    eBrewery_Ender_10 = 10,
    eMudomoVault_Ender_11 = 11,
    eFeeCoDepot_Ender_12 = 12,
    eBarracks_Ender_13 = 13,
    eBonewerkz_Ender_14 = 14,
    eNotUsed_15 = 15, // Probably test level?
    eCredits_16 = 16
};

struct FmvInfo
{
    const s8* field_0_pName;
    u16 field_4_id;
    s16 field_6_flags;
    u16 field_8_flags;
    s16 field_A_volume; // usually 127 ?
};
ALIVE_ASSERT_SIZEOF(FmvInfo, 0xC);

struct CollisionInfo
{
    TCollisionsFactory field_0_fn_ptr;
    s16 field_4_left;
    s16 field_6_right;
    s16 field_8_top;
    s16 field_A_bottom;
    u32 field_C_collision_offset;
    u32 field_10_num_collision_items;
    u32 field_14_grid_width;
    u32 field_18_grid_height;
};
ALIVE_ASSERT_SIZEOF(CollisionInfo, 0x1C);

struct PathData
{
    s16 field_0_bLeft;
    s16 field_2_bRight;
    s16 field_4_bTop;
    s16 field_6_bBottom;
    s16 field_A_grid_width;
    s16 field_C_grid_height;
    s16 field_E_width;
    s16 field_10_height;
    u32 field_12_object_offset;
    u32 field_16_object_indextable_offset;
    s16 field_1A_abe_start_xpos;
    s16 field_1C_abe_start_ypos;
    PathFunctionTable field_1E_object_funcs;
};
ALIVE_ASSERT_SIZEOF(PathData, 0x41C);

struct PathBlyRec
{
    const s8* field_0_blyName;
    const PathData* field_4_pPathData;
    const CollisionInfo* field_8_pCollisionData;
    u16 field_C_overlay_id;
    u16 field_E_padding;
};

struct SoundBlockInfo
{
    const s8* field_0_vab_header_name;
    const s8* field_4_vab_body_name;
    s32 field_8_vab_id;
    u8* field_C_pVabHeader;
};

struct PathRoot
{
    const PathBlyRec* field_0_pBlyArrayPtr;
    FmvInfo* field_4_pFmvArray;
    SoundBlockInfo* field_8_pMusicInfo;
    const s8* field_C_bsq_file_name;
    s16 field_10_reverb;
    s16 field_12_bg_music_id;
    const s8* field_14_lvl_name;
    s16 field_18_num_paths;
    s16 field_1A_unused; // message to display to change cd ??
    s32 field_1C_padding;
    const s8* field_20_lvl_name_cd;
    s32 field_24_padding;
    const s8* field_28_ovl_name_cd;
    s32 field_2C_padding;
    const s8* field_30_mov_name_cd;
    const s8* field_34_idx_name;
    const s8* field_38_bnd_name;
};

struct PathRootContainer
{
    PathRoot paths[17];
};

struct PerLvlData
{
    const s8* field_0_display_name;
    LevelIds field_4_level;
    u16 field_6_path;
    u16 field_8_camera;
    u16 field_A_id;
    u16 field_C_abe_x_off;
    u16 field_E_abe_y_off;
};

struct OpenSeqHandle
{
    const s8* field_0_mBsqName;
    s32 field_4_generated_res_id; // A hash of the named which matches the resource Id
    s8 field_8_sound_block_idx;
    s8 field_9_volume;
    s16 field_A_id_seqOpenId;
    u8* field_C_ppSeq_Data;
};
ALIVE_ASSERT_SIZEOF(OpenSeqHandle, 0x10);

struct SeqHandleTable
{
    OpenSeqHandle mSeqs[145];
};

EXPORT const PathBlyRec* CC Path_Get_Bly_Record_460F30(LevelIds lvlId, u16 pathId);

EXPORT FmvInfo* CC Path_Get_FMV_Record_460F70(LevelIds lvlId, u16 fmvId);

EXPORT void CC Path_Format_CameraName_460FB0(s8* pStrBuffer, LevelIds levelId, s16 pathId, s16 cameraId);

const s8* CdLvlName(LevelIds lvlId);

ALIVE_VAR_EXTERN(PathRootContainer, sPathData_559660);
ALIVE_VAR_EXTERN(SeqHandleTable, sSeqData_558D50);
