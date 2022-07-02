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
        Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

        mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;

        mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
        mBaseAnimatedWithPhysicsGameObject_YPos = ypos;

        field_FC_numGrenades = numGrenades;

        field_F4_state = BoomMachineStates::eInactive_0;
    }

    void DropGrenadeAnimation_445820()
    {
        if (field_F4_state == BoomMachineStates::eInactive_0)
        {
            field_F4_state = BoomMachineStates::eDropGrenadeAnimation_2;
            field_F8_timer = sGnFrame + 10;
        }
    }

    void AlreadyUsed_445860()
    {
        if (field_F4_state == BoomMachineStates::eInactive_0)
        {
            field_F4_state = BoomMachineStates::eAlreadyUsed_1;
            field_F8_timer = sGnFrame + 10;
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
                if (static_cast<s32>(sGnFrame) > field_F8_timer)
                {
                    SFX_Play_Pitch(SoundEffect::ZPop_4, 60, -1800);
                    field_F4_state = BoomMachineStates::eInactive_0;
                }
                break;

            case BoomMachineStates::eDropGrenadeAnimation_2:
                if (static_cast<s32>(sGnFrame) > field_F8_timer)
                {
                    field_F4_state = BoomMachineStates::eDropGrenade_3;
                    const AnimRecord& animRec = AnimRec(AnimId::BoomMachine_Nozzle_DropGrenade);
                    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                }
                break;

            case BoomMachineStates::eDropGrenade_3:
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
                {
                    SFX_Play_Pitch(SoundEffect::PickupItem_28, 127, -900);

                    if (!gpThrowableArray_5D1E2C)
                    {
                        gpThrowableArray_5D1E2C = relive_new ThrowableArray();
                    }

                    gpThrowableArray_5D1E2C->Add(field_FC_numGrenades);

                    FP directedScale = {};
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        directedScale = -mBaseAnimatedWithPhysicsGameObject_SpriteScale;
                    }
                    else
                    {
                        directedScale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
                    }
                    auto pGrenade = relive_new Grenade(
                        (FP_FromInteger(6) * directedScale) + mBaseAnimatedWithPhysicsGameObject_XPos,
                        (-FP_FromInteger(6) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_YPos,
                        field_FC_numGrenades,
                        0,
                        0,
                        nullptr);
 
                    pGrenade->VThrow((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) != 0 ? -FP_FromDouble(0.75) : FP_FromDouble(0.75), FP_FromInteger(3));

                    const AnimRecord& animRec = AnimRec(AnimId::BoomMachine_Nozzle_Idle);
                    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                    field_F4_state = BoomMachineStates::eInactive_0;
                }
                break;
        }
    }

private:
    BoomMachineStates field_F4_state;
    s32 field_F8_timer;
    s16 field_FC_numGrenades;
};
ALIVE_ASSERT_SIZEOF(GrenadeMachineNozzle, 0x100);

BoomMachine::BoomMachine(Path_BoomMachine* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eGrenadeMachine);

    const AnimRecord& rec = AnimRec(AnimId::BoomMachine_Button_Off);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    field_F4_tlvInfo = tlvInfo;
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
    }
    else if (pTlv->field_10_scale == Scale_short::eFull_0)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2)) + FP_FromInteger(pTlv->mTopLeft.x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);

    auto pNozzle = relive_new GrenadeMachineNozzle(
        ((pTlv->field_12_nozzle_side == Path_BoomMachine::NozzleSide::eLeft_1 ? -mBaseAnimatedWithPhysicsGameObject_SpriteScale : mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(30)) + mBaseAnimatedWithPhysicsGameObject_XPos,
        (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-30)) + mBaseAnimatedWithPhysicsGameObject_YPos,
        mBaseAnimatedWithPhysicsGameObject_SpriteScale,
        pTlv->field_16_number_of_grenades);
    if (pNozzle)
    {
        pNozzle->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pTlv->field_12_nozzle_side == Path_BoomMachine::NozzleSide::eLeft_1);
        field_F8_nozzle_id = pNozzle->field_8_object_id;
    }

    if (gpThrowableArray_5D1E2C && gpThrowableArray_5D1E2C->field_20_count)
    {
        field_FC_bIsButtonOn = 1;
        const AnimRecord& animRec = AnimRec(AnimId::BoomMachine_Button_On);
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
    }
    else
    {
        field_FC_bIsButtonOn = 0;
    }
}

void BoomMachine::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!field_FC_bIsButtonOn)
    {
        if (!gpThrowableArray_5D1E2C || gpThrowableArray_5D1E2C->field_20_count == 0)
        {
            field_FC_bIsButtonOn = 1;
            const AnimRecord& animRec = AnimRec(AnimId::BoomMachine_Button_On);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        }
    }
    else if (field_FC_bIsButtonOn)
    {
        if (gpThrowableArray_5D1E2C && gpThrowableArray_5D1E2C->field_20_count > 0)
        {
            field_FC_bIsButtonOn = 0;
            const AnimRecord& animRec = AnimRec(AnimId::BoomMachine_Button_Off);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        }

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 3)
        {
            SFX_Play_Pitch(SoundEffect::RedTick_3, 25, -1200);
        }
    }
}

void BoomMachine::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

bool BoomMachine::VIsButtonOn()
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
        pObj->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    Path::TLV_Reset(field_F4_tlvInfo, -1, 0, 0);
}
