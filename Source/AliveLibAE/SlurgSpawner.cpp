#include "stdafx.h"
#include "SlurgSpawner.hpp"
#include "DDCheat.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"

SlurgSpawner* SlurgSpawner::ctor_4C82E0(Path_SlurgSpawner* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x547704);

    field_4_typeId = Types::eSlurgSpawner_128;
    field_20_tlvInfo = tlvInfo;
    field_24_slurg_tlv = pTlv;
    field_28 = pTlv->field_18;
    field_3E_delay_counter = 0;
    field_40_spawned_count = pTlv->field_1_unknown;
    if ((field_40_spawned_count & 0x80u) != 0)
    {
        field_40_spawned_count = 0;
    }
    return this;
}

BaseGameObject* SlurgSpawner::VDestructor(signed int flags)
{
    return vdtor_4C8370(flags);
}

void SlurgSpawner::VUpdate()
{
    vUpdate_4C83C0();
}

void SlurgSpawner::VScreenChanged()
{
    vScreenChanged_4C84A0();
}

void SlurgSpawner::vUpdate_4C83C0()
{
    if (sDDCheat_ShowAI_Info_5C1BD8)
    {
        DDCheat::DebugStr_4F5560("SPAWNER EXISTS/n");
    }

    const short oldDelay = field_3E_delay_counter;
    field_3E_delay_counter++;

    if (field_3E_delay_counter < field_28.field_8_delay_between_slurgs && field_40_spawned_count < field_28.field_A_max_slurgs)
    {
        if (SwitchStates_Get_466020(field_28.field_C_switch_id))
        {
            field_3E_delay_counter = 0;
            field_40_spawned_count++;
            auto pSlurg = ae_new<Slurg>();
            if (pSlurg)
            {
                pSlurg->ctor_4C84E0(field_24_slurg_tlv, 0xFFFFFFFF);
            }
        }
    }
}

void SlurgSpawner::dtor_4C83A0()
{
    BaseGameObject_dtor_4DBEC0();
}

SlurgSpawner* SlurgSpawner::vdtor_4C8370(signed int flags)
{
    dtor_4C83A0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void SlurgSpawner::vScreenChanged_4C84A0()
{
    Path::TLV_Reset_4DB8E0(field_20_tlvInfo, field_40_spawned_count, 0, 0);
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}
