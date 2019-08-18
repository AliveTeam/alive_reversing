#include "stdafx.h"
#include "Fleech.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "SnoozeParticle.hpp"
#include "Dove.hpp"
#include "Blood.hpp"
#include "Shadow.hpp"
#include "Particle.hpp"
#include "Gibs.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "PsxDisplay.hpp"

ALIVE_VAR(1, 0x5BC20C, BYTE, sFleechRandomIdx_5BC20C, 0);
ALIVE_VAR(1, 0x5BC20E, short, sFleechCount_5BC20E, 0);

const TFleechMotionFn sFleech_motion_table_551798[19] =
{
    &Fleech::M_Sleeping_0_42F0B0,
    &Fleech::M_WakingUp_1_42F270,
    &Fleech::M_Unknown_2_42F2F0,
    &Fleech::M_Idle_3_42E850,
    &Fleech::M_Crawl_4_42E960,
    &Fleech::M_PatrolCry_5_42E810,
    &Fleech::M_Knockback_6_42EAF0,
    &Fleech::M_StopCrawling_7_42EBB0, //Stop due to being aligned vertically with target
    &Fleech::M_StopMidCrawlCycle_8_42EB20, //Stop mid-crawl cycle
    &Fleech::M_Fall_9_42ECD0,
    &Fleech::M_Land_10_42F330,
    &Fleech::M_RaiseHead_11_42F590,
    &Fleech::M_Climb_12_42F7F0,
    &Fleech::M_SettleOnGround_13_42FB00,
    &Fleech::M_ExtendTongueFromEnemy_14_42FBD0,
    &Fleech::M_RetractTongueFromEnemey_15_42FC40,
    &Fleech::M_DeathByFalling_16_42FCE0,
    &Fleech::M_SleepingWithTongue_17_42F370,
    &Fleech::M_Consume_18_42FDF0
};

enum eFleechMotions
{
    M_Sleeping_0_42F0B0,
    M_WakingUp_1_42F270,
    M_Unknown_2_42F2F0,
    M_Idle_3_42E850,
    M_Crawl_4_42E960,
    M_PatrolCry_5_42E810,
    M_Knockback_6_42EAF0,
    M_StopCrawling_7_42EBB0,
    M_StopMidCrawlCycle_8_42EB20, 
    M_Fall_9_42ECD0,
    M_Land_10_42F330,
    M_RaiseHead_11_42F590,
    M_Climb_12_42F7F0,
    M_SettleOnGround_13_42FB00,
    M_ExtendTongueFromEnemy_14_42FBD0,
    M_RetractTongueFromEnemey_15_42FC40,
    M_DeathByFalling_16_42FCE0,
    M_SleepingWithTongue_17_42F370,
    M_Consume_18_42FDF0
};

const TFleechAIFn sFleech_ai_table_551830[4] =
{
    &Fleech::AI_Patrol_0_430BA0,
    &Fleech::AI_ChasingAbe_1_428760,
    &Fleech::AI_Scared_2_42D310,
    &Fleech::AI_Death_3_42D1E0
};

enum eFleechBrains
{
    eAI_Patrol_0_430BA0 = 0,
    eAI_ChasingAbe_1_428760 = 1,
    eAI_Scared_2_42D310 = 2,
    eAI_Death_3_42D1E0 = 3,
};

const static AIFunctionData<TFleechAIFn> sFleechAiTable[4] =
{
    { &Fleech::AI_Patrol_0_430BA0, 0x430BA0, "AI_Patrol_0" }, // no stub ??
    { &Fleech::AI_ChasingAbe_1_428760, 0x428760, "AI_ChasingAbe_1" }, // no stub ??
    { &Fleech::AI_Scared_2_42D310, 0x42D310, "AI_Scared_2" }, // no stub ??
    { &Fleech::AI_Death_3_42D1E0, 0x42D1E0, "AI_Death_3" }, // no stub ??
};

static BYTE Fleech_NextRandom()
{
    return sRandomBytes_546744[sFleechRandomIdx_5BC20C++];
}

