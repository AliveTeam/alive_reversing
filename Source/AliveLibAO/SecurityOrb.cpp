#include "stdafx_ao.h"
#include "Function.hpp"
#include "SecurityOrb.hpp"
#include "ResourceManager.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"
#include "Explosion.hpp"
#include "Gibs.hpp"
#include "ZapLine.hpp"
#include "Sfx.hpp"
#include "Sparks.hpp"
#include "Abe.hpp"
#include "PossessionFlicker.hpp"
#include "ScreenShake.hpp"
#include "Flash.hpp"
#include "Game.hpp"
#include "Events.hpp"

namespace AO {

SecurityOrb* SecurityOrb::ctor_436C80(Path_SecurityOrb* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BB7D0);

    field_6_flags.Set(Options::eCanExplode_Bit7);

    field_4_typeId = Types::SecurityOrb_53;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2006, 1, 0);
    Animation_Init_417FD0(
        10864,
        52,
        28,
        ppRes,
        1);

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_10C_tlvInfo = tlvInfo;

    if (pTlv->field_18_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }

    field_110_state = SecurityOrbStates::eIdle_0;
    field_118_sound_channels = 0;

    return this;
}

BaseGameObject* SecurityOrb::dtor_436D60()
{
    SetVTable(this, 0x4BB7D0);

    if (field_118_sound_channels)
    {
        SND_Stop_Channels_Mask_4774A0(field_118_sound_channels);
    }

    if (field_100_health > FP_FromInteger(0))
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 1);
    }
    return dtor_401000();
}

BaseGameObject* SecurityOrb::VDestructor(signed int flags)
{
    return Vdtor_4373B0(flags);
}

SecurityOrb* SecurityOrb::Vdtor_4373B0(signed int flags)
{
    dtor_436D60();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void SecurityOrb::VScreenChanged()
{
    VScreenChanged_4373A0();
}

void SecurityOrb::VScreenChanged_4373A0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

signed __int16 SecurityOrb::VTakeDamage_437280(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        return 0;
    }

    switch (pFrom->field_4_typeId)
    {
    case Types::eAbilityRing_69:
    case Types::eShrykull_85:
    {
        auto pExplosion = ao_new<Explosion>();
        if (pExplosion)
        {
            pExplosion->ctor_458B80(
                field_A8_xpos,
                field_AC_ypos - (field_BC_sprite_scale * FP_FromInteger(5)),
                field_BC_sprite_scale);
        }

        auto pGibs = ao_new<Gibs>();
        if (pGibs)
        {
            pGibs->ctor_407B20(
                5,
                field_A8_xpos,
                field_AC_ypos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_BC_sprite_scale);
        }
        field_100_health = FP_FromInteger(0);
    }
        break;

    default:
        break;
    }

    field_6_flags.Set(BaseGameObject::eDead_Bit3);

    return 1;
}

void SecurityOrb::VUpdate()
{
    VUpdate_436DF0();
}

