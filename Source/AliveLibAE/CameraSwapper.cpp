#include "stdafx.h"
#include "CameraSwapper.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "BackgroundMusic.hpp"
#include "MusicController.hpp"
#include "AmbientSound.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "Sfx.hpp"
#include "Movie.hpp"
#include "ScreenClipper.hpp"
#include "Sys_common.hpp"
#include "BaseGameAutoPlayer.hpp"

CameraSwapper* CameraSwapper::ctor_4E4CA0(u8** ppCamRes, s32 movieSector, s32 movieId, s8 movieFlag, s16 movieFlags, s16 movieVol)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x5480E4); // vTbl_CameraSwapper_5480E4

    Init_4E50C0(ppCamRes, CameraSwapEffects::ePlay1FMV_5);

    PSX_ResetCallBack_4FAA20();

    Movie* pMovie = ae_new<Movie>();
    if (pMovie)
    {
        pMovie->ctor_4DFDE0(movieId, movieSector, movieFlag, movieFlags, movieVol);
    }

    field_4C_movie_bPutDispEnv = movieFlags;

    return this;
}

CameraSwapper* CameraSwapper::ctor_4E4DC0(u8** ppCamRes, s32 moviePos1, s32 movieId1, s32 moviePos2, s32 movieId2, s8 movieFlag1, s16 movieFlags1, s16 movieVol1, s16 movieFlag2, s16 movieFlags2, s16 movieVol2)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x5480E4); // vTbl_CameraSwapper_5480E4

    Init_4E50C0(ppCamRes, CameraSwapEffects::ePlay2FMVs_9);

    PSX_ResetCallBack_4FAA20();

    Movie* pMovie = ae_new<Movie>();
    if (pMovie)
    {
        pMovie->ctor_4DFDE0(movieId1, moviePos1, movieFlag1, movieFlags1, movieVol1);
    }

    field_24_movie_id_3 = movieId2;
    field_20_movie_pos_3 = moviePos2;
    field_44_movie_vol_3 = movieVol2;
    field_42_movie_flags_3 = movieFlags2;
    field_40_movie_flag_3 = movieFlag2;

    field_4C_movie_bPutDispEnv = movieFlags1;

    return this;
}

CameraSwapper* CameraSwapper::ctor_4E4ED0(u8** ppCamRes, s32 moviePos1, s32 movieId1, s32 moviePos2, s32 movieId2, s32 moviePos3, s32 movieId3, s8 movieFlag1, s16 movieFlags1, s16 movieVol1, s16 movieFlag2, s16 movieFlags2, s16 movieVol2, s16 moveFlag3, s16 movieFlags3, s16 movieVol3)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x5480E4); // vTbl_CameraSwapper_5480E4

    Init_4E50C0(ppCamRes, CameraSwapEffects::ePlay3FMVs_10);

    PSX_ResetCallBack_4FAA20();
    Movie* pMovie = ae_new<Movie>();
    if (pMovie)
    {
        pMovie->ctor_4DFDE0(movieId1, moviePos1, movieFlag1, movieFlags1, movieVol1);
    }

    field_2C_movie_id_2 = movieId2;
    field_28_movie_pos_2 = moviePos2;
    field_46_movie_flag_2 = movieFlag2;
    field_48_movie_flags_2 = movieFlags2;
    field_4A_movie_vol_2 = movieVol2;

    field_24_movie_id_3 = movieId3;
    field_20_movie_pos_3 = moviePos3;
    field_40_movie_flag_3 = moveFlag3;
    field_42_movie_flags_3 = movieFlags3;
    field_44_movie_vol_3 = movieVol3;

    field_4C_movie_bPutDispEnv = movieFlags1;

    return this;
}

CameraSwapper* CameraSwapper::ctor_4E5000(u8** ppCamRes, CameraSwapEffects changeEffect, s16 xpos, s16 ypos)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x5480E4); // vTbl_CameraSwapper_5480E4
    field_4E_xpos_converted = PsxToPCX(xpos);
    field_50_ypos_converted = ypos;
    Init_4E50C0(ppCamRes, changeEffect);
    return this;
}

