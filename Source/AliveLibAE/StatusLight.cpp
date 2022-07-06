#include "stdafx.h"
#include "StatusLight.hpp"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Collisions.hpp"
#include "SwitchStates.hpp"
#include "DDCheat.hpp"
#include "stdlib.hpp"
#include "Grid.hpp"

StatusLight::StatusLight(Path_StatusLight* pTlv, u32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    field_F4_tlvInfo = tlvInfo;
    field_F8_switch_id = pTlv->field_10_switch_id;

    if (pTlv->field_12_scale != Scale_short::eFull_0)
    {
        if (pTlv->field_12_scale == Scale_short::eHalf_1)
        {
            mSpriteScale = FP_FromDouble(0.5);
            mAnim.mRenderLayer = Layer::eLayer_8;
            mScale = Scale::Bg;
        }
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_27;
        mScale = Scale::Fg;
    }

    field_FA_id1 = pTlv->field_14_id1;
    field_FC_id2 = pTlv->field_16_id2;
    field_FE_id3 = pTlv->field_18_id3;
    field_100_id4 = pTlv->field_1A_id4;
    field_102_id5 = pTlv->field_1C_id5;
    field_104_bIgnore_grid_snapping = pTlv->field_1E_bIgnore_grid_snapping;

    const AnimRecord& rec = AnimRec(AnimId::Status_Light_Red);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::Status_Light_Red, ppRes);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mAnim.mFlags.Clear(AnimFlags::eBit3_Render);

    mXPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);

    if (field_104_bIgnore_grid_snapping == Choice_short::eNo_0)
    {
        mXPos = FP_FromInteger(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos)));
    }

    mYPos = FP_FromInteger((pTlv->mTopLeft.y + pTlv->mBottomRight.y) / 2);

    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = sCollisions->Raycast(
        mXPos,
        mYPos,
        mXPos,
        mYPos + (mSpriteScale * FP_FromInteger(56)),
        &pPathLine,
        &hitX,
        &hitY,
        mScale == Scale::Fg ? kFgFloor : kBgFloor); // TODO: mouze check, 0xF1 : 0x10 seemed like it should be 0x1 : 0x10

    if (field_104_bIgnore_grid_snapping == Choice_short::eNo_0)
    {
        if (bCollision)
        {
            mYPos = hitY - (mSpriteScale * FP_FromInteger(56));
        }
    }

    field_108_xpos = mXPos;
    field_10C_ypos = mYPos;
}

void StatusLight::VUpdate()
{
    // TODO: Document how this works

    mXPos = FP_FromInteger(sTweakX_5C1BD0) + field_108_xpos;
    mYPos = FP_FromInteger(sTweakY_5C1BD4) + field_10C_ypos;

    if (SwitchStates_Get(field_F8_switch_id))
    {
        if ((!SwitchStates_Get(field_FA_id1) && field_FA_id1) || (!SwitchStates_Get(field_FC_id2) && field_FC_id2) || (!SwitchStates_Get(field_FE_id3) && field_FE_id3) || (!SwitchStates_Get(field_100_id4) && field_100_id4) || (!SwitchStates_Get(field_102_id5) && field_102_id5)
            || (sGnFrame % 8) >= 4)
        {
            mAnim.mFlags.Set(AnimFlags::eBit3_Render);
        }
        else
        {
            mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
        }
        mAnim.Set_Animation_Data(AnimId::Status_Light_Green, nullptr);
    }
    else if (SwitchStates_Get(field_FA_id1) || SwitchStates_Get(field_FC_id2) || SwitchStates_Get(field_FE_id3) || SwitchStates_Get(field_100_id4) || SwitchStates_Get(field_102_id5))
    {
        if ((sGnFrame % 8) >= 4)
        {
            mAnim.mFlags.Set(AnimFlags::eBit3_Render);
        }
        else
        {
            mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
        }
        mAnim.Set_Animation_Data(AnimId::Status_Light_Red, nullptr);
    }
    else
    {
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
        mAnim.Set_Animation_Data(AnimId::Status_Light_Red, nullptr);
    }
}

StatusLight::~StatusLight()
{
    Path::TLV_Reset(field_F4_tlvInfo, -1, 0, 0);
}

void StatusLight::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}
