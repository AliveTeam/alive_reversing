#include "stdafx.h"
#include "MotionDetector.hpp"
#include "stdlib.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "SwitchStates.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "BaseAliveGameObject.hpp"
#include "DDCheat.hpp"
#include "Abe.hpp"
#include "../relive_lib/Events.hpp"
#include "Greeter.hpp"
#include "Sfx.hpp"
#include "Alarm.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/FatalError.hpp"

MotionDetectorLaser::MotionDetectorLaser(FP xpos, FP ypos, FP scale, Layer layer)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eRedLaser);
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MotionDetector_Laser));
    Animation_Init(GetAnimRes(AnimId::MotionDetector_Laser));
    GetAnimation().SetRenderLayer(layer);
    mXPos = xpos;
    SetSpriteScale(scale);
    GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
    mYPos = ypos;
}

// =====================================================================================

MotionDetector::MotionDetector(relive::Path_MotionDetector* pTlv, const Guid& tlvId, BaseAnimatedWithPhysicsGameObject* pOwner)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eGreeterBody);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MotionDetector_Flare));
    Animation_Init(GetAnimRes(AnimId::MotionDetector_Flare));

    GetAnimation().mFlags.Set(AnimFlags::eSemiTrans);
    GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
    GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);

    mYOffset = 0;

    mRGB.SetRGB(64, 0, 0);

    field_178_bObjectInLaser = 0;

    if (!pOwner)
    {
        field_110_bDontComeBack = 1;
        field_10E_bUnknown = 0;
        field_FC_owner_id = Guid{};

        field_F4_tlvInfo = tlvId;
        SetSpriteScale(FP_FromInteger(1));

        if (pTlv->mScale != relive::reliveScale::eFull)
        {
            SetSpriteScale(FP_FromDouble(0.5));
        }

        field_114_x1_fp = FP_FromInteger(pTlv->mTopLeftX);
        field_11C_y1_fp = FP_FromInteger(pTlv->mBottomRightX);
        field_118_x2_fp = FP_FromInteger(pTlv->mTopLeftY);
        field_120_y2_fp = FP_FromInteger(pTlv->mBottomRightY);

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

        field_174_speed = FP_FromRaw((u16) pTlv->mSpeedx256 << 8);

        MotionDetectorLaser* pLaser = nullptr;
        if (pTlv->mInitialMoveDirection == relive::Path_MotionDetector::InitialMoveDirection::eLeft)
        {
            field_100_state = States::eMoveLeft_2;
            pLaser = relive_new MotionDetectorLaser(field_11C_y1_fp, field_120_y2_fp, GetSpriteScale(), Layer::eLayer_Foreground_36);
        }
        else if (pTlv->mInitialMoveDirection == relive::Path_MotionDetector::InitialMoveDirection::eRight)
        {
            field_100_state = States::eMoveRight_0;
            pLaser = relive_new MotionDetectorLaser(field_114_x1_fp, field_120_y2_fp, GetSpriteScale(), Layer::eLayer_Foreground_36);
        }
        else
        {
            ALIVE_FATAL("couldn't find start move direction for motion detector");
        }

        if (pTlv->mDrawFlare == relive::reliveChoice::eYes)
        {
            GetAnimation().mFlags.Set(AnimFlags::eRender);
        }
        else
        {
            GetAnimation().mFlags.Clear(AnimFlags::eRender);
        }

        if (pLaser)
        {
            field_F8_laser_id = pLaser->mBaseGameObjectId;
            field_108_disable_switch_id = pTlv->mDisableSwitchId;

            if (SwitchStates_Get(field_108_disable_switch_id) == 0)
            {
                pLaser->GetAnimation().mFlags.Set(AnimFlags::eRender);
            }
            else
            {
                pLaser->GetAnimation().mFlags.Clear(AnimFlags::eRender);
            }
        }

        field_10A_alarm_switch_id = pTlv->mAlarmSwitchId;
        field_10C_alarm_duration = pTlv->mAlarmDuration;
        return;
    }

    field_10E_bUnknown = 1;
    SetSpriteScale(pOwner->GetSpriteScale());

    field_114_x1_fp = pOwner->mXPos - (GetSpriteScale() * FP_FromInteger(75));
    field_11C_y1_fp = (GetSpriteScale() * FP_FromInteger(75)) + pOwner->mXPos;
    field_118_x2_fp = pOwner->mYPos - (GetSpriteScale() * FP_FromInteger(20));
    field_120_y2_fp = pOwner->mYPos;

    mXPos = pOwner->mXPos;
    mYPos = pOwner->mYPos - (GetSpriteScale() * FP_FromInteger(20));

    field_174_speed = FP_FromInteger(2);
    field_100_state = States::eMoveRight_0;

    auto pLaserMem = relive_new MotionDetectorLaser(pOwner->mXPos, pOwner->mYPos, GetSpriteScale(), Layer::eLayer_Foreground_36);
    if (pLaserMem)
    {
        field_F8_laser_id = pLaserMem->mBaseGameObjectId;
    }

    GetAnimation().mFlags.Set(AnimFlags::eRender);
    field_FC_owner_id = pOwner->mBaseGameObjectId;
    field_10A_alarm_switch_id = 0;
    field_10C_alarm_duration = 0;
}

