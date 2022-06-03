#include "stdafx_ao.h"
#include "Function.hpp"
#include "BoomMachine.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
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
                if (static_cast<s32>(gnFrameCount_507670) > field_E8_timer)
                {
                    SFX_Play_Pitch(SoundEffect::ZPop_5, 60, -1800, 0);
                    field_E4_state = BoomMachineStates::eInactive_0;
                }
                break;

            case BoomMachineStates::eDropGrenadeAnimation_2:
                if (static_cast<s32>(gnFrameCount_507670) > field_E8_timer)
                {
                    field_E4_state = BoomMachineStates::eDropGrenade_3;
                    const AnimRecord& rec = AO::AnimRec(AnimId::BoomMachine_Nozzle_DropGrenade);
                    mAnim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
                }
                break;

            case BoomMachineStates::eDropGrenade_3:
                if (mAnim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    SFX_Play_Pitch(SoundEffect::PickupItem_33, 127, -900, 0);
                    if (!gpThrowableArray_50E26C)
                    {
                        gpThrowableArray_50E26C = ao_new<ThrowableArray>();
                    }

                    gpThrowableArray_50E26C->Add(field_EC_num_grenades);

                    FP directedScale = {};
                    if (mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        directedScale = -mSpriteScale;
                    }
                    else
                    {
                        directedScale = mSpriteScale;
                    }
                    auto pNewNade = ao_new<Grenade>(
                        mXPos + (FP_FromInteger(6) * directedScale),
                        mYPos + (-FP_FromInteger(6) * directedScale),
                        field_EC_num_grenades);
                    if (pNewNade)
                    {
                        pNewNade->VThrow(mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? FP_FromDouble(-0.75) : FP_FromDouble(0.75), FP_FromInteger(3));
                    }

                    const AnimRecord& rec = AO::AnimRec(AnimId::BoomMachine_Nozzle_Idle);
                    mAnim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
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

    s32 field_D4_padding[4];
    BoomMachineStates field_E4_state;
    s16 field_E6_padding;
    s32 field_E8_timer;
    s16 field_EC_num_grenades;
    s16 field_EE_pad;
};
ALIVE_ASSERT_SIZEOF(GrenadeMachineNozzle, 0xF0);

void BoomMachine::VHandleButton()
{
    if (VIsButtonOn())
    {
        if (field_EC_pNozzle->field_E4_state == BoomMachineStates::eInactive_0)
        {
            field_EC_pNozzle->field_E4_state = BoomMachineStates::eDropGrenadeAnimation_2;
            field_EC_pNozzle->field_E8_timer = gnFrameCount_507670 + 10;
        }
    }
    else
    {
        if (field_EC_pNozzle->field_E4_state == BoomMachineStates::eInactive_0)
        {
            field_EC_pNozzle->field_E4_state = BoomMachineStates::eAlreadyUsed_1;
            field_EC_pNozzle->field_E8_timer = gnFrameCount_507670 + 10;
        }
    }
}

Bool32 BoomMachine::VIsButtonOn()
{
    return field_E8_bIsButtonOn == 1;
}

void BoomMachine::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_E8_bIsButtonOn == 0)
    {
        if (!gpThrowableArray_50E26C || gpThrowableArray_50E26C->field_10_count == 0)
        {
            field_E8_bIsButtonOn = 1;
            const AnimRecord& rec = AO::AnimRec(AnimId::BoomMachine_Button_On);
            mAnim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
        }
    }
    else if (field_E8_bIsButtonOn == 1)
    {
        if (gpThrowableArray_50E26C && gpThrowableArray_50E26C->field_10_count > 0)
        {
            field_E8_bIsButtonOn = 0;
            const AnimRecord& rec = AO::AnimRec(AnimId::BoomMachine_Button_Off);
            mAnim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
        }

        if (mAnim.field_92_current_frame == 3)
        {
            SFX_Play_Pitch(SoundEffect::RedTick_4, 25, -1200, 0);
        }
    }
}

BoomMachine::~BoomMachine()
{
    if (field_EC_pNozzle)
    {
        field_EC_pNozzle->field_C_refCount--;
        field_EC_pNozzle->mGameObjectFlags.Set(Options::eDead);
    }

    gMap.TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
}

BoomMachine::BoomMachine(Path_BoomMachine* pTlv, s32 tlvInfo)
{
    mTypeId = Types::eGrenadeMachine_41;

    const AnimRecord& rec = AO::AnimRec(AnimId::BoomMachine_Button_On);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mApplyShadows &= ~1u;
    field_E4_tlvInfo = tlvInfo;
    mAnim.mRenderMode = TPageAbr::eBlend_1;

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
    }

    mXPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x) + (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
    mYPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    auto pNozzle = ao_new<GrenadeMachineNozzle>();
    if (pNozzle)
    {
        FP directedScale = mSpriteScale;
        if (pTlv->field_1A_nozzle_side == Path_BoomMachine::NozzleSide::eLeft_1)
        {
            directedScale = -directedScale;
        }

        const AnimRecord rec2 = AO::AnimRec(AnimId::BoomMachine_Nozzle_Idle);
        u8** ppRes2 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec2.mResourceId, 1, 0);
        pNozzle->Animation_Init_417FD0(rec2.mFrameTableOffset, rec2.mMaxW, rec2.mMaxH, ppRes2, 1);

        pNozzle->mAnim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        pNozzle->mSpriteScale = mSpriteScale;
        pNozzle->mApplyShadows &= ~1u;
        pNozzle->field_E4_state = BoomMachineStates::eInactive_0;
        pNozzle->mXPos = mXPos + (directedScale * FP_FromInteger(30));
        pNozzle->mYPos = mYPos + (mSpriteScale * FP_FromInteger(-30));
        pNozzle->field_EC_num_grenades = static_cast<s16>(pTlv->field_1E_number_of_grenades);
    }

    pNozzle->mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pTlv->field_1A_nozzle_side == Path_BoomMachine::NozzleSide::eLeft_1);

    pNozzle->field_C_refCount++;
    field_EC_pNozzle = pNozzle;

    if (gpThrowableArray_50E26C && gpThrowableArray_50E26C->field_10_count)
    {
        field_E8_bIsButtonOn = 1;
        mAnim.Set_Animation_Data(1572, 0);
    }
    else
    {
        field_E8_bIsButtonOn = 0;
    }

    mYOffset = 0;
}


void BoomMachine::VScreenChanged()
{
    mGameObjectFlags.Set(BaseGameObject::eDead);
}

} // namespace AO
