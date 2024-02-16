#pragma once

#include "BaseAliveGameObject.hpp"
#include "MudokonEnums.hpp"
#include "../relive_lib/SaveStateBase.hpp"
#include "../relive_lib/FatalError.hpp"

enum class eAbeMotions
{
    None_m1 = -1,
    Motion_0_Idle_44EEB0                  ,
    Motion_1_WalkLoop_44FBA0              ,
    Motion_2_StandingTurn_451830          ,
    Motion_3_Fall_459B60                  ,
    Motion_4_WalkToIdle_44FFC0            ,
    Motion_5_MidWalkToIdle_450080         ,
    Motion_6_WalkBegin_44FEE0             ,
    Motion_7_Speak_45B140                 ,
    Motion_8_Speak_45B160                 ,
    Motion_9_Speak_45B180                 ,
    Motion_10_Fart_45B1A0                 ,
    Motion_11_Speak_45B0A0                ,
    Motion_12_Null_4569C0                 ,
    Motion_13_HoistBegin_452B20           ,
    Motion_14_HoistIdle_452440            ,
    Motion_15_HoistLand_452BA0            ,
    Motion_16_LandSoft_45A360             ,
    Motion_17_CrouchIdle_456BC0           ,
    Motion_18_CrouchToStand_454600        ,
    Motion_19_StandToCrouch_453DC0        ,
    Motion_20_CrouchSpeak_454550          ,
    jMotion_21_ToCrouchSpeak_4545E0       ,
    Motion_22_RollBegin_4539A0            ,
    Motion_23_RollLoop_453A90             ,
    Motion_24_453D00                      ,
    Motion_25_RunSlideStop_451330         ,
    Motion_26_RunTurn_451500              ,
    Motion_27_HopBegin_4521C0             ,
    Motion_28_HopMid_451C50               ,
    Motion_29_HopLand_4523D0              ,
    Motion_30_RunJumpBegin_4532E0         ,
    Motion_31_RunJumpMid_452C10           ,
    Motion_32_RunJumpLand_453460          ,
    Motion_33_RunLoop_4508E0              ,
    Motion_34_DunnoBegin_44ECF0           ,
    Motion_35_DunnoEnd_44ED10             ,
    Motion_36_Null_45BC50                 ,
    Motion_37_CrouchTurn_454390           ,
    jMotion_38_RunToRoll_453A70           ,
    Motion_39_StandingToRun_450D40        ,
    Motion_40_SneakLoop_450550            ,
    Motion_41_WalkToSneak_450250          ,
    Motion_42_SneakToWalk_4503D0          ,
    Motion_43_MidWalkToSneak_450380       ,
    Motion_44_MidSneakToWalk_450500       ,
    Motion_45_SneakBegin_4507A0           ,
    Motion_46_SneakToIdle_450870          ,
    jMotion_47_MidSneakToIdle_4508C0      ,
    Motion_48_WalkToRun_4500A0            ,
    Motion_49_MidWalkToRun_450200         ,
    Motion_50_RunToWalk_450E20            ,
    Motion_51_MidRunToWalk_450F50         ,
    Motion_52_RunTurnToRun_451710         ,
    Motion_53_RunTurnToWalk_451800        ,
    Motion_54_RunJumpLandRun_4538F0       ,
    Motion_55_RunJumpLandWalk_453970      ,
    Motion_56_DeathDropFall_4591F0        ,
    Motion_57_Dead_4589A0                 ,
    Motion_58_DeadPre_4593E0              ,
    Motion_59_Null_459450                 ,
    Motion_60_Unused_4A3200                      ,
    Motion_61_TurnToRun_456530            ,
    Motion_62_Punch_454750                ,
    Motion_63_Sorry_454670                ,
    Motion_64_AfterSorry_454730           ,
    Motion_65_LedgeAscend_4548E0          ,
    Motion_66_LedgeDescend_454970         ,
    Motion_67_LedgeHang_454E20            ,
    Motion_68_ToOffScreenHoist_454B80     ,
    Motion_69_LedgeHangWobble_454EF0      ,
    Motion_70_RingRopePullHang_455AF0     ,
    Motion_71_Knockback_455090            ,
    Motion_72_KnockbackGetUp_455340       ,
    Motion_73_PushWall_4553A0             ,
    Motion_74_RollingKnockback_455290     ,
    Motion_75_JumpIntoWell_45C7B0         ,
    Motion_76_ToInsideOfAWellLocal_45CA40 ,
    Motion_77_ToWellShotOut_45D130        ,
    Motion_78_WellBegin_45C810            ,
    Motion_79_InsideWellLocal_45CA60      ,
    Motion_80_WellShotOut_45D150          ,
    jMotion_81_WellBegin_45C7F0           ,
    Motion_82_InsideWellExpress_45CC80    ,
    Motion_83_WellExpressShotOut_45CF70   ,
    Motion_84_FallLandDie_45A420          ,
    jMotion_85_Fall_455070                ,
    Motion_86_HandstoneBegin       ,
    Motion_87_HandstoneEnd         ,
    Motion_88_GrenadeMachineUse    ,
    Motion_89_BrewMachineBegin     ,
    Motion_90_BrewMachineEnd       ,
    Motion_91_FallingFromGrab      ,
    Motion_92_ForceDownFromHoist   ,
    Motion_93_WalkOffEdge          ,
    Motion_94_RunOffEdge           ,
    Motion_95_SneakOffEdge         ,
    Motion_96_HopToFall            ,
    Motion_97_RunJumpToFall        ,
    Motion_98_RollOffEdge          ,
    Motion_99_LeverUse             ,
    Motion_100_SlapBomb            ,
    Motion_101_KnockForward        ,
    Motion_102_RollingKnockForward ,
    jMotion_103_KnockForwardGetUp  ,
    Motion_104_RockThrowStandingHold,
    Motion_105_RockThrowStandingThrow,
    Motion_106_RockThrowStandingEnd,
    Motion_107_RockThrowCrouchingHold,
    Motion_108_RockThrowCrouchingThrow,
    Motion_109_ZShotRolling        ,
    Motion_110_ZShot               ,
    Motion_111_PickupItem          ,
    Motion_112_Chant               ,
    Motion_113_ChantEnd            ,
    Motion_114_DoorEnter           ,
    Motion_115_DoorExit            ,
    Motion_116_MineCarEnter        ,
    Motion_117_InMineCar           ,
    Motion_118_MineCarExit         ,
    Motion_119_ToShrykull          ,
    Motion_120_EndShrykull         ,
    Motion_121_LiftGrabBegin       ,
    Motion_122_LiftGrabEnd         ,
    Motion_123_LiftGrabIdle        ,
    Motion_124_LiftUseUp           ,
    Motion_125_LiftUseDown         ,
    Motion_126_TurnWheelBegin      ,
    Motion_127_TurnWheelLoop       ,
    Motion_128_TurnWheelEnd        ,
    Motion_129_PoisonGasDeath
};

