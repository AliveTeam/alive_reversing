#include "stdafx_ao.h"
#include "MeatSack.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/Events.hpp"
#include "Path.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "ThrowableArray.hpp"
#include "Abe.hpp"
#include "Meat.hpp"

namespace AO
{

void MeatSack::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MeatSack_Hit));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MeatSack_Idle));
}

MeatSack::MeatSack(relive::Path_MeatSack* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::eMeatSack);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::MeatSack_Idle));

    SetApplyShadowZoneColour(false);
    mTlvId = tlvId;

    mDoMeatSackIdleAnim = false;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mTlvVelX = FP_FromRaw(pTlv->mVelX << 8);

    // Throw the meat up into the air as it falls from the sack
    mTlvVelY = -FP_FromRaw(pTlv->mVelY << 8);

    if (pTlv->mMeatFallDirection == relive::reliveXDirection::eLeft)
    {
        mTlvVelX = -mTlvVelX;
    }

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_8);
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_27);
        SetScale(Scale::Fg);
    }

    mMeatAmount = pTlv->mMeatAmount;

    CreateShadow();
}

MeatSack::~MeatSack()
{
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void MeatSack::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (mPlayWobbleSound)
        {
            if (Math_NextRandom() < 40u)
            {
                mPlayWobbleSound = false;
                SFX_Play_Pitch(relive::SoundEffects::SackWobble, 24, Math_RandomRange(-2400, -2200));
            }
        }
    }
    else
    {
        mPlayWobbleSound = true;
    }

    if (mDoMeatSackIdleAnim)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MeatSack_Idle));
            mDoMeatSackIdleAnim = false;
        }
        return;
    }

    const PSX_RECT abeRect = sActiveHero->VGetBoundingRect();
    const PSX_RECT ourRect = VGetBoundingRect();

    if (RectsOverlap(ourRect, abeRect))
    {
        if (GetSpriteScale() == sActiveHero->GetSpriteScale())
        {
            if (!gThrowableArray)
            {
                gThrowableArray = relive_new ThrowableArray();
            }

            if (gThrowableArray)
            {
                if (gThrowableArray->mCount > 0)
                {
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MeatSack_Hit));
                    mDoMeatSackIdleAnim = true;
                    return;
                }

                gThrowableArray->Add(mMeatAmount);
            }

            auto pMeat = relive_new Meat(
                mXPos,
                mYPos - FP_FromInteger(30),
                mMeatAmount);
            if (pMeat)
            {
                pMeat->VThrow(mTlvVelX, mTlvVelY);
                pMeat->SetSpriteScale(GetSpriteScale());
            }

            SfxPlayMono(relive::SoundEffects::SackHit, 0);
            Environment_SFX(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, nullptr);

            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MeatSack_Hit));
            mDoMeatSackIdleAnim = true;
            return;
        }
    }
}

void MeatSack::VScreenChanged()
{
    SetDead(true);
}

Meat::Meat(FP xpos, FP ypos, s16 count)
    : BaseThrowable()
{
    mBaseThrowableDead = 0;

    SetType(ReliveTypes::eMeat);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Meat));
    Animation_Init(GetAnimRes(AnimId::Meat));

    mXPos = xpos;
    mYPos = ypos;

    mPreviousXPos = xpos;
    mPreviousYPos = ypos;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mShimmerTimer = 0;
    SetInteractive(false);

    GetAnimation().SetRender(false);
    GetAnimation().SetSemiTrans(false);

    mDeadTimer = sGnFrame + 600;
    mPathLine = 0;
    mBaseThrowableCount = count;
    mState = 0;

    CreateShadow();
}

} // namespace AO