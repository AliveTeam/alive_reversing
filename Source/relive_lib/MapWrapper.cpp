#include "MapWrapper.hpp"
#include "../AliveLibAE/Map.hpp"
#include "../AliveLibAO/Map.hpp"
#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAE/PathData.hpp"
#include "../relive_lib/FatalError.hpp"
#include "IBaseAliveGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

bool gMap_bDoPurpleLightEffect = false;


// Map Path_ChangeTLV::field_18_wipe to CameraSwapEffects
const CameraSwapEffects kPathChangeEffectToInternalScreenChangeEffect[10] = {
    CameraSwapEffects::ePlay1FMV_5,
    CameraSwapEffects::eRightToLeft_2,
    CameraSwapEffects::eLeftToRight_1,
    CameraSwapEffects::eBottomToTop_4,
    CameraSwapEffects::eTopToBottom_3,
    CameraSwapEffects::eBoxOut_8,
    CameraSwapEffects::eVerticalSplit_6,
    CameraSwapEffects::eHorizontalSplit_7,
    CameraSwapEffects::eUnknown_11,
    CameraSwapEffects::eInstantChange_0};

EReliveLevelIds MapWrapper::FromAO(AO::LevelIds levelId)
{
    switch (levelId)
    {
        case AO::LevelIds::eNone:
            return EReliveLevelIds::eNone;
        case AO::LevelIds::eMenu_0:
            return EReliveLevelIds::eMenu;
        case AO::LevelIds::eRuptureFarms_1:
            return EReliveLevelIds::eRuptureFarms;
        case AO::LevelIds::eLines_2:
            return EReliveLevelIds::eLines;
        case AO::LevelIds::eForest_3:
            return EReliveLevelIds::eForest;
        case AO::LevelIds::eForestTemple_4:
            return EReliveLevelIds::eForestTemple;
        case AO::LevelIds::eStockYards_5:
            return EReliveLevelIds::eStockYards;
        case AO::LevelIds::eStockYardsReturn_6:
            return EReliveLevelIds::eStockYardsReturn;
        case AO::LevelIds::eDesert_8:
            return EReliveLevelIds::eDesert;
        case AO::LevelIds::eDesertTemple_9:
            return EReliveLevelIds::eDesertTemple;
        case AO::LevelIds::eCredits_10:
            return EReliveLevelIds::eCredits;
        case AO::LevelIds::eBoardRoom_12:
            return EReliveLevelIds::eBoardRoom;
        case AO::LevelIds::eRuptureFarmsReturn_13:
            return EReliveLevelIds::eRuptureFarmsReturn;
        case AO::LevelIds::eForestChase_14:
            return EReliveLevelIds::eForestChase;
        case AO::LevelIds::eDesertEscape_15:
            return EReliveLevelIds::eDesertEscape;
        default:
            ALIVE_FATAL("Unknown AO level");
    }
}

AO::LevelIds MapWrapper::ToAO(EReliveLevelIds levelId)
{
    switch (levelId)
    {
        case EReliveLevelIds::eNone:
            return AO::LevelIds::eNone;
        case EReliveLevelIds::eMenu:
            return AO::LevelIds::eMenu_0;
        case EReliveLevelIds::eRuptureFarms:
            return AO::LevelIds::eRuptureFarms_1;
        case EReliveLevelIds::eLines:
            return AO::LevelIds::eLines_2;
        case EReliveLevelIds::eForest:
            return AO::LevelIds::eForest_3;
        case EReliveLevelIds::eForestTemple:
            return AO::LevelIds::eForestTemple_4;
        case EReliveLevelIds::eStockYards:
            return AO::LevelIds::eStockYards_5;
        case EReliveLevelIds::eStockYardsReturn:
            return AO::LevelIds::eStockYardsReturn_6;
        case EReliveLevelIds::eDesert:
            return AO::LevelIds::eDesert_8;
        case EReliveLevelIds::eDesertTemple:
            return AO::LevelIds::eDesertTemple_9;
        case EReliveLevelIds::eCredits:
            return AO::LevelIds::eCredits_10;
        case EReliveLevelIds::eBoardRoom:
            return AO::LevelIds::eBoardRoom_12;
        case EReliveLevelIds::eRuptureFarmsReturn:
            return AO::LevelIds::eRuptureFarmsReturn_13;
        case EReliveLevelIds::eForestChase:
            return AO::LevelIds::eForestChase_14;
        case EReliveLevelIds::eDesertEscape:
            return AO::LevelIds::eDesertEscape_15;
        default:
            ALIVE_FATAL("Can't map to an AO lvl");
    }
}

