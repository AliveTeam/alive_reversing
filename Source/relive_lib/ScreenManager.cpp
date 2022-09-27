#include "stdafx.h"
#include "ScreenManager.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "VRam.hpp"
#include "../AliveLibAE/Renderer/IRenderer.hpp"
#include "../AliveLibCommon/CamDecompressor.hpp"
#include "../relive_lib/GameType.hpp"
#include "ResourceManagerWrapper.hpp"

ALIVE_VAR(1, 0x5BB5F4, ScreenManager*, pScreenManager, nullptr);

void ScreenManager::DecompressCameraToVRam(CamResource& camRes)
{
    mCamRes = camRes;
}

ScreenManager::ScreenManager(CamResource& camRes, FP_Point* pCameraOffset)
    : BaseGameObject(TRUE, 0)
{
    mCamPos = pCameraOffset;

    mBaseGameObjectFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    mBaseGameObjectFlags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);

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