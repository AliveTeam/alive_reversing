#include "stdafx.h"
#include "Drill.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Sfx.hpp"
#include "SwitchStates.hpp"
#include "Sound/Midi.hpp"
#include "Abe.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "Spark.hpp"
#include "Blood.hpp"

const TintEntry kDrillTints_551548[18] = {
    {LevelIds_s8::eMines_1, 127u, 127u, 127u},
    {LevelIds_s8::eNecrum_2, 137u, 137u, 137u},
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


Drill* Drill::ctor_4200D0(Path_Drill* pTlv, u32 tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x544AD8);

    SetType(AETypes::eDrill_30);

    const AnimRecord& rec = AnimRec(AnimId::Drill_Vertical_Off);
    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.field_B_render_mode = TPageAbr::eBlend_0;

    SetTint_425600(kDrillTints_551548, gMap_5C3030.field_0_current_level);
    Path_Drill_Data tlvData = pTlv->field_10_data;

    field_128_flags.Clear(Flags::eBit2_ToggleStartState_StartOn);
    field_128_flags.Clear(Flags::eBit5_SpeedChanged);
    field_128_flags.Clear(Flags::eBit4_Toggle);

    if (tlvData.field_1C_bStart_state_on == Choice_short::eYes_1)
    {
        field_128_flags.Clear(Flags::eBit1_StartOff);
    }
    else
    {
        field_128_flags.Set(Flags::eBit1_StartOff);
    }

    field_F8_id = tlvData.field_16_id;

    if (SwitchStates_Get_466020(field_F8_id) && field_128_flags.Get(Flags::eBit1_StartOff))
    {
        field_128_flags.Set(Flags::eBit2_ToggleStartState_StartOn);
    }

    if (tlvData.field_10_scale == Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = Layer::eLayer_RopeWebDrill_24;
        field_D6_scale = 1;
    }
    else
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = Layer::eLayer_RopeWebDrill_Half_5;
        field_D6_scale = 0;
    }

    field_FA_direction = tlvData.field_26_direction;
    if (tlvData.field_24_bStart_position_bottom == Choice_short::eYes_1)
    {
        field_128_flags.Set(Flags::eBit6_StartPosIsBottom);
    }
    else
    {
        field_128_flags.Clear(Flags::eBit6_StartPosIsBottom);
    }

    if (field_128_flags.Get(Flags::eBit2_ToggleStartState_StartOn))
    {
        if (field_128_flags.Get(Flags::eBit6_StartPosIsBottom))
        {
            field_F4_state = DrillStates::State_2_GoingUp;
        }
        else
        {
            field_F4_state = DrillStates::State_1_Going_Down;
        }

        const CameraPos direction = gMap_5C3030.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos);
        field_10C_audio_channels_mask = SFX_Play_46FC20(SoundEffect::DrillMovement_97, 25, direction);
    }

    switch (field_FA_direction)
    {
        case DrillDirection::eDown_0:
            field_110_xPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x + 12);
            field_B8_xpos = field_110_xPos;
            field_114_yPos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);

            if (field_128_flags.Get(Flags::eBit2_ToggleStartState_StartOn))
            {
                const AnimRecord& animRec = AnimRec(AnimId::Drill_Vertical_On);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            }
            else
            {
                const AnimRecord& animRec = AnimRec(AnimId::Drill_Vertical_Off);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            }

            field_F6_width = pTlv->field_C_bottom_right.field_2_y - pTlv->field_8_top_left.field_2_y;
            if (field_128_flags.Get(Flags::eBit6_StartPosIsBottom))
            {
                field_124_xyoff = FP_FromInteger(0);
            }
            else
            {
                field_124_xyoff = FP_FromInteger(field_F6_width);
            }
            field_BC_ypos = field_114_yPos - field_124_xyoff;
            break;

        case DrillDirection::eRight_1:
            field_110_xPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x + 12);
            field_114_yPos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
            field_BC_ypos = field_114_yPos;

            if (field_128_flags.Get(Flags::eBit2_ToggleStartState_StartOn))
            {
                const AnimRecord& animRec = AnimRec(AnimId::Drill_Horizontal_On);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            }
            else
            {
                const AnimRecord& animRec = AnimRec(AnimId::Drill_Horizontal_Off);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            }

            field_F6_width = pTlv->field_C_bottom_right.field_0_x - pTlv->field_8_top_left.field_0_x;
            if (field_128_flags.Get(Flags::eBit6_StartPosIsBottom))
            {
                field_124_xyoff = FP_FromInteger(0);
            }
            else
            {
                field_124_xyoff = FP_FromInteger(field_F6_width);
            }
            field_B8_xpos = field_124_xyoff + field_110_xPos;
            break;

        case DrillDirection::eLeft_2:
            field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);

            field_110_xPos = FP_FromInteger(pTlv->field_C_bottom_right.field_0_x - 12);
            field_114_yPos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
            field_BC_ypos = field_114_yPos;

            if (field_128_flags.Get(Flags::eBit2_ToggleStartState_StartOn))
            {
                const AnimRecord& animRec = AnimRec(AnimId::Drill_Horizontal_On);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            }
            else
            {
                const AnimRecord& animRec = AnimRec(AnimId::Drill_Horizontal_Off);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            }

            field_F6_width = pTlv->field_C_bottom_right.field_0_x - pTlv->field_8_top_left.field_0_x;
            if (field_128_flags.Get(Flags::eBit6_StartPosIsBottom))
            {
                field_124_xyoff = FP_FromInteger(0);
            }
            else
            {
                field_124_xyoff = FP_FromInteger(field_F6_width);
            }
            field_B8_xpos = field_110_xPos - field_124_xyoff;
            break;
    }

    field_FE_max_off_time = tlvData.field_14_max_off_time;
    field_FC_min_off_time = tlvData.field_12_min_off_time;
    switch (tlvData.field_18_behavior)
    {
        case DrillBehavior::eToggle_1:
            field_128_flags.Set(Flags::eBit3_UseId);
            field_128_flags.Clear(Flags::eBit4_Toggle);
            break;

        case DrillBehavior::eUse_2:
            field_128_flags.Set(Flags::eBit3_UseId);
            field_128_flags.Set(Flags::eBit4_Toggle);
            break;

        default:
            field_128_flags.Clear(Flags::eBit3_UseId);
            field_128_flags.Clear(Flags::eBit4_Toggle);
            break;
    }

    field_118_speed = FP_FromInteger(tlvData.field_1A_speed);
    if (tlvData.field_1A_speed == 250) // TODO: figure out why this is needed
    {
        field_118_speed = FP_FromDouble(0.2);
    }

    field_11C_speed2 = field_118_speed;
    field_120_off_speed = FP_FromInteger(tlvData.field_1E_off_speed);
    if (tlvData.field_1E_off_speed == 250) // TODO: figure out why this is needed
    {
        field_120_off_speed = FP_FromDouble(0.2);
    }

    field_100_min_off_time_speed_change = tlvData.field_20_min_off_time_speed_change;
    field_102_max_off_time_speed_change = tlvData.field_22_max_off_time_speed_change;
    field_108_off_timer = 0;
    field_F4_state = DrillStates::State_0_Restart_Cycle;
    field_104_tlv = tlvInfo;
    field_10C_audio_channels_mask = 0;

    auto pShadow = ae_new<Shadow>();
    if (pShadow)
    {
        pShadow->ctor_4AC990();
    }
    field_E0_pShadow = pShadow;

    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID);
    return this;
}

