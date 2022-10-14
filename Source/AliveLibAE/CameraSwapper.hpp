#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Map.hpp"

class ScreenClipper;

class CameraSwapper final : public BaseGameObject
{
public:
    CameraSwapper(CamResource& ppCamRes, s32 movieFlag, s32 movieFlags, s32 movieVol);
    CameraSwapper(CamResource& ppCamRes, s32 movieFlag1, s32 movieFlags1, s32 movieVol1, s32 movieFlag2, s32 movieFlags2, s32 movieVol2);
    CameraSwapper(CamResource& ppCamRes, s32 movieFlag1, s32 movieFlags1, s32 movieVol1, s32 movieFlag2, s32 movieFlags2, s32 movieVol2, s32 moveFlag3, s32 movieFlags3, s32 movieVol3);
    CameraSwapper(CamResource& ppCamRes, CameraSwapEffects changeEffect, s32 xpos, s32 ypos);
    ~CameraSwapper();

    void Init(CamResource& ppCamRes, CameraSwapEffects changeEffect);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    CamResource field_30_ppCamRes;
    ScreenClipper* field_34_pSubObject = nullptr;

    CameraSwapEffects field_38_changeEffect = CameraSwapEffects::eInstantChange_0;

    s16 field_3A_current_slice = 0;
    s16 field_3C_slices_per_tick = 0;
    s16 field_3E_total_slices = 0;

    s16 field_40_movie_flag_3 = 0;
    s16 field_42_movie_flags_3 = 0;
    s16 field_44_movie_vol_3 = 0;

    s16 field_46_movie_flag_2 = 0;
    s16 field_48_movie_flags_2 = 0;
    s16 field_4A_movie_vol_2 = 0;

    s16 field_4C_movie_bPutDispEnv = 0;

    s16 field_4E_xpos_converted = 0;
    s16 field_50_ypos_converted = 0;

    s16 field_52_XSlices = 0;
    s16 field_54_YSlices = 0;
    s16 field_56_slice_width = 0;
};
