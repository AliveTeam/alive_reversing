#include "stdafx_ao.h"
#include "Function.hpp"
#include "Well.hpp"
#include "Math.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "SwitchStates.hpp"
#include "Leaf.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "DDCheat.hpp"

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
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void Well::VRender(PrimHeader** ppOt)
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
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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
                            pLeaf->mAnim.mRenderLayer = Layer::eLayer_BeforeWell_Half_3;
                        }
                        else
                        {
                            pLeaf->mAnim.mRenderLayer = Layer::eLayer_BeforeWell_22;
                        }
                    }
                }
            }
        }
    }
}

void Well::WellExpress_Init(relive::Path_WellExpress* pTlv, FP /*xpos*/, FP ypos)
{
    //u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, pTlv->field_1E_anim_id, 1, 0);
    //auto pHeader = reinterpret_cast<AnimHeader*>(*ppRes);
    //LOG_INFO("frametableoffset: " << pHeader->field_4_frame_table_offset << " anim id: " << pTlv->field_1E_anim_id << " max w: " << pHeader->field_0_max_w << " max h: " << pHeader->field_2_max_h);

   // const BgAnimRecord& anim = AO::BgAnimRec(pTlv->mAnimId);
   // u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, anim.mBgAnimId, 1, 0);
    //if (ppRes)
    {
        //Animation_Init(anim.mFrameTableOffset, anim.mMaxW, anim.mMaxH, ppRes);
        // TODO: Fix bg anims
        Animation_Init(ResourceManagerWrapper::LoadAnimation(AnimId::Abe_Head_Gib));
        mAnim.mFlags.Clear(AnimFlags::eSemiTrans);
        mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    }
    /*
    else
    {
        mAnim.mFlags.Clear(AnimFlags::eRender);
        mBaseGameObjectFlags.Clear(Options::eDrawable_Bit4);
    }*/

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mAnim.mRenderLayer = Layer::eLayer_Well_Half_4;
        mLeafScale = FP_FromDouble(0.5);
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_Well_23;
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
    //u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, pTlv->field_1E_anim_id, 1, 0);
    //auto pHeader = reinterpret_cast<AnimHeader*>(*ppRes);
    //LOG_INFO("frametableoffset: " << pHeader->field_4_frame_table_offset << " anim id: " << pTlv->field_1E_anim_id << " max w: " << pHeader->field_0_max_w << " max h: " << pHeader->field_2_max_h);

    //const BgAnimRecord& anim = AO::BgAnimRec(pTlv->mAnimId);
    //u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, anim.mBgAnimId, 1, 0);
    //if (ppRes)
    {
        // TODO: Fix epic frame tables in tlvs
        Animation_Init(ResourceManagerWrapper::LoadAnimation(AnimId::Abe_Head_Gib));
        //Animation_Init(anim.mFrameTableOffset, anim.mMaxW, anim.mMaxH, ppRes);

        mAnim.mFlags.Clear(AnimFlags::eSemiTrans);
        mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    }
    /*
    else
    {
        mAnim.mFlags.Clear(AnimFlags::eRender);
        mBaseGameObjectFlags.Clear(Options::eDrawable_Bit4);
    }*/

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mAnim.mRenderLayer = Layer::eLayer_Well_Half_4;
        mLeafScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_Well_23;
        mLeafScale = FP_FromInteger(1);
        mScale = Scale::Fg;
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
