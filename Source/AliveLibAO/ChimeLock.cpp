#include "stdafx_ao.h"
#include "Function.hpp"
#include "ChimeLock.hpp"
#include "ResourceManager.hpp"
#include "Bells.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "CheatController.hpp"
#include "Math.hpp"
#include "Collisions.hpp"
#include "Events.hpp"
#include "MusicTrigger.hpp"
#include "Input.hpp"
#include "Particle.hpp"
#include "Sys_common.hpp"

namespace AO {

// TODO: Index is always >=1 so first entry is redundant ??
const s32 dword_4C5054[11] = {0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

ChimeLock* ChimeLock::ctor_40AB20(Path_ChimeLock* pTlv, s32 tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BA3C8);
    field_4_typeId = Types::eChimeLock_14;

    field_10C_tlvInfo = tlvInfo;

    const AnimRecord& rec = AO::AnimRec(AnimId::Chime_Ball);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    field_10_anim.field_C_layer = Layer::eLayer_FG1_37;

    FP scale = {};
    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        scale = FP_FromDouble(0.5);
    }
    else
    {
        scale = FP_FromInteger(1);
    }

    field_114_left_bell = ao_new<Bells>();
    if (field_114_left_bell)
    {
        field_114_left_bell->ctor_40A650(
            BellSize::eBig_0,
            FP_FromInteger(pTlv->field_10_top_left.field_0_x),
            FP_FromInteger(pTlv->field_10_top_left.field_2_y),
            scale);
        field_114_left_bell->field_C_refCount++;
    }

    field_118_center_bell = ao_new<Bells>();
    if (field_118_center_bell)
    {
        field_118_center_bell->ctor_40A650(
            BellSize::eMedium_1,
            FP_FromInteger(pTlv->field_10_top_left.field_0_x),
            FP_FromInteger(pTlv->field_10_top_left.field_2_y),
            scale);
        field_118_center_bell->field_C_refCount++;
    }

    field_11C_right_bell = ao_new<Bells>();
    if (field_11C_right_bell)
    {
        field_11C_right_bell->ctor_40A650(
            BellSize::eSmall_2,
            FP_FromInteger(pTlv->field_10_top_left.field_0_x),
            FP_FromInteger(pTlv->field_10_top_left.field_2_y),
            scale);
        field_11C_right_bell->field_C_refCount++;
    }


    field_124_code1 = pTlv->field_1C_code1;

    s32 code2 = pTlv->field_1E_code2;
    if (code2)
    {
        field_120_max_idx = 0;
        while (code2 / dword_4C5054[field_120_max_idx + 1])
        {
            field_120_max_idx++;
        }
        field_124_code1 = code2 + field_124_code1 * dword_4C5054[field_120_max_idx + 1];
    }

    field_120_max_idx = 0;
    for (s32 i = 0; i < 10; i++)
    {
        if (!(field_124_code1 / dword_4C5054[field_120_max_idx + 1]))
        {
            break;
        }
        field_120_max_idx++;
    }

    field_15E_ball_angle = 0;

    field_140_targetY = FP_FromInteger(pTlv->field_10_top_left.field_2_y + 40);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y + 40);

    field_B8_vely = FP_FromInteger(0);

    field_13C_targetX = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_14C_increase_vely_by = FP_FromInteger(1);

    field_130_song_matching = 0;
    if (SwitchStates_Get(pTlv->field_20_password_switch_id))
    {
        if (!SwitchStates_Get(pTlv->field_1A_solve_switch_id))
        {
            field_130_song_matching = 1;
        }
    }

    field_138_flags &= ~2u;

    field_10A_flags.Clear(Flags_10A::e10A_Bit2_bPossesed);
    field_10A_flags.Set(Flags_10A::e10A_Bit1_Can_Be_Possessed);

    field_132_solve_switch_id = pTlv->field_1A_solve_switch_id;

    field_15C_ball_state = BallStates::eIdle_0;
    field_128_idx = 0;
    field_164_ChimeLock_num[0] = BellPositions::eNone_0;
    field_164_ChimeLock_num[1] = BellPositions::eNone_0;
    field_110_state = ChimeLockStates::eIdle_0;

    return this;
}


