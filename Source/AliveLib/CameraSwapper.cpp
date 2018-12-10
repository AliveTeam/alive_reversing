#include "stdafx.h"
#include "CameraSwapper.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "BackgroundMusic.hpp"
#include "MusicController.hpp"
#include "Sound.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "Sfx.hpp"
#include "Movie.hpp"


class ScreenClipper : public BaseGameObject
{
public:
    EXPORT ScreenClipper* ctor_416D60(PSX_Point xy, PSX_Point wh, __int16 layer)
    {
        BaseGameObject_ctor_4DBFA0(1, 0);

        SetVTable(this, 0x5445C4);
        field_4_typeId = Types::eScreenClipper_114;

        field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap);
        field_6_flags.Set(BaseGameObject::eDrawable);

        field_40_rect.x = xy.field_0_x;
        field_40_rect.y = xy.field_2_y;
        field_40_rect.w = wh.field_0_x;
        field_40_rect.h = wh.field_2_y;

        field_48_ot_layer = layer;

        gObjList_drawables_5C1124->Push_Back(this);

        // This actually matters here due how its called
        return this;
    }

    virtual BaseGameObject* VDestructor(signed int flags)
    {
        return vdtor_416E00(flags);
    }

    virtual void VRender(int** ot) override
    {
        vRender_416F70(ot);
    }

    virtual void VScreenChanged() {}
    virtual void VUpdate() { }

    void vRender_416F70(int** ot)
    {
        PSX_RECT clipRect = {};

        clipRect.x = field_40_rect.x;
        clipRect.y = field_40_rect.y;
        clipRect.w = field_40_rect.w - field_40_rect.x;
        clipRect.h = field_40_rect.h - field_40_rect.y;

        if (gPsxDisplay_5C1130.field_C_buffer_index)
        {
            // Move to the lower buffer if not using the top buffer
            clipRect.y += 256;
        }

        Prim_PrimClipper* pClipper = &field_20_clippers[gPsxDisplay_5C1130.field_C_buffer_index];
        Init_PrimClipper_4F5B80(pClipper, &clipRect);
        OrderingTable_Add_4F8AA0(&ot[field_48_ot_layer], &pClipper->mBase);
    }

    EXPORT void dtor_416E30()
    {
        SetVTable(this, 0x5445C4);
        gObjList_drawables_5C1124->Remove_Item(this);
        BaseGameObject_dtor_4DBEC0();
    }

    EXPORT BaseGameObject* vdtor_416E00(signed int flags)
    {
        dtor_416E30();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

    EXPORT void Update_Clip_Rect_416EB0(PSX_Point xy, PSX_Point wh)
    {
        field_40_rect.x = min(xy.field_0_x, field_40_rect.x);
        field_40_rect.y = min(xy.field_2_y, field_40_rect.y);

        field_40_rect.w = max(wh.field_0_x, field_40_rect.w);
        field_40_rect.h = max(wh.field_2_y, field_40_rect.h);
    }

private:
    Prim_PrimClipper field_20_clippers[2];
    PSX_RECT field_40_rect;
    __int16 field_48_ot_layer;
};
ALIVE_ASSERT_SIZEOF(ScreenClipper, 0x4C);

CameraSwapper* CameraSwapper::ctor_4E4CA0(BYTE** ppCamRes, int movieSector, int movieId, char movieFlag, __int16  movieFlags, __int16 movieVol)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x5480E4); // vTbl_CameraSwapper_5480E4

    Init_4E50C0(ppCamRes, CameraSwapEffects::eEffect5_1_FMV);

    PSX_ResetCallBack_4FAA20();

    Movie* pMovie = alive_new<Movie>();
    if (pMovie)
    {
        pMovie->ctor_4DFDE0(movieId, movieSector, movieFlag, movieFlags, movieVol);
    }

    field_4C_movie_bPutDispEnv = movieFlags;

    return this;
}

CameraSwapper* CameraSwapper::ctor_4E4DC0(BYTE** ppCamRes, int moviePos1, int movieId1, int moviePos2, int movieId2, char movieFlag1, __int16 movieFlags1, __int16 movieVol1, __int16 movieFlag2, __int16 movieFlags2, __int16 movieVol2)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x5480E4); // vTbl_CameraSwapper_5480E4

    Init_4E50C0(ppCamRes, CameraSwapEffects::eEffect9_2_FMV);
    
    PSX_ResetCallBack_4FAA20();

    Movie* pMovie = alive_new<Movie>();
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

