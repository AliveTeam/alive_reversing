#include "stdafx.h"
#include "MotionDetector.hpp"
#include "stdlib.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "BaseAliveGameObject.hpp"
#include "DDCheat.hpp"
#include "Abe.hpp"
#include "../relive_lib/Events.hpp"
#include "Greeter.hpp"
#include "Sfx.hpp"
#include "Alarm.hpp"
#include "../relive_lib/Function.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/FatalError.hpp"
#include "Game.hpp"

MotionDetectorLaser::MotionDetectorLaser(FP xpos, FP ypos, FP scale, Layer layer)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eRedLaser);
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MotionDetector_Laser));
    Animation_Init(GetAnimRes(AnimId::MotionDetector_Laser));
    GetAnimation().SetRenderLayer(layer);
    mXPos = xpos;
    SetSpriteScale(scale);
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);
    mYPos = ypos;
}

// =====================================================================================

MotionDetector::MotionDetector(relive::Path_MotionDetector* pTlv, const Guid& tlvId, BaseAnimatedWithPhysicsGameObject* pOwner)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eGreeterBody);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MotionDetector_Flare));
    Animation_Init(GetAnimRes(AnimId::MotionDetector_Flare));

    GetAnimation().SetSemiTrans(true);
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);
    GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);

    mYOffset = 0;

    mRGB.SetRGB(64, 0, 0);

    mObjectInLaser = false;

    if (!pOwner)
    {
        mDontComeBack = true;
        mHasOwner = false;
        mOwnerId = Guid{};

        mTlvId = tlvId;
        SetSpriteScale(FP_FromInteger(1));

        if (pTlv->mScale != relive::reliveScale::eFull)
        {
            SetSpriteScale(FP_FromDouble(0.5));
        }

        mTopLeftX = FP_FromInteger(pTlv->mTopLeftX);
        mBottomRightX = FP_FromInteger(pTlv->mBottomRightX);
        mTopLeftY = FP_FromInteger(pTlv->mTopLeftY);
        mBottomRightY = FP_FromInteger(pTlv->mBottomRightY);

        PSX_Point pos = {};
        gMap.Get_Abe_Spawn_Pos(&pos);
        if (pTlv->mDeviceX)
        {
            mXPos = FP_FromInteger(pTlv->mDeviceX - pos.x);
            mYPos = FP_FromInteger(pTlv->mDeviceY - pos.y);
        }
        else
        {
            mXPos = FP_FromInteger(pTlv->mTopLeftX);
            mYPos = FP_FromInteger(pTlv->mTopLeftY);
        }

        mSpeed = FP_FromRaw((u16) pTlv->mSpeedx256 << 8);

        MotionDetectorLaser* pLaser = nullptr;
        if (pTlv->mInitialMoveDirection == relive::Path_MotionDetector::InitialMoveDirection::eLeft)
        {
            mState = States::eMoveLeft_2;
            pLaser = relive_new MotionDetectorLaser(mBottomRightX, mBottomRightY, GetSpriteScale(), Layer::eLayer_Foreground_36);
        }
        else if (pTlv->mInitialMoveDirection == relive::Path_MotionDetector::InitialMoveDirection::eRight)
        {
            mState = States::eMoveRight_0;
            pLaser = relive_new MotionDetectorLaser(mTopLeftX, mBottomRightY, GetSpriteScale(), Layer::eLayer_Foreground_36);
        }
        else
        {
            ALIVE_FATAL("couldn't find start move direction for motion detector");
        }

        if (pTlv->mDrawFlare == relive::reliveChoice::eYes)
        {
            GetAnimation().SetRender(true);
        }
        else
        {
            GetAnimation().SetRender(false);
        }

        if (pLaser)
        {
            mLaserId = pLaser->mBaseGameObjectId;
            mDisableSwitchId = pTlv->mDisableSwitchId;

            if (SwitchStates_Get(mDisableSwitchId) == 0)
            {
                pLaser->GetAnimation().SetRender(true);
            }
            else
            {
                pLaser->GetAnimation().SetRender(false);
            }
        }

        mAlarmSwitchId = pTlv->mAlarmSwitchId;
        mAlarmDuration = pTlv->mAlarmDuration;
        return;
    }

    mHasOwner = true;
    SetSpriteScale(pOwner->GetSpriteScale());

    mTopLeftX = pOwner->mXPos - (GetSpriteScale() * FP_FromInteger(75));
    mBottomRightX = (GetSpriteScale() * FP_FromInteger(75)) + pOwner->mXPos;
    mTopLeftY = pOwner->mYPos - (GetSpriteScale() * FP_FromInteger(20));
    mBottomRightY = pOwner->mYPos;

    mXPos = pOwner->mXPos;
    mYPos = pOwner->mYPos - (GetSpriteScale() * FP_FromInteger(20));

    mSpeed = FP_FromInteger(2);
    mState = States::eMoveRight_0;

    auto pLaserMem = relive_new MotionDetectorLaser(pOwner->mXPos, pOwner->mYPos, GetSpriteScale(), Layer::eLayer_Foreground_36);
    if (pLaserMem)
    {
        mLaserId = pLaserMem->mBaseGameObjectId;
    }

    GetAnimation().SetRender(true);
    mOwnerId = pOwner->mBaseGameObjectId;
    mAlarmSwitchId = 0;
    mAlarmDuration = 0;
}

