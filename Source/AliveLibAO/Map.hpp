#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

class Map
{
public:
    EXPORT void Init_443EE0(__int16 level, __int16 path, __int16 camera, __int16 screenChangeEffect, __int16 a6, __int16 forceChange);

    EXPORT void Shutdown_443F90();

    EXPORT void ScreenChange_4444D0();

    unsigned __int16 field_0_current_level;
    unsigned __int16 field_2_current_path;
    __int16 field_4_current_camera;
    __int16 field_6_state;
    __int16 field_8_force_load;
    __int16 field_A_level;
    __int16 field_C_path;
    __int16 field_E_camera;
    __int16 field_10_screenChangeEffect;
    __int16 field_12_fmv_base_id;
    __int16 field_14_direction;
    __int16 field_16;
    int field_18_pAliveObj;
    __int16 field_1C_cameraSwapEffect;
    __int16 field_1E_door;
    __int16 field_20_camX_idx;
    __int16 field_22_camY_idx;
    __int16 field_24_max_cams_x;
    __int16 field_26_max_cams_y;
    __int16 field_28_cd_or_overlay_num;
    __int16 field_2A;
    int field_2C_camera_offset;
    int field_30;
    int field_34_camera_array[5];
    int field_48_stru_5[5];
    int field_5C_path_res_array[30];
    int field_D4_tlv_start_offset;
    __int16 field_D8;
    __int16 field_DA;
    __int16 field_DC;
    __int16 field_DE;
    int field_E0;
};

ALIVE_ASSERT_SIZEOF(Map, 0xE4);

ALIVE_VAR_EXTERN(Map, gMap_507BA8);

END_NS_AO

