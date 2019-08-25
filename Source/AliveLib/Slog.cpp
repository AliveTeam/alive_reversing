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
#include "Midi.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Bone.hpp"
#include "Sfx.hpp"
#include "Blood.hpp"
#include "SnoozeParticle.hpp"
#include "Gibs.hpp"
#include "Bullet.hpp"
#include "Particle.hpp"
#include "GameSpeak.hpp"
#include "SwitchStates.hpp"

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

Slog* Slog::ctor_4C4540(FP xpos, FP ypos, FP scale, __int16 bListenToSligs, __int16 jumpDelay)
{
    ctor_408240(5);
    SetVTable(this, 0x547578);

    field_134 = -1;

    field_BC_ypos = ypos;
    field_B8_xpos = xpos;
    
    field_CC_sprite_scale = scale;
    
    Init_4C46A0();
    
    field_160_flags.Clear(Flags_160::eBit5);
    field_12C_tlvInfo = 0xFFFF;
    field_120_brain_state_idx = 2;
    field_122_brain_state_result = 0;
    
    BaseAliveGameObject* pTarget = FindTarget_4C33C0(0, 0);
    if (!pTarget)
    {
        pTarget = sControlledCharacter_5C1B8C;
    }
    field_118 = pTarget->field_8_object_id;

    field_160_flags.Clear(Flags_160::eBit2_ListenToSligs);
    field_160_flags.Clear(Flags_160::eBit7);
    field_160_flags.Clear(Flags_160::eBit9);

    field_160_flags.Set(Flags_160::eBit2_ListenToSligs, bListenToSligs & 1);

    field_144 = 0;
    field_158_jump_delay = jumpDelay;
    field_154_angry_id = 0;
    field_106_current_motion = eSlogMotions::M_Idle_0_4C5F90;
    field_146_total_anger = 10;
    field_148_chase_anger = 20;
    field_156_bone_eating_time = 60;

    return this;
}

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
    field_160_flags.Set(Flags_160::eBit2_ListenToSligs);
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

void Slog::VOnTrapDoorOpen()
{
    vOn_TrapDoor_Open_4C5D50();
}

void Slog::VOn_TLV_Collision_4087F0(Path_TLV* pTlv)
{
    vOn_Tlv_Collision_4C5060(pTlv);
}

__int16 Slog::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_4C4B80(pFrom);
}

void Slog::vnull_4081A0(BaseGameObject* pFrom)
{
    vsub_4C4B50(pFrom);
}

void Slog::M_Idle_0_4C5F90()
{
    if (!ToNextMotion_4C5A30())
    {
        if (field_108_next_motion == eSlogMotions::M_JumpForwards_18_4C7210)
        {
            ToJump_4C5C60();
        }
        else if (field_108_next_motion == -1)
        {
            if (!field_106_current_motion)
            {
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
                {
                    SND_SEQ_PlaySeq_4CA960(13, 1, 0);
                }

                if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
                {
                    if (MusicController::sub_47FDA0(0, 0, 0) == 6)
                    {
                        MusicController::sub_47FD60(6, this, 0, 0);
                    }
                    else
                    {
                        MusicController::sub_47FD60(5, this, 0, 0);
                    }
                }
            }
        }
        else
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
    }
}

const int sSlogWalkVelXTable_5475EC[18] = // TODO: Convert values
{
    87355,
    162991,
    166294,
    129418,
    197630,
    203424,
    180786,
    205305,
    238954,
    153781,
    145622,
    142723,
    145083,
    131209,
    158174,
    143230,
    268753,
    285268
};

void Slog::M_Walk_1_4C60C0()
{
    FP velX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -FP_FromRaw(sSlogWalkVelXTable_5475EC[field_20_animation.field_92_current_frame]);
    }
    else
    {
        velX = FP_FromRaw(sSlogWalkVelXTable_5475EC[field_20_animation.field_92_current_frame]);
    }

    field_C4_velx = (field_CC_sprite_scale * velX);

    if (CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx * FP_FromInteger(4)))
    {
        ToIdle_4C5C10();

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B8_xpos += field_C4_velx + (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
        }
        else
        {
            field_B8_xpos -= (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2)) - field_C4_velx;
        }
    }
    else
    {
        MoveOnLine_4C5DA0();

        if (field_106_current_motion == eSlogMotions::M_Walk_1_4C60C0)
        {
            if (field_20_animation.field_92_current_frame == 2 || field_20_animation.field_92_current_frame == 11)
            {
                if (!field_108_next_motion)
                {
                    field_106_current_motion = eSlogMotions::M_EndWalking_9_4C6310;
                    field_108_next_motion = -1;
                }
                if (field_108_next_motion == eSlogMotions::M_TurnAround_3_4C65C0)
                {
                    field_106_current_motion = eSlogMotions::M_EndWalking_9_4C6310;
                }
            }
            else if (field_20_animation.field_92_current_frame == 5 || field_20_animation.field_92_current_frame == 14)
            {
                Sfx_4C7D30(18);

                if (!field_160_flags.Get(Flags_160::eBit8))
                {
                    field_160_flags.Set(Flags_160::eBit8);
                    MapFollowMe_408D10(FALSE);
                }

                if (field_108_next_motion == eSlogMotions::M_Run_2_4C6340)
                {
                    field_106_current_motion = eSlogMotions::M_Run_2_4C6340;
                    field_108_next_motion = -1;
                }
            }
            else
            {
                field_160_flags.Clear(Flags_160::eBit8);
            }
        }
    }
}

ALIVE_VAR(1, 0xBAF7F0, BYTE, sSlogRandomIdx_BAF7F0, 0);

static BYTE Slog_NextRandom()
{
    return sRandomBytes_546744[sSlogRandomIdx_BAF7F0++];
}


const int sSlogRunVelXTable_547634[9] = // TODO: Convert values
{
    565307,
    221956,
    230637,
    334381,
    441752,
    512611,
    550112,
    428138,
    2097152000
};

void Slog::M_Run_2_4C6340()
{
    if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(7, this, 0, 0);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = (field_CC_sprite_scale * -FP_FromRaw(sSlogRunVelXTable_547634[field_20_animation.field_92_current_frame]));
    }
    else
    {
        field_C4_velx = (field_CC_sprite_scale * FP_FromRaw(sSlogRunVelXTable_547634[field_20_animation.field_92_current_frame]));
    }

    if (CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx * FP_FromInteger(4)))
    {
        ToIdle_4C5C10();
    }
    else
    {
        MoveOnLine_4C5DA0();

        if (field_106_current_motion == 2)
        {
            if (Slog_NextRandom() < 35u)
            {
                Sfx_4C7D30(5);
            }

            if (field_20_animation.field_92_current_frame == 4 || field_20_animation.field_92_current_frame == 7)
            {
                Sfx_4C7D30(17);

                if (!field_160_flags.Get(Flags_160::eBit8))
                {
                    field_160_flags.Set(Flags_160::eBit8);
                    MapFollowMe_408D10(FALSE);
                }

                if (field_108_next_motion == eSlogMotions::M_Idle_0_4C5F90)
                {
                    field_106_current_motion = eSlogMotions::M_StopRunning_6_4C66C0;
                    field_108_next_motion = -1;
                }
                else if (field_108_next_motion == eSlogMotions::M_JumpForwards_18_4C7210)
                {
                    ToJump_4C5C60();
                    field_108_next_motion = -1;
                }
                else if (field_108_next_motion != -1)
                {
                    field_106_current_motion = field_108_next_motion;
                    field_108_next_motion = -1;
                }
            }
            else
            {
                field_160_flags.Clear(Flags_160::eBit8);
            }
        }
    }
}

