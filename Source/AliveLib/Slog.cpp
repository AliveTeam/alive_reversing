#include "stdafx.h"
#include "Slog.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "MusicController.hpp"
#include "Collisions.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "DDCheat.hpp"
#include "Shadow.hpp"
#include "Map.hpp"

ALIVE_VAR(1, 0xBAF7F2, short, sSlogCount_BAF7F2, 0);

const TSlogMotionFn sSlog_motion_table_560978[24] =
{
    &Slog::M_Idle_0_4C5F90,
    &Slog::M_Walk_1_4C60C0,
    &Slog::M_Run_2_4C6340,
    &Slog::M_TurnAround_3_4C65C0,
    &Slog::M_Fall_4_4C6930,
    &Slog::M_MoveHeadUpwards_5_4C5F20,
    &Slog::M_StopRunning_6_4C66C0,
    &Slog::M_SlideTurn_7_4C6790,
    &Slog::M_StartWalking_8_4C62E0,
    &Slog::M_EndWalking_9_4C6310,
    &Slog::M_Land_10_4C7820,
    &Slog::M_Unknown_11_4C7860,
    &Slog::M_StartFastBarking_12_4C7880,
    &Slog::M_EndFastBarking_13_4C78D0,
    &Slog::M_Unknown_14_4C6CF0,
    &Slog::M_Sleeping_15_4C6D60,
    &Slog::M_MoveHeadDownwards_16_4C70D0,
    &Slog::M_Bark_17_4C7000,
    &Slog::M_JumpForwards_18_4C7210,
    &Slog::M_JumpUpwards_19_4C7470,
    &Slog::M_Eating_20_4C75F0,
    &Slog::M_Unknown_21_4C77F0,
    &Slog::M_Scratch_22_4C7120,
    &Slog::M_Growl_23_4C7170
};

enum eSlogMotions
{
    M_Idle_0_4C5F90,
    M_Walk_1_4C60C0,
    M_Run_2_4C6340,
    M_TurnAround_3_4C65C0,
    M_Fall_4_4C6930,
    M_MoveHeadUpwards_5_4C5F20,
    M_StopRunning_6_4C66C0,
    M_SlideTurn_7_4C6790,
    M_StartWalking_8_4C62E0,
    M_EndWalking_9_4C6310,
    M_Land_10_4C7820,
    M_Unknown_11_4C7860,
    M_StartFastBarking_12_4C7880,
    M_EndFastBarking_13_4C78D0,
    M_Unknown_14_4C6CF0,
    M_Sleeping_15_4C6D60,
    M_MoveHeadDownwards_16_4C70D0,
    M_Bark_17_4C7000,
    M_JumpForwards_18_4C7210,
    M_JumpUpwards_19_4C7470,
    M_Eating_20_4C75F0,
    M_Unknown_21_4C77F0,
    M_Scratch_22_4C7120,
    M_Growl_23_4C7170
};

const TSlogAIFn sSlog_fns_ai_560A38[4] =
{
    &Slog::AI_ListeningToSlig_0_4C3790,
    &Slog::AI_Idle_1_4C2830,
    &Slog::AI_ChasingAbe_2_4C0A00,
    &Slog::AI_Death_3_4C3250
};

const static AIFunctionData<TSlogAIFn> sSlogAiTable[4] =
{
    { &Slog::AI_ListeningToSlig_0_4C3790, 0x4C3790, "AI_ListeningToSlig_0" }, // no stubs for any of these ??
    { &Slog::AI_Idle_1_4C2830, 0x4C2830, "AI_Idle_1" },
    { &Slog::AI_ChasingAbe_2_4C0A00, 0x4C0A00, "AI_ChasingAbe_2" },
    { &Slog::AI_Death_3_4C3250, 0x4C3250, "AI_Death_3" },
};

Slog* Slog::ctor_4C42E0(Path_Slog* pTlv, int tlvInfo)
{
    ctor_408240(5);
    field_134 = -1;
    SetVTable(this, 0x547578);

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (pTlv->field_10_scale)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_CC_sprite_scale = FP_FromDouble(1);
    }

    field_C_objectId = tlvInfo;
    
    Init_4C46A0();

    field_160_flags.Clear(Flags_160::eBit9);
    field_160_flags.Set(Flags_160::eBit2);
    field_160_flags.Set(Flags_160::eBit7, pTlv->field_14_asleep & 1);
    field_160_flags.Clear(Flags_160::eBit5);

    field_6_flags.Set(BaseGameObject::eCanExplode);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_12_direction == 0);

    field_12C_tlvInfo = tlvInfo;
    field_C_objectId = tlvInfo;
    field_120_brain_state_idx = 1;
    field_118 = -1;
    field_144 = pTlv->field_16_wake_up_anger;
    field_146_total_anger = pTlv->field_16_wake_up_anger + pTlv->field_18_bark_anger;
    field_148_chase_anger = field_146_total_anger + pTlv->field_1A_chase_anger;
    field_158_jump_delay = pTlv->field_1C_jump_delay;
    field_154_angry_id = pTlv->field_20_angry_id;
    field_156_bone_eating_time = pTlv->field_22_bone_eating_time;

    if (field_160_flags.Get(Flags_160::eBit7))
    {
        field_106_current_motion = 15;
        SetAnimFrame_4C42A0();
    }
    else
    {
        field_106_current_motion = 0;
    }
    
    VUpdate();

    return this;
}

