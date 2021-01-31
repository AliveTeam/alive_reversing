#include "stdafx_ao.h"
#include "Function.hpp"
#include "LiftMover.hpp"
#include "LiftPoint.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"

namespace AO {

LiftMover* LiftMover::ctor_4054E0(Path_LiftMover* pTlv, int tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BA158);

    field_14_tlvInfo = tlvInfo;
    field_18_pLiftPoint = nullptr;
    field_4_typeId = Types::eLiftMover_7;

    field_10_enabled_by_switch_id = pTlv->field_18_switch_id;
    field_12_target_lift_point_id = pTlv->field_1A_lift_id;

    if (pTlv->field_1C_direction)
    {
        field_1C_speed = FP_FromInteger(-4);
    }
    else
    {
        field_1C_speed = FP_FromInteger(4);
    }

    field_20_state = 0;

    return this;
}

BaseGameObject* LiftMover::dtor_405550()
{
    SetVTable(this, 0x4BA158);
    if (field_18_pLiftPoint)
    {
        field_18_pLiftPoint->field_C_refCount--;
        field_18_pLiftPoint = nullptr;
    }
    gMap_507BA8.TLV_Reset_446870(field_14_tlvInfo, -1, 0, 0);
    return dtor_487DF0();
}

BaseGameObject* LiftMover::VDestructor(signed int flags)
{
    return Vdtor_405A50(flags);
}

EXPORT LiftMover* LiftMover::Vdtor_405A50(signed int flags)
{
    dtor_405550();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void LiftMover::VUpdate()
{
    VUpdate_4055C0();
}

void LiftMover::VUpdate_4055C0()
{
    if (field_18_pLiftPoint && field_18_pLiftPoint->field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        field_18_pLiftPoint->field_C_refCount--;
        field_18_pLiftPoint = nullptr;
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return;
    }

    // NOTE: Isn't null checked, could easily crash later :)
    auto pLiftPoint = static_cast<LiftPoint*>(field_18_pLiftPoint);

    switch (field_20_state)
    {
    case 0:
        if (SwitchStates_Get(field_10_enabled_by_switch_id))
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
                    for (short y = 0; y < gMap_507BA8.field_26_max_cams_y; y++)
                    {
                        for (short x = 0; x < gMap_507BA8.field_24_max_cams_x; x++)
                        {
                            gMap_507BA8.Loader_446590(x, y, LoadMode::Mode_0, TlvTypes::LiftPoint_8);
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
            pLiftPoint->Move_435740(FP_FromInteger(0), field_1C_speed, 0);

            if (field_1C_speed > FP_FromInteger(0) && pLiftPoint->OnBottomFloor() ||
                field_1C_speed < FP_FromInteger(0) && pLiftPoint->OnTopFloor())
            {
                field_20_state = 2;
            }
        }
        break;

    case 2:
        if (!pLiftPoint->OnAFloorLiftMoverCanUse())
        {
            pLiftPoint->Move_435740(FP_FromInteger(0), field_1C_speed, 0);
        }
        else
        {
            pLiftPoint->Move_435740(FP_FromInteger(0), FP_FromInteger(0), 0);
            field_20_state = 5;
        }
        break;

    case 3:
        if (pLiftPoint->OnAFloorLiftMoverCanUse())
        {
            pLiftPoint->Move_435740(FP_FromInteger(0), field_1C_speed, 0);

            if (field_1C_speed > FP_FromInteger(0) && pLiftPoint->OnBottomFloor() ||
                field_1C_speed < FP_FromInteger(0) && pLiftPoint->OnTopFloor())
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
            pLiftPoint->Move_435740(FP_FromInteger(0), FP_FromInteger(0), 0);
            field_20_state = 0;
            field_1C_speed = -field_1C_speed;
        }
        else
        {
            pLiftPoint->Move_435740(FP_FromInteger(0), field_1C_speed, 0);
        }
        break;

    case 5:
        if (!SwitchStates_Get(field_10_enabled_by_switch_id))
        {
            field_20_state = 3;
            field_1C_speed = -field_1C_speed;
        }
        break;

    default:
        break;
    }

    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

LiftPoint* LiftMover::FindLiftPointWithId(short id)
{
    for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        auto pItem = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pItem)
        {
            break;
        }

        if (pItem->field_4_typeId == Types::eLiftPoint_51)
        {
            auto pLiftPoint = static_cast<LiftPoint*>(pItem);
            if (pLiftPoint->field_278_point_id == id)
            {
                return pLiftPoint;
            }
        }
    }
    return nullptr;
}

}
