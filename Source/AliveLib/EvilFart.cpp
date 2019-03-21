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

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 6017); // TODO: Id

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

    field_124_bPlayerControlled = 0;
    field_118_bFartCountDown = 0;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    field_11A_isNotChanting = 0;

    field_20_animation.field_B_render_mode = 1;
    field_11C_k900 = 220;

    return this;
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

    field_11C_k900 = 900;
    
    field_20_animation.field_B_render_mode = 1;

    field_120_level = gMap_5C3030.sCurrentLevelId_5C3030;
    field_11E_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_122_camera = gMap_5C3030.sCurrentCamId_5C3034;

    sControlledCharacter_5C1B8C = this;

    field_124_bPlayerControlled = 1;
    field_11A_isNotChanting = 1;

    field_D2_g = 128;
    field_D0_r = 32;
    field_D4_b = 32;
}

signed __int16 EvilFart::VTakeDamage_423B70(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    if (pFrom->field_4_typeId == Types::eElectricWall_39)
    {
        field_11C_k900 = 0;
    }

    return 1;
}

void EvilFart::vUpdate_423100()
{
    NOT_IMPLEMENTED();

    /*
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (sActiveHero_5C1B68->field_106_current_state == 86 // State_86_HandstoneBegin_45BD00
        || (v2 = field_11C_k900, field_11C_k900 = v2 - 1, v2))
    {
    LABEL_11:
        if (!field_118_bFartCountDown)
        {
            goto LABEL_14;
        }
        goto LABEL_12;
    }

    if (!field_118_bFartCountDown)
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

        if (field_124_bPlayerControlled)
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_118_bFartCountDown = 1;
            field_12C_timer = sGnFrame_5C1B84 + 35;
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
        goto LABEL_11;
    }

LABEL_12:
    if (sGnFrame_5C1B84 > field_12C_timer)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        field_6_flags.Set(BaseGameObject::eDead);
        gMap_5C3030.SetActiveCam_480D30(field_120_level, field_11E_path, field_122_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
    }
LABEL_14:

    const __int16 k900Value = field_11C_k900;
    if (k900Value < 251 && !(k900Value % 50))
    {
        if (k900Value)
        {
            if (!field_118_bFartCountDown)
            {
                auto pIndicatorMem = alive_new<ThrowableTotalIndicator>();
                if (pIndicatorMem)
                {
                    v11 = Math_FixedPoint_Multiply_496C50(field_CC_sprite_scale, 0x320000);
                    pIndicatorMem->ctor_431CB0(
                        field_B8_xpos,
                        field_BC_ypos - v11,
                        field_20_animation.field_C_render_layer,
                        field_20_animation.field_14_scale,
                        ((unsigned int)((unsigned __int64)(1374389535i64 * field_11C_k900) >> 32) >> 31)
                        + ((signed int)((unsigned __int64)(1374389535i64 * field_11C_k900) >> 32) >> 4),
                        1);
                }
                v12 = field_CC_sprite_scale;
                hack1 = -1;
                fartPitch = Math_FixedPoint_Multiply_496C50(v12, 0x320000);
                v14 = field_BC_ypos - fartPitch;
                LOWORD(fartPitch) = 300 - field_11C_k900;
                field_BC_ypos = v14;
                Abe_SFX_457EC0(7u, 0, 10 * fartPitch, this);
                field_BC_ypos += Math_FixedPoint_Multiply_496C50(field_CC_sprite_scale,  0x320000);
            }
        }
    }

    if (!field_124_bPlayerControlled)
    {
        v43 = Math_FixedPoint_Multiply_496C50(0x8000, field_11C_k900 << 16);
        v42 = 0x8000;
        field_D0_r = (signed int)(0x800000 - v43) / 0x10000;
    LABEL_75:
        field_D2_g = (signed int)(Math_FixedPoint_Multiply_496C50(
            v42,
            field_11C_k900 << 16)
            + 0x260000)
            / 0x10000;
        return;
    }

    if (field_124_bPlayerControlled == 1)
    {
        velX1 = field_C4_velx;
        if (field_C4_velx / 0x10000
            || field_C8_vely / 0x10000)
        {
            if (!((signed int)sGnFrame_5C1B84 % 3))
            {
                velY1 = field_C8_vely;
                velY2 = field_C8_vely;
                if (velY1 < 0)
                {
                    velY2 = -velY2;
                }
                velX2 = field_C4_velx;
                if (velX1 < 0)
                {
                    velX2 = -velX1;
                }
                if (velX2 <= velY2)
                {
                    if (velY1 >= 0)
                    {
                        pVel = &field_C8_vely;
                    }
                    else
                    {
                        X2 = -velY1;
                        pVel = &X2;
                    }
                }
                else if (velX1 >= 0)
                {
                    pVel = &field_C4_velx;
                }
                else
                {
                    X2 = -velX1;
                    pVel = &X2;
                }
                v28 = Math_FixedPoint_Multiply_496C50(*pVel, 0xFA0000);
                v29 = Math_FixedPoint_Multiply_496C50(0x8000, field_CC_sprite_scale);
                v30 = Math_FixedPoint_Multiply_496C50(
                    field_BC_ypos - 3604480,
                    field_CC_sprite_scale);
                v31 = Math_FixedPoint_Multiply_496C50(
                    field_B8_xpos,
                    field_CC_sprite_scale);

                New_Particles_426C70(
                    v31,
                    v30,
                    v29,
                    3,
                    field_D0_r,
                    field_D2_g,
                    0x20u);

                if (field_130_sound_channels)
                {
                    SND_Stop_Channels_Mask_4CA810(field_130_sound_channels);
                }

                Abe_SFX_457EC0(7u, 50, (v28 >> 16) - 2000, 0);
                field_130_sound_channels = v32;
            }
        }
        else
        {
            if (field_130_sound_channels)
            {
                SND_Stop_Channels_Mask_4CA810(field_130_sound_channels);
                field_130_sound_channels = 0;
            }
            if (!((signed int)sGnFrame_5C1B84 % 30) && !Math_RandomRange_496AB0(0, 1))
            {
                LOWORD(v23) = Math_RandomRange_496AB0(-1500, -2000);
                Abe_SFX_457EC0(7u, 50, v23, 0);
            }
        }
        
        InputControlFart_423BB0();
        sub_408C40();

        v33 = 196608;
        if (field_C4_velx < FP_FromInteger(0))
        {
            v33 = -196608;
        }
        v34 = 196608;
        if (field_C8_vely < FP_FromInteger(0))
        {
            v34 = -196608;
        }

        v35 = v34
            + field_BC_ypos
            + field_C8_vely
            - Math_FixedPoint_Multiply_496C50(field_CC_sprite_scale, 3538944);
        X2 = v33 + field_B8_xpos + field_C4_velx;
        v36 = Math_FixedPoint_Multiply_496C50(field_CC_sprite_scale, 3538944);
        
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

        v37 = v34
            + field_BC_ypos
            + field_C8_vely
            - Math_FixedPoint_Multiply_496C50(field_CC_sprite_scale, 3538944);
        v38 = field_B8_xpos + field_C4_velx + v33;
        v39 = Math_FixedPoint_Multiply_496C50(field_CC_sprite_scale, 3538944);

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
            field_11A_isNotChanting = 0;
        }

        BYTE1(field_20_animation.field_4_flags) |= 0x40u;
        field_20_animation.field_B_render_mode = 1;
        if (!field_C4_velx && !field_C8_vely)
        {
            if (Input_IsChanting_45F260())
            {
                if (!field_11A_isNotChanting)
                {
                    field_124_bPlayerControlled = 2;
                    field_128_timer = sGnFrame_5C1B84 + 15;
                    field_12C_timer = sGnFrame_5C1B84 + 50;
                    SFX_Play_46FA90(0x11u, 0);
                }
            }
        }
        v41 = Math_FixedPoint_Multiply_496C50(6553, field_11C_k900 << 16);
        v42 = 6553;
        field_D0_r = (signed int)(0x800000 - v41) / 0x10000;
        goto LABEL_75;
    }

    if (field_124_bPlayerControlled != 2)
    {
        return;
    }

    if (!Input_IsChanting_45F260())
    {
        field_124_bPlayerControlled = 1;
        return;
    }

    if (!((signed int)sGnFrame_5C1B84 % 4))
    {
        if (field_118_bFartCountDown)
        {
            return;
        }
        v15 = Math_RandomRange_496AB0(-20, 20);
        v16 = Math_FixedPoint_Multiply_496C50(field_CC_sprite_scale, 0x360000);
        v17 = Math_RandomRange_496AB0(-20, 10);
        v18 = Math_FixedPoint_Multiply_496C50(
            field_CC_sprite_scale,
            (signed __int16)(((((signed int)((v17 << 16) - v16) >> 31) & 0xFFFF) + (v17 << 16) - v16) >> 16) << 16)
            + field_BC_ypos;
        v19 = Math_FixedPoint_Multiply_496C50(field_CC_sprite_scale, v15 << 16);

        New_Chant_Particle_426BE0(
            v19 + field_B8_xpos,
            v18,
            field_CC_sprite_scale,
            0);
    }

    if (!field_118_bFartCountDown && sGnFrame_5C1B84 > field_128_timer)
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
        field_118_bFartCountDown = 1;
    }
    */
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
