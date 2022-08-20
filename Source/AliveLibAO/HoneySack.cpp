#include "stdafx_ao.h"
#include "Function.hpp"
#include "HoneySack.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "BeeSwarm.hpp"
#include "HoneyDrip.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Collisions.hpp"

namespace AO {

HoneySack::HoneySack(Path_HoneySack* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = ReliveTypes::eHoneySack;

    const AnimRecord hangingRec = AO::AnimRec(AnimId::HoneySack_Hanging);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, hangingRec.mResourceId, 1, 0);
    Animation_Init(AnimId::HoneySack_Hanging, ppRes);

    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
    mTlvInfo = tlvInfo;

    mChaseTime = pTlv->mChaseTime;
    mAnim.mRenderLayer = Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31;

    mXPos = FP_FromInteger(pTlv->mTopLeft.x);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);
    mOldYPos = FP_FromInteger(pTlv->mTopLeft.y);

    mHitGround = 0;

    if (pTlv->mScale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    if (pTlv->field_1_unknown)
    {
        mYPos += FP_FromInteger(pTlv->field_1_unknown);

        mState = State::eUpdateHoneySackOnGround_3;
        mAnim.Set_Animation_Data(AnimId::HoneySack_OnGround, nullptr);
        mBeeSwarm = nullptr;
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

        mState = State::eDripHoney_0;
        mTimer = sGnFrame + 90;

        if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kWaspAOResID, 0, 0))
        {
            ResourceManager::LoadResourceFile_455270("WASP.BAN", nullptr);
        }

        mBeeSwarm = relive_new BeeSwarm(mXPos, mYPos, FP_FromInteger(0), 5, 0);
        if (mBeeSwarm)
        {
            mBeeSwarm->mBaseGameObjectRefCount++;
            mBeeSwarm->mSpriteScale = mSpriteScale;
        }

        mDripTargetX = FP_FromInteger(0);
        mDripTargetY = FP_FromInteger(0);

        Path_TLV* pHoneyDripTarget = gMap.TLV_First_Of_Type_In_Camera(TlvTypes::HoneyDripTarget_42, 0);
        if (pHoneyDripTarget)
        {
            mDripTargetX = FP_FromInteger(pHoneyDripTarget->mTopLeft.x);
            mDripTargetY = FP_FromInteger(pHoneyDripTarget->mTopLeft.y);
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
        Path::TLV_Reset(mTlvInfo, FP_GetExponent(mYPos - mOldYPos), 0, 0);
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
                mAnim.Set_Animation_Data(AnimId::HoneySack_Falling, nullptr);
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
                SfxPlayMono(SoundEffect::MountingElum_38, 90, 0);
                Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 90, -1000, nullptr);
                mYPos = hitY;
                mState = State::eUpdateHoneySackOnGround_3;
                mAnim.Set_Animation_Data(AnimId::HoneySack_FallingToSmashed, nullptr);

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

                    if (pObj->mBaseGameObjectTypeId == ReliveTypes::eHoney)
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

                    if (pObj->mBaseGameObjectTypeId == ReliveTypes::eHoney)
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
