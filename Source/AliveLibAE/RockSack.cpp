#include "stdafx.h"
#include "RockSack.hpp"
#include "Rock.hpp"
#include "Function.hpp"
#include "ThrowableArray.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"

void RockSack::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::RockSack_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::RockSack_SoftHit));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::RockSack_HardHit));
}

RockSack::RockSack(relive::Path_RockSack* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eRockSack);

    LoadAnimations();

    const AnimRecord& rec = AnimRec(AnimId::RockSack_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);

    // Set RockSack idle animation speed.
    auto pAnimationHeader = reinterpret_cast<AnimationHeader*>(*ppRes + rec.mFrameTableOffset);
    pAnimationHeader->field_0_fps = 0;

    Animation_Init(GetAnimRes(AnimId::RockSack_Idle));

    mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

    field_118_tlvInfo = tlvId;
    field_11C_has_been_hit = 0;
    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mBottomRightY);
    field_124_x_vel = FP_FromRaw(pTlv->mVelX << 8);
    field_128_y_vel = FP_FromRaw(-256 * pTlv->mVelY);

    if (pTlv->mRockFallDirection == relive::reliveXDirection::eLeft)
    {
        field_124_x_vel = -field_124_x_vel;
    }

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    field_11E_rock_amount = pTlv->mRockAmount;
    field_120_can_play_wobble_sound = 1;
    field_122_force_wobble_sound = 1;

    mShadow = relive_new Shadow();

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
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
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mAnim.mCurrentFrame == 2)
    {
        if (field_120_can_play_wobble_sound)
        {
            if (Math_NextRandom() < 40u || field_122_force_wobble_sound)
            {
                field_120_can_play_wobble_sound = 0;
                field_122_force_wobble_sound = 0;
                SFX_Play_Pitch(relive::SoundEffects::SackWobble, 24, Math_RandomRange(-2400, -2200));
            }
        }
    }
    else
    {
        field_120_can_play_wobble_sound = 1;
    }

    if (field_11C_has_been_hit)
    {
        if (field_11C_has_been_hit == 1 && mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::RockSack_Idle));
            field_11C_has_been_hit = 0;
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

        if (bRect.x <= bPlayerRect.w && bRect.w >= bPlayerRect.x && bRect.h >= bPlayerRect.y && bRect.y <= bPlayerRect.h && mSpriteScale == sActiveHero->mSpriteScale)
        {
            if (gpThrowableArray)
            {
                if (gpThrowableArray->field_20_count)
                {
                    if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_31_RunJumpMid_452C10)
                    {
                        mAnim.Set_Animation_Data(GetAnimRes(AnimId::RockSack_HardHit));
                    }
                    else
                    {
                        mAnim.Set_Animation_Data(GetAnimRes(AnimId::RockSack_SoftHit));
                    }
                    field_11C_has_been_hit = 1;
                    return;
                }
            }
            else
            {
                gpThrowableArray = relive_new ThrowableArray();
            }

            gpThrowableArray->Add(field_11E_rock_amount);

            auto pRock = relive_new Rock(mXPos, mYPos - FP_FromInteger(30), field_11E_rock_amount);
            if (pRock)
            {
                pRock->VThrow(field_124_x_vel, field_128_y_vel);
            }

            SfxPlayMono(relive::SoundEffects::SackHit, 0);
            Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);

            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_31_RunJumpMid_452C10)
            {
                mAnim.Set_Animation_Data(GetAnimRes(AnimId::RockSack_HardHit));
            }
            else
            {
                mAnim.Set_Animation_Data(GetAnimRes(AnimId::RockSack_SoftHit));
            }
            field_11C_has_been_hit = 1;
        }
    }
}