Fleech* Fleech::ctor_429DC0(Path_Fleech* pTlv, int tlvInfo)
{
    ctor_408240(2);
    SetVTable(this, 0x544F28);
    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_C_objectId = tlvInfo;

    if (pTlv->field_10_scale)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
    }

    Init_42A170();

    field_118_tlvInfo = tlvInfo;
    field_124_brain_state = eFleechBrains::eAI_Patrol_0_430BA0;
    field_11C_obj_id = -1;
    field_170 = -1;

    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_12_direction == 0);

    field_174_flags.Set(Flags_174::eBit5_asleep, pTlv->field_14_asleep & 1);
    field_174_flags.Set(Flags_174::eBit6_goes_to_sleep, pTlv->field_24_goes_to_sleep & 1);
    field_174_flags.Set(Flags_174::eBit7_persistant, pTlv->field_2C_persistant & 1);

    field_140 = 2;
    field_158 = 10;

    field_142_attack_anger = pTlv->field_1A_attack_anger + 2;
    field_144_wake_up_id = pTlv->field_1E_wake_up_id;
    field_146_tlv_28 = pTlv->field_28_unused;
    field_148_wake_up_switch_value = SwitchStates_Get_466020(pTlv->field_1E_wake_up_id) & 0xFFFF;
    field_14A_allow_wake_up_id = pTlv->field_2A_allow_wake_up_id;
    field_150_patrol_range = FP_GetExponent(FP_FromInteger(pTlv->field_26_patrol_range_in_grids) * ScaleToGridSize_4498B0(field_CC_sprite_scale));
    field_15C_lost_target_timeout = pTlv->field_22_lost_target_timeout;

    InitPolys_42B6E0();

    field_13E_anger = 0;

    if (pTlv->field_20_hanging)
    {
        field_160 = (pTlv->field_C_bottom_right.field_0_x + pTlv->field_8_top_left.field_0_x) / 2;
        field_166_angle = Fleech_NextRandom();
        field_BC_ypos -= FP_FromInteger(pTlv->field_8_top_left.field_2_y - pTlv->field_C_bottom_right.field_2_y);
        sub_42B9A0((pTlv->field_C_bottom_right.field_0_x + pTlv->field_8_top_left.field_0_x) / 2, pTlv->field_8_top_left.field_2_y);
        field_106_current_motion = 17;
        SetAnim_429D80();
    }
    else if (field_174_flags.Get(Flags_174::eBit5_asleep))
    {
        field_106_current_motion = 0;
        SetAnim_429D80();
    }
    else
    {
        field_106_current_motion = 3;
        field_13E_anger = field_140 + (field_142_attack_anger - field_140) / 2;
    }

    return this;
}

BaseGameObject* Fleech::VDestructor(signed int flags)
{
    return vdtor_42A140(flags);
}

void Fleech::VUpdate()
{
    vUpdate_42AB20();
}

void Fleech::VScreenChanged()
{
    vScreenChanged_42A4C0();
}

void Fleech::VOn_TLV_Collision_4087F0(Path_TLV* pTlv)
{
    vOn_Tlv_Collision_42AAB0(pTlv);
}

__int16 Fleech::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_42A5C0(pFrom);
}

void Fleech::VOnTrapDoorOpen()
{
    vOnTrapDoorOpen_42E5C0();
}

void Fleech::M_Sleeping_0_42F0B0()
{
    if (field_108_next_motion == -1)
    {
        if (field_20_animation.field_92_current_frame || sGnFrame_5C1B84 & 3)
        {
            if (field_20_animation.field_92_current_frame == 4 && !(sGnFrame_5C1B84 & 3))
            {
                Sound_430520(4);

                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
                {
                    auto pSnoozeParticle = alive_new<SnoozeParticle>();
                    if (pSnoozeParticle)
                    {
                        const FP yPos = (field_CC_sprite_scale * FP_FromInteger(-20)) + field_BC_ypos;
                        FP xOff = {};
                        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                        {
                            xOff = -(field_CC_sprite_scale * FP_FromInteger(10));
                        }
                        else
                        {
                            xOff = (field_CC_sprite_scale * FP_FromInteger(10));
                        }
                        pSnoozeParticle->ctor_4B06F0(xOff + field_B8_xpos, yPos, field_20_animation.field_C_render_layer, field_20_animation.field_14_scale);
                    }
                }
            }
        }
        else
        {
            Sound_430520(3);
        }
    }
    else
    {
        field_106_current_motion = field_108_next_motion;
        field_108_next_motion = -1;
    }
}

void Fleech::M_WakingUp_1_42F270()
{
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        auto pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eSnoozParticle_124)
        {
            static_cast<SnoozeParticle*>(pObj)->field_1E4_state = 2;
        }
    }

    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
            Sound_430520(6);
        }
    }
}

void Fleech::M_Unknown_2_42F2F0()
{
    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
    }
}

