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
