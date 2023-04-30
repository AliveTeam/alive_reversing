#pragma once
#include "../AliveLibCommon/FunctionFwd.hpp"


enum class AnimId
{
    None = 0,
    Abe_Head_Gib,
    Abe_Arm_Gib,
    Abe_Body_Gib,
    BlindMud_Head_Gib,
    BlindMud_Arm_Gib,
    BlindMud_Body_Gib,
    Glukkon_Head_Gib,
    Glukkon_Arm_Gib,
    Glukkon_Body_Gib,
    Aslik_Head_Gib,
    Aslik_Arm_Gib,
    Aslik_Body_Gib,
    Dripik_Head_Gib,
    Dripik_Arm_Gib,
    Dripik_Body_Gib,
    Phleg_Head_Gib,
    Phleg_Arm_Gib,
    Phleg_Body_Gib,
    Fleech_Head_Gib,
    Fleech_Body_Gib,
    Elum_Head_Gib,
    Elum_Arm_Gib,
    Elum_Body_Gib,
    Meat_Gib,
    Metal_Gib,
    Rock_Gib,
    Slig_Head_Gib,
    Slig_Arm_Gib,
    Slig_Body_Gib,
    Slog_Head_Gib,
    Slog_Body_Gib,
    Stick_Gib,

    Mudokon_RunToRoll,
    Mudokon_CrouchSpeak,
    Mudokon_ToCrouchSpeak,
    Mudokon_FallLandDie,
    Mudokon_PoisonGasDeath,
    Mudokon_ZShotRolling,
    Mudokon_ZShot,
    Mudokon_DoorEnter,
    Mudokon_DoorExit,
    Mudokon_WalkOffEdge,
    Mudokon_RunOffEdge,
    Mudokon_SneakOffEdge,
    Mudokon_HandstoneEnd,
    Mudokon_GrenadeMachineUse,
    Mudokon_HandstoneBegin,
    Mudokon_LedgeHang,
    Mudokon_LedgeDescend,
    Mudokon_RingRopePullHang,
    Mudokon_LedgeHangWobble,
    Mudokon_LedgeAscend,
    Mudokon_HopMid,
    Mudokon_HopLand,
    Mudokon_HopBegin,
    Mudokon_PickupItem,
    Mudokon_RollingKnockback,
    Mudokon_RollingKnockForward,
    Mudokon_LiftGrabIdle,
    Mudokon_LiftUseDown,
    Mudokon_LiftGrabEnd,
    Mudokon_LiftGrabBegin,
    Mudokon_LiftUseUp,
    Mudokon_MineCarEnter,
    Mudokon_MineCarExit,
    Mudokon_PushWall,
    Mudokon_RockThrowStandingHold,
    Mudokon_RockThrowStandingEnd,
    Mudokon_RockThrowCrouchingHold,
    Mudokon_RockThrowCrouchingThrow,
    Mudokon_RockThrowStandingThrow,
    Mudokon_Rolling,
    Mudokon_RollingEnd_Unused,
    Mudokon_RollBegin,
    Mudokon_DunnoEnd,
    Mudokon_EndShrykull,
    Mudokon_ToShrykull,
    Mudokon_SlapBomb,
    Mudokon_Sorry,
    Mudokon_AfterSorry,
    Mudokon_ToSpeak,
    Mudokon_Null,
    Mudokon_RunJumpLand,
    Mudokon_RunTurnToWalk,
    Mudokon_RunJumpLandRun,
    Mudokon_RunJumpLandWalk,
    Mudokon_TurnToRun,
    Mudokon_HopToFall,
    Mudokon_RunJumpToFall,
    Mudokon_RollOffEdge,
    Mudokon_Well_Idle,
    Mudokon_WellBegin,
    Mudokon_JumpIntoWell,

    Bat_Flying,
    Bat,
    Bat_Unknown,
    Bee_Swarm,

    Fleech_Climb,
    Fleech_SettleOnGround,
    Fleech_Consume,
    Fleech_Fall,
    Fleech_Sleeping,
    Fleech_Idle,
    Fleech_StopCrawling,
    Fleech_StopMidCrawlCycle,
    Fleech_Land,
    Fleech_SleepingWithTongue,
    Fleech_PatrolCry,
    Fleech_RaiseHead,
    Fleech_WakingUp,
    Fleech_Unused,
    Fleech_Knockback,
    Fleech_ExtendTongueFromEnemy,
    Fleech_RetractTongueFromEnemey,
    Fleech_DeathByFalling,
    Fleech_Crawl,

    FlyingSlig_Idle,
    FlyingSlig_MoveHorizontal,
    FlyingSlig_IdleTurnAround,
    FlyingSlig_MoveDown,
    FlyingSlig_MoveDownTurnAround,
    FlyingSlig_MoveUp,
    FlyingSlig_MoveUpTurnAround,
    FlyingSlig_PullLever,
    FlyingSlig_Speak,
    FlyingSlig_Possession,
    FlyingSlig_MoveHorizontalEnd,
    FlyingSlig_MoveUpStart,
    FlyingSlig_MoveHorizontalToDown,
    FlyingSlig_MoveUpToHorizontal,
    FlyingSlig_MoveDownToHorizontal,
    FlyingSlig_TurnQuick,
    FlyingSlig_IdleToHorizontal,
    FlyingSlig_BeginDownMovement,
    FlyingSlig_EndDownMovement,
    FlyingSlig_DownKnockback,
    FlyingSlig_UpKnockback,
    FlyingSlig_EndUpMovement,
    FlyingSlig_InstantUpXTurn,
    FlyingSlig_InstantDownXTurn,
    FlyingSlig_HorizontalToUpMovement,
    FlyingSlig_TurnToHorizontalMovement,

    Glukkon_Aslik_KnockBack,
    Glukkon_Aslik_Idle,
    Glukkon_Aslik_Jump,
    Glukkon_Aslik_Land,
    Glukkon_Aslik_LongLaugh,
    Glukkon_Aslik_ChantShake,
    Glukkon_Aslik_GetShot,
    Glukkon_Aslik_Speak1,
    Glukkon_Aslik_Speak2,
    Glukkon_Aslik_Speak3,
    Glukkon_Aslik_KnockBackStandBegin,
    Glukkon_Aslik_KnockBackStandEnd,
    Glukkon_Aslik_Turn,
    Glukkon_Aslik_JumpToFall,
    Glukkon_Aslik_WalkToFall,
    Glukkon_Aslik_Fall,
    Glukkon_Aslik_DeathFall,
    Glukkon_Aslik_BeginWalk,
    Glukkon_Aslik_EndWalk,
    Glukkon_Aslik_StandToJump,
    Glukkon_Aslik_JumpToStand,
    Glukkon_Aslik_WalkToJump,
    Glukkon_Aslik_JumpToWalk,
    Glukkon_Aslik_EndSingleStep,
    Glukkon_Aslik_Walk,

