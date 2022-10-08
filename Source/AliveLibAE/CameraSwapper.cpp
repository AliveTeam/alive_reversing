#include "stdafx.h"
#include "CameraSwapper.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "BackgroundMusic.hpp"
#include "MusicController.hpp"
#include "AmbientSound.hpp"
#include "stdlib.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Sfx.hpp"
#include "Movie.hpp"
#include "ScreenClipper.hpp"

CameraSwapper::CameraSwapper(CamResource& ppCamRes, s32 movieSector, s32 movieId, s32 movieFlag, s32 movieFlags, s32 movieVol)
    : BaseGameObject(TRUE, 0)
{
    Init(ppCamRes, CameraSwapEffects::ePlay1FMV_5);

    PSX_ResetCallBack_4FAA20();

    relive_new Movie(movieId, movieSector, movieFlag, movieFlags, movieVol);

    field_4C_movie_bPutDispEnv = static_cast<s16>(movieFlags);
}

CameraSwapper::CameraSwapper(CamResource& ppCamRes, s32 moviePos1, s32 movieId1, s32 moviePos2, s32 movieId2, s32 movieFlag1, s32 movieFlags1, s32 movieVol1, s32 movieFlag2, s32 movieFlags2, s32 movieVol2)
    : BaseGameObject(TRUE, 0)
{
    Init(ppCamRes, CameraSwapEffects::ePlay2FMVs_9);

    PSX_ResetCallBack_4FAA20();

    relive_new Movie(movieId1, moviePos1, movieFlag1, movieFlags1, movieVol1);

    field_24_movie_id_3 = movieId2;
    field_20_movie_pos_3 = moviePos2;
    field_44_movie_vol_3 = static_cast<s16>(movieVol2);
    field_42_movie_flags_3 = static_cast<s16>(movieFlags2);
    field_40_movie_flag_3 = static_cast<s16>(movieFlag2);

    field_4C_movie_bPutDispEnv = static_cast<s16>(movieFlags1);
}

CameraSwapper::CameraSwapper(CamResource& ppCamRes, s32 moviePos1, s32 movieId1, s32 moviePos2, s32 movieId2, s32 moviePos3, s32 movieId3, s32 movieFlag1, s32 movieFlags1, s32 movieVol1, s32 movieFlag2, s32 movieFlags2, s32 movieVol2, s32 moveFlag3, s32 movieFlags3, s32 movieVol3)
    : BaseGameObject(TRUE, 0)
{
    Init(ppCamRes, CameraSwapEffects::ePlay3FMVs_10);

    PSX_ResetCallBack_4FAA20();
    relive_new Movie(movieId1, moviePos1, movieFlag1, movieFlags1, movieVol1);

    field_2C_movie_id_2 = movieId2;
    field_28_movie_pos_2 = moviePos2;
    field_46_movie_flag_2 = static_cast<s16>(movieFlag2);
    field_48_movie_flags_2 = static_cast<s16>(movieFlags2);
    field_4A_movie_vol_2 = static_cast<s16>(movieVol2);

    field_24_movie_id_3 = movieId3;
    field_20_movie_pos_3 = moviePos3;
    field_40_movie_flag_3 = static_cast<s16>(moveFlag3);
    field_42_movie_flags_3 = static_cast<s16>(movieFlags3);
    field_44_movie_vol_3 = static_cast<s16>(movieVol3);

    field_4C_movie_bPutDispEnv = static_cast<s16>(movieFlags1);
}

CameraSwapper::CameraSwapper(CamResource& ppCamRes, CameraSwapEffects changeEffect, s32 xpos, s32 ypos)
    : BaseGameObject(TRUE, 0)
{
    field_4E_xpos_converted = static_cast<s16>(PsxToPCX(xpos));
    field_50_ypos_converted = static_cast<s16>(ypos);
    Init(ppCamRes, changeEffect);
}