// Save data contains random values out side of the valid level range, convert invalid entries to 0
EReliveLevelIds MapWrapper::FromAESaveData(LevelIds levelId)
{
    if (levelId < LevelIds::eNone || levelId > LevelIds::eCredits_16)
    {
        LOG_WARNING("converting invalid AE level id %d to 0", static_cast<s32>(levelId));
        levelId = LevelIds::eMenu_0;
    }
    return MapWrapper::FromAE(levelId);
}

EReliveLevelIds MapWrapper::FromAE(LevelIds levelId)
{
    switch (levelId)
    {
    case LevelIds::eNone:
        return EReliveLevelIds::eNone;
    case LevelIds::eMenu_0:
        return EReliveLevelIds::eMenu;
    case LevelIds::eMines_1:
        return EReliveLevelIds::eMines;
    case LevelIds::eNecrum_2:
        return EReliveLevelIds::eNecrum;
    case LevelIds::eMudomoVault_3:
        return EReliveLevelIds::eMudomoVault;
    case LevelIds::eMudancheeVault_4:
        return EReliveLevelIds::eMudancheeVault;
    case LevelIds::eFeeCoDepot_5:
        return EReliveLevelIds::eFeeCoDepot;
    case LevelIds::eBarracks_6:
        return EReliveLevelIds::eBarracks;
    case LevelIds::eMudancheeVault_Ender_7:
        return EReliveLevelIds::eMudancheeVault_Ender;
    case LevelIds::eBonewerkz_8:
        return EReliveLevelIds::eBonewerkz;
    case LevelIds::eBrewery_9:
        return EReliveLevelIds::eBrewery;
    case LevelIds::eBrewery_Ender_10:
        return EReliveLevelIds::eBrewery_Ender;
    case LevelIds::eMudomoVault_Ender_11:
        return EReliveLevelIds::eMudomoVault_Ender;
    case LevelIds::eFeeCoDepot_Ender_12:
        return EReliveLevelIds::eFeeCoDepot_Ender;
    case LevelIds::eBarracks_Ender_13:
        return EReliveLevelIds::eBarracks_Ender;
    case LevelIds::eBonewerkz_Ender_14:
        return EReliveLevelIds::eBonewerkz_Ender;
    case LevelIds::eCredits_16:
        return EReliveLevelIds::eCredits;
    default:
        ALIVE_FATAL("Unknown AE level");
    }
}

