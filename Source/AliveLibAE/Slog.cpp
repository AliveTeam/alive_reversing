#include "stdafx.h"
#include "Slog.hpp"
#include "stdlib.hpp"
#include "MusicController.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Map.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Bone.hpp"
#include "Sfx.hpp"
#include "../relive_lib/GameObjects/Blood.hpp"
#include "../relive_lib/GameObjects/SnoozeParticle.hpp"
#include "../relive_lib/GameObjects/Gibs.hpp"
#include "Bullet.hpp"
#include "GameSpeak.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Mudokon.hpp"
#include "../relive_lib/Grid.hpp"
#include "../relive_lib/Function.hpp"
#include "AnimationCallBacks.hpp"
#include "Path.hpp"
#include "Game.hpp"
#include "QuikSave.hpp"

s16 sSlogCount = 0;

const TSlogMotionFn sSlogMotionTable[24] = {
    &Slog::Motion_0_Idle,
    &Slog::Motion_1_Walk,
    &Slog::Motion_2_Run,
    &Slog::Motion_3_TurnAround,
    &Slog::Motion_4_Fall,
    &Slog::Motion_5_MoveHeadUpwards,
    &Slog::Motion_6_StopRunning,
    &Slog::Motion_7_SlideTurn,
    &Slog::Motion_8_StartWalking,
    &Slog::Motion_9_EndWalking,
    &Slog::Motion_10_Land,
    &Slog::Motion_11_Unused,
    &Slog::Motion_12_StartFastBarking,
    &Slog::Motion_13_EndFastBarking,
    &Slog::Motion_14_AngryBark,
    &Slog::Motion_15_Sleeping,
    &Slog::Motion_16_MoveHeadDownwards,
    &Slog::Motion_17_WakeUp,
    &Slog::Motion_18_JumpForwards,
    &Slog::Motion_19_JumpUpwards,
    &Slog::Motion_20_Eating,
    &Slog::Motion_21_Dying,
    &Slog::Motion_22_Scratch,
    &Slog::Motion_23_Growl};

static const TSlogBrainFn sSlogBrainTable[4] = {
    &Slog::Brain_0_ListeningToSlig,
    &Slog::Brain_1_Idle,
    &Slog::Brain_2_ChasingAbe,
    &Slog::Brain_3_Death};

static const relive::SfxDefinition sSlogSFXList[19] = {
    {0u, 12u, 38u, 30u, 0, 0},
    {0u, 12u, 39u, 30u, 0, 0},
    {0u, 12u, 40u, 100u, -256, 0},
    {0u, 12u, 41u, 60u, 0, 0},
    {0u, 12u, 42u, 50u, 0, 0},
    {0u, 12u, 40u, 100u, -256, 0},
    {0u, 12u, 44u, 90u, 0, 0},
    {0u, 12u, 60u, 100u, 0, 0},
    {0u, 12u, 45u, 100u, -256, 0},
    {0u, 12u, 46u, 127u, -127, 127},
    {0u, 12u, 47u, 70u, 0, 0},
    {0u, 12u, 48u, 70u, 0, 0},
    {0u, 12u, 49u, 30u, 0, 0},
    {0u, 12u, 40u, 120u, 300, 400},
    {0u, 12u, 61u, 45u, 0, 0},
    {0u, 12u, 62u, 45u, 0, 0},
    {0u, 3u, 59u, 67u, 0, 0},
    {0u, 12u, 33u, 45u, 0, 127},
    {0u, 12u, 33u, 40u, -127, 0},
};

Slog::Slog(FP xpos, FP ypos, FP scale, s16 bListenToSligs, s16 chaseDelay)
    : BaseAliveGameObject(5)
{
    LoadAnimations();

    mYPos = ypos;
    mXPos = xpos;

    SetSpriteScale(scale);

    Init();

    mCommandedToAttack = false;
    mTlvId = Guid{};
    mBrainState = eSlogBrains::Brain_2_ChasingAbe;
    mBrainSubState = 0;

    IBaseAliveGameObject* pTarget = FindTarget(0, 0);
    if (!pTarget)
    {
        pTarget = sControlledCharacter;
    }
    mTargetId = pTarget->mBaseGameObjectId;

    mAsleep = false;
    mMovedOffScreen = false;

    mListenToSligs = bListenToSligs;

    mWakeUpAnger = 0;
    mChaseDelay = chaseDelay;
    mAngerSwitchId = 0;
    mCurrentMotion = eSlogMotions::Motion_0_Idle;
    mTotalAnger = 10;
    mChaseAnger = 20;
    mBoneEatingTime = 60;
}

void Slog::LoadAnimations()
{
    for (auto& animId : sSlogAnimIdTable)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

Slog::Slog(relive::Path_Slog* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(5)
{
    LoadAnimations();

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromDouble(0.5));
    }
    else
    {
        SetSpriteScale(FP_FromDouble(1));
    }

    mBaseGameObjectTlvInfo = tlvId;

    Init();

    mMovedOffScreen = false;
    mListenToSligs = true;
    mAsleep = pTlv->mAsleep;
    mCommandedToAttack = false;

    SetCanExplode(true);

    GetAnimation().SetFlipX(pTlv->mFacing == relive::reliveXDirection::eLeft);

    mTlvId = tlvId;
    mBaseGameObjectTlvInfo = tlvId;
    mBrainState = eSlogBrains::Brain_1_Idle;
    mTargetId = Guid{};
    mWakeUpAnger = pTlv->mWakeUpAnger;
    mTotalAnger = pTlv->mWakeUpAnger + pTlv->mBarkAnger;
    mChaseAnger = mTotalAnger + pTlv->mChaseAnger;
    mChaseDelay = pTlv->mChaseDelay;
    mAngerSwitchId = pTlv->mAngerSwitchId;
    mBoneEatingTime = pTlv->mBoneEatingTime;

    if (mAsleep)
    {
        mCurrentMotion = eSlogMotions::Motion_15_Sleeping;
        SetAnimFrame();
    }
    else
    {
        mCurrentMotion = eSlogMotions::Motion_0_Idle;
    }

    VUpdate();
}

static u8 sSlogRandomIdx = 0;

void Slog::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    if (GetElectrocuted())
    {
        return;
    }

    SlogSaveState data = {};

    data.mType = ReliveTypes::eSlog;

    data.mBaseTlvId = mBaseGameObjectTlvInfo;

    data.mXPos = mXPos;
    data.mYPos = mYPos;
    data.mVelX = mVelX;
    data.mVelY = mVelY;

    data.mFallingVelxScaleFactor = mFallingVelxScaleFactor;

    data.mCurrentPath = mCurrentPath;
    data.mCurrentLevel = mCurrentLevel;
    data.mSpriteScale = GetSpriteScale();

    data.mR = mRGB.r;
    data.mG = mRGB.g;
    data.mB = mRGB.b;

    data.mFlipX = GetAnimation().GetFlipX();
    data.mCurrentMotion = mCurrentMotion;
    data.mCurrentFrame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    data.mFrameChangeCounter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());
    data.mDrawable = GetDrawable();
    data.mRender = GetAnimation().GetRender();
    data.mHealth = mHealth;
    data.mCurrentMotion2 = mCurrentMotion;
    data.mNextMotion = mNextMotion;
    data.mLastLineYPos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);

    if (BaseAliveGameObjectCollisionLine)
    {
        data.mCollisionLineType = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        data.mCollisionLineType = eLineTypes::eNone_m1;
    }

    data.mPlatformId = BaseAliveGameObject_PlatformId;
    data.mSlogTlvId = mTlvId;
    data.mTargetId = Guid{};

    if (mTargetId != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(mTargetId);
        if (pObj)
        {
            data.mTargetId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    data.mBrainState = mBrainState;
    data.mBrainSubState = mBrainSubState;
    data.mMultiUseTimer = mMultiUseTimer;
    data.mFallingVelxScaleFactor = mFallingVelxScaleFactor;
    data.mSlogTlvId = mTlvId;
    data.mListeningToSligId = Guid{};

    if (mListeningToSligId != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(mListeningToSligId);
        if (pObj)
        {
            data.mListeningToSligId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    data.mHasWoofed = mHasWoofed;
    data.mWaitingCounter = mWaitingCounter;
    data.mResponseIdx = mResponseIdx;
    data.mResponsePart = mResponsePart;
    data.mAngerLevel = mAngerLevel;
    data.mJumpCounter = mJumpCounter;
    data.mScratchTimer = mScratchTimer;
    data.mGrowlTimer = mGrowlTimer;
    data.mBoneId = Guid{};

    if (mBoneId != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(mBoneId);
        if (pObj)
        {
            data.mBoneId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    data.mChaseDelay = mChaseDelay;
    data.mSlogRandomIdx = sSlogRandomIdx;

    data.mBitingTarget = mBitingTarget;
    data.eBit3_Asleep = eBit8_Asleep;
    data.mMovedOffScreen = mMovedOffScreen;
    data.mStopRunning = mStopRunning;
    data.mShot = mShot;
    data.mHungry = mHungry;
    data.mCommandedToAttack = mCommandedToAttack;
    data.mHitByAbilityRing = mHitByAbilityRing;
    data.mListenToSligs = mListenToSligs;

    pSaveBuffer.Write(data);
}

void Slog::CreateFromSaveState(SerializedObjectData& pBuffer)
{
    const auto pState = pBuffer.ReadTmpPtr<SlogSaveState>();
    auto pTlv = static_cast<relive::Path_Slog*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->mSlogTlvId));

    Slog* pSlog = nullptr;
    if (pState->mSlogTlvId == Guid{})
    {
        pSlog = relive_new Slog(pState->mXPos,
                                  pState->mYPos,
                                  pState->mSpriteScale, pState->mListenToSligs, pState->mChaseDelay);

        if (pSlog)
        {
            pSlog->mBaseGameObjectTlvInfo = pState->mBaseTlvId;
        }
    }
    else
    {
        pSlog = relive_new Slog(pTlv, pState->mSlogTlvId);
    }

    if (pSlog)
    {
        pSlog->BaseAliveGameObjectPathTLV = nullptr;
        pSlog->BaseAliveGameObjectCollisionLine = nullptr;
        pSlog->BaseAliveGameObject_PlatformId = pState->mPlatformId;
        pSlog->mXPos = pState->mXPos;
        pSlog->mYPos = pState->mYPos;
        pSlog->mVelX = pState->mVelX;
        pSlog->mVelY = pState->mVelY;
        pSlog->mFallingVelxScaleFactor = pState->mFallingVelxScaleFactor;
        pSlog->mCurrentPath = pState->mCurrentPath;
        pSlog->mCurrentLevel = pState->mCurrentLevel;
        pSlog->SetSpriteScale(pState->mSpriteScale);
        pSlog->mRGB.SetRGB(pState->mR, pState->mG, pState->mB);

        pSlog->mCurrentMotion = pState->mCurrentMotion;
        pSlog->GetAnimation().Set_Animation_Data(pSlog->GetAnimRes(sSlogAnimIdTable[static_cast<u32>(pSlog->mCurrentMotion)]));
        
        pSlog->GetAnimation().SetCurrentFrame(pState->mCurrentFrame);
        pSlog->GetAnimation().SetFrameChangeCounter(pState->mFrameChangeCounter);

        pSlog->GetAnimation().SetFlipX(pState->mFlipX & 1);
        pSlog->GetAnimation().SetRender(pState->mRender & 1);

        pSlog->SetDrawable(pState->mDrawable & 1);

        if (IsLastFrame(&pSlog->GetAnimation()))
        {
            pSlog->GetAnimation().SetIsLastFrame(true);
        }

        pSlog->mHealth = pState->mHealth;
        pSlog->mCurrentMotion = pState->mCurrentMotion2;
        pSlog->mNextMotion = pState->mNextMotion;
        pSlog->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->mLastLineYPos);
        pSlog->SetRestoredFromQuickSave(true);
        pSlog->BaseAliveGameObjectCollisionLineType = pState->mCollisionLineType;
        pSlog->mTargetId = pState->mTargetId;
        pSlog->mBrainState = pState->mBrainState;
        pSlog->mBrainSubState = pState->mBrainSubState;
        pSlog->mMultiUseTimer = pState->mMultiUseTimer;
        pSlog->mFallingVelxScaleFactor = pState->mFallingVelxScaleFactor;
        pSlog->mTlvId = pState->mSlogTlvId;
        pSlog->mListeningToSligId = pState->mListeningToSligId;
        pSlog->mHasWoofed = pState->mHasWoofed;
        pSlog->mWaitingCounter = pState->mWaitingCounter;
        pSlog->mResponseIdx = pState->mResponseIdx;
        pSlog->mResponsePart = pState->mResponsePart;
        pSlog->mAngerLevel = pState->mAngerLevel;
        pSlog->mJumpCounter = pState->mJumpCounter;
        pSlog->mScratchTimer = pState->mScratchTimer;
        pSlog->mGrowlTimer = pState->mGrowlTimer;
        pSlog->mChaseDelay = pState->mChaseDelay;
        pSlog->mBoneId = pState->mBoneId;
        sSlogRandomIdx = pState->mSlogRandomIdx;


        pSlog->mBitingTarget = pState->mBitingTarget;
        pSlog->eBit8_Asleep = pState->eBit3_Asleep;
        pSlog->mMovedOffScreen = pState->mMovedOffScreen;
        pSlog->mStopRunning = pState->mStopRunning;
        pSlog->mShot = pState->mShot;
        pSlog->mHungry = pState->mHungry;
        pSlog->mCommandedToAttack = pState->mCommandedToAttack;
        pSlog->mHitByAbilityRing = pState->mHitByAbilityRing;
        pSlog->mListenToSligs = pState->mListenToSligs;

        if (pSlog->mShot)
        {
            sSlogCount--;
        }
    }
}

void Slog::Motion_0_Idle()
{
    if (!ToNextMotion())
    {
        if (mNextMotion == eSlogMotions::Motion_18_JumpForwards)
        {
            ToJump();
        }
        else if (mNextMotion == eSlogMotions::m1)
        {
            if (mCurrentMotion == eSlogMotions::Motion_0_Idle)
            {
                if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    SND_SEQ_PlaySeq(SeqId::Empty_13, 1, 0);
                }

                if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
                {
                    if (MusicController::static_GetMusicType(0, 0, 0) == MusicController::MusicTypes::eSlogChaseTension_6)
                    {
                        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogChaseTension_6, this, 0, 0);
                    }
                    else
                    {
                        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogTension_5, this, 0, 0);
                    }
                }
            }
        }
        else
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = eSlogMotions::m1;
        }
    }
}

