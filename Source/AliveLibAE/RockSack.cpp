#include "stdafx.h"
#include "RockSack.hpp"
#include "Rock.hpp"
#include "../relive_lib/Function.hpp"
#include "ThrowableArray.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "Path.hpp"
#include "Math.hpp"

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
    Animation_Init(GetAnimRes(AnimId::RockSack_Idle));

    GetAnimation().SetSemiTrans(false);

    SetApplyShadowZoneColour(false);

    mTlvId = tlvId;
    mHasBeenHit = false;
    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mBottomRightY);
    mTlvVelX = FP_FromRaw(pTlv->mVelX << 8);
    mTlvVelY = FP_FromRaw(-256 * pTlv->mVelY);

    if (pTlv->mRockFallDirection == relive::reliveXDirection::eLeft)
    {
        mTlvVelX = -mTlvVelX;
    }

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
    }

    mRockAmount = pTlv->mRockAmount;
    mPlayWobbleSound = true;
    mForceWobbleSound = true;

    CreateShadow();

    SetDoPurpleLightEffect(true);
}

RockSack::~RockSack()
{
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void RockSack::VScreenChanged()
{
    SetDead(true);
}

void RockSack::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (mPlayWobbleSound)
        {
            if (Math_NextRandom() < 40u || mForceWobbleSound)
            {
                mPlayWobbleSound = false;
                mForceWobbleSound = false;
                SFX_Play_Pitch(relive::SoundEffects::SackWobble, 24, Math_RandomRange(-2400, -2200));
            }
        }
    }
    else
    {
        mPlayWobbleSound = true;
    }

    if (mHasBeenHit)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::RockSack_Idle));
            mHasBeenHit = false;
        }
    }
    else
    {
        if (GetAnimation().GetFrameChangeCounter() == 0)
        {
            GetAnimation().SetFrameChangeCounter(Math_RandomRange(2, 10));
        }

        const PSX_RECT bPlayerRect = sActiveHero->VGetBoundingRect();
        const PSX_RECT bRect = VGetBoundingRect();

        if (bRect.x <= bPlayerRect.w 
            && bRect.w >= bPlayerRect.x 
            && bRect.h >= bPlayerRect.y 
            && bRect.y <= bPlayerRect.h 
            && GetSpriteScale() == sActiveHero->GetSpriteScale())
        {
            if (gThrowableArray)
            {
                if (gThrowableArray->mCount)
                {
                    if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_31_RunJumpMid_452C10)
                    {
                        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::RockSack_HardHit));
                    }
                    else
                    {
                        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::RockSack_SoftHit));
                    }
                    mHasBeenHit = true;
                    return;
                }
            }
            else
            {
                gThrowableArray = relive_new ThrowableArray();
            }

            gThrowableArray->Add(mRockAmount);

            auto pRock = relive_new Rock(mXPos, mYPos - FP_FromInteger(30), mRockAmount);
            if (pRock)
            {
                pRock->VThrow(mTlvVelX, mTlvVelY);
            }

            SfxPlayMono(relive::SoundEffects::SackHit, 0);
            Environment_SFX(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);

            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_31_RunJumpMid_452C10)
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::RockSack_HardHit));
            }
            else
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::RockSack_SoftHit));
            }

            mHasBeenHit = true;
        }
    }
}