    Glukkon_Dripik_KnockBack,
    Glukkon_Dripik_Idle,
    Glukkon_Dripik_Jump,
    Glukkon_Dripik_Land,
    Glukkon_Dripik_LongLaugh,
    Glukkon_Dripik_ChantShake,
    Glukkon_Dripik_GetShot,
    Glukkon_Dripik_Speak1,
    Glukkon_Dripik_Speak2,
    Glukkon_Dripik_Speak3,
    Glukkon_Dripik_KnockBackStandBegin,
    Glukkon_Dripik_KnockBackStandEnd,
    Glukkon_Dripik_Turn,
    Glukkon_Dripik_JumpToFall,
    Glukkon_Dripik_WalkToFall,
    Glukkon_Dripik_Fall,
    Glukkon_Dripik_DeathFall,
    Glukkon_Dripik_BeginWalk,
    Glukkon_Dripik_EndWalk,
    Glukkon_Dripik_StandToJump,
    Glukkon_Dripik_JumpToStand,
    Glukkon_Dripik_WalkToJump,
    Glukkon_Dripik_JumpToWalk,
    Glukkon_Dripik_EndSingleStep,
    Glukkon_Dripik_Walk,

    Glukkon_Normal_KnockBack,
    Glukkon_Normal_Idle,
    Glukkon_Normal_Jump,
    Glukkon_Normal_Land,
    Glukkon_Normal_LongLaugh,
    Glukkon_Normal_ChantShake,
    Glukkon_Normal_GetShot,
    Glukkon_Normal_Speak1,
    Glukkon_Normal_Speak2,
    Glukkon_Normal_Speak3,
    Glukkon_Normal_KnockBackStandBegin,
    Glukkon_Normal_KnockBackStandEnd,
    Glukkon_Normal_Turn,
    Glukkon_Normal_JumpToFall,
    Glukkon_Normal_WalkToFall,
    Glukkon_Normal_Fall,
    Glukkon_Normal_DeathFall,
    Glukkon_Normal_BeginWalk,
    Glukkon_Normal_EndWalk,
    Glukkon_Normal_StandToJump,
    Glukkon_Normal_JumpToStand,
    Glukkon_Normal_WalkToJump,
    Glukkon_Normal_JumpToWalk,
    Glukkon_Normal_EndSingleStep,
    Glukkon_Normal_Walk,

    Glukkon_Phleg_KnockBack,
    Glukkon_Phleg_Idle,
    Glukkon_Phleg_Jump,
    Glukkon_Phleg_Land,
    Glukkon_Phleg_LongLaugh,
    Glukkon_Phleg_ChantShake,
    Glukkon_Phleg_GetShot,
    Glukkon_Phleg_Speak1,
    Glukkon_Phleg_Speak2,
    Glukkon_Phleg_Speak3,
    Glukkon_Phleg_KnockBackStandBegin,
    Glukkon_Phleg_KnockBackStandEnd,
    Glukkon_Phleg_Turn,
    Glukkon_Phleg_JumpToFall,
    Glukkon_Phleg_WalkToFall,
    Glukkon_Phleg_Fall,
    Glukkon_Phleg_DeathFall,
    Glukkon_Phleg_BeginWalk,
    Glukkon_Phleg_EndWalk,
    Glukkon_Phleg_StandToJump,
    Glukkon_Phleg_JumpToStand,
    Glukkon_Phleg_WalkToJump,
    Glukkon_Phleg_JumpToWalk,
    Glukkon_Phleg_EndSingleStep,
    Glukkon_Phleg_Walk,

    Greeter_Chase,
    Greeter_Falling,
    Greeter_Hit,
    Greeter_Moving,
    Greeter_Speak,
    Greeter_Turn,

    Mine_Car_Closed,
    Mine_Car_Open,
    Mine_Car_Shake_A,
    Mine_Car_Shake_B,
    Mine_Car_Tread_Idle,
    Mine_Car_Tread_Move_A,
    Mine_Car_Tread_Move_B,

    Mudokon_Chant,
    Mudokon_ChantEnd,
    Mudokon_Chisel,
    Mudokon_CrouchIdle,
    Mudokon_ToDuck,
    Mudokon_CrouchToStand,
    Mudokon_CrouchTurn,
    Mudokon_Duck,
    Mudokon_DuckToCrouch,
    Mudokon_Fall,
    Mudokon_KnockbackGetUp,
    Mudokon_SlapOwnHead,
    Mudokon_HoistIdle,
    Mudokon_LandSoft,
    Mudokon_HoistBegin,
    Mudokon_Idle,
    Mudokon_KnockForward,
    Mudokon_Knockback,
    Mudokon_LeverUse,
    Mudokon_Run,
    Mudokon_RunSlideStop,
    Mudokon_RunSlideTurn,
    Mudokon_RunJumpMid,
    Mudokon_RunJumpBegin,
    Mudokon_CrouchScrub,
    Mudokon_DunnoBegin,
    Mudokon_Punch,
    Mudokon_Sling_Idle,
    Mudokon_Sling_Angry,
    Mudokon_Sling_Speak,
    Mudokon_Sling_ShootStart,
    Mudokon_Sling_ShootEnd,
    Mudokon_Sling_AngryToIdle,
    Mudokon_StandScrubLoop,
    Mudokon_StandScrubLoopToPause,
    Mudokon_StandScrubPauseToLoop,
    Mudokon_StandScrubPause,
    Mudokon_IdleToStandScrub,
    Mudokon_StandScrubToIdle,
    Mudokon_Struggle,
    Mudokon_StruggleToCrouchChant,
    Mudokon_CrouchChant,
    Mudokon_CrouchChantToStruggle,
    Mudokon_DunnoMid,
    Mudokon_AO_M_15_Null,
    Mudokon_CrouchSpeak1,
    Mudokon_CrouchSpeak2,
    Mudokon_ElumWalkLoop,
    Mudokon_ElumIdle,
    Mudokon_ElumRunSlideStop,
    Mudokon_ElumRunTurn,
    Mudokon_AO_M_106_Null,
    Mudokon_ElumHopBegin,
    Mudokon_ElumHopMid,
    Mudokon_ElumHopLand,
    Mudokon_ElumRunJumpBegin,
    Mudokon_ElumRunJumpMid,
    Mudokon_ElumRunJumpLand,
    Mudokon_ElumTurn,
    Mudokon_ElumRunLoop,
    Mudokon_ElumSpeak,
    Mudokon_AO_M_116_Null,
    Mudokon_ElumWalkBegin,
    Mudokon_ElumRunBegin,
    Mudokon_AO_M_119_Null,
    Mudokon_ElumRunToWalk,
    Mudokon_ElumMidRunToWalk,
    Mudokon_ElumRunTurnToRun,
    Mudokon_ElumRunTurnToWalk,
    Mudokon_ElumWalkEnd,
    Mudokon_ElumMidWalkEnd,
    Mudokon_ElumBeesStruggling,
    Mudokon_ElumMountEnd,
    Mudokon_ElumUnmountBegin,
    Mudokon_ElumUnmountEnd,
    Mudokon_ElumMountBegin,
    Mudokon_ElumFallOffEdge,
    Mudokon_ElumFall,
    Mudokon_ElumLand,
    Mudokon_ElumJumpToFall,
    Mudokon_ElumKnockback,
    Mudokon_Idle_RubEyes,
    Mudokon_Idle_StretchArms,
    Mudokon_Idle_Yawn,
    Mudokon_AO_HoistBegin_Long,
    Mudokon_AO_LandSoft_Long,
    Mudokon_Sneak,
    Mudokon_WalkToSneak,
    Mudokon_StandToCrouch,
    Mudokon_Speak1,
    Mudokon_Speak3,
    Mudokon_Speak2,
    Mudokon_SpeakFart,
    Mudokon_StandingTurn,
    Mudokon_WalkBegin,
    Mudokon_WalkToIdle,
    Mudokon_MidWalkToIdle,
    Mudokon_StartChisel,
    Mudokon_StopChisel,
    Mudokon_WalkToRun,
    Mudokon_MidWalkToRun,
    Mudokon_RunToWalk,
    Mudokon_MidRunToWalk,
    Mudokon_RunTurnToRun,
    Mudokon_SneakToWalk,
    Mudokon_MidWalkToSneak,
    Mudokon_MidSneakToWalk,
    Mudokon_SneakBegin,
    Mudokon_SneakToIdle,
    Mudokon_MidSneakToIdle,
    Mudokon_StandToRun,
    Mudokon_FallingFromGrab,
    Mudokon_DuckKnockback,
    Mudokon_Walk,
    Mudokon_TurnWheel,
    Mudokon_TurnWheelEnd,
    Mudokon_TurnWheelBegin,