const FP sSlogWalkVelXTable_5475EC[18] = {
    FP_FromDouble(1.3329315185546875),
    FP_FromDouble(2.4870452880859375),
    FP_FromDouble(2.537445068359375),
    FP_FromDouble(1.974761962890625),
    FP_FromDouble(3.015594482421875),
    FP_FromDouble(3.10400390625),
    FP_FromDouble(2.758575439453125),
    FP_FromDouble(3.1327056884765625),
    FP_FromDouble(3.646148681640625),
    FP_FromDouble(2.3465118408203125),
    FP_FromDouble(2.222015380859375),
    FP_FromDouble(2.1777801513671875),
    FP_FromDouble(2.2137908935546875),
    FP_FromDouble(2.0020904541015625),
    FP_FromDouble(2.413543701171875),
    FP_FromDouble(2.185516357421875),
    FP_FromDouble(4.1008453369140625),
    FP_FromDouble(4.35284423828125)};

void Slog::Motion_1_Walk()
{
    FP velX = {};
    if (GetAnimation().GetFlipX())
    {
        velX = -sSlogWalkVelXTable_5475EC[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        velX = sSlogWalkVelXTable_5475EC[GetAnimation().GetCurrentFrame()];
    }

    mVelX = (GetSpriteScale() * velX);

    if (CollisionCheck(GetSpriteScale() * FP_FromInteger(20), mVelX * FP_FromInteger(4)))
    {
        ToIdle();

        if (GetAnimation().GetFlipX())
        {
            mXPos += mVelX + (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
        }
        else
        {
            mXPos -= (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2)) - mVelX;
        }
    }
    else
    {
        MoveOnLine();

        if (mCurrentMotion == eSlogMotions::Motion_1_Walk)
        {
            if (GetAnimation().GetCurrentFrame() == 2 || GetAnimation().GetCurrentFrame() == 11)
            {
                if (mNextMotion == eSlogMotions::Motion_0_Idle)
                {
                    mCurrentMotion = eSlogMotions::Motion_9_EndWalking;
                    mNextMotion = eSlogMotions::m1;
                }
                if (mNextMotion == eSlogMotions::Motion_3_TurnAround)
                {
                    mCurrentMotion = eSlogMotions::Motion_9_EndWalking;
                }
            }
            else if (GetAnimation().GetCurrentFrame() == 5 || GetAnimation().GetCurrentFrame() == 14)
            {
                Sfx(SlogSound::SlowStep_18);

                if (!eBit8_Asleep)
                {
                    eBit8_Asleep = true;
                    MapFollowMe(false);
                }

                if (mNextMotion == eSlogMotions::Motion_2_Run)
                {
                    mCurrentMotion = eSlogMotions::Motion_2_Run;
                    mNextMotion = eSlogMotions::m1;
                }
            }
            else
            {
                eBit8_Asleep = false;
            }
        }
    }
}

static u8 Slog_NextRandom()
{
    return gRandomBytes[sSlogRandomIdx++];
}


const FP sSlogRunVelXTable_547634[8] = {
    FP_FromDouble(8.625900268554688),
    FP_FromDouble(3.38677978515625),
    FP_FromDouble(3.5192413330078125),
    FP_FromDouble(5.1022491455078125),
    FP_FromDouble(6.7406005859375),
    FP_FromDouble(7.8218231201171875),
    FP_FromDouble(8.39404296875),
    FP_FromDouble(6.532867431640625)};

void Slog::Motion_2_Run()
{
    if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eIntenseChase_7, this, 0, 0);
    }

    if (GetAnimation().GetFlipX())
    {
        mVelX = (GetSpriteScale() * -sSlogRunVelXTable_547634[GetAnimation().GetCurrentFrame()]);
    }
    else
    {
        mVelX = (GetSpriteScale() * sSlogRunVelXTable_547634[GetAnimation().GetCurrentFrame()]);
    }

    if (CollisionCheck(GetSpriteScale() * FP_FromInteger(20), mVelX * FP_FromInteger(4)))
    {
        ToIdle();
    }
    else
    {
        MoveOnLine();

        if (mCurrentMotion == eSlogMotions::Motion_2_Run)
        {
            if (Slog_NextRandom() < 35u)
            {
                Sfx(SlogSound::CautiousWoof_5);
            }

            if (GetAnimation().GetCurrentFrame() == 4 || GetAnimation().GetCurrentFrame() == 7)
            {
                Sfx(SlogSound::FastStep_17);

                if (!eBit8_Asleep)
                {
                    eBit8_Asleep = true;
                    MapFollowMe(false);
                }

                if (mNextMotion == eSlogMotions::Motion_0_Idle)
                {
                    mCurrentMotion = eSlogMotions::Motion_6_StopRunning;
                    mNextMotion = eSlogMotions::m1;
                }
                else if (mNextMotion == eSlogMotions::Motion_18_JumpForwards)
                {
                    ToJump();
                    mNextMotion = eSlogMotions::m1;
                }
                else if (mNextMotion != eSlogMotions::m1)
                {
                    mCurrentMotion = mNextMotion;
                    mNextMotion = eSlogMotions::m1;
                }
            }
            else
            {
                eBit8_Asleep = false;
            }
        }
    }
}

void Slog::Motion_3_TurnAround()
{
    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();
        ToIdle();
    }
}

void Slog::Motion_4_Fall()
{
    mVelY += (GetSpriteScale() * FP_FromDouble(1.8));
    if (mVelY > (GetSpriteScale() * FP_FromInteger(20)))
    {
        mVelY = (GetSpriteScale() * FP_FromInteger(20));
    }

    if (mVelX > FP_FromInteger(0))
    {
        if (mVelX > (FP_FromInteger(8) * GetSpriteScale()))
        {
            mVelX = (FP_FromInteger(8) * GetSpriteScale());
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        if (mVelX < -(FP_FromInteger(8) * GetSpriteScale()))
        {
            mVelX = -(FP_FromInteger(8) * GetSpriteScale());
        }
    }

    if (mVelX > FP_FromInteger(0))
    {
        mVelX -= (GetSpriteScale() * mFallingVelxScaleFactor);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += (GetSpriteScale() * mFallingVelxScaleFactor);
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    const FP xposBeforeChange = mXPos;
    const FP yposBeforeChange = mYPos;

    mXPos += mVelX;
    mYPos += mVelY;

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (gCollisions->Raycast(
            xposBeforeChange,
            yposBeforeChange - (GetSpriteScale() * FP_FromInteger(20)),
            mXPos,
            mYPos,
            &pLine, &hitX, &hitY, CollisionMask(eCeiling_3, eWallRight_2, eWallLeft_1, eFloor_0, eDynamicCollision_32)))
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                BaseAliveGameObjectCollisionLine = pLine;
                mYPos = hitY;
                mXPos = hitX;
                MapFollowMe(false);
                if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                {
                    const PSX_RECT bRect = VGetBoundingRect();

                    const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                    const PSX_Point wh = {bRect.w, static_cast<s16>(FP_GetExponent(mYPos) + 5)};
                    OnCollisionWith(
                        xy,
                        wh,
                        gPlatformsArray);
                }
                mCurrentMotion = eSlogMotions::Motion_10_Land;
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                mYPos = hitY;
                mXPos = hitX - mVelX;
                MapFollowMe(false);
                mVelX = FP_FromInteger(0);
                break;

            default:
                return;
        }
    }
}

void Slog::Motion_5_MoveHeadUpwards()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Sfx(SlogSound::IdleWoof_2);
        mHasWoofed = 1;
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (Math_RandomRange(0, 100) < 30)
        {
            Sfx(SlogSound::IdleGrrAlt_4);
        }
        ToIdle();
    }
}

const FP sSlogStopRunningVelX_547658[10] = {
    FP_FromDouble(3.468994140625),
    FP_FromDouble(4.5489959716796875),
    FP_FromDouble(8.642990112304688),
    FP_FromDouble(9.31298828125),
    FP_FromDouble(6.4949951171875),
    FP_FromDouble(4.631988525390625),
    FP_FromDouble(3.9169921875),
    FP_FromDouble(4.334991455078125),
    FP_FromDouble(3.6609954833984375),
    FP_FromDouble(1.2819976806640625)};

void Slog::Motion_6_StopRunning()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Sfx(SlogSound::Skid_14);
    }

    FP velX = {};
    if (GetAnimation().GetFlipX())
    {
        velX = -sSlogStopRunningVelX_547658[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        velX = sSlogStopRunningVelX_547658[GetAnimation().GetCurrentFrame()];
    }

    mVelX = (GetSpriteScale() * velX);

    if (!CollisionCheck(GetSpriteScale() * FP_FromInteger(20), mVelX * FP_FromInteger(4)))
    {
        MoveOnLine();
        if (!GetAnimation().GetIsLastFrame())
        {
            return;
        }
    }

    ToIdle();
}

