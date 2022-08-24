#include "stdafx.h"
#include "SligGetPantsAndWings.hpp"
#include "Function.hpp"
#include "../relive_lib/Events.hpp"
#include "stdlib.hpp"
#include "FixedPoint.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Slig.hpp"

SligGetPantsAndWings::SligGetPantsAndWings(Path_TLV* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eSligGetPantsOrWings);
    mTlvInfo = tlvInfo;

    const AnimRecord& rec = AnimRec(AnimId::CrawlingSligLocker_Closed);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::CrawlingSligLocker_Closed, ppRes);

    // HACK: See header for details
    auto pHack = static_cast<Path_Slig*>(pTlv);
    if (pHack->mScale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
    }
    else if (pHack->mScale == Scale_short::eFull_0)
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
    }

    mXPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);
    mYPos = FP_FromInteger(pTlv->mBottomRight.y);
}

void SligGetPantsAndWings::VUpdate()
{
    Path_TLV* pTlv = sPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo);
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
                SfxPlayMono(SoundEffect::NakedSligTransformEnd_92, 0);
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
