#pragma once

#include "BaseAliveGameObject.hpp"
#include "Sfx.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class CrawlingSlig;

using TCrawlingSligBrainFn = s16 (CrawlingSlig::*)();
using TCrawlingSligMotionFn = void (CrawlingSlig::*)();

enum class LevelIds : s16;
enum class SligSpeak : s8;

struct CrawlingSligSaveState final
{
    ReliveTypes mType;
    s16 field_2_padding;
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
    s16 mCurrentMotion;
    s16 mCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mRender;
    s8 mDrawable;
    FP mHealth;
    s16 mCurrentMotion2;
    s16 mNextMotion;
    s16 mLastLineYPos;
    s16 mCollisionLineType;
    s16 field_3C_padding;
    s16 field_3E_padding;
    s8 mControlled;
    s8 field_41_padding;
    s16 field_42_padding;
    Guid mCrawlingSligTlvId;
    s32 mBrainState;
    s16 field_4C_padding;
    s16 field_4E_padding;
    s16 mBrainSubState;
    s16 field_52_padding;
    s32 mMultiUseTimer;
    FP mVelxScaleFactor;
    s16 field_5C_padding;
    s16 mChanting;
    EReliveLevelIds mAbeLevel;
    s16 mAbePath;
    s16 mAbeCamera;
    s16 field_66_unused;
    s32 field_68_unused;
    Guid mSligButtonTlvId;
    Guid field_70_obj_id;
    Guid mTransformedSligId;
    SligSpeak mSpeak;
    s8 field_79_padding;
    s16 field_7A_unused_counter;
    s32 mSayHelpTimer;
};

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
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pBuffer);

    s16 Brain_0_Sleeping();
    s16 Brain_1_Idle();
    s16 Brain_2_PanicGetALocker();
    s16 Brain_3_Possessed();
    s16 Brain_4_GetKilled();
    s16 Brain_5_Transformed();

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
    Guid mGuid;
    std::shared_ptr<AnimationPal> field_11C_mPal;
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
};
