#include "stdafx.h"
#include "Map.hpp"
#include "PathData.hpp"
#include "Function.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "Sound/Midi.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "BackgroundMusic.hpp"
#include "stdlib.hpp"
#include "QuikSave.hpp"
#include "Sfx.hpp"
#include "CameraSwapper.hpp"
#include "MainMenu.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Particle.hpp"
#include "Sound/PsxSpuApi.hpp"
#include "Sys.hpp"
#include <assert.h>
#include "../relive_lib/Camera.hpp"
#include "Path.hpp"
#include "FixedPoint.hpp"
#include "../relive_lib/PsxDisplay.hpp"

#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../AliveLibCommon/FatalError.hpp"
#include "../relive_lib/BinaryPath.hpp"

u32 sSoundChannelsMask = 0;


s32 MaxGridBlocks(FP scale)
{
    if (scale == FP_FromDouble(0.5))
    {
        return 30; // (29+1) * 13 (grid block size) for 377/390
    }
    else if (scale == FP_FromInteger(1))
    {
        return 16; // (15+1) * 25 (grid block size) for 375/400
    }
    else
    {
        ALIVE_FATAL("Scale should be 0.5 or 1 but got %f", FP_GetDouble(scale));
    }
}

void Map::ScreenChange_Common()
{
    if (mCamState == CamChangeStates::eSliceCam_1)
    {
        Handle_PathTransition();
    }
    else if (mCamState == CamChangeStates::eInstantChange_2)
    {
        GoTo_Camera();
    }

    mCamState = CamChangeStates::eInactive_0;

    SND_Stop_Channels_Mask(sSoundChannelsMask);
    sSoundChannelsMask = 0;
}

Map::Map()
{
    Reset();
}

void Map::Reset()
{
    for (s32 i = 0; i < ALIVE_COUNTOF(field_2C_camera_array); i++)
    {
        field_2C_camera_array[i] = nullptr;
    }

    ClearPathResourceBlocks();

    mFreeAllAnimAndPalts = 0;
    mRestoreQuickSaveData = 0;
}

void Map::Init(EReliveLevelIds level, s16 path, s16 camera, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange)
{
    sPathInfo = relive_new Path();

    field_2C_camera_array[0] = nullptr;
    field_2C_camera_array[1] = nullptr;
    field_2C_camera_array[2] = nullptr;
    field_2C_camera_array[3] = nullptr;
    field_2C_camera_array[4] = nullptr;

    mOverlayId = -1;

    mCurrentCamera = static_cast<s16>(-1);
    mCurrentPath = static_cast<s16>(-1);
    mCurrentLevel = EReliveLevelIds::eNone;

    mForceLoad = 0;

    SetActiveCam(level, path, camera, screenChangeEffect, fmvBaseId, forceChange);
    GoTo_Camera();

    mCamState = CamChangeStates::eInactive_0;
}

void Map::ScreenChange()
{
    if (mCamState == CamChangeStates::eInactive_0)
    {
        return;
    }

    if (sMap_bDoPurpleLightEffect)
    {
        RemoveObjectsWithPurpleLight(1);
    }

    for (s32 i = 0; i < 2; i++) // Not sure why this is done twice?
    {
        for (s32 j = 0; j < gBaseGameObjects->Size(); j++)
        {
            BaseGameObject* pItem = gBaseGameObjects->ItemAt(j);
            if (!pItem)
            {
                break;
            }

            pItem->VScreenChanged();

            // Did the screen change kill the object?
            if (pItem->GetDead())
            {
                j = gBaseGameObjects->RemoveAt(j);
                delete pItem;
            }
        }
    }

    // TODO: Refactor this logic
    if (!sMap_bDoPurpleLightEffect && mNextLevel == mCurrentLevel)
    {
        ScreenChange_Common();
        return;
    }

    if (mNextLevel != mCurrentLevel)
    {
        SsUtAllKeyOff_4FDFE0(0);
    }

    if (mNextLevel != EReliveLevelIds::eNone)
    {
        if (mNextLevel == EReliveLevelIds::eCredits)
        {
            sSoundChannelsMask = 0;
            ScreenChange_Common();
            return;
        }
    }
    else if (mCurrentLevel == EReliveLevelIds::eMenu)
    {
        sSoundChannelsMask = 0;
        ScreenChange_Common();
        return;
    }
    sSoundChannelsMask = SND_MIDI(0, 0, 36, 70, 0, 0);
    ScreenChange_Common();
}


void Map::Shutdown()
{
    // Free Path resources
    FreePathResourceBlocks();

    // Free cameras
    for (s32 i = 0; i < ALIVE_COUNTOF(field_2C_camera_array); i++)
    {
        if (field_2C_camera_array[i])
        {
            relive_delete field_2C_camera_array[i];
            field_2C_camera_array[i] = nullptr;
        }
    }

    pScreenManager = nullptr;

    // Free 
    relive_delete sPathInfo;
    sPathInfo = nullptr;

    Reset();
}


