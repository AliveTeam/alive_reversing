#include "stdafx.h"
#include "SlapLock.hpp"
#include "Function.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "OrbWhirlWind.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"

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
        auto pWhirlWind = alive_new<OrbWhirlWind>();
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
        field_6_flags.Set(BaseGameObject::eDead);
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
        Mem_Free_495540(this);
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
        field_6_flags.Set(BaseGameObject::eDead);
    }
    else
    {
        OrbWhirlWind* pWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds_5C1B70.Find_449CF0(field_38_orb_whirlwind_id));
        if (field_3C_state == 1)
        {
            if (!(static_cast<int>(sGnFrame_5C1B84) % 10))
            {
                SFX_Play_46FBA0(
                    108u,
                    static_cast<short>(127 - (static_cast<int>(sGnFrame_5C1B84) - field_40_timer) / 2),
                    4 * (sGnFrame_5C1B84 - field_40_timer));
            }

            if (!pWhirlWind || pWhirlWind->field_6_flags.Get(BaseGameObject::eDead))
            {
                SwitchStates_Do_Operation_465F00(field_44_switch_id, SwitchOp::eSetTrue_0);
                field_6_flags.Set(BaseGameObject::eDead);
            }
        }
        else if (field_3C_state == 0)
        {
            if (!(static_cast<int>(sGnFrame_5C1B84) % 10))
            {
                if (static_cast<int>(sGnFrame_5C1B84) % 20)
                {
                    SFX_Play_46FA90(0x6Bu, 0);
                }
                else
                {
                    SFX_Play_46FA90(0x6Cu, 0);
                }
            }

            if (static_cast<int>(sGnFrame_5C1B84) > field_40_timer)
            {
                if (pWhirlWind)
                {
                    pWhirlWind->sub_4E3FD0(field_2C_door_x, field_30_door_y, field_34_door_scale, 0);
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

SlapLock* SlapLock::ctor_43DC80(Path_SlapLock* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x545224);

    field_4_typeId = Types::eLockedSoul_61;
    field_118_pTlv = pTlv;
    field_11C_tlvInfo = tlvInfo;
    field_C_objectId = tlvInfo;

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 1053); // TODO: Id
    Animation_Init_424E10(7068, 58, 44, ppRes, 1, 1);

    if (field_118_pTlv->field_10_scale)
    {
        field_20_animation.field_C_render_layer = 6;
    }
    else
    {
        field_20_animation.field_C_render_layer = 25;
    }

    field_120_state = 0;
    field_124_timer1 = (Math_NextRandom() & 7) + sGnFrame_5C1B84 + 25;
    field_134_id = -1;
    field_138_possesion_flicker_id = -1;
    const FP midX = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_128_midX = midX;
    field_B8_xpos = midX;

    const FP ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    field_12C_midY = ypos;
    field_BC_ypos = ypos;

    field_130_has_ghost = field_118_pTlv->field_18_has_ghost;

    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eSlapLock_OrbWhirlWind_60 && static_cast<SlapLockWhirlWind*>(pObj)->SwitchId() == field_118_pTlv->field_14_target_tomb_id2)
        {
            field_130_has_ghost = 0;
        }
    }

    if (SwitchStates_Get_466020(pTlv->field_14_target_tomb_id2))
    {
        field_130_has_ghost = 0;
    }

    if (pTlv->field_1_unknown == 0)
    {
        return this;
    }

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_124_timer1 = sGnFrame_5C1B84 + 60;
    field_13C_timer2 = sGnFrame_5C1B84 + 30;

    if (field_118_pTlv->field_1A_has_powerup != 0)
    {
        field_120_state = 4;
    }
    else
    {
        field_120_state = 3;
    }

    return this;
}

void SlapLock::dtor_43DF00()
{
    SetVTable(this, 0x545224);
    Path::TLV_Reset_4DB8E0(field_11C_tlvInfo, -1, 0, 0);
    dtor_4080B0();
}

SlapLock* SlapLock::vdtor_43DED0(signed int flags)
{
    dtor_43DF00();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void SlapLock::vScreenChanged_43E840()
{
    if (field_120_state == 5 || field_120_state == 6)
    {
        GiveInvisiblity_43E880();
    }
    field_6_flags.Set(BaseGameObject::eDead);
}

void SlapLock::GiveInvisiblity_43E880()
{
    field_118_pTlv = static_cast<Path_SlapLock*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_11C_tlvInfo));
    if (sActiveHero_5C1B68)
    {
        sActiveHero_5C1B68->field_176 = field_118_pTlv->field_1C_powerup_id;
        sActiveHero_5C1B68->field_16C_bHaveShrykull = 0;
        sActiveHero_5C1B68->field_16E_bHaveInvisiblity = 1;
        sActiveHero_5C1B68->field_168_ring_pulse_timer = sGnFrame_5C1B84 + 200000;
    }
}
