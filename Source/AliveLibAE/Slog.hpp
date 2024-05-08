#pragma once

#include "BaseAliveGameObject.hpp"
#include "../relive_lib/SaveStateBase.hpp"
#include "../relive_lib/FatalError.hpp"
#include "GameSpeak.hpp"

class Bone;
enum eLineTypes : s16;

namespace relive
{
    struct Path_Slog;
}

enum class SlogSound : s32
{
    Padding_0 = 0,
    Padding_1 = 1,
    IdleWoof_2 = 2,
    IdleGrrr_3 = 3,
    IdleGrrAlt_4 = 4,
    CautiousWoof_5 = 5,
    JumpBite_6 = 6,
    Padding_7 = 7,
    AttackGrowl_8 = 8,
    DeathWhine_9 = 9,
    YawnStart_10 = 10,
    YawnEnd_11 = 11,
    Padding_12 = 12,
    HungryYip_13 = 13,
    Skid_14 = 14,
    Padding_15 = 15,
    Landing_16 = 16,
    FastStep_17 = 17,
    SlowStep_18 = 18
};

enum class eSlogMotions
{
    m2 = -2,
    m1 = -1,
    Motion_0_Idle,
    Motion_1_Walk,
    Motion_2_Run,
    Motion_3_TurnAround,
    Motion_4_Fall,
    Motion_5_MoveHeadUpwards,
    Motion_6_StopRunning,
    Motion_7_SlideTurn,
    Motion_8_StartWalking,
    Motion_9_EndWalking,
    Motion_10_Land,
    Motion_11_Unused,
    Motion_12_StartFastBarking,
    Motion_13_EndFastBarking,
    Motion_14_AngryBark,
    Motion_15_Sleeping,
    Motion_16_MoveHeadDownwards,
    Motion_17_WakeUp,
    Motion_18_JumpForwards,
    Motion_19_JumpUpwards,
    Motion_20_Eating,
    Motion_21_Dying,
    Motion_22_Scratch,
    Motion_23_Growl
};

enum class eSlogBrains : s16
{
    Brain_0_ListeningToSlig,
    Brain_1_Idle,
    Brain_2_ChasingAbe,
    Brain_3_Death
};

struct SlogSaveState final : public SaveStateBase
{
    SlogSaveState()
        : SaveStateBase(ReliveTypes::eSlog, sizeof(*this))
    { }
    Guid mBaseTlvId;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    s16 mCurrentPath;
    EReliveLevelIds mCurrentLevel;
    FP mSpriteScale;
    s16 mR;
    s16 mG;
    s16 mB;
    s16 mFlipX;
    eSlogMotions mCurrentMotion;
    s16 mCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mRender;
    s8 mDrawable;
    FP mHealth;
    eSlogMotions mCurrentMotion2;
    eSlogMotions mNextMotion;
    s16 mLastLineYPos;
    eLineTypes mCollisionLineType;
    Guid mPlatformId;
    Guid mSlogTlvId;
    Guid mTargetId;
    eSlogBrains mBrainState;
    s16 mBrainSubState;
    s32 mMultiUseTimer;
    FP mFallingVelxScaleFactor;
    Guid mListeningToSligId;
    s16 mHasWoofed;
    s16 mWaitingCounter;
    s16 mResponseIdx;
    s16 mResponsePart;
    s16 mAngerLevel;
    s16 mJumpCounter;
    s32 mScratchTimer;
    s32 mGrowlTimer;
    Guid mBoneId;
    s16 mChaseDelay;
    u8 mSlogRandomIdx;
    bool mBitingTarget;
    bool eBit3_Asleep;
    bool mMovedOffScreen;
    bool mStopRunning;
    bool mShot;
    bool mHungry;
    bool mCommandedToAttack;
    bool mHitByAbilityRing;
    bool mListenToSligs;
};

class Slog;
using TSlogBrainFn = s16 (Slog::*)();
using TSlogMotionFn = void (Slog::*)();

class Slog final : public BaseAliveGameObject
{
public:
    static constexpr AnimId sSlogAnimIdTable[24] = {
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
        AnimId::Slog_Growl};

    Slog(FP xpos, FP ypos, FP scale, s16 bListenToSligs, s16 chaseDelay);
    Slog(relive::Path_Slog* pTlv, const Guid& tlvId);
    ~Slog();

    void LoadAnimations();
    virtual void VUpdate() override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;
    virtual s16 VGetMotion(eMotionType motionType) override
    {
        switch (motionType)
        {
            case eMotionType::ePreviousMotion:
                return 0;
            case eMotionType::eCurrentMotion:
                return static_cast<s16>(mCurrentMotion);
            case eMotionType::eNextMotion:
                return static_cast<s16>(mNextMotion);
            default:
                ALIVE_FATAL("Invalid motion type %d", static_cast<s32>(motionType));
        }
    }
    static void CreateFromSaveState(SerializedObjectData& pBuffer);

public:
    // Motions
    void Motion_0_Idle();
    void Motion_1_Walk();
    void Motion_2_Run();
    void Motion_3_TurnAround();
    void Motion_4_Fall();
    void Motion_5_MoveHeadUpwards();
    void Motion_6_StopRunning();
    void Motion_7_SlideTurn();
    void Motion_8_StartWalking();
    void Motion_9_EndWalking();
    void Motion_10_Land();
    void Motion_11_Unused();
    void Motion_12_StartFastBarking();
    void Motion_13_EndFastBarking();
    void Motion_14_AngryBark();
    void Motion_15_Sleeping();
    void Motion_16_MoveHeadDownwards();
    void Motion_17_WakeUp();
    void Motion_18_JumpForwards();
    void Motion_19_JumpUpwards();
    void Motion_20_Eating();
    void Motion_21_Dying();
    void Motion_22_Scratch();
    void Motion_23_Growl();

public:
    s16 Brain_0_ListeningToSlig();

