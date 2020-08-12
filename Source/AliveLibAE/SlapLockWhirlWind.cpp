#include "stdafx.h"
#include "SlapLockWhirlWind.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "OrbWhirlWind.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "Door.hpp"

int CC SlapLockWhirlWind::CreateFromSaveState_43DC20(const BYTE* pBuffer)
{
    auto pSaveState = reinterpret_cast<const SlapLockWhirlWind_State*>(pBuffer);
    SwitchStates_Do_Operation_465F00(pSaveState->field_2_switch_id, SwitchOp::eSetTrue_0);
    return sizeof(SlapLockWhirlWind_State);
}

SlapLockWhirlWind* SlapLockWhirlWind::ctor_43D7E0(__int16 doorNumber, __int16 switchId, FP xpos, FP ypos, FP scale)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x545208);
    field_4_typeId = Types::eSlapLock_OrbWhirlWind_60;

    field_20_xpos = xpos;
    field_24_ypos = ypos;

    field_28_scale = scale;
    field_44_switch_id = switchId;

    bool bFoundTarget = false;
    for (short y = 0; y < sPath_dword_BB47C0->field_8_cams_on_y; y++)
    {
        for (short x = 0; x < sPath_dword_BB47C0->field_6_cams_on_x; x++)
        {
            Path_Door* pDoorTlv = static_cast<Path_Door*>(sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera_4DB610(
                x - gMap_5C3030.field_D0_cam_x_idx,
                y - gMap_5C3030.field_D2_cam_y_idx));
            while (pDoorTlv)
            {
                if (pDoorTlv->field_4_type == TlvTypes::Door_5 && pDoorTlv->field_18_door_number == doorNumber)
                {
                    // For some reason once found we just keep on searching...
                    bFoundTarget = true;

                    field_2C_door_x = FP_FromInteger((pDoorTlv->field_8_top_left.field_0_x + pDoorTlv->field_C_bottom_right.field_0_x) / 2);
                    field_30_door_y = FP_FromInteger((pDoorTlv->field_8_top_left.field_2_y + pDoorTlv->field_C_bottom_right.field_2_y) / 2);

                    if (pDoorTlv->field_16_scale)
                    {
                        field_34_door_scale = FP_FromDouble(0.5);
                    }
                    else
                    {
                        field_34_door_scale = FP_FromInteger(1);
                    }

                    field_30_door_y -= (FP_FromInteger(40) * field_34_door_scale);
                }
                pDoorTlv = static_cast<Path_Door*>(sPath_dword_BB47C0->Next_TLV_4DB6A0(pDoorTlv));
            }
        }
    }

    if (bFoundTarget)
    {
        auto pWhirlWind = ae_new<OrbWhirlWind>();
        if (pWhirlWind)
        {
            pWhirlWind->ctor_4E3C90(xpos, ypos, scale, 1);
        }
        field_3C_state = 0;
        field_38_orb_whirlwind_id = pWhirlWind->field_8_object_id;
        field_40_timer = sGnFrame_5C1B84 + 70;
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    return this;
}

BaseGameObject* SlapLockWhirlWind::VDestructor(signed int flags)
{
    return vdtor_43DA40(flags);
}

void SlapLockWhirlWind::VUpdate()
{
    vUpdate_43DA90();
}

int SlapLockWhirlWind::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_43DC50(reinterpret_cast<SlapLockWhirlWind_State*>(pSaveBuffer));
}

void SlapLockWhirlWind::dtor_43DA70()
{
    SetVTable(this, 0x545208);
    BaseGameObject_dtor_4DBEC0();
}

SlapLockWhirlWind* SlapLockWhirlWind::vdtor_43DA40(signed int flags)
{
    dtor_43DA70();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

signed int SlapLockWhirlWind::vGetSaveState_43DC50(SlapLockWhirlWind_State* pSaveState)
{
    pSaveState->field_0_type = Types::eSlapLock_OrbWhirlWind_60;
    pSaveState->field_2_switch_id = field_44_switch_id;
    return sizeof(SlapLockWhirlWind_State);
}

void SlapLockWhirlWind::vUpdate_43DA90()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else
    {
        OrbWhirlWind* pWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds_5C1B70.Find_449CF0(field_38_orb_whirlwind_id));
        if (field_3C_state == 1)
        {
            if (!(static_cast<int>(sGnFrame_5C1B84) % 10))
            {
                SFX_Play_46FBA0(
                    SoundEffect::FlyingSpirit2_108,
                    static_cast<short>(127 - (static_cast<int>(sGnFrame_5C1B84) - field_40_timer) / 2),
                    4 * (sGnFrame_5C1B84 - field_40_timer));
            }

            if (!pWhirlWind || pWhirlWind->field_6_flags.Get(BaseGameObject::eDead_Bit3))
            {
                SwitchStates_Do_Operation_465F00(field_44_switch_id, SwitchOp::eSetTrue_0);
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
        }
        else if (field_3C_state == 0)
        {
            if (!(static_cast<int>(sGnFrame_5C1B84) % 10))
            {
                if (static_cast<int>(sGnFrame_5C1B84) % 20)
                {
                    SFX_Play_46FA90(SoundEffect::FlyingSpirit1_107, 0);
                }
                else
                {
                    SFX_Play_46FA90(SoundEffect::FlyingSpirit2_108, 0);
                }
            }

            if (static_cast<int>(sGnFrame_5C1B84) > field_40_timer)
            {
                if (pWhirlWind)
                {
                    pWhirlWind->ToSpin_4E3FD0(field_2C_door_x, field_30_door_y, field_34_door_scale, 0);
                }
                field_3C_state = 1;
            }
        }
    }
}

__int16 SlapLockWhirlWind::SwitchId() const
{
    return field_44_switch_id;
}
