#include "stdafx_ao.h"
#include "Function.hpp"
#include "SlogSpawner.hpp"
#include "stdlib.hpp"

START_NS_AO

void SlogSpawner::VScreenChanged_475F90()
{
    gMap_507BA8.TLV_Reset_446870(field_10_tlvInfo, field_14_spawned_count, 0, 0);
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void SlogSpawner::VScreenChanged()
{
    VScreenChanged_475F90();
}

SlogSpawner* SlogSpawner::ctor_475DD0(Path_SlogSpawner* pTlv, int tlvInfo)
{
    ctor_487E10(1);

    field_10_tlvInfo = tlvInfo;
    SetVTable(this, 0x4BCD00);
    field_20_spawn_timer = 0;

    field_24_scale = pTlv->field_18_scale;
    field_26_num_slogs = pTlv->field_1A_num_slogs;
    field_28_num_at_a_time = pTlv->field_1C_num_at_a_time;
    field_2A_direction = pTlv->field_1E_direction;
    field_2C_ticks_between_slogs = pTlv->field_20_ticks_between_slogs;
    field_2E_start_id = pTlv->field_22_start_id;

    field_14_spawned_count = pTlv->field_1_unknown;

    field_18_xPos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_1C_yPos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);
    return this;
}

BaseGameObject* SlogSpawner::VDestructor(signed int flags)
{
    dtor_487DF0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO

