#include "stdafx_ao.h"
#include "Function.hpp"
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
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../AliveLibCommon/FatalError.hpp"
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

    GetAnimation().mFlags.Set(AnimFlags::eSwapXY);
    GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
    GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);
    mYOffset = 0;
    mRGB.SetRGB(64, 0, 0);
    field_160_bObjectInLaser = 0;
    field_F6_bDontComeBack = true;
    field_E4_tlvInfo = tlvId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
    }

    field_F8_top_left_x = FP_FromInteger(pTlv->mTopLeftX);
    field_100_bottom_right_x = FP_FromInteger(pTlv->mBottomRightX);

    field_FC_top_left_y = FP_FromInteger(pTlv->mTopLeftY);
    field_104_bottom_right_y = FP_FromInteger(pTlv->mBottomRightY);

    mXPos = FP_FromInteger(pTlv->mDeviceX);
    mYPos = FP_FromInteger(pTlv->mDeviceY);

    field_15C_speed = FP_FromRaw(pTlv->mSpeedx256 << 8);

    MotionDetectorLaser* pMotionDetectors = nullptr;
    if (pTlv->mInitialMoveDirection == relive::Path_MotionDetector::InitialMoveDirection::eRight)
    {
        field_E8_state = States::eMoveRight_0;
        pMotionDetectors = relive_new MotionDetectorLaser();
        if (pMotionDetectors)
        {
            pMotionDetectors->SetType(ReliveTypes::eRedLaser);
            
            pMotionDetectors->Animation_Init(pMotionDetectors->GetAnimRes(AnimId::MotionDetector_Laser));
            
            pMotionDetectors->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
            pMotionDetectors->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);

            pMotionDetectors->mXPos = field_F8_top_left_x;
            pMotionDetectors->mYPos = field_104_bottom_right_y;

            pMotionDetectors->SetSpriteScale(GetSpriteScale());
            pMotionDetectors->mYOffset = 0;
            field_F8_laser_id = pMotionDetectors->mBaseGameObjectId;
        }
    }
    else if (pTlv->mInitialMoveDirection == relive::Path_MotionDetector::InitialMoveDirection::eLeft)
    {
        field_E8_state = States::eMoveLeft_2;
        pMotionDetectors = relive_new MotionDetectorLaser();
        if (pMotionDetectors)
        {
            pMotionDetectors->SetType(ReliveTypes::eRedLaser);
            
            pMotionDetectors->Animation_Init(pMotionDetectors->GetAnimRes(AnimId::MotionDetector_Laser));
            
            pMotionDetectors->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
            pMotionDetectors->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);
            pMotionDetectors->mXPos = field_100_bottom_right_x;
            pMotionDetectors->mYPos = field_104_bottom_right_y;
            pMotionDetectors->SetSpriteScale(GetSpriteScale());
            pMotionDetectors->mYOffset = 0;
            field_F8_laser_id = pMotionDetectors->mBaseGameObjectId;
        }
    }
    else
    {
        ALIVE_FATAL("couldn't find start move direction for motion detector");
    }

    field_F0_disable_switch_id = pTlv->mDisableSwitchId;

    pMotionDetectors->GetAnimation().mFlags.Set(AnimFlags::eRender, SwitchStates_Get(field_F0_disable_switch_id) == 0);

    GetAnimation().mFlags.Set(AnimFlags::eRender, pTlv->mDrawFlare == relive::reliveChoice::eYes);

    field_F4_alarm_duration = pTlv->mAlarmDuration;

    field_F2_alarm_switch_id = pTlv->mAlarmSwitchId;
}

void MotionDetector::SetDontComeBack(bool bDontComeBack)
{
    field_F6_bDontComeBack = bDontComeBack;
}