s16 Map::SetActiveCam(EReliveLevelIds level, s16 path, s16 cam, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange)
{
    if (!forceChange && cam == mCurrentCamera && level == mCurrentLevel && path == mCurrentPath)
    {
        return 0;
    }

    mNextCamera = cam;
    mFmvBaseId = fmvBaseId;
    mNextPath = path;
    mNextLevel = level;
    mCameraSwapEffect = screenChangeEffect;
    mCamState = CamChangeStates::eInstantChange_2;

    if (screenChangeEffect == CameraSwapEffects::ePlay1FMV_5 || screenChangeEffect == CameraSwapEffects::eUnknown_11)
    {
        sMap_bDoPurpleLightEffect = 1;
    }
    else
    {
        sMap_bDoPurpleLightEffect = 0;
    }

    return 1;
}

void Map::FreePathResourceBlocks()
{
    mLoadedPaths.clear();
}

BinaryPath* Map::GetPathResourceBlockPtr(u32 pathId)
{
    for (auto& loadedPath : mLoadedPaths)
    {
        if (loadedPath->GetPathId() == pathId)
        {
            return loadedPath.get();
        }
    }
    return nullptr;
}

void Map::ClearPathResourceBlocks()
{
    mLoadedPaths.clear();
}

void Map::RemoveObjectsWithPurpleLight(s16 bMakeInvisible)
{
    auto pObjectsWithLightsArray = relive_new DynamicArrayT<BaseAnimatedWithPhysicsGameObject>(16);

    auto pPurpleLightArray = relive_new DynamicArrayT<Particle>(16);

    bool bAddedALight = false;
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetIsBaseAnimatedWithPhysicsObj())
        {
            if (pObj->GetDrawable())
            {
                auto pBaseObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

                const PSX_RECT objRect = pBaseObj->VGetBoundingRect();

                if (pBaseObj->GetDoPurpleLightEffect())
                {
                    if (pBaseObj->GetAnimation().GetRender())
                    {
                        if (!pBaseObj->GetDead() && pBaseObj != sControlledCharacter && gMap.Rect_Location_Relative_To_Active_Camera(&objRect) == CameraPos::eCamCurrent_0)
                        {
                            pObjectsWithLightsArray->Push_Back(pBaseObj);
                            const FP k60Scaled = pBaseObj->GetSpriteScale() * FP_FromInteger(60);
                            Particle* pPurpleLight = New_DestroyOrCreateObject_Particle(
                                FP_FromInteger((objRect.x + objRect.w) / 2),
                                FP_FromInteger((objRect.y + objRect.h) / 2) + k60Scaled,
                                pBaseObj->GetSpriteScale());

                            if (pPurpleLight)
                            {
                                pPurpleLightArray->Push_Back(pPurpleLight);
                                bAddedALight = true;
                            }
                        }
                    }
                }
            }
        }
    }

    if (bAddedALight)
    {
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);

        for (s32 counter = 0; counter < 12; counter++)
        {
            if (bMakeInvisible && counter == 4)
            {
                // Make all the objects that have lights invisible now that the lights have been rendered for a few frames
                for (s32 i = 0; i < pObjectsWithLightsArray->Size(); i++)
                {
                    BaseAnimatedWithPhysicsGameObject* pObj = pObjectsWithLightsArray->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }
                    pObj->GetAnimation().SetRender(false);
                }
            }

            for (s32 i = 0; i < pPurpleLightArray->Size(); i++)
            {
                Particle* pLight = pPurpleLightArray->ItemAt(i);
                if (!pLight)
                {
                    break;
                }

                if (!pLight->GetDead())
                {
                    pLight->VUpdate();
                }
            }

            // TODO/HACK what is the point of the f64 loop? Why not do both in 1 iteration ??
            for (s32 i = 0; i < pPurpleLightArray->Size(); i++)
            {
                Particle* pLight = pPurpleLightArray->ItemAt(i);
                if (!pLight)
                {
                    break;
                }

                if (!pLight->GetDead())
                {
                    pLight->GetAnimation().VDecode();
                }
            }

            for (s32 i = 0; i < gObjListDrawables->Size(); i++)
            {
                BaseGameObject* pDrawable = gObjListDrawables->ItemAt(i);
                if (!pDrawable)
                {
                    break;
                }

                if (!pDrawable->GetDead())
                {
                    // TODO: Seems strange to check this flag, how did it get in the drawable list if its not a drawable ??
                    if (pDrawable->GetDrawable())
                    {
                        pDrawable->VRender(gPsxDisplay.mDrawEnvs[gPsxDisplay.mBufferIndex].mOrderingTable);
                    }
                }
            }

            pScreenManager->VRender(gPsxDisplay.mDrawEnvs[gPsxDisplay.mBufferIndex].mOrderingTable);
            SYS_EventsPump();
            gPsxDisplay.RenderOrderingTable();
        }

        if (bMakeInvisible)
        {
            // Make all the objects that had lights visible again
            for (s32 i = 0; i < pObjectsWithLightsArray->Size(); i++)
            {
                BaseAnimatedWithPhysicsGameObject* pObj = pObjectsWithLightsArray->ItemAt(i);
                if (!pObj)
                {
                    break;
                }
                pObj->GetAnimation().SetRender(true);
            }
        }
    }

    pObjectsWithLightsArray->field_4_used_size = 0;
    pPurpleLightArray->field_4_used_size = 0;

    relive_delete pObjectsWithLightsArray;
    relive_delete pPurpleLightArray;
}