class PullRingRope;

enum class EnvironmentSfx : s8
{
    eSlideStop_0 = 0,
    eWalkingFootstep_1 = 1,
    eRunningFootstep_2 = 2,
    eSneakFootstep_3 = 3,
    eRunSlide_4 = 4,
    eLandingSoft_5 = 5,
    eHitGroundSoft_6 = 6,
    eDeathNoise_7 = 7,
    eRollingNoise_8 = 8,
    eGenericMovement_9 = 9,
    eExhaustingHoistNoise_10 = 10, // empty in AE?
    eRunJumpOrLedgeHoist_11 = 11,
    eAllOYa_12 = 12,
    eKnockback_13 = 13,
    eElumHitWall_14 = 14,                // TODO: better name
    eFallingDeathScreamHitGround_15 = 15 // TODO: better name
};

enum class ChantStates : u16
{
    eIdleChanting_0 = 0,
    ePossessVictim_1 = 1,
    ePossessedVictim_2 = 2,
    eWaitForUnpossessing_3 = 3,
    eUnpossessing_4 = 4,
    ePadding_5 = 5,
    eChantingForBirdPortal_6 = 6,
};

enum class StoneStates : u16
{
    eHandstoneBegin_0 = 0,
    eGetHandstoneType_1 = 1,
    eHandstoneMovieDone_2 = 2,
    eHandstoneEnd_3 = 3,
    eWaitForInput_4 = 4,
    eCamChangeTransition_5 = 5,
    eSetActiveCamToAbe_6 = 6,
    eCircularFadeExit_7 = 7,
};

enum class AbeDoorStates : u16
{
    eAbeComesIn_0 = 0,
    ePadding_1 = 1,
    eWaitABit_2 = 2,
    eToState4_3 = 3,
    eSetNewActiveCamera_4 = 4,
    eSetNewAbePosition_5 = 5,
    eAbeComesOut_6 = 6
};

enum class PortalSubStates : s16
{
    eJumpingInsidePortal_0 = 0,
    eSetNewActiveCamera_1 = 1,
    eHopOutOfPortal_2 = 2,
    ePadding_3 = 3,
    eSetNewAbePosition_4 = 4
};

enum class WorkWheelStates : u16
{
    eTurningWheel_0 = 0,
    eCheckForNoLongerTurningWheel_1 = 1,
    eMapChanging_2 = 2,
};

union AllInternalStates
{
    ChantStates chant;
    StoneStates stone;
    AbeDoorStates door;
    WorkWheelStates wheel;
    u16 raw;
};

enum class Mud_Emotion : s16;
enum eLineTypes : s16;

void Mudokon_SFX(MudSounds idx, s16 volume, s32 pitch, BaseAliveGameObject* pHero);