void Fleech::M_Idle_3_42E850()
{
    if (!CanMove_42E3E0())
    {
        if (field_108_next_motion == -1)
        {
            FP hitX = {};
            FP hitY = {};
            PathLine* pLine = nullptr;
            if (field_106_current_motion == eFleechMotions::M_Idle_3_42E850 &&
                field_20_animation.field_92_current_frame == 0 && 
                !sCollisions_DArray_5C1128->Raycast_417A60(
                    field_B8_xpos - FP_FromInteger(5),
                    field_BC_ypos - FP_FromInteger(5),
                    field_B8_xpos + FP_FromInteger(5),
                    field_BC_ypos + FP_FromInteger(1),
                    &pLine,
                    &hitX,
                    &hitY,
                    field_D6_scale != 0 ? 1 : 16))
            {
                field_138 = 0;
                field_F8_LastLineYPos = field_BC_ypos;
                field_106_current_motion = eFleechMotions::M_Fall_9_42ECD0;
            }
        }
        else
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
    }

}

void Fleech::M_Crawl_4_42E960()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_PatrolCry_5_42E810()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Sound_430520(0);
        field_13C = 1;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_42E520();
    }
}

void Fleech::M_Knockback_6_42EAF0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        ToIdle_42E520();
    }
}

void Fleech::M_StopCrawling_7_42EBB0()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_StopMidCrawlCycle_8_42EB20()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_Fall_9_42ECD0()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_Land_10_42F330()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Sound_430520(9);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_42E520();
    }
}

void Fleech::M_RaiseHead_11_42F590()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_Climb_12_42F7F0()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_SettleOnGround_13_42FB00()
{
    if (!field_20_animation.field_92_current_frame)
    {
        sub_42BAD0();

        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos - FP_FromInteger(5),
            field_BC_ypos - FP_FromInteger(5),
            field_B8_xpos + FP_FromInteger(5),
            field_BC_ypos + FP_FromInteger(5),
            &pLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16))
        {
            field_100_pCollisionLine = pLine;
            MapFollowMe_408D10(TRUE);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Fleech::ToIdle_42E520();
    }
}

void Fleech::M_ExtendTongueFromEnemy_14_42FBD0()
{
    if (field_11C_obj_id == sActiveHero_5C1B68->field_8_object_id && 
        (sActiveHero_5C1B68->CantBeDamaged_44BAB0() || sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible)))
    {
        ToIdle_42E520();
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        PullTargetIn_42BAF0();
        field_106_current_motion = eFleechMotions::M_RetractTongueFromEnemey_15_42FC40;
    }
}

void Fleech::M_RetractTongueFromEnemey_15_42FC40()
{
    if (sObjectIds_5C1B70.Find_449CF0(field_11C_obj_id) == sActiveHero_5C1B68 && 
        ((sActiveHero_5C1B68->CantBeDamaged_44BAB0()) || sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible)))
    {
        sub_42B8C0();
        ToIdle_42E520();
    }
    else
    {
        if (!sub_42B8A0())
        {
            ToIdle_42E520();
        }

        if (field_108_next_motion == eFleechMotions::M_Consume_18_42FDF0)
        {
            field_106_current_motion = eFleechMotions::M_Consume_18_42FDF0;
            field_108_next_motion = -1;
        }
    }
}

void Fleech::M_DeathByFalling_16_42FCE0()
{
    if (field_10C_health > FP_FromInteger(0))
    {
        auto pBlood = alive_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_40F0B0(field_B8_xpos, field_BC_ypos - FP_FromInteger(8), FP_FromInteger(0), -FP_FromInteger(5), field_CC_sprite_scale, 50);
        }
        
        Sound_430520(12);
        Sound_430520(7);
        
        field_10C_health = FP_FromInteger(0);
        field_124_brain_state = eFleechBrains::eAI_Death_3_42D1E0;
        field_174_flags.Set(Flags_174::eBit3);
        field_108_next_motion = -1;
        field_12C = sGnFrame_5C1B84 + 127;
        sFleechCount_5BC20E--;
    }
}

void Fleech::M_SleepingWithTongue_17_42F370()
{
    if (field_108_next_motion != -1)
    {
        sub_42B9F0();
        field_106_current_motion = field_108_next_motion;
        field_108_next_motion = -1;
    }
    else
    {
        if (field_20_animation.field_92_current_frame || sGnFrame_5C1B84 & 3)
        {
            if (field_20_animation.field_92_current_frame == 4 && !(sGnFrame_5C1B84 & 3))
            {
                Sound_430520(4);
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
                {
                    auto pSnoozeParticle = alive_new<SnoozeParticle>();
                    if (pSnoozeParticle)
                    {
                        const FP yPos = (field_CC_sprite_scale * FP_FromInteger(-20)) + field_BC_ypos;
                        FP xOff = {};
                        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                        {
                            xOff = -(field_CC_sprite_scale * FP_FromInteger(10));
                        }
                        else
                        {
                            xOff = (field_CC_sprite_scale * FP_FromInteger(10));
                        }
                        pSnoozeParticle->ctor_4B06F0(xOff + field_B8_xpos, yPos, field_20_animation.field_C_render_layer, field_20_animation.field_14_scale);
                    }
                }
            }
        }
        else
        {
            Sound_430520(3u);
        }

        field_B8_xpos = FP_FromInteger(field_160) + (Math_Cosine_496CD0(field_166_angle) * FP_FromInteger(4)) - FP_FromInteger(field_DA_xOffset);
        field_166_angle += 2;
    }
}

