#include "stdafx_ao.h"
#include "PossessionFlicker.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Game.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {
PossessionFlicker::PossessionFlicker(IBaseAliveGameObject* pToApplyFlicker, s32 duration, s32 r, s32 g, s32 b)
    : BaseGameObject(true, 0)
{
    mTargetObj = pToApplyFlicker;
    mTargetObj->mBaseGameObjectRefCount++;

    mNewRed = static_cast<s16>(r);
    mNewGreen = static_cast<s16>(g);
    mNewBlue = static_cast<s16>(b);

    mOldRed = pToApplyFlicker->mRGB.r;
    mOldGreen = pToApplyFlicker->mRGB.g;
    mOldBlue = pToApplyFlicker->mRGB.b;

    pToApplyFlicker->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);

    mTimeToFlicker = duration + sGnFrame;
}

PossessionFlicker::~PossessionFlicker()
{
    if (mTargetObj)
    {
        mTargetObj->GetAnimation().SetRenderMode(TPageAbr::eBlend_0);

        mTargetObj->mRGB.r = mOldRed;
        mTargetObj->mRGB.g = mOldGreen;
        mTargetObj->mRGB.b = mOldBlue;

        mTargetObj->mBaseGameObjectRefCount--;
    }
}

void PossessionFlicker::VScreenChanged()
{
    if (mTargetObj->GetDead())
    {
        mTargetObj->mBaseGameObjectRefCount--;
        mTargetObj = nullptr;
        SetDead(true);
    }
}

void PossessionFlicker::VUpdate()
{
    bool bFlicker = false;
    if (mTargetObj->GetDead())
    {
        mTargetObj->mBaseGameObjectRefCount--;
        mTargetObj = nullptr;
        SetDead(true);
        bFlicker = false;
    }
    else
    {
        bFlicker = true;
    }

    if (bFlicker)
    {
        if (sGnFrame % 2)
        {
            mTargetObj->mRGB.r = mOldRed;
            mTargetObj->mRGB.g = mOldGreen;
            mTargetObj->mRGB.b = mOldBlue;
        }
        else
        {
            mTargetObj->mRGB.r = mNewRed;
            mTargetObj->mRGB.g = mNewGreen;
            mTargetObj->mRGB.b = mNewBlue;
        }

        if (static_cast<s32>(sGnFrame) > mTimeToFlicker)
        {
            SetDead(true);
        }
    }
}

} // namespace AO
