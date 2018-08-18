#pragma once

#include "FunctionFwd.hpp"
#include "SwitchStates.hpp"

void Quicksave_ForceLink();

struct Quicksave_PSX_Header
{
    char field_0_frame_1_name[128];
    char field_80_frame_2[128];
    char field_100_frame_3[128];
    char field_180_frame_4[128];
};
ALIVE_ASSERT_SIZEOF(Quicksave_PSX_Header, 0x200);

struct Quicksave_WorldInfo
{
    int field_0_gnFrame;
    __int16 field_4_level;
    __int16 field_6_path;
    __int16 field_8_cam;
    unsigned __int16 field_A_unknown_1;
    __int16 field_C_controlled_x;
    __int16 field_E_controlled_y;
    __int16 field_10_controlled_scale;
    __int16 field_12_saved_muds;
    __int16 field_14_killed_muds;
    __int16 field_16_muds_in_area;
    char field_18_saved_killed_muds_per_path[20];
    char field_2C;
    char field_2D;
    __int16 field_2E;
    __int16 field_30;
    __int16 field_32_visited_bonewerks;
    __int16 field_34_visited_barracks;
    __int16 field_36_visited_feeco_ender;
    int field_38;
    int field_3C;
};
ALIVE_ASSERT_SIZEOF(Quicksave_WorldInfo, 0x40);

struct Quicksave
{
    Quicksave_PSX_Header field_0_header;
    int field_200_accumulated_obj_count;
    Quicksave_WorldInfo field_204_world_info;
    Quicksave_WorldInfo field_244_restart_path_world_info;
    char field_284_restart_path_abe_state[216];
    SwitchStates field_35C_restart_path_switch_states;
    SwitchStates field_45C_switch_states;
    char field_55C_objects_state_data[6820];
};
ALIVE_ASSERT_SIZEOF(Quicksave, 0x2000);


ALIVE_VAR_EXTERN(Quicksave, sActiveQuicksaveData_BAF7F8);

EXPORT void CC Quicksave_LoadActive_4C9170();
EXPORT void CC Quicksave_4C90D0();
EXPORT void CC Quicksave_ReadWorldInfo_4C9490(Quicksave_WorldInfo *a1);
EXPORT void CC Quicksave_FindSaves_4D4150();