struct AbeSaveState final : public SaveStateBase
{
    AbeSaveState()
        : SaveStateBase(ReliveTypes::eAbe, sizeof(*this))
    { }
    s16 field_2_padding;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    u16 mCurrentPath;
    EReliveLevelIds mCurrentLevel;
    FP mSpriteScale;
    Scale mScale;
    u16 mRed;
    u16 mGreen;
    u16 mBlue;
    bool bAnimFlipX;
    eAbeMotions mCurrentMotion;
    s32 mCurrentFrame;
    u16 mFrameChangeCounter;
    s8 mRenderLayer;
    bool mAnimRender;
    bool mIsDrawable;
    FP mHealth;
    eAbeMotions mCurrentMotion2; // the same as mCurrentMotion
    eAbeMotions mNextMotion;
    u16 mLastLineYPos;
    eLineTypes mCollisionLineType;
    Guid mPlatformId;
    bool mIsElectrocuted;
    bool mIsInvisible;
    s8 mIsAbeControlled;
    FP field_48_x_vel_slow_by;
    u32 field_50_state;
    u32 field_54_timer;
    u32 field_58_abe_timer;
    u32 mRegenHealthTimer;
    Mud_Emotion mMood;
    MudSounds mSay;
    u32 mAutoSayTimer;
    u32 mRingPulseTimer;
    s8 mBaseThrowableCount;
    bool mShrivel;
    s8 mHaveShrykull;
    s8 bHaveInvisiblity;
    u16 mPrevInput;
    u16 mReleasedButtons;
    eAbeMotions mKnockdownMotion;
    s16 field_76_padding;
    u32 mRollingMotionTimer;
    Guid mDeathFadeOutId;
    Guid mCircularFadeId;
    Guid mOrbWhirlWindId;
    Guid mPossessedObjectId;
    Guid mThrowableId;
    Guid mPullRingRopeId;
    Guid mSlappableOrPickupId;
    Guid mWorkWheelId;
    u32 mInvisibilityTimer;
    u16 mInvisibilityDuration;
    s8 mHandStoneCamIdx;
    ReliveTypes mHandStoneType;
    u16 mFmvId;
    u16 mHandStoneCam1;
    u16 mHandStoneCam2;
    u16 mHandStoneCam3;
    bool mHasEvilFart;
    EReliveLevelIds mDstWellLevel;
    u16 mDstWellPath;
    u16 mDstWellCamera;
    u16 door_id;
    s8 mThrowDirection;
    u16 mBirdPortalSubState;
    Guid mBirdPortalId;
    bool mReturnToPreviousMotion;
    bool mPreventChanting;
    bool mLandSoftly;
    bool mLaughAtChantEnd;
    bool mPlayLedgeGrabSounds;
    bool mHaveHealing;
    bool mTeleporting;
    bool mMudancheeDone;
    bool mMudomoDone;
    bool mShadowEnabled;
    bool mShadowAtBottom;
};

class Bullet;

