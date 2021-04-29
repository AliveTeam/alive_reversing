#pragma once

#include "FunctionFwd.hpp"
#include "MainMenu.hpp"
#include "SwitchStates.hpp"

struct Quicksave_PSX_Header
{
    s8 field_0_frame_1_name[128];
    s8 field_80_frame_2_padding[128];
    s8 field_100_frame_3_padding[128];
    s8 field_180_frame_4_padding[128];
};
ALIVE_ASSERT_SIZEOF(Quicksave_PSX_Header, 0x200);

enum class LevelIds : s16;

struct Quicksave_WorldInfo
{
    s32 field_0_gnFrame;
    LevelIds field_4_level;
    s16 field_6_path;
    s16 field_8_cam;
    s16 field_A_save_num;
    s16 field_C_controlled_x;
    s16 field_E_controlled_y;
    s16 field_10_controlled_scale;
    s16 field_12_saved_muds;
    s16 field_14_killed_muds;
    s8 field_16_muds_in_area;
    s8 field_17_last_saved_killed_muds_per_path;
    s8 field_18_saved_killed_muds_per_path[20];
    s8 field_2C_stats_sign_current_area;
    s8 field_2D_total_meter_bars;
    s16 field_2E_use_alt_save_header;
    s16 field_30_bDrawMeterCountDown;
    s16 field_32_visited_bonewerks;
    s16 field_34_visited_barracks;
    s16 field_36_visited_feeco_ender;
    s32 field_38_gas_timer;
    s16 field_3C_bBulletProof;
    s16 field_3E_padding;
};
ALIVE_ASSERT_SIZEOF(Quicksave_WorldInfo, 0x40);

struct Quicksave
{
    Quicksave_PSX_Header field_0_header;
    s32 field_200_accumulated_obj_count;
    Quicksave_WorldInfo field_204_world_info;
    Quicksave_WorldInfo field_244_restart_path_world_info;
    u8 field_284_restart_path_abe_state[216];
    SwitchStates field_35C_restart_path_switch_states;
    SwitchStates field_45C_switch_states;
    u8 field_55C_objects_state_data[6820];
};
ALIVE_ASSERT_SIZEOF(Quicksave, 0x2000);

struct SaveFileRec
{
    s8 field_0_fileName[32];
    u32 field_20_lastWriteTimeStamp;
};
ALIVE_ASSERT_SIZEOF(SaveFileRec, 0x24);

ALIVE_ARY_EXTERN(SaveFileRec, 128, sSaveFileRecords_BB31D8);
ALIVE_VAR_EXTERN(Quicksave, sActiveQuicksaveData_BAF7F8);
ALIVE_VAR_EXTERN(s32, sAccumulatedObjectCount_5C1BF4);
ALIVE_VAR_EXTERN(s32, sSavedGameToLoadIdx_BB43FC);
ALIVE_VAR_EXTERN(s32, sTotalSaveFilesCount_BB43E0);
ALIVE_VAR_EXTERN(u16, sQuickSave_saved_switchResetters_count_BB234C);

EXPORT void CC Quicksave_LoadActive_4C9170();
EXPORT void CC Quicksave_4C90D0();
EXPORT void CC Quicksave_ReadWorldInfo_4C9490(const Quicksave_WorldInfo* pInfo);
EXPORT void CC Quicksave_SaveWorldInfo_4C9310(Quicksave_WorldInfo* pInfo);
EXPORT void CC Quicksave_FindSaves_4D4150();
void QuikSave_RestoreBlyData_D481890_4C9BE0(const u8* pSaveData);
EXPORT void CC Quicksave_SaveSwitchResetterStates_4C9870();
EXPORT void CC Quicksave_RestoreSwitchResetterStates_4C9A30();
