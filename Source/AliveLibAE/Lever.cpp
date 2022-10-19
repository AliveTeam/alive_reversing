#include "stdafx.h"
#include "Lever.hpp"
#include "Function.hpp"
#include "Collisions.hpp"
#include "BaseAliveGameObject.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "PathData.hpp"
#include "SwitchStates.hpp"
#include "Grid.hpp"

const TintEntry kLeverTints_563228[18] = {
    {LevelIds_s8::eMines_1, 127u, 127u, 127u},
    {LevelIds_s8::eNecrum_2, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_3, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_4, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_5, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_6, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_Ender_7, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_8, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_9, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_Ender_10, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_Ender_11, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_Ender_12, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_Ender_13, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_Ender_14, 127u, 127u, 127u},
    {LevelIds_s8::eNone, 127u, 127u, 127u}};

BaseGameObject* Lever::VDestructor(s32 flags)
{
    return vdtor_4D5AD0(flags);
}

void Lever::VUpdate()
{
    vUpdate_4D5C00();
}

void Lever::VScreenChanged()
{
    vScreenChanged_4D5B90();
}

s16 Lever::VPull_4D6050(s16 bLeftDirection)
{
    return vPull_4D6050(bLeftDirection);
}

Lever* Lever::ctor_4D5860(Path_Lever* pTlv, u32 tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x547A5C);

    SetType(AETypes::eLever_139);
    const AnimRecord& rec = AnimRec(AnimId::Lever_Idle);
    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_F4_switch_id = pTlv->field_1A_switch_id;
    field_102_action = pTlv->field_10_action;
    field_100_flags.Clear(Flags_100::eBit1_lever_anim_left_direction);

    if (pTlv->field_1C_persist_offscreen == Choice_short::eYes_1)
    {
        field_100_flags.Set(Flags_100::eBit2_persist_offscreen);
    }
    else
    {
        field_100_flags.Clear(Flags_100::eBit2_persist_offscreen);
    }

    if (pTlv->field_12_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = Layer::eLayer_BeforeShadow_Half_6;
        field_D6_scale = 0;
    }
    else if (pTlv->field_12_scale == Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = Layer::eLayer_BeforeShadow_25;
        field_D6_scale = 1;
    }

    SetTint_425600(&kLeverTints_563228[0], gMap_5C3030.field_0_current_level);
    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_B8_xpos = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos + FP_FromInteger(24),
            &pPathLine,
            &hitX,
            &hitY,
            (field_D6_scale != 0) ? 1 : 16))
    {
        field_BC_ypos = hitY;
    }

    field_104_on_sound = pTlv->field_14_on_sound;
    field_106_off_sound = pTlv->field_16_off_sound;
    field_FC_tlvInfo = tlvInfo;
    field_108_sound_direction = pTlv->field_18_sound_direction;

    field_F8_state = LeverState::eWaiting_0;
    field_DC_bApplyShadows |= 2u;

    return this;
}