CameraSwapper* CameraSwapper::ctor_4E4ED0(BYTE** ppCamRes, int moviePos1, int movieId1, int moviePos2, int movieId2, int moviePos3, int movieId3, char movieFlag1, __int16  movieFlags1, __int16 movieVol1, __int16 movieFlag2, __int16 movieFlags2, __int16 movieVol2, __int16 moveFlag3, __int16 movieFlags3, __int16 movieVol3)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x5480E4); // vTbl_CameraSwapper_5480E4

    Init_4E50C0(ppCamRes, CameraSwapEffects::eEffect10_3_FMV);

    PSX_ResetCallBack_4FAA20();
    Movie* pMovie = alive_new<Movie>();
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

CameraSwapper* CameraSwapper::ctor_4E5000(BYTE** ppCamRes, CameraSwapEffects changeEffect, __int16 xpos, __int16 ypos)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x5480E4); // vTbl_CameraSwapper_5480E4
    field_4E_xpos_converted = (40 * xpos) / 23;
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
        field_34_pSubObject->field_6_flags.Set(BaseGameObject::eDead);
    }

    if (sMap_word_5C311C)
    {
        gMap_5C3030.sub_480740(0);
        sMap_word_5C311C = 0;
    }

    BackgroundMusic::Play_4CB030();
    MusicController::EnableMusic_47FE10(1);
    Start_Sounds_For_Objects_In_Near_Cameras_4CBB60();
    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* CameraSwapper::vdtor_4E4D90(signed int flags)
{
    dtor_4E5790();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

const int kSliceWidth = 8;

void CameraSwapper::Init_4E50C0(BYTE** ppCamRes, CameraSwapEffects changeEffect)
{
    field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap);

    field_4_typeId = Types::eCameraSwapper_149;

    field_34_pSubObject = nullptr;

    if (changeEffect == CameraSwapEffects::eEffect5_1_FMV || changeEffect == CameraSwapEffects::eEffect9_2_FMV || changeEffect == CameraSwapEffects::eEffect10_3_FMV)
    {
        field_30_ppCamRes = ppCamRes;
    }
    else
    {
        pScreenManager_5BB5F4->DecompressToVRam_40EF60(reinterpret_cast<WORD**>(ppCamRes));
    }

    sNum_CamSwappers_5C1B66++;

    if (sNum_CamSwappers_5C1B66 != 1)
    {
        field_6_flags.Clear(BaseGameObject::eUpdatable);
        field_6_flags.Set(BaseGameObject::eListAddFailed);
        field_6_flags.Set(BaseGameObject::eDead);

        // There can only be 1 active at a time
        return;
    }
    field_38_changeEffect = changeEffect;
    
    PSX_Point xy = {};
    PSX_Point wh = {};

    switch (changeEffect)
    {
    case CameraSwapEffects::eEffect0_InstantChange:
        pScreenManager_5BB5F4->InvalidateRect_Layer3_40EDB0(0, 0, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height);
        field_6_flags.Set(BaseGameObject::eDead);
        field_34_pSubObject = nullptr;
        return;

    case CameraSwapEffects::eEffect1_LeftToRight:
        field_56_slice_width = gPsxDisplay_5C1130.field_0_width / kSliceWidth;
        field_3C_slices_per_tick = -1;
        field_3E_total_slices = gPsxDisplay_5C1130.field_0_width / field_56_slice_width;
        field_3A_current_slice = field_3E_total_slices;

        xy.field_2_y = 0;
        xy.field_0_x = gPsxDisplay_5C1130.field_0_width;

        wh.field_0_x = gPsxDisplay_5C1130.field_0_width;
        wh.field_2_y = gPsxDisplay_5C1130.field_2_height;

        pScreenManager_5BB5F4->field_44 = 1;

        field_34_pSubObject = alive_new<ScreenClipper>();
        field_34_pSubObject->ctor_416D60(xy, wh, 0);
        break;

    case CameraSwapEffects::eEffect2_RightToLeft:
        field_56_slice_width = gPsxDisplay_5C1130.field_0_width / kSliceWidth;
        field_3C_slices_per_tick = 1;
        field_3E_total_slices = gPsxDisplay_5C1130.field_0_width / field_56_slice_width;
        field_3A_current_slice = -1;

        xy.field_2_y = 0;
        xy.field_0_x = 0;

        wh.field_0_x = 0;
        wh.field_2_y = gPsxDisplay_5C1130.field_2_height;

        pScreenManager_5BB5F4->field_44 = 1;

        field_34_pSubObject = alive_new<ScreenClipper>();
        field_34_pSubObject->ctor_416D60(xy, wh, 0);
        break;

    case CameraSwapEffects::eEffect3_TopToBottom:
        field_56_slice_width = gPsxDisplay_5C1130.field_2_height / kSliceWidth;
        field_3C_slices_per_tick = -1;
        field_3E_total_slices = gPsxDisplay_5C1130.field_2_height / field_56_slice_width;
        field_3A_current_slice = field_3E_total_slices;

        xy.field_0_x = 0;
        xy.field_2_y = gPsxDisplay_5C1130.field_2_height;

        wh.field_0_x = gPsxDisplay_5C1130.field_0_width;
        wh.field_2_y = gPsxDisplay_5C1130.field_2_height;

        pScreenManager_5BB5F4->field_44 = 1;

        field_34_pSubObject = alive_new<ScreenClipper>();
        field_34_pSubObject->ctor_416D60(xy, wh, 0);
        break;

    case CameraSwapEffects::eEffect4_BottomToTop:
        field_56_slice_width = gPsxDisplay_5C1130.field_2_height / kSliceWidth;
        field_3C_slices_per_tick = 1;
        field_3E_total_slices = gPsxDisplay_5C1130.field_2_height / field_56_slice_width;
        field_3A_current_slice = -1;

        xy.field_0_x = 0;
        xy.field_2_y = 0;

        wh.field_0_x = gPsxDisplay_5C1130.field_0_width;
        wh.field_2_y = 0;

        pScreenManager_5BB5F4->field_44 = 1;

        field_34_pSubObject = alive_new<ScreenClipper>();
        field_34_pSubObject->ctor_416D60(xy, wh, 0);
        break;

    case CameraSwapEffects::eEffect6_VerticalSplit:
        field_56_slice_width = (gPsxDisplay_5C1130.field_0_width / 2) / kSliceWidth;
        field_3C_slices_per_tick = 1;
        field_3E_total_slices = (gPsxDisplay_5C1130.field_0_width / 2) / field_56_slice_width;
        field_3A_current_slice = 0;

        pScreenManager_5BB5F4->field_44 = 1;

        xy.field_0_x = gPsxDisplay_5C1130.field_0_width / 2;
        xy.field_2_y = 0;

        wh.field_0_x = gPsxDisplay_5C1130.field_0_width / 2;
        wh.field_2_y = gPsxDisplay_5C1130.field_2_height;

        field_34_pSubObject = alive_new<ScreenClipper>();
        field_34_pSubObject->ctor_416D60(xy, wh, 0);
        break;

    case CameraSwapEffects::eEffect7_HorizontalSplit:
        field_56_slice_width = (gPsxDisplay_5C1130.field_2_height / 2) / kSliceWidth;
        field_3C_slices_per_tick = 1;
        field_3E_total_slices = (gPsxDisplay_5C1130.field_2_height / 2) / field_56_slice_width;
        field_3A_current_slice = 0;

        pScreenManager_5BB5F4->field_44 = 1;

        xy.field_0_x = 0;
        xy.field_2_y = gPsxDisplay_5C1130.field_2_height / 2;

        wh.field_0_x = gPsxDisplay_5C1130.field_0_width;
        wh.field_2_y = gPsxDisplay_5C1130.field_2_height / 2;

        field_34_pSubObject = alive_new<ScreenClipper>();
        field_34_pSubObject->ctor_416D60(xy, wh, 0);
        break;

    case CameraSwapEffects::eEffect8_BoxOut:
    {
        field_52_XSlices = (gPsxDisplay_5C1130.field_0_width / 2) / kSliceWidth;
        field_54_YSlices = (gPsxDisplay_5C1130.field_2_height / 2) / kSliceWidth;
        
        short xDiff = gPsxDisplay_5C1130.field_0_width - field_4E_xpos_converted;
        if (xDiff <= field_4E_xpos_converted)
        {
            xDiff = field_4E_xpos_converted;
        }

        short yDiff = gPsxDisplay_5C1130.field_2_height - field_50_ypos_converted;
        if (yDiff <= field_50_ypos_converted)
        {
            yDiff = field_50_ypos_converted;
        }

        short startingSlice = 0;
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

        pScreenManager_5BB5F4->field_44 = 1;

        xy.field_0_x = gPsxDisplay_5C1130.field_0_width - 1;
        xy.field_2_y = gPsxDisplay_5C1130.field_2_height - 1;

        field_34_pSubObject = alive_new<ScreenClipper>();
        field_34_pSubObject->ctor_416D60(xy, { 1, 1 }, 0);

        // "Whoosh" door sound effect
        SFX_Play_46FA90(84u, 127, 0x10000);
    }
    break;

    case CameraSwapEffects::eEffect5_1_FMV:
    case CameraSwapEffects::eEffect9_2_FMV:
    case CameraSwapEffects::eEffect10_3_FMV:
        pScreenManager_5BB5F4->field_44 = 1;

        field_34_pSubObject = alive_new<ScreenClipper>();
        field_34_pSubObject->ctor_416D60({0, 0}, {1, 1}, 0);
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
    if (field_6_flags.Get(BaseGameObject::eDead))
    {
        return;
    }

    switch (field_38_changeEffect)
    {
    case CameraSwapEffects::eEffect1_LeftToRight:
    case CameraSwapEffects::eEffect2_RightToLeft:
    {
        field_3A_current_slice += field_3C_slices_per_tick;
        if (field_3A_current_slice < 0 || field_3A_current_slice >= field_3E_total_slices)
        {
            // All slices done
            field_6_flags.Set(BaseGameObject::eDead);
            return;
        }

        const short xpos = field_56_slice_width * field_3A_current_slice;
        short width = (field_56_slice_width * (field_3A_current_slice + 1));
        if (width > gPsxDisplay_5C1130.field_0_width - 1)
        {
            width = gPsxDisplay_5C1130.field_0_width - 1;
        }

        pScreenManager_5BB5F4->InvalidateRect_Layer3_40EDB0(xpos, 0, width, gPsxDisplay_5C1130.field_2_height);
        field_34_pSubObject->Update_Clip_Rect_416EB0({ xpos, 0 }, { width + 1, gPsxDisplay_5C1130.field_2_height });
    }
    break;

    case CameraSwapEffects::eEffect3_TopToBottom:
    case CameraSwapEffects::eEffect4_BottomToTop:
    {
        field_3A_current_slice += field_3C_slices_per_tick;
        if (field_3A_current_slice < 0 || field_3A_current_slice >= field_3E_total_slices)
        {
            // All slices done
            field_6_flags.Set(BaseGameObject::eDead);
            return;
        }

        const short ypos = field_56_slice_width * field_3A_current_slice;
        short height = (field_56_slice_width * (field_3A_current_slice + 1));

        pScreenManager_5BB5F4->InvalidateRect_Layer3_40EDB0(0, ypos, gPsxDisplay_5C1130.field_0_width, height);
        field_34_pSubObject->Update_Clip_Rect_416EB0({ 0, ypos }, { gPsxDisplay_5C1130.field_0_width, height });
    }
    break;

    case CameraSwapEffects::eEffect6_VerticalSplit:
    {
        field_3A_current_slice += field_3C_slices_per_tick;
        if (field_3A_current_slice < 0 || field_3A_current_slice > field_3E_total_slices)
        {
            // All slices done
            field_6_flags.Set(BaseGameObject::eDead);
            return;
        }

        const short xpos = field_56_slice_width * field_3A_current_slice;
        const short halfDisplayWidth = gPsxDisplay_5C1130.field_0_width / 2;

        pScreenManager_5BB5F4->InvalidateRect_Layer3_40EDB0(halfDisplayWidth - xpos, 0, xpos + halfDisplayWidth, gPsxDisplay_5C1130.field_2_height);
        field_34_pSubObject->Update_Clip_Rect_416EB0({ halfDisplayWidth - xpos, 0 }, { xpos + halfDisplayWidth + 1,  gPsxDisplay_5C1130.field_2_height });
    }
    break;

    case CameraSwapEffects::eEffect7_HorizontalSplit:
    {
        field_3A_current_slice += field_3C_slices_per_tick;
        if (field_3A_current_slice < 0 || field_3A_current_slice > field_3E_total_slices)
        {
            // All slices done
            field_6_flags.Set(BaseGameObject::eDead);
            return;
        }

        const short ypos = field_56_slice_width * field_3A_current_slice;
        const short halfDisplayHeight = gPsxDisplay_5C1130.field_2_height / 2;

        pScreenManager_5BB5F4->InvalidateRect_Layer3_40EDB0(0, halfDisplayHeight - ypos, 640, halfDisplayHeight + ypos);
        field_34_pSubObject->Update_Clip_Rect_416EB0({ 0, halfDisplayHeight - ypos }, { 640, halfDisplayHeight + ypos });
    }
    break;

    case CameraSwapEffects::eEffect8_BoxOut:
    {
        field_3A_current_slice += field_3C_slices_per_tick;
        if (field_3A_current_slice < 0 || field_3A_current_slice > field_3E_total_slices)
        {
            // All slices done
            field_6_flags.Set(BaseGameObject::eDead);
            return;
        }

        const short xSlicePos = field_3A_current_slice * field_52_XSlices;
        const short width = (field_4E_xpos_converted + xSlicePos > gPsxDisplay_5C1130.field_0_width) ? gPsxDisplay_5C1130.field_0_width : field_4E_xpos_converted + xSlicePos;

        const short ySlicePos = field_3A_current_slice * field_54_YSlices;
        const short height = (ySlicePos + field_50_ypos_converted > gPsxDisplay_5C1130.field_2_height) ? gPsxDisplay_5C1130.field_2_height : ySlicePos + field_50_ypos_converted;

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

    case CameraSwapEffects::eEffect5_1_FMV:
    {
        if (sMovie_ref_count_BB4AE4)
        {
            // A movie is still playing
            return;
        }

        if (field_4C_movie_bPutDispEnv == 1)
        {
            gPsxDisplay_5C1130.PutCurrentDispEnv_41DFA0();
        }

        pScreenManager_5BB5F4->field_44 = 1;

        // Now apply the camera we where storing now that the movie is finished
        if (field_30_ppCamRes)
        {
            pScreenManager_5BB5F4->DecompressToVRam_40EF60(reinterpret_cast<WORD**>(field_30_ppCamRes));
            pScreenManager_5BB5F4->InvalidateRect_40EC90(0, 0, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height, 0);
            pScreenManager_5BB5F4->InvalidateRect_40EC90(0, 0, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height, 1);
            pScreenManager_5BB5F4->InvalidateRect_40EC90(0, 0, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height, 2);
        }

        field_6_flags.Set(BaseGameObject::eDead);
    }
    break;

    // TODO: 2 and 3 FMV cases can be de-duped
    case CameraSwapEffects::eEffect9_2_FMV:
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
            auto pMovie = alive_new<Movie>();
            if (pMovie)
            {
                pMovie->ctor_4DFDE0(
                    field_24_movie_id_3,
                    field_20_movie_pos_3,
                    field_40_movie_flag_3,
                    field_42_movie_flags_3,
                    field_44_movie_vol_3);
            }
            field_38_changeEffect = CameraSwapEffects::eEffect5_1_FMV;
            field_4C_movie_bPutDispEnv = field_48_movie_flags_2;
        }
        break;

    case CameraSwapEffects::eEffect10_3_FMV:
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
            auto pMovie = alive_new<Movie>();
            if (pMovie)
            {
                pMovie->ctor_4DFDE0(
                    field_2C_movie_id_2,
                    field_28_movie_pos_2,
                    field_46_movie_flag_2,
                    field_48_movie_flags_2,
                    field_4A_movie_vol_2);
            }
            field_38_changeEffect = CameraSwapEffects::eEffect9_2_FMV;
            field_4C_movie_bPutDispEnv = field_48_movie_flags_2;
        }
        break;
    }
}

BaseGameObject* CameraSwapper::VDestructor(signed int flags)
{
    return vdtor_4E4D90(flags);
}

void CameraSwapper::VScreenChanged()
{

}