const FP sSlogSlideTurnVelXTable_547684[25] = {
    FP_FromDouble(17.197998046875),
    FP_FromDouble(11.907989501953125),
    FP_FromDouble(8.52899169921875),
    FP_FromDouble(7.33599853515625),
    FP_FromDouble(4.168212890625),
    FP_FromDouble(5.3128204345703125),
    FP_FromDouble(3.81121826171875),
    FP_FromDouble(1.503692626953125),
    FP_FromDouble(0.5045166015625),
    FP_FromDouble(-0.2426605224609375),
    FP_FromDouble(-0.5961456298828125),
    FP_FromDouble(-1.2430877685546875),
    FP_FromDouble(-4.89166259765625)};


void Slog::Motion_7_SlideTurn()
{
    FP velX = {};
    if (GetAnimation().GetFlipX())
    {
        velX = -sSlogSlideTurnVelXTable_547684[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        velX = sSlogSlideTurnVelXTable_547684[GetAnimation().GetCurrentFrame()];
    }

    mVelX = (GetSpriteScale() * velX);
    if (CollisionCheck(GetSpriteScale() * FP_FromInteger(20), mVelX * FP_FromInteger(4)))
    {
        ToIdle();
    }
    else
    {
        MoveOnLine();

        if (mCurrentMotion == eSlogMotions::Motion_7_SlideTurn)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                MapFollowMe(false);

                if (GetAnimation().GetFlipX())
                {
                    GetAnimation().SetFlipX(false);
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(3));
                }
                else
                {
                    GetAnimation().SetFlipX(true);
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(3));
                }
                mCurrentMotion = eSlogMotions::Motion_2_Run;
            }
        }
    }
}

void Slog::Motion_8_StartWalking()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eSlogMotions::Motion_1_Walk;
    }

    MoveOnLine();
}

void Slog::Motion_9_EndWalking()
{
    MoveOnLine();

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Slog::Motion_10_Land()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Sfx(SlogSound::Landing_16);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Slog::Motion_11_Unused()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eSlogMotions::Motion_12_StartFastBarking;
    }
}

void Slog::Motion_12_StartFastBarking()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Sfx(SlogSound::IdleWoof_2);
        mHasWoofed = 1;
    }

    if (mNextMotion != eSlogMotions::m1)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eSlogMotions::Motion_13_EndFastBarking;
        }
    }
}

void Slog::Motion_13_EndFastBarking()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Slog::Motion_14_AngryBark()
{
    if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 6)
    {
        Sfx(SlogSound::CautiousWoof_5);
    }

    if (mNextMotion != eSlogMotions::m1)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = eSlogMotions::m1;
        }
    }
}

void Slog::Motion_15_Sleeping()
{
    if (mNextMotion != eSlogMotions::m1 && GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = mNextMotion;
        mNextMotion = eSlogMotions::m1;
        return;
    }

    bool createParticle = false;
    if (static_cast<s32>(sGnFrame) % 60)
    {
        if (!(static_cast<s32>((sGnFrame - 20)) % 60))
        {
            Sfx(SlogSound::YawnEnd_11);
            createParticle = true;
        }
    }
    else
    {
        Sfx(SlogSound::YawnStart_10);
        createParticle = true;
    }

    if (createParticle)
    {
        if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
        {
            FP xOff = {};
            if (GetAnimation().GetFlipX())
            {
                xOff = -(GetSpriteScale() * FP_FromInteger(18));
            }
            else
            {
                xOff = (GetSpriteScale() * FP_FromInteger(18));
            }
            relive_new SnoozeParticle(xOff + mXPos,
                                                          (GetSpriteScale() * FP_FromInteger(-13)) + mYPos,
                                                          GetAnimation().GetRenderLayer(), GetAnimation().GetSpriteScale());
        }
    }
}

void Slog::Motion_16_MoveHeadDownwards()
{
    if (mNextMotion != eSlogMotions::m1)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = eSlogMotions::m1;
        }
    }

    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
}

void Slog::Motion_17_WakeUp()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eSnoozeParticle)
        {
            static_cast<SnoozeParticle*>(pObj)->mState = SnoozeParticle::SnoozeParticleState::eBlowingUp_2;
        }
    }

    if (mNextMotion != eSlogMotions::m1)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = eSlogMotions::m1;
            Sfx(SlogSound::IdleGrrAlt_4);
        }
    }

    if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogTension_5, this, 0, 0);
    }
}

void Slog::Motion_18_JumpForwards()
{
    mVelY += (GetSpriteScale() * FP_FromDouble(1.8));

    const FP k20Scaled = GetSpriteScale() * FP_FromInteger(20);
    if (mVelY > k20Scaled)
    {
        mVelY = k20Scaled;
    }

    const FP oldXPos = mXPos;
    const FP oldYPos = mYPos;

    mXPos += mVelX;
    mYPos += mVelY;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(oldXPos, oldYPos - k20Scaled, mXPos, mYPos, &pLine, &hitX, &hitY, CollisionMask(eCeiling_3, eWallRight_2, eWallLeft_1, eFloor_0, eDynamicCollision_32)
) == 1)
    {
        switch (pLine->mLineType) // TODO: line enum
        {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                if (mVelY > FP_FromInteger(0))
                {
                    BaseAliveGameObjectCollisionLine = pLine;
                    mNextMotion = eSlogMotions::m1;
                    mCurrentMotion = eSlogMotions::Motion_2_Run;
                    mYPos = hitY;
                    mVelY = FP_FromInteger(0);
                }
                break;

            case 1u:
            case 5u:
                if (mVelX < FP_FromInteger(0))
                {
                    mVelX = (-mVelX / FP_FromInteger(2));
                }
                break;

            case 2u:
            case 6u:
                if (mVelX > FP_FromInteger(0))
                {
                    mVelX = (-mVelX / FP_FromInteger(2));
                }
                break;

            default:
                break;
        }
    }

    if (mYPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(2))
    {
        mFallingVelxScaleFactor = FP_FromDouble(0.3);
        BaseAliveGameObjectLastLineYPos = mYPos;
        mCurrentMotion = eSlogMotions::Motion_4_Fall;
    }
}

void Slog::Motion_19_JumpUpwards()
{
    const PSX_RECT bRect = VGetBoundingRect();

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            FP_FromInteger(bRect.x),
            FP_FromInteger(bRect.y),
            FP_FromInteger(bRect.w),
            FP_FromInteger(bRect.h),
            &pLine, &hitX, &hitY, CollisionMask(eCeiling_3)))
    {
        if (GetAnimation().GetCurrentFrame() < 12)
        {
            GetAnimation().SetFrame(24 - GetAnimation().GetCurrentFrame());
        }
    }

    if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogChaseTension_6, this, 0, 0);
    }

    if (GetAnimation().GetCurrentFrame() == 5)
    {
        if (mHungry && mTargetId == gAbe->mBaseGameObjectId && gAbe->GetScale() == GetScale() && (gAbe->mCurrentMotion == eAbeMotions::Motion_104_RockThrowStandingHold || gAbe->mCurrentMotion == eAbeMotions::Motion_107_RockThrowCrouchingHold))
        {
            Sfx(SlogSound::HungryYip_13);
        }
        else
        {
            Sfx(SlogSound::JumpBite_6);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eSlogMotions::Motion_0_Idle;
        mNextMotion = eSlogMotions::m1;
    }
}

void Slog::Motion_20_Eating()
{
    SND_SEQ_Stop(SeqId::Empty_13);
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        GetAnimation().SetLoopBackwards(false);
        if (mNextMotion != eSlogMotions::m1 && mNextMotion != eSlogMotions::Motion_20_Eating)
        {
            mCurrentMotion = eSlogMotions::Motion_0_Idle;
            return;
        }
    }

    if (GetAnimation().GetCurrentFrame() == 3 && !GetAnimation().GetLoopBackwards())
    {
        SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Eating1, relive::SoundEffects::Eating2), 100);
        relive_new Blood(((GetAnimation().GetFlipX()) != 0 ? FP_FromInteger(-25) : FP_FromInteger(25)) * GetSpriteScale() + mXPos,
                      mYPos - (FP_FromInteger(4) * GetSpriteScale()),
                      FP_FromInteger(0), FP_FromInteger(0),
                      GetSpriteScale(), 12);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (Math_RandomRange(0, 100) < 85)
        {
            if (static_cast<s32>(sGnFrame) > mGrowlTimer && Math_RandomRange(0, 100) < 60)
            {
                mGrowlTimer = MakeTimer(16);
                Sfx(SlogSound::IdleGrrr_3);
            }
            GetAnimation().SetLoopBackwards(true);
        }
    }

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        GetAnimation().SetLoopBackwards(false);
    }
}

void Slog::Motion_21_Dying()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_4_Fall();
        mCurrentMotion = eSlogMotions::Motion_21_Dying;
    }
}

void Slog::Motion_22_Scratch()
{
    if (GetAnimation().GetCurrentFrame() == 4)
    {
        SND_SEQ_PlaySeq(SeqId::Scratching_12, 1, 1);
    }

    if (mNextMotion != eSlogMotions::m1)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = eSlogMotions::m1;
        }
    }
}

void Slog::Motion_23_Growl()
{
    if (GetAnimation().GetCurrentFrame() == 3)
    {
        if (mHasWoofed)
        {
            Sfx(SlogSound::IdleGrrr_3);
            GetAnimation().SetAnimate(false);
            mHasWoofed = 0;
            mGrowlTimer = MakeTimer(12);
        }
    }

    if (static_cast<s32>(sGnFrame) > mGrowlTimer)
    {
        GetAnimation().SetAnimate(true);
    }

    if (mNextMotion != eSlogMotions::m1)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = eSlogMotions::m1;
        }
    }
}

static const eSlogMotions sSlogResponseMotion[3][10] = {
    {eSlogMotions::Motion_3_TurnAround,
     eSlogMotions::Motion_2_Run,
     eSlogMotions::Motion_7_SlideTurn,
     eSlogMotions::m1,
     eSlogMotions::Motion_7_SlideTurn,
     eSlogMotions::m1,
     eSlogMotions::Motion_6_StopRunning,
     eSlogMotions::m2,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle},
    {eSlogMotions::Motion_12_StartFastBarking,
     eSlogMotions::Motion_2_Run,
     eSlogMotions::Motion_6_StopRunning,
     eSlogMotions::m2,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle},
    {eSlogMotions::Motion_12_StartFastBarking,
     eSlogMotions::m2,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle}};

enum class Brain_0_ListeningToSligs : s16
{
    eInit = 0,
    eIdle = 1,
    eListening = 2,
    eWalking = 3,
    eRunning = 4,
    eWaiting = 5,
    eResponding = 6
};

s16 Slog::Brain_0_ListeningToSlig()
{
    if (!mListeningToSligId.IsValid())
    {
        ALIVE_FATAL("Slog wants to listen to a slig, but we ain't got one");
    }

    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mListeningToSligId));

    // TODO: OG bug - return never used?
    //sObjectIds.Find_449CF0(field_118);

    if (!pObj || pObj->GetDead())
    {
        mAngerLevel = 0;
        mListeningToSligId = Guid{};
        mTargetId = Guid{};
        mBrainState = eSlogBrains::Brain_1_Idle;
        return 0;
    }

    FP gridSize1Directed = ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(1);
    if (pObj->GetAnimation().GetFlipX())
    {
        gridSize1Directed = -gridSize1Directed;
    }

    const FP xpos1GridAHead = gridSize1Directed + pObj->mXPos;
    switch (mBrainSubState)
    {
        case 0:
            return Brain_ListeningToSlig_0_Init();
        case 1:
            return Brain_ListeningToSlig_1_Idle(xpos1GridAHead);
        case 2:
            return Brain_ListeningToSlig_2_Listening(xpos1GridAHead, pObj);
        case 3:
            return Brain_ListeningToSlig_3_Walking(xpos1GridAHead);
        case 4:
            return Brain_ListeningToSlig_4_Running(xpos1GridAHead);
        case 5:
            return Brain_ListeningToSlig_5_Waiting();
        case 6:
            return Brain_ListeningToSlig_6_Responding();
        default:
            return mBrainSubState;
    }
}

