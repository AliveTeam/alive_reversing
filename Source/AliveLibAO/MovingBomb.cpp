#include "stdafx_ao.h"
#include "Function.hpp"
#include "MovingBomb.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/Collisions.hpp"
#include "PlatformBase.hpp"
#include "Midi.hpp"
#include "Gibs.hpp"
#include "AirExplosion.hpp"
#include "Sfx.hpp"
#include "SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Math.hpp"
#include "Grid.hpp"

namespace AO {

static const TintEntry kMovingBombTints[4] = {
    {EReliveLevelIds::eStockYards, 30u, 30u, 55u},
    {EReliveLevelIds::eStockYardsReturn, 30u, 30u, 55u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

static MovingBomb* sMovingBomb = nullptr;

MovingBomb::MovingBomb(relive::Path_MovingBomb* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
    SetType(ReliveTypes::eTimedMine);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MovingBomb));
    Animation_Init(GetAnimRes(AnimId::MovingBomb));

    mAnim.mFlags.Set(AnimFlags::eSemiTrans);
    mAnim.mRenderMode = TPageAbr::eBlend_0;
    field_10C_state = States::eTriggeredBySwitch_1;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
        mAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_Half_16;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
        mAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_35;
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    field_118_speed = FP_FromRaw(pTlv->mSpeed << 8);
    mVelX = FP_FromRaw(pTlv->mStartSpeed << 8);
    field_11C_switch_id = pTlv->mStartMovingSwitchId;
    field_114_timer = sGnFrame;
    mYOffset = 0;
    field_110_tlvInfo = tlvId;
    field_120_max = 0;
    field_11E_min = 0;
    field_12A_persist_offscreen = pTlv->mPersistOffscreen;
    field_124_sound_channels = 0;

    if (pTlv->mTriggeredByAlarm == relive::reliveChoice::eYes)
    {
        field_10C_state = States::eTriggeredByAlarm_0;
        mAnim.mFlags.Clear(AnimFlags::eRender);
    }

    SetTint(kMovingBombTints, gMap.mCurrentLevel);

    mLiftPoint = nullptr;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos + FP_FromInteger(24),
            mYPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            CollisionMask(eTrackLine_8)))
    {
        mYPos = hitY;
        mXPos = hitX;
    }

    mShadow = relive_new Shadow();
}

