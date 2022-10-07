#include "stdafx_ao.h"
#include "Function.hpp"
#include "OrbWhirlWindParticle.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Math.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

void OrbWhirlWindParticle::CalculateRenderProperties(s16 bStarted)
{
    mRenderAngle += mCounter;

    if (bStarted && mCounter <= mMaxCounter && !(sGnFrame % 3))
    {
        mCounter++;
    }
    else if (!bStarted && mCounter >= 1 && !(sGnFrame % 3))
    {
        mCounter--;
        mRadiusX += FP_FromInteger(4);
    }

    mXPosRenderOffset = ((mCurrentScale * mRadiusX) * Math_Sine_451110(static_cast<u8>(mRenderAngle))) + mXPosMid;
    mYPosRenderOffset = ((mCurrentScale * mRadiusY) * Math_Cosine_4510A0(static_cast<u8>(mRenderAngle))) + mYPosMid;
    mRenderAsScale = (mCurrentScale * mRandomScale);
    if (mCurrentScale > FP_FromDouble(0.599))
    {
        mAnim.mRenderLayer = Layer::eLayer_AbeMenu_32;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_AbeMenu_Half_13;
    }
}

void OrbWhirlWindParticle::ToStop()
{
    mState = State::eStop;
    mPositionTimer = sGnFrame + Math_RandomRange(0, 32);
}

s32 OrbWhirlWindParticle::IsActive()
{
    return mFlags.Get(Flags::eIsActive);
}

void OrbWhirlWindParticle::Render(PrimHeader** ppOt)
{
    mAnim.field_14_scale = mRenderAsScale;

    const s16 xpos = FP_GetExponent(FP_FromInteger(pScreenManager->mCamXOff)
                                    + mXPosRenderOffset
                                    - pScreenManager->mCamPos->x);

    const s16 ypos = FP_GetExponent(FP_FromInteger(pScreenManager->mCamYOff + 5)
                                    + mYPosRenderOffset
                                    - pScreenManager->mCamPos->y);

    mAnim.VRender(
        xpos,
        ypos,
        ppOt,
        0,
        0);

    PSX_RECT rect = {};
    mAnim.Get_Frame_Rect(&rect);
}

void OrbWhirlWindParticle::SetActive(u8 active)
{
    if (active)
    {
        mFlags.Set(Flags::eIsActive);
    }
    else
    {
        mFlags.Clear(Flags::eIsActive);
    }
}

