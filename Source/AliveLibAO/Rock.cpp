#include "stdafx_ao.h"
#include "Rock.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Grenade.hpp"
#include "ThrowableArray.hpp"
#include "Math.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "../relive_lib/Particle.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "../AliveLibAE/FixedPoint.hpp"

namespace AO {

Rock::Rock(FP xpos, FP ypos, s16 count)
    : BaseThrowable()
{
    SetType(ReliveTypes::eRock);

    mBaseThrowableDead = 0;

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Rock));
    Animation_Init(GetAnimRes(AnimId::Rock));

    SetInteractive(false);
    GetAnimation().SetRender(false);
    GetAnimation().SetSemiTrans(false);

    mXPos = xpos;
    mPreviousXPos = xpos;

    mYPos = ypos;
    mPreviousYPos = ypos;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    mBaseThrowableCount = count;
    mState = States::eNone_0;

    mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::BlueRock));

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        // TODO: I think this only existed in certain lvls, will need a way to know
        // which pal to use per lvl/path
        GetAnimation().LoadPal(GetPalRes(PalId::BlueRock));
    }
    else
    {
        GetAnimation().ReloadPal();
    }

    mBounceCount = 0;

    CreateShadow();
}

Rock::~Rock()
{
    if (!gInfiniteGrenades && !mBaseThrowableDead)
    {
        if (gThrowableArray)
        {
            gThrowableArray->Remove(mBaseThrowableCount >= 1u ? mBaseThrowableCount : 1);
        }
    }
}

void Rock::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    switch (mState)
    {
        case States::eFallingOutOfRockSack_1:
            InTheAir();
            break;

        case States::eRolling_2:
            if (FP_Abs(mVelX) >= FP_FromInteger(1))
            {
                if (mVelX < FP_FromInteger(0))
                {
                    mVelX += FP_FromDouble(0.01);
                }
                else
                {
                    mVelX -= FP_FromDouble(0.01);
                }

                mPathLine->MoveOnLine(
                    &mXPos,
                    &mYPos,
                    mVelX);

                if (!mPathLine)
                {
                    mState = States::eBouncing_4;
                    GetAnimation().SetLoop(true);
                }
            }
            else
            {
                const s16 x_exp = FP_GetExponent(mXPos);
                const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid(GetSpriteScale(), x_exp & 0x3FF);
                if (abs(xSnapped - x_exp) > 1)
                {
                    mPathLine = mPathLine->MoveOnLine(
                        &mXPos,
                        &mYPos,
                        mVelX);
                    if (!mPathLine)
                    {
                        mState = States::eBouncing_4;
                        GetAnimation().SetLoop(true);
                    }
                }
                else
                {
                    mVelX = FP_FromInteger(0);
                    mCollectionRect.x = mXPos - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                    mCollectionRect.w = mXPos + (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));

                    SetInteractive(true);

                    GetAnimation().SetLoop(false);
                    mCollectionRect.y = mYPos - ScaleToGridSize(GetSpriteScale());
                    mCollectionRect.h = mYPos;
                    mState = States::eOnGround_3;
                    mShimmerTimer = sGnFrame;
                }
            }
            break;

        case States::eOnGround_3:
            if (static_cast<s32>(sGnFrame) > mShimmerTimer)
            {
                New_TintShiny_Particle(
                    (GetSpriteScale() * FP_FromInteger(1)) + mXPos,
                    (GetSpriteScale() * FP_FromInteger(-7)) + mYPos,
                    FP_FromDouble(0.3),
                    Layer::eLayer_Foreground_36);
                mShimmerTimer = (Math_NextRandom() % 16) + sGnFrame + 60;
            }
            break;

        case States::eBouncing_4:
        {
            InTheAir();
            const PSX_RECT bRect = VGetBoundingRect();
            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            OnCollisionWith(xy, wh, gBaseGameObjects);

            if (mVelY > FP_FromInteger(30))
            {
                mState = States::eFallingOutOfWorld_5;
            }
        }
        break;

        case States::eFallingOutOfWorld_5:
            mVelY += FP_FromInteger(1);
            mXPos += mVelX;
            mYPos += mVelY;
            if (!gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                SetDead(true);
            }
            break;
        default:
            return;
    }
}

//TODO Identical to AE - merge
void Rock::VScreenChanged()
{
    if (gMap.mCurrentPath != gMap.mNextPath
        || gMap.mCurrentLevel != gMap.mNextLevel)
    {
        SetDead(true);
    }
}

