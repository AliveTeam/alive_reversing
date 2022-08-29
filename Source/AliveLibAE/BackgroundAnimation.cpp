#include "stdafx.h"
#include "BackgroundAnimation.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "DDCheat.hpp"

BackgroundAnimation::BackgroundAnimation(relive::Path_BackgroundAnimation* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eBackgroundAnimation);
    field_F8_tlvInfo = tlvId;

    const BgAnimRecord& anim = BgAnimRec(pTlv->mAnimId);
    field_F4_res = reinterpret_cast<AnimationFileHeader**>(Add_Resource(ResourceManager::Resource_Animation, anim.mBgAnimId));
    if (!field_F4_res)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mObjectXPos = FP_FromInteger(pTlv->mTopLeftX);
    mObjectYPos = FP_FromInteger(pTlv->mTopLeftY);

    Animation_Init(anim.mFrameTableOffset, anim.mMaxW, anim.mMaxH, reinterpret_cast<u8**>(field_F4_res));

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans, pTlv->mIsSemiTrans == relive::reliveChoice::eYes);
    mAnim.mFlags.Set(AnimFlags::eBit16_bBlending);

    mAnim.mRenderMode = pTlv->mSemiTransMode;

    if (pTlv->mLayer > relive::Path_BackgroundAnimation::Layer::eLayer0)
    {
        const s32 translatedLayer = static_cast<s32>(pTlv->mLayer) - 1;
        if (!translatedLayer)
        {
            mAnim.mRenderLayer = Layer::eLayer_Above_FG1_Half_20;
        }
        if (translatedLayer == 1)
        {
            mAnim.mRenderLayer = Layer::eLayer_Above_FG1_39;
        }
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_1;
    }
}

void BackgroundAnimation::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        mXPos = FP_FromInteger(sTweakX_5C1BD0) + mObjectXPos;
        mYPos = FP_FromInteger(sTweakY_5C1BD4) + mObjectYPos;
    }
}

void BackgroundAnimation::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

BackgroundAnimation::~BackgroundAnimation()
{
    Path::TLV_Reset(field_F8_tlvInfo, -1, 0, 0);
}
