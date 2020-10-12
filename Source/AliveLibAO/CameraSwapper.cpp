#include "stdafx_ao.h"
#include "CameraSwapper.hpp"
#include "Function.hpp"
#include "PsxDisplay.hpp"
#include "Movie.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Psx.hpp"
#include "ScreenClipper.hpp"
#include "MusicController.hpp"
#include "BackgroundMusic.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "Sfx.hpp"
#include "Sys_common.hpp"

START_NS_AO

ALIVE_VAR(1, 0x507668, short, sNumCamSwappers_507668, 0);

CameraSwapper* CameraSwapper::ctor_48C540(BYTE** ppBits, int movieId, int movieFlag, char movieFlags, __int16 flags, __int16 volume)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BD7C8);
    
    Init_48C830(ppBits, CameraSwapEffects::eEffect5_1_FMV);
    
    PSX_ResetCallBack_49AFB0();

    auto pMovie = ao_new<Movie>();
    if (pMovie)
    {
        pMovie->ctor_489C90(movieFlag, movieId, movieFlags, flags, volume);
    }

    field_3C_movie_bPutDispEnv = flags;
    return this;
}

CameraSwapper* CameraSwapper::ctor_48C5E0(BYTE** ppBits, int moviePos1, int movieId1, int moviePos2, int movieFlag1, char movieFlags1, __int16 movieVol1, __int16 movieFlag2, __int16 movieFlag2_1, __int16 movieFlags2_1, __int16 movieVol2)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BD7C8);

    Init_48C830(ppBits, CameraSwapEffects::eEffect9_2_FMV);
    
    PSX_ResetCallBack_49AFB0();

    auto pMovie = ao_new<Movie>();
    if (pMovie)
    {
        pMovie->ctor_489C90(movieId1, moviePos1, movieFlags1, movieVol1, movieFlag2);
    }

    field_14_movie_id_3 = movieFlag1;
    field_10_movie_pos_3 = moviePos2;
    field_34_movie_vol_3 = movieVol2;
    field_32_movie_flags_3 = movieFlags2_1;
    field_30_movie_flag_3 = movieFlag2_1;

    field_3C_movie_bPutDispEnv = movieVol1;

    return this;
}

EXPORT CameraSwapper* CameraSwapper::ctor_48C6B0(BYTE** ppBits, int moviePos1, int movieIds1, int moviePos2, int movieId2, int moviePos3, int movieId3, char movieFlag1, __int16 movieFlags1, __int16 movieVol1, __int16 movieFlag2, __int16 movieFlags2, __int16 movieVol2, __int16 movieFlag3, __int16 movieFlags3, __int16 movieVol3)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BD7C8);

    Init_48C830(ppBits, CameraSwapEffects::eEffect10_3_FMV);

    PSX_ResetCallBack_49AFB0();
    auto pMovie = ao_new<Movie>();
    if (pMovie)
    {
        pMovie->ctor_489C90(movieIds1, moviePos1, movieFlag1, movieFlags1, movieVol1);
    }

    field_18_movie_pos_2 = moviePos2;
    field_1C_movie_id_2 = movieId2;
    field_38_movie_flags_2 = movieFlags2;
    field_36_movie_flag_2 = movieFlag2;
    field_3A_movie_vol_2 = movieVol2;

    field_14_movie_id_3 = movieId3;
    field_10_movie_pos_3 = moviePos3;
    field_30_movie_flag_3 = movieFlag3;
    field_34_movie_vol_3 = movieVol3;
    field_32_movie_flags_3 = movieFlags3;

    field_3C_movie_bPutDispEnv = movieFlags1;

    return this;
}

BaseGameObject* CameraSwapper::VDestructor(signed int flags)
{
    return Vdtor_48D380(flags);
}


void CameraSwapper::VScreenChanged()
{
    // Empty
}

CameraSwapper* CameraSwapper::Vdtor_48D380(signed int flags)
{
    dtor_48CE00();

    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

CameraSwapper* CameraSwapper::ctor_48C7A0(BYTE** ppBits, CameraSwapEffects changeEffect, __int16 xpos, __int16 ypos)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BD7C8);

    field_40_ypos_converted = ypos;
    field_3E_xpos_converted = PsxToPCX(xpos);
    Init_48C830(ppBits, changeEffect);

    return this;
}