void Fleech::M_Consume_18_42FDF0()
{
    if (field_20_animation.field_92_current_frame == 2)
    {
        Sound_430520(2);
    }
    else if (field_20_animation.field_92_current_frame == 15 && field_11C_obj_id == sActiveHero_5C1B68->field_8_object_id)
    {
        sActiveHero_5C1B68->sub_459430();

        Sound_430520(1);

        for (int i = 0; i < 3; i++)
        {
            auto pDove = alive_new<Dove>();
            if (pDove)
            {
                pDove->ctor_41F660(5516, 41, 20u, 60, field_B8_xpos, field_BC_ypos + FP_FromInteger(10), field_CC_sprite_scale);
            }

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                pDove->field_B8_xpos += (field_CC_sprite_scale * FP_FromInteger(15));
            }
            else
            {
                pDove->field_B8_xpos -= (field_CC_sprite_scale * FP_FromInteger(15));
            }

            pDove->field_CC_sprite_scale = field_CC_sprite_scale;
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_42E520();
    }
}

__int16 Fleech::AI_Patrol_0_430BA0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Fleech::AI_ChasingAbe_1_428760()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Fleech::AI_Scared_2_42D310()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Fleech::AI_Death_3_42D1E0()
{
    field_11C_obj_id = -1;
    MusicController::sub_47FD60(0, this, 0, 0);

    if (field_12C < static_cast<int>(sGnFrame_5C1B84 + 80))
    {
        field_CC_sprite_scale -= FP_FromDouble(0.022);
        field_D0_r -= 2;
        field_D2_g -= 2;
        field_D4_b -= 2;
    }

    if (static_cast<int>(sGnFrame_5C1B84) < field_12C - 24 && !(sGnFrame_5C1B84 % 5))
    {
        const FP xRand = (FP_FromInteger(Math_RandomRange_496AB0(-24, 24)) * field_CC_sprite_scale);
        New_Particles_426C70(xRand + field_B8_xpos, field_BC_ypos - FP_FromInteger(6), field_CC_sprite_scale / FP_FromInteger(2), 2, 128u, 128u, 128u);
    }

    if (field_CC_sprite_scale < FP_FromInteger(0))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    return 100;

}

void Fleech::dtor_42A3A0()
{
    SetVTable(this, 0x544F28);
    field_170 = -1;
    if (field_118_tlvInfo != 0xFFFF)
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, field_10C_health <= FP_FromInteger(0));
    }

    MusicController::sub_47FD60(0, this, 0, 0);
    sub_42CF70();

    if (field_106_current_motion == 18)
    {
        if (sActiveHero_5C1B68)
        {
            if (field_11C_obj_id == sActiveHero_5C1B68->field_8_object_id)
            {
                sActiveHero_5C1B68->sub_459430(); // kills abe ??
            }
        }
    }
    field_11C_obj_id = -1;

    if (!field_174_flags.Get(Flags_174::eBit3))
    {
        sFleechCount_5BC20E--;
    }

    dtor_4080B0();
}

