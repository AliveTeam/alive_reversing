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
#include "AbilityRing.hpp"
#include "PossessionFlicker.hpp"
#include "Particle.hpp"
#include "ParticleBurst.hpp"

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

    field_120_state = SlapLockStates::State_0;
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
        field_120_state = SlapLockStates::State_4;
    }
    else
    {
        field_120_state = SlapLockStates::State_3;
    }

    return this;
}

void SlapLock::dtor_43DF00()
{
    SetVTable(this, 0x545224);
    Path::TLV_Reset_4DB8E0(field_11C_tlvInfo, -1, 0, 0);
    dtor_4080B0();
}

void SlapLock::VUpdate()
{
    vUpdate_43DF90();
}

BaseGameObject* SlapLock::VDestructor(signed int flags)
{
    return vdtor_43DED0(flags);
}

void SlapLock::VScreenChanged()
{
    vScreenChanged_43E840();
}

int SlapLock::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_43EB30(reinterpret_cast<SlapLock_State*>(pSaveBuffer));
}

__int16 SlapLock::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_43E5D0(pFrom);
}

int CC SlapLock::CreateFromSaveState_43EA00(const BYTE* pBuffer)
{
    auto pState = reinterpret_cast<const SlapLock_State*>(pBuffer);

    auto pTlv = static_cast<Path_SlapLock*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_4_tlvInfo));
  
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 1053, FALSE, FALSE)) // TODO: Id
    {
        ResourceManager::LoadResourceFile_49C170("GHOSTTRP.BAN", nullptr);
    }

    auto pSlapLock = alive_new<SlapLock>();
    if (pSlapLock)
    {
        pSlapLock->ctor_43DC80(pTlv, pState->field_4_tlvInfo);
    }

    pSlapLock->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_2 & 1);

    pSlapLock->field_11C_tlvInfo = pState->field_4_tlvInfo;

    pTlv->field_1_unknown = pState->field_8_tlv_state;

    pSlapLock->field_120_state = pState->field_A_state;
    pSlapLock->field_124_timer1 = pState->field_C_timer1;
    pSlapLock->field_114_flags.Set(Flags_114::e114_Bit1_bShot);
    pSlapLock->field_13C_timer2 = pState->field_14_timer2;
    return sizeof(SlapLock_State);
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
    if (field_120_state == SlapLockStates::State_5 || field_120_state == SlapLockStates::State_6)
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

signed int SlapLock::vGetSaveState_43EB30(SlapLock_State* pState)
{
    pState->field_0_type = Types::eLockedSoul_61;
    pState->field_2 = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render) & 1;
    pState->field_4_tlvInfo = field_11C_tlvInfo;
    pState->field_8_tlv_state = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_11C_tlvInfo)->field_1_unknown;
    pState->field_A_state = field_120_state;
    pState->field_C_timer1 = field_124_timer1;
    pState->field_14_timer2 = field_13C_timer2;
    pState->field_10_obj_id = -1;

    if (field_134_id == -1)
    {
        return sizeof(SlapLock_State);
    }

    BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_134_id);
    if (pObj)
    {
        pState->field_10_obj_id = pObj->field_C_objectId;
    }
    return sizeof(SlapLock_State);
}