BaseGameObject* CameraSwapper::dtor_48CE00()
{
    SetVTable(this, 0x4BD7C8);

    sNumCamSwappers_507668--;

    if (field_24_pSubObject)
    {
        field_24_pSubObject->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (sMap_bDoPurpleLightEffect_507C9C)
    {
        gMap_507BA8.RemoveObjectsWithPurpleLight_4440D0(0);
        sMap_bDoPurpleLightEffect_507C9C = FALSE;
    }

    BackgroundMusic::Play_4762B0();
    MusicController::EnableMusic_443900(1);
    gMap_507BA8.Start_Sounds_For_Objects_In_Near_Cameras_4467D0();

    return dtor_487DF0();
}

void CameraSwapper::VUpdate()
{
    VUpdate_48CEA0();
}

void CameraSwapper::Init_48C830(BYTE** ppCamRes, CameraSwapEffects changeEffect)
{
    field_6_flags.Set(Options::eUpdateDuringCamSwap_Bit10);

    field_4_typeId = Types::eCameraSwapper_102;

    field_24_pSubObject = nullptr;

    if (changeEffect == CameraSwapEffects::eEffect5_1_FMV || changeEffect == CameraSwapEffects::eEffect9_2_FMV || changeEffect == CameraSwapEffects::eEffect10_3_FMV)
    {
        field_20_ppCamRes = ppCamRes;
    }
    else
    {
        pScreenManager_4FF7C8->DecompressToVRam_407110(reinterpret_cast<WORD**>(ppCamRes));
    }

    sNumCamSwappers_507668++;

    if (sNumCamSwappers_507668 != 1)
    {
        field_6_flags.Clear(BaseGameObject::eUpdatable_Bit2);
        field_6_flags.Set(BaseGameObject::eListAddFailed_Bit1);
        field_6_flags.Set(BaseGameObject::eDead_Bit3);

        // There can only be 1 active at a time
        return;
    }

    field_28_changeEffect = changeEffect;

    PSX_Point xy = {};
    PSX_Point wh = {};

    switch (field_28_changeEffect)
    {
    case CameraSwapEffects::eEffect0_InstantChange:
        pScreenManager_4FF7C8->InvalidateRect_Layer3_406F20(0, 0, 640, 240);
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_24_pSubObject = nullptr;
        break;

    case CameraSwapEffects::eEffect1_LeftToRight:
        field_46_slice_width = 48; // 384 / 8 ?
        field_2C_slices_per_tick = -1;
        field_2E_total_slices = gPsxDisplay_504C78.field_0_width / field_46_slice_width;
        field_2A_current_slice = field_2E_total_slices;
      
        xy.field_2_y = 0;
        xy.field_0_x = gPsxDisplay_504C78.field_0_width;

        wh.field_0_x = gPsxDisplay_504C78.field_0_width;
        wh.field_2_y =  gPsxDisplay_504C78.field_2_height;

        pScreenManager_4FF7C8->field_38 = 1;

        field_24_pSubObject = ao_new<ScreenClipper>();
        field_24_pSubObject->ctor_40BD60(xy, wh, 0);
        break;

    case CameraSwapEffects::eEffect2_RightToLeft:
        field_46_slice_width = 48;
        field_2C_slices_per_tick = 1;
        field_2E_total_slices = gPsxDisplay_504C78.field_0_width / field_46_slice_width;
        field_2A_current_slice = -1;

        xy.field_2_y = 0;
        xy.field_0_x = 0;

        wh.field_0_x = 0;
        wh.field_2_y = gPsxDisplay_504C78.field_2_height;

        pScreenManager_4FF7C8->field_38 = 1;

        field_24_pSubObject = ao_new<ScreenClipper>();
        field_24_pSubObject->ctor_40BD60(xy, wh, 0);
        break;

    case CameraSwapEffects::eEffect3_TopToBottom:
        field_46_slice_width = 30; // 240 / 8 ?
        field_2C_slices_per_tick = -1;
        field_2E_total_slices =  gPsxDisplay_504C78.field_2_height / field_46_slice_width;
        field_2A_current_slice = field_2E_total_slices;

        xy.field_0_x = 0;
        xy.field_2_y = gPsxDisplay_504C78.field_2_height;
        
        wh.field_0_x = gPsxDisplay_504C78.field_0_width;
        wh.field_2_y = gPsxDisplay_504C78.field_2_height;

        pScreenManager_4FF7C8->field_38 = 1;

        field_24_pSubObject = ao_new<ScreenClipper>();
        field_24_pSubObject->ctor_40BD60(xy, wh, 0);
        break;

    case CameraSwapEffects::eEffect4_BottomToTop:
        field_46_slice_width = 30; // 240 / 8 ?
        field_2C_slices_per_tick = 1;
        field_2E_total_slices = gPsxDisplay_504C78.field_2_height / field_46_slice_width;
        field_2A_current_slice = -1;
         
        xy.field_0_x = 0;
        xy.field_2_y = 0;

        wh.field_0_x = gPsxDisplay_504C78.field_0_width;
        wh.field_2_y = 0;

        pScreenManager_4FF7C8->field_38 = 1;

        field_24_pSubObject = ao_new<ScreenClipper>();
        field_24_pSubObject->ctor_40BD60(xy, wh, 0);
        break;

    case CameraSwapEffects::eEffect6_VerticalSplit:
        field_46_slice_width = 24; // ??
        field_2E_total_slices = (gPsxDisplay_504C78.field_0_width / 2) / field_46_slice_width;
        field_2C_slices_per_tick = 1;
        field_2A_current_slice = 0;

        pScreenManager_4FF7C8->field_38 = 1;

        xy.field_0_x = gPsxDisplay_504C78.field_0_width / 2;
        xy.field_2_y = 0;

        wh.field_0_x = gPsxDisplay_504C78.field_0_width / 2;
        wh.field_2_y = gPsxDisplay_504C78.field_2_height;

        field_24_pSubObject = ao_new<ScreenClipper>();
        field_24_pSubObject->ctor_40BD60(xy, wh, 0);
        break;


    case CameraSwapEffects::eEffect7_HorizontalSplit:
        field_46_slice_width = 15; // ??
        field_2C_slices_per_tick = 1;
        field_2E_total_slices = (gPsxDisplay_504C78.field_2_height / 2) / field_46_slice_width;
        field_2A_current_slice = 0;

        pScreenManager_4FF7C8->field_38 = 1;

        xy.field_0_x = 0;
        xy.field_2_y = gPsxDisplay_504C78.field_2_height / 2;

        wh.field_0_x = gPsxDisplay_504C78.field_0_width;
        wh.field_2_y = gPsxDisplay_504C78.field_2_height / 2;

        field_24_pSubObject = ao_new<ScreenClipper>();
        field_24_pSubObject->ctor_40BD60(xy, wh, 0);
        break;

    case CameraSwapEffects::eEffect8_BoxOut:
    {
        field_42_XSlices = 24; // ??
        field_44_YSlices = 15; // ??

        short xDiff = gPsxDisplay_504C78.field_0_width - field_3E_xpos_converted;
        if (xDiff <= field_3E_xpos_converted)
        {
            xDiff = field_3E_xpos_converted;
        }

        short yDiff = gPsxDisplay_504C78.field_2_height - field_40_ypos_converted;
        if (yDiff <= field_40_ypos_converted)
        {
            yDiff = field_40_ypos_converted;
        }

        short startingSlice = 0;
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

        pScreenManager_4FF7C8->field_38 = 1;

        xy.field_0_x = gPsxDisplay_504C78.field_0_width - 1;
        xy.field_2_y = gPsxDisplay_504C78.field_2_height - 1;

        field_24_pSubObject = ao_new<ScreenClipper>();
        field_24_pSubObject->ctor_40BD60(xy, { 1, 1 }, 0);

        // "Whoosh" door sound effect
        SFX_Play_43AD70(SoundEffect::IngameTransition_107, 127);
        break;
    }

    case CameraSwapEffects::eEffect5_1_FMV:
    case CameraSwapEffects::eEffect9_2_FMV:
    case CameraSwapEffects::eEffect10_3_FMV:
        pScreenManager_4FF7C8->field_38 = 1;
        field_24_pSubObject = ao_new<ScreenClipper>();
        field_24_pSubObject->ctor_40BD60({0, 0}, {1, 1}, 0);
        field_2A_current_slice = 0;
        break;

    default:
        ALIVE_FATAL("Unknown camera swap effect"); // Or one that isn't handled here
    }
}

void CameraSwapper::VUpdate_48CEA0()
{
    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        return;
    }

    switch (field_28_changeEffect)
    {
    case CameraSwapEffects::eEffect0_InstantChange:
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return;

    case CameraSwapEffects::eEffect1_LeftToRight:
    case CameraSwapEffects::eEffect2_RightToLeft:
    {
        field_2A_current_slice += field_2C_slices_per_tick;
        if (field_2A_current_slice < 0 || field_2A_current_slice >= field_2E_total_slices)
        {
            // All slices done
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            return;
        }

        const short xpos = field_46_slice_width * field_2A_current_slice;
        short width = field_46_slice_width * (field_2A_current_slice + 1);
        if (width > gPsxDisplay_504C78.field_0_width - 1)
        {
            width = gPsxDisplay_504C78.field_0_width - 1;
        }

        pScreenManager_4FF7C8->InvalidateRect_Layer3_406F20(xpos, 0, width, gPsxDisplay_504C78.field_2_height);
        field_24_pSubObject->Update_Clip_Rect_40BE40({ xpos, 0 }, { static_cast<short>(width + 1), static_cast<short>(gPsxDisplay_504C78.field_2_height) });
    }
    break;

    case CameraSwapEffects::eEffect3_TopToBottom:
    case CameraSwapEffects::eEffect4_BottomToTop:
    {
        field_2A_current_slice += field_2C_slices_per_tick;
        if (field_2A_current_slice < 0 || field_2A_current_slice >= field_2E_total_slices)
        {
            // All slices done
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            return;
        }

        const short ypos = field_46_slice_width * field_2A_current_slice;
        short height = field_46_slice_width * (field_2A_current_slice + 1);

        pScreenManager_4FF7C8->InvalidateRect_Layer3_406F20(
            0,
            ypos,
            gPsxDisplay_504C78.field_0_width,
            height);
        field_24_pSubObject->Update_Clip_Rect_40BE40({ 0, ypos }, {  static_cast<short>(gPsxDisplay_504C78.field_0_width), height});
    }
    break;

    case CameraSwapEffects::eEffect5_1_FMV:
    {
        if (sMovie_ref_count_9F309C)
        {
            // A movie is still playing
            return;
        }

        if (field_3C_movie_bPutDispEnv == 1)
        {
            gPsxDisplay_504C78.PutCurrentDispEnv_40DE40();
        }

        pScreenManager_4FF7C8->field_38 = 1;

        // Now apply the camera we where storing now that the movie is finished
        if (field_20_ppCamRes)
        {
            pScreenManager_4FF7C8->DecompressToVRam_407110(reinterpret_cast<WORD**>(field_20_ppCamRes));
            pScreenManager_4FF7C8->InvalidateRect_406E40(0, 0, 640, 240, 0);
            pScreenManager_4FF7C8->InvalidateRect_406E40(0, 0, 640, 240, 1);
            pScreenManager_4FF7C8->InvalidateRect_406E40(0, 0, 640, 240, 2);
            pScreenManager_4FF7C8->field_36_flags |= 1;
        }
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    break;

    case CameraSwapEffects::eEffect6_VerticalSplit:
    {
        field_2A_current_slice += field_2C_slices_per_tick;
        if (field_2A_current_slice < 0 || field_2A_current_slice > field_2E_total_slices)
        {
            // All slices done
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            return;
        }

        const short xpos = field_46_slice_width * field_2A_current_slice;
        const short halfDisplayWidth = gPsxDisplay_504C78.field_0_width / 2;

        pScreenManager_4FF7C8->InvalidateRect_Layer3_406F20(static_cast<short>(halfDisplayWidth - xpos), 0, static_cast<short>(xpos + halfDisplayWidth), gPsxDisplay_504C78.field_2_height);
        field_24_pSubObject->Update_Clip_Rect_40BE40({ static_cast<short>(halfDisplayWidth - xpos), 0 }, { static_cast<short>(xpos + halfDisplayWidth + 1), static_cast<short>(gPsxDisplay_504C78.field_2_height) });
    }
    break;

    case CameraSwapEffects::eEffect7_HorizontalSplit:
    {
        field_2A_current_slice += field_2C_slices_per_tick;
        if (field_2A_current_slice < 0 || field_2A_current_slice > field_2E_total_slices)
        {
            // All slices done
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            return;
        }

        const short ypos = field_46_slice_width * field_2A_current_slice;
        const short halfDisplayHeight = gPsxDisplay_504C78.field_2_height / 2;

        pScreenManager_4FF7C8->InvalidateRect_Layer3_406F20(0, static_cast<short>(halfDisplayHeight - ypos), 640, static_cast<short>(halfDisplayHeight + ypos));
        field_24_pSubObject->Update_Clip_Rect_40BE40({ 0,  static_cast<short>(halfDisplayHeight - ypos) }, { 640,  static_cast<short>(halfDisplayHeight + ypos) });
    }
    break;

    case CameraSwapEffects::eEffect8_BoxOut:
    {
        field_2A_current_slice += field_2C_slices_per_tick;
        if (field_2A_current_slice < 0 || field_2A_current_slice > field_2E_total_slices)
        {
            // All slices done
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            return;
        }

        const short xSlicePos = field_2A_current_slice * field_42_XSlices;
        const short width = (field_3E_xpos_converted + xSlicePos > gPsxDisplay_504C78.field_0_width) ? gPsxDisplay_504C78.field_0_width : field_3E_xpos_converted + xSlicePos;

        const short ySlicePos = field_2A_current_slice * field_44_YSlices;
        const short height = (ySlicePos + field_40_ypos_converted > gPsxDisplay_504C78.field_2_height) ? gPsxDisplay_504C78.field_2_height : ySlicePos + field_40_ypos_converted;

        PSX_Point rect_xy = {};
        rect_xy.field_0_x = (field_3E_xpos_converted - xSlicePos <= 0) ? 0 : field_3E_xpos_converted - xSlicePos;
        rect_xy.field_2_y = (field_40_ypos_converted - ySlicePos <= 0) ? 0 : field_40_ypos_converted - ySlicePos;

        pScreenManager_4FF7C8->InvalidateRect_Layer3_406F20(
            rect_xy.field_0_x,
            rect_xy.field_2_y,
            width,
            height);

        PSX_Point rect_wh = {};
        rect_wh.field_0_x = width + 1;
        rect_wh.field_2_y = height;

        field_24_pSubObject->Update_Clip_Rect_40BE40(rect_xy, rect_wh);
    }
    break;

    // TODO: 2 and 3 FMV cases can be de-duped
    case CameraSwapEffects::eEffect9_2_FMV:
        if (field_2A_current_slice < 1)
        {
            field_2A_current_slice++;
        }
        else
        {
            pScreenManager_4FF7C8->field_36_flags &= ~1u;
        }

        // When no movie is playing start the next one
        if (sMovie_ref_count_9F309C == 0)
        {
            auto pMovie = ao_new<Movie>();
            if (pMovie)
            {
                pMovie->ctor_489C90(
                    field_14_movie_id_3,
                    field_10_movie_pos_3,
                    static_cast<char>(field_30_movie_flag_3),
                    field_32_movie_flags_3,
                    field_34_movie_vol_3);
            }
            field_28_changeEffect = CameraSwapEffects::eEffect5_1_FMV;
            field_3C_movie_bPutDispEnv = field_32_movie_flags_3;
        }
        break;

    case CameraSwapEffects::eEffect10_3_FMV:
        if (field_2A_current_slice < 1)
        {
            field_2A_current_slice++;
        }
        else
        {
            pScreenManager_4FF7C8->field_36_flags &= ~1u;
        }

        // When no movie is playing start the next one
        if (sMovie_ref_count_9F309C == 0)
        {
            auto pMovie = ao_new<Movie>();
            if (pMovie)
            {
                pMovie->ctor_489C90(
                    field_1C_movie_id_2,
                    field_18_movie_pos_2,
                    static_cast<char>(field_36_movie_flag_2),
                    field_38_movie_flags_2,
                    field_3A_movie_vol_2);
            }
            field_28_changeEffect = CameraSwapEffects::eEffect9_2_FMV;
            field_3C_movie_bPutDispEnv = field_38_movie_flags_2;
        }
        break;

    default:
        break;
    }
}

END_NS_AO
