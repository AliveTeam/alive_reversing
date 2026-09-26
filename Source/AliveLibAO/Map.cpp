#include "stdafx_ao.h"
#include "Map.hpp"
#include "../relive_lib/Function.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "PathData.hpp"
#include "Engine.hpp"
#include "Midi.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "../relive_lib/Sound/PsxSpuApi.hpp"
#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include "Abe.hpp"
#include "QuikSave.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "AmbientSound.hpp"
#include "../relive_lib/GameObjects/BackgroundMusic.hpp"
#include "MusicController.hpp"
#include "CameraSwapper.hpp"
#include "../relive_lib/Engine.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Sfx.hpp"
#include "Elum.hpp"
#include "../relive_lib/Sys.hpp"
#include "../relive_lib/Camera.hpp"

#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/FatalError.hpp"
#include "../relive_lib/BinaryPath.hpp"
#include "../AliveLibAE/PathData.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "../relive_lib/GameObjects/PlatformBase.hpp"

class BaseGameObject;

namespace AO {


OpenSeqHandle g_SeqTable_4C9E70[165] = {
    {"D1AMB.SEQ", 0, 0, 100, -1, {}},
    {"D2AMB.SEQ", 0, 0, 100, -1, {}},
    {"E1AMB.SEQ", 0, 0, 60, -1, {}},
    {"E2AMB.SEQ", 0, 0, 60, -1, {}},
    {"E2AMB2.SEQ", 0, 0, 60, -1, {}},
    {"F1AMB.SEQ", 0, 0, 60, -1, {}},
    {"F2AMB.SEQ", 0, 0, 100, -1, {}},
    {"MLAMB.SEQ", 0, 0, 60, -1, {}},
    {"RFAMB.SEQ", 0, 0, 70, -1, {}},
    {"OPTAMB.SEQ", 0, 0, 100, -1, {}},
    {"GUN.SEQ", 0, 0, 40, -1, {}},
    {"OHM.SEQ", 0, 0, 80, -1, {}},
    {"MUDOHM.SEQ", 0, 0, 80, -1, {}},
    {"EBELL2.SEQ", 0, 0, 127, -1, {}},
    {"ABEMOUNT.SEQ", 0, 0, 85, -1, {}},
    {"ESCRATCH.SEQ", 0, 0, 45, -1, {}},
    {"SSCRATCH.SEQ", 0, 0, 55, -1, {}},
    {"PANTING.SEQ", 0, 0, 45, -1, {}},
    {"BATSQUEK.SEQ", 0, 0, 55, -1, {}},
    {"WHISTLE1.SEQ", 0, 0, 127, -1, {}},
    {"WHISTLE2.SEQ", 0, 0, 127, -1, {}},
    {"SLIGBOMB.SEQ", 0, 0, 127, -1, {}},
    {"SLIGBOM2.SEQ", 0, 0, 127, -1, {}},
    {"OOPS.SEQ", 0, 0, 40, -1, {}},
    {"PIGEONS.SEQ", 0, 0, 40, -1, {}},
    {"CHIPPER.SEQ", 0, 0, 90, -1, {}},
    {"PATROL.SEQ", 0, 0, 60, -1, {}},
    {"SLEEPING.SEQ", 0, 0, 60, -1, {}},
    {"ONCHAIN.SEQ", 0, 0, 60, -1, {}},
    {"SLOSLEEP.SEQ", 0, 0, 60, -1, {}},
    {"PARAPANT.SEQ", 0, 0, 35, -1, {}},
    {"GRINDER.SEQ", 0, 0, 35, -1, {}},
    {"BASICTRK.SEQ", 0, 0, 90, -1, {}},
    {"LE_LO_1.SEQ", 0, 0, 127, -1, {}},
    {"LE_LO_2.SEQ", 0, 0, 127, -1, {}},
    {"LE_LO_3.SEQ", 0, 0, 127, -1, {}},
    {"LE_LO_4.SEQ", 0, 0, 127, -1, {}},
    {"LE_SH_1.SEQ", 0, 0, 127, -1, {}},
    {"LE_SH_2.SEQ", 0, 0, 127, -1, {}},
    {"LE_SH_3.SEQ", 0, 0, 127, -1, {}},
    {"LE_SH_4.SEQ", 0, 0, 127, -1, {}},
    {"MYSTERY1.SEQ", 0, 0, 60, -1, {}},
    {"MYSTERY2.SEQ", 0, 0, 60, -1, {}},
    {"NEGATIV1.SEQ", 0, 0, 60, -1, {}},
    {"NEGATIV3.SEQ", 0, 0, 60, -1, {}},
    {"POSITIV1.SEQ", 0, 0, 60, -1, {}},
    {"POSITIV9.SEQ", 0, 0, 60, -1, {}},
    {"D1_0_1.SEQ", 0, 0, 60, -1, {}},
    {"D1_0_2.SEQ", 0, 0, 60, -1, {}},
    {"D1_0_3.SEQ", 0, 0, 60, -1, {}},
    {"D1_1_1.SEQ", 0, 0, 60, -1, {}},
    {"D1_1_2.SEQ", 0, 0, 60, -1, {}},
    {"D1_1_3.SEQ", 0, 0, 60, -1, {}},
    {"D1_1_4.SEQ", 0, 0, 60, -1, {}},
    {"D1_1_5.SEQ", 0, 0, 60, -1, {}},
    {"D1_2_1.SEQ", 0, 0, 60, -1, {}},
    {"D1_2_2.SEQ", 0, 0, 60, -1, {}},
    {"D1_2_3.SEQ", 0, 0, 60, -1, {}},
    {"D1_2_4.SEQ", 0, 0, 60, -1, {}},
    {"D1_2_5.SEQ", 0, 0, 60, -1, {}},
    {"D1_3_1.SEQ", 0, 0, 60, -1, {}},
    {"D1_4_1.SEQ", 0, 0, 60, -1, {}},
    {"D1_5_1.SEQ", 0, 0, 60, -1, {}},
    {"D1_6_1.SEQ", 0, 0, 60, -1, {}},
    {"D2_0_1.SEQ", 0, 0, 60, -1, {}},
    {"D2_0_2.SEQ", 0, 0, 60, -1, {}},
    {"D2_1_1.SEQ", 0, 0, 60, -1, {}},
    {"D2_1_2.SEQ", 0, 0, 60, -1, {}},
    {"D2_2_1.SEQ", 0, 0, 60, -1, {}},
    {"D2_2_2.SEQ", 0, 0, 60, -1, {}},
    {"D2_4_1.SEQ", 0, 0, 60, -1, {}},
    {"D2_5_1.SEQ", 0, 0, 60, -1, {}},
    {"D2_6_1.SEQ", 0, 0, 60, -1, {}},
    {"DE_2_1.SEQ", 0, 0, 90, -1, {}},
    {"DE_4_1.SEQ", 0, 0, 90, -1, {}},
    {"DE_5_1.SEQ", 0, 0, 90, -1, {}},
    {"E1_0_1.SEQ", 0, 0, 60, -1, {}},
    {"E1_0_2.SEQ", 0, 0, 60, -1, {}},
    {"E1_0_3.SEQ", 0, 0, 60, -1, {}},
    {"E1_0_4.SEQ", 0, 0, 60, -1, {}},
    {"E1_0_5.SEQ", 0, 0, 60, -1, {}},
    {"E1_1_1.SEQ", 0, 0, 60, -1, {}},
    {"E1_1_2.SEQ", 0, 0, 60, -1, {}},
    {"E1_1_3.SEQ", 0, 0, 60, -1, {}},
    {"E1_1_4.SEQ", 0, 0, 90, -1, {}},
    {"E1_1_5.SEQ", 0, 0, 90, -1, {}},
    {"E1_4_1.SEQ", 0, 0, 90, -1, {}},
    {"E1_5_1.SEQ", 0, 0, 90, -1, {}},
    {"E1_6_1.SEQ", 0, 0, 90, -1, {}},
    {"F1_0_1.SEQ", 0, 0, 90, -1, {}},
    {"F1_0_2.SEQ", 0, 0, 90, -1, {}},
    {"F1_0_3.SEQ", 0, 0, 90, -1, {}},
    {"F1_1_1.SEQ", 0, 0, 90, -1, {}},
    {"F1_1_2.SEQ", 0, 0, 90, -1, {}},
    {"F1_1_3.SEQ", 0, 0, 90, -1, {}},
    {"F1_2_1.SEQ", 0, 0, 90, -1, {}},
    {"F1_2_2.SEQ", 0, 0, 90, -1, {}},
    {"F1_2_3.SEQ", 0, 0, 90, -1, {}},
    {"F1_2_4.SEQ", 0, 0, 90, -1, {}},
    {"F1_3_1.SEQ", 0, 0, 90, -1, {}},
    {"F1_4_1.SEQ", 0, 0, 90, -1, {}},
    {"F1_5_1.SEQ", 0, 0, 90, -1, {}},
    {"F1_6_1.SEQ", 0, 0, 90, -1, {}},
    {"F2_0_1.SEQ", 0, 0, 90, -1, {}},
    {"F2_1_1.SEQ", 0, 0, 90, -1, {}},
    {"F2_2_1.SEQ", 0, 0, 90, -1, {}},
    {"F2_4_1.SEQ", 0, 0, 90, -1, {}},
    {"F2_5_1.SEQ", 0, 0, 90, -1, {}},
    {"F2_6_1.SEQ", 0, 0, 90, -1, {}},
    {"FE_2_1.SEQ", 0, 0, 90, -1, {}},
    {"FE_4_1.SEQ", 0, 0, 90, -1, {}},
    {"FE_5_1.SEQ", 0, 0, 90, -1, {}},
    {"ML_0_1.SEQ", 0, 0, 90, -1, {}},
    {"ML_0_2.SEQ", 0, 0, 90, -1, {}},
    {"ML_0_3.SEQ", 0, 0, 90, -1, {}},
    {"ML_0_4.SEQ", 0, 0, 90, -1, {}},
    {"ML_0_5.SEQ", 0, 0, 90, -1, {}},
    {"ML_1_1.SEQ", 0, 0, 90, -1, {}},
    {"ML_1_2.SEQ", 0, 0, 90, -1, {}},
    {"ML_1_3.SEQ", 0, 0, 90, -1, {}},
    {"ML_1_4.SEQ", 0, 0, 90, -1, {}},
    {"ML_1_5.SEQ", 0, 0, 90, -1, {}},
    {"ML_2_1.SEQ", 0, 0, 90, -1, {}},
    {"ML_2_2.SEQ", 0, 0, 90, -1, {}},
    {"ML_2_3.SEQ", 0, 0, 90, -1, {}},
    {"ML_2_4.SEQ", 0, 0, 90, -1, {}},
    {"ML_2_5.SEQ", 0, 0, 90, -1, {}},
    {"ML_3_1.SEQ", 0, 0, 90, -1, {}},
    {"ML_4_1.SEQ", 0, 0, 90, -1, {}},
    {"ML_5_1.SEQ", 0, 0, 90, -1, {}},
    {"ML_6_1.SEQ", 0, 0, 90, -1, {}},
    {"RF_0_1.SEQ", 0, 0, 90, -1, {}},
    {"RF_0_2.SEQ", 0, 0, 90, -1, {}},
    {"RF_0_3.SEQ", 0, 0, 90, -1, {}},
    {"RF_0_4.SEQ", 0, 0, 90, -1, {}},
    {"RF_1_1.SEQ", 0, 0, 90, -1, {}},
    {"RF_1_2.SEQ", 0, 0, 90, -1, {}},
    {"RF_1_3.SEQ", 0, 0, 90, -1, {}},
    {"RF_2_1.SEQ", 0, 0, 90, -1, {}},
    {"RF_2_2.SEQ", 0, 0, 90, -1, {}},
    {"RF_2_3.SEQ", 0, 0, 90, -1, {}},
    {"RF_2_4.SEQ", 0, 0, 90, -1, {}},
    {"RF_4_1.SEQ", 0, 0, 90, -1, {}},
    {"RF_5_1.SEQ", 0, 0, 90, -1, {}},
    {"RF_6_1.SEQ", 0, 0, 90, -1, {}},
    {"RE_2_1.SEQ", 0, 0, 90, -1, {}},
    {"RE_4_1.SEQ", 0, 0, 90, -1, {}},
    {"RE_5_1.SEQ", 0, 0, 90, -1, {}},
    {"OPT_0_1.SEQ", 0, 0, 60, -1, {}},
    {"OPT_0_2.SEQ", 0, 0, 60, -1, {}},
    {"OPT_0_3.SEQ", 0, 0, 60, -1, {}},
    {"OPT_0_4.SEQ", 0, 0, 60, -1, {}},
    {"OPT_0_5.SEQ", 0, 0, 60, -1, {}},
    {"OPT_1_1.SEQ", 0, 0, 60, -1, {}},
    {"OPT_1_2.SEQ", 0, 0, 60, -1, {}},
    {"OPT_1_3.SEQ", 0, 0, 60, -1, {}},
    {"OPT_1_4.SEQ", 0, 0, 90, -1, {}},
    {"OPT_1_5.SEQ", 0, 0, 90, -1, {}},
    {"ALL_4_1.SEQ", 0, 0, 90, -1, {}},
    {"ALL_5_1.SEQ", 0, 0, 90, -1, {}},
    {"ALL_5_2.SEQ", 0, 0, 90, -1, {}},
    {"ALL_5_3.SEQ", 0, 0, 90, -1, {}},
    {"ALL_7_1.SEQ", 0, 0, 90, -1, {}},
    {"ALL_8_1.SEQ", 0, 0, 90, -1, {}},
    {nullptr, 0, 0, 0, 0, {}}};

Map::Map(ResourceManagerWrapper& resMan, relive::Factory& factory)
    : BaseMap(resMan, factory)
    , mPath(*this, factory)
{
    Reset();
}

s16 Map::GetOverlayId()
{
    return AO::Path_Get_Bly_Record(mNextLevel, mNextPath)->mOverlayId;
}

CameraPos Map::Rect_Location_Relative_To_Active_Camera(const PSX_RECT* pRect, s16 width)
{
    if (EventGet(Event::kEventDeathReset))
    {
        return CameraPos::eCamNone_5;
    }

    FP xTweak = {};
    FP yTweak = {};
    if (width)
    {
        xTweak = FP_FromInteger(234);
        yTweak = FP_FromInteger(150);
    }
    else
    {
        xTweak = FP_FromInteger(184);
        yTweak = FP_FromInteger(120);
    }

    if (pRect->x > FP_GetExponent(mCameraOffset.x + xTweak))
    {
        return CameraPos::eCamRight_4;
    }

    if (pRect->y > FP_GetExponent(mCameraOffset.y + yTweak))
    {
        return CameraPos::eCamBottom_2;
    }

    if (pRect->w >= FP_GetExponent(mCameraOffset.x - xTweak))
    {
        if (pRect->h < FP_GetExponent(mCameraOffset.y - yTweak))
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

s16 Map::Get_Camera_World_Rect(CameraPos camIdx, PSX_RECT* pRect)
{
    if (camIdx < CameraPos::eCamCurrent_0 || camIdx > CameraPos::eCamRight_4)
    {
        return 0;
    }

    Camera* pCamera = mCurrentCameras[static_cast<s32>(camIdx)];
    if (!pCamera)
    {
        return 0;
    }

    if (!pRect)
    {
        return 1;
    }

    s16 cam_x_pos = mPath.mPathData->mGridWidth * pCamera->mCamXOff;
    cam_x_pos += 120;

    const s16 cam_y_pos = mPath.mPathData->mGridHeight * pCamera->mCamYOff;

    pRect->x = cam_x_pos;
    pRect->y = cam_y_pos + 120;
    pRect->w = cam_x_pos + 640;
    pRect->h = cam_y_pos + 360;
    return 1;
}

s16 Map::Is_Point_In_Current_Camera(EReliveLevelIds level, s32 path, FP xpos, FP ypos, s16 width)
{
    if (level != mCurrentLevel || path != mCurrentPath) // TODO: Remove when 100%
    {
        return false;
    }

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(xpos);
    rect.w = FP_GetExponent(xpos);
    rect.y = FP_GetExponent(ypos);
    rect.h = FP_GetExponent(ypos);
    return Rect_Location_Relative_To_Active_Camera(&rect, width) == CameraPos::eCamCurrent_0;
}

ScreenChangeResult Map::GoTo_Camera()
{
    if (mScreenChangeResume == ScreenChangeResume::eAfterCameraLoad)
    {
        mScreenChangeResume = ScreenChangeResume::eNone;
        FinishLoadCamera();
        return ScreenChangeResult::eDone;
    }

    if (mScreenChangeResume == ScreenChangeResume::eAfterPathsLoad)
    {
        LoadPathsAndPendSounds();
        mScreenChangeResume = ScreenChangeResume::eAfterSoundsLoad;
        return ScreenChangeResult::eWaiting;
    }

    if (mScreenChangeResume == ScreenChangeResume::eAfterSoundsLoad)
    {
        ContinueLoadCamera();
        mScreenChangeResume = ScreenChangeResume::eAfterCameraLoad;
        return ScreenChangeResult::eWaiting;
    }

    if (mCameraSwapEffect == CameraSwapEffects::eUnknown_11)
    {
        if (mScreenChangeResume != ScreenChangeResume::eAfterFmvPass)
        {
            CamResource nullRes;
            BaseGameObject* pFmvRet = FMV_Camera_Change(nullRes, this, mCurrentLevel);
            RunFmvCameraChangePass(pFmvRet, false);
            mScreenChangeResume = ScreenChangeResume::eAfterFmvPass;
            return ScreenChangeResult::eWaiting;
        }
        mScreenChangeResume = ScreenChangeResume::eNone;
    }

    if (StartLoadCamera() == ScreenChangeResult::eWaiting)
    {
        mScreenChangeResume = ScreenChangeResume::eAfterPathsLoad;
        return ScreenChangeResult::eWaiting;
    }

    ContinueLoadCamera();
    mScreenChangeResume = ScreenChangeResume::eAfterCameraLoad;
    return ScreenChangeResult::eWaiting;
}

ScreenChangeResult Map::StartLoadCamera()
{
    // NOTE: None check changed to match AE
    if (mCurrentLevel != EReliveLevelIds::eMenu && mCurrentLevel != EReliveLevelIds::eNone)
    {
        if (LevelChanged() || (PathChanged() && mCameraSwapEffect == CameraSwapEffects::ePlay1FMV_5))
        {
            mResourceManager.RequestLoadingWait(LoadingIcon::eNow);
        }
    }

    if (LevelChanged() || PathChanged())
    {
        mOverlayId = GetOverlayId();
    }

    if (LevelChanged())
    {
        // Free all cameras
        for (s32 i = 0; i < ALIVE_COUNTOF(mCurrentCameras); i++)
        {
            if (mCurrentCameras[i])
            {
                Free_Resources_For_Camera(mCurrentCameras[i]);
                relive_delete mCurrentCameras[i];
                mCurrentCameras[i] = nullptr;
            }
        }

        if (mCurrentLevel != EReliveLevelIds::eNone)
        {
            if (LevelChanged())
            {
                SND_Reset();
                FreePathResourceBlocks();
            }

        }

        mResourceManager.PendPaths(mNextLevel);
        mResourceManager.RequestLoadingWait();
        return ScreenChangeResult::eWaiting;
    }
    return ScreenChangeResult::eDone;
}

void Map::LoadPathsAndPendSounds()
{
    // Pended by StartLoadCamera
    mLoadedPaths = mResourceManager.LoadPaths(mNextLevel);
    mResourceManager.FlushMissingResourceReports();

    SND_Pend_Sound_Files(*mLoadedPaths[0]->GetSoundInfo(), mResourceManager);
    mResourceManager.RequestLoadingWait();
}

void Map::ContinueLoadCamera()
{
    if (LevelChanged())
    {
        // Sound files pended by LoadPathsAndPendSounds
        SND_Load_VABS(mLoadedPaths[0]->GetSoundInfo(), AO::Path_Get_Reverb(mNextLevel), mResourceManager, *this); // TODO: Remove hard coded data
        SND_Load_Seqs(g_SeqTable_4C9E70, mLoadedPaths[0]->GetSoundInfo(), mResourceManager, *this);

        relive_new BackgroundMusic(AO::Path_Get_BackGroundMusicId(mNextLevel), mResourceManager, *this); // TODO: Remove hard coded data

        // TODO: Re-add function
        for (s32 i = 0; i < 236; i++)
        {
            gSwitchStates.mData[i] = 0;
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


    const auto old_current_path = mCurrentPath;
    const auto old_current_level = mCurrentLevel;

    mMapChanged = mNextPath != old_current_path || LevelChanged();

    mCurrentCamera = mNextCamera;
    mCurrentPath = mNextPath;
    mCurrentLevel = mNextLevel;

    const PathBlyRec* pPathRecord = AO::Path_Get_Bly_Record(mNextLevel, mNextPath);
    BinaryPath* pNextPath = GetPathResourceBlockPtr(mNextPath);
    mPath.Init(pPathRecord->mPathData, mNextLevel, mNextPath, mNextCamera, pNextPath);

    mCamsOnX = (mPath.mPathData->mTop - mPath.mPathData->mLeft) / mPath.mPathData->mGridWidth;
    mCamsOnY = (mPath.mPathData->mBottom - mPath.mPathData->mRight) / mPath.mPathData->mGridHeight;

    mCamIdxOnX = 0;
    mCamIdxOnY = 0;
    for (auto& cam : pNextPath->GetCameras())
    {
        if (pNextPath->CameraNameAsInteger(cam->mName.c_str()) == static_cast<u32>(mNextCamera))
        {
            mCamIdxOnX = static_cast<s16>(cam->mX);
            mCamIdxOnY = static_cast<s16>(cam->mY);
            break;
        }
    }


    mCameraOffset.x = FP_FromInteger(mCamIdxOnX * mPath.mPathData->mGridWidth + 440);
    mCameraOffset.y = FP_FromInteger(mCamIdxOnY * mPath.mPathData->mGridHeight + 240);

    if (old_current_path != mCurrentPath || old_current_level != mCurrentLevel)
    {
        if (gCollisions)
        {
            // OG FIX: Remove any pointers to the line objects that we are about to delete
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                ::BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
                if (!pObjIter)
                {
                    break;
                }

                if (pObjIter->GetIsBaseAliveGameObject())
                {
                    auto pBaseAliveGameObj = static_cast<::BaseAliveGameObject*>(pObjIter);
                    pBaseAliveGameObj->BaseAliveGameObjectCollisionLine = nullptr;
                }
            }

            if (PlatformBase::Platforms().Size() > 0)
            {
                ALIVE_FATAL("%d Platforms have been leaked!", PlatformBase::Platforms().Size());
            }

            relive_delete gCollisions;
        }

        gCollisions = relive_new Collisions(GetPathResourceBlockPtr(mCurrentPath)->GetCollisions());
    }

    if (mPendingSaveRestore)
    {
        QuikSave::RestoreBlyData(*mPendingSaveRestore, mResourceManager, *this);
        mPendingSaveRestore = nullptr;
    }

    // Copy camera array and blank out the source
    for (s32 i = 0; i < ALIVE_COUNTOF(mCurrentCameras); i++)
    {
        mPreviousCameras[i] = mCurrentCameras[i];
        mCurrentCameras[i] = nullptr;
    }

    mCurrentCameras[0] = Create_Camera(mCamIdxOnX, mCamIdxOnY, 1);
    mCurrentCameras[3] = Create_Camera(mCamIdxOnX - 1, mCamIdxOnY, 0);
    mCurrentCameras[4] = Create_Camera(mCamIdxOnX + 1, mCamIdxOnY, 0);
    mCurrentCameras[1] = Create_Camera(mCamIdxOnX, mCamIdxOnY - 1, 0);
    mCurrentCameras[2] = Create_Camera(mCamIdxOnX, mCamIdxOnY + 1, 0);

    // Free resources for each camera
    for (s32 i = 0; i < ALIVE_COUNTOF(mPreviousCameras); i++)
    {
        if (mPreviousCameras[i])
        {
            Free_Resources_For_Camera(mPreviousCameras[i]);
        }
    }

    // Free each camera itself
    for (s32 i = 0; i < ALIVE_COUNTOF(mPreviousCameras); i++)
    {
        if (mPreviousCameras[i])
        {
            relive_delete mPreviousCameras[i];
            mPreviousCameras[i] = nullptr;
        }
    }

    Load_Path_Items(mCurrentCameras[0], relive::Factory::LoadMode::ConstructObject_0);

    // The camera's objects are made by FinishLoadCamera, once the main loop has waited for
    // what they need
    mLoadCameraPrevPath = old_current_path;
    mLoadCameraPrevLevel = old_current_level;
    mResourceManager.RequestLoadingWait();
}

void Map::FinishLoadCamera()
{
    const s16 old_current_path = mLoadCameraPrevPath;
    const EReliveLevelIds old_current_level = mLoadCameraPrevLevel;

    Finish_Load_Cam(mCurrentCameras[0]);

    Load_Path_Items(mCurrentCameras[3], relive::Factory::LoadMode::ConstructObject_0);
    Load_Path_Items(mCurrentCameras[4], relive::Factory::LoadMode::ConstructObject_0);
    Load_Path_Items(mCurrentCameras[1], relive::Factory::LoadMode::ConstructObject_0);
    Load_Path_Items(mCurrentCameras[2], relive::Factory::LoadMode::ConstructObject_0);

    if (!gScreenManager)
    {
        gScreenManager = relive_new ScreenManager(mCurrentCameras[0]->mCamRes, &mCameraOffset, mResourceManager, *this);
    }

    mPath.Loader(mCamIdxOnX, mCamIdxOnY, relive::Factory::LoadMode::ConstructObject_0, ReliveTypes::eNone); // none = load all

    if (old_current_path != mCurrentPath || old_current_level != mCurrentLevel)
    {
        if (gAbe && mCurrentPath == gAbe->mCurrentPath)
        {
            gAbe->VCheckCollisionLineStillValid(10);
        }

        if (gElum && sControlledCharacter != gElum && mCurrentPath == gElum->mCurrentPath)
        {
            gElum->VCheckCollisionLineStillValid(10);
        }
    }

    Create_FG1s();

    if (mCameraSwapEffect == CameraSwapEffects::ePlay1FMV_5)
    {
        FMV_Camera_Change(mCurrentCameras[0]->mCamRes, this, mNextLevel);
    }

    if (mCameraSwapEffect == CameraSwapEffects::eUnknown_11)
    {
        gScreenManager->DecompressCameraToVRam(mCurrentCameras[0]->mCamRes);
        gScreenManager->EnableRendering();
    }

    if (mCameraSwapEffect != CameraSwapEffects::ePlay1FMV_5 && mCameraSwapEffect != CameraSwapEffects::eUnknown_11)
    {
        if (mPendingTransition == PendingTransition::eDoor_1)
        {
            TlvIterator doorIterator = TLV_First_Of_Type_In_Camera(ReliveTypes::eDoor, 0);
            while (doorIterator.GetTlv<relive::Path_Door>()->mDoorId != gAbe->field_196_door_id)
            {
                doorIterator = Path_TLV::TLV_Next_Of_Type_446500(doorIterator, ReliveTypes::eDoor);
            }

            const auto pCamPos = gScreenManager->mCamPos;
            const auto xpos = gScreenManager->mCamXOff + doorIterator.GetTlv()->MidPointX() - FP_GetExponent(pCamPos->x);
            const auto ypos = gScreenManager->mCamYOff + doorIterator.GetTlv()->mTopLeftY - FP_GetExponent(pCamPos->y);
            relive_new CameraSwapper(
                mCurrentCameras[0]->mCamRes,
                mResourceManager,
                *this,
                mCameraSwapEffect,
                static_cast<s16>(xpos),
                static_cast<s16>(ypos));
        }
        else
        {
            relive_new CameraSwapper(mCurrentCameras[0]->mCamRes, mResourceManager, *this, mCameraSwapEffect, 184, 120);
        }
    }
}

ScreenChangeResult Map::ScreenChange()
{
    if (mCamState == CamChangeStates::eInactive_0)
    {
        return ScreenChangeResult::eDone;
    }

    if (mScreenChangeResume == ScreenChangeResume::eNone && gMap_bDoPurpleLightEffect && mCurrentLevel != EReliveLevelIds::eBoardRoom)
    {
        if (RemoveObjectsWithPurpleLight(1) == PurpleLightResult::eShowing)
        {
            mScreenChangeResume = ScreenChangeResume::eAfterPurpleLight;
            return ScreenChangeResult::eWaiting;
        }
    }

    if (mScreenChangeResume == ScreenChangeResume::eNone || mScreenChangeResume == ScreenChangeResume::eAfterPurpleLight)
    {
        mScreenChangeResume = ScreenChangeResume::eNone;
        NotifyObjectsOfScreenChange();
    }

    return ScreenChange_Common();
}

void Map::NotifyObjectsOfScreenChange()
{
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
            if (pItem->GetDead() && pItem->mChaseCounter == 0)
            {
                j = gBaseGameObjects->RemoveAt(j);
                relive_delete pItem;
            }
        }
    }

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        ::BaseGameObject* pItem = gBaseGameObjects->ItemAt(i);
        if (!pItem)
        {
            break;
        }

        if (pItem->GetDead() && pItem->mChaseCounter == 0)
        {
            i = gBaseGameObjects->RemoveAt(i);
            relive_delete pItem;
        }
    }

    if (gMap_bDoPurpleLightEffect || LevelChanged())
    {
        if (LevelChanged())
        {
            SsUtAllKeyOff(0);
        }

        // TODO: Re-check this logic
        if (mNextLevel != EReliveLevelIds::eMenu)
        {
            if ((mNextLevel != EReliveLevelIds::eRuptureFarmsReturn && mNextLevel != EReliveLevelIds::eForestChase && mNextLevel != EReliveLevelIds::eDesertEscape) || (mNextLevel == EReliveLevelIds::eBoardRoom && mCurrentLevel == EReliveLevelIds::eBoardRoom))
            {
                mSoundChannelsMask = 0;
            }
        }
        else
        {
            mSoundChannelsMask = 0;
        }
    }
}

void Map::VCameraSwapFinished()
{
    gMap_bDoPurpleLightEffect = false;

    BackgroundMusic::Play();
    MusicController::EnableMusic(1);
    Start_Sounds_For_Objects_In_Near_Cameras();
}

ScreenChangeResult Map::Handle_PathTransition()
{
    // Otherwise carrying on after the endings' FMVs or the camera's loading, see GoTo_Camera
    if (mScreenChangeResume == ScreenChangeResume::eNone)
    {
        mPathTransitionFromTlv = false;

        relive::Path_PathTransition* pTlv = nullptr;
        if (mAliveObj)
        {
            pTlv = VTLV_Get_At_Of_Type(
                FP_GetExponent(mAliveObj->mXPos),
                FP_GetExponent(mAliveObj->mYPos),
                FP_GetExponent(mAliveObj->mXPos),
                FP_GetExponent(mAliveObj->mYPos),
                ReliveTypes::ePathTransition).GetTlv<relive::Path_PathTransition>();
        }

        if (mAliveObj && pTlv)
        {
            mNextLevel = pTlv->mNextLevel;
            mNextPath = pTlv->mNextPath;
            mNextCamera = pTlv->mNextCamera;
            mFmvIds = FmvIds{pTlv->mMovie1, pTlv->mMovie2, pTlv->mMovie3};

            mCameraSwapEffect = kPathChangeEffectToInternalScreenChangeEffect[pTlv->mWipeEffect];

            mAliveObj->mCurrentLevel = pTlv->mNextLevel;
            mAliveObj->mCurrentPath = pTlv->mNextPath;

            // TODO: Probably OG bug, when changing camera/path the TLV pointer can become invalid
            // resulting in a corrupted next_path_scale value ?
            // Pointer points to the Path res which is invalid after ResourceManager::GetLoadedResource(ResourceManager::Resource_Path, i, true, false);
            // is called. Happens even if calling real func below.
            mPathTransitionFromTlv = true;
            mPathTransitionScale = pTlv->mNextPathScale;
        }
        else
        {
            switch (mMapDirection)
            {
                case MapDirections::eMapLeft_0:
                    mCamIdxOnX--;
                    if (mAliveObj)
                    {
                        mAliveObj->VSetXSpawn(
                            mCamIdxOnX * mPath.mPathData->mGridWidth,
                            MaxGridBlocks(mAliveObj->GetSpriteScale()) - 1);
                    }
                    mCameraSwapEffect = CameraSwapEffects::eRightToLeft_2;
                    break;
                case MapDirections::eMapRight_1:
                    mCamIdxOnX++;
                    if (mAliveObj)
                    {
                        mAliveObj->VSetXSpawn(mCamIdxOnX * mPath.mPathData->mGridWidth,
                                                       1);
                    }
                    mCameraSwapEffect = CameraSwapEffects::eLeftToRight_1;
                    break;
                case MapDirections::eMapTop_2:
                    mCamIdxOnY--;
                    if (mAliveObj)
                    {
                        mAliveObj->VSetYSpawn(mCamIdxOnY * mPath.mPathData->mGridHeight,
                                                       1);
                    }
                    mCameraSwapEffect = CameraSwapEffects::eBottomToTop_4;
                    break;
                case MapDirections::eMapBottom_3:
                    mCamIdxOnY++;
                    if (mAliveObj)
                    {
                        mAliveObj->VSetYSpawn(mCamIdxOnY * mPath.mPathData->mGridHeight,
                                                       2);
                    }
                    mCameraSwapEffect = CameraSwapEffects::eTopToBottom_3;
                    break;
                default:
                    break;
            }

            const BinaryPath* pPathRes = GetPathResourceBlockPtr(mCurrentPath);
            const char* pCameraName = pPathRes->CameraName(mCamIdxOnX, mCamIdxOnY);
            mNextCamera = static_cast<s16>(pPathRes->CameraNameAsInteger(pCameraName));
        }
    }

    if (GoTo_Camera() == ScreenChangeResult::eWaiting)
    {
        return ScreenChangeResult::eWaiting;
    }

    if (mPathTransitionFromTlv)
    {
        FinishPathTransition();
    }
    return ScreenChangeResult::eDone;
}

void Map::FinishPathTransition()
{
    switch (mPathTransitionScale)
    {
        case relive::reliveScale::eFull:
            gAbe->SetSpriteScale(FP_FromInteger(1));
            gAbe->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
            if (gElum)
            {
                gElum->SetSpriteScale(gAbe->GetSpriteScale());
                gElum->GetAnimation().SetRenderLayer(Layer::eLayer_ZapLinesElumMuds_28);
            }
            break;

        case relive::reliveScale::eHalf:
            gAbe->SetSpriteScale(FP_FromDouble(0.5));
            gAbe->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
            if (gElum)
            {
                gElum->SetSpriteScale(gAbe->GetSpriteScale());
                gElum->GetAnimation().SetRenderLayer(Layer::eLayer_ZapLinesMudsElum_Half_9);
            }
            break;

        default:
            LOG_ERROR("Invalid scale %d", static_cast<s16>(mPathTransitionScale));
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
        mPath.mPathData->mGridWidth * mCamIdxOnX,
        mPath.mPathData->mGridHeight * mCamIdxOnY,
        remapped);
}

void Map::VCollectPurpleLightObjects(DynamicArrayT<BaseAnimatedWithPhysicsGameObject>& objects, DynamicArrayT<Particle>& lights)
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        auto pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetDrawable())
        {
            auto pBaseObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);
            if (pBaseObj->GetDoPurpleLightEffect())
            {
                if (pBaseObj->GetAnimation().GetRender())
                {
                    if (!pBaseObj->GetDead() && pObj != sControlledCharacter)
                    {
                        bool bAdd = false;
                        if (pBaseObj->mCurrentLevel == mCurrentLevel
                            && pBaseObj->mCurrentPath == mCurrentPath)
                        {
                            PSX_RECT rect = {};
                            rect.x = FP_GetExponent(pBaseObj->mXPos);
                            rect.w = FP_GetExponent(pBaseObj->mXPos);
                            rect.y = FP_GetExponent(pBaseObj->mYPos);
                            rect.h = FP_GetExponent(pBaseObj->mYPos);
                            bAdd = Rect_Location_Relative_To_Active_Camera(&rect, 0) == CameraPos::eCamCurrent_0;
                        }

                        if (bAdd)
                        {
                            AddPurpleLight(pBaseObj, objects, lights);
                        }
                    }
                }
            }
        }
    }
}

