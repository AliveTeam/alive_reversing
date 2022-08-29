#include "stdafx_ao.h"
#include "Function.hpp"
#include "LiftMover.hpp"
#include "LiftPoint.hpp"
#include "../relive_lib/Events.hpp"
#include "SwitchStates.hpp"
#include "../AliveLibAE/stdlib.hpp"

namespace AO {

LiftMover::LiftMover(relive::Path_LiftMover* pTlv, const Guid& tlvId)
    : BaseGameObject(TRUE, 0)
{
    field_14_tlvInfo = tlvId;
    field_18_pLiftPoint = nullptr;
    mBaseGameObjectTypeId = ReliveTypes::eLiftMover;

    field_10_lift_mover_switch_id = pTlv->mLiftMoverSwitchId;
    field_12_target_lift_point_id = pTlv->mTargetLiftPointId;

    if (pTlv->mMoveDirection == relive::Path_LiftMover::YDirection::eUp)
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
        field_18_pLiftPoint->mBaseGameObjectRefCount--;
        field_18_pLiftPoint = nullptr;
    }
    Path::TLV_Reset(field_14_tlvInfo, -1, 0, 0);
}

void LiftMover::VUpdate()
{
    if (field_18_pLiftPoint && field_18_pLiftPoint->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        field_18_pLiftPoint->mBaseGameObjectRefCount--;
        field_18_pLiftPoint = nullptr;
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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
                                gMap.Loader(x, y, LoadMode::ConstructObject_0, ReliveTypes::eLiftPoint);
                            }
                        }

                        // And have another look now that we might have just loaded it in
                        field_18_pLiftPoint = FindLiftPointWithId(field_12_target_lift_point_id);
                    }

                    if (field_18_pLiftPoint)
                    {
                        field_18_pLiftPoint->mBaseGameObjectRefCount++;
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

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

LiftPoint* LiftMover::FindLiftPointWithId(s16 id)
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        auto pItem = gBaseAliveGameObjects->ItemAt(i);
        if (!pItem)
        {
            break;
        }

        if (pItem->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint)
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
