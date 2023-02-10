#pragma once

#include "BaseAliveGameObject.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

#define GLUKKON_MOTIONS_ENUM(ENTRY)        \
    ENTRY(Motion_0_Idle)                 \
    ENTRY(Motion_1_Walk)                 \
    ENTRY(Motion_2_Turn)                 \
    ENTRY(Motion_3_KnockBack)            \
    ENTRY(Motion_4_Jump)                 \
    ENTRY(Motion_5_JumpToFall)           \
    ENTRY(Motion_6_WalkToFall)           \
    ENTRY(Motion_7_Fall)                 \
    ENTRY(Motion_8_DeathFall)            \
    ENTRY(Motion_9_Land)                 \
    ENTRY(Motion_10_ChantShake)          \
    ENTRY(Motion_11_Speak1)              \
    ENTRY(Motion_12_Speak2)              \
    ENTRY(Motion_13_LongLaugh)           \
    ENTRY(Motion_14_BeginWalk)           \
    ENTRY(Motion_15_EndWalk)             \
    ENTRY(Motion_16_StandToJump)         \
    ENTRY(Motion_17_JumpToStand)         \
    ENTRY(Motion_18_WalkToJump)          \
    ENTRY(Motion_19_JumpToWalk)          \
    ENTRY(Motion_20_KnockBackStandBegin) \
    ENTRY(Motion_21_GetShot)             \
    ENTRY(Motion_22_KnockBackStandEnd)   \
    ENTRY(Motion_23_Speak3)              \
    ENTRY(Motion_24_EndSingleStep)

#define MAKE_ENUM(VAR) VAR,
enum class eGlukkonMotions : s16
{
    eNone_m1 = -1,
    GLUKKON_MOTIONS_ENUM(MAKE_ENUM)
};

enum class GlukkonSpeak : s8
{
    None = -1,
    Hey_0 = 0,
    DoIt_1 = 1,
    StayHere_2 = 2,
    Commere_3 = 3,
    AllOYa_4 = 4,
    Heh_5 = 5,
    Help_6 = 6,
    Laugh_7 = 7,
    KillEm_8 = 8,
    Unused_9 = 9, // no sound
    What_11 = 11
};

enum class LevelIds : s16;
enum eLineTypes : s16;

class Glukkon;
using TGlukkonBrainFn = s16 (Glukkon::*)();
using TGlukkonMotionFn = void (Glukkon::*)();

struct GlukkonSaveState final
{
    ReliveTypes mType;
    Guid field_4_object_id;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    s16 mCurrentPath;
    EReliveLevelIds mCurrentLevel;
    FP mSpriteScale;
    u16 mRed;
    u16 mGreen;
    u16 mBlue;
    u16 mFlipX;
    eGlukkonMotions mCurrentMotion;
    u16 mCurrentFrame;
    u16 mFrameChangeCounter;
    u8 mRender;
    u8 mDrawable;
    FP mHealth;
    eGlukkonMotions mCurrentMotion2;
    eGlukkonMotions mNextMotion;
    s16 field_38_last_line_ypos;
    eLineTypes mLineType;
    bool mIsActiveChar;
    Guid mTlvId;
    s32 mBrainStateIdx;
    s16 mBrainSubState;
    s32 field_54_timer;
    FP mFallingVelXScaleFactor;
    bool mPreventDepossession;
    EReliveLevelIds mAbeLevel;
    s16 mAbePath;
    s16 mAbeCamera;
    GlukkonSpeak mSpeak;
    s16 mGamespeakPitch;
    FP mPreviousYPos;
    s32 mRandomishSpeakTimer;
    s32 mTurnOrHelpTimer;
    s32 mPanicTimer;
    s16 field_7C;
    s32 mKnockbackDelayAfterGettingShotTimer;
    s32 mGettingShotTimer;
    Guid mFadeId;
    s16 mCanBePossessed;
    ReliveTypes mCurrentType; // defaults to none until the glukkon gets spawned
};

class Glukkon final : public BaseAliveGameObject
{
public:
    Glukkon(relive::Path_Glukkon* pTlv, const Guid& tlvId);
    ~Glukkon();

