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
    field_4_typeId = Types::eBellHammer_27;

    const AnimRecord& rec = AO::AnimRec(AnimId::BellHammer_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
    field_F0_bSpawnElum = FALSE;
    field_E4_state = BellHammerStates::eWaitForActivation_0;

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 82);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y + 94);

    field_E6_switch_id = pTlv->field_18_switch_id;
    field_E8_tlvInfo = tlvInfo;

    if (pTlv->field_1C_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
        field_10_anim.field_C_layer = Layer::eLayer_BeforeShadow_Half_6;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
        field_10_anim.field_C_layer = Layer::eLayer_BeforeShadow_25;
    }

    if (pTlv->field_1E_direction == XDirection_short::eRight_1)
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    field_EC_pending_resource_count = 0;

    if (gElum_507680)
    {
        return;
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmfallAOResID_216, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile("ELMFALL.BAN", BellHammer::OnResLoaded, this);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmbasicAOResID_200, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile("ELMBASIC.BAN", BellHammer::OnResLoaded, this);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmprmntAOResID__222, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile("ELMPRMNT.BAN", BellHammer::OnResLoaded, this);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAneprmntAOResID, 0, 0))
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

    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmfallAOResID_216, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmbasicAOResID_200, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmprmntAOResID__222, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAneprmntAOResID, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

    gMap.TLV_Reset(field_E8_tlvInfo, -1, 0, 0);
}

void BellHammer::VScreenChanged()
{
    mFlags.Set(BaseGameObject::eDead);
}

void BellHammer::VUpdate()
{
    switch (field_E4_state)
    {
        case BellHammerStates::eWaitForActivation_0:
            if (SwitchStates_Get(field_E6_switch_id))
            {
                field_E4_state = BellHammerStates::eSmashingBell_1;
                const AnimRecord& rec = AO::AnimRec(AnimId::BellHammer_Smashing);
                field_10_anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
            }
            break;

        case BellHammerStates::eSmashingBell_1:
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_E4_state = BellHammerStates::eWaitForActivation_0;
                const AnimRecord& rec = AO::AnimRec(AnimId::BellHammer_Idle);
                field_10_anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
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
                if (field_10_anim.field_92_current_frame == 5)
                {
                    SFX_Play_Mono(SoundEffect::RingBellHammer_9, 0, 0);
                }
                else if (field_10_anim.field_92_current_frame == 15)
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

            gElum_507680->field_A8_xpos = (FP_FromInteger(mapCoords.field_0_x + XGrid_Index_To_XPos(field_BC_sprite_scale, 0))) - ScaleToGridSize(field_BC_sprite_scale);
            gElum_507680->field_AC_ypos = gElum_507680->field_AC_ypos + FP_FromInteger(450);
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAneprmntAOResID, 1, 0);
        }
    }
}

void BellHammer::OnResLoaded(BellHammer* pThis)
{
    pThis->field_EC_pending_resource_count--;
}

} // namespace AO
