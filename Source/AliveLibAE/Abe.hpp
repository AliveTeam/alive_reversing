#pragma once

#include "BaseAliveGameObject.hpp"

#define ABE_MOTIONS_ENUM(ENTRY)                      \
    ENTRY(Motion_0_Idle_44EEB0)                      \
    ENTRY(Motion_1_WalkLoop_44FBA0)                  \
    ENTRY(Motion_2_StandingTurn_451830)              \
    ENTRY(Motion_3_Fall_459B60)                      \
    ENTRY(Motion_4_WalkToIdle_44FFC0)                \
    ENTRY(Motion_5_MidWalkToIdle_450080)             \
    ENTRY(Motion_6_WalkBegin_44FEE0)                 \
    ENTRY(Motion_7_Speak_45B140)                     \
    ENTRY(Motion_8_Speak_45B160)                     \
    ENTRY(Motion_9_Speak_45B180)                     \
    ENTRY(Motion_10_Fart_45B1A0)                     \
    ENTRY(Motion_11_Speak_45B0A0)                    \
    ENTRY(Motion_12_Null_4569C0)                     \
    ENTRY(Motion_13_HoistBegin_452B20)               \
    ENTRY(Motion_14_HoistIdle_452440)                \
    ENTRY(Motion_15_HoistLand_452BA0)                \
    ENTRY(Motion_16_LandSoft_45A360)                 \
    ENTRY(Motion_17_CrouchIdle_456BC0)               \
    ENTRY(Motion_18_CrouchToStand_454600)            \
    ENTRY(Motion_19_StandToCrouch_453DC0)            \
    ENTRY(Motion_20_CrouchSpeak_454550)              \
    ENTRY(jMotion_21_ToCrouchSpeak_4545E0)           \
    ENTRY(Motion_22_RollBegin_4539A0)                \
    ENTRY(Motion_23_RollLoop_453A90)                 \
    ENTRY(Motion_24_453D00)                          \
    ENTRY(Motion_25_RunSlideStop_451330)             \
    ENTRY(Motion_26_RunTurn_451500)                  \
    ENTRY(Motion_27_HopBegin_4521C0)                 \
    ENTRY(Motion_28_HopMid_451C50)                   \
    ENTRY(Motion_29_HopLand_4523D0)                  \
    ENTRY(Motion_30_RunJumpBegin_4532E0)             \
    ENTRY(Motion_31_RunJumpMid_452C10)               \
    ENTRY(Motion_32_RunJumpLand_453460)              \
    ENTRY(Motion_33_RunLoop_4508E0)                  \
    ENTRY(Motion_34_DunnoBegin_44ECF0)               \
    ENTRY(Motion_35_DunnoEnd_44ED10)                 \
    ENTRY(Motion_36_Null_45BC50)                     \
    ENTRY(Motion_37_CrouchTurn_454390)               \
    ENTRY(jMotion_38_RunToRoll_453A70)               \
    ENTRY(Motion_39_StandingToRun_450D40)            \
    ENTRY(Motion_40_SneakLoop_450550)                \
    ENTRY(Motion_41_WalkToSneak_450250)              \
    ENTRY(Motion_42_SneakToWalk_4503D0)              \
    ENTRY(Motion_43_MidWalkToSneak_450380)           \
    ENTRY(Motion_44_MidSneakToWalk_450500)           \
    ENTRY(Motion_45_SneakBegin_4507A0)               \
    ENTRY(Motion_46_SneakToIdle_450870)              \
    ENTRY(jMotion_47_MidSneakToIdle_4508C0)          \
    ENTRY(Motion_48_WalkToRun_4500A0)                \
    ENTRY(Motion_49_MidWalkToRun_450200)             \
    ENTRY(Motion_50_RunToWalk_450E20)                \
    ENTRY(Motion_51_MidRunToWalk_450F50)             \
    ENTRY(Motion_52_RunTurnToRun_451710)             \
    ENTRY(Motion_53_RunTurnToWalk_451800)            \
    ENTRY(Motion_54_RunJumpLandRun_4538F0)           \
    ENTRY(Motion_55_RunJumpLandWalk_453970)          \
    ENTRY(Motion_56_DeathDropFall_4591F0)            \
    ENTRY(Motion_57_Dead_4589A0)                     \
    ENTRY(Motion_58_DeadPre_4593E0)                  \
    ENTRY(Motion_59_Null_459450)                     \
    ENTRY(Motion_60_Unused_4A3200)                          \
    ENTRY(Motion_61_TurnToRun_456530)                \
    ENTRY(Motion_62_Punch_454750)                    \
    ENTRY(Motion_63_Sorry_454670)                    \
    ENTRY(Motion_64_AfterSorry_454730)               \
    ENTRY(Motion_65_LedgeAscend_4548E0)              \
    ENTRY(Motion_66_LedgeDescend_454970)             \
    ENTRY(Motion_67_LedgeHang_454E20)                \
    ENTRY(Motion_68_ToOffScreenHoist_454B80)         \
    ENTRY(Motion_69_LedgeHangWobble_454EF0)          \
    ENTRY(Motion_70_RingRopePullHang_455AF0)         \
    ENTRY(Motion_71_Knockback_455090)                \
    ENTRY(Motion_72_KnockbackGetUp_455340)           \
    ENTRY(Motion_73_PushWall_4553A0)                 \
    ENTRY(Motion_74_RollingKnockback_455290)         \
    ENTRY(Motion_75_JumpIntoWell_45C7B0)             \
    ENTRY(Motion_76_ToInsideOfAWellLocal_45CA40)     \
    ENTRY(Motion_77_ToWellShotOut_45D130)            \
    ENTRY(Motion_78_WellBegin_45C810)                \
    ENTRY(Motion_79_InsideWellLocal_45CA60)          \
    ENTRY(Motion_80_WellShotOut_45D150)              \
    ENTRY(jMotion_81_WellBegin_45C7F0)               \
    ENTRY(Motion_82_InsideWellExpress_45CC80)        \
    ENTRY(Motion_83_WellExpressShotOut_45CF70)       \
    ENTRY(Motion_84_FallLandDie_45A420)              \
    ENTRY(jMotion_85_Fall_455070)                    \
    ENTRY(Motion_86_HandstoneBegin)           \
    ENTRY(Motion_87_HandstoneEnd)             \
    ENTRY(Motion_88_GrenadeMachineUse)        \
    ENTRY(Motion_89_BrewMachineBegin)         \
    ENTRY(Motion_90_BrewMachineEnd)           \
    ENTRY(Motion_91_FallingFromGrab)          \
    ENTRY(Motion_92_ForceDownFromHoist)       \
    ENTRY(Motion_93_WalkOffEdge)              \
    ENTRY(Motion_94_RunOffEdge)               \
    ENTRY(Motion_95_SneakOffEdge)             \
    ENTRY(Motion_96_HopToFall)                \
    ENTRY(Motion_97_RunJumpToFall)            \
    ENTRY(Motion_98_RollOffEdge)              \
    ENTRY(Motion_99_LeverUse)                 \
    ENTRY(Motion_100_SlapBomb)                \
    ENTRY(Motion_101_KnockForward)            \
    ENTRY(Motion_102_RollingKnockForward)     \
    ENTRY(jMotion_103_KnockForwardGetUp)      \
    ENTRY(Motion_104_RockThrowStandingHold)   \
    ENTRY(Motion_105_RockThrowStandingThrow)  \
    ENTRY(Motion_106_RockThrowStandingEnd)    \
    ENTRY(Motion_107_RockThrowCrouchingHold)  \
    ENTRY(Motion_108_RockThrowCrouchingThrow) \
    ENTRY(Motion_109_ZShotRolling)            \
    ENTRY(Motion_110_ZShot)                   \
    ENTRY(Motion_111_PickupItem)              \
    ENTRY(Motion_112_Chant)                   \
    ENTRY(Motion_113_ChantEnd)                \
    ENTRY(Motion_114_DoorEnter)               \
    ENTRY(Motion_115_DoorExit)                \
    ENTRY(Motion_116_MineCarEnter)            \
    ENTRY(Motion_117_InMineCar)               \
    ENTRY(Motion_118_MineCarExit)             \
    ENTRY(Motion_119_ToShrykull)              \
    ENTRY(Motion_120_EndShrykull)             \
    ENTRY(Motion_121_LiftGrabBegin)           \
    ENTRY(Motion_122_LiftGrabEnd)             \
    ENTRY(Motion_123_LiftGrabIdle)            \
    ENTRY(Motion_124_LiftUseUp)               \
    ENTRY(Motion_125_LiftUseDown)             \
    ENTRY(Motion_126_TurnWheelBegin)          \
    ENTRY(Motion_127_TurnWheelLoop)           \
    ENTRY(Motion_128_TurnWheelEnd)            \
    ENTRY(Motion_129_PoisonGasDeath)

