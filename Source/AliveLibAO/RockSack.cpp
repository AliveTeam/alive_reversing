#include "stdlib.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Events.hpp"
#include "Math.hpp"
#include "Rock.hpp"
#include "RockSack.hpp"
#include "Sfx.hpp"
#include "ThrowableArray.hpp"
#include "AnimResources.hpp"
#include "ResourceManager.hpp"
#include "FixedPoint_common.hpp"
#include "Abe.hpp"
#include "Function.hpp"
#include "Shadow.hpp"

namespace AO {

void RockSack::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 2)
    {
        if (field_114_can_play_wobble_sound)
        {
            if (Math_NextRandom() < 40u || field_116_force_wobble_sound)
            {
                field_114_can_play_wobble_sound = 0;
                field_116_force_wobble_sound = 0;
                SFX_Play_Pitch(SoundEffect::SackWobble_34, 24, Math_RandomRange(-2400, -2200), 0);
            }
        }
    }
    else
    {
        field_114_can_play_wobble_sound = 1;
    }

    if (field_110_has_been_hit)
    {
        if (field_110_has_been_hit == 1)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(13756, 0);
                field_110_has_been_hit = 0;
            }
        }
    }
    else
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter == 0)
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter = Math_RandomRange(2, 10);
        }

        const PSX_RECT bPlayerRect = sActiveHero_507678->VGetBoundingRect();
        const PSX_RECT bRect = VGetBoundingRect();

        if (bRect.x <= bPlayerRect.w
            && bRect.w >= bPlayerRect.x
            && bRect.h >= bPlayerRect.y
            && bRect.y <= bPlayerRect.h
            && mBaseAnimatedWithPhysicsGameObject_SpriteScale == sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_SpriteScale)
        {
            if (!gpThrowableArray_50E26C || !gpThrowableArray_50E26C->field_10_count)
            {
                if (!gpThrowableArray_50E26C)
                {
                    gpThrowableArray_50E26C = relive_new ThrowableArray();
                }

                gpThrowableArray_50E26C->Add(field_112_rock_amount);

                auto pRock = relive_new Rock(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(30),
                    field_112_rock_amount);
                if (pRock)
                {
                    pRock->VThrow(field_118_x_vel, field_11C_y_vel);
                }

                SFX_Play_Mono(SoundEffect::SackHit_30, 0, 0);
                Environment_SFX_42A220(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);
            }

            if (sActiveHero_507678->mCurrentMotion == eAbeMotions::Motion_33_RunJumpMid_426FA0)
            {
                const AnimRecord& hardHitRec = AO::AnimRec(AnimId::RockSack_HardHit);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(hardHitRec.mFrameTableOffset, 0);
            }
            else
            {
                const AnimRecord& softHitRec = AO::AnimRec(AnimId::RockSack_SoftHit);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(softHitRec.mFrameTableOffset, 0);
            }

            field_110_has_been_hit = 1;
        }
    }
}

RockSack::~RockSack()
{
    gMap.TLV_Reset(field_10C_tlvInfo, -1, 0, 0);
}

RockSack::RockSack(Path_RockSack* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eRockSack;

    const AnimRecord rec = AO::AnimRec(AnimId::RockSack_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);

    //  Set RockSack idle anim speed
    auto pAnimationHeader = reinterpret_cast<AnimationHeader*>(*ppRes + 6878); // TODO: frametableoffset
    pAnimationHeader->field_0_fps = 0;

    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_10C_tlvInfo = tlvInfo;
    field_110_has_been_hit = 0;
    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->mTopLeft.x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);
    field_118_x_vel = FP_FromRaw(pTlv->field_1A_x_vel << 8);
    field_11C_y_vel = FP_FromRaw(-256 * pTlv->field_1C_y_vel);

    if (pTlv->field_18_fall_direction == XDirection_short::eLeft_0)
    {
        field_118_x_vel = -field_118_x_vel;
    }

    if (pTlv->field_1E_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    field_112_rock_amount = pTlv->field_20_rock_amount;
    field_114_can_play_wobble_sound = 1;
    field_116_force_wobble_sound = 1;

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        u8** ppPal = ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kP2c2bagAOResID, 0, 0);
        mBaseAnimatedWithPhysicsGameObject_Anim.LoadPal(ppPal, 0);
    }

    mShadow = relive_new Shadow();
}

void RockSack::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

}
