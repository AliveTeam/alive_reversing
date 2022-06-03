#include "stdafx.h"
#include "EvilFart.hpp"
#include "Abe.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "Particle.hpp"
#include "Sound/Midi.hpp"
#include "Explosion.hpp"
#include "Sfx.hpp"
#include "Function.hpp"

struct Colour final
{
    s16 r, g, b;
};

constexpr Colour greenFart = {32, 128, 32};
constexpr Colour redFart = {128, 38, 32};

EvilFart::EvilFart()
    : BaseAliveGameObject(0)
{
    SetType(AETypes::eEvilFart_45);

    const AnimRecord& rec = AnimRec(AnimId::Fart);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);

    field_DC_bApplyShadows &= ~1u;

    field_CC_sprite_scale = sActiveHero_5C1B68->field_CC_sprite_scale;

    field_D6_scale = sActiveHero_5C1B68->field_D6_scale;
    if (field_D6_scale == 1)
    {
        field_20_animation.mRenderLayer = Layer::eLayer_SligGreeterFarts_33;
    }
    else
    {
        field_20_animation.mRenderLayer = Layer::eLayer_SligGreeterFarts_Half_14;
    }

    if (sActiveHero_5C1B68->field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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
    if (sCollisions_DArray_5C1128->Raycast(
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

    field_20_animation.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit4_bPossesed);
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit8_bInvisible);

    ResetFartColour();

    field_124_state = FartStates::eIdle_0;
    field_118_bBlowUp = 0;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    field_11A_bPossesed = 0;

    field_20_animation.mRenderMode = TPageAbr::eBlend_1;
    field_11C_alive_timer = 220;
}

s32 EvilFart::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const EvilFart_State*>(pBuffer);

    ResourceManager::LoadResourceFile_49C170("EVILFART.BAN", nullptr);
    ResourceManager::LoadResourceFile_49C170("EXPLO2.BAN", nullptr);
    ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", nullptr);

    auto pFart = ae_new<EvilFart>();

    if (pState->field_2C.Get(EvilFart_State::eBit1_bControlled))
    {
        sControlledCharacter_5C1B8C = pFart;
    }

    pFart->field_B8_xpos = pState->field_C_xpos;
    pFart->field_BC_ypos = pState->field_10_ypos;

    pFart->field_C4_velx = pState->field_14_velx;
    pFart->field_C8_vely = pState->field_18_vely;

    pFart->field_C0_path_number = pState->field_8_path_number;
    pFart->field_C2_lvl_number = pState->field_A_lvl_number;
    pFart->field_CC_sprite_scale = pState->field_1C_sprite_scale;

    pFart->field_D0_r = pState->field_2_r;
    pFart->field_D2_g = pState->field_4_g;
    pFart->field_D4_b = pState->field_6_b;

    pFart->field_20_animation.field_92_current_frame = pState->field_20_anim_cur_frame;
    pFart->field_20_animation.mFrameChangeCounter = pState->field_22_frame_change_counter;

    pFart->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_25_bDrawable & 1);
    pFart->field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_24_bAnimRender & 1);

    if (IsLastFrame(&pFart->field_20_animation))
    {
        pFart->field_20_animation.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pFart->field_120_level = pState->field_26_level;
    pFart->field_11E_path = pState->field_28_path;
    pFart->field_122_camera = pState->field_2A_camera;
    pFart->field_118_bBlowUp = pState->field_2C.Get(EvilFart_State::eBit2_bBlowUp);
    pFart->field_11C_alive_timer = pState->field_2E_alive_timer;
    pFart->field_124_state = pState->field_30_state;
    pFart->field_128_timer = pState->field_34_timer;
    pFart->field_12C_back_to_abe_timer = pState->field_38_timer;
    return sizeof(EvilFart_State);
}

s32 EvilFart::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<EvilFart_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eEvilFart_45;

    pState->field_C_xpos = field_B8_xpos;
    pState->field_10_ypos = field_BC_ypos;
    pState->field_14_velx = field_C4_velx;
    pState->field_18_vely = field_C8_vely;

    pState->field_8_path_number = field_C0_path_number;
    pState->field_A_lvl_number = field_C2_lvl_number;
    pState->field_1C_sprite_scale = field_CC_sprite_scale;

    pState->field_2_r = field_D0_r;
    pState->field_4_g = field_D2_g;
    pState->field_6_b = field_D4_b;

    pState->field_2C.Set(EvilFart_State::eBit1_bControlled, sControlledCharacter_5C1B8C == this);
    pState->field_20_anim_cur_frame = field_20_animation.field_92_current_frame;
    pState->field_22_frame_change_counter = field_20_animation.mFrameChangeCounter;

    pState->field_25_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_24_bAnimRender = field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render);

    pState->field_26_level = field_120_level;
    pState->field_28_path = field_11E_path;
    pState->field_2A_camera = field_122_camera;
    pState->field_2C.Set(EvilFart_State::eBit2_bBlowUp, field_118_bBlowUp & 1);
    pState->field_2E_alive_timer = field_11C_alive_timer;
    pState->field_30_state = field_124_state;
    pState->field_34_timer = field_128_timer;
    pState->field_38_timer = field_12C_back_to_abe_timer;
    return sizeof(EvilFart_State);
}

