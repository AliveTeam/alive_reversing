#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"
#include "Psx.hpp"

void Map_ForceLink();

struct Map_120
{
    BYTE** field_0_pPathRecs[30];
};
ALIVE_ASSERT_SIZEOF(Map_120, 120);

class BaseAliveGameObject;
class Camera;
struct PathData;

class Map
{
public:
    unsigned __int16 sCurrentLevelId_5C3030;
    unsigned __int16 sCurrentPathId_5C3032;
    unsigned __int16 sCurrentCamId_5C3034;
    __int16 field_6;
    __int16 field_8;

    unsigned __int16 field_A_5C303A_levelId;
    unsigned __int16 field_C_5C303C_pathId;
    __int16 field_E_cameraId;
    __int16 field_10;
    __int16 field_12_ticks;

    __int16 field_14_direction;
    __int16 field_16;

    BaseAliveGameObject *field_18_pAliveObj;

    __int16 field_1C;
    __int16 field_1E;
    __int16 field_20;
    __int16 field_22;

    FP_Point field_24_camera_offset;

    Camera* field_2C_5C305C_camera_array[5];
    Camera* field_40_stru_5[5];
    Map_120 field_54_path_res_array;

    __int16 field_CC;
    __int16 field_CE;

    __int16 field_D0_cam_x_idx;
    __int16 field_D2_cam_y_idx;

    PathData* field_D4_ptr;
    int field_D8_restore_quick_save;


    EXPORT void sub_480B80();
    EXPORT int sub_4803F0(__int16 a2, __int16 a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7);
    EXPORT void sub_4804E0();
    EXPORT void GetCurrentCamCoords_480680(PSX_Point* pPoint);
    EXPORT void sub_480D30(__int16 level, __int16 path, __int16 cam, __int16 a5, __int16 a6, __int16 a7);
};
ALIVE_ASSERT_SIZEOF(Map, 0xDC);

ALIVE_VAR_EXTERN(Map, gMap_5C3030);