MotionDetector::~MotionDetector()
{
    if (!field_10E_bUnknown)
    {
        if (field_110_bDontComeBack)
        {
            Path::TLV_Reset(field_F4_tlvInfo, -1, 0, 0);
        }
        else
        {
            Path::TLV_Reset(field_F4_tlvInfo, -1, 0, 1);
        }
    }

    BaseGameObject* pLaser = sObjectIds.Find_Impl(field_F8_laser_id);
    if (pLaser)
    {
        pLaser->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void MotionDetector::VScreenChanged()
{
    BaseGameObject::VScreenChanged();

    BaseGameObject* pOwner = sObjectIds.Find_Impl(field_FC_owner_id);
    if (!pOwner)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void MotionDetector::VRender(PrimHeader** ppOt)
{
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

    if (GetAnimation().mFlags.Get(AnimFlags::eRender))
    {
        auto pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find(field_F8_laser_id, ReliveTypes::eRedLaser));
        const PSX_RECT bLaserRect = pLaser->VGetBoundingRect();

        const FP camXFp = pScreenManager->CamXPos();
        const FP camYFp = pScreenManager->CamYPos();

        const s16 screenX = FP_GetExponent(mXPos) - FP_GetExponent(camXFp);

        const s16 x0 = static_cast<s16>(PsxToPCX(screenX, 11));
        const s16 y0 = FP_GetExponent(mYPos) - FP_GetExponent(camYFp);
        const s16 y1 = FP_GetExponent(pLaser->mYPos - camYFp);
        const s16 y2 = y1 + bLaserRect.y - bLaserRect.h;
        const s16 x1 = PsxToPCX(FP_GetExponent(pLaser->mXPos - camXFp), 11);

        Poly_F3* pPrim = &field_124_prims[gPsxDisplay.mBufferIndex];
        PolyF3_Init(pPrim);

        SetXY0(pPrim, x0, y0);
        SetXY1(pPrim, x1, y1);
        SetXY2(pPrim, x1, y2);

        SetRGB0(pPrim, 80, 0, 0);

        // Add triangle
        Poly_Set_SemiTrans(&pPrim->mBase.header, true);
        OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pPrim->mBase.header);

        // Add tpage
        const s32 tpage = PSX_getTPage(field_178_bObjectInLaser != 0 ? TPageAbr::eBlend_1 : TPageAbr::eBlend_3); // When detected transparency is off, gives the "solid red" triangle
        Prim_SetTPage* pTPage = &field_154_tPage[gPsxDisplay.mBufferIndex];
        Init_SetTPage(pTPage, 0, 0, tpage);
        OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pTPage->mBase);
    }
}

