#pragma once

#include "BaseGameObject.hpp"

enum class CameraSwapEffects : s16;

class ScreenClipper;

class CameraSwapper final : public BaseGameObject
{
public:
    CameraSwapper(u8** ppCamRes, s32 movieSector, s32 movieId, s32 movieFlag, s32 movieFlags, s32 movieVol);
    CameraSwapper(u8** ppCamRes, s32 moviePos1, s32 movieId1, s32 moviePos2, s32 movieId2, s32 movieFlag1, s32 movieFlags1, s32 movieVol1, s32 movieFlag2, s32 movieFlags2, s32 movieVol2);
    CameraSwapper(u8** ppCamRes, s32 moviePos1, s32 movieId1, s32 moviePos2, s32 movieId2, s32 moviePos3, s32 movieId3, s32 movieFlag1, s32 movieFlags1, s32 movieVol1, s32 movieFlag2, s32 movieFlags2, s32 movieVol2, s32 moveFlag3, s32 movieFlags3, s32 movieVol3);
    CameraSwapper(u8** ppCamRes, CameraSwapEffects changeEffect, s32 xpos, s32 ypos);
    ~CameraSwapper();

    void Init(u8** ppCamRes, CameraSwapEffects changeEffect);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s32 field_20_movie_pos_3;
    s32 field_24_movie_id_3;

    s32 field_28_movie_pos_2;
    s32 field_2C_movie_id_2;

    u8** field_30_ppCamRes;
    ScreenClipper* field_34_pSubObject;

    CameraSwapEffects field_38_changeEffect;

    s16 field_3A_current_slice;
    s16 field_3C_slices_per_tick;
    s16 field_3E_total_slices;

    s16 field_40_movie_flag_3;
    s16 field_42_movie_flags_3;
    s16 field_44_movie_vol_3;

    s16 field_46_movie_flag_2;
    s16 field_48_movie_flags_2;
    s16 field_4A_movie_vol_2;

    s16 field_4C_movie_bPutDispEnv;

    s16 field_4E_xpos_converted;
    s16 field_50_ypos_converted;

    s16 field_52_XSlices;
    s16 field_54_YSlices;
    s16 field_56_slice_width;
};
ALIVE_ASSERT_SIZEOF(CameraSwapper, 0x58);