BaseGameObject* ChimeLock::dtor_40AE60()
{
    SetVTable(this, 0x4BA3C8);

    if (field_114_left_bell)
    {
        field_114_left_bell->field_6_flags.Set(Options::eDead_Bit3);
        field_114_left_bell->field_C_refCount--;
        field_114_left_bell = nullptr;
    }

    if (field_118_center_bell)
    {
        field_118_center_bell->field_6_flags.Set(Options::eDead_Bit3);
        field_118_center_bell->field_C_refCount--;
        field_118_center_bell = nullptr;
    }

    if (field_11C_right_bell)
    {
        field_11C_right_bell->field_6_flags.Set(Options::eDead_Bit3);
        field_11C_right_bell->field_C_refCount--;
        field_11C_right_bell = nullptr;
    }

    gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);

    return dtor_401000();
}

ChimeLock* ChimeLock::Vdtor_40BD40(s32 flags)
{
    dtor_40AE60();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}


void ChimeLock::VScreenChanged()
{
    VScreenChanged_40BCD0();
}


void ChimeLock::VScreenChanged_40BCD0()
{
    field_6_flags.Set(Options::eDead_Bit3);
}

void ChimeLock::VPossessed()
{
    VPossessed_40BC40();
}

void ChimeLock::VUnPosses()
{
    VUnPosses_40BC90();
}

void ChimeLock::VUnPosses_40BC90()
{
    field_10A_flags.Clear(Flags_10A::e10A_Bit2_bPossesed);
    field_110_state = ChimeLockStates::eIdle_0;
    sActiveHero_507678->SetActiveControlledCharacter_421480();
    SFX_Play_43AE60(SoundEffect::PossessEffect_21, 70, 400, 0);
}

s16 ChimeLock::DoNote_40BB20(s16 note)
{
    if ((field_130_song_matching || sVoiceCheat_507708) && field_124_code1 / dword_4C5054[field_120_max_idx - field_128_idx] % 10 == note)
    {
        field_128_idx++;
        if (field_128_idx >= field_120_max_idx)
        {
            return 1;
        }
        return 0;
    }

    if ((!field_130_song_matching && !sVoiceCheat_507708) || (field_124_code1 / dword_4C5054[field_120_max_idx]) != note)
    {
        field_128_idx = 0;
        return 0;
    }

    field_128_idx = 1;

    if (field_120_max_idx > 1)
    {
        return 0;
    }

    return 1;
}

void ChimeLock::SetBallTarget_40B7B0(FP ballTargetX, FP ballTargetY, s16 timer, s16 xSize, s16 ySize, s16 bHitBell)
{
    if (timer > 0)
    {
        const FP timerFP = FP_FromInteger(timer);

        field_160_ball_timer = timer;
        field_15E_ball_angle = 0;

        field_B4_velx = (ballTargetX - field_A8_xpos) / timerFP;
        field_B8_vely = (ballTargetY - field_AC_ypos) / timerFP;

        field_144_ball_start_x = field_A8_xpos;
        field_148_ball_start_y = field_AC_ypos;

        field_150_xpos_offset = FP_FromInteger(256) / timerFP;
        field_154_ypos_offset = FP_FromInteger(256) / timerFP;

        field_158_xSize = xSize;
        field_15A_ySize = ySize;

        if (bHitBell)
        {
            field_15C_ball_state = BallStates::eMovingToBell_1;
        }
        else
        {
            field_15C_ball_state = BallStates::eMovingBackToIdle_2;
        }
    }
    else
    {
        field_15C_ball_state = BallStates::eIdle_0;
    }
}

