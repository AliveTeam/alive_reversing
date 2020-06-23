#include "stdafx_ao.h"
#include "Function.hpp"
#include "SligSpawner.hpp"
#include "Slig.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "stdlib.hpp"

START_NS_AO

SligSpawner* SligSpawner::ctor_402850(Path_Slig* pTlv, int tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BA078);

    field_4_typeId = Types::eSligSpawner_91;

    field_10_tlvInfo = tlvInfo;
    field_18_tlv = *pTlv;

    field_16_flags = 1;

    field_14_slig_id = pTlv->field_54_id;
    return this;
}

void SligSpawner::VUpdate()
{
    VUpdate_4028A0();
}

void SligSpawner::VScreenChanged()
{
    VScreenChanged_402960();
}

BaseGameObject* SligSpawner::VDestructor(signed int flags)
{
    return VDtor_4029F0(flags);
}

EXPORT void SligSpawner::VScreenChanged_402960()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

EXPORT void SligSpawner::VUpdate_4028A0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (SwitchStates_Get(field_14_slig_id))
    {
        auto pTlv = static_cast<Path_Slig*>(gMap_507BA8.TLV_Get_At_446260(
            field_18_tlv.field_C_sound_pos.field_0_x,
            field_18_tlv.field_C_sound_pos.field_2_y,
            field_18_tlv.field_C_sound_pos.field_0_x,
            field_18_tlv.field_C_sound_pos.field_2_y,
            TlvTypes::Slig_66));

        if (pTlv)
        {
            auto pSlig = ao_new<Slig>();
            if (pSlig)
            {
                pSlig->ctor_464D40(pTlv, 0xFFFF);
            }
        }

        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_16_flags = 0;
    }
}

EXPORT BaseGameObject* SligSpawner::dtor_402970()
{
    SetVTable(this, 0x4BA078);
    if (field_16_flags)
    {
        gMap_507BA8.TLV_Reset_446870(field_10_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_10_tlvInfo, -1, 0, 1);
    }
    return dtor_487DF0();
}

EXPORT SligSpawner* SligSpawner::VDtor_4029F0(signed int flags)
{
    dtor_402970();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