MotionDetector::~MotionDetector()
{
    if (!mHasOwner)
    {
        if (mDontComeBack)
        {
            Path::TLV_Reset(mTlvId);
        }
        else
        {
            Path::TLV_Delete(mTlvId);
        }
    }

    BaseGameObject* pLaser = sObjectIds.Find_Impl(mLaserId);
    if (pLaser)
    {
        pLaser->SetDead(true);
    }
}

void MotionDetector::VScreenChanged()
{
    BaseGameObject::VScreenChanged();

    BaseGameObject* pOwner = sObjectIds.Find_Impl(mOwnerId);
    if (!pOwner)
    {
        SetDead(true);
    }
}

s16 MotionDetector::IsInLaser(IBaseAliveGameObject* pWho, IBaseAliveGameObject* pOwner)
{
    if (pWho->Type() == ReliveTypes::eAbe)
    {
        const auto pAbe = static_cast<Abe*>(pWho);
        // Abe is safe in these states or if electrocuted or in ddcheat fly mode.
        if (pAbe->mCurrentMotion == eAbeMotions::Motion_0_Idle_44EEB0 || pAbe->mCurrentMotion == eAbeMotions::Motion_17_CrouchIdle_456BC0 || pAbe->mCurrentMotion == eAbeMotions::Motion_67_LedgeHang_454E20 || pAbe->mCurrentMotion == eAbeMotions::Motion_60_Unused_4A3200 || pAbe->mCurrentMotion == eAbeMotions::Motion_57_Dead_4589A0 || pAbe->mCurrentMotion == eAbeMotions::Motion_117_InMineCar || pAbe->GetElectrocuted() || gDDCheat_FlyingEnabled)
        {
            return 0;
        }
    }
    else if (pWho->Type() == ReliveTypes::eMudokon)
    {
        // For some reason when ddcheat is on muds are also invincible to lasers?
        if (gDDCheat_FlyingEnabled)
        {
            return 0;
        }

        // If mud isn't moving then he is safe.
        if (pWho->mVelX == FP_FromInteger(0) && pWho->mVelY == FP_FromInteger(0))
        {
            return 0;
        }
    }
    else if (FP_GetExponent(pWho->mVelX) == 0 && FP_GetExponent(pWho->mVelY) == 0)
    {
        // Whatever this is isn't moving
        return 0;
    }

    if (!(GetAnimation().GetRender()))
    {
        // Not being rendered so can't set off the motion beam
        return 0;
    }

    if (pOwner && (pWho->Type() != ReliveTypes::eMudokon && pWho->Type() != ReliveTypes::eAbe))
    {
        // If there is an owner (such as a greeter) then only muds and abe can set off the beam?
        return 0;
    }

    return 1;
}

