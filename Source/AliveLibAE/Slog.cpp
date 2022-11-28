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
#include "Sound/Midi.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Bone.hpp"
#include "Sfx.hpp"
#include "Blood.hpp"
#include "SnoozeParticle.hpp"
#include "Gibs.hpp"
#include "Bullet.hpp"
#include "GameSpeak.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Mudokon.hpp"
#include "Grid.hpp"
#include "Function.hpp"
#include "AnimationCallBacks.hpp"
#include "Path.hpp"

s16 sSlogCount = 0;

const AnimId sSlogAnimIdTable[24] = {
    AnimId::Slog_Idle,
    AnimId::Slog_Walk,
    AnimId::Slog_Run,
    AnimId::Slog_TurnAround,
    AnimId::Slog_Fall,
    AnimId::Slog_MoveHeadUpwards,
    AnimId::Slog_StopRunning,
    AnimId::Slog_SlideTurn,
    AnimId::Slog_StartWalking,
    AnimId::Slog_EndWalking,
    AnimId::Slog_Land,
    AnimId::Slog_Unused,
    AnimId::Slog_StartFastBarking,
    AnimId::Slog_EndFastBarking,
    AnimId::Slog_AngryBark,
    AnimId::Slog_Sleeping,
    AnimId::Slog_MoveHeadDownwards,
    AnimId::Slog_WakeUp,
    AnimId::Slog_JumpForwards,
    AnimId::Slog_JumpUpwards,
    AnimId::Slog_Eating,
    AnimId::Slog_Dying,
    AnimId::Slog_Scratch,
    AnimId::Slog_Growl };

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
    field_134_last_event_index = -1;

    mYPos = ypos;
    mXPos = xpos;

    SetSpriteScale(scale);

    Init();

    mCommandedToAttack = false;
    mTlvId = Guid{};
    mBrainState = 2;
    mBrainSubState = 0;

    IBaseAliveGameObject* pTarget = FindTarget(0, 0);
    if (!pTarget)
    {
        pTarget = sControlledCharacter;
    }
    mTargetId = pTarget->mBaseGameObjectId;

    mListenToSligs = false;
    eBit7_Asleep = false;
    mMovedOffScreen = false;

    mListenToSligs = bListenToSligs;

    mWakeUpAnger = 0;
    mChaseDelay = chaseDelay;
    mAngerSwitchId = 0;
    SetCurrentMotion(eSlogMotions::Motion_0_Idle);
    field_146_total_anger = 10;
    field_148_chase_anger = 20;
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
    field_134_last_event_index = -1;

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
    eBit7_Asleep = pTlv->mAsleep == relive::reliveChoice::eYes;
    mCommandedToAttack = false;

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    GetAnimation().mFlags.Set(AnimFlags::eFlipX, pTlv->mFacing == relive::reliveXDirection::eLeft);

    mTlvId = tlvId;
    mBaseGameObjectTlvInfo = tlvId;
    mBrainState = 1;
    mTargetId = Guid{};
    mWakeUpAnger = pTlv->mWakeUpAnger;
    field_146_total_anger = pTlv->mWakeUpAnger + pTlv->mBarkAnger;
    field_148_chase_anger = field_146_total_anger + pTlv->mChaseAnger;
    mChaseDelay = pTlv->mChaseDelay;
    mAngerSwitchId = pTlv->mAngerSwitchId;
    mBoneEatingTime = pTlv->mBoneEatingTime;

    if (eBit7_Asleep)
    {
        SetCurrentMotion(eSlogMotions::Motion_15_Sleeping);
        SetAnimFrame();
    }
    else
    {
        SetCurrentMotion(eSlogMotions::Motion_0_Idle);
    }

    VUpdate();
}

static u8 sSlogRandomIdx = 0;

s32 Slog::VGetSaveState(u8* pSaveBuffer)
{
    if (mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eElectrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<SlogSaveState*>(pSaveBuffer);
    pState->mType = ReliveTypes::eSlog;

    pState->mBaseTlvId = mBaseGameObjectTlvInfo;

    pState->mXPos = mXPos;
    pState->mYPos = mYPos;
    pState->mVelX = mVelX;
    pState->mVelY = mVelY;

    pState->mFallingVelxScaleFactor = mFallingVelxScaleFactor;

    pState->mCurrentPath = mCurrentPath;
    pState->mCurrentLevel = mCurrentLevel;
    pState->mSpriteScale = GetSpriteScale();

    pState->mR = mRGB.r;
    pState->mG = mRGB.g;
    pState->mB = mRGB.b;

    pState->mFlipX = GetAnimation().mFlags.Get(AnimFlags::eFlipX);
    pState->mCurrentMotion = mCurrentMotion;
    pState->mCurrentFrame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    pState->mFrameChangeCounter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());
    pState->mDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->mRender = GetAnimation().mFlags.Get(AnimFlags::eRender);
    pState->mHealth = mHealth;
    pState->mCurrentMotion2 = mCurrentMotion;
    pState->mNextMotion = mNextMotion;
    pState->mLastLineYPos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->mCollisionLineType = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pState->mCollisionLineType = -1;
    }

    pState->mPlatformId = BaseAliveGameObject_PlatformId;
    pState->mSlogTlvId = mTlvId;
    pState->mTargetId = Guid{};

    if (mTargetId != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(mTargetId);
        if (pObj)
        {
            pState->mTargetId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pState->mBrainState = mBrainState;
    pState->mBrainSubState = mBrainSubState;
    pState->mMultiUseTimer = mMultiUseTimer;
    pState->mFallingVelxScaleFactor = mFallingVelxScaleFactor;
    pState->mSlogTlvId = mTlvId;
    pState->mListeningToSligId = Guid{};

    if (mListeningToSligId != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(mListeningToSligId);
        if (pObj)
        {
            pState->mListeningToSligId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pState->mHasWoofed = mHasWoofed;
    pState->mWaitingCounter = mWaitingCounter;
    pState->mResponseIdx = mResponseIdx;
    pState->mResponsePart = mResponsePart;
    pState->mAngerLevel = mAngerLevel;
    pState->mJumpCounter = mJumpCounter;
    pState->mScratchTimer = mScratchTimer;
    pState->mGrowlTimer = mGrowlTimer;
    pState->mBoneId = Guid{};

    if (mBoneId != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(mBoneId);
        if (pObj)
        {
            pState->mBoneId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pState->mChaseDelay = mChaseDelay;
    pState->mSlogRandomIdx = sSlogRandomIdx;

    pState->mBitingTarget = mBitingTarget;
    pState->eBit3_Asleep = eBit8_Asleep;
    pState->mMovedOffScreen = mMovedOffScreen;
    pState->mStopRunning = mStopRunning;
    pState->mShot = mShot;
    pState->mHungry = mHungry;
    pState->mCommandedToAttack = mCommandedToAttack;
    pState->mHitByAbilityRing = mHitByAbilityRing;
    pState->mListenToSligs = mListenToSligs;

    return sizeof(SlogSaveState);
}

s32 Slog::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const SlogSaveState*>(pBuffer);
    auto pTlv = static_cast<relive::Path_Slog*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->mSlogTlvId));

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

        pSlog->SetCurrentMotion(pState->mCurrentMotion);
        pSlog->GetAnimation().Set_Animation_Data(pSlog->GetAnimRes(sSlogAnimIdTable[pSlog->mCurrentMotion]));
        
        pSlog->GetAnimation().SetCurrentFrame(pState->mCurrentFrame);
        pSlog->GetAnimation().SetFrameChangeCounter(pState->mFrameChangeCounter);

        pSlog->GetAnimation().mFlags.Set(AnimFlags::eFlipX, pState->mFlipX & 1);
        pSlog->GetAnimation().mFlags.Set(AnimFlags::eRender, pState->mRender & 1);

        pSlog->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->mDrawable & 1);

        if (IsLastFrame(&pSlog->GetAnimation()))
        {
            pSlog->GetAnimation().mFlags.Set(AnimFlags::eIsLastFrame);
        }

        pSlog->mHealth = pState->mHealth;
        pSlog->mCurrentMotion = pState->mCurrentMotion2;
        pSlog->mNextMotion = pState->mNextMotion;
        pSlog->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->mLastLineYPos);
        pSlog->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eRestoredFromQuickSave);
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

    return sizeof(SlogSaveState);
}

