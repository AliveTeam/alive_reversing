#pragma once

#include "BaseAliveGameObject.hpp"
#include "Sfx.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class CrawlingSlig;

using TCrawlingSligBrainFn = s16 (CrawlingSlig::*)();
using TCrawlingSligMotionFn = void (CrawlingSlig::*)();

enum class LevelIds : s16;
enum class SligSpeak : s8;
enum eLineTypes : s16;

enum class CrawlingSligMotion : s16
{
    None = -1,
    Motion_0_Idle = 0,
    Motion_1_UsingButton = 1,
    Motion_2_WakingUp = 2,
    Motion_3_Crawling = 3,
    Motion_4_StartFalling = 4,
    Motion_5_Falling = 5,
    Motion_6_Landing = 6,
    Motion_7_ToShakingToIdle = 7,
    Motion_8_Speaking = 8,
    Motion_9_Snoozing = 9,
    Motion_10_PushingWall = 10,
    Motion_11_TurnAround = 11,
    Motion_12_Shaking = 12,
    Motion_13_Empty = 13,
    Motion_14_ShakingToIdle = 14,
    Motion_15_EndCrawling = 15,
    Motion_16_IdleToPushingWall = 16,
    Motion_17_EndPushingWall = 17
};

class ISligBrain
{
public:
    enum class EBrainTypes
    {
        Sleeping,
        Idle,
        PanicGetALocker,
        Possessed,
        GetKilled,
        Transformed
    };
    virtual ~ISligBrain() { }
    virtual void VUpdate() = 0;
    virtual EBrainTypes GetBrain() = 0;
};

struct CrawlingSligSaveState final
{
    ReliveTypes mType;
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
    CrawlingSligMotion mCurrentMotion;
    s16 mCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mRender;
    s8 mDrawable;
    FP mHealth;
    CrawlingSligMotion mCurrentMotion2;
    CrawlingSligMotion mNextMotion;
    s16 mLastLineYPos;
    eLineTypes mCollisionLineType;
    s8 mControlled;
    Guid mCrawlingSligTlvId;
    ISligBrain::EBrainTypes mBrainType;
    s16 mBrainSubState;
    s32 mMultiUseTimer;
    FP mVelxScaleFactor;
    s16 mChanting;
    EReliveLevelIds mAbeLevel;
    s16 mAbePath;
    s16 mAbeCamera;
    Guid mSligButtonTlvId;
    Guid field_70_obj_id;
    Guid mTransformedSligId;
    SligSpeak mSpeak;
    s32 mSayHelpTimer;
};

class SleepingBrain final : public ISligBrain
{
public:
    explicit SleepingBrain(CrawlingSlig& slig) : mSlig(slig) {}

    void VUpdate() override;
    EBrainTypes GetBrain() override { return EBrainTypes::Sleeping; }
private:
    enum EState
    {
        eSleeping = 0,
        eWakingUp = 1,
        eIsAwake = 2
    };

    CrawlingSlig& mSlig;
    EState mBrainState = EState::eSleeping;
};

class IdleBrain final : public ISligBrain
{
public:
    explicit IdleBrain(CrawlingSlig& slig) : mSlig(slig) {}

    void VUpdate() override;
    EBrainTypes GetBrain() override { return EBrainTypes::Idle; }
private:
    CrawlingSlig& mSlig;
};

class PanicGetALockerBrain final : public ISligBrain
{
public:
    explicit PanicGetALockerBrain(CrawlingSlig& slig) : mSlig(slig) {}

    void VUpdate() override;
    EBrainTypes GetBrain() override { return EBrainTypes::PanicGetALocker; }
    enum EState
    {
        eDetermineCrawlDirection,
        eFalling,
        eSearchLocker,
        eTurnAroundForLocker,
        eSearchLockerOrTurnAround,
        eGetPantsOrWings,
        eUsingButton,
        eTurnAround,
        eCrawling,
        eCheckIfWallHit,
        eBeatBySlig
    };

    void SetState(EState state)
    {
        mBrainState = state;
    }

private:
    CrawlingSlig& mSlig;
    EState mBrainState = EState::eDetermineCrawlDirection;
};

class PossessedBrain final : public ISligBrain
{
public:
    explicit PossessedBrain(CrawlingSlig& slig) : mSlig(slig) {}

    void VUpdate() override;
    EBrainTypes GetBrain() override { return EBrainTypes::Possessed; }
    enum EState
    {
        eStartPossession,
        ePossessed,
        eUnpossessing,
        eBeatBySlig
    };

    void SetState(EState state)
    {
        mBrainState = state;
    }

    EState State()
    {
        return mBrainState;
    }

private:
    CrawlingSlig& mSlig;
    EState mBrainState = EState::eStartPossession;
};

