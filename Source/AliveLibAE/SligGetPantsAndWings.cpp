#include "stdafx.h"
#include "SligGetPantsAndWings.hpp"
#include "../relive_lib/Events.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Path.hpp"

void SligGetPantsAndWings::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::CrawlingSligLocker_Closed));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::CrawlingSligLocker_Open));
}

SligGetPantsAndWings::SligGetPantsAndWings(relive::Path_TLV* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eSligGetPantsOrWings);
    mTlvInfo = tlvId;

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::CrawlingSligLocker_Closed));

    // HACK: See header for details
    auto pHack = static_cast<relive::Path_Slig*>(pTlv);
    if (pHack->mData.mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
    }
    else if (pHack->mData.mScale == relive::reliveScale::eFull)
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
    }

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mBottomRightY);
}

void SligGetPantsAndWings::VUpdate()
{
    relive::Path_TLV* pTlv = sPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo);
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (mState)
    {
        case State::eWaiting:
            if (pTlv->mTlvSpecificMeaning)
            {
                mState = State::eActive;
                mTransformTimer = sGnFrame + 8;
            }
            break;

        case State::eActive:
            if (static_cast<s32>(sGnFrame) > mTransformTimer)
            {
                mState = State::eFinished;
                SfxPlayMono(relive::SoundEffects::NakedSligTransformEnd, 0);
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::CrawlingSligLocker_Open));
            }
            break;

        case State::eFinished:
            if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                mState = State::eWaiting;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::CrawlingSligLocker_Closed));
                pTlv->mTlvSpecificMeaning = 0;
            }
            break;
    }
}

void SligGetPantsAndWings::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

SligGetPantsAndWings::~SligGetPantsAndWings()
{
    Path::TLV_Reset(mTlvInfo, 0, 0, 0);
}
