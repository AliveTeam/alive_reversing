#pragma once

#include "FunctionFwd.hpp"
#include "Psx_common.hpp"
#include "FixedPoint_common.hpp"
#include "PathData.hpp"
#include "BaseGameObject.hpp"

START_NS_AO

void Map_ForceLink();

struct PathData;
class Camera;
class BaseAliveGameObject;

enum class CameraSwapEffects : __int16;

struct CameraName
{
    char name[8];
};
ALIVE_ASSERT_SIZEOF(CameraName, 8);

// TODO move to another file
class CameraSwapper : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT CameraSwapper* Vdtor_48D380(signed int flags);

    EXPORT CameraSwapper* ctor_48C7A0(BYTE** ppBits, CameraSwapEffects changeEffect, __int16 xpos, __int16 ypos);

    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    __int16 field_28;
    __int16 field_2A;
    int field_2C;
    __int16 field_30;
    __int16 field_32;
    __int16 field_34;
    __int16 field_36;
    __int16 field_38;
    __int16 field_3A;
    __int16 field_3C;
    __int16 field_3E;
    __int16 field_40;
    __int16 field_42;
    int field_44;
};
ALIVE_ASSERT_SIZEOF(CameraSwapper, 0x48);


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

    EXPORT void Init_443EE0(LevelIds level, __int16 path, __int16 camera, CameraSwapEffects screenChangeEffect, __int16 fmvBaseId, __int16 forceChange);
    
    EXPORT void Shutdown_443F90();
    void Reset();
    
    EXPORT __int16 SetActiveCam_444660(LevelIds level, __int16 path, __int16 cam, CameraSwapEffects screenChangeEffect, __int16 fmvBaseId, __int16 forceChange);

    EXPORT __int16 GetOverlayId_4440B0();

    EXPORT void ScreenChange_4444D0();

    EXPORT void GoTo_Camera_445050();

    EXPORT void Loader_446590(__int16 camX, __int16 camY, int loadMode, __int16 typeToLoad);

    EXPORT BYTE* TLV_Reset_446870(unsigned int a2, __int16 a3, unsigned __int8 a4, char a5);

    EXPORT void RemoveObjectsWithPurpleLight_4440D0(__int16 bMakeInvisible);

    EXPORT void Handle_PathTransition_444DD0();

    void ScreenChange_Common();

    EXPORT void Get_map_size_444870(PSX_Point* pPoint);

    EXPORT void GetCurrentCamCoords_444890(PSX_Point* pPoint);
    __int16 GetOverlayId();

    EXPORT static CameraSwapper* CC FMV_Camera_Change_4458D0(BYTE** ppBits, Map* pMap, LevelIds levelId);

    EXPORT void Create_FG1s_4447D0();

    EXPORT Camera* Create_Camera_445BE0(__int16 xpos, __int16 ypos, int a4);

    EXPORT void RestoreObjectStates_446A90(__int16* pSaveData);

    EXPORT void Load_Path_Items_445DA0(Camera* pCamera, __int16 kZero);

    EXPORT Path_TLV* TLV_First_Of_Type_In_Camera_4464A0(unsigned __int16 type, int camX);

    EXPORT Path_TLV* CCSTD TLV_Next_Of_Type_446500(Path_TLV* pTlv, unsigned __int16 type);

    EXPORT Path_TLV* TLV_Get_At_446260(__int16 xpos, __int16 ypos, __int16 width, __int16 height, unsigned __int16 typeToFind);

    LevelIds field_0_current_level;
    __int16 field_2_current_path;
    __int16 field_4_current_camera;
    __int16 field_6_state;
    __int16 field_8_force_load;
    LevelIds field_A_level;
    __int16 field_C_path;
    __int16 field_E_camera;
    CameraSwapEffects field_10_screenChangeEffect;
    __int16 field_12_fmv_base_id;
    MapDirections field_14_direction;
    __int16 field_16;
    BaseAliveGameObject* field_18_pAliveObj;
    __int16 field_1C_cameraSwapEffect;
    __int16 field_1E_door;
    __int16 field_20_camX_idx;
    __int16 field_22_camY_idx;
    unsigned __int16 field_24_max_cams_x;
    unsigned __int16 field_26_max_cams_y;
    __int16 field_28_cd_or_overlay_num;
    __int16 field_2A;
    FP_Point field_2C_camera_offset;
    Camera* field_34_camera_array[5];
    Camera* field_48_stru_5[5];
    Map_PathsArray field_5C_path_res_array;
    const PathData* field_D4_pPathData;
    __int16 field_D8;
    __int16 field_DA_bMapChanged;
    __int16 field_DC_free_all_anim_and_palts;
    __int16 field_DE;
    __int16* field_E0_save_data;
};

ALIVE_ASSERT_SIZEOF(Map, 0xE4);

ALIVE_VAR_EXTERN(Map, gMap_507BA8);

END_NS_AO