void Map::Handle_PathTransition()
{
    relive::Path_PathTransition* pTlv = nullptr;
    if (mAliveObj)
    {
        pTlv = static_cast<relive::Path_PathTransition*>(sPathInfo->TLV_Get_At(
            FP_GetExponent(mAliveObj->mXPos),
            FP_GetExponent(mAliveObj->mYPos),
            FP_GetExponent(mAliveObj->mXPos),
            FP_GetExponent(mAliveObj->mYPos),
            ReliveTypes::ePathTransition));
    }

    if (mAliveObj && pTlv)
    {
        mNextLevel = pTlv->mNextLevel;
        mNextPath = pTlv->mNextPath;
        mNextCamera = pTlv->mNextCamera;
        mFmvBaseId = pTlv->mMovieId;

        mCameraSwapEffect = kPathChangeEffectToInternalScreenChangeEffect[pTlv->mWipeEffect];

        mAliveObj->mCurrentLevel = pTlv->mNextLevel;
        mAliveObj->mCurrentPath = pTlv->mNextPath;

        // Save value before pPathChangeTLV is deleted
        const auto next_path_scale = pTlv->mNextPathScale;

        GoTo_Camera();

        switch (next_path_scale)
        {
            case relive::reliveScale::eFull:
                sActiveHero->SetSpriteScale(FP_FromInteger(1));
                sActiveHero->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
                break;

            case relive::reliveScale::eHalf:
                sActiveHero->SetSpriteScale(FP_FromDouble(0.5));
                sActiveHero->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
                break;

            default:
                LOG_ERROR("Invalid scale %d", static_cast<s16>(next_path_scale));
                break;
        }

        CameraPos remapped = CameraPos::eCamInvalid_m1;
        switch (mMapDirection)
        {
            case MapDirections::eMapLeft_0:
                remapped = CameraPos::eCamLeft_3;
                break;
            case MapDirections::eMapRight_1:
                remapped = CameraPos::eCamRight_4;
                break;
            case MapDirections::eMapTop_2:
                remapped = CameraPos::eCamTop_1;
                break;
            case MapDirections::eMapBottom_3:
                remapped = CameraPos::eCamBottom_2;
                break;
        }

        mAliveObj->VOnPathTransition(
            mPathData->field_A_grid_width * mCamIdxOnX,
            mPathData->field_C_grid_height * mCamIdxOnY,
            remapped);
    }
    else
    {
        switch (mMapDirection)
        {
            case MapDirections::eMapLeft_0:
                mCamIdxOnX--;
                mCameraSwapEffect = CameraSwapEffects::eRightToLeft_2;
                break;
            case MapDirections::eMapRight_1:
                mCamIdxOnX++;
                mCameraSwapEffect = CameraSwapEffects::eLeftToRight_1;
                break;
            case MapDirections::eMapTop_2:
                mCamIdxOnY--;
                mCameraSwapEffect = CameraSwapEffects::eBottomToTop_4;
                break;
            case MapDirections::eMapBottom_3:
                mCamIdxOnY++;
                mCameraSwapEffect = CameraSwapEffects::eTopToBottom_3;
                break;
            default:
                break;
        }

        const BinaryPath* pPathRes = GetPathResourceBlockPtr(mCurrentPath);
        auto pCameraName = pPathRes->CameraName(mCamIdxOnX, mCamIdxOnY);

        // Convert the 2 digit camera number string to an integer
        mNextCamera = 1 * (pCameraName[7] - '0') + 10 * (pCameraName[6] - '0');

        GoTo_Camera();
    }
}

