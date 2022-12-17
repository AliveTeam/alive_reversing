#pragma once

#include "BaseGameObject.hpp"
#include "Primitives.hpp"

class ScreenManager final : public BaseGameObject
{
public:
    void DecompressCameraToVRam(CamResource& camRes);

    ScreenManager(CamResource& camRes, FP_Point* pCameraOffset);

    void Init(CamResource& camRes);

    void VUpdate() override;
    void VScreenChanged() override;
    void VRender(PrimHeader** ppOt) override;

    FP CamXPos() const
    {
        return mCamPos->x - FP_FromInteger(mCamXOff);
    }

    FP CamYPos() const
    {
        return mCamPos->y - FP_FromInteger(mCamYOff);
    }

    void DisableRendering()
    {
        mRenderingDisabled = true;
    }

    void EnableRendering()
    {
        mRenderingDisabled = false;
    }

    bool RenderingEnabled() const
    {
        return !mRenderingDisabled;
    }

public:
    FP_Point* mCamPos = nullptr;
    s16 mCamXOff = 0;
    u16 mCamYOff = 0;
private:
    s16 mCamWidth = 0;
    s16 mCamHeight = 0;
    bool mRenderingDisabled = false;

    CamResource mCamRes;
    Poly_FT4 mPoly = {};
};

extern ScreenManager* gScreenManager;