void Slog::M_TurnAround_3_4C65C0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        ToIdle_4C5C10();
    }
}

void Slog::M_Fall_4_4C6930()
{
    field_C8_vely += (field_CC_sprite_scale * FP_FromDouble(1.8));
    if (field_C8_vely > (field_CC_sprite_scale * FP_FromInteger(20)))
    {
        field_C8_vely = (field_CC_sprite_scale * FP_FromInteger(20));
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (field_C4_velx > (FP_FromInteger(8) * field_CC_sprite_scale))
        {
            field_C4_velx = FP_FromInteger(8) * field_CC_sprite_scale;
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        if (field_C4_velx < -(FP_FromInteger(8) * field_CC_sprite_scale))
        {
            field_C4_velx = -(FP_FromInteger(8) * field_CC_sprite_scale);
        }
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        field_C4_velx -= field_CC_sprite_scale * field_128;
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        field_C4_velx += field_CC_sprite_scale * field_128;
        if (field_C4_velx > FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }

    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(20)),
        field_B8_xpos,
        field_BC_ypos,
        &pLine, &hitX, &hitY, 15))
    {
        switch (pLine->field_8_type)
        {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            field_100_pCollisionLine = pLine;
            field_BC_ypos = hitY;
            field_B8_xpos = hitX;
            MapFollowMe_408D10(FALSE);
            if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
            {
                PSX_RECT bRect = {};
                vGetBoundingRect_424FD0(&bRect, 1);

                const PSX_Point xy = { bRect.x, static_cast<short>(bRect.y + 5) };
                const PSX_Point wh = { bRect.w, static_cast<short>(FP_GetExponent(field_BC_ypos) + 5) };
                vOnCollisionWith_424EE0(
                    xy,
                    wh,
                    ObjList_5C1B78,
                    1,
                    (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
            }
            field_106_current_motion = eSlogMotions::M_Land_10_4C7820;
            break;

        case 1u:
        case 2u:
        case 5u:
        case 6u:
            field_BC_ypos = hitY;
            field_B8_xpos = hitX - field_C4_velx;
            MapFollowMe_408D10(FALSE);
            field_C4_velx = FP_FromInteger(0);
            break;

        default:
            return;
        }
    }
}

void Slog::M_MoveHeadUpwards_5_4C5F20()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Slog::Sfx_4C7D30(2);
        field_132 = 1;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (Math_RandomRange_496AB0(0, 100) < 30)
        {
            Sfx_4C7D30(4);
        }
        ToIdle_4C5C10();
    }
}

const int sSlogStopRunningVelX_547658[11] =
{
    227344,
    298123,
    566427,
    610336,
    425656,
    303562,
    256704,
    284098,
    239927,
    84017,
    2097152000
};

void Slog::M_StopRunning_6_4C66C0()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Sfx_4C7D30(14);
    }

    FP velX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -FP_FromRaw(sSlogStopRunningVelX_547658[field_20_animation.field_92_current_frame]);
    }
    else
    {
        velX = FP_FromRaw(sSlogStopRunningVelX_547658[field_20_animation.field_92_current_frame]);
    }

    field_C4_velx = (field_CC_sprite_scale * velX);

    if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx * FP_FromInteger(4)))
    {
        MoveOnLine_4C5DA0();
        if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return;
        }
    }

    ToIdle_4C5C10();
}

const int sSlogSlideTurnVelXTable_547684[25] = // TODO: Convert values
{
    1127088,
    780402,
    558956,
    480772,
    273168,
    348181,
    249772,
    98546,
    33064,
    -15903,
    -39069,
    -81467,
    -320580,
    2097152000,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};


void Slog::M_SlideTurn_7_4C6790()
{
    FP velX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -FP_FromRaw(sSlogSlideTurnVelXTable_547684[field_20_animation.field_92_current_frame]);
    }
    else
    {
        velX = FP_FromRaw(sSlogSlideTurnVelXTable_547684[field_20_animation.field_92_current_frame]);
    }

    field_C4_velx = (field_CC_sprite_scale * velX);
    if (CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx * FP_FromInteger(4)))
    {
        ToIdle_4C5C10();
    }
    else
    {
        MoveOnLine_4C5DA0();

        if (field_106_current_motion == eSlogMotions::M_SlideTurn_7_4C6790)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_408D10(FALSE);

                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
                    field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(3));
                }
                else
                {
                    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
                    field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(3));
                }
                field_106_current_motion = eSlogMotions::M_Run_2_4C6340;
            }
        }
    }
}

void Slog::M_StartWalking_8_4C62E0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eSlogMotions::M_Walk_1_4C60C0;
    }

    MoveOnLine_4C5DA0();
}

void Slog::M_EndWalking_9_4C6310()
{
    MoveOnLine_4C5DA0();

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_4C5C10();
    }
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
    if (field_20_animation.field_92_current_frame == 0 || field_20_animation.field_92_current_frame == 6)
    {
        Sfx_4C7D30(5);
    }

    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
    }
}

void Slog::M_Sleeping_15_4C6D60()
{
    if (field_108_next_motion != -1 && field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = field_108_next_motion;
        field_108_next_motion = -1;
        return;
    }

    bool createParticle = false;
    if (static_cast<int>(sGnFrame_5C1B84) % 60)
    {
        if (!(static_cast<int>((sGnFrame_5C1B84 - 20)) % 60))
        {
            Sfx_4C7D30(11);
            createParticle = true;
        }
    }
    else
    {
        Sfx_4C7D30(10);
        createParticle = true;
    }

    if (createParticle)
    {
        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
        {
            auto pSnoozeParticle = alive_new<SnoozeParticle>();
            if (pSnoozeParticle)
            {
                FP xOff = {};
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    xOff = -(field_CC_sprite_scale * FP_FromInteger(18));
                }
                else
                {
                    xOff = (field_CC_sprite_scale * FP_FromInteger(18));
                }
                pSnoozeParticle->ctor_4B06F0(
                    xOff + field_B8_xpos,
                    (field_CC_sprite_scale * FP_FromInteger(-13)) + field_BC_ypos,
                    field_20_animation.field_C_render_layer, field_20_animation.field_14_scale);
            }
        }
    }
}

void Slog::M_MoveHeadDownwards_16_4C70D0()
{
    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
    }

    MusicController::sub_47FD60(0, this, 0, 0);
}

void Slog::M_Bark_17_4C7000()
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
            Sfx_4C7D30(4);
        }
    }

    if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(5, this, 0, 0);
    }
}

void Slog::M_JumpForwards_18_4C7210()
{
    field_C8_vely += (field_CC_sprite_scale * FP_FromDouble(1.8));

    const FP k20Scaled = field_CC_sprite_scale * FP_FromInteger(20);
    if (field_C8_vely > k20Scaled)
    {
        field_C8_vely = k20Scaled;
    }

    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(field_B8_xpos, field_BC_ypos - k20Scaled, field_B8_xpos, field_BC_ypos, &pLine, &hitX, &hitY, 15) == 1)
    {
        switch (pLine->field_8_type)
        {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            if (field_C8_vely > FP_FromInteger(0))
            {
                field_100_pCollisionLine = pLine;
                field_108_next_motion = -1;
                field_106_current_motion = eSlogMotions::M_Run_2_4C6340;
                field_BC_ypos = hitY;
                field_C8_vely = FP_FromInteger(0);
            }
            break;

        case 1u:
        case 5u:
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
            }
            break;

        case 2u:
        case 6u:
            if (field_C4_velx > FP_FromInteger(0))
            {
                field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
            }
            break;

        default:
            break;
        }
    }

    if (field_BC_ypos - field_F8_LastLineYPos > FP_FromInteger(2))
    {
        field_128 = FP_FromDouble(0.3);
        field_F8_LastLineYPos = field_BC_ypos;
        field_106_current_motion = eSlogMotions::M_Fall_4_4C6930;
    }
}

