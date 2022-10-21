#include "stdafx_ao.h"
#include "CameraSwapper.hpp"
#include "Function.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Movie.hpp"
#include "Map.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Psx.hpp"
#include "ScreenClipper.hpp"
#include "MusicController.hpp"
#include "BackgroundMusic.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Sfx.hpp"
#include "../AliveLibCommon/FatalError.hpp"
#include "Sys_common.hpp"
#include "BaseGameAutoPlayer.hpp"

namespace AO {

s16 gNumCamSwappers = 0;

CameraSwapper::CameraSwapper(CamResource& ppBits, bool bPutDispEnv1, const char_type* pFmv1, bool bPutDispEnv2, const char_type* pFmv2, bool bPutDispEnv3, const char_type* pFmv3)
    : BaseGameObject(TRUE, 0)
{
    mFmvs[0] = pFmv1;
    mPutDispEnv[0] = bPutDispEnv1;

    mFmvs[1] = pFmv2;
    mPutDispEnv[1] = bPutDispEnv2;

    mFmvs[2] = pFmv3;
    mPutDispEnv[2] = bPutDispEnv3;

    if (mFmvs[0] && mFmvs[1] && mFmvs[2])
    {
        Init(ppBits, CameraSwapEffects::ePlay3FMVs_10);
    }
    else if (pFmv1 && pFmv2)
    {
        Init(ppBits, CameraSwapEffects::ePlay2FMVs_9);
    }
    else
    {
        Init(ppBits, CameraSwapEffects::ePlay1FMV_5);
    }

    relive_new Movie(mFmvs[0]);

    field_3C_movie_bPutDispEnv = mPutDispEnv[0];
}

CameraSwapper::CameraSwapper(CamResource& ppBits, CameraSwapEffects changeEffect, s32 xpos, s32 ypos)
    : BaseGameObject(TRUE, 0)
{
    field_40_ypos_converted = static_cast<s16>(ypos);
    field_3E_xpos_converted = static_cast<s16>(PsxToPCX(xpos));
    Init(ppBits, changeEffect);
}

CameraSwapper::~CameraSwapper()
{
    gNumCamSwappers--;

    if (field_24_pSubObject)
    {
        field_24_pSubObject->mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (sMap_bDoPurpleLightEffect_507C9C)
    {
        gMap.RemoveObjectsWithPurpleLight(0);
        sMap_bDoPurpleLightEffect_507C9C = FALSE;
    }

    BackgroundMusic::Play();
    MusicController::EnableMusic(1);
    gMap.Start_Sounds_For_Objects_In_Near_Cameras();
}

void CameraSwapper::Init(CamResource& ppCamRes, CameraSwapEffects changeEffect)
{
    mBaseGameObjectFlags.Set(Options::eUpdateDuringCamSwap_Bit10);

    SetType(ReliveTypes::eCameraSwapper);

    field_24_pSubObject = nullptr;

    if (changeEffect == CameraSwapEffects::ePlay1FMV_5 || changeEffect == CameraSwapEffects::ePlay2FMVs_9 || changeEffect == CameraSwapEffects::ePlay3FMVs_10)
    {
        field_20_ppCamRes = ppCamRes;
    }
    else
    {
        pScreenManager->DecompressCameraToVRam(ppCamRes);
    }

    gNumCamSwappers++;

    if (gNumCamSwappers != 1)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eUpdatable_Bit2);
        mBaseGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);