void Slog::Motion_0_Idle()
{
    if (!ToNextMotion())
    {
        if (GetNextMotion() == eSlogMotions::Motion_18_JumpForwards)
        {
            ToJump();
        }
        else if (GetNextMotion() == eSlogMotions::m1)
        {
            if (!mCurrentMotion)
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
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
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
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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

        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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

        if (GetCurrentMotion() == eSlogMotions::Motion_1_Walk)
        {
            if (GetAnimation().GetCurrentFrame() == 2 || GetAnimation().GetCurrentFrame() == 11)
            {
                if (GetNextMotion() == eSlogMotions::Motion_0_Idle)
                {
                    SetCurrentMotion(eSlogMotions::Motion_9_EndWalking);
                    SetNextMotion(eSlogMotions::m1);
                }
                if (GetNextMotion() == eSlogMotions::Motion_3_TurnAround)
                {
                    SetCurrentMotion(eSlogMotions::Motion_9_EndWalking);
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

                if (GetNextMotion() == eSlogMotions::Motion_2_Run)
                {
                    SetCurrentMotion(eSlogMotions::Motion_2_Run);
                    SetNextMotion(eSlogMotions::m1);
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
    return sRandomBytes_546744[sSlogRandomIdx++];
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

    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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

        if (GetCurrentMotion() == eSlogMotions::Motion_2_Run)
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

                if (GetNextMotion() == eSlogMotions::Motion_0_Idle)
                {
                    SetCurrentMotion(eSlogMotions::Motion_6_StopRunning);
                    SetNextMotion(eSlogMotions::m1);
                }
                else if (GetNextMotion() == eSlogMotions::Motion_18_JumpForwards)
                {
                    ToJump();
                    SetNextMotion(eSlogMotions::m1);
                }
                else if (GetNextMotion() != eSlogMotions::m1)
                {
                    SetCurrentMotion(mNextMotion);
                    SetNextMotion(eSlogMotions::m1);
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
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
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
    if (sCollisions->Raycast(
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
                SetCurrentMotion(eSlogMotions::Motion_10_Land);
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

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
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
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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
        if (!GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
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
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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

        if (GetCurrentMotion() == eSlogMotions::Motion_7_SlideTurn)
        {
            if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                MapFollowMe(false);

                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                {
                    GetAnimation().mFlags.Clear(AnimFlags::eFlipX);
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(3));
                }
                else
                {
                    GetAnimation().mFlags.Set(AnimFlags::eFlipX);
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(3));
                }
                SetCurrentMotion(eSlogMotions::Motion_2_Run);
            }
        }
    }
}

void Slog::Motion_8_StartWalking()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eSlogMotions::Motion_1_Walk);
    }

    MoveOnLine();
}

void Slog::Motion_9_EndWalking()
{
    MoveOnLine();

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
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

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToIdle();
    }
}

void Slog::Motion_11_Unused()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eSlogMotions::Motion_12_StartFastBarking);
    }
}

void Slog::Motion_12_StartFastBarking()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Sfx(SlogSound::IdleWoof_2);
        mHasWoofed = 1;
    }

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            SetCurrentMotion(eSlogMotions::Motion_13_EndFastBarking);
        }
    }
}

void Slog::Motion_13_EndFastBarking()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
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

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
        }
    }
}

void Slog::Motion_15_Sleeping()
{
    if (GetNextMotion() != eSlogMotions::m1 && GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(mNextMotion);
        SetNextMotion(eSlogMotions::m1);
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
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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
    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
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

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
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
    if (sCollisions->Raycast(oldXPos, oldYPos - k20Scaled, mXPos, mYPos, &pLine, &hitX, &hitY, CollisionMask(eCeiling_3, eWallRight_2, eWallLeft_1, eFloor_0, eDynamicCollision_32)
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
                    SetNextMotion(eSlogMotions::m1);
                    SetCurrentMotion(eSlogMotions::Motion_2_Run);
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
        SetCurrentMotion(eSlogMotions::Motion_4_Fall);
    }
}

void Slog::Motion_19_JumpUpwards()
{
    const PSX_RECT bRect = VGetBoundingRect();

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
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
        if (mHungry && mTargetId == sActiveHero->mBaseGameObjectId && sActiveHero->GetScale() == GetScale() && (sActiveHero->mCurrentMotion == eAbeMotions::Motion_104_RockThrowStandingHold || sActiveHero->mCurrentMotion == eAbeMotions::Motion_107_RockThrowCrouchingHold))
        {
            Sfx(SlogSound::HungryYip_13);
        }
        else
        {
            Sfx(SlogSound::JumpBite_6);
        }
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eSlogMotions::Motion_0_Idle);
        SetNextMotion(eSlogMotions::m1);
    }
}