void Slog::M_JumpUpwards_19_4C7470()
{
    PSX_RECT bRect = {};
    vGetBoundingRect_424FD0(&bRect, 1);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        FP_FromInteger(bRect.x),
        FP_FromInteger(bRect.y),
        FP_FromInteger(bRect.w),
        FP_FromInteger(bRect.h),
        &pLine, &hitX, &hitY, 8))
    {
        if (field_20_animation.field_92_current_frame < 12)
        {
            field_20_animation.SetFrame_409D50(24 - field_20_animation.field_92_current_frame);
        }
    }

    if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(6, this, 0, 0);
    }

    if (field_20_animation.field_92_current_frame == 5)
    {
        if (field_160_flags.Get(Flags_160::eBit4) &&
            field_118 == sActiveHero_5C1B68->field_8_object_id  &&
            sActiveHero_5C1B68->field_D6_scale == field_D6_scale &&
            (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_104_RockThrowStandingHold_455DF0 ||
             sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_107_RockThrowCrouchingHold_454410))
        {
            Sfx_4C7D30(13);
        }
        else
        {
            Sfx_4C7D30(6);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eSlogMotions::M_Idle_0_4C5F90;
        field_108_next_motion = -1;
    }
}

void Slog::M_Eating_20_4C75F0()
{
    SND_SEQ_Stop_4CAE60(13u);
    if (field_20_animation.field_92_current_frame == 0)
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards);
        if (field_108_next_motion != -1 && field_108_next_motion != eSlogMotions::M_Eating_20_4C75F0)
        {
            field_106_current_motion = eSlogMotions::M_Idle_0_4C5F90;
            return;
        }
    }

    if (field_20_animation.field_92_current_frame == 3 && !field_20_animation.field_4_flags.Get(AnimFlags::eBit19_LoopBackwards))
    {
        SFX_Play_46FA90(static_cast<BYTE>(Math_RandomRange_496AB0(65, 66)), 100);
        auto pBlood = alive_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_40F0B0(
                ((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) != 0 ? FP_FromInteger(-25) : FP_FromInteger(25)) * field_CC_sprite_scale + field_B8_xpos,
                field_BC_ypos - (FP_FromInteger(4) * field_CC_sprite_scale),
                FP_FromInteger(0), FP_FromInteger(0), 
                field_CC_sprite_scale, 12);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (Math_RandomRange_496AB0(0, 100) < 85)
        {
            if (static_cast<int>(sGnFrame_5C1B84) > field_150_timer && Math_RandomRange_496AB0(0, 100) < 60)
            {
                field_150_timer = sGnFrame_5C1B84 + 16;
                Sfx_4C7D30(3);
            }
            field_20_animation.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
        }
    }

    if (field_20_animation.field_92_current_frame == 0)
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards);
    }
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
    if (field_20_animation.field_92_current_frame == 4)
    {
        SND_SEQ_PlaySeq_4CA960(12u, 1, 1);
    }

    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
    }
}

void Slog::M_Growl_23_4C7170()
{
    if (field_20_animation.field_92_current_frame == 3)
    {
        if (field_132)
        {
            Sfx_4C7D30(3);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
            field_132 = 0;
            field_150_timer = sGnFrame_5C1B84 + 12;
        }
    }

    if (static_cast<int>(sGnFrame_5C1B84) > field_150_timer)
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
    }

    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
    }
}

__int16 Slog::AI_ListeningToSlig_0_4C3790()
{
    NOT_IMPLEMENTED(); 
    return 0;
}