void MotionDetector::VUpdate()
{
    MotionDetectorLaser* pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find(mLaserId, ReliveTypes::eRedLaser));
    Greeter* pOwner = static_cast<Greeter*>(sObjectIds.Find(mOwnerId, ReliveTypes::eGreeter));

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (!gNumCamSwappers)
    {
        if (!pOwner)
        {
            // A laser not part of greeter and disabled, do nothing.
            if (SwitchStates_Get(mDisableSwitchId))
            {
                pLaser->GetAnimation().SetRender(false);
                return;
            }
            pLaser->GetAnimation().SetRender(true);
        }

        const PSX_RECT bLaserRect = pLaser->VGetBoundingRect();

        mObjectInLaser = false;

        for (s32 idx = 0; idx < gBaseAliveGameObjects->Size(); idx++)
        {
            IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(idx);
            if (!pObj)
            {
                break;
            }

            if (pObj->Type() != ReliveTypes::eTimedMine_or_MovingBomb && (pObj->Type() == ReliveTypes::eAbe || pObj->Type() == ReliveTypes::eMudokon || !pOwner))
            {
                const PSX_RECT objRect = pObj->VGetBoundingRect();

                // Can't use PSX_Rects_overlap_no_adjustment because its checking <= and adjusting x/y
                if (bLaserRect.x <= (objRect.w - 8)
                    && bLaserRect.w >= (objRect.x + 8)
                    && bLaserRect.h >= objRect.y
                    && bLaserRect.y <= objRect.h
                    && pObj->GetSpriteScale() == GetSpriteScale())
                {
                    if (IsAbe(pObj))
                    {
                        if (sGnFrame % 2)
                        {
                            SfxPlayMono(relive::SoundEffects::Zap2, 45);
                        }
                    }

                    if (IsInLaser(pObj, pOwner))
                    {
                        mObjectInLaser = true;

                        if (pOwner == nullptr)
                        {
                            // Trigger alarms if its not already blasting
                            if (gAlarmInstanceCount == 0)
                            {
                                relive_new Alarm(mAlarmDuration, mAlarmSwitchId, 0, Layer::eLayer_Above_FG1_39);

                                if (IsAbe(pObj) && pObj->mHealth > FP_FromInteger(0))
                                {
                                    Mudokon_SFX(MudSounds::eOops_14, 0, 0, 0);
                                }
                            }
                        }
                        else
                        {
                            // Tell greeter to KILL
                            if (pLaser->mXPos <= pOwner->mXPos)
                            {
                                pOwner->field_13E_targetOnLeft = 1;
                            }
                            else
                            {
                                pOwner->field_140_targetOnRight = 1;
                            }

                            GetAnimation().SetRender(false);
                            pLaser->GetAnimation().SetRender(false);
                        }
                        break;
                    }
                }
            }
        }

        if (pOwner)
        {
            mXPos = pOwner->mXPos;
            mYPos = pOwner->mYPos - (GetSpriteScale() * FP_FromInteger(20));

            pLaser->mXPos += pOwner->mVelX;

            mTopLeftX = pOwner->mXPos - (GetSpriteScale() * FP_FromInteger(75));
            mBottomRightX = (GetSpriteScale() * FP_FromInteger(75)) + pOwner->mXPos;
            mTopLeftY = pOwner->mYPos - (GetSpriteScale() * FP_FromInteger(20));
            mBottomRightY = pOwner->mYPos;

            if (pOwner->mBrainState == GreeterBrainStates::eBrain_0_Patrol || pOwner->mBrainState == GreeterBrainStates::eBrain_1_PatrolTurn)
            {
                GetAnimation().SetRender(true);
                pLaser->GetAnimation().SetRender(true);
                pLaser->mYPos = pOwner->mYPos;
            }

            if (pOwner->mBrainState == GreeterBrainStates::eBrain_4_Chase || pOwner->mBrainState == GreeterBrainStates::eBrain_6_ToChase)
            {
                GetAnimation().SetRender(false);
                pLaser->GetAnimation().SetRender(false);
            }
        }

        switch (mState)
        {
            case States::eMoveRight_0:
                if (pLaser->mXPos >= mBottomRightX)
                {
                    mState = States::eWaitThenMoveLeft_1;
                    mPauseTimer = MakeTimer(15);
                    const CameraPos soundDirection = gMap.GetDirection(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos);
                    SFX_Play_Camera(relive::SoundEffects::MenuNavigation, 0, soundDirection, GetSpriteScale());
                }
                else
                {
                    pLaser->mXPos += mSpeed;
                }
                break;

            case States::eWaitThenMoveLeft_1:
                if (static_cast<s32>(sGnFrame) > mPauseTimer)
                {
                    mState = States::eMoveLeft_2;
                }
                break;

            case States::eMoveLeft_2:
                if (pLaser->mXPos <= mTopLeftX)
                {
                    mState = States::eWaitThenMoveRight_3;
                    mPauseTimer = MakeTimer(15);
                    const CameraPos soundDirection = gMap.GetDirection(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos);
                    SFX_Play_Camera(relive::SoundEffects::MenuNavigation, 0, soundDirection, GetSpriteScale());
                }
                else
                {
                    pLaser->mXPos -= mSpeed;
                }
                break;

            case States::eWaitThenMoveRight_3:
                if (static_cast<s32>(sGnFrame) > mPauseTimer)
                {
                    mState = States::eMoveRight_0;
                }
                break;
        }
    }
}

void MotionDetector::VRender(OrderingTable& ot)
{
    BaseAnimatedWithPhysicsGameObject::VRender(ot);

    if (GetAnimation().GetRender())
    {
        auto pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find(mLaserId, ReliveTypes::eRedLaser));
        const PSX_RECT bLaserRect = pLaser->VGetBoundingRect();

        const FP camXFp = gScreenManager->CamXPos();
        const FP camYFp = gScreenManager->CamYPos();

        const s16 screenX = FP_GetExponent(mXPos) - FP_GetExponent(camXFp);

        const s16 x0 = static_cast<s16>(PsxToPCX(screenX, 11));
        const s16 y0 = FP_GetExponent(mYPos) - FP_GetExponent(camYFp);
        const s16 y1 = FP_GetExponent(pLaser->mYPos - camYFp);
        const s16 y2 = y1 + bLaserRect.y - bLaserRect.h;
        const s16 x1 = PsxToPCX(FP_GetExponent(pLaser->mXPos - camXFp), 11);

        Poly_G3* pPrim = &mPrims;

        pPrim->SetXY0(x0, y0);
        pPrim->SetXY1(x1, y1);
        pPrim->SetXY2(x1, y2);

        pPrim->SetRGB0(80, 0, 0);
        pPrim->SetRGB1(80, 0, 0);
        pPrim->SetRGB2(80, 0, 0);

        // Add triangle
        pPrim->SetSemiTransparent(true);
        // When detected transparency is off, gives the "solid red" triangle
        pPrim->SetBlendMode(mObjectInLaser ? relive::TBlendModes::eBlend_1 : relive::TBlendModes::eBlend_3);
        ot.Add(GetAnimation().GetRenderLayer(), pPrim);
    }
}
