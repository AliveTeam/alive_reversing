#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibCommon/Types.hpp"

enum class EReliveLevelIds : s16;

struct CollisionInfo;

namespace AO {

enum class LevelIds : s16
{
    eNone = -1,
    eMenu_0 = 0,
    eRuptureFarms_1 = 1,
    eLines_2 = 2,
    eForest_3 = 3,
    eForestTemple_4 = 4,
    eStockYards_5 = 5,
    eStockYardsReturn_6 = 6,
    eRemoved_7 = 7,
    eDesert_8 = 8,
    eDesertTemple_9 = 9,
    eCredits_10 = 10,
    eRemoved_11 = 11,
    eBoardRoom_12 = 12,
    eRuptureFarmsReturn_13 = 13,
    eForestChase_14 = 14,
    eDesertEscape_15 = 15,
};

struct Path_TLV;
class Map;

using TTempFn = s32(CC*)();

struct PathData final
{
    TTempFn field_0;
    s16 field_4_bLeft;
    s16 field_6_bRight;
    s16 field_8_bTop;
    s16 field_A_bBottom;
    s16 field_C_grid_width;
    s16 field_E_grid_height;
    s16 field_10;
    s16 field_12;
    s32 field_14_object_offset;
    s32 field_18_object_index_table_offset;
};

struct PathBlyRec final
{
    const char_type* field_0_blyName;
    PathData* field_4_pPathData;
    CollisionInfo* field_8_pCollisionData;
    u16 mOverlayId;
    u16 field_E;
};

struct FmvInfo final
{
    const char_type* field_0_pName;
    u16 field_4_id;
    s16 field_6;
    s16 field_8_stop_music;
    s16 field_A;
    s16 field_C_volume;
    s16 field_E;
};

struct SoundBlockInfo final
{
    const char_type* field_0_vab_header_name;
    const char_type* field_4_vab_body_name;
    s32 field_8_vab_id;
    u8* field_C_pVabHeader;
};

struct PathRoot final
{
    PathBlyRec* field_0_pBlyArrayPtr;
    FmvInfo* field_4_pFmvArray;
    SoundBlockInfo* field_8_pMusicInfo;
    const char_type* field_C_bsq_file_name;
    s16 field_10_reverb;
    s16 field_12_bg_music_id;
    const char_type* field_14_lvl_name;
    s16 field_18_num_paths;
    s16 field_1A_unused; // message to display to change cd ??
    s32 field_1C_overlay_idx;
    const char_type* field_20_lvl_name_cd;
    s32 field_24;
    const char_type* field_28_ovl_name_cd;
    s32 field_2C;
    const char_type* field_30_mov_name_cd;
    const char_type* field_34_idx_name;
    const char_type* field_38_bnd_name;
};

struct PathRootContainer final
{
    PathRoot paths[16];
};

const PathBlyRec* Path_Get_Bly_Record(EReliveLevelIds level, u16 path);

FmvInfo* Path_Get_FMV_Record(EReliveLevelIds levelId, u16 fmvId);

s32 Path_Format_CameraName(char_type* pNameBuffer, EReliveLevelIds level, s16 path, s16 camera);

const char_type* CdLvlName(EReliveLevelIds lvlId);

const char_type* Path_Get_Lvl_Name(EReliveLevelIds lvlId);

s16 Path_Get_Num_Paths(EReliveLevelIds lvlId);

s16 Path_Get_Unknown(EReliveLevelIds lvlId);

const char_type* Path_Get_BndName(EReliveLevelIds lvlId);

// note: has to be writable
SoundBlockInfo* Path_Get_MusicInfo(EReliveLevelIds lvlId);

s16 Path_Get_Reverb(EReliveLevelIds lvlId);

const char_type* Path_Get_BsqFileName(EReliveLevelIds lvlId);

s16 Path_Get_BackGroundMusicId(EReliveLevelIds lvlId);

s32 Path_Get_Paths_Count();

PathRoot* Path_Get_PathRoot(s32 lvlId);

s32 Path_Get_OverlayIdx(EReliveLevelIds lvlId);

CollisionInfo* GetCollisions(s32 lvlId);

PathData* GetPathData(s32 lvlId);

} // namespace AO