void SlapLock::vUpdate_43DF90()
{
    field_118_pTlv = static_cast<Path_SlapLock*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_11C_tlvInfo));

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (field_114_flags.Get(Flags_114::e114_Bit9))
        {
            field_114_flags.Clear(Flags_114::e114_Bit9);

            if (field_118_pTlv->field_1_unknown)
            {
                SwitchStates_Do_Operation_465F00(field_118_pTlv->field_14_target_tomb_id2, SwitchOp::eSetTrue_0);
            }

            if (field_134_id != -1)
            {
                for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
                {
                    BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->field_4_typeId == Types::eType_104 && pObj->field_C_objectId == field_134_id)
                    {
                        field_134_id = pObj->field_8_object_id;
                        break;
                    }
                }
            }
        }

        BaseGameObject* pRingObj = sObjectIds_5C1B70.Find_449CF0(field_134_id);
        BaseGameObject* pFlickerObj = sObjectIds_5C1B70.Find_449CF0(field_138_possesion_flicker_id);

        switch (field_120_state)
        {
        case SlapLockStates::State_0:
            if (field_118_pTlv->field_1A_has_powerup)
            {
                if (!(sGnFrame_5C1B84 & 63))
                {
                    AbilityRing::Factory_482F80(
                        field_B8_xpos,
                        field_BC_ypos - (FP_FromInteger(40) * field_CC_sprite_scale),
                        RingTypes::eInvisible_Pulse_Large_8,
                        field_CC_sprite_scale);
                }
            }

            if (static_cast<int>(sGnFrame_5C1B84) <= field_124_timer1)
            {
                return;
            }

            if (!field_130_has_ghost)
            {
                return;
            }

            field_20_animation.Set_Animation_Data_409C80(6976, 0);
            field_120_state = SlapLockStates::State_1;
            SFX_Play_46FA90(0x69u, 0);
            return;

        case SlapLockStates::State_1:
            if (field_118_pTlv->field_1A_has_powerup)
            {
                if (!(sGnFrame_5C1B84 & 63))
                {
                    AbilityRing::Factory_482F80(
                        field_B8_xpos,
                        field_BC_ypos - (FP_FromInteger(40) * field_CC_sprite_scale),
                        RingTypes::eInvisible_Pulse_Large_8,
                        field_CC_sprite_scale);
                }
            }

            if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return;
            }

            field_20_animation.Set_Animation_Data_409C80(7068, 0);
            field_120_state = SlapLockStates::State_0;
            field_124_timer1 = Math_NextRandom() + sGnFrame_5C1B84 + 25;
            return;

        case SlapLockStates::State_2:
            if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return;
            }

            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

            if (!field_118_pTlv->field_1A_has_powerup)
            {
                field_13C_timer2 = sGnFrame_5C1B84 + 60;
                field_120_state = SlapLockStates::State_3;
                break;
            }
            else
            {
                field_120_state = SlapLockStates::State_5;
                return;
            }
            break;

        case SlapLockStates::State_3:
            if (static_cast<int>(sGnFrame_5C1B84) <= field_13C_timer2)
            {
                return;
            }

            New_Particle_426C30(
                (field_CC_sprite_scale * (FP_FromInteger(Math_RandomRange_496AB0(-2, 2)) + FP_FromInteger(1))) + field_B8_xpos,
                (field_CC_sprite_scale * (FP_FromInteger(Math_RandomRange_496AB0(-3, 3)) - FP_FromInteger(33))) + field_BC_ypos,
                FP_FromDouble(0.3),
                field_20_animation.field_C_render_layer);

            field_13C_timer2 = Math_RandomRange_496AB0(-30, 30) + sGnFrame_5C1B84 + 60;
            return;

        case SlapLockStates::State_4:
            if (static_cast<int>(sGnFrame_5C1B84) > field_124_timer1)
            {
                if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                    sActiveHero_5C1B68->field_C2_lvl_number,
                    sActiveHero_5C1B68->field_C0_path_number,
                    sActiveHero_5C1B68->field_B8_xpos,
                    sActiveHero_5C1B68->field_BC_ypos,
                    1)
                    || sActiveHero_5C1B68->field_168_ring_pulse_timer
                    || sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
                {
                    AbilityRing::Factory_482F80(
                        field_B8_xpos,
                        field_BC_ypos - (FP_FromInteger(40) * field_CC_sprite_scale),
                        RingTypes::eInvisible_Pulse_Large_8,
                        field_CC_sprite_scale);
                    field_124_timer1 = Math_RandomRange_496AB0(1, 10) + sGnFrame_5C1B84 + 55;
                }
                else
                {
                    GivePowerUp_43E910();
                    field_120_state = SlapLockStates::State_5;
                }
            }

            if (static_cast<int>(sGnFrame_5C1B84) <= field_13C_timer2)
            {
                return;
            }

            New_Particle_426C30(
                (field_CC_sprite_scale * (FP_FromInteger(Math_RandomRange_496AB0(-2, 2)) + FP_FromInteger(1)))  + field_B8_xpos,
                (field_CC_sprite_scale * (FP_FromInteger(Math_RandomRange_496AB0(-3, 3)) - FP_FromInteger(33))) + field_BC_ypos,
                FP_FromDouble(0.3),
                field_20_animation.field_C_render_layer);

            field_13C_timer2 = Math_RandomRange_496AB0(-30, 30) + sGnFrame_5C1B84 + 60;
            return;

        case SlapLockStates::State_5:
            if (pRingObj)
            {
                return;
            }

            if (sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
            {
                field_120_state = SlapLockStates::State_7;
            }
            else
            {
                auto pFlicker = alive_new<PossessionFlicker>();
                if (pFlicker)
                {
                    pFlicker->ctor_4319E0(sActiveHero_5C1B68, 8, 128, 255, 128);
                }
                field_120_state = SlapLockStates::State_6;
                field_138_possesion_flicker_id = pFlicker->field_8_object_id;
            }
            return;

        case SlapLockStates::State_6:
            if (pFlickerObj)
            {
                return;
            }
            GiveInvisiblity_43E880();
            field_13C_timer2 = sGnFrame_5C1B84 + 60;
            field_120_state = SlapLockStates::State_3;
            break;

        case SlapLockStates::State_7:
            GiveInvisiblity_43E880();
            field_13C_timer2 = sGnFrame_5C1B84 + 60;
            field_120_state = SlapLockStates::State_3;
            break;

        default:
            return;
        }
    }
}

