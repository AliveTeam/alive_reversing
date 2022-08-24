#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibCommon/Psx_common.hpp"
#include "../AliveLibCommon/FixedPoint_common.hpp"
#include "PathData.hpp"
#include "../AliveLibAE/Path.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibAE/Map.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "Path.hpp"

namespace AO {

void Map_ForceLink();

struct PathData;
class Camera;
class BaseAliveGameObject;
class CameraSwapper;

struct CameraName final
{
    char_type name[8];
};
ALIVE_ASSERT_SIZEOF(CameraName, 8);

struct OverlayRecord final
{
    const char_type* field_0_fileName;
    u32 field_4_pos;
};
ALIVE_ASSERT_SIZEOF(OverlayRecord, 0x8);

struct OverlayRecords final
{
    OverlayRecord records[54];
};

struct Map_PathsArray final
{
    u8** field_0_pPathRecs[30];
};
ALIVE_ASSERT_SIZEOF(Map_PathsArray, 120);

namespace CameraIds::Menu
{
    const s16 eMainMenu_1 = 1;
    const s16 eOptions_2 = 2;
    const s16 eGamespeakGamepad_3 = 3;
    const s16 eMotions_4 = 4;
    const s16 eSound_5 = 5;
    const s16 eLoad_6 = 6;
    const s16 eMotionsGamespeakGamepad_7 = 7;
    const s16 eCopyright_10 = 10;
    const s16 eLoading_21 = 21;
    const s16 eFmvSelect_30 = 30;
    const s16 eLvlSelect_31 = 31;
    const s16 eGamespeakKeyboard_33 = 33;
    const s16 eMotionsGamespeakKeyboard_37 = 37;
    const s16 eController_40 = 40;
    const s16 eControllerConfig_41 = 41;
}

// TODO: Assuming the same values as AE - test this
enum class CameraSwapEffects : s16
{
    eInstantChange_0 = 0,
    eLeftToRight_1 = 1,     // Left to right
    eRightToLeft_2 = 2,     // Right to left
    eTopToBottom_3 = 3,     // Top to bottom
    eBottomToTop_4 = 4,     // Bottom to top
    ePlay1FMV_5 = 5,        // Play single fmv
    eVerticalSplit_6 = 6,   // Screen splits from the middle and moves out up/down
    eHorizontalSplit_7 = 7, // Screen splits from the middle and moves out left/right
    eBoxOut_8 = 8,          // A rect "grows" out from the centre of the screen
    ePlay2FMVs_9 = 9,       // Play 2 fmvs
    ePlay3FMVs_10 = 10,     // Play 3 fmvs - apparently just taking an array of fmvs is too simple ?
    eUnknown_11 = 11        // Unknown, has special handing in the map object
};

extern const CameraSwapEffects kPathChangeEffectToInternalScreenChangeEffect_4CDC78[10];


struct Path_EnemyStopper final : public Path_TLV
{
    enum class StopDirection : s16
    {
        Left_0 = 0,
        Right_1 = 1,
        Both_2 = 2,
    };
    StopDirection mStopDirection;
    s16 mSwitchId;
};
// TODO:: size

struct Path_PathTransition final : public Path_TLV
{
    LevelIds mNextLevel;
    s16 mNextPath;
    s16 mNextCamera;
    s16 mMovieId;
    u16 mWipeEffect;
    Scale_short mNextPathScale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_PathTransition, 0x24);

enum class CameraPos : s16
{
    eCamInvalid_m1 = -1,
    eCamCurrent_0 = 0,
    eCamTop_1 = 1,
    eCamBottom_2 = 2,
    eCamLeft_3 = 3,
    eCamRight_4 = 4,
    eCamNone_5 = 5, // Not "in" the camera
};

class Map final : public IMap
{
public:
    enum class MapDirections : s16
    {
        eMapLeft_0 = 0,
        eMapRight_1 = 1,
        eMapTop_2 = 2,
        eMapBottom_3 = 3,
    };

    Map();

    void Init(EReliveLevelIds level, s16 path, s16 camera, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange);

    void Shutdown();
    void Reset();

    s16 SetActiveCam(EReliveLevelIds level, s16 path, s16 cam, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange);

    void ScreenChange();

    void FreePathResourceBlocks();
    void GetPathResourceBlockPtrs();
    u8** GetPathResourceBlockPtr(u32 pathId);
    void ClearPathResourceBlocks();

