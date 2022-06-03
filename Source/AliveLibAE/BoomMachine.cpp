#include "stdafx.h"
#include "BoomMachine.hpp"
#include "Function.hpp"
#include "ObjectIds.hpp"
#include "stdlib.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Throwable.hpp"
#include "Grenade.hpp"
#include "Grid.hpp"

class GrenadeMachineNozzle final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    GrenadeMachineNozzle(FP xpos, FP ypos, FP scale, s16 numGrenades)
        : BaseAnimatedWithPhysicsGameObject(0)
    {
        const AnimRecord& rec = AnimRec(AnimId::BoomMachine_Nozzle_Idle);
        u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
        Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

        mAnim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        mApplyShadows &= ~1u;

        mSpriteScale = scale;

        mXPos = xpos;
        mYPos = ypos;

        field_FC_numGrenades = numGrenades;

        field_F4_state = BoomMachineStates::eInactive_0;
    }

    void DropGrenadeAnimation_445820()
    {
        if (field_F4_state == BoomMachineStates::eInactive_0)
        {
            field_F4_state = BoomMachineStates::eDropGrenadeAnimation_2;
            field_F8_timer = sGnFrame_5C1B84 + 10;
        }
    }

    void AlreadyUsed_445860()
    {
        if (field_F4_state == BoomMachineStates::eInactive_0)
        {
            field_F4_state = BoomMachineStates::eAlreadyUsed_1;
            field_F8_timer = sGnFrame_5C1B84 + 10;
        }
    }

private:
    void VUpdate() override
    {
        switch (field_F4_state)
        {
            case BoomMachineStates::eInactive_0:
                // do nothing
                break;
            case BoomMachineStates::eAlreadyUsed_1:
                if (static_cast<s32>(sGnFrame_5C1B84) > field_F8_timer)
                {
                    SFX_Play_Pitch(SoundEffect::ZPop_4, 60, -1800);
                    field_F4_state = BoomMachineStates::eInactive_0;
                }
                break;

            case BoomMachineStates::eDropGrenadeAnimation_2:
                if (static_cast<s32>(sGnFrame_5C1B84) > field_F8_timer)
                {
                    field_F4_state = BoomMachineStates::eDropGrenade_3;
                    const AnimRecord& animRec = AnimRec(AnimId::BoomMachine_Nozzle_DropGrenade);
                    mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                }
                break;

            case BoomMachineStates::eDropGrenade_3:
                if (mAnim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
                {
                    SFX_Play_Pitch(SoundEffect::PickupItem_28, 127, -900);

                    if (!gpThrowableArray_5D1E2C)
                    {
                        gpThrowableArray_5D1E2C = ae_new<ThrowableArray>();
                    }

                    gpThrowableArray_5D1E2C->Add(field_FC_numGrenades);

                    FP directedScale = {};
                    if (mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        directedScale = -mSpriteScale;
                    }
                    else
                    {
                        directedScale = mSpriteScale;
                    }
                    auto pGrenade = ae_new<Grenade>(
                        (FP_FromInteger(6) * directedScale) + mXPos,
                        (-FP_FromInteger(6) * mSpriteScale) + mYPos,
                        field_FC_numGrenades,
                        0,
                        0,
                        nullptr);
 
                    pGrenade->VThrow((mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) != 0 ? -FP_FromDouble(0.75) : FP_FromDouble(0.75), FP_FromInteger(3));

                    const AnimRecord& animRec = AnimRec(AnimId::BoomMachine_Nozzle_Idle);
                    mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                    field_F4_state = BoomMachineStates::eInactive_0;
                }
                break;
        }
    }

private:
    BoomMachineStates field_F4_state;
    s16 field_F6_pad;
    s32 field_F8_timer;
    s16 field_FC_numGrenades;
    s16 field_FE_pad;
};
ALIVE_ASSERT_SIZEOF(GrenadeMachineNozzle, 0x100);

BoomMachine::BoomMachine(Path_BoomMachine* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eGrenadeMachine_66);

    const AnimRecord& rec = AnimRec(AnimId::BoomMachine_Button_Off);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    mApplyShadows &= ~1u;
    field_F4_tlvInfo = tlvInfo;
    mAnim.mRenderMode = TPageAbr::eBlend_1;

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
    }
    else if (pTlv->field_10_scale == Scale_short::eFull_0)
    {
        mSpriteScale = FP_FromInteger(1);
    }

    mXPos = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2)) + FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    mYPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    auto pNozzle = ae_new<GrenadeMachineNozzle>(
        ((pTlv->field_12_nozzle_side == Path_BoomMachine::NozzleSide::eLeft_1 ? -mSpriteScale : mSpriteScale) * FP_FromInteger(30)) + mXPos,
        (mSpriteScale * FP_FromInteger(-30)) + mYPos,
        mSpriteScale,
        pTlv->field_16_number_of_grenades);
    if (pNozzle)
    {
        pNozzle->mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pTlv->field_12_nozzle_side == Path_BoomMachine::NozzleSide::eLeft_1);
        field_F8_nozzle_id = pNozzle->field_8_object_id;
    }

    if (gpThrowableArray_5D1E2C && gpThrowableArray_5D1E2C->field_20_count)
    {
        field_FC_bIsButtonOn = 1;
        const AnimRecord& animRec = AnimRec(AnimId::BoomMachine_Button_On);
        mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
    }
    else
    {
        field_FC_bIsButtonOn = 0;
    }
}

void BoomMachine::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!field_FC_bIsButtonOn)
    {
        if (!gpThrowableArray_5D1E2C || gpThrowableArray_5D1E2C->field_20_count == 0)
        {
            field_FC_bIsButtonOn = 1;
            const AnimRecord& animRec = AnimRec(AnimId::BoomMachine_Button_On);
            mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        }
    }
    else if (field_FC_bIsButtonOn)
    {
        if (gpThrowableArray_5D1E2C && gpThrowableArray_5D1E2C->field_20_count > 0)
        {
            field_FC_bIsButtonOn = 0;
            const AnimRecord& animRec = AnimRec(AnimId::BoomMachine_Button_Off);
            mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        }

        if (mAnim.field_92_current_frame == 3)
        {
            SFX_Play_Pitch(SoundEffect::RedTick_3, 25, -1200);
        }
    }
}

void BoomMachine::VScreenChanged()
{
    mGameObjectFlags.Set(BaseGameObject::eDead);
}

Bool32 BoomMachine::VIsButtonOn()
{
    return field_FC_bIsButtonOn == 1;
}

void BoomMachine::VHandleButton()
{
    auto pNozzle = static_cast<GrenadeMachineNozzle*>(sObjectIds.Find_Impl(field_F8_nozzle_id));
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
    BaseGameObject* pObj = sObjectIds.Find_Impl(field_F8_nozzle_id);
    if (pObj)
    {
        pObj->mGameObjectFlags.Set(BaseGameObject::eDead);
    }
    Path::TLV_Reset(field_F4_tlvInfo, -1, 0, 0);
}