void Slog::Motion_20_Eating()
{
    SND_SEQ_Stop(SeqId::Empty_13);
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        GetAnimation().mFlags.Clear(AnimFlags::eLoopBackwards);
        if (GetNextMotion() != eSlogMotions::m1 && GetNextMotion() != eSlogMotions::Motion_20_Eating)
        {
            SetCurrentMotion(eSlogMotions::Motion_0_Idle);
            return;
        }
    }

    if (GetAnimation().GetCurrentFrame() == 3 && !GetAnimation().mFlags.Get(AnimFlags::eLoopBackwards))
    {
        SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Eating1, relive::SoundEffects::Eating2), 100);
        relive_new Blood(((GetAnimation().mFlags.Get(AnimFlags::eFlipX)) != 0 ? FP_FromInteger(-25) : FP_FromInteger(25)) * GetSpriteScale() + mXPos,
                      mYPos - (FP_FromInteger(4) * GetSpriteScale()),
                      FP_FromInteger(0), FP_FromInteger(0),
                      GetSpriteScale(), 12);
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (Math_RandomRange(0, 100) < 85)
        {
            if (static_cast<s32>(sGnFrame) > mGrowlTimer && Math_RandomRange(0, 100) < 60)
            {
                mGrowlTimer = sGnFrame + 16;
                Sfx(SlogSound::IdleGrrr_3);
            }
            GetAnimation().mFlags.Set(AnimFlags::eLoopBackwards);
        }
    }

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        GetAnimation().mFlags.Clear(AnimFlags::eLoopBackwards);
    }
}

void Slog::Motion_21_Dying()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_4_Fall();
        SetCurrentMotion(eSlogMotions::Motion_21_Dying);
    }
}

void Slog::Motion_22_Scratch()
{
    if (GetAnimation().GetCurrentFrame() == 4)
    {
        SND_SEQ_PlaySeq(SeqId::Scratching_12, 1, 1);
    }

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
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
            GetAnimation().mFlags.Clear(AnimFlags::eAnimate);
            mHasWoofed = 0;
            mGrowlTimer = sGnFrame + 12;
        }
    }

    if (static_cast<s32>(sGnFrame) > mGrowlTimer)
    {
        GetAnimation().mFlags.Set(AnimFlags::eAnimate);
    }

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
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

s16 Slog::Brain_0_ListeningToSlig()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mListeningToSligId));

    // TODO: OG bug - return never used?
    //sObjectIds.Find_449CF0(field_118);

    if (!pObj || pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        mAngerLevel = 0;
        mListeningToSligId = Guid{};
        mTargetId = Guid{};
        mBrainState = 1;
        return 0;
    }

    FP gridSize1Directed = ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(1);
    if (pObj->GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        gridSize1Directed = -gridSize1Directed;
    }

    const FP xpos1GridAHead = gridSize1Directed + pObj->mXPos;
    switch (mBrainSubState)
    {
        case 0:
            return Brain_ListeningToSligSaveState_0_Init();
        case 1:
            return Brain_ListeningToSligSaveState_1_Idle(xpos1GridAHead);
        case 2:
            return Brain_ListeningToSligSaveState_2_Listening(xpos1GridAHead, pObj);
        case 3:
            return Brain_ListeningToSligSaveState_3_Walking(xpos1GridAHead);
        case 4:
            return Brain_ListeningToSligSaveState_4_Running(xpos1GridAHead);
        case 5:
            return Brain_ListeningToSligSaveState_5_Waiting();
        case 6:
            return Brain_ListeningToSligSaveState_6_Responding();
        default:
            return mBrainSubState;
    }
}

s16 Slog::Brain_ListeningToSligSaveState_6_Responding()
{
    if (static_cast<s32>(sGnFrame) <= mMultiUseTimer)
    {
        return mBrainSubState;
    }

    mMultiUseTimer = sGnFrame + 10;
    SetNextMotion(sSlogResponseMotion[mResponseIdx][mResponsePart++]);

    if (GetNextMotion() == eSlogMotions::m2)
    {
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return 2;
    }
    else
    {
        return mBrainSubState;
    }
}

s16 Slog::Brain_ListeningToSligSaveState_5_Waiting()
{
    if (static_cast<s32>(sGnFrame) <= mMultiUseTimer)
    {
        return mBrainSubState;
    }
    mWaitingCounter--;
    SetNextMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
    return 2;
}

s16 Slog::Brain_ListeningToSligSaveState_4_Running(const FP xpos1GridAHead)
{
    if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
    {
        return 2;
    }

    if (GetCurrentMotion() != eSlogMotions::Motion_2_Run)
    {
        return mBrainSubState;
    }

    if (Facing(xpos1GridAHead))
    {
        if (FP_Abs(xpos1GridAHead - mXPos) > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3)))
        {
            return mBrainSubState;
        }
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return 2;
    }
    else
    {
        SetNextMotion(eSlogMotions::Motion_7_SlideTurn);
        return mBrainSubState;
    }
}

s16 Slog::Brain_ListeningToSligSaveState_3_Walking(const FP xpos1GridAHead)
{
    if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
    {
        return 2;
    }

    if (GetCurrentMotion() != eSlogMotions::Motion_1_Walk)
    {
        return mBrainSubState;
    }

    if (!Facing(xpos1GridAHead))
    {
        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
        return 2;
    }

    if (FP_Abs(xpos1GridAHead - mXPos) > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4)))
    {
        SetNextMotion(eSlogMotions::Motion_2_Run);
        return 4;
    }

    if (FP_Abs(xpos1GridAHead - mXPos) > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3)))
    {
        return mBrainSubState;
    }

    SetNextMotion(eSlogMotions::Motion_0_Idle);
    return 2;
}