MovingBomb::~MovingBomb()
{
    if (mLiftPoint)
    {
        mLiftPoint->VRemove(this);
        mLiftPoint->mBaseGameObjectRefCount--;
        mLiftPoint = nullptr;
    }

    if (field_10C_state == States::eBlowingUp_6 || field_10C_state == States::eKillMovingBomb_7)
    {
        Path::TLV_Reset(field_110_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(field_110_tlvInfo, -1, 0, 0);
    }

    if (sMovingBomb == this)
    {
        if (field_124_sound_channels)
        {
            SND_Stop_Channels_Mask(field_124_sound_channels);
            field_124_sound_channels = 0;
        }
        sMovingBomb = nullptr;
    }
}

void MovingBomb::VScreenChanged()
{
    if (field_12A_persist_offscreen == relive::reliveChoice::eNo || gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s16 MovingBomb::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 1;
    }

    if (mHealth <= FP_FromInteger(0))
    {
        return 1;
    }

    if (pFrom->Type() != ReliveTypes::eAbilityRing && pFrom->Type() != ReliveTypes::eAirExplosion && pFrom->Type() != ReliveTypes::eShrykull)
    {
        return 0;
    }

    mHealth = FP_FromInteger(0);

    relive_new AirExplosion(
        mXPos,
        mYPos,
        mSpriteScale);

    relive_new Gibs(
        GibType::Metal_5,
        mXPos,
        mYPos,
        FP_FromInteger(0),
        FP_FromInteger(5),
        mSpriteScale);

    field_10C_state = States::eKillMovingBomb_7;
    mAnim.mFlags.Clear(AnimFlags::eRender);
    field_114_timer = sGnFrame + 4;
    return 0;
}

void MovingBomb::VRender(PrimHeader** ppOt)
{
    if (mAnim.mFlags.Get(AnimFlags::eRender))
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void MovingBomb::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    mBaseGameObjectFlags.Clear(Options::eCanExplode_Bit7);
    field_10C_state = States::eBlowingUp_6;
    mVelY = FP_FromInteger(0);
    field_114_timer = sGnFrame + 1;
    SfxPlayMono(relive::SoundEffects::GreenTick, 100);
}

s16 MovingBomb::HitObject()
{
    const PSX_RECT ourRect = VGetBoundingRect();
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter != this)
        {
            if (pObjIter->mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit4_SetOffExplosives))
            {
                if (pObjIter->mHealth > FP_FromInteger(0))
                {
                    const PSX_RECT objRect = pObjIter->VGetBoundingRect();
                    if (RectsOverlap(ourRect, objRect) && pObjIter->mSpriteScale == mSpriteScale)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void MovingBomb::FollowLine()
{
    if (BaseAliveGameObjectCollisionLine)
    {
        const FP oldX = mXPos;
        const FP oldY = mYPos;

        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
        if (BaseAliveGameObjectCollisionLine)
        {
            u16 a4 = 0;
            const FP screen_x = CamX_VoidSkipper(oldX, mXPos - oldX, 12, &a4);
            if (a4)
            {
                FP hitX = {};
                FP hitY = {};
                mXPos = screen_x;
                if (sCollisions->Raycast(
                        mXPos,
                        mYPos - FP_FromInteger(20),
                        mXPos,
                        mYPos + FP_FromInteger(20),
                        &BaseAliveGameObjectCollisionLine,
                        &hitX,
                        &hitY,
                        CollisionMask(eTrackLine_8)))
                {
                    mYPos = hitY;
                }
            }

            // OG bug? Why y = oldx, surely y-oldy ?
            const FP screen_y = CamY_VoidSkipper(oldY, mYPos - oldX, 12, &a4);
            if (a4)
            {
                FP hitX = {};
                FP hitY = {};
                mYPos = screen_y;
                if (sCollisions->Raycast(
                        mXPos - FP_FromInteger(20),
                        mYPos,
                        mXPos + FP_FromInteger(20),
                        mYPos,
                        &BaseAliveGameObjectCollisionLine,
                        &hitX,
                        &hitY,
                        CollisionMask(eTrackLine_8)))
                {
                    mXPos = hitX;
                }
            }
        }
    }
}

void MovingBomb::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (field_10C_state == States::eTriggeredByAlarm_0 || field_10C_state == States::eTriggeredBySwitch_1 || field_10C_state == States::eMoving_2 || field_10C_state == States::eStopMoving_3 || field_10C_state == States::eWaitABit_4 || field_10C_state == States::eToMoving_5)
    {
        if (HitObject())
        {
            mBaseGameObjectFlags.Clear(Options::eCanExplode_Bit7);
            field_10C_state = States::eBlowingUp_6;
            mVelY = FP_FromInteger(0);
            field_114_timer = sGnFrame + 1;
            SfxPlayMono(relive::SoundEffects::GreenTick, 100);
        }
    }

    if (!sMovingBomb || sMovingBomb == this)
    {
        if (mAnim.mCurrentFrame != 0 && mAnim.mCurrentFrame != 7)
        {
            sMovingBomb = this;
        }
        else
        {
            if (field_124_sound_channels)
            {
                SND_Stop_Channels_Mask(field_124_sound_channels);
            }

            if (VIsObjNearby(FP_FromInteger(700), sActiveHero))
            {
                if (FP_Abs(sActiveHero->mYPos - mYPos) <= FP_FromInteger(700))
                {
                    if (field_10C_state == States::eWaitABit_4)
                    {
                        field_124_sound_channels = SfxPlayMono(relive::SoundEffects::SecurityOrb, 55);
                    }
                    else
                    {
                        field_124_sound_channels = SfxPlayMono(relive::SoundEffects::SecurityOrb, 80);
                    }
                }
            }
            else
            {
                if (field_10C_state == States::eWaitABit_4)
                {
                    field_124_sound_channels = 0;
                    sMovingBomb = this;
                }
                else
                {
                    field_124_sound_channels = SfxPlayMono(relive::SoundEffects::SecurityOrb, 12);
                    sMovingBomb = this;
                }
            }
        }
    }

    switch (field_10C_state)
    {
        case States::eTriggeredByAlarm_0:
            if (EventGet(kEventAlarm))
            {
                mAnim.mFlags.Set(AnimFlags::eRender);
                field_10C_state = States::eMoving_2;
            }
            break;

        case States::eTriggeredBySwitch_1:
            if (SwitchStates_Get(field_11C_switch_id))
            {
                field_10C_state = States::eMoving_2;
            }
            break;

        case States::eMoving_2:
            if (mVelX < field_118_speed)
            {
                mVelX += (mSpriteScale * FP_FromDouble(0.5));
            }

            FollowLine();

            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eMovingBombStopper);

            if (BaseAliveGameObjectPathTLV)
            {
                auto pStopper = static_cast<relive::Path_MovingBombStopper*>(BaseAliveGameObjectPathTLV);
                field_11E_min = pStopper->mMinDelay;
                field_120_max = pStopper->mMaxDelay;
                field_10C_state = States::eStopMoving_3;
            }
            break;

        case States::eStopMoving_3:
            mVelX -= (mSpriteScale * FP_FromDouble(0.5));
            if (mVelX < FP_FromInteger(0))
            {
                field_10C_state = States::eWaitABit_4;
                field_114_timer = sGnFrame + Math_RandomRange(field_11E_min, field_120_max);
            }

            FollowLine();
            break;

        case States::eWaitABit_4:
            if (field_114_timer <= static_cast<s32>(sGnFrame))
            {
                field_10C_state = States::eToMoving_5;
            }
            break;

        case States::eToMoving_5:
            if (mVelX < field_118_speed)
            {
                mVelX += (mSpriteScale * FP_FromDouble(0.5));
            }

            FollowLine();

            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eMovingBombStopper);
            if (!BaseAliveGameObjectPathTLV)
            {
                field_10C_state = States::eMoving_2;
            }
            break;

        case States::eBlowingUp_6:
            if (field_114_timer <= static_cast<s32>(sGnFrame))
            {
                SfxPlayMono(relive::SoundEffects::GreenTick, 100);

                mHealth = FP_FromInteger(0);

                relive_new AirExplosion(
                    mXPos,
                    mYPos,
                    mSpriteScale);

                relive_new Gibs(
                    GibType::Metal_5,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(5),
                    mSpriteScale);

                field_10C_state = States::eKillMovingBomb_7;
                mAnim.mFlags.Clear(AnimFlags::eRender);
                field_114_timer = sGnFrame + 4;
            }
            break;

        case States::eKillMovingBomb_7:
            if (field_114_timer <= static_cast<s32>(sGnFrame))
            {
                mBaseGameObjectFlags.Set(Options::eDead);
            }
            break;

        default:
            break;
    }
}

} // namespace AO