s16 Slog::Brain_ListeningToSlig_6_Responding()
{
    if (static_cast<s32>(sGnFrame) <= mMultiUseTimer)
    {
        return mBrainSubState;
    }

    mMultiUseTimer = MakeTimer(10);
    mNextMotion = sSlogResponseMotion[mResponseIdx][mResponsePart++];

    if (mNextMotion == eSlogMotions::m2)
    {
        mNextMotion = eSlogMotions::Motion_0_Idle;
        return 2;
    }
    else
    {
        return mBrainSubState;
    }
}

s16 Slog::Brain_ListeningToSlig_5_Waiting()
{
    if (static_cast<s32>(sGnFrame) <= mMultiUseTimer)
    {
        return mBrainSubState;
    }
    mWaitingCounter--;
    mNextMotion = eSlogMotions::Motion_5_MoveHeadUpwards;
    return 2;
}

s16 Slog::Brain_ListeningToSlig_4_Running(const FP xpos1GridAHead)
{
    if (mCurrentMotion == eSlogMotions::Motion_0_Idle)
    {
        return 2;
    }

    if (mCurrentMotion != eSlogMotions::Motion_2_Run)
    {
        return mBrainSubState;
    }

    if (Facing(xpos1GridAHead))
    {
        if (FP_Abs(xpos1GridAHead - mXPos) > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3)))
        {
            return mBrainSubState;
        }
        mNextMotion = eSlogMotions::Motion_0_Idle;
        return 2;
    }
    else
    {
        mNextMotion = eSlogMotions::Motion_7_SlideTurn;
        return mBrainSubState;
    }
}

s16 Slog::Brain_ListeningToSlig_3_Walking(const FP xpos1GridAHead)
{
    if (mCurrentMotion == eSlogMotions::Motion_0_Idle)
    {
        return 2;
    }

    if (mCurrentMotion != eSlogMotions::Motion_1_Walk)
    {
        return mBrainSubState;
    }

    if (!Facing(xpos1GridAHead))
    {
        mNextMotion = eSlogMotions::Motion_3_TurnAround;
        return 2;
    }

    if (FP_Abs(xpos1GridAHead - mXPos) > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4)))
    {
        mNextMotion = eSlogMotions::Motion_2_Run;
        return 4;
    }

    if (FP_Abs(xpos1GridAHead - mXPos) > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3)))
    {
        return mBrainSubState;
    }

    mNextMotion = eSlogMotions::Motion_0_Idle;
    return 2;
}

s16 Slog::Brain_ListeningToSlig_2_Listening(const FP xpos1GridAHead, IBaseAliveGameObject* pObj)
{
    if (mCurrentMotion != eSlogMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }

    GameSpeakEvents speakValue = GameSpeakEvents::eNone;
    if (mLastGameSpeakEvent == gEventSystem->mLastEventIndex)
    {
        if (gEventSystem->mLastEvent == GameSpeakEvents::eNone)
        {
            speakValue = GameSpeakEvents::eNone;
        }
        else
        {
            speakValue = GameSpeakEvents::eSameAsLast;
        }
    }
    else
    {
        mLastGameSpeakEvent = gEventSystem->mLastEventIndex;
        speakValue = gEventSystem->mLastEvent;
    }

    switch (speakValue)
    {
        case GameSpeakEvents::eSlig_LookOut:
            DelayedResponse(2);
            return 6;

        case GameSpeakEvents::eSlig_Hi:
            mWaitingCounter++;
            if (static_cast<s32>(sGnFrame) % 2)
            {
                mWaitingCounter++;
            }
            [[fallthrough]];

        case GameSpeakEvents::eSlig_HereBoy:
            mMultiUseTimer = sGnFrame - Math_NextRandom() % 8 + 15;
            mWaitingCounter++;
            break;

        case GameSpeakEvents::eSlig_GetEm:
        {
            auto pTarget = FindTarget(1, 0);
            if (pTarget)
            {
                mListeningToSligId = Guid{};
                mCommandedToAttack = true;
                mTargetId = pTarget->mBaseGameObjectId;
                mBrainState = eSlogBrains::Brain_2_ChasingAbe;
                return 0;
            }

            FP gridSize = {};
            if (GetAnimation().GetFlipX())
            {
                gridSize = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridSize = ScaleToGridSize(GetSpriteScale());
            }

            if (!CollisionCheck(GetSpriteScale() * FP_FromInteger(20), gridSize * FP_FromInteger(2)))
            {
                DelayedResponse(1);
                return 6;
            }
            break;
        }

        default:
            break;
    }

    if (!(Math_NextRandom() % 128))
    {
        mWaitingCounter++;
    }

    if (mWaitingCounter > 0)
    {
        mMultiUseTimer += Math_NextRandom() % 16;
        return 5;
    }

    if (FP_Abs(xpos1GridAHead - mXPos) > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4)))
    {
        if (!Facing(xpos1GridAHead))
        {
            mNextMotion = eSlogMotions::Motion_3_TurnAround;
            return mBrainSubState;
        }

        FP gridSize2 = {};
        if (GetAnimation().GetFlipX())
        {
            gridSize2 = -ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            gridSize2 = ScaleToGridSize(GetSpriteScale());
        }

        if (!CollisionCheck(GetSpriteScale() * FP_FromInteger(20), gridSize2 * FP_FromInteger(2)))
        {
            mNextMotion = eSlogMotions::Motion_2_Run;
            return 4;
        }
    }

    if (FP_Abs(xpos1GridAHead - mXPos) > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(1)))
    {
        if (!Facing(xpos1GridAHead))
        {
            mNextMotion = eSlogMotions::Motion_3_TurnAround;
            return mBrainSubState;
        }

        FP gridSize3 = {};
        if (GetAnimation().GetFlipX())
        {
            gridSize3 = -ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            gridSize3 = ScaleToGridSize(GetSpriteScale());
        }

        if (!CollisionCheck(GetSpriteScale() * FP_FromInteger(20), gridSize3 * FP_FromInteger(2)))
        {
            mNextMotion = eSlogMotions::Motion_1_Walk;
            return 3;
        }
    }

    if (pObj->GetAnimation().GetFlipX() != GetAnimation().GetFlipX())
    {
        mNextMotion = eSlogMotions::Motion_3_TurnAround;
    }

    return mBrainSubState;
}

s16 Slog::Brain_ListeningToSlig_1_Idle(const FP xpos1GridAHead)
{
    if (mCurrentMotion != eSlogMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }

    if (!Facing(xpos1GridAHead))
    {
        mNextMotion = eSlogMotions::Motion_3_TurnAround;
        return mBrainSubState;
    }

    if (static_cast<s32>(sGnFrame) <= mMultiUseTimer)
    {
        return mBrainSubState;
    }
    mNextMotion = eSlogMotions::Motion_5_MoveHeadUpwards;
    return 2;
}

s16 Slog::Brain_ListeningToSlig_0_Init()
{
    mNextMotion = eSlogMotions::Motion_0_Idle;
    mWaitingCounter = 0;
    mMultiUseTimer = MakeTimer(15);
    return 1;
}

s16 Slog::Brain_1_Idle()
{
    BaseGameObject* pTarget = sObjectIds.Find_Impl(mTargetId);

    // OG dead code - return never used
    //sObjectIds.Find_449CF0(field_138_bottom_right);

    if (pTarget && pTarget->GetDead())
    {
        mTargetId = Guid{};
    }

    if (mLastGameSpeakEvent != gEventSystem->mLastEventIndex)
    {
        mLastGameSpeakEvent = gEventSystem->mLastEventIndex;
        if (gEventSystem->mLastEvent == GameSpeakEvents::eSlig_HereBoy && sControlledCharacter->Type() == ReliveTypes::eSlig)
        {
            mBrainState = eSlogBrains::Brain_0_ListeningToSlig;
            mTargetId = Guid{};
            mListeningToSligId = sControlledCharacter->mBaseGameObjectId;
            return 0;
        }
    }

    if (SwitchStates_Get(mAngerSwitchId))
    {
        mCommandedToAttack = false;
        mBrainState = eSlogBrains::Brain_2_ChasingAbe;
        return 0;
    }

    switch (mBrainSubState)
    {
        case 0:
            if (mCurrentMotion != eSlogMotions::Motion_15_Sleeping && mCurrentMotion != eSlogMotions::Motion_0_Idle)
            {
                mNextMotion = eSlogMotions::Motion_0_Idle;
                return mBrainSubState;
            }

            if (mAsleep)
            {
                mAngerLevel = 0;
                return 1;
            }
            else
            {
                mAngerLevel = mWakeUpAnger;
                return 4;
            }
            break;

        case 1:
            if (IsEventInRange(kEventSuspiciousNoise, mXPos, mYPos, EventScale::Full))
            {
                mAngerLevel++;
            }

            if (IsEventInRange(kEventSpeaking, mXPos, mYPos, EventScale::Full))
            {
                mAngerLevel += Slog_NextRandom() % 8 + 15;
            }

            if (!(static_cast<s32>(sGnFrame) % 16))
            {
                // Calm down a bit
                if (mAngerLevel > 0)
                {
                    mAngerLevel--;
                }
            }

            if (mAngerLevel <= mWakeUpAnger)
            {
                return mBrainSubState;
            }

            mNextMotion = eSlogMotions::Motion_17_WakeUp;
            return 2;

        case 2:
            if (mCurrentMotion != eSlogMotions::Motion_17_WakeUp)
            {
                return mBrainSubState;
            }

            mNextMotion = eSlogMotions::Motion_5_MoveHeadUpwards;
            mScratchTimer = Math_NextRandom() % 32 + MakeTimer(120);
            mGrowlTimer = Math_NextRandom() % 32 + MakeTimer(60);
            return 4;

        case 3:
            if (mCurrentMotion != eSlogMotions::Motion_16_MoveHeadDownwards)
            {
                if (mNextMotion != eSlogMotions::Motion_16_MoveHeadDownwards)
                {
                    mNextMotion = eSlogMotions::Motion_16_MoveHeadDownwards;
                }

                return mBrainSubState;
            }

            mNextMotion = eSlogMotions::Motion_15_Sleeping;
            return 1;

        case 4:
            if (IsEventInRange(kEventSuspiciousNoise, mXPos, mYPos, EventScale::Full))
            {
                mAngerLevel++;
            }

            if (IsEventInRange(kEventSpeaking, mXPos, mYPos, EventScale::Full))
            {
                mAngerLevel += Slog_NextRandom() % 8 + 15;
            }

            if (!(static_cast<s32>(sGnFrame) % 32))
            {
                if (mAngerLevel)
                {
                    if (mAsleep)
                    {
                        mAngerLevel--;
                    }
                }
            }

            if (PlayerOrNakedSligNear())
            {
                mAngerLevel += 2;
            }

            if (!(Slog_NextRandom() % 64) && mCurrentMotion == eSlogMotions::Motion_0_Idle)
            {
                mCurrentMotion = eSlogMotions::Motion_5_MoveHeadUpwards;
                return mBrainSubState;
            }

            if (static_cast<s32>(sGnFrame) > mGrowlTimer && mCurrentMotion == eSlogMotions::Motion_0_Idle)
            {
                mCurrentMotion = eSlogMotions::Motion_23_Growl;
                mNextMotion = eSlogMotions::Motion_0_Idle;

                mGrowlTimer = Math_NextRandom() % 32 + MakeTimer(60);
            }

            if (static_cast<s32>(sGnFrame) > mScratchTimer && mCurrentMotion == eSlogMotions::Motion_0_Idle)
            {
                mScratchTimer = Math_NextRandom() % 32 + MakeTimer(120);
                mCurrentMotion = eSlogMotions::Motion_22_Scratch;
                mNextMotion = eSlogMotions::Motion_0_Idle;
            }

            if (mAngerLevel > mTotalAnger)
            {
                mNextMotion = eSlogMotions::Motion_14_AngryBark;
                mAngerLevel = mAngerLevel + Slog_NextRandom() % 8;
                return 5;
            }

            if (mAngerLevel >= mWakeUpAnger)
            {
                return mBrainSubState;
            }

            mNextMotion = eSlogMotions::Motion_16_MoveHeadDownwards;
            return 3;

        case 5:
            if (IsEventInRange(kEventSuspiciousNoise, mXPos, mYPos, EventScale::Full))
            {
                mAngerLevel++;
            }

            if (IsEventInRange(kEventSpeaking, mXPos, mYPos, EventScale::Full))
            {
                mAngerLevel += Math_NextRandom() % 8 + 15;
            }

            if (!(static_cast<s32>(sGnFrame) % 2))
            {
                if (mAngerLevel > 0)
                {
                    mAngerLevel--;
                }
            }

            if (PlayerOrNakedSligNear())
            {
                mAngerLevel += 2;
            }

            if (mAngerLevel >= mTotalAnger)
            {
                if (mAngerLevel <= mChaseAnger)
                {
                    return mBrainSubState;
                }
                else
                {
                    mCommandedToAttack = false;
                    mBrainState = eSlogBrains::Brain_2_ChasingAbe;
                    return 0;
                }
            }
            else
            {
                mCurrentMotion = eSlogMotions::Motion_0_Idle;
                mScratchTimer = Math_NextRandom() % 32 + MakeTimer(120);
                mGrowlTimer = Math_NextRandom() % 32 + MakeTimer(60);
                return 4;
            }
            break;

        default:
            return mBrainSubState;
    }
}

