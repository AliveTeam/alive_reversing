#include "stdafx.h"
#include "ChantSuppressor.hpp"
#include "Sound/Midi.hpp"
#include "stdlib.hpp"
#include "Gibs.hpp"
#include "ScreenShake.hpp"
#include "Events.hpp"
#include "Flash.hpp"
#include "ParticleBurst.hpp"
#include "Particle.hpp"
#include "Abe.hpp"
#include "PossessionFlicker.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Explosion.hpp"
#include "ZapLine.hpp"
#include "Function.hpp"

class Sparks : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Sparks* ctor_416390(FP xpos, FP ypos, FP scale)
    {
        BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
        SetVTable(this, 0x544534);
        field_4_typeId = Types::eType_22;

        BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSparksResID);
        Animation_Init_424E10(808, 19, 11, ppRes, 1, 1);
        
        field_DC_bApplyShadows &= ~1u;

        field_20_animation.field_C_render_layer = 37;
        field_20_animation.field_B_render_mode = 1;

        field_20_animation.field_A_b = 80;
        field_20_animation.field_9_g = 80;
        field_20_animation.field_8_r = 80;

        field_CC_sprite_scale = scale * (((FP_FromInteger(Math_NextRandom() % 6)) / FP_FromInteger(10)) + FP_FromDouble(0.7));
        field_FA_16_random = Math_RandomRange_496AB0(0, 16);

        field_B8_xpos = xpos;
        field_BC_ypos = ypos;

        field_C4_velx = FP_FromInteger(Math_RandomRange_496AB0(-8, 8));
        field_C8_vely = FP_FromInteger(Math_RandomRange_496AB0(-6, -3));
        return this;
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_416520(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_416570();
    }

    virtual void VScreenChanged() override
    {
        vScreenChanged_416720();
    }

private:

    EXPORT void vUpdate_416570()
    {
        if (field_FA_16_random > 0)
        {
            field_FA_16_random--;
        }

        if (field_FA_16_random == 0)
        {
            field_20_animation.Set_Animation_Data_409C80(808, 0);
            field_FA_16_random = -1;
        }

        field_C8_vely += FP_FromDouble(0.8);

        field_C4_velx *= FP_FromDouble(0.9);
        field_C8_vely *= FP_FromDouble(0.9);

        field_C4_velx += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
        field_C8_vely += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);

        field_BC_ypos += field_C8_vely;
        field_B8_xpos += field_C4_velx;

        if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }

    EXPORT void vScreenChanged_416720()
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    EXPORT void dtor_416550()
    {
        BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    }

    EXPORT Sparks* vdtor_416520(signed int flags)
    {
        dtor_416550();
        if (flags & 1)
        {
            ae_delete_free_495540(this);
        }
        return this;
    }

private:
    int field_F4_not_used1;
    __int16 field_F8_not_used2;
    __int16 field_FA_16_random;
};
ALIVE_ASSERT_SIZEOF(Sparks, 0xFC);

const TintEntry sChantOrbTints_55C1EC[18] =
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
    { 3u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};


ChantSuppressor* ChantSuppressor::ctor_466350(Path_ChantSuppressor* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x545F3C);
    
    field_4_typeId = Types::eAntiChant_83;

    field_6_flags.Set(BaseGameObject::eCanExplode_Bit7);

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kMaimGameResID);
    Animation_Init_424E10(2228, 53, 28u, ppRes, 1, 1u);
    
    SetTint_425600(sChantOrbTints_55C1EC, gMap_5C3030.field_0_current_level);

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_118_tlvInfo = tlvInfo;

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = 8;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
        field_20_animation.field_C_render_layer = 27;
    }

    field_DC_bApplyShadows |= 2u;
    field_11C_state = 0;
    field_124_sound_channels_mask = 0;

    return this;
}

ChantSuppressor* ChantSuppressor::vdtor_4664B0(signed int flags)
{
    dtor_4664E0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void ChantSuppressor::dtor_4664E0()
{
    SetVTable(this, 0x545F3C);

    if (field_124_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_124_sound_channels_mask);
    }

    if (field_10C_health > FP_FromInteger(0))
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 1);
    }

    dtor_4080B0();
}

void ChantSuppressor::vScreenChanged_466D20()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

signed __int16 ChantSuppressor::vTakeDamage_466BB0(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        return 0;
    }

    field_6_flags.Set(BaseGameObject::eDead_Bit3);
    field_10C_health = FP_FromInteger(0);

    if (pFrom->field_4_typeId == Types::eMineCar_89 || pFrom->field_4_typeId == Types::eType_104 || pFrom->field_4_typeId == Types::eShrykull_121)
    {
        auto pExplosionMem = ae_new<Explosion>();
        if (pExplosionMem)
        {
            pExplosionMem->ctor_4A1200(
                field_B8_xpos,
                field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(5)),
                field_CC_sprite_scale,
                0);
        }

        auto pGibsMem = ae_new<Gibs>();
        if (pGibsMem)
        {
            pGibsMem->ctor_40FB40(
                5,
                field_B8_xpos,
                field_BC_ypos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_CC_sprite_scale,
                0);
        }
    }

    return 1;
}

