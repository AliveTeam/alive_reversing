#include "stdafx.h"
#include "BackgroundAnimation.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "DDCheat.hpp"

BackgroundAnimation::BackgroundAnimation(Path_BackgroundAnimation* pTlv, TlvItemInfoUnion tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eBackgroundAnimation);
    field_F8_tlvInfo = tlvInfo;

    const BgAnimRecord& anim = BgAnimRec(pTlv->mAnimId);
    field_F4_res = reinterpret_cast<AnimationFileHeader**>(Add_Resource(ResourceManager::Resource_Animation, anim.mBgAnimId));
    if (!field_F4_res)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    mXPos = FP_FromInteger(pTlv->mTopLeft.x);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);

    field_FC_animXPos = FP_FromInteger(pTlv->mTopLeft.x);
    field_100_animYPos = FP_FromInteger(pTlv->mTopLeft.y);

    Animation_Init(anim.mFrameTableOffset, anim.mMaxW, anim.mMaxH, reinterpret_cast<u8**>(field_F4_res));

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans, pTlv->mIsSemiTrans == Choice_short::eYes_1);
    mAnim.mFlags.Set(AnimFlags::eBit16_bBlending);

    mAnim.mRenderMode = pTlv->mSemiTransMode;

    if (pTlv->mLayer > Layer::eLayer_0)
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
        mXPos = FP_FromInteger(sTweakX_5C1BD0) + field_FC_animXPos;
        mYPos = FP_FromInteger(sTweakY_5C1BD4) + field_100_animYPos;
    }
}

void BackgroundAnimation::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

BackgroundAnimation::~BackgroundAnimation()
{
    Path::TLV_Reset(field_F8_tlvInfo.all, -1, 0, 0);
}