BaseGameObject* Drill::VDestructor(s32 flags)
{
    return vdtor_4206A0(flags);
}

void Drill::VUpdate()
{
    vUpdate_420C50();
}

void Drill::VRender(PrimHeader** ppOt)
{
    vRender_4213D0(ppOt);
}

void Drill::VScreenChanged()
{
    vScreenChanged_4214B0();
}

void Drill::VStopAudio()
{
    vStopAudio_4215C0();
}

s32 Drill::VGetSaveState(u8* pSaveBuffer)
{
    return vGetSaveState_4217B0(pSaveBuffer);
}

s32 CC Drill::CreateFromSaveState_421600(const u8* pData)
{
    const Drill_State* pState = reinterpret_cast<const Drill_State*>(pData);

    Path_Drill* pTlv = static_cast<Path_Drill*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_8_tlvInfo));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", 0);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("DOGBLOW.BAN", 0);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kDrillResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("DRILL.BAN", 0);
    }

    auto pDrill = ae_new<Drill>();
    if (pDrill)
    {
        pDrill->ctor_4200D0(pTlv, pState->field_8_tlvInfo);
    }

    if (pState->field_10_state != DrillStates::State_0_Restart_Cycle)
    {
        switch (pDrill->field_FA_direction)
        {
            case DrillDirection::eDown_0:
            {
                const AnimRecord& animRec = AnimRec(AnimId::Drill_Vertical_On);
                pDrill->field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                break;
            }
            case DrillDirection::eRight_1:
            case DrillDirection::eLeft_2:
            {
                const AnimRecord& animRec = AnimRec(AnimId::Drill_Horizontal_On);
                pDrill->field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                break;
            }
        }
    }

    pDrill->field_108_off_timer = pState->field_C_off_timer;
    pDrill->field_F4_state = pState->field_10_state;
    pDrill->field_124_xyoff = FP_FromInteger(pState->field_12_xyoff);
    return sizeof(Drill_State);
}

