#include "stdafx_ao.h"
#include "Function.hpp"
#include "Well.hpp"
#include "Math.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "Leaf.hpp"
#include "Events.hpp"
#include "PsxDisplay.hpp"
#include "DDCheat.hpp"

namespace AO {

ALIVE_VAR(1, 0x4D1488, u8, sWellRndSeed_4D1488, 4);

static s16 Well_NextRandom()
{
    return sRandomBytes_4BBE30[sWellRndSeed_4D1488++];
}

Well::~Well()
{
    if (field_E4_tlvInfo != -1)
    {
        gMap.TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
    }
}

void Well::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void Well::VRender(PrimHeader** ppOt)
{
    mBaseAnimatedWithPhysicsGameObject_XPos += FP_FromInteger(gTweak_X_5076D8);
    mBaseAnimatedWithPhysicsGameObject_YPos += FP_FromInteger(gTweak_Y_5076DC);
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    mBaseAnimatedWithPhysicsGameObject_XPos -= FP_FromInteger(gTweak_X_5076D8);
    mBaseAnimatedWithPhysicsGameObject_YPos -= FP_FromInteger(gTweak_Y_5076DC);
}

void Well::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        gMap.TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
    }

    if (field_100_emit_leaves == Choice_short::eYes_1)
    {
        // Always on or has been enabled?
        if (!field_E8_switch_id || SwitchStates_Get(field_E8_switch_id))
        {
            // Random chance of leaves emitting
            if (Well_NextRandom() < 10)
            {
                auto pLeaf = relive_new Leaf(
                    field_F8_leaf_xpos,
                    field_FC_leaf_ypos,
                    field_F0_exit_x,
                    field_F4_exit_y,
                    field_EC_scale);

                if (field_F4_exit_y > FP_FromInteger(0))
                {
                    if (pLeaf)
                    {
                        if (field_EC_scale == FP_FromDouble(0.5))
                        {
                            pLeaf->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeWell_Half_3;
                        }
                        else
                        {
                            pLeaf->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeWell_22;
                        }
                    }
                }
            }
        }
    }
}

void Well::WellExpress_Init(Path_WellExpress* pTlv, FP /*xpos*/, FP ypos)
{
    //u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, pTlv->field_1E_anim_id, 1, 0);
    //auto pHeader = reinterpret_cast<AnimHeader*>(*ppRes);
    //LOG_INFO("frametableoffset: " << pHeader->field_4_frame_table_offset << " anim id: " << pTlv->field_1E_anim_id << " max w: " << pHeader->field_0_max_w << " max h: " << pHeader->field_2_max_h);

    const BgAnimRecord& anim = AO::BgAnimRec(pTlv->field_1E_anim_id);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, anim.mBgAnimId, 1, 0);
    if (ppRes)
    {
        Animation_Init(anim.mFrameTableOffset, anim.mMaxW, anim.mMaxH, ppRes);
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        mBaseGameObjectFlags.Clear(Options::eDrawable_Bit4);
    }

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Well_Half_4;
        field_EC_scale = FP_FromDouble(0.5);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Well_23;
        field_EC_scale = FP_FromInteger(1);
    }

    field_E8_switch_id = pTlv->field_1A_switch_id;
    field_F0_exit_x = FP_FromInteger(pTlv->field_20_exit_x) / FP_FromInteger(100);
    field_F4_exit_y = FP_FromInteger(pTlv->field_22_exit_y) / FP_FromInteger(100);

    field_100_emit_leaves = pTlv->field_34_emit_leaves;

    if (field_100_emit_leaves == Choice_short::eYes_1)
    {
        field_F8_leaf_xpos = FP_FromInteger(pTlv->field_36_leaf_x);
        if (!FP_GetExponent(field_F8_leaf_xpos))
        {
            field_F8_leaf_xpos = FP_FromInteger(pTlv->mTopLeft.x
                                                + (PsxToPCX(pTlv->mBottomRight.x - pTlv->mTopLeft.x, +11)
                                                   / 2));
        }

        field_FC_leaf_ypos = FP_FromInteger(pTlv->field_38_leaf_y);
        if (!FP_GetExponent(field_FC_leaf_ypos))
        {
            field_FC_leaf_ypos = ypos;
        }
    }
}

void Well::WellLocal_Init(Path_WellLocal* pTlv, FP /*xpos*/, FP ypos)
{
    //u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, pTlv->field_1E_anim_id, 1, 0);
    //auto pHeader = reinterpret_cast<AnimHeader*>(*ppRes);
    //LOG_INFO("frametableoffset: " << pHeader->field_4_frame_table_offset << " anim id: " << pTlv->field_1E_anim_id << " max w: " << pHeader->field_0_max_w << " max h: " << pHeader->field_2_max_h);

    const BgAnimRecord& anim = AO::BgAnimRec(pTlv->field_1E_anim_id);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, anim.mBgAnimId, 1, 0);
    if (ppRes)
    {
        Animation_Init(anim.mFrameTableOffset, anim.mMaxW, anim.mMaxH, ppRes);
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        mBaseGameObjectFlags.Clear(Options::eDrawable_Bit4);
    }

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Well_Half_4;
        field_EC_scale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Well_23;
        field_EC_scale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    field_E8_switch_id = pTlv->field_1A_switch_id;
    field_F0_exit_x = FP_FromInteger(pTlv->field_20_exit_x) / FP_FromInteger(100);
    field_F4_exit_y = FP_FromInteger(pTlv->field_22_exit_y) / FP_FromInteger(100);

    field_100_emit_leaves = pTlv->field_2C_bEmit_leaves;

    if (field_100_emit_leaves == Choice_short::eYes_1)
    {
        field_F8_leaf_xpos = FP_FromInteger(pTlv->field_2E_leaf_x);
        if (!FP_GetExponent(field_F8_leaf_xpos))
        {
            field_F8_leaf_xpos = FP_FromInteger(pTlv->mTopLeft.x + (PsxToPCX(pTlv->mBottomRight.x - pTlv->mTopLeft.x, 11) / 2));
        }

        field_FC_leaf_ypos = FP_FromInteger(pTlv->field_30_leaf_y);
        if (!FP_GetExponent(field_FC_leaf_ypos))
        {
            field_FC_leaf_ypos = ypos;
        }
    }
}

Well::Well(Path_WellBase* pTlv, FP xpos, FP ypos, s32 tlvInfo)
{
    field_E4_tlvInfo = tlvInfo;
    mBaseGameObjectTypeId = ReliveTypes::eWell;

    mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(128, 128, 128);

    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;

    if (pTlv->mTlvType32 == TlvTypes::WellLocal_11)
    {
        WellLocal_Init(static_cast<Path_WellLocal*>(pTlv), xpos, ypos);
    }
    else
    {
        WellExpress_Init(static_cast<Path_WellExpress*>(pTlv), xpos, ypos);
    }
}

} // namespace AO