    Paramite_Attack,
    Paramite_Struggle,
    Paramite_AO_M_7_Unknown,
    Paramite_AO_M_22_Unknown,
    Paramite_WebGoingDown,
    Paramite_WebLeaveDown,
    Paramite_WebLeaveUp,
    Paramite_WebIdle,
    Paramite_WebGrab,
    Paramite_WebGoingUp,
    Paramite_Eating,
    Paramite_Death,
    Paramite_Idle,
    Paramite_WalkBegin,
    Paramite_JumpUpMidair,
    Paramite_JumpUpLand,
    Paramite_JumpUpBegin,
    Paramite_Walking,
    Paramite_RunningAttack,
    Paramite_Running,
    Paramite_Hop,
    Paramite_PreHiss,
    Paramite_AllOYaGameSpeakBegin,
    Paramite_GameSpeakEnd,
    Paramite_GameSpeakBegin,
    Paramite_Turn,
    Paramite_Unused,
    Paramite_WalkRunTransition,
    Paramite_WalkEnd,
    Paramite_RunBegin,
    Paramite_RunEnd,
    Paramite_Falling,
    Paramite_RopePull,
    Paramite_CloseAttack,
    Paramite_Landing,
    Paramite_Unused2,
    Paramite_Knockback,
    Paramite_SurpriseWeb,
    Paramite_Hiss,
    Paramite_PostHiss,

    Scrab_FeedToGulp,
    Scrab_Stamp,
    Scrab_DeathEnd,
    Scrab_Feed,
    Scrab_StandToFeed,
    Scrab_Idle,
    Scrab_Jump,
    Scrab_LegKick,
    Scrab_HopBegin,
    Scrab_Landing,
    Scrab_Knockback,
    Scrab_Shriek,
    Scrab_Run,
    Scrab_RunToStand,
    Scrab_HowlBegin,
    Scrab_HowlEnd,
    Scrab_Turn,
    Scrab_JumpAndRunToFall,
    Scrab_StandToWalk,
    Scrab_StandToRun,
    Scrab_WalkToStand,
    Scrab_WalkToFall,
    Scrab_WalkToRun,
    Scrab_RunToWalk,
    Scrab_GetEaten,
    Scrab_Empty,
    Scrab_GulpToStand,
    Scrab_AttackLunge,
    Scrab_DeathBegin,
    Scrab_AO_ToFall,
    Scrab_AO_ToFeed,
    Scrab_AO_M_19_Unused,
    Scrab_Walk,
    Scrab_AttackSpin,
    Scrab_ScrabBattleAnim,

        Crawling_Slig_Idle,
        Crawling_Slig_UsingButton,
        Crawling_Slig_WakingUp,
        Crawling_Slig_Crawling,
        Crawling_Slig_StartFalling,
        Crawling_Slig_Falling,
        Crawling_Slig_Landing,
        Crawling_Slig_ToShakingToIdle,
        Crawling_Slig_Speaking,
        Crawling_Slig_Snoozing,
        Crawling_Slig_PushingWall,
        Crawling_Slig_TurnAround,
        Crawling_Slig_Shaking,
        Crawling_Slig_Empty,
        Crawling_Slig_ShakingToIdle,
        Crawling_Slig_EndCrawling,
        Crawling_Slig_IdleToPushingWall,
        Crawling_Slig_EndPushingWall,

    Slig_Beat,
    Slig_OutToFall,
    Slig_LandingFatal,
    Slig_Smash,
    Slig_LandingSoft,
    Slig_FallingInitiate,
    Slig_KnockbackToStand,
    Slig_Idle,
    Slig_ShootToStand,
    Slig_Knockback,
    Slig_PullLever,
    Slig_LiftGripping,
    Slig_LiftUp,
    Slig_LiftDown,
    Slig_LiftGrip,
    Slig_LiftUngrip,
    Slig_PossessShake,
    Slig_ReloadGun,
    Slig_Running,
    Slig_StandToRun,
    Slig_SlidingToStand,
    Slig_SlidingTurn,
    Slig_Shoot,
    Slig_Recoil,
    Slig_ShootZ,
    Slig_ShootZtoStand,
    Slig_SteppingToStand,
    Slig_Sleeping,
    Slig_Speak1,
    Slig_Speak2,
    Slig_Speak3,
    Slig_Speak4,
    Slig_TurnAroundStanding,
    Slig_Walking,
    Slig_StandToWalk,
    Slig_SlidingTurnToWalk,
    Slig_SlidingTurnToRun,
    Slig_StandingToStep,
    Slig_Gamespeak,
    Slig_WalkToStand,
    Slig_SleepingToStand,

    CrawlingSligButton,
    CrawlingSligButtonUse,
    BloodDrop,
    SpotLight,
    ShrykullStart,
    ShrykullTransform,
    ShrykullDetransform,
    NormalMudIcon,
    AngryMudIcon,
    HappyMudIcon,

    MenuHighlight_ButtonRemapSquare,
    MenuHighlight_Circle,
    MenuHighlight_Triangle,
    MenuHighlight_Square,

    MenuDoor,

    AbeIntro,

    MenuAbeSpeak_Laugh,
    MenuAbeSpeak_WhistleHigh,
    MenuAbeSpeak_WhistleLow,
    MenuAbeSpeak_Fart,
    MenuAbeSpeak_Hello,
    MenuAbeSpeak_Idle,
    MenuAbeSpeak_IdleBlink,
    MenuAbeSpeak_Ok,

    MenuAbeSpeak_FollowMe,
    MenuAbeSpeak_Wait,
    MenuAbeSpeak_Work,
    MenuAbeSpeak_Anger,
    MenuAbeSpeak_AllYa,
    MenuAbeSpeak_Sympathy,
    MenuAbeSpeak_StopIt,
    MenuAbeSpeak_Chant,
    MenuAbeSpeak_ChantEnd,
    MenuAbeSpeak_Goodbye,