class Abe final : public BaseAliveGameObject
{
public:
    static constexpr AnimId sAbeAnimIdTable[130] = {
        AnimId::Mudokon_Idle,
        AnimId::Mudokon_Walk,
        AnimId::Mudokon_StandingTurn,
        AnimId::Mudokon_Idle,
        AnimId::Mudokon_WalkToIdle,
        AnimId::Mudokon_MidWalkToIdle,
        AnimId::Mudokon_WalkBegin,
        AnimId::Mudokon_Speak1,
        AnimId::Mudokon_Speak2,
        AnimId::Mudokon_Speak3,
        AnimId::Mudokon_SpeakFart,
        AnimId::Mudokon_ToSpeak,
        AnimId::Mudokon_Null,
        AnimId::Mudokon_HoistBegin,
        AnimId::Mudokon_HoistIdle,
        AnimId::Mudokon_LandSoft,
        AnimId::Mudokon_LandSoft,
        AnimId::Mudokon_CrouchIdle,
        AnimId::Mudokon_CrouchToStand,
        AnimId::Mudokon_StandToCrouch,
        AnimId::Mudokon_CrouchSpeak,
        AnimId::Mudokon_ToCrouchSpeak,
        AnimId::Mudokon_RollBegin,
        AnimId::Mudokon_Rolling,
        AnimId::Mudokon_RollingEnd_Unused,
        AnimId::Mudokon_RunSlideStop,
        AnimId::Mudokon_RunSlideTurn,
        AnimId::Mudokon_HopBegin,
        AnimId::Mudokon_HopMid,
        AnimId::Mudokon_HopLand,
        AnimId::Mudokon_RunJumpBegin,
        AnimId::Mudokon_RunJumpMid,
        AnimId::Mudokon_RunJumpLand,
        AnimId::Mudokon_Run,
        AnimId::Mudokon_DunnoBegin,
        AnimId::Mudokon_DunnoEnd,
        AnimId::Mudokon_DunnoBegin,
        AnimId::Mudokon_CrouchTurn,
        AnimId::Mudokon_RunToRoll,
        AnimId::Mudokon_StandToRun,
        AnimId::Mudokon_Sneak,
        AnimId::Mudokon_WalkToSneak,
        AnimId::Mudokon_SneakToWalk,
        AnimId::Mudokon_MidWalkToSneak,
        AnimId::Mudokon_MidSneakToWalk,
        AnimId::Mudokon_SneakBegin,
        AnimId::Mudokon_SneakToIdle,
        AnimId::Mudokon_MidSneakToIdle,
        AnimId::Mudokon_WalkToRun,
        AnimId::Mudokon_MidWalkToRun,
        AnimId::Mudokon_RunToWalk,
        AnimId::Mudokon_MidRunToWalk,
        AnimId::Mudokon_RunTurnToRun,
        AnimId::Mudokon_RunTurnToWalk,
        AnimId::Mudokon_RunJumpLandRun,
        AnimId::Mudokon_RunJumpLandWalk,
        AnimId::Mudokon_CrouchIdle,
        AnimId::Mudokon_CrouchIdle,
        AnimId::Mudokon_CrouchIdle,
        AnimId::Mudokon_CrouchIdle,
        AnimId::Mudokon_CrouchIdle,
        AnimId::Mudokon_TurnToRun,
        AnimId::Mudokon_Punch,
        AnimId::Mudokon_Sorry,
        AnimId::Mudokon_AfterSorry,
        AnimId::Mudokon_LedgeAscend,
        AnimId::Mudokon_LedgeDescend,
        AnimId::Mudokon_LedgeHang,
        AnimId::Mudokon_LedgeHang,
        AnimId::Mudokon_LedgeHangWobble,
        AnimId::Mudokon_RingRopePullHang,
        AnimId::Mudokon_Knockback,
        AnimId::Mudokon_KnockbackGetUp,
        AnimId::Mudokon_PushWall,
        AnimId::Mudokon_RollingKnockback,
        AnimId::Mudokon_JumpIntoWell,
        AnimId::Mudokon_Well_Idle,
        AnimId::Mudokon_Well_Idle,
        AnimId::Mudokon_WellBegin,
        AnimId::Mudokon_Well_Idle,
        AnimId::Mudokon_Well_Idle,
        AnimId::Mudokon_WellBegin,
        AnimId::Mudokon_Well_Idle,
        AnimId::Mudokon_Well_Idle,
        AnimId::Mudokon_FallLandDie,
        AnimId::Mudokon_Fall,
        AnimId::Mudokon_HandstoneBegin,
        AnimId::Mudokon_HandstoneEnd,
        AnimId::Mudokon_GrenadeMachineUse,
        AnimId::Mudokon_HandstoneBegin,
        AnimId::Mudokon_HandstoneEnd,
        AnimId::Mudokon_FallingFromGrab,
        AnimId::Mudokon_FallingFromGrab,
        AnimId::Mudokon_WalkOffEdge,
        AnimId::Mudokon_RunOffEdge,
        AnimId::Mudokon_SneakOffEdge,
        AnimId::Mudokon_HopToFall,
        AnimId::Mudokon_RunJumpToFall,
        AnimId::Mudokon_RollOffEdge,
        AnimId::Mudokon_LeverUse,
        AnimId::Mudokon_SlapBomb,
        AnimId::Mudokon_KnockForward,
        AnimId::Mudokon_RollingKnockForward,
        AnimId::Mudokon_Idle, // KnockForwardGetUp?
        AnimId::Mudokon_RockThrowStandingHold,
        AnimId::Mudokon_RockThrowStandingThrow,
        AnimId::Mudokon_RockThrowStandingEnd,
        AnimId::Mudokon_RockThrowCrouchingHold,
        AnimId::Mudokon_RockThrowCrouchingThrow,
        AnimId::Mudokon_ZShotRolling,
        AnimId::Mudokon_ZShot,
        AnimId::Mudokon_PickupItem,
        AnimId::Mudokon_Chant,
        AnimId::Mudokon_ChantEnd,
        AnimId::Mudokon_DoorEnter,
        AnimId::Mudokon_DoorExit,
        AnimId::Mudokon_MineCarEnter,
        AnimId::Mudokon_MineCarEnter,
        AnimId::Mudokon_MineCarExit,
        AnimId::Mudokon_ToShrykull,
        AnimId::Mudokon_EndShrykull,
        AnimId::Mudokon_LiftGrabBegin,
        AnimId::Mudokon_LiftGrabEnd,
        AnimId::Mudokon_LiftGrabIdle,
        AnimId::Mudokon_LiftUseUp,
        AnimId::Mudokon_LiftUseDown,
        AnimId::Mudokon_TurnWheelBegin,
        AnimId::Mudokon_TurnWheel,
        AnimId::Mudokon_TurnWheelEnd,
        AnimId::Mudokon_PoisonGasDeath};

    Abe();
    ~Abe();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VScreenChanged() override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;
    void GetSaveState(AbeSaveState& pSaveBuffer);
    
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual BirdPortal* VIntoBirdPortal(s16 gridBlocks) override;
    virtual void VOnTrapDoorOpen() override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual s16 VGetMotion(eMotionType motionType) override
    {
        switch (motionType)
        {
            case eMotionType::ePreviousMotion:
                return static_cast<s16>(mPreviousMotion);
            case eMotionType::eCurrentMotion:
                return static_cast<s16>(mCurrentMotion);
            case eMotionType::eNextMotion:
                return static_cast<s16>(mNextMotion);
            default:
                ALIVE_FATAL("Invalid motion type %d", static_cast<s32>(motionType));
        }
    }