__int16 Slog::AI_Idle_1_4C2830()
{
    BaseGameObject* pTarget = sObjectIds_5C1B70.Find_449CF0(field_118);
    
    // OG dead code - return never used
    //sObjectIds_5C1B70.Find_449CF0(field_138);

    if (pTarget && pTarget->field_6_flags.Get(BaseGameObject::eDead))
    {
        field_118 = -1;
    }

    if (field_134 != pEventSystem_5BC11C->field_28_last_event_index)
    {
        field_134 = pEventSystem_5BC11C->field_28_last_event_index;
        if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Slig_HereBoy_28 &&
            sControlledCharacter_5C1B8C->field_4_typeId == Types::eSlig_125)
        {
            field_120_brain_state_idx = 0;
            field_118 = -1;
            field_138 = sControlledCharacter_5C1B8C->field_8_object_id;
            return 0;
        }
    }

    if (SwitchStates_Get_466020(field_154_angry_id))
    {
        field_160_flags.Clear(Flags_160::eBit5);
        field_120_brain_state_idx = 2;
        return 0;
    }

    switch (field_122_brain_state_result)
    {
    case 0:
        if (field_106_current_motion != eSlogMotions::M_Sleeping_15_4C6D60 &&
            field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
        {
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
            return field_122_brain_state_result;
        }

        if (field_160_flags.Get(Flags_160::eBit7))
        {
            field_142_anger_level = 0;
            return 1;
        }
        else
        {
            field_142_anger_level = field_144;
            return 4;
        }
        break;

    case 1:
        if (Event_Is_Event_In_Range_422C30(kEventSuspiciousNoise, field_B8_xpos, field_BC_ypos, 1))
        {
            field_142_anger_level++;
        }

        if (Event_Is_Event_In_Range_422C30(kEventSpeaking, field_B8_xpos, field_BC_ypos, 1))
        {
            field_142_anger_level += Slog_NextRandom() % 8 + 15;
        }

        if (!(static_cast<int>(sGnFrame_5C1B84) % 16))
        {
            // Calm down a bit
            if (field_142_anger_level > 0)
            {
                field_142_anger_level--;
            }
        }

        if (field_142_anger_level <= field_144)
        {
            return field_122_brain_state_result;
        }

        field_108_next_motion = eSlogMotions::M_Bark_17_4C7000;
        return 2;

    case 2:
        if (field_106_current_motion != eSlogMotions::M_Bark_17_4C7000)
        {
            return field_122_brain_state_result;
        }
        field_108_next_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
        field_14C = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
        field_150_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
        return 4;

    case 3:
        if (field_106_current_motion != eSlogMotions::M_MoveHeadDownwards_16_4C70D0)
        {
            if (field_108_next_motion != eSlogMotions::M_MoveHeadDownwards_16_4C70D0)
            {
                field_108_next_motion = eSlogMotions::M_MoveHeadDownwards_16_4C70D0;
            }

            if (field_106_current_motion != eSlogMotions::M_MoveHeadDownwards_16_4C70D0)
            {
                return field_122_brain_state_result;
            }
        }
        field_108_next_motion =  eSlogMotions::M_Sleeping_15_4C6D60;
        return 1;

    case 4:
        if (Event_Is_Event_In_Range_422C30(kEventSuspiciousNoise, field_B8_xpos, field_BC_ypos, 1))
        {
            field_142_anger_level++;
        }

        if (Event_Is_Event_In_Range_422C30(kEventSpeaking, field_B8_xpos, field_BC_ypos, 1))
        {
            field_142_anger_level += Slog_NextRandom() % 8 + 15;
        }

        if (!(static_cast<int>(sGnFrame_5C1B84) % 32))
        {
            if (field_142_anger_level)
            {
                if (field_160_flags.Get(Flags_160::eBit7))
                {
                    field_142_anger_level--;
                }
            }
        }

        if (PlayerOrNakedSligNear_4C26A0())
        {
            field_142_anger_level += 2;
        }

        if (!(Slog_NextRandom() % 64) && field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
        {
            field_106_current_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
            return field_122_brain_state_result;
        }

        if (static_cast<int>(sGnFrame_5C1B84) > field_150_timer && field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
        {
            field_150_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
            field_106_current_motion = eSlogMotions::M_Growl_23_4C7170;
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
            sGnFrame_5C1B84 = sGnFrame_5C1B84;
        }

        if (static_cast<int>(sGnFrame_5C1B84) > field_14C && field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
        {
            field_14C = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
            field_106_current_motion = eSlogMotions::M_Scratch_22_4C7120;
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
        }

        if (field_142_anger_level > field_146_total_anger)
        {
            field_108_next_motion = eSlogMotions::M_Unknown_14_4C6CF0;
            field_142_anger_level = field_142_anger_level + Slog_NextRandom() % 8;
            return 5;
        }

        if (field_142_anger_level >= field_144)
        {
            return field_122_brain_state_result;
        }

        field_108_next_motion = eSlogMotions::M_MoveHeadDownwards_16_4C70D0;
        return 3;

    case 5:
        if (Event_Is_Event_In_Range_422C30(kEventSuspiciousNoise, field_B8_xpos, field_BC_ypos, 1))
        {
            field_142_anger_level++;
        }

        if (Event_Is_Event_In_Range_422C30(kEventSpeaking, field_B8_xpos, field_BC_ypos, 1))
        {
            field_142_anger_level += Math_NextRandom() % 8 + 15;
        }

        if (!(static_cast<int>(sGnFrame_5C1B84) % 2))
        {
            if (field_142_anger_level > 0)
            {
                field_142_anger_level--;
            }
        }

        if (PlayerOrNakedSligNear_4C26A0())
        {
            field_142_anger_level += 2;
        }

        if (field_142_anger_level >= field_146_total_anger)
        {
            if (field_142_anger_level <= field_148_chase_anger)
            {
                return field_122_brain_state_result;
            }
            else
            {
                field_160_flags.Clear(Flags_160::eBit5);
                field_120_brain_state_idx = 2;
                return 0;
            }
        }
        else
        {
            field_106_current_motion = eSlogMotions::M_Idle_0_4C5F90;
            field_14C = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
            field_150_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
            return 4;
        }
        break;

    default:
        return field_122_brain_state_result;
    }
}

__int16 Slog::AI_ChasingAbe_2_4C0A00()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_118));
    if (field_160_flags.Get(Flags_160::eBit2_ListenToSligs))
    {
        if (field_134 != pEventSystem_5BC11C->field_28_last_event_index)
        {
            field_134 = pEventSystem_5BC11C->field_28_last_event_index;
            if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Slig_HereBoy_28 &&
                sControlledCharacter_5C1B8C->field_4_typeId == Types::eSlig_125)
            {
                field_120_brain_state_idx = 0;
                field_118 = -1;
                field_138 = sControlledCharacter_5C1B8C->field_8_object_id;
                return 0;
            }
        }
    }

    if (!pTarget)
    {
        if (field_118 != -1)
        {
            field_118 = -1;
            field_142_anger_level = 0;
            field_120_brain_state_idx = 1;
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
            return 0;
        }
        goto LABEL_11;
    }

    if (!field_160_flags.Get(Flags_160::eBit5) || pTarget->field_CC_sprite_scale == FP_FromDouble(0.5))
    {
    LABEL_11:
        if (!field_11C)
        {
            pTarget = FindTarget_4C33C0(0, 0);
            if (!pTarget)
            {
                pTarget = FindTarget_4C33C0(0, 1);
                if (!pTarget)
                {
                    pTarget = sControlledCharacter_5C1B8C;
                    if (sControlledCharacter_5C1B8C->field_CC_sprite_scale == FP_FromDouble(0.5))
                    {
                        field_118 = -1;
                        field_142_anger_level = 0;
                        field_120_brain_state_idx = 1;
                        field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
                        return 0;
                    }
                }
            }
            field_118 = pTarget->field_8_object_id;
        }
    }
    FP v84 = {};

    switch (field_122_brain_state_result)
    {
    case 0:
        return AI_ChasingAbe_State_0();

    case 1:
        return AI_ChasingAbe_State_1();

    case 2:
        return AI_ChasingAbe_State_2(pTarget);

    case 3:
        return AI_ChasingAbe_State_3(pTarget);

    case 4:
        return AI_ChasingAbe_State_4(pTarget);

    case 7:
        return AI_ChasingAbe_State_7(pTarget);

    case 8:
        return AI_ChasingAbe_State_8();

    case 9:
        return AI_ChasingAbe_State_9();

    case 10:
        return AI_ChasingAbe_State_10();

    case 11:
    {
        auto v41 = static_cast<Bone*>(sObjectIds_5C1B70.Find_449CF0(field_15C_bone_id));
        if (!v41 || v41->VIsFalling_49E330())
        {
            field_15C_bone_id = -1;
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
            return 2;
        }

        const FP v42 = FP_Abs(field_BC_ypos - v41->field_BC_ypos);
        if (v42 <= FP_FromInteger(50) || v41->VCanBeEaten_411560())
        {
            if (!vIsFacingMe_4254A0(v41) && field_106_current_motion == eSlogMotions::M_Run_2_4C6340)
            {
                field_108_next_motion = eSlogMotions::M_SlideTurn_7_4C6790;
            }

            if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(4), v41))
            {
                goto LABEL_273;
            }

            if (FP_Abs(field_BC_ypos - v41->field_BC_ypos) < FP_FromInteger(50) && field_106_current_motion == eSlogMotions::M_Run_2_4C6340)
            {
                field_108_next_motion = eSlogMotions::M_StopRunning_6_4C66C0;
                return 12;
            }
            else
            {
            LABEL_273:
                if (field_106_current_motion == eSlogMotions::M_SlideTurn_7_4C6790)
                {
                    field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
                }

                if (field_106_current_motion == eSlogMotions::M_Walk_1_4C60C0)
                {
                    field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
                }

                if (field_106_current_motion == eSlogMotions::M_Fall_4_4C6930)
                {
                    field_15C_bone_id = -1;
                    return 9;
                }
                else
                {
                    if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
                    {
                        return field_122_brain_state_result;
                    }

                    if (vIsFacingMe_4254A0(v41))
                    {
                        FP v47 = {};
                        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                        {
                            v47 = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
                        }
                        else
                        {
                            v47 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                        }

                        if (CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), FP_FromInteger(4) * v47))
                        {
                            FP v50 = {};
                            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                            {
                                v50 = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
                            }
                            else
                            {
                                v50 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                            }

                            if (CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), FP_FromInteger(1) * v50))
                            {
                                if (Slog_NextRandom() % 32)
                                {
                                    return field_122_brain_state_result;
                                }
                                field_106_current_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
                                return field_122_brain_state_result;
                            }
                            else
                            {
                                field_108_next_motion = eSlogMotions::M_Walk_1_4C60C0;
                                return 12;
                            }
                        }
                        else
                        {
                            field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
                            return field_122_brain_state_result;
                        }
                    }
                    else
                    {
                        field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
                        return field_122_brain_state_result;
                    }
                }
            }
        }
        else
        {
            field_15C_bone_id = -1;
            return 2;
        }
        break;
    }

    case 12:
    {
        if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90 && field_106_current_motion != eSlogMotions::M_Walk_1_4C60C0)
        {
            return field_122_brain_state_result;
        }

        auto v55 = static_cast<Bone*>(sObjectIds_5C1B70.Find_449CF0(field_15C_bone_id));
        if (!v55 || v55->VIsFalling_49E330())
        {
            field_15C_bone_id = -1;
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
            return 2;
        }

        if (field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
        {
            field_108_next_motion = eSlogMotions::M_Walk_1_4C60C0;
        }

        if (FP_Abs(field_BC_ypos - v55->field_BC_ypos) <= FP_FromInteger(50) || v55->VCanBeEaten_411560())
        {
            if (vIsFacingMe_4254A0(v55))
            {
                if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromDouble(1.5), v55))
                {
                    field_108_next_motion = eSlogMotions::M_Walk_1_4C60C0;
                }

                if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromDouble(1.5), v55) ||
                    v55->field_C4_velx > FP_FromInteger(0))
                {
                    return field_122_brain_state_result;
                }
                field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
                field_124_timer = sGnFrame_5C1B84 + field_156_bone_eating_time;
                return 13;
            }
            else
            {
                field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
                return field_122_brain_state_result;
            }
        }
        else
        {
            field_15C_bone_id = -1;
            return 2;
        }
        break;
    }

    case 13:
    {
        auto v69 = static_cast<Bone*>(sObjectIds_5C1B70.Find_449CF0(field_15C_bone_id));
        if (!v69 || v69->VIsFalling_49E330())
        {
            field_15C_bone_id = -1;
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
            return 2;
        }
        else
        {
            const FP v71 = FP_Abs(field_BC_ypos - v69->field_BC_ypos);
            if (v71 <= FP_FromInteger(50) || v69->VCanBeEaten_411560())
            {
                if (vIsFacingMe_4254A0(v69))
                {
                    if (field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
                    {
                        field_108_next_motion = eSlogMotions::M_Eating_20_4C75F0;
                    }

                    if (field_124_timer > static_cast<int>(sGnFrame_5C1B84))
                    {
                        return field_122_brain_state_result;
                    }

                    v69->field_6_flags.Set(BaseGameObject::eDead);
                    field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
                    field_15C_bone_id = -1;
                    return 2;
                }
                else
                {
                    field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
                    return 12;
                }
            }
            else
            {
                field_15C_bone_id = -1;
                return 2;
            }
        }
        break;
    }

    case 14:
    {
        auto v62 = static_cast<Bone*>(sObjectIds_5C1B70.Find_449CF0(field_15C_bone_id));
        if (!v62)
        {
            return 0;
        }

        const FP v63 = FP_Abs(field_BC_ypos - v62->field_BC_ypos);
        if (v63 <= FP_FromInteger(50) || v62->VCanBeEaten_411560())
        {
            if (v62->VIsFalling_49E330())
            {
                field_15C_bone_id = -1;
                field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
                return 2;
            }
            else
            {
                if (field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
                {
                    FP v64 = {};
                    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        v64 = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
                    }
                    else
                    {
                        v64 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                    }

                    if (CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), v64 * FP_FromInteger(2)))
                    {
                        field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
                    }
                    else
                    {
                        field_108_next_motion = eSlogMotions::M_Walk_1_4C60C0;
                    }
                }

                if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) *  FP_FromInteger(1), v62))
                {
                    return field_122_brain_state_result;
                }
                return 12;
            }
        }
        else
        {
            field_15C_bone_id = -1;
            return 2;
        }
        break;
    }

    case 15:
        if (field_C4_velx > FP_FromInteger(0) && HandleEnemyStopper_4C5340())
        {
            field_108_next_motion = eSlogMotions::M_StopRunning_6_4C66C0;
            field_160_flags.Set(Flags_160::eBit1, field_C4_velx < FP_FromInteger(0));
            field_14C = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
            field_150_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
            return 20;
        }
        else
        {
            if (!vIsFacingMe_4254A0(pTarget) && field_106_current_motion == eSlogMotions::M_Run_2_4C6340)
            {
                field_108_next_motion = eSlogMotions::M_SlideTurn_7_4C6790;
            }

            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), pTarget))
            {
                if (pTarget->field_CC_sprite_scale == FP_FromInteger(1) &&
                    field_106_current_motion == eSlogMotions::M_Run_2_4C6340)
                {
                    if (vIsFacingMe_4254A0(pTarget))
                    {
                        field_108_next_motion = eSlogMotions::M_StopRunning_6_4C66C0;
                    }
                }
            }
            if (field_106_current_motion == eSlogMotions::M_SlideTurn_7_4C6790)
            {
                field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
            }

            if (field_BC_ypos >= pTarget->field_BC_ypos + FP_FromInteger(50))
            {
                auto v75 = FindBone_4C25B0();
                if (v75)
                {
                    field_15C_bone_id = v75->field_8_object_id;
                    return 11;
                }
                else
                {
                    if (field_106_current_motion == eSlogMotions::M_Fall_4_4C6930)
                    {
                        return 9;
                    }
                    else
                    {
                        if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
                        {
                            return field_122_brain_state_result;
                        }

                        if (vIsFacingMe_4254A0(pTarget))
                        {
                            if (field_15A < 100 &&
                                vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), pTarget))
                            {
                                field_108_next_motion = eSlogMotions::M_JumpUpwards_19_4C7470;
                                return 16;
                            }
                            else
                            {
                                if (Math_RandomRange_496AB0(0, 100) < 20)
                                {
                                    field_106_current_motion = eSlogMotions::M_Growl_23_4C7170;
                                }
                                field_108_next_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
                                return 18;
                            }
                        }
                        else
                        {
                            field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
                            return field_122_brain_state_result;
                        }
                    }
                }
            }
            else
            {
                field_124_timer = sGnFrame_5C1B84 + field_158_jump_delay;
                return 17;
            }
        }
        break;

    case 16:
        if (field_106_current_motion)
        {
            return field_122_brain_state_result;
        }
        field_15A += Slog_NextRandom() % 64;
        return 15;

    case 17:
        if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
        {
            goto LABEL_241;
        }

        if (!vIsFacingMe_4254A0(pTarget))
        {
            field_108_next_motion = eSlogMotions::M_TurnAround_3_4C65C0;
            return field_122_brain_state_result;
        }
        field_108_next_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
    LABEL_241:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_124_timer)
        {
            return field_122_brain_state_result;
        }
        if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), pTarget))
        {
            field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
        }
        return 2;

    case 18:
        if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
        {
            return field_122_brain_state_result;
        }

        field_15A -= 20;

        if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), pTarget))
        {
            return 15;
        }
        field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
        return 2;

    case 19:
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            v84 = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            v84 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }

        if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), FP_FromInteger(2) * v84))
        {
            return 2;
        }

        if (pTarget)
        {
            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), pTarget))
            {
                if (pTarget->field_10C_health > FP_FromInteger(0) && vOnSameYLevel_425520(pTarget))
                {
                    field_108_next_motion = eSlogMotions::M_JumpForwards_18_4C7210;
                    return 4;
                }
            }
        }

        if (!(Slog_NextRandom() % 64))
        {
            field_106_current_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
            return field_122_brain_state_result;
        }
        // Fall through
    }

    case 20:
    {
        auto v89 = FindBone_4C25B0();
        if (v89)
        {
            field_15C_bone_id = v89->field_8_object_id;
            return 11;
        }

        if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
        {
            return field_122_brain_state_result;
        }

        if (field_160_flags.Get(Flags_160::eBit1))
        {
            if (pTarget->field_B8_xpos > field_B8_xpos)
            {
                return 2;
            }
        }
        else if (pTarget->field_B8_xpos < field_B8_xpos)
        {
            return 2;
        }

        if (static_cast<int>(sGnFrame_5C1B84) > field_150_timer)
        {
            field_150_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
            field_106_current_motion = eSlogMotions::M_Growl_23_4C7170;
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
        }

        if (static_cast<int>(sGnFrame_5C1B84) <= field_14C)
        {
            return field_122_brain_state_result;
        }
        field_14C = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
        field_106_current_motion = eSlogMotions::M_Scratch_22_4C7120;
        field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
        return field_122_brain_state_result;
    }

    default:
        return field_122_brain_state_result;
    }
}