        // There can only be 1 active at a time
        return;
    }

    field_28_changeEffect = changeEffect;

    PSX_Point xy = {};
    PSX_Point wh = {};

    switch (field_28_changeEffect)
    {
        case CameraSwapEffects::eInstantChange_0:
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            field_24_pSubObject = nullptr;
            break;

        case CameraSwapEffects::eLeftToRight_1:
            field_46_slice_width = 48; // 384 / 8 ?
            field_2C_slices_per_tick = -1;
            field_2E_total_slices = gPsxDisplay.mWidth / field_46_slice_width;
            field_2A_current_slice = field_2E_total_slices;

            xy.y = 0;
            xy.x = gPsxDisplay.mWidth;

            wh.x = gPsxDisplay.mWidth;
            wh.y = gPsxDisplay.mHeight;

            field_24_pSubObject = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eRightToLeft_2:
            field_46_slice_width = 48;
            field_2C_slices_per_tick = 1;
            field_2E_total_slices = gPsxDisplay.mWidth / field_46_slice_width + 1;
            field_2A_current_slice = -1;

            xy.y = 0;
            xy.x = 0;

            wh.x = 0;
            wh.y = gPsxDisplay.mHeight;

            field_24_pSubObject = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eTopToBottom_3:
            field_46_slice_width = 30; // 240 / 8 ?
            field_2C_slices_per_tick = -1;
            field_2E_total_slices = gPsxDisplay.mHeight / field_46_slice_width;
            field_2A_current_slice = field_2E_total_slices;

            xy.x = 0;
            xy.y = gPsxDisplay.mHeight;

            wh.x = gPsxDisplay.mWidth;
            wh.y = gPsxDisplay.mHeight;

            field_24_pSubObject = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eBottomToTop_4:
            field_46_slice_width = 30; // 240 / 8 ?
            field_2C_slices_per_tick = 1;
            field_2E_total_slices = gPsxDisplay.mHeight / field_46_slice_width;
            field_2A_current_slice = -1;

            xy.x = 0;
            xy.y = 0;

            wh.x = gPsxDisplay.mWidth;
            wh.y = 0;

            field_24_pSubObject = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eVerticalSplit_6:
            field_46_slice_width = 24; // ??
            field_2E_total_slices = (gPsxDisplay.mWidth / 2) / field_46_slice_width;
            field_2C_slices_per_tick = 1;
            field_2A_current_slice = 0;

            xy.x = gPsxDisplay.mWidth / 2;
            xy.y = 0;

            wh.x = gPsxDisplay.mWidth / 2;
            wh.y = gPsxDisplay.mHeight;

            field_24_pSubObject = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eHorizontalSplit_7:
            field_46_slice_width = 15; // ??
            field_2C_slices_per_tick = 1;
            field_2E_total_slices = (gPsxDisplay.mHeight / 2) / field_46_slice_width;
            field_2A_current_slice = 0;

            xy.x = 0;
            xy.y = gPsxDisplay.mHeight / 2;

            wh.x = gPsxDisplay.mWidth;
            wh.y = gPsxDisplay.mHeight / 2;

            field_24_pSubObject = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eBoxOut_8:
        {
            field_42_XSlices = 24; // ??
            field_44_YSlices = 15; // ??

            s16 xDiff = gPsxDisplay.mWidth - field_3E_xpos_converted;
            if (xDiff <= field_3E_xpos_converted)
            {
                xDiff = field_3E_xpos_converted;
            }

            s16 yDiff = gPsxDisplay.mHeight - field_40_ypos_converted;
            if (yDiff <= field_40_ypos_converted)
            {
                yDiff = field_40_ypos_converted;
            }

            s16 startingSlice = 0;
            if ((xDiff / field_42_XSlices) <= (yDiff / field_44_YSlices))
            {
                startingSlice = yDiff / field_44_YSlices;
            }
            else
            {
                startingSlice = xDiff / field_42_XSlices;
            }

            field_2C_slices_per_tick = 1;
            field_2E_total_slices = startingSlice + 1;
            field_2A_current_slice = 0;

            xy.x = gPsxDisplay.mWidth - 1;
            xy.y = gPsxDisplay.mHeight - 1;

            field_24_pSubObject = relive_new ScreenClipper(xy, PSX_Point{ 1, 1 }, Layer::eLayer_0);

            // "Whoosh" door sound effect
            SfxPlayMono(relive::SoundEffects::IngameTransition, 127);
        }
        break;

        case CameraSwapEffects::ePlay1FMV_5:
        case CameraSwapEffects::ePlay2FMVs_9:
        case CameraSwapEffects::ePlay3FMVs_10:
            field_24_pSubObject = relive_new ScreenClipper(PSX_Point{ 0, 0 }, PSX_Point{ 1, 1 }, Layer::eLayer_0);
            field_2A_current_slice = 0;
            break;

        default:
            ALIVE_FATAL("Unknown camera swap effect"); // Or one that isn't handled here
    }
}

