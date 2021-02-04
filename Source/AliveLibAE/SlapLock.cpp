#include "stdafx.h"
#include "SlapLock.hpp"
#include "SlapLockWhirlWind.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "AbilityRing.hpp"
#include "PossessionFlicker.hpp"
#include "Particle.hpp"
#include "ParticleBurst.hpp"

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

    const AnimRecord& rec = AnimRec(AnimId::Slap_Lock_Idle_A);
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    if (field_118_pTlv->field_10_scale)
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_6;
    }
    else
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_25;
    }

    field_120_state = SlapLockStates::eShaking_0;
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
        field_120_state = SlapLockStates::eEmitPowerupRing_4;
    }
    else
    {
        field_120_state = SlapLockStates::eBroken_3;
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
  
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kUnknownResID_1053, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("GHOSTTRP.BAN", nullptr);
    }

    auto pSlapLock = ae_new<SlapLock>();
    if (pSlapLock)
    {
        pSlapLock->ctor_43DC80(pTlv, pState->field_4_tlvInfo);
    }

    pSlapLock->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_2_render & 1);

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
        ae_delete_free_495540(this);
    }
    return this;
}

void SlapLock::vScreenChanged_43E840()
{
    if (field_120_state == SlapLockStates::eFlickerHero_5 || field_120_state == SlapLockStates::eGiveInvisibilityFromFlicker_6)
    {
        GiveInvisibility_43E880();
    }
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void SlapLock::GiveInvisibility_43E880()
{
    field_118_pTlv = static_cast<Path_SlapLock*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_11C_tlvInfo));
    if (sActiveHero_5C1B68)
    {
        sActiveHero_5C1B68->field_176_invisibility_id = field_118_pTlv->field_1C_powerup_id;
        sActiveHero_5C1B68->field_16C_bHaveShrykull = 0;
        sActiveHero_5C1B68->field_16E_bHaveInvisiblity = 1;
        sActiveHero_5C1B68->field_168_ring_pulse_timer = sGnFrame_5C1B84 + 200000;
    }
}

signed int SlapLock::vGetSaveState_43EB30(SlapLock_State* pState)
{
    pState->field_0_type = Types::eLockedSoul_61;
    pState->field_2_render = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render) & 1;
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
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else
    {
        if (field_114_flags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
        {
            field_114_flags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);

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

                    if (pObj->field_4_typeId == Types::eAbilityRing_104 && pObj->field_C_objectId == field_134_id)
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
        case SlapLockStates::eShaking_0:
        {
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

			const AnimRecord& animRec = AnimRec(AnimId::Slap_Lock_Shake);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            
			field_120_state = SlapLockStates::eIdle_1;
            SFX_Play_46FA90(SoundEffect::SpiritLockShake_105, 0);
            return;
        }
        case SlapLockStates::eIdle_1:
        {
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

            const AnimRecord& animRec = AnimRec(AnimId::Slap_Lock_Idle_A);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);

            field_120_state = SlapLockStates::eShaking_0;
            field_124_timer1 = Math_NextRandom() + sGnFrame_5C1B84 + 25;
            return;
        }
        case SlapLockStates::eSlapped_2:
        {
            if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return;
            }

            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

            if (!field_118_pTlv->field_1A_has_powerup)
            {
                field_13C_timer2 = sGnFrame_5C1B84 + 60;
                field_120_state = SlapLockStates::eBroken_3;
                break;
            }
            else
            {
                field_120_state = SlapLockStates::eFlickerHero_5;
                return;
            }
            break;
        }
        case SlapLockStates::eBroken_3:
        {
            if (static_cast<int>(sGnFrame_5C1B84) <= field_13C_timer2)
            {
                return;
            }

            New_TintShiny_Particle_426C30(
                (field_CC_sprite_scale * (FP_FromInteger(Math_RandomRange_496AB0(-2, 2)) + FP_FromInteger(1))) + field_B8_xpos,
                (field_CC_sprite_scale * (FP_FromInteger(Math_RandomRange_496AB0(-3, 3)) - FP_FromInteger(33))) + field_BC_ypos,
                FP_FromDouble(0.3),
                field_20_animation.field_C_render_layer);

            field_13C_timer2 = Math_RandomRange_496AB0(-30, 30) + sGnFrame_5C1B84 + 60;
            return;
        }
        case SlapLockStates::eEmitPowerupRing_4:
        {
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
                    field_120_state = SlapLockStates::eFlickerHero_5;
                }
            }

            if (static_cast<int>(sGnFrame_5C1B84) <= field_13C_timer2)
            {
                return;
            }

            New_TintShiny_Particle_426C30(
                (field_CC_sprite_scale * (FP_FromInteger(Math_RandomRange_496AB0(-2, 2)) + FP_FromInteger(1)))  + field_B8_xpos,
                (field_CC_sprite_scale * (FP_FromInteger(Math_RandomRange_496AB0(-3, 3)) - FP_FromInteger(33))) + field_BC_ypos,
                FP_FromDouble(0.3),
                field_20_animation.field_C_render_layer);

            field_13C_timer2 = Math_RandomRange_496AB0(-30, 30) + sGnFrame_5C1B84 + 60;
            return;
        }
        case SlapLockStates::eFlickerHero_5:
        {
            if (pRingObj)
            {
                return;
            }

            if (sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
            {
                field_120_state = SlapLockStates::eGiveInvisibility_7;
            }
            else
            {
                auto pFlicker = ae_new<PossessionFlicker>();
                if (pFlicker)
                {
                    pFlicker->ctor_4319E0(sActiveHero_5C1B68, 8, 128, 255, 128);
                }
                field_120_state = SlapLockStates::eGiveInvisibilityFromFlicker_6;
                field_138_possesion_flicker_id = pFlicker->field_8_object_id;
            }
            return;
        }
        case SlapLockStates::eGiveInvisibilityFromFlicker_6:
        {
            if (pFlickerObj)
            {
                return;
            }
            GiveInvisibility_43E880();
            field_13C_timer2 = sGnFrame_5C1B84 + 60;
            field_120_state = SlapLockStates::eBroken_3;
            break;
        }
        case SlapLockStates::eGiveInvisibility_7:
        {
            GiveInvisibility_43E880();
            field_13C_timer2 = sGnFrame_5C1B84 + 60;
            field_120_state = SlapLockStates::eBroken_3;
            break;
        }
        default:
        {
            return;
        }
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

    if (field_120_state != SlapLockStates::eShaking_0 && field_120_state != SlapLockStates::eIdle_1)
    {
        return 0;
    }

    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        return 0;
    }

    sActiveHero_5C1B68->ToKnockback_44E700(1, 0);

    if (field_130_has_ghost)
    {
        field_130_has_ghost = 0;
        auto pSlapWhirlWind = ae_new<SlapLockWhirlWind>();
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

    field_120_state = SlapLockStates::eSlapped_2;
    SwitchStates_Do_Operation_465F00(field_118_pTlv->field_1E_option_id, SwitchOp::eToggle_2);
    SFX_Play_46FA90(SoundEffect::SpiritLockBreak_106, 0, field_CC_sprite_scale);
    Event_Broadcast_422BC0(kEventLoudNoise, this);

    auto pParticleBurst = ae_new<ParticleBurst>();
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

    const AnimRecord& animRec = AnimRec(AnimId::Slap_Lock_Idle_B);
    field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);

    field_118_pTlv->field_1_unknown = 1;
    return 1;
}
