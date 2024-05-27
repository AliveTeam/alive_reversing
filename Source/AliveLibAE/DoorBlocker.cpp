#include "stdafx.h"
#include "DoorBlocker.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"
#include "Path.hpp"

DoorBlocker::DoorBlocker(relive::Path_DoorBlocker* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0),
    mTlvId(tlvId),
    mSwitchId(pTlv->mSwitchId)
{
    SetCanExplode(true);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Door_Lock_Idle));

    SetType(ReliveTypes::eDoorLock);
    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_Shadow_Half_7);
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_Shadow_26);
        SetScale(Scale::Fg);
    }

    if (SwitchStates_Get(mSwitchId))
    {
        SetDead(true);
    }

    SetDoPurpleLightEffect(true);
}

DoorBlocker::~DoorBlocker()
{
    Path::TLV_Reset(mTlvId);
}

void DoorBlocker::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_Lock_Open));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_Lock_Idle));
}

void DoorBlocker::VUpdate()
{
    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
    }

    if (!GetDead())
    {
        if (mDone)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                SetDead(true);
            }
        }
        else if (SwitchStates_Get(mSwitchId))
        {
            SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, 900);
            SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, -100);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Door_Lock_Open));
            mDone = true;
        }
    }
}
