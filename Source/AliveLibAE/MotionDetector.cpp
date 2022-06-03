#include "stdafx.h"
#include "MotionDetector.hpp"
#include "stdlib.hpp"
#include "ObjectIds.hpp"
#include "SwitchStates.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "BaseAliveGameObject.hpp"
#include "DDCheat.hpp"
#include "Abe.hpp"
#include "Events.hpp"
#include "Greeter.hpp"
#include "Sfx.hpp"
#include "Alarm.hpp"
#include "Function.hpp"
#include "Sys_common.hpp"

MotionDetectorLaser::MotionDetectorLaser(FP xpos, FP ypos, FP scale, Layer layer)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eRedLaser_111);
    const AnimRecord& rec = AnimRec(AnimId::MotionDetector_Laser);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
    field_20_animation.mRenderLayer = layer;
    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    field_CC_sprite_scale = scale;
    field_20_animation.mRenderMode = TPageAbr::eBlend_1;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
}

// =====================================================================================

MotionDetector::MotionDetector(Path_MotionDetector* pTlv, s32 tlvInfo, BaseAnimatedWithPhysicsGameObject* pOwner)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eGreeterBody_91);

    const AnimRecord& rec = AnimRec(AnimId::MotionDetector_Flare);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.mRenderMode = TPageAbr::eBlend_1;
    field_20_animation.mRenderLayer = Layer::eLayer_Foreground_36;

    field_D8_yOffset = 0;

    field_D0_r = 64;
    field_D4_b = 0;
    field_D2_g = 0;

    field_178_bObjectInLaser = 0;

    if (!pOwner)
    {
        field_110_bDontComeBack = 1;
        field_10E_bUnknown = 0;
        field_FC_owner_id = -1;

        field_F4_tlvInfo = tlvInfo;
        field_CC_sprite_scale = FP_FromInteger(1);

        if (pTlv->field_10_scale != Scale_short::eFull_0)
        {
            field_CC_sprite_scale = FP_FromDouble(0.5);
        }

        field_114_x1_fp = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
        field_11C_y1_fp = FP_FromInteger(pTlv->field_C_bottom_right.field_0_x);
        field_118_x2_fp = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
        field_120_y2_fp = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);

        PSX_Point pos = {};
        gMap.Get_Abe_Spawn_Pos(&pos);
        if (pTlv->field_12_device_x)
        {
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_12_device_x - pos.field_0_x);
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_14_device_y - pos.field_2_y);
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
        }

        field_174_speed = FP_FromRaw((u16) pTlv->field_16_speed_x256 << 8);

        MotionDetectorLaser* pLaser = nullptr;
        if (pTlv->field_18_initial_move_direction == Path_MotionDetector::InitialMoveDirection::eLeft_1)
        {
            field_100_state = States::eMoveLeft_2;
            pLaser = ae_new<MotionDetectorLaser>(field_11C_y1_fp, field_120_y2_fp, field_CC_sprite_scale, Layer::eLayer_Foreground_36);
        }
        else if (pTlv->field_18_initial_move_direction == Path_MotionDetector::InitialMoveDirection::eRight_0)
        {
            field_100_state = States::eMoveRight_0;
            pLaser = ae_new<MotionDetectorLaser>(field_114_x1_fp, field_120_y2_fp, field_CC_sprite_scale, Layer::eLayer_Foreground_36);
        }
        else
        {
            ALIVE_FATAL("couldn't find start move direction for motion detector");
        }

        if (pTlv->field_1A_draw_flare == Choice_short::eYes_1)
        {
            field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render);
        }
        else
        {
            field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        }

        if (pLaser)
        {
            field_F8_laser_id = pLaser->field_8_object_id;
            field_108_disable_switch_id = pTlv->field_1C_disable_switch_id;

            if (SwitchStates_Get(field_108_disable_switch_id) == 0)
            {
                pLaser->field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render);
            }
            else
            {
                pLaser->field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            }
        }

        field_10A_alarm_switch_id = pTlv->field_1E_alarm_switch_id;
        field_10C_alarm_duration = pTlv->field_20_alarm_duration;
        return;
    }

    field_10E_bUnknown = 1;
    field_CC_sprite_scale = pOwner->field_CC_sprite_scale;

    field_114_x1_fp = pOwner->mBaseAnimatedWithPhysicsGameObject_XPos - (field_CC_sprite_scale * FP_FromInteger(75));
    field_11C_y1_fp = (field_CC_sprite_scale * FP_FromInteger(75)) + pOwner->mBaseAnimatedWithPhysicsGameObject_XPos;
    field_118_x2_fp = pOwner->mBaseAnimatedWithPhysicsGameObject_YPos - (field_CC_sprite_scale * FP_FromInteger(20));
    field_120_y2_fp = pOwner->mBaseAnimatedWithPhysicsGameObject_YPos;

    mBaseAnimatedWithPhysicsGameObject_XPos = pOwner->mBaseAnimatedWithPhysicsGameObject_XPos;
    mBaseAnimatedWithPhysicsGameObject_YPos = pOwner->mBaseAnimatedWithPhysicsGameObject_YPos - (field_CC_sprite_scale * FP_FromInteger(20));

    field_174_speed = FP_FromInteger(2);
    field_100_state = States::eMoveRight_0;

    auto pLaserMem = ae_new<MotionDetectorLaser>(pOwner->mBaseAnimatedWithPhysicsGameObject_XPos, pOwner->mBaseAnimatedWithPhysicsGameObject_YPos, field_CC_sprite_scale, Layer::eLayer_Foreground_36);
    if (pLaserMem)
    {
        field_F8_laser_id = pLaserMem->field_8_object_id;
    }

    field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render);
    field_FC_owner_id = pOwner->field_8_object_id;
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

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        auto pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find(field_F8_laser_id, AETypes::eRedLaser_111));
        PSX_RECT bLaserRect = {};
        pLaser->VGetBoundingRect(&bLaserRect, 1);

        const FP camXFp = pScreenManager_5BB5F4->field_20_pCamPos->field_0_x;
        const FP camYFp = pScreenManager_5BB5F4->field_20_pCamPos->field_4_y;

        const s16 screenX = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) - FP_GetExponent(camXFp);

        const s16 x0 = static_cast<s16>(PsxToPCX(screenX, 11));
        const s16 y0 = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) - FP_GetExponent(camYFp);
        const s16 y1 = FP_GetExponent(pLaser->mBaseAnimatedWithPhysicsGameObject_YPos - camYFp);
        const s16 y2 = y1 + bLaserRect.y - bLaserRect.h;
        const s16 x1 = PsxToPCX(FP_GetExponent(pLaser->mBaseAnimatedWithPhysicsGameObject_XPos - camXFp), 11);

        Poly_F3* pPrim = &field_124_prims[gPsxDisplay_5C1130.field_C_buffer_index];
        PolyF3_Init(pPrim);

        SetXY0(pPrim, x0, y0);
        SetXY1(pPrim, x1, y1);
        SetXY2(pPrim, x1, y2);

        SetRGB0(pPrim, 80, 0, 0);

        // Add triangle
        Poly_Set_SemiTrans_4F8A60(&pPrim->mBase.header, TRUE);
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_20_animation.mRenderLayer), &pPrim->mBase.header);

        // Add tpage
        const s32 tpage = PSX_getTPage_4F60E0(TPageMode::e16Bit_2, field_178_bObjectInLaser != 0 ? TPageAbr::eBlend_1 : TPageAbr::eBlend_3, 0, 0); // When detected transparency is off, gives the "solid red" triangle
        Prim_SetTPage* pTPage = &field_154_tPage[gPsxDisplay_5C1130.field_C_buffer_index];
        Init_SetTPage_4F5B60(pTPage, 0, 0, tpage);
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_20_animation.mRenderLayer), &pTPage->mBase);

        pScreenManager_5BB5F4->InvalidateRect_40EC90(
            std::min(x0, std::min(x1, x1)),
            std::min(y0, std::min(y1, y2)),
            std::max(x0, std::max(x1, x1)),
            std::max(y0, std::max(y1, y2)),
            pScreenManager_5BB5F4->field_3A_idx);
    }
}