void ChantSuppressor::vUpdate_4665A0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_11C_state)
    {
        const int stateM1 = field_11C_state - 1;
        if (stateM1)
        {
            if (stateM1 == 1)
            {
                if (static_cast<int>(sGnFrame_5C1B84) == field_120_timer - 5 || static_cast<int>(sGnFrame_5C1B84) == field_120_timer - 1)
                {
                    auto pFlash1 = ae_new<Flash>();
                    if (pFlash1)
                    {
                        pFlash1->ctor_428570(39, 255, 0, 0, 1, 3, 1);
                    }
                }
                if (static_cast<int>(sGnFrame_5C1B84) == field_120_timer - 4)
                {
                    auto pFlash2 = ae_new<Flash>();
                    if (pFlash2)
                    {
                        pFlash2->ctor_428570(39, 255, 0, 0, 1, 1, 1);
                    }
                }

                const int timerFrame = field_120_timer - sGnFrame_5C1B84;
                if (timerFrame == 4)
                {
                    SFX_Play_46FA90(SoundEffect::Zap1_49, 0, field_CC_sprite_scale);
                }
                else if (timerFrame == 1)
                {
                    SFX_Play_46FA90(SoundEffect::Zap2_50, 0, field_CC_sprite_scale);
                }

                if (static_cast<int>(sGnFrame_5C1B84) > field_120_timer)
                {
                    field_11C_state = 0;
                }
            }
        }
        else if (static_cast<int>(sGnFrame_5C1B84) > field_120_timer)
        {
            PSX_RECT bRect = {};
            sActiveHero_5C1B68->vGetBoundingRect_424FD0(&bRect, 1);

            const FP xpos = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP ypos = FP_FromInteger((bRect.y + bRect.h) / 2);

            auto pZapLine = ae_new<ZapLine>();
            if (pZapLine)
            {
                pZapLine->ctor_4CC690(
                    field_B8_xpos,
                    field_BC_ypos - (FP_FromInteger(8) * field_CC_sprite_scale),
                    xpos,
                    ypos,
                    8,
                    0,
                    28);
            }

            auto v14 = ae_new<PossessionFlicker>();
            if (v14)
            {
                v14->ctor_4319E0(sActiveHero_5C1B68, 8, 255, 100, 100);
            }

            if (sActiveHero_5C1B68->field_10C_health > FP_FromInteger(0))
            {
                sActiveHero_5C1B68->VTakeDamage_408730(this);
            }

            field_120_timer = sGnFrame_5C1B84 + 8;
            field_11C_state = 2;

            auto pScreenShake = ae_new<ScreenShake>();
            if (pScreenShake)
            {
                pScreenShake->ctor_4ACF70(1, 0);
            }

            auto pUnknownObj = ae_new<Sparks>();
            if (pUnknownObj)
            {
                pUnknownObj->ctor_416390(field_B8_xpos, field_BC_ypos - (FP_FromInteger(8) * field_CC_sprite_scale), field_CC_sprite_scale);

                pUnknownObj->field_D2_g = 65;
                pUnknownObj->field_D4_b = 65;
                pUnknownObj->field_D0_r = 255;
            }

            auto pUnknownObj2 = ae_new<Sparks>();
            if (pUnknownObj2)
            {
                pUnknownObj2->ctor_416390(field_B8_xpos, field_BC_ypos - (FP_FromInteger(8) * field_CC_sprite_scale), field_CC_sprite_scale);

                pUnknownObj2->field_D2_g = 65;
                pUnknownObj2->field_D4_b = 65;
                pUnknownObj2->field_D0_r = 255;
            }

            for (int i = 0; i < 9; i++)
            {
                auto pUnknownObj4 = ae_new<Sparks>();
                if (pUnknownObj4)
                {
                    pUnknownObj4->ctor_416390(xpos, ypos, field_CC_sprite_scale);
                    pUnknownObj4->field_D2_g = 65;
                    pUnknownObj4->field_D4_b = 65;
                    pUnknownObj4->field_D0_r = 255;
                }
            }
        }
    }
    else
    {
        if (field_20_animation.field_92_current_frame == 2 ||
            field_20_animation.field_92_current_frame == 6 ||
            field_20_animation.field_92_current_frame == 10)
        {
            if (field_124_sound_channels_mask)
            {
                SND_Stop_Channels_Mask_4CA810(field_124_sound_channels_mask);
            }

            if (field_CC_sprite_scale == FP_FromDouble(0.5))
            {
                field_124_sound_channels_mask = SFX_Play_46FBA0(SoundEffect::SecurityOrb_48, 35, 720, field_CC_sprite_scale);
            }
            else
            {
                field_124_sound_channels_mask = SFX_Play_46FBA0(SoundEffect::SecurityOrb_48, 55, 700, field_CC_sprite_scale);
            }
        }

        if (Event_Get_422C00(kEventAbeOhm))
        {
            if (!sActiveHero_5C1B68->field_168_ring_pulse_timer ||
                !sActiveHero_5C1B68->field_16C_bHaveShrykull ||
                sActiveHero_5C1B68->field_CC_sprite_scale != FP_FromInteger(1))
            {
                field_11C_state = 1;
                field_120_timer = sGnFrame_5C1B84 + 20;
            }
        }
    }
}
