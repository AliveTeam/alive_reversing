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

BellHammer* BellHammer::ctor_405010(Path_BellHammer* pTlv, s32 tlvInfo)
{
    
    SetVTable(this, 0x4BA120);

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
        return this;
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmfallAOResID_216, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile("ELMFALL.BAN", BellHammer::OnResLoaded_405210, this);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmbasicAOResID_200, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile("ELMBASIC.BAN", BellHammer::OnResLoaded_405210, this);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmprmntAOResID__222, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile("ELMPRMNT.BAN", BellHammer::OnResLoaded_405210, this);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAneprmntAOResID, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile("ANEPRMNT.BAN", BellHammer::OnResLoaded_405210, this);
    }

    return this;
}

BaseGameObject* BellHammer::dtor_405220()
{
    SetVTable(this, 0x4BA120);

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

    gMap.TLV_Reset_446870(field_E8_tlvInfo, -1, 0, 0);
    return dtor_417D10();
}

BaseGameObject* BellHammer::VDestructor(s32 flags)
{
    return Vdtor_4054C0(flags);
}

void BellHammer::VScreenChanged_4054B0()
{
    mFlags.Set(BaseGameObject::eDead);
}

BellHammer* BellHammer::Vdtor_4054C0(s32 flags)
{
    dtor_405220();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void BellHammer::VUpdate()
{
    VUpdate_405320();
}

void BellHammer::VUpdate_405320()
{
    switch (field_E4_state)
    {
        case BellHammerStates::eWaitForActivation_0:
            if (SwitchStates_Get(field_E6_switch_id))
            {
                field_E4_state = BellHammerStates::eSmashingBell_1;
                const AnimRecord& rec = AO::AnimRec(AnimId::BellHammer_Smashing);
                field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, nullptr);
            }
            break;

        case BellHammerStates::eSmashingBell_1:
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_E4_state = BellHammerStates::eWaitForActivation_0;
                const AnimRecord& rec = AO::AnimRec(AnimId::BellHammer_Idle);
                field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, nullptr);
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
                    SFX_Play_43AD70(SoundEffect::RingBellHammer_9, 0, 0);
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
            Elum::Spawn_410E90(info);

            PSX_Point mapCoords = {};
            gMap.GetCurrentCamCoords_444890(&mapCoords);

            gElum_507680->field_A8_xpos = (FP_FromInteger(mapCoords.field_0_x + XGrid_Index_To_XPos_41FA60(field_BC_sprite_scale, 0))) - ScaleToGridSize_41FA30(field_BC_sprite_scale);
            gElum_507680->field_AC_ypos = gElum_507680->field_AC_ypos + FP_FromInteger(450);
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAneprmntAOResID, 1, 0);
        }
    }
}

void BellHammer::VScreenChanged()
{
    VScreenChanged_4054B0();
}

void CC BellHammer::OnResLoaded_405210(BellHammer* pThis)
{
    pThis->field_EC_pending_resource_count--;
}

} // namespace AO
