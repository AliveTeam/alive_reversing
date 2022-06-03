#include "stdafx_ao.h"
#include "Function.hpp"
#include "LiftMover.hpp"
#include "LiftPoint.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"

namespace AO {

LiftMover::LiftMover(Path_LiftMover* pTlv, s32 tlvInfo)
    : BaseGameObject(1)
{
    field_14_tlvInfo = tlvInfo;
    field_18_pLiftPoint = nullptr;
    mTypeId = Types::eLiftMover_7;

    field_10_lift_mover_switch_id = pTlv->field_18_lift_mover_switch_id;
    field_12_target_lift_point_id = pTlv->field_1A_target_lift_point_id;

    if (pTlv->field_1C_direction == Path_LiftMover::YDirection::eUp_1)
    {
        field_1C_speed = FP_FromInteger(-4);
    }
    else
    {
        field_1C_speed = FP_FromInteger(4);
    }

    field_20_state = 0;
}

LiftMover::~LiftMover()
{
    if (field_18_pLiftPoint)
    {
        field_18_pLiftPoint->field_C_refCount--;
        field_18_pLiftPoint = nullptr;
    }
    gMap.TLV_Reset(field_14_tlvInfo, -1, 0, 0);
}

void LiftMover::VUpdate()
{
    if (field_18_pLiftPoint && field_18_pLiftPoint->mGameObjectFlags.Get(BaseGameObject::eDead))
    {
        field_18_pLiftPoint->field_C_refCount--;
        field_18_pLiftPoint = nullptr;
        mGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    // NOTE: Isn't null checked, could easily crash later :)
    auto pLiftPoint = static_cast<LiftPoint*>(field_18_pLiftPoint);

    switch (field_20_state)
    {
        case 0:
            if (SwitchStates_Get(field_10_lift_mover_switch_id))
            {
                if (pLiftPoint)
                {
                    field_20_state = 1;
                }
                else
                {
                    // Find the lift point
                    field_18_pLiftPoint = FindLiftPointWithId(field_12_target_lift_point_id);

                    if (!field_18_pLiftPoint)
                    {
                        // Load lift point objects (I guess in case for some reason it got unloaded ??)
                        // AE doesn't do this.
                        for (s16 y = 0; y < gMap.field_26_max_cams_y; y++)
                        {
                            for (s16 x = 0; x < gMap.field_24_max_cams_x; x++)
                            {
                                gMap.Loader(x, y, LoadMode::ConstructObject_0, TlvTypes::LiftPoint_8);
                            }
                        }

                        // And have another look now that we might have just loaded it in
                        field_18_pLiftPoint = FindLiftPointWithId(field_12_target_lift_point_id);
                    }

                    if (field_18_pLiftPoint)
                    {
                        field_18_pLiftPoint->field_C_refCount++;
                        field_20_state = 1;
                    }
                }
            }
            break;

        case 1:
            if (!pLiftPoint->OnAnyFloor())
            {
                pLiftPoint->field_27A_flags.Set(LiftPoint::Flags::eBit8_KeepOnMiddleFloor);
                field_20_state = 2;
            }
            else
            {
                pLiftPoint->Move(FP_FromInteger(0), field_1C_speed, 0);

                if ((field_1C_speed > FP_FromInteger(0) && pLiftPoint->OnBottomFloor()) || (field_1C_speed < FP_FromInteger(0) && pLiftPoint->OnTopFloor()))
                {
                    field_20_state = 2;
                }
            }
            break;

        case 2:
            if (!pLiftPoint->OnAFloorLiftMoverCanUse())
            {
                pLiftPoint->Move(FP_FromInteger(0), field_1C_speed, 0);
            }
            else
            {
                pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0), 0);
                field_20_state = 5;
            }
            break;

        case 3:
            if (pLiftPoint->OnAFloorLiftMoverCanUse())
            {
                pLiftPoint->Move(FP_FromInteger(0), field_1C_speed, 0);

                if ((field_1C_speed > FP_FromInteger(0) && pLiftPoint->OnBottomFloor()) || (field_1C_speed < FP_FromInteger(0) && pLiftPoint->OnTopFloor()))
                {
                    field_20_state = 2;
                }
            }
            else
            {
                pLiftPoint->field_27A_flags.Set(LiftPoint::Flags::eBit8_KeepOnMiddleFloor);
                field_20_state = 4;
            }
            break;

        case 4:
            if (pLiftPoint->OnAFloorLiftMoverCanUse())
            {
                pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0), 0);
                field_20_state = 0;
                field_1C_speed = -field_1C_speed;
            }
            else
            {
                pLiftPoint->Move(FP_FromInteger(0), field_1C_speed, 0);
            }
            break;

        case 5:
            if (!SwitchStates_Get(field_10_lift_mover_switch_id))
            {
                field_20_state = 3;
                field_1C_speed = -field_1C_speed;
            }
            break;

        default:
            break;
    }

    if (Event_Get(kEventDeathReset_4))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

LiftPoint* LiftMover::FindLiftPointWithId(s16 id)
{
    for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        auto pItem = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pItem)
        {
            break;
        }

        if (pItem->mTypeId == Types::eLiftPoint_51)
        {
            auto pLiftPoint = static_cast<LiftPoint*>(pItem);
            if (pLiftPoint->field_278_lift_point_id == id)
            {
                return pLiftPoint;
            }
        }
    }
    return nullptr;
}

} // namespace AO
