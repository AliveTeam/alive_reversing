#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Events.hpp"
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
#include "../relive_lib/Shadow.hpp"

namespace AO {

void RockSack::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mAnim.mCurrentFrame == 2)
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
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mAnim.Set_Animation_Data(AnimId::RockSack_Idle, nullptr);
                field_110_has_been_hit = 0;
            }
        }
    }
    else
    {
        if (mAnim.mFrameChangeCounter == 0)
        {
            mAnim.mFrameChangeCounter = Math_RandomRange(2, 10);
        }

        const PSX_RECT bPlayerRect = sActiveHero->VGetBoundingRect();
        const PSX_RECT bRect = VGetBoundingRect();

        if (bRect.x <= bPlayerRect.w
            && bRect.w >= bPlayerRect.x
            && bRect.h >= bPlayerRect.y
            && bRect.y <= bPlayerRect.h
            && mSpriteScale == sActiveHero->mSpriteScale)
        {
            if (!gpThrowableArray_50E26C || !gpThrowableArray_50E26C->field_10_count)
            {
                if (!gpThrowableArray_50E26C)
                {
                    gpThrowableArray_50E26C = relive_new ThrowableArray();
                }

                gpThrowableArray_50E26C->Add(field_112_rock_amount);

                auto pRock = relive_new Rock(
                    mXPos,
                    mYPos - FP_FromInteger(30),
                    field_112_rock_amount);
                if (pRock)
                {
                    pRock->VThrow(field_118_x_vel, field_11C_y_vel);
                }

                SfxPlayMono(SoundEffect::SackHit_30, 0, 0);
                Environment_SFX_42A220(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);
            }

            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_33_RunJumpMid_426FA0)
            {
                mAnim.Set_Animation_Data(AnimId::RockSack_HardHit, nullptr);
            }
            else
            {
                mAnim.Set_Animation_Data(AnimId::RockSack_SoftHit, nullptr);
            }

            field_110_has_been_hit = 1;
        }
    }
}

RockSack::~RockSack()
{
    Path::TLV_Reset(field_10C_tlvInfo, -1, 0, 0);
}

RockSack::RockSack(relive::Path_RockSack* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eRockSack;

    const AnimRecord rec = AO::AnimRec(AnimId::RockSack_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);

    //  Set RockSack idle anim speed
    auto pAnimationHeader = reinterpret_cast<AnimationHeader*>(*ppRes + 6878); // TODO: frametableoffset
    pAnimationHeader->field_0_fps = 0;

    Animation_Init(AnimId::RockSack_Idle, ppRes);

    mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_10C_tlvInfo = tlvId;
    field_110_has_been_hit = 0;
    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    field_118_x_vel = FP_FromRaw(pTlv->mVelX << 8);
    field_11C_y_vel = FP_FromRaw(-256 * pTlv->mVelY);

    if (pTlv->mRockFallDirection == relive::reliveXDirection::eLeft)
    {
        field_118_x_vel = -field_118_x_vel;
    }

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    field_112_rock_amount = pTlv->mRockAmount;
    field_114_can_play_wobble_sound = 1;
    field_116_force_wobble_sound = 1;

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        u8** ppPal = ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kP2c2bagAOResID, 0, 0);
        mAnim.LoadPal(ppPal, 0);
    }

    mShadow = relive_new Shadow();
}

void RockSack::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

}
