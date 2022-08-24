#include "stdafx_ao.h"
#include "Function.hpp"
#include "MovingBomb.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/Collisions.hpp"
#include "PlatformBase.hpp"
#include "Midi.hpp"
#include "Gibs.hpp"
#include "Explosion.hpp"
#include "Sfx.hpp"
#include "SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Math.hpp"
#include "Grid.hpp"

namespace AO {

const TintEntry kMovingBombTints_4CD310[4] = {
    {EReliveLevelIds::eStockYards, 30u, 30u, 55u},
    {EReliveLevelIds::eStockYardsReturn, 30u, 30u, 55u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

ALIVE_VAR(1, 0x507B8C, MovingBomb*, gMovingBomb_507B8C, nullptr);

MovingBomb::MovingBomb(relive::Path_MovingBomb* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
    mBaseGameObjectTypeId = ReliveTypes::eTimedMine;
    const AnimRecord rec = AO::AnimRec(AnimId::MovingBomb);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::MovingBomb, ppRes);

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
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
    field_110_tlvInfo = tlvInfo;
    field_120_max = 0;
    field_11E_min = 0;
    field_12A_persist_offscreen = pTlv->mPersistOffscreen;
    field_124_sound_channels = 0;

    if (pTlv->mTriggeredByAlarm == relive::reliveChoice::eYes)
    {
        field_10C_state = States::eTriggeredByAlarm_0;
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
    }

    SetTint(kMovingBombTints_4CD310, gMap.mCurrentLevel);

    field_128_disabled_resources = pTlv->mDisabledResources;

    if (!(field_128_disabled_resources & 1))
    {
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 1, 0);
    }

    if (!(field_128_disabled_resources & 2))
    {
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, 1, 0);
    }

    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, 1, 0);
    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID, 1, 0);
    ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, 1, 0);
    ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, 1, 0);

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
    if (!(field_128_disabled_resources & 1))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 0, 0));
    }

    if (!(field_128_disabled_resources & 2))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, 0, 0));
    }

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, 0, 0));

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

    if (gMovingBomb_507B8C == this)
    {
        if (field_124_sound_channels)
        {
            SND_Stop_Channels_Mask(field_124_sound_channels);
            field_124_sound_channels = 0;
        }
        gMovingBomb_507B8C = nullptr;
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

    if (pFrom->mBaseGameObjectTypeId != ReliveTypes::eAbilityRing && pFrom->mBaseGameObjectTypeId != ReliveTypes::eExplosion && pFrom->mBaseGameObjectTypeId != ReliveTypes::eShrykull)
    {
        return 0;
    }

    mHealth = FP_FromInteger(0);

    relive_new Explosion(
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
    mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
    field_114_timer = sGnFrame + 4;
    return 0;
}

void MovingBomb::VRender(PrimHeader** ppOt)
{
    if (mAnim.mFlags.Get(AnimFlags::eBit3_Render))
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
    SfxPlayMono(SoundEffect::GreenTick_3, 100, 0);
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
            SfxPlayMono(SoundEffect::GreenTick_3, 100);
        }
    }

    if (!gMovingBomb_507B8C || gMovingBomb_507B8C == this)
    {
        if (mAnim.mCurrentFrame != 0 && mAnim.mCurrentFrame != 7)
        {
            gMovingBomb_507B8C = this;
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
                        field_124_sound_channels = SfxPlayMono(SoundEffect::SecurityOrb_56, 55);
                    }
                    else
                    {
                        field_124_sound_channels = SfxPlayMono(SoundEffect::SecurityOrb_56, 80);
                    }
                }
            }
            else
            {
                if (field_10C_state == States::eWaitABit_4)
                {
                    field_124_sound_channels = 0;
                    gMovingBomb_507B8C = this;
                }
                else
                {
                    field_124_sound_channels = SfxPlayMono(SoundEffect::SecurityOrb_56, 12, 0);
                    gMovingBomb_507B8C = this;
                }
            }
        }
    }

    switch (field_10C_state)
    {
        case States::eTriggeredByAlarm_0:
            if (EventGet(kEventAlarm))
            {
                mAnim.mFlags.Set(AnimFlags::eBit3_Render);
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

            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446260(
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

            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446260(
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
                SfxPlayMono(SoundEffect::GreenTick_3, 100, 0);

                mHealth = FP_FromInteger(0);

                relive_new Explosion(
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
                mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
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