BaseGameObject* Slog::VDestructor(signed int flags)
{
    return vdtor_4C4510(flags);
}

void Slog::VUpdate()
{
    vUpdate_4C50D0();
}

void Slog::M_Idle_0_4C5F90()
{
    NOT_IMPLEMENTED();
}

void Slog::M_Walk_1_4C60C0()
{
    NOT_IMPLEMENTED();
}

void Slog::M_Run_2_4C6340()
{
    NOT_IMPLEMENTED();
}

void Slog::M_TurnAround_3_4C65C0()
{
    NOT_IMPLEMENTED();
}

void Slog::M_Fall_4_4C6930()
{
    NOT_IMPLEMENTED();
}

void Slog::M_MoveHeadUpwards_5_4C5F20()
{
    NOT_IMPLEMENTED();
}

void Slog::M_StopRunning_6_4C66C0()
{
    NOT_IMPLEMENTED();
}

void Slog::M_SlideTurn_7_4C6790()
{
    NOT_IMPLEMENTED();
}

void Slog::M_StartWalking_8_4C62E0()
{
    NOT_IMPLEMENTED();
}

void Slog::M_EndWalking_9_4C6310()
{
    NOT_IMPLEMENTED();
}

void Slog::M_Land_10_4C7820()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Sfx_4C7D30(16);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_4C5C10();
    }
}

void Slog::M_Unknown_11_4C7860()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eSlogMotions::M_StartFastBarking_12_4C7880;
    }
}

void Slog::M_StartFastBarking_12_4C7880()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Sfx_4C7D30(2);
        field_132 = 1;
    }

    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = eSlogMotions::M_EndFastBarking_13_4C78D0;
        }
    }

}

void Slog::M_EndFastBarking_13_4C78D0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_4C5C10();
    }
}

void Slog::M_Unknown_14_4C6CF0()
{
    NOT_IMPLEMENTED();
}

void Slog::M_Sleeping_15_4C6D60()
{
    NOT_IMPLEMENTED();
}

void Slog::M_MoveHeadDownwards_16_4C70D0()
{
    NOT_IMPLEMENTED();
}

void Slog::M_Bark_17_4C7000()
{
    NOT_IMPLEMENTED();
}

void Slog::M_JumpForwards_18_4C7210()
{
    NOT_IMPLEMENTED();
}

void Slog::M_JumpUpwards_19_4C7470()
{
    NOT_IMPLEMENTED();
}

void Slog::M_Eating_20_4C75F0()
{
    NOT_IMPLEMENTED();
}

void Slog::M_Unknown_21_4C77F0()
{
    if (!field_100_pCollisionLine)
    {
        M_Fall_4_4C6930();
        field_106_current_motion = eSlogMotions::M_Unknown_21_4C77F0;
    }
}

void Slog::M_Scratch_22_4C7120()
{
    NOT_IMPLEMENTED();
}

void Slog::M_Growl_23_4C7170()
{
    NOT_IMPLEMENTED();
}

__int16 Slog::AI_ListeningToSlig_0_4C3790()
{
    NOT_IMPLEMENTED(); 
    return 0;
}

__int16 Slog::AI_Idle_1_4C2830()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slog::AI_ChasingAbe_2_4C0A00()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slog::AI_Death_3_4C3250()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Slog::SetAnimFrame_4C42A0()
{
    NOT_IMPLEMENTED();
}

TintEntry stru_560A48[] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 127u, 127u, 127u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 127u, 127u, 127u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 11u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u }
};