    MenuGlukkonSpeak_Idle,
    MenuGlukkonSpeak_IdleVariation,
    MenuGlukkonSpeak_Hey,
    MenuGlukkonSpeak_Commere,
    MenuGlukkonSpeak_StayHere,
    MenuGlukkonSpeak_DoIt,
    MenuGlukkonSpeak_KillEm,
    MenuGlukkonSpeak_AllOYa,
    MenuGlukkonSpeak_Help,
    MenuGlukkonSpeak_Laugh,

    MenuScrabSpeak_Idle,
    MenuScrabSpeak_IdleVariation,
    MenuScrabSpeak_ShredPower,
    MenuScrabSpeak_Howl,

    MenuParamiteSpeak_Howdy,
    MenuParamiteSpeak_Idle,
    MenuParamiteSpeak_IdleVariation,
    MenuParamiteSpeak_Cmon,
    MenuParamiteSpeak_Stay,
    MenuParamiteSpeak_DoIt,
    MenuParamiteSpeak_Attack,
    MenuParamiteSpeak_AllAYa,

    MenuSligSpeak_Idle,
    MenuSligSpeak_Hi,
    MenuSligSpeak_HereBoy,
    MenuSligSpeak_Freeze,
    MenuSligSpeak_GetEm,
    MenuSligSpeak_SmoBs,
    MenuSligSpeak_Bs,
    MenuSligSpeak_LookOut,
    MenuSligSpeak_Laugh,

    Slog_MoveHeadUpwards,
    Slog_StartFastBarking,
    Slog_AngryBark,
    Slog_Dying,
    Slog_Eating,
    Slog_Fall,
    Slog_Idle,
    Slog_Growl,
    Slog_AO_M_12_Unknown,
    Slog_JumpUpwards,
    Slog_JumpForwards,
    Slog_Run,
    Slog_StopRunning,
    Slog_SlideTurn,
    Slog_Scratch,
    Slog_Sleeping,
    Slog_MoveHeadDownwards,
    Slog_WakeUp,
    Slog_TurnAround,
    Slog_StartWalking,
    Slog_EndWalking,
    Slog_Land,
    Slog_Unused,
    Slog_EndFastBarking,
    Slog_Walk,

    Slurg_Burst,
    Slurg_Move,
    Slurg_Turn_Around,

    Background_Glukkon_Idle,
    Background_Glukkon_Laugh,
    Background_Glukkon_KillHim1,
    Background_Glukkon_KillHim2,
    Background_Glukkon_Dying,
    BellHammer_Idle,
    BellHammer_Smashing,
    ObjectShadow,

    Elum_Idle,
    Elum_WalkLoop,
    Elum_Turn,
    Elum_WalkToIdle,
    Elum_MidWalkToIdle,
    Elum_IdleToWalk2,
    Elum_IdleToWalk1,
    Elum_ToYell,
    Elum_Yell,
    Elum_Unknown1,
    Elum_RunTurn,
    Elum_RunTurnToWalk,
    Elum_Speak,
    Elum_Land,
    Elum_RunOffEdge,
    Elum_WalkOffEdge,
    Elum_JumpToFall,
    Elum_LickingHoney,
    Elum_LickingToStruggle,
    Elum_AbeMountingEnd,
    Elum_AbeUnmountingEnd,
    Elum_BeesStruggling,
    Elum_HopBegin,
    Elum_HopMid,
    Elum_HopLand,
    Elum_RunJumpBegin,
    Elum_RunJumpMid,
    Elum_RunJumpLand,
    Elum_RunLoop,
    Elum_RunSlideStop,
    Elum_RunTurnToRun,
    Elum_IdleToRun,
    Elum_WalkToRun,
    Elum_MidWalkToRun,
    Elum_RunToWalk,
    Elum_MidRunToWalk,
    Elum_ScratchBegin,
    Elum_ScratchLoop,
    Elum_ScratchEnd,
    Elum_ScratchUnknown,
    Elum_MountUnmountBegin,
    Elum_Knockback,

    BirdPortal_TerminatorGrow,
    BirdPortal_Sparks,
    BirdPortal_Flash,
    PullRingRope_Idle,
    PullRingRope_UseEnd,
    PullRingRope_UseBegin,
    Bone,
    BoneBag_Idle,
    BoneBag_HardHit,
    BoneBag_SoftHit,
    Drill_Horizontal_Off,
    Drill_Horizontal_On,
    Drill_Vertical_Off,
    Drill_Vertical_On,
    Bullet_Shell,
    BigChime,
    BigChime_Moving,
    MediumChime,
    MediumChime_Moving,
    SmallChime,
    SmallChime_Moving,
    Chime_Ball,
    Circular_Fade,
    DeathFlare_1,
    DeathFlare_2,
    HoistRock1,
    HoistRock2,
    HoistRock3,
    AO_HoistRock1,
    AO_HoistRock2,
    AO_HoistRock3,
    RuptureFarms_HoistRock1,
    RuptureFarms_HoistRock2,
    RuptureFarms_HoistRock3,
    Dust_Particle,
    Loading_Icon,
    Loading_Icon2,
    Vaporize_Particle,
    ShootingFire_Particle,
    ChantOrb_Particle,
    SquibSmoke_Particle,
    Explosion_Rocks,
    Explosion_Sticks,
    //Explosion_Skull,
    //Explosion_Bone,
    Mine_Flash,
    OptionChantOrb_Particle,
    ShootingZFire_Particle,
    WaterSplash,
    BirdPortal_TerminatorShrink,
    BirdPortal_TerminatorIdle,
    WaterDrop,
    BrewMachine_Button,
    CrawlingSligLocker_Closed,
    CrawlingSligLocker_Open,
    LiftPlatform_Mines,
    LiftBottomWheel_Mines,
    LiftTopWheel_Mines,
    LiftPlatform_Necrum,
    LiftBottomWheel_Necrum,
    LiftTopWheel_Necrum,
    Door_Barracks_Closed,
    Door_Barracks_Open,
    Door_BarracksMetal_Closed,
    Door_BarracksMetal_Open,
    Door_Bonewerkz_Closed,
    Door_Bonewerkz_Open,
    Door_Brewery_Closed,
    Door_Brewery_Open,
    Door_Feeco_Closed,
    Door_Feeco_Open,
    Fire,
    Door_FireBackgroundGlow,
    Door_Lock_Idle,
    Door_Lock_Open,
    Door_Mines_Closed,
    Door_Mines_Open,
    Door_Temple_Closed,
    Door_Temple_Open,
    Door_Train_Closed,
    Door_Train_Closing,
    Door_RuptureFarms_Open,
    Door_RuptureFarms_Closed,
    Door_Lines_Open,
    Door_Lines_Closed,
    Door_Forest_Closed,
    Door_Forest_Open,
    HubDoor_Forest_Closed,
    HubDoor_Forest_Open,
    FinalTestDoor_Forest_Open,
    FinalTestDoor_Forest_Closed,
    Door_Desert_Open,
    Door_Desert_Closed,
    HubDoor_Desert_Open,
    HubDoor_Desert_Closed,
    FinalTestDoor_Desert_Open,
    FinalTestDoor_Desert_Closed,
    Dove_Idle,
    Dove_Flying,
    Electric_Wall,
    AirExplosion,
    GroundExplosion,
    AirExplosion_Small,
    FallingCrate_Falling,
    FallingCrate_Waiting,
    AE_FallingRock_Falling,
    AE_FallingRock_Waiting,
    AO_FallingRock_Falling,
    AO_FallingRock_Waiting,
    FallingMeat_Waiting,
    FallingMeat_Falling,
    Fart,
    Foot_Switch_Bonewerkz_Idle,
    Foot_Switch_Bonewerkz_Pressed,
    Foot_Switch_Industrial_Idle,
    Foot_Switch_Industrial_Pressed,
    Foot_Switch_Temple,
    Foot_Switch_Temple_Pressed,
    Foot_Switch_Vault_Idle,
    Foot_Switch_Vault_Pressed,
    Grenade,
    BoomMachine_Button_Off,
    BoomMachine_Button_On,
    BoomMachine_Nozzle_Idle,
    BoomMachine_Nozzle_DropGrenade,
    HintFly,
    Honey,
    Honey_Drip,
    HoneySack_Hanging,
    HoneySack_Falling,
    HoneySack_FallingToSmashed,
    HoneySack_OnGround,
    AE_Rope,
    Meat,
    MeatSack_Idle,
    MeatSack_Hit,
    MeatSaw_Idle,
    MeatSaw_Moving,
    MeatSawMotor,
    Mine,
    MotionDetector_Flare,
    MotionDetector_Laser,
    MovingBomb,
    ParamiteWeb,
    Pullring_Desert_Idle,
    Pullring_Desert_UseBegin,
    Pullring_Desert_UseEnd,
    Pullring_Farms_Idle,
    Pullring_Farms_UseBegin,
    Pullring_Farms_UseEnd,
    Rock,
    RockSack_Idle,
    RockSack_SoftHit,
    RockSack_HardHit,
    Rope_R1,
    Rope_Lines,
    Security_Claw_Lower_Open,
    Security_Claw_Lower_Close,
    Security_Claw_Lower_Idle,
    Security_Claw_Upper_Rotating,
    Security_Claw_Upper_NoRotation,
    Security_Door_Idle,
    Security_Door_Speak,
    Security_Orb,
    Slam_Door_Industrial_Closed,
    Slam_Door_Industrial_Closing,
    Slam_Door_Industrial_Opening,
    Slam_Door_Vault_Closed,
    Slam_Door_Vault_Closing,
    Slam_Door_Vault_Opening,
    SlapLock_Initiate,
    SlapLock_Punched,
    SlapLock_Shaking,
    AE_ZapSpark,
    Star,
    Status_Light_Green,
    Status_Light_Red,
    Stone_Ball,
    Stone_Ball_Rolling,
    Stone_Ball_Stopper,
    Swinging_Ball_Fast,
    Swinging_Ball_Normal,
    Swinging_Ball_Slow,
    Switch_Idle,
    Switch_Pull_Left,
    Switch_Pull_Release_Left,
    Switch_Pull_Right,
    Switch_Pull_Release_Right,

