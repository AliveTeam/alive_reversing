#include "stdafx.h"
#include "CameraSwapper.hpp"
#include "../relive_lib/Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "BackgroundMusic.hpp"
#include "MusicController.hpp"
#include "AmbientSound.hpp"
#include "stdlib.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Sfx.hpp"
#include "Movie.hpp"
#include "ScreenClipper.hpp"
#include "../relive_lib/FatalError.hpp"

CameraSwapper::CameraSwapper(CamResource& camRes, bool bPutDispEnv1, const char_type* pFmv1, bool bPutDispEnv2, const char_type* pFmv2, bool bPutDispEnv3, const char_type* pFmv3)
    : BaseGameObject(true, 0)
{
    mFmvs[0] = pFmv1;
    mPutDispEnv[0] = bPutDispEnv1;

    mFmvs[1] = pFmv2;
    mPutDispEnv[1] = bPutDispEnv2;

    mFmvs[2] = pFmv3;
    mPutDispEnv[2] = bPutDispEnv3;

    if (mFmvs[0] && mFmvs[1] && mFmvs[2])
    {
        Init(camRes, CameraSwapEffects::ePlay3FMVs_10);
    }
    else if (mFmvs[0] && mFmvs[1])
    {
        Init(camRes, CameraSwapEffects::ePlay2FMVs_9);
    }
    else
    {
        Init(camRes, CameraSwapEffects::ePlay1FMV_5);
    }

    relive_new Movie(mFmvs[0]);

    mMoviePutDispEnv = mPutDispEnv[0];
}

CameraSwapper::CameraSwapper(CamResource& camRes, CameraSwapEffects changeEffect, s32 xpos, s32 ypos)
    : BaseGameObject(true, 0)
{
    mXPosConverted = static_cast<s16>(PsxToPCX(xpos));
    mYPosConverted = static_cast<s16>(ypos);
    Init(camRes, changeEffect);
}

CameraSwapper::~CameraSwapper()
{
    gNumCamSwappers--;

    if (mScreenClipper)
    {
        mScreenClipper->SetDead(true);
    }

    if (gMap_bDoPurpleLightEffect)
    {
        gMap.RemoveObjectsWithPurpleLight(0);
        gMap_bDoPurpleLightEffect = 0;
    }

    BackgroundMusic::Play();
    MusicController::static_EnableMusic(1);
    Start_Sounds_For_Objects_In_Near_Cameras();
}

static const s32 kSliceWidth = 8;

