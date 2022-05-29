#include "stdafx.h"
#include "SlurgSpawner.hpp"
#include "DDCheat.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"

SlurgSpawner::SlurgSpawner(Path_SlurgSpawner* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    SetType(AETypes::eSlurgSpawner_128);
    field_20_tlvInfo = tlvInfo;
    field_24_slurg_tlv = pTlv;
    field_28_path_data = pTlv->field_18_spawner_data;
    field_3E_delay_counter = 0;
    field_40_spawned_count = pTlv->field_1_tlv_state;
    if ((field_40_spawned_count & 0x80u) != 0)
    {
        field_40_spawned_count = 0;
    }
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

    field_3E_delay_counter++;

    if (field_3E_delay_counter >= field_28_path_data.field_8_spawn_delay_between_slurgs &&
        field_40_spawned_count < field_28_path_data.field_A_max_slurgs)
    {
        if (SwitchStates_Get_466020(field_28_path_data.field_C_switch_id))
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

void SlurgSpawner::vScreenChanged_4C84A0()
{
    Path::TLV_Reset_4DB8E0(field_20_tlvInfo, field_40_spawned_count, 0, 0);
    mFlags.Set(BaseGameObject::eDead);
}