Fleech* Fleech::vdtor_42A140(signed int flags)
{
    dtor_42A3A0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Fleech::vUpdate_42AB20()
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
        field_11C_obj_id = BaseGameObject::Find_Flags_4DC170(field_11C_obj_id);
        field_170 = BaseGameObject::Find_Flags_4DC170(field_170);
        field_110_id = BaseGameObject::Find_Flags_4DC170(field_110_id);
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if ((FP_Abs(field_B8_xpos - sControlledCharacter_5C1B8C->field_B8_xpos) <= FP_FromInteger(750) &&
        FP_Abs(field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos) <= FP_FromInteger(520)) ||
        field_174_flags.Get(Flags_174::eBit7_persistant))
    {
        const auto oldMotion = field_106_current_motion;
        const auto oldBrain = sFleech_ai_table_551830[field_124_brain_state];
        const auto oldBrainRet = field_126_state;

        field_126_state = (this->*sFleech_ai_table_551830[field_124_brain_state])();

        sub_42BD30();

        if (field_BC_ypos < FP_FromInteger(0))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }

        const FP oldY = field_BC_ypos;
        const FP oldX = field_B8_xpos;

        (this->*sFleech_motion_table_551798[field_106_current_motion])();

        if (oldX != field_B8_xpos || oldY != field_BC_ypos)
        {
            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);
            VOn_TLV_Collision_4087F0(field_FC_pPathTLV);
        }

        // TODO: This is extra debug logging to figure out the ai names
        if (oldBrain != sFleech_ai_table_551830[field_124_brain_state])
        {
            LOG_INFO("Fleech: Old brain = " << GetOriginalFn(oldBrain, sFleechAiTable).fnName << " new brain = " << GetOriginalFn(sFleech_ai_table_551830[field_124_brain_state], sFleechAiTable).fnName);
        }
        else
        {
            if (oldBrainRet != field_126_state)
            {
                LOG_INFO("Fleech: Brain state = " << GetOriginalFn(sFleech_ai_table_551830[field_124_brain_state], sFleechAiTable).fnName << " State change from " << oldBrainRet << " to " << field_126_state);
            }
        }

        if (oldMotion == field_106_current_motion)
        {
            if (field_128)
            {
                field_106_current_motion = field_F4;
                SetAnim_429D80();
                field_20_animation.SetFrame_409D50(field_F6_anim_frame);
                field_128 = 0;
            }
        }
        else
        {
            SetAnim_429D80();
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void Fleech::vScreenChanged_42A4C0()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId || 
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId ||
        gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710())
    {
        field_6_flags.Set(BaseGameObject::eDead);
        field_11C_obj_id = -1;
        field_170 = -1;
    }
}

void Fleech::vOn_Tlv_Collision_42AAB0(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            field_10C_health = FP_FromInteger(0);
            field_6_flags.Set(BaseGameObject::eDead);
        }
        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv, field_B8_xpos, field_BC_ypos, field_B8_xpos, field_BC_ypos);
    }
}

TintEntry stru_551844[15] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 137u, 137u, 137u },
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
    { -1, 127u, 127u, 127u }
};

void Fleech::Init_42A170()
{
    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kFleechResID, TRUE, FALSE));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 580, TRUE, FALSE));
    
    Animation_Init_424E10(37704, 73, 35u, field_10_resources_array.ItemAt(0), 1, 1);

    field_20_animation.field_1C_fn_ptr_array = kFleech_Anim_Frame_Fns_55EFD0;

    field_4_typeId = Types::eFleech_50;

    field_6_flags.Set(BaseGameObject::eCanExplode);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_174_flags.Clear(Flags_174::eBit3);
    field_174_flags.Clear(Flags_174::eBit4);

    field_DC_bApplyShadows |= 2u;

    field_12C = 0;
    field_126_state = 0;
    field_108_next_motion = -1;
    field_110_id = -1;
    field_128 = 0;
    field_11C_obj_id = -1;
    field_170 = -1;
    field_15E = 0;
    
    SetTint_425600(&stru_551844[0], gMap_5C3030.sCurrentLevelId_5C3030);
    
    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = 34;
        field_D6_scale = 1;
    }
    else
    {
        field_20_animation.field_C_render_layer = 15;
        field_D6_scale = 0;
    }

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(24),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_BC_ypos = hitY;
    }
    
    MapFollowMe_408D10(TRUE);
    
    vStackOnObjectsOfType_425840(Types::eFleech_50);
    
    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    sFleechCount_5BC20E++;
}

void Fleech::InitPolys_42B6E0()
{
    NOT_IMPLEMENTED();
}

const static int sFleechFrameTableOffsets_5517E4[19] =
{
    37808,
    37884,
    37896,
    37704,
    37748,
    37784,
    37924,
    37936,
    37960,
    37984,
    38060,
    38112,
    38156,
    38208,
    38260,
    37848,
    38248,
    38396,
    38276
};


void Fleech::SetAnim_429D80()
{
    BYTE** ppRes = ResBlockForMotion_42A530(field_106_current_motion);
    field_20_animation.Set_Animation_Data_409C80(sFleechFrameTableOffsets_5517E4[field_106_current_motion], ppRes);
}

void Fleech::sub_42CF70()
{
    NOT_IMPLEMENTED();
}

void Fleech::sub_42B9A0(__int16 a2, __int16 a3)
{
    field_18A |= 2u;
    field_178 = 2;
    field_186 = a3;
    field_184 = a2;
    field_188 = 0;
}