__int16 Slog::AI_ChasingAbe_State_10()
{
    if (field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
    {
        field_108_next_motion = eSlogMotions::M_JumpUpwards_19_4C7470;
    }

    if (static_cast<int>(sGnFrame_5C1B84) <= field_124_timer)
    {
        auto pBone = FindBone_4C25B0();
        if (pBone)
        {
            field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
            field_15C_bone_id = pBone->field_8_object_id;
            return 11;
        }

        if (field_106_current_motion == eSlogMotions::M_StopRunning_6_4C66C0)
        {
            return field_122_brain_state_result;
        }

        if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
        {
            return field_122_brain_state_result;
        }

        if (Slog_NextRandom() % 16)
        {
            return field_122_brain_state_result;
        }

        field_108_next_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
        return field_122_brain_state_result;
    }

    field_160_flags.Clear(Flags_160::eBit4);
    field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
    return 2;
}

__int16 Slog::AI_ChasingAbe_State_9()
{
    if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
    {
        return field_122_brain_state_result;
    }
    field_106_current_motion = eSlogMotions::M_Run_2_4C6340;
    return 2;
}

__int16 Slog::AI_ChasingAbe_State_8()
{
    if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
    {
        return field_122_brain_state_result;
    }
    field_142_anger_level = 0;
    field_120_brain_state_idx = 1;
    field_11C = 0;
    return 0;
}

__int16 Slog::AI_ChasingAbe_State_7(BaseAliveGameObject* pTarget)
{
    if (static_cast<int>(sGnFrame_5C1B84) <= field_124_timer && pTarget->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
        {
            return field_122_brain_state_result;
        }

        field_106_current_motion = eSlogMotions::M_Eating_20_4C75F0;
        return field_122_brain_state_result;
    }

    field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
    return 8;
}

__int16 Slog::AI_ChasingAbe_State_4(BaseAliveGameObject* pTarget)
{
    if (field_106_current_motion == eSlogMotions::M_Run_2_4C6340)
    {
        if (pTarget->field_10C_health > FP_FromInteger(0))
        {
            field_124_timer = Math_RandomRange_496AB0(1, 3) + sGnFrame_5C1B84 + field_158_jump_delay;
            return 1;
        }

        if (FP_Abs(pTarget->field_B8_xpos - field_B8_xpos) > ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2))
        {
            field_124_timer = Math_RandomRange_496AB0(1, 3) + sGnFrame_5C1B84 + field_158_jump_delay;
            return 1;
        }

        if (FP_Abs(pTarget->field_BC_ypos - field_BC_ypos) > ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2))
        {
            field_124_timer = Math_RandomRange_496AB0(1, 3) + sGnFrame_5C1B84 + field_158_jump_delay;
            return 1;
        }

        if (vIsFacingMe_4254A0(pTarget))
        {
            field_106_current_motion = eSlogMotions::M_Eating_20_4C75F0;
            field_108_next_motion = -1;
        }
        else
        {
            field_106_current_motion = eSlogMotions::M_TurnAround_3_4C65C0;
            field_108_next_motion = eSlogMotions::M_Eating_20_4C75F0;
        }

        field_124_timer = sGnFrame_5C1B84 + 90;
        return 7;
    }

    if (field_106_current_motion != eSlogMotions::M_Fall_4_4C6930)
    {
        return field_122_brain_state_result;
    }

    return 9;
}

