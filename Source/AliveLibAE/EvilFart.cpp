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
    SetType(ReliveTypes::eEvilFart);

    const AnimRecord& rec = AnimRec(AnimId::Fart);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);

    mApplyShadows &= ~1u;

    mBaseAnimatedWithPhysicsGameObject_SpriteScale = sActiveHero->mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    mBaseAnimatedWithPhysicsGameObject_Scale = sActiveHero->mBaseAnimatedWithPhysicsGameObject_Scale;
    if (mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SligGreeterFarts_33;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SligGreeterFarts_Half_14;
    }

    if (sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos + (FP_FromInteger(12) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos - (FP_FromInteger(12) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    mBaseAnimatedWithPhysicsGameObject_YPos = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(22)) + sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos;

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(3),
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(3),
            mBaseAnimatedWithPhysicsGameObject_YPos,
            &pLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? CollisionMask(eFlyingSligCeiling_17, eWallRight_2, eWallLeft_1) : CollisionMask(eBackgroundFlyingSligCeiling_18, eBackgroundWallRight_6, eBackgroundWallLeft_5)))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos;
    }
    
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit4_bPossesed);
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit8_bInvisible);

    ResetFartColour();

    field_124_state = FartStates::eIdle_0;
    field_118_bBlowUp = 0;

    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

    field_11A_bPossesed = 0;

    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;
    field_11C_alive_timer = 220;
}

s32 EvilFart::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const EvilFart_State*>(pBuffer);

    ResourceManager::LoadResourceFile_49C170("EVILFART.BAN", nullptr);
    ResourceManager::LoadResourceFile_49C170("EXPLO2.BAN", nullptr);
    ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", nullptr);

    auto pFart = relive_new EvilFart();

    if (pState->field_2C.Get(EvilFart_State::eBit1_bControlled))
    {
        sControlledCharacter_5C1B8C = pFart;
    }

    pFart->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_C_xpos;
    pFart->mBaseAnimatedWithPhysicsGameObject_YPos = pState->field_10_ypos;

    pFart->mBaseAnimatedWithPhysicsGameObject_VelX = pState->field_14_velx;
    pFart->mBaseAnimatedWithPhysicsGameObject_VelY = pState->field_18_vely;

    pFart->mBaseAnimatedWithPhysicsGameObject_PathNumber = pState->field_8_path_number;
    pFart->mBaseAnimatedWithPhysicsGameObject_LvlNumber = MapWrapper::FromAE(pState->field_A_lvl_number);
    pFart->mBaseAnimatedWithPhysicsGameObject_SpriteScale = pState->field_1C_sprite_scale;

    pFart->mBaseAnimatedWithPhysicsGameObject_RGB.r = pState->field_2_r;
    pFart->mBaseAnimatedWithPhysicsGameObject_RGB.g = pState->field_4_g;
    pFart->mBaseAnimatedWithPhysicsGameObject_RGB.b = pState->field_6_b;

    pFart->mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame = pState->field_20_anim_cur_frame;
    pFart->mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter = pState->field_22_frame_change_counter;

    pFart->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_25_bDrawable & 1);
    pFart->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_24_bAnimRender & 1);

    if (IsLastFrame(&pFart->mBaseAnimatedWithPhysicsGameObject_Anim))
    {
        pFart->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pFart->field_120_level = MapWrapper::FromAE(pState->field_26_level);
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

    pState->field_C_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->field_10_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;
    pState->field_14_velx = mBaseAnimatedWithPhysicsGameObject_VelX;
    pState->field_18_vely = mBaseAnimatedWithPhysicsGameObject_VelY;

    pState->field_8_path_number = mBaseAnimatedWithPhysicsGameObject_PathNumber;
    pState->field_A_lvl_number = MapWrapper::ToAE(mBaseAnimatedWithPhysicsGameObject_LvlNumber);
    pState->field_1C_sprite_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    pState->field_2_r = mBaseAnimatedWithPhysicsGameObject_RGB.r;
    pState->field_4_g = mBaseAnimatedWithPhysicsGameObject_RGB.g;
    pState->field_6_b = mBaseAnimatedWithPhysicsGameObject_RGB.b;

    pState->field_2C.Set(EvilFart_State::eBit1_bControlled, sControlledCharacter_5C1B8C == this);
    pState->field_20_anim_cur_frame = mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame;
    pState->field_22_frame_change_counter = mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter;

    pState->field_25_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_24_bAnimRender = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render);

    pState->field_26_level = MapWrapper::ToAE(field_120_level);
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
        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(4))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX += kFartSpeed;
        }
    }

    if (sInputKey_Left_5550D4 & pressedKeys)
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(-4))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX -= kFartSpeed;
        }
    }

    if (sInputKey_Down_5550DC & pressedKeys)
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(4))
        {
            mBaseAnimatedWithPhysicsGameObject_VelY += kFartSpeed;
        }
    }

    if (sInputKey_Up_5550D8 & pressedKeys)
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(-4))
        {
            mBaseAnimatedWithPhysicsGameObject_VelY -= kFartSpeed;
        }
    }

    if (!(pressedKeys & sInputKey_Right_5550D0) && !(pressedKeys & sInputKey_Left_5550D4))
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX -= kFartSpeed;
        }

        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX += kFartSpeed;
        }
    }

    if (!(pressedKeys & sInputKey_Up_5550D8) && !(pressedKeys & sInputKey_Down_5550DC))
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelY -= kFartSpeed;
        }

        if (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelY += kFartSpeed;
        }
    }
}