s16 MotionDetector::IsInLaser(BaseAliveGameObject* pWho, BaseGameObject* pOwner)
{
    if (pWho->Type() == AETypes::eAbe_69)
    {
        // Abe is safe in these states or if electrocuted or in ddcheat fly mode.
        if (pWho->mCurrentMotion == eAbeMotions::Motion_0_Idle_44EEB0 || pWho->mCurrentMotion == eAbeMotions::Motion_17_CrouchIdle_456BC0 || pWho->mCurrentMotion == eAbeMotions::Motion_67_LedgeHang_454E20 || pWho->mCurrentMotion == eAbeMotions::Motion_60_Unused_4A3200 || pWho->mCurrentMotion == eAbeMotions::Motion_57_Dead_4589A0 || pWho->mCurrentMotion == eAbeMotions::Motion_117_InMineCar_4587C0 || pWho->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted) || sDDCheat_FlyingEnabled_5C2C08)
        {
            return 0;
        }
    }
    else if (pWho->Type() == AETypes::eMudokon_110)
    {
        // For some reason when ddcheat is on muds are also invincible to lasers?
        if (sDDCheat_FlyingEnabled_5C2C08)
        {
            return 0;
        }

        // If mud isn't moving then he is safe.
        if (pWho->field_C4_velx == FP_FromInteger(0) && pWho->field_C8_vely == FP_FromInteger(0))
        {
            return 0;
        }
    }
    else if (FP_GetExponent(pWho->field_C4_velx) == 0 && FP_GetExponent(pWho->field_C8_vely) == 0)
    {
        // Whatever this is isn't moving
        return 0;
    }

    if (!(field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render)))
    {
        // Not being rendered so can't set off the motion beam
        return 0;
    }

    if (pOwner && (pWho->Type() != AETypes::eMudokon_110 && pWho->Type() != AETypes::eAbe_69))
    {
        // If there is an owner (such as a greeter) then only muds and abe can set off the beam?
        return 0;
    }

    return 1;
}

