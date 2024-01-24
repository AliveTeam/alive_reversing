#include "stdafx_ao.h"
#include "SwitchStateBooleanLogic.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Map.hpp"

namespace AO {

void SwitchStateBooleanLogic::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
}

SwitchStateBooleanLogic::~SwitchStateBooleanLogic()
{
    Path::TLV_Reset(mTlvId);
}

SwitchStateBooleanLogic::SwitchStateBooleanLogic(Path_SwitchStateBooleanLogic* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mTlvId = tlvId;

    mInput1 = pTlv->mInput1;
    mInput2 = pTlv->mInput2;

    mOutput = pTlv->mOutput;

    mOperator = pTlv->mOperator;
}


void SwitchStateBooleanLogic::VUpdate()
{
    const s32 id1_value = SwitchStates_Get(mInput1);
    const s32 id2_value = SwitchStates_Get(mInput2);

    s8 id_value = 0;
    switch (mOperator)
    {
        case Path_SwitchStateBooleanLogic::BooleanOperatorType::eAllOn_0:
            if (id1_value && id2_value)
            {
                id_value = 1;
            }
            else
            {
                id_value = 0;
            }
            break;

        case Path_SwitchStateBooleanLogic::BooleanOperatorType::e1OnAnd2Off_1:
            if (id1_value && !id2_value)
            {
                id_value = 1;
            }
            else
            {
                id_value = 0;
            }
            break;

        case Path_SwitchStateBooleanLogic::BooleanOperatorType::e1Or2On_2:
            if (id1_value || id2_value)
            {
                id_value = 1;
            }
            else
            {
                id_value = 0;
            }
            break;

        case Path_SwitchStateBooleanLogic::BooleanOperatorType::e1OnOr2Off_3:
            if (id1_value || !id2_value)
            {
                id_value = 1;
            }
            else
            {
                id_value = 0;
            }
            break;

        default:
            id_value = 0;
            break;
    }

    SwitchStates_Set(mOutput, id_value);

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
}

} // namespace AO
