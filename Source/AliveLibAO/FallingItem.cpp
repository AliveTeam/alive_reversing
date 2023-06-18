#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "FallingItem.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "../relive_lib/Collisions.hpp"
#include "ScreenShake.hpp"
#include "ParticleBurst.hpp"
#include "Midi.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Path.hpp"
#include "../relive_lib/FixedPoint.hpp"

namespace AO {

static FallingItem* sPrimaryFallingItem = nullptr;

static const FallingItem_Data sFallingItemData[16] = {
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting}, // menu
    {AnimId::FallingMeat_Falling, AnimId::FallingMeat_Waiting},       // rupture farms
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting}, // lines
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting}, // forest
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting}, // forest temple
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting}, // stock yards
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting}, // stock yards return
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting}, // removed
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting}, // desert
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting}, // desert temple
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting}, // credits
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting}, // removed
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting}, // board room
    {AnimId::FallingMeat_Falling, AnimId::FallingMeat_Waiting},       // rupture farms return
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting}, // forest chase
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting}};// desert escape

void FallingItem::LoadAnimations()
{
    for (u32 i = 0; i < ALIVE_COUNTOF(sFallingItemData); i++)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sFallingItemData[i].mFallingAnimId));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sFallingItemData[i].mWaitingAnimId));
    }
}

FallingItem::FallingItem(relive::Path_FallingItem* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetCanExplode(true);

    SetType(ReliveTypes::eRockSpawner);

    LoadAnimations();

    mTlvId = tlvId;

    const s32 lvlIdx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
    Animation_Init(GetAnimRes(sFallingItemData[lvlIdx].mFallingAnimId));

    GetAnimation().SetRenderLayer(Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31);
    if (gMap.mCurrentLevel == EReliveLevelIds::eLines)
    {
        mRGB.SetRGB(77, 120, 190);
    }

    mSwitchId = pTlv->mSwitchId;
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


    mFallInterval = pTlv->mFallInterval;
    mMaxFallingItems = pTlv->mMaxFallingItems;
    mRemainingFallingItems = pTlv->mMaxFallingItems;

    mResetSwitchIdAfterUse = pTlv->mResetSwitchIdAfterUse;
    mDoAirStreamSound = true;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mTlvXPos = FP_FromInteger((pTlv->mBottomRightX + pTlv->mTopLeftX) / 2);
    mTlvYPos = FP_FromInteger(pTlv->mBottomRightY);

    mStartYPos = mYPos;
    mState = State::eWaitForIdEnable_0;
    mAirStreamSndChannels = 0;

    // Not sure why this rupture farms primary item hack is required
    if (!sPrimaryFallingItem && (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn))
    {
        sPrimaryFallingItem = this;
        mCreatedGnFrame = sGnFrame;
    }

    CreateShadow();
}

FallingItem::~FallingItem()
{
    if (sPrimaryFallingItem == this)
    {
        sPrimaryFallingItem = nullptr;
    }
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void FallingItem::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel
        || gMap.mCurrentPath != gMap.mNextPath
        || mState != State::eFalling_3)
    {
        SetDead(true);
    }
}

