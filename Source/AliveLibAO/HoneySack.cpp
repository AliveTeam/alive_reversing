#include "stdafx_ao.h"
#include "Function.hpp"
#include "HoneySack.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "BeeSwarm.hpp"
#include "HoneyDrip.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Collisions.hpp"

namespace AO {

void HoneySack::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::HoneySack_Hanging));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::HoneySack_OnGround));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::HoneySack_Falling));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::HoneySack_FallingToSmashed));
}

HoneySack::HoneySack(relive::Path_HoneySack* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eHoneySack);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::HoneySack_Hanging));

    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
    mTlvInfo = tlvId;

    mChaseTime = pTlv->mChaseTime;
    GetAnimation().SetRenderLayer(Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31);

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    mObjectYPos = FP_FromInteger(pTlv->mTopLeftY);

    mHitGround = 0;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    if (pTlv->mTlvSpecificMeaning)
    {
        mYPos += FP_FromInteger(pTlv->mTlvSpecificMeaning);

        mState = State::eUpdateHoneySackOnGround_3;
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::HoneySack_OnGround));
        mBeeSwarm = nullptr;
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

        mState = State::eDripHoney_0;
        mTimer = sGnFrame + 90;

        mBeeSwarm = relive_new BeeSwarm(mXPos, mYPos, FP_FromInteger(0), 5, 0);
        if (mBeeSwarm)
        {
            mBeeSwarm->mBaseGameObjectRefCount++;
            mBeeSwarm->mSpriteScale = mSpriteScale;
        }

        mDripTargetX = FP_FromInteger(0);
        mDripTargetY = FP_FromInteger(0);

        relive::Path_TLV* pHoneyDripTarget = gMap.TLV_First_Of_Type_In_Camera(ReliveTypes::eHoneyDripTarget, 0);
        if (pHoneyDripTarget)
        {
            mDripTargetX = FP_FromInteger(pHoneyDripTarget->mTopLeftX);
            mDripTargetY = FP_FromInteger(pHoneyDripTarget->mTopLeftY);
        }
    }
}

HoneySack::~HoneySack()
{
    mBaseGameObjectFlags.Clear(Options::eCanExplode_Bit7);

    if (mState == State::eDripHoney_0)
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvInfo, FP_GetExponent(mYPos - mObjectYPos), 0, 0);
    }

    if (mBeeSwarm)
    {
        mBeeSwarm->mBaseGameObjectRefCount--;
        mBeeSwarm = nullptr;
    }
}

void HoneySack::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void HoneySack::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    mBaseGameObjectFlags.Clear(Options::eCanExplode_Bit7);
    mState = State::eSetFallAnimation_1;
}

void HoneySack::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (mBeeSwarm)
    {
        if (mBeeSwarm->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            mBeeSwarm->mBaseGameObjectRefCount--;
            mBeeSwarm = nullptr;
        }
    }

    switch (mState)
    {
        case State::eDripHoney_0:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                relive_new HoneyDrip(mDripTargetX, mDripTargetY);

                mTimer = sGnFrame + 90;
            }
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

        case State::eSetFallAnimation_1:
            if (static_cast<s32>(sGnFrame) > mTimer - 68)
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::HoneySack_Falling));
                mState = State::eFallOnGround_2;
                mVelX = FP_FromInteger(0);
                mVelY = FP_FromInteger(0);
            }
            break;

        case State::eFallOnGround_2:
        {
            if (mVelY < FP_FromInteger(18))
            {
                mVelY += FP_FromInteger(1);
            }

            const FP oldY = mYPos;
            mYPos += mVelY;

            if (mBeeSwarm)
            {
                mBeeSwarm->mChaseTargetX = mXPos;
                mBeeSwarm->mChaseTargetY = mYPos;
            }

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};

            if (sCollisions->Raycast(
                    mXPos,
                    oldY,
                    mXPos,
                    mYPos,
                    &pLine,
                    &hitX,
                    &hitY,
                    mSpriteScale == FP_FromInteger(1) ? kFgFloor : kBgFloor))
            {
                SfxPlayMono(relive::SoundEffects::MountingElum, 90);
                Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 90, -1000, nullptr);
                mYPos = hitY;
                mState = State::eUpdateHoneySackOnGround_3;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::HoneySack_FallingToSmashed));

                auto pNewBee = relive_new BeeSwarm(
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    24,
                    mChaseTime);
                if (pNewBee)
                {
                    pNewBee->Chase(sActiveHero);
                }

                if (mBeeSwarm)
                {
                    mBeeSwarm->mBaseGameObjectRefCount--;
                    mBeeSwarm->mBaseGameObjectFlags.Set(Options::eDead);
                    mBeeSwarm = nullptr;
                }

                for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
                {
                    BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->Type() == ReliveTypes::eHoney)
                    {
                        pObj->mBaseGameObjectFlags.Set(Options::eDead);
                        mHitGround = 1;
                        return;
                    }
                }
            }
        }
        break;

        case State::eUpdateHoneySackOnGround_3:
            if (!mHitGround)
            {
                for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
                {
                    BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->Type() == ReliveTypes::eHoney)
                    {
                        pObj->mBaseGameObjectFlags.Set(Options::eDead);
                        mHitGround = 1;
                        break;
                    }
                }
            }

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

} // namespace AO
