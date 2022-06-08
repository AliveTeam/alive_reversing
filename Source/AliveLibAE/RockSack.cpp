#include "stdafx.h"
#include "RockSack.hpp"
#include "Rock.hpp"
#include "Function.hpp"
#include "ThrowableArray.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "Abe.hpp"

RockSack::RockSack(Path_RockSack* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eRockSack);

    const AnimRecord& rec = AnimRec(AnimId::RockSack_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);

    // Set RockSack idle animation speed.
    auto pAnimationHeader = reinterpret_cast<AnimationHeader*>(*ppRes + rec.mFrameTableOffset);
    pAnimationHeader->field_0_fps = 0;

    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mApplyShadows &= ~1u;

    field_118_tlvInfo = tlvInfo;
    field_11C_has_been_hit = 0;
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    field_124_x_vel = FP_FromRaw(pTlv->field_12_x_vel << 8);
    field_128_y_vel = FP_FromRaw(-256 * pTlv->field_14_y_vel);

    if (pTlv->field_10_fall_direction == XDirection_short::eLeft_0)
    {
        field_124_x_vel = -field_124_x_vel;
    }

    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else if (pTlv->field_16_scale == Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    field_11E_rock_amount = pTlv->field_18_rock_amount;
    field_120_can_play_wobble_sound = 1;
    field_122_force_wobble_sound = 1;

    mShadow = relive_new Shadow();

    mApplyShadows |= 2u;
}

RockSack::~RockSack()
{
    Path::TLV_Reset(field_118_tlvInfo, -1, 0, 0);
}

void RockSack::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void RockSack::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_20_animation.field_92_current_frame == 2)
    {
        if (field_120_can_play_wobble_sound)
        {
            if (Math_NextRandom() < 40u || field_122_force_wobble_sound)
            {
                field_120_can_play_wobble_sound = 0;
                field_122_force_wobble_sound = 0;
                SFX_Play_Pitch(SoundEffect::SackWobble_29, 24, Math_RandomRange(-2400, -2200));
            }
        }
    }
    else
    {
        field_120_can_play_wobble_sound = 1;
    }

    if (field_11C_has_been_hit)
    {
        if (field_11C_has_been_hit == 1 && field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            const AnimRecord& animRec = AnimRec(AnimId::RockSack_Idle);
            field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
            field_11C_has_been_hit = 0;
        }
    }
    else
    {
        if (field_20_animation.mFrameChangeCounter == 0)
        {
            field_20_animation.mFrameChangeCounter = Math_RandomRange(2, 10);
        }

        PSX_RECT bPlayerRect = {};
        sActiveHero->VGetBoundingRect(&bPlayerRect, 1);

        PSX_RECT bRect = {};
        VGetBoundingRect(&bRect, 1);

        if (bRect.x <= bPlayerRect.w && bRect.w >= bPlayerRect.x && bRect.h >= bPlayerRect.y && bRect.y <= bPlayerRect.h && field_CC_sprite_scale == sActiveHero->field_CC_sprite_scale)
        {
            if (gpThrowableArray_5D1E2C)
            {
                if (gpThrowableArray_5D1E2C->field_20_count)
                {
                    if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_31_RunJumpMid_452C10)
                    {
                        const AnimRecord& animRec = AnimRec(AnimId::RockSack_HardHit);
                        field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                    }
                    else
                    {
                        const AnimRecord& animRec = AnimRec(AnimId::RockSack_SoftHit);
                        field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                    }
                    field_11C_has_been_hit = 1;
                    return;
                }
            }
            else
            {
                gpThrowableArray_5D1E2C = relive_new ThrowableArray();
            }

            gpThrowableArray_5D1E2C->Add(field_11E_rock_amount);

            auto pRock = relive_new Rock(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(30), field_11E_rock_amount);
            if (pRock)
            {
                pRock->VThrow(field_124_x_vel, field_128_y_vel);
            }

            SFX_Play_Mono(SoundEffect::SackHit_25, 0);
            Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);

            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_31_RunJumpMid_452C10)
            {
                const AnimRecord& animRec = AnimRec(AnimId::RockSack_HardHit);
                field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
            }
            else
            {
                const AnimRecord& animRec = AnimRec(AnimId::RockSack_SoftHit);
                field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
            }
            field_11C_has_been_hit = 1;
        }
    }
}