    s16 CantBeDamaged_44BAB0();
    void ChangeChantState(bool bLaughAtChantEnd);
    void ToKnockback_44E700(s16 bKnockbackSound, s16 bDelayedAnger);
    void SetAsDead();
    void FleechDeath_459350();
    void ExitShrykull(bool bResetRingTimer);

    static void CreateFromSaveState(SerializedObjectData& pData);
    static void CreateFromSaveState(const AbeSaveState& pData);

    bool mShrivel = false;
    bool mReturnToPreviousMotion = false;
    bool mPreventChanting = true;
    bool mLandSoftly = true;
    bool mLaughAtChantEnd = false;
    bool mPlayLedgeGrabSounds = false;
    bool mHaveHealing = false;
    bool mMudancheeDone = false;
    s32 field_0_abe_timer = 0;
    s32 mRegenHealthTimer = 0;
    FP field_8_x_vel_slow_by = {};
    //u16 field_10_resource_index;
    Mud_Emotion mMood = Mud_Emotion::eNormal_0;
    s32 mRollingMotionTimer = 0;
    MudSounds mSay = MudSounds::eNone;
    s32 field_124_timer = 0;
    Guid mBirdPortalId;
    s8 mBaseThrowableCount = 0;
    Guid mThrowableId;
    s16 field_1A0_door_id = 0;
    s8 mThrowDirection = 0;
    s32 mRingPulseTimer = 0;
    s16 mHaveShrykull = 0;
    s32 mAutoSayTimer = 0;
    s16 mHaveInvisibility = 0;
    s16 mInvisibilityDuration = 0;
    eAbeMotions mPreviousMotion = eAbeMotions::Motion_0_Idle_44EEB0;
    eAbeMotions mCurrentMotion = eAbeMotions::Motion_0_Idle_44EEB0;
    eAbeMotions mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
    bool mbMotionChanged = false;

private:
    void LoadAnimations();

    void DoRunJump();

    IBaseAliveGameObject* FindObjectToPossess_44B7B0();
    void Load_Basic_Resources_44D460();
    void Free_Resources_44D420();
    bool IsStanding_449D30();
    void Free_Shrykull_Resources_45AA90();