void Drill::vUpdate_420C50()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    const CameraPos soundDirection = gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos);

    switch (field_F4_state)
    {
        case DrillStates::State_0_Restart_Cycle:
            if (Expired(field_108_off_timer) || field_128_flags.Get(eBit4_Toggle))
            {
                if ((!field_128_flags.Get(Flags::eBit3_UseId)) || (!!SwitchStates_Get_466020(field_F8_id) == (field_128_flags.Get(eBit1_StartOff))))
                {
                    field_F4_state = DrillStates::State_1_Going_Down;

                    switch (field_FA_direction)
                    {
                        case DrillDirection::eDown_0:
                        {
                            const AnimRecord& animRec = AnimRec(AnimId::Drill_Vertical_On);
                            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                            break;
                        }
                        case DrillDirection::eRight_1:
                        case DrillDirection::eLeft_2:
                        {
                            const AnimRecord& animRec = AnimRec(AnimId::Drill_Horizontal_On);
                            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                            break;
                        }
                    }

                    field_128_flags.Clear(Flags::eBit5_SpeedChanged);
                    field_11C_speed2 = field_118_speed;
                    field_10C_audio_channels_mask = SFX_Play_46FC20(SoundEffect::DrillMovement_97, 25, soundDirection);
                    return;
                }
            }

            if (field_128_flags.Get(Flags::eBit3_UseId) && !field_128_flags.Get(Flags::eBit4_Toggle) && FP_GetExponent(field_120_off_speed) > 0 && Expired(field_108_off_timer))
            {
                field_F4_state = DrillStates::State_1_Going_Down;

                switch (field_FA_direction)
                {
                    case DrillDirection::eDown_0:
                    {
                        const AnimRecord& animRec = AnimRec(AnimId::Drill_Vertical_On);
                        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                        break;
                    }

                    case DrillDirection::eRight_1:
                    {
                        const AnimRecord& animRec = AnimRec(AnimId::Drill_Horizontal_On);
                        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                        break;
                    }

                    case DrillDirection::eLeft_2:
                    {
                        const AnimRecord& animRec = AnimRec(AnimId::Drill_Horizontal_On);
                        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
                        break;
                    }
                }

                field_128_flags.Set(Flags::eBit5_SpeedChanged);
                field_11C_speed2 = field_120_off_speed;
                field_10C_audio_channels_mask = SFX_Play_46FC20(SoundEffect::DrillMovement_97, 25, soundDirection);
            }
            break;

        case DrillStates::State_1_Going_Down:
            if (!field_10C_audio_channels_mask)
            {
                field_10C_audio_channels_mask = SFX_Play_46FC20(SoundEffect::DrillMovement_97, 25, soundDirection);
            }

            DamageTouchingObjects_421060();

            field_124_xyoff = field_124_xyoff - field_11C_speed2;
            if (field_124_xyoff <= FP_FromInteger(0))
            {
                field_F4_state = DrillStates::State_2_GoingUp;
                SFX_Play_46FC20(SoundEffect::DrillCollision_99, 50, soundDirection, FP_FromInteger(1));
            }
            EmitSparks_4206D0();
            break;

        case DrillStates::State_2_GoingUp:
            if (!field_10C_audio_channels_mask)
            {
                field_10C_audio_channels_mask = SFX_Play_46FC20(SoundEffect::DrillMovement_97, 25, soundDirection);
            }

            DamageTouchingObjects_421060();

            field_124_xyoff = field_11C_speed2 + field_124_xyoff;
            if (field_124_xyoff >= FP_FromInteger(field_F6_width))
            {
                if (field_10C_audio_channels_mask)
                {
                    SND_Stop_Channels_Mask_4CA810(field_10C_audio_channels_mask);
                    field_10C_audio_channels_mask = 0;
                }

                field_F4_state = DrillStates::State_0_Restart_Cycle;
                SFX_Play_46FC20(SoundEffect::DrillCollision_99, 50, soundDirection);

                s16 max_off = 0;
                s16 min_off = 0;
                if (field_128_flags.Get(Flags::eBit5_SpeedChanged))
                {
                    max_off = field_102_max_off_time_speed_change;
                    min_off = field_100_min_off_time_speed_change;
                }
                else
                {
                    max_off = field_FE_max_off_time;
                    min_off = field_FC_min_off_time;
                }

                field_108_off_timer = MakeTimer(Math_RandomRange_496AB0(min_off, max_off));

                if (field_FA_direction == DrillDirection::eDown_0)
                {
                    const AnimRecord& animRec = AnimRec(AnimId::Drill_Vertical_Off);
                    field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                }
                else
                {
                    const AnimRecord& animRec = AnimRec(AnimId::Drill_Horizontal_Off);
                    field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                }

                if (field_128_flags.Get(eBit4_Toggle))
                {
                    SwitchStates_Set_465FF0(field_F8_id, !field_128_flags.Get(eBit1_StartOff));
                }
            }

            EmitSparks_4206D0();
            break;
    }
}

