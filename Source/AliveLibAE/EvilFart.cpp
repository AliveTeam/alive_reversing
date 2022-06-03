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

    mApplyShadows &= ~1u;

    mSpriteScale = sActiveHero->mSpriteScale;

    mScale = sActiveHero->mScale;
    if (mScale == 1)
    {
        mAnim.mRenderLayer = Layer::eLayer_SligGreeterFarts_33;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_SligGreeterFarts_Half_14;
    }

    if (sActiveHero->mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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
    if (sCollisions_DArray_5C1128->Raycast(
            mXPos - FP_FromInteger(3),
            mYPos,
            mXPos + FP_FromInteger(3),
            mYPos,
            &pLine,
            &hitX,
            &hitY,
            mScale != 0 ? 0x20006 : 0x40060))
    {
        mXPos = sActiveHero->mXPos;
    }

    mAnim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    mAliveGameObjectFlags.Clear(Flags_114::e114_Bit4_bPossesed);
    mAliveGameObjectFlags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    mAliveGameObjectFlags.Set(Flags_114::e114_Bit8_bInvisible);

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

    auto pFart = ae_new<EvilFart>();

    if (pState->field_2C.Get(EvilFart_State::eBit1_bControlled))
    {
        sControlledCharacter = pFart;
    }

    pFart->mXPos = pState->field_C_xpos;
    pFart->mYPos = pState->field_10_ypos;

    pFart->mVelX = pState->field_14_velx;
    pFart->mVelY = pState->field_18_vely;

    pFart->mPathNumber = pState->field_8_path_number;
    pFart->mLvlNumber = pState->field_A_lvl_number;
    pFart->mSpriteScale = pState->field_1C_sprite_scale;

    pFart->mRed = pState->field_2_r;
    pFart->mGreen = pState->field_4_g;
    pFart->mBlue = pState->field_6_b;

    pFart->mAnim.field_92_current_frame = pState->field_20_anim_cur_frame;
    pFart->mAnim.mFrameChangeCounter = pState->field_22_frame_change_counter;

    pFart->mGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_25_bDrawable & 1);
    pFart->mAnim.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_24_bAnimRender & 1);

    if (IsLastFrame(&pFart->mAnim))
    {
        pFart->mAnim.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
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

    pState->field_C_xpos = mXPos;
    pState->field_10_ypos = mYPos;
    pState->field_14_velx = mVelX;
    pState->field_18_vely = mVelY;

    pState->field_8_path_number = mPathNumber;
    pState->field_A_lvl_number = mLvlNumber;
    pState->field_1C_sprite_scale = mSpriteScale;

    pState->field_2_r = mRed;
    pState->field_4_g = mGreen;
    pState->field_6_b = mBlue;

    pState->field_2C.Set(EvilFart_State::eBit1_bControlled, sControlledCharacter == this);
    pState->field_20_anim_cur_frame = mAnim.field_92_current_frame;
    pState->field_22_frame_change_counter = mAnim.mFrameChangeCounter;

    pState->field_25_bDrawable = mGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_24_bAnimRender = mAnim.mAnimFlags.Get(AnimFlags::eBit3_Render);

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
    mAliveGameObjectFlags.Set(Flags_114::e114_Bit4_bPossesed);
    mAnim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

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
    mRed = greenFart.r;
    mGreen = greenFart.g;
    mBlue = greenFart.b;
}

s16 EvilFart::VTakeDamage(BaseGameObject* pFrom)
{
    if (mGameObjectFlags.Get(BaseGameObject::eDead))
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
        mGameObjectFlags.Set(BaseGameObject::eDead);
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
                mGameObjectFlags.Set(BaseGameObject::eDead);
            }
            else
            {
                mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                field_118_bBlowUp = 1;
                field_12C_back_to_abe_timer = sGnFrame_5C1B84 + 35;
            }
        }
    }

    if (field_118_bBlowUp && static_cast<s32>(sGnFrame_5C1B84) > field_12C_back_to_abe_timer)
    {
        sControlledCharacter = sActiveHero;
        mGameObjectFlags.Set(BaseGameObject::eDead);
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
            if (!(sGnFrame_5C1B84 % 3))
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
                    static_cast<u8>(mRed),
                    static_cast<u8>(mGreen),
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
        if (sCollisions_DArray_5C1128->Raycast(
                mXPos,
                mYPos - (mSpriteScale * FP_FromInteger(54)),
                x2Offset + mXPos + mVelX,
                y2Offset + mYPos + mVelY - (mSpriteScale * FP_FromInteger(54)),
                &pLine,
                &hitX,
                &hitY,
                mScale != 0 ? 0x20006 : 0x40060))
        {
            mVelX = FP_FromInteger(0);
        }
        else
        {
            mXPos += mVelX;
        }

        if (sCollisions_DArray_5C1128->Raycast(
                mXPos,
                mYPos - (mSpriteScale * FP_FromInteger(54)),
                mXPos + mVelX + x2Offset,
                y2Offset + mYPos + mVelY - (mSpriteScale * FP_FromInteger(54)),
                &pLine,
                &hitX,
                &hitY,
                mScale != 0 ? 131081 : 262288))
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

        mAnim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

        mAnim.mRenderMode = TPageAbr::eBlend_1;
        if (mVelX == FP_FromInteger(0) && mVelY == FP_FromInteger(0))
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

            const FP yposOffset = (mSpriteScale * FP_FromInteger(Math_RandomRange(-20, 10)));
            const FP xposOffset = (mSpriteScale * FP_FromInteger(Math_RandomRange(-20, 20)));
            New_TintChant_Particle(
                xposOffset + mXPos,
                yposOffset + mYPos - (mSpriteScale * FP_FromInteger(54)),
                mSpriteScale,
                Layer::eLayer_0);
        }

        if (!field_118_bBlowUp && static_cast<s32>(sGnFrame_5C1B84) > field_128_timer)
        {
            BlowUp();

            mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            field_118_bBlowUp = 1;
        }
        return;
    }
}

void EvilFart::BlowUp()
{
    ae_new<Explosion>(mXPos,
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
    mRed = FP_GetExponent(FP_FromInteger(redFart.r) - (scaledValue * FP_FromInteger(redFart.r - greenFart.r)));
    mGreen = FP_GetExponent(FP_FromInteger(redFart.g) + (scaledValue * FP_FromInteger(greenFart.g - redFart.g)));
}
