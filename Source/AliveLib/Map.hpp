#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"
#include "Psx.hpp"
#include "ResourceManager.hpp"

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
    __int16 field_10_screen_change_effect;
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

    EXPORT void Get_map_size_480640(PSX_Point* pPoint);
    EXPORT void GetCurrentCamCoords_480680(PSX_Point* pPoint);
    EXPORT void Get_Abe_Spawn_Pos_4806D0(PSX_Point* pPoint);
    EXPORT __int16 Get_Path_Unknown_480710();

    EXPORT signed __int16 SetActiveCam_480D30(__int16 level, __int16 path, __int16 cam, __int16 a5, __int16 a6, __int16 a7);

    EXPORT static void __stdcall Load_Path_Items_482C10(Camera* pCamera, __int16 loadMode);

    EXPORT static void CC LoadResource_4DBE00(const char* pFileName, int type, int resourceId, __int16 loadMode, __int16 bDontLoad);
    EXPORT static void CC LoadResourcesFromList_4DBE70(const char* pFileName, ResourceManager::ResourcesToLoadList* pList, __int16 loadMode, __int16 bDontLoad);
};
ALIVE_ASSERT_SIZEOF(Map, 0xDC);

ALIVE_VAR_EXTERN(Map, gMap_5C3030);

struct Path_TLV
{
    __int16 field_0_flags;
    __int16 field_2_length;
    int field_4_type;
    PSX_Point field_8_top_left;
    PSX_Point field_C_bottom_right;
};
ALIVE_ASSERT_SIZEOF(Path_TLV, 0x10);


class Path
{
public:
    EXPORT void ctor_4DB170();
    EXPORT void dtor_4DB1A0();
    EXPORT void Free_4DB1C0();
    EXPORT void Init_4DB200(PathData* pPathData, __int16 level, __int16 path, __int16 cameraId, BYTE** ppPathRes);

    
    EXPORT void Loader_4DB800(__int16 xpos, __int16 ypos, int loadMode, __int16 typeToLoad);
    
    EXPORT Path_TLV* Get_First_TLV_For_Offsetted_Camera_4DB610(__int16 cam_x_idx, __int16 cam_y_idx);
    EXPORT static Path_TLV* __stdcall Next_TLV_4DB6A0(Path_TLV* pTlv);
    EXPORT Path_TLV* TLV_First_Of_Type_In_Camera_4DB6D0(unsigned __int16 objectType, __int16 camX);
    EXPORT static Path_TLV* __stdcall TLV_Next_Of_Type_4DB720(Path_TLV* pTlv, unsigned __int16 type);

    unsigned __int16 field_0_levelId;
    unsigned __int16 field_2_pathId;
    __int16 field_4_cameraId;
    unsigned __int16 field_6_cams_on_x;
    unsigned __int16 field_8_cams_on_y;
    __int16 field_A;
    PathData* field_C_pPathData;
    BYTE** field_10_ppRes;
};
ALIVE_ASSERT_SIZEOF(Path, 0x14);