void Drill::dtor_420B60()
{
    SetVTable(this, 0x544AD8); // vTbl_Grinder_544AD8

    if (field_10C_audio_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_10C_audio_channels_mask);
        field_10C_audio_channels_mask = 0;
    }

    if (field_128_flags.Get(Flags::eBit3_UseId) && !!SwitchStates_Get_466020(field_F8_id) != field_128_flags.Get(Flags::eBit1_StartOff))
    {
        Path::TLV_Reset_4DB8E0(field_104_tlv, 1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_104_tlv, 0, 0, 0);
    }

    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

Drill* Drill::vdtor_4206A0(s32 flags)
{
    dtor_420B60();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Drill::vScreenChanged_4214B0()
{
    if (field_F4_state != DrillStates::State_0_Restart_Cycle)
    {
        if (field_128_flags.Get(Flags::eBit6_StartPosIsBottom))
        {
            field_124_xyoff = FP_FromInteger(0);
        }
        else
        {
            field_124_xyoff = FP_FromInteger(field_F6_width);
        }
    }

    // Stop that sound
    if (field_10C_audio_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_10C_audio_channels_mask);
        field_10C_audio_channels_mask = 0;
    }

    // Seems the real function does this, but then always kill object at the end!
    // So it appears drills where supposed to survive for up to 1 screen away.

    /*
    // Map changed
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId || gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId)
    {
        field_6_flags.Set(BaseGameObject::eDead);
        return;
    }

    // More than 1 screen away on X?
    if (FP_Abs(sControlledCharacter_5C1B8C->field_B8_xpos - field_B8_xpos) > FP_FromInteger(375))
    {
        field_6_flags.Set(BaseGameObject::eDead);
        return;
    }

    // More than 1 screen away on Y?
    if (FP_Abs(sControlledCharacter_5C1B8C->field_BC_ypos - field_BC_ypos) > FP_FromInteger(260))
    {
        field_6_flags.Set(BaseGameObject::eDead);
        return;
    }
    */

    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void Drill::vRender_4213D0(PrimHeader** ppOt)
{
    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0))
    {
        if (field_FA_direction == DrillDirection::eDown_0)
        {
            field_BC_ypos = field_114_yPos - field_124_xyoff;
            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
        }
        else if (field_FA_direction == DrillDirection::eRight_1)
        {
            field_B8_xpos = field_110_xPos + field_124_xyoff;
            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
        }
        else if (field_FA_direction == DrillDirection::eLeft_2)
        {
            field_B8_xpos = field_110_xPos - field_124_xyoff;
            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
        }
    }
}