    s16 Brain_ListeningToSlig_0_Init();
    s16 Brain_ListeningToSlig_1_Idle(const FP xpos1GridAHead);
    s16 Brain_ListeningToSlig_2_Listening(const FP xpos1GridAHead, IBaseAliveGameObject* pObj);
    s16 Brain_ListeningToSlig_3_Walking(const FP xpos1GridAHead);
    s16 Brain_ListeningToSlig_4_Running(const FP xpos1GridAHead);
    s16 Brain_ListeningToSlig_5_Waiting();
    s16 Brain_ListeningToSlig_6_Responding();

    s16 Brain_1_Idle();

    s16 Brain_2_ChasingAbe();
    s16 Brain_ChasingAbe_State_0_Init();
    s16 Brain_ChasingAbe_State_1_Waiting();
    s16 Brain_ChasingAbe_State_2_Thinking(IBaseAliveGameObject* pTarget);
    s16 Brain_ChasingAbe_State_3_GrowlOrScratch(IBaseAliveGameObject* pTarget);
    s16 Brain_ChasingAbe_State_4_LungingAtTarget(IBaseAliveGameObject* pTarget);


    s16 Brain_ChasingAbe_State_7_EatingTarget(IBaseAliveGameObject* pTarget);
    s16 Brain_ChasingAbe_State_8_ToIdle();
    s16 Brain_ChasingAbe_State_9_Falling();
    s16 Brain_ChasingAbe_State_10_HungryForBone();
    s16 Brain_ChasingAbe_State_11_ChasingAfterBone();
    s16 Brain_ChasingAbe_State_12_WalkingToBone();
    s16 Brain_ChasingAbe_State_13_EatingBone();
    s16 Brain_ChasingAbe_State_14_CheckingIfBoneNearby();
    s16 Brain_ChasingAbe_State_15_ChasingAfterTarget(IBaseAliveGameObject* pTarget);
    s16 Brain_ChasingAbe_State_16_JumpingUpwards();
    s16 Brain_ChasingAbe_State_17_WaitingToChase(IBaseAliveGameObject* pTarget);
    s16 Brain_ChasingAbe_State_18_WaitingToJump(IBaseAliveGameObject* pTarget);
    s16 Brain_ChasingAbe_State_19_AboutToCollide(IBaseAliveGameObject* pTarget);
    s16 Brain_ChasingAbe_State_20_Collided(IBaseAliveGameObject* pTarget);

    s16 Brain_3_Death();

private:
    void SetAnimFrame();
    void Init();

private:

    void ToIdle();
    void Sfx(SlogSound effectId);
    void ToJump();
    s16 ToNextMotion();
    bool CollisionCheck(FP hitY, FP hitX);
    void MoveOnLine();
    Bone* FindBone();
    IBaseAliveGameObject* FindTarget(s16 bKillSligs, s16 bLookingUp);
    s16 PlayerOrNakedSligNear();
    void DelayedResponse(s16 responseIdx);
    s16 HandleEnemyStopper();
    s16 Facing(FP xpos);

public:
    Guid mTargetId;
    bool mBitingTarget = false;

private:
    eSlogBrains mBrainState = eSlogBrains::Brain_0_ListeningToSlig;
    s16 mBrainSubState = 0;
    s32 mMultiUseTimer = 0; // this timer is used for multiple things like chase delay, bone eating time etc.
    FP mFallingVelxScaleFactor = {};
    Guid mTlvId;
    s16 mHasWoofed = 0;
    GameSpeakListener mListener;
    Guid mListeningToSligId;
    s16 mWaitingCounter = 0;
    s16 mResponseIdx = 0;
    s16 mResponsePart = 0;
    s16 mAngerLevel = 0;
    s16 mWakeUpAnger = 0;
    s16 mTotalAnger = 0;
    s16 mChaseAnger = 0;
    s32 mScratchTimer = 0;
    s32 mGrowlTimer = 0;
    s16 mAngerSwitchId = 0;
    s16 mBoneEatingTime = 0;
    s16 mChaseDelay = 0;
    s16 mJumpCounter = 0;
    Guid mBoneId;
    bool eBit8_Asleep = false; // TODO: wrong name this is something else
    bool mMovedOffScreen = false;
    bool mStopRunning = false;
    bool mShot = false;
    bool mHungry = false;
    bool mCommandedToAttack = false;
    bool mHitByAbilityRing = false;
    bool mListenToSligs = false;
    bool mAsleep = false;
    eSlogMotions mCurrentMotion = eSlogMotions::Motion_0_Idle;
    eSlogMotions mNextMotion = eSlogMotions::Motion_0_Idle;
};

extern s16 sSlogCount;