#define MAKE_ENUM(VAR) VAR,
enum eAbeMotions : s32
{
    ABE_MOTIONS_ENUM(MAKE_ENUM)
};

class PullRingRope;

extern const TintEntry sAbeTintTable[16];

enum class MudSounds : s16
{
    eNone = -1,
    eEmpty_0 = 0,
    eEmpty_1 = 1,
    eEmpty_2 = 2,
    eHelloNeutral_3 = 3,
    eFollowMe_4 = 4,
    eAnger_5 = 5,
    eWait_6 = 6,
    eFart_7 = 7,
    eGiggle_8 = 8,
    eHurt2_9 = 9,
    eLaugh_10 = 10,
    eGoodbye_11 = 11,
    eOkay_12 = 12,
    eNuhUh_13 = 13,
    eOops_14 = 14,
    eDeathDropScream_15 = 15,
    eHurt1_16 = 16,
    eAllOYa_17 = 17,
    eHiAngry_18 = 18,
    eHiHappy_19 = 19,
    eHiSad_20 = 20,
    eNoAngry_21 = 21,
    eNoSad_22 = 22,
    eFartPuh_23 = 23,
    eSick_24 = 24,
    eWork_25 = 25,
    eStopIt_26 = 26,
    eSorry_27 = 27,
    eSadUgh_28 = 28,
};

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

