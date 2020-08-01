#include "stdafx_ao.h"
#include "Function.hpp"
#include "Slog.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"
#include "Math.hpp"
#include "Game.hpp"

void Slog_ForceLink() {}

START_NS_AO


using TSlogStateFunction = decltype(&Slog::State_0_Idle_4742E0);

const TSlogStateFunction sSlogMotionTable_4CFD30[] =
{
    &Slog::State_0_Idle_4742E0,
    &Slog::State_1_Walk_4743F0,
    &Slog::State_2_Run_4749A0,
    &Slog::State_3_TurnAround_474C70,
    &Slog::State_4_Fall_4750C0,
    &Slog::State_5_Unknown_474070,
    &Slog::State_6_MoveHeadUpwards_474220,
    &Slog::State_7_SlideTurn_474DB0,
    &Slog::State_8_StopRunning_474EC0,
    &Slog::State_9_StartWalking_474690,
    &Slog::State_10_EndWalking_4747D0,
    &Slog::State_11_Land_475AB0,
    &Slog::State_12_Unknown_475B50,
    &Slog::State_13_StartFastBarking_475B70,
    &Slog::State_14_EndFastBarking_475BB0,
    &Slog::State_15_Empty_475290,
    &Slog::State_16_Sleeping_4752E0,
    &Slog::State_17_FallAsleep_475510,
    &Slog::State_18_WakeUp_475460,
    &Slog::State_19_JumpForwards_475610,
    &Slog::State_20_JumpUpwards_475890,
    &Slog::State_21_Eating_475900,
    &Slog::State_22_Empty_475A90,
    &Slog::State_23_Scratch_475550,
    &Slog::State_24_Growl_475590,
};

const int dword_4CFDD8[6] = { 14784, 14832, 14808, 37952, 38008, 38112 };

ALIVE_VAR(1, 0x9F11C8, short, gNumSlogs_9F11C8, 0);
ALIVE_VAR(1, 0x9F11C4, BYTE, sSlogRndSeed_9F11C4, 0);

static BYTE Slog_NextRandom()
{
    return sRandomBytes_4BBE30[sSlogRndSeed_9F11C4++];
}

Slog* Slog::ctor_472EE0(Path_Slog* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BCBC8);

    field_148 = -1;

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

    if (pTlv->field_18_scale == 0)
    {
        field_BC_sprite_scale = FP_FromInteger(1);
    }
    else
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
    }

    Init_473130();

    field_138_tlvInfo = tlvInfo;
    field_114_brain_idx = 1;
    field_176 = 1;

    field_158_bark_anger = pTlv->field_1E_bark_anger;

    field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_1A_direction == 0);

    field_15A = pTlv->field_1E_bark_anger + pTlv->field_20_sleeps + pTlv->field_22_chase_anger;
    field_15C = pTlv->field_1E_bark_anger + pTlv->field_20_sleeps + pTlv->field_22_chase_anger;
    field_10C = 0;
    field_17E = pTlv->field_1C_wakeup_anger;
    field_170 = pTlv->field_24_jump_attack_delay;
    field_168 = pTlv->field_28_anger_trigger_id;

    if (pTlv->field_1C_wakeup_anger)
    {
        field_FC_current_motion = eSlogStates::State_16_Sleeping_4752E0;
        field_13C_res_idx = 1;
        field_10_anim.Set_Animation_Data_402A40(dword_4CFDD8[0], field_180_resources[2]);
    }
    else
    {
        field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
    }
    return this;
}

Slog* Slog::ctor_473050(FP xpos, FP ypos, FP scale)
{
    ctor_401090();
    SetVTable(this, 0x4BCBC8);

    field_148 = -1;

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;
    field_BC_sprite_scale = scale;

    Init_473130();

    field_116_brain_state = 0;

    field_10C = sControlledCharacter_50767C;
    field_176 = 0;
    sControlledCharacter_50767C->field_C_refCount++;
    field_17E = 0;
    field_158_bark_anger = 0;

    field_170 = 0;
    field_168 = 0;
    field_FC_current_motion = 0;
    field_138_tlvInfo = 0xFFFF;
    field_114_brain_idx = 2;
    field_15A = 10;
    field_15C = 20;

    return this;
}

BaseGameObject* Slog::VDestructor(signed int flags)
{
    return Vdtor_473CB0(flags);
}

