#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"
#include "Psx.hpp"
#include "ResourceManager.hpp"

void Map_ForceLink();

EXPORT void CC static_map_init_4802D0();

struct Map_120
{
    BYTE** field_0_pPathRecs[30];
};
ALIVE_ASSERT_SIZEOF(Map_120, 120);

class BaseAliveGameObject;
class Camera;
struct PathData;
struct Path_TLV;

enum class LevelIds : __int16;

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

struct CameraName
{
    char name[8];
};
ALIVE_ASSERT_SIZEOF(CameraName, 8);

enum class CameraPos : __int16
{
    eCamInvalid_m1 = -1,
    eCamCurrent_0 = 0,
    eCamTop_1 = 1,
    eCamBottom_2 = 2,
    eCamLeft_3 = 3,
    eCamRight_4 = 4,
    eCamNone_5 = 5,   // Not "in" the camera
};

class Map
{
public:
    enum class MapDirections : __int16
    {
        eMapLeft_0 = 0,
        eMapRight_1 = 1,
        eMapTop_2 = 2,
        eMapBottom_3 = 3,
    };

    LevelIds sCurrentLevelId_5C3030;
    __int16 sCurrentPathId_5C3032;
    __int16 sCurrentCamId_5C3034;
    __int16 field_6_state;
    __int16 field_8_force_load;

    LevelIds field_A_5C303A_levelId;
    __int16 field_C_5C303C_pathId;
    __int16 field_E_cameraId;
    CameraSwapEffects field_10_screen_change_effect;
    unsigned __int16 field_12_fmv_base_id;

    MapDirections field_14_direction;
    __int16 field_16;

    BaseAliveGameObject *field_18_pAliveObj;

    CameraSwapEffects field_1C;
    __int16 field_1E_door;
    __int16 field_20;
    __int16 field_22;

    FP_Point field_24_camera_offset;

    Camera* field_2C_5C305C_camera_array[5];
    Camera* field_40_stru_5[5];
    Map_120 field_54_path_res_array;

    __int16 field_CC;
    __int16 field_CE_free_all_anim_and_palts;

    __int16 field_D0_cam_x_idx;
    __int16 field_D2_cam_y_idx;

    const PathData* field_D4_ptr;
    BYTE* field_D8_restore_quick_save;


    EXPORT void ScreenChange_480B80();
    EXPORT void RemoveObjectsWithPurpleLight(__int16 a2);
    EXPORT void sub_481610();
    EXPORT void Init_4803F0(LevelIds level, __int16 path, __int16 camera, CameraSwapEffects screenChangeEffect, __int16 a6, __int16 forceChange);
    EXPORT void Shutdown_4804E0();
    EXPORT void Reset_4805D0();
    EXPORT void GoTo_Camera_481890();

    EXPORT void Get_map_size_480640(PSX_Point* pPoint);
    EXPORT void GetCurrentCamCoords_480680(PSX_Point* pPoint);
    EXPORT void Get_Abe_Spawn_Pos_4806D0(PSX_Point* pPoint);
    EXPORT __int16 Get_Path_Unknown_480710();
    EXPORT void Create_FG1s_480F10();
    EXPORT CameraPos Is_Rect_In_Current_Camera_480FE0(PSX_RECT* pRect);
    EXPORT signed __int16 SetActiveCam_480D30(LevelIds level, __int16 path, __int16 cam, CameraSwapEffects screenChangeEffect, __int16 fmvBaseId, __int16 forceChange);
    EXPORT static BaseGameObject* CC FMV_Camera_Change_482650(BYTE** ppBits, Map* pMap, LevelIds lvlId);
    EXPORT Camera* Create_Camera_4829E0(__int16 xpos, __int16 ypos, int a4);
    EXPORT static void CCSTD Load_Path_Items_482C10(Camera* pCamera, __int16 loadMode);

    EXPORT static void CC LoadResource_4DBE00(const char* pFileName, int type, int resourceId, __int16 loadMode, __int16 bDontLoad = FALSE);
    EXPORT static void CC LoadResourcesFromList_4DBE70(const char* pFileName, ResourceManager::ResourcesToLoadList* pList, __int16 loadMode, __int16 bDontLoad = FALSE);

    EXPORT signed __int16 SetActiveCameraDelayed_4814A0(MapDirections direction, BaseAliveGameObject* pObj, __int16 kMinus1);

    // Type safe wrappers as int level is bigger than the enum type size
    __int16 Is_Point_In_Current_Camera_4810D0(LevelIds level, int path, FP xpos, FP ypos, __int16 width)
    {
        return Is_Point_In_Current_Camera_4810D0(static_cast<int>(level), path, xpos, ypos, width);
    }
    
    CameraPos GetDirection_4811A0(LevelIds level, int path, FP xpos, FP ypos)
    {
        return GetDirection_4811A0(static_cast<int>(level), path, xpos, ypos);
    }

    EXPORT signed __int16 Get_Camera_World_Rect_481410(CameraPos camIdx, PSX_RECT* pRect);
private:
    EXPORT __int16 Is_Point_In_Current_Camera_4810D0(int level, int path, FP xpos, FP ypos, __int16 width);
    EXPORT CameraPos GetDirection_4811A0(int level, int path, FP xpos, FP ypos);

    Camera* GetCamera(CameraPos pos);

    void CreateScreenTransistionForTLV(Path_TLV* pTlv);
    void sub_480B80_Common();
};
ALIVE_ASSERT_SIZEOF(Map, 0xDC);

ALIVE_VAR_EXTERN(Map, gMap_5C3030);
ALIVE_VAR_EXTERN(__int16, sMap_bDoPurpleLightEffect_5C311C);
extern const CameraSwapEffects kPathChangeEffectToInternalScreenChangeEffect_55D55C[12];