s16 ChimeLock::UpdateBall_40B8A0()
{
    field_15E_ball_angle++;

    switch (field_15C_ball_state)
    {
        case BallStates::eIdle_0:
            field_A8_xpos = (FP_FromInteger(5) * Math_Cosine_4510A0((4 * field_15E_ball_angle) & 0xFF)) + field_13C_targetX;
            field_AC_ypos = (FP_FromInteger(3) * Math_Cosine_4510A0((3 * field_15E_ball_angle) & 0xFF)) + field_140_targetY;
            return 0;

        case BallStates::eMovingToBell_1:
        case BallStates::eMovingBackToIdle_2:
            field_144_ball_start_x += field_B4_velx;
            field_148_ball_start_y += field_B8_vely;
            field_A8_xpos = (FP_FromInteger(field_158_xSize) * Math_Cosine_4510A0(FP_GetExponent(FP_FromInteger(field_15E_ball_angle) * field_150_xpos_offset) & 0xFF)) + field_144_ball_start_x;
            field_AC_ypos = (FP_FromInteger(field_15A_ySize) * Math_Cosine_4510A0(FP_GetExponent(FP_FromInteger(field_15E_ball_angle) * field_154_ypos_offset) & 0xFF)) + field_148_ball_start_y;
            if (field_15E_ball_angle >= field_160_ball_timer)
            {
                field_15E_ball_angle = 0;

                if (field_15C_ball_state == BallStates::eMovingToBell_1)
                {
                    field_15C_ball_state = BallStates::eMovingBackToIdle_2;
                    return 1;
                }
                field_15C_ball_state = BallStates::eIdle_0;
            }
            return 0;

        case BallStates::eNeverRead_3:
        {
            if (true)
                ALIVE_FATAL("never expected BallStates::eNeverRead_3 to be called");

            field_B8_vely += field_14C_increase_vely_by;
            field_AC_ypos += field_B8_vely;

            FP hitX = {};
            FP hitY = {};
            if (sCollisions_DArray_504C6C->RayCast_40C410(
                    field_A8_xpos,
                    field_B8_vely - field_B8_vely,
                    field_A8_xpos,
                    field_AC_ypos,
                    &field_F4_pLine,
                    &hitX,
                    &hitY,
                    field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70)
                == 1)
            {
                if (field_F4_pLine->field_8_type == eLineTypes ::eFloor_0 ||
                    field_F4_pLine->field_8_type == eLineTypes::eBackgroundFloor_4)
                {
                    field_AC_ypos = hitY - FP_FromInteger(1);
                    field_B8_vely = -(field_B8_vely * FP_FromDouble(0.4));
                    if (field_162_never_set >= 3)
                    {
                        return 1;
                    }
                }
            }
            return 0;
        }

        default:
            return 0;
    }
}

void ChimeLock::VUpdate()
{
    VUpdate_40AEF0();
}

