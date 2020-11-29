#include "stdafx_ao.h"
#include "Function.hpp"
#include "SecurityClaw.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"
#include "MotionDetector.hpp"
#include "Gibs.hpp"
#include "Explosion.hpp"
#include "ZapLine.hpp"
#include "Sfx.hpp"
#include "Sparks.hpp"
#include "Abe.hpp"
#include "Game.hpp"
#include "Flash.hpp"
#include "Events.hpp"
#include "Alarm.hpp"
#include "ScreenShake.hpp"
#include "PossessionFlicker.hpp"

namespace AO {

TintEntry stru_4C5488[] =
{
    { 5, 105u, 55u, 55u },
    { 6, 105u, 55u, 55u },
    { -1, 127u, 127u, 127u },
};

TintEntry stru_4C5498[3] =
{
    { 5, 80u, 55u, 55u },
    { 6, 80u, 55u, 55u },
    { -1, 127u, 127u, 127u }
};

void Claw::ctor()
{
    ctor_417C10();
    SetVTable(this, 0x4BAA70);
    field_4_typeId = Types::eClaw_48;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2008, 1, 0);
    Animation_Init_417FD0(22468, 152, 31, ppRes, 1);
}

BaseGameObject* Claw::VDestructor(signed int flags)
{
    return Vdtor(flags);
}

BaseGameObject* Claw::Vdtor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

SecurityClaw* SecurityClaw::ctor_418A70(Path_SecurityClaw* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BAAA8);

    field_4_typeId = Types::eSecurityClaw_31;

    field_6_flags.Set(Options::eCanExplode_Bit7);
    field_12C_pDetector = 1;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2008, 1, 0);
    Animation_Init_417FD0(22480, 152, 31, ppRes, 1);

    field_10C_tlvInfo = tlvInfo;

    field_11C_clawX = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_120_clawY = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

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

    field_124 = 0; // LOBYTE

    field_A8_xpos = field_11C_clawX + ((Math_Sine_451110(0) * field_BC_sprite_scale) * FP_FromInteger(8)) * FP_FromDouble(0.25);
    field_AC_ypos = field_120_clawY + ((Math_Cosine_4510A0(0) * field_BC_sprite_scale) * FP_FromInteger(8));
    SetTint_418750(&stru_4C5488[0], gMap_507BA8.field_0_current_level);

    field_134 = pTlv->field_10_top_left;
    field_138 = pTlv->field_14_bottom_right;

    field_118_alarm_id = pTlv->field_1A_alarm_id;
    field_11A = pTlv->field_1C_alarm_time;

    field_110_state = SecurityClawStates::eCamSwap_0;

    auto pClaw = ao_new<Claw>();
    if (pClaw)
    {
        pClaw->ctor();
        pClaw->field_BC_sprite_scale = field_BC_sprite_scale;
        pClaw->field_10_anim.field_C_layer = field_10_anim.field_C_layer + 1;
    }
    field_130_pClaw = pClaw;

    pClaw->field_A8_xpos = field_11C_clawX;
    pClaw->field_AC_ypos = field_120_clawY;
    pClaw->SetTint_418750(&stru_4C5498[0], gMap_507BA8.field_0_current_level);

    field_6_flags.Set(Options::eUpdateDuringCamSwap_Bit10);
    field_13C_pArray = nullptr;
    field_128_sound_channels = 0;

    return this;
}

BaseGameObject* SecurityClaw::dtor_418CE0()
{
    SetVTable(this, 0x4BAAA8);

    if (field_12C_pDetector)
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 1);
    }

    if (field_130_pClaw)
    {
        field_130_pClaw->field_6_flags.Set(Options::eDead_Bit3);
        field_130_pClaw = nullptr;
    }

    if (field_13C_pArray)
    {
        for (int i = 0; i < field_13C_pArray->Size(); i++)
        {
            auto pObjIter = field_13C_pArray->ItemAt(i);

            pObjIter->SetDontComeBack_437E00(field_12C_pDetector);
            pObjIter->field_C_refCount--;
            pObjIter->field_6_flags.Set(Options::eDead_Bit3);
        }

        if (field_13C_pArray)
        {
            field_13C_pArray->dtor_404440();
            ao_delete_free_447540(field_13C_pArray);
        }
    }

    if (field_128_sound_channels)
    {
        SND_Stop_Channels_Mask_4774A0(field_128_sound_channels);
    }
    return dtor_401000();
}