CameraPos Map::GetDirection(EReliveLevelIds level, s32 path, FP xpos, FP ypos)
{
    if (level != mCurrentLevel)
    {
        return CameraPos::eCamInvalid_m1;
    }

    if (path != mCurrentPath)
    {
        return CameraPos::eCamInvalid_m1;
    }

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(xpos);
    rect.w = FP_GetExponent(xpos);
    rect.y = FP_GetExponent(ypos);
    rect.h = FP_GetExponent(ypos);

    CameraPos ret = Rect_Location_Relative_To_Active_Camera(&rect);

    PSX_RECT camWorldRect = {};
    if (!Get_Camera_World_Rect(ret, &camWorldRect))
    {
        return CameraPos::eCamInvalid_m1;
    }

    const FP x = FP_FromInteger(camWorldRect.x);
    const FP y = FP_FromInteger(camWorldRect.y);
    const FP w = FP_FromInteger(camWorldRect.w);
    const FP h = FP_FromInteger(camWorldRect.h);

    switch (ret)
    {
        case CameraPos::eCamCurrent_0:
            return ret;

        case CameraPos::eCamTop_1:
            if (ypos < y || xpos < x || xpos > w)
            {
                return CameraPos::eCamInvalid_m1;
            }
            return ypos > h ? CameraPos::eCamCurrent_0 : ret;

        case CameraPos::eCamBottom_2:
            if (ypos > h || xpos < x || xpos > w)
            {
                return CameraPos::eCamInvalid_m1;
            }
            return ypos < y ? CameraPos::eCamCurrent_0 : ret;

        case CameraPos::eCamLeft_3:
            if (xpos < x || ypos < y || ypos > h)
            {
                return CameraPos::eCamInvalid_m1;
            }
            return xpos > w ? CameraPos::eCamCurrent_0 : ret;

        case CameraPos::eCamRight_4:
            if (xpos > w || ypos < y || ypos > h)
            {
                return CameraPos::eCamInvalid_m1;
            }
            return xpos < x ? CameraPos::eCamCurrent_0 : ret;

        default:
            return CameraPos::eCamInvalid_m1;
    }
}




Map::~Map()
{
    Shutdown();
}


