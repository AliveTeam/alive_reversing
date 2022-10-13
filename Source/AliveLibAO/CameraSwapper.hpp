#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"

namespace AO {

enum class CameraSwapEffects : s16;

class ScreenClipper;

class CameraSwapper final : public ::BaseGameObject
{
public:
    CameraSwapper(CamResource& ppBits, s32 movieId, s32 movieFlag, s8 movieFlags, s16 flags, s16 volume);
    CameraSwapper(CamResource& ppBits, s32 moviePos1, s32 movieId1, s32 moviePos2, s32 movieFlag1, s8 movieFlags1, s16 movieVol1, s16 movieFlag2, s16 movieFlag2_1, s16 movieFlags2_1, s16 movieVol2);
    CameraSwapper(CamResource& ppBits, s32 moviePos1, s32 movieIds1, s32 moviePos2, s32 movieId2, s32 moviePos3, s32 movieId3, s8 movieFlag1, s16 movieFlags1, s16 movieVol1, s16 movieFlag2, s16 movieFlags2, s16 movieVol2, s16 movieFlag3, s16 movieFlags3, s16 movieVol3);
    CameraSwapper(CamResource& ppBits, CameraSwapEffects changeEffect, s32 xpos, s32 ypos);
    ~CameraSwapper();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    void Init(CamResource& ppCamRes, CameraSwapEffects changeEffect);

    s32 field_10_movie_pos_3 = 0;
    s32 field_14_movie_id_3 = 0;
    s32 field_18_movie_pos_2 = 0;
    s32 field_1C_movie_id_2 = 0;
    CamResource field_20_ppCamRes;
    ScreenClipper* field_24_pSubObject = nullptr;
    CameraSwapEffects field_28_changeEffect = CameraSwapEffects::eInstantChange_0;
    s16 field_2A_current_slice = 0;
    s16 field_2C_slices_per_tick = 0;
    s16 field_2E_total_slices = 0;
    s16 field_30_movie_flag_3 = 0;
    s16 field_32_movie_flags_3 = 0;
    s16 field_34_movie_vol_3 = 0;
    s16 field_36_movie_flag_2 = 0;
    s16 field_38_movie_flags_2 = 0;
    s16 field_3A_movie_vol_2 = 0;
    s16 field_3C_movie_bPutDispEnv = 0;
    s16 field_3E_xpos_converted = 0;
    s16 field_40_ypos_converted = 0;
    s16 field_42_XSlices = 0;
    s16 field_44_YSlices = 0;
    s16 field_46_slice_width = 0;
};

extern s16 gNumCamSwappers; // TODO: Move to own file

} // namespace AO
