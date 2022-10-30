#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"

enum class CameraSwapEffects : s16;

namespace AO {


class ScreenClipper;

class CameraSwapper final : public ::BaseGameObject
{
public:
    CameraSwapper(CamResource& ppBits, bool bPutDispEnv1, const char_type* pFmv1, bool bPutDispEnv2 = false, const char_type* pFmv2 = nullptr, bool bPutDispEnv3 = false, const char_type* pFmv3 = nullptr);
    CameraSwapper(CamResource& ppBits, CameraSwapEffects changeEffect, s32 xpos, s32 ypos);
    ~CameraSwapper();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    void Init(CamResource& ppCamRes, CameraSwapEffects changeEffect);

    const char_type* mFmvs[3] = {nullptr, nullptr, nullptr};
    bool mPutDispEnv[3] = {false, false, false};

    //s32 field_10_movie_pos_3 = 0;
    //s32 field_14_movie_id_3 = 0;
    //s32 field_18_movie_pos_2 = 0;
    //s32 field_1C_movie_id_2 = 0;
    CamResource field_20_ppCamRes;
    ScreenClipper* field_24_pSubObject = nullptr;
    CameraSwapEffects field_28_changeEffect = CameraSwapEffects::eInstantChange_0;
    s16 field_2A_current_slice = 0;
    s16 field_2C_slices_per_tick = 0;
    s16 field_2E_total_slices = 0;
   // s16 field_30_movie_flag_3 = 0;
   // s16 field_32_movie_flags_3 = 0;
   // s16 field_34_movie_vol_3 = 0;
   // s16 field_36_movie_flag_2 = 0;
   // s16 field_38_movie_flags_2 = 0;
   // s16 field_3A_movie_vol_2 = 0;
    s16 field_3C_movie_bPutDispEnv = 0;
    s16 field_3E_xpos_converted = 0;
    s16 field_40_ypos_converted = 0;
    s16 field_42_XSlices = 0;
    s16 field_44_YSlices = 0;
    s16 field_46_slice_width = 0;
};

extern s16 gNumCamSwappers; // TODO: Move to own file

} // namespace AO