void Fleech::sub_42BD30()
{
    NOT_IMPLEMENTED();
}

void Fleech::ToIdle_42E520()
{
    MapFollowMe_408D10(TRUE);
    field_138 = 0;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_106_current_motion = eFleechMotions::M_Idle_3_42E850;
    field_108_next_motion = -1;
    field_134 = 60 * sRandomBytes_546744[sFleechRandomIdx_5BC20C++] / 256 + sGnFrame_5C1B84 + 120;
}

int Fleech::Sound_430520(unsigned __int8 /*sfx*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

BYTE** Fleech::ResBlockForMotion_42A530(int /*motion*/)
{
    return field_10_resources_array.ItemAt(0);
}

__int16 Fleech::CanMove_42E3E0()
{
    if (field_108_next_motion == eFleechMotions::M_Knockback_6_42EAF0)
    {
        field_106_current_motion = eFleechMotions::M_Knockback_6_42EAF0;
        field_108_next_motion = -1;
        return 1;
    }

    if (field_108_next_motion != eFleechMotions::M_Crawl_4_42E960)
    {
        return 0;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(7));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(7));
    }

    const FP yDist = FP_FromInteger(field_CC_sprite_scale >= FP_FromInteger(1) ? 10 : 5);
    const FP xDist = ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? -1 : 1);
    if (!WallHit_408750(yDist, xDist) && !HandleEnemyStopperOrSlamDoor_42ADC0(1))
    {
        field_106_current_motion = eFleechMotions::M_Crawl_4_42E960;
        field_108_next_motion = -1;
        return 1;
    }

    ToIdle_42E520();
    return 0;
}

__int16 Fleech::HandleEnemyStopperOrSlamDoor_42ADC0(int /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

int Fleech::UpdateWakeUpSwitchValue_4308B0()
{
    const __int16 curSwitchValue = static_cast<__int16>(SwitchStates_Get_466020(field_144_wake_up_id));
    const __int16 wakeUpValue = field_148_wake_up_switch_value;
    
    if (curSwitchValue == wakeUpValue)
    {
        return 0;
    }

    if (curSwitchValue)
    {
        field_148_wake_up_switch_value = curSwitchValue;
        return (curSwitchValue - wakeUpValue) ? 1 : 0;
    }
    else
    {
        field_148_wake_up_switch_value = 0;
        return 1;
    }
}

__int16 Fleech::vTakeDamage_42A5C0(BaseGameObject* pFrom)
{
    if (field_10C_health <= FP_FromInteger(0))
    {
        return 0;
    }
    
    sub_42B8C0();
    sub_42CF70();
    
    switch (pFrom->field_4_typeId)
    {
    case Types::eBullet_15:
    case Types::eGrinder_30:
    case Types::eBaseBomb_46:
    case Types::eExplosion_109:
    case Types::eSlig_125:
    {
        Sound_430520(7);
        field_10C_health = FP_FromInteger(0);
        auto pGibsMem = alive_new<Gibs>();
        if (pGibsMem)
        {
            pGibsMem->ctor_40FB40(10, field_B8_xpos, field_BC_ypos, field_C4_velx, field_C8_vely, field_CC_sprite_scale, 0);
        }

        PSX_RECT bRect = {};
        vGetBoundingRect_424FD0(&bRect, 1);
        auto pBloodMem = alive_new<Blood>();
        if (pBloodMem)
        {
            pBloodMem->ctor_40F0B0(
                FP_FromInteger((bRect.x + bRect.w) / 2),
                FP_FromInteger((bRect.y + bRect.h) / 2),
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_CC_sprite_scale, 50);
        }

        field_6_flags.Set(BaseGameObject::eDead);
    }
    break;


    case Types::eRockSpawner_48:
        Sound_430520(7);
        field_10C_health = FP_FromInteger(0);
        field_124_brain_state = eFleechBrains::eAI_Death_3_42D1E0;
        field_108_next_motion = -1;
        field_12C = sGnFrame_5C1B84 + 127;
        field_106_current_motion = eFleechMotions::M_Idle_3_42E850;
        SetAnim_429D80();
        field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
        break;

    case Types::eParamite_96:
        Sound_430520(0xDu);
        // Fall through

    case Types::eScrab_112:
    {
        auto pGibsMem2 = alive_new<Gibs>();
        if (pGibsMem2)
        {
            pGibsMem2->ctor_40FB40(10, field_B8_xpos, field_BC_ypos, field_C4_velx, field_C8_vely, field_CC_sprite_scale, 0);
        }

        if (static_cast<BaseAliveGameObject*>(pFrom)->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            auto pBloodMem2 = alive_new<Blood>();
            if (pBloodMem2)
            {
                pBloodMem2->ctor_40F0B0(field_B8_xpos, field_BC_ypos - FP_FromInteger(8), -FP_FromInteger(5), -FP_FromInteger(5), field_CC_sprite_scale, 50);
            }
        }
        else
        {
            auto pBloodMem3 = alive_new<Blood>();
            if (pBloodMem3)
            {
                pBloodMem3->ctor_40F0B0(field_B8_xpos, field_BC_ypos - FP_FromInteger(8), FP_FromInteger(5), -FP_FromInteger(5), field_CC_sprite_scale, 50);
            }
        }

        if (!field_174_flags.Get(Flags_174::eBit4))
        {
            field_174_flags.Set(Flags_174::eBit4);
            Sound_430520(7);
        }

        field_10C_health = FP_FromInteger(0);
        field_124_brain_state = eFleechBrains::eAI_Death_3_42D1E0;
        field_106_current_motion = eFleechMotions::M_Idle_3_42E850;
        field_12C = sGnFrame_5C1B84 + 127;
        field_108_next_motion = -1;
        SetAnim_429D80();
        field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
        field_174_flags.Set(Flags_174::eBit3);
        field_6_flags.Set(BaseGameObject::eDead);
        sFleechCount_5BC20E--;
    }
    break;

    case Types::eElectrocute_150:
        field_6_flags.Set(BaseGameObject::eDead);
        field_10C_health = FP_FromInteger(0);
        field_124_brain_state = eFleechBrains::eAI_Death_3_42D1E0;
        break;

    default:
        Sound_430520(7);
        field_114_flags.Set(Flags_114::e114_Bit7_Electrocuted);
        sub_42B8C0();
        break;
    }
    return 1;
}

void Fleech::vOnTrapDoorOpen_42E5C0()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
    }
}

