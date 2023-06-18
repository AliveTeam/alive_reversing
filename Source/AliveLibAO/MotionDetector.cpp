#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "MotionDetector.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "CameraSwapper.hpp"
#include "Abe.hpp"
#include "DDCheat.hpp"
#include "Game.hpp"
#include "Alarm.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/FatalError.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Path.hpp"

namespace AO {

#undef min
#undef max

MotionDetector::MotionDetector(relive::Path_MotionDetector* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eMotionDetector);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MotionDetector_Flare));
    Animation_Init(GetAnimRes(AnimId::MotionDetector_Flare));

    GetAnimation().SetSwapXY(true);
    GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
    GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);
    mYOffset = 0;
    mRGB.SetRGB(64, 0, 0);
    mObjectInLaser = false;
    mDontComeBack = true;
    mTlvId = tlvId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
    }

    mTopLeftX = FP_FromInteger(pTlv->mTopLeftX);
    mBottomRightX = FP_FromInteger(pTlv->mBottomRightX);

    mTopLeftY = FP_FromInteger(pTlv->mTopLeftY);
    mBottomRightY = FP_FromInteger(pTlv->mBottomRightY);

    mXPos = FP_FromInteger(pTlv->mDeviceX);
    mYPos = FP_FromInteger(pTlv->mDeviceY);

    mSpeed = FP_FromRaw(pTlv->mSpeedx256 << 8);

    MotionDetectorLaser* pMotionDetectors = nullptr;
    if (pTlv->mInitialMoveDirection == relive::Path_MotionDetector::InitialMoveDirection::eRight)
    {
        mState = States::eMoveRight_0;
        pMotionDetectors = relive_new MotionDetectorLaser();
        if (pMotionDetectors)
        {
            pMotionDetectors->SetType(ReliveTypes::eRedLaser);
            
            pMotionDetectors->Animation_Init(pMotionDetectors->GetAnimRes(AnimId::MotionDetector_Laser));
            
            pMotionDetectors->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
            pMotionDetectors->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);

            pMotionDetectors->mXPos = mTopLeftX;
            pMotionDetectors->mYPos = mBottomRightY;

            pMotionDetectors->SetSpriteScale(GetSpriteScale());
            pMotionDetectors->mYOffset = 0;
            mLaserId = pMotionDetectors->mBaseGameObjectId;
        }
    }
    else if (pTlv->mInitialMoveDirection == relive::Path_MotionDetector::InitialMoveDirection::eLeft)
    {
        mState = States::eMoveLeft_2;
        pMotionDetectors = relive_new MotionDetectorLaser();
        if (pMotionDetectors)
        {
            pMotionDetectors->SetType(ReliveTypes::eRedLaser);
            
            pMotionDetectors->Animation_Init(pMotionDetectors->GetAnimRes(AnimId::MotionDetector_Laser));
            
            pMotionDetectors->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
            pMotionDetectors->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);
            pMotionDetectors->mXPos = mBottomRightX;
            pMotionDetectors->mYPos = mBottomRightY;
            pMotionDetectors->SetSpriteScale(GetSpriteScale());
            pMotionDetectors->mYOffset = 0;
            mLaserId = pMotionDetectors->mBaseGameObjectId;
        }
    }
    else
    {
        ALIVE_FATAL("couldn't find start move direction for motion detector");
    }

    mDisableSwitchId = pTlv->mDisableSwitchId;

    pMotionDetectors->GetAnimation().SetRender(SwitchStates_Get(mDisableSwitchId) == 0);

    GetAnimation().SetRender(pTlv->mDrawFlare == relive::reliveChoice::eYes);

    mAlarmDuration = pTlv->mAlarmDuration;

    mAlarmSwitchId = pTlv->mAlarmSwitchId;
}

void MotionDetector::SetDontComeBack(bool bDontComeBack)
{
    mDontComeBack = bDontComeBack;
}

MotionDetector::~MotionDetector()
{
    if (mDontComeBack)
    {
        Path::TLV_Reset(mTlvId, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvId, -1, 0, 1);
    }

    BaseGameObject* pLaser = sObjectIds.Find_Impl(mLaserId);
    if (pLaser)
    {
        pLaser->SetDead(true);
    }
}

void MotionDetector::VScreenChanged()
{
    SetDead(true);
}

