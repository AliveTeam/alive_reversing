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

START_NS_AO

// TODO: Index is always >=1 so first entry is redundant ??
const int dword_4C5054[11] = { 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

ChimeLock* ChimeLock::ctor_40AB20(Path_ChimeLock* pTlv, signed int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BA3C8);
    field_4_typeId = Types::eChimeLock_14;

    field_10C_tlvInfo = tlvInfo;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2024, 1, 0);
    Animation_Init_417FD0(136, 16, 8, ppRes, 1);
    field_10_anim.field_C_layer = 37;

    FP scale = {};
    if (pTlv->field_18_scale == 1)
    {
        scale = FP_FromDouble(0.5);
    }
    else
    {
        scale = FP_FromInteger(1);
    }

    field_114_bells = ao_new<Bells>();
    if (field_114_bells)
    {
        field_114_bells->ctor_40A650(
            BellType::eType_0,
            FP_FromInteger(pTlv->field_C_sound_pos.field_0_x),
            FP_FromInteger(pTlv->field_C_sound_pos.field_2_y),
            scale);
    }

    field_118_bells = ao_new<Bells>();
    if (field_118_bells)
    {
        field_118_bells->ctor_40A650(
            BellType::eType_1,
            FP_FromInteger(pTlv->field_C_sound_pos.field_0_x),
            FP_FromInteger(pTlv->field_C_sound_pos.field_2_y),
            scale);
    }

    field_11C_bells = ao_new<Bells>();
    if (field_11C_bells)
    {
        field_11C_bells->ctor_40A650(
            BellType::eType_2,
            FP_FromInteger(pTlv->field_C_sound_pos.field_0_x),
            FP_FromInteger(pTlv->field_C_sound_pos.field_2_y),
            scale);
    }

    field_114_bells->field_C_refCount++;
    field_118_bells->field_C_refCount++;
    field_11C_bells->field_C_refCount++;

    field_124_code1 = pTlv->field_1C_code1;

    int code2 = pTlv->field_1E_code2;
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
    for (int i = 0; i < 10; i++)
    {
        if (!(field_124_code1 / dword_4C5054[field_120_max_idx + 1]))
        {
            break;
        }
        field_120_max_idx++;
    }

    field_15E_ball_angle = 0;

    field_140_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y + 40);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y + 40);
   
    field_B8_vely = FP_FromInteger(0);

    field_13C = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_14C = FP_FromInteger(1);

    field_130_song_matching = 0;
    if (SwitchStates_Get(pTlv->field_20_id))
    {
        if (!SwitchStates_Get(pTlv->field_1A_solve_id))
        {
            field_130_song_matching = 1;
        }
    }

    field_138 &= ~2u;

    field_10A_flags.Clear(Flags_10A::e10A_Bit2_bPossesed);
    field_10A_flags.Set(Flags_10A::e10A_Bit1_Can_Be_Possessed);

    field_132_solve_id = pTlv->field_1A_solve_id;

    field_15C_ball_state = 0;
    field_128_idx = 0;
    field_164 = 0;
    field_110_state = 0;

    return this;
}


BaseGameObject* ChimeLock::dtor_40AE60()
{
    SetVTable(this, 0x4BA3C8);

    if (field_114_bells)
    {
        field_114_bells->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_118_bells)
    {
        field_118_bells->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_11C_bells)
    {
        field_11C_bells->field_6_flags.Set(Options::eDead_Bit3);
    }

    gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);

    return dtor_401000();
}

ChimeLock* ChimeLock::Vdtor_40BD40(signed int flags)
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
    if (field_114_bells)
    {
        field_114_bells->field_6_flags.Set(Options::eDead_Bit3);
        field_114_bells->field_C_refCount--;
        field_114_bells = nullptr;
    }

    if (field_118_bells)
    {
        field_118_bells->field_6_flags.Set(Options::eDead_Bit3);
        field_118_bells->field_C_refCount--;
        field_118_bells = nullptr;
    }

    if (field_11C_bells)
    {
        field_11C_bells->field_6_flags.Set(Options::eDead_Bit3);
        field_11C_bells->field_C_refCount--;
        field_11C_bells = nullptr;
    }

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
    field_110_state = 0;
    sActiveHero_507678->SetActiveControlledCharacter_421480();
    SFX_Play_43AE60(SoundEffect::PossessEffect_21, 70, 400, 0);
}

__int16 ChimeLock::DoNote_40BB20(__int16 note)
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

    if (!field_130_song_matching && !sVoiceCheat_507708 || (field_124_code1 / dword_4C5054[field_120_max_idx]) != note)
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

void ChimeLock::SetBallTarget_40B7B0(FP ballTargetX, FP ballTargetY, __int16 timer, __int16 xSize, __int16 ySize, __int16 bHitBell)
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

        field_150 = FP_FromInteger(256) / timerFP;
        field_154 = FP_FromInteger(256) / timerFP;

        field_158_xSize = xSize;
        field_15A_ySize = ySize;

        if (bHitBell)
        {
            field_15C_ball_state = 1;
        }
        else
        {
            field_15C_ball_state = 2;
        }
    }
    else
    {
        field_15C_ball_state = 0;
    }
}

void ChimeLock::VPossessed_40BC40()
{
    field_138 &= ~3u;
    field_10A_flags.Set(Flags_10A::e10A_Bit2_bPossesed);
    field_110_state = 2;
    field_128_idx = 0;
    field_12C = gnFrameCount_507670 + 45;
    field_15C_ball_state = 0;
    field_164 = 0;
}

BaseGameObject* ChimeLock::VDestructor(signed int flags)
{
    return Vdtor_40BD40(flags);
}

END_NS_AO

