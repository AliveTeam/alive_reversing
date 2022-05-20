#pragma once

#include "BaseGameObject.hpp"

enum class CameraSwapEffects : s16;

class ScreenClipper;

class CameraSwapper final : public BaseGameObject
{
public:
    EXPORT CameraSwapper* ctor_4E4CA0(u8** ppCamRes, s32 movieSector, s32 movieId, s8 movieFlag, s16 movieFlags, s16 movieVol);
    EXPORT CameraSwapper* ctor_4E4DC0(u8** ppCamRes, s32 moviePos1, s32 movieId1, s32 moviePos2, s32 movieId2, s8 movieFlag1, s16 movieFlags1, s16 movieVol1, s16 movieFlag2, s16 movieFlags2, s16 movieVol2);
    EXPORT CameraSwapper* ctor_4E4ED0(u8** ppCamRes, s32 moviePos1, s32 movieId1, s32 moviePos2, s32 movieId2, s32 moviePos3, s32 movieId3, s8 movieFlag1, s16 movieFlags1, s16 movieVol1, s16 movieFlag2, s16 movieFlags2, s16 movieVol2, s16 moveFlag3, s16 movieFlags3, s16 movieVol3);
    EXPORT CameraSwapper* ctor_4E5000(u8** ppCamRes, CameraSwapEffects changeEffect, s16 xpos, s16 ypos);
    EXPORT void dtor_4E5790();
    EXPORT BaseGameObject* vdtor_4E4D90(s32 flags);
    EXPORT void Init_4E50C0(u8** ppCamRes, CameraSwapEffects changeEffect);
    EXPORT void vUpdate_4E5850();

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
