#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "HoneySack.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "BeeSwarm.hpp"
#include "HoneyDrip.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "Path.hpp"

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

    SetCanExplode(true);
    mTlvInfo = tlvId;

    mChaseTime = pTlv->mChaseTime;
    GetAnimation().SetRenderLayer(Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31);

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    mObjectYPos = FP_FromInteger(pTlv->mTopLeftY);

    mHitGround = 0;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
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
        SetCanExplode(true);

        mState = State::eDripHoney_0;
        mTimer = MakeTimer(90);

        mBeeSwarm = relive_new BeeSwarm(mXPos, mYPos, FP_FromInteger(0), 5, 0);
        if (mBeeSwarm)
        {
            mBeeSwarm->mBaseGameObjectRefCount++;
            mBeeSwarm->SetSpriteScale(GetSpriteScale());
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
    SetCanExplode(false);

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
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
}

void HoneySack::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    SetCanExplode(false);
    mState = State::eSetFallAnimation_1;
}

void HoneySack::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (mBeeSwarm)
    {
        if (mBeeSwarm->GetDead())
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

                mTimer = MakeTimer(90);
            }
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

            if (gCollisions->Raycast(
                    mXPos,
                    oldY,
                    mXPos,
                    mYPos,
                    &pLine,
                    &hitX,
                    &hitY,
                    GetSpriteScale() == FP_FromInteger(1) ? kFgFloor : kBgFloor))
            {
                SfxPlayMono(relive::SoundEffects::MountingElum, 90);
                Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 90, -1000, nullptr);
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
                    mBeeSwarm->SetDead(true);
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
                        pObj->SetDead(true);
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
                        pObj->SetDead(true);
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
                SetDead(true);
            }
            break;

        default:
            return;
    }
}

} // namespace AO