void Map::GoTo_Camera()
{
    s16 bShowLoadingIcon = false;
    if (mCurrentLevel != EReliveLevelIds::eMenu && mCurrentLevel != EReliveLevelIds::eCredits && mCurrentLevel != EReliveLevelIds::eNone)
    {
        bShowLoadingIcon = true;
    }

    if (mCameraSwapEffect == CameraSwapEffects::eUnknown_11)
    {
        CamResource nullRes;
        BaseGameObject* pFmvRet = FMV_Camera_Change(nullRes, this, mCurrentLevel);
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            SYS_EventsPump();

            BaseGameObject* pBaseGameObj = gBaseGameObjects->ItemAt(i);
            if (!pBaseGameObj)
            {
                break;
            }

            if (pBaseGameObj->GetDead() && !pBaseGameObj->GetCantKill())
            {
                i = gBaseGameObjects->RemoveAt(i);
                relive_delete pBaseGameObj;
                if (pBaseGameObj == pFmvRet)
                {
                    // FMV trans done
                    break;
                }
            }
            else if (pBaseGameObj->GetUpdatable())
            {
                if (!pBaseGameObj->GetDead() && (!gNumCamSwappers || pBaseGameObj->GetUpdateDuringCamSwap()))
                {
                    const s32 updateDelay = pBaseGameObj->UpdateDelay();
                    if (updateDelay > 0)
                    {
                        pBaseGameObj->SetUpdateDelay(updateDelay - 1);
                    }
                    else
                    {
                        pBaseGameObj->VUpdate();
                    }
                }
            }
        }

        if (sSoundChannelsMask)
        {
            SND_Stop_Channels_Mask(sSoundChannelsMask);
        }
        sSoundChannelsMask = SND_MIDI(0, 0, 36, 70, 0, 0);
    }

    if (mCurrentLevel != EReliveLevelIds::eMenu && mCurrentLevel != EReliveLevelIds::eNone)
    {
        if (mNextLevel != mCurrentLevel
            || mForceLoad
            || (mNextPath != mCurrentPath && mCameraSwapEffect == CameraSwapEffects::ePlay1FMV_5))
        {
            Game_ShowLoadingIcon_482D80();
        }
    }

    if (mNextLevel != mCurrentLevel || mNextPath != mCurrentPath || mForceLoad)
    {
        mOverlayId = GetOverlayId();
    }


    if (mNextLevel != mCurrentLevel || mForceLoad)
    {
        pResourceManager->LoadingLoop(bShowLoadingIcon);

        // Free all cameras
        for (s32 i = 0; i < ALIVE_COUNTOF(field_2C_camera_array); i++)
        {
            if (field_2C_camera_array[i])
            {
                relive_delete field_2C_camera_array[i];
                field_2C_camera_array[i] = nullptr;
            }
        }

        if (mCurrentLevel != EReliveLevelIds::eNone)
        {
            if (mNextLevel != mCurrentLevel)
            {
                SND_Reset();
                FreePathResourceBlocks();
                sPathInfo->Free();
            }

        }

        pResourceManager->LoadingLoop(bShowLoadingIcon);

        if (mNextLevel == mCurrentLevel)
        {
            MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, sActiveHero, 0, 0);
        }
        else
        {
            // Don't let the force flag make us reload paths for no reason
            mLoadedPaths = ResourceManagerWrapper::LoadPaths(mNextLevel);

            // TODO: This data is now per path rather than lvl - logic needs updating to reflect this
            SND_Load_VABS(mLoadedPaths[0]->GetSoundInfo(), Path_Get_Reverb(mNextLevel)); // TODO: Remove hard coded data
            SND_Load_Seqs(sSeqData_558D50.mSeqs, mLoadedPaths[0]->GetSoundInfo());

            // TODO: Remove hard coded data
            relive_new BackgroundMusic(Path_Get_BackGroundMusicId(mNextLevel));
        }

        if (!mForceLoad)
        {
            SwitchStates_ClearRange(2, 255);
        }

        if (mFreeAllAnimAndPalts)
        {
            mFreeAllAnimAndPalts = false;
        }
    }

    if (!mNextPath)
    {
        mNextPath = 1;
    }

    const s16 prevPathId = mCurrentPath;
    const EReliveLevelIds prevLevelId = mCurrentLevel;

    mCurrentPath = mNextPath;
    mCurrentLevel = mNextLevel;
    mCurrentCamera = mNextCamera;

    const PathBlyRec* pPathRec_1 = Path_Get_Bly_Record(mNextLevel, mNextPath);
    mPathData = pPathRec_1->field_4_pPathData;

    sPathInfo->Init(
        mPathData,
        mNextLevel,
        mNextPath,
        mNextCamera,
        GetPathResourceBlockPtr(mNextPath));

    if (sQuickSave_saved_switchResetters_count_BB234C > 0)
    {
        Quicksave_RestoreSwitchResetterStates();
    }

    char_type camNameBuffer[20] = {};
    Path_Format_CameraName(camNameBuffer, mNextLevel, mNextPath, mNextCamera);

    BinaryPath* pNextPath = GetPathResourceBlockPtr(mNextPath);
    for (auto& cam : pNextPath->GetCameras())
    {
        if (!strncmp(cam->mName.c_str(), camNameBuffer, sizeof(CameraName)))
        {
            mCamIdxOnX = static_cast<s16>(cam->mX);
            mCamIdxOnY = static_cast<s16>(cam->mY);
            break;
        }
    }

    field_24_camera_offset.x = FP_FromInteger(mCamIdxOnX * mPathData->field_A_grid_width);
    field_24_camera_offset.y = FP_FromInteger(mCamIdxOnY * mPathData->field_C_grid_height);

    // If map has changed then load new collision info
    if (prevPathId != mCurrentPath || prevLevelId != mCurrentLevel)
    {
        relive_delete sCollisions;
        sCollisions = relive_new Collisions(GetPathResourceBlockPtr(mCurrentPath)->GetCollisions());
    }

    if (mRestoreQuickSaveData)
    {
        QuikSave_RestoreBlyData(mRestoreQuickSaveData);
        mRestoreQuickSaveData = nullptr;
    }

    // Copy camera array and blank out the source
    for (s32 i = 0; i < ALIVE_COUNTOF(field_40_stru_5); i++)
    {
        field_40_stru_5[i] = field_2C_camera_array[i];
        field_2C_camera_array[i] = nullptr;
    }

    field_2C_camera_array[0] = Create_Camera(mCamIdxOnX, mCamIdxOnY, 1);
    field_2C_camera_array[3] = Create_Camera(mCamIdxOnX - 1, mCamIdxOnY, 0);
    field_2C_camera_array[4] = Create_Camera(mCamIdxOnX + 1, mCamIdxOnY, 0);
    field_2C_camera_array[1] = Create_Camera(mCamIdxOnX, mCamIdxOnY - 1, 0);
    field_2C_camera_array[2] = Create_Camera(mCamIdxOnX, mCamIdxOnY + 1, 0);

    // Free resources for each camera
    for (s32 i = 0; i < ALIVE_COUNTOF(field_40_stru_5); i++)
    {
        if (field_40_stru_5[i])
        {
            //pResourceManager->Free_Resources_For_Camera_4656F0(field_40_stru_5[i]);
        }
    }

    pResourceManager->LoadingLoop(bShowLoadingIcon);

    // Free each camera itself
    for (s32 i = 0; i < ALIVE_COUNTOF(field_40_stru_5); i++)
    {
        if (field_40_stru_5[i])
        {
            relive_delete field_40_stru_5[i];
            field_40_stru_5[i] = nullptr;
        }
    }

    Map::Load_Path_Items(field_2C_camera_array[0], LoadMode::ConstructObject_0);
    pResourceManager->LoadingLoop(bShowLoadingIcon);
    Map::Load_Path_Items(field_2C_camera_array[3], LoadMode::ConstructObject_0);
    Map::Load_Path_Items(field_2C_camera_array[4], LoadMode::ConstructObject_0);
    Map::Load_Path_Items(field_2C_camera_array[1], LoadMode::ConstructObject_0);
    Map::Load_Path_Items(field_2C_camera_array[2], LoadMode::ConstructObject_0);

    // Create the screen manager if it hasn't already been done (probably should have always been done by this point though?)
    if (!pScreenManager)
    {
        pScreenManager = relive_new ScreenManager(field_2C_camera_array[0]->field_C_pCamRes, &field_24_camera_offset);
    }

    sPathInfo->Loader_4DB800(mCamIdxOnX, mCamIdxOnY, LoadMode::ConstructObject_0, ReliveTypes::eNone); // none = load all

    if (prevPathId != mCurrentPath || prevLevelId != mCurrentLevel)
    {
        if (sActiveHero)
        {
            if (mCurrentPath == sActiveHero->mCurrentPath)
            {
                sActiveHero->VCheckCollisionLineStillValid(10);
            }
        }
    }

    Create_FG1s();

    if (mCameraSwapEffect == CameraSwapEffects::ePlay1FMV_5)
    {
        Map::FMV_Camera_Change(field_2C_camera_array[0]->field_C_pCamRes, this, mNextLevel);
    }

    if (mCameraSwapEffect == CameraSwapEffects::eUnknown_11)
    {
        pScreenManager->DecompressCameraToVRam(field_2C_camera_array[0]->field_C_pCamRes);
        pScreenManager->EnableRendering();
    }

    if (prevLevelId != mCurrentLevel)
    {
        pResourceManager->LoadingLoop(false);
    }

    if (mCameraSwapEffect != CameraSwapEffects::ePlay1FMV_5 && mCameraSwapEffect != CameraSwapEffects::eUnknown_11)
    {
        if (mDoorTransition)
        {
            // TODO: Add template helpers

            // Door transition
            relive::Path_Door* pDoorTlv = static_cast<relive::Path_Door*>(sPathInfo->TLV_First_Of_Type_In_Camera(ReliveTypes::eDoor, 0));
            while (pDoorTlv->mDoorId != sActiveHero->field_1A0_door_id)
            {
                pDoorTlv = static_cast<relive::Path_Door*>(Path::TLV_Next_Of_Type(pDoorTlv, ReliveTypes::eDoor));
            }

            CreateScreenTransistionForTLV(pDoorTlv);
        }
        else
        {
            if (!mTeleporterTransition)
            {
                relive_new CameraSwapper(field_2C_camera_array[0]->field_C_pCamRes, mCameraSwapEffect, 368 / 2, 240 / 2);
            }
            else
            {
                // TODO: Add template helpers

                // Teleporter transition
                relive::Path_Teleporter* pTeleporterTlv = static_cast<relive::Path_Teleporter*>(sPathInfo->TLV_First_Of_Type_In_Camera(ReliveTypes::eTeleporter, 0));
                while (pTeleporterTlv->mTeleporterId != sActiveHero->field_1A0_door_id)
                {
                    pTeleporterTlv = static_cast<relive::Path_Teleporter*>(Path::TLV_Next_Of_Type(pTeleporterTlv, ReliveTypes::eTeleporter));
                }

                CreateScreenTransistionForTLV(pTeleporterTlv);
            }
        }
    }

    bHideLoadingIcon_5C1BAA = 0;
    loading_ticks_5C1BAC = 0;

    mForceLoad = 0;

    if (sSoundChannelsMask)
    {
        SND_Stop_Channels_Mask(sSoundChannelsMask);
        sSoundChannelsMask = 0;
    }
}

