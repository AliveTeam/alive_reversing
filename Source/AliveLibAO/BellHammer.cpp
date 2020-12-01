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

BellHammer* BellHammer::ctor_405010(Path_BellHammer* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BA120);

    field_4_typeId = Types::eBellHammer_27;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kHammerResID, 1, 0);
    Animation_Init_417FD0(4488, 71, 69, ppRes, 1);

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
    field_F0_bSpawnElum = 0;
    field_E4_state = 0;

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x + 82);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y + 94);
    
    field_E6_switch_id = pTlv->field_18_id;
    field_E8_tlvInfo = tlvInfo;

    if (pTlv->field_1C_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
        field_10_anim.field_C_layer = 6;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
        field_10_anim.field_C_layer = 25;
    }

    if (pTlv->field_1E_direction)
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    field_EC_pending_resource_count = 0;

    if (gElum_507680)
    {
        return this;
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kElmfallResID_216, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile("ELMFALL.BAN", BellHammer::OnResLoaded_405210, this);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kElmbasicResID_200, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile("ELMBASIC.BAN", BellHammer::OnResLoaded_405210, this);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kElmprmntResID__222, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile("ELMPRMNT.BAN", BellHammer::OnResLoaded_405210, this);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAneprmntResID, 0, 0))
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

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kElmfallResID_216, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kElmbasicResID_200, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kElmprmntResID__222, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAneprmntResID, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

    gMap_507BA8.TLV_Reset_446870(field_E8_tlvInfo, -1, 0, 0);
    return dtor_417D10();
}

BaseGameObject* BellHammer::VDestructor(signed int flags)
{
    return Vdtor_4054C0(flags);
}

void BellHammer::VScreenChanged_4054B0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

BellHammer* BellHammer::Vdtor_4054C0(signed int flags)
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
    case 0:
        if (SwitchStates_Get(field_E6_switch_id))
        {
            field_E4_state = 1;
            field_10_anim.Set_Animation_Data_402A40(4500, nullptr);
        }
        break;

    case 1:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_E4_state = 0;
            field_10_anim.Set_Animation_Data_402A40(4488, nullptr);
            SwitchStates_Set(field_E6_switch_id, 0);

            // Spawn the foo if he ain't already here
            if (gElum_507680 == nullptr)
            {
                field_F0_bSpawnElum = 1;
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
                SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_13, 1, 1);
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
            field_F0_bSpawnElum = 0;
            TlvItemInfoUnion info;
            info.all = field_E8_tlvInfo;
            Elum::Spawn_410E90(info);

            PSX_Point mapCoords = {};
            gMap_507BA8.GetCurrentCamCoords_444890(&mapCoords);

            gElum_507680->field_A8_xpos = (FP_FromInteger(mapCoords.field_0_x +  XGrid_Index_To_XPos_41FA60(field_BC_sprite_scale, 0))) - ScaleToGridSize_41FA30(field_BC_sprite_scale);
            gElum_507680->field_AC_ypos = gElum_507680->field_AC_ypos + FP_FromInteger(450);
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAneprmntResID, 1, 0);
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

}