void EvilFart::InputControlFart()
{
    const FP kFartSpeed = FP_FromDouble(0.2);
    const u32 pressedKeys = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

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

void EvilFart::VPossessed()
{
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit4_bPossesed);
    field_20_animation.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    field_11C_alive_timer = 900;

    field_20_animation.mRenderMode = TPageAbr::eBlend_1;

    field_120_level = gMap.mCurrentLevel;
    field_11E_path = gMap.mCurrentPath;
    field_122_camera = gMap.mCurrentCamera;

    sControlledCharacter_5C1B8C = this;

    field_124_state = FartStates::eFlying_1;
    field_11A_bPossesed = 1;

    ResetFartColour();
}

void EvilFart::ResetFartColour()
{
    field_D0_r = greenFart.r;
    field_D2_g = greenFart.g;
    field_D4_b = greenFart.b;
}

s16 EvilFart::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    if (pFrom->Type() == AETypes::eElectricWall_39)
    {
        field_11C_alive_timer = 0;
    }

    return 1;
}

void EvilFart::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (sActiveHero_5C1B68->mCurrentMotion != eAbeMotions::Motion_86_HandstoneBegin_45BD00)
    {
        field_11C_alive_timer--;
    }

    if ((sActiveHero_5C1B68->mCurrentMotion != eAbeMotions::Motion_86_HandstoneBegin_45BD00) && field_11C_alive_timer + 1 <= 0)
    {
        if (!field_118_bBlowUp)
        {
            BlowUp();
            if (field_124_state == FartStates::eIdle_0)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            else
            {
                field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                field_118_bBlowUp = 1;
                field_12C_back_to_abe_timer = sGnFrame_5C1B84 + 35;
            }
        }
    }

    if (field_118_bBlowUp && static_cast<s32>(sGnFrame_5C1B84) > field_12C_back_to_abe_timer)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        gMap.SetActiveCam(field_120_level, field_11E_path, field_122_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
    }

    // Show the count to the boom
    if (field_11C_alive_timer < 251 && !(field_11C_alive_timer % 50))
    {
        if (field_11C_alive_timer > 0)
        {
            if (!field_118_bBlowUp)
            {
                ae_new<ThrowableTotalIndicator>(
                    field_B8_xpos,
                    field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(50)),
                    field_20_animation.mRenderLayer,
                    field_20_animation.field_14_scale,
                    field_11C_alive_timer / 50,
                    1);

                field_BC_ypos = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(50));
                Mudokon_SFX(MudSounds::eFart_7, 0, 10 * (300 - field_11C_alive_timer), this);
                field_BC_ypos += field_CC_sprite_scale * FP_FromInteger(50);
            }
        }
    }

    if (field_124_state == FartStates::eIdle_0)
    {
        CalculateFartColour();
        return;
    }

    if (field_124_state == FartStates::eFlying_1)
    {
        if (FP_GetExponent(field_C4_velx) || FP_GetExponent(field_C8_vely))
        {
            if (!(sGnFrame_5C1B84 % 3))
            {
                FP velocityToUse = {};

                FP directedVelY = field_C8_vely;
                if (directedVelY < FP_FromInteger(0))
                {
                    directedVelY = -directedVelY;
                }

                FP directedVelX = field_C4_velx;
                if (directedVelX < FP_FromInteger(0))
                {
                    directedVelX = -field_C4_velx;
                }

                if (directedVelX <= directedVelY)
                {
                    if (field_C8_vely >= FP_FromInteger(0))
                    {
                        velocityToUse = field_C8_vely;
                    }
                    else
                    {
                        velocityToUse = -field_C8_vely;
                    }
                }
                else if (field_C4_velx >= FP_FromInteger(0))
                {
                    velocityToUse = field_C4_velx;
                }
                else
                {
                    velocityToUse = -field_C4_velx;
                }

                New_Smoke_Particles(
                    field_B8_xpos * field_CC_sprite_scale,
                    (field_BC_ypos - FP_FromInteger(55)) * field_CC_sprite_scale,
                    FP_FromDouble(0.5) * field_CC_sprite_scale,
                    3,
                    static_cast<u8>(field_D0_r),
                    static_cast<u8>(field_D2_g),
                    0x20u);


                if (field_130_sound_channels)
                {
                    SND_Stop_Channels_Mask(field_130_sound_channels);
                }

                Mudokon_SFX(MudSounds::eFart_7, 50, FP_GetExponent(velocityToUse * FP_FromInteger(250)) - 2000, nullptr);
                field_130_sound_channels = 0; // TODO OG BUG ?? v32;
            }
        }
        else
        {
            if (field_130_sound_channels)
            {
                SND_Stop_Channels_Mask(field_130_sound_channels);
                field_130_sound_channels = 0;
            }
            if (!(sGnFrame_5C1B84 % 30) && !Math_RandomRange(0, 1))
            {
                Mudokon_SFX(MudSounds::eFart_7, 50, Math_RandomRange(-1500, -2000), nullptr);
            }
        }

        InputControlFart();
        SetActiveCameraDelayedFromDir();

        FP x2Offset = {};
        if (field_C4_velx < FP_FromInteger(0))
        {
            x2Offset = FP_FromInteger(-3);
        }
        else
        {
            FP_FromInteger(3);
        }

        FP y2Offset = {};
        if (field_C8_vely < FP_FromInteger(0))
        {
            y2Offset = FP_FromInteger(-3);
        }
        else
        {
            y2Offset = FP_FromInteger(3);
        }

        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_5C1128->Raycast(
                field_B8_xpos,
                field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(54)),
                x2Offset + field_B8_xpos + field_C4_velx,
                y2Offset + field_BC_ypos + field_C8_vely - (field_CC_sprite_scale * FP_FromInteger(54)),
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

        if (sCollisions_DArray_5C1128->Raycast(
                field_B8_xpos,
                field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(54)),
                field_B8_xpos + field_C4_velx + x2Offset,
                y2Offset + field_BC_ypos + field_C8_vely - (field_CC_sprite_scale * FP_FromInteger(54)),
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

        field_20_animation.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

        field_20_animation.mRenderMode = TPageAbr::eBlend_1;
        if (field_C4_velx == FP_FromInteger(0) && field_C8_vely == FP_FromInteger(0))
        {
            if (Input_IsChanting_45F260())
            {
                if (!field_11A_bPossesed)
                {
                    field_124_state = FartStates::eDechanting_2;
                    field_128_timer = sGnFrame_5C1B84 + 15;
                    field_12C_back_to_abe_timer = sGnFrame_5C1B84 + 50;
                    SFX_Play_Mono(SoundEffect::PossessEffect_17, 0);
                }
            }
        }

        CalculateFartColour();
        return;
    }

    if (field_124_state == FartStates::eDechanting_2)
    {
        if (!Input_IsChanting_45F260())
        {
            field_124_state = FartStates::eFlying_1;
            return;
        }

        if (!(sGnFrame_5C1B84 % 4))
        {
            if (field_118_bBlowUp)
            {
                return;
            }

            const FP yposOffset = (field_CC_sprite_scale * FP_FromInteger(Math_RandomRange(-20, 10)));
            const FP xposOffset = (field_CC_sprite_scale * FP_FromInteger(Math_RandomRange(-20, 20)));
            New_TintChant_Particle(
                xposOffset + field_B8_xpos,
                yposOffset + field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(54)),
                field_CC_sprite_scale,
                Layer::eLayer_0);
        }

        if (!field_118_bBlowUp && static_cast<s32>(sGnFrame_5C1B84) > field_128_timer)
        {
            BlowUp();

            field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            field_118_bBlowUp = 1;
        }
        return;
    }
}

void EvilFart::BlowUp()
{
    ae_new<Explosion>(field_B8_xpos,
        field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(50)),
        field_CC_sprite_scale,
        0);
}

void EvilFart::CalculateFartColour()
{
    FP scaledValue;
    if (field_124_state == FartStates::eIdle_0)
    {
        scaledValue = FP_FromInteger(field_11C_alive_timer) / FP_FromInteger(220);
    }
    else
    {
        scaledValue = FP_FromInteger(field_11C_alive_timer) / FP_FromInteger(900);
    }
    // Linear change from greenFart to redFart
    field_D0_r = FP_GetExponent(FP_FromInteger(redFart.r) - (scaledValue * FP_FromInteger(redFart.r - greenFart.r)));
    field_D2_g = FP_GetExponent(FP_FromInteger(redFart.g) + (scaledValue * FP_FromInteger(greenFart.g - redFart.g)));
}
