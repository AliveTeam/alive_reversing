#pragma once

#include "stdafx.h"

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

class IMap
{
public:
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

    virtual void TLV_Reset(const Guid& tlvId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed) = 0;

    virtual CameraPos Rect_Location_Relative_To_Active_Camera(const PSX_RECT* pRect, s16 width = 0) = 0;
};