s16 Slog::Brain_ListeningToSligSaveState_2_Listening(const FP xpos1GridAHead, IBaseAliveGameObject* pObj)
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }

    GameSpeakEvents speakValue = GameSpeakEvents::eNone_m1;
    if (field_134_last_event_index == gEventSystem->field_28_last_event_index)
    {
        if (gEventSystem->field_20_last_event == GameSpeakEvents::eNone_m1)
        {
            speakValue = GameSpeakEvents::eNone_m1;
        }
        else
        {
            speakValue = GameSpeakEvents::eSameAsLast_m2;
        }
    }
    else
    {
        field_134_last_event_index = gEventSystem->field_28_last_event_index;
        speakValue = gEventSystem->field_20_last_event;
    }

    switch (speakValue)
    {
        case GameSpeakEvents::Slig_LookOut_6:
            DelayedResponse(2);
            return 6;

        case GameSpeakEvents::Slig_Hi_27:
            mWaitingCounter++;
            if (static_cast<s32>(sGnFrame) % 2)
            {
                mWaitingCounter++;
            }
            [[fallthrough]];

        case GameSpeakEvents::Slig_HereBoy_28:
            mMultiUseTimer = sGnFrame - Math_NextRandom() % 8 + 15;
            mWaitingCounter++;
            break;

        case GameSpeakEvents::Slig_GetEm_29:
        {
            auto pTarget = FindTarget(1, 0);
            if (pTarget)
            {
                mListeningToSligId = Guid{};
                mCommandedToAttack = true;
                mTargetId = pTarget->mBaseGameObjectId;
                mBrainState = 2;
                return 0;
            }

            FP gridSize = {};
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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

        case GameSpeakEvents::eUnknown_33:
            DelayedResponse(0);
            return 6;

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
            SetNextMotion(eSlogMotions::Motion_3_TurnAround);
            return mBrainSubState;
        }

        FP gridSize2 = {};
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            gridSize2 = -ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            gridSize2 = ScaleToGridSize(GetSpriteScale());
        }

        if (!CollisionCheck(GetSpriteScale() * FP_FromInteger(20), gridSize2 * FP_FromInteger(2)))
        {
            SetNextMotion(eSlogMotions::Motion_2_Run);
            return 4;
        }
    }

    if (FP_Abs(xpos1GridAHead - mXPos) > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(1)))
    {
        if (!Facing(xpos1GridAHead))
        {
            SetNextMotion(eSlogMotions::Motion_3_TurnAround);
            return mBrainSubState;
        }

        FP gridSize3 = {};
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            gridSize3 = -ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            gridSize3 = ScaleToGridSize(GetSpriteScale());
        }

        if (!CollisionCheck(GetSpriteScale() * FP_FromInteger(20), gridSize3 * FP_FromInteger(2)))
        {
            SetNextMotion(eSlogMotions::Motion_1_Walk);
            return 3;
        }
    }

    if (pObj->GetAnimation().mFlags.Get(AnimFlags::eFlipX) != GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
    }

    return mBrainSubState;
}

s16 Slog::Brain_ListeningToSligSaveState_1_Idle(const FP xpos1GridAHead)
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }

    if (!Facing(xpos1GridAHead))
    {
        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
        return mBrainSubState;
    }

    if (static_cast<s32>(sGnFrame) <= mMultiUseTimer)
    {
        return mBrainSubState;
    }
    SetNextMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
    return 2;
}

s16 Slog::Brain_ListeningToSligSaveState_0_Init()
{
    SetNextMotion(eSlogMotions::Motion_0_Idle);
    mWaitingCounter = 0;
    mMultiUseTimer = sGnFrame + 15;
    return 1;
}

s16 Slog::Brain_1_Idle()
{
    BaseGameObject* pTarget = sObjectIds.Find_Impl(mTargetId);

    // OG dead code - return never used
    //sObjectIds.Find_449CF0(field_138_bottom_right);

    if (pTarget && pTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        mTargetId = Guid{};
    }

    if (field_134_last_event_index != gEventSystem->field_28_last_event_index)
    {
        field_134_last_event_index = gEventSystem->field_28_last_event_index;
        if (gEventSystem->field_20_last_event == GameSpeakEvents::Slig_HereBoy_28 && sControlledCharacter->Type() == ReliveTypes::eSlig)
        {
            mBrainState = 0;
            mTargetId = Guid{};
            mListeningToSligId = sControlledCharacter->mBaseGameObjectId;
            return 0;
        }
    }

    if (SwitchStates_Get(mAngerSwitchId))
    {
        mCommandedToAttack = false;
        mBrainState = 2;
        return 0;
    }

    switch (mBrainSubState)
    {
        case 0:
            if (GetCurrentMotion() != eSlogMotions::Motion_15_Sleeping && GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
            {
                SetNextMotion(eSlogMotions::Motion_0_Idle);
                return mBrainSubState;
            }

            if (eBit7_Asleep)
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

            SetNextMotion(eSlogMotions::Motion_17_WakeUp);
            return 2;

        case 2:
            if (GetCurrentMotion() != eSlogMotions::Motion_17_WakeUp)
            {
                return mBrainSubState;
            }

            SetNextMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
            mScratchTimer = Math_NextRandom() % 32 + sGnFrame + 120;
            mGrowlTimer = Math_NextRandom() % 32 + sGnFrame + 60;
            return 4;

        case 3:
            if (GetCurrentMotion() != eSlogMotions::Motion_16_MoveHeadDownwards)
            {
                if (GetNextMotion() != eSlogMotions::Motion_16_MoveHeadDownwards)
                {
                    SetNextMotion(eSlogMotions::Motion_16_MoveHeadDownwards);
                }

                return mBrainSubState;
            }

            SetNextMotion(eSlogMotions::Motion_15_Sleeping);
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
                    if (eBit7_Asleep)
                    {
                        mAngerLevel--;
                    }
                }
            }

            if (PlayerOrNakedSligNear())
            {
                mAngerLevel += 2;
            }

            if (!(Slog_NextRandom() % 64) && GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
            {
                SetCurrentMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
                return mBrainSubState;
            }

            if (static_cast<s32>(sGnFrame) > mGrowlTimer && GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
            {
                mGrowlTimer = Math_NextRandom() % 32 + sGnFrame + 60;
                SetCurrentMotion(eSlogMotions::Motion_23_Growl);
                SetNextMotion(eSlogMotions::Motion_0_Idle);
                sGnFrame = sGnFrame; // TODO: rev bug? check this in IDA
            }

            if (static_cast<s32>(sGnFrame) > mScratchTimer && GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
            {
                mScratchTimer = Math_NextRandom() % 32 + sGnFrame + 120;
                SetCurrentMotion(eSlogMotions::Motion_22_Scratch);
                SetNextMotion(eSlogMotions::Motion_0_Idle);
            }

            if (mAngerLevel > field_146_total_anger)
            {
                SetNextMotion(eSlogMotions::Motion_14_AngryBark);
                mAngerLevel = mAngerLevel + Slog_NextRandom() % 8;
                return 5;
            }

            if (mAngerLevel >= mWakeUpAnger)
            {
                return mBrainSubState;
            }

            SetNextMotion(eSlogMotions::Motion_16_MoveHeadDownwards);
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

            if (mAngerLevel >= field_146_total_anger)
            {
                if (mAngerLevel <= field_148_chase_anger)
                {
                    return mBrainSubState;
                }
                else
                {
                    mCommandedToAttack = false;
                    mBrainState = 2;
                    return 0;
                }
            }
            else
            {
                SetCurrentMotion(eSlogMotions::Motion_0_Idle);
                mScratchTimer = Math_NextRandom() % 32 + sGnFrame + 120;
                mGrowlTimer = Math_NextRandom() % 32 + sGnFrame + 60;
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
        if (field_134_last_event_index != gEventSystem->field_28_last_event_index)
        {
            field_134_last_event_index = gEventSystem->field_28_last_event_index;
            if (gEventSystem->field_20_last_event == GameSpeakEvents::Slig_HereBoy_28 && sControlledCharacter->Type() == ReliveTypes::eSlig)
            {
                mBrainState = 0;
                mTargetId = Guid{};
                mListeningToSligId = sControlledCharacter->mBaseGameObjectId;
                return 0;
            }
        }
    }

    bool updateTarget = false;
    if (!pTarget)
    {
        if (mTargetId != Guid{})
        {
            mTargetId = Guid{};
            mAngerLevel = 0;
            mBrainState = 1;
            SetNextMotion(eSlogMotions::Motion_0_Idle);
            return 0;
        }
        updateTarget = true;
    }

    if (updateTarget || !mCommandedToAttack || pTarget->GetSpriteScale() == FP_FromDouble(0.5))
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
                        mBrainState = 1;
                        SetNextMotion(eSlogMotions::Motion_0_Idle);
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

    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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
                SetNextMotion(eSlogMotions::Motion_18_JumpForwards);
                return 4;
            }
        }
    }

    if (!(Slog_NextRandom() % 64))
    {
        SetCurrentMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
        return mBrainSubState;
    }
    return Brain_ChasingAbe_State_20_Collided(pTarget);
}

