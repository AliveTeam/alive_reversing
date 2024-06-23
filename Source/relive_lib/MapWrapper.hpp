#pragma once

#include "Types.hpp"
#include "../AliveLibAE/Math.hpp"

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

enum class EReliveLevelIds : s16
{
    eNone = 0, // TODO: Shouldn't depend on this value in the future
    eMenu = 2000,
    eMines,
    eNecrum,
    eMudomoVault,
    eMudancheeVault,
    eFeeCoDepot,
    eBarracks,
    eMudancheeVault_Ender,
    eBonewerkz,
    eBrewery,
    eBrewery_Ender,
    eMudomoVault_Ender,
    eFeeCoDepot_Ender,
    eBarracks_Ender,
    eBonewerkz_Ender,
    eCredits,
    eRuptureFarms,
    eLines,
    eForest,
    eForestTemple,
    eStockYards,
    eStockYardsReturn,
    eDesert,
    eDesertTemple,
    eBoardRoom,
    eRuptureFarmsReturn,
    eForestChase,
    eDesertEscape,
};

enum class LevelIds : s16;
namespace AO {
    enum class LevelIds : s16;
}

namespace relive {
    class Path_TLV;
}

class BaseAliveGameObject;

class MapWrapper
{
public:
    // TODO: Need to remap EReliveLevelIds enums before we can use this
    static EReliveLevelIds FromAO(AO::LevelIds levelId);
    static AO::LevelIds ToAO(EReliveLevelIds levelId);

    static EReliveLevelIds FromAESaveData(LevelIds levelId);
    static EReliveLevelIds FromAE(LevelIds levelId);
    static LevelIds ToAE(EReliveLevelIds levelId);
};

class Guid;
struct PSX_RECT;
class Camera;

enum class ReliveTypes : s16;

enum class MapDirections : s16
{
    eMapLeft_0 = 0,
    eMapRight_1 = 1,
    eMapTop_2 = 2,
    eMapBottom_3 = 3,
};

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

class IMap
{
public:
    enum class CamChangeStates : s16
    {
        eInactive_0 = 0,
        eSliceCam_1 = 1,
        eInstantChange_2 = 2
    };

    virtual ~IMap()
    {

    }

    bool LevelChanged() const
    {
        return mCurrentLevel != mNextLevel;
    }

    bool PathChanged() const
    {
        return mCurrentPath != mNextPath;
    }

    bool CameraChanged() const
    {
        return mCurrentCamera != mNextCamera;
    }

    EReliveLevelIds mCurrentLevel;
    s16 mCurrentPath;
    s16 mCurrentCamera;

    EReliveLevelIds mNextLevel;
    s16 mNextPath;
    s16 mNextCamera;

    s16 mOverlayId;

    virtual s16 GetOverlayId() = 0;

    virtual void TLV_Reset(const Guid& tlvId, s16 hiFlags = -1) = 0;
    virtual void TLV_Persist(const Guid& tlvId, s16 hiFlags = -1) = 0;
    virtual void TLV_Delete(const Guid& tlvId, s16 hiFlags = -1) = 0;
    virtual void Set_TLVData(const Guid& tlvId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed) = 0;

    virtual CameraPos Rect_Location_Relative_To_Active_Camera(const PSX_RECT* pRect, s16 width = 0) = 0;

    s16 SetActiveCameraDelayed(MapDirections direction, BaseAliveGameObject* pObj, s16 swapEffect);

    Camera* GetCamera(CameraPos pos);

    virtual relive::Path_TLV* VTLV_Get_At_Of_Type(s16 xpos, s16 ypos, s16 width, s16 height, ReliveTypes typeToFind) = 0;

    virtual s16 Is_Point_In_Current_Camera(EReliveLevelIds level, s32 path, FP xpos, FP ypos, s16 width) = 0;

    virtual CameraPos GetDirection(EReliveLevelIds level, s32 path, FP xpos, FP ypos) = 0;

    virtual relive::Path_TLV* TLV_First_Of_Type_In_Camera(ReliveTypes objectType, s16 camX) = 0;

    virtual relive::Path_TLV* TLV_Get_At(relive::Path_TLV* pTlv, FP xpos, FP ypos, FP width, FP height) = 0;

    virtual void GetCurrentCamCoords(PSX_Point* pPoint) = 0;

public:
    MapDirections mMapDirection = MapDirections::eMapLeft_0;
    BaseAliveGameObject* mAliveObj = nullptr;
    CamChangeStates mCamState = CamChangeStates::eInactive_0;

    Camera* field_2C_camera_array[5] = {};
    Camera* field_40_stru_5[5] = {};
};