void Fleech::sub_42B9F0()
{
    field_178 = 1;
}

void Fleech::IncreaseAnger_430920()
{
    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        BaseAnimatedWithPhysicsGameObject* pEvent = Event_Is_Event_In_Range_422C30(kEventSpeaking, field_B8_xpos, field_BC_ypos, field_D6_scale);

        if (!pEvent)
        {
            pEvent = Event_Is_Event_In_Range_422C30(kEventAlarm, field_B8_xpos, field_BC_ypos, field_D6_scale);
        }

        if (!pEvent)
        {
            pEvent = Event_Is_Event_In_Range_422C30(kEventLoudNoise, field_B8_xpos, field_BC_ypos, field_D6_scale);
        }

        if (pEvent)
        {
            if ((pEvent != sActiveHero_5C1B68 || !sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible)) &&
                gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, pEvent->field_B8_xpos, pEvent->field_BC_ypos, 0))
            {
                field_13E_anger += field_142_attack_anger;
                if (vOnSameYLevel_425520(pEvent))
                {
                    if (pEvent->field_4_typeId == Types::eScrab_112 || pEvent->field_4_typeId == Types::eParamite_96)
                    {
                        field_14E = FP_GetExponent(pEvent->field_B8_xpos);
                    }
                    else
                    {
                        field_14C = FP_GetExponent(pEvent->field_B8_xpos);
                    }
                }
            }
        }

        pEvent = Event_Is_Event_In_Range_422C30(kEventNoise, field_B8_xpos, field_BC_ypos, field_D6_scale);
        if (pEvent)
        {
            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(6), pEvent))
            {
                if ((pEvent != sActiveHero_5C1B68 || !sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))  &&
                    gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, pEvent->field_B8_xpos, pEvent->field_BC_ypos, 0))
                {
                    field_13E_anger += field_140;
                }
            }
        }
    }
}

__int16 Fleech::InRange_4307C0(BaseAliveGameObject* pObj)
{
    if (!pObj || pObj == sActiveHero_5C1B68 && sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        return FALSE;
    }

    if (FP_Abs(pObj->field_B8_xpos - field_B8_xpos) >= ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(10) ||
        FP_Abs(pObj->field_BC_ypos - field_BC_ypos) >= ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(1) || 
        pObj->field_CC_sprite_scale != field_CC_sprite_scale)
    {
        return FALSE;
    }

    return TRUE;
}

int Fleech::sub_42B8A0()
{
    return field_18A & 1;
}

