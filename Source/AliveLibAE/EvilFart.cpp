#include "stdafx.h"
#include "EvilFart.hpp"
#include "Abe.hpp"
#include "../relive_lib/Collisions.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Game.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "AirExplosion.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Function.hpp"
#include "Map.hpp"
#include "Input.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "Math.hpp"

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

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Fart));
    Animation_Init(GetAnimRes(AnimId::Fart));

    SetApplyShadowZoneColour(false);

    SetSpriteScale(sActiveHero->GetSpriteScale());

    SetScale(sActiveHero->GetScale());
    if (GetScale() == Scale::Fg)
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBats_33);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBat_Half_14);
    }

    if (sActiveHero->GetAnimation().GetFlipX())
    {
        mXPos = sActiveHero->mXPos + (FP_FromInteger(12) * GetSpriteScale());
    }
    else
    {
        mXPos = sActiveHero->mXPos - (FP_FromInteger(12) * GetSpriteScale());
    }

    mYPos = (GetSpriteScale() * FP_FromInteger(22)) + sActiveHero->mYPos;

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
            GetScale() == Scale::Fg ? CollisionMask(eFlyingObjectWall_17, eWallRight_2, eWallLeft_1) : CollisionMask(eBackgroundFlyingObjectWall_18, eBackgroundWallRight_6, eBackgroundWallLeft_5)))
    {
        mXPos = sActiveHero->mXPos;
    }
    
    GetAnimation().SetSemiTrans(true);

    SetPossessed(false);
    SetCanBePossessed(true);
    SetInvisible(true);

    ResetFartColour();

    GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
    mPossessedAliveTimer = 220;
}

s32 EvilFart::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const EvilFartSaveState*>(pBuffer);

    auto pFart = relive_new EvilFart();

    if (pState->mControlled)
    {
        sControlledCharacter = pFart;
    }

    pFart->mXPos = pState->mXPos;
    pFart->mYPos = pState->mYPos;

    pFart->mVelX = pState->mVelX;
    pFart->mVelY = pState->mVelY;

    pFart->mCurrentPath = pState->mCurrentPath;
    pFart->mCurrentLevel = pState->mCurrentLevel;
    pFart->SetSpriteScale(pState->mSpriteScale);

    pFart->mRGB.SetRGB(pState->mRed, pState->mGreen, pState->mBlue);

    pFart->GetAnimation().SetCurrentFrame(pState->mCurrentFrame);
    pFart->GetAnimation().SetFrameChangeCounter(pState->mFrameChangeCounter);

    pFart->SetDrawable(pState->mDrawable & 1);
    pFart->GetAnimation().SetRender(pState->mAnimRender & 1);

    if (IsLastFrame(&pFart->GetAnimation()))
    {
        pFart->GetAnimation().SetIsLastFrame(true);
    }

    pFart->mAbeLevel = pState->mAbeLevel;
    pFart->mAbePath = pState->mAbePath;
    pFart->mAbeCamera = pState->mAbeCamera;
    pFart->mFartExploded = pState->mFartExploded;
    pFart->mPossessedAliveTimer = pState->mPossessedAliveTimer;
    pFart->mState = pState->mState;
    pFart->mUnpossessionTimer = pState->mUnpossessionTimer;
    pFart->mBackToAbeTimer = pState->mBackToAbeTimer;
    return sizeof(EvilFartSaveState);
}

s32 EvilFart::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<EvilFartSaveState*>(pSaveBuffer);

    pState->mType = ReliveTypes::eEvilFart;

    pState->mXPos = mXPos;
    pState->mYPos = mYPos;
    pState->mVelX = mVelX;
    pState->mVelY = mVelY;

    pState->mCurrentPath = mCurrentPath;
    pState->mCurrentLevel = mCurrentLevel;
    pState->mSpriteScale = GetSpriteScale();

    pState->mRed = mRGB.r;
    pState->mGreen = mRGB.g;
    pState->mBlue = mRGB.b;

    if (sControlledCharacter == this)
    {
        pState->mControlled = true;
    }

    pState->mCurrentFrame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    pState->mFrameChangeCounter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());

    pState->mDrawable = GetDrawable();
    pState->mAnimRender = GetAnimation().GetRender();

    pState->mAbeLevel = mAbeLevel;
    pState->mAbePath = mAbePath;
    pState->mAbeCamera = mAbeCamera;
    pState->mFartExploded = mFartExploded;
    pState->mPossessedAliveTimer = mPossessedAliveTimer;
    pState->mState = mState;
    pState->mUnpossessionTimer = mUnpossessionTimer;
    pState->mBackToAbeTimer = mBackToAbeTimer;
    return sizeof(EvilFartSaveState);
}

