#include "stdafx.h"
#include "OrbWhirlWindParticle.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "BaseAliveGameObject.hpp"
#include "stdlib.hpp"

OrbWhirlWindParticle::OrbWhirlWindParticle(FP xpos, FP ypos, FP scale, s16 bIsMudokonSpirit)
{
    AnimResource res = ResourceManagerWrapper::LoadAnimation(AnimId::ChantOrb_Particle);
    mAnim.Init(res, nullptr);

    mAnim.mFlags.Set(AnimFlags::eSemiTrans);

    mAnim.SetRenderLayer(Layer::eLayer_AbeMenu_32);
    mAnim.SetRenderMode(TPageAbr::eBlend_1);
    if (bIsMudokonSpirit == 1)
    {
        mAnim.mFlags.Clear(AnimFlags::eBlending);
        mAnim.SetRGB(0, 255, 32);
    }
    else
    {
        mAnim.mFlags.Set(AnimFlags::eBlending);
        mAnim.SetRGB(80, 80, 80);
    }

    mAnim.SetFrame(Math_RandomRange(0, static_cast<s16>(mAnim.Get_Frame_Count() - 1)));
    SetActive(0);
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
    mRenderAsScale = (mCurrentScale * mRandomScale);
}

s32 OrbWhirlWindParticle::IsActive()
{
    return mFlags.Get(Flags::eIsActive);
}

void OrbWhirlWindParticle::Spin(FP xpos, FP ypos, FP scale, BaseGameObject* pObj)
{
    mPositionTimer = sGnFrame + Math_RandomRange(0, 16);
    mState = State::eSpin;
    mRingTargetObjId = pObj;
    mYPosIncrement = (mCurrentScale * (mMoveY - mYPosMid)) / FP_FromInteger(16);
    mYPos = ypos;
    mXPos = xpos;
    mTargetObjScale = scale;
}

void OrbWhirlWindParticle::ToStop()
{
    mState = State::eStop;
    mPositionTimer = sGnFrame + Math_RandomRange(0, 32);
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
                if (mRingTargetObjId && mRingTargetObjId->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
                {
                    ToStop();
                }
                else
                {
                    mXPosOffset2 = mXPosMid;
                    mXPosOffset = mXPosMid;
                    mYPosOffset2 = mYPosMid;
                    mYPosOffset = mYPosMid;
                    mScaleOffsetFlyToTarget = (mTargetObjScale - mCurrentScale) / FP_FromInteger(16);
                    mPositionTimer = sGnFrame + 16;
                    mState = State::eFlyToTarget;
                    CalculateRenderProperties(1);
                }
            }
            break;

        case State::eFlyToTarget:
            if (mRingTargetObjId && mRingTargetObjId->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                ToStop();
            }
            else
            {
                FP xpos = {};
                FP ypos = {};
                if (mRingTargetObjId)
                {
                    const PSX_RECT bRect = static_cast<BaseAliveGameObject*>(mRingTargetObjId)->VGetBoundingRect();

                    xpos = FP_FromInteger((bRect.x + bRect.w) / 2);
                    ypos = FP_FromInteger((bRect.y + bRect.h) / 2);
                }
                else
                {
                    xpos = mXPos;
                    ypos = mYPos;
                }

                if (static_cast<s32>(sGnFrame) < mPositionTimer)
                {
                    mCurrentScale += mScaleOffsetFlyToTarget;
                    const FP v16 = FP_FromInteger(16 - (mPositionTimer - sGnFrame)) / FP_FromInteger(16);
                    mXPosOffset2 = ((xpos - mXPosOffset) * v16) + mXPosOffset;
                    mYPosOffset2 = ((ypos - mYPosOffset) * v16) + mYPosOffset;
                    mXPosMid = (FP_FromInteger(32) * mCurrentScale) * Math_Sine_496DF0(FP_FromInteger(128) * v16) + mXPosOffset2;
                    mYPosMid = (FP_FromInteger(32) * mCurrentScale) * Math_Cosine_496D60(FP_FromInteger(128) * v16) + mYPosOffset2;
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
                SetActive(1);
            }

            mYPosMid = (mScaleOffsetSpinAtTarget * Math_Cosine_496D60((FP_FromInteger(128) * FP_FromInteger(32 - (mPositionTimer - sGnFrame)) / FP_FromInteger(32)))) + mYPosOffset2;
            mRadiusX = mRadiusX - mRadiusOffsetX;
            CalculateRenderProperties(1);
            break;

        case State::eStop:
            if (static_cast<s32>(sGnFrame) >= mPositionTimer)
            {
                SetActive(1);
            }
            CalculateRenderProperties(0);
            break;

        default:
            return;
    }
}

void OrbWhirlWindParticle::Render(PrimHeader** ppOt)
{
    const FP x = std::min(pScreenManager->CamXPos(),
                          pScreenManager->CamXPos() + FP_FromInteger(367));

    const FP w = std::max(pScreenManager->CamXPos(),
                          pScreenManager->CamXPos() + FP_FromInteger(367));

    const FP y = std::min(pScreenManager->CamYPos(),
                          pScreenManager->CamYPos() + FP_FromInteger(239));

    const FP h = std::max(pScreenManager->CamYPos(),
                          pScreenManager->CamYPos() + FP_FromInteger(239));

    if (mXPosRenderOffset >= x && mXPosRenderOffset <= w)
    {
        if (mYPosRenderOffset + FP_FromInteger(5) >= y && mYPosRenderOffset + FP_FromInteger(5) <= h)
        {
            mAnim.SetSpriteScale(mRenderAsScale);
            const FP xpos = mXPosRenderOffset - pScreenManager->CamXPos();
            const FP ypos = mYPosRenderOffset - pScreenManager->CamYPos() + FP_FromInteger(5);

            mAnim.VRender(
                FP_GetExponent(xpos),
                FP_GetExponent(ypos),
                ppOt,
                0,
                0);
        }
    }
}

OrbWhirlWindParticle::~OrbWhirlWindParticle()
{
    mAnim.VCleanUp();
}

void OrbWhirlWindParticle::CalculateRenderProperties(s16 bStarted)
{
    mRenderAngle += mCounter;

    if (bStarted)
    {
        if (mCounter <= mMaxCounter && !(static_cast<s32>(sGnFrame) % 3))
        {
            mCounter++;
        }
    }
    else if (mCounter >= 1 && !(static_cast<s32>(sGnFrame) % 3))
    {
        mCounter--;
        mRadiusX = mRadiusX + FP_FromInteger(4);
    }

    mXPosRenderOffset = ((mCurrentScale * mRadiusX) * Math_Sine_496DD0(static_cast<u8>(mRenderAngle))) + mXPosMid;
    mYPosRenderOffset = ((mCurrentScale * mRadiusY) * Math_Cosine_496CD0(static_cast<u8>(mRenderAngle))) + mYPosMid;
    mRenderAsScale = mCurrentScale * mRandomScale;

    if (mCurrentScale > FP_FromDouble(0.599)) // TODO: Check VS 39321
    {
        mAnim.SetRenderLayer(Layer::eLayer_AbeMenu_32);
    }
    else
    {
        mAnim.SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
    }
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