    // Motions
    void Motion_0_Idle_44EEB0();
    void Motion_1_WalkLoop_44FBA0();
    void Motion_2_StandingTurn_451830();
    void Motion_3_Fall_459B60();
    void Motion_4_WalkToIdle_44FFC0();
    void Motion_5_MidWalkToIdle_450080();
    void Motion_6_WalkBegin_44FEE0();
    void Motion_7_Speak_45B140();
    void Motion_8_Speak_45B160();
    void Motion_9_Speak_45B180();
    void Motion_10_Fart_45B1A0();
    void Motion_11_ToSpeak_45B0A0();
    void Motion_12_Null_4569C0();
    void Motion_13_HoistBegin_452B20();
    void Motion_14_HoistIdle_452440();
    void Motion_15_HoistLand_452BA0();
    void Motion_16_LandSoft_45A360();
    void Motion_17_CrouchIdle_456BC0();
    void Motion_18_CrouchToStand_454600();
    void Motion_19_StandToCrouch_453DC0();
    void Motion_20_CrouchSpeak_454550();
    void jMotion_21_ToCrouchSpeak_4545E0();
    void Motion_22_RollBegin_4539A0();
    void Motion_23_RollLoop_453A90();
    void Motion_24_453D00();
    void Motion_25_RunSlideStop_451330();
    void Motion_26_RunTurn_451500();
    void Motion_27_HopBegin_4521C0();
    void Motion_28_HopMid_451C50();
    void Motion_29_HopLand_4523D0();
    void Motion_30_RunJumpBegin_4532E0();
    void Motion_31_RunJumpMid_452C10();
    void Motion_32_RunJumpLand_453460();
    void Motion_33_RunLoop_4508E0();
    void Motion_34_DunnoBegin_44ECF0();
    void Motion_35_DunnoEnd_44ED10();
    void Motion_36_Null_45BC50();
    void Motion_37_CrouchTurn_454390();
    void jMotion_38_RunToRoll_453A70();
    void Motion_39_StandingToRun_450D40();
    void Motion_40_SneakLoop_450550();
    void Motion_41_WalkToSneak_450250();
    void Motion_42_SneakToWalk_4503D0();
    void Motion_43_MidWalkToSneak_450380();
    void Motion_44_MidSneakToWalk_450500();
    void Motion_45_SneakBegin_4507A0();
    void Motion_46_SneakToIdle_450870();
    void jMotion_47_MidSneakToIdle_4508C0();
    void Motion_48_WalkToRun_4500A0();
    void Motion_49_MidWalkToRun_450200();
    void Motion_50_RunToWalk_450E20();
    void Motion_51_MidRunToWalk_450F50();
    void Motion_52_RunTurnToRun_451710();
    void Motion_53_RunTurnToWalk_451800();
    void Motion_54_RunJumpLandRun_4538F0();
    void Motion_55_RunJumpLandWalk_453970();
    void Motion_56_DeathDropFall_4591F0();
    void Motion_57_Dead_4589A0();
    void Motion_58_DeadPre_4593E0();
    void Motion_59_Null_459450();
    void Motion_60_Unused_4A3200();
    void Motion_61_TurnToRun_456530();
    void Motion_62_Punch_454750();
    void Motion_63_Sorry_454670();
    void Motion_64_AfterSorry_454730();
    void Motion_65_LedgeAscend_4548E0();
    void Motion_66_LedgeDescend_454970();
    void Motion_67_LedgeHang_454E20();
    void Motion_68_ToOffScreenHoist_454B80();
    void Motion_69_LedgeHangWobble_454EF0();
    void Motion_70_RingRopePullHang_455AF0();
    void Motion_71_Knockback_455090();
    void Motion_72_KnockbackGetUp_455340();
    void Motion_73_PushWall_4553A0();
    void Motion_74_RollingKnockback_455290();
    void Motion_75_JumpIntoWell_45C7B0();
    void Motion_76_ToInsideOfAWellLocal_45CA40();
    void Motion_77_ToWellShotOut_45D130();
    void Motion_78_WellBegin_45C810();
    void Motion_79_InsideWellLocal_45CA60();
    void Motion_80_WellShotOut_45D150();
    void jMotion_81_WellBegin_45C7F0();
    void Motion_82_InsideWellExpress_45CC80();
    void Motion_83_WellExpressShotOut_45CF70();
    void Motion_84_FallLandDie_45A420();
    void jMotion_85_Fall_455070();
    void Motion_86_HandstoneBegin();
    void Motion_87_HandstoneEnd();
    void Motion_88_GrenadeMachineUse();
    void Motion_89_BrewMachineBegin();
    void Motion_90_BrewMachineEnd();
    void Motion_91_FallingFromGrab();
    void Motion_92_ForceDownFromHoist();
    void Motion_93_WalkOffEdge();
    void Motion_94_RunOffEdge();
    void Motion_95_SneakOffEdge();
    void Motion_96_HopToFall();
    void Motion_97_RunJumpToFall();
    void Motion_98_RollOffEdge();
    void Motion_99_LeverUse();
    void Motion_100_SlapBomb();
    void Motion_101_KnockForward();
    void Motion_102_RollingKnockForward();
    void jMotion_103_KnockForwardGetUp();
    void Motion_104_RockThrowStandingHold();
    void Motion_105_RockThrowStandingThrow();
    void Motion_106_RockThrowStandingEnd();
    void Motion_107_RockThrowCrouchingHold();
    void Motion_108_RockThrowCrouchingThrow();
    void Motion_109_ZShotRolling();
    void Motion_110_ZShot();
    void Motion_111_PickupItem();
    void Motion_112_Chant();
    void Motion_113_ChantEnd();
    void Motion_114_DoorEnter();
    void Motion_115_DoorExit();
    void Motion_116_MineCarEnter();
    void Motion_117_InMineCar();
    void Motion_118_MineCarExit();
    void Motion_119_ToShrykull();
    void Motion_120_EndShrykull();
    void Motion_121_LiftGrabBegin();
    void Motion_122_LiftGrabEnd();
    void Motion_123_LiftGrabIdle();
    void Motion_124_LiftUseUp();
    void Motion_125_LiftUseDown();
    void Motion_126_TurnWheelBegin();
    void Motion_127_TurnWheelLoop();
    void Motion_128_TurnWheelEnd();
    void Motion_129_PoisonGasDeath();

    void jMotion_81_WellBegin_4017F8();

    // End motions


    void ToDie_4588D0();
    void ToIdle_44E6B0();
    void PickUpThrowabe_Or_PressBomb_454090(FP fpX, s32 fpY, s32 bStandToCrouch);
    s16 ToLeftRightMovement_44E340();
    void TryHoist_44ED30();
    static void Create_Fart_421D20();
    s16 TryEnterMineCar_4569E0();
    s32 NearDoorIsOpen_44EE10();
    eAbeMotions HandleDoAction_455BD0();
    void PushWall_44E890();
    void MoveForward_44E9A0();
    s16 CrouchingGameSpeak_453E10();
    bool Is_Celling_Above_44E8D0();
    void MoveWithVelocity_450FA0(FP velocityX);
    s16 RunTryEnterDoor_451220();
    s16 RunTryEnterWell_451060();
    void ToDieFinal_458910();
    eAbeMotions DoGameSpeak_45AB70(s32 input);
    void FallOnBombs_44EC10();
    s16 ForceDownIfHoisting_44BA30();
    void BulletDamage_44C980(Bullet* pObj);
    void GiveControlBackToMe_44BA10();
    PullRingRope* GetPullRope();
    void IntoPortalStates_451990();
    void Calc_Well_Velocity_45C530(s16 xPosSource, s16 yPosSource, s16 XPosDest, s16 yPosDest);
    void FollowLift_45A500();

