#include "stdafx.h"
#include "BoneBag.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/Events.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "ThrowableArray.hpp"
#include "Abe.hpp"
#include "Bone.hpp"

static const TintEntry kBoneBagTints[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 137u, 137u, 137u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eCredits, 127u, 127u, 127u} };

void BoneBag::LoadAnimations()
{
    const static AnimId sBoneBagAnimIds[] =
    {
        AnimId::BoneBag_Idle,
        AnimId::BoneBag_SoftHit,
        AnimId::BoneBag_HardHit
    };

    for (auto& animId : sBoneBagAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

BoneBag::BoneBag(relive::Path_BoneBag* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0),
    mTlvInfo(tlvId),
    mTlvVelX(FP_FromRaw(pTlv->mVelX << 8)),
    mTlvVelY(FP_FromRaw(-256 * pTlv->mVelY)), // TODO: << 8 negated ??
    mBoneAmount(pTlv->mBoneAmount)
{
    SetType(ReliveTypes::eBoneBag);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::BoneBag_Idle));
    GetAnimation().SetSemiTrans(false);
    SetTint(&kBoneBagTints[0], gMap.mCurrentLevel);

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mBottomRightY);

    SetApplyShadowZoneColour(false);

    mTlvVelX = FP_FromRaw(pTlv->mVelX << 8);
    mTlvVelY = FP_FromRaw(-256 * pTlv->mVelY); 

    if (pTlv->mBoneFallDirection == relive::reliveXDirection::eLeft)
    {
        mTlvVelX = -mTlvVelX;
    }

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
    }

    CreateShadow();
}

void BoneBag::VScreenChanged()
{
    SetDead(true);
}

BoneBag::~BoneBag()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

void BoneBag::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (mPlayWobbleSound)
        {
            if (Math_NextRandom() < 40 || mForcePlayWobbleSound)
            {
                mPlayWobbleSound = false;
                mForcePlayWobbleSound = false;
                SFX_Play_Pitch(relive::SoundEffects::SackWobble, 24, Math_RandomRange(-2400, -2200));
            }
        }
    }
    else
    {
        mPlayWobbleSound = false;
    }

    if (mHasBeenHit)
    {
        if (mHasBeenHit != 1)
        {
            return;
        }

        if (!GetAnimation().GetIsLastFrame())
        {
            return;
        }

        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoneBag_Idle));
        mHasBeenHit = false;
        return;
    }

    if (GetAnimation().GetFrameChangeCounter() == 0)
    {
        GetAnimation().SetFrameChangeCounter(Math_RandomRange(2, 10));
    }

    const PSX_RECT bPlayerRect = gAbe->VGetBoundingRect();
    const PSX_RECT bRect = VGetBoundingRect();

    if (bRect.x <= bPlayerRect.w && bRect.w >= bPlayerRect.x && bRect.h >= bPlayerRect.y && bRect.y <= bPlayerRect.h && GetSpriteScale() == gAbe->GetSpriteScale())
    {
        if (gThrowableArray)
        {
            if (gThrowableArray->mCount)
            {
                if (gAbe->mCurrentMotion == eAbeMotions::Motion_31_RunJumpMid_452C10)
                {
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoneBag_HardHit));
                }
                else
                {
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoneBag_SoftHit));
                }
                mHasBeenHit = true;
                return;
            }
        }
        else
        {
            gThrowableArray = relive_new ThrowableArray();
        }

        gThrowableArray->Add(mBoneAmount);

        auto pBone = relive_new Bone(mXPos, mYPos - FP_FromInteger(30), mBoneAmount);

        pBone->SetSpriteScale(GetSpriteScale());
        pBone->SetScale(GetScale());

        pBone->VThrow(mTlvVelX, mTlvVelY);

        SfxPlayMono(relive::SoundEffects::SackHit, 0);
        Environment_SFX(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);

        if (gAbe->mCurrentMotion == eAbeMotions::Motion_31_RunJumpMid_452C10)
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoneBag_HardHit));
        }
        else
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoneBag_SoftHit));
        }

        mHasBeenHit = true;
    }
}