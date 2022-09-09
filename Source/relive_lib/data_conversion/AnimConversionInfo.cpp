#include "AnimConversionInfo.hpp"
#include "../../AliveLibCommon/Sys_common.hpp"

AnimRecConversionInfo kAnimRecConversionInfo[910] = {
    { AnimId::Abe_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Abe_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Abe_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BlindMud_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::BlindMud_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::BlindMud_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Aslik_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Aslik_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Aslik_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Dripik_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Dripik_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Dripik_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Phleg_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Phleg_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Phleg_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Elum_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Meat_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Metal_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Rock_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Slig_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slog_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Stick_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },

    { AnimId::Mudokon_RunToRoll, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchSpeak, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_ToCrouchSpeak, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_FallLandDie, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_PoisonGasDeath, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ZShotRolling, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ZShot, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_DoorEnter, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_DoorExit, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_WalkOffEdge, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunOffEdge, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_SneakOffEdge, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_HandstoneEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_GrenadeMachineUse, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_HandstoneBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_LedgeHang, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_LedgeDescend, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RingRopePullHang, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_LedgeHangWobble, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_LedgeAscend, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_HopMid, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_HopLand, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_HopBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_PickupItem, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RollingKnockback, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RollingKnockForward, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_LiftGrabIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_LiftUseDown, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_AO_LiftUse, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_LiftGrabEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_LiftGrabBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_LiftUseUp, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_MineCarEnter, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_MineCarExit, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_PushWall, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RockThrowStandingHold, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RockThrowStandingEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RockThrowCrouchingHold, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RockThrowCrouchingThrow, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RockThrowStandingThrow, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Rolling, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RollingEnd_Unused, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RollBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_DunnoEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_EndShrykull, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_ToShrykull, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_SlapBomb, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sorry, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_AfterSorry, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_ToSpeak, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Null, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_RunJumpLand, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunTurnToWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunJumpLandRun, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunJumpLandWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_TurnToRun, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_HopToFall, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunJumpToFall, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RollOffEdge, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Well_Idle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_WellBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_JumpIntoWell, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },

    // Should probably make another group called Mudokon and/or remove the Abe group since they share
    // the same animations
    {AnimId::Mudokon_Chant, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_ChantEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    { AnimId::Mudokon_Chisel, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_CrouchIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ToDuck, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchToStand, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchTurn, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Duck, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_DuckToCrouch, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Fall, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_KnockbackGetUp, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_SlapOwnHead, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_HoistIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_LandSoft, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_HoistBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Idle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_KnockForward, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Knockback, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_LeverUse, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Run, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunSlideStop, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunSlideTurn, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunJumpMid, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunJumpBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchScrub, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_DunnoBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Punch, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_Sling_Idle, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sling_Angry, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sling_Speak, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sling_ShootStart, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sling_ShootEnd, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sling_AngryToIdle, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandScrubLoop, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandScrubLoopToPause, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandScrubPauseToLoop, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandScrubPause, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_IdleToStandScrub, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandScrubToIdle, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Struggle, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StruggleToCrouchChant, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchChant, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchChantToStruggle, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_DunnoMid, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_M_15_Null, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchSpeak1, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchSpeak2, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumWalkLoop, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumIdle, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumRunSlideStop, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumRunTurn, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_AO_M_106_Null, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumHopBegin, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumHopMid, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumHopLand, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumRunJumpBegin, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumRunJumpMid, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumRunJumpLand, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumTurn, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumRunLoop, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumSpeak, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_AO_M_116_Null, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumWalkBegin, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumRunBegin, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_AO_M_119_Null, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumRunToWalk, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumMidRunToWalk, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumRunTurnToRun, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumRunTurnToWalk, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumWalkEnd, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumMidWalkEnd, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumBeesStruggling, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumMountEnd, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumUnmountBegin, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumUnmountEnd, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumMountBegin, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumFallOffEdge, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumFall, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumLand, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumJumpToFall, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_ElumKnockback, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Mudokon_Idle_RubEyes, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Idle_StretchArms, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Idle_Yawn, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_ToShrykull, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_ShrykullEnd, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_HoistBegin_Long, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_LandSoft_Long, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sneak, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_WalkToSneak, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandToCrouch, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Speak1, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Speak3, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Speak2, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_SpeakFart, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandingTurn, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_WalkBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_WalkToIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_MidWalkToIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_Null, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StartChisel, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_StopChisel, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_WalkToRun, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_MidWalkToRun, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunToWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_MidRunToWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunTurnToRun, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_SneakToWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_MidWalkToSneak, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_MidSneakToWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_SneakBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_SneakToIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_MidSneakToIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandToRun, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_FallingFromGrab, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_DuckKnockback, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Walk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_TurnWheel, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_TurnWheelEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mudokon_TurnWheelBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Bat_Flying, EAnimGroup::Bat, EReliveLevelIds::eNone, EReliveLevelIds::eStockYards, false },
    { AnimId::Bat, EAnimGroup::Bat, EReliveLevelIds::eNone, EReliveLevelIds::eStockYards, false },
    { AnimId::Bat_Unknown, EAnimGroup::Bat, EReliveLevelIds::eNone, EReliveLevelIds::eStockYards, false },

    { AnimId::Fleech_Climb, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_SettleOnGround, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_Consume, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_Fall, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_Sleeping, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_Idle, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_StopCrawling, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_StopMidCrawlCycle, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_Land, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_SleepingWithTongue, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_PatrolCry, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_RaiseHead, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_WakingUp, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_Unused, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_Knockback, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_ExtendTongueFromEnemy, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_RetractTongueFromEnemey, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_DeathByFalling, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Fleech_Crawl, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },

    {AnimId::FlyingSlig_Idle, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_MoveHorizontal, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_IdleTurnAround, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_MoveDown, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_MoveDownTurnAround, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_MoveUp, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_MoveUpTurnAround, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_PullLever, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_Speak, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_Possession, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_MoveHorizontalEnd, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    { AnimId::FlyingSlig_MoveUpStart, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_MoveHorizontalToDown, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_MoveUpToHorizontal, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_MoveDownToHorizontal, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_TurnQuick, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_IdleToHorizontal, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_BeginDownMovement, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_EndDownMovement, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_DownKnockback, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_UpKnockback, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_EndUpMovement, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_InstantUpXTurn, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_InstantDownXTurn, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_HorizontalToUpMovement, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_TurnToHorizontalMovement, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Glukkon_Aslik_KnockBack, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Idle, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Jump, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Land, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_LongLaugh, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_ChantShake, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_GetShot, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Speak1, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Speak2, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Speak3, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_KnockBackStandBegin, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_KnockBackStandEnd, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Turn, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_JumpToFall, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_WalkToFall, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Fall, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_DeathFall, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_BeginWalk, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_EndWalk, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_StandToJump, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_JumpToStand, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_WalkToJump, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_JumpToWalk, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_EndSingleStep, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Walk, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },

    { AnimId::Glukkon_Dripik_KnockBack, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Idle, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Jump, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Land, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_LongLaugh, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_ChantShake, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_GetShot, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Speak1, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Speak2, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Speak3, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_KnockBackStandBegin, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_KnockBackStandEnd, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Turn, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_JumpToFall, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_WalkToFall, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Fall, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_DeathFall, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_BeginWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_EndWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_StandToJump, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_JumpToStand, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_WalkToJump, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_JumpToWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_EndSingleStep, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Walk, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },

    { AnimId::Glukkon_Normal_KnockBack, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Idle, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Jump, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Land, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_LongLaugh, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_ChantShake, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_GetShot, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Speak1, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Speak2, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Speak3, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_KnockBackStandBegin, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_KnockBackStandEnd, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Turn, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_JumpToFall, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_WalkToFall, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Fall, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_DeathFall, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_BeginWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_EndWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_StandToJump, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_JumpToStand, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_WalkToJump, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_JumpToWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_EndSingleStep, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Walk, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },

    { AnimId::Glukkon_Phleg_KnockBack, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Idle, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Jump, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Land, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_LongLaugh, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_ChantShake, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_GetShot, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Speak1, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Speak2, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Speak3, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_KnockBackStandBegin, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_KnockBackStandEnd, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Turn, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_JumpToFall, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_WalkToFall, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Fall, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_DeathFall, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_BeginWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_EndWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_StandToJump, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_JumpToStand, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_WalkToJump, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_JumpToWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_EndSingleStep, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Walk, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },

    { AnimId::Greeter_Chase, EAnimGroup::Greeter, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Greeter_Falling, EAnimGroup::Greeter, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Greeter_Hit, EAnimGroup::Greeter, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Greeter_Moving, EAnimGroup::Greeter, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Greeter_Speak, EAnimGroup::Greeter, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Greeter_Turn, EAnimGroup::Greeter, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },

    { AnimId::Mine_Car_Closed, EAnimGroup::MineCar, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mine_Car_Open, EAnimGroup::MineCar, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mine_Car_Shake_A, EAnimGroup::MineCar, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mine_Car_Shake_B, EAnimGroup::MineCar, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mine_Car_Tread_Idle, EAnimGroup::MineCar, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mine_Car_Tread_Move_A, EAnimGroup::MineCar, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mine_Car_Tread_Move_B, EAnimGroup::MineCar, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Paramite_Attack, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_Struggle, EAnimGroup::Paramite, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },

    // TODO: this doesnt want to convert for some reason
    //{ AnimId::Paramite_AO_M_7_Unknown, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_AO_M_22_Unknown, EAnimGroup::Paramite, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_WebGoingDown, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_WebLeaveDown, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_WebLeaveUp, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_WebIdle, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_WebGrab, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_WebGoingUp, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_Eating, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Death, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Idle, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_WalkBegin, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_JumpUpMidair, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_JumpUpLand, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_JumpUpBegin, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_Walking, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_RunningAttack, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Running, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Hop, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_PreHiss, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Squawk, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_AllOYaGameSpeakBegin, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_GameSpeakEnd, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_GameSpeakBegin, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Turn, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Unused, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_WalkRunTransition, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_WalkEnd, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_RunBegin, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_RunEnd, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Falling, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_RopePull, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_CloseAttack, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_Landing, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_Unused2, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_Knockback, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Paramite_SurpriseWeb, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Hiss, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_PostHiss, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },

    { AnimId::Scrab_FeedToGulp, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Stamp, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_DeathEnd, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Scrab_Feed, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_StandToFeed, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Scrab_Idle, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Jump, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_LegKick, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_HopBegin, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Landing, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Knockback, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Scrab_Shriek, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Run, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_RunToStand, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_HowlBegin, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_HowlEnd, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Turn, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_JumpAndRunToFall, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Scrab_StandToWalk, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_StandToRun, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_WalkToStand, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_WalkToFall, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Scrab_WalkToRun, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Scrab_RunToWalk, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Scrab_GetEaten, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Empty, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Scrab_GulpToStand, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Scrab_AttackLunge, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_DeathBegin, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_AO_ToFall, EAnimGroup::Scrab, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_AO_ToFeed, EAnimGroup::Scrab, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_AO_M_19_Unused, EAnimGroup::Scrab, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Walk, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_AttackSpin, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Scrab_ScrabBattleAnim, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },

    { AnimId::CrawlingSlig_Idle, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_UsingButton, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_WakingUp, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_Crawling, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_StartFalling, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_Falling, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_Landing, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_ToShakingToIdle, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_Speaking, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_Snoozing, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_PushingWall, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_TurnAround, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_Shaking, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_Empty, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_ShakingToIdle, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_EndCrawling, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_IdleToPushingWall, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_EndPushingWall, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },

    { AnimId::Slig_Beat, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_OutToFall, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_LandingFatal, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Smash, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_LandingSoft, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_FallingInitiate, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_KnockbackToStand, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Idle, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_ShootToStand, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Knockback, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_PullLever, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Slig_LiftGripping, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_LiftUp, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_LiftDown, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_LiftGrip, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_LiftUngrip, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_PossessShake, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_ReloadGun, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Running, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_StandToRun, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_SlidingToStand, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_SlidingTurn, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Shoot, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Recoil, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_ShootZ, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_ShootZtoStand, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_SteppingToStand, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Sleeping, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Speak1, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Speak2, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Speak3, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Speak4, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_TurnAroundStanding, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Walking, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_StandToWalk, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_SlidingTurnToWalk, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_SlidingTurnToRun, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_StandingToStep, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Gamespeak, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_WalkToStand, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_SleepingToStand, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::CrawlingSligButton, EAnimGroup::CrawlingSligButton, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSligButtonUse, EAnimGroup::CrawlingSligButton, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },

    { AnimId::ShrykullStart, EAnimGroup::Shrykull, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::ShrykullTransform, EAnimGroup::Shrykull, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::ShrykullDetransform, EAnimGroup::Shrykull, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },

    { AnimId::NormalMudIcon, EAnimGroup::Icons, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::AngryMudIcon, EAnimGroup::Icons, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::HappyMudIcon, EAnimGroup::Icons, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::MenuHighlight_ButtonRemapSquare, EAnimGroup::MenuHighlight, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuHighlight_Circle, EAnimGroup::MenuHighlight, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuHighlight_Triangle, EAnimGroup::MenuHighlight, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuHighlight_Square, EAnimGroup::MenuHighlight, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },

    { AnimId::MenuAbeSpeak_Laugh, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eNone, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_WhistleHigh, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eNone, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_WhistleLow, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eNone, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Fart, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eNone, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Hello, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Idle, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_IdleBlink, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Ok, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_FollowMe, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Wait, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Work, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuAbeSpeak_Anger, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_AllYa, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuAbeSpeak_Sympathy, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuAbeSpeak_StopIt, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuAbeSpeak_Chant, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_ChantEnd, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Goodbye, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },

    { AnimId::MenuGlukkonSpeak_Idle, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_IdleVariation, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_Hey, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_Commere, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_StayHere, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_DoIt, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_KillEm, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_AllOYa, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_Help, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_Laugh, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },

    { AnimId::MenuScrabSpeak_Idle, EAnimGroup::MenuScrabSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuScrabSpeak_IdleVariation, EAnimGroup::MenuScrabSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuScrabSpeak_ShredPower, EAnimGroup::MenuScrabSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuScrabSpeak_Howl, EAnimGroup::MenuScrabSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },

    { AnimId::MenuParamiteSpeak_Howdy, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuParamiteSpeak_Idle, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuParamiteSpeak_IdleVariation, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuParamiteSpeak_Cmon, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuParamiteSpeak_Stay, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuParamiteSpeak_DoIt, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuParamiteSpeak_Attack, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuParamiteSpeak_AllAYa, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },

    { AnimId::MenuSligSpeak_Idle, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_Hi, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_HereBoy, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_Freeze, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_GetEm, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_SmoBs, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_Bs, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_LookOut, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_Laugh, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },

    { AnimId::Slog_MoveHeadUpwards, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_StartFastBarking, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_AngryBark, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Dying, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Eating, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Fall, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Idle, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Growl, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_AO_M_12_Unknown, EAnimGroup::Slog, EReliveLevelIds::eNone, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_JumpUpwards, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_JumpForwards, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Run, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_StopRunning, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_SlideTurn, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Scratch, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Sleeping, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_MoveHeadDownwards, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_WakeUp, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_TurnAround, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_StartWalking, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_EndWalking, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Land, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Unused, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Slog_EndFastBarking, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Walk, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },

    { AnimId::Slurg_Burst, EAnimGroup::Slurg, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Slurg_Move, EAnimGroup::Slurg, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Slurg_Turn_Around, EAnimGroup::Slurg, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },

    { AnimId::Background_Glukkon_Idle, EAnimGroup::Glukkons, EReliveLevelIds::eNone, EReliveLevelIds::eBoardRoom, false },
    { AnimId::Background_Glukkon_Laugh, EAnimGroup::Glukkons, EReliveLevelIds::eNone, EReliveLevelIds::eBoardRoom, false },
    { AnimId::Background_Glukkon_KillHim1, EAnimGroup::Glukkons, EReliveLevelIds::eNone, EReliveLevelIds::eBoardRoom, false },
    { AnimId::Background_Glukkon_KillHim2, EAnimGroup::Glukkons, EReliveLevelIds::eNone, EReliveLevelIds::eBoardRoom, false },
    { AnimId::Background_Glukkon_Dying, EAnimGroup::Glukkons, EReliveLevelIds::eNone, EReliveLevelIds::eBoardRoom, false },

    { AnimId::BellHammer_Idle, EAnimGroup::BellHammer, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::BellHammer_Smashing, EAnimGroup::BellHammer, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },

    { AnimId::Elum_Idle, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_WalkLoop, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Turn, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_WalkToIdle, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_MidWalkToIdle, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_IdleToWalk2, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_IdleToWalk1, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_ToYell, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Yell, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Unknown1, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunTurn, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunTurnToWalk, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Speak, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Land, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunOffEdge, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_WalkOffEdge, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_JumpToFall, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_LickingHoney, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_LickingToStruggle, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_AbeMountingEnd, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_AbeUnmountingEnd, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_BeesStruggling, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_HopBegin, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_HopMid, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_HopLand, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunJumpBegin, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunJumpMid, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunJumpLand, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunLoop, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunSlideStop, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunTurnToRun, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_IdleToRun, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_WalkToRun, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_MidWalkToRun, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunToWalk, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_MidRunToWalk, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_ScratchBegin, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_ScratchLoop, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_ScratchEnd, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_ScratchUnknown, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_MountUnmountBegin, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Knockback, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },

    { AnimId::BirdPortal_TerminatorGrow, EAnimGroup::BirdPortal, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BirdPortal_Sparks, EAnimGroup::BirdPortal, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BirdPortal_Flash, EAnimGroup::BirdPortal, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BirdPortal_TerminatorShrink, EAnimGroup::BirdPortal, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BirdPortal_TerminatorIdle, EAnimGroup::BirdPortal, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::PullRingRope_Idle, EAnimGroup::PullRingRope, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::PullRingRope_UseEnd, EAnimGroup::PullRingRope, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::PullRingRope_UseBegin, EAnimGroup::PullRingRope, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Pullring_Desert_Idle, EAnimGroup::PullRingRope, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Pullring_Desert_UseBegin, EAnimGroup::PullRingRope, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Pullring_Desert_UseEnd, EAnimGroup::PullRingRope, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Pullring_Farms_Idle, EAnimGroup::PullRingRope, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Pullring_Farms_UseBegin, EAnimGroup::PullRingRope, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Pullring_Farms_UseEnd, EAnimGroup::PullRingRope, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Bone, EAnimGroup::Throwables, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::BoneBag_Idle, EAnimGroup::BoneBag, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::BoneBag_HardHit, EAnimGroup::BoneBag, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::BoneBag_SoftHit, EAnimGroup::BoneBag, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },

    { AnimId::Rock, EAnimGroup::Throwables, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::RockSack_Idle, EAnimGroup::RockSack, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::RockSack_SoftHit, EAnimGroup::RockSack, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::RockSack_HardHit, EAnimGroup::RockSack, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },

    { AnimId::Security_Claw_Lower_Open, EAnimGroup::SecurityClaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Security_Claw_Lower_Close, EAnimGroup::SecurityClaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Security_Claw_Lower_Idle, EAnimGroup::SecurityClaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Security_Claw_Upper_Rotating, EAnimGroup::SecurityClaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Security_Claw_Upper_NoRotation, EAnimGroup::SecurityClaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Security_Door, EAnimGroup::SecurityDoor, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarmsReturn, false },
    { AnimId::Security_Door_Idle, EAnimGroup::SecurityDoor, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::Security_Door_Speak, EAnimGroup::SecurityDoor, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },

    { AnimId::Security_Orb, EAnimGroup::SecurityOrb, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },

    { AnimId::Slam_Door_Industrial_Closed, EAnimGroup::SlamDoors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Slam_Door_Industrial_Closing, EAnimGroup::SlamDoors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Slam_Door_Industrial_Opening, EAnimGroup::SlamDoors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Slam_Door_Vault_Closed, EAnimGroup::SlamDoors, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Slam_Door_Vault_Closing, EAnimGroup::SlamDoors, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Slam_Door_Vault_Opening, EAnimGroup::SlamDoors, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },

    { AnimId::SlapLock_Initiate, EAnimGroup::SlapLock, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::SlapLock_Punched, EAnimGroup::SlapLock, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::SlapLock_Shaking, EAnimGroup::SlapLock, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },

    { AnimId::Status_Light_Green, EAnimGroup::StatusLight, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Status_Light_Red, EAnimGroup::StatusLight, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Swinging_Ball_Fast, EAnimGroup::SwingingBall, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::Swinging_Ball_Normal, EAnimGroup::SwingingBall, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::Swinging_Ball_Slow, EAnimGroup::SwingingBall, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },

    { AnimId::Lever_Idle, EAnimGroup::Lever, EReliveLevelIds::eMines, EReliveLevelIds::eDesert, false },
    { AnimId::Lever_Pull_Left, EAnimGroup::Lever, EReliveLevelIds::eMines, EReliveLevelIds::eDesert, false },
    { AnimId::Lever_Pull_Release_Left, EAnimGroup::Lever, EReliveLevelIds::eMines, EReliveLevelIds::eDesert, false },
    { AnimId::Lever_Pull_Right, EAnimGroup::Lever, EReliveLevelIds::eMines, EReliveLevelIds::eDesert, false },
    { AnimId::Lever_Pull_Release_Right, EAnimGroup::Lever, EReliveLevelIds::eMines, EReliveLevelIds::eDesert, false },

    { AnimId::RuptureFarms_Lever_Idle, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::RuptureFarms_Lever_Pull_Left, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::RuptureFarms_Lever_Pull_Release_Left, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::RuptureFarms_Lever_Pull_Right, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::RuptureFarms_Lever_Pull_Release_Right, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Lines_Lever_Idle, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Lines_Lever_Pull_Left, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Lines_Lever_Pull_Release_Left, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Lines_Lever_Pull_Right, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Lines_Lever_Pull_Release_Right, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },

    { AnimId::TimedMine_Activated, EAnimGroup::TimedMine, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::TimedMine_Idle, EAnimGroup::TimedMine, EReliveLevelIds::eMines, EReliveLevelIds::eForest, false },

    { AnimId::Tortured_Mudokon, EAnimGroup::TorturedMudokon, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Tortured_Mudokon_Released, EAnimGroup::TorturedMudokon, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Tortured_Mudokon_Zap, EAnimGroup::TorturedMudokon, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Tortured_Mudokon_Tears, EAnimGroup::TorturedMudokon, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },

    { AnimId::Trap_Door_Closed, EAnimGroup::TrapDoors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Trap_Door_Closing, EAnimGroup::TrapDoors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Trap_Door_Open, EAnimGroup::TrapDoors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Trap_Door_Opening, EAnimGroup::TrapDoors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Trap_Door_Tribal_Closed, EAnimGroup::TrapDoors, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Trap_Door_Tribal_Closing, EAnimGroup::TrapDoors, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Trap_Door_Tribal_Open, EAnimGroup::TrapDoors, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Trap_Door_Tribal_Opening, EAnimGroup::TrapDoors, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },

    { AnimId::R1_TrapDoor_Open, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::R1_TrapDoor_Closed, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::R1_TrapDoor_Opening, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::R1_TrapDoor_Closing, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Lines_TrapDoor_Open, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Lines_TrapDoor_Closed, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Lines_TrapDoor_Opening, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Lines_TrapDoor_Closing, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },

    { AnimId::Desert_TrapDoor_Open, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertEscape, false },
    { AnimId::Desert_TrapDoor_Closed, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertEscape, false },
    { AnimId::Desert_TrapDoor_Opening, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertEscape, false },
    { AnimId::Desert_TrapDoor_Closing, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertEscape, false },

    { AnimId::UXB_Active, EAnimGroup::UXB, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::UXB_Disabled, EAnimGroup::UXB, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::UXB_Toggle, EAnimGroup::UXB, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Work_Wheel_Idle, EAnimGroup::WorkWheel, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Work_Wheel_Turning, EAnimGroup::WorkWheel, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Zap_Line_Blue, EAnimGroup::ZapLine, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Zap_Line_Red, EAnimGroup::ZapLine, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Drill_Horizontal_Off, EAnimGroup::Drill, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Drill_Horizontal_On, EAnimGroup::Drill, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Drill_Vertical_Off, EAnimGroup::Drill, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Drill_Vertical_On, EAnimGroup::Drill, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::BigChime, EAnimGroup::Chime, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::BigChime_Moving, EAnimGroup::Chime, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::MediumChime, EAnimGroup::Chime, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::MediumChime_Moving, EAnimGroup::Chime, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::SmallChime, EAnimGroup::Chime, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::SmallChime_Moving, EAnimGroup::Chime, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Chime_Ball, EAnimGroup::Chime, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },

    { AnimId::HoistRock1, EAnimGroup::HoistRocks, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::HoistRock2, EAnimGroup::HoistRocks, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::HoistRock3, EAnimGroup::HoistRocks, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::AO_HoistRock1, EAnimGroup::HoistRocks, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::AO_HoistRock2, EAnimGroup::HoistRocks, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::AO_HoistRock3, EAnimGroup::HoistRocks, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::RuptureFarms_HoistRock1, EAnimGroup::HoistRocks, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::RuptureFarms_HoistRock2, EAnimGroup::HoistRocks, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::RuptureFarms_HoistRock3, EAnimGroup::HoistRocks, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Explosion_Rocks, EAnimGroup::ExplosionRocks, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Explosion_Sticks, EAnimGroup::ExplosionRocks, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },

    { AnimId::CrawlingSligLocker_Closed, EAnimGroup::CrawlingSligLocker, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSligLocker_Open, EAnimGroup::CrawlingSligLocker, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },

    { AnimId::FlintLock_Hammers_Activating, EAnimGroup::FlintLock, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::FlintLock_Hammers_Disabled, EAnimGroup::FlintLock, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::FlintLock_Gourd, EAnimGroup::FlintLock, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },

    { AnimId::GreenDoorLight, EAnimGroup::Lights, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarmsReturn, false },
    { AnimId::GreenHubLight, EAnimGroup::Lights, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarmsReturn, false },
    { AnimId::RedDoorLight, EAnimGroup::Lights, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarmsReturn, false },
    { AnimId::RedHubLight, EAnimGroup::Lights, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarmsReturn, false },

    { AnimId::LiftPlatform_Mines, EAnimGroup::Lifts, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::LiftBottomWheel_Mines, EAnimGroup::Lifts, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::LiftTopWheel_Mines, EAnimGroup::Lifts, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::LiftPlatform_Necrum, EAnimGroup::Lifts, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::LiftBottomWheel_Necrum, EAnimGroup::Lifts, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::LiftTopWheel_Necrum, EAnimGroup::Lifts, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },

    { AnimId::LiftPlatform_Forest, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false },
    { AnimId::LiftBottomWheel_Forest, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false },
    { AnimId::LiftTopWheel_Forest, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false },

    { AnimId::LiftPlatform_Desert, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::LiftBottomWheel_Desert, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::LiftTopWheel_Desert, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },

    { AnimId::LiftPlatform_Desert2, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::LiftBottomWheel_Desert2, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::LiftTopWheel_Desert2, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },

    { AnimId::LiftPlatform_RuptureFarms, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::LiftBottomWheel_RuptureFarms, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::LiftTopWheel_RuptureFarms, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::LiftPlatform_Lines, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::LiftBottomWheel_Lines, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::LiftTopWheel_Lines, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },

    { AnimId::Door_Barracks_Closed, EAnimGroup::Doors, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::Door_Barracks_Open, EAnimGroup::Doors, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::Door_BarracksMetal_Closed, EAnimGroup::Doors, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::Door_BarracksMetal_Open, EAnimGroup::Doors, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::Door_Bonewerkz_Closed, EAnimGroup::Doors, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::Door_Bonewerkz_Open, EAnimGroup::Doors, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::Door_Brewery_Closed, EAnimGroup::Doors, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Door_Brewery_Open, EAnimGroup::Doors, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Door_Feeco_Closed, EAnimGroup::Doors, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Door_Feeco_Open, EAnimGroup::Doors, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Door_Mines_Closed, EAnimGroup::Doors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Door_Mines_Open, EAnimGroup::Doors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Door_Temple_Closed, EAnimGroup::Doors, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Door_Temple_Open, EAnimGroup::Doors, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Door_Train_Closed, EAnimGroup::Doors, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::Door_Train_Closing, EAnimGroup::Doors, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::Door_RuptureFarms_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Door_RuptureFarms_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Door_Lines_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Door_Lines_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Door_Forest_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::Door_Forest_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::HubDoor_Forest_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::HubDoor_Forest_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::FinalTestDoor_Forest_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::FinalTestDoor_Forest_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::Door_Desert_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Door_Desert_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::HubDoor_Desert_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::HubDoor_Desert_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::FinalTestDoor_Desert_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::FinalTestDoor_Desert_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },

    { AnimId::Rope_Lines, EAnimGroup::Ropes, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Rope_R1, EAnimGroup::Ropes, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::AE_Rope, EAnimGroup::Ropes, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Electric_Wall, EAnimGroup::ElectricWall, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Explosion, EAnimGroup::Explosions, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Explosion_Mine, EAnimGroup::Explosions, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Explosion_Small, EAnimGroup::Explosions, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::FallingCrate_Falling, EAnimGroup::FallingItems, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::FallingCrate_Waiting, EAnimGroup::FallingItems, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },

    { AnimId::AE_FallingRock_Falling, EAnimGroup::FallingItems, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::AE_FallingRock_Waiting, EAnimGroup::FallingItems, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::AO_FallingRock_Falling, EAnimGroup::FallingItems, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::AO_FallingRock_Waiting, EAnimGroup::FallingItems, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },

    { AnimId::FallingMeat_Waiting, EAnimGroup::FallingItems, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::FallingMeat_Falling, EAnimGroup::FallingItems, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Foot_Switch_Bonewerkz_Idle, EAnimGroup::FootSwitches, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::Foot_Switch_Bonewerkz_Pressed, EAnimGroup::FootSwitches, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },

    { AnimId::Foot_Switch_Industrial_Idle, EAnimGroup::FootSwitches, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Foot_Switch_Industrial_Pressed, EAnimGroup::FootSwitches, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Foot_Switch_Temple, EAnimGroup::FootSwitches, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::Foot_Switch_Temple_Pressed, EAnimGroup::FootSwitches, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },

    { AnimId::Foot_Switch_Vault_Idle, EAnimGroup::FootSwitches, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Foot_Switch_Vault_Pressed, EAnimGroup::FootSwitches, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },

    { AnimId::Grenade, EAnimGroup::Throwables, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BoomMachine_Button_Off, EAnimGroup::BoomMachine, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BoomMachine_Button_On, EAnimGroup::BoomMachine, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BoomMachine_Nozzle_Idle, EAnimGroup::BoomMachine, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BoomMachine_Nozzle_DropGrenade, EAnimGroup::BoomMachine, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Honey_Drip, EAnimGroup::HoneySack, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false },
    { AnimId::HoneySack_Hanging, EAnimGroup::HoneySack, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false },
    { AnimId::HoneySack_Falling, EAnimGroup::HoneySack, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false },
    { AnimId::HoneySack_FallingToSmashed, EAnimGroup::HoneySack, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false },
    { AnimId::HoneySack_OnGround, EAnimGroup::HoneySack, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false },

    { AnimId::Meat, EAnimGroup::Throwables, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::MeatSack_Idle, EAnimGroup::MeatSack, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::MeatSack_Hit, EAnimGroup::MeatSack, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },

    { AnimId::MeatSaw_Idle, EAnimGroup::MeatSaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::MeatSaw_Moving, EAnimGroup::MeatSaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::MeatSawMotor, EAnimGroup::MeatSaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::MotionDetector_Flare, EAnimGroup::MotionDetector, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::MotionDetector_Laser, EAnimGroup::MotionDetector, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },

    { AnimId::Door_Lock_Idle, EAnimGroup::DoorLock, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Door_Lock_Open, EAnimGroup::DoorLock, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },

    { AnimId::Mine_Flash, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::WaterDrop, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::BrewMachine_Button, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Circular_Fade, EAnimGroup::Ungrouped, EReliveLevelIds::eNone, EReliveLevelIds::eMenu, false },
    { AnimId::DeathFlare_1, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::DeathFlare_2, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Bullet_Shell, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Blood, EAnimGroup::Ungrouped, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::MenuDoor, EAnimGroup::Ungrouped, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::AbeIntro, EAnimGroup::Ungrouped, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::BloodDrop, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Scoopz, EAnimGroup::Ungrouped, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::FlintGlow, EAnimGroup::Ungrouped, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::GreenGlow, EAnimGroup::Ungrouped, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::GoldGlow, EAnimGroup::Ungrouped, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::ChantOrb_Particle_Small, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Well_Leaf, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Bomb_RedGreenTick, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Bomb_Flash, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Stone_Ball, EAnimGroup::Ungrouped, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Stone_Ball_Stopper, EAnimGroup::Ungrouped, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Star, EAnimGroup::Ungrouped, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::AE_ZapSpark, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::ParamiteWeb, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eForestTemple, false },
    { AnimId::MovingBomb, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Mine, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Honey, EAnimGroup::Ungrouped, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::HintFly, EAnimGroup::Ungrouped, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Fart, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::WaterSplash, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::ShootingZFire_Particle, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::OptionChantOrb_Particle, EAnimGroup::Ungrouped, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::SquibSmoke_Particle, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::ChantOrb_Particle, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::ShootingFire_Particle, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Vaporize_Particle, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Loading_Icon2, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Loading_Icon, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Dust_Particle, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::ObjectShadow, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::SpotLight, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Door_FireBackgroundGlow, EAnimGroup::Ungrouped, EReliveLevelIds::eNecrum, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Fire, EAnimGroup::Ungrouped, EReliveLevelIds::eNecrum, EReliveLevelIds::eDesertTemple, false },
};

constexpr AnimRecNames kAnimRecNames[] = {
    {AnimId::Abe_Arm_Gib, "arm_gib"},
    {AnimId::Abe_Body_Gib, "body_gib"},
    {AnimId::Mudokon_StandingTurn, "standing_turn"}

};

const char_type* AnimBaseName(AnimId id)
{
    for (auto& rec : kAnimRecNames)
    {
        if (rec.mAnimId == id)
        {
            return rec.mAnimName;
        }
    }
    ALIVE_FATAL("No name in mapping table");
}

const char_type* AnimRecName(AnimId anim)
{
    for (const auto& rec : kAnimRecNames)
    {
        if (rec.mAnimId == anim)
        {
            return rec.mAnimName;
        }
    }
    ALIVE_FATAL("Unknown anim id");
}

const char* ToString(EAnimGroup animGroup)
{
    switch (animGroup)
    {
    case EAnimGroup::Ungrouped:
        return "ungrouped";
    case EAnimGroup::Gibs:
        return "gibs";
    case EAnimGroup::Ropes:
        return "ropes";
    case EAnimGroup::Throwables:
        return "throwables";
    case EAnimGroup::Abe:
        return "abe";
    case EAnimGroup::Lights:
        return "lights";
    case EAnimGroup::Bat:
        return "bat";
    case EAnimGroup::Fleech:
        return "fleech";
    case EAnimGroup::FlyingSlig:
        return "flying_slig";
    case EAnimGroup::Glukkons:
        return "glukkons";
    case EAnimGroup::Greeter:
        return "greeter";
    case EAnimGroup::MineCar:
        return "mine_car";
    case EAnimGroup::Paramite:
        return "paramite";
    case EAnimGroup::Scrab:
        return "scrab";
    case EAnimGroup::CrawlingSlig:
        return "crawling_slig";
    case EAnimGroup::Slig:
        return "slig";
    case EAnimGroup::Shrykull:
        return "shrykull";
    case EAnimGroup::Icons:
        return "icons";
    case EAnimGroup::MenuHighlight:
        return "menu_highlight";
    case EAnimGroup::MenuAbeSpeak:
        return "menu_abe_speak";
    case EAnimGroup::MenuGlukkonSpeak:
        return "menu_glukkon_speak";
    case EAnimGroup::MenuScrabSpeak:
        return "menu_Scrab_speak";
    case EAnimGroup::MenuParamiteSpeak:
        return "menu_paramite_speak";
    case EAnimGroup::MenuSligSpeak:
        return "menu_slig_speak";
    case EAnimGroup::Slog:
        return "slog";
    case EAnimGroup::Slurg:
        return "slurg";
    case EAnimGroup::Elum:
        return "elum";
    case EAnimGroup::BirdPortal:
        return "bird_portal";
    case EAnimGroup::PullRingRope:
        return "pull_ring_rope";
    case EAnimGroup::BoneBag:
        return "bone_bag";
    case EAnimGroup::Drill:
        return "drill";
    case EAnimGroup::Chime:
        return "chime";
    case EAnimGroup::HoistRocks:
        return "hoist_rocks";
    case EAnimGroup::ExplosionRocks:
        return "explosion_rocks";
    case EAnimGroup::ExplosionSticks:
        return "explosion_sticks";
    case EAnimGroup::CrawlingSligLocker:
        return "crawling_slig_locker";
    case EAnimGroup::Lifts:
        return "lifts";
    case EAnimGroup::Doors:
        return "doors";
    case EAnimGroup::Dove:
        return "dove";
    case EAnimGroup::ElectricWall:
        return "electric_wall";
    case EAnimGroup::Explosions:
        return "explosions";
    case EAnimGroup::FallingItems:
        return "falling_items";
    case EAnimGroup::FootSwitches:
        return "foot_switches";
    case EAnimGroup::Grenade:
        return "grenade";
    case EAnimGroup::HoneySack:
        return "honey_sack";
    case EAnimGroup::MeatSack:
        return "meat_sack";
    case EAnimGroup::MeatSaw:
        return "meat_saw";
    case EAnimGroup::MotionDetector:
        return "motion_detector";
    case EAnimGroup::MovingBomb:
        return "moving_bomb";
    case EAnimGroup::RockSack:
        return "rock_sack";
    case EAnimGroup::SecurityClaw:
        return "security_claw";
    case EAnimGroup::SecurityDoor:
        return "security_door";
    case EAnimGroup::SecurityOrb:
        return "security_orb";
    case EAnimGroup::SlamDoors:
        return "slam_doors";
    case EAnimGroup::SlapLock:
        return "slap_lock";
    case EAnimGroup::StatusLight:
        return "status_light";
    case EAnimGroup::SwingingBall:
        return "swinging_ball";
    case EAnimGroup::Lever:
        return "lever";
    case EAnimGroup::TimedMine:
        return "timed_mine";
    case EAnimGroup::TorturedMudokon:
        return "tortured_mudokon";
    case EAnimGroup::TrapDoors:
        return "trap_doors";
    case EAnimGroup::UXB:
        return "uxb";
    case EAnimGroup::WorkWheel:
        return "work_wheel";
    case EAnimGroup::ZapLine:
        return "zap_line";
    case EAnimGroup::FlintLock:
        return "flint_lock";
    case EAnimGroup::BoomMachine:
        return "boom_machine";
    case EAnimGroup::CrawlingSligButton:
        return "crawling_slig_button";
    case EAnimGroup::BellHammer:
        return "bell_hammer";
    case EAnimGroup::DoorLock:
        return "door_lock";
    default:
        ALIVE_FATAL("unknown group");
    }
}
