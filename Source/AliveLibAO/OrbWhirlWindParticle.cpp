#include "stdafx_ao.h"
#include "OrbWhirlWindParticle.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Math.hpp"
#include "../relive_lib/GameObjects/IBaseAliveGameObject.hpp"
#include "../relive_lib/FixedPoint.hpp"

namespace AO {


OrbWhirlWindParticle::OrbWhirlWindParticle(FP xpos, FP ypos, FP scale)
{
    mAnim.Init(ResourceManagerWrapper::LoadAnimation(AnimId::ChantOrb_Particle), nullptr);

    mAnim.SetSemiTrans(true);

    mAnim.SetRenderLayer(Layer::eLayer_AbeMenu_32);
    mAnim.SetBlendMode(relive::TBlendModes::eBlend_1);

    mAnim.SetRGB(80, 80, 80);

    mAnim.SetFrame(Math_RandomRange(0, static_cast<s16>(mAnim.Get_Frame_Count() - 1)));
    mIsActive = false;
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

s32 OrbWhirlWindParticle::IsActive()
{
    return mIsActive;
}

void OrbWhirlWindParticle::Spin(FP xpos, FP ypos, IBaseAliveGameObject* pObj)
{
    mPositionTimer = BaseGameObject::MakeTimer(Math_RandomRange(0, 16));
    mState = State::eSpin;
    mRingTargetObjId = pObj;

    mYPosIncrement = (mCurrentScale * (mMoveY - mYPosMid)) / FP_FromInteger(16);
    mXpos_Unused = xpos;
    mYPos_Unused = ypos;
}

void OrbWhirlWindParticle::ToStop()
{
    mState = State::eStop;
    mPositionTimer = BaseGameObject::MakeTimer(Math_RandomRange(0, 32));
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
                if (!mRingTargetObjId || mRingTargetObjId->GetDead())
                {
                    ToStop();
                }
                else
                {
                    mXPosOffset2 = mXPosMid;
                    mXPosOffset = mXPosMid;
                    mYPosOffset2 = mYPosMid;
                    mYPosOffset = mYPosMid;
                    mScaleOffsetFlyToTarget = (mRingTargetObjId->GetSpriteScale() - mCurrentScale) / FP_FromInteger(16);
                    mPositionTimer = BaseGameObject::MakeTimer(16);
                    mState = State::eFlyToTarget;
                    CalculateRenderProperties(1);
                }
            }
            break;

        case State::eFlyToTarget:
            if (!mRingTargetObjId || mRingTargetObjId->GetDead())
            {
                ToStop();
            }
            else
            {
                const PSX_RECT bRect = mRingTargetObjId->VGetBoundingRect();

                const FP xpos = FP_FromInteger((bRect.x + bRect.w) / 2);
                const FP ypos = FP_FromInteger((bRect.y + bRect.h) / 2);

                if (static_cast<s32>(sGnFrame) < mPositionTimer)
                {
                    mCurrentScale += mScaleOffsetFlyToTarget;
                    const FP v25 = FP_FromInteger(16 - (mPositionTimer - sGnFrame)) / FP_FromInteger(16);
                    mXPosOffset2 = ((xpos - mXPosOffset) * v25) + mXPosOffset;
                    mYPosOffset2 = ((ypos - mYPosOffset) * v25) + mYPosOffset;
                    mXPosMid = mXPosOffset2 + ((FP_FromInteger(32) * mCurrentScale) * Math_Sine(FP_GetExponent(FP_FromInteger(128) * v25) & 0xFF));
                    mYPosMid = mYPosOffset2 + ((FP_FromInteger(32) * mCurrentScale) * Math_Cosine(FP_GetExponent(FP_FromInteger(128) * v25) & 0xFF));
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
                    mPositionTimer = BaseGameObject::MakeTimer(32);
                    mState = State::eSpinAtTarget;
                }
                CalculateRenderProperties(1);
            }
            break;

        case State::eSpinAtTarget:
            if (static_cast<s32>(sGnFrame) >= mPositionTimer)
            {
                mIsActive = !mIsActive;
            }

            mYPosMid = (mScaleOffsetSpinAtTarget * Math_Cosine(FP_GetExponent(FP_FromInteger(128) * FP_FromInteger(32 - (mPositionTimer + sGnFrame)) / FP_FromInteger(32)) & 0xFF)) + mYPosOffset2;
            mRadiusX -= mRadiusOffsetX;
            CalculateRenderProperties(1);
            break;

        case State::eStop:
            if (static_cast<s32>(sGnFrame) >= mPositionTimer)
            {
                mIsActive = !mIsActive;
            }
            CalculateRenderProperties(0);
            break;

        default:
            return;
    }
}

void OrbWhirlWindParticle::Render(BasePrimitive** ppOt)
{
    mAnim.SetSpriteScale(mRenderAsScale);

    const s16 xpos = FP_GetExponent(FP_FromInteger(gScreenManager->mCamXOff)
                                    + mXPosRenderOffset
                                    - gScreenManager->mCamPos->x);

    const s16 ypos = FP_GetExponent(FP_FromInteger(gScreenManager->mCamYOff + 5)
                                    + mYPosRenderOffset
                                    - gScreenManager->mCamPos->y);

    mAnim.VRender(
        xpos,
        ypos,
        ppOt,
        0,
        0);
}


OrbWhirlWindParticle::~OrbWhirlWindParticle()
{
    mAnim.VCleanUp();
}

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

    mXPosRenderOffset = ((mCurrentScale * mRadiusX) * Math_Sine(static_cast<u8>(mRenderAngle))) + mXPosMid;
    mYPosRenderOffset = ((mCurrentScale * mRadiusY) * Math_Cosine(static_cast<u8>(mRenderAngle))) + mYPosMid;
    mRenderAsScale = mCurrentScale * mRandomScale;

    if (mCurrentScale > FP_FromDouble(0.599))
    {
        mAnim.SetRenderLayer(Layer::eLayer_AbeMenu_32);
    }
    else
    {
        mAnim.SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
    }
}

} // namespace AO