    eAbeMotions MoveLiftUpOrDown_45A7E0(FP yVelocity);
    s16 GetEvilFart_4585F0(s16 bDontLoad);
    void HandleDDCheat();

    s32 mPrevInput = 0;
    s32 mReleasedButtons = 0;
    AllInternalStates field_120_state = {};
    eAbeMotions mKnockdownMotion = eAbeMotions::None_m1;
    Guid mFadeId;
    Guid mCircularFadeId;
    Guid mOrbWhirlWindId;
    Guid mPossessedObjectId;
    Guid mPullRingRopeId;
    Guid mSlappableOrPickupId;
    Guid mWorkWheelId;
    s32 mInvisibilityTimer = 0;
    Guid mInvisibleEffectId;
    s8 mHandStoneCamIdx = 0;
    ReliveTypes mHandStoneType = {};
    s16 mFmvId = 0;
    s16 mHandStoneCams[3] = {};
    bool mHasEvilFart = false;
    EReliveLevelIds mDstWellLevel = EReliveLevelIds::eNone;
    s16 mDstWellPath = 0;
    s16 mDstWellCamera = 0;
    PortalSubStates mBirdPortalSubState = PortalSubStates::eJumpingInsidePortal_0;
    bool mDoQuicksave = false;
    bool mMudomoDone = false;
    s16 mSaveFileId = 0;

    using TAbeMotionFunction = decltype(&Abe::Motion_0_Idle_44EEB0);

