#include "stdafx.h"
#include "DoorBlocker.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"

DoorBlocker::DoorBlocker(Path_DoorBlocker* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    mGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    field_11A_switch_id = pTlv->field_12_switch_id;

    const AnimRecord& rec = AnimRec(AnimId::Door_Lock_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_118_bDone &= ~1u;

    SetType(AETypes::eDoorLock_34);
    mXPos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    mYPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_11C_tlvInfo = tlvInfo;

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_Shadow_Half_7;
        mScale = 0;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_Shadow_26;
        mScale = 1;
    }

    if (SwitchStates_Get(field_11A_switch_id))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mApplyShadows |= 2u;
}

DoorBlocker::~DoorBlocker()
{
    Path::TLV_Reset(field_11C_tlvInfo, -1, 0, 0);
}

void DoorBlocker::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!mGameObjectFlags.Get(BaseGameObject::eDead))
    {
        if (field_118_bDone & 1)
        {
            if (mAnim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
        else if (SwitchStates_Get(field_11A_switch_id))
        {
            SFX_Play_Pitch(SoundEffect::DoorEffect_57, 100, 900);
            SFX_Play_Pitch(SoundEffect::DoorEffect_57, 100, -100);
            const AnimRecord& animRec = AnimRec(AnimId::Door_Lock_Open);
            mAnim.Set_Animation_Data(animRec.mFrameTableOffset, 0);
            field_118_bDone |= 1u;
        }
    }
}
