#pragma once

#include "FunctionFwd.hpp"
#include "Psx_common.hpp"
#include "FixedPoint_common.hpp"
#include "PathData.hpp"
#include "BaseGameObject.hpp"

namespace AO {

void Map_ForceLink();

struct PathData;
class Camera;
class BaseAliveGameObject;
class CameraSwapper;

struct CameraName
{
    char name[8];
};
ALIVE_ASSERT_SIZEOF(CameraName, 8);

struct OverlayRecord
{
    const char* field_0_fileName;
    DWORD field_4_pos;
};
ALIVE_ASSERT_SIZEOF(OverlayRecord, 0x8);

struct OverlayRecords
{
    OverlayRecord records[54];
};

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

extern const CameraSwapEffects kPathChangeEffectToInternalScreenChangeEffect_4CDC78[10];

enum TLV_Flags
{
    eBit1_Created = 0x1,
    eBit2_Unknown = 0x2,
    eBit3_End_TLV_List = 0x4,
};

struct Path_TLV
{
    BitField8<TLV_Flags> field_0_flags;
    char field_1_unknown;
    __int16 field_2_length;
    int field_4_type;
    int field_8;
    PSX_Point field_C_sound_pos;
    PSX_Point field_10_top_left;
    PSX_Point field_14_bottom_right;

    // Note: Part of Path object in AE
    EXPORT static Path_TLV* CCSTD Next_446460(Path_TLV* pTlv);
    static Path_TLV* Next_NoCheck(Path_TLV* pTlv);

    EXPORT static Path_TLV* CCSTD TLV_Next_Of_Type_446500(Path_TLV* pTlv, unsigned __int16 type);

    // Some strange self terminate check that is inlined everywhere
    void RangeCheck()
    {
        if (field_2_length < 24u || field_2_length > 480u)
        {
            field_0_flags.Set(eBit3_End_TLV_List);
        }
    }
};
ALIVE_ASSERT_SIZEOF(Path_TLV, 0x18);

struct Path_EnemyStopper : public Path_TLV
{
    enum class StopDirection : __int16
    {
        Left_0 = 0,
        Right_1 = 1,
        Both_2 = 2,
    };
    StopDirection field_18_direction;
    __int16 field_1A_id;
};

struct Path_ChangeTLV : public Path_TLV
{
    LevelIds field_18_level;
    __int16 field_1A_path;
    __int16 field_1C_camera;
    __int16 field_1E_movie;
    unsigned __int16 field_20_wipe;
    __int16 field_22_scale;
};
ALIVE_ASSERT_SIZEOF(Path_ChangeTLV, 0x24);

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

    EXPORT static void ctor_static_443E10();
    EXPORT static void dtor_static_443E60();

    void ctor();

    EXPORT void Init_443EE0(LevelIds level, __int16 path, __int16 camera, CameraSwapEffects screenChangeEffect, __int16 fmvBaseId, __int16 forceChange);
    
    EXPORT void Shutdown_443F90();
    void Reset();
    
    EXPORT __int16 SetActiveCam_444660(LevelIds level, __int16 path, __int16 cam, CameraSwapEffects screenChangeEffect, __int16 fmvBaseId, __int16 forceChange);

    EXPORT __int16 GetOverlayId_4440B0();

    EXPORT void ScreenChange_4444D0();

    EXPORT void GoTo_Camera_445050();

    EXPORT void Loader_446590(__int16 camX, __int16 camY, LoadMode loadMode, __int16 typeToLoad);

    EXPORT void TLV_Reset_446870(unsigned int tlvOffset_levelId_PathId, __int16 hiFlags, char bSetCreated, char bBit2);

    EXPORT void RemoveObjectsWithPurpleLight_4440D0(__int16 bMakeInvisible);

    EXPORT void Handle_PathTransition_444DD0();

    void ScreenChange_Common();

    EXPORT void Get_map_size_444870(PSX_Point* pPoint);

    EXPORT void GetCurrentCamCoords_444890(PSX_Point* pPoint);
    __int16 GetOverlayId();

    EXPORT static CameraSwapper* CC FMV_Camera_Change_4458D0(BYTE** ppBits, Map* pMap, LevelIds levelId);

    EXPORT void Create_FG1s_4447D0();

    EXPORT Camera* Create_Camera_445BE0(__int16 xpos, __int16 ypos, int a4);

    EXPORT void RestoreBlyData_446A90(const BYTE* pSaveData);

    EXPORT void Load_Path_Items_445DA0(Camera* pCamera, __int16 kZero);

    EXPORT Path_TLV* TLV_First_Of_Type_In_Camera_4464A0(TlvTypes type, int camX);

    EXPORT Path_TLV* TLV_Get_At_446260(__int16 xpos, __int16 ypos, __int16 width, __int16 height, unsigned __int16 typeToFind);

    EXPORT Path_TLV* TLV_Get_At_446060(Path_TLV* pTlv, FP xpos, FP ypos, FP width, FP height);

    EXPORT void sub_447430(unsigned __int16 pathNum);

    CameraPos GetDirection(LevelIds level, int path, FP xpos, FP ypos)
    {
        return GetDirection_444A40(static_cast<int>(level), path, xpos, ypos);
    }

    EXPORT CameraPos GetDirection_444A40(int level, int path, FP xpos, FP ypos);

    EXPORT CameraPos Rect_Location_Relative_To_Active_Camera_4448C0(PSX_RECT* pRect, __int16 width);

    EXPORT signed __int16 Get_Camera_World_Rect_444C30(CameraPos camIdx, PSX_RECT* pRect);

    __int16 Is_Point_In_Current_Camera_4449C0(LevelIds level, int path, FP xpos, FP ypos, __int16 width)
    {
        return Is_Point_In_Current_Camera_4449C0(static_cast<int>(level), path, xpos, ypos, width);
    }

    EXPORT __int16 Is_Point_In_Current_Camera_4449C0(int level, int path, FP xpos, FP ypos, __int16 width);

    EXPORT signed __int16 SetActiveCameraDelayed_444CA0(MapDirections direction, BaseAliveGameObject* pObj, __int16 swapEffect);

    Camera* GetCamera(CameraPos pos);

    // NOTE: Global func in AE
    EXPORT void Start_Sounds_For_Objects_In_Near_Cameras_4467D0();

    // NOTE: Part of Path object in AE
    EXPORT void Start_Sounds_For_Objects_In_Camera_4466A0(CameraPos direction, __int16 cam_x_idx, __int16 cam_y_idx);

    // NOTE: Part of Path object in AE
    EXPORT Path_TLV* Get_First_TLV_For_Offsetted_Camera_4463B0(__int16 camX, __int16 camY);

    EXPORT void SaveBlyData_446900(BYTE* pSaveBuffer);

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
    CameraSwapEffects field_1C_cameraSwapEffect;
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
    BYTE* field_E0_save_data;
};

ALIVE_ASSERT_SIZEOF(Map, 0xE4);

ALIVE_VAR_EXTERN(Map, gMap_507BA8);
ALIVE_VAR_EXTERN(short, sMap_bDoPurpleLightEffect_507C9C);

ALIVE_VAR_EXTERN(OverlayRecords, sOverlayTable_4C5AA8);
ALIVE_VAR_EXTERN(Camera*, sCameraBeingLoaded_507C98);

EXPORT int CC MaxGridBlocks_41FA10(FP scale);

}