struct Abe_1BC_20_sub_object final
{
    s32 field_0_abe_timer;
    s32 mRegenHealthTimer;
    FP field_8_x_vel_slow_by;
    s32 field_C_unused;
    //u16 field_10_resource_index;
    Mud_Emotion mMood;
    s32 mRollingMotionTimer;
    MudSounds mSay;
    s16 field_1A_padding;
};
ALIVE_ASSERT_SIZEOF(Abe_1BC_20_sub_object, 0x1C);

void Mudokon_SFX(MudSounds idx, s16 volume, s32 pitch, BaseAliveGameObject* pHero);

struct AbeSaveState final
{
    ReliveTypes mAEType;
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
    u16 bAnimFlipX;
    u16 mCurrentMotion;
    u16 mCurrentFrame;
    u16 mFrameChangeCounter;
    s8 mRenderLayer;
    s8 mAnimRender;
    s8 mIsDrawable;
    s8 field_2F_padding;
    FP mHealth;
    u16 mCurrentMotion2; // the same as mCurrentMotion
    u16 mNextMotion;
    u16 mLastLineYPos;
    s16 mCollisionLineType;
    Guid mPlatformId;
    u16 mIsElectrocuted;
    u16 mIsInvisible;
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
    u16 mKnockdownMotion;
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
    u16 mHasEvilFart;
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
    Abe();
    ~Abe();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual BirdPortal* VIntoBirdPortal(s16 gridBlocks) override;
    virtual void VOnTrapDoorOpen() override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;

    s16 CantBeDamaged_44BAB0();
    void ChangeChantState_45BB90(s16 bLaughAtChantEnd);
    void ToKnockback_44E700(s16 bKnockbackSound, s16 bDelayedAnger);
    void SetAsDead_459430();
    void FleechDeath_459350();
    void ExitShrykull_45A9D0(bool bResetRingTimer);

    static s32 CreateFromSaveState(const u8* pData);

    bool mShrivel = false;
    bool mReturnToPreviousMotion = false;
    bool mPreventChanting = false;
    bool mLandSoftly = false;
    bool mLaughAtChantEnd = false;
    bool mPlayLedgeGrabSounds = false;
    bool mHaveHealing = false;
    bool mMudancheeDone = false;
    Abe_1BC_20_sub_object field_128 = {};
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
    s16 HandleDoAction_455BD0();
    void PushWall_44E890();
    void MoveForward_44E9A0();
    s16 CrouchingGameSpeak_453E10();
    bool Is_Celling_Above_44E8D0();
    void MoveWithVelocity_450FA0(FP velocityX);
    s16 RunTryEnterDoor_451220();
    s16 RunTryEnterWell_451060();
    void ToDieFinal_458910();
    s16 DoGameSpeak_45AB70(s32 input);
    void FallOnBombs_44EC10();
    s16 ForceDownIfHoisting_44BA30();
    void BulletDamage_44C980(Bullet* pObj);
    void GiveControlBackToMe_44BA10();
    PullRingRope* GetPullRope_44D120();
    void IntoPortalStates_451990();
    void Calc_Well_Velocity_45C530(s16 xPosSource, s16 yPosSource, s16 XPosDest, s16 yPosDest);
    void FollowLift_45A500();

    s16 MoveLiftUpOrDown_45A7E0(FP yVelocity);
    s16 GetEvilFart_4585F0(s16 bDontLoad);
    void HandleDDCheat();

    s32 mPrevInput = 0;
    s32 mReleasedButtons = 0;
    AllInternalStates field_120_state = {};
    s16 mKnockdownMotion = 0;
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
    s16 mHasEvilFart = 0;
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

bool IsActiveHero(BaseGameObject* pObj);

extern Abe* sActiveHero;
extern IBaseAliveGameObject* sControlledCharacter;

extern bool gAbeInvincible;

extern const char_type* const sAbeMotionNames[130];

s32 Environment_SFX(EnvironmentSfx sfxId, s32 volume, s32 pitchMin, BaseAliveGameObject* pAliveObj);

void Animation_OnFrame_Abe_455F80(BaseGameObject* pPtr, u32&, const IndexedPoint& point);

s32 XGrid_Index_To_XPos_4498F0(FP scale, s32 xGridIndex);