//TODO Identical to AE - merge
void Rock::VThrow(FP velX, FP velY)
{
    mVelX = velX;
    mVelY = velY;

    GetAnimation().SetRender(true);

    if (mBaseThrowableCount == 0)
    {
        mState = States::eBouncing_4;
    }
    else
    {
        mState = States::eFallingOutOfRockSack_1;
    }
}

bool Rock::VCanThrow()
{
    return mState == States::eBouncing_4;
}

void Rock::InTheAir()
{
    mPreviousXPos = mXPos;
    mPreviousYPos = mYPos;

    if (mVelY > FP_FromInteger(30))
    {
        SetDead(true);
    }

    mVelY += FP_FromInteger(1);

    mXPos += mVelX;
    mYPos += mVelY;

    u16 result = 0;
    mXPos = CamX_VoidSkipper(mXPos, mVelX, 8, &result);
    mYPos = CamY_VoidSkipper(mYPos, mVelY, 8, &result);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mPreviousXPos,
            mPreviousYPos,
            mXPos,
            mYPos,
            &mPathLine,
            &hitX,
            &hitY,
            GetSpriteScale() == FP_FromInteger(1) ? kFgWallsOrFloor : kBgWallsOrFloor))
    {
        switch (mPathLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                if (mVelY > FP_FromInteger(0))
                {
                    if (mState != States::eBouncing_4 || mVelY >= FP_FromInteger(5))
                    {
                        if (mState != States::eFallingOutOfRockSack_1 || mVelY >= FP_FromInteger(1))
                        {
                            mYPos = hitY;
                            mVelY = (-mVelY / FP_FromInteger(2));
                            mVelX = (mVelX / FP_FromInteger(2));
                            s32 vol = 20 * (4 - mBounceCount);
                            if (vol < 40)
                            {
                                vol = 40;
                            }
                            SfxPlayMono(relive::SoundEffects::RockBounce, vol);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            mBounceCount++;
                        }
                        else
                        {
                            mState = States::eRolling_2;
                            if (mVelX >= FP_FromInteger(0) && mVelX < FP_FromInteger(1))
                            {
                                mVelX = FP_FromInteger(1);
                            }

                            if (mVelX < FP_FromInteger(0) && mVelX > FP_FromInteger(-1))
                            {
                                mVelX = FP_FromInteger(-1);
                            }
                        }
                    }
                    else
                    {
                        mState = States::eFallingOutOfWorld_5;
                    }
                }
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eBackgroundWallLeft_5:
                if (mVelX < FP_FromInteger(0))
                {
                    BounceHorizontally( hitX, hitY );
                }
                break;
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (mVelX > FP_FromInteger(0))
                {
                    BounceHorizontally( hitX, hitY );
                }
                break;

            default:
                return;
        }
    }
}

//TODO Identical to AE - merge
void Rock::BounceHorizontally( FP hitX, FP hitY )
{
    mVelX = (-mVelX / FP_FromInteger(2));
    mXPos = hitX;
    mYPos = hitY;
    s32 vol = 20 * (4 - mBounceCount);
    if (vol < 40)
    {
        vol = 40;
    }

    SfxPlayMono(relive::SoundEffects::RockBounce, vol);
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
}

//TODO Identical to AE - merge
bool Rock::OnCollision(BaseAnimatedWithPhysicsGameObject* pObj)
{
    if (!pObj->GetCanExplode())
    {
        return true;
    }

    const PSX_RECT bRect = pObj->VGetBoundingRect();

    if (mPreviousXPos < FP_FromInteger(bRect.x) || mPreviousXPos > FP_FromInteger(bRect.w))
    {
        mXPos -= mVelX;
        mVelX = (-mVelX / FP_FromInteger(2));
    }
    else
    {
        mYPos -= mVelY;
        mVelY = (-mVelY / FP_FromInteger(2));
    }

    pObj->VOnThrowableHit(this);

    SfxPlayMono(relive::SoundEffects::RockBounceOnMine, 80);
    return false;
}

bool Rock::VIsFalling()
{
    // Same as meat falling func - compiler seems to have made them both
    // use the same func, or should it go in the base ??
    return mState == States::eFallingOutOfWorld_5;
}

void Rock::VTimeToExplodeRandom()
{
    // Empty ?
}

} // namespace AO