s16 Slog::Brain_2_ChasingAbe()
{
    auto pTarget = static_cast<IBaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetId));
    if (mListenToSligs)
    {
        if (mLastGameSpeakEvent != gEventSystem->mLastEventIndex)
        {
            mLastGameSpeakEvent = gEventSystem->mLastEventIndex;
            if (gEventSystem->mLastEvent == GameSpeakEvents::eSlig_HereBoy && sControlledCharacter->Type() == ReliveTypes::eSlig)
            {
                mBrainState = eSlogBrains::Brain_0_ListeningToSlig;
                mTargetId = Guid{};
                mListeningToSligId = sControlledCharacter->mBaseGameObjectId;
                return 0;
            }
        }
    }

    bool oldTargetGoneGetNewTarget = false;

    // Do we still have the target?
    if (!pTarget)
    {
        // Did we have a target
        if (mTargetId != Guid{})
        {
            mTargetId = Guid{};
            mAngerLevel = 0;
            mBrainState = eSlogBrains::Brain_1_Idle;
            mNextMotion = eSlogMotions::Motion_0_Idle;
            return 0;
        }
        oldTargetGoneGetNewTarget = true;
    }

    if (oldTargetGoneGetNewTarget || !mCommandedToAttack || pTarget->GetSpriteScale() == FP_FromDouble(0.5))
    {
        if (!mBitingTarget)
        {
            pTarget = FindTarget(0, 0);
            if (!pTarget)
            {
                pTarget = FindTarget(0, 1);
                if (!pTarget)
                {
                    pTarget = sControlledCharacter;
                    if (sControlledCharacter->GetSpriteScale() == FP_FromDouble(0.5))
                    {
                        mTargetId = Guid{};
                        mAngerLevel = 0;
                        mBrainState = eSlogBrains::Brain_1_Idle;
                        mNextMotion = eSlogMotions::Motion_0_Idle;
                        return 0;
                    }
                }
            }
            mTargetId = pTarget->mBaseGameObjectId;
        }
    }

    switch (mBrainSubState)
    {
        case 0:
            return Brain_ChasingAbe_State_0_Init();
        case 1:
            return Brain_ChasingAbe_State_1_Waiting();
        case 2:
            return Brain_ChasingAbe_State_2_Thinking(pTarget);
        case 3:
            return Brain_ChasingAbe_State_3_GrowlOrScratch(pTarget);
        case 4:
            return Brain_ChasingAbe_State_4_LungingAtTarget(pTarget);
        case 7:
            return Brain_ChasingAbe_State_7_EatingTarget(pTarget);
        case 8:
            return Brain_ChasingAbe_State_8_ToIdle();
        case 9:
            return Brain_ChasingAbe_State_9_Falling();
        case 10:
            return Brain_ChasingAbe_State_10_HungryForBone();
        case 11:
            return Brain_ChasingAbe_State_11_ChasingAfterBone();
        case 12:
            return Brain_ChasingAbe_State_12_WalkingToBone();
        case 13:
            return Brain_ChasingAbe_State_13_EatingBone();
        case 14:
            return Brain_ChasingAbe_State_14_CheckingIfBoneNearby();
        case 15:
            return Brain_ChasingAbe_State_15_ChasingAfterTarget(pTarget);
        case 16:
            return Brain_ChasingAbe_State_16_JumpingUpwards();
        case 17:
            return Brain_ChasingAbe_State_17_WaitingToChase(pTarget);
        case 18:
            return Brain_ChasingAbe_State_18_WaitingToJump(pTarget);
        case 19:
            return Brain_ChasingAbe_State_19_AboutToCollide(pTarget);
        case 20:
            return Brain_ChasingAbe_State_20_Collided(pTarget);
        default:
            return mBrainSubState;
    }
}

s16 Slog::Brain_ChasingAbe_State_19_AboutToCollide(IBaseAliveGameObject* pTarget)
{
    FP gridSize = {};

    if (GetAnimation().GetFlipX())
    {
        gridSize = -ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        gridSize = ScaleToGridSize(GetSpriteScale());
    }

    if (!CollisionCheck(GetSpriteScale() * FP_FromInteger(20), FP_FromInteger(2) * gridSize))
    {
        return 2;
    }

    if (pTarget)
    {
        if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()), pTarget))
        {
            if (pTarget->mHealth > FP_FromInteger(0) && VOnSameYLevel(pTarget))
            {
                mNextMotion = eSlogMotions::Motion_18_JumpForwards;
                return 4;
            }
        }
    }

    if (!(Slog_NextRandom() % 64))
    {
        mCurrentMotion = eSlogMotions::Motion_5_MoveHeadUpwards;
        return mBrainSubState;
    }
    return Brain_ChasingAbe_State_20_Collided(pTarget);
}

s16 Slog::Brain_ChasingAbe_State_18_WaitingToJump(IBaseAliveGameObject* pTarget)
{
    if (mCurrentMotion != eSlogMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }

    mJumpCounter -= 20;

    if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), pTarget))
    {
        return 15;
    }
    mNextMotion = eSlogMotions::Motion_2_Run;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_17_WaitingToChase(IBaseAliveGameObject* pTarget)
{
    if (mCurrentMotion == eSlogMotions::Motion_0_Idle)
    {
        if (!VIsFacingMe(pTarget))
        {
            mNextMotion = eSlogMotions::Motion_3_TurnAround;
            return mBrainSubState;
        }
        else
        {
            mNextMotion = eSlogMotions::Motion_5_MoveHeadUpwards;
        }
    }

    if (static_cast<s32>(sGnFrame) <= mMultiUseTimer)
    {
        return mBrainSubState;
    }

    if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), pTarget))
    {
        mNextMotion = eSlogMotions::Motion_2_Run;
    }
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_16_JumpingUpwards()
{
    if (mCurrentMotion != eSlogMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }
    mJumpCounter += Slog_NextRandom() % 64;
    return 15;
}

s16 Slog::Brain_ChasingAbe_State_15_ChasingAfterTarget(IBaseAliveGameObject* pTarget)
{
    if (mVelX > FP_FromInteger(0) && HandleEnemyStopper())
    {
        mNextMotion = eSlogMotions::Motion_6_StopRunning;
        mStopRunning = mVelX < FP_FromInteger(0) ? true : false;
        mScratchTimer = Math_NextRandom() % 32 + MakeTimer(120);
        mGrowlTimer = Math_NextRandom() % 32 + MakeTimer(60);
        return 20;
    }

    if (!VIsFacingMe(pTarget) && mCurrentMotion == eSlogMotions::Motion_2_Run)
    {
        mNextMotion = eSlogMotions::Motion_7_SlideTurn;
    }

    if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), pTarget))
    {
        if (pTarget->GetSpriteScale() == FP_FromInteger(1) && mCurrentMotion == eSlogMotions::Motion_2_Run)
        {
            if (VIsFacingMe(pTarget))
            {
                mNextMotion = eSlogMotions::Motion_6_StopRunning;
            }
        }
    }

    if (mCurrentMotion == eSlogMotions::Motion_7_SlideTurn)
    {
        mNextMotion = eSlogMotions::Motion_2_Run;
    }

    if (mYPos >= pTarget->mYPos + FP_FromInteger(50))
    {
        auto pBone = FindBone();
        if (pBone)
        {
            mBoneId = pBone->mBaseGameObjectId;
            return 11;
        }

        if (mCurrentMotion == eSlogMotions::Motion_4_Fall)
        {
            return 9;
        }

        if (mCurrentMotion != eSlogMotions::Motion_0_Idle)
        {
            return mBrainSubState;
        }

        if (VIsFacingMe(pTarget))
        {
            if (mJumpCounter < 100 && VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), pTarget))
            {
                mNextMotion = eSlogMotions::Motion_19_JumpUpwards;
                return 16;
            }

            if (Math_RandomRange(0, 100) < 20)
            {
                mCurrentMotion = eSlogMotions::Motion_23_Growl;
            }

            mNextMotion = eSlogMotions::Motion_5_MoveHeadUpwards;
            return 18;
        }

        mNextMotion = eSlogMotions::Motion_3_TurnAround;
        return mBrainSubState;
    }

    mMultiUseTimer = MakeTimer(mChaseDelay);
    return 17;
}

