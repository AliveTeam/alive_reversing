#include "stdafx.h"
#include "SlogSpawner.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "Slog.hpp"
#include "Sfx.hpp"

SlogSpawner* SlogSpawner::ctor_4C7FF0(Path_SlogSpawner* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x5476E8);

    field_20_tlvInfo = tlvInfo;

    field_34_scale = pTlv->field_10_scale;
    field_36_max_slogs = pTlv->field_12_number_of_slogs;
    field_38_at_a_time = pTlv->field_14_at_a_time;
    field_3A_direction = pTlv->field_16_direction;
    field_3C_spawn_time = pTlv->field_18_ticks_between_slogs;
    field_3E_switch_id = pTlv->field_1A_id;
    field_40_listen_to_sligs = pTlv->field_1C_listen_to_sligs;
    field_42_jump_delay = pTlv->field_1E_jump_attack_delay;

    field_24_tlv_saved_slog_count = pTlv->field_1_unknown;

    field_28_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_2C_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_30_spawn_timer = 0;

    return this;
}

BaseGameObject* SlogSpawner::VDestructor(signed int flags)
{
    return vdtor_4C8080(flags);
}

void SlogSpawner::VUpdate()
{
    vUpdate_4C80D0();
}

void SlogSpawner::VScreenChanged()
{
    vScreenChanged_4C82A0();
}

SlogSpawner* SlogSpawner::vdtor_4C8080(signed int flags)
{
    BaseGameObject_dtor_4DBEC0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void SlogSpawner::vScreenChanged_4C82A0()
{
    Path::TLV_Reset_4DB8E0(field_20_tlvInfo, field_24_tlv_saved_slog_count, 0, 0);
    field_6_flags.Set(BaseGameObject::eDead);
}

void SlogSpawner::vUpdate_4C80D0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (static_cast<int>(sGnFrame_5C1B84) > field_30_spawn_timer && sSlogCount_BAF7F2 < field_38_at_a_time)
    {
        if (SwitchStates_Get_466020(field_3E_switch_id))
        {
            field_30_spawn_timer = (field_3C_spawn_time + sGnFrame_5C1B84) + Math_NextRandom() % 8;
            auto pSlog = alive_new<Slog>();
            pSlog->ctor_4C4540(field_28_xpos, field_2C_ypos, field_34_scale != 0 ? FP_FromDouble(0.5) : FP_FromInteger(1), field_40_listen_to_sligs, field_42_jump_delay);
            pSlog->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, field_3A_direction & 1);

            ++field_24_tlv_saved_slog_count;
            SFX_Play_46FA90(115u, 0);

            if (field_24_tlv_saved_slog_count >= field_36_max_slogs)
            {
                Path::TLV_Reset_4DB8E0(field_20_tlvInfo, field_24_tlv_saved_slog_count, 0, 1);
                field_6_flags.Set(BaseGameObject::eDead);
            }
        }
    }
}
