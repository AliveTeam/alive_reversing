#include "stdafx_ao.h"
#include "Function.hpp"
#include "BellHammer.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Elum.hpp"
#include "Abe.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "Midi.hpp"
#include "Grid.hpp"

namespace AO {

void BellHammer::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BellHammer_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BellHammer_Smashing));
}


BellHammer::BellHammer(relive::Path_BellHammer* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eBellHammer);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::BellHammer_Idle));

    mAnim.mFlags.Clear(AnimFlags::eSemiTrans);
    mSpawnElum = false;
    mState = BellHammerStates::eWaitForActivation_0;

    mXPos = FP_FromInteger(pTlv->mTopLeftX + 82);
    mYPos = FP_FromInteger(pTlv->mTopLeftY + 94);

    mSwitchId = pTlv->mSwitchId;
    mTlvInfo = tlvId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
    }

    if (pTlv->mDirection == relive::reliveXDirection::eRight)
    {
        mAnim.mFlags.Set(AnimFlags::eFlipX);
    }

    if (gElum)
    {
        return;
    }
}

BellHammer::~BellHammer()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

void BellHammer::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void BellHammer::VUpdate()
{
    switch (mState)
    {
        case BellHammerStates::eWaitForActivation_0:
            if (SwitchStates_Get(mSwitchId))
            {
                mState = BellHammerStates::eSmashingBell_1;
                mAnim.Set_Animation_Data(GetAnimRes(AnimId::BellHammer_Smashing));
            }
            break;

        case BellHammerStates::eSmashingBell_1:
            if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                mState = BellHammerStates::eWaitForActivation_0;
                mAnim.Set_Animation_Data(GetAnimRes(AnimId::BellHammer_Idle));
                SwitchStates_Set(mSwitchId, 0);

                // Spawn the foo if he ain't already here
                if (gElum == nullptr)
                {
                    mSpawnElum = true;
                }
            }
            else
            {
                // Play those bell smashing sounds
                if (mAnim.mCurrentFrame == 5)
                {
                    SfxPlayMono(relive::SoundEffects::RingBellHammer, 0);
                }
                else if (mAnim.mCurrentFrame == 15)
                {
                    SND_SEQ_PlaySeq_4775A0(SeqId::eRingBellHammerAndExtraSound_13, 1, 1);
                }
            }
            break;

        default:
            break;
    }

    if (mSpawnElum)
    {
        mSpawnElum = false;
        Elum::Spawn(mTlvInfo);

        PSX_Point mapCoords = {};
        gMap.GetCurrentCamCoords(&mapCoords);

        if (gElum)
        {
            gElum->mXPos = (FP_FromInteger(mapCoords.x + XGrid_Index_To_XPos(mSpriteScale, 0))) - ScaleToGridSize(mSpriteScale);
            gElum->mYPos = gElum->mYPos + FP_FromInteger(450);
        }
    }
}

} // namespace AO