s16 Slog::Brain_ChasingAbe_State_14_CheckingIfBoneNearby()
{
    auto pBone = static_cast<Bone*>(sObjectIds.Find_Impl(mBoneId));
    if (!pBone)
    {
        return 0;
    }

    if (FP_Abs(mYPos - pBone->mYPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
    {
        if (pBone->VIsFalling())
        {
            mBoneId = Guid{};
            mNextMotion = eSlogMotions::Motion_0_Idle;
            return 2;
        }

        if (mCurrentMotion == eSlogMotions::Motion_0_Idle)
        {
            FP gridSize = {};
            if (GetAnimation().GetFlipX())
            {
                gridSize = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridSize = ScaleToGridSize(GetSpriteScale());
            }

            if (CollisionCheck(GetSpriteScale() * FP_FromInteger(20), gridSize * FP_FromInteger(2)))
            {
                mNextMotion = eSlogMotions::Motion_3_TurnAround;
            }
            else
            {
                mNextMotion = eSlogMotions::Motion_1_Walk;
            }
        }

        if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(1), pBone))
        {
            return mBrainSubState;
        }
        return 12;
    }

    mBoneId = Guid{};
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_13_EatingBone()
{
    auto pBone = static_cast<Bone*>(sObjectIds.Find_Impl(mBoneId));
    if (!pBone || pBone->VIsFalling())
    {
        mBoneId = Guid{};
        mNextMotion = eSlogMotions::Motion_0_Idle;
        return 2;
    }

    if (FP_Abs(mYPos - pBone->mYPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
    {
        if (VIsFacingMe(pBone))
        {
            if (mCurrentMotion == eSlogMotions::Motion_0_Idle)
            {
                mNextMotion = eSlogMotions::Motion_20_Eating;
            }

            if (mMultiUseTimer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }

            pBone->SetDead(true);
            mNextMotion = eSlogMotions::Motion_0_Idle;
            mBoneId = Guid{};
            return 2;
        }

        mNextMotion = eSlogMotions::Motion_3_TurnAround;
        return 12;
    }

    mBoneId = Guid{};
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_12_WalkingToBone()
{
    if (mCurrentMotion != eSlogMotions::Motion_0_Idle && mCurrentMotion != eSlogMotions::Motion_1_Walk)
    {
        return mBrainSubState;
    }

    auto pBone = static_cast<Bone*>(sObjectIds.Find_Impl(mBoneId));
    if (!pBone || pBone->VIsFalling())
    {
        mBoneId = Guid{};
        mNextMotion = eSlogMotions::Motion_0_Idle;
        return 2;
    }

    if (mCurrentMotion == eSlogMotions::Motion_0_Idle)
    {
        mNextMotion = eSlogMotions::Motion_1_Walk;
    }

    if (FP_Abs(mYPos - pBone->mYPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
    {
        if (VIsFacingMe(pBone))
        {
            if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(1.5), pBone))
            {
                mNextMotion = eSlogMotions::Motion_1_Walk;
            }

            if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(1.5), pBone) || pBone->mVelX > FP_FromInteger(0))
            {
                return mBrainSubState;
            }

            mNextMotion = eSlogMotions::Motion_0_Idle;
            mMultiUseTimer = MakeTimer(mBoneEatingTime);
            return 13;
        }

        mNextMotion = eSlogMotions::Motion_3_TurnAround;
        return mBrainSubState;
    }

    mBoneId = Guid{};
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_11_ChasingAfterBone()
{
    auto pBone = static_cast<Bone*>(sObjectIds.Find_Impl(mBoneId));
    if (!pBone || pBone->VIsFalling())
    {
        mBoneId = Guid{};
        mNextMotion = eSlogMotions::Motion_0_Idle;
        return 2;
    }

    if (FP_Abs(mYPos - pBone->mYPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
    {
        if (!VIsFacingMe(pBone) && mCurrentMotion == eSlogMotions::Motion_2_Run)
        {
            mNextMotion = eSlogMotions::Motion_7_SlideTurn;
        }

        if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4), pBone))
        {
            if (FP_Abs(mYPos - pBone->mYPos) < FP_FromInteger(50) && mCurrentMotion == eSlogMotions::Motion_2_Run)
            {
                mNextMotion = eSlogMotions::Motion_6_StopRunning;
                return 12;
            }
        }

        if (mCurrentMotion == eSlogMotions::Motion_7_SlideTurn)
        {
            mNextMotion = eSlogMotions::Motion_2_Run;
        }

        if (mCurrentMotion == eSlogMotions::Motion_1_Walk)
        {
            mNextMotion = eSlogMotions::Motion_2_Run;
        }

        if (mCurrentMotion == eSlogMotions::Motion_4_Fall)
        {
            mBoneId = Guid{};
            return 9;
        }

        if (mCurrentMotion != eSlogMotions::Motion_0_Idle)
        {
            return mBrainSubState;
        }

        if (VIsFacingMe(pBone))
        {
            FP gridSize = {};
            if (GetAnimation().GetFlipX())
            {
                gridSize = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridSize = ScaleToGridSize(GetSpriteScale());
            }

            // TODO: Same check twice ??
            if (CollisionCheck(GetSpriteScale() * FP_FromInteger(20), FP_FromInteger(4) * gridSize))
            {
                FP gridSize2 = {};
                if (GetAnimation().GetFlipX())
                {
                    gridSize2 = -ScaleToGridSize(GetSpriteScale());
                }
                else
                {
                    gridSize2 = ScaleToGridSize(GetSpriteScale());
                }

                if (CollisionCheck(GetSpriteScale() * FP_FromInteger(20), FP_FromInteger(1) * gridSize2))
                {
                    if (Slog_NextRandom() % 32)
                    {
                        return mBrainSubState;
                    }
                    mCurrentMotion = eSlogMotions::Motion_5_MoveHeadUpwards;
                    return mBrainSubState;
                }

                mNextMotion = eSlogMotions::Motion_1_Walk;
                return 12;
            }

            mNextMotion = eSlogMotions::Motion_2_Run;
            return mBrainSubState;
        }

        mNextMotion = eSlogMotions::Motion_3_TurnAround;
        return mBrainSubState;
    }

    mBoneId = Guid{};
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_20_Collided(IBaseAliveGameObject* pTarget)
{
    auto pBone = FindBone();
    if (pBone)
    {
        mBoneId = pBone->mBaseGameObjectId;
        return 11;
    }

    if (mCurrentMotion != eSlogMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }

    if (mStopRunning)
    {
        if (pTarget->mXPos > mXPos)
        {
            return 2;
        }
    }
    else
    {
        if (pTarget->mXPos < mXPos)
        {
            return 2;
        }
    }

    if (static_cast<s32>(sGnFrame) > mGrowlTimer)
    {
        mGrowlTimer = Math_NextRandom() % 32 + MakeTimer(60);
        mCurrentMotion = eSlogMotions::Motion_23_Growl;
        mNextMotion = eSlogMotions::Motion_0_Idle;
    }

    if (static_cast<s32>(sGnFrame) <= mScratchTimer)
    {
        return mBrainSubState;
    }

    mScratchTimer = Math_NextRandom() % 32 + MakeTimer(120);
    mCurrentMotion = eSlogMotions::Motion_22_Scratch;
    mNextMotion = eSlogMotions::Motion_0_Idle;
    return mBrainSubState;
}

s16 Slog::Brain_ChasingAbe_State_10_HungryForBone()
{
    if (mCurrentMotion == eSlogMotions::Motion_0_Idle)
    {
        mNextMotion = eSlogMotions::Motion_19_JumpUpwards;
    }

    if (static_cast<s32>(sGnFrame) <= mMultiUseTimer)
    {
        auto pBone = FindBone();
        if (pBone)
        {
            mNextMotion = eSlogMotions::Motion_2_Run;
            mBoneId = pBone->mBaseGameObjectId;
            return 11;
        }

        if (mCurrentMotion == eSlogMotions::Motion_6_StopRunning)
        {
            return mBrainSubState;
        }

        if (mCurrentMotion != eSlogMotions::Motion_0_Idle)
        {
            return mBrainSubState;
        }

        if (Slog_NextRandom() % 16)
        {
            return mBrainSubState;
        }

        mNextMotion = eSlogMotions::Motion_5_MoveHeadUpwards;
        return mBrainSubState;
    }

    mHungry = false;
    mNextMotion = eSlogMotions::Motion_2_Run;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_9_Falling()
{
    if (mCurrentMotion != eSlogMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }
    mCurrentMotion = eSlogMotions::Motion_2_Run;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_8_ToIdle()
{
    if (mCurrentMotion != eSlogMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }
    mAngerLevel = 0;
    mBrainState = eSlogBrains::Brain_1_Idle;
    mBitingTarget = 0;
    return 0;
}

s16 Slog::Brain_ChasingAbe_State_7_EatingTarget(IBaseAliveGameObject* pTarget)
{
    if (static_cast<s32>(sGnFrame) <= mMultiUseTimer && pTarget->GetAnimation().GetRender())
    {
        if (mCurrentMotion != eSlogMotions::Motion_0_Idle)
        {
            return mBrainSubState;
        }

        mCurrentMotion = eSlogMotions::Motion_20_Eating;
        return mBrainSubState;
    }

    mNextMotion = eSlogMotions::Motion_0_Idle;
    return 8;
}

s16 Slog::Brain_ChasingAbe_State_4_LungingAtTarget(IBaseAliveGameObject* pTarget)
{
    if (mCurrentMotion == eSlogMotions::Motion_2_Run)
    {
        if (pTarget->mHealth > FP_FromInteger(0))
        {
            mMultiUseTimer = Math_RandomRange(1, 3) + MakeTimer(mChaseDelay);
            return 1;
        }

        if (FP_Abs(pTarget->mXPos - mXPos) > ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2))
        {
            mMultiUseTimer = Math_RandomRange(1, 3) + MakeTimer(mChaseDelay);
            return 1;
        }

        if (FP_Abs(pTarget->mYPos - mYPos) > ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2))
        {
            mMultiUseTimer = Math_RandomRange(1, 3) + MakeTimer(mChaseDelay);
            return 1;
        }

        if (VIsFacingMe(pTarget))
        {
            mCurrentMotion = eSlogMotions::Motion_20_Eating;
            mNextMotion = eSlogMotions::m1;
        }
        else
        {
            mCurrentMotion = eSlogMotions::Motion_3_TurnAround;
            mNextMotion = eSlogMotions::Motion_20_Eating;
        }

        mMultiUseTimer = MakeTimer(90);
        return 7;
    }

    if (mCurrentMotion != eSlogMotions::Motion_4_Fall)
    {
        return mBrainSubState;
    }

    return 9;
}

s16 Slog::Brain_ChasingAbe_State_3_GrowlOrScratch(IBaseAliveGameObject* pTarget)
{
    if (mCurrentMotion != eSlogMotions::Motion_0_Idle)
    {
        mNextMotion = eSlogMotions::Motion_0_Idle;
        return mBrainSubState;
    }

    if (Slog_NextRandom() % 64)
    {
        if (static_cast<s32>(sGnFrame) > mGrowlTimer)
        {
            mGrowlTimer = Math_NextRandom() % 32 + MakeTimer(60);
            mCurrentMotion = eSlogMotions::Motion_23_Growl;
            mNextMotion = eSlogMotions::Motion_0_Idle;
        }

        if (static_cast<s32>(sGnFrame) > mScratchTimer)
        {
            mScratchTimer = Math_NextRandom() % 32 + MakeTimer(120);
            mCurrentMotion = eSlogMotions::Motion_22_Scratch;
            mNextMotion = eSlogMotions::Motion_0_Idle;
        }

        if (pTarget->GetSpriteScale() != FP_FromInteger(1))
        {
            return mBrainSubState;
        }
        return 2;
    }

    mCurrentMotion = eSlogMotions::Motion_5_MoveHeadUpwards;
    return mBrainSubState;
}

s16 Slog::Brain_ChasingAbe_State_2_Thinking(IBaseAliveGameObject* pTarget)
{
    if (mVelX > FP_FromInteger(0) && HandleEnemyStopper())
    {
        mNextMotion = eSlogMotions::Motion_6_StopRunning;
        mStopRunning = mVelX < FP_FromInteger(0) ? true : false;
        mScratchTimer = Math_NextRandom() % 32 + MakeTimer(120);
        mGrowlTimer = Math_NextRandom() % 32 + MakeTimer(60);
        return 20;
    }

    if (CollisionCheck(GetSpriteScale() * FP_FromInteger(20), mVelX * FP_FromInteger(4)))
    {
        mNextMotion = eSlogMotions::Motion_6_StopRunning;
        mStopRunning = mVelX < FP_FromInteger(0) ? true : false;
        mScratchTimer = Math_NextRandom() % 32 + MakeTimer(120);
        mGrowlTimer = Math_NextRandom() % 32 + MakeTimer(60);
        return 19;
    }

    if (!VIsFacingMe(pTarget) && mCurrentMotion == eSlogMotions::Motion_2_Run)
    {
        mNextMotion = eSlogMotions::Motion_7_SlideTurn;
    }

    if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), pTarget))
    {
        if (VOnSameYLevel(pTarget))
        {
            if (VIsFacingMe(pTarget))
            {
                if (!CollisionCheck(GetSpriteScale() * FP_FromInteger(20), pTarget->mXPos - mXPos) && !mMovedOffScreen)
                {
                    if (pTarget->mHealth <= FP_FromInteger(0))
                    {
                        mNextMotion = eSlogMotions::Motion_0_Idle;
                    }
                    else
                    {
                        mNextMotion = eSlogMotions::Motion_18_JumpForwards;
                    }
                }
            }
        }
    }

    if (mCurrentMotion == eSlogMotions::Motion_7_SlideTurn)
    {
        mNextMotion = eSlogMotions::Motion_2_Run;
    }

    auto pBone = FindBone();
    if (pBone)
    {
        mBoneId = pBone->mBaseGameObjectId;
        return 11;
    }

    if (mCurrentMotion == eSlogMotions::Motion_0_Idle)
    {
        if (VIsFacingMe(pTarget))
        {
            FP gridSizeDirected = {};
            if (GetAnimation().GetFlipX())
            {
                gridSizeDirected = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridSizeDirected = ScaleToGridSize(GetSpriteScale());
            }

            if (!CollisionCheck(GetSpriteScale() * FP_FromInteger(20), FP_FromInteger(2) * gridSizeDirected))
            {
                if (pTarget->mHealth > FP_FromInteger(0))
                {
                    mMultiUseTimer = Math_RandomRange(1, 3) + MakeTimer(mChaseDelay);
                    return 1;
                }

                if (FP_Abs(pTarget->mXPos - mXPos) > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2)))
                {
                    mMultiUseTimer = Math_RandomRange(1, 3) + MakeTimer(mChaseDelay);
                    return 1;
                }

                if (FP_Abs(pTarget->mYPos - mYPos) > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2)))
                {
                    mMultiUseTimer = Math_RandomRange(1, 3) + MakeTimer(mChaseDelay);
                    return 1;
                }
                mMultiUseTimer = MakeTimer(90);
                return 7;
            }

            mStopRunning = GetAnimation().GetFlipX();

            mScratchTimer = Math_NextRandom() % 32 + MakeTimer(120);
            mGrowlTimer = Math_NextRandom() % 32 + MakeTimer(60);
            return 19;
        }
        mCurrentMotion = eSlogMotions::Motion_3_TurnAround;
    }

    if (mHungry && IsAbe(pTarget) && pTarget->GetScale() == GetScale() && (gAbe->mCurrentMotion == eAbeMotions::Motion_104_RockThrowStandingHold || gAbe->mCurrentMotion == eAbeMotions::Motion_107_RockThrowCrouchingHold))
    {
        mNextMotion = eSlogMotions::Motion_6_StopRunning;
        mMultiUseTimer = MakeTimer(90);
        return 10;
    }

    if (mCurrentMotion == eSlogMotions::Motion_4_Fall)
    {
        return 9;
    }

    if (mCurrentMotion == eSlogMotions::Motion_18_JumpForwards)
    {
        return 4;
    }

    if (mYPos <= pTarget->mYPos + FP_FromInteger(50))
    {
        if (pTarget->GetSpriteScale() != FP_FromDouble(0.5))
        {
            return mBrainSubState;
        }
        mGrowlTimer = Math_NextRandom() % 32 + MakeTimer(60);
        mScratchTimer = Math_NextRandom() % 32 + MakeTimer(120);
        return 3;
    }

    return 15;
}