LevelIds MapWrapper::ToAE(EReliveLevelIds levelId)
{
    switch (levelId)
    {
    case EReliveLevelIds::eNone:
        return LevelIds::eNone;
    case EReliveLevelIds::eMenu:
        return LevelIds::eMenu_0;
    case EReliveLevelIds::eMines:
        return LevelIds::eMines_1;
    case EReliveLevelIds::eNecrum:
        return LevelIds::eNecrum_2;
    case EReliveLevelIds::eMudomoVault:
        return LevelIds::eMudomoVault_3;
    case EReliveLevelIds::eMudancheeVault:
        return LevelIds::eMudancheeVault_4;
    case EReliveLevelIds::eFeeCoDepot:
        return LevelIds::eFeeCoDepot_5;
    case EReliveLevelIds::eBarracks:
        return LevelIds::eBarracks_6;
    case EReliveLevelIds::eMudancheeVault_Ender:
        return LevelIds::eMudancheeVault_Ender_7;
    case EReliveLevelIds::eBonewerkz:
        return LevelIds::eBonewerkz_8;
    case EReliveLevelIds::eBrewery:
        return LevelIds::eBrewery_9;
    case EReliveLevelIds::eBrewery_Ender:
        return LevelIds::eBrewery_Ender_10;
    case EReliveLevelIds::eMudomoVault_Ender:
        return LevelIds::eMudomoVault_Ender_11;
    case EReliveLevelIds::eFeeCoDepot_Ender:
        return LevelIds::eFeeCoDepot_Ender_12;
    case EReliveLevelIds::eBarracks_Ender:
        return LevelIds::eBarracks_Ender_13;
    case EReliveLevelIds::eBonewerkz_Ender:
        return LevelIds::eBonewerkz_Ender_14;
    case EReliveLevelIds::eCredits:
        return LevelIds::eCredits_16;
    default:
        //if (static_cast<s32>(levelId) != 0)
        {
            ALIVE_FATAL("Can't map to an AE lvl");
        }
    }
}

s16 IMap::SetActiveCameraDelayed(MapDirections direction, IBaseAliveGameObject* pObj, s16 swapEffect)
{
    relive::Path_PathTransition* pPathChangeTLV = nullptr;
    CameraSwapEffects convertedSwapEffect = CameraSwapEffects::eInstantChange_0;
    if (pObj)
    {
        pPathChangeTLV = static_cast<relive::Path_PathTransition*>(VTLV_Get_At(
            FP_GetExponent(pObj->mXPos),
            FP_GetExponent(pObj->mYPos),
            FP_GetExponent(pObj->mXPos),
            FP_GetExponent(pObj->mYPos),
            ReliveTypes::ePathTransition));
    }

    if (pObj && pPathChangeTLV)
    {
        mNextLevel = pPathChangeTLV->mNextLevel;
        mNextPath = pPathChangeTLV->mNextPath;
        mNextCamera = pPathChangeTLV->mNextCamera;
        if (swapEffect < 0)
        {
            // Map the TLV/editor value of screen change to the internal screen change
            convertedSwapEffect = kPathChangeEffectToInternalScreenChangeEffect[pPathChangeTLV->mWipeEffect];
        }
        else
        {
            // If not negative then its an actual swap effect
            convertedSwapEffect = static_cast<CameraSwapEffects>(swapEffect);
        }
    }
    else
    {
        switch (direction)
        {
            case MapDirections::eMapLeft_0:
                if (!GetCamera(CameraPos::eCamLeft_3))
                {
                    return 0;
                }
                break;
            case MapDirections::eMapRight_1:
                if (!GetCamera(CameraPos::eCamRight_4))
                {
                    return 0;
                }
                break;
            case MapDirections::eMapBottom_3:
                if (!GetCamera(CameraPos::eCamBottom_2))
                {
                    return 0;
                }
                break;
            case MapDirections::eMapTop_2:
                if (!GetCamera(CameraPos::eCamTop_1))
                {
                    return 0;
                }
                break;
        }

        mNextPath = mCurrentPath;
        mNextLevel = mCurrentLevel;
        convertedSwapEffect = static_cast<CameraSwapEffects>(swapEffect); // TODO: Correct ??
    }

    mMapDirection = direction;
    mAliveObj = pObj;
    mCamState = CamChangeStates::eSliceCam_1;
    gMap_bDoPurpleLightEffect = false;

    if (convertedSwapEffect == CameraSwapEffects::ePlay1FMV_5 || convertedSwapEffect == CameraSwapEffects::eUnknown_11)
    {
        gMap_bDoPurpleLightEffect = true;
    }

    return 1;
}

Camera* IMap::GetCamera(CameraPos pos)
{
    return field_2C_camera_array[static_cast<s32>(pos)];
}