void EvilFart::VPossessed()
{
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit4_bPossesed);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    field_11C_alive_timer = 900;

    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;

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
    mBaseAnimatedWithPhysicsGameObject_RGB.r = greenFart.r;
    mBaseAnimatedWithPhysicsGameObject_RGB.g = greenFart.g;
    mBaseAnimatedWithPhysicsGameObject_RGB.b = greenFart.b;
}

s16 EvilFart::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    if (pFrom->Type() == ReliveTypes::eElectricWall)
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

    if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_86_HandstoneBegin_45BD00)
    {
        field_11C_alive_timer--;
    }

    if ((sActiveHero->mCurrentMotion != eAbeMotions::Motion_86_HandstoneBegin_45BD00) && field_11C_alive_timer + 1 <= 0)
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
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                field_118_bBlowUp = 1;
                field_12C_back_to_abe_timer = sGnFrame + 35;
            }
        }
    }

    if (field_118_bBlowUp && static_cast<s32>(sGnFrame) > field_12C_back_to_abe_timer)
    {
        sControlledCharacter_5C1B8C = sActiveHero;
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
                relive_new ThrowableTotalIndicator(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50)),
                    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer,
                    mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale,
                    field_11C_alive_timer / 50,
                    1);

                mBaseAnimatedWithPhysicsGameObject_YPos = mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50));
                Mudokon_SFX(MudSounds::eFart_7, 0, 10 * (300 - field_11C_alive_timer), this);
                mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50);
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
        if (FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_VelX) || FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_VelY))
        {
            if (!(sGnFrame % 3))
            {
                FP velocityToUse = {};

                FP directedVelY = mBaseAnimatedWithPhysicsGameObject_VelY;
                if (directedVelY < FP_FromInteger(0))
                {
                    directedVelY = -directedVelY;
                }

                FP directedVelX = mBaseAnimatedWithPhysicsGameObject_VelX;
                if (directedVelX < FP_FromInteger(0))
                {
                    directedVelX = -mBaseAnimatedWithPhysicsGameObject_VelX;
                }

                if (directedVelX <= directedVelY)
                {
                    if (mBaseAnimatedWithPhysicsGameObject_VelY >= FP_FromInteger(0))
                    {
                        velocityToUse = mBaseAnimatedWithPhysicsGameObject_VelY;
                    }
                    else
                    {
                        velocityToUse = -mBaseAnimatedWithPhysicsGameObject_VelY;
                    }
                }
                else if (mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0))
                {
                    velocityToUse = mBaseAnimatedWithPhysicsGameObject_VelX;
                }
                else
                {
                    velocityToUse = -mBaseAnimatedWithPhysicsGameObject_VelX;
                }

                New_Smoke_Particles(
                    mBaseAnimatedWithPhysicsGameObject_XPos * mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    (mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(55)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    FP_FromDouble(0.5) * mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    3,
                    static_cast<u8>(mBaseAnimatedWithPhysicsGameObject_RGB.r),
                    static_cast<u8>(mBaseAnimatedWithPhysicsGameObject_RGB.g),
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
            if (!(sGnFrame % 30) && !Math_RandomRange(0, 1))
            {
                Mudokon_SFX(MudSounds::eFart_7, 50, Math_RandomRange(-1500, -2000), nullptr);
            }
        }

        InputControlFart();
        SetActiveCameraDelayedFromDir();

        FP x2Offset = {};
        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            x2Offset = FP_FromInteger(-3);
        }
        else
        {
            FP_FromInteger(3);
        }

        FP y2Offset = {};
        if (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(0))
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
        if (sCollisions->Raycast(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(54)),
                x2Offset + mBaseAnimatedWithPhysicsGameObject_XPos + mBaseAnimatedWithPhysicsGameObject_VelX,
                y2Offset + mBaseAnimatedWithPhysicsGameObject_YPos + mBaseAnimatedWithPhysicsGameObject_VelY - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(54)),
                &pLine,
                &hitX,
                &hitY,
                mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? CollisionMask(eFlyingSligCeiling_17, eWallRight_2, eWallLeft_1) : CollisionMask(eBackgroundFlyingSligCeiling_18, eBackgroundWallRight_6, eBackgroundWallLeft_5)))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
        }

        if (sCollisions->Raycast(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(54)),
                mBaseAnimatedWithPhysicsGameObject_XPos + mBaseAnimatedWithPhysicsGameObject_VelX + x2Offset,
                y2Offset + mBaseAnimatedWithPhysicsGameObject_YPos + mBaseAnimatedWithPhysicsGameObject_VelY - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(54)),
                &pLine,
                &hitX,
                &hitY,
                mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? CollisionMask(eFlyingSligCeiling_17, eCeiling_3, eFloor_0, eDynamicCollision_32) : CollisionMask(eBackgroundFlyingSligCeiling_18, eBackgroundCeiling_7, eBackgroundFloor_4, eBackgroundDynamicCollision_36)))
        {
            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
        }

        if (!Input_IsChanting_45F260())
        {
            field_11A_bPossesed = 0;
        }

        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;
        if (mBaseAnimatedWithPhysicsGameObject_VelX == FP_FromInteger(0) && mBaseAnimatedWithPhysicsGameObject_VelY == FP_FromInteger(0))
        {
            if (Input_IsChanting_45F260())
            {
                if (!field_11A_bPossesed)
                {
                    field_124_state = FartStates::eDechanting_2;
                    field_128_timer = sGnFrame + 15;
                    field_12C_back_to_abe_timer = sGnFrame + 50;
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

        if (!(sGnFrame % 4))
        {
            if (field_118_bBlowUp)
            {
                return;
            }

            const FP yposOffset = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(Math_RandomRange(-20, 10)));
            const FP xposOffset = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(Math_RandomRange(-20, 20)));
            New_TintChant_Particle(
                xposOffset + mBaseAnimatedWithPhysicsGameObject_XPos,
                yposOffset + mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(54)),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                Layer::eLayer_0);
        }

        if (!field_118_bBlowUp && static_cast<s32>(sGnFrame) > field_128_timer)
        {
            BlowUp();

            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            field_118_bBlowUp = 1;
        }
        return;
    }
}

void EvilFart::BlowUp()
{
    relive_new Explosion(mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50)),
        mBaseAnimatedWithPhysicsGameObject_SpriteScale,
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
    mBaseAnimatedWithPhysicsGameObject_RGB.r = FP_GetExponent(FP_FromInteger(redFart.r) - (scaledValue * FP_FromInteger(redFart.r - greenFart.r)));
    mBaseAnimatedWithPhysicsGameObject_RGB.g = FP_GetExponent(FP_FromInteger(redFart.g) + (scaledValue * FP_FromInteger(greenFart.g - redFart.g)));
}
