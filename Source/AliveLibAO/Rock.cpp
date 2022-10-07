#include "stdafx_ao.h"
#include "Function.hpp"
#include "Rock.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Grenade.hpp"
#include "ThrowableArray.hpp"
#include "Math.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "../relive_lib/Particle.hpp"
#include "Grid.hpp"

namespace AO {

Rock::Rock(FP xpos, FP ypos, s16 count)
    : BaseThrowable()
{
    SetType(ReliveTypes::eRock);

    field_10E_bDead = 0;

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Rock));
    Animation_Init(GetAnimRes(AnimId::Rock));

    mBaseGameObjectFlags.Clear(Options::eInteractive_Bit8);
    mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
    mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mXPos = xpos;
    field_11C_xpos = xpos;

    mYPos = ypos;
    field_120_ypos = ypos;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    field_10C_count = count;
    field_110_state = States::eNone_0;

    mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::BlueRock));

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        // TODO: I think this only existed in certain lvls, will need a way to know
        // which pal to use per lvl/path
        mAnim.LoadPal(GetPalRes(PalId::BlueRock));
    }
    else
    {
        mAnim.ReloadPal();
    }

    field_118_vol = 0;

    mShadow = relive_new Shadow();
}

Rock::~Rock()
{
    if (!gInfiniteGrenades_5076EC && !field_10E_bDead)
    {
        if (gpThrowableArray_50E26C)
        {
            gpThrowableArray_50E26C->Remove(field_10C_count >= 1u ? field_10C_count : 1);
        }
    }
}

void Rock::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    switch (field_110_state)
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

                field_114_pLine->MoveOnLine(
                    &mXPos,
                    &mYPos,
                    mVelX);

                if (!field_114_pLine)
                {
                    field_110_state = States::eBouncing_4;
                    mAnim.mFlags.Set(AnimFlags::eBit8_Loop);
                }
            }
            else
            {
                const s16 x_exp = FP_GetExponent(mXPos);
                const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid(mSpriteScale, x_exp & 0x3FF);
                if (abs(xSnapped - x_exp) > 1)
                {
                    field_114_pLine = field_114_pLine->MoveOnLine(
                        &mXPos,
                        &mYPos,
                        mVelX);
                    if (!field_114_pLine)
                    {
                        field_110_state = States::eBouncing_4;
                        mAnim.mFlags.Set(AnimFlags::eBit8_Loop);
                    }
                }
                else
                {
                    mVelX = FP_FromInteger(0);
                    mCollectionRect.x = mXPos - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                    mCollectionRect.w = mXPos + (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));

                    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);

                    mAnim.mFlags.Clear(AnimFlags::eBit8_Loop);
                    mCollectionRect.y = mYPos - ScaleToGridSize(mSpriteScale);
                    mCollectionRect.h = mYPos;
                    field_110_state = States::eOnGround_3;
                    field_124_shimmer_timer = sGnFrame;
                }
            }
            break;

        case States::eOnGround_3:
            if (static_cast<s32>(sGnFrame) > field_124_shimmer_timer)
            {
                New_TintShiny_Particle(
                    (mSpriteScale * FP_FromInteger(1)) + mXPos,
                    (mSpriteScale * FP_FromInteger(-7)) + mYPos,
                    FP_FromDouble(0.3),
                    Layer::eLayer_Foreground_36);
                field_124_shimmer_timer = (Math_NextRandom() % 16) + sGnFrame + 60;
            }
            break;

        case States::eBouncing_4:
        {
            InTheAir();
            const PSX_RECT bRect = VGetBoundingRect();
            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            VOnCollisionWith(
                xy,
                wh,
                gBaseGameObjects,
                (TCollisionCallBack) &Rock::OnCollision);

            if (mVelY > FP_FromInteger(30))
            {
                field_110_state = States::eFallingOutOfWorld_5;
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
                mBaseGameObjectFlags.Set(Options::eDead);
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
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

//TODO Identical to AE - merge
void Rock::VThrow(FP velX, FP velY)
{
    mVelX = velX;
    mVelY = velY;

    mAnim.mFlags.Set(AnimFlags::eBit3_Render);

    if (field_10C_count == 0)
    {
        field_110_state = States::eBouncing_4;
    }
    else
    {
        field_110_state = States::eFallingOutOfRockSack_1;
    }
}

s16 Rock::VCanThrow()
{
    return field_110_state == States::eBouncing_4;
}

void Rock::InTheAir()
{
    field_11C_xpos = mXPos;
    field_120_ypos = mYPos;

    if (mVelY > FP_FromInteger(30))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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
            field_11C_xpos,
            field_120_ypos,
            mXPos,
            mYPos,
            &field_114_pLine,
            &hitX,
            &hitY,
            mSpriteScale == FP_FromInteger(1) ? kFgWallsOrFloor : kBgWallsOrFloor))
    {
        switch (field_114_pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                if (mVelY > FP_FromInteger(0))
                {
                    if (field_110_state != States::eBouncing_4 || mVelY >= FP_FromInteger(5))
                    {
                        if (field_110_state != States::eFallingOutOfRockSack_1 || mVelY >= FP_FromInteger(1))
                        {
                            mYPos = hitY;
                            mVelY = (-mVelY / FP_FromInteger(2));
                            mVelX = (mVelX / FP_FromInteger(2));
                            s32 vol = 20 * (4 - field_118_vol);
                            if (vol < 40)
                            {
                                vol = 40;
                            }
                            SfxPlayMono(relive::SoundEffects::RockBounce, vol);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            field_118_vol++;
                        }
                        else
                        {
                            field_110_state = States::eRolling_2;
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
                        field_110_state = States::eFallingOutOfWorld_5;
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
    s32 vol = 20 * (4 - field_118_vol);
    if (vol < 40)
    {
        vol = 40;
    }

    SfxPlayMono(relive::SoundEffects::RockBounce, vol);
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
}

//TODO Identical to AE - merge
s16 Rock::OnCollision(BaseAnimatedWithPhysicsGameObject* pObj)
{
    if (!pObj->mBaseGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    const PSX_RECT bRect = pObj->VGetBoundingRect();

    if (field_11C_xpos < FP_FromInteger(bRect.x) || field_11C_xpos > FP_FromInteger(bRect.w))
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
    return 0;
}

s16 Rock::VIsFalling()
{
    // Same as meat falling func - compiler seems to have made them both
    // use the same func, or should it go in the base ??
    return field_110_state == States::eFallingOutOfWorld_5;
}

void Rock::VTimeToExplodeRandom()
{
    // Empty ?
}

} // namespace AO