__int16 Slog::AI_ChasingAbe_State_3(BaseAliveGameObject* pTarget)
{
    if (field_106_current_motion != eSlogMotions::M_Idle_0_4C5F90)
    {
        field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
        return field_122_brain_state_result;
    }

    if (Slog_NextRandom() % 64)
    {
        if (static_cast<int>(sGnFrame_5C1B84) > field_150_timer)
        {
            field_150_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
            field_106_current_motion = eSlogMotions::M_Growl_23_4C7170;
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
        }

        if (static_cast<int>(sGnFrame_5C1B84) > field_14C)
        {
            field_14C = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
            field_106_current_motion = eSlogMotions::M_Scratch_22_4C7120;
            field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
        }

        if (pTarget->field_CC_sprite_scale != FP_FromInteger(1))
        {
            return field_122_brain_state_result;
        }
        return 2;
    }

    field_106_current_motion = eSlogMotions::M_MoveHeadUpwards_5_4C5F20;
    return field_122_brain_state_result;
}

__int16 Slog::AI_ChasingAbe_State_2(BaseAliveGameObject* pTarget)
{
    if (field_C4_velx > FP_FromInteger(0) && HandleEnemyStopper_4C5340())
    {
        field_108_next_motion = eSlogMotions::M_StopRunning_6_4C66C0;
        field_160_flags.Set(Flags_160::eBit1, field_C4_velx < FP_FromInteger(0));
        field_14C = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
        field_150_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
        return 20;
    }

    if (CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx * FP_FromInteger(4)))
    {
        field_108_next_motion = eSlogMotions::M_StopRunning_6_4C66C0;
        field_160_flags.Set(Flags_160::eBit1, field_C4_velx < FP_FromInteger(0));
        field_14C = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
        field_150_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
        return 19;
    }

    if (!vIsFacingMe_4254A0(pTarget) && field_106_current_motion == eSlogMotions::M_Run_2_4C6340)
    {
        field_108_next_motion = eSlogMotions::M_SlideTurn_7_4C6790;
    }

    if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), pTarget))
    {
        if (vOnSameYLevel_425520(pTarget))
        {
            if (vIsFacingMe_4254A0(pTarget))
            {
                if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), pTarget->field_B8_xpos - field_B8_xpos) &&
                    !field_160_flags.Get(Flags_160::eBit9))
                {
                    if (pTarget->field_10C_health <= FP_FromInteger(0))
                    {
                        field_108_next_motion = eSlogMotions::M_Idle_0_4C5F90;
                    }
                    else
                    {
                        field_108_next_motion = eSlogMotions::M_JumpForwards_18_4C7210;
                    }
                }
            }
        }
    }

    if (field_106_current_motion == eSlogMotions::M_SlideTurn_7_4C6790)
    {
        field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
    }

    auto pBone = FindBone_4C25B0();
    if (pBone)
    {
        field_15C_bone_id = pBone->field_8_object_id;
        return 11;
    }

    if (field_106_current_motion == eSlogMotions::M_Idle_0_4C5F90)
    {
        if (vIsFacingMe_4254A0(pTarget))
        {
            FP gridSizeDirected = {};
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSizeDirected = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                gridSizeDirected = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }

            if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), FP_FromInteger(2) * gridSizeDirected))
            {
                if (pTarget->field_10C_health > FP_FromInteger(0))
                {
                    field_124_timer = Math_RandomRange_496AB0(1, 3) + sGnFrame_5C1B84 + field_158_jump_delay;
                    return 1;
                }

                if (FP_Abs(pTarget->field_B8_xpos - field_B8_xpos) > (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2)))
                {
                    field_124_timer = Math_RandomRange_496AB0(1, 3) + sGnFrame_5C1B84 + field_158_jump_delay;
                    return 1;
                }

                if (FP_Abs(pTarget->field_BC_ypos - field_BC_ypos) > (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2)))
                {
                    field_124_timer = Math_RandomRange_496AB0(1, 3) + sGnFrame_5C1B84 + field_158_jump_delay;
                    return 1;
                }
                field_124_timer = sGnFrame_5C1B84 + 90;
                return 7;
            }

            field_160_flags.Set(Flags_160::eBit1, field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX));

            field_14C = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
            field_150_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
            return 19;
        }
        field_106_current_motion = eSlogMotions::M_TurnAround_3_4C65C0;
    }

    if (field_160_flags.Get(Flags_160::eBit4) &&
        pTarget == sActiveHero_5C1B68 &&
        pTarget->field_D6_scale == field_D6_scale &&
        (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_104_RockThrowStandingHold_455DF0 ||
            sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_107_RockThrowCrouchingHold_454410))
    {
        field_108_next_motion = eSlogMotions::M_StopRunning_6_4C66C0;
        field_124_timer = sGnFrame_5C1B84 + 90;
        return 10;
    }

    if (field_106_current_motion == eSlogMotions::M_Fall_4_4C6930)
    {
        return 9;
    }

    if (field_106_current_motion == eSlogMotions::M_JumpForwards_18_4C7210)
    {
        return 4;
    }

    if (field_BC_ypos <= pTarget->field_BC_ypos + FP_FromInteger(50))
    {
        if (pTarget->field_CC_sprite_scale != FP_FromDouble(0.5))
        {
            return field_122_brain_state_result;
        }
        field_150_timer = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 60;
        field_14C = Math_NextRandom() % 32 + sGnFrame_5C1B84 + 120;
        return 3;
    }

    return 15;
}

