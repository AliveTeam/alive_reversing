#include "stdafx.h"
#include "SlogSpawner.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "Slog.hpp"
#include "Sfx.hpp"

SlogSpawner::SlogSpawner(Path_SlogSpawner* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_20_tlvInfo = tlvInfo;

    field_34_scale = pTlv->field_10_scale;
    field_36_max_slogs = pTlv->field_12_max_slogs;
    field_38_max_slogs_at_a_time = pTlv->field_14_max_slogs_at_a_time;
    field_3A_start_direction = pTlv->field_16_start_direction;
    field_3C_slog_spawn_interval = pTlv->field_18_slog_spawn_interval;
    field_3E_spawner_switch_id = pTlv->field_1A_spawner_switch_id;
    field_40_listen_to_sligs = pTlv->field_1C_listen_to_sligs;
    field_42_chase_delay = pTlv->field_1E_chase_delay;

    field_24_tlv_saved_slog_count = pTlv->field_1_tlv_state;

    field_28_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_2C_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_30_spawn_timer = 0;
}

void SlogSpawner::VScreenChanged()
{
    Path::TLV_Reset(field_20_tlvInfo, field_24_tlv_saved_slog_count, 0, 0);
    mFlags.Set(BaseGameObject::eDead);
}

void SlogSpawner::VUpdate()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    if (static_cast<s32>(sGnFrame_5C1B84) > field_30_spawn_timer && sSlogCount_BAF7F2 < field_38_max_slogs_at_a_time)
    {
        if (SwitchStates_Get(field_3E_spawner_switch_id))
        {
            field_30_spawn_timer = (field_3C_slog_spawn_interval + sGnFrame_5C1B84) + Math_NextRandom() % 8;
            auto pSlog = ae_new<Slog>(field_28_xpos, field_2C_ypos, field_34_scale != Scale_short::eFull_0 ? FP_FromDouble(0.5) : FP_FromInteger(1), static_cast<s16>(field_40_listen_to_sligs), field_42_chase_delay);
            if (pSlog)
            {
                pSlog->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, field_3A_start_direction == StartDirection::eLeft_1);
            }

            ++field_24_tlv_saved_slog_count;
            SFX_Play_Mono(SoundEffect::SlogSpawn_115, 0);

            if (field_24_tlv_saved_slog_count >= field_36_max_slogs)
            {
                Path::TLV_Reset(field_20_tlvInfo, field_24_tlv_saved_slog_count, 0, 1);
                mFlags.Set(BaseGameObject::eDead);
            }
        }
    }
}