s16 Slog::Brain_ChasingAbe_State_18_WaitingToJump(IBaseAliveGameObject* pTarget)
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }

    mJumpCounter -= 20;

    if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), pTarget))
    {
        return 15;
    }
    SetNextMotion(eSlogMotions::Motion_2_Run);
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_17_WaitingToChase(IBaseAliveGameObject* pTarget)
{
    if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
    {
        if (!VIsFacingMe(pTarget))
        {
            SetNextMotion(eSlogMotions::Motion_3_TurnAround);
            return mBrainSubState;
        }
        else
        {
            SetNextMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
        }
    }

    if (static_cast<s32>(sGnFrame) <= mMultiUseTimer)
    {
        return mBrainSubState;
    }

    if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), pTarget))
    {
        SetNextMotion(eSlogMotions::Motion_2_Run);
    }
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_16_JumpingUpwards()
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
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
        SetNextMotion(eSlogMotions::Motion_6_StopRunning);
        mStopRunning = mVelX < FP_FromInteger(0) ? true : false;
        mScratchTimer = Math_NextRandom() % 32 + sGnFrame + 120;
        mGrowlTimer = Math_NextRandom() % 32 + sGnFrame + 60;
        return 20;
    }

    if (!VIsFacingMe(pTarget) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
    {
        SetNextMotion(eSlogMotions::Motion_7_SlideTurn);
    }

    if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), pTarget))
    {
        if (pTarget->GetSpriteScale() == FP_FromInteger(1) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
        {
            if (VIsFacingMe(pTarget))
            {
                SetNextMotion(eSlogMotions::Motion_6_StopRunning);
            }
        }
    }

    if (GetCurrentMotion() == eSlogMotions::Motion_7_SlideTurn)
    {
        SetNextMotion(eSlogMotions::Motion_2_Run);
    }

    if (mYPos >= pTarget->mYPos + FP_FromInteger(50))
    {
        auto pBone = FindBone();
        if (pBone)
        {
            mBoneId = pBone->mBaseGameObjectId;
            return 11;
        }

        if (GetCurrentMotion() == eSlogMotions::Motion_4_Fall)
        {
            return 9;
        }

        if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
        {
            return mBrainSubState;
        }

        if (VIsFacingMe(pTarget))
        {
            if (mJumpCounter < 100 && VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), pTarget))
            {
                SetNextMotion(eSlogMotions::Motion_19_JumpUpwards);
                return 16;
            }

            if (Math_RandomRange(0, 100) < 20)
            {
                SetCurrentMotion(eSlogMotions::Motion_23_Growl);
            }

            SetNextMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
            return 18;
        }

        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
        return mBrainSubState;
    }

    mMultiUseTimer = sGnFrame + mChaseDelay;
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
            SetNextMotion(eSlogMotions::Motion_0_Idle);
            return 2;
        }

        if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
        {
            FP gridSize = {};
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                gridSize = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridSize = ScaleToGridSize(GetSpriteScale());
            }

            if (CollisionCheck(GetSpriteScale() * FP_FromInteger(20), gridSize * FP_FromInteger(2)))
            {
                SetNextMotion(eSlogMotions::Motion_3_TurnAround);
            }
            else
            {
                SetNextMotion(eSlogMotions::Motion_1_Walk);
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
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return 2;
    }

    if (FP_Abs(mYPos - pBone->mYPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
    {
        if (VIsFacingMe(pBone))
        {
            if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
            {
                SetNextMotion(eSlogMotions::Motion_20_Eating);
            }

            if (mMultiUseTimer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }

            pBone->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            SetNextMotion(eSlogMotions::Motion_0_Idle);
            mBoneId = Guid{};
            return 2;
        }

        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
        return 12;
    }

    mBoneId = Guid{};
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_12_WalkingToBone()
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle && GetCurrentMotion() != eSlogMotions::Motion_1_Walk)
    {
        return mBrainSubState;
    }

    auto pBone = static_cast<Bone*>(sObjectIds.Find_Impl(mBoneId));
    if (!pBone || pBone->VIsFalling())
    {
        mBoneId = Guid{};
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return 2;
    }

    if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
    {
        SetNextMotion(eSlogMotions::Motion_1_Walk);
    }

    if (FP_Abs(mYPos - pBone->mYPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
    {
        if (VIsFacingMe(pBone))
        {
            if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(1.5), pBone))
            {
                SetNextMotion(eSlogMotions::Motion_1_Walk);
            }

            if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(1.5), pBone) || pBone->mVelX > FP_FromInteger(0))
            {
                return mBrainSubState;
            }

            SetNextMotion(eSlogMotions::Motion_0_Idle);
            mMultiUseTimer = sGnFrame + mBoneEatingTime;
            return 13;
        }

        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
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
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return 2;
    }

    if (FP_Abs(mYPos - pBone->mYPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
    {
        if (!VIsFacingMe(pBone) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
        {
            SetNextMotion(eSlogMotions::Motion_7_SlideTurn);
        }

        if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4), pBone))
        {
            if (FP_Abs(mYPos - pBone->mYPos) < FP_FromInteger(50) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
            {
                SetNextMotion(eSlogMotions::Motion_6_StopRunning);
                return 12;
            }
        }

        if (GetCurrentMotion() == eSlogMotions::Motion_7_SlideTurn)
        {
            SetNextMotion(eSlogMotions::Motion_2_Run);
        }

        if (GetCurrentMotion() == eSlogMotions::Motion_1_Walk)
        {
            SetNextMotion(eSlogMotions::Motion_2_Run);
        }

        if (GetCurrentMotion() == eSlogMotions::Motion_4_Fall)
        {
            mBoneId = Guid{};
            return 9;
        }

        if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
        {
            return mBrainSubState;
        }

        if (VIsFacingMe(pBone))
        {
            FP gridSize = {};
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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
                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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
                    SetCurrentMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
                    return mBrainSubState;
                }

                SetNextMotion(eSlogMotions::Motion_1_Walk);
                return 12;
            }

            SetNextMotion(eSlogMotions::Motion_2_Run);
            return mBrainSubState;
        }

        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
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

    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
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
        mGrowlTimer = Math_NextRandom() % 32 + sGnFrame + 60;
        SetCurrentMotion(eSlogMotions::Motion_23_Growl);
        SetNextMotion(eSlogMotions::Motion_0_Idle);
    }

    if (static_cast<s32>(sGnFrame) <= mScratchTimer)
    {
        return mBrainSubState;
    }

    mScratchTimer = Math_NextRandom() % 32 + sGnFrame + 120;
    SetCurrentMotion(eSlogMotions::Motion_22_Scratch);
    SetNextMotion(eSlogMotions::Motion_0_Idle);
    return mBrainSubState;
}

