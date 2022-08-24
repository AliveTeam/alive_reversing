#include "stdafx_ao.h"
#include "Function.hpp"
#include "BoomMachine.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "BaseAliveGameObject.hpp"
#include "Sfx.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "Grenade.hpp"

namespace AO {

class GrenadeMachineNozzle final : public BaseAnimatedWithPhysicsGameObject
{
public:

    virtual void VUpdate() override
    {
        switch (field_E4_state)
        {
            case BoomMachineStates::eAlreadyUsed_1:
                if (static_cast<s32>(sGnFrame) > field_E8_timer)
                {
                    SFX_Play_Pitch(SoundEffect::ZPop_5, 60, -1800, 0);
                    field_E4_state = BoomMachineStates::eInactive_0;
                }
                break;

            case BoomMachineStates::eDropGrenadeAnimation_2:
                if (static_cast<s32>(sGnFrame) > field_E8_timer)
                {
                    field_E4_state = BoomMachineStates::eDropGrenade_3;
                    mAnim.Set_Animation_Data(AnimId::BoomMachine_Nozzle_DropGrenade, nullptr);
                }
                break;

            case BoomMachineStates::eDropGrenade_3:
                if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    SFX_Play_Pitch(SoundEffect::PickupItem_33, 127, -900, 0);
                    if (!gpThrowableArray_50E26C)
                    {
                        gpThrowableArray_50E26C = relive_new ThrowableArray();
                    }

                    gpThrowableArray_50E26C->Add(field_EC_num_grenades);

                    FP directedScale = {};
                    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
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
                        pNewNade->VThrow(mAnim.mFlags.Get(AnimFlags::eBit5_FlipX) ? FP_FromDouble(-0.75) : FP_FromDouble(0.75), FP_FromInteger(3));
                    }

                    mAnim.Set_Animation_Data(AnimId::BoomMachine_Nozzle_Idle, nullptr);
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
ALIVE_ASSERT_SIZEOF(GrenadeMachineNozzle, 0xF0);

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
        if (!gpThrowableArray_50E26C || gpThrowableArray_50E26C->field_10_count == 0)
        {
            field_E8_bIsButtonOn = 1;
            mAnim.Set_Animation_Data(AnimId::BoomMachine_Button_On, nullptr);
        }
    }
    else if (field_E8_bIsButtonOn == 1)
    {
        if (gpThrowableArray_50E26C && gpThrowableArray_50E26C->field_10_count > 0)
        {
            field_E8_bIsButtonOn = 0;
            mAnim.Set_Animation_Data(AnimId::BoomMachine_Button_Off, nullptr);
        }

        if (mAnim.mCurrentFrame == 3)
        {
            SFX_Play_Pitch(SoundEffect::RedTick_4, 25, -1200, 0);
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

BoomMachine::BoomMachine(relive::Path_BoomMachine* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = ReliveTypes::eBoomMachine;

    const AnimRecord rec = AO::AnimRec(AnimId::BoomMachine_Button_On);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::BoomMachine_Button_On, ppRes);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    field_E4_tlvInfo = tlvInfo;
    mAnim.mRenderMode = TPageAbr::eBlend_1;

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

        const AnimRecord rec2 = AO::AnimRec(AnimId::BoomMachine_Nozzle_Idle);
        u8** ppRes2 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec2.mResourceId, 1, 0);
        pNozzle->Animation_Init(AnimId::BoomMachine_Nozzle_Idle, ppRes2);

        pNozzle->mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        pNozzle->mSpriteScale = mSpriteScale;
        pNozzle->mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
        pNozzle->field_E4_state = BoomMachineStates::eInactive_0;
        pNozzle->mXPos = mXPos + (directedScale * FP_FromInteger(30));
        pNozzle->mYPos = mYPos + (mSpriteScale * FP_FromInteger(-30));
        pNozzle->field_EC_num_grenades = static_cast<s16>(pTlv->mGrenadeAmount);
    }

    pNozzle->mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, pTlv->mNozzleSide == relive::Path_BoomMachine::NozzleSide::eLeft);

    pNozzle->mBaseGameObjectRefCount++;
    field_EC_pNozzle = pNozzle;

    if (gpThrowableArray_50E26C && gpThrowableArray_50E26C->field_10_count)
    {
        field_E8_bIsButtonOn = 1;
        mAnim.Set_Animation_Data(AnimId::BoomMachine_Button_On, nullptr);
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