CameraSwapper::~CameraSwapper()
{
    sNum_CamSwappers_5C1B66--;

    if (field_34_pSubObject)
    {
        field_34_pSubObject->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (sMap_bDoPurpleLightEffect_5C311C)
    {
        gMap.RemoveObjectsWithPurpleLight(0);
        sMap_bDoPurpleLightEffect_5C311C = 0;
    }

    BackgroundMusic::Play();
    MusicController::static_EnableMusic(1);
    Start_Sounds_For_Objects_In_Near_Cameras();
}

const s32 kSliceWidth = 8;

void CameraSwapper::Init(CamResource& ppCamRes, CameraSwapEffects changeEffect)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);

    SetType(ReliveTypes::eCameraSwapper);

    field_34_pSubObject = nullptr;

    if (changeEffect == CameraSwapEffects::ePlay1FMV_5 || changeEffect == CameraSwapEffects::ePlay2FMVs_9 || changeEffect == CameraSwapEffects::ePlay3FMVs_10)
    {
        field_30_ppCamRes = ppCamRes;
    }
    else
    {
        pScreenManager->DecompressCameraToVRam(ppCamRes);
    }

    sNum_CamSwappers_5C1B66++;

    if (sNum_CamSwappers_5C1B66 != 1)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eUpdatable_Bit2);
        mBaseGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);

        // There can only be 1 active at a time
        return;
    }
    field_38_changeEffect = changeEffect;

    PSX_Point xy = {};
    PSX_Point wh = {};

    switch (changeEffect)
    {
        case CameraSwapEffects::eInstantChange_0:
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            field_34_pSubObject = nullptr;
            return;

        case CameraSwapEffects::eLeftToRight_1:
            field_56_slice_width = gPsxDisplay.mWidth / kSliceWidth;
            field_3C_slices_per_tick = -1;
            field_3E_total_slices = gPsxDisplay.mWidth / field_56_slice_width;
            field_3A_current_slice = field_3E_total_slices;

            xy.y = 0;
            xy.x = gPsxDisplay.mWidth;

            wh.x = gPsxDisplay.mWidth;
            wh.y = gPsxDisplay.mHeight;

            field_34_pSubObject = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eRightToLeft_2:
            field_56_slice_width = gPsxDisplay.mWidth / kSliceWidth;
            field_3C_slices_per_tick = 1;
            field_3E_total_slices = gPsxDisplay.mWidth / field_56_slice_width;
            field_3A_current_slice = -1;

            xy.y = 0;
            xy.x = 0;

            wh.x = 0;
            wh.y = gPsxDisplay.mHeight;

            field_34_pSubObject = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eTopToBottom_3:
            field_56_slice_width = gPsxDisplay.mHeight / kSliceWidth;
            field_3C_slices_per_tick = -1;
            field_3E_total_slices = gPsxDisplay.mHeight / field_56_slice_width;
            field_3A_current_slice = field_3E_total_slices;

            xy.x = 0;
            xy.y = gPsxDisplay.mHeight;

            wh.x = gPsxDisplay.mWidth;
            wh.y = gPsxDisplay.mHeight;

            field_34_pSubObject = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eBottomToTop_4:
            field_56_slice_width = gPsxDisplay.mHeight / kSliceWidth;
            field_3C_slices_per_tick = 1;
            field_3E_total_slices = gPsxDisplay.mHeight / field_56_slice_width;
            field_3A_current_slice = -1;

            xy.x = 0;
            xy.y = 0;

            wh.x = gPsxDisplay.mWidth;
            wh.y = 0;

            field_34_pSubObject = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eVerticalSplit_6:
            field_56_slice_width = (gPsxDisplay.mWidth / 2) / kSliceWidth;
            field_3C_slices_per_tick = 1;
            field_3E_total_slices = (gPsxDisplay.mWidth / 2) / field_56_slice_width;
            field_3A_current_slice = 0;

            xy.x = gPsxDisplay.mWidth / 2;
            xy.y = 0;

            wh.x = gPsxDisplay.mWidth / 2;
            wh.y = gPsxDisplay.mHeight;

            field_34_pSubObject = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eHorizontalSplit_7:
            field_56_slice_width = (gPsxDisplay.mHeight / 2) / kSliceWidth;
            field_3C_slices_per_tick = 1;
            field_3E_total_slices = (gPsxDisplay.mHeight / 2) / field_56_slice_width;
            field_3A_current_slice = 0;

            xy.x = 0;
            xy.y = gPsxDisplay.mHeight / 2;

            wh.x = gPsxDisplay.mWidth;
            wh.y = gPsxDisplay.mHeight / 2;

            field_34_pSubObject = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eBoxOut_8:
        {
            field_52_XSlices = (gPsxDisplay.mWidth / 2) / kSliceWidth;
            field_54_YSlices = (gPsxDisplay.mHeight / 2) / kSliceWidth;

            s16 xDiff = gPsxDisplay.mWidth - field_4E_xpos_converted;
            if (xDiff <= field_4E_xpos_converted)
            {
                xDiff = field_4E_xpos_converted;
            }

            s16 yDiff = gPsxDisplay.mHeight - field_50_ypos_converted;
            if (yDiff <= field_50_ypos_converted)
            {
                yDiff = field_50_ypos_converted;
            }

            s16 startingSlice = 0;
            if ((xDiff / field_52_XSlices) <= (yDiff / field_54_YSlices))
            {
                startingSlice = yDiff / field_54_YSlices;
            }
            else
            {
                startingSlice = xDiff / field_52_XSlices;
            }

            field_3C_slices_per_tick = 1;
            field_3E_total_slices = startingSlice + 1;
            field_3A_current_slice = 0;

            xy.x = gPsxDisplay.mWidth - 1;
            xy.y = gPsxDisplay.mHeight - 1;

            field_34_pSubObject = relive_new ScreenClipper(xy, PSX_Point{1, 1}, Layer::eLayer_0);

            // "Whoosh" door sound effect
            SfxPlayMono(relive::SoundEffects::IngameTransition, 127);
        }
        break;

        case CameraSwapEffects::ePlay1FMV_5:
        case CameraSwapEffects::ePlay2FMVs_9:
        case CameraSwapEffects::ePlay3FMVs_10:
            field_34_pSubObject = relive_new ScreenClipper(PSX_Point{0, 0}, PSX_Point{1, 1}, Layer::eLayer_0);
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

    switch (field_38_changeEffect)
    {
        case CameraSwapEffects::eLeftToRight_1:
        case CameraSwapEffects::eRightToLeft_2:
        {
            field_3A_current_slice += field_3C_slices_per_tick;
            if (field_3A_current_slice < 0 || field_3A_current_slice >= field_3E_total_slices)
            {
                // All slices done
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            const s16 xpos = field_56_slice_width * field_3A_current_slice;
            s16 width = (field_56_slice_width * (field_3A_current_slice + 1));
            if (width > gPsxDisplay.mWidth - 1)
            {
                width = gPsxDisplay.mWidth - 1;
            }

            field_34_pSubObject->Update_Clip_Rect({xpos, 0}, {static_cast<s16>(width + 1), gPsxDisplay.mHeight});
        }
        break;

        case CameraSwapEffects::eTopToBottom_3:
        case CameraSwapEffects::eBottomToTop_4:
        {
            field_3A_current_slice += field_3C_slices_per_tick;
            if (field_3A_current_slice < 0 || field_3A_current_slice >= field_3E_total_slices)
            {
                // All slices done
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            const s16 ypos = field_56_slice_width * field_3A_current_slice;
            s16 height = (field_56_slice_width * (field_3A_current_slice + 1));

            field_34_pSubObject->Update_Clip_Rect({0, ypos}, {gPsxDisplay.mWidth, height});
        }
        break;

        case CameraSwapEffects::eVerticalSplit_6:
        {
            field_3A_current_slice += field_3C_slices_per_tick;
            if (field_3A_current_slice < 0 || field_3A_current_slice > field_3E_total_slices)
            {
                // All slices done
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            const s16 xpos = field_56_slice_width * field_3A_current_slice;
            const s16 halfDisplayWidth = gPsxDisplay.mWidth / 2;

            field_34_pSubObject->Update_Clip_Rect({static_cast<s16>(halfDisplayWidth - xpos), 0}, {static_cast<s16>(xpos + halfDisplayWidth + 1), gPsxDisplay.mHeight});
        }
        break;

        case CameraSwapEffects::eHorizontalSplit_7:
        {
            field_3A_current_slice += field_3C_slices_per_tick;
            if (field_3A_current_slice < 0 || field_3A_current_slice > field_3E_total_slices)
            {
                // All slices done
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            const s16 ypos = field_56_slice_width * field_3A_current_slice;
            const s16 halfDisplayHeight = gPsxDisplay.mHeight / 2;

            field_34_pSubObject->Update_Clip_Rect({0, static_cast<s16>(halfDisplayHeight - ypos)}, {640, static_cast<s16>(halfDisplayHeight + ypos)});
        }
        break;

        case CameraSwapEffects::eBoxOut_8:
        {
            field_3A_current_slice += field_3C_slices_per_tick;
            if (field_3A_current_slice < 0 || field_3A_current_slice > field_3E_total_slices)
            {
                // All slices done
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            const s16 xSlicePos = field_3A_current_slice * field_52_XSlices;
            const s16 width = (field_4E_xpos_converted + xSlicePos > gPsxDisplay.mWidth) ? gPsxDisplay.mWidth : field_4E_xpos_converted + xSlicePos;

            const s16 ySlicePos = field_3A_current_slice * field_54_YSlices;
            const s16 height = (ySlicePos + field_50_ypos_converted > gPsxDisplay.mHeight) ? gPsxDisplay.mHeight : ySlicePos + field_50_ypos_converted;

            PSX_Point rect_xy = {};
            rect_xy.x = (field_4E_xpos_converted - xSlicePos <= 0) ? 0 : field_4E_xpos_converted - xSlicePos;
            rect_xy.y = (field_50_ypos_converted - ySlicePos <= 0) ? 0 : field_50_ypos_converted - ySlicePos;

            PSX_Point rect_wh = {};
            rect_wh.x = width + 1;
            rect_wh.y = height;

            field_34_pSubObject->Update_Clip_Rect(rect_xy, rect_wh);
        }
        break;

        case CameraSwapEffects::ePlay1FMV_5:
        {
            if (sMovie_ref_count_BB4AE4)
            {
                // A movie is still playing
                return;
            }

            if (field_4C_movie_bPutDispEnv == 1)
            {
                gPsxDisplay.PutCurrentDispEnv();
            }

            // Now apply the camera we where storing now that the movie is finished
            if (field_30_ppCamRes.mData.mPixels)
            {
                pScreenManager->DecompressCameraToVRam(field_30_ppCamRes);
            }

            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
        break;

        // TODO: 2 and 3 FMV cases can be de-duped
        case CameraSwapEffects::ePlay2FMVs_9:
            if (field_3A_current_slice < 1)
            {
                field_3A_current_slice++;
            }

            // When no movie is playing start the next one
            if (sMovie_ref_count_BB4AE4 == 0)
            {
                relive_new Movie(field_24_movie_id_3,
                                            field_20_movie_pos_3,
                                            field_40_movie_flag_3,
                                            field_42_movie_flags_3,
                                            field_44_movie_vol_3);
                field_38_changeEffect = CameraSwapEffects::ePlay1FMV_5;
                field_4C_movie_bPutDispEnv = field_48_movie_flags_2;
            }
            break;

        case CameraSwapEffects::ePlay3FMVs_10:
            if (field_3A_current_slice < 1)
            {
                field_3A_current_slice++;
            }

            // When no movie is playing start the next one
            if (sMovie_ref_count_BB4AE4 == 0)
            {
                relive_new Movie(field_2C_movie_id_2,
                                            field_28_movie_pos_2,
                                            field_46_movie_flag_2,
                                            field_48_movie_flags_2,
                                            field_4A_movie_vol_2);
                field_38_changeEffect = CameraSwapEffects::ePlay2FMVs_9;
                field_4C_movie_bPutDispEnv = field_48_movie_flags_2;
            }
            break;

        default:
            break;
    }
}

void CameraSwapper::VScreenChanged()
{
}