    RuptureFarms_Switch_Idle,
    RuptureFarms_Switch_Pull_Left,
    RuptureFarms_Switch_Pull_Release_Left,
    RuptureFarms_Switch_Pull_Right,
    RuptureFarms_Switch_Pull_Release_Right,

    Lines_Switch_Idle,
    Lines_Switch_Pull_Left,
    Lines_Switch_Pull_Release_Left,
    Lines_Switch_Pull_Right,
    Lines_Switch_Pull_Release_Right,

    TimedMine_Activated,
    TimedMine_Idle,
    Tortured_Mudokon,
    Tortured_Mudokon_Released,
    Tortured_Mudokon_Zap,
    Tortured_Mudokon_Tears,
    Trap_Door_Closed,
    Trap_Door_Closing,
    Trap_Door_Open,
    Trap_Door_Opening,
    Trap_Door_Tribal_Closed,
    Trap_Door_Tribal_Closing,
    Trap_Door_Tribal_Open,
    Trap_Door_Tribal_Opening,
    R1_TrapDoor_Open,
    R1_TrapDoor_Closed,
    R1_TrapDoor_Opening,
    R1_TrapDoor_Closing,
    Lines_TrapDoor_Open,
    Lines_TrapDoor_Closed,
    Lines_TrapDoor_Opening,
    Lines_TrapDoor_Closing,
    Desert_TrapDoor_Open,
    Desert_TrapDoor_Closed,
    Desert_TrapDoor_Opening,
    Desert_TrapDoor_Closing,
    UXB_Active,
    UXB_Disabled,
    UXB_Toggle,
    Bomb_Flash,
    Bomb_RedGreenTick,
    Well_Leaf,
    Work_Wheel_Idle,
    Work_Wheel_Turning,
    Zap_Line_Blue,
    Zap_Line_Red,
    ChantOrb_Particle_Small,
    GoldGlow,
    GreenGlow,
    FlintGlow,
    GreenDoorLight,
    GreenHubLight,
    RedDoorLight,
    RedHubLight,
    FlintLock_Hammers_Activating,
    FlintLock_Hammers_Disabled,
    FlintLock_Gourd,
    LiftPlatform_Forest,
    LiftPlatform_RuptureFarms,
    LiftPlatform_Desert,
    LiftPlatform_Desert2,
    LiftPlatform_Lines,
    LiftBottomWheel_RuptureFarms,
    LiftBottomWheel_Lines,
    LiftBottomWheel_Forest,
    LiftBottomWheel_Desert,
    LiftBottomWheel_Desert2,
    LiftTopWheel_RuptureFarms,
    LiftTopWheel_Lines,
    LiftTopWheel_Forest,
    LiftTopWheel_Desert,
    LiftTopWheel_Desert2,
    Scoopz,

    Anim_Tester, // For animation testing with the TestAnimation.cpp, ignore this.
};

enum class PalId
{
    Default, // Don't override the anims default pal
    BlindMud,
};

struct BgAnimDetails final
{
    const char_type* mBanName;
    s32 mFrameTableOffset;
    s16 mMaxW;
    s16 mMaxH;
};

struct CombinedBgAnimRecord final
{
    s32 mBgAnimId;
    BgAnimDetails mAEData;
    BgAnimDetails mAOData;
};

struct BgAnimRecord final
{
    s32 mBgAnimId;
    const char_type* mBanName;
    s32 mFrameTableOffset;
    s16 mMaxW;
    s16 mMaxH;
};

struct PalDetails final
{
    const char_type* mBanName;
    s32 mResourceId;
};

struct CombinedPalRecord final
{
    PalId mId;
    PalDetails mAEData;
    PalDetails mAOData;
};

struct PalRecord final
{
    PalId mId;
    const char_type* mBanName;
    s32 mResourceId;
};

struct AnimDetails final
{
    const char_type* mBanName;
    s32 mFrameTableOffset;
    s16 mMaxW;
    s16 mMaxH;
    s32 mResourceId;
    PalId mPalOverride;
};

struct AnimRecord final
{
    AnimId mId;
    const char_type* mBanName;
    s32 mFrameTableOffset;
    s16 mMaxW;
    s16 mMaxH;
    s32 mResourceId;
    PalId mPalOverride;
};

struct CombinedAnimRecord final
{
    AnimId mId;
    AnimDetails mAEData;
    AnimDetails mAOData;
};

