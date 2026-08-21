#include "stdafx.h"
#include "BaseMap.hpp"
#include "../AliveLibAE/Game.hpp" // DestroyObjects
#include "../AliveLibAE/Map.hpp"
#include "../AliveLibAO/Map.hpp"
#include "GameObjects/BaseAliveGameObject.hpp"
#include "data_conversion/relive_tlvs.hpp"
#include "data_conversion/string_util.hpp"

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

s16 BaseMap::SetActiveCameraDelayed(MapDirections direction, BaseAliveGameObject* pObj, s16 swapEffect)
{
    relive::Path_PathTransition* pPathChangeTLV = nullptr;
    CameraSwapEffects convertedSwapEffect = CameraSwapEffects::eInstantChange_0;
    if (pObj)
    {
        pPathChangeTLV = static_cast<relive::Path_PathTransition*>(VTLV_Get_At_Of_Type(
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

Camera* BaseMap::GetCamera(CameraPos pos)
{
    return field_2C_camera_array[static_cast<s32>(pos)];
}

relive::Path_TLV* BaseMap::TLV_From_Offset_Lvl_Cam(const Guid& /*tlvId*/)
{
    LOG_WARNING("TLV_From_Offset_Lvl_Cam() not implmeneted in BaseMap");
    return nullptr;
}

void BaseMap::ReloadPathJsonRequest(const std::string& pathJsonFileName)
{
    for (auto& binaryPath : mLoadedPaths)
    {
        if (string_util::endsWith(pathJsonFileName, binaryPath->JsonFileName()))
        {
            LOG_INFO("Reloading paths...");
            mForceLoad = true;
            EReliveLevelIds oldCurrentLevel = mCurrentLevel;
            mNextLevel = oldCurrentLevel;
            mCurrentLevel = EReliveLevelIds::eNone;
            DestroyObjects();
            GoTo_Camera();
            return;
        }
    }
}