s32 Map::VPurpleLightFrameCount(s16 bMakeInvisible)
{
    return bMakeInvisible != 0 ? 12 : 4;
}

CameraSwapper* Map::FMV_Camera_Change(CamResource& ppBits, Map* pMap, EReliveLevelIds levelId)
{
    FmvInfo* pFmvRec1 = AO::Path_Get_FMV_Record(levelId, pMap->mFmvIds.mFmv1);
    FmvInfo* pFmvRec2 = AO::Path_Get_FMV_Record(levelId, pMap->mFmvIds.mFmv2);
    FmvInfo* pFmvRec3 = AO::Path_Get_FMV_Record(levelId, pMap->mFmvIds.mFmv3);

    if ((pFmvRec1 && pFmvRec1->mFlags) || (pFmvRec2 && pFmvRec2->mFlags) || (pFmvRec3 && pFmvRec3->mFlags))
    {
        BackgroundMusic::Stop();
        MusicController::EnableMusic(0);
    }

    return relive_new CameraSwapper(
        ppBits,
        pMap->mResourceManager,
        *pMap,
        pFmvRec1 && pFmvRec1->mFlag2 == 1, pFmvRec1 ? pFmvRec1->mName : nullptr,
        pFmvRec2 && pFmvRec2->mFlag2 == 1, pFmvRec2 ? pFmvRec2->mName : nullptr,
        pFmvRec3 && pFmvRec3->mFlag2 == 1, pFmvRec3 ? pFmvRec3->mName : nullptr);
}

relive::Path_TLV* Path_TLV::Next_446460(relive::Path_TLV* pTlv)
{
    return Next(pTlv);
}

} // namespace AO