s16 Slog::Brain_ChasingAbe_State_1_Waiting()
{
    if (mMultiUseTimer > static_cast<s32>(sGnFrame))
    {
        return mBrainSubState;
    }
    mNextMotion = eSlogMotions::Motion_2_Run;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_0_Init()
{
    mBitingTarget = 0;
    mJumpCounter = 0;
    mBoneId = Guid{};
    mMultiUseTimer = Math_RandomRange(1, 3) + MakeTimer(mChaseDelay);
    Sfx(SlogSound::AttackGrowl_8);
    return 1;
}

s16 Slog::Brain_3_Death()
{
    mListeningToSligId = Guid{};
    mTargetId = Guid{};

    if (mMultiUseTimer < static_cast<s32>(sGnFrame + 80))
    {
        SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.023));
        mRGB.r -= 2;
        mRGB.g -= 2;
        mRGB.b -= 2;
    }

    if (static_cast<s32>(sGnFrame) < mMultiUseTimer - 24)
    {
        DeathSmokeEffect(true);
    }

    if (GetSpriteScale() < FP_FromInteger(0))
    {
        SetDead(true);
    }

    return 100;
}

void Slog::SetAnimFrame()
{
    GetAnimation().Set_Animation_Data(GetAnimRes(sSlogAnimIdTable[static_cast<u32>(mCurrentMotion)]));
}

const TintEntry sSlogTints_560A48[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};

void Slog::Init()
{
    SetType(ReliveTypes::eSlog);
    Animation_Init(GetAnimRes(AnimId::Slog_Idle));

    SetCanSetOffExplosives(true);

    mShot = false;
    mHitByAbilityRing = false;
    mHungry = true;

    SetDoPurpleLightEffect(true);
    GetAnimation().SetFnPtrArray(gSlog_Anim_Frame_Fns);
    mMultiUseTimer = 0;
    mBrainSubState = 0;
    mNextMotion = eSlogMotions::m1;

    BaseAliveGameObject_PlatformId = Guid{};
    mListeningToSligId = Guid{};
    mTargetId = Guid{};
    mBoneId = Guid{};
    SetTint(&sSlogTints_560A48[0], gMap.mCurrentLevel);
    GetAnimation().SetRenderLayer(Layer::eLayer_SlogFleech_34);

    if (GetSpriteScale() == FP_FromInteger(1))
    {
        mYOffset = 1;
    }
    else
    {
        mYOffset = 2;
    }

    SetScale(Scale::Fg);

    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            mXPos, mYPos,
            mXPos, mYPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine, &hitX, &hitY, CollisionMask(eFloor_0, eDynamicCollision_32))
        == 1)
    {
        mYPos = hitY;
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32)
        {
            const PSX_RECT bRect = VGetBoundingRect();
            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            OnCollisionWith(xy, wh, gPlatformsArray);
        }
    }

    MapFollowMe(false);
    CreateShadow();

    sSlogCount++;

    VStackOnObjectsOfType(ReliveTypes::eSlog);
}

void Slog::VUpdate()
{
    if (GetRestoredFromQuickSave())
    {
        SetRestoredFromQuickSave(false);
        if (BaseAliveGameObjectCollisionLineType == -1)
        {
            BaseAliveGameObjectCollisionLine = nullptr;
        }
        else
        {
            gCollisions->Raycast(
                mXPos,
                mYPos - FP_FromInteger(20),
                mXPos,
                mYPos + FP_FromInteger(20),
                &BaseAliveGameObjectCollisionLine,
                &mXPos,
                &mYPos,
                CollisionMask(static_cast<eLineTypes>(BaseAliveGameObjectCollisionLineType)));
        }
        BaseAliveGameObjectCollisionLineType = 0;
        mTargetId = BaseGameObject::RefreshId(mTargetId);
        mListeningToSligId = BaseGameObject::RefreshId(mListeningToSligId);
        mBoneId = BaseGameObject::RefreshId(mBoneId);
    }

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (FP_Abs(mXPos - sControlledCharacter->mXPos) > FP_FromInteger(750) || FP_Abs(mYPos - sControlledCharacter->mYPos) > FP_FromInteger(390))
    {
        GetAnimation().SetAnimate(false);
        GetAnimation().SetRender(false);
    }
    else
    {
        if (mHealth > FP_FromInteger(0))
        {
            GetAnimation().SetAnimate(true);
            GetAnimation().SetRender(true);
        }

        const auto oldMotion = mCurrentMotion;
        mBrainSubState = InvokeMemberFunction(this, sSlogBrainTable, mBrainState);
        if (gDDCheat_ShowAI_Info)
        {
            DDCheat::DebugStr("Slog:  Motion=%d  BrainState=%d\n", mCurrentMotion, mBrainSubState);
        }

        const FP oldXPos = mXPos;
        const FP oldYPos = mYPos;

        InvokeMemberFunction(this, sSlogMotionTable, mCurrentMotion);

        if (oldXPos != mXPos || oldYPos != mYPos)
        {
            BaseAliveGameObjectPathTLV = gPathInfo->TLV_Get_At(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
            VOnTlvCollision(BaseAliveGameObjectPathTLV);
        }

        if (oldMotion != mCurrentMotion)
        {
            SetAnimFrame();
        }
    }
}

Slog::~Slog()
{
    mTargetId = Guid{};
    mListeningToSligId = Guid{};
    mBoneId = Guid{};

    if (mTlvId.IsValid())
    {
        if (mHealth <= FP_FromInteger(0))
        {
            Path::TLV_Delete(mTlvId);
        }
        else
        {
            Path::TLV_Reset(mTlvId);
        }
    }

    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

    if (!mShot)
    {
        sSlogCount--;
    }
}


void Slog::ToIdle()
{
    MapFollowMe(false);
    mFallingVelxScaleFactor = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mCurrentMotion = eSlogMotions::Motion_0_Idle;
    mNextMotion = eSlogMotions::m1;
}

const relive::SfxDefinition& getSfxDef(SlogSound effectId)
{
    return sSlogSFXList[static_cast<s32>(effectId)];
}

void Slog::Sfx(SlogSound effectId)
{
    s16 volumeLeft = 0;
    s16 volumeRight = 0;
    const relive::SfxDefinition& effectDef = getSfxDef(effectId);


    const CameraPos direction = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);
    PSX_RECT pRect = {};
    gMap.Get_Camera_World_Rect(direction, &pRect);

    const s16 defaultSndIdxVol = effectDef.mDefaultVolume;
    volumeRight = defaultSndIdxVol;
    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
            volumeLeft = volumeRight;
            break;
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
            volumeRight = volumeLeft;
        }
        break;
        case CameraPos::eCamLeft_3:
        {
            const FP percentHowFar = (FP_FromInteger(pRect.w) - mXPos) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
        }
        break;
        case CameraPos::eCamRight_4:
        {
            const FP percentHowFar = (mXPos - FP_FromInteger(pRect.x)) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
        }
        break;
        default:
            return;
    }

    if (GetSpriteScale() == FP_FromDouble(0.5))
    {
        SFX_SfxDefinition_Play_Stereo(
            effectDef,
            volumeLeft,
            volumeRight,
            effectDef.mPitchMin + 1524,
            effectDef.mPitchMax + 1524);
    }
    else
    {
        SFX_SfxDefinition_Play_Stereo(
            effectDef,
            volumeLeft,
            volumeRight,
            effectDef.mPitchMin,
            effectDef.mPitchMax);
    }
}

void Slog::ToJump()
{
    mVelX = (GetAnimation().GetFlipX() ? FP_FromDouble(-10.18) : FP_FromDouble(10.18)) * GetSpriteScale();
    mVelY = FP_FromInteger(-8) * GetSpriteScale();

    BaseAliveGameObjectLastLineYPos = mYPos;

    VOnTrapDoorOpen();

    mCurrentMotion = eSlogMotions::Motion_18_JumpForwards;
    BaseAliveGameObjectCollisionLine = nullptr;

    Sfx(SlogSound::AttackGrowl_8);

    if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eIntenseChase_7, this, 0, 0);
    }
}

