#include "stdafx_ao.h"
#include "Function.hpp"
#include "BellHammer.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Elum.hpp"
#include "Abe.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "Midi.hpp"

namespace AO {

BellHammer::BellHammer(Path_BellHammer* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = ReliveTypes::eBellHammer;

    const AnimRecord rec = AO::AnimRec(AnimId::BellHammer_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::BellHammer_Idle, ppRes);

    mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
    field_F0_bSpawnElum = FALSE;
    field_E4_state = BellHammerStates::eWaitForActivation_0;

    mXPos = FP_FromInteger(pTlv->mTopLeft.x + 82);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y + 94);

    field_E6_switch_id = pTlv->field_18_switch_id;
    field_E8_tlvInfo = tlvInfo;

    if (pTlv->field_1C_scale == Scale_short::eHalf_1)
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

    if (pTlv->field_1E_direction == XDirection_short::eRight_1)
    {
        mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
    }

    field_EC_pending_resource_count = 0;

    if (gElum_507680)
    {
        return;
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmfallAOResID_216, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile("ELMFALL.BAN", BellHammer::OnResLoaded, this);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmbasicAOResID_200, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile("ELMBASIC.BAN", BellHammer::OnResLoaded, this);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmprmntAOResID__222, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile("ELMPRMNT.BAN", BellHammer::OnResLoaded, this);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAneprmntAOResID, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile("ANEPRMNT.BAN", BellHammer::OnResLoaded, this);
    }
}

BellHammer::~BellHammer()
{
    if (field_EC_pending_resource_count)
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

    gMap.TLV_Reset(field_E8_tlvInfo, -1, 0, 0);
}

void BellHammer::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void BellHammer::VUpdate()
{
    switch (field_E4_state)
    {
        case BellHammerStates::eWaitForActivation_0:
            if (SwitchStates_Get(field_E6_switch_id))
            {
                field_E4_state = BellHammerStates::eSmashingBell_1;
                mAnim.Set_Animation_Data(AnimId::BellHammer_Smashing, nullptr);
            }
            break;

        case BellHammerStates::eSmashingBell_1:
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_E4_state = BellHammerStates::eWaitForActivation_0;
                mAnim.Set_Animation_Data(AnimId::BellHammer_Idle, nullptr);
                SwitchStates_Set(field_E6_switch_id, 0);

                // Spawn the foo if he ain't already here
                if (gElum_507680 == nullptr)
                {
                    field_F0_bSpawnElum = TRUE;
                }
            }
            else
            {
                // Play those bell smashing sounds
                if (mAnim.mCurrentFrame == 5)
                {
                    SfxPlayMono(SoundEffect::RingBellHammer_9, 0, 0);
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

    if (field_F0_bSpawnElum)
    {
        if (field_EC_pending_resource_count == 0)
        {
            field_F0_bSpawnElum = FALSE;
            TlvItemInfoUnion info;
            info.all = field_E8_tlvInfo;
            Elum::Spawn(info);

            PSX_Point mapCoords = {};
            gMap.GetCurrentCamCoords(&mapCoords);

            gElum_507680->mXPos = (FP_FromInteger(mapCoords.x + XGrid_Index_To_XPos(mSpriteScale, 0))) - ScaleToGridSize(mSpriteScale);
            gElum_507680->mYPos = gElum_507680->mYPos + FP_FromInteger(450);
            ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAneprmntAOResID, 1, 0);
        }
    }
}

void BellHammer::OnResLoaded(BellHammer* pThis)
{
    pThis->field_EC_pending_resource_count--;
}

} // namespace AO