const PalRecord PalRec(PalId toFind);
const AnimRecord AnimRec(AnimId toFind);
bool AnimRecExists(bool isAe, AnimId toFind);
std::vector<CombinedAnimRecord> GetAnimRecords();
const AnimRecord AnimRecFrameTable(int frameTableOffset, int resourceId, bool isAe);
const BgAnimRecord BgAnimRec(s32 toFind);
void FrameTableOffsetExists(int frameTableOffset, bool isAe, int maxW, int maxH);
bool FrameTableOffsetExists(int frameTableOffset, bool isAe);

namespace AO 
{
    const PalRecord PalRec(PalId toFind);
    const AnimRecord AnimRec(AnimId toFind);
    const BgAnimRecord BgAnimRec(s32 toFind);
}

// AE background animation id's
enum AEBgAnimID
{
    BG_Brewery_Barrel1 = 1206,
    BG_Brewery_Barrel2 = 1207,
    BG_Mine_Fan = 1201,
    BG_Feeco_Small_Fan = 1202,
    BG_Heat_Extractor_Fan = 1204,
};

// AO background animation id's
enum AOBgAnimID
{
    BG_None = 0,
    BG_RuptureFarms_Barrel1 = 6016,
    BG_RuptureFarms_Barrel2 = 6017,
    BG_RuptureFarms_Barrel3 = 6018,
    BG_RuptureFarms_Barrel4 = 6019,
    BG_RuptureFarms_Barrel5 = 6020,
    BG_RuptureFarms_Barrel6 = 6021,
    BG_RuptureFarms_Barrel7 = 6022,
    BG_RuptureFarms_Barrel8 = 6023,
    BG_RuptureFarms_Barrel9 = 6024,
    BG_RuptureFarms_Barrel10 = 6025,
    BG_RuptureFarms_Barrel11 = 6026,

    BG_Campfire1 = 7000,
    BG_Campfire2 = 7001,
    BG_Campfire3 = 7003,
    BG_Campfire4 = 7004,
    BG_Campfire5 = 7006,
    BG_Campfire6 = 7010,
    BG_Campfire7 = 7007,
    BG_Campfire8 = 7008,
    BG_Campfire9 = 7002,

    BG_Well1 = 5002,
    BG_Well2 = 5006,
    BG_Well3 = 5007,
    BG_Well4 = 5003,
    BG_Well5 = 5005,
    BG_Well6 = 5004,
    BG_Well7 = 5001,
    BG_Well8 = 5000,
    BG_Well9 = 3052,
    BG_Well10 = 3039,
    BG_Well11 = 3040,
    BG_Well12 = 3016,
    BG_Well13 = 3017,
    BG_Well14 = 3018,
    BG_Well15 = 3051,
    BG_Well16 = 3050,
    BG_Well17 = 3041,
    BG_Well18 = 3027,
    BG_Well19 = 3047,
    BG_Well20 = 3048,
    BG_Well21 = 3049,
    BG_Well22 = 3031,
    BG_Well23 = 3032,
    BG_Well24 = 3038,
    BG_Well25 = 3037,
    BG_Well26 = 4022,
    BG_Well27 = 4023,
    BG_Well28 = 4012,
    BG_Well29 = 4013,
    BG_Well30 = 4027,
    BG_Well31 = 4009,
    BG_Well32 = 4011,
    BG_Well33 = 4024,
    BG_Well34 = 4025,
    BG_Well35 = 4026,
    BG_Well36 = 4010,
    BG_Well37 = 4028,
    BG_Well38 = 4015,
    BG_Well39 = 4014,
    BG_Well40 = 4017,
    BG_Well41 = 4018,
    BG_Well42 = 4019,
    BG_Well43 = 4016,
    BG_Well44 = 4021,
    BG_Well45 = 4006,
    BG_Well46 = 4007,
    BG_Well47 = 4008,
    BG_Well48 = 4030,
    BG_Well49 = 4029,
    BG_Well50 = 4031,
    BG_Well51 = 4032,
    BG_Well52 = 4033,
    BG_Well53 = 4034,
    BG_Well54 = 1200,
    BG_Well55 = 1050,
    BG_Well56 = 1018,
    BG_Well57 = 1017,
    BG_Well58 = 1038,
    BG_Well59 = 1039,
    BG_Well60 = 2013,
    BG_Well61 = 2014,
    BG_Well62 = 2036,
    BG_Well63 = 2030,
    BG_Well64 = 2031,
    BG_Well65 = 2029,
    BG_Well66 = 2022,
    BG_Well67 = 2041,
    BG_Well68 = 2042,
    BG_Well69 = 2040,
    BG_Well70 = 2043,
    BG_Well71 = 2044,
    BG_Well72 = 2045,
    BG_Well73 = 2046,
    BG_Well74 = 2049,
    BG_Well75 = 2047,
    BG_Well76 = 2048,
    BG_Well77 = 2051,
    BG_Well78 = 2052,
    BG_Well79 = 2050,
    BG_Well80 = 2053,
    BG_Well81 = 2054,

    BG_Windmill1 = 1023,
    BG_Windmill2 = 1024,
    BG_Windmill3 = 1031,

    BG_RedEyes1 = 2020,
    BG_RedEyes2 = 2021,

    BG_PinkFlame1 = 6014,
    BG_PinkFlame2 = 6015,
};

// TODO: remove unused res id's from AO when both AO and AE res id's were in this enum
enum AEResourceID
{
    kUnknownResID_0 = 0,
    kDemoResID = 1,
    kLcdfontResID = 2,
    kAbebasicResID = 10,
    kAbepullResID = 11,
    kAbepickResID = 12,
    kAbebombResID = 13,
    kAbethrowResID = 14,
    kAberingResID = 17,
    kAbesmashResID = 19,
    kAbefallResID = 20,
    kAbestoneResID = 21,
    kAbecrushResID = 22,
    kAbeblowResID = 25,
    kAbeknbkResID = 26,
    kAbeknfdResID = 27,
    kAbeknokzResID = 28,
    kAbebordResID = 34,
    kAbeeyesResID = 35,
    kAbebuttResID = 36,
    kAbenutsResID = 37,
    kAbepnlkResID = 38,
    kAbewizzResID = 39,
    kAbetalkResID = 40,
    kAbehoistResID = 42,
    kAbeedgeResID = 43,
    kAbeswingResID = 44,
    kAbedoorResID = 45,
    kAbewellResID = 47,
    kAbeommResID = 48,
    kAbeliftResID = 53,
    kAbebsic1ResID = 55,
    kDovbasicResID = 60,
    kHintflyResID = 62, // remove?
    kBatBasicResID = 63, // remove?
    kFireFlyResID = 64,
    kDovshootResID = 66,

    kAbeCarResId = 113,
    kAneprmntResID = 115,
    kAbemorphResID = 117,
    kAbegasResID = 118,
    kShrmorphResID = 121,
    kUnknownResID_125 = 125,
    kAbespeakResID = 130,
    kAbeintroResID = 131,
    kAbespek2ResID = 132,
    kSligSpeakResID = 140,
    kGlukkonSpeakResID = 150,
    kParamiteSpeakResID = 160,
    kScrabSpeakResID = 170,

    kHappyiconResID = 201,
    kAngryiconResID = 202,
    kNormaliconResID = 203,

