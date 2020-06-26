#include "stdafx_ao.h"
#include "Function.hpp"
#include "Elum.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"

START_NS_AO;

ALIVE_VAR(1, 0x507680, Elum*, gElum_507680, nullptr);

BaseGameObject* Elum::VDestructor(signed int flags)
{
    return Vdtor_411710(flags);
}

Elum* Elum::Vdtor_411710(signed int flags)
{
    dtor_410BC0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

EXPORT BaseGameObject *Elum::dtor_410BC0()
{
    SetVTable(this, 0x4BA8F8);

    for (BYTE**& ppRes : field_174_resources.res)
    {
        if (ppRes && field_10_anim.field_20_ppBlock != ppRes)
        {
            ResourceManager::FreeResource_455550(ppRes);
            ppRes = nullptr;
        }
    }

    gElum_507680 = 0;
    Vsub_412700();

    ResourceManager::FreeResource_455550(
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 115, 0, 0)
    );

    if (field_104_pending_resource_count)
    {
        ResourceManager::CancelPendingResourcesFor_41EA60(this);
    }
    field_104_pending_resource_count = 0;

    const int anims[] = { 230, 222, 220, 221 };
    for (int anim : anims)
    {
        ResourceManager::FreeResource_455550(
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, anim, 0, 0)
        );
    }
    return dtor_base_416FE0();
}

void Elum::Vsub_412700()
{
    NOT_IMPLEMENTED();
    //LiftPoint *v2; // ecx
    //LiftPoint *v3; // eax

    //v2 = field_F8_pLiftPoint;
    //if (v2)
    //{
    //    ((void(__stdcall *)(Elum *))v2->field_0_VTable->Grenade.Grenade__Vsub_453EC0)(this);
    //    v3 = field_F8_pLiftPoint;
    //    field_F8_pLiftPoint = nullptr;
    //    --v3->field_C_bCanKill;
    //}
}

BaseAliveGameObject* Elum::dtor_base_416FE0()
{
    SetVTable(this, 0x4BA970);
    return dtor_401000();
}

void Elum::VUpdate()
{
    VUpdate_4102A0();
}

void Elum::VRender(int** pOrderingTable)
{
    VRender_410E40(pOrderingTable);
}

void Elum::VScreenChanged()
{
    vScreenChange_411340();
}

void Elum::VUpdate_4102A0()
{
    NOT_IMPLEMENTED();
}

void Elum::VRender_410E40(int** ot)
{
    if (field_B2_lvl_number == gMap_507BA8.field_0_current_level
        && field_B0_path_number == gMap_507BA8.field_2_current_path
        && !field_8_update_delay)
    {
        VRender_417DA0(ot);
    }
}

void Elum::vScreenChange_411340()
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x4C52F0, short, word_4C52F0, 0x88);
ALIVE_VAR(1, 0x4C52F2, short, word_4C52F2, 0x38);
ALIVE_VAR(1, 0x4C52F4, DWORD, dword_4C52F4, 0x1C92C);
ALIVE_VAR(1, 0x4C52F8, DWORD, dword_4C52F8, 0xC8);
ALIVE_VAR(1, 0x4C52FA, short, word_4C52FA, 0);

void Elum::Spawn_410E90(TlvItemInfoUnion tlvInfo)
{
    anythingForTheTimeBeing struct1;
    anythingForTheTimeBeing struct2;

    auto pElum = ao_new<Elum>();
    struct1.field_0 = word_4C52F2;
    struct1.field_2 = tlvInfo.parts.tlvOffset; //todo check it (if it's ever used)

    struct2.field_0 = word_4C52F0;
    struct2.field_2 = word_4C52FA;

    pElum->ctor_410870(dword_4C52F4, struct2, struct1, dword_4C52F8, tlvInfo);
}

Elum* Elum::ctor_410870(int, anythingForTheTimeBeing, anythingForTheTimeBeing, int, TlvItemInfoUnion /*tlvInfo*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

END_NS_AO;
