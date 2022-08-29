#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint.hpp"
#include "Psx.hpp"
#include "ResourceManager.hpp"
#include "Path.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/BinaryPath.hpp"


class BaseAliveGameObject;
class Camera;
struct PathData;
struct Path_TLV;

enum class LevelIds : s16;

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

struct CameraName final
{
    char_type name[8];
};
ALIVE_ASSERT_SIZEOF(CameraName, 8);

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

struct Path_PathTransition final : public Path_TLV
{
    LevelIds mNextLevel;
    s16 mNextPath;
    s16 mNextCamera;
    s16 mMovieId;
    s16 mWipeEffect;
    Scale_short mNextPathScale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_PathTransition, 0x1C);

enum class LoadMode : s16;

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

    enum class CamChangeStates : s16
    {
        eInactive_0 = 0,
        eSliceCam_1 = 1,
        eInstantChange_2 = 2
    };
    CamChangeStates mCamState = CamChangeStates::eInactive_0;
    s16 mForceLoad = 0;

    CameraSwapEffects mCameraSwapEffect = CameraSwapEffects::eInstantChange_0;
    u16 mFmvBaseId = 0;

    MapDirections mMapDirection = MapDirections::eMapLeft_0;

    BaseAliveGameObject* mAliveObj = nullptr;

    CameraSwapEffects mConvertedCameraSwapEffect_NeverRead = CameraSwapEffects::eInstantChange_0;
    s16 mDoorTransition = 0;
    s16 mTeleporterTransition = 0;

    FP_Point field_24_camera_offset = {};

    Camera* field_2C_camera_array[5] = {};
    Camera* field_40_stru_5[5] = {};

    s16 field_CC_unused = 0;
    s16 mFreeAllAnimAndPalts = 0;

    s16 mCamIdxOnX = 0;
    s16 mCamIdxOnY = 0;

    const PathData* mPathData = nullptr;
    u8* mRestoreQuickSaveData = nullptr;


    void ScreenChange();

    void FreePathResourceBlocks();
    BinaryPath* GetPathResourceBlockPtr(u32 pathId);
    void ClearPathResourceBlocks();

    void RemoveObjectsWithPurpleLight(s16 a2);
    void Handle_PathTransition();
    void Init(EReliveLevelIds level, s16 path, s16 camera, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange);
    void Shutdown();
    void Reset();
    Map();
    ~Map();
    void GoTo_Camera();

    void Get_map_size(PSX_Point* pPoint);
    void GetCurrentCamCoords(PSX_Point* pPoint);
    void Get_Abe_Spawn_Pos(PSX_Point* pPoint);
    s16 GetOverlayId() override;
    void Create_FG1s();
    CameraPos Rect_Location_Relative_To_Active_Camera(const PSX_RECT* pRect);
    s16 SetActiveCam(EReliveLevelIds level, s16 path, s16 cam, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange);
    static BaseGameObject* FMV_Camera_Change(u8** ppBits, Map* pMap, EReliveLevelIds lvlId);
    Camera* Create_Camera(s16 xpos, s16 ypos, s32 a4);
    static void Load_Path_Items(Camera* pCamera, LoadMode loadMode);

    static void LoadResource(const char_type* pFileName, s32 type, s32 resourceId, LoadMode loadMode, s16 bDontLoad = 0);
    static void LoadResourcesFromList(const char_type* pFileName, ResourceManager::ResourcesToLoadList* pList, LoadMode loadMode, s16 bDontLoad = 0);

    s16 SetActiveCameraDelayed(MapDirections direction, BaseAliveGameObject* pObj, s16 kMinus1);

    s16 Is_Point_In_Current_Camera(EReliveLevelIds level, s32 path, FP xpos, FP ypos, s16 width);
    CameraPos GetDirection(EReliveLevelIds level, s32 path, FP xpos, FP ypos);

    s16 Get_Camera_World_Rect(CameraPos camIdx, PSX_RECT* pRect);

private:
    Camera* GetCamera(CameraPos pos);

    void CreateScreenTransistionForTLV(relive::Path_TLV* pTlv);
    void ScreenChange_Common();

    std::vector<std::unique_ptr<class BinaryPath>> mLoadedPaths;
};
ALIVE_ASSERT_SIZEOF(Map, 0xDC);

ALIVE_VAR_EXTERN(Map, gMap);
ALIVE_VAR_EXTERN(s16, sMap_bDoPurpleLightEffect_5C311C);
extern const CameraSwapEffects kPathChangeEffectToInternalScreenChangeEffect_55D55C[10];