    kBgexpldResID = 300,
    kExplo2ResID = 301,
    kAlrmFlarResID = 302,
    kDustResID = 303,
    kHubFireResID = 304, // remove?
    kVaporResID = 305,
    kSlurgResID = 306,
    kGreeterResID = 307,
    kOmmflareResID = 312,
    kPortalTerminatorResID = 313,
    kSparksResID = 314,
    kSpotliteResID = 316,
    kSkiddustResID = 317,
    kSmokeResID = 318,
    kBigflashResID = 319,
    kWorkwhelResID = 320,
    kUnknownResID_333 = 333,
    kWellLeafResID = 341,
    kSquibResID = 343,
    kZflashResID = 344,
    kFxflareResID = 348,
    kDeathFlareResID = 349,
    kAberockResID = 350,
    kPortliteResID = 351,
    kPortlitResID = 353,
    kSquibSmokeResID = 354,
    kSplineResID = 355,
    kHoistRocks = 357,
    kStickGibResID = 358,
    kShellResID = 360,
    kGlowResID = 361,
    kMetalGibResID = 365,
    kBloodropResID = 366,
    kOptionFlareResID = 367,
    kWaterDropResID = 368,
    kSplashResID = 369,
    kSmallExplo2ResID = 372,
    kStatusLightResID = 373,
    kDoorLockResID_374 = 374,
    kTorturedMudTearsResID = 375,

    kSlgbasicResID = 412,
    kSlgsleepResID = 413,
    kSlgknbkResID = 414,
    kSlgedgeResID = 415,
    kSlgsmashResID = 416,
    kSlgzshotResID = 417,
    kSlgknfdResID = 418,
    kSlgleverResID = 419,
    kSlgliftResID = 420,
    kSligBlowResID = 423,
    kSlgbeatResID = 426,
    kCrawlingSligLockerResID_448 = 448,
    kCrawlingSligResID_449 = 449,
    kFlySligResID = 450,

    kSlingmudResID_508 = 508, // remove?
    kMudscrubResID = 510,
    kMudchslResID = 511,
    kMudidleResID = 512,
    kMudoduckResID = 514,
    kAbeworkResID = 515,
    kMudltusResID = 516, // remove?
    kMudbtlnkResID = 517,
    kTorturedMud = 518,
    kMudangryResID = 530,
    kMudsadResID = 531,
    kMudwiredResID = 532,
    kMudblindResID = 533,
    kMudsickResID = 534,
    kMudombieResID = 550,
    kDogbasicResID = 570,
    kDogrstnResID = 571,
    kDogattkResID = 572,
    kDogknfdResID = 573,
    kDogidleResID = 574,
    kSlogBlowResID = 576,
    kFleeBlowResID_580 = 580,
    kBoneBagResID_590 = 590,
    kBoneResID = 591,

    kArjbasicResID = 600,
    kArjeatResID = 601,
    kArjclimbResID = 604,
    kArjpumpResID = 605,
    kArjscrchResID = 609,
    kArjwaspResID = 610,
    kArjjumpResID = 611,
    kArjponceResID = 614,
    kArjfalrkResID = 615,

    kArsbasicResID = 700,
    kArseatResID = 701,
    kArsprceResID = 702,
    kArsskwrResID = 703,
    kArswhirlResID = 704,
    kArshowlResID = 705,
    kArsdanceResID = 706,
    kArssackResID = 707,
    kArsdeadResID = 708,
    kArsroarResID = 709,
    kArsgrwlResID = 710,
    kArschewResID = 711,
    kArscrshResID = 713,

    kGlkbasicResID = 800,
    kGlukkonResID_801 = 801,
    kGlukAslikResID = 802,
    kAslikResID_803 = 803,
    kGlukDripikResID = 804,
    kDripikResID_805 = 805,
    kGlukPhlegResID = 806,
    kPhlegResID_807 = 807,
    kGlukredResID = 825,
    kGlukgrenResID = 826,
    kGlukblueResID = 827,
    kGlukaquaResID = 828,

    kFleechResID = 900,

    kRopesResID = 1000,
    kLiftWheelsResID = 1001,
    kP2c2bagResID = 1002,
    kP5c1wlkResID = 1003,
    kP6c1trapResID = 1004,
    kBombResID = 1005,
    kGrenflshResID = 1006,
    kP4c5hiveResID = 1008,
    kSwitchResID = 1009,
    kBombflshResID = 1011,
    kPullringResID = 1014,
    kP6c4tubeResID = 1017,
    kP7c1spikResID = 1018,
    kHammerResID = 1019, // remove?
    kCrystalResID = 1030,
    kLiftResID = 1032,
    kLandmineResID = 1036,
    kUXBResID = 1037,
    kBGStarResID = 1039, // remove?
    kMineflshResID = 1040,
    kP1c3tubeResID = 1050,
    kGhostTrpResID_1053 = 1053,
    kCrawlingSligButtonResID_1057 = 1057,
    kDebrisID00ResID = 1105,
    kTubeResID = 1200,
    kScoopzResID = 1205,

    kTestanimResID = 2000,
    kF2zballResID = 2001, // remove?
    kF2stnbalResID = 2002, // remove?
    kF2stnsckResID = 2003, // remove?
    kChimeResID = 2005, // remove?
    kMaimGameResID = 2006,
    kF2rockResID = 2007,
    kF2eyeorbResID = 2008,
    kPupilResID = 2009,
    kTriggerResID = 2010,
    kF2p3dorResID = 2012, // remove?
    kTrainDoorResID_2013 = 2013,
    kFlintLockResID = 2017, // remove?
    kRockdoorResID = 2018, // remove?
    kHubdoorResID = 2019,
    kSlamResID = 2020,
    kMeatResID = 2023,
    kF2ChimeBallResID = 2024, // remove?
    kF2ChimeRackResID = 2025,
    kGourdResID = 2028, // remove?
    kWebResID = 2034,
    kObjectShadowResID = 2035,

    kMovebombResID = 3006,

    kMeatSackResID = 4002,

    kElecwallResID = 6000,
    kMotionResID = 6001,
    kMflareResID = 6002,
    kMeatSawResID = 6003, // remove?
    kDrillResID = 6004,
    kGrenadeResID = 6005,
    kR1sdoorResID = 6006,
    kR1buttonResID = 6008,
    kR1bpipeResID = 6009,
    kGreenGlowResID_6010,
    kGoldGlowResID_6011 = 6011,
    kBayrollResID_6013 = 6013,
    kBrewButtonResID_6016 = 6016,
    kEvilFartResID = 6017,
    kR1sdosResID_6027 = 6027,
    kFlintGlowResID_6028 = 6028,
    kGreenDoorLightResID_6031 = 6031,
    kRedDoorLightResID_6032 = 6032,
    kTwnkleResID = 7012,

    kControlsResID = 8000,
    kMenuDoorResID = 8001,
    kHighliteResID = 8002,
    kHighlitePalResID = 8003,
    kLoadingResID = 8010,
};

