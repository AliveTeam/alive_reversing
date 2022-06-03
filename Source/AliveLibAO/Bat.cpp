#include "stdafx_ao.h"
#include "Function.hpp"
#include "Bat.hpp"
#include "ResourceManager.hpp"
#include "Collisions.hpp"
#include "Math.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "CameraSwapper.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"

namespace AO {

Bat::Bat(Path_Bat* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = Types::eBat_6;

    const AnimRecord& rec = AO::AnimRec(AnimId::Bat);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    
    if (!ppRes)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    field_10_anim.field_C_layer = Layer::eLayer_SligBat_33;

    FP hitX = {};
    FP hitY = {};
    // OG Bug fix, if bat isn't spawned on a line then we crash
    // so field_E4_pLine is checked here and in VUpdate
    sCollisions_DArray_504C6C->RayCast(
        FP_FromInteger(pTlv->field_10_top_left.field_0_x),
        FP_FromInteger(pTlv->field_10_top_left.field_2_y),
        FP_FromInteger(pTlv->field_14_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_14_bottom_right.field_2_y),
        &field_E4_pLine,
        &hitX,
        &hitY,
        0x100);

    field_F0_tlvInfo = tlvInfo;

    if (field_E4_pLine)
    {
        field_A8_xpos = FP_FromInteger(field_E4_pLine->field_0_rect.x);
        field_AC_ypos = FP_FromInteger(field_E4_pLine->field_0_rect.y);
    }

    field_EC_ticks_before_moving = pTlv->field_18_ticks_before_moving;
    field_E8_speed = FP_FromRaw(pTlv->field_1A_speed << 8);

    if (pTlv->field_1C_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
        field_10_anim.field_C_layer = Layer::eLayer_BeforeShadow_Half_6;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
        field_10_anim.field_C_layer = Layer::eLayer_BeforeShadow_25;
    }

    field_F4_state = BatStates::eSetTimer_0;
    field_10C = nullptr;
    field_F6_attack_duration = pTlv->field_1E_attack_duration;
}

Bat::~Bat()
{
    if (field_10C)
    {
        field_10C->mBaseGameObjectRefCount--;
    }
    gMap.TLV_Reset(field_F0_tlvInfo, -1, 0, 0);
}

void Bat::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void Bat::FlyTo(FP xpos, FP ypos, FP* xSpeed, FP* ySpeed)
{
    const FP xd = FP_Abs(xpos - field_104_target_xpos);
    if (xd > FP_FromInteger(350))
    {
        field_A8_xpos += *xSpeed;
    }

    const FP yd = FP_Abs(ypos - field_108_target_ypos);
    if (yd > FP_FromInteger(200))
    {
        field_AC_ypos += *ySpeed;
    }

    field_104_target_xpos = xpos;
    field_108_target_ypos = ypos;

    *xSpeed = xpos + FP_FromInteger((Math_NextRandom() & 63) - 32);
    *xSpeed = *xSpeed - field_A8_xpos;

    *ySpeed = ypos + FP_FromInteger((Math_NextRandom() & 31) - 8);
    *ySpeed = *ySpeed - field_AC_ypos;

    const s32 ySpeedi = FP_GetExponent(*ySpeed);
    const s32 xSpeedi = FP_GetExponent(*xSpeed);
    FP x_final = FP_FromInteger(Math_SquareRoot_Int_4511B0((ySpeedi * ySpeedi) + (xSpeedi * xSpeedi)));
    if (FP_GetExponent(x_final) == 0)
    {
        LOG_WARNING("Fixing divide by zero crash");
        x_final += FP_FromInteger(1);
    }

    field_B4_velx = (FP_FromInteger(8) * *xSpeed) / x_final;
    field_B8_vely = (FP_FromInteger(8) * *ySpeed) / x_final;

    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;
}

void Bat::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (sNumCamSwappers_507668 != 0)
    {
        return;
    }

    FP xSpeed = {};
    FP ySpeed = {};

    switch (field_F4_state)
    {
        case BatStates::eSetTimer_0:
            field_F4_state = BatStates::eInit_1;
            field_F8_timer = gnFrameCount_507670 + field_EC_ticks_before_moving;
            break;

        case BatStates::eInit_1:
            if (static_cast<s32>(gnFrameCount_507670) > field_F8_timer)
            {
                field_F4_state = BatStates::eStartMoving_2;
                field_100_velx = FP_FromInteger(0);
                field_10_anim.Set_Animation_Data(6608, nullptr);
            }
            break;

        case BatStates::eStartMoving_2:
            if (field_100_velx < field_E8_speed)
            {
                field_100_velx += FP_FromDouble(1.8);
                if (field_100_velx > field_E8_speed)
                {
                    field_100_velx = field_E8_speed;
                }
            }

            if (field_E4_pLine)
            {
                field_E4_pLine = field_E4_pLine->MoveOnLine(&field_A8_xpos, &field_AC_ypos, field_100_velx);
            }

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_F4_state = BatStates::eFlying_3;
                const AnimRecord& rec = AO::AnimRec(AnimId::Bat_Flying);
                field_10_anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
                field_F8_timer = gnFrameCount_507670 + Math_RandomRange_450F20(0, 90);
            }
            break;

