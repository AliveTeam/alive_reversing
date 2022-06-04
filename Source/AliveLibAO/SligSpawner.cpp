#include "stdafx_ao.h"
#include "Function.hpp"
#include "SligSpawner.hpp"
#include "Slig.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "stdlib.hpp"

namespace AO {

SligSpawner::SligSpawner(Path_Slig* pTlv, s32 tlvInfo)
    : BaseGameObject(1)
{
    mBaseGameObjectTypeId = ReliveTypes::eSligSpawner;

    field_10_tlvInfo = tlvInfo;
    field_18_tlv = *pTlv;

    field_16_flags = 1;

    field_14_slig_spawner_switch_id = pTlv->field_54_slig_spawner_switch_id;
}

void SligSpawner::VUpdate()
{
    VUpdate_4028A0();
}

void SligSpawner::VScreenChanged()
{
    VScreenChanged_402960();
}

void SligSpawner::VScreenChanged_402960()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void SligSpawner::VUpdate_4028A0()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (SwitchStates_Get(field_14_slig_spawner_switch_id))
    {
        auto pTlv = static_cast<Path_Slig*>(gMap.TLV_Get_At_446260(
            field_18_tlv.field_10_top_left.field_0_x,
            field_18_tlv.field_10_top_left.field_2_y,
            field_18_tlv.field_10_top_left.field_0_x,
            field_18_tlv.field_10_top_left.field_2_y,
            TlvTypes::SligSpawner_66));

        if (pTlv)
        {
            ao_new<Slig>(pTlv, 0xFFFF);
        }

        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_16_flags = 0;
    }
}

SligSpawner::~SligSpawner()
{
    if (field_16_flags)
    {
        gMap.TLV_Reset(field_10_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap.TLV_Reset(field_10_tlvInfo, -1, 0, 1);
    }
}

} // namespace AO
