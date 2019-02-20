#include "stdafx.h"
#include "LiftMover.hpp"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "LiftPoint.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "ObjectIds.hpp"
#include "stdlib.hpp"

EXPORT LiftMover* LiftMover::ctor_40CCD0(Path_LiftMover* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x5440D4);

    field_24_tlvInfo = tlvInfo;
    field_28_lift_id = -1;
    field_4_typeId = Types::eLiftMover_9;

    field_20_enabled_by_switch_id = pTlv->field_10_switch_id;
    field_22_target_lift_point_id = pTlv->field_12_lift_id;

    if (pTlv->field_14_direction)
    {
        field_2C_speed = FP_FromInteger(-4);
    }
    else
    {
        field_2C_speed = FP_FromInteger(4);
    }

    field_30_state = 0;
    field_32_bMoveInProgress = 0;
    return this;
}

int CC LiftMover::CreateFromSaveState_40D180(const BYTE* pData)
{
    auto pState = reinterpret_cast<const LiftMover_State*>(pData);

    Path_LiftMover* pTlv = static_cast<Path_LiftMover*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_4_tlvInfo));
    auto pLiftMover = alive_new<LiftMover>();
    if (pLiftMover)
    {
        pLiftMover->ctor_40CCD0(pTlv, pState->field_4_tlvInfo);

        if (pState->field_8_state != 0)
        {
            pLiftMover->field_32_bMoveInProgress = 1;
        }
        pLiftMover->field_30_state = pState->field_8_state;

    }

    return sizeof(LiftMover_State);
}

BaseGameObject* LiftMover::VDestructor(signed int flags)
{
    return vdtor_40CD70(flags);
}

void LiftMover::VUpdate()
{
    vUpdate_40CE20();
}

int LiftMover::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_40D240(reinterpret_cast<LiftMover_State*>(pSaveBuffer));
}

void LiftMover::vUpdate_40CE20()
{
    LiftPoint* pLift = static_cast<LiftPoint*>(sObjectIds_5C1B70.Find_449CF0(field_28_lift_id));
    if (field_32_bMoveInProgress)
    {
        pLift = GetLiftPoint_40D0F0();
        if (!pLift)
        {
            return;
        }
        field_32_bMoveInProgress = 0;
    }

    if (pLift && pLift->field_6_flags.Get(BaseGameObject::eDead))
    {
        // Set extra dead for good measure
        field_6_flags.Set(BaseGameObject::eDead);
        field_28_lift_id = -1;
    }
    else
    {
        switch (field_30_state)
        {
        case 0:
            if (SwitchStates_Get_466020(field_20_enabled_by_switch_id))
            {
                if (!pLift)
                {
                    pLift = GetLiftPoint_40D0F0();
                }

                if (pLift)
                {
                    field_28_lift_id = pLift->field_8_object_id;
                    field_30_state = 1;
                }
            }
            break;

        case 1:
            if (!pLift)
            {
                return;
            }

            if (!pLift->vOnAnyFloor_461920())
            {
                field_30_state = 2;
                pLift->vKeepOnMiddleFloor_461870();
            }
            else
            {
                pLift->vMove_4626A0(FP_FromInteger(0), field_2C_speed, 0);
                if (field_2C_speed < FP_FromInteger(0))
                {
                    if (pLift->vOnTopFloor_461890())
                    {
                        field_30_state = 2;
                    }
                }
            }
            break;

        case 2:
            if (!pLift)
            {
                return;
            }

            if (!pLift->vOnAFloorLiftMoverCanUse_461960())
            {
                pLift->vMove_4626A0(FP_FromInteger(0), field_2C_speed, 0);
            }
            else
            {
                pLift->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
                field_30_state = 5;
            }
            break;

        case 3:
            if (!pLift)
            {
                return;
            }

            if (pLift->vOnAFloorLiftMoverCanUse_461960())
            {
                pLift->vMove_4626A0(FP_FromInteger(0), field_2C_speed, 0);
                if (field_2C_speed < FP_FromInteger(0))
                {
                    if (pLift->vOnTopFloor_461890())
                    {
                        field_30_state = 2;
                    }
                }

                if (field_2C_speed > FP_FromInteger(0))
                {
                    if (pLift->vOnBottomFloor_4618F0())
                    {
                        field_30_state = 2;
                    }
                }
            }
            else
            {
                field_30_state = 4;
                pLift->vKeepOnMiddleFloor_461870();
            }
            break;

        case 4:
            if (!pLift)
            {
                return;
            }

            if (pLift->vOnAFloorLiftMoverCanUse_461960())
            {
                pLift->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);

                field_30_state = 0;
                field_2C_speed = -field_2C_speed;
            }
            else
            {
                pLift->vMove_4626A0(FP_FromInteger(0), field_2C_speed, 0);
            }
            break;

        case 5:
            if (!SwitchStates_Get_466020(field_20_enabled_by_switch_id))
            {
                field_30_state = 3;
                field_2C_speed = -field_2C_speed;
            }
            break;

        default:
            break;
        }

        if (Event_Get_422C00(kEventDeathReset))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }
}

void LiftMover::dtor_40CDA0()
{
    SetVTable(this, 0x5440D4);
    Path::TLV_Reset_4DB8E0(field_24_tlvInfo, -1, 0, 0);
    BaseGameObject_dtor_4DBEC0();
}

LiftMover* LiftMover::vdtor_40CD70(signed int flags)
{
    dtor_40CDA0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

int LiftMover::vGetSaveState_40D240(LiftMover_State* pState)
{
    pState->field_0_type = 9; // TODO: Types
    pState->field_4_tlvInfo = field_24_tlvInfo;
    pState->field_8_state = field_30_state;
    return sizeof(LiftMover_State);
}

LiftPoint* LiftMover::GetLiftPoint_40D0F0()
{
    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eLiftPoint_78)
        {
            LiftPoint* pLift = static_cast<LiftPoint*>(pObj);
            if (field_22_target_lift_point_id == pLift->field_278_lift_point_id)
            {
                field_28_lift_id = pObj->field_8_object_id;
                return pLift;
            }
        }
    }
    return nullptr;
}
