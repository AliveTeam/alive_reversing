#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "MainMenu.hpp"
#include "../relive_lib/SwitchStates.hpp"

struct Quicksave_PSX_Header final
{
    u8 field_0_frame_1_name[128];
    s8 field_80_frame_2_padding[128];
    s8 field_100_frame_3_padding[128];
    s8 field_180_frame_4_padding[128];
};
ALIVE_ASSERT_SIZEOF(Quicksave_PSX_Header, 0x200);

enum class LevelIds : s16;

struct Quicksave_WorldInfo final
{
    s32 mGnFrame;
    LevelIds field_4_level;
    s16 field_6_path;
    s16 field_8_cam;
    s16 mSaveFileId;
    s16 field_C_controlled_x;
    s16 field_E_controlled_y;
    s16 field_10_controlled_scale;
    s16 field_12_saved_muds;
    s16 field_14_killed_muds;
    s8 field_16_muds_in_area;
    s8 field_17_last_saved_killed_muds_per_path;
    s8 field_18_saved_killed_muds_per_zulag[20];
    s8 field_2C_current_zulag_number;
    s8 field_2D_total_meter_bars;
    s16 field_2E_use_alt_save_header;
    s16 field_30_bDrawMeterCountDown;
    s16 mVisitedBonewerkz;
    s16 mVisitedBarracks;
    s16 mVisitedFeecoEnder;
    s32 mGasTimer;
    s16 mAbeInvincible;
    s16 field_3E_padding;
};
ALIVE_ASSERT_SIZEOF(Quicksave_WorldInfo, 0x40);

struct Quicksave final
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

struct SaveFileRec final
{
    char_type field_0_fileName[32];
    u32 field_20_lastWriteTimeStamp;
};
ALIVE_ASSERT_SIZEOF(SaveFileRec, 0x24);

extern SaveFileRec sSaveFileRecords_BB31D8[128];
extern Quicksave sActiveQuicksaveData;
extern s32 sSavedGameToLoadIdx_BB43FC;
extern s32 sTotalSaveFilesCount_BB43E0;
extern u16 sQuickSave_saved_switchResetters_count_BB234C;

void Quicksave_LoadActive();
void DoQuicksave();
void Quicksave_ReadWorldInfo(const Quicksave_WorldInfo* pInfo);
void Quicksave_SaveWorldInfo(Quicksave_WorldInfo* pInfo);
void Quicksave_FindSaves();
void QuikSave_RestoreBlyData(const u8* pSaveData);
void Quicksave_SaveSwitchResetterStates();
void Quicksave_RestoreSwitchResetterStates();
