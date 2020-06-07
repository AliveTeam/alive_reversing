#pragma once

#include "FunctionFwd.hpp"
#include "Factory.hpp"

START_NS_AO

enum class LevelIds : __int16
{
    eNone = -1,
    eMenu_0 = 0,

};

struct Path_TLV;
class Map;

using TTempFn = void(CC*)();

struct PathData
{
    TTempFn field_0;
    __int16 field_4;
    __int16 field_6;
    __int16 field_8;
    __int16 field_A;
    __int16 field_C;
    __int16 field_E;
    __int16 field_10;
    __int16 field_12;
    int field_14_offset;
    int field_18_object_index_table_offset;
    PathFunctionTable field_1C_object_funcs;
};

using TCollisionsFactory = std::add_pointer<void(Path_TLV*, Map*, TlvItemInfoUnion, __int16)>::type;

struct CollisionInfo
{
    TCollisionsFactory field_0_fn_ptr;
    __int16 field_4_left;
    __int16 field_6_right;
    __int16 field_8_top;
    __int16 field_A_bottom;
    unsigned int field_C_collision_offset;
    unsigned int field_10_num_collision_items;
    unsigned int field_14_grid_width;
    unsigned int field_18_grid_height;
};

struct PathBlyRec
{
    const char* field_0_blyName;
    const PathData* field_4_pPathData;
    const CollisionInfo* field_8_pCollisionData;
    WORD field_C_overlay_id;
    WORD field_E;
};

struct FmvInfo
{
    const char* field_0_pName;
    unsigned __int16 field_4;
    __int16 field_6;
    __int16 field_8_stop_music;
    __int16 field_A;
    __int16 field_C_volume;
    __int16 field_E;
};

EXPORT const PathBlyRec* CC Path_Get_Bly_Record_434650(unsigned __int16 level, unsigned __int16 path);

EXPORT FmvInfo* CC Path_Get_FMV_Record_434680(unsigned __int16 levelId, unsigned __int16 fmvId);

EXPORT int CC Path_Format_CameraName_4346B0(char* pNameBuffer, __int16 level, __int16 path, __int16 camera);

END_NS_AO