s16 Slog::ToNextMotion()
{
    const auto slogMotion = static_cast<eSlogMotions>(mNextMotion);
    switch (slogMotion)
    {
        case eSlogMotions::Motion_3_TurnAround:
            mCurrentMotion = eSlogMotions::Motion_3_TurnAround;
            mNextMotion = eSlogMotions::m1;
            return 1;

        case eSlogMotions::Motion_1_Walk:
            if (GetAnimation().GetFlipX())
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
            }

            if (!CollisionCheck(GetSpriteScale() * FP_FromInteger(20), (mVelX * FP_FromInteger(9)) * FP_FromInteger(2)))
            {
                mCurrentMotion = eSlogMotions::Motion_8_StartWalking;
                mNextMotion = eSlogMotions::m1;
                return 1;
            }
            ToIdle();
            return 0;

        case eSlogMotions::Motion_2_Run:
            if (GetAnimation().GetFlipX())
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(3));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(3));
            }

            if (!CollisionCheck(GetSpriteScale() * FP_FromInteger(20), mVelX * FP_FromInteger(4)))
            {
                mCurrentMotion = eSlogMotions::Motion_2_Run;
                mNextMotion = eSlogMotions::m1;
                return 1;
            }
            ToIdle();
            return 0;
    }
    return 0;
}

bool Slog::CollisionCheck(FP hitY, FP hitX)
{
    PathLine* pLine = nullptr;
    return gCollisions->Raycast(mXPos, mYPos - hitY, mXPos + hitX, mYPos - hitY, &pLine, &hitX, &hitY, CollisionMask(eWallRight_2, eWallLeft_1)) != 0;
}

void Slog::MoveOnLine()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    const FP oldXPos = mXPos;

    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
        if (BaseAliveGameObjectCollisionLine)
        {
            if (pPlatform)
            {
                if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32 && BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
                {
                    pPlatform->VRemove(this);
                    BaseAliveGameObject_PlatformId = Guid{};
                }
            }
            else if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                const PSX_RECT bRect = VGetBoundingRect();
                const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
                OnCollisionWith(xy, wh, gPlatformsArray);
            }
        }
        else
        {
            VOnTrapDoorOpen();
            mFallingVelxScaleFactor = FP_FromDouble(0.3);
            BaseAliveGameObjectLastLineYPos = mYPos;
            mXPos = oldXPos + mVelX;
        }
    }
    else
    {
        mFallingVelxScaleFactor = FP_FromDouble(0.3);
        BaseAliveGameObjectLastLineYPos = mYPos;
        mCurrentMotion = eSlogMotions::Motion_4_Fall;
    }
}

Bone* Slog::FindBone()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eBone)
        {
            auto pBone = static_cast<Bone*>(pObj);
            if (pBone->VCanThrow())
            {
                if (gMap.Is_Point_In_Current_Camera(pBone->mCurrentLevel, pBone->mCurrentPath, pBone->mXPos, pBone->mYPos, 0) && pBone->GetScale() == GetScale())
                {
                    if (FP_Abs(mYPos - pBone->mYPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
                    {
                        return pBone;
                    }
                }
            }
        }
    }
    return nullptr;
}

IBaseAliveGameObject* Slog::FindTarget(s16 bKillSligs, s16 bLookingUp)
{
    PSX_RECT bRect = VGetBoundingRect();

    if (GetAnimation().GetFlipX())
    {
        bRect.x -= 368;
    }
    else
    {
        bRect.w += 368;
    }

    if (bLookingUp)
    {
        bRect.x -= 368;
        bRect.w += 368;
        bRect.y -= gPsxDisplay.mHeight;
        bRect.h += gPsxDisplay.mHeight;
    }

    FP distanceToLastFoundObj = FP_FromInteger(gPsxDisplay.mWidth);
    auto pSligBeingListendTo = static_cast<IBaseAliveGameObject*>(sObjectIds.Find_Impl(mListeningToSligId));

    IBaseAliveGameObject* pBestObj = nullptr;
    IBaseAliveGameObject* pLastFoundObj = nullptr;

    s32 array_idx = 0;
    Guid local_array[10] = {};

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eSlog)
        {
            auto pSlog = static_cast<Slog*>(pObj);
            if (pSlog->mTargetId != Guid{} && array_idx < ALIVE_COUNTOF(local_array))
            {
                local_array[array_idx++] = pSlog->mTargetId;
            }
        }

        if (pObj != pSligBeingListendTo && pObj != this && pObj->GetScale() == GetScale())
        {
            switch (pObj->Type())
            {
                case ReliveTypes::eCrawlingSlig:
                case ReliveTypes::eFlyingSlig:
                case ReliveTypes::eGlukkon:
                case ReliveTypes::eAbe:
                case ReliveTypes::eMudokon:
                case ReliveTypes::eSlig:
                    if (bKillSligs || (!bKillSligs && (pObj->Type() == ReliveTypes::eAbe || pObj->Type() == ReliveTypes::eCrawlingSlig || pObj->Type() == ReliveTypes::eFlyingSlig || pObj->Type() == ReliveTypes::eGlukkon || (pObj->Type() == ReliveTypes::eMudokon && static_cast<Mudokon*>(pObj)->mBrainState == Mud_Brain_State::Brain_4_ListeningToAbe))))
                    {
                        const PSX_RECT objRect = pObj->VGetBoundingRect();

                        if (objRect.x <= bRect.w && objRect.w >= bRect.x && objRect.h >= bRect.y && objRect.y <= bRect.h)
                        {
                            pLastFoundObj = pObj;

                            const FP xDelta = FP_Abs(mXPos - pObj->mXPos);
                            if (xDelta < distanceToLastFoundObj)
                            {
                                s32 array_idx_iter = 0;
                                if (array_idx)
                                {
                                    // Something to do with finding who is the last attacker
                                    while (local_array[array_idx_iter] != pObj->mBaseGameObjectId)
                                    {
                                        if (++array_idx_iter >= array_idx)
                                        {
                                            distanceToLastFoundObj = xDelta;
                                            pBestObj = pObj;
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    distanceToLastFoundObj = xDelta;
                                    pBestObj = pObj;
                                }
                            }
                        }
                    }
                    break;

                default:
                    break;
            }
        }
    }

    if (pBestObj)
    {
        return pBestObj;
    }

    if (pLastFoundObj)
    {
        return pLastFoundObj;
    }

    return nullptr;
}

void Slog::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        mCurrentMotion = eSlogMotions::Motion_4_Fall;
    }
}

void Slog::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            mHealth = FP_FromInteger(0);
            SetDead(true);
        }
        pTlv = gPathInfo->TLV_Get_At(pTlv, mXPos, mYPos, mXPos, mYPos);
    }
}

bool Slog::VTakeDamage(BaseGameObject* pFrom)
{
    if (mHealth <= FP_FromInteger(0))
    {
        return false;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eBullet:
        {
            auto pBullet = static_cast<Bullet*>(pFrom);
            switch (pBullet->mBulletType)
            {
                case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
                case BulletType::eNormalBullet_2:
                    if (pBullet->XDistance() <= FP_FromInteger(0))
                    {
                        relive_new Blood(mXPos,
                                                    pBullet->ypos(),
                                                    FP_FromInteger(-24),
                                                    FP_FromInteger(0),
                                                    GetSpriteScale(), 50);
                    }
                    else
                    {
                        relive_new Blood(mXPos,
                                                    pBullet->ypos(),
                                                    FP_FromInteger(24),
                                                    FP_FromInteger(0),
                                                    GetSpriteScale(), 50);
                    }
                    break;

                case BulletType::ePossessedSligZBullet_1:
                case BulletType::eZBullet_3:
                {
                    relive_new Blood(mXPos,
                                                mYPos - (FP_FromInteger(20) * GetSpriteScale()),
                                                FP_FromInteger(0),
                                                FP_FromInteger(0),
                                                GetSpriteScale(), 50);
                    break;
                }

                default:
                    break;
            }

            Sfx(SlogSound::DeathWhine_9);
            mHealth = FP_FromInteger(0);
            mBrainState = eSlogBrains::Brain_3_Death;
            mCurrentMotion = eSlogMotions::Motion_21_Dying;
            mMultiUseTimer = MakeTimer(90);
            SetAnimFrame();
            GetAnimation().SetAnimate(true);
            mShot = true;
            sSlogCount--;
            break;
        }

        case ReliveTypes::eDrill:
        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eAirExplosion:
        {
            Sfx(SlogSound::DeathWhine_9);
            mHealth = FP_FromInteger(0);
            relive_new Gibs(GibType::eSlog, mXPos, mYPos, mVelX, mVelY, GetSpriteScale(), 0);

            const PSX_RECT bRect = VGetBoundingRect();
            relive_new Blood(FP_FromInteger((bRect.x + bRect.w) / 2),
                                        FP_FromInteger((bRect.y + bRect.h) / 2),
                                        FP_FromInteger(0),
                                        FP_FromInteger(0),
                                        GetSpriteScale(), 50);

            SetDead(true);
            break;
        }

        case ReliveTypes::eElectricWall:
            Sfx(SlogSound::DeathWhine_9);
            break;

        case ReliveTypes::eRockSpawner:
        case ReliveTypes::eMineCar:
            Sfx(SlogSound::DeathWhine_9);
            mHealth = FP_FromInteger(0);
            mBrainState = eSlogBrains::Brain_3_Death;
            mCurrentMotion = eSlogMotions::Motion_21_Dying;
            mMultiUseTimer = MakeTimer(90);
            SetAnimFrame();
            GetAnimation().SetAnimate(true);
            break;

        case ReliveTypes::eAbilityRing:
            if (!mHitByAbilityRing)
            {
                mHitByAbilityRing = true;
                Sfx(SlogSound::DeathWhine_9);
            }
            break;

        case ReliveTypes::eElectrocute:
            mHealth = FP_FromInteger(0);
            mBrainState = eSlogBrains::Brain_3_Death;
            SetDead(true);
            break;

        default:
            return true;
    }
    return true;
}

void Slog::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    mAngerLevel += mChaseAnger;
}

s16 Slog::PlayerOrNakedSligNear()
{
    const FP kMinXDist = FP_FromInteger(100) * GetSpriteScale();
    const FP kMinYDist = FP_FromInteger(25) * GetSpriteScale();

    // Is the player near?
    if (FP_Abs(gAbe->mXPos - mXPos) < kMinXDist && FP_Abs(gAbe->mYPos - mYPos) < kMinYDist && gAbe->GetSpriteScale() == FP_FromInteger(1))
    {
        return 1;
    }

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eCrawlingSlig)
        {
            // Is this naked slig near?
            if (FP_Abs(pObj->mXPos - mXPos) < kMinXDist && FP_Abs(pObj->mYPos - mYPos) < kMinYDist && pObj->GetSpriteScale() == GetSpriteScale())
            {
                return 1;
            }
        }
    }
    return 0;
}

void Slog::DelayedResponse(s16 responseIdx)
{
    mResponsePart = 0;
    mResponseIdx = responseIdx;
    mMultiUseTimer = MakeTimer(10);
}

s16 Slog::HandleEnemyStopper()
{
    FP xPos = ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2);
    if (mVelX >= FP_FromInteger(0))
    {
        xPos += mXPos;
    }
    FP width = mXPos;
    if (mXPos <= xPos)
    {
        width = xPos;
    }
    FP xToUse = xPos;
    if (mXPos <= xPos)
    {
        xToUse = mXPos;
    }

    const auto stopperPath = static_cast<relive::Path_EnemyStopper*>(
        gPathInfo->VTLV_Get_At_Of_Type(
            FP_GetExponent(xToUse), FP_GetExponent(mYPos),
            FP_GetExponent(width), FP_GetExponent(mYPos), ReliveTypes::eEnemyStopper));

    return stopperPath != nullptr && stopperPath->mStopDirection == (mVelX > FP_FromInteger(0) ? relive::Path_EnemyStopper::StopDirection::Right : relive::Path_EnemyStopper::StopDirection::Left) && SwitchStates_Get(stopperPath->mSwitchId) > 0;
}

s16 Slog::Facing(FP xpos)
{
    if (mXPos < xpos && !GetAnimation().GetFlipX())
    {
        return true;
    }

    if (xpos < mXPos && GetAnimation().GetFlipX())
    {
        return true;
    }

    return false;
}
