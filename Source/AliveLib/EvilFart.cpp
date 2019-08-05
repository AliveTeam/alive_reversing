#include "stdafx.h"
#include "EvilFart.hpp"
#include "Abe.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "Particle.hpp"
#include "Midi.hpp"
#include "Explosion.hpp"
#include "Sfx.hpp"
#include "Function.hpp"

EvilFart* EvilFart::ctor_422E30()
{
    ctor_408240(0);
    SetVTable(this, 0x544BE0);

    field_4_typeId = Types::eType_45_EvilFart;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kEvilFartResID);

    Animation_Init_424E10(3532, 61, 39, ppRes, 1, 1);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID);

    field_DC_bApplyShadows &= ~1u;
    
    field_CC_sprite_scale = sActiveHero_5C1B68->field_CC_sprite_scale;
    
    field_D6_scale = sActiveHero_5C1B68->field_D6_scale;
    if (field_D6_scale == 1)
    {
        field_20_animation.field_C_render_layer = 33;
    }
    else
    {
        field_20_animation.field_C_render_layer = 14;
    }

    if (sActiveHero_5C1B68->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B8_xpos = sActiveHero_5C1B68->field_B8_xpos + (FP_FromInteger(12) * field_CC_sprite_scale);
    }
    else
    {
        field_B8_xpos = sActiveHero_5C1B68->field_B8_xpos - (FP_FromInteger(12) * field_CC_sprite_scale);
    }

    field_BC_ypos = (field_CC_sprite_scale * FP_FromInteger(22)) + sActiveHero_5C1B68->field_BC_ypos;

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos - FP_FromInteger(3),
        field_BC_ypos,
        field_B8_xpos + FP_FromInteger(3),
        field_BC_ypos,
        &pLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 0x20006 : 0x40060))
    {
        field_B8_xpos = sActiveHero_5C1B68->field_B8_xpos;
    }

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
    field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Set(Flags_114::e114_Bit8);

    field_D0_r = 32;
    field_D2_g = 128;
    field_D4_b = 32;

    field_124_state = 0;
    field_118_bBlowUp = 0;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    field_11A_bPossesed = 0;

    field_20_animation.field_B_render_mode = 1;
    field_11C_alive_timer = 220;

    return this;
}

BaseGameObject* EvilFart::VDestructor(signed int flags)
{
    return vdtor_4230D0(flags);
}

void EvilFart::VUpdate()
{
    vUpdate_423100();
}

__int16 EvilFart::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return VTakeDamage_423B70(pFrom);
}

void EvilFart::VPossessed_408F70()
{
    vOnPossesed_423DA0();
}

void EvilFart::InputControlFart_423BB0()
{
    const FP kFartSpeed = FP_FromDouble(0.2);
    const DWORD pressedKeys = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    if (sInputKey_Right_5550D0 & pressedKeys)
    {
        if (field_C4_velx < FP_FromInteger(4))
        {
            field_C4_velx += kFartSpeed;
        }
    }

    if (sInputKey_Left_5550D4 & pressedKeys)
    {
        if (field_C4_velx > FP_FromInteger(-4))
        {
            field_C4_velx -= kFartSpeed;
        }
    }

    if (sInputKey_Down_5550DC & pressedKeys)
    {
        if (field_C8_vely < FP_FromInteger(4))
        {
            field_C8_vely += kFartSpeed;
        }
    }

    if (sInputKey_Up_5550D8 & pressedKeys)
    {
        if (field_C8_vely > FP_FromInteger(-4))
        {
            field_C8_vely -= kFartSpeed;
        }
    }

    if (!(pressedKeys & sInputKey_Right_5550D0) && !(pressedKeys & sInputKey_Left_5550D4))
    {

        if (field_C4_velx > FP_FromInteger(0))
        {
            field_C4_velx -= kFartSpeed;
        }

        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx += kFartSpeed;
        }
    }

    if (!(pressedKeys & sInputKey_Up_5550D8) && !(pressedKeys & sInputKey_Down_5550DC))
    {
        if (field_C8_vely > FP_FromInteger(0))
        {
            field_C8_vely -= kFartSpeed;
        }

        if (field_C8_vely < FP_FromInteger(0))
        {
            field_C8_vely += kFartSpeed;
        }
    }
}

void EvilFart::vOnPossesed_423DA0()
{
    field_114_flags.Set(Flags_114::e114_Bit4_bPossesed);
    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_11C_alive_timer = 900;
    
    field_20_animation.field_B_render_mode = 1;

    field_120_level = gMap_5C3030.sCurrentLevelId_5C3030;
    field_11E_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_122_camera = gMap_5C3030.sCurrentCamId_5C3034;

    sControlledCharacter_5C1B8C = this;

    field_124_state = 1;
    field_11A_bPossesed = 1;

    field_D0_r = 32;
    field_D2_g = 128;
    field_D4_b = 32;
}