void CameraSwapper::Init(CamResource& camRes, CameraSwapEffects changeEffect)
{
    SetUpdateDuringCamSwap(true);

    SetType(ReliveTypes::eCameraSwapper);

    mScreenClipper = nullptr;

    if (changeEffect == CameraSwapEffects::ePlay1FMV_5 || changeEffect == CameraSwapEffects::ePlay2FMVs_9 || changeEffect == CameraSwapEffects::ePlay3FMVs_10)
    {
        mCamRes = camRes;
    }
    else
    {
        gScreenManager->DecompressCameraToVRam(camRes);
    }

    gNumCamSwappers++;

    if (gNumCamSwappers != 1)
    {
        SetUpdatable(false);
        SetListAddFailed(true);
        SetDead(true);

        // There can only be 1 active at a time
        return;
    }
    mCamChangeEffect = changeEffect;

    PSX_Point xy = {};
    PSX_Point wh = {};

    switch (changeEffect)
    {
        case CameraSwapEffects::eInstantChange_0:
            SetDead(true);
            mScreenClipper = nullptr;
            return;

        case CameraSwapEffects::eLeftToRight_1:
            mSliceWidth = gPsxDisplay.mWidth / kSliceWidth;
            mSlicesPerTick = -1;
            mTotalSlices = gPsxDisplay.mWidth / mSliceWidth;
            mCurrentSlice = mTotalSlices;

            xy.y = 0;
            xy.x = gPsxDisplay.mWidth;

            wh.x = gPsxDisplay.mWidth;
            wh.y = gPsxDisplay.mHeight;

            mScreenClipper = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eRightToLeft_2:
            mSliceWidth = gPsxDisplay.mWidth / kSliceWidth;
            mSlicesPerTick = 1;
            mTotalSlices = gPsxDisplay.mWidth / mSliceWidth;
            mCurrentSlice = -1;

            xy.y = 0;
            xy.x = 0;

            wh.x = 0;
            wh.y = gPsxDisplay.mHeight;

            mScreenClipper = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eTopToBottom_3:
            mSliceWidth = gPsxDisplay.mHeight / kSliceWidth;
            mSlicesPerTick = -1;
            mTotalSlices = gPsxDisplay.mHeight / mSliceWidth;
            mCurrentSlice = mTotalSlices;

            xy.x = 0;
            xy.y = gPsxDisplay.mHeight;

            wh.x = gPsxDisplay.mWidth;
            wh.y = gPsxDisplay.mHeight;

            mScreenClipper = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eBottomToTop_4:
            mSliceWidth = gPsxDisplay.mHeight / kSliceWidth;
            mSlicesPerTick = 1;
            mTotalSlices = gPsxDisplay.mHeight / mSliceWidth;
            mCurrentSlice = -1;

            xy.x = 0;
            xy.y = 0;

            wh.x = gPsxDisplay.mWidth;
            wh.y = 0;

            mScreenClipper = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eVerticalSplit_6:
            mSliceWidth = (gPsxDisplay.mWidth / 2) / kSliceWidth;
            mSlicesPerTick = 1;
            mTotalSlices = (gPsxDisplay.mWidth / 2) / mSliceWidth;
            mCurrentSlice = 0;

            xy.x = gPsxDisplay.mWidth / 2;
            xy.y = 0;

            wh.x = gPsxDisplay.mWidth / 2;
            wh.y = gPsxDisplay.mHeight;

            mScreenClipper = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eHorizontalSplit_7:
            mSliceWidth = (gPsxDisplay.mHeight / 2) / kSliceWidth;
            mSlicesPerTick = 1;
            mTotalSlices = (gPsxDisplay.mHeight / 2) / mSliceWidth;
            mCurrentSlice = 0;

            xy.x = 0;
            xy.y = gPsxDisplay.mHeight / 2;

            wh.x = gPsxDisplay.mWidth;
            wh.y = gPsxDisplay.mHeight / 2;

            mScreenClipper = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eBoxOut_8:
        {
            mXSlices = (gPsxDisplay.mWidth / 2) / kSliceWidth;
            mYSlices = (gPsxDisplay.mHeight / 2) / kSliceWidth;

            s16 xDiff = gPsxDisplay.mWidth - mXPosConverted;
            if (xDiff <= mXPosConverted)
            {
                xDiff = mXPosConverted;
            }

            s16 yDiff = gPsxDisplay.mHeight - mYPosConverted;
            if (yDiff <= mYPosConverted)
            {
                yDiff = mYPosConverted;
            }

            s16 startingSlice = 0;
            if ((xDiff / mXSlices) <= (yDiff / mYSlices))
            {
                startingSlice = yDiff / mYSlices;
            }
            else
            {
                startingSlice = xDiff / mXSlices;
            }

            mSlicesPerTick = 1;
            mTotalSlices = startingSlice + 1;
            mCurrentSlice = 0;

            xy.x = gPsxDisplay.mWidth - 1;
            xy.y = gPsxDisplay.mHeight - 1;

            mScreenClipper = relive_new ScreenClipper(xy, PSX_Point{1, 1}, Layer::eLayer_0);

            // "Whoosh" door sound effect
            SfxPlayMono(relive::SoundEffects::IngameTransition, 127);
        }
        break;

        case CameraSwapEffects::ePlay1FMV_5:
        case CameraSwapEffects::ePlay2FMVs_9:
        case CameraSwapEffects::ePlay3FMVs_10:
            mScreenClipper = relive_new ScreenClipper(PSX_Point{0, 0}, PSX_Point{1, 1}, Layer::eLayer_0);
            break;

        default:
            ALIVE_FATAL("Unknown camera swap effect"); // Or one that isn't handled here
    }
}

