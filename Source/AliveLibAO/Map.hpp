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
    s8 name[8];
};
ALIVE_ASSERT_SIZEOF(CameraName, 8);

struct OverlayRecord
{
    const s8* field_0_fileName;
    u32 field_4_pos;
};
ALIVE_ASSERT_SIZEOF(OverlayRecord, 0x8);

struct OverlayRecords
{
    OverlayRecord records[54];
};

struct Map_PathsArray
{
    u8** field_0_pPathRecs[30];
};
ALIVE_ASSERT_SIZEOF(Map_PathsArray, 120);

// TODO: Assuming the same values as AE - test this
enum class CameraSwapEffects : s16
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

// ABI fix to allow using the enum as a 32bit type
struct TlvTypes32
{
    TlvTypes mType;
    s16 padto32Bits;

    bool operator == (TlvTypes type) const
    {
        return mType == type;
    }
};
static_assert(std::is_pod<TlvTypes32>::value, "TlvTypes32 must be pod");
ALIVE_ASSERT_SIZEOF(TlvTypes32, 4);

struct Path_TLV
{
    BitField8<TLV_Flags> field_0_flags;
    s8 field_1_unknown;
    s16 field_2_length;
    TlvTypes32 field_4_type;
    s32 field_8; // only ever used as some sort of hacky memory overwrite check, always 0 in the Path data
    PSX_Point field_C_sound_pos; // a completely pointless copy of field_10_top_left, the data is always the same in all released Paths
    PSX_Point field_10_top_left;
    PSX_Point field_14_bottom_right;

    // Note: Part of Path object in AE
    EXPORT static Path_TLV* CCSTD Next_446460(Path_TLV* pTlv);
    static Path_TLV* Next_NoCheck(Path_TLV* pTlv);

    EXPORT static Path_TLV* CCSTD TLV_Next_Of_Type_446500(Path_TLV* pTlv, TlvTypes type);

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
    enum class StopDirection : s16
    {
        Left_0 = 0,
        Right_1 = 1,
        Both_2 = 2,
    };
    StopDirection field_18_direction;
    s16 field_1A_id;
};

struct Path_ChangeTLV : public Path_TLV
{
    LevelIds field_18_level;
    s16 field_1A_path;
    s16 field_1C_camera;
    s16 field_1E_movie;
    u16 field_20_wipe;
    s16 field_22_scale;
};
ALIVE_ASSERT_SIZEOF(Path_ChangeTLV, 0x24);