void OrbWhirlWindParticle::Update()
{
    switch (mState)
    {
        case State::eStart:
            CalculateRenderProperties(1);
            break;

        case State::eSpin:
            if (static_cast<s32>(sGnFrame) < mPositionTimer + 16)
            {
                if (static_cast<s32>(sGnFrame) >= mPositionTimer)
                {
                    mYPosMid += mYPosIncrement;
                    mRadiusX -= FP_FromInteger(2);
                    mRadiusY -= FP_FromInteger(1);
                }
                CalculateRenderProperties(1);
            }
            else
            {
                if (!mTargetObj || mTargetObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
                {
                    ToStop();
                }
                else
                {
                    mXPosOffset2 = mXPosMid;
                    mXPosOffset = mXPosMid;
                    mYPosOffset2 = mYPosMid;
                    mYPosOffset = mYPosMid;
                    mScaleOffsetFlyToTarget = (mTargetObj->mSpriteScale - mCurrentScale) / FP_FromInteger(16);
                    mPositionTimer = sGnFrame + 16;
                    mState = State::eFlyToTarget;
                    CalculateRenderProperties(1);
                }
            }
            break;

        case State::eFlyToTarget:
            if (!mTargetObj || mTargetObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                ToStop();
            }
            else
            {
                const PSX_RECT bRect = mTargetObj->VGetBoundingRect();

                const FP xpos = FP_FromInteger((bRect.x + bRect.w) / 2);
                const FP ypos = FP_FromInteger((bRect.y + bRect.h) / 2);

                if (static_cast<s32>(sGnFrame) < mPositionTimer)
                {
                    mCurrentScale += mScaleOffsetFlyToTarget;
                    const FP v25 = FP_FromInteger(16 - (mPositionTimer - sGnFrame)) / FP_FromInteger(16);
                    mXPosOffset2 = ((xpos - mXPosOffset) * v25) + mXPosOffset;
                    mYPosOffset2 = ((ypos - mYPosOffset) * v25) + mYPosOffset;
                    mXPosMid = mXPosOffset2 + ((FP_FromInteger(32) * mCurrentScale) * Math_Sine_451110(FP_GetExponent(FP_FromInteger(128) * v25) & 0xFF));
                    mYPosMid = mYPosOffset2 + ((FP_FromInteger(32) * mCurrentScale) * Math_Cosine_4510A0(FP_GetExponent(FP_FromInteger(128) * v25) & 0xFF));
                }
                else
                {
                    mXPosOffset2 = xpos;
                    mYPosOffset2 = ypos;
                    mXPosMid = xpos;
                    mYPosMid = ypos;
                    mRenderAngle = 192;
                    mRadiusX = FP_FromInteger(40);
                    mRadiusOffsetX = mRadiusX / FP_FromInteger(32);
                    mScaleOffsetSpinAtTarget = mCurrentScale * FP_FromInteger(Math_RandomRange(-16, 16));
                    mPositionTimer = sGnFrame + 32;
                    mState = State::eSpinAtTarget;
                }
                CalculateRenderProperties(1);
            }
            break;

        case State::eSpinAtTarget:
            if (static_cast<s32>(sGnFrame) >= mPositionTimer)
            {
                SetActive(IsActive() ? 0 : 1);
            }
            mYPosMid = (mScaleOffsetSpinAtTarget * Math_Cosine_4510A0(FP_GetExponent(FP_FromInteger(128) * FP_FromInteger(32 - (mPositionTimer + sGnFrame)) / FP_FromInteger(32)) & 0xFF)) + mYPosOffset2;
            mRadiusX -= mRadiusOffsetX;
            CalculateRenderProperties(1);
            break;

        case State::eStop:
            if (static_cast<s32>(sGnFrame) >= mPositionTimer)
            {
                SetActive(IsActive() ? 0 : 1);
            }
            CalculateRenderProperties(0);
            break;

        default:
            return;
    }
}

OrbWhirlWindParticle::OrbWhirlWindParticle(FP xpos, FP ypos, FP scale)
{
    mAnim.Init(ResourceManagerWrapper::LoadAnimation(AnimId::ChantOrb_Particle), nullptr);

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);

    mAnim.mRenderLayer = Layer::eLayer_AbeMenu_32;
    mAnim.mRenderMode = TPageAbr::eBlend_1;

    mAnim.mRed = 80;
    mAnim.mGreen = 80;
    mAnim.mBlue = 80;

    mAnim.SetFrame(Math_RandomRange(0, static_cast<s16>(mAnim.Get_Frame_Count() - 1)));
    mFlags.Clear(Flags::eIsActive);
    mState = State::eStart;
    mRenderAngle = Math_RandomRange(0, 255);
    mCounter = 1;
    mMaxCounter = Math_RandomRange(9, 15);
    mXPosMid = xpos;
    mYPosMid = ypos + FP_FromInteger(Math_RandomRange(-12, 12));
    mMoveY = ypos - FP_FromInteger(16);
    mRadiusX = FP_FromInteger(Math_RandomRange(37, 43));
    mRadiusY = FP_FromDouble(0.25) * mRadiusX;
    mCurrentScale = scale;
    mRandomScale = FP_FromInteger(Math_RandomRange(7, 10)) / FP_FromInteger(10);
    mRenderAsScale = mCurrentScale * mRandomScale;
}


OrbWhirlWindParticle::~OrbWhirlWindParticle()
{
    mAnim.VCleanUp();
}

void OrbWhirlWindParticle::Spin(FP xpos, FP ypos, BaseAliveGameObject* pObj)
{
    mPositionTimer = sGnFrame + Math_RandomRange(0, 16);
    mState = State::eSpin;
    mTargetObj = pObj;

    mYPosIncrement = (mCurrentScale * (mMoveY - mYPosMid)) / FP_FromInteger(16);
    mXpos_Unused = xpos;
    mYPos_Unused = ypos;
}

} // namespace AO