void Fleech::PullTargetIn_42BAF0()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_11C_obj_id));
    if (pTarget)
    {
        field_18A |= 3u;
        field_178 = 6;
        field_17A = 0;

        PSX_RECT bRect = {};
        pTarget->vGetBoundingRect_424FD0(&bRect, 1);

        field_17C = FP_GetExponent(pTarget->field_B8_xpos);
        field_17E = (bRect.y + bRect.h) / 2;
        field_184 = FP_GetExponent(((FP_FromInteger(field_17C) + field_B8_xpos) / FP_FromInteger(2)));
        field_188 = 0;
        field_186 = FP_GetExponent(((FP_FromInteger(field_17E) + field_BC_ypos) / FP_FromInteger(2)));
    }
}

void Fleech::sub_42B8C0()
{
    sub_42CF70();

    if (field_178 > 1)
    {
        field_178 = 10;
        field_184 = FP_GetExponent(((FP_FromInteger(field_184)) + field_B8_xpos) / FP_FromInteger(2));
        field_188 = 0;
        field_186 = FP_GetExponent(((FP_FromInteger(field_186)) + field_BC_ypos) / FP_FromInteger(2));
    }
    else
    {
        field_18A &= ~2u;
        field_178 = 1;
    }
}

void Fleech::sub_42BA10()
{
    field_18A |= 3u;
    field_178 = 3;
    field_17A = 0;
    field_184 = FP_GetExponent(((FP_FromInteger(field_160)) + field_B8_xpos) / FP_FromInteger(2));
    field_188 = 0;
    field_186 = FP_GetExponent(((FP_FromInteger(field_162)) + field_BC_ypos) / FP_FromInteger(2));
}

void Fleech::sub_42BAD0()
{
    field_178 = 5;
}

BaseAliveGameObject* Fleech::FindMudOrAbe_42CFD0()
{
    BaseAliveGameObject* pRet = nullptr;
    FP lastDist = FP_FromInteger(gPsxDisplay_5C1130.field_0_width);
    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if ((pObj->field_4_typeId == Types::eMudokon_110 || pObj->field_4_typeId == Types::eType_Abe_69) &&
            pObj->field_D6_scale == field_D6_scale &&
            pObj->field_10C_health > FP_FromInteger(0))
        {
            const FP dist = FP_FromInteger(
                Math_Distance_496EB0(
                    FP_GetExponent(pObj->field_B8_xpos),
                    FP_GetExponent(pObj->field_BC_ypos),
                    FP_GetExponent(field_B8_xpos), 
                    FP_GetExponent(field_BC_ypos)));

            if (dist < lastDist && 
                FP_GetExponent(field_B8_xpos) / 375 == (FP_GetExponent(pObj->field_B8_xpos) / 375) && 
                FP_GetExponent(field_BC_ypos) / 260 == (FP_GetExponent(pObj->field_BC_ypos) / 260))
            {
                lastDist = dist;
                pRet = pObj;
            }
        }
    }
    return pRet;
}

void Fleech::MoveAlongFloor_42E600()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));

    const FP prev_xpos = field_B8_xpos;
    const FP prev_ypos = field_BC_ypos;

    if (field_100_pCollisionLine)
    {
        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
        if (field_100_pCollisionLine && ((field_D6_scale != 0 ? 1 : 16) & (1 << field_100_pCollisionLine->field_8_type)))
        {
            if (pPlatform)
            {
                if (field_100_pCollisionLine->field_8_type != 32 && field_100_pCollisionLine->field_8_type != 36)
                {
                    pPlatform->VRemove(this);
                    field_110_id = -1;
                }
            }
            else if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
            {
                PSX_RECT bRect = {};
                vGetBoundingRect_424FD0(&bRect, 1);
                const PSX_Point xy = { bRect.x, static_cast<short>(bRect.y + 5) };
                const PSX_Point wh = { bRect.w, static_cast<short>(bRect.h + 5) };
                vOnCollisionWith_424EE0(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
            }
        }
        else if (field_124_brain_state != eFleechBrains::eAI_Patrol_0_430BA0)
        {
            VOnTrapDoorOpen();
            field_138 = 0;
            field_F8_LastLineYPos = field_BC_ypos;
            field_B8_xpos = prev_xpos + field_C4_velx;
            field_106_current_motion = eFleechMotions::M_Fall_9_42ECD0;
        }
        else
        {
            field_B8_xpos = prev_xpos;
            field_BC_ypos = prev_ypos;
            field_106_current_motion = eFleechMotions::M_Knockback_6_42EAF0;
        }
    }
    else
    {
        field_F8_LastLineYPos = prev_ypos;
        field_138 = 0;
        field_106_current_motion = eFleechMotions::M_Fall_9_42ECD0;
    }
}