void ChimeLock::VUpdate_40AEF0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    switch (field_110_state)
    {
        case ChimeLockStates::eIdle_0:
            UpdateBall_40B8A0();
            return;

        case ChimeLockStates::eUnused_1:
            if (UpdateBall_40B8A0())
            {
                field_110_state = ChimeLockStates::ePossessed_2;
                field_15C_ball_state = BallStates::eIdle_0;
            }
            ALIVE_FATAL("never expected ChimeLockStates::eUnused_1 to be called");
            return;

        case ChimeLockStates::ePossessed_2:
            if (UpdateBall_40B8A0())
            {
                BellPositions Bell = field_164_ChimeLock_num[0];
                field_164_ChimeLock_num[0] = BellPositions::eNone_0;

                switch (Bell)
                {
                    case BellPositions::eLeftBell_1:
                        field_114_left_bell->Ring_40AA80();
                        if ((field_138_flags >> 1) & 1)
                        {
                            SetTargetBellIfSpace(2);
                        }
                        break;

                    case BellPositions::eCenterBell_2:
                        field_118_center_bell->Ring_40AA80();
                        if ((field_138_flags >> 1) & 1)
                        {
                            SetTargetBellIfSpace(3);
                        }
                        break;

                    case BellPositions::eRightBell_3:
                        field_11C_right_bell->Ring_40AA80();
                        if ((field_138_flags >> 1) & 1)
                        {
                            field_138_flags &= ~2u;
                        }
                        break;
                }

                SetBallTarget_40B7B0(
                    field_13C_targetX,
                    field_140_targetY,
                    36,
                    Math_RandomRange_450F20(6, 9),
                    Math_RandomRange_450F20(6, 9),
                    0);

                if (DoNote_40BB20(static_cast<s16>(Bell)))
                {
                    field_110_state = ChimeLockStates::eNeverRead_6;
                    SwitchStates_Do_Operation_436A10(field_132_solve_switch_id, SwitchOp::eSetTrue_0);
                    VUnPosses();
                    auto pMusic = ao_new<MusicTrigger>();
                    if (pMusic)
                    {
                        pMusic->ctor_443A60(MusicTriggerMusicType::eSecretAreaShort_6, TriggeredBy::eTouching_1, 0, 15);
                    }
                    return;
                }
            }

            if (field_15C_ball_state != BallStates::eMovingToBell_1)
            {
                switch (field_164_ChimeLock_num[0])
                {
                    case BellPositions::eLeftBell_1:
                        if (field_114_left_bell->CanSmash_40AA70())
                        {
                            SetBallTarget_40B7B0(
                                field_13C_targetX - FP_FromInteger(35),
                                field_140_targetY - FP_FromInteger(4),
                                2,
                                0,
                                0,
                                1);
                        }
                        break;

                    case BellPositions::eCenterBell_2:
                        if (field_118_center_bell->CanSmash_40AA70())
                        {
                            SetBallTarget_40B7B0(
                                field_13C_targetX - FP_FromInteger(4),
                                field_140_targetY - FP_FromInteger(16),
                                2,
                                0,
                                0,
                                1);
                        }
                        break;

                    case BellPositions::eRightBell_3:
                        if (field_11C_right_bell->CanSmash_40AA70())
                        {
                            SetBallTarget_40B7B0(
                                field_13C_targetX + FP_FromInteger(37),
                                field_140_targetY - FP_FromInteger(8),
                                2,
                                0,
                                0,
                                1);
                        }
                        break;

                    case BellPositions::eUnused_4:
                        SetBallTarget_40B7B0(
                            field_13C_targetX,
                            field_140_targetY,
                            30,
                            Math_RandomRange_450F20(6, 9),
                            Math_RandomRange_450F20(6, 9),
                            1);
                        ALIVE_FATAL("never expected target bell num 4 in ChimeLockStates::ePossessed_2 to be called");
                        break;

                    default:
                        break;
                }
            }

            if (!field_130_song_matching && !sVoiceCheat_507708)
            {
                if (!Input_IsChanting_4334C0())
                {
                    field_138_flags |= 1u;
                }

                if (field_138_flags & 1 && Input_IsChanting_4334C0())
                {
                    field_136_unpossession_timer = 30;
                    field_110_state = ChimeLockStates::eUnPossessing_3;
                    field_134_pressed = Input().Pressed();

                    field_164_ChimeLock_num[0] = BellPositions::eNone_0;
                    field_164_ChimeLock_num[1] = BellPositions::eNone_0;

                    SFX_Play_43AE60(SoundEffect::PossessEffect_21, 0, -600, 0);
                    return;
                }

                const auto pressed = Input().Pressed();
                if (Input().IsAnyPressed(eThrowItem) && !(field_134_pressed & eThrowItem))
                {
                    SetTargetBellIfSpace(3);
                    field_134_pressed = pressed;
                    return;
                }

                if (Input().IsAnyPressed(eHop) && !(field_134_pressed & eHop))
                {
                    SetTargetBellIfSpace(2);
                    field_134_pressed = pressed;
                    return;
                }

                if (Input().IsAnyPressed(eDoAction) && !(field_134_pressed & eDoAction))
                {
                    SetTargetBellIfSpace(1);
                    field_134_pressed = pressed;
                    return;
                }

                if (Input().IsAnyPressed(eCrouchOrRoll) && !(field_134_pressed & eCrouchOrRoll))
                {
                    // hit all 3 bells from left to right
                    SetTargetBellIfSpace(1);
                    field_138_flags |= 2u;
                    field_134_pressed = pressed;
                    return;
                }

                if (Input().IsAnyPressed(eRight) && !(field_134_pressed & eRight))
                {
                    SetTargetBellIfSpace(3);
                    field_134_pressed = pressed;
                    return;
                }

                if (Input().IsAnyPressed(eUp) && !(field_134_pressed & eUp))
                {
                    SetTargetBellIfSpace(2);
                    field_134_pressed = pressed;
                    return;
                }

                if (Input().IsAnyPressed(eLeft) && !(field_134_pressed & eLeft))
                {
                    SetTargetBellIfSpace(1);
                    field_134_pressed = pressed;
                    return;
                }

                if (Input().IsAnyPressed(eDown) && !(field_134_pressed & eDown))
                {
                    // hit all 3 bells from left to right
                    SetTargetBellIfSpace(1);
                    field_138_flags |= 2u;
                }
                field_134_pressed = pressed;
                return;
            }

            if (static_cast<s32>(gnFrameCount_507670) >= field_12C_timer)
            {
                SetTargetBellIfSpace(field_124_code1 / dword_4C5054[field_120_max_idx - field_128_idx] % 10);
                field_12C_timer = gnFrameCount_507670 + 15;
            }
            return;

        case ChimeLockStates::eUnPossessing_3:
            if (UpdateBall_40B8A0())
            {
                SetBallTarget_40B7B0(field_13C_targetX, field_140_targetY, 36, Math_RandomRange_450F20(6, 9), Math_RandomRange_450F20(6, 9), 0);
            }

            if (!Input_IsChanting_4334C0())
            {
                field_110_state = ChimeLockStates::ePossessed_2;
            }
            else
            {
                field_136_unpossession_timer--;

                if (field_136_unpossession_timer == 0)
                {
                    field_110_state = ChimeLockStates::eUnPossess_4;
                }
            }

            if (!(gnFrameCount_507670 % 4))
            {
                return;
            }

            New_Chant_Particle_4198E0(
                field_13C_targetX + (field_BC_sprite_scale * FP_FromInteger(Math_RandomRange_450F20(-30, 30))),
                field_140_targetY - (field_BC_sprite_scale * FP_FromInteger(Math_RandomRange_450F20(-20, 20))),
                field_BC_sprite_scale,
                Layer::eLayer_0);
            return;

        case ChimeLockStates::eUnPossess_4:
            VUnPosses();
            return;

        case ChimeLockStates::eUnused_5:
            if (UpdateBall_40B8A0())
            {
                field_110_state = ChimeLockStates::eIdle_0;
            }
            ALIVE_FATAL("never expected ChimeLockStates::eUnused_5 to be called");
            return;

        default:
            return;
    }
}


