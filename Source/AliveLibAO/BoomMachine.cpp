#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "BoomMachine.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "Grenade.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "Path.hpp"
#include "../relive_lib/Grid.hpp"
#include "BoomMachinePipe.hpp"

namespace AO {

void BoomMachine::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BoomMachine_Button_Off));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BoomMachine_Button_On));
}

BoomMachine::BoomMachine(relive::Path_BoomMachine* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eBoomMachine);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::BoomMachine_Button_On));

    SetApplyShadowZoneColour(false);
    mTlvId = tlvId;
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX) + (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    auto pPipe = relive_new BoomMachinePipe();
    if (pPipe)
    {
        FP directedScale = GetSpriteScale();
        if (pTlv->mPipeSide == relive::Path_BoomMachine::PipeSide::eLeft)
        {
            directedScale = -directedScale;
        }

        pPipe->Animation_Init(pPipe->GetAnimRes(AnimId::BoomMachine_Pipe_Idle));

        pPipe->GetAnimation().SetSemiTrans(false);
        pPipe->SetSpriteScale(GetSpriteScale());
        pPipe->SetApplyShadowZoneColour(false);
        pPipe->mState = BoomMachinePipeStates::eInactive;
        pPipe->mXPos = mXPos + (directedScale * FP_FromInteger(30));
        pPipe->mYPos = mYPos + (GetSpriteScale() * FP_FromInteger(-30));
        pPipe->mGrenadeCount = static_cast<s16>(pTlv->mGrenadeAmount);
        pPipe->GetAnimation().SetFlipX(pTlv->mPipeSide == relive::Path_BoomMachine::PipeSide::eLeft);

        mPipeId = pPipe->mBaseGameObjectId;
    }

    if (gThrowableArray && gThrowableArray->mCount)
    {
        mIsButtonOn = true;
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoomMachine_Button_On));
    }
    else
    {
        mIsButtonOn = false;
    }

    mYOffset = 0;
}

void BoomMachine::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (!mIsButtonOn)
    {
        if (!gThrowableArray || gThrowableArray->mCount == 0)
        {
            mIsButtonOn = true;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoomMachine_Button_On));
        }
    }
    else if (mIsButtonOn)
    {
        if (gThrowableArray && gThrowableArray->mCount > 0)
        {
            mIsButtonOn = false;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoomMachine_Button_Off));
        }

        if (GetAnimation().GetCurrentFrame() == 3)
        {
            SFX_Play_Pitch(relive::SoundEffects::RedTick, 25, -1200);
        }
    }
}

void BoomMachine::VScreenChanged()
{
    SetDead(true);
}

bool BoomMachine::VIsButtonOn()
{
    return mIsButtonOn;
}

void BoomMachine::VHandleButton()
{
    auto pPipe = static_cast<BoomMachinePipe*>(sObjectIds.Find_Impl(mPipeId));
    if (pPipe)
    {
        if (VIsButtonOn())
        {
            pPipe->DropGrenadeAnimation();
        }
        else
        {
            pPipe->AlreadyUsed();
        }
    }
}

BoomMachine::~BoomMachine()
{
    BaseGameObject* pObj = sObjectIds.Find_Impl(mPipeId);
    if (pObj)
    {
        pObj->SetDead(true);
    }
    Path::TLV_Reset(mTlvId);
}
} // namespace AO