        case BatStates::eFlying_3:
            if (field_100_velx < field_E8_speed)
            {
                field_100_velx += FP_FromDouble(1.8);
                if (field_100_velx > field_E8_speed)
                {
                    field_100_velx = field_E8_speed;
                }
            }

            if (!(field_10_anim.field_92_current_frame % 3))
            {
                SFX_Play_Mono(static_cast<SoundEffect>(Math_RandomRange_450F20(SoundEffect::Bat1_41, SoundEffect::Bat2_42) & 0xFF), Math_RandomRange_450F20(20, 26), 0);
            }

            if (static_cast<s32>(gnFrameCount_507670) > field_F8_timer)
            {
                SND_SEQ_PlaySeq_4775A0(SeqId::eBatSqueaking_18, 1, 1);
                field_F8_timer = gnFrameCount_507670 + Math_RandomRange_450F20(120, 240);
            }

            if (field_E4_pLine)
            {
                field_E4_pLine = field_E4_pLine->MoveOnLine(&field_A8_xpos, &field_AC_ypos, field_100_velx);
            }

            if (!field_E4_pLine)
            {
                mBaseGameObjectFlags.Set(Options::eDead);
            }

            if (!sActiveHero_507678->field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel))
            {
                for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
                {
                    BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
                    if (!pObjIter)
                    {
                        break;
                    }

                    if (pObjIter->mBaseGameObjectTypeId != Types::SecurityOrb_53 && pObjIter->mBaseGameObjectTypeId != Types::eSlig_88 && pObjIter->mBaseGameObjectTypeId != Types::eSlog_89)
                    {
                        PSX_RECT bObjRect = {};
                        pObjIter->VGetBoundingRect(&bObjRect, 1);

                        if (FP_GetExponent(field_A8_xpos) >= bObjRect.x && FP_GetExponent(field_A8_xpos) <= bObjRect.w && FP_GetExponent(field_AC_ypos) >= bObjRect.y && FP_GetExponent(field_AC_ypos) <= bObjRect.h && pObjIter->field_BC_sprite_scale == field_BC_sprite_scale)
                        {
                            for (s32 j = 0; j < gBaseGameObjects->Size(); j++)
                            {
                                BaseGameObject* pMaybeBat = gBaseGameObjects->ItemAt(j);
                                if (!pMaybeBat)
                                {
                                    break;
                                }

                                if (pMaybeBat->mBaseGameObjectTypeId == Types::eBat_6)
                                {
                                    auto pBat = static_cast<Bat*>(pMaybeBat);

                                    pBat->field_10C = pObjIter;
                                    pBat->field_10C->mBaseGameObjectRefCount++;

                                    pBat->field_F4_state = BatStates::eAttackTarget_4;
                                    const AnimRecord& rec = AO::AnimRec(AnimId::Bat_Flying);
                                    pBat->field_10_anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);

                                    pBat->field_F8_timer = 0;
                                    pBat->field_FC_attack_duration_timer = gnFrameCount_507670 + pBat->field_F6_attack_duration;

                                    pBat->field_104_target_xpos = pBat->field_10C->field_A8_xpos;
                                    pBat->field_108_target_ypos = pBat->field_10C->field_AC_ypos;
                                }
                            }
                        }
                    }
                }
            }
            break;

        case BatStates::eAttackTarget_4:
        {
            if (field_10C->mBaseGameObjectFlags.Get(BaseGameObject::eDead) || Event_Get(kEventDeathReset_4) || Event_Get(kEvent_9))
            {
                mBaseGameObjectFlags.Set(Options::eDead);
                return;
            }

            PSX_RECT bRect = {};
            field_10C->VGetBoundingRect(&bRect, 1);
            FlyTo(
                FP_FromInteger((bRect.w + bRect.x) / 2),
                FP_FromInteger((bRect.h + bRect.y) / 2),
                &xSpeed,
                &ySpeed);

            if (FP_Abs(xSpeed) < FP_FromInteger(10))
            {
                if (FP_Abs(ySpeed) < FP_FromInteger(20) && static_cast<s32>(gnFrameCount_507670) > field_F8_timer)
                {
                    field_10C->VTakeDamage(this);
                    field_F8_timer = gnFrameCount_507670 + 30;
                    SND_SEQ_PlaySeq_4775A0(SeqId::eBatSqueaking_18, 1, 1);
                }
            }

            if (field_FC_attack_duration_timer <= static_cast<s32>(gnFrameCount_507670))
            {
                field_10C->mBaseGameObjectRefCount--;
                field_10C = nullptr;
                field_F4_state = BatStates::eFlyAwayAndDie_5;
            }
        }
        break;

        case BatStates::eFlyAwayAndDie_5:
        {
            FlyTo(field_A8_xpos, field_AC_ypos - FP_FromInteger(40), &xSpeed, &ySpeed);
            if (Event_Get(kEventDeathReset_4) || Event_Get(kEvent_9))
            {
                mBaseGameObjectFlags.Set(Options::eDead);
            }
        }
        break;

        default:
            return;
    }
}

} // namespace AO
