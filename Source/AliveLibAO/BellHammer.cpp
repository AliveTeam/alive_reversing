#include "stdafx_ao.h"
#include "Function.hpp"
#include "BellHammer.hpp"
#include "ResourceManager.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Elum.hpp"
#include "Abe.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "Midi.hpp"

namespace AO {

BellHammer::BellHammer(relive::Path_BellHammer* pTlv, const Guid& tlvId)
{
    mBaseGameObjectTypeId = ReliveTypes::eBellHammer;

    const AnimRecord rec = AO::AnimRec(AnimId::BellHammer_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::BellHammer_Idle, ppRes);

    mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
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
        mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mPendingResourceCount = 0;

    if (gElum)
    {
        return;
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmfallAOResID_216, 0, 0))
    {
        mPendingResourceCount++;
        ResourceManager::LoadResourceFile("ELMFALL.BAN", BellHammer::OnResLoaded, this);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmbasicAOResID_200, 0, 0))
    {
        mPendingResourceCount++;
        ResourceManager::LoadResourceFile("ELMBASIC.BAN", BellHammer::OnResLoaded, this);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmprmntAOResID__222, 0, 0))
    {
        mPendingResourceCount++;
        ResourceManager::LoadResourceFile("ELMPRMNT.BAN", BellHammer::OnResLoaded, this);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAneprmntAOResID, 0, 0))
    {
        mPendingResourceCount++;
        ResourceManager::LoadResourceFile("ANEPRMNT.BAN", BellHammer::OnResLoaded, this);
    }
}

BellHammer::~BellHammer()
{
    if (mPendingResourceCount)
    {
        ResourceManager::WaitForPendingResources_41EA60(this);
    }

    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmfallAOResID_216, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

    ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmbasicAOResID_200, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

    ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmprmntAOResID__222, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

    ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAneprmntAOResID, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

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
                mAnim.Set_Animation_Data(AnimId::BellHammer_Smashing, nullptr);
            }
            break;

        case BellHammerStates::eSmashingBell_1:
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mState = BellHammerStates::eWaitForActivation_0;
                mAnim.Set_Animation_Data(AnimId::BellHammer_Idle, nullptr);
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
                    SfxPlayMono(relive::SoundEffects::RingBellHammer, 0, 0);
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
        if (mPendingResourceCount == 0)
        {
            mSpawnElum = false;
            Elum::Spawn(mTlvInfo);

            PSX_Point mapCoords = {};
            gMap.GetCurrentCamCoords(&mapCoords);

            gElum->mXPos = (FP_FromInteger(mapCoords.x + XGrid_Index_To_XPos(mSpriteScale, 0))) - ScaleToGridSize(mSpriteScale);
            gElum->mYPos = gElum->mYPos + FP_FromInteger(450);
            ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAneprmntAOResID, 1, 0);
        }
    }
}

void BellHammer::OnResLoaded(BellHammer* pThis)
{
    pThis->mPendingResourceCount--;
}

} // namespace AO
