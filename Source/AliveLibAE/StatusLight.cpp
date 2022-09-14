#include "stdafx.h"
#include "StatusLight.hpp"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/Collisions.hpp"
#include "SwitchStates.hpp"
#include "DDCheat.hpp"
#include "stdlib.hpp"
#include "Grid.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"

void StatusLight::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Status_Light_Green));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Status_Light_Red));
}

StatusLight::StatusLight(relive::Path_StatusLight* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    LoadAnimations();

    field_F4_tlvInfo = tlvId;
    mInputSwitchId = pTlv->mInputSwitchId;

    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        if (pTlv->mScale == relive::reliveScale::eHalf)
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

    mLinkedStatusLightSwitchId1 = pTlv->mLinkedStatusLightSwitchId1;
    mLinkedStatusLightSwitchId2 = pTlv->mLinkedStatusLightSwitchId2;
    mLinkedStatusLightSwitchId3 = pTlv->mLinkedStatusLightSwitchId3;
    mLinkedStatusLightSwitchId4 = pTlv->mLinkedStatusLightSwitchId4;
    mLinkedStatusLightSwitchId5 = pTlv->mLinkedStatusLightSwitchId5;
    mIgnoreGridSnapping = pTlv->mIgnoreGridSnapping;

    Animation_Init(GetAnimRes(AnimId::Status_Light_Red));

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mAnim.mFlags.Clear(AnimFlags::eBit3_Render);

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);

    if (mIgnoreGridSnapping == relive::reliveChoice::eNo)
    {
        mXPos = FP_FromInteger(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos)));
    }

    mYPos = FP_FromInteger((pTlv->mTopLeftY + pTlv->mBottomRightY) / 2);

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

    if (mIgnoreGridSnapping == relive::reliveChoice::eNo)
    {
        if (bCollision)
        {
            mYPos = hitY - (mSpriteScale * FP_FromInteger(56));
        }
    }

    field_108_xpos = mXPos;
    field_10C_ypos = mYPos;
}

bool StatusLight::isLinkedAndDisabled(s16 switchId)
{
    return !SwitchStates_Get(switchId) && switchId;
}

void StatusLight::VUpdate()
{
    // TODO: Document how this works

    mXPos = FP_FromInteger(sTweakX_5C1BD0) + field_108_xpos;
    mYPos = FP_FromInteger(sTweakY_5C1BD4) + field_10C_ypos;

    if (SwitchStates_Get(mInputSwitchId))
    {
        if (isLinkedAndDisabled(mLinkedStatusLightSwitchId1) ||
            isLinkedAndDisabled(mLinkedStatusLightSwitchId2) || 
            isLinkedAndDisabled(mLinkedStatusLightSwitchId3) || 
            isLinkedAndDisabled(mLinkedStatusLightSwitchId4) || 
            isLinkedAndDisabled(mLinkedStatusLightSwitchId5) ||
            (sGnFrame % 8) >= 4)
        {
            mAnim.mFlags.Set(AnimFlags::eBit3_Render);
        }
        else
        {
            mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
        }
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::Status_Light_Green));
    }
    else if (SwitchStates_Get(mLinkedStatusLightSwitchId1) ||
        SwitchStates_Get(mLinkedStatusLightSwitchId2) ||
        SwitchStates_Get(mLinkedStatusLightSwitchId3) ||
        SwitchStates_Get(mLinkedStatusLightSwitchId4) ||
        SwitchStates_Get(mLinkedStatusLightSwitchId5))
    {
        if ((sGnFrame % 8) >= 4)
        {
            mAnim.mFlags.Set(AnimFlags::eBit3_Render);
        }
        else
        {
            mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
        }
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::Status_Light_Red));
    }
    else
    {
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::Status_Light_Red));
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
