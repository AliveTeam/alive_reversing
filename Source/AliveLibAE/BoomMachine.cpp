#include "stdafx.h"
#include "BoomMachine.hpp"
#include "Function.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "stdlib.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Grenade.hpp"
#include "Grid.hpp"
#include "Path.hpp"
#include "FixedPoint.hpp"

const static AnimId sBoomMachineNozzleAnimIds[] =
{
    AnimId::BoomMachine_Nozzle_DropGrenade,
    AnimId::BoomMachine_Nozzle_Idle
};

const static AnimId sBoomMachineAnimIds[] =
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

        GetAnimation().mFlags.Clear(AnimFlags::eSemiTrans);
        mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

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

    void DropGrenadeAnimation_445820()
    {
        if (mState == BoomMachineStates::eInactive_0)
        {
            mState = BoomMachineStates::eDropGrenadeAnimation_2;
            mTimer = sGnFrame + 10;
        }
    }

    void AlreadyUsed_445860()
    {
        if (mState == BoomMachineStates::eInactive_0)
        {
            mState = BoomMachineStates::eAlreadyUsed_1;
            mTimer = sGnFrame + 10;
        }
    }

private:
    void VUpdate() override
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
                if (GetAnimation().mFlags.Get(AnimFlags::eForwardLoopCompleted))
                {
                    SFX_Play_Pitch(relive::SoundEffects::PickupItem, 127, -900);

                    if (!gpThrowableArray)
                    {
                        gpThrowableArray = relive_new ThrowableArray();
                    }

                    gpThrowableArray->Add(mGrenadeCount);

                    FP directedScale = {};
                    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                    {
                        directedScale = -GetSpriteScale();
                    }
                    else
                    {
                        directedScale = GetSpriteScale();
                    }
                    auto pGrenade = relive_new Grenade(
                        (FP_FromInteger(6) * directedScale) + mXPos,
                        (-FP_FromInteger(6) * GetSpriteScale()) + mYPos,
                        mGrenadeCount,
                        0,
                        nullptr);
 
                    pGrenade->VThrow((GetAnimation().mFlags.Get(AnimFlags::eFlipX)) != 0 ? -FP_FromDouble(0.75) : FP_FromDouble(0.75), FP_FromInteger(3));

                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoomMachine_Nozzle_Idle));
                    mState = BoomMachineStates::eInactive_0;
                }
                break;
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
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eBoomMachine);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::BoomMachine_Button_Off));

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mTlvId = tlvId;
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
        pNozzle->GetAnimation().mFlags.Set(AnimFlags::eFlipX, pTlv->mNozzleSide == relive::Path_BoomMachine::NozzleSide::eLeft);
        mNozzleId = pNozzle->mBaseGameObjectId;
    }

    if (gpThrowableArray && gpThrowableArray->mCount)
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
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!mIsButtonOn)
    {
        if (!gpThrowableArray || gpThrowableArray->mCount == 0)
        {
            mIsButtonOn = true;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoomMachine_Button_On));
        }
    }
    else if (mIsButtonOn)
    {
        if (gpThrowableArray && gpThrowableArray->mCount > 0)
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
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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
            pNozzle->DropGrenadeAnimation_445820();
        }
        else
        {
            pNozzle->AlreadyUsed_445860();
        }
    }
}

BoomMachine::~BoomMachine()
{
    BaseGameObject* pObj = sObjectIds.Find_Impl(mNozzleId);
    if (pObj)
    {
        pObj->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}
