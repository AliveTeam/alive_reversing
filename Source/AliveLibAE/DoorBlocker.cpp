#include "stdafx.h"
#include "DoorBlocker.hpp"
#include "SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"
#include "Path.hpp"

DoorBlocker::DoorBlocker(relive::Path_DoorBlocker* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    field_11A_switch_id = pTlv->mSwitchId;

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Door_Lock_Idle));

    field_118_bDone &= ~1u;

    SetType(ReliveTypes::eDoorLock);
    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    field_11C_tlvInfo = tlvId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_Shadow_Half_7;
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_Shadow_26;
        mScale = Scale::Fg;
    }

    if (SwitchStates_Get(field_11A_switch_id))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
}

DoorBlocker::~DoorBlocker()
{
    Path::TLV_Reset(field_11C_tlvInfo, -1, 0, 0);
}

void DoorBlocker::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_Lock_Open));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_Lock_Idle));
}

void DoorBlocker::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        if (field_118_bDone & 1)
        {
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
        else if (SwitchStates_Get(field_11A_switch_id))
        {
            SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, 900);
            SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, -100);
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::Door_Lock_Open));
            field_118_bDone |= 1u;
        }
    }
}