void SlapLock::GivePowerUp_43E910()
{
    AbilityRing::Factory_482F80(
        field_B8_xpos,
        field_BC_ypos - (FP_FromInteger(40) * field_CC_sprite_scale),
        RingTypes::eInvisible_Pulse_Emit_9,
        field_CC_sprite_scale);

    PSX_RECT bRect = {};
    sActiveHero_5C1B68->vGetBoundingRect_424FD0(&bRect, 1);

    AbilityRing* pRing = AbilityRing::Factory_482F80(
        FP_FromInteger((bRect.x + bRect.w) / 2),
        FP_FromInteger((bRect.y + bRect.h) / 2),
        RingTypes::eInvisible_Pulse_Give_10,
        sActiveHero_5C1B68->field_CC_sprite_scale);

    pRing->field_C_objectId = field_C_objectId;
    field_134_id = pRing->field_8_object_id;

    pRing->VSetTarget(sActiveHero_5C1B68);
}

__int16 SlapLock::vTakeDamage_43E5D0(BaseGameObject* pFrom)
{
    field_118_pTlv = static_cast<Path_SlapLock*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_11C_tlvInfo));

    if (pFrom->field_4_typeId != Types::eAbe_69)
    {
        // Only Abe can slap me up
        return 0;
    }

    if (sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_62_Punch_454750)
    {
        // If Abe isn't slapping then he can't hurt me
        return 0;
    }

    if (field_120_state != SlapLockStates::State_0 && field_120_state != SlapLockStates::State_1)
    {
        return 0;
    }

    if (field_6_flags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    sActiveHero_5C1B68->ToKnockback_44E700(1, 0);

    if (field_130_has_ghost)
    {
        field_130_has_ghost = 0;
        auto pSlapWhirlWind = alive_new<SlapLockWhirlWind>();
        if (pSlapWhirlWind)
        {
            pSlapWhirlWind->ctor_43D7E0(
                field_118_pTlv->field_12_target_tomb_id1,
                field_118_pTlv->field_14_target_tomb_id2,
                field_B8_xpos,
                field_BC_ypos - (FP_FromInteger(40) * field_CC_sprite_scale),
                field_CC_sprite_scale);
        }
    }

    if (field_118_pTlv->field_1A_has_powerup)
    {
        GivePowerUp_43E910();
    }

    field_120_state = SlapLockStates::State_2;
    SwitchStates_Do_Operation_465F00(field_118_pTlv->field_1E_option_id, SwitchOp::eToggle_2);
    SFX_Play_46FA90(106u, 0, field_CC_sprite_scale);
    Event_Broadcast_422BC0(kEventLoudNoise, this);

    auto pParticleBurst = alive_new<ParticleBurst>();
    if (pParticleBurst)
    {
        pParticleBurst->ctor_41CF50(
            field_B8_xpos,
            field_BC_ypos - (FP_FromInteger(40) * field_CC_sprite_scale),
            15,
            field_CC_sprite_scale,
            BurstType::eGreenSparks_5,
            11);
    }

    field_20_animation.Set_Animation_Data_409C80(7056, 0);

    field_118_pTlv->field_1_unknown = 1;
    return 1;
}