s16 Slog::Brain_ChasingAbe_State_10_HungryForBone()
{
    if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
    {
        SetNextMotion(eSlogMotions::Motion_19_JumpUpwards);
    }

    if (static_cast<s32>(sGnFrame) <= mMultiUseTimer)
    {
        auto pBone = FindBone();
        if (pBone)
        {
            SetNextMotion(eSlogMotions::Motion_2_Run);
            mBoneId = pBone->mBaseGameObjectId;
            return 11;
        }

        if (GetCurrentMotion() == eSlogMotions::Motion_6_StopRunning)
        {
            return mBrainSubState;
        }

        if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
        {
            return mBrainSubState;
        }

        if (Slog_NextRandom() % 16)
        {
            return mBrainSubState;
        }

        SetNextMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
        return mBrainSubState;
    }

    mHungry = false;
    SetNextMotion(eSlogMotions::Motion_2_Run);
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_9_Falling()
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }
    SetCurrentMotion(eSlogMotions::Motion_2_Run);
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_8_ToIdle()
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }
    mAngerLevel = 0;
    mBrainState = 1;
    mBitingTarget = 0;
    return 0;
}

s16 Slog::Brain_ChasingAbe_State_7_EatingTarget(IBaseAliveGameObject* pTarget)
{
    if (static_cast<s32>(sGnFrame) <= mMultiUseTimer && pTarget->GetAnimation().mFlags.Get(AnimFlags::eRender))
    {
        if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
        {
            return mBrainSubState;
        }

        SetCurrentMotion(eSlogMotions::Motion_20_Eating);
        return mBrainSubState;
    }

    SetNextMotion(eSlogMotions::Motion_0_Idle);
    return 8;
}

s16 Slog::Brain_ChasingAbe_State_4_LungingAtTarget(IBaseAliveGameObject* pTarget)
{
    if (GetCurrentMotion() == eSlogMotions::Motion_2_Run)
    {
        if (pTarget->mHealth > FP_FromInteger(0))
        {
            mMultiUseTimer = Math_RandomRange(1, 3) + sGnFrame + mChaseDelay;
            return 1;
        }

        if (FP_Abs(pTarget->mXPos - mXPos) > ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2))
        {
            mMultiUseTimer = Math_RandomRange(1, 3) + sGnFrame + mChaseDelay;
            return 1;
        }

        if (FP_Abs(pTarget->mYPos - mYPos) > ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2))
        {
            mMultiUseTimer = Math_RandomRange(1, 3) + sGnFrame + mChaseDelay;
            return 1;
        }

        if (VIsFacingMe(pTarget))
        {
            SetCurrentMotion(eSlogMotions::Motion_20_Eating);
            SetNextMotion(eSlogMotions::m1);
        }
        else
        {
            SetCurrentMotion(eSlogMotions::Motion_3_TurnAround);
            SetNextMotion(eSlogMotions::Motion_20_Eating);
        }

        mMultiUseTimer = sGnFrame + 90;
        return 7;
    }

    if (GetCurrentMotion() != eSlogMotions::Motion_4_Fall)
    {
        return mBrainSubState;
    }

    return 9;
}

s16 Slog::Brain_ChasingAbe_State_3_GrowlOrScratch(IBaseAliveGameObject* pTarget)
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return mBrainSubState;
    }

    if (Slog_NextRandom() % 64)
    {
        if (static_cast<s32>(sGnFrame) > mGrowlTimer)
        {
            mGrowlTimer = Math_NextRandom() % 32 + sGnFrame + 60;
            SetCurrentMotion(eSlogMotions::Motion_23_Growl);
            SetNextMotion(eSlogMotions::Motion_0_Idle);
        }

        if (static_cast<s32>(sGnFrame) > mScratchTimer)
        {
            mScratchTimer = Math_NextRandom() % 32 + sGnFrame + 120;
            SetCurrentMotion(eSlogMotions::Motion_22_Scratch);
            SetNextMotion(eSlogMotions::Motion_0_Idle);
        }

        if (pTarget->GetSpriteScale() != FP_FromInteger(1))
        {
            return mBrainSubState;
        }
        return 2;
    }

    SetCurrentMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
    return mBrainSubState;
}

