#pragma once

#include "FunctionFwd.hpp"
#include "Psx_common.hpp"

START_NS_AO

void Map_ForceLink();

struct PathData;
class Camera;

struct Map_PathsArray
{
    BYTE** field_0_pPathRecs[30];
};
ALIVE_ASSERT_SIZEOF(Map_PathsArray, 120);

// TODO: Assuming the same values as AE - test this
enum class CameraSwapEffects : __int16
{
    eEffect0_InstantChange = 0,
    eEffect1_LeftToRight = 1,       // Left to right
    eEffect2_RightToLeft = 2,       // Right to left
    eEffect3_TopToBottom = 3,       // Top to bottom
    eEffect4_BottomToTop = 4,       // Bottom to top
    eEffect5_1_FMV = 5,             // Play single fmv
    eEffect6_VerticalSplit = 6,     // Screen splits from the middle and moves out up/down
    eEffect7_HorizontalSplit = 7,   // Screen splits from the middle and moves out left/right
    eEffect8_BoxOut = 8,            // A rect "grows" out from the centre of the screen
    eEffect9_2_FMV = 9,             // Play 2 fmvs
    eEffect10_3_FMV = 10,           // Play 3 fmvs - apparently just taking an array of fmvs is too simple ?
    eEffect11 = 11                  // Unknown, has special handing in the map object
};

struct Path_TLV
{
    char field_0_flags;
    char field_1_unknown;
    __int16 field_2_length;
    int field_4_type;
    PSX_Point field_8_top_left;
    PSX_Point field_C_bottom_right;
    __int16 field_10;
    __int16 field_12;
    __int16 field_14;
    __int16 field_16;
};
ALIVE_ASSERT_SIZEOF(Path_TLV, 0x18);

class Map
{
public:
    EXPORT void Init_443EE0(__int16 level, __int16 path, __int16 camera, CameraSwapEffects screenChangeEffect, __int16 fmvBaseId, __int16 forceChange);
    
    EXPORT void Shutdown_443F90();
    void Reset();
    
    EXPORT __int16 SetActiveCam_444660(__int16 level, __int16 path, __int16 cam, CameraSwapEffects screenChangeEffect, __int16 fmvBaseId, __int16 forceChange);

    EXPORT __int16 GetOverlayId_4440B0();

    EXPORT void ScreenChange_4444D0();

    EXPORT void GoTo_Camera_445050();

    EXPORT void Loader_446590(__int16 camX, __int16 camY, int loadMode, __int16 typeToLoad);

    __int16 field_0_current_level;
    __int16 field_2_current_path;
    __int16 field_4_current_camera;
    __int16 field_6_state;
    __int16 field_8_force_load;
    __int16 field_A_level;
    __int16 field_C_path;
    __int16 field_E_camera;
    CameraSwapEffects field_10_screenChangeEffect;
    __int16 field_12_fmv_base_id;
    __int16 field_14_direction;
    __int16 field_16;
    int field_18_pAliveObj;
    __int16 field_1C_cameraSwapEffect;
    __int16 field_1E_door;
    __int16 field_20_camX_idx;
    __int16 field_22_camY_idx;
    unsigned __int16 field_24_max_cams_x;
    unsigned __int16 field_26_max_cams_y;
    __int16 field_28_cd_or_overlay_num;
    __int16 field_2A;
    int field_2C_camera_offset;
    int field_30;
    Camera* field_34_camera_array[5];
    int field_48_stru_5[5];
    Map_PathsArray field_5C_path_res_array;
    PathData* field_D4_pPathData;
    __int16 field_D8;
    __int16 field_DA;
    __int16 field_DC;
    __int16 field_DE;
    int field_E0_save_data;
};

ALIVE_ASSERT_SIZEOF(Map, 0xE4);

ALIVE_VAR_EXTERN(Map, gMap_507BA8);

END_NS_AO