void ChimeLock::SetTargetBellIfSpace(s16 targetNum)
{
    s32 magic = 0;
    auto pChimeLock_num = &field_164_ChimeLock_num[0];

    // looks like it's just doing: field_164_ChimeLock_num[0] = static_cast<BellPositions>(targetNum);
    // everytime. the while loop looks pointless to me
    while (*pChimeLock_num != BellPositions::eNone_0)
    {
        ++magic;
        ++pChimeLock_num;
        if (magic >= ALIVE_COUNTOF(field_164_ChimeLock_num))
        {
            return;
        }
    }
    field_164_ChimeLock_num[magic] = static_cast<BellPositions>(targetNum);
}

void ChimeLock::VPossessed_40BC40()
{
    field_138_flags &= ~3u;
    field_10A_flags.Set(Flags_10A::e10A_Bit2_bPossesed);
    field_110_state = ChimeLockStates::ePossessed_2;
    field_128_idx = 0;
    field_12C_timer = gnFrameCount_507670 + 45;
    field_15C_ball_state = BallStates::eIdle_0;
    field_164_ChimeLock_num[0] = BellPositions::eNone_0;
    field_164_ChimeLock_num[1] = BellPositions::eNone_0;
}

BaseGameObject* ChimeLock::VDestructor(s32 flags)
{
    return Vdtor_40BD40(flags);
}

} // namespace AO
