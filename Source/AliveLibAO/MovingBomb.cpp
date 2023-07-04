#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
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
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Math.hpp"
#include "Grid.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Path.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/FixedPoint.hpp"

namespace AO {

static const TintEntry kMovingBombTints[4] = {
    {EReliveLevelIds::eStockYards, 30u, 30u, 55u},
    {EReliveLevelIds::eStockYardsReturn, 30u, 30u, 55u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

static MovingBomb* sMovingBomb = nullptr;

MovingBomb::MovingBomb(relive::Path_MovingBomb* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetCanExplode(true);
    SetType(ReliveTypes::eTimedMine);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MovingBomb));
    Animation_Init(GetAnimRes(AnimId::MovingBomb));

    GetAnimation().SetSemiTrans(true);
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_0);
    mState = States::eTriggeredBySwitch_1;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_Half_16);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_35);
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mSpeed = FP_FromRaw(pTlv->mSpeed << 8);
    mVelX = FP_FromRaw(pTlv->mStartSpeed << 8);
    mStartMovingSwitchId = pTlv->mStartMovingSwitchId;
    mTimer = sGnFrame;
    mYOffset = 0;
    mTlvId = tlvId;
    mMaxStopTime = 0;
    mMinStopTime = 0;
    mPersistOffscreen = pTlv->mPersistOffscreen;
    mChannelMask = 0;

    if (pTlv->mTriggeredByAlarm == relive::reliveChoice::eYes)
    {
        mState = States::eTriggeredByAlarm_0;
        GetAnimation().SetRender(false);
    }

    SetTint(kMovingBombTints, gMap.mCurrentLevel);

    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
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

    CreateShadow();
}

MovingBomb::~MovingBomb()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
    }

    if (mState == States::eBlowingUp_6 || mState == States::eKillMovingBomb_7)
    {
        Path::TLV_Reset(mTlvId, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(mTlvId, -1, 0, 0);
    }

    if (sMovingBomb == this)
    {
        if (mChannelMask)
        {
            SND_Stop_Channels_Mask(mChannelMask);
            mChannelMask = 0;
        }
        sMovingBomb = nullptr;
    }
}

void MovingBomb::VScreenChanged()
{
    if (mPersistOffscreen == relive::reliveChoice::eNo || gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        SetDead(true);
    }
}

bool MovingBomb::VTakeDamage(BaseGameObject* pFrom)
{
    if (GetDead())
    {
        return true;
    }

    if (mHealth <= FP_FromInteger(0))
    {
        return true;
    }

    if (pFrom->Type() != ReliveTypes::eAbilityRing && pFrom->Type() != ReliveTypes::eAirExplosion && pFrom->Type() != ReliveTypes::eShrykull)
    {
        return false;
    }

    mHealth = FP_FromInteger(0);

    relive_new AirExplosion(
        mXPos,
        mYPos,
        GetSpriteScale());

    relive_new Gibs(
        GibType::Metal_5,
        mXPos,
        mYPos,
        FP_FromInteger(0),
        FP_FromInteger(5),
        GetSpriteScale());

    mState = States::eKillMovingBomb_7;
    GetAnimation().SetRender(false);
    mTimer = MakeTimer(4);
    return false;
}

void MovingBomb::VRender(BasePrimitive** ppOt)
{
    if (GetAnimation().GetRender())
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void MovingBomb::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    SetCanExplode(false);
    mState = States::eBlowingUp_6;
    mVelY = FP_FromInteger(0);
    mTimer = MakeTimer(1);
    SfxPlayMono(relive::SoundEffects::GreenTick, 100);
}

