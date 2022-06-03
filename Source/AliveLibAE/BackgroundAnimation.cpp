#include "stdafx.h"
#include "BackgroundAnimation.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"

BackgroundAnimation::BackgroundAnimation(Path_BackgroundAnimation* pTlv, TlvItemInfoUnion tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eBackgroundAnimation_7);
    field_F8_tlvInfo = tlvInfo;

    const BgAnimRecord& anim = BgAnimRec(pTlv->field_10_anim_id);
    field_F4_res = reinterpret_cast<AnimHeader**>(Add_Resource(ResourceManager::Resource_Animation, anim.mBgAnimId));
    if (!field_F4_res)
    {
        mGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    mXPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    mYPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_FC_animXPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_100_animYPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    Animation_Init(
        anim.mFrameTableOffset,
        anim.mMaxW,
        anim.mMaxH,
        reinterpret_cast<u8**>(field_F4_res),
        1,
        1u);

    mAnim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans, pTlv->field_12_is_semi_trans == Choice_short::eYes_1);
    mAnim.mAnimFlags.Set(AnimFlags::eBit16_bBlending);

    mAnim.mRenderMode = pTlv->field_14_semi_trans_mode;

    if (pTlv->field_1A_layer > Layer::eLayer_0)
    {
        const s32 translatedLayer = static_cast<s32>(pTlv->field_1A_layer) - 1;
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
    if (Event_Get(kEventDeathReset))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        mXPos = FP_FromInteger(sTweakX_5C1BD0) + field_FC_animXPos;
        mYPos = FP_FromInteger(sTweakY_5C1BD4) + field_100_animYPos;
    }
}

void BackgroundAnimation::VScreenChanged()
{
    mGameObjectFlags.Set(BaseGameObject::eDead);
}

BackgroundAnimation::~BackgroundAnimation()
{
    Path::TLV_Reset(field_F8_tlvInfo.all, -1, 0, 0);
}