MotionDetector::~MotionDetector()
{
    if (field_F6_bDontComeBack)
    {
        Path::TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(field_E4_tlvInfo, -1, 0, 1);
    }

    BaseGameObject* pLaser = sObjectIds.Find_Impl(field_F8_laser_id);
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
    MotionDetectorLaser* pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find_Impl(field_F8_laser_id));
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (!gNumCamSwappers)
    {
        if (SwitchStates_Get(field_F0_disable_switch_id))
        {
            pLaser->GetAnimation().mFlags.Clear(AnimFlags::eRender);
        }
        else
        {
            pLaser->GetAnimation().mFlags.Set(AnimFlags::eRender);

            const PSX_RECT laserRect = pLaser->VGetBoundingRect();

            field_160_bObjectInLaser = false;

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

                    if (laserRect.x <= (objRect.w - 8) && laserRect.w >= (objRect.x + 8) && laserRect.h >= objRect.y && laserRect.y <= objRect.h && pObj->GetSpriteScale() == GetSpriteScale())
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
                            field_160_bObjectInLaser = true;

                            if (gAlarmInstanceCount == 0)
                            {
                                relive_new Alarm(
                                    field_F4_alarm_duration,
                                    field_F2_alarm_switch_id,
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


            switch (field_E8_state)
            {
                case States::eMoveRight_0:
                    if (pLaser->mXPos >= field_100_bottom_right_x)
                    {
                        field_E8_state = States::eWaitThenMoveLeft_1;
                        field_EC_timer = sGnFrame + 15;
                        SfxPlayMono(relive::SoundEffects::MenuNavigation, 0);
                    }
                    else
                    {
                        pLaser->mXPos += field_15C_speed;
                    }
                    break;

                case States::eWaitThenMoveLeft_1:
                    if (static_cast<s32>(sGnFrame) > field_EC_timer)
                    {
                        field_E8_state = States::eMoveLeft_2;
                    }
                    break;

                case States::eMoveLeft_2:
                    if (pLaser->mXPos <= field_F8_top_left_x)
                    {
                        field_E8_state = States::eWaitThenMoveRight_3;
                        field_EC_timer = sGnFrame + 15;
                        SfxPlayMono(relive::SoundEffects::MenuNavigation, 0);
                    }
                    else
                    {
                        pLaser->mXPos -= field_15C_speed;
                    }
                    break;

                case States::eWaitThenMoveRight_3:
                    if (static_cast<s32>(sGnFrame) > field_EC_timer)
                    {
                        field_E8_state = States::eMoveRight_0;
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

    if (!SwitchStates_Get(field_F0_disable_switch_id))
    {
        const s16 screen_top = FP_GetExponent(pScreenManager->mCamPos->y - FP_FromInteger(pScreenManager->mCamYOff));

        const s16 screen_left = FP_GetExponent(pScreenManager->mCamPos->x - FP_FromInteger(pScreenManager->mCamXOff));

        auto pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find_Impl(field_F8_laser_id));
        const PSX_RECT bLaserRect = pLaser->VGetBoundingRect();

        const s16 x0 = static_cast<s16>(PsxToPCX(FP_GetExponent(mXPos) - screen_left, 11));
        const s16 y0 = FP_GetExponent(mYPos) - screen_top;
        const s16 y1 = FP_GetExponent(pLaser->mYPos) - screen_top;
        const s16 y2 = y1 + bLaserRect.y - bLaserRect.h;
        const s16 x1 = static_cast<s16>(PsxToPCX(FP_GetExponent(pLaser->mXPos) - screen_left, 11));

        Poly_G3* pPrim = &field_10C_prims[gPsxDisplay.mBufferIndex];
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
        Init_SetTPage(&field_13C_tPage[gPsxDisplay.mBufferIndex], 0, 0, PSX_getTPage(field_160_bObjectInLaser != 0 ? TPageAbr::eBlend_1 : TPageAbr::eBlend_3)); // When detected transparency is off, gives the "solid red" triangle
        OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &field_13C_tPage[gPsxDisplay.mBufferIndex].mBase);
    }
}

} // namespace AO
