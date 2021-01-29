#include "stdafx_ao.h"
#include "Function.hpp"
#include "Bells.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "ScreenWave.hpp"
#include "Sparks.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Sfx.hpp"

namespace AO {

Bells* Bells::ctor_40A650(BellSize bellType, FP xpos, FP ypos, FP scale)
{
    ctor_417C10();
    SetVTable(this, 0x4BA390);

    field_6_flags.Clear(Options::eCanExplode_Bit7);

    field_4_typeId = Types::eBells_13;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kChimeResID, 1, 0);

    switch (bellType)
    {
    case BellSize::eBig_0:
        field_EA_sound = Sounds::eLowPitch_0;
        Animation_Init_417FD0(19240, 49, 46, ppRes, 1);
        break;

    case BellSize::eMedium_1:
        field_EA_sound = Sounds::eMediumPitch_1;
        Animation_Init_417FD0(19368, 49, 46, ppRes, 1);
        break;
    case BellSize::eSmall_2:
        field_EA_sound = Sounds::eHighPitch_2;
        Animation_Init_417FD0(19252, 49, 46, ppRes, 1);
        break;
    }

    field_CC_bApplyShadows &= ~1u;
    field_BC_sprite_scale = scale;

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;

    field_10_anim.field_C_layer = 36;

    field_E8_bSmashing = 0;

    field_EC_timer = 0;
    field_F4_timer = 0;
    field_F0_timer = 0;

    return this;
}

BaseGameObject* Bells::dtor_40A760()
{
    SetVTable(this, 0x4BA390);
    return dtor_417D10();
}

void Bells::VUpdate()
{
    VUpdate_40A770();
}

void Bells::VUpdate_40A770()
{
    LOG_INFO("timer: " << (int)field_F0_timer);
    if (field_F0_timer > 0 && static_cast<int>(gnFrameCount_507670) >= field_F4_timer)
    {
        field_F4_timer = gnFrameCount_507670 + 4;
        field_F0_timer--;

        if (field_EA_sound == Sounds::eLowPitch_0)
        {
            SFX_Play_43AD70(SoundEffect::BellChime_LowPitch_52, 0, 0);
        }
        else if (field_EA_sound == Sounds::eMediumPitch_1)
        {
            SFX_Play_43AE60(SoundEffect::BellChime_MediumPitch_51, 45 * (field_F0_timer + 1), 128 - (field_F0_timer << 7), 0);
        }
        else if (field_EA_sound == Sounds::eHighPitch_2)
        {
            SFX_Play_43AE60(SoundEffect::BellChime_HighPitch_50, 30 * (field_F0_timer + 1), (2 - field_F0_timer) << 7, 0);
        }
    }


    if (field_E8_bSmashing == 1)
    {
        if (field_EC_timer > 0)
        {
            field_EC_timer--;

            FP xOff = {};
            FP yOff = {};
            if (field_EA_sound == Sounds::eLowPitch_0)
            {
                xOff = FP_FromInteger(-35);
                yOff = FP_FromInteger(36);
                auto pScreenWave = ao_new<ScreenWave>();
                if (pScreenWave)
                {
                    const FP wave_ypos = field_AC_ypos + FP_FromInteger(36);
                    const FP wave_xpos = field_A8_xpos - FP_FromInteger(35);
                    pScreenWave->ctor_462A70(wave_xpos, wave_ypos, 37, FP_FromInteger(18), FP_FromInteger(12), 0);
                }
            }
            else if (field_EA_sound == Sounds::eHighPitch_2)
            {
                xOff = FP_FromInteger(37);
                yOff = FP_FromInteger(32);
                auto pScreenWave = ao_new<ScreenWave>();
                if (pScreenWave)
                {
                    const FP wave_ypos = field_AC_ypos + FP_FromInteger(32);
                    const FP wave_xpos = field_A8_xpos + FP_FromInteger(37);
                    pScreenWave->ctor_462A70(wave_xpos, wave_ypos, 37, FP_FromInteger(12), FP_FromInteger(12), 0);
                }
            }
            else if (field_EA_sound == Sounds::eMediumPitch_1)
            {
                xOff = FP_FromInteger(-4);
                yOff = FP_FromInteger(24);
                auto pScreenWave = ao_new<ScreenWave>();
                if (pScreenWave)
                {
                    const FP wave_ypos = field_AC_ypos + FP_FromInteger(24);
                    const FP wave_xpos = field_A8_xpos - FP_FromInteger(4);
                    pScreenWave->ctor_462A70(wave_xpos, wave_ypos, 37, FP_FromInteger(14), FP_FromInteger(12), 0);
                }
            }

            for (int i = 0; i < 4; i++)
            {
                const FP sparkx = field_A8_xpos + FP_FromInteger(Math_RandomRange_450F20(-2, 2)) + xOff;
                const FP sparky = field_AC_ypos + FP_FromInteger(Math_RandomRange_450F20(-2, 2)) + yOff;
                auto pSpark = ao_new<Sparks>();
                if (pSpark)
                {
                    pSpark->ctor_40A3A0(sparkx, sparky, field_BC_sprite_scale);
                }
            }
        }

        PlaySounds();
    }
}

void Bells::PlaySounds()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_EA_sound == Sounds::eLowPitch_0)
        {
            field_10_anim.Set_Animation_Data_402A40(19240, 0);
        }
        else if (field_EA_sound == Sounds::eMediumPitch_1)
        {
            field_10_anim.Set_Animation_Data_402A40(19368, 0);
        }
        else if (field_EA_sound == Sounds::eHighPitch_2)
        {
            field_10_anim.Set_Animation_Data_402A40(19252, 0);
        }

        field_E8_bSmashing = 0;
    }
}

Bells* Bells::Vdtor_40AB00(signed int flags)
{
    dtor_40A760();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}


BOOL Bells::CanSmash_40AA70()
{
    return field_E8_bSmashing == 0;
}

void Bells::Ring_40AA80()
{
    if (field_E8_bSmashing == 0)
    {
        field_E8_bSmashing = 1;
        field_EC_timer = 1;
        field_F4_timer = 0;

        if (field_EA_sound == Sounds::eLowPitch_0)
        {
            field_F0_timer = 1;
            field_10_anim.Set_Animation_Data_402A40(19380, 0);
        }
        else if (field_EA_sound == Sounds::eMediumPitch_1)
        {
            field_F0_timer = 2;
            field_10_anim.Set_Animation_Data_402A40(19312, 0);
        }
        else if (field_EA_sound == Sounds::eHighPitch_2)
        {
            field_F0_timer = 3;
            field_10_anim.Set_Animation_Data_402A40(19264, 0);
        }
    }
}

BaseGameObject* Bells::VDestructor(signed int flags)
{
    return Vdtor_40AB00(flags);
}

}
