#include "stdafx_ao.h"
#include "Function.hpp"
#include "Abe.hpp"
#include "Map.hpp"
#include "ThrowableArray.hpp"
#include "Elum.hpp"
#include "ResourceManager.hpp"

START_NS_AO;

ALIVE_VAR(1, 0x5C1B68, Abe*, sActiveHero_5C1B68, nullptr);
ALIVE_VAR(1, 0x50767C, BaseAliveGameObject*, sControlledCharacter_50767C, nullptr);

BaseGameObject* Abe::VDestructor(signed int flags)
{
    return vdtor_422A70(flags);
}

void Abe::VUpdate()
{
    vUpdate_41FDB0();
}

void Abe::VRender(int** pOrderingTable)
{
    vRender_420F30(pOrderingTable);
}

void Abe::VScreenChanged()
{
    vScreenChanged_422640();
}

Abe* Abe::ctor_420770(int /*frameTableOffset*/, int /*a3*/, int /*a4*/, int /*a5*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

BaseGameObject* Abe::vdtor_422A70(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void Abe::vUpdate_41FDB0()
{
    NOT_IMPLEMENTED();
}

void Abe::vRender_420F30(int** pOrderingTable)
{
    // When in death shrivel don't reset scale else can't shrivel into a black blob
    if (!(field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel)))
    {
        field_10_anim.field_14_scale = field_BC_sprite_scale;
    }

    if (field_FC_state != 15 && field_FC_state != 81 && field_FC_state != 75)
    {
        VRender_417DA0(pOrderingTable);
    }
}

void Abe::sub_42F4C0()
{
    NOT_IMPLEMENTED();
}

const TintEntry sTintTable_Abe_4C6438[5] =
{
    { 5, 25u, 25u, 25u },
    { 6, 25u, 25u, 25u },
    { 8, 125u, 125u, 95u },
    { 9, 120u, 120u, 90u },
    { -1, 102u, 102u, 102u }
};

void Abe::vScreenChanged_422640()
{
    if (sControlledCharacter_50767C == this
        || sControlledCharacter_50767C == gElum_507680)
    {
        field_B2_lvl_number = gMap_507BA8.field_A_level;
        field_B0_path_number = gMap_507BA8.field_C_path;
    }

    // Level has changed?
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level)
    {
        SetTint_418750(sTintTable_Abe_4C6438, static_cast<short>(gMap_507BA8.field_A_level));
        if (gMap_507BA8.field_0_current_level != LevelIds::eMenu_0)
        {
            if (field_19C && gpThrowableArray_50E26C)
            {
                gpThrowableArray_50E26C->sub_4540D0(field_19C);
            }
            field_19C = 0;

            ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::ResourceType::Resource_Palt, 350, 0, 0));

            if (field_168 && field_16C)
            {
                sub_42F4C0();
            }
            field_168 = 0;
        }
        if (gMap_507BA8.field_A_level == LevelIds::eCredits_10 || gMap_507BA8.field_A_level == LevelIds::eMenu_0)
        {
            // Remove Abe for menu/credits levels?
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

END_NS_AO;
