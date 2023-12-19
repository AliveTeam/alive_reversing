#include "stdafx.h"
#include "ScreenManager.hpp"
#include "GameType.hpp"
#include "ResourceManagerWrapper.hpp"
#include "Layer.hpp"
#include "Primitives.hpp"
#include "../AliveLibAE/PsxRender.hpp"

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

void ScreenManager::VRender(OrderingTable& ot)
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
        ot.Add(Layer::eLayer_1, &mPoly);
        mPoly.mCam = &mCamRes;
    }
}

void ScreenManager::VScreenChanged()
{
    // Empty
}

ScreenBounds ScreenManager::PerGameScreenBounds()
{
    ScreenBounds bounds = {};
    if (GetGameType() == GameType::eAo)
    {
        bounds.screenLeft = gScreenManager->mCamPos->x - FP_FromInteger(gScreenManager->mCamXOff);
        bounds.screenRight = gScreenManager->mCamPos->x + FP_FromInteger(gScreenManager->mCamXOff);

        bounds.screenTop = gScreenManager->mCamPos->y - FP_FromInteger(gScreenManager->mCamYOff);
        bounds.screenBottom = gScreenManager->mCamPos->y + FP_FromInteger(gScreenManager->mCamYOff);
        return bounds;
    }
    else
    {
        const FP camX = gScreenManager->CamXPos();
        const FP camY = gScreenManager->CamYPos();

        bounds.screenLeft = camX;
        bounds.screenRight = camX + FP_FromInteger(640);

        bounds.screenTop = camY;
        bounds.screenBottom = camY + FP_FromInteger(240);
        return bounds;
    }
}

bool ScreenManager::InScreenBounds(FP x, FP y)
{
    ScreenBounds bounds = PerGameScreenBounds();
    if (x >= bounds.screenLeft && x <= bounds.screenRight)
    {
        if (y >= bounds.screenTop && y <= bounds.screenBottom)
        {
            return true;
        }
    }
    return false;
}