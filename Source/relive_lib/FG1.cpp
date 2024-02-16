#include "stdafx.h"
#include "FG1.hpp"
#include "Primitives.hpp"
#include "../AliveLibAE/PsxRender.hpp"
#include "Layer.hpp"

extern DynamicArrayT<BaseGameObject>* gObjListDrawables;

FG1::~FG1()
{
    gObjListDrawables->Remove_Item(this);
}

FG1::FG1(Fg1Resource& pFg1Res, CamResource& camRes)
    : BaseGameObject(true, 0)
    , mFG1Res(pFg1Res)
    , mCamRes(camRes)
{
    SetDrawable(true);
    SetSurviveDeathReset(true);
    SetUpdateDuringCamSwap(true);

    SetType(ReliveTypes::eFG1);

    gObjListDrawables->Push_Back(this);
}

void FG1::VScreenChanged()
{
    SetDead(true);
}

void FG1::VRender(OrderingTable& ot)
{
    if (mFG1Res.Any())
    {
        if (mFG1Res.mBg.mImage.mPixels)
        {
            mPolys[0].mFg1 = &mFG1Res.mBg;
            mPolys[0].mCam = &mCamRes;
            mPolys[0].SetRGB0(127, 127, 127);
            mPolys[0].SetXYWH(0, 0, 640, 240);
            ot.Add(Layer::eLayer_FG1_Half_18, &mPolys[0]);
        }

        if (mFG1Res.mFg.mImage.mPixels)
        {
            mPolys[1].mFg1 = &mFG1Res.mFg;
            mPolys[1].mCam = &mCamRes;
            mPolys[1].SetRGB0(127, 127, 127);
            mPolys[1].SetXYWH(0, 0, 640, 240);
            ot.Add(Layer::eLayer_FG1_37, &mPolys[1]);
        }

        if (mFG1Res.mBgWell.mImage.mPixels)
        {
            mPolys[2].mFg1 = &mFG1Res.mBgWell;
            mPolys[2].mCam = &mCamRes;
            mPolys[2].SetRGB0(127, 127, 127);
            mPolys[2].SetXYWH(0, 0, 640, 240);
            ot.Add(Layer::eLayer_Well_Half_4, &mPolys[2]);
        }

        if (mFG1Res.mFgWell.mImage.mPixels)
        {
            mPolys[3].mFg1 = &mFG1Res.mFgWell;
            mPolys[3].mCam = &mCamRes;
            mPolys[3].SetRGB0(127, 127, 127);
            mPolys[3].SetXYWH(0, 0, 640, 240);
            ot.Add(Layer::eLayer_Well_23, &mPolys[3]);
        }
    }
}
