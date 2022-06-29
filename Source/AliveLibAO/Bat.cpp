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
    mBaseGameObjectTypeId = ReliveTypes::eBat;

    const AnimRecord rec = AO::AnimRec(AnimId::Bat);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    
    if (!ppRes)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SligBat_33;

    FP hitX = {};
    FP hitY = {};
    // OG Bug fix, if bat isn't spawned on a line then we crash
    // so field_E4_pLine is checked here and in VUpdate
    sCollisions->Raycast(
        FP_FromInteger(pTlv->mTopLeft.x),
        FP_FromInteger(pTlv->mTopLeft.y),
        FP_FromInteger(pTlv->mBottomRight.x),
        FP_FromInteger(pTlv->mBottomRight.y),
        &field_E4_pLine,
        &hitX,
        &hitY,
        0x100);

    field_F0_tlvInfo = tlvInfo;

    if (field_E4_pLine)
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(field_E4_pLine->field_0_rect.x);
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(field_E4_pLine->field_0_rect.y);
    }

    field_EC_ticks_before_moving = pTlv->field_18_ticks_before_moving;
    field_E8_speed = FP_FromRaw(pTlv->field_1A_speed << 8);

    if (pTlv->field_1C_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
    }

    field_F4_state = BatStates::eSetTimer_0;
    field_10C_pBat = nullptr;
    field_F6_attack_duration = pTlv->field_1E_attack_duration;
}

Bat::~Bat()
{
    if (field_10C_pBat)
    {
        field_10C_pBat->mBaseGameObjectRefCount--;
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
        mBaseAnimatedWithPhysicsGameObject_XPos += *xSpeed;
    }

    const FP yd = FP_Abs(ypos - field_108_target_ypos);
    if (yd > FP_FromInteger(200))
    {
        mBaseAnimatedWithPhysicsGameObject_YPos += *ySpeed;
    }

    field_104_target_xpos = xpos;
    field_108_target_ypos = ypos;

    *xSpeed = xpos + FP_FromInteger((Math_NextRandom() & 63) - 32);
    *xSpeed = *xSpeed - mBaseAnimatedWithPhysicsGameObject_XPos;

    *ySpeed = ypos + FP_FromInteger((Math_NextRandom() & 31) - 8);
    *ySpeed = *ySpeed - mBaseAnimatedWithPhysicsGameObject_YPos;

    const s32 ySpeedi = FP_GetExponent(*ySpeed);
    const s32 xSpeedi = FP_GetExponent(*xSpeed);
    FP x_final = FP_FromInteger(Math_SquareRoot_Int_4511B0((ySpeedi * ySpeedi) + (xSpeedi * xSpeedi)));
    if (FP_GetExponent(x_final) == 0)
    {
        LOG_WARNING("Fixing divide by zero crash");
        x_final += FP_FromInteger(1);
    }

    mBaseAnimatedWithPhysicsGameObject_VelX = (FP_FromInteger(8) * *xSpeed) / x_final;
    mBaseAnimatedWithPhysicsGameObject_VelY = (FP_FromInteger(8) * *ySpeed) / x_final;

    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
}