Slog* Slog::Vdtor_473CB0(signed int flags)
{
    dtor_473370();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* Slog::dtor_473370()
{
    SetVTable(this, 0x4BCBC8);

    if (field_10C)
    {
        field_10C->field_C_refCount--;
        field_10C = nullptr;
    }

    if (field_14C)
    {
        field_14C->field_C_refCount--;
        field_14C = nullptr;
    }

    if (field_16C)
    {
        field_16C->field_C_refCount--;
        field_16C = nullptr;
    }

    if (field_138_tlvInfo != 0xFFFF)
    {
        if (field_100_health <= FP_FromInteger(0))
        {
            gMap_507BA8.TLV_Reset_446870(field_138_tlvInfo, -1, 0, 1);
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(field_138_tlvInfo, -1, 0, 0);
        }
    }

    for (int i = 1; i < ALIVE_COUNTOF(field_180_resources); i++)
    {
        if (field_10_anim.field_20_ppBlock != field_180_resources[i])
        {
            if (field_180_resources[i])
            {
                ResourceManager::FreeResource_455550(field_180_resources[i]);
                field_180_resources[i] = nullptr;
            }
        }
    }

    MusicController::sub_443810(MusicController::MusicTypes::eType0, this, 0, 0);

    if (!field_178)
    {
        gNumSlogs_9F11C8--;
    }

    return dtor_401000();
}

void Slog::VUpdate_Real_4739C0()
{
    NOT_IMPLEMENTED();
}

void Slog::VUpdate_4739C0()
{
    const __int16 oldMotion = field_FC_current_motion;

    VUpdate_Real_4739C0();

    if (oldMotion != field_FC_current_motion)
    {
        LOG_INFO("oldMotion = " << oldMotion << " newMotion = " << field_FC_current_motion);
    }
}

void Slog::Init_473130()
{
    NOT_IMPLEMENTED();
}

__int16 Slog::ToNextMotion_473CE0()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Slog::ToJump_473FB0()
{
    NOT_IMPLEMENTED();
}

void Slog::Sfx_475BD0(int /*idx*/)
{
    NOT_IMPLEMENTED();
}

void Slog::State_0_Idle_4742E0()
{
    if (!ToNextMotion_473CE0())
    {
        if (field_FE_next_state == eSlogStates::State_19_JumpForwards_475610)
        {
            ToJump_473FB0();
        }
        else if (field_FE_next_state == -1)
        {
            if (field_FC_current_motion != eSlogStates::State_0_Idle_4742E0)
            {
                if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0))
                {
                    SND_SEQ_PlaySeq_4775A0(17, 1, 0);
                }

                if (gMap_507BA8.GetDirection(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos) >= CameraPos::eCamCurrent_0
                    && gMap_507BA8.GetDirection(
                        field_B2_lvl_number,
                        field_B0_path_number,
                        field_A8_xpos,
                        field_AC_ypos) >= CameraPos::eCamCurrent_0)
                {
                    if (MusicController::sub_443840(0, 0, 0) == 9)
                    {
                        MusicController::sub_443810(MusicController::MusicTypes::eType9, this, 0, 0);
                    }
                    else
                    {
                        MusicController::sub_443810(MusicController::MusicTypes::eType7, this, 0, 0);
                    }
                }
            }
        }
        else
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
        }
    }
}

void Slog::State_1_Walk_4743F0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_2_Run_4749A0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_3_TurnAround_474C70()
{
    NOT_IMPLEMENTED();
}

void Slog::State_4_Fall_4750C0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_5_Unknown_474070()
{
    NOT_IMPLEMENTED();
}

void Slog::State_6_MoveHeadUpwards_474220()
{
    NOT_IMPLEMENTED();
}

void Slog::State_7_SlideTurn_474DB0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_8_StopRunning_474EC0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_9_StartWalking_474690()
{
    NOT_IMPLEMENTED();
}

void Slog::State_10_EndWalking_4747D0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_11_Land_475AB0()
{
    if (!field_10_anim.field_92_current_frame)
    {
        Sfx_475BD0(15);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit6_FlipY);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit7_SwapXY);

        MapFollowMe_401D30(0);

        field_12C = 0;
        field_130 = 0;

        field_B4_velx = FP_FromInteger(0);
        field_B8_vely = FP_FromInteger(0);

        field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
        field_FE_next_state = -1;

        field_128 = 60 * Slog_NextRandom() / 256 + gnFrameCount_507670 + 120;
    }
}

void Slog::State_12_Unknown_475B50()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_FC_current_motion = eSlogStates::State_13_StartFastBarking_475B70;
    }
}

void Slog::State_13_StartFastBarking_475B70()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        Sfx_475BD0(2);
        field_144 = 1;
    }

    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = eSlogStates::State_14_EndFastBarking_475BB0;
        }
    }
}

void Slog::State_14_EndFastBarking_475BB0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
    }
}

void Slog::State_15_Empty_475290()
{
    NOT_IMPLEMENTED();
}

void Slog::State_16_Sleeping_4752E0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_17_FallAsleep_475510()
{
    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
        }
    }

    MusicController::sub_443810(MusicController::MusicTypes::eType0, this, 0, 0);
}

void Slog::State_18_WakeUp_475460()
{
    NOT_IMPLEMENTED();
}

void Slog::State_19_JumpForwards_475610()
{
    NOT_IMPLEMENTED();
}

void Slog::State_20_JumpUpwards_475890()
{
    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
        MusicController::sub_443810(MusicController::MusicTypes::eType9, this, 0, 0);

    if (field_10_anim.field_92_current_frame == 5)
    {
        Sfx_475BD0(6);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
        field_FE_next_state = -1;
    }
}

void Slog::State_21_Eating_475900()
{
    NOT_IMPLEMENTED();
}

void Slog::State_22_Empty_475A90()
{
    NOT_IMPLEMENTED();
}

void Slog::State_23_Scratch_475550()
{
    if (field_10_anim.field_92_current_frame == 4)
    {
        SND_SEQ_PlaySeq_4775A0(16u, 1, 1);
    }

    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
        }
    }
}

void Slog::State_24_Growl_475590()
{
    if (field_10_anim.field_92_current_frame == 3)
    {
        if (field_144)
        {
            Sfx_475BD0(3);
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
            field_144 = 0;
            field_164_timer = gnFrameCount_507670 + 12;
        }
    }

    if (static_cast<int>(gnFrameCount_507670) > field_164_timer)
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
    }

    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
        }
    }
}

END_NS_AO