void MotionDetector::VUpdate()
{
    MotionDetectorLaser* pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find(field_F8_laser_id, AETypes::eRedLaser_111));
    Greeter* pOwner = static_cast<Greeter*>(sObjectIds.Find(field_FC_owner_id, AETypes::eGreeter_64));

    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!sNum_CamSwappers_5C1B66)
    {
        if (!pOwner)
        {
            // A laser not part of greeter and disabled, do nothing.
            if (SwitchStates_Get(field_108_disable_switch_id))
            {
                pLaser->field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                return;
            }
            pLaser->field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render);
        }

        PSX_RECT bLaserRect = {};
        pLaser->VGetBoundingRect(&bLaserRect, 1);

        field_178_bObjectInLaser = 0;

        for (s32 idx = 0; idx < gBaseAliveGameObjects_5C1B7C->Size(); idx++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(idx);
            if (!pObj)
            {
                break;
            }

            if (pObj->Type() != AETypes::eTimedMine_or_MovingBomb_10 && (pObj->Type() == AETypes::eAbe_69 || pObj->Type() == AETypes::eMudokon_110 || !pOwner))
            {
                PSX_RECT objRect = {};
                pObj->VGetBoundingRect(&objRect, 1);

                // Can't use PSX_Rects_overlap_no_adjustment because its checking <= and adjusting x/y
                if (bLaserRect.x <= (objRect.w - 8)
                    && bLaserRect.w >= (objRect.x + 8)
                    && bLaserRect.h >= objRect.y
                    && bLaserRect.y <= objRect.h
                    && pObj->field_CC_sprite_scale == field_CC_sprite_scale)
                {
                    if (pObj == sActiveHero_5C1B68)
                    {
                        if (sGnFrame_5C1B84 % 2)
                        {
                            SFX_Play_Mono(SoundEffect::Zap2_50, 45);
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
                                ae_new<Alarm>(field_10C_alarm_duration, field_10A_alarm_switch_id, 0, Layer::eLayer_Above_FG1_39);

                                if (pObj == sActiveHero_5C1B68 && pObj->mHealth > FP_FromInteger(0))
                                {
                                    Mudokon_SFX(MudSounds::eOops_14, 0, 0, 0);
                                }
                            }
                        }
                        else
                        {
                            // Tell greeter to KILL
                            if (pLaser->mBaseAnimatedWithPhysicsGameObject_XPos <= pOwner->mBaseAnimatedWithPhysicsGameObject_XPos)
                            {
                                pOwner->field_13E_targetOnLeft = 1;
                            }
                            else
                            {
                                pOwner->field_140_targetOnRight = 1;
                            }

                            field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                            pLaser->field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                        }
                        break;
                    }
                }
            }
        }

        if (pOwner)
        {
            mBaseAnimatedWithPhysicsGameObject_XPos = pOwner->mBaseAnimatedWithPhysicsGameObject_XPos;
            mBaseAnimatedWithPhysicsGameObject_YPos = pOwner->mBaseAnimatedWithPhysicsGameObject_YPos - (field_CC_sprite_scale * FP_FromInteger(20));

            pLaser->mBaseAnimatedWithPhysicsGameObject_XPos += pOwner->field_C4_velx;

            field_114_x1_fp = pOwner->mBaseAnimatedWithPhysicsGameObject_XPos - (field_CC_sprite_scale * FP_FromInteger(75));
            field_11C_y1_fp = (field_CC_sprite_scale * FP_FromInteger(75)) + pOwner->mBaseAnimatedWithPhysicsGameObject_XPos;
            field_118_x2_fp = pOwner->mBaseAnimatedWithPhysicsGameObject_YPos - (field_CC_sprite_scale * FP_FromInteger(20));
            field_120_y2_fp = pOwner->mBaseAnimatedWithPhysicsGameObject_YPos;

            if (pOwner->field_13C_brain_state == GreeterBrainStates::eBrain_0_Patrol || pOwner->field_13C_brain_state == GreeterBrainStates::eBrain_1_PatrolTurn)
            {
                field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render);
                pLaser->field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render);
                pLaser->mBaseAnimatedWithPhysicsGameObject_YPos = pOwner->mBaseAnimatedWithPhysicsGameObject_YPos;
            }

            if (pOwner->field_13C_brain_state == GreeterBrainStates::eBrain_4_Chase || pOwner->field_13C_brain_state == GreeterBrainStates::eBrain_6_ToChase)
            {
                field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                pLaser->field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            }
        }

        switch (field_100_state)
        {
            case States::eMoveRight_0:
                if (pLaser->mBaseAnimatedWithPhysicsGameObject_XPos >= field_11C_y1_fp)
                {
                    field_100_state = States::eWaitThenMoveLeft_1;
                    field_104_timer = sGnFrame_5C1B84 + 15;
                    const CameraPos soundDirection = gMap.GetDirection_4811A0(
                        field_C2_lvl_number,
                        field_C0_path_number,
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos);
                    SFX_Play_Camera(SoundEffect::MenuNavigation_52, 0, soundDirection, field_CC_sprite_scale);
                }
                else
                {
                    pLaser->mBaseAnimatedWithPhysicsGameObject_XPos += field_174_speed;
                }
                break;

            case States::eWaitThenMoveLeft_1:
                if (static_cast<s32>(sGnFrame_5C1B84) > field_104_timer)
                {
                    field_100_state = States::eMoveLeft_2;
                }
                break;

            case States::eMoveLeft_2:
                if (pLaser->mBaseAnimatedWithPhysicsGameObject_XPos <= field_114_x1_fp)
                {
                    field_100_state = States::eWaitThenMoveRight_3;
                    field_104_timer = sGnFrame_5C1B84 + 15;
                    const CameraPos soundDirection = gMap.GetDirection_4811A0(
                        field_C2_lvl_number,
                        field_C0_path_number,
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos);
                    SFX_Play_Camera(SoundEffect::MenuNavigation_52, 0, soundDirection, field_CC_sprite_scale);
                }
                else
                {
                    pLaser->mBaseAnimatedWithPhysicsGameObject_XPos -= field_174_speed;
                }
                break;

            case States::eWaitThenMoveRight_3:
                if (static_cast<s32>(sGnFrame_5C1B84) > field_104_timer)
                {
                    field_100_state = States::eMoveRight_0;
                }
                break;
        }
    }
}