void Drill::vStopAudio_4215C0()
{
    if (field_10C_audio_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_10C_audio_channels_mask);
        field_10C_audio_channels_mask = 0;
    }
}

s32 Drill::vGetSaveState_4217B0(u8* pSaveBuffer)
{
    Drill_State* pState = reinterpret_cast<Drill_State*>(pSaveBuffer);
    pState->field_0 = 30;
    pState->field_8_tlvInfo = field_104_tlv;
    pState->field_C_off_timer = field_108_off_timer;
    pState->field_10_state = field_F4_state;
    pState->field_12_xyoff = FP_GetExponent(field_124_xyoff);
    return sizeof(Drill_State);
}

void Drill::EmitSparks_4206D0()
{
    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        s32 speed = 0;
        if (field_F4_state == DrillStates::State_1_Going_Down)
        {
            speed = -FP_GetExponent(field_11C_speed2 - FP_FromInteger(2));
        }
        else if (field_F4_state == DrillStates::State_2_GoingUp)
        {
            speed = FP_GetExponent(field_11C_speed2);
        }

        // 1 in 6 chance of sparks
        if (Math_RandomRange_496AB0(0, 6) == 0)
        {
            if (field_FA_direction == DrillDirection::eRight_1)
            {
                auto pSpark1 = ae_new<Spark>();
                if (pSpark1)
                {
                    pSpark1->ctor_4CBBB0(
                        field_B8_xpos - (field_CC_sprite_scale * FP_FromInteger(17)) + FP_FromInteger(speed),
                        field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(12)),
                        field_CC_sprite_scale,
                        6u,
                        50,
                        205,
                        SparkType::eSmallChantParticle_0);
                }

                auto pSpark2 = ae_new<Spark>();
                if (pSpark2)
                {
                    pSpark2->ctor_4CBBB0(
                        field_B8_xpos + (field_CC_sprite_scale * FP_FromInteger(17)) + FP_FromInteger(speed),
                        field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(12)),
                        field_CC_sprite_scale,
                        6u,
                        50,
                        205,
                        SparkType::eSmallChantParticle_0);
                }
            }
            else if (field_FA_direction == DrillDirection::eLeft_2)
            {
                auto pSpark1 = ae_new<Spark>();
                if (pSpark1)
                {
                    pSpark1->ctor_4CBBB0(
                        field_B8_xpos + (field_CC_sprite_scale * FP_FromInteger(17)) - FP_FromInteger(speed),
                        field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(12)),
                        field_CC_sprite_scale,
                        6u,
                        50,
                        205,
                        SparkType::eSmallChantParticle_0);
                }

                auto pSpark2 = ae_new<Spark>();
                if (pSpark2)
                {
                    pSpark2->ctor_4CBBB0(
                        field_B8_xpos - (field_CC_sprite_scale * FP_FromInteger(17)) - FP_FromInteger(speed),
                        field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(12)),
                        field_CC_sprite_scale,
                        6u,
                        50,
                        205,
                        SparkType::eSmallChantParticle_0);
                }
            }
            else if (field_FA_direction == DrillDirection::eDown_0)
            {
                auto pSpark1 = ae_new<Spark>();
                if (pSpark1)
                {
                    pSpark1->ctor_4CBBB0(
                        field_B8_xpos,
                        field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(22)) - FP_FromInteger(speed),
                        field_CC_sprite_scale,
                        6u,
                        50,
                        205,
                        SparkType::eSmallChantParticle_0);
                }

                auto pSpark2 = ae_new<Spark>();
                if (pSpark2)
                {
                    pSpark2->ctor_4CBBB0(
                        field_B8_xpos,
                        field_BC_ypos + (field_CC_sprite_scale * FP_FromInteger(4)) - FP_FromInteger(speed),
                        field_CC_sprite_scale,
                        6u,
                        50,
                        205,
                        SparkType::eSmallChantParticle_0);
                }
            }
        }
    }
}