    void GoTo_Camera();

    void Loader(s16 camX, s16 camY, LoadMode loadMode, TlvTypes typeToLoad);

    void RemoveObjectsWithPurpleLight(s16 bMakeInvisible);

    void Handle_PathTransition();

    void ScreenChange_Common();

    void Get_map_size(PSX_Point* pPoint);

    void GetCurrentCamCoords(PSX_Point* pPoint);
    s16 GetOverlayId() override;

    static CameraSwapper* FMV_Camera_Change(u8** ppBits, Map* pMap, EReliveLevelIds levelId);

    void Create_FG1s();

    Camera* Create_Camera(s16 xpos, s16 ypos, s32 a4);

    void RestoreBlyData(const u8* pSaveData);

    void Load_Path_Items(Camera* pCamera, LoadMode loadMode);

    relive::Path_TLV* TLV_First_Of_Type_In_Camera(TlvTypes type, s32 camX);

    relive::Path_TLV* TLV_Get_At_446260(s16 xpos, s16 ypos, s16 width, s16 height, TlvTypes typeToFind);

    relive::Path_TLV* TLV_Get_At_446060(relive::Path_TLV* pTlv, FP xpos, FP ypos, FP width, FP height);

    void sub_447430(u16 pathNum);

    CameraPos GetDirection(EReliveLevelIds level, s32 path, FP xpos, FP ypos);

    CameraPos Rect_Location_Relative_To_Active_Camera(const PSX_RECT* pRect, s16 width);

    s16 Get_Camera_World_Rect(CameraPos camIdx, PSX_RECT* pRect);

    s16 Is_Point_In_Current_Camera(EReliveLevelIds level, s32 path, FP xpos, FP ypos, s16 width);

    s16 SetActiveCameraDelayed(MapDirections direction, BaseAliveGameObject* pObj, s16 swapEffect);

    Camera* GetCamera(CameraPos pos);

    // NOTE: Global func in AE
    void Start_Sounds_For_Objects_In_Near_Cameras();

    // NOTE: Part of Path object in AE
    void Start_Sounds_For_Objects_In_Camera(CameraPos direction, s16 cam_x_idx, s16 cam_y_idx);

    // NOTE: Part of Path object in AE
    relive::Path_TLV* Get_First_TLV_For_Offsetted_Camera(s16 camX, s16 camY);

    void SaveBlyData(u8* pSaveBuffer);

    enum class CamChangeStates : s16
    {
        eInactive_0 = 0,
        eSliceCam_1 = 1,
        eInstantChange_2 = 2
    };
    CamChangeStates field_6_state = CamChangeStates::eInactive_0;
    s16 field_8_force_load = 0;

    CameraSwapEffects field_10_screenChangeEffect = CameraSwapEffects::eInstantChange_0;
    s16 field_12_fmv_base_id = 0;
    MapDirections field_14_direction = MapDirections::eMapLeft_0;
    s16 field_16 = 0;
    BaseAliveGameObject* field_18_pAliveObj = nullptr;
    CameraSwapEffects field_1C_cameraSwapEffect = CameraSwapEffects::eInstantChange_0;
    s16 field_1E_door = 0;
    s16 field_20_camX_idx = 0;
    s16 field_22_camY_idx = 0;
    u16 field_24_max_cams_x = 0;
    u16 field_26_max_cams_y = 0;;
    s16 field_2A = 0;
    FP_Point field_2C_camera_offset = {};
    Camera* field_34_camera_array[5] = {};
    Camera* field_48_stru_5[5] = {};
    Map_PathsArray field_5C_path_res_array = {};
    const PathData* field_D4_pPathData = nullptr;
    s16 field_D8 = 0;
    s16 field_DA_bMapChanged = 0;
    s16 field_DC_free_all_anim_and_palts = 0;
    s16 field_DE = 0;
    u8* field_E0_save_data = nullptr;
};

ALIVE_ASSERT_SIZEOF(Map, 0xE4);

ALIVE_VAR_EXTERN(Map, gMap);
ALIVE_VAR_EXTERN(s16, sMap_bDoPurpleLightEffect_507C9C);

ALIVE_VAR_EXTERN(OverlayRecords, sOverlayTable_4C5AA8);
ALIVE_VAR_EXTERN(Camera*, sCameraBeingLoaded_507C98);

s32 MaxGridBlocks_41FA10(FP scale);

} // namespace AO
