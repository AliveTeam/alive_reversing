#include "stdafx.h"
#include "BoomMachine.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "stdlib.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Grenade.hpp"
#include "Grid.hpp"
#include "Path.hpp"
#include "../relive_lib/FixedPoint.hpp"

const static AnimId sBoomMachineNozzleAnimIds[2] =
{
    AnimId::BoomMachine_Nozzle_DropGrenade,
    AnimId::BoomMachine_Nozzle_Idle
};

const static AnimId sBoomMachineAnimIds[2] =
{
    AnimId::BoomMachine_Button_Off,
    AnimId::BoomMachine_Button_On
};

class GrenadeMachineNozzle final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    GrenadeMachineNozzle(FP xpos, FP ypos, FP scale, s16 numGrenades)
        : BaseAnimatedWithPhysicsGameObject(0)
    {
        LoadAnimations();
        Animation_Init(GetAnimRes(AnimId::BoomMachine_Nozzle_Idle));

        GetAnimation().SetSemiTrans(false);
        SetApplyShadowZoneColour(false);

        SetSpriteScale(scale);

        mXPos = xpos;
        mYPos = ypos;

        mGrenadeCount = numGrenades;

        mState = BoomMachineStates::eInactive_0;
    }

    void LoadAnimations()
    {
        for (auto& animId : sBoomMachineNozzleAnimIds)
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
        }
    }

    void DropGrenadeAnimation()
    {
        if (mState == BoomMachineStates::eInactive_0)
        {
            mState = BoomMachineStates::eDropGrenadeAnimation_2;
            mTimer = sGnFrame + 10;
        }
    }

    void AlreadyUsed()
    {
        if (mState == BoomMachineStates::eInactive_0)
        {
            mState = BoomMachineStates::eAlreadyUsed_1;
            mTimer = sGnFrame + 10;
        }
    }

private:
    virtual void VUpdate() override
    {
        switch (mState)
        {
            case BoomMachineStates::eInactive_0:
                // do nothing
                break;
            case BoomMachineStates::eAlreadyUsed_1:
                if (static_cast<s32>(sGnFrame) > mTimer)
                {
                    SFX_Play_Pitch(relive::SoundEffects::ZPop, 60, -1800);
                    mState = BoomMachineStates::eInactive_0;
                }
                break;

            case BoomMachineStates::eDropGrenadeAnimation_2:
                if (static_cast<s32>(sGnFrame) > mTimer)
                {
                    mState = BoomMachineStates::eDropGrenade_3;
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoomMachine_Nozzle_DropGrenade));
                }
                break;

            case BoomMachineStates::eDropGrenade_3:
                if (GetAnimation().GetForwardLoopCompleted())
                {
                    SFX_Play_Pitch(relive::SoundEffects::PickupItem, 127, -900);

                    if (!gThrowableArray)
                    {
                        gThrowableArray = relive_new ThrowableArray();
                    }

                    gThrowableArray->Add(mGrenadeCount);

                    FP directedScale = {};
                    if (GetAnimation().GetFlipX())
                    {
                        directedScale = -GetSpriteScale();
                    }
                    else
                    {
                        directedScale = GetSpriteScale();
                    }
                    auto pGrenade = relive_new Grenade(
                        mXPos + (FP_FromInteger(6) * directedScale),
                        mYPos + (-FP_FromInteger(6) * GetSpriteScale()),
                        mGrenadeCount,
                        0,
                        nullptr);
                    if (pGrenade)
                    {
                        pGrenade->VThrow((GetAnimation().GetFlipX()) != 0 ? -FP_FromDouble(0.75) : FP_FromDouble(0.75), FP_FromInteger(3));
                    }

                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoomMachine_Nozzle_Idle));
                    mState = BoomMachineStates::eInactive_0;
                }
                break;
            default:
                return;
        }
    }

private:
    BoomMachineStates mState = BoomMachineStates::eInactive_0;
    s32 mTimer = 0;
    s16 mGrenadeCount = 0;
};

void BoomMachine::LoadAnimations()
{
    for (auto& animId : sBoomMachineAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

BoomMachine::BoomMachine(relive::Path_BoomMachine* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0),
    mTlvId(tlvId)
{
    SetType(ReliveTypes::eBoomMachine);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::BoomMachine_Button_Off));

    SetApplyShadowZoneColour(false);
    GetAnimation().SetRenderMode(TPageAbr::eBlend_1);

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

    auto pNozzle = relive_new GrenadeMachineNozzle(
        ((pTlv->mNozzleSide == relive::Path_BoomMachine::NozzleSide::eLeft ? -GetSpriteScale() : GetSpriteScale()) * FP_FromInteger(30)) + mXPos,
        (GetSpriteScale() * FP_FromInteger(-30)) + mYPos,
        GetSpriteScale(),
        pTlv->mGrenadeAmount);
    if (pNozzle)
    {
        pNozzle->GetAnimation().SetFlipX(pTlv->mNozzleSide == relive::Path_BoomMachine::NozzleSide::eLeft);
        mNozzleId = pNozzle->mBaseGameObjectId;
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
    return mIsButtonOn == true;
}

void BoomMachine::VHandleButton()
{
    auto pNozzle = static_cast<GrenadeMachineNozzle*>(sObjectIds.Find_Impl(mNozzleId));
    if (pNozzle)
    {
        if (VIsButtonOn())
        {
            pNozzle->DropGrenadeAnimation();
        }
        else
        {
            pNozzle->AlreadyUsed();
        }
    }
}

BoomMachine::~BoomMachine()
{
    BaseGameObject* pObj = sObjectIds.Find_Impl(mNozzleId);
    if (pObj)
    {
        pObj->SetDead(true);
    }
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}
