#include "stdafx_ao.h"
#include "Function.hpp"
#include "BoomMachine.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "Grenade.hpp"
#include "Grid.hpp"

namespace AO {

class GrenadeMachineNozzle final : public BaseAnimatedWithPhysicsGameObject
{
public:

    void LoadAnimations()
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BoomMachine_Nozzle_DropGrenade));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BoomMachine_Nozzle_Idle));
    }

    GrenadeMachineNozzle()
        : BaseAnimatedWithPhysicsGameObject(0)
    {
        LoadAnimations();
    }

    virtual void VUpdate() override
    {
        switch (field_E4_state)
        {
            case BoomMachineStates::eAlreadyUsed_1:
                if (static_cast<s32>(sGnFrame) > field_E8_timer)
                {
                    SFX_Play_Pitch(relive::SoundEffects::ZPop, 60, -1800);
                    field_E4_state = BoomMachineStates::eInactive_0;
                }
                break;

            case BoomMachineStates::eDropGrenadeAnimation_2:
                if (static_cast<s32>(sGnFrame) > field_E8_timer)
                {
                    field_E4_state = BoomMachineStates::eDropGrenade_3;
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoomMachine_Nozzle_DropGrenade));
                }
                break;

            case BoomMachineStates::eDropGrenade_3:
                if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
                {
                    SFX_Play_Pitch(relive::SoundEffects::PickupItem, 127, -900);
                    if (!gThrowableArray)
                    {
                        gThrowableArray = relive_new ThrowableArray();
                    }

                    gThrowableArray->Add(field_EC_num_grenades);

                    FP directedScale = {};
                    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                    {
                        directedScale = -mSpriteScale;
                    }
                    else
                    {
                        directedScale = mSpriteScale;
                    }
                    auto pNewNade = relive_new Grenade(
                        mXPos + (FP_FromInteger(6) * directedScale),
                        mYPos + (-FP_FromInteger(6) * directedScale),
                        field_EC_num_grenades);
                    if (pNewNade)
                    {
                        pNewNade->VThrow(GetAnimation().mFlags.Get(AnimFlags::eFlipX) ? FP_FromDouble(-0.75) : FP_FromDouble(0.75), FP_FromInteger(3));
                    }

                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoomMachine_Nozzle_Idle));
                    field_E4_state = BoomMachineStates::eInactive_0;
                }
                break;
            default:
                return;
        }
    }

    virtual void VScreenChanged() override
    {
        // Empty
    }

    BoomMachineStates field_E4_state = BoomMachineStates::eInactive_0;
    s32 field_E8_timer = 0;
    s16 field_EC_num_grenades = 0;
};

void BoomMachine::VHandleButton()
{
    if (VIsButtonOn())
    {
        if (field_EC_pNozzle->field_E4_state == BoomMachineStates::eInactive_0)
        {
            field_EC_pNozzle->field_E4_state = BoomMachineStates::eDropGrenadeAnimation_2;
            field_EC_pNozzle->field_E8_timer = sGnFrame + 10;
        }
    }
    else
    {
        if (field_EC_pNozzle->field_E4_state == BoomMachineStates::eInactive_0)
        {
            field_EC_pNozzle->field_E4_state = BoomMachineStates::eAlreadyUsed_1;
            field_EC_pNozzle->field_E8_timer = sGnFrame + 10;
        }
    }
}

bool BoomMachine::VIsButtonOn()
{
    return field_E8_bIsButtonOn == 1;
}

void BoomMachine::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_E8_bIsButtonOn == 0)
    {
        if (!gThrowableArray || gThrowableArray->field_10_count == 0)
        {
            field_E8_bIsButtonOn = 1;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoomMachine_Button_On));
        }
    }
    else if (field_E8_bIsButtonOn == 1)
    {
        if (gThrowableArray && gThrowableArray->field_10_count > 0)
        {
            field_E8_bIsButtonOn = 0;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoomMachine_Button_Off));
        }

        if (GetAnimation().GetCurrentFrame() == 3)
        {
            SFX_Play_Pitch(relive::SoundEffects::RedTick, 25, -1200);
        }
    }
}

BoomMachine::~BoomMachine()
{
    if (field_EC_pNozzle)
    {
        field_EC_pNozzle->mBaseGameObjectRefCount--;
        field_EC_pNozzle->mBaseGameObjectFlags.Set(Options::eDead);
    }

    Path::TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
}

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

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    field_E4_tlvInfo = tlvId;
    GetAnimation().SetRenderMode(TPageAbr::eBlend_1);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX) + (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    auto pNozzle = relive_new GrenadeMachineNozzle();
    if (pNozzle)
    {
        FP directedScale = mSpriteScale;
        if (pTlv->mNozzleSide == relive::Path_BoomMachine::NozzleSide::eLeft)
        {
            directedScale = -directedScale;
        }

        pNozzle->Animation_Init(pNozzle->GetAnimRes(AnimId::BoomMachine_Nozzle_Idle));

        pNozzle->GetAnimation().mFlags.Clear(AnimFlags::eSemiTrans);
        pNozzle->mSpriteScale = mSpriteScale;
        pNozzle->mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
        pNozzle->field_E4_state = BoomMachineStates::eInactive_0;
        pNozzle->mXPos = mXPos + (directedScale * FP_FromInteger(30));
        pNozzle->mYPos = mYPos + (mSpriteScale * FP_FromInteger(-30));
        pNozzle->field_EC_num_grenades = static_cast<s16>(pTlv->mGrenadeAmount);
    }

    pNozzle->GetAnimation().mFlags.Set(AnimFlags::eFlipX, pTlv->mNozzleSide == relive::Path_BoomMachine::NozzleSide::eLeft);

    pNozzle->mBaseGameObjectRefCount++;
    field_EC_pNozzle = pNozzle;

    if (gThrowableArray && gThrowableArray->field_10_count)
    {
        field_E8_bIsButtonOn = 1;
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoomMachine_Button_On));
    }
    else
    {
        field_E8_bIsButtonOn = 0;
    }

    mYOffset = 0;
}


void BoomMachine::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

} // namespace AO
