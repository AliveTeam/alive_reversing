#include "stdafx.h"
#include "BackgroundAnimation.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"

BackgroundAnimation::BackgroundAnimation(Path_BackgroundAnimation* pTlv, TlvItemInfoUnion tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eBackgroundAnimation);
    field_F8_tlvInfo = tlvInfo;

    const BgAnimRecord& anim = BgAnimRec(pTlv->field_10_anim_id);
    field_F4_res = reinterpret_cast<AnimHeader**>(Add_Resource(ResourceManager::Resource_Animation, anim.mBgAnimId));
    if (!field_F4_res)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->mTopLeft.x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);

    field_FC_animXPos = FP_FromInteger(pTlv->mTopLeft.x);
    field_100_animYPos = FP_FromInteger(pTlv->mTopLeft.y);

    Animation_Init(anim.mFrameTableOffset, anim.mMaxW, anim.mMaxH, reinterpret_cast<u8**>(field_F4_res));

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans, pTlv->field_12_is_semi_trans == Choice_short::eYes_1);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit16_bBlending);

    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = pTlv->field_14_semi_trans_mode;

    if (pTlv->field_1A_layer > Layer::eLayer_0)
    {
        const s32 translatedLayer = static_cast<s32>(pTlv->field_1A_layer) - 1;
        if (!translatedLayer)
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Above_FG1_Half_20;
        }
        if (translatedLayer == 1)
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Above_FG1_39;
        }
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_1;
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
        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(sTweakX_5C1BD0) + field_FC_animXPos;
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(sTweakY_5C1BD4) + field_100_animYPos;
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
