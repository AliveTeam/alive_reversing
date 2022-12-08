#include "stdafx.h"
#include "BackgroundAnimation.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "DDCheat.hpp"
#include "Path.hpp"

BackgroundAnimation::BackgroundAnimation(relive::Path_BackgroundAnimation* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eBackgroundAnimation);
    field_F8_tlvInfo = tlvId;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mObjectXPos = FP_FromInteger(pTlv->mTopLeftX);
    mObjectYPos = FP_FromInteger(pTlv->mTopLeftY);

    const AnimRecord& anim = ::BgAnimRec(pTlv->mAnimId);
    AnimResource res = ResourceManagerWrapper::LoadAnimation(anim.mId);

    Animation_Init(res);

    GetAnimation().mFlags.Set(AnimFlags::eSemiTrans, pTlv->mIsSemiTrans == relive::reliveChoice::eYes);
    GetAnimation().mFlags.Set(AnimFlags::eBlending);

    GetAnimation().SetRenderMode(pTlv->mSemiTransMode);

    if (pTlv->mLayer > relive::Path_BackgroundAnimation::Layer::eLayer0)
    {
        const s32 translatedLayer = static_cast<s32>(pTlv->mLayer) - 1;
        if (!translatedLayer)
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_Above_FG1_Half_20);
        }
        if (translatedLayer == 1)
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_Above_FG1_39);
        }
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_1);
    }
}

void BackgroundAnimation::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
    else
    {
        mXPos = FP_FromInteger(gTweakX) + mObjectXPos;
        mYPos = FP_FromInteger(gTweakY) + mObjectYPos;
    }
}

void BackgroundAnimation::VScreenChanged()
{
    SetDead(true);
}

BackgroundAnimation::~BackgroundAnimation()
{
    Path::TLV_Reset(field_F8_tlvInfo, -1, 0, 0);
}