namespace AO {

enum AOResourceID
{
    kLcdfontAOResID = 2,
    kAbebasicAOResID = 10,
    kAbepullAOResID = 11,
    kAbepickAOResID = 12,
    kAbebombAOResID = 13,
    kAbethrowAOResID = 14,
    kAbewaspAOResID = 16,
    kAbesmashAOResID = 19,
    kAbefallAOResID = 20,
    kAbestoneAOResID = 21,
    kAbeblowAOResID = 25,
    kAbeknbkAOResID = 26,
    kAbeknfdAOResID = 27,
    kAbeknokzAOResID = 28,
    kAbeyawnAOResID = 31,
    kAberubAOResID = 32,
    kAbesizeAOResID = 33,
    kAbehoistAOResID = 42,
    kAbeedgeAOResID = 43,
    kAbedoorAOResID = 45,
    kAbewellAOResID = 47,
    kAbeommAOResID = 48,
    kAbeliftAOResID = 53,
    kAbebsic1AOResID = 55,
    kDovbasicAOResID = 60,
    kBatBasicAOResID = 63,
    kWaspAOResID = 61,
    kHintflyAOResID = 62,

    kAbeWElmAOResID_100 = 100, // abe elum something
    kAneknbkAOResID = 106,
    kAneedgeAOResID = 108,
    kElumUnknownAOResID_110 = 110, // abe elum something
    kElumUnknownAOResID_112 = 112, // abe elum something
    kAbeANEDSMNTAOResID_113 = 113, // abe elum something
    kAneprmntAOResID = 115,
    kAbemorphAOResID = 117,
    kAbegasAOResID = 118,
    kShrmorphAOResID = 121,
    kAbespeakAOResID = 130,
    kAbeintroAOResID = 131,
    kAbespek2AOResID = 132,

    kElmbasicAOResID_200 = 200,
    kElmHoneyAOResID = 203,
    kElmWaspAOResID_204 = 204,
    kElmnknbkAOResID = 215,
    kElmfallAOResID_216 = 216,
    kElmblowAOResID_217 = 217,
    kElumRideAOResID_220 = 220,
    kElumPdmntAOResID_221 = 221,
    kElmprmntAOResID__222 = 222,
    kElumAneMountAOResID_223 = 223,
    kElumANEDSMNTAOResID_224 = 224,
    kElmaloneAOResID_230 = 230,

    kBgexpldAOResID = 300,
    kExplo2AOResID = 301,
    kHubFireAOResID = 304,
    kOmmflareAOResID = 312,
    kPortalTerminatorAOResID = 313,
    kSparksAOResID = 314,
    kSpotliteAOResID = 316,
    kBigflashAOResID = 319,
    kUnknownAOResID_333 = 333,
    kHoneyAOResID = 337,
    kWellLeafAOResID = 341,
    kZflashAOResID = 344,
    kDeathFlareAOResID = 349,
    kAberockAOResID = 350,
    kPortliteAOResID = 351,
    kPortlitAOResID = 353,
    kSquibSmokeAOResID = 354,
    kSplineAOResID = 355,
    kHoistRocksAOResID = 357,
    kStickGibAOResID = 358,
    kShellAOResID = 360,
    kGlowAOResID = 361,
    kMetalGibAOResID = 365,
    kBloodropAOResID = 366,
    kOptionFlareAOResID = 367,

    kSlgbasicAOResID = 412,
    kSlgsleepAOResID = 413,
    kSlgknbkAOResID = 414,
    kSlgedgeAOResID = 415,
    kSlgsmashAOResID = 416,
    kSlgzshotAOResID = 417,
    kSlgknfdAOResID = 418,
    kSlgleverAOResID = 419,
    kSlgliftAOResID = 420,
    kSligBlowAOResID = 423,
    kSlgbeatAOResID = 426,

    kSlingmudAOResID_508 = 508,
    kMudscrubAOResID = 510,
    kMudchslAOResID = 511,
    kMudoduckAOResID = 514,
    kMudltusAOResID = 516,
    kMudbtlnkAOResID = 517,
    kDogbasicAOResID = 570,
    kDogrstnAOResID = 571,
    kDogattkAOResID = 572,
    kDogknfdAOResID = 573,
    kDogidleAOResID = 574,
    kSlogBlowAOResID = 576,

    kArjbasicAOResID = 600,
    kArjeatAOResID = 601,
    kArjclimbAOResID = 604,
    kArjpumpAOResID = 605,
    kArjscrchAOResID = 609,
    kArjwaspAOResID = 610,
    kArjponceAOResID = 614,
    kArjfalrkAOResID = 615,

    kArsbasicAOResID = 700,
    kArseatAOResID = 701,
    kArsprceAOResID = 702,
    kArsskwrAOResID = 703,
    kArswhirlAOResID = 704,
    kArshowlAOResID = 705,
    kArsdanceAOResID = 706,
    kArsdeadAOResID = 708,
    kArsroarAOResID = 709,
    kArsgrwlAOResID = 710,
    kArschewAOResID = 711,
    kArscrshAOResID = 713,

    kGlkbasicAOResID = 800,
    kGlukredAOResID = 825,
    kGlukgrenAOResID = 826,
    kGlukblueAOResID = 827,
    kGlukaquaAOResID = 828,

    kRopesAOResID = 1000,
    kLiftWheelsAOResID = 1001,
    kP2c2bagAOResID = 1002,
    kP6c1trapAOResID = 1004,
    kBombAOResID = 1005,
    kGrenflshAOResID = 1006,
    kSwitchAOResID = 1009,
    kBombflshAOResID = 1011,
    kP2c3HiveAOResID = 1013,
    kPullringAOResID = 1014,
    kHammerAOResID = 1019,
    kLiftAOResID = 1032,
    kLandmineAOResID = 1036,
    kUXBAOResID = 1037,
    kBGStarAOResID = 1039,
    kMineflshAOResID = 1040,
    kDebrisID00AOResID = 1105,

    kF2zballAOResID = 2001,
    kF2stnbalAOResID = 2002,
    kF2stnsckAOResID = 2003,
    kChimeAOResID = 2005,
    kMaimGameAOResID = 2006,
    kF2rockAOResID = 2007,
    kF2eyeorbAOResID = 2008,
    kTriggerAOResID = 2010,
    kF2p3dorAOResID = 2012,
    kFlintLockAOResID = 2017,
    kRockdoorAOResID = 2018,
    kHubdoorAOResID = 2019,
    kMeatAOResID = 2023,
    kF2ChimeBallAOResID = 2024,
    kGourdAOResID = 2028,
    kWebAOResID = 2034,
    kObjectShadowAOResID = 2035,

    kMovebombAOResID = 3006,

    kMeatSackAOResID = 4002,

    kElecwallAOResID = 6000,
    kMotionAOResID = 6001,
    kMflareAOResID = 6002,
    kMeatSawAOResID = 6003,
    kGrenadeAOResID = 6005,
    kR1buttonAOResID = 6008,
    kR1bpipeAOResID = 6009,
    kGreenGlowAOResID_6010 = 6010,
    kGoldGlowAOResID_6011 = 6011,
    kMeatGibAOResID_6014 = 6014,
    kR1sdosAOResID_6027 = 6027,
    kFlintGlowAOResID_6028 = 6028,
    kGreenDoorLightAOResID_6031 = 6031,
    kRedDoorLightAOResID_6032 = 6032,

    kMenuDoorAOResID = 8001,
    kHighliteAOResID = 8002,
    kHighlitePalAOResID = 8003,
    kLoadingAOResID = 8010,

};

} // namespace AO