enum class CameraPos : s16
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
    enum class MapDirections : s16
    {
        eMapLeft_0 = 0,
        eMapRight_1 = 1,
        eMapTop_2 = 2,
        eMapBottom_3 = 3,
    };

    EXPORT static void ctor_static_443E10();
    EXPORT static void dtor_static_443E60();

    void ctor();

    EXPORT void Init_443EE0(LevelIds level, s16 path, s16 camera, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange);
    
    EXPORT void Shutdown_443F90();
    void Reset();
    
    EXPORT s16 SetActiveCam_444660(LevelIds level, s16 path, s16 cam, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange);

    EXPORT s16 GetOverlayId_4440B0();

    EXPORT void ScreenChange_4444D0();

    EXPORT void GoTo_Camera_445050();

    EXPORT void Loader_446590(s16 camX, s16 camY, LoadMode loadMode, TlvTypes typeToLoad);

    EXPORT void TLV_Reset_446870(u32 tlvOffset_levelId_PathId, s16 hiFlags, s8 bSetCreated, s8 bBit2);

    EXPORT void RemoveObjectsWithPurpleLight_4440D0(s16 bMakeInvisible);

    EXPORT void Handle_PathTransition_444DD0();

    void ScreenChange_Common();

    EXPORT void Get_map_size_444870(PSX_Point* pPoint);

    EXPORT void GetCurrentCamCoords_444890(PSX_Point* pPoint);
    s16 GetOverlayId();

    EXPORT static CameraSwapper* CC FMV_Camera_Change_4458D0(u8** ppBits, Map* pMap, LevelIds levelId);

    EXPORT void Create_FG1s_4447D0();

    EXPORT Camera* Create_Camera_445BE0(s16 xpos, s16 ypos, s32 a4);

    EXPORT void RestoreBlyData_446A90(const u8* pSaveData);

    EXPORT void Load_Path_Items_445DA0(Camera* pCamera, s16 kZero);

    EXPORT Path_TLV* TLV_First_Of_Type_In_Camera_4464A0(TlvTypes type, s32 camX);

    EXPORT Path_TLV* TLV_Get_At_446260(s16 xpos, s16 ypos, s16 width, s16 height, TlvTypes typeToFind);

    EXPORT Path_TLV* TLV_Get_At_446060(Path_TLV* pTlv, FP xpos, FP ypos, FP width, FP height);

    EXPORT void sub_447430(u16 pathNum);

    CameraPos GetDirection(LevelIds level, s32 path, FP xpos, FP ypos)
    {
        return GetDirection_444A40(static_cast<s32>(level), path, xpos, ypos);
    }

    EXPORT CameraPos GetDirection_444A40(s32 level, s32 path, FP xpos, FP ypos);

    EXPORT CameraPos Rect_Location_Relative_To_Active_Camera_4448C0(PSX_RECT* pRect, s16 width);

    EXPORT s16 Get_Camera_World_Rect_444C30(CameraPos camIdx, PSX_RECT* pRect);

    s16 Is_Point_In_Current_Camera_4449C0(LevelIds level, s32 path, FP xpos, FP ypos, s16 width)
    {
        return Is_Point_In_Current_Camera_4449C0(static_cast<s32>(level), path, xpos, ypos, width);
    }

    EXPORT s16 Is_Point_In_Current_Camera_4449C0(s32 level, s32 path, FP xpos, FP ypos, s16 width);

    EXPORT s16 SetActiveCameraDelayed_444CA0(MapDirections direction, BaseAliveGameObject* pObj, s16 swapEffect);

    Camera* GetCamera(CameraPos pos);

    // NOTE: Global func in AE
    EXPORT void Start_Sounds_For_Objects_In_Near_Cameras_4467D0();

    // NOTE: Part of Path object in AE
    EXPORT void Start_Sounds_For_Objects_In_Camera_4466A0(CameraPos direction, s16 cam_x_idx, s16 cam_y_idx);

    // NOTE: Part of Path object in AE
    EXPORT Path_TLV* Get_First_TLV_For_Offsetted_Camera_4463B0(s16 camX, s16 camY);

    EXPORT void SaveBlyData_446900(u8* pSaveBuffer);

    LevelIds field_0_current_level;
    s16 field_2_current_path;
    s16 field_4_current_camera;
    s16 field_6_state;
    s16 field_8_force_load;
    LevelIds field_A_level;
    s16 field_C_path;
    s16 field_E_camera;
    CameraSwapEffects field_10_screenChangeEffect;
    s16 field_12_fmv_base_id;
    MapDirections field_14_direction;
    s16 field_16;
    BaseAliveGameObject* field_18_pAliveObj;
    CameraSwapEffects field_1C_cameraSwapEffect;
    s16 field_1E_door;
    s16 field_20_camX_idx;
    s16 field_22_camY_idx;
    u16 field_24_max_cams_x;
    u16 field_26_max_cams_y;
    s16 field_28_cd_or_overlay_num;
    s16 field_2A;
    FP_Point field_2C_camera_offset;
    Camera* field_34_camera_array[5];
    Camera* field_48_stru_5[5];
    Map_PathsArray field_5C_path_res_array;
    const PathData* field_D4_pPathData;
    s16 field_D8;
    s16 field_DA_bMapChanged;
    s16 field_DC_free_all_anim_and_palts;
    s16 field_DE;
    u8* field_E0_save_data;
};

ALIVE_ASSERT_SIZEOF(Map, 0xE4);

ALIVE_VAR_EXTERN(Map, gMap_507BA8);
ALIVE_VAR_EXTERN(s16, sMap_bDoPurpleLightEffect_507C9C);

ALIVE_VAR_EXTERN(OverlayRecords, sOverlayTable_4C5AA8);
ALIVE_VAR_EXTERN(Camera*, sCameraBeingLoaded_507C98);

EXPORT s32 CC MaxGridBlocks_41FA10(FP scale);

}

