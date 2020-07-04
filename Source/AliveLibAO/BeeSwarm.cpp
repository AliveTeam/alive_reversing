#include "stdafx_ao.h"
#include "Function.hpp"
#include "BeeSwarm.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Game.hpp"

START_NS_AO

ALIVE_VAR(1, 0x5076B0, short, gBeeInstanceCount_5076B0, 0);
ALIVE_VAR(1, 0x5076AC, short, word_5076AC, 0);


BeeSwarm* BeeSwarm::ctor_47FC60(FP xpos, FP ypos, FP speed, signed __int16 numBees, int chaseTicks)
{
    ctor_417C10();

    for (auto& bee : field_E4_bees.bees)
    {
        SetVTable(&bee.field_10_anim, 0x4BA470);
    }

    SetVTable(this, 0x4BCEB0);
    field_4_typeId = Types::eBeeSwarm_95;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 61, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 16, 1, 0);
    if (gMap_507BA8.field_0_current_level != LevelIds::eForestTemple_4 && gMap_507BA8.field_0_current_level != LevelIds::eDesertTemple_9)
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 204, 1, 0);
    }

    short numBeesToUse = 0;
    gBeeInstanceCount_5076B0++;
    if (gBeeInstanceCount_5076B0 < 3)
    {
        numBeesToUse = numBees;
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        numBeesToUse = 1;
    }

    Animation_Init_417FD0(636, 7, 4, ppRes, 1);
    if (numBeesToUse <= 25)
    {
        field_D64_num_bees = numBeesToUse;
    }
    else
    {
        field_D64_num_bees = 25;
    }

    field_D78_speed = speed;

    field_D70_chase_target_x = xpos;
    field_D74_chase_target_y = ypos;

    field_D84_chaseTicks = chaseTicks;
    field_D68_xpos = xpos;
    field_A8_xpos = xpos;
    field_D6C_ypos = ypos;
    field_D80_state = 0;
    field_D98_pChaseTarget = 0;
    field_DA4 = 0;
    field_D9C_chase_timer = 0;
    field_DA0_timer = 0;
    field_D88 = 0;
    field_D90_fp = FP_FromInteger(1);
    field_D8C = 0;
    field_D94_fp = FP_FromInteger(1);
    field_D66_bee_count = 0;
    field_D7C = 0;
    field_AC_ypos = ypos;
    return this;
}

BaseGameObject* BeeSwarm::dtor_47FDF0()
{
    SetVTable(this, 0x4BCEB0);

    gBeeInstanceCount_5076B0--;

    word_5076AC = 0;

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 16, 0, 0));

    if (gMap_507BA8.field_0_current_level != LevelIds::eForestTemple_4 && gMap_507BA8.field_0_current_level != LevelIds::eDesertTemple_9)
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 204, 0, 0));
    }

    if (field_D98_pChaseTarget)
    {
        field_D98_pChaseTarget->field_C_refCount--;
    }

    return dtor_417D10();
}

BaseGameObject* BeeSwarm::VDestructor(signed int flags)
{
    return Vdtor_480E00(flags);
}

BeeSwarm* BeeSwarm::Vdtor_480E00(signed int flags)
{
    dtor_47FDF0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void BeeSwarm::VScreenChanged()
{
    VScreenChange_480D40();
}

void BeeSwarm::VScreenChange_480D40()
{
    if (gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0())
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level
        || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_D98_pChaseTarget)
    {
        if (field_D98_pChaseTarget->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            field_D80_state = 3;
            field_D74_chase_target_y -= FP_FromInteger(240);
            field_D9C_chase_timer = gnFrameCount_507670 + 120;
            word_5076AC = 0;
            field_D98_pChaseTarget->field_C_refCount--;
            field_D98_pChaseTarget = nullptr;
        }
    }

    if (!sActiveHero_507678 || field_D98_pChaseTarget == sActiveHero_507678
        && sActiveHero_507678->field_FC_state == 156)
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }
}

void BeeSwarm::FollowLine_47FF10(PathLine* pLine, FP target_x, FP target_y, FP speed)
{
    field_DA8_pLine = pLine;
    field_DAC_line_follow_speed = speed;
    field_D80_state = 2;
    field_D70_chase_target_x = target_x;
    field_D74_chase_target_y = target_y;
    field_D98_pChaseTarget = nullptr;
}

void BeeSwarm::Chase_47FEB0(BaseAliveGameObject* pChaseTarget)
{
    if (field_D98_pChaseTarget)
    {
        field_D98_pChaseTarget->field_C_refCount--;
    }

    field_D80_state = 1;

    field_D98_pChaseTarget = pChaseTarget;
    field_DA4 = 0;

    pChaseTarget->field_C_refCount++;

    field_D70_chase_target_x = pChaseTarget->field_A8_xpos;
    field_D74_chase_target_y = pChaseTarget->field_AC_ypos;

    field_D9C_chase_timer = gnFrameCount_507670 + field_D84_chaseTicks;
}

END_NS_AO
