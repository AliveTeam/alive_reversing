#include "stdafx_ao.h"
#include "Function.hpp"
#include "ZzzSpawner.hpp"
#include "stdlib.hpp"
#include "SnoozeParticle.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"

namespace AO {

ZzzSpawner::~ZzzSpawner()
{
    gMap.TLV_Reset(field_1C_tlvInfo, -1, 0, 0);
}

ZzzSpawner::ZzzSpawner(Path_ZzzSpawner* pTlv, s32 tlvInfo)
    : BaseGameObject(1)
{
    field_10_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_14_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    field_1C_tlvInfo = tlvInfo;

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        field_18_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_18_scale = FP_FromInteger(1);
    }

    field_20_switch_id = pTlv->field_1A_switch_id;
    field_28_Zzz_delay = pTlv->field_1C_Zzz_delay;
    field_24_timer = 0;
}

void ZzzSpawner::VScreenChanged()
{
    mGameObjectFlags.Set(BaseGameObject::eDead);
}

void ZzzSpawner::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!SwitchStates_Get(field_20_switch_id) && static_cast<s32>(gnFrameCount_507670) > field_24_timer)
    {
        Layer snoozeLayer = Layer::eLayer_0;
        if (field_18_scale != FP_FromInteger(1))
        {
            snoozeLayer = Layer::eLayer_Above_FG1_Half_20;
        }
        else
        {
            snoozeLayer = Layer::eLayer_Above_FG1_39;
        }
        ao_new<SnoozeParticle>(
            field_10_xpos,
            field_14_ypos,
            snoozeLayer,
            field_18_scale);

        field_24_timer = gnFrameCount_507670 + field_28_Zzz_delay;
    }
}

} // namespace AO
