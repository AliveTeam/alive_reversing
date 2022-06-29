#include "stdafx.h"
#include "ZzzSpawner.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "SnoozeParticle.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"

ZzzSpawner::ZzzSpawner(Path_ZzzSpawner* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_20_xpos = FP_FromInteger(pTlv->mTopLeft.x);
    field_24_ypos = FP_FromInteger(pTlv->mTopLeft.y);
    field_2C_tlvInfo = tlvInfo;

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        field_28_scale = FP_FromDouble(0.5);
    }
    else if (pTlv->field_10_scale == Scale_short::eFull_0)
    {
        field_28_scale = FP_FromInteger(1);
    }

    field_30_switch_id = pTlv->field_12_switch_id;
    field_38_Zzz_interval = pTlv->field_14_Zzz_interval;
    field_34_Zzz_timer = 0;
}

ZzzSpawner::~ZzzSpawner()
{
    Path::TLV_Reset(field_2C_tlvInfo, -1, 0, 0);
}

void ZzzSpawner::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void ZzzSpawner::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!SwitchStates_Get(field_30_switch_id) && static_cast<s32>(sGnFrame) > field_34_Zzz_timer)
    {
        relive_new SnoozeParticle(field_20_xpos, field_24_ypos, Layer::eLayer_Above_FG1_39, field_28_scale);

        field_34_Zzz_timer = sGnFrame + field_38_Zzz_interval;
    }
}
