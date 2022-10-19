#include "stdafx_ao.h"
#include "Function.hpp"
#include "BeeNest.hpp"
#include "ResourceManager.hpp"
#include "BeeSwarm.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "Abe.hpp"

namespace AO {

BeeNest* BeeNest::ctor_480E20(Path_BeeNest* pTlv, s32 tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BCEE8);
    field_4_typeId = Types::eBeeNest_96;

    field_1C_tlvInfo = tlvInfo;

    field_20_top_left = pTlv->field_10_top_left;
    field_18_level = gMap_507BA8.field_0_current_level;
    field_24_bottom_right = pTlv->field_14_bottom_right;

    field_2A_swarm_size = pTlv->field_1A_swarm_size;
    field_1A_path = gMap_507BA8.field_2_current_path;

    field_28_switch_id = pTlv->field_18_id;

    field_10_bee_x = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_14_bee_y = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_2C_chase_ticks = pTlv->field_1C_chase_time;

    field_30_speed = FP_FromRaw(pTlv->field_1E_speed << 8);

    field_34_pBeeSwarm = nullptr;

    field_2E_state = BeeNestStates::eWaitForTrigger_0;

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kWaspAOResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_455270("WASP.BAN", nullptr);
    }

    // The "idle" swarm that hovers around the nest
    auto pBeeSwarm = ao_new<BeeSwarm>();
    if (pBeeSwarm)
    {
        pBeeSwarm->ctor_47FC60(field_10_bee_x, field_14_bee_y, FP_FromInteger(0), pTlv->field_22_num_bees, 0);
    }

    return this;
}

BaseGameObject* BeeNest::dtor_4810C0()
{
    SetVTable(this, 0x4BCEE8);
    if (field_34_pBeeSwarm)
    {
        field_34_pBeeSwarm->field_C_refCount--;
    }
    return dtor_487DF0();
}

BaseGameObject* BeeNest::VDestructor(s32 flags)
{
    return Vdtor_4810E0(flags);
}

BeeNest* BeeNest::Vdtor_4810E0(s32 flags)
{
    dtor_4810C0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void BeeNest::VScreenChanged()
{
    VScreenChanged_481040();
}

void BeeNest::VScreenChanged_481040()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path || !field_34_pBeeSwarm)
    {
        gMap_507BA8.TLV_Reset_446870(field_1C_tlvInfo, -1, 0, 0);
        if (field_34_pBeeSwarm)
        {
            field_34_pBeeSwarm->field_C_refCount--;
            field_34_pBeeSwarm = nullptr;
        }
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void BeeNest::VUpdate()
{
    VUpdate_480F30();
}

void BeeNest::VUpdate_480F30()
{
    switch (field_2E_state)
    {
        case BeeNestStates::eWaitForTrigger_0:
            if (SwitchStates_Get(field_28_switch_id))
            {
                field_34_pBeeSwarm = ao_new<BeeSwarm>();
                if (field_34_pBeeSwarm)
                {
                    field_34_pBeeSwarm->ctor_47FC60(
                        field_10_bee_x,
                        field_14_bee_y,
                        field_30_speed,
                        field_2A_swarm_size,
                        field_2C_chase_ticks);

                    field_34_pBeeSwarm->field_C_refCount++;
                    field_34_pBeeSwarm->Chase_47FEB0(sActiveHero_507678);
                    field_2E_state = BeeNestStates::eResetIfDead_1;
                }
            }
            break;

        case BeeNestStates::eResetIfDead_1:
            if (field_34_pBeeSwarm->field_6_flags.Get(BaseGameObject::eDead_Bit3))
            {
                field_2E_state = BeeNestStates::eWaitForTrigger_0;
                field_34_pBeeSwarm->field_C_refCount--;
                field_34_pBeeSwarm = nullptr;
                SwitchStates_Set(field_28_switch_id, 0);
            }
            break;

        default:
            break;
    }
}

} // namespace AO
