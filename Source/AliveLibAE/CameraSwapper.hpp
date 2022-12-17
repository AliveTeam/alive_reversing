#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "Map.hpp"

class ScreenClipper;

class CameraSwapper final : public BaseGameObject
{
public:
    CameraSwapper(CamResource& camRes, bool bPutDispEnv1, const char_type* pFmv1, bool bPutDispEnv2 = false, const char_type* pFmv2 = nullptr, bool bPutDispEnv3 = false, const char_type* pFmv3 = nullptr);
    CameraSwapper(CamResource& camRes, CameraSwapEffects changeEffect, s32 xpos, s32 ypos);
    ~CameraSwapper();

    void Init(CamResource& camRes, CameraSwapEffects changeEffect);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    CamResource mCamRes;
    ScreenClipper* mScreenClipper = nullptr;

    CameraSwapEffects mCamChangeEffect = CameraSwapEffects::eInstantChange_0;

    const char_type* mFmvs[3] = {nullptr, nullptr, nullptr};
    bool mPutDispEnv[3] = {false, false, false};

    s16 mCurrentSlice = 0;
    s16 mSlicesPerTick = 0;
    s16 mTotalSlices = 0;

    bool mMoviePutDispEnv = false;

    s16 mXPosConverted = 0;
    s16 mYPosConverted = 0;

    s16 mXSlices = 0;
    s16 mYSlices = 0;
    s16 mSliceWidth = 0;
};
