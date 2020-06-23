#include "stdafx_ao.h"
#include "Function.hpp"
#include "BellHammer.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"

START_NS_AO

BellHammer* BellHammer::ctor_405010(Path_BellHammer* pTlv, int tlvInfo)
{
    NOT_IMPLEMENTED();

    ctor_417C10();
    SetVTable(this, 0x4BA120);

    field_4_typeId = Types::eBellHammer_27;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1019, 1, 0);
    Animation_Init_417FD0(4488, 71, 69, ppRes, 1);

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
    field_F0 = 0;
    field_E4 = 0;

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x + 82);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y + 94);
    
    field_E6 = pTlv->field_18_id;
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
    /*
    if (gElum_507680)
    {
        return this;
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 216, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile_4551E0("ELMFALL.BAN", BellHammer::OnResLoaded_405210, this, nullptr);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 200, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile_4551E0("ELMBASIC.BAN", BellHammer::OnResLoaded_405210, this, nullptr);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 222, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile_4551E0("ELMPRMNT.BAN", BellHammer::OnResLoaded_405210, this, nullptr);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 115, 0, 0))
    {
        field_EC_pending_resource_count++;
        ResourceManager::LoadResourceFile_4551E0("ANEPRMNT.BAN", BellHammer::OnResLoaded_405210, this, nullptr);
    }
    */

    return this;
}

BaseGameObject* BellHammer::dtor_405220()
{
    SetVTable(this, 0x4BA120);

    if (field_EC_pending_resource_count)
    {
        ResourceManager::CancelPendingResourcesFor_41EA60(this);
    }

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 216, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 200, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 222, 1, 0);
    if (ppRes)
    {
        ResourceManager::FreeResource_455550(ppRes);
    }

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 115, 1, 0);
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

void BellHammer::VScreenChanged()
{
    VScreenChanged_4054B0();
}

void CC BellHammer::OnResLoaded_405210(BellHammer* pThis)
{
    pThis->field_EC_pending_resource_count--;
}

END_NS_AO