s16 Drill::DamageTouchingObjects_421060()
{
    PSX_RECT drillRect = {};
    vGetBoundingRect_424FD0(&drillRect, 1);

    if (field_FA_direction == DrillDirection::eDown_0)
    {
        drillRect.y += 16;
    }

    BaseAliveGameObject* pFound = nullptr;
    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            return 0;
        }

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6) || pObj->Type() == AETypes::eRockSpawner_48)
        {
            if (pObj->field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
            {
                if (pObj->Type() != AETypes::eMeat_84 && pObj->Type() != AETypes::eEvilFart_45 && (pObj->Type() != AETypes::eAbe_69 || pObj->field_106_current_motion != eAbeMotions::Motion_68_ToOffScreenHoist_454B80))
                {
                    PSX_RECT objRect = {};
                    pObj->vGetBoundingRect_424FD0(&objRect, 1);

                    if (RectsOverlap(drillRect, objRect) && pObj->field_D6_scale == field_D6_scale && pObj->field_10C_health > FP_FromInteger(0))
                    {
                        if (pObj->field_B8_xpos + FP_FromInteger(3) >= FP_FromInteger(drillRect.x) && pObj->field_B8_xpos - FP_FromInteger(3) <= FP_FromInteger(drillRect.w))
                        {
                            pFound = pObj;
                            break;
                        }
                    }
                }
            }
        }

        pObj = nullptr;
    }

    if (!pFound)
    {
        return 0;
    }

    if (!pFound->VTakeDamage_408730(this))
    {
        return 1;
    }

    auto pBlood = ae_new<Blood>();
    if (pBlood)
    {
        pBlood->ctor_40F0B0(
            pFound->field_B8_xpos,
            FP_FromInteger(drillRect.h - 10),
            FP_FromInteger(-5),
            FP_FromInteger(5),
            field_CC_sprite_scale,
            50);
    }

    auto pBlood2 = ae_new<Blood>();
    if (pBlood2)
    {
        pBlood2->ctor_40F0B0(
            pFound->field_B8_xpos,
            FP_FromInteger(drillRect.h - 10),
            FP_FromInteger(0),
            FP_FromInteger(5),
            field_CC_sprite_scale,
            50);
    }

    auto pBlood3 = ae_new<Blood>();
    if (pBlood3)
    {
        pBlood3->ctor_40F0B0(
            pFound->field_B8_xpos,
            FP_FromInteger(drillRect.h - 10),
            FP_FromInteger(5),
            FP_FromInteger(5),
            field_CC_sprite_scale,
            50);
    }

    SFX_Play_46FBA0(SoundEffect::DrillCollision_99, 127, -500);
    SFX_Play_46FA90(SoundEffect::KillEffect_64, 127);
    SFX_Play_46FBA0(SoundEffect::KillEffect_64, 127, -700);

    return 1;
}