void MotionDetector::VUpdate()
{
    MotionDetectorLaser* pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find_Impl(mLaserId));
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (!gNumCamSwappers)
    {
        if (SwitchStates_Get(mDisableSwitchId))
        {
            pLaser->GetAnimation().SetRender(false);
        }
        else
        {
            pLaser->GetAnimation().SetRender(true);

            const PSX_RECT bLaserRect = pLaser->VGetBoundingRect();

            mObjectInLaser = false;

            for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
            {
                IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->Type() != ReliveTypes::eTimedMine)
                {
                    const PSX_RECT objRect = pObj->VGetBoundingRect();

                    if (bLaserRect.x <= (objRect.w - 8) 
                        && bLaserRect.w >= (objRect.x + 8) 
                        && bLaserRect.h >= objRect.y 
                        && bLaserRect.y <= objRect.h 
                        && pObj->GetSpriteScale() == GetSpriteScale())
                    {
                        if (pObj == sActiveHero)
                        {
                            if (sGnFrame % 2)
                            {
                                SfxPlayMono(relive::SoundEffects::Zap2, 45);
                            }
                        }

                        bool alarm = false;
                        if (pObj->Type() == ReliveTypes::eAbe)
                        {
                            if (pObj->mCurrentMotion != eAbeMotions::Motion_0_Idle && pObj->mCurrentMotion != eAbeMotions::Motion_19_CrouchIdle && pObj->mCurrentMotion != eAbeMotions::Motion_66_LedgeHang && pObj->mCurrentMotion != eAbeMotions::Motion_62_LoadedSaveSpawn && pObj->mCurrentMotion != eAbeMotions::Motion_60_Dead && !gDDCheat_FlyingEnabled)
                            {
                                alarm = true;
                            }
                        }
                        else if (FP_GetExponent(pObj->mVelX) || FP_GetExponent(pObj->mVelY))
                        {
                            alarm = true;
                        }

                        if (alarm)
                        {
                            mObjectInLaser = true;

                            if (gAlarmInstanceCount == 0)
                            {
                                relive_new Alarm(
                                    mAlarmDuration,
                                    mAlarmSwitchId,
                                    0,
                                    Layer::eLayer_Above_FG1_39);

                                if (pObj == sActiveHero)
                                {
                                    Mudokon_SFX(MudSounds::eOops_16, 0, 0, nullptr);
                                }
                            }
                        }
                    }
                }
            }


            switch (mState)
            {
                case States::eMoveRight_0:
                    if (pLaser->mXPos >= mBottomRightX)
                    {
                        mState = States::eWaitThenMoveLeft_1;
                        mPauseTimer = MakeTimer(15);
                        SfxPlayMono(relive::SoundEffects::MenuNavigation, 0);
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
                        SfxPlayMono(relive::SoundEffects::MenuNavigation, 0);
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

                default:
                    return;
            }
        }
    }
}

void MotionDetector::VRender(PrimHeader** ppOt)
{
    mXPos += FP_FromInteger(11);
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    mXPos -= FP_FromInteger(11);

    if (!SwitchStates_Get(mDisableSwitchId))
    {
        const s16 screen_top = FP_GetExponent(gScreenManager->mCamPos->y - FP_FromInteger(gScreenManager->mCamYOff));

        const s16 screen_left = FP_GetExponent(gScreenManager->mCamPos->x - FP_FromInteger(gScreenManager->mCamXOff));

        auto pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find_Impl(mLaserId));
        const PSX_RECT bLaserRect = pLaser->VGetBoundingRect();

        const s16 x0 = static_cast<s16>(PsxToPCX(FP_GetExponent(mXPos) - screen_left, 11));
        const s16 y0 = FP_GetExponent(mYPos) - screen_top;
        const s16 y1 = FP_GetExponent(pLaser->mYPos) - screen_top;
        const s16 y2 = y1 + bLaserRect.y - bLaserRect.h;
        const s16 x1 = static_cast<s16>(PsxToPCX(FP_GetExponent(pLaser->mXPos) - screen_left, 11));

        Poly_G3* pPrim = &mPrims[gPsxDisplay.mBufferIndex];
        PolyG3_Init(pPrim);

        SetXY0(pPrim, x0, y0);
        SetXY1(pPrim, x1, y1);
        SetXY2(pPrim, x1, y2);

        SetRGB0(pPrim, 64, 0, 0);
        SetRGB1(pPrim, 64, 0, 0);
        SetRGB2(pPrim, 64, 0, 0);

        // Add triangle
        Poly_Set_SemiTrans(&pPrim->mBase.header, true);
        OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pPrim->mBase.header);

        // Add tpage
        Init_SetTPage(&mTPage[gPsxDisplay.mBufferIndex], PSX_getTPage(mObjectInLaser != 0 ? TPageAbr::eBlend_1 : TPageAbr::eBlend_3)); // When detected transparency is off, gives the "solid red" triangle
        OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &mTPage[gPsxDisplay.mBufferIndex].mBase);
    }
}

} // namespace AO
