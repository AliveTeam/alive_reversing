#include "stdafx.h"
#include "BackgroundAnimation.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "DDCheat.hpp"
#include "ResourceManager.hpp"
#include "Path.hpp"

BackgroundAnimation::BackgroundAnimation(relive::Path_BackgroundAnimation* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eBackgroundAnimation);
    field_F8_tlvInfo = tlvId;

    /*
    const BgAnimRecord& anim = BgAnimRec(pTlv->mAnimId);

    field_F4_res = reinterpret_cast<AnimationFileHeader**>(Add_Resource(ResourceManager::Resource_Animation, anim.mBgAnimId));
    if (!field_F4_res)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }
    */

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mObjectXPos = FP_FromInteger(pTlv->mTopLeftX);
    mObjectYPos = FP_FromInteger(pTlv->mTopLeftY);

    // TODO: Convert frame table to anim id in conversion
    Animation_Init(ResourceManagerWrapper::LoadAnimation(AnimId::Abe_Head_Gib));

    //Animation_Init(anim.mFrameTableOffset, anim.mMaxW, anim.mMaxH, reinterpret_cast<u8**>(field_F4_res));

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
