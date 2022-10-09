#include "stdafx_ao.h"
#include "FG1.hpp"
#include "Function.hpp"
#include "../relive_lib/Primitives.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "../relive_lib/VRam.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../AliveLibAE/Renderer/IRenderer.hpp"
#include "FG1Reader.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

FG1::~FG1()
{
    gObjListDrawables->Remove_Item(this);
}

FG1::FG1(Fg1Resource& pFg1Res, CamResource& camRes)
    : BaseGameObject(TRUE, 0)
    , mFG1Res(pFg1Res)
    , mCamRes(camRes)
{
    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);
    mBaseGameObjectFlags.Set(Options::eSurviveDeathReset_Bit9);
    mBaseGameObjectFlags.Set(Options::eUpdateDuringCamSwap_Bit10);

    SetType(ReliveTypes::eFG1);

    //field_10_cam_pos_x = FP_GetExponent(pScreenManager->mCamPos->x);
    //field_12_cam_pos_y = FP_GetExponent(pScreenManager->mCamPos->y);

    field_16_current_path = gMap.mCurrentPath;
    field_14_current_level = gMap.mCurrentLevel;

    gObjListDrawables->Push_Back(this);
}

void FG1::VUpdate()
{
    // Empty
}

void FG1::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void FG1::VRender(PrimHeader** ppOt)
{
    if (mFG1Res.Any())
    {
        if (mFG1Res.mBg.mImage.mPixels)
        {
            PolyFT4_Init(&mPolys[0]);
            mPolys[0].mFg1 = &mFG1Res.mBg;
            mPolys[0].mCam = &mCamRes;
            SetRGB0(&mPolys[0], 127, 127, 127);
            SetXYWH(&mPolys[0], 0, 0, 640, 240);
            OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_FG1_Half_18), &mPolys[0].mBase.header);
        }

        if (mFG1Res.mFg.mImage.mPixels)
        {
            PolyFT4_Init(&mPolys[1]);
            mPolys[1].mFg1 = &mFG1Res.mFg;
            mPolys[1].mCam = &mCamRes;
            SetRGB0(&mPolys[1], 127, 127, 127);
            SetXYWH(&mPolys[1], 0, 0, 640, 240);
            OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_FG1_37), &mPolys[1].mBase.header);
        }

        if (mFG1Res.mBgWell.mImage.mPixels)
        {
            PolyFT4_Init(&mPolys[2]);
            mPolys[2].mFg1 = &mFG1Res.mBgWell;
            mPolys[2].mCam = &mCamRes;
            SetRGB0(&mPolys[2], 127, 127, 127);
            SetXYWH(&mPolys[2], 0, 0, 640, 240);
            OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_Well_Half_4), &mPolys[2].mBase.header);
        }

        if (mFG1Res.mFgWell.mImage.mPixels)
        {
            PolyFT4_Init(&mPolys[3]);
            mPolys[3].mFg1 = &mFG1Res.mFgWell;
            mPolys[3].mCam = &mCamRes;
            SetRGB0(&mPolys[3], 127, 127, 127);
            SetXYWH(&mPolys[3], 0, 0, 640, 240);
            OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_Well_23), &mPolys[3].mBase.header);
        }
    }
}

} // namespace AO
