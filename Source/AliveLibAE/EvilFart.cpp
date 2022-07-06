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
    Animation_Init(AnimId::Fart, ppRes);

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

    mSpriteScale = sActiveHero->mSpriteScale;

    mScale = sActiveHero->mScale;
    if (mScale == Scale::Fg)
    {
        mAnim.mRenderLayer = Layer::eLayer_SligGreeterFartsBats_33;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_SligGreeterFartsBat_Half_14;
    }

    if (sActiveHero->mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mXPos = sActiveHero->mXPos + (FP_FromInteger(12) * mSpriteScale);
    }
    else
    {
        mXPos = sActiveHero->mXPos - (FP_FromInteger(12) * mSpriteScale);
    }

    mYPos = (mSpriteScale * FP_FromInteger(22)) + sActiveHero->mYPos;

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions->Raycast(
            mXPos - FP_FromInteger(3),
            mYPos,
            mXPos + FP_FromInteger(3),
            mYPos,
            &pLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? CollisionMask(eFlyingSligCeiling_17, eWallRight_2, eWallLeft_1) : CollisionMask(eBackgroundFlyingSligCeiling_18, eBackgroundWallRight_6, eBackgroundWallLeft_5)))
    {
        mXPos = sActiveHero->mXPos;
    }
    
    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);

    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit4_bPossesed);
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit8_bInvisible);

    ResetFartColour();

    field_124_state = FartStates::eIdle_0;
    field_118_bBlowUp = 0;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    field_11A_bPossesed = 0;

    mAnim.mRenderMode = TPageAbr::eBlend_1;
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
        sControlledCharacter = pFart;
    }

    pFart->mXPos = pState->field_C_xpos;
    pFart->mYPos = pState->field_10_ypos;

    pFart->mVelX = pState->field_14_velx;
    pFart->mVelY = pState->field_18_vely;

    pFart->mCurrentPath = pState->field_8_path_number;
    pFart->mCurrentLevel = MapWrapper::FromAE(pState->field_A_lvl_number);
    pFart->mSpriteScale = pState->field_1C_sprite_scale;

    pFart->mRGB.SetRGB(pState->field_2_r, pState->field_4_g, pState->field_6_b);

    pFart->mAnim.mCurrentFrame = pState->field_20_anim_cur_frame;
    pFart->mAnim.mFrameChangeCounter = pState->field_22_frame_change_counter;

    pFart->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_25_bDrawable & 1);
    pFart->mAnim.mFlags.Set(AnimFlags::eBit3_Render, pState->field_24_bAnimRender & 1);

    if (IsLastFrame(&pFart->mAnim))
    {
        pFart->mAnim.mFlags.Set(AnimFlags::eBit18_IsLastFrame);
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

    pState->field_C_xpos = mXPos;
    pState->field_10_ypos = mYPos;
    pState->field_14_velx = mVelX;
    pState->field_18_vely = mVelY;

    pState->field_8_path_number = mCurrentPath;
    pState->field_A_lvl_number = MapWrapper::ToAE(mCurrentLevel);
    pState->field_1C_sprite_scale = mSpriteScale;

    pState->field_2_r = mRGB.r;
    pState->field_4_g = mRGB.g;
    pState->field_6_b = mRGB.b;

    pState->field_2C.Set(EvilFart_State::eBit1_bControlled, sControlledCharacter == this);
    pState->field_20_anim_cur_frame = mAnim.mCurrentFrame;
    pState->field_22_frame_change_counter = mAnim.mFrameChangeCounter;

    pState->field_25_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_24_bAnimRender = mAnim.mFlags.Get(AnimFlags::eBit3_Render);

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
        if (mVelX < FP_FromInteger(4))
        {
            mVelX += kFartSpeed;
        }
    }

    if (sInputKey_Left_5550D4 & pressedKeys)
    {
        if (mVelX > FP_FromInteger(-4))
        {
            mVelX -= kFartSpeed;
        }
    }

    if (sInputKey_Down_5550DC & pressedKeys)
    {
        if (mVelY < FP_FromInteger(4))
        {
            mVelY += kFartSpeed;
        }
    }

    if (sInputKey_Up_5550D8 & pressedKeys)
    {
        if (mVelY > FP_FromInteger(-4))
        {
            mVelY -= kFartSpeed;
        }
    }

    if (!(pressedKeys & sInputKey_Right_5550D0) && !(pressedKeys & sInputKey_Left_5550D4))
    {
        if (mVelX > FP_FromInteger(0))
        {
            mVelX -= kFartSpeed;
        }

        if (mVelX < FP_FromInteger(0))
        {
            mVelX += kFartSpeed;
        }
    }

    if (!(pressedKeys & sInputKey_Up_5550D8) && !(pressedKeys & sInputKey_Down_5550DC))
    {
        if (mVelY > FP_FromInteger(0))
        {
            mVelY -= kFartSpeed;
        }

        if (mVelY < FP_FromInteger(0))
        {
            mVelY += kFartSpeed;
        }
    }
}

void EvilFart::VPossessed()
{
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit4_bPossesed);
    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);

    field_11C_alive_timer = 900;

    mAnim.mRenderMode = TPageAbr::eBlend_1;

    field_120_level = gMap.mCurrentLevel;
    field_11E_path = gMap.mCurrentPath;
    field_122_camera = gMap.mCurrentCamera;

    sControlledCharacter = this;

    field_124_state = FartStates::eFlying_1;
    field_11A_bPossesed = 1;

    ResetFartColour();
}