void EvilFart::InputControlFart()
{
    const FP kFartSpeed = FP_FromDouble(0.2);
    const u32 held = Input().GetHeld();

    if (InputCommands::eRight & held)
    {
        if (mVelX < FP_FromInteger(4))
        {
            mVelX += kFartSpeed;
        }
    }

    if (InputCommands::eLeft & held)
    {
        if (mVelX > FP_FromInteger(-4))
        {
            mVelX -= kFartSpeed;
        }
    }

    if (InputCommands::eDown & held)
    {
        if (mVelY < FP_FromInteger(4))
        {
            mVelY += kFartSpeed;
        }
    }

    if (InputCommands::eUp & held)
    {
        if (mVelY > FP_FromInteger(-4))
        {
            mVelY -= kFartSpeed;
        }
    }

    if (!(held & InputCommands::eRight) && !(held & InputCommands::eLeft))
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

    if (!(held & InputCommands::eUp) && !(held & InputCommands::eDown))
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
    SetPossessed(true);
    GetAnimation().SetSemiTrans(true);

    mPossessedAliveTimer = 900;

    GetAnimation().SetRenderMode(TPageAbr::eBlend_1);

    mAbeLevel = gMap.mCurrentLevel;
    mAbePath = gMap.mCurrentPath;
    mAbeCamera = gMap.mCurrentCamera;

    sControlledCharacter = this;

    mState = FartStates::eFlying_1;
    mPossessed = true;

    ResetFartColour();
}

void EvilFart::ResetFartColour()
{
    mRGB.SetRGB(greenFart.r, greenFart.g, greenFart.b);
}

bool EvilFart::VTakeDamage(BaseGameObject* pFrom)
{
    if (GetDead())
    {
        return false;
    }

    if (pFrom->Type() == ReliveTypes::eElectricWall)
    {
        mPossessedAliveTimer = 0;
    }

    return true;
}