void Map::Create_FG1s()
{
    Camera* pCamera = field_2C_camera_array[0];
    pCamera->CreateFG1();
    /*
    for (s32 i = 0; i < pCamera->field_0.Size(); i++)
    {
        u8** ppRes = pCamera->field_0.ItemAt(i);
        if (!ppRes)
        {
            break;
        }

        if (*ppRes)
        {
            ResourceManager::Header* pHeader = ResourceManager::Get_Header_49C410(ppRes);
            if (pHeader->mResourceType == ResourceManager::Resource_FG1)
            {
                relive_new FG1(ppRes);
            }
        }
    }*/
}

void Map::TLV_Reset(const Guid& tlvId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed)
{
    Path::TLV_Reset(tlvId, hiFlags, bSetCreated, bSetDestroyed);
}

void Map::CreateScreenTransistionForTLV(relive::Path_TLV* pTlv)
{
    // TODO: Refactor
    const s16 doorYDiff = static_cast<s16>(pTlv->mTopLeftY - FP_GetExponent(pScreenManager->CamYPos()));
    const s16 midX = (pTlv->mTopLeftX + pTlv->mBottomRightX) / 2;
    const s16 rightPos = static_cast<s16>(midX - FP_GetExponent(pScreenManager->CamXPos()));
    relive_new CameraSwapper(field_2C_camera_array[0]->field_C_pCamRes, mCameraSwapEffect, rightPos, doorYDiff);
}

void Map::Get_map_size(PSX_Point* pPoint)
{
    pPoint->x = mPathData->field_4_bTop;
    pPoint->y = mPathData->field_6_bBottom;
}