s16 Slog::Brain_ChasingAbe_State_2_Thinking(IBaseAliveGameObject* pTarget)
{
    if (mVelX > FP_FromInteger(0) && HandleEnemyStopper())
    {
        SetNextMotion(eSlogMotions::Motion_6_StopRunning);
        mStopRunning = mVelX < FP_FromInteger(0) ? true : false;
        mScratchTimer = Math_NextRandom() % 32 + sGnFrame + 120;
        mGrowlTimer = Math_NextRandom() % 32 + sGnFrame + 60;
        return 20;
    }

    if (CollisionCheck(GetSpriteScale() * FP_FromInteger(20), mVelX * FP_FromInteger(4)))
    {
        SetNextMotion(eSlogMotions::Motion_6_StopRunning);
        mStopRunning = mVelX < FP_FromInteger(0) ? true : false;
        mScratchTimer = Math_NextRandom() % 32 + sGnFrame + 120;
        mGrowlTimer = Math_NextRandom() % 32 + sGnFrame + 60;
        return 19;
    }

    if (!VIsFacingMe(pTarget) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
    {
        SetNextMotion(eSlogMotions::Motion_7_SlideTurn);
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
                        SetNextMotion(eSlogMotions::Motion_0_Idle);
                    }
                    else
                    {
                        SetNextMotion(eSlogMotions::Motion_18_JumpForwards);
                    }
                }
            }
        }
    }

    if (GetCurrentMotion() == eSlogMotions::Motion_7_SlideTurn)
    {
        SetNextMotion(eSlogMotions::Motion_2_Run);
    }

    auto pBone = FindBone();
    if (pBone)
    {
        mBoneId = pBone->mBaseGameObjectId;
        return 11;
    }

    if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
    {
        if (VIsFacingMe(pTarget))
        {
            FP gridSizeDirected = {};
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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
                    mMultiUseTimer = Math_RandomRange(1, 3) + sGnFrame + mChaseDelay;
                    return 1;
                }

                if (FP_Abs(pTarget->mXPos - mXPos) > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2)))
                {
                    mMultiUseTimer = Math_RandomRange(1, 3) + sGnFrame + mChaseDelay;
                    return 1;
                }

                if (FP_Abs(pTarget->mYPos - mYPos) > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2)))
                {
                    mMultiUseTimer = Math_RandomRange(1, 3) + sGnFrame + mChaseDelay;
                    return 1;
                }
                mMultiUseTimer = sGnFrame + 90;
                return 7;
            }

            mStopRunning = GetAnimation().mFlags.Get(AnimFlags::eFlipX);

            mScratchTimer = Math_NextRandom() % 32 + sGnFrame + 120;
            mGrowlTimer = Math_NextRandom() % 32 + sGnFrame + 60;
            return 19;
        }
        SetCurrentMotion(eSlogMotions::Motion_3_TurnAround);
    }

    if (mHungry && IsActiveHero(pTarget) && pTarget->GetScale() == GetScale() && (sActiveHero->mCurrentMotion == eAbeMotions::Motion_104_RockThrowStandingHold || sActiveHero->mCurrentMotion == eAbeMotions::Motion_107_RockThrowCrouchingHold))
    {
        SetNextMotion(eSlogMotions::Motion_6_StopRunning);
        mMultiUseTimer = sGnFrame + 90;
        return 10;
    }

    if (GetCurrentMotion() == eSlogMotions::Motion_4_Fall)
    {
        return 9;
    }

    if (GetCurrentMotion() == eSlogMotions::Motion_18_JumpForwards)
    {
        return 4;
    }

    if (mYPos <= pTarget->mYPos + FP_FromInteger(50))
    {
        if (pTarget->GetSpriteScale() != FP_FromDouble(0.5))
        {
            return mBrainSubState;
        }
        mGrowlTimer = Math_NextRandom() % 32 + sGnFrame + 60;
        mScratchTimer = Math_NextRandom() % 32 + sGnFrame + 120;
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
    SetNextMotion(eSlogMotions::Motion_2_Run);
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_0_Init()
{
    mBitingTarget = 0;
    mJumpCounter = 0;
    mBoneId = Guid{};
    mMultiUseTimer = Math_RandomRange(1, 3) + sGnFrame + mChaseDelay;
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
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 100;
}

u8** Slog::ResBlockForMotion(s16 motion)
{
    const auto slogMotion = static_cast<eSlogMotions>(motion);
    if (slogMotion < eSlogMotions::Motion_14_AngryBark)
    {
        field_130_motion_resource_block_index = 0;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }

    if (slogMotion < eSlogMotions::Motion_18_JumpForwards)
    {
        field_130_motion_resource_block_index = 1;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }

    if (slogMotion < eSlogMotions::Motion_21_Dying)
    {
        field_130_motion_resource_block_index = 2;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }

    if (slogMotion < eSlogMotions::Motion_22_Scratch)
    {
        field_130_motion_resource_block_index = 3;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }

    if (motion < 24) // last + 1
    {
        field_130_motion_resource_block_index = 4;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }
    else
    {
        field_130_motion_resource_block_index = 0;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }
}

void Slog::SetAnimFrame()
{
    GetAnimation().Set_Animation_Data(GetAnimRes(sSlogAnimIdTable[mCurrentMotion]));
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

    mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanSetOffExplosives);

    mShot = false;
    mHitByAbilityRing = false;
    mHungry = true;

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
    GetAnimation().SetFnPtrArray(kSlog_Anim_Frame_Fns_55EFBC);
    mMultiUseTimer = 0;
    mBrainSubState = 0;
    SetNextMotion(eSlogMotions::m1);
    field_130_motion_resource_block_index = 0;
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
    if (sCollisions->Raycast(
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
    if (mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eRestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eRestoredFromQuickSave);
        if (BaseAliveGameObjectCollisionLineType == -1)
        {
            BaseAliveGameObjectCollisionLine = nullptr;
        }
        else
        {
            sCollisions->Raycast(
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
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (FP_Abs(mXPos - sControlledCharacter->mXPos) > FP_FromInteger(750) || FP_Abs(mYPos - sControlledCharacter->mYPos) > FP_FromInteger(390))
    {
        GetAnimation().mFlags.Clear(AnimFlags::eAnimate);
        GetAnimation().mFlags.Clear(AnimFlags::eRender);
    }
    else
    {
        if (mHealth > FP_FromInteger(0))
        {
            GetAnimation().mFlags.Set(AnimFlags::eAnimate);
            GetAnimation().mFlags.Set(AnimFlags::eRender);
        }

        const auto oldMotion = mCurrentMotion;
        mBrainSubState = InvokeMemberFunction(this, sSlogBrainTable, mBrainState);
        if (sDDCheat_ShowAI_Info)
        {
            DDCheat::DebugStr("Slog:  Motion=%d  BrainState=%d\n", mCurrentMotion, mBrainSubState);
        }

        const FP oldXPos = mXPos;
        const FP oldYPos = mYPos;

        InvokeMemberFunction(this, sSlogMotionTable, mCurrentMotion);

        if (oldXPos != mXPos || oldYPos != mYPos)
        {
            BaseAliveGameObjectPathTLV = sPathInfo->TlvGetAt(
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

    if (mTlvId != Guid{})
    {
        Path::TLV_Reset(mTlvId, -1, 0, mHealth <= FP_FromInteger(0));
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
    SetCurrentMotion(eSlogMotions::Motion_0_Idle);
    SetNextMotion(eSlogMotions::m1);
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

    const s16 defaultSndIdxVol = effectDef.field_C_default_volume;
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
            effectDef.field_E_pitch_min + 1524,
            effectDef.field_10_pitch_max + 1524);
    }
    else
    {
        SFX_SfxDefinition_Play_Stereo(
            effectDef,
            volumeLeft,
            volumeRight,
            effectDef.field_E_pitch_min,
            effectDef.field_10_pitch_max);
    }
}

void Slog::ToJump()
{
    mVelX = (GetAnimation().mFlags.Get(AnimFlags::eFlipX) ? FP_FromDouble(-10.18) : FP_FromDouble(10.18)) * GetSpriteScale();
    mVelY = FP_FromInteger(-8) * GetSpriteScale();

    BaseAliveGameObjectLastLineYPos = mYPos;

    VOnTrapDoorOpen();

    SetCurrentMotion(eSlogMotions::Motion_18_JumpForwards);
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
            SetCurrentMotion(eSlogMotions::Motion_3_TurnAround);
            SetNextMotion(eSlogMotions::m1);
            return 1;

        case eSlogMotions::Motion_1_Walk:
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
            }

            if (!CollisionCheck(GetSpriteScale() * FP_FromInteger(20), (mVelX * FP_FromInteger(9)) * FP_FromInteger(2)))
            {
                SetCurrentMotion(eSlogMotions::Motion_8_StartWalking);
                SetNextMotion(eSlogMotions::m1);
                return 1;
            }
            ToIdle();
            return 0;

        case eSlogMotions::Motion_2_Run:
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(3));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(3));
            }

            if (!CollisionCheck(GetSpriteScale() * FP_FromInteger(20), mVelX * FP_FromInteger(4)))
            {
                SetCurrentMotion(eSlogMotions::Motion_2_Run);
                SetNextMotion(eSlogMotions::m1);
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
    return sCollisions->Raycast(mXPos, mYPos - hitY, mXPos + hitX, mYPos - hitY, &pLine, &hitX, &hitY, CollisionMask(eWallRight_2, eWallLeft_1)) != 0;
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
        SetCurrentMotion(eSlogMotions::Motion_4_Fall);
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

    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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
        SetCurrentMotion(eSlogMotions::Motion_4_Fall);
    }
}

void Slog::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            mHealth = FP_FromInteger(0);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
        pTlv = sPathInfo->TlvGetAt(pTlv, mXPos, mYPos, mXPos, mYPos);
    }
}