void CameraSwapper::VUpdate()
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return;
    }

    switch (field_28_changeEffect)
    {
        case CameraSwapEffects::eInstantChange_0:
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            return;

        case CameraSwapEffects::eLeftToRight_1:
        case CameraSwapEffects::eRightToLeft_2:
        {
            field_2A_current_slice += field_2C_slices_per_tick;
            if (field_2A_current_slice < 0 || field_2A_current_slice >= field_2E_total_slices)
            {
                // All slices done
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            const s16 xpos = field_46_slice_width * field_2A_current_slice;
            s16 width = field_46_slice_width * (field_2A_current_slice + 1);
            if (width > gPsxDisplay.mWidth - 1)
            {
                width = gPsxDisplay.mWidth - 1;
            }

            field_24_pSubObject->Update_Clip_Rect({xpos, 0}, {static_cast<s16>(width + 1), static_cast<s16>(gPsxDisplay.mHeight)});
        }
        break;

        case CameraSwapEffects::eTopToBottom_3:
        case CameraSwapEffects::eBottomToTop_4:
        {
            field_2A_current_slice += field_2C_slices_per_tick;
            if (field_2A_current_slice < 0 || field_2A_current_slice >= field_2E_total_slices)
            {
                // All slices done
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            const s16 ypos = field_46_slice_width * field_2A_current_slice;
            s16 height = field_46_slice_width * (field_2A_current_slice + 1);
            field_24_pSubObject->Update_Clip_Rect({0, ypos}, {static_cast<s16>(gPsxDisplay.mWidth), height});
        }
        break;

        case CameraSwapEffects::eVerticalSplit_6:
        {
            field_2A_current_slice += field_2C_slices_per_tick;
            if (field_2A_current_slice < 0 || field_2A_current_slice > field_2E_total_slices)
            {
                // All slices done
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            const s16 xpos = field_46_slice_width * field_2A_current_slice;
            const s16 halfDisplayWidth = gPsxDisplay.mWidth / 2;

            field_24_pSubObject->Update_Clip_Rect({static_cast<s16>(halfDisplayWidth - xpos), 0}, {static_cast<s16>(xpos + halfDisplayWidth + 1), static_cast<s16>(gPsxDisplay.mHeight)});
        }
        break;

        case CameraSwapEffects::eHorizontalSplit_7:
        {
            field_2A_current_slice += field_2C_slices_per_tick;
            if (field_2A_current_slice < 0 || field_2A_current_slice > field_2E_total_slices)
            {
                // All slices done
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            const s16 ypos = field_46_slice_width * field_2A_current_slice;
            const s16 halfDisplayHeight = gPsxDisplay.mHeight / 2;

            field_24_pSubObject->Update_Clip_Rect({0, static_cast<s16>(halfDisplayHeight - ypos)}, {640, static_cast<s16>(halfDisplayHeight + ypos)});
        }
        break;

        case CameraSwapEffects::eBoxOut_8:
        {
            field_2A_current_slice += field_2C_slices_per_tick;
            if (field_2A_current_slice < 0 || field_2A_current_slice > field_2E_total_slices)
            {
                // All slices done
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            const s16 xSlicePos = field_2A_current_slice * field_42_XSlices;
            const s16 width = (field_3E_xpos_converted + xSlicePos > gPsxDisplay.mWidth) ? gPsxDisplay.mWidth : field_3E_xpos_converted + xSlicePos;

            const s16 ySlicePos = field_2A_current_slice * field_44_YSlices;
            const s16 height = (ySlicePos + field_40_ypos_converted > gPsxDisplay.mHeight) ? gPsxDisplay.mHeight : ySlicePos + field_40_ypos_converted;

            PSX_Point rect_xy = {};
            rect_xy.x = (field_3E_xpos_converted - xSlicePos <= 0) ? 0 : field_3E_xpos_converted - xSlicePos;
            rect_xy.y = (field_40_ypos_converted - ySlicePos <= 0) ? 0 : field_40_ypos_converted - ySlicePos;

            PSX_Point rect_wh = {};
            rect_wh.x = width + 1;
            rect_wh.y = height;

            field_24_pSubObject->Update_Clip_Rect(rect_xy, rect_wh);
        }
        break;

        case CameraSwapEffects::ePlay1FMV_5:
        {
            if (sMovie_ref_count_9F309C)
            {
                // A movie is still playing
                return;
            }

            if (field_3C_movie_bPutDispEnv == 1)
            {
                //gPsxDisplay.PutCurrentDispEnv();
            }

            // Now apply the camera we where storing now that the movie is finished
            if (field_20_ppCamRes.mData.mPixels)
            {
                pScreenManager->DecompressCameraToVRam(field_20_ppCamRes);
                pScreenManager->EnableRendering();
            }

            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
        break;

        // TODO: 2 and 3 FMV cases can be de-duped
        case CameraSwapEffects::ePlay2FMVs_9:
            if (field_2A_current_slice < 1)
            {
                field_2A_current_slice++;
            }
            else
            {
                pScreenManager->DisableRendering();
            }

            // When no movie is playing start the next one
            if (sMovie_ref_count_9F309C == 0)
            {
                relive_new Movie(mFmvs[1]);
                field_28_changeEffect = CameraSwapEffects::ePlay1FMV_5;
                field_3C_movie_bPutDispEnv = mPutDispEnv[2];
            }
            break;

        case CameraSwapEffects::ePlay3FMVs_10:
            if (field_2A_current_slice < 1)
            {
                field_2A_current_slice++;
            }
            else
            {
                pScreenManager->DisableRendering();
            }

            // When no movie is playing start the next one
            if (sMovie_ref_count_9F309C == 0)
            {
                relive_new Movie(mFmvs[2]);
                field_28_changeEffect = CameraSwapEffects::ePlay2FMVs_9;
                field_3C_movie_bPutDispEnv = mPutDispEnv[1];
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

} // namespace AO