void EvilFart::ResetFartColour()
{
    mRGB.SetRGB(greenFart.r, greenFart.g, greenFart.b);
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
    if (EventGet(kEventDeathReset))
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
                mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
                field_118_bBlowUp = 1;
                field_12C_back_to_abe_timer = sGnFrame + 35;
            }
        }
    }

    if (field_118_bBlowUp && static_cast<s32>(sGnFrame) > field_12C_back_to_abe_timer)
    {
        sControlledCharacter = sActiveHero;
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
                    mXPos,
                    mYPos - (mSpriteScale * FP_FromInteger(50)),
                    mAnim.mRenderLayer,
                    mAnim.field_14_scale,
                    field_11C_alive_timer / 50,
                    1);

                mYPos = mYPos - (mSpriteScale * FP_FromInteger(50));
                Mudokon_SFX(MudSounds::eFart_7, 0, 10 * (300 - field_11C_alive_timer), this);
                mYPos += mSpriteScale * FP_FromInteger(50);
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
        if (FP_GetExponent(mVelX) || FP_GetExponent(mVelY))
        {
            if (!(sGnFrame % 3))
            {
                FP velocityToUse = {};

                FP directedVelY = mVelY;
                if (directedVelY < FP_FromInteger(0))
                {
                    directedVelY = -directedVelY;
                }

                FP directedVelX = mVelX;
                if (directedVelX < FP_FromInteger(0))
                {
                    directedVelX = -mVelX;
                }

                if (directedVelX <= directedVelY)
                {
                    if (mVelY >= FP_FromInteger(0))
                    {
                        velocityToUse = mVelY;
                    }
                    else
                    {
                        velocityToUse = -mVelY;
                    }
                }
                else if (mVelX >= FP_FromInteger(0))
                {
                    velocityToUse = mVelX;
                }
                else
                {
                    velocityToUse = -mVelX;
                }

                New_Smoke_Particles(
                    mXPos * mSpriteScale,
                    (mYPos - FP_FromInteger(55)) * mSpriteScale,
                    FP_FromDouble(0.5) * mSpriteScale,
                    3,
                    static_cast<u8>(mRGB.r),
                    static_cast<u8>(mRGB.g),
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
        if (mVelX < FP_FromInteger(0))
        {
            x2Offset = FP_FromInteger(-3);
        }
        else
        {
            FP_FromInteger(3);
        }

        FP y2Offset = {};
        if (mVelY < FP_FromInteger(0))
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
                mXPos,
                mYPos - (mSpriteScale * FP_FromInteger(54)),
                x2Offset + mXPos + mVelX,
                y2Offset + mYPos + mVelY - (mSpriteScale * FP_FromInteger(54)),
                &pLine,
                &hitX,
                &hitY,
                mScale == Scale::Fg ? CollisionMask(eFlyingSligCeiling_17, eWallRight_2, eWallLeft_1) : CollisionMask(eBackgroundFlyingSligCeiling_18, eBackgroundWallRight_6, eBackgroundWallLeft_5)))
        {
            mVelX = FP_FromInteger(0);
        }
        else
        {
            mXPos += mVelX;
        }

        if (sCollisions->Raycast(
                mXPos,
                mYPos - (mSpriteScale * FP_FromInteger(54)),
                mXPos + mVelX + x2Offset,
                y2Offset + mYPos + mVelY - (mSpriteScale * FP_FromInteger(54)),
                &pLine,
                &hitX,
                &hitY,
                mScale == Scale::Fg ? CollisionMask(eFlyingSligCeiling_17, eCeiling_3, eFloor_0, eDynamicCollision_32) : CollisionMask(eBackgroundFlyingSligCeiling_18, eBackgroundCeiling_7, eBackgroundFloor_4, eBackgroundDynamicCollision_36)))
        {
            mVelY = FP_FromInteger(0);
        }
        else
        {
            mYPos += mVelY;
        }

        if (!Input_IsChanting_45F260())
        {
            field_11A_bPossesed = 0;
        }

        mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);

        mAnim.mRenderMode = TPageAbr::eBlend_1;
        if (mVelX == FP_FromInteger(0) && mVelY == FP_FromInteger(0))
        {
            if (Input_IsChanting_45F260())
            {
                if (!field_11A_bPossesed)
                {
                    field_124_state = FartStates::eDechanting_2;
                    field_128_timer = sGnFrame + 15;
                    field_12C_back_to_abe_timer = sGnFrame + 50;
                    SfxPlayMono(SoundEffect::PossessEffect_17, 0);
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

            const FP yposOffset = (mSpriteScale * FP_FromInteger(Math_RandomRange(-20, 10)));
            const FP xposOffset = (mSpriteScale * FP_FromInteger(Math_RandomRange(-20, 20)));
            New_TintChant_Particle(
                xposOffset + mXPos,
                yposOffset + mYPos - (mSpriteScale * FP_FromInteger(54)),
                mSpriteScale,
                Layer::eLayer_0);
        }

        if (!field_118_bBlowUp && static_cast<s32>(sGnFrame) > field_128_timer)
        {
            BlowUp();

            mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
            field_118_bBlowUp = 1;
        }
        return;
    }
}

void EvilFart::BlowUp()
{
    relive_new Explosion(mXPos,
        mYPos - (mSpriteScale * FP_FromInteger(50)),
        mSpriteScale,
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
    mRGB.r = FP_GetExponent(FP_FromInteger(redFart.r) - (scaledValue * FP_FromInteger(redFart.r - greenFart.r)));
    mRGB.g = FP_GetExponent(FP_FromInteger(redFart.g) + (scaledValue * FP_FromInteger(greenFart.g - redFart.g)));
}
