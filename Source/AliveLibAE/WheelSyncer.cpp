#include "stdafx.h"
#include "WheelSyncer.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

WheelSyncer::WheelSyncer(relive::Path_WheelSyncer* pTlv, const Guid& tlvId)
    : BaseGameObject(TRUE, 0)
{
    mTlvInfo = tlvId;
    mInputSwitchId1 = pTlv->mInputSwitchId1;
    mInputSwitchId2 = pTlv->mInputSwitchId2;
    mOutputSwitchId = pTlv->mOutputSwitchId;
    mOutputRequirement = pTlv->mOutputRequirement;
    mInputSwitchId3 = pTlv->mInputSwitchId3;
    mInputSwitchId4 = pTlv->mInputSwitchId4;
    mInputSwitchId5 = pTlv->mInputSwitchId5;
    mInputSwitchId6 = pTlv->mInputSwitchId6;
}

void WheelSyncer::VUpdate()
{
    const s32 state1 = SwitchStates_Get(mInputSwitchId1);
    const s32 state2 = SwitchStates_Get(mInputSwitchId2);
    const s32 state3 = SwitchStates_Get(mInputSwitchId3);
    const s32 state4 = SwitchStates_Get(mInputSwitchId4);
    const s32 state5 = SwitchStates_Get(mInputSwitchId5);
    const s32 state6 = SwitchStates_Get(mInputSwitchId6);

    s32 switchValue = 0;

    switch (mOutputRequirement)
    {
        case relive::Path_WheelSyncer::OutputRequirement::eAllOn:
            if (!state1 || !state2 || !state3 || !state4 || !state5 || !state6)
            {
                switchValue = 0;
            }
            else
            {
                switchValue = 1;
            }
            break;

        case relive::Path_WheelSyncer::OutputRequirement::e1OnAnd2Off:
            if (!state1 || state2)
            {
                switchValue = 0;
            }
            else
            {
                switchValue = 1;
            }
            break;

        case relive::Path_WheelSyncer::OutputRequirement::e1Or2On:
            if (state1 || state2)
            {
                switchValue = 1;
            }
            else
            {
                switchValue = 0;
            }
            break;

        case relive::Path_WheelSyncer::OutputRequirement::e1OnOr2Off:
            if (state1 || !state2)
            {
                switchValue = 1;
            }
            else
            {
                switchValue = 0;
            }
            break;

        default:
            switchValue = state5;
            break;
    }

    SwitchStates_Set(mOutputSwitchId, static_cast<s8>(switchValue));

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

WheelSyncer::~WheelSyncer()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

void WheelSyncer::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}