s16 MovingBomb::HitObject()
{
    const PSX_RECT ourRect = VGetBoundingRect();
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter != this)
        {
            if (pObjIter->GetCanSetOffExplosives())
            {
                if (pObjIter->mHealth > FP_FromInteger(0))
                {
                    const PSX_RECT objRect = pObjIter->VGetBoundingRect();
                    if (RectsOverlap(ourRect, objRect) && pObjIter->GetSpriteScale() == GetSpriteScale())
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
                if (gCollisions->Raycast(
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
                if (gCollisions->Raycast(
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
        SetDead(true);
    }

    if (mState == States::eTriggeredByAlarm_0 || mState == States::eTriggeredBySwitch_1 || mState == States::eMoving_2 || mState == States::eStopMoving_3 || mState == States::eWaitABit_4 || mState == States::eToMoving_5)
    {
        if (HitObject())
        {
            SetCanExplode(false);
            mState = States::eBlowingUp_6;
            mVelY = FP_FromInteger(0);
            mTimer = MakeTimer(1);
            SfxPlayMono(relive::SoundEffects::GreenTick, 100);
        }
    }

    if (!sMovingBomb || sMovingBomb == this)
    {
        if (GetAnimation().GetCurrentFrame() != 0 && GetAnimation().GetCurrentFrame() != 7)
        {
            sMovingBomb = this;
        }
        else
        {
            if (mChannelMask)
            {
                SND_Stop_Channels_Mask(mChannelMask);
            }

            if (VIsObjNearby(FP_FromInteger(700), sActiveHero))
            {
                if (FP_Abs(sActiveHero->mYPos - mYPos) <= FP_FromInteger(700))
                {
                    if (mState == States::eWaitABit_4)
                    {
                        mChannelMask = SfxPlayMono(relive::SoundEffects::SecurityOrb, 55);
                    }
                    else
                    {
                        mChannelMask = SfxPlayMono(relive::SoundEffects::SecurityOrb, 80);
                    }
                }
            }
            else
            {
                if (mState == States::eWaitABit_4)
                {
                    mChannelMask = 0;
                    sMovingBomb = this;
                }
                else
                {
                    mChannelMask = SfxPlayMono(relive::SoundEffects::SecurityOrb, 12);
                    sMovingBomb = this;
                }
            }
        }
    }

    switch (mState)
    {
        case States::eTriggeredByAlarm_0:
            if (EventGet(kEventAlarm))
            {
                GetAnimation().SetRender(true);
                mState = States::eMoving_2;
            }
            break;

        case States::eTriggeredBySwitch_1:
            if (SwitchStates_Get(mStartMovingSwitchId))
            {
                mState = States::eMoving_2;
            }
            break;

        case States::eMoving_2:
            if (mVelX < mSpeed)
            {
                mVelX += (GetSpriteScale() * FP_FromDouble(0.5));
            }

            FollowLine();

            BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eMovingBombStopper);

            if (BaseAliveGameObjectPathTLV)
            {
                auto pStopper = static_cast<relive::Path_MovingBombStopper*>(BaseAliveGameObjectPathTLV);
                mMinStopTime = pStopper->mMinStopTime;
                mMaxStopTime = pStopper->mMaxStopTime;
                mState = States::eStopMoving_3;
            }
            break;

        case States::eStopMoving_3:
            mVelX -= (GetSpriteScale() * FP_FromDouble(0.5));
            if (mVelX < FP_FromInteger(0))
            {
                mState = States::eWaitABit_4;
                mTimer = MakeTimer(Math_RandomRange(mMinStopTime, mMaxStopTime));
            }

            FollowLine();
            break;

        case States::eWaitABit_4:
            if (mTimer <= static_cast<s32>(sGnFrame))
            {
                mState = States::eToMoving_5;
            }
            break;

        case States::eToMoving_5:
            if (mVelX < mSpeed)
            {
                mVelX += (GetSpriteScale() * FP_FromDouble(0.5));
            }

            FollowLine();

            BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eMovingBombStopper);
            if (!BaseAliveGameObjectPathTLV)
            {
                mState = States::eMoving_2;
            }
            break;

        case States::eBlowingUp_6:
            if (mTimer <= static_cast<s32>(sGnFrame))
            {
                SfxPlayMono(relive::SoundEffects::GreenTick, 100);

                mHealth = FP_FromInteger(0);

                relive_new AirExplosion(
                    mXPos,
                    mYPos,
                    GetSpriteScale());

                relive_new Gibs(
                    GibType::Metal_5,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(5),
                    GetSpriteScale());

                mState = States::eKillMovingBomb_7;
                GetAnimation().SetRender(false);
                mTimer = MakeTimer(4);
            }
            break;

        case States::eKillMovingBomb_7:
            if (mTimer <= static_cast<s32>(sGnFrame))
            {
                SetDead(true);
            }
            break;

        default:
            break;
    }
}

} // namespace AO