void Map::GetCurrentCamCoords(PSX_Point* pPoint)
{
    pPoint->x = mCamIdxOnX * mPathData->field_A_grid_width;
    pPoint->y = mCamIdxOnY * mPathData->field_C_grid_height;
}

void Map::Get_Abe_Spawn_Pos(PSX_Point* pPoint)
{
    pPoint->x = mPathData->field_1A_abe_start_xpos;
    pPoint->y = mPathData->field_1C_abe_start_ypos;
}

s16 Map::GetOverlayId()
{
    // TODO: Probably need to redo field_C data as 1 bytes instead of a word
    return Path_Get_Bly_Record(mNextLevel, mNextPath)->field_C_overlay_id & 0xFF;
}


s16 Map::Get_Camera_World_Rect(CameraPos camIdx, PSX_RECT* pRect)
{
    if (camIdx < CameraPos::eCamCurrent_0 || camIdx > CameraPos::eCamRight_4)
    {
        return 0;
    }

    Camera* pCamera = field_2C_camera_array[static_cast<s32>(camIdx)];
    if (!pCamera)
    {
        return 0;
    }

    if (!pRect)
    {
        return 1;
    }

    const s16 xpos = pCamera->mCamXOff * mPathData->field_A_grid_width;
    const s16 ypos = pCamera->mCamYOff * mPathData->field_C_grid_height;

    pRect->x = xpos;
    pRect->y = ypos;
    pRect->w = xpos + 368;
    pRect->h = ypos + 240;
    return 1;
}

s16 Map::Is_Point_In_Current_Camera(EReliveLevelIds level, s32 path, FP xpos, FP ypos, s16 width)
{
    const FP calculated_width = (width != 0) ? FP_FromInteger(6) : FP_FromInteger(0);
    if (level != mCurrentLevel || path != mCurrentPath) // TODO: Remove when 100%
    {
        return false;
    }

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(xpos - calculated_width);
    rect.w = FP_GetExponent(calculated_width + xpos);
    rect.y = FP_GetExponent(ypos);
    rect.h = FP_GetExponent(ypos);
    return Rect_Location_Relative_To_Active_Camera(&rect) == CameraPos::eCamCurrent_0;
}


CameraPos Map::Rect_Location_Relative_To_Active_Camera(const PSX_RECT* pRect, s16 /*width*/)
{
    if (EventGet(kEventDeathReset))
    {
        return CameraPos::eCamNone_5;
    }

    const s32 camX = FP_GetExponent(field_24_camera_offset.x);
    const s32 camY = FP_GetExponent(field_24_camera_offset.y);

    if (pRect->x > (camX + 368))
    {
        return CameraPos::eCamRight_4;
    }

    if (pRect->y > (camY + 240))
    {
        return CameraPos::eCamBottom_2;
    }

    if (pRect->w >= camX)
    {
        if (pRect->h < camY)
        {
            return CameraPos::eCamTop_1;
        }
        else
        {
            return CameraPos::eCamCurrent_0;
        }
    }

    return CameraPos::eCamLeft_3;
}


BaseGameObject* Map::FMV_Camera_Change(CamResource& ppBits, Map* pMap, EReliveLevelIds lvlId)
{
    if (pMap->mFmvBaseId > 10000u)
    {
        // Trippe FMV
        FmvInfo* pFmvRec1 = Path_Get_FMV_Record(lvlId, pMap->mFmvBaseId / 10000);
        FmvInfo* pFmvRec2 = Path_Get_FMV_Record(lvlId, pMap->mFmvBaseId % 100);
        FmvInfo* pFmvRec3 = Path_Get_FMV_Record(lvlId, pMap->mFmvBaseId / 100 % 100);
        sLevelId_dword_5CA408 = static_cast<s32>(MapWrapper::ToAE(lvlId)); // HACK

        return relive_new CameraSwapper(ppBits,
                                        pFmvRec1->field_8_flags == 1,
                                        pFmvRec1->field_0_pName,
                                        pFmvRec2->field_8_flags == 1, // TODO: Master branch gobbed
                                        pFmvRec2->field_0_pName,
                                        pFmvRec3->field_8_flags == 1,
                                        pFmvRec3->field_0_pName);
    }
    else if (pMap->mFmvBaseId >= 100u)
    {
        // Double FMV
        FmvInfo* pFmvRec1 = Path_Get_FMV_Record(lvlId, pMap->mFmvBaseId / 100);
        FmvInfo* pFmvRec2 = Path_Get_FMV_Record(lvlId, pMap->mFmvBaseId % 100);
        sLevelId_dword_5CA408 = static_cast<s32>(MapWrapper::ToAE(lvlId)); // HACK
        return relive_new CameraSwapper(ppBits,
                                              pFmvRec1->field_8_flags == 1,
                                              pFmvRec1->field_0_pName,
                                              pFmvRec2->field_8_flags == 1,
                                              pFmvRec2->field_0_pName);
    }
    else // < 100
    {
        // Single FMV
        FmvInfo* pFmvRec1 = Path_Get_FMV_Record(lvlId, pMap->mFmvBaseId);
        sLevelId_dword_5CA408 = static_cast<s32>(MapWrapper::ToAE(lvlId)); // HACK
        return relive_new CameraSwapper(ppBits,
                                              pFmvRec1->field_8_flags == 1,
                                              pFmvRec1->field_0_pName);
    }
}