__int16 Slog::AI_ChasingAbe_State_1()
{
    if (field_124_timer > static_cast<int>(sGnFrame_5C1B84))
    {
        return field_122_brain_state_result;
    }
    field_108_next_motion = eSlogMotions::M_Run_2_4C6340;
    return 2;
}

__int16 Slog::AI_ChasingAbe_State_0()
{
    field_11C = 0;
    field_15A = 0;
    field_15C_bone_id = -1;
    field_124_timer = Math_RandomRange_496AB0(1, 3) + sGnFrame_5C1B84 + field_158_jump_delay;
    Sfx_4C7D30(8);
    return 1;
}

__int16 Slog::AI_Death_3_4C3250()
{
    field_138 = -1;
    field_118 = -1;

    if (field_124_timer < static_cast<int>(sGnFrame_5C1B84 + 80))
    {
        field_CC_sprite_scale -= FP_FromDouble(0.023);
        field_D0_r -= 2;
        field_D2_g -= 2;
        field_D4_b -= 2;
    }

    if (static_cast<int>(sGnFrame_5C1B84) < field_124_timer - 24 && !(sGnFrame_5C1B84 % 5))
    {
        New_Particles_426C70(
            (FP_FromInteger(Math_RandomRange_496AB0(-24, 24)) * field_CC_sprite_scale) + field_B8_xpos,
            field_BC_ypos - FP_FromInteger(6),
            field_CC_sprite_scale / FP_FromInteger(2),
            2, 128u, 128u, 128u);
        SFX_Play_46FBA0(79u, 25, FP_GetExponent(FP_FromInteger(2200) * field_CC_sprite_scale));
    }

    if (field_CC_sprite_scale < FP_FromInteger(0))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    return 100;
}

const int sSlogFrameOffsetTable_5609D8[24] =
{
    96464,
    96344,
    96424,
    96764,
    96692,
    96496,
    96532,
    96580,
    96640,
    96660,
    96876,
    96716,
    96728,
    96752,
    15068,
    15108,
    15156,
    15132,
    38904,
    38960,
    39064,
    12412,
    12724,
    12812
};


BYTE** Slog::ResBlockForMotion_4C4A80(__int16 motion)
{
    if (motion < eSlogMotions::M_Unknown_14_4C6CF0)
    {
        field_130 = 0;
        return field_10_resources_array.ItemAt(field_130);
    }

    if (motion < eSlogMotions::M_JumpForwards_18_4C7210)
    {
        field_130 = 1;
        return field_10_resources_array.ItemAt(field_130);
    }

    if (motion < eSlogMotions::M_Unknown_21_4C77F0)
    {
        field_130 = 2;
        return field_10_resources_array.ItemAt(field_130);
    }

    if (motion < eSlogMotions::M_Scratch_22_4C7120)
    {
        field_130 = 3;
        return field_10_resources_array.ItemAt(field_130);
    }

    if (motion < 24) // last + 1
    {
        field_130 = 4;
        return field_10_resources_array.ItemAt(field_130);
    }
    else
    {
        field_130 = 0;
        return field_10_resources_array.ItemAt(field_130);
    }
}

void Slog::SetAnimFrame_4C42A0()
{
    BYTE** ppRes = ResBlockForMotion_4C4A80(field_106_current_motion);
    field_20_animation.Set_Animation_Data_409C80(sSlogFrameOffsetTable_5609D8[field_106_current_motion], ppRes);
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
    field_15C_bone_id = -1;
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
        field_15C_bone_id = BaseGameObject::Find_Flags_4DC170(field_15C_bone_id);
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
    field_15C_bone_id = -1;

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
    field_128 = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_106_current_motion = 0;
    field_108_next_motion = -1;
}

void Slog::Sfx_4C7D30(int /*effectId*/)
{
    NOT_IMPLEMENTED();
}

void Slog::ToJump_4C5C60()
{
    field_C4_velx = (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromDouble(-10.18) : FP_FromDouble(10.18)) * field_CC_sprite_scale;
    field_C8_vely = FP_FromInteger(-8) * field_CC_sprite_scale;
    
    field_F8_LastLineYPos = field_BC_ypos;

    VOnTrapDoorOpen();

    field_106_current_motion = eSlogMotions::M_JumpForwards_18_4C7210;
    field_100_pCollisionLine = nullptr;

    Sfx_4C7D30(8);

    if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(7, this, 0, 0);
    }
}

__int16 Slog::ToNextMotion_4C5A30()
{
    switch (field_108_next_motion)
    {
    case eSlogMotions::M_TurnAround_3_4C65C0:
        field_106_current_motion = eSlogMotions::M_TurnAround_3_4C65C0;
        field_108_next_motion = -1;
        return 1;

    case eSlogMotions::M_Walk_1_4C60C0:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
        }
        else
        {
            field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
        }

        if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), (field_C4_velx * FP_FromInteger(9)) * FP_FromInteger(2)))
        {
            field_106_current_motion = eSlogMotions::M_StartWalking_8_4C62E0;
            field_108_next_motion = -1;
            return 1;
        }
        ToIdle_4C5C10();
        return 0;

    case eSlogMotions::M_Run_2_4C6340:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(3));
        }
        else
        {
            field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(3));
        }

        if (!CollisionCheck_4C5480(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx * FP_FromInteger(4)))
        {
            field_106_current_motion = eSlogMotions::M_Run_2_4C6340;
            field_108_next_motion = -1;
            return 1;
        }
        ToIdle_4C5C10();
        return 0;
    }
    return 0;
}

