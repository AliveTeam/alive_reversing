#include "stdafx.h"
#include "MovingBomb.hpp"
#include "../relive_lib/Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "../relive_lib/Shadow.hpp"
#include "stdlib.hpp"
#include "Sound/Midi.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "AirExplosion.hpp"
#include "Gibs.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Path.hpp"
#include "../relive_lib/Collisions.hpp"
#include "FixedPoint.hpp"

static const TintEntry kMovingBombTints[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 97u, 97u, 97u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};

static MovingBomb* sMovingBomb = nullptr;

MovingBomb::MovingBomb(relive::Path_MovingBomb* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    SetCanExplode(true);

    SetType(ReliveTypes::eTimedMine_or_MovingBomb);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MovingBomb));
    Animation_Init(GetAnimRes(AnimId::MovingBomb));

    GetAnimation().SetSemiTrans(true);
    GetAnimation().SetRenderMode(TPageAbr::eBlend_0);
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

    SetTint(&kMovingBombTints[0], gMap.mCurrentLevel);

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

    if (mState >= States::eBlowingUp_6)
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

void MovingBomb::BlowUp()
{
    SetCanExplode(false);
    mState = States::eBlowingUp_6;
    mVelY = FP_FromInteger(0);
    mTimer = sGnFrame + 1;
    SfxPlayMono(relive::SoundEffects::GreenTick, 100, GetSpriteScale());
}

void MovingBomb::VRender(PrimHeader** ot)
{
    if (GetAnimation().GetRender())
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
    }
}

void MovingBomb::VScreenChanged()
{
    BaseGameObject::VScreenChanged();

    if (mPersistOffscreen == relive::reliveChoice::eNo)
    {
        SetDead(true);
        return;
    }

    const FP xDelta = FP_Abs(sControlledCharacter->mXPos - mXPos);
    if (xDelta > FP_FromInteger(750))
    {
        SetDead(true);
        return;
    }

    const FP yDelta = FP_Abs(sControlledCharacter->mYPos - mYPos);
    if (yDelta > FP_FromInteger(520))
    {
        SetDead(true);
        return;
    }
}

void MovingBomb::FollowLine()
{
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
    }
}

bool MovingBomb::VTakeDamage(BaseGameObject* pFrom)
{
    if (GetDead() || mHealth <= FP_FromInteger(0))
    {
        return true;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eAirExplosion:
        case ReliveTypes::eShrykull:
        {
            mHealth = FP_FromInteger(0);
            relive_new AirExplosion(mXPos, mYPos, GetSpriteScale(), 0);

            relive_new Gibs(GibType::Metal_5, mXPos, mYPos, FP_FromInteger(0), FP_FromInteger(5), GetSpriteScale(), 0);

            mState = States::eKillMovingBomb_7;

            GetAnimation().SetRender(false);
            mTimer = sGnFrame + 4;
        }
            return false;

        case ReliveTypes::eElectrocute:
            mState = States::eKillMovingBomb_7;
            return false;

        default:
            return true;
    }
}

void MovingBomb::VOnThrowableHit(BaseGameObject* /*pObj*/)
{
    if (!GetElectrocuted())
    {
        BlowUp();
    }
}

s16 MovingBomb::HitObject()
{
    const PSX_RECT bRect = VGetBoundingRect();
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
                    if (bRect.x <= objRect.w && bRect.w >= objRect.x && bRect.h >= objRect.y && bRect.y <= objRect.h && pObjIter->GetSpriteScale() == GetSpriteScale() && pObjIter->mCurrentPath == mCurrentPath)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void MovingBomb::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (mState < States::eBlowingUp_6)
    {
        if (HitObject())
        {
            if (!GetElectrocuted())
            {
                BlowUp();
            }
        }
    }

    if (sMovingBomb == nullptr || sMovingBomb == this)
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
                const FP yDelta = FP_Abs(sActiveHero->mYPos - mYPos);
                if (yDelta <= FP_FromInteger(700))
                {
                    if (mState == States::eWaitABit_4)
                    {
                        mChannelMask = SfxPlayMono(relive::SoundEffects::SecurityOrb, 55, GetSpriteScale());
                    }
                    else
                    {
                        mChannelMask = SfxPlayMono(relive::SoundEffects::SecurityOrb, 80, GetSpriteScale());
                    }
                    sMovingBomb = this;
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
                        mChannelMask = SfxPlayMono(relive::SoundEffects::SecurityOrb, 12, GetSpriteScale());
                        sMovingBomb = this;
                    }
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

            BaseAliveGameObjectPathTLV = gPathInfo->TLV_Get_At(
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
                mTimer = sGnFrame + Math_RandomRange(mMinStopTime, mMaxStopTime);
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

            BaseAliveGameObjectPathTLV = gPathInfo->TLV_Get_At(
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
                SfxPlayMono(relive::SoundEffects::GreenTick, 100, GetSpriteScale());

                mHealth = FP_FromInteger(0);

                relive_new AirExplosion(
                    mXPos,
                    mYPos,
                    GetSpriteScale(),
                    0);

                relive_new Gibs(
                    GibType::Metal_5,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(5),
                    GetSpriteScale(),
                    0);

                mState = States::eKillMovingBomb_7;
                GetAnimation().SetRender(false);
                mTimer = sGnFrame + 4;
            }
            break;

        case States::eKillMovingBomb_7:
            if (mTimer <= static_cast<s32>(sGnFrame))
            {
                SetDead(true);
            }
            break;

        default:
            return;
    }
}