void CameraSwapper::dtor_4E5790()
{
    SetVTable(this, 0x5480E4); // vTbl_CameraSwapper_5480E4

    sNum_CamSwappers_5C1B66--;

    if (field_34_pSubObject)
    {
        field_34_pSubObject->field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (sMap_bDoPurpleLightEffect_5C311C)
    {
        gMap_5C3030.RemoveObjectsWithPurpleLight_480740(0);
        sMap_bDoPurpleLightEffect_5C311C = 0;
    }

    BackgroundMusic::Play_4CB030();
    MusicController::EnableMusic_47FE10(1);
    Start_Sounds_For_Objects_In_Near_Cameras_4CBB60();
    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* CameraSwapper::vdtor_4E4D90(s32 flags)
{
    dtor_4E5790();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

const s32 kSliceWidth = 8;

void CameraSwapper::Init_4E50C0(u8** ppCamRes, CameraSwapEffects changeEffect)
{
    field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);

    SetType(AETypes::eCameraSwapper_149);

    field_34_pSubObject = nullptr;

    if (changeEffect == CameraSwapEffects::ePlay1FMV_5 || changeEffect == CameraSwapEffects::ePlay2FMVs_9 || changeEffect == CameraSwapEffects::ePlay3FMVs_10)
    {
        field_30_ppCamRes = ppCamRes;
    }
    else
    {
        pScreenManager_5BB5F4->DecompressCameraToVRam_40EF60(reinterpret_cast<u16**>(ppCamRes));
    }

    sNum_CamSwappers_5C1B66++;

    if (sNum_CamSwappers_5C1B66 != 1)
    {
        field_6_flags.Clear(BaseGameObject::eUpdatable_Bit2);
        field_6_flags.Set(BaseGameObject::eListAddFailed_Bit1);
        field_6_flags.Set(BaseGameObject::eDead_Bit3);

        // There can only be 1 active at a time
        return;
    }
    field_38_changeEffect = changeEffect;

    PSX_Point xy = {};
    PSX_Point wh = {};

    switch (changeEffect)
    {
        case CameraSwapEffects::eInstantChange_0:
            pScreenManager_5BB5F4->InvalidateRect_Layer3_40EDB0(0, 0, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height);
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_34_pSubObject = nullptr;
            return;

        case CameraSwapEffects::eLeftToRight_1:
            field_56_slice_width = gPsxDisplay_5C1130.field_0_width / kSliceWidth;
            field_3C_slices_per_tick = -1;
            field_3E_total_slices = gPsxDisplay_5C1130.field_0_width / field_56_slice_width;
            field_3A_current_slice = field_3E_total_slices;

            xy.field_2_y = 0;
            xy.field_0_x = gPsxDisplay_5C1130.field_0_width;

            wh.field_0_x = gPsxDisplay_5C1130.field_0_width;
            wh.field_2_y = gPsxDisplay_5C1130.field_2_height;

            pScreenManager_5BB5F4->field_44_unused = 1;

            field_34_pSubObject = ae_new<ScreenClipper>();
            field_34_pSubObject->ctor_416D60(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eRightToLeft_2:
            field_56_slice_width = gPsxDisplay_5C1130.field_0_width / kSliceWidth;
            field_3C_slices_per_tick = 1;
            field_3E_total_slices = gPsxDisplay_5C1130.field_0_width / field_56_slice_width;
            field_3A_current_slice = -1;

            xy.field_2_y = 0;
            xy.field_0_x = 0;

            wh.field_0_x = 0;
            wh.field_2_y = gPsxDisplay_5C1130.field_2_height;

            pScreenManager_5BB5F4->field_44_unused = 1;

            field_34_pSubObject = ae_new<ScreenClipper>();
            field_34_pSubObject->ctor_416D60(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eTopToBottom_3:
            field_56_slice_width = gPsxDisplay_5C1130.field_2_height / kSliceWidth;
            field_3C_slices_per_tick = -1;
            field_3E_total_slices = gPsxDisplay_5C1130.field_2_height / field_56_slice_width;
            field_3A_current_slice = field_3E_total_slices;

            xy.field_0_x = 0;
            xy.field_2_y = gPsxDisplay_5C1130.field_2_height;

            wh.field_0_x = gPsxDisplay_5C1130.field_0_width;
            wh.field_2_y = gPsxDisplay_5C1130.field_2_height;

            pScreenManager_5BB5F4->field_44_unused = 1;

            field_34_pSubObject = ae_new<ScreenClipper>();
            field_34_pSubObject->ctor_416D60(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eBottomToTop_4:
            field_56_slice_width = gPsxDisplay_5C1130.field_2_height / kSliceWidth;
            field_3C_slices_per_tick = 1;
            field_3E_total_slices = gPsxDisplay_5C1130.field_2_height / field_56_slice_width;
            field_3A_current_slice = -1;

            xy.field_0_x = 0;
            xy.field_2_y = 0;

            wh.field_0_x = gPsxDisplay_5C1130.field_0_width;
            wh.field_2_y = 0;

            pScreenManager_5BB5F4->field_44_unused = 1;

            field_34_pSubObject = ae_new<ScreenClipper>();
            field_34_pSubObject->ctor_416D60(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eVerticalSplit_6:
            field_56_slice_width = (gPsxDisplay_5C1130.field_0_width / 2) / kSliceWidth;
            field_3C_slices_per_tick = 1;
            field_3E_total_slices = (gPsxDisplay_5C1130.field_0_width / 2) / field_56_slice_width;
            field_3A_current_slice = 0;

            pScreenManager_5BB5F4->field_44_unused = 1;

            xy.field_0_x = gPsxDisplay_5C1130.field_0_width / 2;
            xy.field_2_y = 0;

            wh.field_0_x = gPsxDisplay_5C1130.field_0_width / 2;
            wh.field_2_y = gPsxDisplay_5C1130.field_2_height;

            field_34_pSubObject = ae_new<ScreenClipper>();
            field_34_pSubObject->ctor_416D60(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eHorizontalSplit_7:
            field_56_slice_width = (gPsxDisplay_5C1130.field_2_height / 2) / kSliceWidth;
            field_3C_slices_per_tick = 1;
            field_3E_total_slices = (gPsxDisplay_5C1130.field_2_height / 2) / field_56_slice_width;
            field_3A_current_slice = 0;

            pScreenManager_5BB5F4->field_44_unused = 1;

            xy.field_0_x = 0;
            xy.field_2_y = gPsxDisplay_5C1130.field_2_height / 2;

            wh.field_0_x = gPsxDisplay_5C1130.field_0_width;
            wh.field_2_y = gPsxDisplay_5C1130.field_2_height / 2;

            field_34_pSubObject = ae_new<ScreenClipper>();
            field_34_pSubObject->ctor_416D60(xy, wh, Layer::eLayer_0);
            break;

        case CameraSwapEffects::eBoxOut_8:
        {
            field_52_XSlices = (gPsxDisplay_5C1130.field_0_width / 2) / kSliceWidth;
            field_54_YSlices = (gPsxDisplay_5C1130.field_2_height / 2) / kSliceWidth;

            s16 xDiff = gPsxDisplay_5C1130.field_0_width - field_4E_xpos_converted;
            if (xDiff <= field_4E_xpos_converted)
            {
                xDiff = field_4E_xpos_converted;
            }

            s16 yDiff = gPsxDisplay_5C1130.field_2_height - field_50_ypos_converted;
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

            pScreenManager_5BB5F4->field_44_unused = 1;

            xy.field_0_x = gPsxDisplay_5C1130.field_0_width - 1;
            xy.field_2_y = gPsxDisplay_5C1130.field_2_height - 1;

            field_34_pSubObject = ae_new<ScreenClipper>();
            field_34_pSubObject->ctor_416D60(xy, {1, 1}, Layer::eLayer_0);

            // "Whoosh" door sound effect
            SFX_Play_46FA90(SoundEffect::IngameTransition_84, 127);
        }
        break;

        case CameraSwapEffects::ePlay1FMV_5:
        case CameraSwapEffects::ePlay2FMVs_9:
        case CameraSwapEffects::ePlay3FMVs_10:
            pScreenManager_5BB5F4->field_44_unused = 1;

            field_34_pSubObject = ae_new<ScreenClipper>();
            field_34_pSubObject->ctor_416D60({0, 0}, {1, 1}, Layer::eLayer_0);
            break;

        default:
            ALIVE_FATAL("Unknown camera swap effect"); // Or one that isn't handled here
    }
}

void CameraSwapper::VUpdate()
{
    vUpdate_4E5850();
}

void CameraSwapper::vUpdate_4E5850()
{
    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
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
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return;
            }

            const s16 xpos = field_56_slice_width * field_3A_current_slice;
            s16 width = (field_56_slice_width * (field_3A_current_slice + 1));
            if (width > gPsxDisplay_5C1130.field_0_width - 1)
            {
                width = gPsxDisplay_5C1130.field_0_width - 1;
            }

            pScreenManager_5BB5F4->InvalidateRect_Layer3_40EDB0(xpos, 0, width, gPsxDisplay_5C1130.field_2_height);
            field_34_pSubObject->Update_Clip_Rect_416EB0({xpos, 0}, {static_cast<s16>(width + 1), gPsxDisplay_5C1130.field_2_height});
        }
        break;

        case CameraSwapEffects::eTopToBottom_3:
        case CameraSwapEffects::eBottomToTop_4:
        {
            field_3A_current_slice += field_3C_slices_per_tick;
            if (field_3A_current_slice < 0 || field_3A_current_slice >= field_3E_total_slices)
            {
                // All slices done
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return;
            }

            const s16 ypos = field_56_slice_width * field_3A_current_slice;
            s16 height = (field_56_slice_width * (field_3A_current_slice + 1));

            pScreenManager_5BB5F4->InvalidateRect_Layer3_40EDB0(0, ypos, gPsxDisplay_5C1130.field_0_width, height);
            field_34_pSubObject->Update_Clip_Rect_416EB0({0, ypos}, {gPsxDisplay_5C1130.field_0_width, height});
        }
        break;

        case CameraSwapEffects::eVerticalSplit_6:
        {
            field_3A_current_slice += field_3C_slices_per_tick;
            if (field_3A_current_slice < 0 || field_3A_current_slice > field_3E_total_slices)
            {
                // All slices done
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return;
            }

            const s16 xpos = field_56_slice_width * field_3A_current_slice;
            const s16 halfDisplayWidth = gPsxDisplay_5C1130.field_0_width / 2;

            pScreenManager_5BB5F4->InvalidateRect_Layer3_40EDB0(static_cast<s16>(halfDisplayWidth - xpos), 0, static_cast<s16>(xpos + halfDisplayWidth), gPsxDisplay_5C1130.field_2_height);
            field_34_pSubObject->Update_Clip_Rect_416EB0({static_cast<s16>(halfDisplayWidth - xpos), 0}, {static_cast<s16>(xpos + halfDisplayWidth + 1), gPsxDisplay_5C1130.field_2_height});
        }
        break;

        case CameraSwapEffects::eHorizontalSplit_7:
        {
            field_3A_current_slice += field_3C_slices_per_tick;
            if (field_3A_current_slice < 0 || field_3A_current_slice > field_3E_total_slices)
            {
                // All slices done
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return;
            }

            const s16 ypos = field_56_slice_width * field_3A_current_slice;
            const s16 halfDisplayHeight = gPsxDisplay_5C1130.field_2_height / 2;

            pScreenManager_5BB5F4->InvalidateRect_Layer3_40EDB0(0, static_cast<s16>(halfDisplayHeight - ypos), 640, static_cast<s16>(halfDisplayHeight + ypos));
            field_34_pSubObject->Update_Clip_Rect_416EB0({0, static_cast<s16>(halfDisplayHeight - ypos)}, {640, static_cast<s16>(halfDisplayHeight + ypos)});
        }
        break;

        case CameraSwapEffects::eBoxOut_8:
        {
            field_3A_current_slice += field_3C_slices_per_tick;
            if (field_3A_current_slice < 0 || field_3A_current_slice > field_3E_total_slices)
            {
                // All slices done
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return;
            }

            const s16 xSlicePos = field_3A_current_slice * field_52_XSlices;
            const s16 width = (field_4E_xpos_converted + xSlicePos > gPsxDisplay_5C1130.field_0_width) ? gPsxDisplay_5C1130.field_0_width : field_4E_xpos_converted + xSlicePos;

            const s16 ySlicePos = field_3A_current_slice * field_54_YSlices;
            const s16 height = (ySlicePos + field_50_ypos_converted > gPsxDisplay_5C1130.field_2_height) ? gPsxDisplay_5C1130.field_2_height : ySlicePos + field_50_ypos_converted;

            PSX_Point rect_xy = {};
            rect_xy.field_0_x = (field_4E_xpos_converted - xSlicePos <= 0) ? 0 : field_4E_xpos_converted - xSlicePos;
            rect_xy.field_2_y = (field_50_ypos_converted - ySlicePos <= 0) ? 0 : field_50_ypos_converted - ySlicePos;

            pScreenManager_5BB5F4->InvalidateRect_Layer3_40EDB0(
                rect_xy.field_0_x,
                rect_xy.field_2_y,
                width,
                height);

            PSX_Point rect_wh = {};
            rect_wh.field_0_x = width + 1;
            rect_wh.field_2_y = height;

            field_34_pSubObject->Update_Clip_Rect_416EB0(rect_xy, rect_wh);
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
                if (!GetGameAutoPlayer().IsRecording() && !GetGameAutoPlayer().IsPlaying())
                {
                    // abi_break never calls this, so don't in recording to avoid de-sync
                    gPsxDisplay_5C1130.PutCurrentDispEnv_41DFA0();
                }
            }

            pScreenManager_5BB5F4->field_44_unused = 1;

            // Now apply the camera we where storing now that the movie is finished
            if (field_30_ppCamRes)
            {
                pScreenManager_5BB5F4->DecompressCameraToVRam_40EF60(reinterpret_cast<u16**>(field_30_ppCamRes));
                pScreenManager_5BB5F4->InvalidateRect_40EC90(0, 0, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height, 0);
                pScreenManager_5BB5F4->InvalidateRect_40EC90(0, 0, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height, 1);
                pScreenManager_5BB5F4->InvalidateRect_40EC90(0, 0, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height, 2);
            }

            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        break;

        // TODO: 2 and 3 FMV cases can be de-duped
        case CameraSwapEffects::ePlay2FMVs_9:
            if (field_3A_current_slice < 1)
            {
                field_3A_current_slice++;
            }
            else
            {
                pScreenManager_5BB5F4->field_40_flags &= ~1u;
            }

            // When no movie is playing start the next one
            if (sMovie_ref_count_BB4AE4 == 0)
            {
                auto pMovie = ae_new<Movie>();
                if (pMovie)
                {
                    pMovie->ctor_4DFDE0(
                        field_24_movie_id_3,
                        field_20_movie_pos_3,
                        field_40_movie_flag_3,
                        field_42_movie_flags_3,
                        field_44_movie_vol_3);
                }
                field_38_changeEffect = CameraSwapEffects::ePlay1FMV_5;
                field_4C_movie_bPutDispEnv = field_48_movie_flags_2;
            }
            break;

        case CameraSwapEffects::ePlay3FMVs_10:
            if (field_3A_current_slice < 1)
            {
                field_3A_current_slice++;
            }
            else
            {
                pScreenManager_5BB5F4->field_40_flags &= ~1u;
            }

            // When no movie is playing start the next one
            if (sMovie_ref_count_BB4AE4 == 0)
            {
                auto pMovie = ae_new<Movie>();
                if (pMovie)
                {
                    pMovie->ctor_4DFDE0(
                        field_2C_movie_id_2,
                        field_28_movie_pos_2,
                        field_46_movie_flag_2,
                        field_48_movie_flags_2,
                        field_4A_movie_vol_2);
                }
                field_38_changeEffect = CameraSwapEffects::ePlay2FMVs_9;
                field_4C_movie_bPutDispEnv = field_48_movie_flags_2;
            }
            break;

        default:
            break;
    }
}

BaseGameObject* CameraSwapper::VDestructor(s32 flags)
{
    return vdtor_4E4D90(flags);
}

void CameraSwapper::VScreenChanged()
{
}