void Bat::VUpdate()
{
    if (EventGet(kEventDeathReset))
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
            field_F8_timer = sGnFrame + field_EC_ticks_before_moving;
            break;

        case BatStates::eInit_1:
            if (static_cast<s32>(sGnFrame) > field_F8_timer)
            {
                field_F4_state = BatStates::eStartMoving_2;
                field_100_velx = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(6608, nullptr);
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
                field_E4_pLine = field_E4_pLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, field_100_velx);
            }

            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_F4_state = BatStates::eFlying_3;
                const AnimRecord& rec = AO::AnimRec(AnimId::Bat_Flying);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
                field_F8_timer = sGnFrame + Math_RandomRange(0, 90);
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

            if (!(mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame % 3))
            {
                SfxPlayMono(static_cast<SoundEffect>(Math_RandomRange(SoundEffect::Bat1_41, SoundEffect::Bat2_42) & 0xFF), Math_RandomRange(20, 26), 0);
            }

            if (static_cast<s32>(sGnFrame) > field_F8_timer)
            {
                SND_SEQ_PlaySeq_4775A0(SeqId::eBatSqueaking_18, 1, 1);
                field_F8_timer = sGnFrame + Math_RandomRange(120, 240);
            }

            if (field_E4_pLine)
            {
                field_E4_pLine = field_E4_pLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, field_100_velx);
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

                    if (pObjIter->mBaseGameObjectTypeId != ReliveTypes::SecurityOrb && pObjIter->mBaseGameObjectTypeId != ReliveTypes::eSlig && pObjIter->mBaseGameObjectTypeId != ReliveTypes::eSlog)
                    {
                        const PSX_RECT bObjRect = pObjIter->VGetBoundingRect();
                        if (FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) >= bObjRect.x && FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) <= bObjRect.w && FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) >= bObjRect.y && FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) <= bObjRect.h && pObjIter->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
                        {
                            for (s32 j = 0; j < gBaseGameObjects->Size(); j++)
                            {
                                BaseGameObject* pMaybeBat = gBaseGameObjects->ItemAt(j);
                                if (!pMaybeBat)
                                {
                                    break;
                                }

                                if (pMaybeBat->mBaseGameObjectTypeId == ReliveTypes::eBat)
                                {
                                    auto pBat = static_cast<Bat*>(pMaybeBat);

                                    pBat->field_10C_pBat = pObjIter;
                                    pBat->field_10C_pBat->mBaseGameObjectRefCount++;

                                    pBat->field_F4_state = BatStates::eAttackTarget_4;
                                    const AnimRecord& rec = AO::AnimRec(AnimId::Bat_Flying);
                                    pBat->mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);

                                    pBat->field_F8_timer = 0;
                                    pBat->field_FC_attack_duration_timer = sGnFrame + pBat->field_F6_attack_duration;

                                    pBat->field_104_target_xpos = pBat->field_10C_pBat->mBaseAnimatedWithPhysicsGameObject_XPos;
                                    pBat->field_108_target_ypos = pBat->field_10C_pBat->mBaseAnimatedWithPhysicsGameObject_YPos;
                                }
                            }
                        }
                    }
                }
            }
            break;

        case BatStates::eAttackTarget_4:
        {
            if (field_10C_pBat->mBaseGameObjectFlags.Get(BaseGameObject::eDead) || EventGet(kEventDeathReset))
            {
                mBaseGameObjectFlags.Set(Options::eDead);
                return;
            }

            const PSX_RECT bRect = field_10C_pBat->VGetBoundingRect();
            FlyTo(
                FP_FromInteger((bRect.w + bRect.x) / 2),
                FP_FromInteger((bRect.h + bRect.y) / 2),
                &xSpeed,
                &ySpeed);

            if (FP_Abs(xSpeed) < FP_FromInteger(10))
            {
                if (FP_Abs(ySpeed) < FP_FromInteger(20) && static_cast<s32>(sGnFrame) > field_F8_timer)
                {
                    field_10C_pBat->VTakeDamage(this);
                    field_F8_timer = sGnFrame + 30;
                    SND_SEQ_PlaySeq_4775A0(SeqId::eBatSqueaking_18, 1, 1);
                }
            }

            if (field_FC_attack_duration_timer <= static_cast<s32>(sGnFrame))
            {
                field_10C_pBat->mBaseGameObjectRefCount--;
                field_10C_pBat = nullptr;
                field_F4_state = BatStates::eFlyAwayAndDie_5;
            }
        }
        break;

        case BatStates::eFlyAwayAndDie_5:
        {
            FlyTo(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(40), &xSpeed, &ySpeed);
            if (EventGet(kEventDeathReset))
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