void EvilFart::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_86_HandstoneBegin)
    {
        mPossessedAliveTimer--;
    }

    if ((sActiveHero->mCurrentMotion != eAbeMotions::Motion_86_HandstoneBegin) && mPossessedAliveTimer + 1 <= 0)
    {
        if (!mFartExploded)
        {
            BlowUp();
            if (mState == FartStates::eIdle_0)
            {
                SetDead(true);
            }
            else
            {
                GetAnimation().SetRender(false);
                mFartExploded = 1;
                mBackToAbeTimer = sGnFrame + 35;
            }
        }
    }

    if (mFartExploded && static_cast<s32>(sGnFrame) > mBackToAbeTimer)
    {
        sControlledCharacter = sActiveHero;
        SetDead(true);
        gMap.SetActiveCam(mAbeLevel, mAbePath, mAbeCamera, CameraSwapEffects::eInstantChange_0, 0, 0);
    }

    // Show the count to the boom
    if (mPossessedAliveTimer < 251 && !(mPossessedAliveTimer % 50))
    {
        if (mPossessedAliveTimer > 0)
        {
            if (!mFartExploded)
            {
                relive_new ThrowableTotalIndicator(
                    mXPos,
                    mYPos - (GetSpriteScale() * FP_FromInteger(50)),
                    GetAnimation().GetRenderLayer(),
                    GetAnimation().GetSpriteScale(),
                    mPossessedAliveTimer / 50,
                    1);

                mYPos = mYPos - (GetSpriteScale() * FP_FromInteger(50));
                Mudokon_SFX(MudSounds::eFart_7, 0, 10 * (300 - mPossessedAliveTimer), this);
                mYPos += GetSpriteScale() * FP_FromInteger(50);
            }
        }
    }

    if (mState == FartStates::eIdle_0)
    {
        CalculateFartColour();
        return;
    }

    if (mState == FartStates::eFlying_1)
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
                    mXPos * GetSpriteScale(),
                    (mYPos - FP_FromInteger(55)) * GetSpriteScale(),
                    FP_FromDouble(0.5) * GetSpriteScale(),
                    3,
                    RGB16{mRGB.r, mRGB.b, 32});


                if (mSoundChannels)
                {
                    SND_Stop_Channels_Mask(mSoundChannels);
                }

                Mudokon_SFX(MudSounds::eFart_7, 50, FP_GetExponent(velocityToUse * FP_FromInteger(250)) - 2000, nullptr);
                mSoundChannels = 0; // TODO OG BUG ?? v32;
            }
        }
        else
        {
            if (mSoundChannels)
            {
                SND_Stop_Channels_Mask(mSoundChannels);
                mSoundChannels = 0;
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
                mYPos - (GetSpriteScale() * FP_FromInteger(54)),
                x2Offset + mXPos + mVelX,
                y2Offset + mYPos + mVelY - (GetSpriteScale() * FP_FromInteger(54)),
                &pLine,
                &hitX,
                &hitY,
                GetScale() == Scale::Fg ? CollisionMask(eFlyingObjectWall_17, eWallRight_2, eWallLeft_1) : CollisionMask(eBackgroundFlyingObjectWall_18, eBackgroundWallRight_6, eBackgroundWallLeft_5)))
        {
            mVelX = FP_FromInteger(0);
        }
        else
        {
            mXPos += mVelX;
        }

        if (sCollisions->Raycast(
                mXPos,
                mYPos - (GetSpriteScale() * FP_FromInteger(54)),
                mXPos + mVelX + x2Offset,
                y2Offset + mYPos + mVelY - (GetSpriteScale() * FP_FromInteger(54)),
                &pLine,
                &hitX,
                &hitY,
                GetScale() == Scale::Fg ? CollisionMask(eFlyingObjectWall_17, eCeiling_3, eFloor_0, eDynamicCollision_32) : CollisionMask(eBackgroundFlyingObjectWall_18, eBackgroundCeiling_7, eBackgroundFloor_4, eBackgroundDynamicCollision_36)))
        {
            mVelY = FP_FromInteger(0);
        }
        else
        {
            mYPos += mVelY;
        }

        if (!Input_IsChanting())
        {
            mPossessed = false;
        }

        GetAnimation().SetSemiTrans(true);

        GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
        if (mVelX == FP_FromInteger(0) && mVelY == FP_FromInteger(0))
        {
            if (Input_IsChanting())
            {
                if (!mPossessed)
                {
                    mState = FartStates::eDechanting_2;
                    mUnpossessionTimer = sGnFrame + 15;
                    mBackToAbeTimer = sGnFrame + 50;
                    SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
                }
            }
        }

        CalculateFartColour();
        return;
    }

    if (mState == FartStates::eDechanting_2)
    {
        if (!Input_IsChanting())
        {
            mState = FartStates::eFlying_1;
            return;
        }

        if (!(sGnFrame % 4))
        {
            if (mFartExploded)
            {
                return;
            }

            const FP yposOffset = (GetSpriteScale() * FP_FromInteger(Math_RandomRange(-20, 10)));
            const FP xposOffset = (GetSpriteScale() * FP_FromInteger(Math_RandomRange(-20, 20)));
            New_TintChant_Particle(
                xposOffset + mXPos,
                yposOffset + mYPos - (GetSpriteScale() * FP_FromInteger(54)),
                GetSpriteScale(),
                Layer::eLayer_0);
        }

        if (!mFartExploded && static_cast<s32>(sGnFrame) > mUnpossessionTimer)
        {
            BlowUp();

            GetAnimation().SetRender(false);
            mFartExploded = 1;
        }
        return;
    }
}

void EvilFart::BlowUp()
{
    relive_new AirExplosion(mXPos,
        mYPos - (GetSpriteScale() * FP_FromInteger(50)),
        GetSpriteScale(),
        0);
}

void EvilFart::CalculateFartColour()
{
    FP scaledValue;
    if (mState == FartStates::eIdle_0)
    {
        scaledValue = FP_FromInteger(mPossessedAliveTimer) / FP_FromInteger(220);
    }
    else
    {
        scaledValue = FP_FromInteger(mPossessedAliveTimer) / FP_FromInteger(900);
    }
    // Linear change from greenFart to redFart
    mRGB.r = FP_GetExponent(FP_FromInteger(redFart.r) - (scaledValue * FP_FromInteger(redFart.r - greenFart.r)));
    mRGB.g = FP_GetExponent(FP_FromInteger(redFart.g) + (scaledValue * FP_FromInteger(greenFart.g - redFart.g)));
}
