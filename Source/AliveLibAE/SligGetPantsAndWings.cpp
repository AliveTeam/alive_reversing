#include "stdafx.h"
#include "SligGetPantsAndWings.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "stdlib.hpp"
#include "FixedPoint.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Slig.hpp"

SligGetPantsAndWings::SligGetPantsAndWings(Path_TLV* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eSligGetPantsOrWings);
    field_F8_tlvInfo = tlvInfo;

    const AnimRecord& rec = AnimRec(AnimId::CrawlingSligLocker_Closed);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::CrawlingSligLocker_Closed, ppRes);

    // HACK: See header for details
    auto pHack = static_cast<Path_Slig*>(pTlv);
    if (pHack->field_10_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
    }
    else if (pHack->field_10_scale == Scale_short::eFull_0)
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
    }

    mXPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);
    mYPos = FP_FromInteger(pTlv->mBottomRight.y);
}

void SligGetPantsAndWings::VUpdate()
{
    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(field_F8_tlvInfo);
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (field_F4_state)
    {
        case State::eWaiting_0:
            if (pTlv->mTlvState)
            {
                field_F4_state = State::eActive_1;
                field_FC_timer = sGnFrame + 8;
            }
            break;

        case State::eActive_1:
            if (static_cast<s32>(sGnFrame) > field_FC_timer)
            {
                field_F4_state = State::eFinished_2;
                SfxPlayMono(SoundEffect::NakedSligTransformEnd_92, 0);
                mAnim.Set_Animation_Data(AnimId::CrawlingSligLocker_Open, nullptr);
            }
            break;

        case State::eFinished_2:
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_F4_state = State::eWaiting_0;
                mAnim.Set_Animation_Data(AnimId::CrawlingSligLocker_Closed, nullptr);
                pTlv->mTlvState = 0;
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
    Path::TLV_Reset(field_F8_tlvInfo, 0, 0, 0);
}