Camera* Map::Create_Camera(s16 xpos, s16 ypos, s32 /*a4*/)
{
    // Check min bound
    if (xpos < 0 || ypos < 0)
    {
        return nullptr;
    }

    // Check max bounds
    if (xpos >= sPathInfo->mCamsOnX || ypos >= sPathInfo->mCamsOnY)
    {
        return nullptr;
    }

    // Return existing camera if we already have one
    for (s32 i = 0; i < ALIVE_COUNTOF(field_40_stru_5); i++)
    {
        if (field_40_stru_5[i]
            && field_40_stru_5[i]->field_1A_level == mCurrentLevel
            && field_40_stru_5[i]->field_18_path == mCurrentPath
            && field_40_stru_5[i]->mCamXOff == xpos
            && field_40_stru_5[i]->mCamYOff == ypos)
        {
            Camera* pTemp = field_40_stru_5[i];
            field_40_stru_5[i] = nullptr;
            return pTemp;
        }
    }

    // Get a pointer to the camera name from the Path resource
    const BinaryPath* pPathData = GetPathResourceBlockPtr(mCurrentPath);
    auto pCamName = pPathData->CameraName(xpos, ypos);

    // Empty/blank camera in the map array
    if (!pCamName || !pCamName[0])
    {
        return nullptr;
    }

    Camera* newCamera = relive_new Camera();

    // Copy in the camera name from the Path resource and append .CAM
    memset(newCamera->field_1E_cam_name, 0, sizeof(newCamera->field_1E_cam_name));
    strncpy(newCamera->field_1E_cam_name, pCamName, ALIVE_COUNTOF(CameraName::name));
    strcat(newCamera->field_1E_cam_name, ".CAM");

    newCamera->mCamXOff = xpos;
    newCamera->mCamYOff = ypos;

    newCamera->field_30_flags &= -1;

    newCamera->field_1A_level = mCurrentLevel;
    newCamera->field_18_path = mCurrentPath;

    // Calculate hash/resource ID of the camera
    newCamera->field_10_camera_resource_id = 1 * (pCamName[7] - '0') + 10 * (pCamName[6] - '0') + 100 * (pCamName[4] - '0') + 1000 * (pCamName[3] - '0');

    // Convert the 2 digit camera number string to an integer
    newCamera->field_1C_camera_number = 1 * (pCamName[7] - '0') + 10 * (pCamName[6] - '0');

    return newCamera;
}

void Map::Load_Path_Items(Camera* pCamera, LoadMode loadMode)
{
    if (!pCamera)
    {
        return;
    }

    // Is camera resource loaded check
    if (!(pCamera->field_30_flags & 1))
    {
        if (loadMode == LoadMode::ConstructObject_0)
        {
            // Async camera load
            // ResourceManager::LoadResourceFile_49C130(pCamera->field_1E_cam_name, Camera::On_Loaded, pCamera, pCamera);
            pCamera->field_C_pCamRes = ResourceManagerWrapper::LoadCam(pCamera->field_1A_level, pCamera->field_18_path, pCamera->field_1C_camera_number);

            sPathInfo->Loader_4DB800(pCamera->mCamXOff, pCamera->mCamYOff, LoadMode::LoadResourceFromList_1, ReliveTypes::eNone); // none = load all
        }
        else
        {
            // Blocking camera load
            // ResourceManager::LoadResourceFile_49C170(pCamera->field_1E_cam_name, pCamera);
            pCamera->field_30_flags |= 1;
            // pCamera->field_C_pCamRes = ResourceManagerWrapper::LoadCam(pCamera->field_1A_level, pCamera->field_18_path, pCamera->field_1C_camera_number);

            sPathInfo->Loader_4DB800(pCamera->mCamXOff, pCamera->mCamYOff, LoadMode::LoadResource_2, ReliveTypes::eNone); // none = load all
        }

    }
}

void Map::LoadResource(const char_type* /*pFileName*/, s32 /*type*/, s32 /*resourceId*/, LoadMode loadMode, s16 bDontLoad)
{
    if (!bDontLoad)
    {
        if (loadMode == LoadMode::LoadResource_2)
        {
            pResourceManager->LoadingLoop(0);
        }
    }
}

relive::Path_TLV* Map::VTLV_Get_At(s16 xpos, s16 ypos, s16 width, s16 height, ReliveTypes typeToFind)
{
    return sPathInfo->TLV_Get_At(xpos, ypos, width, height, typeToFind);
}

Map gMap = {};