s16 MotionDetector::IsInLaser(IBaseAliveGameObject* pWho, IBaseAliveGameObject* pOwner)
{
    if (pWho->Type() == ReliveTypes::eAbe)
    {
        // Abe is safe in these states or if electrocuted or in ddcheat fly mode.
        if (pWho->mCurrentMotion == eAbeMotions::Motion_0_Idle_44EEB0 || pWho->mCurrentMotion == eAbeMotions::Motion_17_CrouchIdle_456BC0 || pWho->mCurrentMotion == eAbeMotions::Motion_67_LedgeHang_454E20 || pWho->mCurrentMotion == eAbeMotions::Motion_60_Unused_4A3200 || pWho->mCurrentMotion == eAbeMotions::Motion_57_Dead_4589A0 || pWho->mCurrentMotion == eAbeMotions::Motion_117_InMineCar || pWho->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eElectrocuted) || sDDCheat_FlyingEnabled_5C2C08)
        {
            return 0;
        }
    }
    else if (pWho->Type() == ReliveTypes::eMudokon)
    {
        // For some reason when ddcheat is on muds are also invincible to lasers?
        if (sDDCheat_FlyingEnabled_5C2C08)
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

    if (!(GetAnimation().mFlags.Get(AnimFlags::eRender)))
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
    MotionDetectorLaser* pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find(field_F8_laser_id, ReliveTypes::eRedLaser));
    Greeter* pOwner = static_cast<Greeter*>(sObjectIds.Find(field_FC_owner_id, ReliveTypes::eGreeter));

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!gNumCamSwappers)
    {
        if (!pOwner)
        {
            // A laser not part of greeter and disabled, do nothing.
            if (SwitchStates_Get(field_108_disable_switch_id))
            {
                pLaser->GetAnimation().mFlags.Clear(AnimFlags::eRender);
                return;
            }
            pLaser->GetAnimation().mFlags.Set(AnimFlags::eRender);
        }

        const PSX_RECT bLaserRect = pLaser->VGetBoundingRect();

        field_178_bObjectInLaser = 0;

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
                    if (IsActiveHero(pObj))
                    {
                        if (sGnFrame % 2)
                        {
                            SfxPlayMono(relive::SoundEffects::Zap2, 45);
                        }
                    }

                    if (IsInLaser(pObj, pOwner))
                    {
                        field_178_bObjectInLaser = 1;

                        if (pOwner == nullptr)
                        {
                            // Trigger alarms if its not already blasting
                            if (alarmInstanceCount_5C1BB4 == 0)
                            {
                                relive_new Alarm(field_10C_alarm_duration, field_10A_alarm_switch_id, 0, Layer::eLayer_Above_FG1_39);

                                if (IsActiveHero(pObj) && pObj->mHealth > FP_FromInteger(0))
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

                            GetAnimation().mFlags.Clear(AnimFlags::eRender);
                            pLaser->GetAnimation().mFlags.Clear(AnimFlags::eRender);
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

            field_114_x1_fp = pOwner->mXPos - (GetSpriteScale() * FP_FromInteger(75));
            field_11C_y1_fp = (GetSpriteScale() * FP_FromInteger(75)) + pOwner->mXPos;
            field_118_x2_fp = pOwner->mYPos - (GetSpriteScale() * FP_FromInteger(20));
            field_120_y2_fp = pOwner->mYPos;

            if (pOwner->field_13C_brain_state == GreeterBrainStates::eBrain_0_Patrol || pOwner->field_13C_brain_state == GreeterBrainStates::eBrain_1_PatrolTurn)
            {
                GetAnimation().mFlags.Set(AnimFlags::eRender);
                pLaser->GetAnimation().mFlags.Set(AnimFlags::eRender);
                pLaser->mYPos = pOwner->mYPos;
            }

            if (pOwner->field_13C_brain_state == GreeterBrainStates::eBrain_4_Chase || pOwner->field_13C_brain_state == GreeterBrainStates::eBrain_6_ToChase)
            {
                GetAnimation().mFlags.Clear(AnimFlags::eRender);
                pLaser->GetAnimation().mFlags.Clear(AnimFlags::eRender);
            }
        }

        switch (field_100_state)
        {
            case States::eMoveRight_0:
                if (pLaser->mXPos >= field_11C_y1_fp)
                {
                    field_100_state = States::eWaitThenMoveLeft_1;
                    field_104_timer = sGnFrame + 15;
                    const CameraPos soundDirection = gMap.GetDirection(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos);
                    SFX_Play_Camera(relive::SoundEffects::MenuNavigation, 0, soundDirection, GetSpriteScale());
                }
                else
                {
                    pLaser->mXPos += field_174_speed;
                }
                break;

            case States::eWaitThenMoveLeft_1:
                if (static_cast<s32>(sGnFrame) > field_104_timer)
                {
                    field_100_state = States::eMoveLeft_2;
                }
                break;

            case States::eMoveLeft_2:
                if (pLaser->mXPos <= field_114_x1_fp)
                {
                    field_100_state = States::eWaitThenMoveRight_3;
                    field_104_timer = sGnFrame + 15;
                    const CameraPos soundDirection = gMap.GetDirection(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos);
                    SFX_Play_Camera(relive::SoundEffects::MenuNavigation, 0, soundDirection, GetSpriteScale());
                }
                else
                {
                    pLaser->mXPos -= field_174_speed;
                }
                break;

            case States::eWaitThenMoveRight_3:
                if (static_cast<s32>(sGnFrame) > field_104_timer)
                {
                    field_100_state = States::eMoveRight_0;
                }
                break;
        }
    }
}
