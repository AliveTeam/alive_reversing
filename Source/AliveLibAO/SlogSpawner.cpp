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

void SlogSpawner::VScreenChanged_475F90()
{
    gMap_507BA8.TLV_Reset_446870(field_10_tlvInfo, field_14_spawned_count, 0, 0);
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void SlogSpawner::VScreenChanged()
{
    VScreenChanged_475F90();
}

SlogSpawner* SlogSpawner::ctor_475DD0(Path_SlogSpawner* pTlv, s32 tlvInfo)
{
    ctor_487E10(1);

    field_10_tlvInfo = tlvInfo;
    SetVTable(this, 0x4BCD00);
    field_20_spawn_timer = 0;

    field_24_scale = pTlv->field_18_scale;
    field_26_max_slogs = pTlv->field_1A_max_slogs;
    field_28_max_slogs_at_a_time = pTlv->field_1C_max_slogs_at_a_time;
    field_2A_direction = pTlv->field_1E_direction;
    field_2C_slog_spawn_delay = pTlv->field_20_slog_spawn_delay;
    field_2E_start_switch_id = pTlv->field_22_start_switch_id;

    field_14_spawned_count = pTlv->field_1_unknown;

    field_18_xPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_1C_yPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    return this;
}

BaseGameObject* SlogSpawner::VDestructor(s32 flags)
{
    dtor_487DF0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void SlogSpawner::VUpdate()
{
    VUpdate_475E30();
}

void SlogSpawner::VUpdate_475E30()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (static_cast<s32>(gnFrameCount_507670) > field_20_spawn_timer && gNumSlogs_9F11C8 < field_28_max_slogs_at_a_time)
    {
        if (SwitchStates_Get(field_2E_start_switch_id))
        {
            field_20_spawn_timer = Math_NextRandom() % 8
                                 + gnFrameCount_507670
                                 + field_2C_slog_spawn_delay;

            auto pSlog = ao_new<Slog>();
            if (pSlog)
            {
                pSlog->ctor_473050(
                    field_18_xPos,
                    field_1C_yPos,
                    field_24_scale != Scale_short::eFull_0 ? FP_FromDouble(0.5) : FP_FromInteger(1));

                pSlog->field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, field_2A_direction == XDirection_short::eRight_1);
            }

            field_14_spawned_count++;

            if (field_14_spawned_count >= field_26_max_slogs)
            {
                gMap_507BA8.TLV_Reset_446870(field_10_tlvInfo, field_14_spawned_count, 0, 1);
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
        }
    }
}

} // namespace AO
