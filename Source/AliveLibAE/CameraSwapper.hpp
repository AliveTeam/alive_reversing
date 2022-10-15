#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Map.hpp"

class ScreenClipper;

class CameraSwapper final : public BaseGameObject
{
public:
    CameraSwapper(CamResource& ppCamRes, bool bPutDispEnv1, const char_type* pFmv1, bool bPutDispEnv2 = false, const char_type* pFmv2 = nullptr, bool bPutDispEnv3 = false, const char_type* pFmv3 = nullptr);
    CameraSwapper(CamResource& ppCamRes, CameraSwapEffects changeEffect, s32 xpos, s32 ypos);
    ~CameraSwapper();

    void Init(CamResource& ppCamRes, CameraSwapEffects changeEffect);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    CamResource field_30_ppCamRes;
    ScreenClipper* field_34_pSubObject = nullptr;

    CameraSwapEffects field_38_changeEffect = CameraSwapEffects::eInstantChange_0;

    const char_type* mFmvs[3] = {nullptr, nullptr, nullptr};
    bool mPutDispEnv[3] = {false, false, false};

    s16 field_3A_current_slice = 0;
    s16 field_3C_slices_per_tick = 0;
    s16 field_3E_total_slices = 0;

    s16 field_4C_movie_bPutDispEnv = 0;

    s16 field_4E_xpos_converted = 0;
    s16 field_50_ypos_converted = 0;

    s16 field_52_XSlices = 0;
    s16 field_54_YSlices = 0;
    s16 field_56_slice_width = 0;
};