__int16 EvilFart::VTakeDamage_423B70(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    if (pFrom->field_4_typeId == Types::eElectricWall_39)
    {
        field_11C_alive_timer = 0;
    }

    return 1;
}

void EvilFart::vUpdate_423100()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    field_11C_alive_timer--;

    if (sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_86_HandstoneBegin_45BD00 && field_11C_alive_timer == 0)
    {
        if (!field_118_bBlowUp)
        {
            auto pExplosionMem = alive_new<Explosion>();
            if (pExplosionMem)
            {
                pExplosionMem->ctor_4A1200(
                    field_B8_xpos,
                    field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(50)),
                    field_CC_sprite_scale,
                    0);
            }
        }

        if (field_124_state == 0)
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
        else
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_118_bBlowUp = 1;
            field_12C_back_to_abe_timer = sGnFrame_5C1B84 + 35;
        }
    }

    if (field_118_bBlowUp && static_cast<int>(sGnFrame_5C1B84) > field_12C_back_to_abe_timer)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        field_6_flags.Set(BaseGameObject::eDead);
        gMap_5C3030.SetActiveCam_480D30(field_120_level, field_11E_path, field_122_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
    }

    // Show the count to the boom
    if (field_11C_alive_timer < 251 && !(field_11C_alive_timer % 50))
    {
        if (field_11C_alive_timer > 0)
        {
            if (!field_118_bBlowUp)
            {
                auto pIndicatorMem = alive_new<ThrowableTotalIndicator>();
                if (pIndicatorMem)
                {
                    pIndicatorMem->ctor_431CB0(
                        field_B8_xpos,
                        field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(50)),
                        field_20_animation.field_C_render_layer,
                        field_20_animation.field_14_scale,
                        field_11C_alive_timer / 50,
                        1);
                }

                field_BC_ypos = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(50));
                Abe_SFX_457EC0(7, 0, 10 * (300 - field_11C_alive_timer), this);
                field_BC_ypos += field_CC_sprite_scale * FP_FromInteger(50);
            }
        }
    }

    if (field_124_state == 0)
    {
        const FP scaledValue = FP_FromInteger(field_11C_alive_timer) / FP_FromInteger(900);
        field_D0_r = FP_GetExponent(FP_FromInteger(128) - (scaledValue * FP_FromInteger(128)));
        field_D2_g = FP_GetExponent(FP_FromInteger(38) + (scaledValue * FP_FromInteger(128)));
        return;
    }
    else if (field_124_state == 1)
    {
        FP velX1 = field_C4_velx;
        if (FP_GetExponent(field_C4_velx) || FP_GetExponent(field_C8_vely))
        {
            if (!(sGnFrame_5C1B84 % 3))
            {
                FP* pVel = nullptr;
                FP X2 = {};

                FP velY1 = field_C8_vely;
                FP velY2 = field_C8_vely;

                if (velY1 < FP_FromInteger(0))
                {
                    velY2 = -velY2;
                }
                FP velX2 = field_C4_velx;

                if (velX1 < FP_FromInteger(0))
                {
                    velX2 = -velX1;
                }

                if (velX2 <= velY2)
                {
                    if (velY1 >= FP_FromInteger(0))
                    {
                        pVel = &field_C8_vely;
                    }
                    else
                    {
                        X2 = -velY1;
                        pVel = &X2;
                    }
                }
                else if (velX1 >= FP_FromInteger(0))
                {
                    pVel = &field_C4_velx;
                }
                else
                {
                    X2 = -velX1;
                    pVel = &X2;
                }

                const FP v28 = (*pVel * FP_FromInteger(250));
                const FP v29 = (FP_FromDouble(0.5) * field_CC_sprite_scale);
                const FP v30 = (field_BC_ypos - FP_FromInteger(55)) * field_CC_sprite_scale;
                const FP v31 = (field_B8_xpos * field_CC_sprite_scale);

                New_Particles_426C70(
                    v31,
                    v30,
                    v29,
                    3,
                    static_cast<BYTE>(field_D0_r),
                    static_cast<BYTE>(field_D2_g),
                    0x20u);
                

                if (field_130_sound_channels)
                {
                    SND_Stop_Channels_Mask_4CA810(field_130_sound_channels);
                }

                Abe_SFX_457EC0(7, 50, FP_GetExponent(v28) - 2000, nullptr);
                field_130_sound_channels = 0; // OG BUG ?? v32;
            }
        }
        else
        {
            if (field_130_sound_channels)
            {
                SND_Stop_Channels_Mask_4CA810(field_130_sound_channels);
                field_130_sound_channels = 0;
            }
            if (!(sGnFrame_5C1B84 % 30) && !Math_RandomRange_496AB0(0, 1))
            {
                Abe_SFX_457EC0(7, 50, Math_RandomRange_496AB0(-1500, -2000), nullptr);
            }
        }
        
        InputControlFart_423BB0();
        sub_408C40();

        
        FP v33 = FP_FromInteger(3);
        if (field_C4_velx < FP_FromInteger(0))
        {
            v33 = FP_FromInteger(-3);
        }

        FP v34 = FP_FromInteger(3);
        if (field_C8_vely < FP_FromInteger(0))
        {
            v34 = FP_FromInteger(-3);
        }

        const FP v35 = v34 + field_BC_ypos + field_C8_vely - (field_CC_sprite_scale * FP_FromInteger(54));
        const FP X2 = v33 + field_B8_xpos + field_C4_velx;
        const FP v36 = (field_CC_sprite_scale * FP_FromInteger(54));
        
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            field_BC_ypos - v36,
            X2,
            v35,
            &pLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 0x20006 : 0x40060))
        {
            field_C4_velx = FP_FromInteger(0);
        }
        else
        {
            field_B8_xpos += field_C4_velx;
        }

        const FP v37 = v34 + field_BC_ypos + field_C8_vely - (field_CC_sprite_scale * FP_FromInteger(54));
        const FP v38 = field_B8_xpos + field_C4_velx + v33;
        const FP v39 = (field_CC_sprite_scale * FP_FromInteger(54));

        if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            field_BC_ypos - v39,
            v38,
            v37,
            &pLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 131081 : 262288))
        {
            field_C8_vely = FP_FromInteger(0);
        }
        else
        {
            field_BC_ypos += field_C8_vely;
        }

        if (!Input_IsChanting_45F260())
        {
            field_11A_bPossesed = 0;
        }

        field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

        field_20_animation.field_B_render_mode = 1;
        if (field_C4_velx == FP_FromInteger(0) && field_C8_vely == FP_FromInteger(0))
        {
            if (Input_IsChanting_45F260())
            {
                if (!field_11A_bPossesed)
                {
                    field_124_state = 2;
                    field_128_timer = sGnFrame_5C1B84 + 15;
                    field_12C_back_to_abe_timer = sGnFrame_5C1B84 + 50;
                    SFX_Play_46FA90(0x11u, 0);
                }
            }
        }

        const FP scaledValue = FP_FromInteger(field_11C_alive_timer) / FP_FromInteger(900);
        field_D0_r = FP_GetExponent(FP_FromInteger(128) - (scaledValue * FP_FromInteger(128)));
        field_D2_g = FP_GetExponent(FP_FromInteger(38) + (scaledValue * FP_FromInteger(128)));
        return;
    }
    else if (field_124_state == 2)
    {
        if (!Input_IsChanting_45F260())
        {
            field_124_state = 1;
            return;
        }

        if (!(sGnFrame_5C1B84 % 4))
        {
            if (field_118_bBlowUp)
            {
                return;
            }

            const short v15 = Math_RandomRange_496AB0(-20, 20);
            const FP v16 = (field_CC_sprite_scale * FP_FromInteger(54));
            const short v17 = Math_RandomRange_496AB0(-20, 10);
            const FP v18 = (field_CC_sprite_scale * FP_NoFractional(FP_FromInteger(v17) - v16) + (FP_FromInteger(v17) - v16)) + field_BC_ypos;
            const FP v19 = (field_CC_sprite_scale * FP_FromInteger(v15));

            New_Chant_Particle_426BE0(
                v19 + field_B8_xpos,
                v18,
                field_CC_sprite_scale,
                0);
        }

        if (!field_118_bBlowUp && static_cast<int>(sGnFrame_5C1B84) > field_128_timer)
        {
            auto pExplosionMem2 = alive_new<Explosion>();
            if (pExplosionMem2)
            {
                pExplosionMem2->ctor_4A1200(
                    field_B8_xpos,
                    field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(50)),
                    field_CC_sprite_scale,
                    0);
            }

            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_118_bBlowUp = 1;
        }
        return;
    }
}

void EvilFart::dtor_423D80()
{
    SetVTable(this, 0x544BE0);
    dtor_4080B0();
}

EvilFart* EvilFart::vdtor_4230D0(signed int flags)
{
    dtor_423D80();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}