s16 Slog::VTakeDamage(BaseGameObject* pFrom)
{
    if (mHealth <= FP_FromInteger(0))
    {
        return 0;
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
                    if (pBullet->mXDistance <= FP_FromInteger(0))
                    {
                        relive_new Blood(mXPos,
                                                    pBullet->mYPos,
                                                    FP_FromInteger(-24),
                                                    FP_FromInteger(0),
                                                    GetSpriteScale(), 50);
                    }
                    else
                    {
                        relive_new Blood(mXPos,
                                                    pBullet->mYPos,
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
            mBrainState = 3;
            SetCurrentMotion(eSlogMotions::Motion_21_Dying);
            mMultiUseTimer = sGnFrame + 90;
            SetAnimFrame();
            GetAnimation().mFlags.Set(AnimFlags::eAnimate);
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
            relive_new Gibs(GibType::Slog_2, mXPos, mYPos, mVelX, mVelY, GetSpriteScale(), 0);

            const PSX_RECT bRect = VGetBoundingRect();
            relive_new Blood(FP_FromInteger((bRect.x + bRect.w) / 2),
                                        FP_FromInteger((bRect.y + bRect.h) / 2),
                                        FP_FromInteger(0),
                                        FP_FromInteger(0),
                                        GetSpriteScale(), 50);

            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            break;
        }

        case ReliveTypes::eElectricWall:
            Sfx(SlogSound::DeathWhine_9);
            break;

        case ReliveTypes::eRockSpawner:
        case ReliveTypes::eMineCar:
            Sfx(SlogSound::DeathWhine_9);
            mHealth = FP_FromInteger(0);
            mBrainState = 3;
            SetCurrentMotion(eSlogMotions::Motion_21_Dying);
            mMultiUseTimer = sGnFrame + 90;
            SetAnimFrame();
            GetAnimation().mFlags.Set(AnimFlags::eAnimate);
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
            mBrainState = 3;
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            break;

        default:
            return 1;
    }
    return 1;
}

void Slog::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    mAngerLevel += field_148_chase_anger; // on throwable hit?
}

s16 Slog::PlayerOrNakedSligNear()
{
    const FP kMinXDist = FP_FromInteger(100) * GetSpriteScale();
    const FP kMinYDist = FP_FromInteger(25) * GetSpriteScale();

    // Is the player near?
    if (FP_Abs(sActiveHero->mXPos - mXPos) < kMinXDist && FP_Abs(sActiveHero->mYPos - mYPos) < kMinYDist && sActiveHero->GetSpriteScale() == FP_FromInteger(1))
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
    mMultiUseTimer = sGnFrame + 10;
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
        sPathInfo->TLV_Get_At(
            FP_GetExponent(xToUse), FP_GetExponent(mYPos),
            FP_GetExponent(width), FP_GetExponent(mYPos), ReliveTypes::eEnemyStopper));

    return stopperPath != nullptr && stopperPath->mStopDirection == (mVelX > FP_FromInteger(0) ? relive::Path_EnemyStopper::StopDirection::Right : relive::Path_EnemyStopper::StopDirection::Left) && SwitchStates_Get(stopperPath->mSwitchId) > 0;
}

s16 Slog::Facing(FP xpos)
{
    if (mXPos < xpos && !GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        return true;
    }

    if (xpos < mXPos && GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        return true;
    }

    return false;
}