BaseGameObject* SecurityClaw::VDestructor(signed int flags)
{
    return Vdtor_419700(flags);
}

SecurityClaw* SecurityClaw::Vdtor_419700(signed int flags)
{
    dtor_418CE0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void SecurityClaw::VScreenChanged()
{
    VScreenChange_4196F0();
}

void SecurityClaw::VScreenChange_4196F0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

__int16 SecurityClaw::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_419520(pFrom);
}

__int16 SecurityClaw::VTakeDamage_419520(BaseGameObject* pFrom)
{
    if (!field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        if (pFrom->field_4_typeId == Types::eAbilityRing_69 || pFrom->field_4_typeId == Types::eShrykull_85)
        {
            field_12C_pDetector = 0;
            field_6_flags.Set(BaseGameObject::eDead_Bit3);

            auto pExplosion = ao_new<Explosion>();
            if (pExplosion)
            {
                pExplosion->ctor_458B80(
                    field_A8_xpos,
                    field_AC_ypos - field_BC_sprite_scale * FP_FromInteger(5),
                    field_BC_sprite_scale);
            }

            auto pGibs = ao_new<Gibs>();
            if (pGibs)
            {
                pGibs->ctor_407B20(
                    5,
                    field_A8_xpos,
                    field_AC_ypos + FP_FromInteger(50),
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_BC_sprite_scale);
            }
        }
        else
        {
            field_12C_pDetector = 0;
            
            auto pGibs = ao_new<Gibs>();
            if (pGibs)
            {
                pGibs->ctor_407B20(
                    5,
                    field_A8_xpos,
                    field_AC_ypos + FP_FromInteger(50),
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_BC_sprite_scale);
            }

            pGibs = ao_new<Gibs>();
            if (pGibs)
            {
                pGibs->ctor_407B20(
                    5,
                    field_A8_xpos,
                    field_AC_ypos + FP_FromInteger(50),
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_BC_sprite_scale);
            }
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
    return 1;
}

void SecurityClaw::VUpdate()
{
    VUpdate_418DE0();
}

void SecurityClaw::VUpdate_418DE0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (!(gnFrameCount_507670 % 20))
    {
        if (field_128_sound_channels)
        {
            SND_Stop_Channels_Mask_4774A0(field_128_sound_channels);
        }
        field_128_sound_channels = SFX_Play_43AE60(56u, 55, -300, 0);
    }

    if (sActiveHero_507678 == sControlledCharacter_50767C)
    {
        if (sActiveHero_507678->field_A8_xpos < field_11C_clawX)
        {
            if (FP_GetExponent(field_11C_clawX) > field_134.field_0_x)
            {
                field_11C_clawX -= FP_FromDouble(0.5);
            }
        }

        if (sActiveHero_507678->field_A8_xpos >= field_11C_clawX)
        {
            if (FP_GetExponent(field_11C_clawX) < field_138.field_0_x)
            {
                field_11C_clawX += FP_FromDouble(0.5);
            }
        }
    }

    field_A8_xpos = field_11C_clawX + (((Math_Sine_451110(field_124) * field_BC_sprite_scale) * FP_FromInteger(8)) * FP_FromDouble(0.25));

    field_124 += 2;

    field_AC_ypos = field_120_clawY + ((Math_Cosine_4510A0(field_124) * field_BC_sprite_scale) * FP_FromInteger(8));

    field_130_pClaw->field_A8_xpos = field_A8_xpos;
    field_130_pClaw->field_AC_ypos = field_AC_ypos;

    if (field_13C_pArray)
    {
        for (int i = 0; i < field_13C_pArray->Size(); i++)
        {
            MotionDetector* pObj = field_13C_pArray->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            pObj->field_A8_xpos = field_A8_xpos - FP_FromInteger(1);
            pObj->field_AC_ypos = field_AC_ypos - FP_FromInteger(11);
        }
    }

    switch (field_110_state)
    {
    case SecurityClawStates::eCamSwap_0:
        for (int i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
        {
            BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            if (pObjIter->field_4_typeId == Types::eMotionDetector_59)
            {
                auto pDetector = static_cast<MotionDetector*>(pObjIter);
                if (!field_13C_pArray)
                {
                    field_10_anim.Set_Animation_Data_402A40(22616, nullptr);
                    field_13C_pArray = ao_new<DynamicArrayT<MotionDetector>>();
                    if (field_13C_pArray)
                    {
                        field_13C_pArray->ctor_4043E0(10);
                    }
                }

                pDetector->field_A8_xpos = field_A8_xpos - FP_FromInteger(1);
                pDetector->field_AC_ypos = field_AC_ypos - FP_FromInteger(11);
                pDetector->field_C_refCount++;
                field_13C_pArray->Push_Back(pDetector);
            }
        }
        field_110_state = SecurityClawStates::eIdle_1;
        field_6_flags.Clear(Options::eUpdateDuringCamSwap_Bit10);
        break;

    case SecurityClawStates::eIdle_1:
        if (Event_Get_417250(kEventAbeOhm_8))
        {
            field_114_timer = gnFrameCount_507670 + 20;
            field_110_state = SecurityClawStates::eDoZapEffects_2;
            field_130_pClaw->field_10_anim.Set_Animation_Data_402A40(22420, nullptr);
            SFX_Play_43AD70(95u, 60, 0);
            SFX_Play_43AE60(95u, 90, -1000, 0);
        }

        if (Event_Get_417250(kEvent_2))
        {
            if (!alarmInstanceCount_5076A8)
            {
                auto pAlarm = ao_new<Alarm>();
                if (pAlarm)
                {
                    pAlarm->ctor_402570(field_11A, field_118_alarm_id, 30, 39);
                }
            }
        }
        break;

    case SecurityClawStates::eDoZapEffects_2:
        if (static_cast<int>(gnFrameCount_507670) > field_114_timer)
        {
            PSX_RECT rect = {};
            sActiveHero_507678->VGetBoundingRect(&rect, 1);
            const FP hero_mid_x = FP_FromInteger((rect.w + rect.x) / 2);
            const FP hero_mid_y = FP_FromInteger((rect.h + rect.y) / 2);

            auto pScreenShake = ao_new<ScreenShake>();
            if (pScreenShake)
            {
                pScreenShake->ctor_4624D0(1);
            }

            auto pZapLine = ao_new<ZapLine>();
            if (pZapLine)
            {
                pZapLine->ctor_4789A0(
                    field_A8_xpos - (FP_FromInteger(3) * field_BC_sprite_scale),
                    field_AC_ypos + (FP_FromInteger(5) * field_BC_sprite_scale),
                    hero_mid_x,
                    hero_mid_y,
                    8, ZapLineType::eThick_0, 28);
            }

            auto pPossessionFlicker = ao_new<PossessionFlicker>();
            if (pPossessionFlicker)
            {
                pPossessionFlicker->ctor_41A8C0(
                    sActiveHero_507678,
                    8,
                    255,
                    100,
                    100);
            }

            sActiveHero_507678->VTakeDamage(this);

            auto pSpark = ao_new<Sparks>();
            if (pSpark)
            {
                pSpark->ctor_40A3A0(
                    field_A8_xpos,
                    field_AC_ypos - (FP_FromInteger(8) * field_BC_sprite_scale),
                    field_BC_sprite_scale);

                pSpark->field_C2_g = 65;
                pSpark->field_C4_b = 65;
                pSpark->field_C0_r = 255;
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
                auto pSpark3 = ao_new<Sparks>();
                if (pSpark3)
                {
                    pSpark3->ctor_40A3A0(
                        hero_mid_x,
                        hero_mid_y,
                        field_BC_sprite_scale);
                    pSpark3->field_C2_g = 65;
                    pSpark3->field_C4_b = 65;
                    pSpark3->field_C0_r = 255;
                }
            }

            field_110_state = SecurityClawStates::eAnimateClaw_DoFlashAndSound_3;
            field_114_timer = gnFrameCount_507670 + 8;
        }
        break;

    case SecurityClawStates::eAnimateClaw_DoFlashAndSound_3:
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
            SFX_Play_43AD70(57u, 0, 0);
        }
        else if (field_114_timer - gnFrameCount_507670 == 1)
        {
            SFX_Play_43AD70(58u, 0, 0);
        }

        if (static_cast<int>(gnFrameCount_507670) > field_114_timer)
        {
            field_110_state = SecurityClawStates::eIdle_1;
            field_130_pClaw->field_10_anim.Set_Animation_Data_402A40(22568, nullptr);
            SFX_Play_43AD70(97u, 0, 0);
        }
        break;

    default:
        return;
    }
}

void SecurityClaw::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    // Empty
}

}
