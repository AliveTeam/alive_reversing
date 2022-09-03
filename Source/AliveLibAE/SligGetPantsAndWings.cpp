#include "stdafx.h"
#include "SligGetPantsAndWings.hpp"
#include "Function.hpp"
#include "../relive_lib/Events.hpp"
#include "stdlib.hpp"
#include "FixedPoint.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Slig.hpp"

SligGetPantsAndWings::SligGetPantsAndWings(relive::Path_TLV* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eSligGetPantsOrWings);
    mTlvInfo = tlvId;

    const AnimRecord& rec = AnimRec(AnimId::CrawlingSligLocker_Closed);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::CrawlingSligLocker_Closed, ppRes);

    // HACK: See header for details
    auto pHack = static_cast<relive::Path_Slig*>(pTlv);
    if (pHack->mData.mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
    }
    else if (pHack->mData.mScale == relive::reliveScale::eFull)
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
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
                mAnim.Set_Animation_Data(AnimId::CrawlingSligLocker_Open, nullptr);
            }
            break;

        case State::eFinished:
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mState = State::eWaiting;
                mAnim.Set_Animation_Data(AnimId::CrawlingSligLocker_Closed, nullptr);
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
