#include "stdafx_ao.h"
#include "Function.hpp"
#include "SlogSpawner.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"
#include "Slog.hpp"
#include "Events.hpp"
#include "Math.hpp"

namespace AO {

void SlogSpawner::VScreenChanged()
{
    gMap.TLV_Reset(field_10_tlvInfo, field_14_spawned_count, 0, 0);
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

SlogSpawner::SlogSpawner(Path_SlogSpawner* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_10_tlvInfo = tlvInfo;
    field_20_spawn_timer = 0;

    field_24_scale = pTlv->field_18_scale;
    field_26_max_slogs = pTlv->field_1A_max_slogs;
    field_28_max_slogs_at_a_time = pTlv->field_1C_max_slogs_at_a_time;
    field_2A_start_direction = pTlv->field_1E_start_direction;
    field_2C_slog_spawn_interval = pTlv->field_20_slog_spawn_interval;
    field_2E_spawner_switch_id = pTlv->field_22_spawner_switch_id;

    field_14_spawned_count = pTlv->field_1_unknown;

    field_18_xPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_1C_yPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
}

void SlogSpawner::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (static_cast<s32>(sGnFrame) > field_20_spawn_timer && gNumSlogs_9F11C8 < field_28_max_slogs_at_a_time)
    {
        if (SwitchStates_Get(field_2E_spawner_switch_id))
        {
            field_20_spawn_timer = Math_NextRandom() % 8
                                 + sGnFrame
                                 + field_2C_slog_spawn_interval;

            auto pSlog = ao_new<Slog>(
                    field_18_xPos,
                    field_1C_yPos,
                    field_24_scale != Scale_short::eFull_0 ? FP_FromDouble(0.5) : FP_FromInteger(1));;
            if (pSlog)
            {
                pSlog->field_10_anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, field_2A_start_direction == StartDirection::eLeft_1);
            }

            field_14_spawned_count++;

            if (field_14_spawned_count >= field_26_max_slogs)
            {
                gMap.TLV_Reset(field_10_tlvInfo, field_14_spawned_count, 0, 1);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
    }
}

} // namespace AO