void CameraSwapper::VUpdate()
{
    if (GetDead())
    {
        return;
    }

    switch (mCamChangeEffect)
    {
        case CameraSwapEffects::eLeftToRight_1:
        case CameraSwapEffects::eRightToLeft_2:
        {
            mCurrentSlice += mSlicesPerTick;
            if (mCurrentSlice < 0 || mCurrentSlice >= mTotalSlices)
            {
                // All slices done
                SetDead(true);
                return;
            }

            const s16 xpos = mSliceWidth * mCurrentSlice;
            s16 width = (mSliceWidth * (mCurrentSlice + 1));
            if (width > gPsxDisplay.mWidth - 1)
            {
                width = gPsxDisplay.mWidth - 1;
            }

            mScreenClipper->Update_Clip_Rect({xpos, 0}, {static_cast<s16>(width + 1), gPsxDisplay.mHeight});
        }
        break;

        case CameraSwapEffects::eTopToBottom_3:
        case CameraSwapEffects::eBottomToTop_4:
        {
            mCurrentSlice += mSlicesPerTick;
            if (mCurrentSlice < 0 || mCurrentSlice >= mTotalSlices)
            {
                // All slices done
                SetDead(true);
                return;
            }

            const s16 ypos = mSliceWidth * mCurrentSlice;
            s16 height = (mSliceWidth * (mCurrentSlice + 1));

            mScreenClipper->Update_Clip_Rect({0, ypos}, {gPsxDisplay.mWidth, height});
        }
        break;

        case CameraSwapEffects::eVerticalSplit_6:
        {
            mCurrentSlice += mSlicesPerTick;
            if (mCurrentSlice < 0 || mCurrentSlice > mTotalSlices)
            {
                // All slices done
                SetDead(true);
                return;
            }

            const s16 xpos = mSliceWidth * mCurrentSlice;
            const s16 halfDisplayWidth = gPsxDisplay.mWidth / 2;

            mScreenClipper->Update_Clip_Rect({static_cast<s16>(halfDisplayWidth - xpos), 0}, {static_cast<s16>(xpos + halfDisplayWidth + 1), gPsxDisplay.mHeight});
        }
        break;

        case CameraSwapEffects::eHorizontalSplit_7:
        {
            mCurrentSlice += mSlicesPerTick;
            if (mCurrentSlice < 0 || mCurrentSlice > mTotalSlices)
            {
                // All slices done
                SetDead(true);
                return;
            }

            const s16 ypos = mSliceWidth * mCurrentSlice;
            const s16 halfDisplayHeight = gPsxDisplay.mHeight / 2;

            mScreenClipper->Update_Clip_Rect({0, static_cast<s16>(halfDisplayHeight - ypos)}, {640, static_cast<s16>(halfDisplayHeight + ypos)});
        }
        break;

        case CameraSwapEffects::eBoxOut_8:
        {
            mCurrentSlice += mSlicesPerTick;
            if (mCurrentSlice < 0 || mCurrentSlice > mTotalSlices)
            {
                // All slices done
                SetDead(true);
                return;
            }

            const s16 xSlicePos = mCurrentSlice * mXSlices;
            const s16 width = (mXPosConverted + xSlicePos > gPsxDisplay.mWidth) ? gPsxDisplay.mWidth : mXPosConverted + xSlicePos;

            const s16 ySlicePos = mCurrentSlice * mYSlices;
            const s16 height = (ySlicePos + mYPosConverted > gPsxDisplay.mHeight) ? gPsxDisplay.mHeight : ySlicePos + mYPosConverted;

            PSX_Point rect_xy = {};
            rect_xy.x = (mXPosConverted - xSlicePos <= 0) ? 0 : mXPosConverted - xSlicePos;
            rect_xy.y = (mYPosConverted - ySlicePos <= 0) ? 0 : mYPosConverted - ySlicePos;

            PSX_Point rect_wh = {};
            rect_wh.x = width + 1;
            rect_wh.y = height;

            mScreenClipper->Update_Clip_Rect(rect_xy, rect_wh);
        }
        break;

        case CameraSwapEffects::ePlay1FMV_5:
        {
            if (gMovieRefCount)
            {
                // A movie is still playing
                return;
            }

            if (mMoviePutDispEnv)
            {
                //gPsxDisplay.PutCurrentDispEnv();
            }

            // Now apply the camera we where storing now that the movie is finished
            if (mCamRes.mData.mPixels)
            {
                gScreenManager->DecompressCameraToVRam(mCamRes);
            }

            SetDead(true);
        }
        break;

        // TODO: 2 and 3 FMV cases can be de-duped
        case CameraSwapEffects::ePlay2FMVs_9:
            if (mCurrentSlice < 1)
            {
                mCurrentSlice++;
            }

            // When no movie is playing start the next one
            if (gMovieRefCount == 0)
            {
                relive_new Movie(mFmvs[1]);
                mCamChangeEffect = CameraSwapEffects::ePlay1FMV_5;
                mMoviePutDispEnv = mPutDispEnv[1]; 
            }
            break;

        case CameraSwapEffects::ePlay3FMVs_10:
            if (mCurrentSlice < 1)
            {
                mCurrentSlice++;
            }

            // When no movie is playing start the next one
            if (gMovieRefCount == 0)
            {
                relive_new Movie(mFmvs[2]);
                mCamChangeEffect = CameraSwapEffects::ePlay2FMVs_9;
                mMoviePutDispEnv = mPutDispEnv[1]; // TODO another master branch bug
            }
            break;

        default:
            break;
    }
}

void CameraSwapper::VScreenChanged()
{
    // Empty
}
