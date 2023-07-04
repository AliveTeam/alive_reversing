#include "stdafx.h"
#include "ScreenManager.hpp"
#include "GameType.hpp"
#include "ResourceManagerWrapper.hpp"
#include "Layer.hpp"
#include "Primitives.hpp"

ScreenManager* gScreenManager = nullptr;

void ScreenManager::DecompressCameraToVRam(CamResource& camRes)
{
    mCamRes = camRes;
}

ScreenManager::ScreenManager(CamResource& camRes, FP_Point* pCameraOffset)
    : BaseGameObject(true, 0)
{
    mCamPos = pCameraOffset;

    SetSurviveDeathReset(true);
    SetUpdateDuringCamSwap(true);

    Init(camRes);
}

void ScreenManager::Init(CamResource& camRes)
{
    EnableRendering();

    SetType(ReliveTypes::eScreenManager);

    // TODO: Refactor out
    if (GetGameType() == GameType::eAo)
    {
        mCamXOff = 184;
        mCamYOff = 120;
    }

    mCamWidth = 640;
    mCamHeight = 240;

    DecompressCameraToVRam(camRes);
}

void ScreenManager::VUpdate()
{
    // Empty
}

void ScreenManager::VRender(BasePrimitive** ppOt)
{
    if (mRenderingDisabled)
    {
        return;
    }

    if (mCamRes.mData.mPixels)
    {
        mPoly.SetXY0(0, 0);
        mPoly.SetXY1(640, 0);
        mPoly.SetXY2( 0, 240);
        mPoly.SetXY3( 640, 240);
        mPoly.SetRGB0(127, 127, 127);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_1), &mPoly);
        mPoly.mCam = &mCamRes;
    }
}

void ScreenManager::VScreenChanged()
{
    // Empty
}
