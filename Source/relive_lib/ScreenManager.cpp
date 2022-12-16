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

void ScreenManager::VRender(PrimHeader** ppOt)
{
    if (mRenderingDisabled)
    {
        return;
    }

    if (mCamRes.mData.mPixels)
    {
        PolyFT4_Init(&mPoly);

        SetXY0(&mPoly, 0, 0);
        SetXY1(&mPoly, 640, 0);
        SetXY2(&mPoly, 0, 240);
        SetXY3(&mPoly, 640, 240);
        SetRGB0(&mPoly, 127, 127, 127);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_1), &mPoly.mBase.header);
        mPoly.mCam = &mCamRes;
    }
}

void ScreenManager::VScreenChanged()
{
    // Empty
}