BOOL Slog::CollisionCheck_4C5480(FP hitY, FP hitX)
{
    PathLine* pLine = nullptr;
    return sCollisions_DArray_5C1128->Raycast_417A60(field_B8_xpos, field_BC_ypos - hitY, field_B8_xpos + hitX, field_BC_ypos - hitY, &pLine, &hitX, &hitY, 6) != 0;
}

void Slog::MoveOnLine_4C5DA0()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    const FP oldXPos = field_B8_xpos;

    if (field_100_pCollisionLine)
    {
        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
        if (field_100_pCollisionLine)
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
        else
        {
            VOnTrapDoorOpen();
            field_128 = FP_FromDouble(0.3);
            field_F8_LastLineYPos = field_BC_ypos;
            field_B8_xpos = oldXPos + field_C4_velx;
        }
    }
    else
    {
        field_128 = FP_FromDouble(0.3);
        field_F8_LastLineYPos = field_BC_ypos;
        field_106_current_motion = eSlogMotions::M_Fall_4_4C6930;
    }
}

Bone* Slog::FindBone_4C25B0()
{
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        auto pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eBone_11)
        {
            auto pBone = static_cast<Bone*>(pObj);
            if (pBone->VCanThrow_49E350())
            {
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(pBone->field_C2_lvl_number, pBone->field_C0_path_number, pBone->field_B8_xpos, pBone->field_BC_ypos, 0) &&
                    pBone->field_D6_scale == field_D6_scale)
                {
                    if (FP_Abs(field_BC_ypos - pBone->field_BC_ypos) <= FP_FromInteger(50) || pBone->VCanBeEaten_411560())
                    {
                        return pBone;
                    }
                }
            }
        }
    }
    return nullptr;
}

BaseAliveGameObject* Slog::FindTarget_4C33C0(__int16 /*a2*/, __int16 /*a3*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void Slog::vOn_TrapDoor_Open_4C5D50()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
        field_106_current_motion = eSlogMotions::M_Fall_4_4C6930;
    }
}

void Slog::vOn_Tlv_Collision_4C5060(Path_TLV* pTlv)
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

__int16 Slog::vTakeDamage_4C4B80(BaseGameObject* pFrom)
{
    if (field_10C_health <= FP_FromInteger(0))
    {
        return 0;
    }

    switch (pFrom->field_4_typeId)
    {
    case Types::eBullet_15:
    {
        auto pBullet = static_cast<Bullet*>(pFrom);
        switch (pBullet->field_20_type)
        {
        case BulletType::Type_0:
        case BulletType::Type_2:
            if (pBullet->field_30 <= FP_FromInteger(0))
            {
                auto pBlood = alive_new<Blood>();
                if (pBlood)
                {
                    pBlood->ctor_40F0B0(
                        field_B8_xpos,
                        pBullet->field_2C_ypos,
                        FP_FromInteger(-24),
                        FP_FromInteger(0),
                        field_CC_sprite_scale, 50);
                }
            }
            else
            {
                auto pBlood = alive_new<Blood>();
                if (pBlood)
                {
                    pBlood->ctor_40F0B0(
                        field_B8_xpos,
                        pBullet->field_2C_ypos,
                        FP_FromInteger(24),
                        FP_FromInteger(0),
                        field_CC_sprite_scale, 50);
                }
            }
            break;

        case BulletType::Type_1:
        case BulletType::Type_3:
        {
            auto pBlood = alive_new<Blood>();
            if (pBlood)
            {
                pBlood->ctor_40F0B0(
                    field_B8_xpos,
                    field_BC_ypos - (FP_FromInteger(20) * field_CC_sprite_scale),
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_CC_sprite_scale, 50);
            }
            break;
        }

        default:
            break;
        }

        Sfx_4C7D30(9);
        field_10C_health = FP_FromInteger(0);
        field_120_brain_state_idx = 3;
        field_106_current_motion = eSlogMotions::M_Unknown_21_4C77F0;
        field_124_timer = sGnFrame_5C1B84 + 90;
        SetAnimFrame_4C42A0();
        field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
        field_160_flags.Set(Flags_160::eBit3);
        sSlogCount_BAF7F2--;
        break;
    }

    case Types::eGrinder_30:
    case Types::eBaseBomb_46:
    case Types::eExplosion_109:
    {
        Sfx_4C7D30(9);
        field_10C_health = FP_FromInteger(0);
        auto pGibs = alive_new<Gibs>();
        if (pGibs)
        {
            pGibs->ctor_40FB40(2, field_B8_xpos, field_BC_ypos, field_C4_velx, field_C8_vely, field_CC_sprite_scale, 0);
        }

        PSX_RECT bRect = {};
        vGetBoundingRect_424FD0(&bRect, 1);
        auto pBlood = alive_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_40F0B0(
                FP_FromInteger((bRect.x + bRect.w) / 2),
                FP_FromInteger((bRect.y + bRect.h) / 2),
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_CC_sprite_scale, 50);
        }

        field_6_flags.Set(BaseGameObject::eDead);
        break;
    }

    case Types::eElectricWall_39:
        Sfx_4C7D30(9);
        field_160_flags.Set(Flags_160::eBit13);
        break;

    case Types::eRockSpawner_48:
    case Types::eMineCar_89:
        Sfx_4C7D30(9);
        field_10C_health = FP_FromInteger(0);
        field_120_brain_state_idx = 3;
        field_106_current_motion = eSlogMotions::M_Unknown_21_4C77F0;
        field_124_timer = sGnFrame_5C1B84 + 90;
        SetAnimFrame_4C42A0();
        field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
        break;

    case Types::eType_104:
        if (!field_160_flags.Get(Flags_160::eBit6))
        {
            field_160_flags.Set(Flags_160::eBit6);
            Sfx_4C7D30(9);
        }
        break;

    case Types::eElectrocute_150:
        field_10C_health = FP_FromInteger(0);
        field_120_brain_state_idx =  3;
        field_6_flags.Set(BaseGameObject::eDead);
        break;

    default:
        return 1;
    }
    return 1;
}

void Slog::vsub_4C4B50(BaseGameObject* /*pFrom*/)
{
    field_142_anger_level += field_148_chase_anger;// on throwable hit?
}

__int16 Slog::PlayerOrNakedSligNear_4C26A0()
{
    const FP kMinXDist = FP_FromInteger(100) * field_CC_sprite_scale;
    const FP kMinYDist = FP_FromInteger(25) * field_CC_sprite_scale;

    // Is the player near?
    if (FP_Abs(sActiveHero_5C1B68->field_B8_xpos - field_B8_xpos) < kMinXDist &&
        FP_Abs(sActiveHero_5C1B68->field_BC_ypos - field_BC_ypos) < kMinYDist &&
        sActiveHero_5C1B68->field_CC_sprite_scale == FP_FromInteger(1))
    {
        return 1;
    }

    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eCrawlingSlig_26)
        {
            // Is this naked slig near?
            if (FP_Abs(pObj->field_B8_xpos - field_B8_xpos) < kMinXDist &&
                FP_Abs(pObj->field_BC_ypos - field_BC_ypos) < kMinYDist &&
                pObj->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                return 1;
            }
        }
    }
    return 0;
}

void Slog::DelayedResponse_4C3750(__int16 a2)
{
    field_140 = 0;
    field_13E = a2;
    field_124_timer = sGnFrame_5C1B84 + 10;
}

__int16 Slog::HandleEnemyStopper_4C5340()
{
    NOT_IMPLEMENTED();
    return 0;
}