    const TAbeMotionFunction sAbeMotionMachineTable_554910[130] = {
        &Abe::Motion_0_Idle_44EEB0,
        &Abe::Motion_1_WalkLoop_44FBA0,
        &Abe::Motion_2_StandingTurn_451830,
        &Abe::Motion_3_Fall_459B60,
        &Abe::Motion_4_WalkToIdle_44FFC0,
        &Abe::Motion_5_MidWalkToIdle_450080,
        &Abe::Motion_6_WalkBegin_44FEE0,
        &Abe::Motion_7_Speak_45B140,
        &Abe::Motion_8_Speak_45B160,
        &Abe::Motion_9_Speak_45B180,
        &Abe::Motion_10_Fart_45B1A0,
        &Abe::Motion_11_ToSpeak_45B0A0,
        &Abe::Motion_12_Null_4569C0,
        &Abe::Motion_13_HoistBegin_452B20,
        &Abe::Motion_14_HoistIdle_452440,
        &Abe::Motion_15_HoistLand_452BA0,
        &Abe::Motion_16_LandSoft_45A360,
        &Abe::Motion_17_CrouchIdle_456BC0,
        &Abe::Motion_18_CrouchToStand_454600,
        &Abe::Motion_19_StandToCrouch_453DC0,
        &Abe::Motion_20_CrouchSpeak_454550,
        &Abe::jMotion_21_ToCrouchSpeak_4545E0,
        &Abe::Motion_22_RollBegin_4539A0,
        &Abe::Motion_23_RollLoop_453A90,
        &Abe::Motion_24_453D00,
        &Abe::Motion_25_RunSlideStop_451330,
        &Abe::Motion_26_RunTurn_451500,
        &Abe::Motion_27_HopBegin_4521C0,
        &Abe::Motion_28_HopMid_451C50,
        &Abe::Motion_29_HopLand_4523D0,
        &Abe::Motion_30_RunJumpBegin_4532E0,
        &Abe::Motion_31_RunJumpMid_452C10,
        &Abe::Motion_32_RunJumpLand_453460,
        &Abe::Motion_33_RunLoop_4508E0,
        &Abe::Motion_34_DunnoBegin_44ECF0,
        &Abe::Motion_35_DunnoEnd_44ED10,
        &Abe::Motion_36_Null_45BC50,
        &Abe::Motion_37_CrouchTurn_454390,
        &Abe::jMotion_38_RunToRoll_453A70,
        &Abe::Motion_39_StandingToRun_450D40,
        &Abe::Motion_40_SneakLoop_450550,
        &Abe::Motion_41_WalkToSneak_450250,
        &Abe::Motion_42_SneakToWalk_4503D0,
        &Abe::Motion_43_MidWalkToSneak_450380,
        &Abe::Motion_44_MidSneakToWalk_450500,
        &Abe::Motion_45_SneakBegin_4507A0,
        &Abe::Motion_46_SneakToIdle_450870,
        &Abe::jMotion_47_MidSneakToIdle_4508C0,
        &Abe::Motion_48_WalkToRun_4500A0,
        &Abe::Motion_49_MidWalkToRun_450200,
        &Abe::Motion_50_RunToWalk_450E20,
        &Abe::Motion_51_MidRunToWalk_450F50,
        &Abe::Motion_52_RunTurnToRun_451710,
        &Abe::Motion_53_RunTurnToWalk_451800,
        &Abe::Motion_54_RunJumpLandRun_4538F0,
        &Abe::Motion_55_RunJumpLandWalk_453970,
        &Abe::Motion_56_DeathDropFall_4591F0,
        &Abe::Motion_57_Dead_4589A0,
        &Abe::Motion_58_DeadPre_4593E0,
        &Abe::Motion_59_Null_459450,
        &Abe::Motion_60_Unused_4A3200,
        &Abe::Motion_61_TurnToRun_456530,
        &Abe::Motion_62_Punch_454750,
        &Abe::Motion_63_Sorry_454670,
        &Abe::Motion_64_AfterSorry_454730,
        &Abe::Motion_65_LedgeAscend_4548E0,
        &Abe::Motion_66_LedgeDescend_454970,
        &Abe::Motion_67_LedgeHang_454E20,
        &Abe::Motion_68_ToOffScreenHoist_454B80,
        &Abe::Motion_69_LedgeHangWobble_454EF0,
        &Abe::Motion_70_RingRopePullHang_455AF0,
        &Abe::Motion_71_Knockback_455090,
        &Abe::Motion_72_KnockbackGetUp_455340,
        &Abe::Motion_73_PushWall_4553A0,
        &Abe::Motion_74_RollingKnockback_455290,
        &Abe::Motion_75_JumpIntoWell_45C7B0,
        &Abe::Motion_76_ToInsideOfAWellLocal_45CA40,
        &Abe::Motion_77_ToWellShotOut_45D130,
        &Abe::Motion_78_WellBegin_45C810,
        &Abe::Motion_79_InsideWellLocal_45CA60,
        &Abe::Motion_80_WellShotOut_45D150,
        &Abe::jMotion_81_WellBegin_45C7F0,
        &Abe::Motion_82_InsideWellExpress_45CC80,
        &Abe::Motion_83_WellExpressShotOut_45CF70,
        &Abe::Motion_84_FallLandDie_45A420,
        &Abe::jMotion_85_Fall_455070,
        &Abe::Motion_86_HandstoneBegin,
        &Abe::Motion_87_HandstoneEnd,
        &Abe::Motion_88_GrenadeMachineUse,
        &Abe::Motion_89_BrewMachineBegin,
        &Abe::Motion_90_BrewMachineEnd,
        &Abe::Motion_91_FallingFromGrab,
        &Abe::Motion_92_ForceDownFromHoist,
        &Abe::Motion_93_WalkOffEdge,
        &Abe::Motion_94_RunOffEdge,
        &Abe::Motion_95_SneakOffEdge,
        &Abe::Motion_96_HopToFall,
        &Abe::Motion_97_RunJumpToFall,
        &Abe::Motion_98_RollOffEdge,
        &Abe::Motion_99_LeverUse,
        &Abe::Motion_100_SlapBomb,
        &Abe::Motion_101_KnockForward,
        &Abe::Motion_102_RollingKnockForward,
        &Abe::jMotion_103_KnockForwardGetUp,
        &Abe::Motion_104_RockThrowStandingHold,
        &Abe::Motion_105_RockThrowStandingThrow,
        &Abe::Motion_106_RockThrowStandingEnd,
        &Abe::Motion_107_RockThrowCrouchingHold,
        &Abe::Motion_108_RockThrowCrouchingThrow,
        &Abe::Motion_109_ZShotRolling,
        &Abe::Motion_110_ZShot,
        &Abe::Motion_111_PickupItem,
        &Abe::Motion_112_Chant,
        &Abe::Motion_113_ChantEnd,
        &Abe::Motion_114_DoorEnter,
        &Abe::Motion_115_DoorExit,
        &Abe::Motion_116_MineCarEnter,
        &Abe::Motion_117_InMineCar,
        &Abe::Motion_118_MineCarExit,
        &Abe::Motion_119_ToShrykull,
        &Abe::Motion_120_EndShrykull,
        &Abe::Motion_121_LiftGrabBegin,
        &Abe::Motion_122_LiftGrabEnd,
        &Abe::Motion_123_LiftGrabIdle,
        &Abe::Motion_124_LiftUseUp,
        &Abe::Motion_125_LiftUseDown,
        &Abe::Motion_126_TurnWheelBegin,
        &Abe::Motion_127_TurnWheelLoop,
        &Abe::Motion_128_TurnWheelEnd,
        &Abe::Motion_129_PoisonGasDeath};
};

bool IsAbe(BaseGameObject* pObj);

extern Abe* gAbe;
extern IBaseAliveGameObject* sControlledCharacter;

extern bool gAbeInvincible;

extern const char_type* const sAbeMotionNames[130];

s32 Environment_SFX(EnvironmentSfx sfxId, s32 volume, s32 pitchMin, BaseAliveGameObject* pAliveObj);

void Animation_OnFrame_Abe(BaseGameObject* pPtr, u32&, const IndexedPoint& point);
