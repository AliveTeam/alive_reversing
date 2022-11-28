#include "stdafx.h"
#include "FallingItem.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Function.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Sound/Midi.hpp"
#include "ParticleBurst.hpp"
#include "../relive_lib/Particle.hpp"
#include "ScreenShake.hpp"
#include "Abe.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/Collisions.hpp"
#include "FixedPoint.hpp"

const AnimId sFallingItemData_544DC0[15][2] = {
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::FallingCrate_Falling, AnimId::FallingCrate_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::FallingCrate_Falling, AnimId::FallingCrate_Waiting}};

static FallingItem* sPrimaryFallingItem = nullptr;

void FallingItem::LoadAnimations()
{
    const AnimId FallingItemAnimIds[] =
    {
        AnimId::AE_FallingRock_Falling,
        AnimId::AE_FallingRock_Waiting,
        AnimId::FallingCrate_Falling,
        AnimId::FallingCrate_Waiting,
    };

    for (auto& animId : FallingItemAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::AirExplosion));
}

FallingItem::FallingItem(relive::Path_FallingItem* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    SetType(ReliveTypes::eRockSpawner);

    mTlvId = tlvId;

    const s32 lvlIdx = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));

    LoadAnimations();
    Animation_Init(GetAnimRes(sFallingItemData_544DC0[lvlIdx][0]));

    mSwitchId = pTlv->mSwitchId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
        GetAnimation().SetRenderLayer(Layer::eLayer_DoorFlameRollingBallFallingItemPortalClip_Half_12);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
        GetAnimation().SetRenderLayer(Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31);
    }

    mFallInterval = pTlv->mFallInterval;
    mMaxFallingItems = pTlv->mMaxFallingItems;
    mRemainingFallingItems = pTlv->mMaxFallingItems;
    mHitDrillOrMineCar = false;
    mResetSwitchIdAfterUse = pTlv->mResetSwitchIdAfterUse;
    mDoAirStreamSound = true;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (mYPos > pScreenManager->CamYPos())
    {
        mYPos = pScreenManager->CamYPos();
    }

    mTlvXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mTlvYPos = FP_FromInteger(pTlv->mBottomRightY);
    mStartYPos = mYPos;
    mState = State::eWaitForIdEnable_0;
    mAirStreamSndChannels = 0;

    if (!sPrimaryFallingItem)
    {
        sPrimaryFallingItem = this;
        mCreatedGnFrame = sGnFrame;
    }

    CreateShadow();
}

 FallingItem::FallingItem(s32 xpos, s32 ypos, s32 scale, s32 id, s32 fallInterval, s32 numItems, s32 bResetIdAfterUse)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eRockSpawner);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
    mTlvId = Guid{};

    const s32 lvlIdx = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));
    LoadAnimations();
    Animation_Init(GetAnimRes(sFallingItemData_544DC0[lvlIdx][0]));

    GetAnimation().SetRenderLayer(Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31);

    if (id)
    {
        mSwitchId = static_cast<s16>(id);
    }
    else
    {
        mSwitchId = 1;
    }

    if (scale)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
    }

    mFallInterval = static_cast<s16>(fallInterval);

    mMaxFallingItems = static_cast<s16>(numItems);
    mRemainingFallingItems = static_cast<s16>(numItems);

    const FP xFixed = FP_FromInteger(xpos);
    const FP yFixed = FP_FromInteger(ypos);

    mResetSwitchIdAfterUse = static_cast<relive::reliveChoice>(bResetIdAfterUse);
    mHitDrillOrMineCar = false;
    mDoAirStreamSound = true;
    mXPos = xFixed;
    mYPos = yFixed;
    mTlvXPos = xFixed;
    mTlvYPos = yFixed;
    mStartYPos = yFixed;
    mState = State::eWaitForIdEnable_0;
    mAirStreamSndChannels = 0;

    if (!sPrimaryFallingItem)
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
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void FallingItem::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    // The primary item controls the main sound effects, otherwise there would be a crazy amount of smashing sounds
    if (sPrimaryFallingItem == this)
    {
        if (!((sGnFrame - mCreatedGnFrame) % 87))
        {
            if (GetScale() == Scale::Fg)
            {
                SfxPlayMono(relive::SoundEffects::FallingItemPresence1, 45);
            }
            else
            {
                SfxPlayMono(relive::SoundEffects::FallingItemPresence1, 20);
            }
        }

        if (!((sGnFrame - mCreatedGnFrame) % 25))
        {
            if (GetScale() == Scale::Fg)
            {
                SfxPlayMono(relive::SoundEffects::FallingItemPresence2, 45);
            }
            else
            {
                SfxPlayMono(relive::SoundEffects::FallingItemPresence2, 20);
            }
        }
    }

    switch (mState)
    {
        case State::eWaitForIdEnable_0:
            if (mSwitchId && SwitchStates_Get(mSwitchId))
            {
                mBaseGameObjectFlags.Clear(BaseGameObject::eCanExplode_Bit7);
                mState = State::eWaitForFallDelay_2;
                mVelX = FP_FromInteger(0);
                mVelY = FP_FromInteger(0);

                GetAnimation().Set_Animation_Data(GetAnimRes(sFallingItemData_544DC0[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1]));

                mFallIntervalTimer = sGnFrame + mFallInterval;
            }
            break;

        // TODO: Must only be set outside of the object
        case State::eGoWaitForDelay_1:
        {
            mBaseGameObjectFlags.Clear(BaseGameObject::eCanExplode_Bit7);
            mState = State::eWaitForFallDelay_2;
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);

            GetAnimation().Set_Animation_Data(GetAnimRes(sFallingItemData_544DC0[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1]));

            mFallIntervalTimer = sGnFrame + mFallInterval;
            break;
        }

        case State::eWaitForFallDelay_2:
            if (static_cast<s32>(sGnFrame) >= mFallIntervalTimer)
            {
                mState = State::eFalling_3;
                mDoAirStreamSound = true;
                if (GetScale() == Scale::Fg)
                {
                    mAirStreamSndChannels = SFX_Play_Pitch(relive::SoundEffects::AirStream, 50, -2600);
                }
                else
                {
                    mAirStreamSndChannels = SFX_Play_Pitch(relive::SoundEffects::AirStream, 25, -2600);
                }
            }
            break;

        case State::eFalling_3:
        {
            if (mDoAirStreamSound)
            {
                if (mYPos >= sActiveHero->mYPos - FP_FromInteger(240 / 2))
                {
                    mDoAirStreamSound = false;
                    if (GetScale() == Scale::Fg)
                    {
                        SFX_Play_Pitch(relive::SoundEffects::AirStream, 127, -1300);
                    }
                    else
                    {
                        SFX_Play_Pitch(relive::SoundEffects::AirStream, 64, -1300);
                    }
                }
            }

            DamageHitItems();

            if (mVelY < FP_FromInteger(20))
            {
                mVelY += GetSpriteScale() * FP_FromDouble(1.8);
            }

            PathLine* pathLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions->Raycast(
                    mXPos,
                    mYPos,
                    mXPos,
                    mVelY + mYPos,
                    &pathLine,
                    &hitX,
                    &hitY,
                    GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
                == 1)
            {
                if (!mHitDrillOrMineCar)
                {
                    mYPos = hitY;
                }
            }
            else if (!mHitDrillOrMineCar)
            {
                mYPos += mVelY;
                return;
            }

            mHitDrillOrMineCar = false;
            mState = State::eSmashed_4;

            relive_new ScreenShake(0, GetSpriteScale() == FP_FromDouble(0.5));

            if (gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz)
            {
                relive_new ParticleBurst(mXPos,
                                                   mYPos,
                                                   20,
                                                   GetSpriteScale(),
                                                   BurstType::eSticks_1,
                                                   13);

                auto pParticle = relive_new Particle(mXPos,
                                                  mYPos - (FP_FromInteger(15) * GetSpriteScale()),
                                                  GetAnimRes(AnimId::AirExplosion));
                if (pParticle)
                {
                    pParticle->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
                    pParticle->SetSpriteScale(GetSpriteScale() * FP_FromDouble(0.75));
                }
            }
            else
            {
                relive_new ParticleBurst(mXPos,
                                                        mYPos,
                                                        25,
                                                        GetSpriteScale(),
                                                        BurstType::eFallingRocks_0,
                                                        13);
            }
        }
        break;

        case State::eSmashed_4:
            if (mAirStreamSndChannels)
            {
                SND_Stop_Channels_Mask(mAirStreamSndChannels);
                mAirStreamSndChannels = 0;
            }

            EventBroadcast(kEventLoudNoise, this);
            SfxPlayMono(relive::SoundEffects::FallingItemLand, 0, GetSpriteScale());

            if (GetScale() == Scale::Fg)
            {
                SFX_Play_Pitch(relive::SoundEffects::FallingItemHit, 110, -1536);
            }
            else
            {
                SFX_Play_Pitch(relive::SoundEffects::FallingItemHit, 55, -1536);
            }

            if (mSwitchId)
            {
                if (mResetSwitchIdAfterUse == relive::reliveChoice::eYes)
                {
                    SwitchStates_Do_Operation(mSwitchId, relive::reliveSwitchOp::eSetFalse);
                }
            }

            --mRemainingFallingItems;

            if ((mMaxFallingItems > 0 && mRemainingFallingItems <= 0) || !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mTlvXPos, mTlvYPos, 0))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            else
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(sFallingItemData_544DC0[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][0]));
                mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);
                mYPos = mStartYPos;
                mState = State::eWaitForIdEnable_0;
            }
            break;

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
            if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6) || pObj->Type() == ReliveTypes::eDrill)
            {
                BaseAnimatedWithPhysicsGameObject* pAliveObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

                PSX_RECT fallingItemRect = VGetBoundingRect();
                PSX_RECT objRect = pAliveObj->VGetBoundingRect();

                if (pAliveObj->GetSpriteScale() == GetSpriteScale())
                {
                    if (pAliveObj->Type() == ReliveTypes::eDrill || pAliveObj->Type() == ReliveTypes::eMineCar)
                    {
                        objRect.x += pAliveObj->mXOffset;
                        objRect.y += pAliveObj->mYOffset;

                        objRect.w += pAliveObj->mXOffset;
                        objRect.h += pAliveObj->mYOffset;
                    }

                    if (PSX_Rects_overlap_no_adjustment(&fallingItemRect, &objRect))
                    {
                        if (pAliveObj->Type() == ReliveTypes::eDrill)
                        {
                            // Drill is not a type that implements VTakeDamage
                            mHitDrillOrMineCar = true;
                        }
                        else if (pAliveObj->Type() == ReliveTypes::eMineCar)
                        {
                            // ?? Could still call VTakeDamage here but OG doesn't ??
                            mHitDrillOrMineCar = true;
                        }
                        else
                        {
                            bool doDamage = true;
                            if (pAliveObj->Type() == ReliveTypes::eParamite)
                            {
                                // Some strange edge case for paramites - prevents them getting smashed by
                                // falling items when stood on an edge by their huge heads peeking over a bit.
                                if (pAliveObj->mXPos < FP_FromInteger(fallingItemRect.x) || pAliveObj->mXPos > FP_FromInteger(fallingItemRect.w))
                                {
                                    doDamage = false;
                                }
                            }

                            if (doDamage)
                            {
                                static_cast<BaseAliveGameObject*>(pAliveObj)->VTakeDamage(this);
                            }
                        }
                    }
                }
            }
        }
    }
}
