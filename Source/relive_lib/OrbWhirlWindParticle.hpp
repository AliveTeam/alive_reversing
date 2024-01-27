#pragma once

#include "../relive_lib/Animation.hpp"
#include "../relive_lib/FixedPoint.hpp"

class BaseGameObject;

class OrbWhirlWindParticle final
{
public:
    OrbWhirlWindParticle(FP xpos, FP ypos, FP scale, bool isMudokonSpirit);
    ~OrbWhirlWindParticle();

    s32 IsActive();
    void Spin(FP xpos, FP ypos, FP scale, BaseGameObject* pObj);
    void ToStop();
    void Update();
    void Render(OrderingTable& ot);

private:
    void CalculateRenderProperties(s16 bStarted);

private:
    bool mIsActive = false;
    Animation mAnim;
    FP mXPosRenderOffset = {};
    FP mYPosRenderOffset = {};
    FP mRenderAsScale = {};
    FP mRadiusOffsetX = {};
    FP mYPosIncrement = {};

    enum class State : s16
    {
        eStart = 0,
        eSpin = 1,
        eFlyToTarget = 2,
        eSpinAtTarget = 3,
        eStop = 4,
    };
    State mState = State::eStart;

    s32 mRenderAngle = 0;
    s16 mCounter = 0;
    s16 mMaxCounter = 0;
    FP mCurrentScale = {};
    FP mRandomScale = {};
    FP mScaleOffsetFlyToTarget = {};
    FP mXPosMid = {};
    FP mYPosMid = {};
    FP mRadiusX = {};
    FP mRadiusY = {};
    s32 mPositionTimer = 0;
    FP mMoveY = {};
    BaseGameObject* mRingTargetObjId = nullptr;
    FP mXPos = {};
    FP mYPos = {};
    FP mTargetObjScale = {};
    FP mXPosOffset = {};
    FP mYPosOffset = {};
    FP mXPosOffset2 = {};
    FP mYPosOffset2 = {};
    FP mScaleOffsetSpinAtTarget = {};
};