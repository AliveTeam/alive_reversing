#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Well.hpp"
#include "Math.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Leaf.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "DDCheat.hpp"
#include "Path.hpp"

namespace AO {

static u8 sWellRndSeed = 4;

static s16 Well_NextRandom()
{
    return gRandomBytes[sWellRndSeed++];
}

Well::~Well()
{
    if (mTlvInfo.IsValid())
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }
}

void Well::VScreenChanged()
{
    SetDead(true);
}

void Well::VRender(BasePrimitive** ppOt)
{
    mXPos += FP_FromInteger(gTweakX);
    mYPos += FP_FromInteger(gTweakY);
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    mXPos -= FP_FromInteger(gTweakX);
    mYPos -= FP_FromInteger(gTweakY);
}

void Well::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }

    if (mEmitLeaves == relive::reliveChoice::eYes)
    {
        // Always on or has been enabled?
        if (!mSwitchId || SwitchStates_Get(mSwitchId))
        {
            // Random chance of leaves emitting
            if (Well_NextRandom() < 10)
            {
                auto pLeaf = relive_new Leaf(
                    mLeafX,
                    mLeafY,
                    mExitX,
                    mExitY,
                    mLeafScale);

                if (mExitY > FP_FromInteger(0))
                {
                    if (pLeaf)
                    {
                        if (mLeafScale == FP_FromDouble(0.5))
                        {
                            pLeaf->GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_Half_3);
                        }
                        else
                        {
                            pLeaf->GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_22);
                        }
                    }
                }
            }
        }
    }
}

void Well::WellExpress_Init(relive::Path_WellExpress* pTlv, FP /*xpos*/, FP ypos)
{
    if (pTlv->mAnimId != 0)
    {
        const AnimRecord& anim = AO::BgAnimRec(pTlv->mAnimId);

        Animation_Init(ResourceManagerWrapper::LoadAnimation(anim.mId));

        GetAnimation().SetSemiTrans(false);
    }

    SetApplyShadowZoneColour(false);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_Well_Half_4);
        mLeafScale = FP_FromDouble(0.5);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_Well_23);
        mLeafScale = FP_FromInteger(1);
    }

    mSwitchId = pTlv->mSwitchId;
    mExitX = FP_FromInteger(pTlv->mExitX) / FP_FromInteger(100);
    mExitY = FP_FromInteger(pTlv->mExitY) / FP_FromInteger(100);

    mEmitLeaves = pTlv->mEmitLeaves;

    if (mEmitLeaves == relive::reliveChoice::eYes)
    {
        mLeafX = FP_FromInteger(pTlv->mLeafX);
        if (!FP_GetExponent(mLeafX))
        {
            mLeafX = FP_FromInteger(pTlv->mTopLeftX
                                                + (PsxToPCX(pTlv->mBottomRightX - pTlv->mTopLeftX, +11)
                                                   / 2));
        }

        mLeafY = FP_FromInteger(pTlv->mLeafY);
        if (!FP_GetExponent(mLeafY))
        {
            mLeafY = ypos;
        }
    }
}

void Well::WellLocal_Init(relive::Path_WellLocal* pTlv, FP /*xpos*/, FP ypos)
{
    const AnimRecord& anim = AO::BgAnimRec(pTlv->mAnimId);

    Animation_Init(ResourceManagerWrapper::LoadAnimation(anim.mId));
    GetAnimation().SetSemiTrans(false);

    SetApplyShadowZoneColour(false);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_Well_Half_4);
        mLeafScale = FP_FromDouble(0.5);
        SetScale(Scale::Bg);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_Well_23);
        mLeafScale = FP_FromInteger(1);
        SetScale(Scale::Fg);
    }

    mSwitchId = pTlv->mSwitchId;
    mExitX = FP_FromInteger(pTlv->mExitX) / FP_FromInteger(100);
    mExitY = FP_FromInteger(pTlv->mExitY) / FP_FromInteger(100);

    mEmitLeaves = pTlv->mEmitLeaves;

    if (mEmitLeaves == relive::reliveChoice::eYes)
    {
        mLeafX = FP_FromInteger(pTlv->mLeafX);
        if (!FP_GetExponent(mLeafX))
        {
            mLeafX = FP_FromInteger(pTlv->mTopLeftX + (PsxToPCX(pTlv->mBottomRightX - pTlv->mTopLeftX, 11) / 2));
        }

        mLeafY = FP_FromInteger(pTlv->mLeafY);
        if (!FP_GetExponent(mLeafY))
        {
            mLeafY = ypos;
        }
    }
}

Well::Well(relive::Path_WellBase* pTlv, FP xpos, FP ypos, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mTlvInfo = tlvId;
    SetType(ReliveTypes::eWell);

    mRGB.SetRGB(128, 128, 128);

    mYPos = ypos;
    mXPos = xpos;

    if (pTlv->mTlvType == ReliveTypes::eWellLocal)
    {
        WellLocal_Init(static_cast<relive::Path_WellLocal*>(pTlv), xpos, ypos);
    }
    else
    {
        WellExpress_Init(static_cast<relive::Path_WellExpress*>(pTlv), xpos, ypos);
    }
}

} // namespace AO
