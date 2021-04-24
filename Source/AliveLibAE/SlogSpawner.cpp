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
    field_38_max_slogs_at_a_time = pTlv->field_14_max_slogs_at_a_time;
    field_3A_direction = pTlv->field_16_direction;
    field_3C_slog_spawn_delay = pTlv->field_18_slog_spawn_delay;
    field_3E_switch_id = pTlv->field_1A_id;
    field_40_listen_to_sligs = pTlv->field_1C_listen_to_sligs;
    field_42_chase_delay = pTlv->field_1E_chase_delay;

    field_24_tlv_saved_slog_count = pTlv->field_1_tlv_state;

    field_28_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_2C_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_30_spawn_timer = 0;

    return this;
}

BaseGameObject* SlogSpawner::VDestructor(s32 flags)
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

SlogSpawner* SlogSpawner::vdtor_4C8080(s32 flags)
{
    BaseGameObject_dtor_4DBEC0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void SlogSpawner::vScreenChanged_4C82A0()
{
    Path::TLV_Reset_4DB8E0(field_20_tlvInfo, field_24_tlv_saved_slog_count, 0, 0);
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void SlogSpawner::vUpdate_4C80D0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (static_cast<int>(sGnFrame_5C1B84) > field_30_spawn_timer && sSlogCount_BAF7F2 < field_38_max_slogs_at_a_time)
    {
        if (SwitchStates_Get_466020(field_3E_switch_id))
        {
            field_30_spawn_timer = (field_3C_slog_spawn_delay + sGnFrame_5C1B84) + Math_NextRandom() % 8;
            auto pSlog = ae_new<Slog>();
            pSlog->ctor_4C4540(field_28_xpos, field_2C_ypos, field_34_scale != Scale_short::eFull_0 ? FP_FromDouble(0.5) : FP_FromInteger(1), static_cast<s16>(field_40_listen_to_sligs), field_42_chase_delay);
            pSlog->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, field_3A_direction == XDirection_short::eRight_1);

            ++field_24_tlv_saved_slog_count;
            SFX_Play_46FA90(SoundEffect::SlogSpawn_115, 0);

            if (field_24_tlv_saved_slog_count >= field_36_max_slogs)
            {
                Path::TLV_Reset_4DB8E0(field_20_tlvInfo, field_24_tlv_saved_slog_count, 0, 1);
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
        }
    }
}