    void LoadAnimations();

    static s32 CreateFromSaveState(const u8* pBuffer);

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual void VScreenChanged() override;
    virtual void VPossessed() override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual void VOnTrapDoorOpen() override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;

public:
    void Motion_0_Idle();
    void Motion_1_Walk();
    void Motion_2_Turn();
    void Motion_3_KnockBack();
    void Motion_4_Jump();

    void JumpHelper();

    void Motion_5_JumpToFall();
    void Motion_6_WalkToFall();
    void Motion_7_Fall();
    void Motion_8_DeathFall();
    void Motion_9_Land();
    void Motion_10_ChantShake();
    void Motion_11_Speak1();
    void Motion_12_Speak2();
    void Motion_13_LongLaugh();
    void Motion_14_BeginWalk();
    void Motion_15_EndWalk();
    void Motion_16_StandToJump();
    void Motion_17_JumpToStand();
    void Motion_18_WalkToJump();
    void Motion_19_JumpToWalk();
    void Motion_20_KnockBackStandBegin();
    void Motion_21_GetShot();
    void Motion_22_KnockBackStandEnd();
    void Motion_23_Speak3();
    void Motion_24_EndSingleStep();

public:
    s16 Brain_0_Calm_WalkAround();
    s16 Brain_1_Panic();
    s16 Brain_2_Slapped();
    s16 Brain_3_PlayerControlled();
    s16 Brain_4_Death();
    s16 Brain_5_WaitToSpawn();

public:
    static void PlaySound_GameSpeak(GlukkonSpeak sndIdx, s16 volume, s16 pitch, Glukkon* pGlukkon);

private:
    eGlukkonMotions GetNextMotion() const
    {
        return static_cast<eGlukkonMotions>(mNextMotion);
    }

    eGlukkonMotions GetCurrentMotion() const
    {
        return static_cast<eGlukkonMotions>(mCurrentMotion);
    }

    void Init();
    void Update_Slurg_WatchPoints();
    void SetAnim(eGlukkonMotions currentMotion, s16 bClearNextMotion = false);
    void Speak(GlukkonSpeak speak);
    void HandleInput();
    s16 ShouldPanic(s16 panicEvenIfNotFacingMe);
    s16 PathBlocked(FP a2, s16 checkBounds);
    void SpeakRandomish();
    s16 DoMovement();
    void FollowLine();
    void GetOnPlatforms();
    static void PlaySound(s32 sndIdx, Glukkon* pGlukkon);
    void ToDead();
    static bool IsLineOfSightBetween(Glukkon* pGlukkon, IBaseAliveGameObject* pOther);
    void ToStand();
    void SlowDown(FP speed);
    void SetBrain(TGlukkonBrainFn fn);
    bool BrainIs(TGlukkonBrainFn fn);

private:
    std::shared_ptr<AnimationPal> mPal;
    s16 mRed = 0;
    s16 mGreen = 0;
    s16 mBlue = 0;
    relive::Path_Glukkon mTlvData;
    s32 field_1D4_timer = 0;
    FP mFallingVelXScaleFactor = {};
    FP mPreviousYPos = {};
    s16 mGamespeakPitch = 0;
    bool mPreventDepossession = false;
    EReliveLevelIds mAbeLevel = EReliveLevelIds::eNone;
    s16 mAbePath = 0;
    s16 mAbeCamera = 0;
    GlukkonSpeak mSpeak = GlukkonSpeak::Hey_0;
    s32 mRandomishSpeakTimer = 0;
    s32 mTurnOrHelpTimer = 0;
    s32 mPanicTimer = 0;
    s16 field_1FC = 0; // When set to true, broadcasts event kEventGlukkonUnknown.
    s32 mKnockbackDelayAfterGettingShotTimer = 0;
    s32 mGettingShotTimer = 0;
    Guid mFadeId;
    TGlukkonBrainFn mBrainState = nullptr;
    s16 mBrainSubState = 0;
    s16 mCurrentWalkPitch = 0;
    Guid mTlvId;
};