class GetKilledBrain final : public ISligBrain
{
public:
    explicit GetKilledBrain(CrawlingSlig& slig) : mSlig(slig) {}

    void VUpdate() override;
    EBrainTypes GetBrain() override { return EBrainTypes::GetKilled; }
    enum EState
    {
        eUnknown,
        eVaporize,
        eGibsDeath,
        eSetDead,
        eDeathBySlog,
        eDeathDrop
    };

    void SetState(EState state)
    {
        mBrainState = state;
    }

private:
    CrawlingSlig& mSlig;
    EState mBrainState = EState::eUnknown;
};

class TransformedBrain final : public ISligBrain
{
public:
    explicit TransformedBrain(CrawlingSlig& slig) : mSlig(slig) {}

    void VUpdate() override;
    EBrainTypes GetBrain() override { return EBrainTypes::Transformed; }
private:
    CrawlingSlig& mSlig;
};

class CrawlingSlig final : public BaseAliveGameObject
{
public:
    CrawlingSlig(relive::Path_CrawlingSlig* pTlv, const Guid& guid);
    ~CrawlingSlig();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VPossessed() override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pBuffer);

    void Motion_0_Idle();
    void Motion_1_UsingButton();
    void Motion_2_WakingUp();
    void Motion_3_Crawling();
    void Motion_4_StartFalling();
    void Motion_5_Falling();
    void Motion_6_Landing();
    void Motion_7_ToShakingToIdle();
    void Motion_8_Speaking();
    void Motion_9_Snoozing();
    void Motion_10_PushingWall();
    void Motion_11_TurnAround();
    void Motion_12_Shaking();
    void Motion_13_Empty();
    void Motion_14_ShakingToIdle();
    void Motion_15_EndCrawling();
    void Motion_16_IdleToPushingWall();
    void Motion_17_EndPushingWall();
private:
    void LoadAnimations();

    void SetBrain(ISligBrain::EBrainTypes type);
    bool BrainIs(ISligBrain* brain);

    CrawlingSligMotion GetNextMotion() const
    {
        return static_cast<CrawlingSligMotion>(mNextMotion);
    }
    CrawlingSligMotion GetCurrentMotion() const
    {
        return static_cast<CrawlingSligMotion>(mCurrentMotion);
    }

    void Set_AnimAndMotion(CrawlingSligMotion currentMotion, s16 bClearNextMotion);
    void UpdateAnimBlock();
    u8** GetAnimBlock(s32 currentMotion);
    static s16 NextRandom();
    s16 HandleEnemyStopper(FP velX);
    relive::Path_TLV* FindPantsOrWings();
    BaseGameObject* FindSligButton();

    void SetBrain(TCrawlingSligBrainFn fn);
    bool BrainIs(TCrawlingSligBrainFn fn);

    bool PanicOn();
    void ToIdle();
    void HandleCommon();
    s16 CanCrawl();
    void MoveOnLine();
    void PlatformCollide();

private:
    std::shared_ptr<AnimationPal> field_11C_mPal;
    Guid mGuid;
    s16 field_1A4_r = 0;
    s16 field_1A6_g = 0;
    s16 field_1A8_b = 0;

    s32 mMultiUseTimer = 0;
    FP mVelxScaleFactor = {}; // TODO: Not sure if this is an accurate name, but can't think of anything better.

    bool mChanting = false;

    EReliveLevelIds mAbeLevel = EReliveLevelIds::eNone;
    s16 mAbePath = 0;
    s16 mAbeCamera = 0;

    SligSpeak mSpeak = SligSpeak::eNone;
    s32 mSayHelpTimer = 0;
    Guid mSligButtonId = Guid{};
    Guid field_1D4_obj_id = Guid{};
    Guid mTransformedSligId = Guid{};
    relive::Path_CrawlingSlig::CrawlDirection mCrawlDirection = relive::Path_CrawlingSlig::CrawlDirection::eLeft;
    relive::Path_TLV* mTlvHeader = nullptr;
    relive::Path_CrawlingSlig mTlv = {};
    TCrawlingSligBrainFn mBrainState = nullptr;
    s16 mBrainSubState = 0;

    friend class SleepingBrain;
    friend class IdleBrain;
    friend class PanicGetALockerBrain;
    friend class PossessedBrain;
    friend class GetKilledBrain;
    friend class TransformedBrain;
    SleepingBrain mSleepingBrain;
    IdleBrain mIdleBrain;
    PanicGetALockerBrain mPanicGetALockerBrain;
    PossessedBrain mPossessedBrain;
    GetKilledBrain mGetKilledBrain;
    TransformedBrain mTransformedBrain;
    ISligBrain* mCurrentBrain = nullptr;
};