void Slog::Init_4C46A0()
{
    field_4_typeId = Types::eSlog_126;
    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kDogbasicResID, 1, 0));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kDogrstnResID, 1, 0));
    field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kDogattkResID, 1, 0));
    field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kDogknfdResID, 1, 0));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kDogidleResID, 1, 0));
    Animation_Init_424E10(96464, 121, 57, field_10_resources_array.ItemAt(0), 1, 1);

    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_160_flags.Clear(Flags_160::eBit3);
    field_160_flags.Clear(Flags_160::eBit6);
    field_160_flags.Set(Flags_160::eBit4);

    field_DC_bApplyShadows |= 2u;
    field_20_animation.field_1C_fn_ptr_array = kSlog_Anim_Frame_Fns_55EFBC;
    field_124_timer = 0;
    field_122_brain_state_result = 0;
    field_108_next_motion = -1;
    field_130 = 0;
    field_110_id = -1;
    field_138 = -1;
    field_118 = -1;
    field_15C = -1;
    SetTint_425600(&stru_560A48[0], gMap_5C3030.sCurrentLevelId_5C3030);
    field_20_animation.field_C_render_layer = 34;

    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_D8_yOffset = 1;
    }
    else
    {
        field_D8_yOffset = 2;
    }

    field_D6_scale = 1;
    
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos, field_BC_ypos, 
        field_B8_xpos, field_BC_ypos + FP_FromInteger(24), 
        &field_100_pCollisionLine, &hitX, &hitY, 1) == 1)
    {
        field_BC_ypos = hitY;
        if (field_100_pCollisionLine->field_8_type == 32)
        {
            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);
            const PSX_Point xy = { bRect.x, static_cast<short>(bRect.y + 5) };
            const PSX_Point wh = { bRect.w, static_cast<short>(bRect.h + 5) };
            vOnCollisionWith_424EE0(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
        }
    }

    MapFollowMe_408D10(FALSE);
    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    sSlogCount_BAF7F2++;

    vStackOnObjectsOfType_425840(Types::eSlog_126);
}

void Slog::vUpdate_4C50D0()
{
    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9);
        if (field_104_collision_line_type == -1)
        {
            field_100_pCollisionLine = nullptr;
        }
        else
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(20),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(20),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104_collision_line_type);
        }
        field_104_collision_line_type = 0;
        field_118 = BaseGameObject::Find_Flags_4DC170(field_118);
        field_138 = BaseGameObject::Find_Flags_4DC170(field_138);
        field_15C = BaseGameObject::Find_Flags_4DC170(field_15C);
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (FP_Abs(field_B8_xpos - sControlledCharacter_5C1B8C->field_B8_xpos) > FP_FromInteger(750) ||
        FP_Abs(field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos) > FP_FromInteger(390))
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        if (field_10C_health > FP_FromInteger(0))
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        }

        const auto oldMotion = field_106_current_motion;
        const auto oldBrain = sSlog_fns_ai_560A38[field_120_brain_state_idx];
        field_122_brain_state_result = (this->*sSlog_fns_ai_560A38[field_120_brain_state_idx])();
        if (sDDCheat_ShowAI_Info_5C1BD8)
        {
            DDCheat::DebugStr_4F5560("Slog:  Motion=%d  BrainState=%d\n", field_106_current_motion, field_122_brain_state_result);
        }
        
        const FP oldXPos = field_B8_xpos;
        const FP oldYPos = field_BC_ypos;
        
        (this->*sSlog_motion_table_560978[field_106_current_motion])();

        if (oldXPos != field_B8_xpos || oldYPos != field_BC_ypos)
        {
            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);
            VOn_TLV_Collision_4087F0(field_FC_pPathTLV);
        }

        // TODO: This is extra debug logging to figure out the motion names
        if (oldBrain != sSlog_fns_ai_560A38[field_120_brain_state_idx])
        {
            LOG_INFO("Slog: Old brain = " << GetOriginalFn(oldBrain, sSlogAiTable).fnName << " new brain = " << GetOriginalFn(sSlog_fns_ai_560A38[field_120_brain_state_idx], sSlogAiTable).fnName);

            //LOG_INFO("Slog: Old motion = " << oldMotion << " new motion = " << field_106_current_motion);
        }

        if (oldMotion != field_106_current_motion)
        {
            SetAnimFrame_4C42A0();
        }
    }
}

void Slog::dtor_4C49A0()
{
    SetVTable(this, 0x547578);

    field_118 = -1;
    field_138 = -1;
    field_15C = -1;

    if (field_12C_tlvInfo != 0xFFFF)
    {
        Path::TLV_Reset_4DB8E0(field_12C_tlvInfo, -1, 0, field_10C_health <= FP_FromInteger(0));
    }

    MusicController::sub_47FD60(0, this, 0, 0);
    
    if (!field_160_flags.Get(Flags_160::eBit3))
    {
        sSlogCount_BAF7F2--;
    }

    dtor_4080B0();
}

Slog* Slog::vdtor_4C4510(signed int flags)
{
    dtor_4C49A0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Slog::ToIdle_4C5C10()
{
    MapFollowMe_408D10(FALSE);
    field_128 = 0;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_106_current_motion = 0;
    field_108_next_motion = -1;
}

void Slog::Sfx_4C7D30(int /*effectId*/)
{
    NOT_IMPLEMENTED();
}
