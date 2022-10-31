#include "stdafx.h"
#include "LiftMover.hpp"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "LiftPoint.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "stdlib.hpp"
#include "Path.hpp"

LiftMover::LiftMover(relive::Path_LiftMover* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    field_24_tlvInfo = tlvId;
    field_28_lift_id = Guid{};
    SetType(ReliveTypes::eLiftMover);

    field_20_lift_mover_switch_id = pTlv->mLiftMoverSwitchId;
    field_22_target_lift_point_id = pTlv->mTargetLiftPointId;

    if (pTlv->mMoveDirection == relive::Path_LiftMover::YDirection::eUp)
    {
        field_2C_speed = FP_FromInteger(-4);
    }
    else
    {
        field_2C_speed = FP_FromInteger(4);
    }

    field_30_state = LiftMoverStates::eInactive_0;
    field_32_bMoveInProgress = false;
}

s32 LiftMover::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const LiftMoverSaveState*>(pData);

    relive::Path_LiftMover* pTlv = static_cast<relive::Path_LiftMover*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_4_tlvInfo));
    auto pLiftMover = relive_new LiftMover(pTlv, pState->field_4_tlvInfo);
    if (pLiftMover)
    {
        if (pState->field_8_state != LiftMoverStates::eInactive_0)
        {
            pLiftMover->field_32_bMoveInProgress = true;
        }
        pLiftMover->field_30_state = pState->field_8_state;
    }

    return sizeof(LiftMoverSaveState);
}

void LiftMover::VUpdate()
{
    LiftPoint* pLift = static_cast<LiftPoint*>(sObjectIds.Find(field_28_lift_id, ReliveTypes::eLiftPoint));
    if (field_32_bMoveInProgress)
    {
        pLift = GetLiftPoint();
        if (!pLift)
        {
            return;
        }
        field_32_bMoveInProgress = false;
    }

    if (pLift && pLift->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        // Set extra dead for good measure
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_28_lift_id = Guid{};
    }
    else
    {
        switch (field_30_state)
        {
            case LiftMoverStates::eInactive_0:
                if (SwitchStates_Get(field_20_lift_mover_switch_id))
                {
                    if (!pLift)
                    {
                        pLift = GetLiftPoint();
                    }

                    if (pLift)
                    {
                        field_28_lift_id = pLift->mBaseGameObjectId;
                        field_30_state = LiftMoverStates::eStartMovingDown_1;
                    }
                }
                break;

            case LiftMoverStates::eStartMovingDown_1:
                if (!pLift)
                {
                    return;
                }

                if (!pLift->vOnAnyFloor())
                {
                    field_30_state = LiftMoverStates::eMovingDown_2;
                    pLift->vKeepOnMiddleFloor();
                }
                else
                {
                    pLift->vMove_4626A0(FP_FromInteger(0), field_2C_speed, 0);
                    if ((field_2C_speed > FP_FromInteger(0) && pLift->vOnBottomFloor()) || (field_2C_speed < FP_FromInteger(0) && pLift->vOnTopFloor()))
                    {
                        field_30_state = LiftMoverStates::eMovingDown_2;
                    }
                }
                break;

            case LiftMoverStates::eMovingDown_2:
                if (!pLift)
                {
                    return;
                }

                if (!pLift->vOnAFloorLiftMoverCanUse())
                {
                    pLift->vMove_4626A0(FP_FromInteger(0), field_2C_speed, 0);
                }
                else
                {
                    pLift->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
                    field_30_state = LiftMoverStates::eMovingDone_5;
                }
                break;

            case LiftMoverStates::eStartMovingUp_3:
                if (!pLift)
                {
                    return;
                }

                if (pLift->vOnAFloorLiftMoverCanUse())
                {
                    pLift->vMove_4626A0(FP_FromInteger(0), field_2C_speed, 0);
                    if (field_2C_speed < FP_FromInteger(0))
                    {
                        if (pLift->vOnTopFloor())
                        {
                            field_30_state = LiftMoverStates::eMovingDown_2;
                        }
                    }

                    if (field_2C_speed > FP_FromInteger(0))
                    {
                        if (pLift->vOnBottomFloor())
                        {
                            field_30_state = LiftMoverStates::eMovingDown_2;
                        }
                    }
                }
                else
                {
                    field_30_state = LiftMoverStates::eMovingUp_4;
                    pLift->vKeepOnMiddleFloor();
                }
                break;

            case LiftMoverStates::eMovingUp_4:
                if (!pLift)
                {
                    return;
                }

                if (pLift->vOnAFloorLiftMoverCanUse())
                {
                    pLift->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);

                    field_30_state = LiftMoverStates::eInactive_0;
                    field_2C_speed = -field_2C_speed;
                }
                else
                {
                    pLift->vMove_4626A0(FP_FromInteger(0), field_2C_speed, 0);
                }
                break;

            case LiftMoverStates::eMovingDone_5:
                if (!SwitchStates_Get(field_20_lift_mover_switch_id))
                {
                    field_30_state = LiftMoverStates::eStartMovingUp_3;
                    field_2C_speed = -field_2C_speed;
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
}

LiftMover::~LiftMover()
{
    Path::TLV_Reset(field_24_tlvInfo, -1, 0, 0);
}

s32 LiftMover::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<LiftMoverSaveState*>(pSaveBuffer);

    pState->field_0_type_id = AETypes::eLiftMover_9;
    pState->field_4_tlvInfo = field_24_tlvInfo;
    pState->field_8_state = field_30_state;
    return sizeof(LiftMoverSaveState);
}

LiftPoint* LiftMover::GetLiftPoint()
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eLiftPoint)
        {
            LiftPoint* pLift = static_cast<LiftPoint*>(pObj);
            if (field_22_target_lift_point_id == pLift->field_278_lift_point_id)
            {
                field_28_lift_id = pObj->mBaseGameObjectId;
                return pLift;
            }
        }
    }
    return nullptr;
}
