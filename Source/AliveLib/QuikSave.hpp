#pragma once

#include "FunctionFwd.hpp"
#include "MainMenu.hpp"
#include "SwitchStates.hpp"

struct Quicksave_PSX_Header
{
    char field_0_frame_1_name[128];
    char field_80_frame_2[128];
    char field_100_frame_3[128];
    char field_180_frame_4[128];
};
ALIVE_ASSERT_SIZEOF(Quicksave_PSX_Header, 0x200);

enum class LevelIds : __int16;

struct Quicksave_WorldInfo
{
    int field_0_gnFrame;
    LevelIds field_4_level;
    __int16 field_6_path;
    __int16 field_8_cam;
    __int16 field_A_unknown_1;
    __int16 field_C_controlled_x;
    __int16 field_E_controlled_y;
    __int16 field_10_controlled_scale;
    __int16 field_12_saved_muds;
    __int16 field_14_killed_muds;
    char field_16_muds_in_area;
    char field_17;
    PerPathMudStats field_18_saved_killed_muds_per_path;
    char field_2C;
    char field_2D;
    __int16 field_2E;
    __int16 field_30;
    __int16 field_32_visited_bonewerks;
    __int16 field_34_visited_barracks;
    __int16 field_36_visited_feeco_ender;
    int field_38;
    __int16 field_3C;
    __int16 field_3E;
};
ALIVE_ASSERT_SIZEOF(Quicksave_WorldInfo, 0x40);

struct Quicksave
{
    Quicksave_PSX_Header field_0_header;
    int field_200_accumulated_obj_count;
    Quicksave_WorldInfo field_204_world_info;
    Quicksave_WorldInfo field_244_restart_path_world_info;
    BYTE field_284_restart_path_abe_state[216];
    SwitchStates field_35C_restart_path_switch_states;
    SwitchStates field_45C_switch_states;
    BYTE field_55C_objects_state_data[6820];
};
ALIVE_ASSERT_SIZEOF(Quicksave, 0x2000);

struct SaveFileRec
{
    char field_0_fileName[32];
    DWORD field_20_lastWriteTimeStamp;
};
ALIVE_ASSERT_SIZEOF(SaveFileRec, 0x24);

ALIVE_ARY_EXTERN(SaveFileRec, 128, sSaveFileRecords_BB31D8);
ALIVE_VAR_EXTERN(Quicksave, sActiveQuicksaveData_BAF7F8);
ALIVE_VAR_EXTERN(int, sAccumulatedObjectCount_5C1BF4);
ALIVE_VAR_EXTERN(int, sSavedGameToLoadIdx_BB43FC);
ALIVE_VAR_EXTERN(signed int, sTotalSaveFilesCount_BB43E0);

EXPORT void CC Quicksave_LoadActive_4C9170();
EXPORT void CC Quicksave_4C90D0();
EXPORT void CC Quicksave_ReadWorldInfo_4C9490(const Quicksave_WorldInfo* pInfo);
EXPORT void CC Quicksave_SaveWorldInfo_4C9310(Quicksave_WorldInfo* pInfo);
EXPORT void CC Quicksave_FindSaves_4D4150();
void QuikSave_RestoreObjectStates_D481890_4C9BE0(const BYTE* pSaveData);