void Lever::dtor_4D5B00()
{
    SetVTable(this, 0x547A5C); // vTbl_Switch_547A5C
    Path::TLV_Reset_4DB8E0(field_FC_tlvInfo, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    //Lever::dtor_4D5840(); // Omitted interface base nop.
}

Lever* Lever::vdtor_4D5AD0(s32 flags)
{
    dtor_4D5B00();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Lever::vScreenChanged_4D5B90()
{
    if (!field_100_flags.Get(Flags_100::eBit2_persist_offscreen) || gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level || gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Lever::vUpdate_4D5C00()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_F8_state == LeverState::ePulled_1)
    {
        if (field_20_animation.field_92_current_frame == 3)
        {
            SFX_Play_46FA90(SoundEffect::LeverPull_63, 0);
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (gMap_5C3030.field_0_current_level == LevelIds::eMines_1
                || gMap_5C3030.field_0_current_level == LevelIds::eBonewerkz_8
                || gMap_5C3030.field_0_current_level == LevelIds::eBonewerkz_Ender_14
                || gMap_5C3030.field_0_current_level == LevelIds::eFeeCoDepot_5
                || gMap_5C3030.field_0_current_level == LevelIds::eFeeCoDepot_Ender_12
                || gMap_5C3030.field_0_current_level == LevelIds::eBarracks_6
                || gMap_5C3030.field_0_current_level == LevelIds::eBarracks_Ender_13
                || gMap_5C3030.field_0_current_level == LevelIds::eBrewery_9
                || gMap_5C3030.field_0_current_level == LevelIds::eBrewery_Ender_10)
            {
                SFX_Play_46FBA0(SoundEffect::IndustrialTrigger_80, 30, 400);
            }

            field_F8_state = LeverState::eFinished_2;

            if (field_100_flags.Get(Flags_100::eBit1_lever_anim_left_direction))
            {
                const AnimRecord& animRec = AnimRec(AnimId::Lever_Pull_Release_Left);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            }
            else
            {
                const AnimRecord& animRec = AnimRec(AnimId::Lever_Pull_Release_Right);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            }

            const s32 switch_state = SwitchStates_Get_466020(field_F4_switch_id);
            SwitchStates_Do_Operation_465F00(field_F4_switch_id, field_102_action);
            const s32 switch_state_after_op = SwitchStates_Get_466020(field_F4_switch_id);
            if (switch_state != switch_state_after_op)
            {
                s32 volLeft = 0;
                s32 volRight = 0;

                if (field_108_sound_direction == LeverSoundDirection::eLeft_1)
                {
                    volLeft = 1;
                    volRight = 0;
                }
                else if (field_108_sound_direction == LeverSoundDirection::eRight_2)
                {
                    volLeft = 0;
                    volRight = 1;
                }
                else
                {
                    volLeft = 1;
                    volRight = 1;
                }

                if (switch_state_after_op)
                {
                    switch (field_104_on_sound)
                    {
                        case LeverSoundType::eWell_1:
                            SFX_Play_46FB10(SoundEffect::WellExit_20, 80 * volLeft + 25, 80 * volRight + 25);
                            Event_Broadcast_422BC0(kEventNoise, this);
                            Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eUnknown_2:
                            SFX_Play_46FB10(SoundEffect::SwitchUnknownTrigger_11, 100 * volLeft + 25, 100 * volRight + 25);
                            Event_Broadcast_422BC0(kEventNoise, this);
                            Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eDoor_3:
                            SFX_Play_46FB10(SoundEffect::DoorEffect_57, 75 * volLeft + 15, 75 * volRight + 15);
                            Event_Broadcast_422BC0(kEventNoise, this);
                            Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eElectricWall_4:
                            SFX_Play_46FB10(SoundEffect::Zap1_49, 35 * volLeft + 25, 35 * volRight + 25);
                            Event_Broadcast_422BC0(kEventNoise, this);
                            Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eSecurityOrb_5:
                            SFX_Play_46FB10(SoundEffect::SecurityOrb_48, 35 * volLeft + 25, 35 * volRight + 25);
                            return;

                        case LeverSoundType::eLift_6:
                            SFX_Play_46FB10(SoundEffect::LiftStop_30, 35 * volLeft + 25, 35 * volRight + 25);
                            Event_Broadcast_422BC0(kEventNoise, this);
                            Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                            break;

                        default:
                            return;
                    }
                }
                else
                {
                    switch (field_106_off_sound)
                    {
                        case LeverSoundType::eWell_1:
                            SFX_Play_46FB10(SoundEffect::WellExit_20, 80 * volLeft + 25, 80 * volRight + 25);
                            Event_Broadcast_422BC0(kEventNoise, this);
                            Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eUnknown_2:
                            SFX_Play_46FB10(SoundEffect::SwitchUnknownTrigger_11, 110 * volLeft + 25, 110 * volRight + 25);
                            Event_Broadcast_422BC0(kEventNoise, this);
                            Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eDoor_3:
                            SFX_Play_46FB10(SoundEffect::DoorEffect_57, 75 * volLeft + 15, 75 * volRight + 15);
                            Event_Broadcast_422BC0(kEventNoise, this);
                            Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eElectricWall_4:
                            SFX_Play_46FB10(SoundEffect::Zap1_49, 80 * volLeft + 25, 80 * volRight + 25);
                            Event_Broadcast_422BC0(kEventNoise, this);
                            Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eSecurityOrb_5:
                            SFX_Play_46FB10(SoundEffect::SecurityOrb_48, 35 * volLeft + 75, 35 * volRight + 75);
                            break;

                        case LeverSoundType::eLift_6:
                            SFX_Play_46FB10(SoundEffect::LiftStop_30, 35 * volLeft + 25, 35 * volRight + 25);
                            Event_Broadcast_422BC0(kEventNoise, this);
                            Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                            break;

                        default:
                            return;
                    }
                }
            }
        }
    }
    else if (field_F8_state == LeverState::eFinished_2)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            field_F8_state = LeverState::eWaiting_0;
            const AnimRecord& animRec = AnimRec(AnimId::Lever_Idle);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
        }
    }
}

s16 Lever::vPull_4D6050(s16 bLeftDirection)
{
    if (field_F8_state != LeverState::eWaiting_0)
    {
        return 0;
    }

    field_F8_state = LeverState::ePulled_1;

    if (bLeftDirection)
    {
        const AnimRecord& animRec = AnimRec(AnimId::Lever_Pull_Left);
        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
        field_100_flags.Set(Flags_100::eBit1_lever_anim_left_direction);
    }
    else
    {
        const AnimRecord& animRec = AnimRec(AnimId::Lever_Pull_Right);
        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
        field_100_flags.Clear(Flags_100::eBit1_lever_anim_left_direction);
    }

    return 1;
}