void SecurityOrb::VUpdate_436DF0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    switch (field_110_state)
    {
    case SecurityOrbStates::eIdle_0:
        if (field_10_anim.field_92_current_frame == 2 ||
            field_10_anim.field_92_current_frame == 6 ||
            field_10_anim.field_92_current_frame == 10)
        {
            if (field_118_sound_channels)
            {
                SND_Stop_Channels_Mask_4774A0(field_118_sound_channels);
            }

            if (field_BC_sprite_scale == FP_FromDouble(0.5))
            {
                field_118_sound_channels = SFX_Play_43AE60(SoundEffect::SecurityOrb_56, 35, 720);
            }
            else
            {
                field_118_sound_channels = SFX_Play_43AE60(SoundEffect::SecurityOrb_56, 55, 700);
            }
        }

        if (Event_Get_417250(kEventAbeOhm_8))
        {
            field_110_state = SecurityOrbStates::eDoZapEffects_1;
            field_114_timer = gnFrameCount_507670 + 20;
        }
        break;

    case SecurityOrbStates::eDoZapEffects_1:
        if (static_cast<int>(gnFrameCount_507670) > field_114_timer)
        {
            PSX_RECT abeRect = {};
            sActiveHero_507678->VGetBoundingRect(&abeRect, 1);

            const int width = abeRect.w + abeRect.x;
            const int height = abeRect.h + abeRect.y;

            auto pZapLine = ao_new<ZapLine>();
            if (pZapLine)
            {
                pZapLine->ctor_4789A0(
                    field_A8_xpos,
                    field_AC_ypos - (FP_FromInteger(8) * field_BC_sprite_scale),
                    FP_FromInteger(width / 2),
                    FP_FromInteger(height / 2),
                    8,
                    ZapLineType::eThick_0,
                    28);
            }

            auto pPossessionFlicker = ao_new<PossessionFlicker>();
            if (pPossessionFlicker)
            {
                pPossessionFlicker->ctor_41A8C0(sActiveHero_507678, 8, 255, 100, 100);
            }

            sActiveHero_507678->VTakeDamage(this);
            field_114_timer = gnFrameCount_507670 + 8;
            field_110_state = SecurityOrbStates::eDoFlashAndSound_2;

            auto pScreenShake = ao_new<ScreenShake>();
            if (pScreenShake)
            {
                pScreenShake->ctor_4624D0(1);
            }

            auto pSpark1 = ao_new<Sparks>();
            if (pSpark1)
            {
                pSpark1->ctor_40A3A0(
                    field_A8_xpos,
                    field_AC_ypos - (FP_FromInteger(8) * field_BC_sprite_scale),
                    field_BC_sprite_scale);

                pSpark1->field_C2_g = 65;
                pSpark1->field_C4_b = 65;
                pSpark1->field_C0_r = 255;
            }

            auto pSpark2 = ao_new<Sparks>();
            if (pSpark2)
            {
                pSpark2->ctor_40A3A0(
                    field_A8_xpos,
                    field_AC_ypos - (FP_FromInteger(8) * field_BC_sprite_scale),
                    field_BC_sprite_scale);
                pSpark2->field_C2_g = 65;
                pSpark2->field_C4_b = 65;
                pSpark2->field_C0_r = 255;
            }


            for (int i = 0; i < 9; i++)
            {
                auto pSparks = ao_new<Sparks>();
                if (pSparks)
                {
                    pSparks->ctor_40A3A0(
                        FP_FromInteger(width / 2),
                        FP_FromInteger(height / 2),
                        field_BC_sprite_scale);

                    pSparks->field_C2_g = 65;
                    pSparks->field_C4_b = 65;
                    pSparks->field_C0_r = 255;
                }
            }
        }
        break;

    case SecurityOrbStates::eDoFlashAndSound_2:
        if (static_cast<int>(gnFrameCount_507670) == field_114_timer - 5 || static_cast<int>(gnFrameCount_507670) == field_114_timer - 1)
        {
            auto pFlash = ao_new<Flash>();
            if (pFlash)
            {
                pFlash->ctor_41A810(39, 255u, 0, 0, 1, 3u, 1);
            }
        }

        if (static_cast<int>(gnFrameCount_507670) == field_114_timer - 4)
        {
            auto pFlash = ao_new<Flash>();
            if (pFlash)
            {
                pFlash->ctor_41A810(39, 255u, 0, 0, 1, 1u, 1);
            }
        }

        if (field_114_timer - gnFrameCount_507670 == 4)
        {
            SFX_Play_43AD70(SoundEffect::Zap1_57, 0, 0);
        }
        else if (field_114_timer - gnFrameCount_507670 == 1)
        {
            SFX_Play_43AD70(SoundEffect::Zap2_58, 0, 0);
        }

        if (static_cast<int>(gnFrameCount_507670) > field_114_timer)
        {
            field_110_state = SecurityOrbStates::eIdle_0;
        }
        break;
    }
}

}