void FallingItem::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    // The primary item controls the main sound effects, otherwise there would be a crazy amount of smashing sounds
    if (sPrimaryFallingItem == this)
    {
        if (!((sGnFrame - mCreatedGnFrame) % 87))
        {
            SfxPlayMono(relive::SoundEffects::MeatsawOffscreen, 45);
        }

        if (!((sGnFrame - mCreatedGnFrame) % 25))
        {
            SfxPlayMono(relive::SoundEffects::MeatsawIdle, 45);
        }
    }

    switch (mState)
    {
        case State::eWaitForIdEnable_0:
            if (!SwitchStates_Get(mSwitchId))
            {
                return;
            }
            [[fallthrough]];

        case State::eGoWaitForDelay_1:
        {
            SetCanExplode(false);
            mState = State::eWaitForFallDelay_2;
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);

            GetAnimation().Set_Animation_Data(GetAnimRes(sFallingItemData[static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))].mWaitingAnimId));

            mFallIntervalTimer = MakeTimer(mFallInterval);
            break;
        }

        case State::eWaitForFallDelay_2:
            if (static_cast<s32>(sGnFrame) >= mFallIntervalTimer)
            {
                mState = State::eFalling_3;
                mDoAirStreamSound = true;
                mAirStreamSndChannels = SFX_Play_Pitch(relive::SoundEffects::AirStream, 50, -2600);
            }
            break;

        case State::eFalling_3:
        {
            if (mDoAirStreamSound)
            {
                if (mYPos >= sActiveHero->mYPos - FP_FromInteger(120))
                {
                    mDoAirStreamSound = false;
                    SFX_Play_Pitch(relive::SoundEffects::AirStream, 127, -1300);
                }
            }

            DamageHitItems();

            if (mVelY < FP_FromInteger(20))
            {
                mVelY += GetSpriteScale() * FP_FromDouble(1.8);
            }

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};

            if (gCollisions->Raycast(
                    mXPos,
                    mYPos,
                    mXPos,
                    mVelY + mYPos,
                    &pLine,
                    &hitX,
                    &hitY,
                    GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor)
                == 1)
            {
                mYPos = hitY;
                mState = State::eSmashed_4;

                relive_new ScreenShake(0);

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    relive_new ParticleBurst(
                        mXPos,
                        mYPos,
                        25,
                        GetSpriteScale(),
                        BurstType::eMeat_4);
                }
                else
                {
                    relive_new ParticleBurst(
                        mXPos,
                        mYPos,
                        25,
                        GetSpriteScale(),
                        BurstType::eFallingRocks_0);
                }

                relive_new ParticleBurst(
                    mXPos,
                    mYPos,
                    25,
                    GetSpriteScale(),
                    BurstType::eSticks_1);
            }
            else
            {
                mYPos += mVelY;
            }
            break;
        }

        case State::eSmashed_4:
        {
            if (mAirStreamSndChannels)
            {
                SND_Stop_Channels_Mask(mAirStreamSndChannels);
                mAirStreamSndChannels = 0;
            }

            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                if (gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos,
                        0))
                {
                    SFX_Play_Pitch(relive::SoundEffects::KillEffect, 127, -700);
                    SfxPlayMono(relive::SoundEffects::FallingItemHit, 110);
                }
                else
                {
                    SND_SEQ_Play(SeqId::Unknown_25, 1, 65, 65);
                    SFX_Play_Pitch(relive::SoundEffects::KillEffect, 90, -700);
                    SfxPlayMono(relive::SoundEffects::FallingItemHit, 33);
                    SFX_Play_Pitch(relive::SoundEffects::MeatsawUp, 80, -400);
                }
            }
            else
            {
                SfxPlayMono(relive::SoundEffects::FallingItemLand, 0);
                SFX_Play_Pitch(relive::SoundEffects::FallingItemHit, 110, -1536);
            }

            if (mSwitchId)
            {
                if (mResetSwitchIdAfterUse == relive::reliveChoice::eYes)
                {
                    SwitchStates_Do_Operation(mSwitchId, relive::reliveSwitchOp::eSetFalse);
                }
            }

            mRemainingFallingItems--;

            if ((mMaxFallingItems && mRemainingFallingItems <= 0) || !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mTlvXPos, mTlvYPos, 0))
            {
                SetDead(true);
            }
            else
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(sFallingItemData[static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))].mFallingAnimId));
                SetCanExplode(true);
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);
                mYPos = mStartYPos;
                mState = State::eWaitForIdEnable_0;
            }
            break;
        }

        default:
            return;
    }
}

void FallingItem::DamageHitItems()
{
    for (s32 idx = 0; idx < gBaseGameObjects->Size(); idx++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj != this)
        {
            if (pObj->GetIsBaseAliveGameObject())
            {
                BaseAnimatedWithPhysicsGameObject* pAliveObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

                const PSX_RECT fallingItemRect = VGetBoundingRect();
                const PSX_RECT objRect = pAliveObj->VGetBoundingRect();

                if (fallingItemRect.x <= objRect.w && fallingItemRect.w >= objRect.x && fallingItemRect.y <= objRect.h && fallingItemRect.h >= objRect.y)
                {
                    if (pAliveObj->GetSpriteScale() == GetSpriteScale())
                    {
                        static_cast<BaseAliveGameObject*>(pAliveObj)->VTakeDamage(this);
                    }
                }
            }
        }
    }
}


void FallingItem::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    // Empty
}


} // namespace AO
