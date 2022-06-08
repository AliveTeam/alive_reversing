#include "stdafx.h"
#include "Mudokon.hpp"
#include "Function.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "AbilityRing.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "DDCheat.hpp"
#include "WorkWheel.hpp"
#include "BirdPortal.hpp"
#include "SwitchStates.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Spark.hpp"
#include "Sound/Midi.hpp"
#include "ScreenManager.hpp"
#include "Lever.hpp"
#include "Particle.hpp"
#include "PossessionFlicker.hpp"
#include "ScreenShake.hpp"
#include "MainMenu.hpp"
#include "LCDStatusBoard.hpp"
#include "MusicTrigger.hpp"
#include "Blood.hpp"
#include "Gibs.hpp"
#include "Bullet.hpp"
#include "LaughingGas.hpp"
#include "Sys_common.hpp"
#include "Grid.hpp"

ALIVE_VAR(1, 0x5C3012, s16, sGoingToBirdPortalMudCount_5C3012, 0);

const TintEntry kMudTints_55C744[18] = {
    {EReliveLevelIds::eMines, 87u, 103u, 67u},
    {EReliveLevelIds::eNecrum, 87u, 103u, 67u},
    {EReliveLevelIds::eMudomoVault, 87u, 103u, 67u},
    {EReliveLevelIds::eMudancheeVault, 87u, 103u, 67u},
    {EReliveLevelIds::eFeeCoDepot, 87u, 103u, 67u},
    {EReliveLevelIds::eBarracks, 87u, 103u, 67u},
    {EReliveLevelIds::eMudancheeVault_Ender, 87u, 103u, 67u},
    {EReliveLevelIds::eBonewerkz, 87u, 103u, 67u},
    {EReliveLevelIds::eBrewery, 87u, 103u, 67u},
    {EReliveLevelIds::eBrewery_Ender, 87u, 103u, 67u},
    {EReliveLevelIds::eMudomoVault_Ender, 87u, 103u, 67u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 87u, 103u, 67u},
    {EReliveLevelIds::eBarracks_Ender, 87u, 103u, 67u},
    {EReliveLevelIds::eBonewerkz_Ender, 87u, 103u, 67u},
    {EReliveLevelIds::eNone, 87u, 103u, 67u}};

using TMudBrainStateFunction = decltype(&Mudokon::Brain_0_GiveRings_470C10);

const TMudBrainStateFunction sMudokon_Brain_Table_55CDF0[10] = {
    &Mudokon::Brain_0_GiveRings_470C10,
    &Mudokon::Brain_1_Chisel_47C5F0,
    &Mudokon::Brain_2_Scrub_47D270,
    &Mudokon::Brain_3_TurnWheel_47E0D0,
    &Mudokon::Brain_4_ListeningToAbe_477B40,
    &Mudokon::Brain_5_ShrivelDeath_4714A0,
    &Mudokon::Brain_6_Escape_47A560,
    &Mudokon::Brain_7_FallAndSmackDeath_471600,
    &Mudokon::Brain_8_AngryWorker_47E910,
    &Mudokon::Brain_9_Sick_47A910};

#define MUD_MOTION_STATES_ENUM(ENTRY) \
    ENTRY(M_Idle_0_4724E0)            \
    ENTRY(M_WalkLoop_1_4728B0)        \
    ENTRY(M_StandingTurn_2_472BF0)    \
    ENTRY(M_Speak_472FA0)             \
    ENTRY(M_Speak_472FA0)             \
    ENTRY(M_Speak_472FA0)             \
    ENTRY(M_Speak_472FA0)             \
    ENTRY(M_WalkBegin_7_472AB0)       \
    ENTRY(M_WalkToIdle_8_472B30)      \
    ENTRY(M_MidWalkToIdle_9_472BD0)   \
    ENTRY(M_LeverUse_10_473020)       \
    ENTRY(M_Chisel_11_4732D0)         \
    ENTRY(M_StartChisel_12_473500)    \
    ENTRY(M_StopChisel_13_473530)     \
    ENTRY(M_CrouchScrub_14_473560)    \
    ENTRY(M_CrouchIdle_15_474040)     \
    ENTRY(M_CrouchTurn_16_4740E0)     \
    ENTRY(M_StandToCrouch_17_474120)  \
    ENTRY(M_CrouchToStand_18_474150)  \
    ENTRY(M_WalkToRun_19_473600)      \
    ENTRY(M_MidWalkToRun_20_4736D0)   \
    ENTRY(M_RunLoop_21_473720)        \
    ENTRY(M_RunToWalk_22_4738E0)      \
    ENTRY(M_MidRunToWalk_23_4739B0)   \
    ENTRY(M_RunSlideStop_24_473A00)   \
    ENTRY(M_RunSlideTurn_25_473AA0)   \
    ENTRY(M_RunTurnToRun_26_473BB0)   \
    ENTRY(M_SneakLoop_27_473C40)      \
    ENTRY(M_MidWalkToSneak_28_473D60)    \
    ENTRY(M_SneakToWalk_29_473E20)    \
    ENTRY(M_WalkToSneak_30_473EE0) \
    ENTRY(M_MidSneakToWalk_31_473F30) \
    ENTRY(M_SneakBegin_32_473F80)     \
    ENTRY(M_SneakToIdle_33_473FF0)    \
    ENTRY(M_MidSneakToIdle_34_474020) \
    ENTRY(M_RunJumpBegin_35_474460)      \
    ENTRY(M_RunJumpMid_36_474570)        \
    ENTRY(M_StandToRun_37_4749A0)      \
    ENTRY(M_Punch_38_474AA0)          \
    ENTRY(M_HoistBegin_39_4748E0)     \
    ENTRY(M_HoistLand_40_474780)      \
    ENTRY(M_LandSoft1_41_474960)      \
    ENTRY(M_LandSoft2_42_4743F0)      \
    ENTRY(M_DunnoBegin_43_472790)     \
    ENTRY(M_DunnoEnd_44_4727B0)       \
    ENTRY(M_KnockForward_45_474180)   \
    ENTRY(M_Knockback_46_4742A0)      \
    ENTRY(M_KnockbackGetUp_47_474380) \
    ENTRY(M_WalkOffEdge_48_4743C0)    \
    ENTRY(M_Fall_49_472C60)           \
    ENTRY(M_Chant_50_473040)          \
    ENTRY(M_ChantEnd_51_4730D0)       \
    ENTRY(M_ToDuck_52_474A20)         \
    ENTRY(M_Duck_53_474A40)           \
    ENTRY(M_DuckToCrouch_54_474A80)   \
    ENTRY(M_DuckKnockback_55_474250)  \
    ENTRY(M_SlapOwnHead_56_4727D0)    \
    ENTRY(M_TurnWheelBegin_57_474C00) \
    ENTRY(M_TurnWheelLoop_58_474CC0)  \
    ENTRY(M_TurnWheelEnd_59_474D30)

const char_type* const sMudMotionStateNames[60] = {
    MUD_MOTION_STATES_ENUM(MAKE_STRINGS)};

struct MudEmotionTableEntry final
{
    MudSounds field_0_sound;
    eMudMotions field_2_next_motion;
    Mud_Emotion field_4_emo_tbl;
    s16 field_6_sub_state;
};

struct MudEmotionTable final
{
    MudEmotionTableEntry field_0_data[8][17];
};

const MudEmotionTable kMudEmoTable_55C790 = {
    {{// Correlates to MudEmotion::eNormal_0.
      {MudSounds::eHelloNeutral_3, eMudMotions::M_Speak_4_472FA0, Mud_Emotion::eNormal_0, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eNormal_0, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eNormal_0, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eNormal_0, 12},
      {MudSounds::eNone, eMudMotions::M_DunnoBegin_43_472790, Mud_Emotion::eNormal_0, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eFartPuh_23, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eNormal_0, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Punch_38_474AA0, Mud_Emotion::eAggressive_2, 13},
      {MudSounds::eNone, eMudMotions::M_DunnoBegin_43_472790, Mud_Emotion::eNormal_0, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eNormal_0, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eNormal_0, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eNormal_0, 12},
      {MudSounds::eSadUgh_28, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eNormal_0, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eNormal_0, 12}},
     {// Correlates to MudEmotion::eAngry_1.
      {MudSounds::eHiAngry_18, eMudMotions::M_Speak_4_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNoAngry_21, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eAngry_1, 0},
      {MudSounds::eNoAngry_21, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNone, eMudMotions::M_DunnoBegin_43_472790, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Punch_38_474AA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Punch_38_474AA0, Mud_Emotion::eAggressive_2, 13},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eNormal_0, 15},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eNormal_0, 15},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eAngry_1, 0},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12}},
     {// Correlates to MudEmotion::eAggressive_2.
      {MudSounds::eHiAngry_18, eMudMotions::M_Speak_4_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNoAngry_21, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eAngry_1, 0},
      {MudSounds::eNoAngry_21, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Punch_38_474AA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Punch_38_474AA0, Mud_Emotion::eAggressive_2, 13},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eNormal_0, 15},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eNormal_0, 15},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eAngry_1, 0},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12}},
     {// Correlates to MudEmotion::eSad_3;
      {MudSounds::eHiSad_20, eMudMotions::M_Speak_4_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSad_3, 0},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNone, eMudMotions::M_DunnoBegin_43_472790, Mud_Emotion::eSad_3, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 13},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eNormal_0, 15},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eNormal_0, 0},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12}},
     {// Correlates to MudEmotion::eSuicidal_4.
      {MudSounds::eHiSad_20, eMudMotions::M_Speak_4_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSuicidal_4, 0},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eSadUgh_28, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSuicidal_4, 0},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 13},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eNormal_0, 0},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSuicidal_4, 0},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSuicidal_4, 0},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSuicidal_4, 0},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSuicidal_4, 0},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSuicidal_4, 0}},
     {// Correlates to MudEmotion::eHappy_5.
      {MudSounds::eHiHappy_19, eMudMotions::M_Speak_4_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_HoistBegin_39_4748E0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eNuhUh_13, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eNormal_0, 13},
      {MudSounds::eNone, eMudMotions::M_DunnoBegin_43_472790, Mud_Emotion::eWired_6, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eWired_6, 0},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12}},
     {// Correlates to MudEmotion::eWired_6.
      {MudSounds::eHiHappy_19, eMudMotions::M_Speak_4_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_HoistBegin_39_4748E0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eNuhUh_13, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eNormal_0, 13},
      {MudSounds::eNone, eMudMotions::M_DunnoBegin_43_472790, Mud_Emotion::eWired_6, 12},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eWired_6, 0},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eWired_6, 0},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12}},
     {// Correlates to MudEmotion::eSick_7.
      {MudSounds::eSick_24, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSick_7, 12},
      {MudSounds::eSick_24, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSick_7, 12},
      {MudSounds::eSick_24, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eSick_7, 12},
      {MudSounds::eSick_24, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eSick_7, 12},
      {MudSounds::eSick_24, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSick_7, 12},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSick_7, 0},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSick_7, 0},
      {MudSounds::eSadUgh_28, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSick_7, 13},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSick_7, 0},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSick_7, 0},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSick_7, 0},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSick_7, 0},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSick_7, 0},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSick_7, 0},
      {MudSounds::eNone, eMudMotions::M_Idle_0_4724E0, Mud_Emotion::eSick_7, 0},
      {MudSounds::eSick_24, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSick_7, 12},
      {MudSounds::eSick_24, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSick_7, 12}}}};



using TMudStateFunction = decltype(&Mudokon::M_Idle_0_4724E0);

const TMudStateFunction sMudokon_motion_states_55CE18[60] = {
    &Mudokon::M_Idle_0_4724E0,
    &Mudokon::M_WalkLoop_1_4728B0,
    &Mudokon::M_StandingTurn_2_472BF0,
    &Mudokon::M_Speak_472FA0,
    &Mudokon::M_Speak_472FA0,
    &Mudokon::M_Speak_472FA0,
    &Mudokon::M_Speak_472FA0,
    &Mudokon::M_WalkBegin_7_472AB0,
    &Mudokon::M_WalkToIdle_8_472B30,
    &Mudokon::M_MidWalkToIdle_9_472BD0,
    &Mudokon::M_LeverUse_10_473020,
    &Mudokon::M_Chisel_11_4732D0,
    &Mudokon::M_StartChisel_12_473500,
    &Mudokon::M_StopChisel_13_473530,
    &Mudokon::M_CrouchScrub_14_473560,
    &Mudokon::M_CrouchIdle_15_474040,
    &Mudokon::M_CrouchTurn_16_4740E0,
    &Mudokon::M_StandToCrouch_17_474120,
    &Mudokon::M_CrouchToStand_18_474150,
    &Mudokon::M_WalkToRun_19_473600,
    &Mudokon::M_MidWalkToRun_20_4736D0,
    &Mudokon::M_RunLoop_21_473720,
    &Mudokon::M_RunToWalk_22_4738E0,
    &Mudokon::M_MidRunToWalk_23_4739B0,
    &Mudokon::M_RunSlideStop_24_473A00,
    &Mudokon::M_RunSlideTurn_25_473AA0,
    &Mudokon::M_RunTurnToRun_26_473BB0,
    &Mudokon::M_SneakLoop_27_473C40,
    &Mudokon::M_MidWalkToSneak_28_473D60,
    &Mudokon::M_SneakToWalk_29_473E20,
    &Mudokon::M_WalkToSneak_30_473EE0,
    &Mudokon::M_MidSneakToWalk_31_473F30,
    &Mudokon::M_SneakBegin_32_473F80,
    &Mudokon::M_SneakToIdle_33_473FF0,
    &Mudokon::M_MidSneakToIdle_34_474020,
    &Mudokon::M_RunJumpBegin_35_474460,
    &Mudokon::M_RunJumpMid_36_474570,
    &Mudokon::M_StandToRun_37_4749A0,
    &Mudokon::M_Punch_38_474AA0,
    &Mudokon::M_HoistBegin_39_4748E0,
    &Mudokon::M_HoistLand_40_474780,
    &Mudokon::M_LandSoft1_41_474960,
    &Mudokon::M_LandSoft2_42_4743F0,
    &Mudokon::M_DunnoBegin_43_472790,
    &Mudokon::M_DunnoEnd_44_4727B0,
    &Mudokon::M_KnockForward_45_474180,
    &Mudokon::M_Knockback_46_4742A0,
    &Mudokon::M_KnockbackGetUp_47_474380,
    &Mudokon::M_WalkOffEdge_48_4743C0,
    &Mudokon::M_Fall_49_472C60,
    &Mudokon::M_Chant_50_473040,
    &Mudokon::M_ChantEnd_51_4730D0,
    &Mudokon::M_ToDuck_52_474A20,
    &Mudokon::M_Duck_53_474A40,
    &Mudokon::M_DuckToCrouch_54_474A80,
    &Mudokon::M_DuckKnockback_55_474250,
    &Mudokon::M_SlapOwnHead_56_4727D0,
    &Mudokon::M_TurnWheelBegin_57_474C00,
    &Mudokon::M_TurnWheelLoop_58_474CC0,
    &Mudokon::M_TurnWheelEnd_59_474D30};


// This is used rather than the un-typesafe word_55CF08 array
static Mud_Emotion TLV_Emo_To_Internal_Emo(Mud_TLV_Emotion emo)
{
    switch (emo)
    {
        case Mud_TLV_Emotion::eNormal_0:
            return Mud_Emotion::eNormal_0;

        case Mud_TLV_Emotion::eAngry_1:
            return Mud_Emotion::eAngry_1;

        case Mud_TLV_Emotion::eSad_2:
            return Mud_Emotion::eSad_3;

        case Mud_TLV_Emotion::eWired_3:
            return Mud_Emotion::eWired_6;

        case Mud_TLV_Emotion::eSick_4:
            return Mud_Emotion::eSick_7;

        default:
            LOG_ERROR("Don't know about emo type " << static_cast<s32>(emo));
            ALIVE_FATAL("Unknown emo");
    }
}

Mudokon::Mudokon(Path_Mudokon* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(18)
{
    field_154_unused = 0;
    field_140_last_event_index = -1;
    field_156_unused = -1;

    field_11C_bird_portal_id = -1;
    field_12C_unused = -1;
    field_158_wheel_id = -1;
    mBaseGameObjectTlvInfo = tlvInfo;
    field_194_timer = 0;
    field_18E_brain_state = Mud_Brain_State::Brain_0_GiveRings_470C10;
    field_190_brain_sub_state = 0;
    mNextMotion = -1;
    field_192_return_to_previous_motion = 0;
    field_13C_voice_pitch = 0;

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, TRUE, FALSE));
    field_10_resources_array.SetAt(1, nullptr);

    const AnimRecord& rec = AnimRec(AnimId::Mudokon_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    mApplyShadows |= 2u;

    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_18C_unused = 0;
    SetTint(kMudTints_55C744, field_C2_lvl_number);

    field_17E_delayed_speak = MudAction::eNone_17;

    field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);
    field_16A_flags.Clear(Flags_16A::eBit3_alerted);
    field_16A_flags.Set(Flags_16A::eBit4_blind, pTlv->field_22_bBlind == Choice_short::eYes_1);
    field_16A_flags.Clear(Flags_16A::eBit5_following);
    field_16A_flags.Clear(Flags_16A::eBit6_standing_for_sad_or_angry);
    field_16A_flags.Clear(Flags_16A::eBit7_stopped_at_wheel);
    field_16A_flags.Clear(Flags_16A::eBit8_do_angry);
    field_16A_flags.Clear(Flags_16A::eBit9_seen_while_sick);
    // 10 and 11 ??
    field_16A_flags.Set(Flags_16A::eBit12_alert_enemies);
    field_16A_flags.Clear(Flags_16A::eBit13);
    field_16A_flags.Clear(Flags_16A::eBit14_make_sad_noise);

    field_180_emo_tbl = TLV_Emo_To_Internal_Emo(pTlv->field_20_emotion);
    field_188_pTblEntry = nullptr;
    field_182 = GameSpeakEvents::eNone_m1;
    field_120_angry_switch_id = pTlv->field_24_angry_switch_id;
    field_16C_flags.Clear(Flags_16C::eBit2_Unknown);
    field_16C_flags.Clear(Flags_16C::eBit3_Unknown);
    field_198_turning_wheel_timer = 0;

    switch (pTlv->field_12_state)
    {
        case Mud_State::eChisle_0:
            field_18E_brain_state = Mud_Brain_State::Brain_1_Chisel_47C5F0;
            field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMudchslResID, TRUE, FALSE));
            break;

        case Mud_State::eScrub_1:
            field_18E_brain_state = Mud_Brain_State::Brain_2_Scrub_47D270;
            field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMudscrubResID, TRUE, FALSE));
            break;

        case Mud_State::eAngryWorker_2:
            field_18E_brain_state = Mud_Brain_State::Brain_8_AngryWorker_47E910;
            field_180_emo_tbl = Mud_Emotion::eAngry_1;
            field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMudscrubResID, TRUE, FALSE));
            break;

        case Mud_State::eDamageRingGiver_3:
        case Mud_State::eHealthRingGiver_4:
            if (pTlv->field_12_state == Mud_State::eDamageRingGiver_3)
            {
                field_168_ring_type = RingTypes::eExplosive_Emit_Effect_2;
            }
            else
            {
                field_168_ring_type = RingTypes::eHealing_Emit_Effect_11;
            }
            field_164_ring_pulse_interval = pTlv->field_2A_ring_pulse_interval;
            field_16A_flags.Set(Flags_16A::eBit16_give_ring_without_password, pTlv->field_2C_bGive_ring_without_password == Choice_short::eYes_1);
            field_16C_flags.Clear(Flags_16C::eBit1_Unknown);
            field_18E_brain_state = Mud_Brain_State::Brain_0_GiveRings_470C10;
            field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeommResID, TRUE, FALSE));
            break;

        default:
            break;
    }

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe_477B40;
    }

    field_10_resources_array.SetAt(9, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMudoduckResID, TRUE, FALSE));
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMudbtlnkResID, TRUE, FALSE));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, TRUE, FALSE));
    field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeknfdResID, TRUE, FALSE));
    field_10_resources_array.SetAt(6, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeknbkResID, TRUE, FALSE));
    field_10_resources_array.SetAt(7, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeedgeResID, TRUE, FALSE));
    field_10_resources_array.SetAt(11, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMudidleResID, TRUE, FALSE));
    field_10_resources_array.SetAt(13, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, AEResourceID::kMudangryResID, TRUE, FALSE));
    field_10_resources_array.SetAt(14, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, AEResourceID::kMudsadResID, TRUE, FALSE));
    field_10_resources_array.SetAt(15, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, AEResourceID::kMudwiredResID, TRUE, FALSE));
    field_10_resources_array.SetAt(16, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, AEResourceID::kMudblindResID, TRUE, FALSE));
    field_10_resources_array.SetAt(17, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, AEResourceID::kMudsickResID, TRUE, FALSE));

    if (field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        SetPal(Mud_Emotion::eNormal_0);
    }

    field_20_animation.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pTlv->field_14_direction == XDirection_short::eLeft_0);

    SetType(ReliveTypes::eMudokon);

    field_13C_voice_pitch = pTlv->field_16_voice_pitch;
    field_17A_rescue_switch_id = pTlv->field_18_rescue_switch_id;

    field_16A_flags.Set(Flags_16A::eBit2_persist_and_reset_offscreen, pTlv->field_1E_persist_and_reset_offscreen == Choice_short::eYes_1);
    field_16A_flags.Set(Flags_16A::eBit10_work_after_turning_wheel, pTlv->field_26_work_after_turning_wheel == Choice_short::eYes_1);
    field_16A_flags.Set(Flags_16A::eBit11_get_depressed, pTlv->field_28_bGets_depressed == Choice_short::eYes_1);
    field_16A_flags.Set(Flags_16A::eBit15_ring_and_angry_mud_timeout, pTlv->field_2A_ring_pulse_interval & 1);

    field_17C_stand_idle_timer = 0;

    VStackOnObjectsOfType(ReliveTypes::eMudokon);

    if (field_180_emo_tbl == Mud_Emotion::eSick_7)
    {
        field_18E_brain_state = Mud_Brain_State::Brain_9_Sick_47A910;
        mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
        mCurrentMotion = eMudMotions::M_CrouchIdle_15_474040;
    }
    else
    {
        mCurrentMotion = eMudMotions::M_Idle_0_4724E0;
    }

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.mRenderLayer = Layer::eLayer_ZapLinesMuds_Half_9;
        field_D6_scale = 0;
    }
    else if (pTlv->field_10_scale == Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.mRenderLayer = Layer::eLayer_ZapLinesMuds_28;
        field_D6_scale = 1;
    }

    FP hitX = {};
    FP hitY = {};
    const auto bCollision = sCollisions->Raycast(
        FP_FromInteger(pTlv->field_8_top_left.field_0_x),
        FP_FromInteger(pTlv->field_8_top_left.field_2_y),
        FP_FromInteger(pTlv->field_C_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_C_bottom_right.field_2_y),
        &BaseAliveGameObjectCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    if (bCollision)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
        if (BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_32 ||
            BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_36)
        {
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);
            VOnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {bRect.w, static_cast<s16>(bRect.h + 5)},
                ObjList_5C1B78,
                1,
                (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
        }
    }

    field_20_animation.field_1C_fn_ptr_array = kAbe_Anim_Frame_Fns_55EF98;
    BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

    if (field_18E_brain_state == Mud_Brain_State::Brain_2_Scrub_47D270 || field_18E_brain_state == Mud_Brain_State::Brain_1_Chisel_47C5F0)
    {
        MapFollowMe(TRUE);
    }

    field_16A_flags.Set(Flags_16A::eBit1_not_rescued);
    field_190_brain_sub_state = 0;
    field_118_tlvInfo = mBaseGameObjectTlvInfo;

    field_128_angry_timer = 0;
    field_160_delayed_speak = MudSounds::eNone;
    field_162_maxXOffset = field_DA_xOffset;

    mShadow = relive_new Shadow();

    VUpdate();
}

const AnimId kMudFrameTableOffsets_55CD00[60] = {
    AnimId::Mudokon_Idle,
    AnimId::Mudokon_Walk,
    AnimId::Mudokon_StandingTurn,
    AnimId::Mudokon_Speak1,
    AnimId::Mudokon_Speak3,
    AnimId::Mudokon_Speak2,
    AnimId::Mudokon_SpeakFart,
    AnimId::Mudokon_WalkBegin,
    AnimId::Mudokon_WalkToIdle,
    AnimId::Mudokon_MidWalkToIdle,
    AnimId::Mudokon_LeverUse,
    AnimId::Mudokon_Chisel,
    AnimId::Mudokon_StartChisel,
    AnimId::Mudokon_StopChisel,
    AnimId::Mudokon_CrouchScrub,
    AnimId::Mudokon_CrouchIdle,
    AnimId::Mudokon_CrouchTurn,
    AnimId::Mudokon_StandToCrouch,
    AnimId::Mudokon_CrouchToStand,
    AnimId::Mudokon_WalkToRun,
    AnimId::Mudokon_MidWalkToRun,
    AnimId::Mudokon_Run,
    AnimId::Mudokon_RunToWalk,
    AnimId::Mudokon_MidRunToWalk,
    AnimId::Mudokon_RunSlideStop,
    AnimId::Mudokon_RunSlideTurn,
    AnimId::Mudokon_RunTurnToRun,
    AnimId::Mudokon_Sneak,
    AnimId::Mudokon_WalkToSneak,
    AnimId::Mudokon_SneakToWalk,
    AnimId::Mudokon_MidWalkToSneak,
    AnimId::Mudokon_MidSneakToWalk,
    AnimId::Mudokon_SneakBegin,
    AnimId::Mudokon_SneakToIdle,
    AnimId::Mudokon_MidSneakToIdle,
    AnimId::Mudokon_RunJumpBegin,
    AnimId::Mudokon_RunJumpMid,
    AnimId::Mudokon_StandToRun,
    AnimId::Mudokon_Punch,
    AnimId::Mudokon_HoistBegin,
    AnimId::Mudokon_HoistIdle,
    AnimId::Mudokon_LandSoft,
    AnimId::Mudokon_LandSoft,
    AnimId::Mudokon_DunnoBegin,
    AnimId::Mudokon_DunnoEnd, // duplicates: 269928
    AnimId::Mudokon_KnockForward,
    AnimId::Mudokon_Knockback,
    AnimId::Mudokon_KnockbackGetUp,
    AnimId::Mudokon_WalkOffEdge,
    AnimId::Mudokon_FallingFromGrab,
    AnimId::Mudokon_Chant,
    AnimId::Mudokon_ChantEnd,
    AnimId::Mudokon_ToDuck,
    AnimId::Mudokon_Duck,
    AnimId::Mudokon_DuckToCrouch,
    AnimId::Mudokon_DuckKnockback,
    AnimId::Mudokon_SlapOwnHead,
    AnimId::Mudokon_TurnWheelBegin,
    AnimId::Mudokon_TurnWheel,
    AnimId::Mudokon_TurnWheelEnd};

ALIVE_VAR(1, 0x5C3010, s16, sAlertedMudCount_5C3010, 0);
ALIVE_VAR(1, 0x5C3018, s16, sIsMudStandingUp_5C3018, 0);


s32 Mudokon::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Mudokon_State*>(pBuffer);

    auto pTlv = static_cast<Path_Mudokon*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_40_tlvInfo));
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBSIC1.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeknfdResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEKNFD.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeknbkResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEKNBK.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeedgeResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEEDGE.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SHADOW.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMudidleResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("MUDIDLE.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, AEResourceID::kMudangryResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("MUDPAL.BND", nullptr);
    }

    if (pTlv->field_12_state != Mud_State::eChisle_0)
    {
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMudscrubResID, FALSE, FALSE))
        {
            ResourceManager::LoadResourceFile_49C170("MUDSCRUB.BAN", nullptr);
        }
    }
    else if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMudchslResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("MUDCHSL.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMudoduckResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("MUDWORK.BND", nullptr);
    }

    if (pState->field_24_current_motion == eMudMotions::M_LeverUse_10_473020 && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbepullResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEPULL.BAN", nullptr);
    }

    if (pState->field_24_current_motion == eMudMotions::M_TurnWheelBegin_57_474C00 && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEWORK.BAN", nullptr);
    }

    if (pState->field_24_current_motion == eMudMotions::M_TurnWheelLoop_58_474CC0 && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEWORK.BAN", nullptr);
    }

    if (pState->field_24_current_motion == eMudMotions::M_TurnWheelEnd_59_474D30 && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEWORK.BAN", nullptr);
    }

    auto pMud = relive_new Mudokon(pTlv, pState->field_40_tlvInfo);
    if (pMud)
    {
        if (pState->field_3D_bIsPlayer)
        {
            sControlledCharacter_5C1B8C = pMud;
        }

        pMud->BaseAliveGameObjectPathTLV = nullptr;
        pMud->BaseAliveGameObjectCollisionLine = nullptr;

        pMud->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_4_xpos;
        pMud->mBaseAnimatedWithPhysicsGameObject_YPos = pState->field_8_ypos;
        pMud->field_C4_velx = pState->field_C_velx;
        pMud->field_C8_vely = pState->field_10_vely;

        pMud->field_134_xVelSlowBy = pState->field_44_velx_slow_by;
        pMud->field_138_unused = pState->field_48_unused;

        pMud->field_C0_path_number = pState->field_14_path_number;
        pMud->field_C2_lvl_number = MapWrapper::FromAE(pState->field_16_lvl_number);
        pMud->field_CC_sprite_scale = pState->field_18_sprite_scale;

        pMud->field_D0_r = pState->field_1C_r;
        pMud->field_D2_g = pState->field_1E_g;
        pMud->field_D4_b = pState->field_20_b;

        pMud->mCurrentMotion = pState->field_24_current_motion;

        const AnimRecord& animRec = AnimRec(kMudFrameTableOffsets_55CD00[pMud->mCurrentMotion]);
        u8** ppRes = pMud->AnimBlockForMotion_474DC0(pState->field_24_current_motion);

        pMud->field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, ppRes);

        pMud->field_20_animation.field_92_current_frame = pState->field_26_anim_current_frame;
        pMud->field_20_animation.mFrameChangeCounter = pState->field_28_anim_frame_change_counter;

        pMud->field_20_animation.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pState->field_22_bFlipX & 1);
        pMud->field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_2A_bAnimRender & 1);

        pMud->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2B_bDrawable & 1);

        if (IsLastFrame(&pMud->field_20_animation))
        {
            pMud->field_20_animation.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
        }

        pMud->mHealth = pState->field_2C_health;
        pMud->mCurrentMotion = pState->field_30_current_motion;
        pMud->mNextMotion = pState->field_32_next_motion;
        pMud->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->field_34_lastLineYPos);

        pMud->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit3_Can_Be_Possessed, pState->field_3C_can_be_possessed & 1);
        pMud->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);

        pMud->BaseAliveGameObjectCollisionLineType = pState->field_36_line_type;
        pMud->field_11C_bird_portal_id = pState->field_4C_portal_id;
        pMud->field_120_angry_switch_id = pState->field_50_angry_trigger;
        pMud->field_124 = pState->field_54_savedfield124;
        pMud->field_128_angry_timer = pState->field_58_angry_timer;
        pMud->field_130_unused = pState->field_5C_unused;
        pMud->field_13C_voice_pitch = pState->field_5E_voice_pitch;
        pMud->field_158_wheel_id = pState->field_60_wheel_id;
        pMud->field_15C_unused = pState->field_64_unused;
        pMud->field_160_delayed_speak = pState->field_68;
        pMud->field_162_maxXOffset = pState->field_6A_maxXOffset;

        if (pState->field_6C.Get(Mudokon_State::Flags_6A::eBit2_unused))
        {
            pMud->field_12C_unused = 0;
        }
        else
        {
            pMud->field_12C_unused = -1;
        }

        pMud->field_16A_flags.Set(Flags_16A::eBit1_not_rescued, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit4_not_rescued));
        pMud->field_16A_flags.Set(Flags_16A::eBit2_persist_and_reset_offscreen, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit5_save_state));
        pMud->field_16A_flags.Set(Flags_16A::eBit3_alerted, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit6_alerted));
        pMud->field_16A_flags.Set(Flags_16A::eBit4_blind, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit7_blind));
        pMud->field_16A_flags.Set(Flags_16A::eBit5_following, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit8_following));
        pMud->field_16A_flags.Set(Flags_16A::eBit6_standing_for_sad_or_angry, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit9_standing_for_sad_or_angry));
        pMud->field_16A_flags.Set(Flags_16A::eBit7_stopped_at_wheel, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit10_stopped_at_wheel));
        pMud->field_16A_flags.Set(Flags_16A::eBit8_do_angry, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit11_do_angry));
        pMud->field_16A_flags.Set(Flags_16A::eBit9_seen_while_sick, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit12_seen_while_sick));
        pMud->field_16A_flags.Set(Flags_16A::eBit10_work_after_turning_wheel, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit13_stop_trigger));

        pMud->field_18C_unused = pState->field_6C.Get(Mudokon_State::Flags_6A::eBit14_unused);
        pMud->field_192_return_to_previous_motion = pState->field_6C.Get(Mudokon_State::Flags_6A::eBit15_return_to_previous_motion);

        pMud->field_16A_flags.Set(Flags_16A::eBit11_get_depressed, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit16_get_depressed));

        pMud->field_16A_flags.Set(Flags_16A::eBit12_alert_enemies, pState->field_6E.Get(Mudokon_State::Flags_6E::e6E_Bit1_alert_enemies));
        pMud->field_16A_flags.Set(Flags_16A::eBit13, pState->field_6E.Get(Mudokon_State::Flags_6E::e6E_Bit2));
        pMud->field_16A_flags.Set(Flags_16A::eBit14_make_sad_noise, pState->field_6E.Get(Mudokon_State::Flags_6E::e6E_Bit3_make_sad_noise));
        pMud->field_16A_flags.Set(Flags_16A::eBit15_ring_and_angry_mud_timeout, pState->field_6E.Get(Mudokon_State::Flags_6E::e6E_Bit4_ring_timeout));

        if (pState->field_6E.Get(Mudokon_State::Flags_6E::e6E_Bit5))
        {
            pMud->field_16C_flags.Set(Flags_16C::eBit1_Unknown);
        }
        else
        {
            pMud->field_16C_flags.Clear(Flags_16C::eBit1_Unknown);
        }

        if (pState->field_6E.Get(Mudokon_State::Flags_6E::e6E_Bit6))
        {
            pMud->field_16C_flags.Set(Flags_16C::eBit2_Unknown);
        }
        else
        {
            pMud->field_16C_flags.Clear(Flags_16C::eBit2_Unknown);
        }

        if (pMud->field_16A_flags.Get(Flags_16A::eBit3_alerted))
        {
            sAlertedMudCount_5C3010++;
        }

        pMud->field_178_brain_sub_state2 = pState->field_70_brain_sub_state2;
        pMud->field_17C_stand_idle_timer = pState->field_72_stand_idle_timer;
        pMud->field_17E_delayed_speak = pState->field_74_delayed_speak;
        pMud->field_180_emo_tbl = pState->field_76_emo_tlb;
        pMud->field_182 = pState->field_78;
        pMud->field_184_next_motion2 = pState->field_7A_motion;
        pMud->field_18E_brain_state = pState->field_7C_brain_state;
        pMud->field_190_brain_sub_state = pState->field_7E_brain_sub_state;
        pMud->field_194_timer = pState->field_80_timer;

        if (pState->field_6E.Get(Mudokon_State::Flags_6E::e6E_Bit6))
        {
            sIsMudStandingUp_5C3018 = 1;
        }

        pMud->field_188_pTblEntry = GetResponseEntry_471790(pState->field_84_response_entry_idx);
    }

    return sizeof(Mudokon_State);
}

s32 Mudokon::VGetSaveState(u8* pSaveBuffer)
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<Mudokon_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eRingOrLiftMud_81;

    pState->field_4_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->field_8_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;
    pState->field_C_velx = field_C4_velx;
    pState->field_10_vely = field_C8_vely;

    pState->field_44_velx_slow_by = field_134_xVelSlowBy;
    pState->field_48_unused = field_138_unused;

    pState->field_14_path_number = field_C0_path_number;
    pState->field_16_lvl_number = MapWrapper::ToAE(field_C2_lvl_number);
    pState->field_18_sprite_scale = field_CC_sprite_scale;

    pState->field_1C_r = field_D0_r;
    pState->field_1E_g = field_D2_g;
    pState->field_20_b = field_D4_b;

    pState->field_22_bFlipX = field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    pState->field_24_current_motion = mCurrentMotion;
    pState->field_26_anim_current_frame = field_20_animation.field_92_current_frame;
    pState->field_28_anim_frame_change_counter = field_20_animation.mFrameChangeCounter;
    pState->field_2B_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2A_bAnimRender = field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render);
    pState->field_2C_health = mHealth;
    pState->field_30_current_motion = mCurrentMotion;
    pState->field_32_next_motion = mNextMotion;
    pState->field_34_lastLineYPos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);
    pState->field_36_line_type = -1;

    pState->field_3C_can_be_possessed = mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit3_Can_Be_Possessed);

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_36_line_type = BaseAliveGameObjectCollisionLine->field_8_type;
    }

    pState->field_3D_bIsPlayer = this == sControlledCharacter_5C1B8C;
    pState->field_40_tlvInfo = field_118_tlvInfo;
    pState->field_4C_portal_id = -1;

    if (field_11C_bird_portal_id != -1)
    {
        BaseGameObject* pBirdPortal = sObjectIds.Find_Impl(field_11C_bird_portal_id);
        if (pBirdPortal)
        {
            pState->field_4C_portal_id = pBirdPortal->mBaseGameObjectTlvInfo;
        }
    }

    pState->field_50_angry_trigger = field_120_angry_switch_id;
    pState->field_54_savedfield124 = field_124;
    pState->field_58_angry_timer = field_128_angry_timer;
    pState->field_5C_unused = field_130_unused;
    pState->field_5E_voice_pitch = field_13C_voice_pitch;
    pState->field_60_wheel_id = -1;

    if (field_158_wheel_id != -1)
    {
        BaseGameObject* pWheel = sObjectIds.Find_Impl(field_158_wheel_id);
        if (pWheel)
        {
            pState->field_60_wheel_id = pWheel->mBaseGameObjectTlvInfo;
        }
    }

    pState->field_64_unused = field_15C_unused;
    pState->field_68 = field_160_delayed_speak;
    pState->field_6A_maxXOffset = field_162_maxXOffset;

    // bit1 never used ??
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit2_unused, field_12C_unused != -1);
    // bit3 never used ??
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit4_not_rescued, field_16A_flags.Get(Flags_16A::eBit1_not_rescued));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit5_save_state, field_16A_flags.Get(Flags_16A::eBit2_persist_and_reset_offscreen));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit6_alerted, field_16A_flags.Get(Flags_16A::eBit3_alerted));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit7_blind, field_16A_flags.Get(Flags_16A::eBit4_blind));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit8_following, field_16A_flags.Get(Flags_16A::eBit5_following));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit9_standing_for_sad_or_angry, field_16A_flags.Get(Flags_16A::eBit6_standing_for_sad_or_angry));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit10_stopped_at_wheel, field_16A_flags.Get(Flags_16A::eBit7_stopped_at_wheel));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit11_do_angry, field_16A_flags.Get(Flags_16A::eBit8_do_angry));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit12_seen_while_sick, field_16A_flags.Get(Flags_16A::eBit9_seen_while_sick));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit13_stop_trigger, field_16A_flags.Get(Flags_16A::eBit10_work_after_turning_wheel));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit14_unused, field_18C_unused & 1);
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit15_return_to_previous_motion, field_192_return_to_previous_motion & 1);
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit16_get_depressed, field_16A_flags.Get(Flags_16A::eBit11_get_depressed));

    pState->field_6E.Set(Mudokon_State::Flags_6E::e6E_Bit1_alert_enemies, field_16A_flags.Get(Flags_16A::eBit12_alert_enemies));
    pState->field_6E.Set(Mudokon_State::Flags_6E::e6E_Bit2, field_16A_flags.Get(Flags_16A::eBit13));
    pState->field_6E.Set(Mudokon_State::Flags_6E::e6E_Bit3_make_sad_noise, field_16A_flags.Get(Flags_16A::eBit14_make_sad_noise));
    pState->field_6E.Set(Mudokon_State::Flags_6E::e6E_Bit4_ring_timeout, field_16A_flags.Get(Flags_16A::eBit15_ring_and_angry_mud_timeout));
    pState->field_6E.Set(Mudokon_State::Flags_6E::e6E_Bit5, field_16C_flags.Get(Flags_16C::eBit1_Unknown));
    pState->field_6E.Set(Mudokon_State::Flags_6E::e6E_Bit6, field_16C_flags.Get(Flags_16C::eBit2_Unknown));

    pState->field_70_brain_sub_state2 = field_178_brain_sub_state2;
    pState->field_72_stand_idle_timer = field_17C_stand_idle_timer;
    pState->field_74_delayed_speak = field_17E_delayed_speak;
    pState->field_76_emo_tlb = field_180_emo_tbl;
    pState->field_78 = field_182;
    pState->field_7A_motion = field_184_next_motion2;
    pState->field_7C_brain_state = field_18E_brain_state;
    pState->field_7E_brain_sub_state = field_190_brain_sub_state;
    pState->field_80_timer = field_194_timer;

    pState->field_84_response_entry_idx = GetResponseEntryIdx_471760();

    return sizeof(Mudokon_State);
}

void Mudokon::VUpdate()
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
        if (BaseAliveGameObjectCollisionLineType != -1)
        {
            sCollisions->Raycast(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(20),
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(20),
                &BaseAliveGameObjectCollisionLine,
                &mBaseAnimatedWithPhysicsGameObject_XPos,
                &mBaseAnimatedWithPhysicsGameObject_YPos,
                1 << BaseAliveGameObjectCollisionLineType);

            if (BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_32 ||
                BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_36)
            {
                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);

                VOnCollisionWith(
                    {bRect.x, static_cast<s16>(bRect.y + 5)},
                    {bRect.w, static_cast<s16>(bRect.h + 5)},
                    ObjList_5C1B78,
                    1,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
            }
        }

        BaseAliveGameObjectCollisionLineType = 0;

        if (field_11C_bird_portal_id != -1)
        {
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->mBaseGameObjectTlvInfo == field_11C_bird_portal_id)
                {
                    field_11C_bird_portal_id = pObj->field_8_object_id;
                    sGoingToBirdPortalMudCount_5C3012++;
                    field_16C_flags.Set(Flags_16C::eBit3_Unknown);
                    if (field_18E_brain_state == Mud_Brain_State::Brain_6_Escape_47A560 && field_190_brain_sub_state == 3)
                    {
                        static_cast<BirdPortal*>(pObj)->VPortalClipper(1);
                        field_20_animation.mRenderLayer = field_CC_sprite_scale != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30;
                    }
                    break;
                }
            }
        }

        if (field_158_wheel_id != -1)
        {
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->mBaseGameObjectTlvInfo == field_158_wheel_id)
                {
                    field_158_wheel_id = pObj->field_8_object_id;
                    static_cast<WorkWheel*>(pObj)->VStartTurning();
                    break;
                }
            }
        }
    }

    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    const FP xDistFromPlayer = FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos);
    if (xDistFromPlayer > FP_FromInteger(750))
    {
        field_20_animation.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
        field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        return;
    }

    const FP yDistanceFromPlayer = FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos);
    if (yDistanceFromPlayer > FP_FromInteger(520))
    {
        field_20_animation.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
        field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        return;
    }

    if (mHealth > FP_FromInteger(0))
    {
        field_20_animation.mAnimFlags.Set(AnimFlags::eBit2_Animate);
        field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render);
    }

    //DDCheat::DebugStr_4F5560("\nMotion = %s BrainState = %s\n", sMudMotionStateNames[mCurrentMotion], sMudAiStateNames[static_cast<s32>(field_18E_brain_state)]);
    //LOG_INFO(sMudMotionStateNames[mCurrentMotion] << " " << static_cast<s32>(field_18E_brain_state));
    //LOG_INFO(sMudMotionStateNames[mCurrentMotion] << " " << sMudAiStateNames[static_cast<s32>(field_18E_brain_state)]);

    const s16 oldMotion = mCurrentMotion;

    field_190_brain_sub_state = (this->*sMudokon_Brain_Table_55CDF0[field_18E_brain_state])();

    const FP oldXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP oldYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

    (this->*sMudokon_motion_states_55CE18[mCurrentMotion])();

    if (oldXPos != mBaseAnimatedWithPhysicsGameObject_XPos || oldYPos != mBaseAnimatedWithPhysicsGameObject_YPos)
    {
        BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            nullptr,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos);
        VOn_TLV_Collision(BaseAliveGameObjectPathTLV);
    }

    if (oldMotion != mCurrentMotion || mBaseAliveGameObjectFlags.Get(Flags_114::e114_MotionChanged_Bit2))
    {
        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_MotionChanged_Bit2);
        VUpdateAnimRes();
    }
    else if (field_192_return_to_previous_motion)
    {
        mCurrentMotion = mPreviousMotion;
        VUpdateAnimRes();
        field_20_animation.SetFrame(mBaseAliveGameObjectLastAnimFrame);
        field_192_return_to_previous_motion = 0;
    }
}


void Mudokon::SetPal(Mud_Emotion emotion)
{
    switch (emotion)
    {
        case Mud_Emotion::eNormal_0:
            if (field_16A_flags.Get(Flags_16A::eBit4_blind))
            {
                SetRGB(63, 63, 63);
                field_20_animation.Load_Pal(field_10_resources_array.ItemAt(16), 0);
            }
            else
            {
                SetRGB(87, 103, 67);
                FrameInfoHeader* pFrameInfoHeader = field_20_animation.Get_FrameHeader(-1);
                FrameHeader* pHeader = reinterpret_cast<FrameHeader*>(&(*field_20_animation.field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);
                field_20_animation.Load_Pal(field_20_animation.field_20_ppBlock, pHeader->field_0_clut_offset);
            }
            break;

        case Mud_Emotion::eAngry_1:
        case Mud_Emotion::eAggressive_2:
            SetRGB(63, 63, 63);
            field_20_animation.Load_Pal(field_10_resources_array.ItemAt(13), 0);
            break;

        case Mud_Emotion::eSad_3:
        case Mud_Emotion::eSuicidal_4:
            SetRGB(63, 63, 63);
            field_20_animation.Load_Pal(field_10_resources_array.ItemAt(14), 0);
            break;

        case Mud_Emotion::eHappy_5:
        case Mud_Emotion::eWired_6:
            SetRGB(74, 74, 74);
            field_20_animation.Load_Pal(field_10_resources_array.ItemAt(15), 0);
            break;

        case Mud_Emotion::eSick_7:
            SetRGB(63, 63, 63);
            field_20_animation.Load_Pal(field_10_resources_array.ItemAt(17), 0);
            break;
    }
}

void Mudokon::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));
    if (pPlatform)
    {
        if (!mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit1_bShot))
        {
            VSetMotion(eMudMotions::M_WalkOffEdge_48_4743C0);
        }

        pPlatform->VRemove(this);
        BaseAliveGameObjectId = -1;
    }
}

void Mudokon::VOn_TLV_Collision(Path_TLV* pTlv)
{
    Path_TLV* pTlvIter = pTlv;
    while (pTlvIter)
    {
        if (pTlvIter->field_4_type == TlvTypes::DeathDrop_4)
        {
            if (mHealth > FP_FromInteger(0))
            {
                field_18E_brain_state = Mud_Brain_State::Brain_7_FallAndSmackDeath_471600;
                field_190_brain_sub_state = 0;
                mHealth = FP_FromInteger(0);
                Event_Broadcast(kEventMudokonDied, this);
                break;
            }
        }

        pTlvIter = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            pTlvIter,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos);
    }
}

s16 Mudokon::FacingTarget(BirdPortal* pTarget)
{
    if (pTarget->field_2C_xpos == mBaseAnimatedWithPhysicsGameObject_XPos)
    {
        return TRUE;
    }
    else if (pTarget->field_2C_xpos > mBaseAnimatedWithPhysicsGameObject_XPos && !field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }
    else if (pTarget->field_2C_xpos < mBaseAnimatedWithPhysicsGameObject_XPos && field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }
    return FALSE;
}

Mudokon::~Mudokon()
{
    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds.Find_Impl(field_11C_bird_portal_id));
    if (mHealth <= FP_FromInteger(0))
    {
        // TODO: Refactor all access to helpers
        sKilledMudokons_5C1BC0++;
        if (sZulagNumber_5C1A20 < ALIVE_COUNTOF(sSavedKilledMudsPerZulag_5C1B50.mData))
        {
            sSavedKilledMudsPerZulag_5C1B50.mData[sZulagNumber_5C1A20]++;
        }
    }

    auto pWheel = static_cast<WorkWheel*>(sObjectIds.Find_Impl(field_158_wheel_id));
    if (pWheel)
    {
        pWheel->VStopTurning(TRUE);
        field_158_wheel_id = -1;
    }

    RemoveAlerted();

    if (field_16C_flags.Get(Flags_16C::eBit3_Unknown))
    {
        sGoingToBirdPortalMudCount_5C3012--;
        field_16C_flags.Clear(Flags_16C::eBit3_Unknown);
        if (!sGoingToBirdPortalMudCount_5C3012)
        {
            if (pBirdPortal)
            {
                pBirdPortal->VKillPortalClipper();
                pBirdPortal->VGiveShrykull(TRUE);
            }
        }
        field_11C_bird_portal_id = -1;
    }

    if (!(field_16A_flags.Get(Flags_16A::eBit1_not_rescued)) || mHealth <= FP_FromInteger(0) || mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        Path::TLV_Reset(field_118_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(field_118_tlvInfo, -1, 0, 0);
    }

    if (!field_18E_brain_state && field_190_brain_sub_state > 4u)
    {
        if (field_164_ring_pulse_interval > 0)
        {
            sActiveHero->field_168_ring_pulse_timer = sGnFrame + field_164_ring_pulse_interval;
        }
        else
        {
            sActiveHero->field_168_ring_pulse_timer = sGnFrame + 200000;
        }

        sActiveHero->field_16C_bHaveShrykull = FALSE;

        if (field_168_ring_type == RingTypes::eHealing_Emit_Effect_11)
        {
            sActiveHero->field_1AC_flags.Set(Abe::e1AC_eBit15_have_healing);
        }
    }

    if (mCurrentMotion == eMudMotions::M_Chant_50_473040)
    {
        SND_SEQ_Stop(SeqId::MudokonChant_11);
    }
}

void Mudokon::VScreenChanged()
{
    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds.Find_Impl(field_11C_bird_portal_id));
    BaseGameObject::VScreenChanged();

    if (field_18E_brain_state == Mud_Brain_State::Brain_0_GiveRings_470C10 ||
        !field_16A_flags.Get(Flags_16A::eBit2_persist_and_reset_offscreen))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        // TODO: Duplicated in dtors + other places
        if (field_16C_flags.Get(Flags_16C::eBit3_Unknown))
        {
            sGoingToBirdPortalMudCount_5C3012--;
            field_16C_flags.Clear(Flags_16C::eBit3_Unknown);
            if (!sGoingToBirdPortalMudCount_5C3012)
            {
                if (pBirdPortal)
                {
                    pBirdPortal->VKillPortalClipper();
                    pBirdPortal->VGiveShrykull(TRUE);
                }
            }
            field_11C_bird_portal_id = -1;
        }
    }
}

void Mudokon::VPossessed()
{
    sControlledCharacter_5C1B8C = sActiveHero;
    if (field_180_emo_tbl == Mud_Emotion::eSick_7 && !FindObjectOfType(ReliveTypes::eTorturedMud, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(50)))
    {
        field_180_emo_tbl = Mud_Emotion::eNormal_0;
        if (field_188_pTblEntry)
        {
            field_188_pTblEntry = Mudokon::ResponseTo_471730(Mud_Emotion::eNormal_0, MudAction::eHelloOrAllYa_0);
        }
    }
}

s16 Mudokon::VTakeDamage(BaseGameObject* pFrom)
{
    switch (pFrom->Type())
    {
        case ReliveTypes::eBullet:
        {
            mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit1_bShot);
            if (mHealth <= FP_FromInteger(0))
            {
                return 1;
            }

            auto pBullet = static_cast<Bullet*>(pFrom);
            switch (pBullet->field_20_type)
            {
                case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
                case BulletType::eNormalBullet_2:
                {
                    relive_new Blood(
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * field_CC_sprite_scale),
                        pBullet->field_30_x_distance <= FP_FromInteger(0) ? FP_FromInteger(-24) : FP_FromInteger(24),
                        FP_FromInteger(0),
                        field_CC_sprite_scale,
                        50);
                    SetPal(Mud_Emotion::eNormal_0);
                    break;
                }

                default:
                    break;
            }

            if (pBullet->field_20_type == BulletType::ePossessedSligZBullet_1 || pBullet->field_20_type == BulletType::eZBullet_3)
            {
                PSX_RECT v11 = {};
                VGetBoundingRect(&v11, 1);
                const FP tlvYPos = FP_FromInteger(v11.h);

                if (Bullet::InZBulletCover(mBaseAnimatedWithPhysicsGameObject_XPos, tlvYPos, v11) || !gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, tlvYPos, 0))
                {
                    // ZCover saved us, or somehow we've not in the current camera
                    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
                    mHealth = FP_FromInteger(1);
                    return 0;
                }

                // Nothing saved us, get shot
                relive_new Blood(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * field_CC_sprite_scale),
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    50);

                SetPal(Mud_Emotion::eNormal_0);
                // Fall though to other cases below
            }

            [[fallthrough]];
        }

        case ReliveTypes::eGasClock:
        case ReliveTypes::eRockSpawner:
        case ReliveTypes::eMineCar:
        case ReliveTypes::eParamite:
        case ReliveTypes::eScrab:
            if (mHealth <= FP_FromInteger(0))
            {
                return 0;
            }
            field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);
            mHealth = FP_FromInteger(0);
            field_18E_brain_state = Mud_Brain_State::Brain_5_ShrivelDeath_4714A0;
            mCurrentMotion = eMudMotions::M_KnockForward_45_474180;
            mNextMotion = -1;
            field_194_timer = sGnFrame + 90;
            VUpdateAnimRes();
            Event_Broadcast(kEventMudokonDied, this);
            if (pFrom->Type() == ReliveTypes::eGasClock)
            {
                SFX_Play_Pitch(SoundEffect::Choke_81, 127, 128);
            }
            else
            {
                Mudokon_SFX(MudSounds::eHurt2_9, 0, Math_RandomRange(-127, 127), this);
            }
            SetPal(Mud_Emotion::eNormal_0);
            return 1;

        case ReliveTypes::eDrill:
        case ReliveTypes::eBaseBomb:
        case ReliveTypes::eMeatSaw:
        case ReliveTypes::eExplosion:
            if (mHealth <= FP_FromInteger(0) || (FindObjectOfType(ReliveTypes::eTorturedMud, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(50)) && field_18E_brain_state == Mud_Brain_State::Brain_9_Sick_47A910))
            {
                return 1;
            }

            mHealth = FP_FromInteger(0);

            if (field_16A_flags.Get(Flags_16A::eBit4_blind))
            {
                relive_new Gibs(
                    GibType::BlindMud_4,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    0);

                relive_new Gibs(
                    GibType::BlindMud_4,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    0);
            }
            else
            {
                relive_new Gibs(
                    GibType::Mud_3,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    0);

                relive_new Gibs(
                    GibType::Mud_3,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    0);
            }

            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            SetPal(Mud_Emotion::eNormal_0);
            Event_Broadcast(kEventMudokonDied, sActiveHero);
            return 1;

        case ReliveTypes::eElectricWall:
            Mudokon_SFX(MudSounds::eDeathDropScream_15, 0, 0, this);
            Event_Broadcast(kEventMudokonDied, this);
            return 1;

        case ReliveTypes::eFleech:
            if (mHealth <= FP_FromInteger(0))
            {
                return 1;
            }

            mHealth -= FP_FromDouble(0.200988769531); // TODO Do we need this level of accuracy?? 0x3374;
            if (mHealth <= FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
                Mudokon_SFX(MudSounds::eHurt1_16, 0, 1000, this);
                field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);
                field_18E_brain_state = Mud_Brain_State::Brain_5_ShrivelDeath_4714A0;
                field_194_timer = sGnFrame + 90;
                mCurrentMotion = eMudMotions::M_Knockback_46_4742A0;
                mNextMotion = -1;
                Event_Broadcast(kEventMudokonDied, this);
                SetPal(Mud_Emotion::eNormal_0);

                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);

                auto pFleech = static_cast<BaseAliveGameObject*>(pFrom);
                relive_new Blood(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    (FP_FromInteger(bRect.y + bRect.h) / FP_FromInteger(2)),
                    mBaseAnimatedWithPhysicsGameObject_XPos - pFleech->mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(0) ? FP_FromInteger(-24) : FP_FromInteger(24),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    50);

                // TODO: Only set if pFrom->mBaseAnimatedWithPhysicsGameObject_XPos != mBaseAnimatedWithPhysicsGameObject_XPos ??
                mCurrentMotion = eMudMotions::M_KnockForward_45_474180;

                VUpdateAnimRes();
                SFX_Play_Mono(SoundEffect::KillEffect_64, 127);
                SFX_Play_Mono(SoundEffect::FallingItemHit_47, 90);
            }
            else
            {
                HurtSound_475DB0();
            }
            return 1;

        case ReliveTypes::eAbe:
            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_62_Punch_454750)
            {
                if (mHealth > FP_FromInteger(0))
                {
                    SetPal(Mud_Emotion::eNormal_0);
                    TakeASlap_476090(pFrom);
                }
            }
            else if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_63_Sorry_454670)
            {
                field_17E_delayed_speak = MudAction::eSorry_8;
            }
            return 1;

        case ReliveTypes::eAbilityRing:
            return 0;

        case ReliveTypes::eMudokon:
            if (static_cast<Mudokon*>(pFrom)->mCurrentMotion != eMudMotions::M_Punch_38_474AA0 || mHealth <= FP_FromInteger(0))
            {
                return 1;
            }
            SetPal(Mud_Emotion::eNormal_0);
            TakeASlap_476090(pFrom);
            return 1;

        case ReliveTypes::eShrykull:
        case ReliveTypes::eElectrocute:
            if (mHealth <= FP_FromInteger(0))
            {
                return 1;
            }
            mHealth = FP_FromInteger(0);
            Event_Broadcast(kEventMudokonDied, this);
            SetPal(Mud_Emotion::eNormal_0);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            return 1;

        case ReliveTypes::eSlamDoor:
            if (mHealth <= FP_FromInteger(0) || mCurrentMotion == eMudMotions::M_RunJumpMid_36_474570)
            {
                return 1;
            }
            ToKnockback_473190();
            VUpdateAnimRes();
            return 1;

        case ReliveTypes::eSlog:
            if (mHealth <= FP_FromInteger(0))
            {
                return 1;
            }
            field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);
            mHealth = FP_FromInteger(0);
            field_18E_brain_state = Mud_Brain_State::Brain_5_ShrivelDeath_4714A0;
            field_194_timer = sGnFrame + 90;
            mCurrentMotion = eMudMotions::M_KnockForward_45_474180;
            Event_Broadcast(kEventMudokonDied, this);
            VUpdateAnimRes();
            SetPal(Mud_Emotion::eNormal_0);
            return 1;

        default:
            if (mHealth <= FP_FromInteger(0))
            {
                return 1;
            }
            HurtSound_475DB0();
            mCurrentMotion = eMudMotions::M_KnockForward_45_474180;
            mNextMotion = -1;
            field_194_timer = sGnFrame + 30;
            VUpdateAnimRes();
            if (field_10_resources_array.ItemAt(2))
            {
                field_18E_brain_state = Mud_Brain_State::Brain_1_Chisel_47C5F0;
                field_190_brain_sub_state = 8;
            }
            else
            {
                field_18E_brain_state = Mud_Brain_State::Brain_2_Scrub_47D270;
                field_190_brain_sub_state = 7;
            }

            field_DA_xOffset = field_162_maxXOffset;

            RemoveAlerted();

            if (pFrom == sControlledCharacter_5C1B8C)
            {
                mHealth -= FP_FromDouble(0.06);
                if (mHealth > FP_FromInteger(0))
                {
                    Event_Broadcast(kEventMudokonAbuse, this);
                }
                else
                {
                    field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);
                    field_18E_brain_state = Mud_Brain_State::Brain_5_ShrivelDeath_4714A0;
                    field_194_timer = sGnFrame + 90;
                    Event_Broadcast(kEventMudokonDied, this);
                }
            }
            SetPal(Mud_Emotion::eNormal_0);
            return 1;
    }
}

s16 Mudokon::TurningWheelHelloOrAllYaResponse()
{
    if (!field_16A_flags.Get(Flags_16A::eBit10_work_after_turning_wheel))
    {
        return field_190_brain_sub_state;
    }

    auto pWheelTlv = static_cast<Path_WorkWheel*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        TlvTypes::WorkWheel_79));

    if (SwitchStates_Get(pWheelTlv->field_12_switch_id))
    {
        if (field_198_turning_wheel_timer == 0)
        {
            field_198_turning_wheel_timer = sGnFrame + 20;
        }
    }

    // OG Bug: The second condition can never resolve to true because field_198_turning_wheel_timer will always be reset to zero before it can happen.
    if (!SwitchStates_Get(pWheelTlv->field_12_switch_id) || field_198_turning_wheel_timer > static_cast<s32>(sGnFrame))
    {
        return field_190_brain_sub_state;
    }

    field_198_turning_wheel_timer = 0;
    field_16A_flags.Clear(Flags_16A::eBit7_stopped_at_wheel);
    field_194_timer = sGnFrame + 45;
    return 1;
}

void Mudokon::VUpdateAnimRes()
{
    u8** ppRes = AnimBlockForMotion_474DC0(mCurrentMotion);
    if (!ppRes)
    {
        LOG_ERROR("No res for " << mCurrentMotion);
    }
    const AnimRecord& animRec = AnimRec(kMudFrameTableOffsets_55CD00[mCurrentMotion]);
    field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, ppRes);
}

enum Brain_0_GiveRings
{
    eBrain0_Inactive_0 = 0,
    eBrain0_PreIdle_1 = 1,
    eBrain0_Idle_2 = 2,
    eBrain0_GoodGameSpeak_3 = 3,
    eBrain0_WrongGameSpeak_4 = 4,
    eBrain0_Shrug_5 = 5,
    eBrain0_SaysOkay_6 = 6,
    eBrain0_Chanting_7 = 7,
    eBrain0_GivingRing_8 = 8,
    eBrain0_ReceivingRing1_9 = 9,
    eBrain0_RecievingRing2_10 = 10
};

enum Brain_1_Chisle
{
    eBrain1_StartToChisle_0 = 0,
    eBrain1_ChisleTheFloor_1 = 1,
    eBrain1_SmallBreak_2 = 2,
    eBrain1_StandUp_3 = 3,
    eBrain1_SadNoise_4 = 4,
    eBrain1_Duck_5 = 5,
    eBrain1_OutOfDuck_6 = 6,
    eBrain1_DuckToChisle_7 = 7,
    eBrain1_DuckKnockback_8 = 8
};

enum Brain_2_Scrub
{
    eBrain2_StartToScrub_0 = 0,
    eBrain2_SmallBreak_1 = 1,
    eBrain2_Scrubbing_2 = 2,
    eBrain2_StandUp_3 = 3,
    eBrain2_SadNoise_4 = 4,
    eBrain2_SmallBreakThroughGameSpeak_5 = 5,
    eBrain2_Duck_6 = 6,
    eBrain2_DuckKnockback_7 = 7
};

enum Brain_3_TurnWheel
{
    eBrain3_TurningWheel_0 = 0,
    eBrain3_SittingDown_1 = 1,
    eBrain3_InterruptAction_2 = 2,
    eBrain3_TurningWheelToDuck_3 = 3,
    eBrain3_Duck_4 = 4,
    eBrain3_DuckToTurningWheel_5 = 5
};

enum Brain_4_ListeningToAbe
{
    eBrain4_Inactive_0 = 0,
    eBrain4_GetsAttentive_1 = 1,
    eBrain4_CrazyFollowMe_2 = 2,
    eBrain4_SteppingBack_3 = 3,
    eBrain4_FollowingIdle_4 = 4,
    eBrain4_Walking_5 = 5,
    eBrain4_Running_6 = 6,
    eBrain4_Idle_7 = 7,
    eBrain4_CrazySlideTurn_8 = 8,
    eBrain4_GetsWaitWhileMoving_9 = 9,
    eBrain4_InitializeMovement_10 = 10,
    eBrain4_PullingLever_11 = 11,
    eBrain4_GetsCommand_12 = 12,
    eBrain4_Slapped_13 = 13,
    eBrain4_Knockback_14 = 14,
    eBrain4_GettingSorry_15 = 15,
    eBrain4_Suiciding_16 = 16,
    eBrain4_RageSlap_17 = 17,
    eBrain4_RageTurn_18 = 18,
    eBrain4_CrazyDeny_19 = 19,
    eBrain4_StopsWhileMoving_20 = 20,
    eBrain4_CrazyLaugh_21 = 21,
    eBrain4_LostAttention_22 = 22
};

enum Brain_6_Escape
{
    eBrain6_PortalOppened_0 = 0,
    eBrain6_StandingUp_1 = 1,
    eBrain6_Running_2 = 2,
    eBrain6_Jumping_3 = 3,
    eBrain6_Replacing_4 = 4
};

enum Brain_8_AngryWorker
{
    eBrain8_Inactive_0 = 0,
    eBrain8_LeverIdle_1 = 1,
    eBrain8_PullingLever_2 = 2,
    eBrain8_Speaking_3 = 3,
    eBrain8_WheelIdle_4 = 4,
    eBrain8_UsingWheel_5 = 5,
    eBrain8_Unused_6 = 6,
    eBrain8_Unused_7 = 7
};

enum Brain_9_Sick
{
    eBrain9_Inactive_0 = 0,
    eBrain9_Idle_1 = 1,
    eBrain9_Duck_2 = 2,
    eBrain9_StandingUp_3 = 3,
    eBrain9_Farting_4 = 4,
};

s16 Mudokon::Brain_0_GiveRings_470C10()
{
    switch (field_190_brain_sub_state)
    {
        case Brain_0_GiveRings::eBrain0_Inactive_0:
            if (gMap.Is_Point_In_Current_Camera_4810D0(
                    sActiveHero->field_C2_lvl_number,
                    sActiveHero->field_C0_path_number,
                    sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos,
                    sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos,
                    0))
            {
                field_16C_flags.Clear(Flags_16C::eBit1_Unknown);
                field_194_timer = sGnFrame + 60;
                mNextMotion = eMudMotions::M_Idle_0_4724E0;

                if (sActiveHero->field_168_ring_pulse_timer)
                {
                    field_16C_flags.Set(Flags_16C::eBit1_Unknown);
                    return Brain_0_GiveRings::eBrain0_Idle_2;
                }
                else
                {
                    return Brain_0_GiveRings::eBrain0_PreIdle_1;
                }
            }
            break;

        case Brain_0_GiveRings::eBrain0_PreIdle_1:
            if (static_cast<s32>(sGnFrame) >= field_194_timer && mCurrentMotion == eMudMotions::M_Idle_0_4724E0)
            {
                if (VIsFacingMe(sActiveHero))
                {
                    Sound_475EC0(MudSounds::eHelloNeutral_3);
                    mNextMotion = eMudMotions::M_Speak_4_472FA0;
                    field_194_timer = StableDelay_477570() + sGnFrame + 150;
                    return Brain_0_GiveRings::eBrain0_Idle_2;
                }
                mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
            }
            break;

        case Brain_0_GiveRings::eBrain0_Idle_2:
            if (field_16A_flags.Get(Flags_16A::eBit16_give_ring_without_password) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && gMap.Is_Point_In_Current_Camera_4810D0(sActiveHero->field_C2_lvl_number, sActiveHero->field_C0_path_number, sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos, sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos, 0)
                && !sActiveHero->field_168_ring_pulse_timer)
            {
                field_194_timer = StableDelay_477570() + sGnFrame + 20;
                return Brain_0_GiveRings::eBrain0_SaysOkay_6;
            }
            else
            {
                const s32 lastEventIdx = pEventSystem_5BC11C->field_28_last_event_index;
                const bool bSameAsLastIdx = field_140_last_event_index == lastEventIdx;
                if (!bSameAsLastIdx)
                {
                    field_140_last_event_index = lastEventIdx;
                }

                if (bSameAsLastIdx || pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eNone_m1 || pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eSameAsLast_m2)
                {
                    if (!(field_16C_flags.Get(Flags_16C::eBit1_Unknown)) && static_cast<s32>(sGnFrame) > field_194_timer)
                    {
                        return Brain_0_GiveRings::eBrain0_Shrug_5;
                    }
                }
                else
                {
                    field_194_timer = StableDelay_477570() + sGnFrame + 20;
                    if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eHello_9)
                    {
                        return Brain_0_GiveRings::eBrain0_GoodGameSpeak_3;
                    }
                    else
                    {
                        return Brain_0_GiveRings::eBrain0_WrongGameSpeak_4;
                    }
                }
            }
            break;

        case Brain_0_GiveRings::eBrain0_GoodGameSpeak_3:
            if (static_cast<s32>(sGnFrame) >= field_194_timer && mCurrentMotion == eMudMotions::M_Idle_0_4724E0)
            {
                if (VIsFacingMe(sActiveHero))
                {
                    if (sActiveHero->field_168_ring_pulse_timer > 0)
                    {
                        Sound_475EC0(MudSounds::eHelloNeutral_3);
                    }
                    else
                    {
                        Sound_475EC0(MudSounds::eOkay_12);
                    }
                    mNextMotion = eMudMotions::M_Speak_4_472FA0;
                    return Brain_0_GiveRings::eBrain0_SaysOkay_6;
                }
                mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
            }
            break;

        case Brain_0_GiveRings::eBrain0_WrongGameSpeak_4:
            if (static_cast<s32>(sGnFrame) >= field_194_timer && mCurrentMotion == eMudMotions::M_Idle_0_4724E0)
            {
                if (VIsFacingMe(sActiveHero))
                {
                    mNextMotion = eMudMotions::M_DunnoBegin_43_472790;
                    return Brain_0_GiveRings::eBrain0_Idle_2;
                }
                mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
            }
            break;

        case Brain_0_GiveRings::eBrain0_Shrug_5:
            if (static_cast<s32>(sGnFrame) >= field_194_timer && mCurrentMotion == eMudMotions::M_Idle_0_4724E0)
            {
                if (VIsFacingMe(sActiveHero))
                {
                    mNextMotion = eMudMotions::M_DunnoBegin_43_472790;
                    field_194_timer = StableDelay_477570() + sGnFrame + 20;
                    return Brain_0_GiveRings::eBrain0_PreIdle_1;
                }
                mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
            }
            break;

        case Brain_0_GiveRings::eBrain0_SaysOkay_6:
            if (mCurrentMotion == eMudMotions::M_Idle_0_4724E0)
            {
                if (sActiveHero->field_168_ring_pulse_timer <= 0)
                {
                    mNextMotion = eMudMotions::M_Chant_50_473040;
                    field_194_timer = sGnFrame + 30;
                    return Brain_0_GiveRings::eBrain0_Chanting_7;
                }
                else
                {
                    return Brain_0_GiveRings::eBrain0_Idle_2;
                }
            }
            break;

        case Brain_0_GiveRings::eBrain0_Chanting_7:
            if (static_cast<s32>(sGnFrame) > field_194_timer)
            {
                if (field_168_ring_type == RingTypes::eExplosive_Emit_Effect_2)
                {
                    // Red flicker
                    relive_new PossessionFlicker(this, 10, 255, 128, 128);
                }
                else
                {
                    // Greenish flicker
                    relive_new PossessionFlicker(this, 10, 255, 255, 32);
                }
                field_194_timer = sGnFrame + 15;
                return Brain_0_GiveRings::eBrain0_GivingRing_8;
            }
            break;

        case Brain_0_GiveRings::eBrain0_GivingRing_8:
            if (static_cast<s32>(sGnFrame) > field_194_timer)
            {
                // Create a ring emitting from us
                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);

                AbilityRing::Factory(
                    FP_FromInteger((bRect.x + bRect.w) / 2),
                    FP_FromInteger((bRect.y + bRect.h) / 2),
                    field_168_ring_type,
                    field_CC_sprite_scale);

                // Create a ring that locks onto abe
                PSX_RECT bRectAbe = {};
                sActiveHero->VGetBoundingRect(&bRectAbe, 1);

                RingTypes ringTypeToGive = RingTypes::eExplosive_Give_3;
                if (field_168_ring_type == RingTypes::eHealing_Emit_Effect_11)
                {
                    ringTypeToGive = RingTypes::eHealing_Give_13;
                }

                AbilityRing* pRing = AbilityRing::Factory(
                    FP_FromInteger((bRectAbe.x + bRectAbe.w) / 2),
                    FP_FromInteger((bRectAbe.y + bRectAbe.h) / 2),
                    ringTypeToGive,
                    sActiveHero->field_CC_sprite_scale);

                // Must set abe as the target to "lock on" to abe
                if (pRing)
                {
                    pRing->VSetTarget(sActiveHero);
                }

                field_194_timer = sGnFrame + 30;
                return Brain_0_GiveRings::eBrain0_ReceivingRing1_9;
            }
            break;

        case Brain_0_GiveRings::eBrain0_ReceivingRing1_9:
            if (static_cast<s32>(sGnFrame) > field_194_timer)
            {
                if (field_164_ring_pulse_interval > 0)
                {
                    sActiveHero->field_168_ring_pulse_timer = sGnFrame + field_164_ring_pulse_interval;
                }
                else
                {
                    sActiveHero->field_168_ring_pulse_timer = sGnFrame + 200000;
                }

                sActiveHero->field_16C_bHaveShrykull = FALSE;

                if (field_168_ring_type == RingTypes::eHealing_Emit_Effect_11)
                {
                    sActiveHero->field_1AC_flags.Set(Abe::e1AC_eBit15_have_healing);
                }

                mNextMotion = eMudMotions::M_Idle_0_4724E0;
                return Brain_0_GiveRings::eBrain0_RecievingRing2_10;
            }
            break;

        case Brain_0_GiveRings::eBrain0_RecievingRing2_10:
            field_16C_flags.Set(Flags_16C::eBit1_Unknown);
            return 2;

        default:
            break;
    }

    return field_190_brain_sub_state;
}

const s16 kDelayTable_55CF7C[6] = {
    0,
    6,
    12,
    18,
    24,
    30,
};
ALIVE_VAR(1, 0x5C3014, s16, sDelayIdx_5C3014, 0);

s16 Mudokon::Brain_1_Chisel_47C5F0()
{
    if (CheckForPortal_4775E0())
    {
        AddAlerted();
        field_18E_brain_state = Mud_Brain_State::Brain_6_Escape_47A560;
        return Brain_1_Chisle::eBrain1_StartToChisle_0;
    }

    if (field_16C_flags.Get(Flags_16C::eBit2_Unknown))
    {
        if (field_190_brain_sub_state != Brain_1_Chisle::eBrain1_StandUp_3)
        {
            sIsMudStandingUp_5C3018 = 0;
            field_16C_flags.Clear(Flags_16C::eBit2_Unknown);
        }
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
    if (lastSpeak == GameSpeakEvents::Slig_LookOut_6 || Event_Get(kEventShooting))
    {
        if (field_190_brain_sub_state != Brain_1_Chisle::eBrain1_StandUp_3 && field_190_brain_sub_state != Brain_1_Chisle::eBrain1_Duck_5 && field_190_brain_sub_state != Brain_1_Chisle::eBrain1_DuckKnockback_8 && field_190_brain_sub_state != Brain_1_Chisle::eBrain1_OutOfDuck_6)
        {
            field_190_brain_sub_state = Brain_1_Chisle::eBrain1_Duck_5;
        }
    }

    IBaseAnimatedWithPhysicsGameObject* pAbuseEvent = Event_Is_Event_In_Range(kEventMudokonAbuse, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, field_D6_scale);
    IBaseAnimatedWithPhysicsGameObject* pDeadMudEvent = Event_Is_Event_In_Range(kEventMudokonDied, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, field_D6_scale);
    IBaseAnimatedWithPhysicsGameObject* pLoudNoiseEvent = Event_Is_Event_In_Range(kEventLoudNoise, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, field_D6_scale);

    const bool reactToAbused = (pAbuseEvent && pAbuseEvent != this && field_190_brain_sub_state != Brain_1_Chisle::eBrain1_StandUp_3 && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0));

    const bool reactToDeadMud = (pDeadMudEvent && pDeadMudEvent != this && field_190_brain_sub_state != Brain_1_Chisle::eBrain1_StandUp_3 && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0));

    const bool reactToLoudNoise = (pLoudNoiseEvent && pLoudNoiseEvent->GetBaseGameObject().Type() == ReliveTypes::eGlukkon && field_190_brain_sub_state != Brain_1_Chisle::eBrain1_StandUp_3 && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0));

    if (reactToAbused)
    {
        field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eMudAbuse_9);
    }

    if (reactToDeadMud)
    {
        field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eMudDied_14);
    }

    if (reactToLoudNoise)
    {
        if (field_16A_flags.Get(Flags_16A::eBit13))
        {
            field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_16);
        }
        else
        {
            field_16A_flags.Set(Flags_16A::eBit13);
            field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_15);
        }
    }

    if (reactToAbused || reactToDeadMud || reactToLoudNoise)
    {
        if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags_16A::eBit11_get_depressed))
        {
            field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
        }

        if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
        {
            AddAlerted();

            if (field_180_emo_tbl == Mud_Emotion::eSad_3)
            {
                field_16A_flags.Set(Flags_16A::eBit6_standing_for_sad_or_angry);
            }

            field_190_brain_sub_state = Brain_1_Chisle::eBrain1_StandUp_3;
            field_194_timer = StableDelay_477570() + sGnFrame + 10;
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (LaughingGasInCurrentScreen_4774A0())
        {
            field_180_emo_tbl = Mud_Emotion::eWired_6;

            AddAlerted();

            field_190_brain_sub_state = Brain_1_Chisle::eBrain1_StandUp_3;
            field_194_timer = StableDelay_477570() + sGnFrame + 10;
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eAngry_1 && !field_128_angry_timer)
    {
        if (SwitchStates_Get(field_120_angry_switch_id))
        {
            field_128_angry_timer = sGnFrame + 20;
        }
    }

    if (!field_16A_flags.Get(Flags_16A::eBit8_do_angry))
    {
        if (field_128_angry_timer > 0)
        {
            if (field_128_angry_timer <= static_cast<s32>(sGnFrame))
            {
                field_16A_flags.Set(Flags_16A::eBit8_do_angry);
                field_180_emo_tbl = Mud_Emotion::eAngry_1;
                field_190_brain_sub_state = Brain_1_Chisle::eBrain1_StandUp_3;
                field_194_timer = sGnFrame;
            }
        }
    }

    switch (field_190_brain_sub_state)
    {
        case Brain_1_Chisle::eBrain1_StartToChisle_0:
            if (mCurrentMotion != eMudMotions::M_StandToCrouch_17_474120)
            {
                mCurrentMotion = eMudMotions::M_Chisel_11_4732D0;
                mNextMotion = -1;
            }
            field_194_timer = (Math_NextRandom() % 64) + (sGnFrame + 35);
            return Brain_1_Chisle::eBrain1_ChisleTheFloor_1;

        case Brain_1_Chisle::eBrain1_ChisleTheFloor_1:
        {
            bool ignoreHellOrAllYa = false;
            if (lastSpeak == GameSpeakEvents::eHello_9)
            {
                if (sIsMudStandingUp_5C3018 || !CanRespond_4770B0())
                {
                    ignoreHellOrAllYa = true;
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAllYa_23)
            {
                ignoreHellOrAllYa = true;
            }

            if (!ignoreHellOrAllYa && sActiveHero->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                mNextMotion = eMudMotions::M_Idle_0_4724E0;
                field_194_timer = StableDelay_477570() + sGnFrame + 10;
                sIsMudStandingUp_5C3018 = 1;
                field_16C_flags.Set(Flags_16C::eBit2_Unknown);

                AddAlerted();
                return Brain_1_Chisle::eBrain1_StandUp_3;
            }

            if (mCurrentMotion == eMudMotions::M_Idle_0_4724E0)
            {
                mNextMotion = eMudMotions::M_Chisel_11_4732D0;
            }

            if (static_cast<s32>(sGnFrame) > field_194_timer && mCurrentMotion == eMudMotions::M_Chisel_11_4732D0)
            {
                mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
            }

            if (mCurrentMotion != eMudMotions::M_CrouchIdle_15_474040)
            {
                return field_190_brain_sub_state;
            }

            if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && (Math_NextRandom() < 120))
            {
                AddAlerted();

                field_16A_flags.Set(Flags_16A::eBit14_make_sad_noise);
                field_16A_flags.Set(Flags_16A::eBit6_standing_for_sad_or_angry);

                mNextMotion = eMudMotions::M_Idle_0_4724E0;
                field_194_timer = StableDelay_477570() + sGnFrame + 10;
                return Brain_1_Chisle::eBrain1_StandUp_3;
            }
            else
            {
                field_194_timer = (Math_NextRandom() % 64) + (sGnFrame + 35);
                return Brain_1_Chisle::eBrain1_SmallBreak_2;
            }
        }
        break;

        case Brain_1_Chisle::eBrain1_SmallBreak_2:
        {
            bool ignoreHellOrAllYa = false;
            if (lastSpeak == GameSpeakEvents::eHello_9)
            {
                if (sIsMudStandingUp_5C3018 || !CanRespond_4770B0())
                {
                    ignoreHellOrAllYa = true;
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAllYa_23)
            {
                ignoreHellOrAllYa = true;
            }

            if (!ignoreHellOrAllYa && sActiveHero->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                AddAlerted();

                mNextMotion = eMudMotions::M_Idle_0_4724E0;
                field_194_timer = StableDelay_477570() + sGnFrame + 10;
                sIsMudStandingUp_5C3018 = 1;
                field_16C_flags.Set(Flags_16C::eBit2_Unknown);
                return Brain_1_Chisle::eBrain1_StandUp_3;
            }

            if (mCurrentMotion == eMudMotions::M_Idle_0_4724E0)
            {
                mNextMotion = eMudMotions::M_StandToCrouch_17_474120;
            }

            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }

            field_194_timer = (Math_NextRandom() % 64) + (sGnFrame + 35);
            mNextMotion = eMudMotions::M_Chisel_11_4732D0;
        }
            return Brain_1_Chisle::eBrain1_ChisleTheFloor_1;

        case Brain_1_Chisle::eBrain1_StandUp_3:
        {
            bool skip = false;
            bool bForce = false;
            if (lastSpeak != GameSpeakEvents::eHello_9)
            {
                if (lastSpeak == GameSpeakEvents::eAllYa_23)
                {
                    bForce = true;
                }
                else
                {
                    skip = true;
                }
            }

            if (!skip && (bForce || (!sIsMudStandingUp_5C3018 && CanRespond_4770B0())))
            {
                if (sActiveHero->field_CC_sprite_scale == field_CC_sprite_scale)
                {
                    AddAlerted();
                    sIsMudStandingUp_5C3018 = 1;
                    field_16C_flags.Set(Flags_16C::eBit2_Unknown);

                    field_16A_flags.Clear(Flags_16A::eBit14_make_sad_noise);
                    field_16A_flags.Clear(Flags_16A::eBit6_standing_for_sad_or_angry);
                }
            }

            if (static_cast<s32>(sGnFrame) <= field_194_timer || mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
            {
                if (mNextMotion != -1)
                {
                    return field_190_brain_sub_state;
                }

                if (mCurrentMotion == eMudMotions::M_Chisel_11_4732D0 || mCurrentMotion == eMudMotions::M_CrouchIdle_15_474040)
                {
                    mNextMotion = eMudMotions::M_Idle_0_4724E0;
                }

                if (mCurrentMotion != eMudMotions::M_Duck_53_474A40)
                {
                    return field_190_brain_sub_state;
                }
                else
                {
                    mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
                    return field_190_brain_sub_state;
                }
            }

            if (field_180_emo_tbl == Mud_Emotion::eSad_3 && field_16A_flags.Get(Flags_16A::eBit14_make_sad_noise))
            {
                field_16A_flags.Clear(Flags_16A::eBit12_alert_enemies);
                field_16A_flags.Clear(Flags_16A::eBit14_make_sad_noise);
                mNextMotion = eMudMotions::M_Speak_6_472FA0;
                Sound_475EC0(MudSounds::eSadUgh_28);
                return Brain_1_Chisle::eBrain1_SadNoise_4;
            }
            else
            {
                sIsMudStandingUp_5C3018 = 0;
                field_16C_flags.Clear(Flags_16C::eBit2_Unknown);
                if (field_180_emo_tbl != Mud_Emotion::eAngry_1 || !field_16A_flags.Get(Flags_16A::eBit8_do_angry))
                {
                    field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe_477B40;
                    return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
                }
                else
                {
                    field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
                    field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe_477B40;
                    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
                    return Brain_4_ListeningToAbe::eBrain4_Knockback_14;
                }
            }
        }
        break;

        case Brain_1_Chisle::eBrain1_SadNoise_4:
            if (mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
            {
                return field_190_brain_sub_state;
            }
            field_16A_flags.Clear(Flags_16A::eBit6_standing_for_sad_or_angry);
            mNextMotion = eMudMotions::M_StandToCrouch_17_474120;
            return Brain_1_Chisle::eBrain1_StartToChisle_0;

        case Brain_1_Chisle::eBrain1_Duck_5:
            if (mCurrentMotion == eMudMotions::M_Idle_0_4724E0)
            {
                mNextMotion = eMudMotions::M_StandToCrouch_17_474120;
                return field_190_brain_sub_state;
            }

            if (mCurrentMotion == eMudMotions::M_Chisel_11_4732D0)
            {
                mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
                return field_190_brain_sub_state;
            }

            if (mCurrentMotion != eMudMotions::M_CrouchIdle_15_474040)
            {
                return field_190_brain_sub_state;
            }

            mNextMotion = eMudMotions::M_Duck_53_474A40;
            field_194_timer = sGnFrame + 60;
            return Brain_1_Chisle::eBrain1_OutOfDuck_6;

        case Brain_1_Chisle::eBrain1_OutOfDuck_6:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
            return Brain_1_Chisle::eBrain1_DuckToChisle_7;

        case Brain_1_Chisle::eBrain1_DuckToChisle_7:
            if (mCurrentMotion == eMudMotions::M_Chisel_11_4732D0)
            {
                return Brain_1_Chisle::eBrain1_StartToChisle_0;
            }
            if (mCurrentMotion != eMudMotions::M_CrouchIdle_15_474040)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::M_Chisel_11_4732D0;
            return field_190_brain_sub_state;

        case Brain_1_Chisle::eBrain1_DuckKnockback_8:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::M_DuckKnockback_55_474250;
            field_194_timer = sGnFrame + 60;
            return Brain_1_Chisle::eBrain1_OutOfDuck_6;

        default:
            return field_190_brain_sub_state;
    }
}

s16 Mudokon::Brain_2_Scrub_47D270()
{
    if (CheckForPortal_4775E0())
    {
        AddAlerted();
        field_18E_brain_state = Mud_Brain_State::Brain_6_Escape_47A560;
        return Brain_6_Escape::eBrain6_PortalOppened_0;
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
    if (field_16C_flags.Get(Flags_16C::eBit2_Unknown))
    {
        if (field_190_brain_sub_state != Brain_2_Scrub::eBrain2_StandUp_3)
        {
            sIsMudStandingUp_5C3018 = 0;
            field_16C_flags.Clear(Flags_16C::eBit2_Unknown);
        }
    }

    if (lastSpeak == GameSpeakEvents::Slig_LookOut_6 || Event_Get(kEventShooting))
    {
        mNextMotion = eMudMotions::M_Duck_53_474A40;
        field_194_timer = sGnFrame + 60;
        return Brain_2_Scrub::eBrain2_Duck_6;
    }

    IBaseAnimatedWithPhysicsGameObject* pAbuse = Event_Is_Event_In_Range(
        kEventMudokonAbuse,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        field_D6_scale);
    if (pAbuse)
    {
        if (pAbuse != this)
        {
            if (field_190_brain_sub_state != Brain_2_Scrub::eBrain2_StandUp_3
                && field_190_brain_sub_state != Brain_2_Scrub::eBrain2_DuckKnockback_7
                && gMap.Is_Point_In_Current_Camera_4810D0(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    0))
            {
                field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eMudAbuse_9);
                if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags_16A::eBit11_get_depressed))
                {
                    field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
                }

                if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    AddAlerted();

                    if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                    {
                        field_16A_flags.Set(Flags_16A::eBit6_standing_for_sad_or_angry);
                    }

                    field_190_brain_sub_state = Brain_2_Scrub::eBrain2_StandUp_3;
                    field_194_timer = StableDelay_477570() + sGnFrame + 10;
                }
            }
        }
    }

    IBaseAnimatedWithPhysicsGameObject* pDied = Event_Is_Event_In_Range(
        kEventMudokonDied,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        field_D6_scale);
    if (pDied)
    {
        if (pDied != this)
        {
            if (field_190_brain_sub_state != Brain_2_Scrub::eBrain2_StandUp_3
                && field_190_brain_sub_state != Brain_2_Scrub::eBrain2_DuckKnockback_7
                && gMap.Is_Point_In_Current_Camera_4810D0(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    0))
            {
                field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eMudDied_14);
                if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags_16A::eBit11_get_depressed))
                {
                    field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
                }

                if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    AddAlerted();

                    if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                    {
                        field_16A_flags.Set(Flags_16A::eBit6_standing_for_sad_or_angry);
                    }

                    field_190_brain_sub_state = Brain_2_Scrub::eBrain2_StandUp_3;
                    field_194_timer = StableDelay_477570() + sGnFrame + 10;
                }
            }
        }
    }

    IBaseAnimatedWithPhysicsGameObject* pLoudNoise = Event_Is_Event_In_Range(
        kEventLoudNoise,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        field_D6_scale);
    if (pLoudNoise)
    {
        if (pLoudNoise->GetBaseGameObject().Type() == ReliveTypes::eGlukkon)
        {
            if (field_190_brain_sub_state != Brain_2_Scrub::eBrain2_StandUp_3
                && field_190_brain_sub_state != Brain_2_Scrub::eBrain2_DuckKnockback_7
                && gMap.Is_Point_In_Current_Camera_4810D0(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    0))
            {
                if (field_16A_flags.Get(Flags_16A::eBit13))
                {
                    field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_16);
                }
                else
                {
                    field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_15);
                    field_16A_flags.Set(Flags_16A::eBit13);
                }

                if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags_16A::eBit11_get_depressed))
                {
                    field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
                }

                if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    AddAlerted();

                    if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                    {
                        field_16A_flags.Set(Flags_16A::eBit6_standing_for_sad_or_angry);
                    }

                    field_190_brain_sub_state = Brain_2_Scrub::eBrain2_StandUp_3;
                    field_194_timer = StableDelay_477570() + sGnFrame + 10;
                }
            }
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (LaughingGasInCurrentScreen_4774A0())
        {
            field_180_emo_tbl = Mud_Emotion::eWired_6;

            AddAlerted();
            field_190_brain_sub_state = Brain_2_Scrub::eBrain2_StandUp_3;
            field_194_timer = StableDelay_477570() + sGnFrame + 10;
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eAngry_1 && !field_128_angry_timer)
    {
        if (SwitchStates_Get(field_120_angry_switch_id))
        {
            field_128_angry_timer = sGnFrame + 20;
        }
    }

    if (!field_16A_flags.Get(Flags_16A::eBit8_do_angry))
    {
        if (field_128_angry_timer)
        {
            if (field_128_angry_timer <= (s32) sGnFrame)
            {
                field_16A_flags.Set(Flags_16A::eBit8_do_angry);
                field_180_emo_tbl = Mud_Emotion::eAngry_1;
                field_190_brain_sub_state = Brain_2_Scrub::eBrain2_StandUp_3;
                field_194_timer = sGnFrame;
            }
        }
    }

    switch (field_190_brain_sub_state)
    {
        case Brain_2_Scrub::eBrain2_StartToScrub_0:
            mCurrentMotion = eMudMotions::M_CrouchScrub_14_473560;
            mNextMotion = -1;
            field_194_timer = Math_NextRandom() % 64 + sGnFrame + 15;
            field_124 = Math_NextRandom() % 64 + sGnFrame + 240;
            return Brain_2_Scrub::eBrain2_Scrubbing_2;

        case Brain_2_Scrub::eBrain2_SmallBreak_1:
        {
            bool checkAlerted = true;
            if (lastSpeak == GameSpeakEvents::eHello_9)
            {
                if (sIsMudStandingUp_5C3018 || !CanRespond_4770B0())
                {
                    checkAlerted = false;
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAllYa_23)
            {
                checkAlerted = false;
            }

            if (checkAlerted)
            {
                if (sActiveHero->field_CC_sprite_scale == field_CC_sprite_scale)
                {
                    AddAlerted();
                    sIsMudStandingUp_5C3018 = 1;
                    field_16C_flags.Set(Flags_16C::eBit2_Unknown);
                    field_194_timer = StableDelay_477570() + sGnFrame + 10;
                    return Brain_2_Scrub::eBrain2_StandUp_3;
                }
            }

            if (mCurrentMotion == eMudMotions::M_Idle_0_4724E0)
            {
                mNextMotion = eMudMotions::M_CrouchScrub_14_473560;
            }

            if (mCurrentMotion == eMudMotions::M_CrouchIdle_15_474040 && static_cast<s32>(sGnFrame) > field_124)
            {
                field_124 = Math_NextRandom() % 64 + sGnFrame + 240;
                mNextMotion = eMudMotions::M_CrouchTurn_16_4740E0;
            }

            if (mCurrentMotion != eMudMotions::M_CrouchIdle_15_474040 || static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                if (mCurrentMotion != eMudMotions::M_CrouchScrub_14_473560)
                {
                    return field_190_brain_sub_state;
                }
                return Brain_2_Scrub::eBrain2_Scrubbing_2;
            }

            if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && Math_NextRandom() < 120)
            {
                AddAlerted();
                field_16A_flags.Set(Flags_16A::eBit14_make_sad_noise);
                field_16A_flags.Set(Flags_16A::eBit6_standing_for_sad_or_angry);
                field_194_timer = StableDelay_477570() + sGnFrame + 10;
                return Brain_2_Scrub::eBrain2_StandUp_3;
            }
            else
            {
                field_194_timer = Math_NextRandom() % 64 + sGnFrame + 35;
                mNextMotion = eMudMotions::M_CrouchScrub_14_473560;
                if (mCurrentMotion != eMudMotions::M_CrouchScrub_14_473560)
                {
                    return field_190_brain_sub_state;
                }
                return Brain_2_Scrub::eBrain2_Scrubbing_2;
            }
            break;
        }

        case Brain_2_Scrub::eBrain2_Scrubbing_2:
        {
            bool checkAlerted = true;
            if (lastSpeak == GameSpeakEvents::eHello_9)
            {
                if (sIsMudStandingUp_5C3018 || !CanRespond_4770B0())
                {
                    checkAlerted = false;
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAllYa_23)
            {
                checkAlerted = false;
            }

            if (checkAlerted)
            {
                if (sActiveHero->field_CC_sprite_scale == field_CC_sprite_scale)
                {
                    AddAlerted();
                    sIsMudStandingUp_5C3018 = 1;
                    field_16C_flags.Set(Flags_16C::eBit2_Unknown);
                    field_194_timer = StableDelay_477570() + sGnFrame + 10;
                    return Brain_2_Scrub::eBrain2_StandUp_3;
                }
            }

            if (mCurrentMotion == eMudMotions::M_Idle_0_4724E0)
            {
                mNextMotion = eMudMotions::M_CrouchScrub_14_473560;
            }

            bool bUnknown = false;
            if (mCurrentMotion != eMudMotions::M_CrouchIdle_15_474040)
            {
                bUnknown = true;
            }
            else
            {
                if (static_cast<s32>(sGnFrame) <= field_194_timer)
                {
                    mCurrentMotion = eMudMotions::M_CrouchScrub_14_473560;
                    bUnknown = true;
                }
            }

            if (bUnknown)
            {
                if (!Event_Is_Event_In_Range(kEventSpeaking, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, field_D6_scale))
                {
                    return field_190_brain_sub_state;
                }
                field_194_timer = Math_NextRandom() % 64 + sGnFrame + 15;
                return Brain_2_Scrub::eBrain2_SmallBreakThroughGameSpeak_5;
            }
            else
            {
                field_194_timer = Math_NextRandom() % 64 + sGnFrame + 15;
                return Brain_2_Scrub::eBrain2_SmallBreak_1;
            }
            break;
        }

        case Brain_2_Scrub::eBrain2_StandUp_3:
        {
            if (lastSpeak == GameSpeakEvents::eHello_9 || lastSpeak == GameSpeakEvents::eAllYa_23)
            {
                bool checkAlerted = lastSpeak == GameSpeakEvents::eHello_9;
                if (lastSpeak == GameSpeakEvents::eAllYa_23)
                {
                    if (!sIsMudStandingUp_5C3018 && CanRespond_4770B0())
                    {
                        checkAlerted = true;
                    }
                }

                if (checkAlerted)
                {
                    if (sActiveHero->field_CC_sprite_scale == field_CC_sprite_scale)
                    {
                        AddAlerted();
                        sIsMudStandingUp_5C3018 = 1;
                        field_16C_flags.Set(Flags_16C::eBit2_Unknown);
                        field_16A_flags.Clear(Flags_16A::eBit14_make_sad_noise);
                        field_16A_flags.Clear(Flags_16A::eBit6_standing_for_sad_or_angry);
                    }
                }
            }

            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }

            if (mCurrentMotion == eMudMotions::M_Duck_53_474A40)
            {
                mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
            }

            if (mCurrentMotion == eMudMotions::M_CrouchIdle_15_474040)
            {
                mNextMotion = eMudMotions::M_CrouchToStand_18_474150;
            }

            if (mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
            {
                return field_190_brain_sub_state;
            }

            if (field_180_emo_tbl == Mud_Emotion::eSad_3 && field_16A_flags.Get(Flags_16A::eBit14_make_sad_noise))
            {
                field_16A_flags.Clear(Flags_16A::eBit14_make_sad_noise);
                field_16A_flags.Clear(Flags_16A::eBit12_alert_enemies);
                mNextMotion = eMudMotions::M_Speak_6_472FA0;
                Sound_475EC0(MudSounds::eSadUgh_28);
                return Brain_2_Scrub::eBrain2_SadNoise_4;
            }
            else
            {
                sIsMudStandingUp_5C3018 = 0;
                field_16C_flags.Clear(Flags_16C::eBit2_Unknown);
                if (field_180_emo_tbl != Mud_Emotion::eAngry_1 || !field_16A_flags.Get(Flags_16A::eBit8_do_angry))
                {
                    field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe_477B40;
                    return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
                }
                else
                {
                    field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
                    field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe_477B40;
                    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
                    return Brain_4_ListeningToAbe::eBrain4_Knockback_14;
                }
            }
            break;
        }

        case Brain_2_Scrub::eBrain2_SadNoise_4:
            if (mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
            {
                return field_190_brain_sub_state;
            }
            field_16A_flags.Clear(Flags_16A::eBit6_standing_for_sad_or_angry);
            mNextMotion = eMudMotions::M_StandToCrouch_17_474120;
            return Brain_2_Scrub::eBrain2_SmallBreak_1;

        case Brain_2_Scrub::eBrain2_SmallBreakThroughGameSpeak_5:
        {
            bool checkAlerted = true;
            if (lastSpeak == GameSpeakEvents::eHello_9)
            {
                if (sIsMudStandingUp_5C3018 || !CanRespond_4770B0())
                {
                    checkAlerted = false;
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAllYa_23)
            {
                checkAlerted = false;
            }

            if (checkAlerted)
            {
                if (sActiveHero->field_CC_sprite_scale == field_CC_sprite_scale)
                {
                    AddAlerted();
                    sIsMudStandingUp_5C3018 = 1;
                    field_16C_flags.Set(Flags_16C::eBit2_Unknown);
                    field_194_timer = StableDelay_477570() + sGnFrame + 10;
                    return Brain_2_Scrub::eBrain2_StandUp_3;
                }
            }

            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }
            return Brain_2_Scrub::eBrain2_SmallBreak_1;
        }

        case Brain_2_Scrub::eBrain2_Duck_6:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
            field_194_timer = Math_NextRandom() % 64 + sGnFrame + 15;
            field_124 = Math_NextRandom() % 64 + sGnFrame + 240;
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(1);
            }
            return Brain_2_Scrub::eBrain2_Scrubbing_2;

        case Brain_2_Scrub::eBrain2_DuckKnockback_7:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::M_DuckKnockback_55_474250;
            field_194_timer = sGnFrame + 60;
            return Brain_2_Scrub::eBrain2_Duck_6;

        default:
            return field_190_brain_sub_state;
    }
}

s16 Mudokon::Brain_3_TurnWheel_47E0D0()
{
    if (CheckForPortal_4775E0())
    {
        AddAlerted();
        field_18E_brain_state = Mud_Brain_State::Brain_6_Escape_47A560;
        mNextMotion = eMudMotions::M_Idle_0_4724E0;
        return Brain_6_Escape::eBrain6_PortalOppened_0;
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
    if (field_16C_flags.Get(Flags_16C::eBit2_Unknown))
    {
        if (field_190_brain_sub_state != Brain_3_TurnWheel::eBrain3_InterruptAction_2)
        {
            sIsMudStandingUp_5C3018 = 0;
            field_16C_flags.Clear(Flags_16C::eBit2_Unknown);
        }
    }

    if (lastSpeak == GameSpeakEvents::Slig_LookOut_6 || Event_Get(kEventShooting))
    {
        if (field_190_brain_sub_state != Brain_3_TurnWheel::eBrain3_TurningWheelToDuck_3 && field_190_brain_sub_state != Brain_3_TurnWheel::eBrain3_Duck_4)
        {
            field_190_brain_sub_state = Brain_3_TurnWheel::eBrain3_TurningWheelToDuck_3;
        }
    }

    IBaseAnimatedWithPhysicsGameObject* pMudAbuseEvent = Event_Is_Event_In_Range(
        kEventMudokonAbuse,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        field_D6_scale);

    if (pMudAbuseEvent)
    {
        if (pMudAbuseEvent != this
            && field_190_brain_sub_state != Brain_3_TurnWheel::eBrain3_InterruptAction_2
            && gMap.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0))
        {
            field_188_pTblEntry = Mudokon::ResponseTo_471730(field_180_emo_tbl, MudAction::eMudAbuse_9);
            if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags_16A::eBit11_get_depressed))
            {
                field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
            }

            if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
            {
                AddAlerted();

                if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    field_16A_flags.Set(Flags_16A::eBit6_standing_for_sad_or_angry);
                }

                mNextMotion = eMudMotions::M_Idle_0_4724E0;
                field_190_brain_sub_state = Brain_3_TurnWheel::eBrain3_InterruptAction_2;
                field_194_timer = StableDelay_477570() + sGnFrame + 10;
            }
        }
    }

    IBaseAnimatedWithPhysicsGameObject* pLoudNoiseEvent = Event_Is_Event_In_Range(
        kEventLoudNoise,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        field_D6_scale);
    if (pLoudNoiseEvent)
    {
        if (pLoudNoiseEvent->GetBaseGameObject().Type() == ReliveTypes::eGlukkon
            && field_190_brain_sub_state != Brain_3_TurnWheel::eBrain3_InterruptAction_2
            && gMap.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0))
        {
            if (field_16A_flags.Get(Flags_16A::eBit13))
            {
                field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_16);
            }
            else
            {
                field_16A_flags.Set(Flags_16A::eBit13);
                field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_15);
            }

            if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags_16A::eBit11_get_depressed))
            {
                field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
            }

            if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
            {
                AddAlerted();

                if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    field_16A_flags.Set(Flags_16A::eBit6_standing_for_sad_or_angry);
                }

                mNextMotion = eMudMotions::M_Idle_0_4724E0;
                field_190_brain_sub_state = Brain_3_TurnWheel::eBrain3_InterruptAction_2;
                field_194_timer = StableDelay_477570() + sGnFrame + 10;
            }
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (LaughingGasInCurrentScreen_4774A0())
        {
            field_180_emo_tbl = Mud_Emotion::eWired_6;
            AddAlerted();
            mNextMotion = eMudMotions::M_Idle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame + 10;
            field_190_brain_sub_state = Brain_3_TurnWheel::eBrain3_InterruptAction_2;
        }
    }

    switch (field_190_brain_sub_state)
    {
        case Brain_3_TurnWheel::eBrain3_TurningWheel_0:
            if (lastSpeak == GameSpeakEvents::eHello_9)
            {
                if (sIsMudStandingUp_5C3018 || !CanRespond_4770B0())
                {
                    return TurningWheelHelloOrAllYaResponse();
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAllYa_23)
            {
                return TurningWheelHelloOrAllYaResponse();
            }

            if (sActiveHero->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                AddAlerted();
                mNextMotion = eMudMotions::M_Idle_0_4724E0;
                field_194_timer = StableDelay_477570() + sGnFrame + 10;
                sIsMudStandingUp_5C3018 = 1;
                field_16C_flags.Set(Flags_16C::eBit2_Unknown);
                return Brain_3_TurnWheel::eBrain3_InterruptAction_2;
            }

            return TurningWheelHelloOrAllYaResponse();

        case Brain_3_TurnWheel::eBrain3_SittingDown_1:
            if (field_194_timer <= static_cast<s32>(sGnFrame))
            {
                return field_190_brain_sub_state;
            }

            if (mCurrentMotion == eMudMotions::M_Idle_0_4724E0)
            {
                mNextMotion = eMudMotions::M_StandToCrouch_17_474120;
            }
            else
            {
                if (mCurrentMotion != eMudMotions::M_StandToCrouch_17_474120 && mCurrentMotion != eMudMotions::M_CrouchIdle_15_474040)
                {
                    mNextMotion = eMudMotions::M_Idle_0_4724E0;
                }

                if (mCurrentMotion == eMudMotions::M_Idle_0_4724E0)
                {
                    mNextMotion = eMudMotions::M_StandToCrouch_17_474120;
                }
            }

            if (mCurrentMotion != eMudMotions::M_CrouchIdle_15_474040)
            {
                return field_190_brain_sub_state;
            }

            if (field_10_resources_array.ItemAt(2))
            {
                field_18E_brain_state = Mud_Brain_State::Brain_1_Chisel_47C5F0;
            }
            else
            {
                field_18E_brain_state = Mud_Brain_State::Brain_2_Scrub_47D270;
            }
            return Brain_3_TurnWheel::eBrain3_TurningWheel_0;

        case Brain_3_TurnWheel::eBrain3_InterruptAction_2:
            field_16A_flags.Clear(Flags_16A::eBit7_stopped_at_wheel);
            if (static_cast<s32>(sGnFrame) <= field_194_timer || mCurrentMotion)
            {
                if (mNextMotion != -1 || mCurrentMotion != eMudMotions::M_TurnWheelLoop_58_474CC0)
                {
                    return field_190_brain_sub_state;
                }
                mNextMotion = eMudMotions::M_Idle_0_4724E0;
                return field_190_brain_sub_state;
            }
            else
            {
                field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe_477B40;
                sIsMudStandingUp_5C3018 = 0;
                field_16C_flags.Clear(Flags_16C::eBit2_Unknown);
                return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
            }
            break;

        case Brain_3_TurnWheel::eBrain3_TurningWheelToDuck_3:
            if (mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
            {
                if (mCurrentMotion != eMudMotions::M_CrouchIdle_15_474040)
                {
                    if (mCurrentMotion != eMudMotions::M_TurnWheelLoop_58_474CC0)
                    {
                        return field_190_brain_sub_state;
                    }
                    else
                    {
                        mNextMotion = eMudMotions::M_Idle_0_4724E0;
                        return field_190_brain_sub_state;
                    }
                }
                else
                {
                    mNextMotion = eMudMotions::M_Duck_53_474A40;
                    field_194_timer = sGnFrame + 60;
                    return Brain_3_TurnWheel::eBrain3_Duck_4;
                }
            }
            else
            {
                mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
                return field_190_brain_sub_state;
            }
            break;

        case Brain_3_TurnWheel::eBrain3_Duck_4:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
            return Brain_3_TurnWheel::eBrain3_DuckToTurningWheel_5;

        case Brain_3_TurnWheel::eBrain3_DuckToTurningWheel_5:
            if (mCurrentMotion == eMudMotions::M_Idle_0_4724E0)
            {
                mNextMotion = eMudMotions::M_TurnWheelBegin_57_474C00;
                return field_190_brain_sub_state;
            }

            if (mCurrentMotion == eMudMotions::M_CrouchIdle_15_474040)
            {
                mNextMotion = eMudMotions::M_Idle_0_4724E0;
                return field_190_brain_sub_state;
            }
            else
            {
                if (mCurrentMotion != eMudMotions::M_TurnWheelLoop_58_474CC0)
                {
                    return field_190_brain_sub_state;
                }
                return Brain_3_TurnWheel::eBrain3_TurningWheel_0;
            }
            break;
    }
    return field_190_brain_sub_state;
}

s16 Mudokon::Brain_4_ListeningToAbe_477B40()
{
    if (CheckForPortal_4775E0())
    {
        field_18E_brain_state = Mud_Brain_State::Brain_6_Escape_47A560;
        mNextMotion = eMudMotions::M_Idle_0_4724E0;
        return Brain_6_Escape::eBrain6_PortalOppened_0;
    }

    if (Event_Is_Event_In_Range(
            kEventMudokonAbuse,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eMudAbuse_9;
    }

    if (Event_Is_Event_In_Range(
            kEventMudokonDied,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eMudDied_14;
    }

    if (Event_Is_Event_In_Range(
            kEventMudokonComfort,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eComfort_10;
    }

    if (Event_Is_Event_In_Range(
            kEventMudokonLaugh,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eLaugh_12;
    }

    if (Event_Is_Event_In_Range(kEventAbeDead,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eDuck_13;
    }

    IBaseAnimatedWithPhysicsGameObject* pNoiseEvent = Event_Is_Event_In_Range(
        kEventLoudNoise,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        field_D6_scale);

    if (pNoiseEvent)
    {
        if (pNoiseEvent->GetBaseGameObject().Type() == ReliveTypes::eGlukkon)
        {
            if (field_16A_flags.Get(Flags_16A::eBit13))
            {
                field_17E_delayed_speak = MudAction::eUnknown_16;
            }
            else
            {
                field_16A_flags.Set(Flags_16A::eBit13);
                field_17E_delayed_speak = MudAction::eUnknown_15;
            }
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (LaughingGasInCurrentScreen_4774A0())
        {
            field_180_emo_tbl = Mud_Emotion::eWired_6;
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eAngry_1 && !field_128_angry_timer)
    {
        if (SwitchStates_Get(field_120_angry_switch_id))
        {
            field_128_angry_timer = sGnFrame + 15;
        }
    }

    if (!field_16A_flags.Get(Flags_16A::eBit8_do_angry))
    {
        if (field_128_angry_timer > 0)
        {
            if (field_128_angry_timer <= static_cast<s32>(sGnFrame))
            {
                field_16A_flags.Set(Flags_16A::eBit8_do_angry);

                if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)),
                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)),
                        TlvTypes::Water_77))
                {
                    field_180_emo_tbl = Mud_Emotion::eAngry_1;
                    field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
                    field_190_brain_sub_state = Brain_4_ListeningToAbe::eBrain4_Knockback_14;
                    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
                }
            }
        }
    }

    switch (field_190_brain_sub_state)
    {
        case Brain_4_ListeningToAbe::eBrain4_Inactive_0:
            return Brain_ListeningToAbe_State_0();

        case Brain_4_ListeningToAbe::eBrain4_GetsAttentive_1:
            return Brain_ListeningToAbe_State_1();

        case Brain_4_ListeningToAbe::eBrain4_CrazyFollowMe_2:
            return Brain_ListeningToAbe_State_2();

        case Brain_4_ListeningToAbe::eBrain4_SteppingBack_3:
            return Brain_ListeningToAbe_State_3();

        case Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4:
            return Brain_ListeningToAbe_State_4();

        case Brain_4_ListeningToAbe::eBrain4_Walking_5:
            return Brain_ListeningToAbe_State_5();

        case Brain_4_ListeningToAbe::eBrain4_Running_6:
            return Brain_ListeningToAbe_State_6();

        case Brain_4_ListeningToAbe::eBrain4_Idle_7:
            return Brain_ListeningToAbe_State_7();

        case Brain_4_ListeningToAbe::eBrain4_CrazySlideTurn_8:
            return Brain_ListeningToAbe_State_8();

        case Brain_4_ListeningToAbe::eBrain4_GetsWaitWhileMoving_9:
            return Brain_ListeningToAbe_State_9();

        case Brain_4_ListeningToAbe::eBrain4_InitializeMovement_10:
            return Brain_ListeningToAbe_State_10();

        case Brain_4_ListeningToAbe::eBrain4_PullingLever_11:
            return Brain_ListeningToAbe_State_11();

        case Brain_4_ListeningToAbe::eBrain4_GetsCommand_12:
            return Brain_ListeningToAbe_State_12();

        case Brain_4_ListeningToAbe::eBrain4_Slapped_13:
            return Brain_ListeningToAbe_State_13();

        case Brain_4_ListeningToAbe::eBrain4_Knockback_14:
            return Brain_ListeningToAbe_State_14();

        case Brain_4_ListeningToAbe::eBrain4_GettingSorry_15:
            return Brain_ListeningToAbe_State_15();

        case Brain_4_ListeningToAbe::eBrain4_Suiciding_16:
            return Brain_ListeningToAbe_State_16();

        case Brain_4_ListeningToAbe::eBrain4_RageSlap_17:
            return Brain_ListeningToAbe_State_17();

        case Brain_4_ListeningToAbe::eBrain4_RageTurn_18:
            return Brain_ListeningToAbe_State_18();

        case Brain_4_ListeningToAbe::eBrain4_CrazyDeny_19:
            return Brain_ListeningToAbe_State_19();

        case Brain_4_ListeningToAbe::eBrain4_StopsWhileMoving_20:
            return Brain_ListeningToAbe_State_20();

        case Brain_4_ListeningToAbe::eBrain4_CrazyLaugh_21:
            return Brain_ListeningToAbe_State_21();

        case Brain_4_ListeningToAbe::eBrain4_LostAttention_22:
            return Brain_ListeningToAbe_State_22();

        default:
            return field_190_brain_sub_state;
    }
}

s16 Mudokon::Brain_ListeningToAbe_State_0()
{
    mNextMotion = -1;
    ToStand_4724A0();
    field_178_brain_sub_state2 = -1;
    return Brain_4_ListeningToAbe::eBrain4_GetsAttentive_1;
}

s16 Mudokon::Brain_ListeningToAbe_State_1()
{
    if (mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
    {
        return field_190_brain_sub_state;
    }

    if (!VIsFacingMe(sActiveHero))
    {
        mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
        return field_190_brain_sub_state;
    }

    switch (field_180_emo_tbl)
    {
        case Mud_Emotion::eNormal_0:
            Sound_475EC0(MudSounds::eHelloNeutral_3);
            break;

        case Mud_Emotion::eAngry_1:
        case Mud_Emotion::eAggressive_2:
            if (field_16A_flags.Get(Flags_16A::eBit6_standing_for_sad_or_angry))
            {
                Sound_475EC0(MudSounds::eAnger_5);
            }
            else
            {
                Sound_475EC0(MudSounds::eHiAngry_18);
            }
            break;

        case Mud_Emotion::eSad_3:
        case Mud_Emotion::eSick_7:
            if (field_16A_flags.Get(Flags_16A::eBit6_standing_for_sad_or_angry))
            {
                Sound_475EC0(MudSounds::eNoSad_22);
            }
            else
            {
                Sound_475EC0(MudSounds::eHiSad_20);
            }
            break;

        case Mud_Emotion::eSuicidal_4:
            Sound_475EC0(MudSounds::eNoSad_22);
            break;

        case Mud_Emotion::eHappy_5:
        case Mud_Emotion::eWired_6:
            Sound_475EC0(MudSounds::eLaugh_10);
            break;

        default:
            break;
    }

    mNextMotion = eMudMotions::M_Speak_4_472FA0;
    if (!field_16A_flags.Get(Flags_16A::eBit6_standing_for_sad_or_angry) || field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        return Brain_4_ListeningToAbe::eBrain4_Idle_7;
    }

    field_16A_flags.Clear(Flags_16A::eBit6_standing_for_sad_or_angry);
    field_16A_flags.Clear(Flags_16A::eBit12_alert_enemies);

    if (field_10_resources_array.ItemAt(2))
    {
        field_184_next_motion2 = eMudMotions::M_Chisel_11_4732D0;
    }
    else
    {
        field_184_next_motion2 = eMudMotions::M_CrouchIdle_15_474040;
    }
    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
    field_16A_flags.Clear(Flags_16A::eBit5_following);
    return Brain_4_ListeningToAbe::eBrain4_InitializeMovement_10;
}

s16 Mudokon::Brain_ListeningToAbe_State_2()
{
    if (!VIsFacingMe(sActiveHero))
    {
        mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
        return field_190_brain_sub_state;
    }

    if (mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
    {
        if (mCurrentMotion == eMudMotions::M_WalkLoop_1_4728B0)
        {
            if (field_16A_flags.Get(Flags_16A::eBit4_blind) || !VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale), sActiveHero))
            {
                return Brain_4_ListeningToAbe::eBrain4_Walking_5;
            }
        }
        else
        {
            if (mCurrentMotion != eMudMotions::M_Knockback_46_4742A0)
            {
                return field_190_brain_sub_state;
            }
        }
        mNextMotion = eMudMotions::M_Idle_0_4724E0;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }
    else
    {
        mNextMotion = eMudMotions::M_WalkLoop_1_4728B0;
    }
    return field_190_brain_sub_state;
}

s16 Mudokon::Brain_ListeningToAbe_State_3()
{
    if (mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
    {
        if (mCurrentMotion != eMudMotions::M_WalkLoop_1_4728B0)
        {
            return field_190_brain_sub_state;
        }
        mNextMotion = eMudMotions::M_Idle_0_4724E0;

        if (field_16A_flags.Get(Flags_16A::eBit5_following))
        {
            //TODO possibly wrong enum value
            LOG_WARNING("bit5 to state 4 wrong?");
            return Brain_4_ListeningToAbe::eBrain4_Walking_5;
        }
        else
        {
            LOG_WARNING("not bit5 to state 7 wrong?");
            return Brain_4_ListeningToAbe::eBrain4_Idle_7;
        }
    }
    else
    {
        if (VIsFacingMe(sActiveHero))
        {
            mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
            return field_190_brain_sub_state;
        }
        mNextMotion = eMudMotions::M_WalkLoop_1_4728B0;
        return field_190_brain_sub_state;
    }
}

s16 Mudokon::Brain_ListeningToAbe_State_4()
{
    if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && field_180_emo_tbl != Mud_Emotion::eHappy_5 && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        field_16A_flags.Clear(Flags_16A::eBit5_following);
        return Brain_4_ListeningToAbe::eBrain4_Idle_7;
    }
    else
    {
        if (mCurrentMotion == eMudMotions::M_RunLoop_21_473720)
        {
            mNextMotion = eMudMotions::M_Idle_0_4724E0;
        }
        if (mBaseAnimatedWithPhysicsGameObject_YPos > pScreenManager->field_20_pCamPos->field_4_y + FP_FromInteger(260) && field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
            if (field_10_resources_array.ItemAt(2))
            {
                field_16A_flags.Clear(Flags_16A::eBit5_following);
                mNextMotion = eMudMotions::M_Chisel_11_4732D0;
                return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
            }
            else
            {
                field_16A_flags.Clear(Flags_16A::eBit5_following);
                mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
                return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
            }
        }

        FP scaleToGridSizeAbs = {};
        if (sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos >= mBaseAnimatedWithPhysicsGameObject_XPos)
        {
            scaleToGridSizeAbs = ScaleToGridSize(field_CC_sprite_scale);
        }
        else
        {
            scaleToGridSizeAbs = -ScaleToGridSize(field_CC_sprite_scale);
        }
        const s32 v44 = WallHit(field_CC_sprite_scale * FP_FromInteger(50), scaleToGridSizeAbs);

        if (mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
        {
            field_17C_stand_idle_timer = 0;
            field_124 = sGnFrame + Math_RandomRange(30, 60);
            return field_190_brain_sub_state;
        }
        else
        {
            field_16A_flags.Clear(Flags_16A::eBit7_stopped_at_wheel);
            if (++field_17C_stand_idle_timer <= 540 || field_180_emo_tbl == Mud_Emotion::eWired_6)
            {
                if (NeedToTurnAround())
                {
                    mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
                    return field_190_brain_sub_state;
                }

                const FP v48 = ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(2);
                if ((VIsObjNearby(v48, sActiveHero) && !(field_16A_flags.Get(Flags_16A::eBit4_blind))) || v44 || FindWheel_4777B0(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos))
                {
                    const GameSpeakEvents lastSpeak_1 = LastGameSpeak_476FF0();
                    if (field_17E_delayed_speak != MudAction::eNone_17)
                    {
                        s16 v18 = GetBrainSubStateResponse_477AF0(field_17E_delayed_speak);
                        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
                        {
                            field_194_timer = sGnFrame + 20;
                        }
                        else
                        {
                            field_194_timer = (u16) StableDelay_477570() + sGnFrame + 20;
                        }
                        field_178_brain_sub_state2 = field_190_brain_sub_state;
                        field_17E_delayed_speak = MudAction::eNone_17;
                        if (!v18)
                        {
                            return field_190_brain_sub_state;
                        }
                        return v18;
                    }

                    if (lastSpeak_1 == GameSpeakEvents::eNone_m1)
                    {
                        if (field_180_emo_tbl != Mud_Emotion::eWired_6 || field_124 > (s32) sGnFrame)
                        {
                            return field_190_brain_sub_state;
                        }
                        field_194_timer = sGnFrame + Math_RandomRange(22, 30);
                        field_178_brain_sub_state2 = field_190_brain_sub_state;
                        return Brain_4_ListeningToAbe::eBrain4_CrazyLaugh_21;
                    }
                    else
                    {
                        field_178_brain_sub_state2 = field_190_brain_sub_state;
                        field_194_timer = (u16) StableDelay_477570() + sGnFrame + 20;
                        switch (lastSpeak_1)
                        {
                            case GameSpeakEvents::eFart_3:
                            {
                                s16 v18 = GetBrainSubStateResponse_477AF0(MudAction::eFart_6);
                                if (VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale), sActiveHero))
                                {
                                    if (VOnSameYLevel(sActiveHero))
                                    {
                                        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_SteppingBack_3;
                                    }
                                }
                                if (!v18)
                                {
                                    return field_190_brain_sub_state;
                                }
                                return v18;
                            }

                            case GameSpeakEvents::eHello_9:
                            case GameSpeakEvents::eAllYa_23:
                            {
                                s16 result = GetBrainSubStateResponse_477AF0(MudAction::eHelloOrAllYa_0);
                                if (result)
                                {
                                    return result;
                                }
                                return field_190_brain_sub_state;
                            }

                            case GameSpeakEvents::eFollowMe_10:
                            {
                                if (v44)
                                {
                                    return Brain_4_ListeningToAbe::eBrain4_CrazyDeny_19;
                                }
                                s16 result = GetBrainSubStateResponse_477AF0(MudAction::eFollowMe_1);
                                field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_CrazyFollowMe_2;
                                if (result)
                                {
                                    return result;
                                }
                                return field_190_brain_sub_state;
                            }

                            case GameSpeakEvents::eAnger_11:
                            case GameSpeakEvents::eSorry_24:
                                field_17C_stand_idle_timer = 0;
                                return field_190_brain_sub_state;

                            case GameSpeakEvents::eWait_12:
                            {
                                if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                                {
                                    return Brain_4_ListeningToAbe::eBrain4_CrazyDeny_19;
                                }
                                field_16A_flags.Clear(Flags_16A::eBit5_following);
                                s16 result = GetBrainSubStateResponse_477AF0(MudAction::eWait_2);
                                field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
                                if (result)
                                {
                                    return result;
                                }
                                return field_190_brain_sub_state;
                            }

                            case GameSpeakEvents::eWork_21:
                                if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                                {
                                    return Brain_4_ListeningToAbe::eBrain4_CrazyDeny_19;
                                }
                                field_16A_flags.Clear(Flags_16A::eBit5_following);
                                field_182 = GameSpeakEvents::eNone_m1;
                                if (FindWheel_4777B0(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos))
                                {
                                    Mudokon* v35 = static_cast<Mudokon*>(FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)));
                                    if (!v35)
                                    {
                                        return BrainStartWheelTurning();
                                    }

                                    while (!(v35->field_16A_flags.Get(Flags_16A::eBit7_stopped_at_wheel)) && v35->mCurrentMotion != eMudMotions::M_TurnWheelLoop_58_474CC0)
                                    {
                                        v35 = static_cast<Mudokon*>(GetStackedSlapTarget(v35->field_8_object_id, ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)));
                                        if (!v35)
                                        {
                                            return BrainStartWheelTurning();
                                        }
                                    }
                                    if (v35)
                                    {
                                        if (field_10_resources_array.ItemAt(2))
                                        {
                                            mNextMotion = eMudMotions::M_Chisel_11_4732D0;
                                            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                                        }
                                        else
                                        {
                                            mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
                                            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                                        }
                                    }
                                    return BrainStartWheelTurning();
                                }

                                if (FindObjectOfType(ReliveTypes::eLever, ScaleToGridSize(field_CC_sprite_scale) + mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
                                {
                                    return Brain_4_ListeningToAbe::eBrain4_PullingLever_11;
                                }
                                if (FindObjectOfType(ReliveTypes::eLever, mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
                                {
                                    return Brain_4_ListeningToAbe::eBrain4_PullingLever_11;
                                }

                                if (field_10_resources_array.ItemAt(2))
                                {
                                    mNextMotion = eMudMotions::M_Chisel_11_4732D0;
                                    return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                                }
                                else
                                {
                                    mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
                                    return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                                }
                                break;

                            case GameSpeakEvents::eStopIt_22:
                            {
                                s16 v18 = GetBrainSubStateResponse_477AF0(MudAction::eStopIt_4);
                                field_194_timer = ((u32)(u16) StableDelay_477570() >> 1) + sGnFrame + 20;
                                if (!v18)
                                {
                                    return field_190_brain_sub_state;
                                }
                                return v18;
                            }

                            default:
                                return field_190_brain_sub_state;
                        }
                    }
                }
                else
                {
                    field_194_timer = sGnFrame + (u16) Mudokon::StableDelay_477570();
                    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                    {
                        field_184_next_motion2 = eMudMotions::M_RunLoop_21_473720;
                        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Running_6;
                        return Brain_4_ListeningToAbe::eBrain4_InitializeMovement_10;
                    }
                    else if (field_16A_flags.Get(Flags_16A::eBit4_blind) || !IsMotionUnknown_4730F0())
                    {
                        field_184_next_motion2 = eMudMotions::M_WalkLoop_1_4728B0;
                        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Walking_5;
                        return Brain_4_ListeningToAbe::eBrain4_InitializeMovement_10;
                    }
                    else
                    {
                        field_184_next_motion2 = eMudMotions::M_SneakLoop_27_473C40;
                        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Walking_5;
                        return Brain_4_ListeningToAbe::eBrain4_InitializeMovement_10;
                    }
                }
            }
            else
            {
                field_16A_flags.Clear(Flags_16A::eBit5_following);
                if (!field_10_resources_array.ItemAt(2))
                {
                    mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
                    return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                }
                else
                {
                    mNextMotion = eMudMotions::M_Chisel_11_4732D0;
                    return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                }
            }
        }
    }
}

s16 Mudokon::Brain_ListeningToAbe_State_5()
{
    if (mCurrentMotion == eMudMotions::M_Idle_0_4724E0 && mNextMotion != eMudMotions::M_WalkLoop_1_4728B0)
    {
        if (!field_16A_flags.Get(Flags_16A::eBit4_blind))
        {
            return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
        }
        mNextMotion = eMudMotions::M_WalkLoop_1_4728B0;
    }

    if (mCurrentMotion == eMudMotions::M_Knockback_46_4742A0)
    {
        Sound_475EC0(MudSounds::eHurt1_16);
        s16 result = GetBrainSubStateResponse_477AF0(MudAction::eSmackIntoWall_11);
        field_17E_delayed_speak = MudAction::eNone_17;
        field_194_timer = sGnFrame + 20;
        if (field_16A_flags.Get(Flags_16A::eBit4_blind))
        {
            field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
            field_16A_flags.Clear(Flags_16A::eBit5_following);
        }
        else
        {
            field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
        }
        if (result)
        {
            return result;
        }
    }

    if (mCurrentMotion != eMudMotions::M_WalkLoop_1_4728B0 && mCurrentMotion != eMudMotions::M_SneakLoop_27_473C40)
    {
        return field_190_brain_sub_state;
    }

    FP v65 = {};
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        v65 = -ScaleToGridSize(field_CC_sprite_scale);
    }
    else
    {
        v65 = ScaleToGridSize(field_CC_sprite_scale);
    }

    const s32 v67 = WallHit(field_CC_sprite_scale * FP_FromInteger(50), v65);
    if (field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        if (!gMap.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0)
            && !VIsFacingMe(sActiveHero))
        {
            const s32 v68 = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) % 375;
            if (v68 > 10u && v68 < 362u)
            {
                field_16A_flags.Clear(Flags_16A::eBit5_following);
                mNextMotion = eMudMotions::M_Idle_0_4724E0;
                return Brain_4_ListeningToAbe::eBrain4_Idle_7;
            }
        }
    }
    if (v67 && !field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        mNextMotion = eMudMotions::M_Idle_0_4724E0;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }

    if (StopAtWheel_477880())
    {
        field_16A_flags.Set(Flags_16A::eBit7_stopped_at_wheel);
        mNextMotion = eMudMotions::M_Idle_0_4724E0;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }

    if (!VIsFacingMe(sActiveHero) && !field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        mNextMotion = eMudMotions::M_Idle_0_4724E0;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }

    if (VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(2), sActiveHero) && !field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        mNextMotion = eMudMotions::M_Idle_0_4724E0;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }

    if (LastGameSpeak_476FF0() == GameSpeakEvents::eWait_12)
    {
        field_16A_flags.Clear(Flags_16A::eBit5_following);
        mNextMotion = eMudMotions::M_Idle_0_4724E0;
        return Brain_4_ListeningToAbe::eBrain4_GetsWaitWhileMoving_9;
    }

    bool bWalkOrSneak = false;
    if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_33_RunLoop_4508E0 && sActiveHero->mCurrentMotion != eAbeMotions::Motion_23_RollLoop_453A90 && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        bWalkOrSneak = true;
    }
    else if (mCurrentMotion == eMudMotions::M_SneakLoop_27_473C40 || field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        mNextMotion = eMudMotions::M_WalkLoop_1_4728B0;
        bWalkOrSneak = true;
    }

    if (bWalkOrSneak)
    {
        if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_40_SneakLoop_450550
            && mCurrentMotion == eMudMotions::M_WalkLoop_1_4728B0
            && !field_16A_flags.Get(Flags_16A::eBit4_blind))
        {
            mNextMotion = eMudMotions::M_SneakLoop_27_473C40;
        }
        if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_1_WalkLoop_44FBA0 || mCurrentMotion != eMudMotions::M_SneakLoop_27_473C40)
        {
            return field_190_brain_sub_state;
        }
        mNextMotion = eMudMotions::M_WalkLoop_1_4728B0;
        return field_190_brain_sub_state;
    }
    else
    {
        mNextMotion = eMudMotions::M_RunLoop_21_473720;
        return Brain_4_ListeningToAbe::eBrain4_Running_6;
    }
}

s16 Mudokon::Brain_ListeningToAbe_State_6()
{
    if (mCurrentMotion == eMudMotions::M_Idle_0_4724E0)
    {
        if (!field_16A_flags.Get(Flags_16A::eBit4_blind))
        {
            return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
        }
        mNextMotion = eMudMotions::M_RunLoop_21_473720;
    }

    if (mCurrentMotion == eMudMotions::M_Knockback_46_4742A0)
    {
        Sound_475EC0(MudSounds::eHurt1_16);
        s16 result = GetBrainSubStateResponse_477AF0(MudAction::eSmackIntoWall_11);
        field_17E_delayed_speak = MudAction::eNone_17;
        field_194_timer = sGnFrame + 20;
        if (field_16A_flags.Get(Flags_16A::eBit4_blind))
        {
            field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
            field_16A_flags.Clear(Flags_16A::eBit5_following);
        }
        else
        {
            field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
        }
        if (result > 0)
        {
            return result;
        }
    }
    if (mCurrentMotion != eMudMotions::M_RunLoop_21_473720)
    {
        return field_190_brain_sub_state;
    }

    FP scaleToGridSizeAbs;
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        scaleToGridSizeAbs = -ScaleToGridSize(field_CC_sprite_scale);
    }
    else
    {
        scaleToGridSizeAbs = ScaleToGridSize(field_CC_sprite_scale);
    }

    if (WallHit(field_CC_sprite_scale * FP_FromInteger(50), scaleToGridSizeAbs * FP_FromInteger(3)) && !field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        mNextMotion = eMudMotions::M_Idle_0_4724E0;
        if (field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
            return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
        }
        field_194_timer = StableDelay_477570() + sGnFrame + 20;

        if (Math_NextRandom() < 20)
        {
            return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
        }
        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
        return Brain_4_ListeningToAbe::eBrain4_CrazyLaugh_21;
    }

    bool gotoTurn = false;
    if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_40_SneakLoop_450550 || field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        if (field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
            mNextMotion = eMudMotions::M_WalkLoop_1_4728B0;
            return Brain_4_ListeningToAbe::eBrain4_Walking_5;
        }
        if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_25_RunSlideStop_451330 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_0_Idle_44EEB0 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_71_Knockback_455090 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_17_CrouchIdle_456BC0)
        {
            mNextMotion = eMudMotions::M_Idle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame + 20;
            if (Math_NextRandom() < 40)
            {
                return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
            }
            field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
            return Brain_4_ListeningToAbe::eBrain4_CrazyLaugh_21;
        }
        gotoTurn = true;
    }

    if (!gotoTurn)
    {
        if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_33_RunLoop_4508E0)
        {
            if (VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(4), sActiveHero))
            {
                mNextMotion = eMudMotions::M_WalkLoop_1_4728B0;
                return Brain_4_ListeningToAbe::eBrain4_Walking_5;
            }
        }

        if (LastGameSpeak_476FF0() == GameSpeakEvents::eWait_12)
        {
            field_16A_flags.Clear(Flags_16A::eBit5_following);
            mNextMotion = eMudMotions::M_Idle_0_4724E0;
            return Brain_4_ListeningToAbe::eBrain4_GetsWaitWhileMoving_9;
        }
    }

    if (!VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale), sActiveHero))
    {
        if (VIsFacingMe(sActiveHero))
        {
            return field_190_brain_sub_state;
        }
    }
    else
    {
        if (!VIsObj_GettingNear(sActiveHero))
        {
            if (VIsFacingMe(sActiveHero))
            {
                return field_190_brain_sub_state;
            }
        }
    }

    if (!field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        mNextMotion = eMudMotions::M_RunSlideTurn_25_473AA0;
        return Brain_4_ListeningToAbe::eBrain4_CrazySlideTurn_8;
    }

    return field_190_brain_sub_state;
}

s16 Mudokon::Brain_ListeningToAbe_State_7()
{
    if (mBaseAnimatedWithPhysicsGameObject_YPos > pScreenManager->field_20_pCamPos->field_4_y + FP_FromInteger(260) && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (field_10_resources_array.ItemAt(2))
        {
            field_16A_flags.Clear(Flags_16A::eBit5_following);
            mNextMotion = eMudMotions::M_Chisel_11_4732D0;
            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
        }
        else
        {
            field_16A_flags.Clear(Flags_16A::eBit5_following);
            mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
        }
    }

    if (mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
    {
        field_17C_stand_idle_timer = 0;
        field_124 = sGnFrame + Math_RandomRange(30, 60);
        return field_190_brain_sub_state;
    }

    if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4)
    {
        field_16A_flags.Clear(Flags_16A::eBit5_following);
        return Brain_4_ListeningToAbe::eBrain4_Suiciding_16;
    }

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        field_16A_flags.Set(Flags_16A::eBit5_following);
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }

    if ((++field_17C_stand_idle_timer > 540 || Is_In_Current_Camera() != CameraPos::eCamCurrent_0) && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (field_10_resources_array.ItemAt(2))
        {
            field_16A_flags.Clear(Flags_16A::eBit5_following);
            mNextMotion = eMudMotions::M_Chisel_11_4732D0;
            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
        }
        else
        {
            field_16A_flags.Clear(Flags_16A::eBit5_following);
            mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
        }
    }

    if (NeedToTurnAround())
    {
        mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
        return field_190_brain_sub_state;
    }

    if (field_17E_delayed_speak != MudAction::eNone_17)
    {
        s16 v18 = GetBrainSubStateResponse_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
        {
            field_194_timer = sGnFrame + 20;
            field_178_brain_sub_state2 = field_190_brain_sub_state;
        }
        else
        {
            field_178_brain_sub_state2 = field_190_brain_sub_state;
            field_194_timer = StableDelay_477570() + sGnFrame + 20;
        }
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return field_190_brain_sub_state;
        }
        return v18;
    }
    GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
    if (lastSpeak == GameSpeakEvents::eNone_m1
        && ((lastSpeak = field_182, lastSpeak == GameSpeakEvents::eNone_m1)
            || (field_180_emo_tbl != Mud_Emotion::eNormal_0 && field_180_emo_tbl != Mud_Emotion::eHappy_5 && field_180_emo_tbl != Mud_Emotion::eWired_6)))
    {
        if (field_180_emo_tbl != Mud_Emotion::eWired_6 || field_124 > (s32) sGnFrame)
        {
            return field_190_brain_sub_state;
        }
        field_194_timer = sGnFrame + Math_RandomRange(22, 30);
        field_178_brain_sub_state2 = field_190_brain_sub_state;
        return Brain_4_ListeningToAbe::eBrain4_CrazyLaugh_21;
    }
    else
    {
        field_178_brain_sub_state2 = field_190_brain_sub_state;
        field_194_timer = (u16) StableDelay_477570() + sGnFrame + 20;
        switch (lastSpeak)
        {
            case GameSpeakEvents::eFart_3:
            {
                field_182 = GameSpeakEvents::eNone_m1;
                const s16 v18 = GetBrainSubStateResponse_477AF0(MudAction::eFart_6);
                if (VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale), sActiveHero))
                {
                    if (VOnSameYLevel(sActiveHero))
                    {
                        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_SteppingBack_3;
                    }
                }
                if (!v18)
                {
                    return field_190_brain_sub_state;
                }
                return v18;
            }

            case GameSpeakEvents::eHello_9:
            case GameSpeakEvents::eAllYa_23:
            {
                field_182 = GameSpeakEvents::eNone_m1;
                s16 result = GetBrainSubStateResponse_477AF0(MudAction::eHelloOrAllYa_0);
                if (result)
                {
                    return result;
                }
                return field_190_brain_sub_state;
            }

            case GameSpeakEvents::eFollowMe_10:
            {
                const s16 v26 = GetBrainSubStateResponse_477AF0(MudAction::eFollowMe_1);
                if (field_188_pTblEntry->field_0_sound == MudSounds::eOkay_12 || field_188_pTblEntry->field_0_sound == MudSounds::eLaugh_10)
                {
                    field_16A_flags.Set(Flags_16A::eBit5_following);
                    field_182 = GameSpeakEvents::eNone_m1;
                    // TODO: Wrong ??
                    if (!VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(2), sActiveHero) || VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale), sActiveHero))
                    {
                        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
                    }
                    else
                    {
                        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_CrazyFollowMe_2;
                    }
                }
                else
                {
                    field_182 = lastSpeak;
                }

                if (!v26)
                {
                    return field_190_brain_sub_state;
                }

                return v26;
            }

            case GameSpeakEvents::eAnger_11:
            case GameSpeakEvents::eSorry_24:
                field_17C_stand_idle_timer = 0;
                return field_190_brain_sub_state;

            case GameSpeakEvents::eWait_12:
                field_16A_flags.Clear(Flags_16A::eBit5_following);
                field_182 = GameSpeakEvents::eNone_m1;
                field_184_next_motion2 = eMudMotions::M_DunnoBegin_43_472790;
                return Brain_4_ListeningToAbe::eBrain4_InitializeMovement_10;

            case GameSpeakEvents::eWork_21:
                if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                {
                    return Brain_4_ListeningToAbe::eBrain4_CrazyDeny_19;
                }
                field_16A_flags.Clear(Flags_16A::eBit5_following);
                field_182 = GameSpeakEvents::eNone_m1;

                if (FindWheel_4777B0(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos))
                {
                    Mudokon* v35 = static_cast<Mudokon*>(FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)));
                    if (!v35)
                    {
                        return BrainStartWheelTurning();
                    }

                    while (!(v35->field_16A_flags.Get(Flags_16A::eBit7_stopped_at_wheel)) && v35->mCurrentMotion != eMudMotions::M_TurnWheelLoop_58_474CC0)
                    {
                        v35 = static_cast<Mudokon*>(GetStackedSlapTarget(v35->field_8_object_id, ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)));
                        if (!v35)
                        {
                            return BrainStartWheelTurning();
                        }
                    }
                    if (v35)
                    {
                        if (field_10_resources_array.ItemAt(2))
                        {
                            mNextMotion = eMudMotions::M_Chisel_11_4732D0;
                            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                        }
                        else
                        {
                            mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
                            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                        }
                    }
                    return BrainStartWheelTurning();
                }

                if (FindObjectOfType(ReliveTypes::eLever, ScaleToGridSize(field_CC_sprite_scale) + mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
                {
                    return Brain_4_ListeningToAbe::eBrain4_PullingLever_11;
                }
                if (FindObjectOfType(ReliveTypes::eLever, mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
                {
                    return Brain_4_ListeningToAbe::eBrain4_PullingLever_11;
                }

                break;

            case GameSpeakEvents::eStopIt_22:
            {
                s16 v18 = GetBrainSubStateResponse_477AF0(MudAction::eStopIt_4);
                field_194_timer = ((u32)(u16) StableDelay_477570() >> 1) + sGnFrame + 20;
                if (!v18)
                {
                    return field_190_brain_sub_state;
                }
                return v18;
            }

            default:
                return field_190_brain_sub_state;
        }

        if (field_10_resources_array.ItemAt(2))
        {
            mNextMotion = eMudMotions::M_Chisel_11_4732D0;
            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
        }
        else
        {
            mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
        }
    }
}

s16 Mudokon::Brain_ListeningToAbe_State_8()
{
    if (mCurrentMotion == eMudMotions::M_RunLoop_21_473720)
    {
        return Brain_4_ListeningToAbe::eBrain4_Running_6;
    }
    if (mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
    {
        return field_190_brain_sub_state;
    }
    return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
}

s16 Mudokon::Brain_ListeningToAbe_State_9()
{
    if (mCurrentMotion)
    {
        return field_190_brain_sub_state;
    }
    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
    field_194_timer = StableDelay_477570() + sGnFrame + 20;
    return Brain_4_ListeningToAbe::eBrain4_StopsWhileMoving_20;
}

s16 Mudokon::Brain_ListeningToAbe_State_10()
{
    if (!VIsFacingMe(sActiveHero))
    {
        mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
        return field_190_brain_sub_state;
    }
    mNextMotion = -1;

    if (static_cast<s32>(sGnFrame) <= field_194_timer)
    {
        return field_190_brain_sub_state;
    }
    mNextMotion = field_184_next_motion2;
    return field_178_brain_sub_state2;
}

s16 Mudokon::Brain_ListeningToAbe_State_11()
{
    BaseGameObject* pLever = nullptr;
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        pLever = FindObjectOfType(ReliveTypes::eLever, mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5));
    }
    else
    {
        pLever = FindObjectOfType(ReliveTypes::eLever, mBaseAnimatedWithPhysicsGameObject_XPos + ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5));
    }

    if (!pLever)
    {
        mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
        return field_190_brain_sub_state;
    }

    if (mCurrentMotion == eMudMotions::M_LeverUse_10_473020)
    {
        mNextMotion = -1;
        auto pSwitch = static_cast<Lever*>(pLever);
        pSwitch->VPull(mBaseAnimatedWithPhysicsGameObject_XPos < pSwitch->mBaseAnimatedWithPhysicsGameObject_XPos);
        field_16A_flags.Clear(Flags_16A::eBit5_following);

        if (field_10_resources_array.ItemAt(2))
        {
            mNextMotion = eMudMotions::M_Chisel_11_4732D0;
        }
        else
        {
            mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
        }
        return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
    }
    else
    {
        mNextMotion = eMudMotions::M_LeverUse_10_473020;
        return field_190_brain_sub_state;
    }
}

s16 Mudokon::Brain_ListeningToAbe_State_12()
{
    if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eMudAbuse_9)
    {
        s16 response = GetBrainSubStateResponse_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
        {
            field_194_timer = sGnFrame + 20;
            field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
            field_17E_delayed_speak = MudAction::eNone_17;
            if (!response)
            {
                return field_190_brain_sub_state;
            }
            return response;
        }
        field_194_timer = (u16)(StableDelay_477570() + sGnFrame + 20);
        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!response)
        {
            return field_190_brain_sub_state;
        }
        return response;
    }

    if (field_188_pTblEntry->field_2_next_motion == eMudMotions::M_Punch_38_474AA0)
    {
        if (LastGameSpeak_476FF0() == GameSpeakEvents::eStopIt_22)
        {
            if (mCurrentMotion == eMudMotions::M_Punch_38_474AA0 || mNextMotion == eMudMotions::M_Punch_38_474AA0)
            {
                ToStand_4724A0();
                mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
                mNextMotion = -1;
            }
            s16 result = GetBrainSubStateResponse_477AF0(MudAction::eStopIt_4);
            field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
            field_17E_delayed_speak = MudAction::eNone_17;
            field_194_timer = sGnFrame + 20;
            if (result)
            {
                return result;
            }
        }
    }

    if (static_cast<s32>(sGnFrame) <= field_194_timer)
    {
        return field_190_brain_sub_state;
    }

    if (field_188_pTblEntry->field_0_sound != MudSounds::eFartPuh_23 || field_178_brain_sub_state2 == Brain_4_ListeningToAbe::eBrain4_SteppingBack_3)
    {
        if (field_188_pTblEntry->field_0_sound != MudSounds::eLaugh_10 || (Math_NextRandom() >= 0x50u))
        {
            if (field_188_pTblEntry->field_0_sound != MudSounds::eNone)
            {
                Sound_475EC0(field_188_pTblEntry->field_0_sound);
            }
        }
        else
        {
            Sound_475EC0(MudSounds::eGiggle_8);
        }
    }
    else
    {
        if (Math_NextRandom() >= 80u)
        {
            Sound_475EC0(MudSounds::eLaugh_10);
        }
        else
        {
            Sound_475EC0(MudSounds::eGiggle_8);
        }
    }

    mNextMotion = field_188_pTblEntry->field_2_next_motion;
    if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags_16A::eBit11_get_depressed))
    {
        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
    }

    if (field_180_emo_tbl == Mud_Emotion::eSick_7)
    {
        field_18E_brain_state = Mud_Brain_State::Brain_9_Sick_47A910;
        return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
    }

    if (mNextMotion != eMudMotions::M_Punch_38_474AA0)
    {
        return field_178_brain_sub_state2;
    }

    BaseGameObject* pMudInSameGridBlock = nullptr;
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        if (FindObjectOfType(ReliveTypes::eAbe, mBaseAnimatedWithPhysicsGameObject_XPos + ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
        {
            return Brain_4_ListeningToAbe::eBrain4_RageTurn_18;
        }
        pMudInSameGridBlock = FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos + ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5));
    }
    else
    {
        if (FindObjectOfType(ReliveTypes::eAbe, mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
        {
            return Brain_4_ListeningToAbe::eBrain4_RageTurn_18;
        }
        pMudInSameGridBlock = FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5));
    }

    if (!pMudInSameGridBlock)
    {
        return field_178_brain_sub_state2;
    }

    return Brain_4_ListeningToAbe::eBrain4_RageTurn_18;
}

s16 Mudokon::Brain_ListeningToAbe_State_13()
{
    ToKnockback_473190();

    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit18_IsLastFrame);
    mNextMotion = -1;

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        return Brain_4_ListeningToAbe::eBrain4_CrazyLaugh_21;
    }

    if (field_188_pTblEntry->field_2_next_motion == eMudMotions::M_Punch_38_474AA0)
    {
        return Brain_4_ListeningToAbe::eBrain4_Knockback_14;
    }
    else
    {
        return Brain_4_ListeningToAbe::eBrain4_GetsCommand_12;
    }
}

s16 Mudokon::Brain_ListeningToAbe_State_14()
{
    if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eMudAbuse_9)
    {
        s16 v18 = GetBrainSubStateResponse_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
        {
            field_194_timer = sGnFrame + 20;
            field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
            field_17E_delayed_speak = MudAction::eNone_17;
            if (!v18)
            {
                return field_190_brain_sub_state;
            }
            return v18;
        }
        field_194_timer = (u16)(StableDelay_477570() + sGnFrame + 20);
        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return field_190_brain_sub_state;
        }
        return v18;
    }
    if (LastGameSpeak_476FF0() == GameSpeakEvents::eStopIt_22)
    {
        if (mCurrentMotion == eMudMotions::M_Punch_38_474AA0 || mNextMotion == eMudMotions::M_Punch_38_474AA0)
        {
            ToStand_4724A0();
            mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
            mNextMotion = -1;
        }

        s16 result = GetBrainSubStateResponse_477AF0(MudAction::eStopIt_4);
        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        field_194_timer = sGnFrame + 20;
        if (result)
        {
            return result;
        }
    }

    if (static_cast<s32>(sGnFrame) < field_194_timer)
    {
        return field_190_brain_sub_state;
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        if (!FindObjectOfType(ReliveTypes::eAbe, mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
        {
            if (FindObjectOfType(ReliveTypes::eAbe, mBaseAnimatedWithPhysicsGameObject_XPos + ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5))
                || (!FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5))
                    && FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos + ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5))))
            {
                mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
                field_194_timer = sGnFrame + 15;
                return Brain_4_ListeningToAbe::eBrain4_GetsCommand_12;
            }
        }
    }
    else
    {
        if (!FindObjectOfType(ReliveTypes::eAbe, mBaseAnimatedWithPhysicsGameObject_XPos + ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
        {
            if (FindObjectOfType(ReliveTypes::eAbe, mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
            {
                mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
                field_194_timer = sGnFrame + 15;
                return Brain_4_ListeningToAbe::eBrain4_GetsCommand_12;
            }

            if (!FindObjectOfType(ReliveTypes::eMudokon, ScaleToGridSize(field_CC_sprite_scale) + mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
            {
                if (FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
                {
                    mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
                    field_194_timer = sGnFrame + 15;
                    return Brain_4_ListeningToAbe::eBrain4_GetsCommand_12;
                }
            }
        }
    }
    return Brain_4_ListeningToAbe::eBrain4_GetsCommand_12;
}

s16 Mudokon::Brain_ListeningToAbe_State_15()
{
    if (static_cast<s32>(sGnFrame) <= field_194_timer)
    {
        return field_190_brain_sub_state;
    }

    if (field_188_pTblEntry->field_0_sound != MudSounds::eNone)
    {
        Sound_475EC0(field_188_pTblEntry->field_0_sound);
    }

    mNextMotion = field_188_pTblEntry->field_2_next_motion;
    if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags_16A::eBit11_get_depressed))
    {
        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
    }
    if (field_182 != GameSpeakEvents::eFollowMe_10)
    {
        return field_178_brain_sub_state2;
    }

    field_16A_flags.Set(Flags_16A::eBit5_following);
    field_182 = GameSpeakEvents::eNone_m1;
    return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
}

s16 Mudokon::Brain_ListeningToAbe_State_16()
{
    if (mCurrentMotion != eMudMotions::M_SlapOwnHead_56_4727D0)
    {
        mNextMotion = eMudMotions::M_SlapOwnHead_56_4727D0;
    }

    if (LastGameSpeak_476FF0() != GameSpeakEvents::eStopIt_22 && field_17E_delayed_speak != MudAction::eSorry_8)
    {
        return field_190_brain_sub_state;
    }

    s16 result = GetBrainSubStateResponse_477AF0(MudAction::eStopIt_4);
    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
    field_17E_delayed_speak = MudAction::eNone_17;
    field_194_timer = sGnFrame + 20;
    if (!result)
    {
        return field_190_brain_sub_state;
    }
    return result;
}

s16 Mudokon::Brain_ListeningToAbe_State_17()
{
    if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eMudAbuse_9)
    {
        s16 v18 = GetBrainSubStateResponse_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
        {
            field_194_timer = sGnFrame + 20;
            field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
            field_17E_delayed_speak = MudAction::eNone_17;
            if (!v18)
            {
                return field_190_brain_sub_state;
            }
            return v18;
        }
        field_194_timer = (u16)(StableDelay_477570() + sGnFrame + 20);
        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return field_190_brain_sub_state;
        }
        return v18;
    }

    if (LastGameSpeak_476FF0() != GameSpeakEvents::eStopIt_22)
    {
        if (mCurrentMotion)
        {
            return field_190_brain_sub_state;
        }
        mNextMotion = eMudMotions::M_Punch_38_474AA0;
        return Brain_4_ListeningToAbe::eBrain4_Idle_7;
    }

    if (mCurrentMotion == eMudMotions::M_Punch_38_474AA0 || mNextMotion == eMudMotions::M_Punch_38_474AA0)
    {
        ToStand_4724A0();
        mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
        mNextMotion = -1;
    }

    s16 result = GetBrainSubStateResponse_477AF0(MudAction::eStopIt_4);
    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
    field_17E_delayed_speak = MudAction::eNone_17;
    field_194_timer = sGnFrame + 20;
    if (result)
    {
        return result;
    }

    if (mCurrentMotion)
    {
        return field_190_brain_sub_state;
    }
    mNextMotion = eMudMotions::M_Punch_38_474AA0;
    return Brain_4_ListeningToAbe::eBrain4_Idle_7;
}

s16 Mudokon::Brain_ListeningToAbe_State_18()
{
    if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eMudAbuse_9)
    {
        s16 v18 = GetBrainSubStateResponse_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
        {
            field_194_timer = (s16)(sGnFrame + 20);
        }
        else
        {
            field_194_timer = (u16) Mudokon::StableDelay_477570() + sGnFrame + 20;
        }
        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return field_190_brain_sub_state;
        }
        return v18;
    }
    else
    {
        if (LastGameSpeak_476FF0() != GameSpeakEvents::eStopIt_22)
        {
            if (mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
            return Brain_4_ListeningToAbe::eBrain4_RageSlap_17;
        }

        if (mCurrentMotion == eMudMotions::M_Punch_38_474AA0 || mNextMotion == eMudMotions::M_Punch_38_474AA0)
        {
            ToStand_4724A0();
            mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
            mNextMotion = -1;
        }

        s16 result = GetBrainSubStateResponse_477AF0(MudAction::eStopIt_4);
        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        field_194_timer = sGnFrame + 20;

        if (!result)
        {
            if (mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
            result = Brain_4_ListeningToAbe::eBrain4_RageSlap_17;
        }
        return result;
    }
}

s16 Mudokon::Brain_ListeningToAbe_State_19()
{
    if (static_cast<s32>(sGnFrame) <= field_194_timer)
    {
        return field_190_brain_sub_state;
    }

    if (field_180_emo_tbl == Mud_Emotion::eAngry_1 || field_180_emo_tbl == Mud_Emotion::eAggressive_2)
    {
        Sound_475EC0(MudSounds::eNoAngry_21);
    }
    else if (field_180_emo_tbl == Mud_Emotion::eSad_3 || field_180_emo_tbl == Mud_Emotion::eSuicidal_4)
    {
        Sound_475EC0(MudSounds::eNoSad_22);
    }
    else
    {
        Sound_475EC0(MudSounds::eNuhUh_13);
    }

    mNextMotion = eMudMotions::M_Speak_6_472FA0;
    return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
}

s16 Mudokon::Brain_ListeningToAbe_State_20()
{
    if (static_cast<s32>(sGnFrame) <= field_194_timer)
    {
        return field_190_brain_sub_state;
    }
    Sound_475EC0(MudSounds::eOkay_12);
    mNextMotion = eMudMotions::M_Speak_3_472FA0;
    return field_178_brain_sub_state2;
}

s16 Mudokon::Brain_ListeningToAbe_State_21()
{
    if (mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
    {
        return Brain_4_ListeningToAbe::eBrain4_CrazyLaugh_21;
    }

    if (static_cast<s32>(sGnFrame) <= field_194_timer)
    {
        return field_190_brain_sub_state;
    }

    field_16A_flags.Clear(Flags_16A::eBit12_alert_enemies);

    if (Math_NextRandom() >= 90u)
    {
        Sound_475EC0(MudSounds::eLaugh_10);
    }
    else
    {
        Sound_475EC0(MudSounds::eGiggle_8);
    }

    mNextMotion = eMudMotions::M_Speak_6_472FA0;
    return field_178_brain_sub_state2;
}

s16 Mudokon::Brain_ListeningToAbe_State_22()
{
    if (field_10_resources_array.ItemAt(2))
    {
        mNextMotion = eMudMotions::M_Chisel_11_4732D0;
    }
    else
    {
        mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
    }

    if (mCurrentMotion != eMudMotions::M_CrouchIdle_15_474040 && mCurrentMotion != eMudMotions::M_Chisel_11_4732D0)
    {
        return field_190_brain_sub_state;
    }

    RemoveAlerted();

    if (field_10_resources_array.ItemAt(2))
    {
        field_18E_brain_state = Mud_Brain_State::Brain_1_Chisel_47C5F0;
    }
    else
    {
        field_18E_brain_state = Mud_Brain_State::Brain_2_Scrub_47D270;
    }

    field_182 = GameSpeakEvents::eNone_m1;
    return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
}

s16 Mudokon::Brain_5_ShrivelDeath_4714A0()
{
    if (field_194_timer < static_cast<s32>(sGnFrame + 80))
    {
        field_CC_sprite_scale -= FP_FromDouble(0.008);
        field_D0_r -= 2;
        field_D2_g -= 2;
        field_D4_b -= 2;
    }

    if (static_cast<s32>(sGnFrame) < field_194_timer - 24)
    {
        DeathSmokeEffect(true);
    }

    // Finally fizzled out
    if (field_CC_sprite_scale < FP_FromInteger(0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 100;
}

s16 Mudokon::Brain_6_Escape_47A560()
{
    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds.Find_Impl(field_11C_bird_portal_id));
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        bool noBirdPortalOrPortalIsDead = false;
        if (!pBirdPortal || pBirdPortal->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            noBirdPortalOrPortalIsDead = true;
        }

        bool bOver60Away = false;
        if (pBirdPortal)
        {
            bOver60Away = FP_Abs(pBirdPortal->field_30_ypos - mBaseAnimatedWithPhysicsGameObject_YPos) > (field_CC_sprite_scale * FP_FromInteger(60));
        }

        if (bOver60Away || noBirdPortalOrPortalIsDead)
        {
            sGoingToBirdPortalMudCount_5C3012--;
            field_16C_flags.Clear(Flags_16C::eBit3_Unknown);

            if (pBirdPortal)
            {
                if (sGoingToBirdPortalMudCount_5C3012 == 0)
                {
                    pBirdPortal->VKillPortalClipper();
                    pBirdPortal->VGiveShrykull(TRUE);
                }
                field_11C_bird_portal_id = -1;
            }

            mNextMotion = eMudMotions::M_Idle_0_4724E0;
            field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe_477B40;
            return Brain_4_ListeningToAbe::eBrain4_Idle_7;
        }
        else
        {
            switch (field_190_brain_sub_state)
            {
                case Brain_6_Escape::eBrain6_PortalOppened_0:
                    if (!pBirdPortal->VActivePortal())
                    {
                        return field_190_brain_sub_state;
                    }
                    field_194_timer = sGnFrame + Math_NextRandom() % 8;
                    return Brain_6_Escape::eBrain6_StandingUp_1;

                case Brain_6_Escape::eBrain6_StandingUp_1:
                    if (static_cast<s32>(sGnFrame) <= field_194_timer)
                    {
                        return field_190_brain_sub_state;
                    }
                    return FP_Abs(pBirdPortal->field_2C_xpos - mBaseAnimatedWithPhysicsGameObject_XPos) >= ScaleToGridSize(field_CC_sprite_scale) ? 2 : 4;

                case Brain_6_Escape::eBrain6_Running_2:
                    if (mCurrentMotion == eMudMotions::M_CrouchIdle_15_474040)
                    {
                        mNextMotion = eMudMotions::M_CrouchToStand_18_474150;
                    }
                    if (mCurrentMotion == eMudMotions::M_Chisel_11_4732D0)
                    {
                        mNextMotion = eMudMotions::M_Idle_0_4724E0;
                    }
                    if (mCurrentMotion == eMudMotions::M_Duck_53_474A40)
                    {
                        mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
                    }

                    if (mCurrentMotion == eMudMotions::M_Idle_0_4724E0 || mCurrentMotion == eMudMotions::M_WalkLoop_1_4728B0)
                    {
                        if (FacingTarget(pBirdPortal))
                        {
                            mNextMotion = eMudMotions::M_RunLoop_21_473720;
                        }
                        else
                        {
                            mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
                        }
                    }

                    if (mCurrentMotion != eMudMotions::M_RunLoop_21_473720)
                    {
                        return field_190_brain_sub_state;
                    }

                    if (!FacingTarget(pBirdPortal))
                    {
                        mNextMotion = eMudMotions::M_RunSlideTurn_25_473AA0;
                        return field_190_brain_sub_state;
                    }

                    if (!VIntoBirdPortal(3)) // Finds portal to jump into
                    {
                        return field_190_brain_sub_state;
                    }

                    field_190_brain_sub_state = Brain_6_Escape::eBrain6_Jumping_3;
                    mNextMotion = eMudMotions::M_RunJumpMid_36_474570;
                    return field_190_brain_sub_state;

                case Brain_6_Escape::eBrain6_Replacing_4:
                    if (mCurrentMotion == eMudMotions::M_CrouchIdle_15_474040)
                    {
                        mNextMotion = eMudMotions::M_CrouchToStand_18_474150;
                    }
                    if (mCurrentMotion == eMudMotions::M_Chisel_11_4732D0)
                    {
                        mNextMotion = eMudMotions::M_Idle_0_4724E0;
                    }
                    if (mCurrentMotion == eMudMotions::M_Duck_53_474A40)
                    {
                        mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
                    }

                    if (mCurrentMotion == eMudMotions::M_Idle_0_4724E0 || mCurrentMotion == eMudMotions::M_WalkLoop_1_4728B0)
                    {
                        if ((pBirdPortal->field_26_side == PortalSide::eRight_0 && field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) || (pBirdPortal->field_26_side == PortalSide::eLeft_1 && !field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
                        {
                            mNextMotion = eMudMotions::M_StandingTurn_2_472BF0;
                        }
                        else
                        {
                            mNextMotion = eMudMotions::M_RunLoop_21_473720;
                        }
                    }

                    if (mCurrentMotion != eMudMotions::M_RunLoop_21_473720)
                    {
                        return field_190_brain_sub_state;
                    }

                    if (FP_Abs(pBirdPortal->field_2C_xpos - mBaseAnimatedWithPhysicsGameObject_XPos) <= ScaleToGridSize(field_CC_sprite_scale))
                    {
                        return field_190_brain_sub_state;
                    }
                    return Brain_6_Escape::eBrain6_Running_2;
            }
        }
    }
    return field_190_brain_sub_state;
}

s16 Mudokon::Brain_7_FallAndSmackDeath_471600()
{
    if (field_190_brain_sub_state == 0)
    {
        // Wait for falling
        Mudokon_SFX(MudSounds::eDeathDropScream_15, 0, 0, this);
        field_194_timer = sGnFrame + 60;
        return 1;
    }

    if (field_190_brain_sub_state == 1)
    {
        // Scream
        if (static_cast<s32>(sGnFrame) == field_194_timer - 6)
        {
            SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
        }

        // Shake from ze floor slam & die
        if (static_cast<s32>(sGnFrame) > field_194_timer)
        {
            Environment_SFX_457A40(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 32767, this);
            relive_new ScreenShake(0, 0);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
    return field_190_brain_sub_state;
}

const MudSounds sAngryWorkerResponses_55CFCA[16] = {
    MudSounds::eHiAngry_18,
    MudSounds::eNoAngry_21,
    MudSounds::eAnger_5,
    MudSounds::eNoAngry_21,
    MudSounds::eAnger_5,
    MudSounds::eAnger_5,
    MudSounds::eAnger_5,
    MudSounds::eAnger_5,
    MudSounds::eAnger_5,
    MudSounds::eAnger_5,
    MudSounds::eAnger_5,
    MudSounds::eAnger_5,
    MudSounds::eNoAngry_21,
    MudSounds::eNoAngry_21,
    MudSounds::eHiAngry_18,
    MudSounds::eAnger_5,
};


s16 Mudokon::Brain_8_AngryWorker_47E910()
{
    switch (field_190_brain_sub_state)
    {
        case Brain_8_AngryWorker::eBrain8_Inactive_0:
            if (mCurrentMotion)
            {
                mNextMotion = eMudMotions::M_Idle_0_4724E0;
            }

            if (field_16A_flags.Get(Flags_16A::eBit15_ring_and_angry_mud_timeout))
            {
                const GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
                if (lastSpeak < GameSpeakEvents::eHello_9 || lastSpeak > GameSpeakEvents::eSorry_24)
                {
                    return field_190_brain_sub_state;
                }
            }

            field_194_timer = sGnFrame + Math_RandomRange(30, 45);
            // adds mudokon lag when quicksaving/quickloading in the same screen
            AddAlerted();

            return sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                       FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                       FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                       FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                       FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                       TlvTypes::WorkWheel_79)
                        != 0
                     ? 4
                     : 1;

        case Brain_8_AngryWorker::eBrain8_LeverIdle_1:
        {
            if (field_17E_delayed_speak == MudAction::eSorry_8)
            {
                field_194_timer = sGnFrame + 20;
                return Brain_8_AngryWorker::eBrain8_Speaking_3;
            }

            const GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
            if (lastSpeak >= GameSpeakEvents::eHello_9
                && lastSpeak <= GameSpeakEvents::eSorry_24
                && sActiveHero->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                field_160_delayed_speak = sAngryWorkerResponses_55CFCA[static_cast<s32>(lastSpeak) - 9];
                field_194_timer = StableDelay_477570() + sGnFrame + 20;
                return Brain_8_AngryWorker::eBrain8_Speaking_3;
            }

            if (field_194_timer > static_cast<s32>(sGnFrame))
            {
                return field_190_brain_sub_state;
            }

            const FP ypos = mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(field_CC_sprite_scale);
            FP xOff = {};
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                xOff = -ScaleToGridSize(field_CC_sprite_scale);
            }
            else
            {
                xOff = ScaleToGridSize(field_CC_sprite_scale);
            }

            auto pLever = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, xOff + mBaseAnimatedWithPhysicsGameObject_XPos, ypos));
            if (pLever)
            {
                mNextMotion = eMudMotions::M_LeverUse_10_473020;
                pLever->VPull(mBaseAnimatedWithPhysicsGameObject_XPos < pLever->mBaseAnimatedWithPhysicsGameObject_XPos);
            }
        }
            return Brain_8_AngryWorker::eBrain8_PullingLever_2;

        case Brain_8_AngryWorker::eBrain8_PullingLever_2:
            if (mCurrentMotion)
            {
                return field_190_brain_sub_state;
            }

            if (gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender)
            {
                field_194_timer = sGnFrame + 90;
            }
            else
            {
                field_194_timer = sGnFrame + Math_RandomRange(30, 45);
            }
            return Brain_8_AngryWorker::eBrain8_LeverIdle_1;

        case Brain_8_AngryWorker::eBrain8_Speaking_3:
            if (field_194_timer > static_cast<s32>(sGnFrame))
            {
                return field_190_brain_sub_state;
            }

            if (field_17E_delayed_speak == MudAction::eSorry_8)
            {
                field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eSorry_8);
                field_17E_delayed_speak = MudAction::eNone_17;
                if (field_188_pTblEntry->field_0_sound != MudSounds::eNone)
                {
                    Sound_475EC0(field_188_pTblEntry->field_0_sound);
                }
                mNextMotion = field_188_pTblEntry->field_2_next_motion;
                field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
                field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe_477B40;
                return Brain_4_ListeningToAbe::eBrain4_Idle_7;
            }
            else
            {
                mNextMotion = eMudMotions::M_Speak_6_472FA0;
                Sound_475EC0(field_160_delayed_speak);
                field_194_timer = sGnFrame + Math_RandomRange(30, 45);
                return sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                           FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                           FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                           FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                           FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                           TlvTypes::WorkWheel_79)
                            != 0
                         ? 4
                         : 1;
            }
            break;

        case Brain_8_AngryWorker::eBrain8_WheelIdle_4:
            if (field_194_timer > static_cast<s32>(sGnFrame))
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::M_TurnWheelBegin_57_474C00;
            return Brain_8_AngryWorker::eBrain8_UsingWheel_5;

        case Brain_8_AngryWorker::eBrain8_UsingWheel_5:
        {
            const GameSpeakEvents lastSpeak2 = LastGameSpeak_476FF0();
            if (lastSpeak2 >= GameSpeakEvents::eHello_9
                && lastSpeak2 <= GameSpeakEvents::eSorry_24
                && sActiveHero->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                mNextMotion = eMudMotions::M_Idle_0_4724E0;
                field_160_delayed_speak = sAngryWorkerResponses_55CFCA[static_cast<s32>(lastSpeak2) - 9];
                field_194_timer = StableDelay_477570() + sGnFrame + 20;
                return Brain_8_AngryWorker::eBrain8_Speaking_3;
            }

            if (field_17E_delayed_speak != MudAction::eSorry_8)
            {
                return field_190_brain_sub_state;
            }

            mNextMotion = eMudMotions::M_Idle_0_4724E0;
            field_194_timer = sGnFrame + 20;
        }
            return Brain_8_AngryWorker::eBrain8_Speaking_3;

        case Brain_8_AngryWorker::eBrain8_Unused_6:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }

            if (mCurrentMotion != eMudMotions::M_Idle_0_4724E0)
            {
                if (mCurrentMotion == eMudMotions::M_CrouchIdle_15_474040)
                {
                    mNextMotion = eMudMotions::M_Idle_0_4724E0;
                    return field_190_brain_sub_state;
                }
                else
                {
                    if (mCurrentMotion == eMudMotions::M_Duck_53_474A40)
                    {
                        mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
                    }
                    return field_190_brain_sub_state;
                }
            }
            else
            {
                return Brain_8_AngryWorker::eBrain8_Inactive_0;
            }
            break;

        case Brain_8_AngryWorker::eBrain8_Unused_7:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::M_DuckKnockback_55_474250;
            field_194_timer = sGnFrame + 60;
            return Brain_8_AngryWorker::eBrain8_Unused_6;
    }

    return field_190_brain_sub_state;
}

s16 Mudokon::Brain_9_Sick_47A910()
{
    // TODO: Lame hack, tortured muds make the "real" mud do nothing.
    if (FindObjectOfType(ReliveTypes::eTorturedMud, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(50)) || !gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        LastGameSpeak_476FF0(); // TODO: Pointless call ??
        return field_190_brain_sub_state;
    }

    if (Event_Is_Event_In_Range(
            kEventMudokonAbuse,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eMudAbuse_9;
    }

    if (Event_Is_Event_In_Range(
            kEventMudokonComfort,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eComfort_10;
    }

    if (Event_Is_Event_In_Range(
            kEventMudokonLaugh,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eLaugh_12;
    }

    if (Event_Is_Event_In_Range(kEventAbeDead,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eDuck_13;
    }

    if (Event_Is_Event_In_Range(
            kEventShooting,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eWait_2;
    }

    // Have Abe make a sad noise when he first sees sick Mudokons.
    if (!field_16A_flags.Get(Flags_16A::eBit9_seen_while_sick) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        if (sActiveHero->field_128.field_18_say == MudSounds::eNone)
        {
            sActiveHero->field_128.field_18_say = MudSounds::eSadUgh_28;
            sActiveHero->field_144_auto_say_timer = sGnFrame + 10;
        }
        field_16A_flags.Set(Flags_16A::eBit9_seen_while_sick);
    }

    switch (field_190_brain_sub_state)
    {
        case Brain_9_Sick::eBrain9_Inactive_0:
            if (mCurrentMotion != eMudMotions::M_CrouchIdle_15_474040)
            {
                if (mCurrentMotion != eMudMotions::M_StandToCrouch_17_474120 && mNextMotion != eMudMotions::M_StandToCrouch_17_474120)
                {
                    mNextMotion = eMudMotions::M_StandToCrouch_17_474120;
                }
                return field_190_brain_sub_state;
            }

            AddAlerted();

            mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
            return Brain_9_Sick::eBrain9_Idle_1;

        case Brain_9_Sick::eBrain9_Idle_1:
            if (mCurrentMotion == eMudMotions::M_Fall_49_472C60)
            {
                return Brain_9_Sick::eBrain9_Inactive_0;
            }

            if (mCurrentMotion != eMudMotions::M_CrouchIdle_15_474040)
            {
                if (mCurrentMotion == eMudMotions::M_Duck_53_474A40)
                {
                    mNextMotion = eMudMotions::M_CrouchIdle_15_474040;
                }
                return field_190_brain_sub_state;
            }

            if (field_180_emo_tbl != Mud_Emotion::eSick_7)
            {
                mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
                mNextMotion = eMudMotions::M_Idle_0_4724E0;
                relive_new PossessionFlicker(this, 8, 155, 255, 32);
                return Brain_9_Sick::eBrain9_StandingUp_3;
            }

            if (field_17E_delayed_speak == MudAction::eNone_17)
            {
                const GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
                if (lastSpeak == GameSpeakEvents::eNone_m1)
                {
                    return field_190_brain_sub_state;
                }

                field_178_brain_sub_state2 = field_190_brain_sub_state;
                field_194_timer = StableDelay_477570() + sGnFrame + 20;

                switch (lastSpeak)
                {
                    case GameSpeakEvents::eFart_3:
                        field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eFart_6);
                        break;
                    case GameSpeakEvents::Slig_BS_5:
                    case GameSpeakEvents::Slig_LookOut_6:
                    case GameSpeakEvents::Slig_BS2_7:
                    case GameSpeakEvents::Slig_Laugh_8:
                    case GameSpeakEvents::eHello_9:
                    case GameSpeakEvents::eFollowMe_10:
                    case GameSpeakEvents::eWait_12:
                    case GameSpeakEvents::eWork_21:
                    case GameSpeakEvents::eStopIt_22:
                    case GameSpeakEvents::eAllYa_23:
                    case GameSpeakEvents::Slig_Hi_27:
                    case GameSpeakEvents::Slig_HereBoy_28:
                    case GameSpeakEvents::Slig_GetEm_29:
                    case GameSpeakEvents::Slig_Freeze_31:
                        field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eWait_2);
                        break;
                    default:
                        return field_190_brain_sub_state;
                }
            }
            else
            {
                field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eSick_7, field_17E_delayed_speak);
                if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
                {
                    field_194_timer = sGnFrame + 20;
                }
                else
                {
                    field_194_timer = StableDelay_477570() + sGnFrame + 20;
                }
                field_178_brain_sub_state2 = field_190_brain_sub_state;
                field_17E_delayed_speak = MudAction::eNone_17;
            }

            field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
            if (field_188_pTblEntry->field_6_sub_state)
            {
                return Brain_9_Sick::eBrain9_Duck_2;
            }
            break;

        case Brain_9_Sick::eBrain9_Duck_2:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }

            if (field_188_pTblEntry->field_0_sound != MudSounds::eNone)
            {
                Sound_475EC0(field_188_pTblEntry->field_0_sound);
            }

            mNextMotion = eMudMotions::M_Duck_53_474A40;
            field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
            return field_178_brain_sub_state2;

        case Brain_9_Sick::eBrain9_StandingUp_3:
            if (mCurrentMotion)
            {
                return field_190_brain_sub_state;
            }

            Sound_475EC0(MudSounds::eFart_7);
            mNextMotion = eMudMotions::M_Speak_6_472FA0;

            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                New_Smoke_Particles(
                    mBaseAnimatedWithPhysicsGameObject_XPos + (FP_FromInteger(12) * field_CC_sprite_scale),
                    mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(24) * field_CC_sprite_scale),
                    (FP_FromDouble(0.5) * field_CC_sprite_scale),
                    3, 32u, 128u, 32u);
            }
            else
            {
                New_Smoke_Particles(
                    mBaseAnimatedWithPhysicsGameObject_XPos - (FP_FromInteger(12) * field_CC_sprite_scale),
                    mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(24) * field_CC_sprite_scale),
                    (FP_FromDouble(0.5) * field_CC_sprite_scale),
                    3, 32u, 128u, 32u);
            }
            return Brain_9_Sick::eBrain9_Farting_4;

        case Brain_9_Sick::eBrain9_Farting_4:
            if (mCurrentMotion)
            {
                return field_190_brain_sub_state;
            }
            field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe_477B40;
            return Brain_4_ListeningToAbe::eBrain4_Inactive_0;

        default:
            return field_190_brain_sub_state;
    }
    return field_190_brain_sub_state;
}

void Mudokon::M_Idle_0_4724E0()
{
    CheckFloorGone_472320();

    BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

    if (BaseAliveGameObjectCollisionLine)
    {
        if ((BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_32 || BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_36) && BaseAliveGameObjectId == -1)
        {
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);

            VOnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {bRect.w, static_cast<s16>(bRect.h + 5)},
                ObjList_5C1B78, 1, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
        }
    }

    switch (mNextMotion)
    {
        case eMudMotions::M_WalkLoop_1_4728B0:
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = -ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9);
            }
            else
            {
                field_C4_velx = ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9);
            }
            mCurrentMotion = eMudMotions::M_WalkBegin_7_472AB0;
            mNextMotion = -1;
            break;

        case eMudMotions::M_SneakLoop_27_473C40:
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = -ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(10);
            }
            else
            {
                field_C4_velx = ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(10);
            }
            mCurrentMotion = eMudMotions::M_SneakBegin_32_473F80;
            mNextMotion = -1;
            break;

        case eMudMotions::M_RunLoop_21_473720:
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = -ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4);
            }
            else
            {
                field_C4_velx = ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4);
            }

            mCurrentMotion = eMudMotions::M_StandToRun_37_4749A0;
            mNextMotion = -1;
            break;

        case eMudMotions::M_Chant_50_473040:
            SND_SEQ_PlaySeq(SeqId::MudokonChant_11, 0, 1);
            mCurrentMotion = eMudMotions::M_Chant_50_473040;
            mNextMotion = -1;
            break;

        case eMudMotions::M_CrouchIdle_15_474040:
            mCurrentMotion = eMudMotions::M_StandToCrouch_17_474120;
            mNextMotion = -1;
            break;

        case eMudMotions::M_Chisel_11_4732D0:
        case eMudMotions::M_CrouchScrub_14_473560:
            mCurrentMotion = eMudMotions::M_StandToCrouch_17_474120;
            break;

        default:
            if (mNextMotion != -1)
            {
                mCurrentMotion = mNextMotion;
                mNextMotion = -1;
            }
            break;
    }
}

void Mudokon::M_WalkLoop_1_4728B0()
{
    Event_Broadcast(kEventNoise, this);
    if (WallHit(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        ToKnockback_473190();
    }
    else if (sObjectIds.Find_Impl(BaseAliveGameObjectId) && field_16A_flags.Get(Flags_16A::eBit4_blind) && (WallHit(field_CC_sprite_scale * FP_FromInteger(1), field_C4_velx)))
    {
        ToKnockback_473190();
    }
    else
    {
        MoveOnLine_4720D0();
        if (mCurrentMotion == eMudMotions::M_WalkLoop_1_4728B0)
        {
            if (field_20_animation.field_92_current_frame == 2 || field_20_animation.field_92_current_frame == 11)
            {
                if (mNextMotion == eMudMotions::M_Idle_0_4724E0)
                {
                    mNextMotion = -1;
                    if (field_20_animation.field_92_current_frame == 2)
                    {
                        mCurrentMotion = eMudMotions::M_MidWalkToIdle_9_472BD0;
                    }
                    else
                    {
                        mCurrentMotion = eMudMotions::M_WalkToIdle_8_472B30;
                    }
                }
                else if (mNextMotion == eMudMotions::M_StandingTurn_2_472BF0)
                {
                    if (field_20_animation.field_92_current_frame != 2)
                    {
                        mCurrentMotion = eMudMotions::M_MidWalkToIdle_9_472BD0;
                    }
                    else
                    {
                        mCurrentMotion = eMudMotions::M_WalkToIdle_8_472B30;
                    }
                }
            }
            else if (field_20_animation.field_92_current_frame == 5 || field_20_animation.field_92_current_frame == 14)
            {
                Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);
                MapFollowMe(TRUE);
                if (mNextMotion == eMudMotions::M_RunLoop_21_473720)
                {
                    mNextMotion = -1;
                    if (field_20_animation.field_92_current_frame == 5)
                    {
                        mCurrentMotion = eMudMotions::M_MidWalkToRun_20_4736D0;
                    }
                    else
                    {
                        mCurrentMotion = eMudMotions::M_WalkToRun_19_473600;
                    }
                }
                else if (mNextMotion == eMudMotions::M_SneakLoop_27_473C40)
                {
                    auto curFrame2 = field_20_animation.field_92_current_frame;
                    mNextMotion = -1;
                    if (curFrame2 == 5)
                    {
                        mCurrentMotion = eMudMotions::M_MidWalkToSneak_28_473D60;
                    }
                    else
                    {
                        mCurrentMotion = eMudMotions::M_WalkToSneak_30_473EE0;
                    }
                }
            }
        }
    }
}

void Mudokon::M_StandingTurn_2_472BF0()
{
    CheckFloorGone_472320();

    if (field_20_animation.field_92_current_frame == 0)
    {
        Environment_SFX_457A40(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        ToStand_4724A0();
    }
}

void Mudokon::M_Speak_472FA0()
{
    CheckFloorGone_472320();
    SetPal(field_180_emo_tbl);

    if (field_16A_flags.Get(Flags_16A::eBit12_alert_enemies))
    {
        Event_Broadcast(kEventNoise, this);
        Event_Broadcast(kEventSuspiciousNoise, this);
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_16A_flags.Set(Flags_16A::eBit12_alert_enemies);
        SetPal(Mud_Emotion::eNormal_0);
        ToStand_4724A0();
    }
}

void Mudokon::M_WalkBegin_7_472AB0()
{
    Event_Broadcast(kEventNoise, this);
    if (WallHit(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = eMudMotions::M_WalkLoop_1_4728B0;
        }
        MoveOnLine_4720D0();
    }
}

void Mudokon::M_WalkToIdle_8_472B30()
{
    Event_Broadcast(kEventNoise, this);
    if (WallHit(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        MoveOnLine_4720D0();

        if (field_20_animation.field_92_current_frame == 0)
        {
            Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);
            return;
        }

        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe(TRUE);
            ToStand_4724A0();
        }
    }
}

void Mudokon::M_MidWalkToIdle_9_472BD0()
{
    M_WalkToIdle_8_472B30();
}

void Mudokon::M_LeverUse_10_473020()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::M_Chisel_11_4732D0()
{
    CheckFloorGone_472320();

    if (gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (sGnFrame & 1)
            {
                SFX_Play_Mono(SoundEffect::Chisel_91, 0, field_CC_sprite_scale);

                FP sparkY = {};
                FP sparkX = {};
                if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    sparkY = mBaseAnimatedWithPhysicsGameObject_YPos - (field_CC_sprite_scale * FP_FromInteger(3));
                    sparkX = mBaseAnimatedWithPhysicsGameObject_XPos - (field_CC_sprite_scale * FP_FromInteger(18));
                }
                else
                {
                    sparkY = mBaseAnimatedWithPhysicsGameObject_YPos - (field_CC_sprite_scale * FP_FromInteger(3));
                    sparkX = (field_CC_sprite_scale * FP_FromInteger(18)) + mBaseAnimatedWithPhysicsGameObject_XPos;
                }

                relive_new Spark(
                    sparkX + FP_FromInteger(field_DA_xOffset),
                    sparkY,
                    field_CC_sprite_scale,
                    9,
                    0,
                    255,
                    SparkType::eSmallChantParticle_0);
            }
        }
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        switch (mNextMotion)
        {
            case eMudMotions::M_Idle_0_4724E0:
                mCurrentMotion = eMudMotions::M_StopChisel_13_473530;
                return;

            case eMudMotions::M_CrouchIdle_15_474040:
                mCurrentMotion = eMudMotions::M_StopChisel_13_473530;
                break;

            case -1:
                return;

            default:
                mCurrentMotion = mNextMotion;
                break;
        }

        mNextMotion = -1;
    }
}

void Mudokon::M_StartChisel_12_473500()
{
    CheckFloorGone_472320();
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_Chisel_11_4732D0;
    }
}

void Mudokon::M_StopChisel_13_473530()
{
    CheckFloorGone_472320();
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_CrouchIdle_15_474040;
    }
}

void Mudokon::M_CrouchScrub_14_473560()
{
    CheckFloorGone_472320();

    if (gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        if (field_20_animation.field_92_current_frame == 2)
        {
            SFX_Play_Mono(SoundEffect::Clean1_71, 0);
        }
        else if (field_20_animation.field_92_current_frame == 6)
        {
            SFX_Play_Mono(SoundEffect::Clean2_72, 0);
        }
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_CrouchIdle_15_474040;
    }
}

void Mudokon::M_CrouchIdle_15_474040()
{
    CheckFloorGone_472320();

    if (mNextMotion == eMudMotions::M_Duck_53_474A40)
    {
        mCurrentMotion = eMudMotions::M_ToDuck_52_474A20;
        mNextMotion = -1;
    }
    else if (mNextMotion == eMudMotions::M_Idle_0_4724E0)
    {
        mCurrentMotion = eMudMotions::M_CrouchToStand_18_474150;
        mNextMotion = -1;
    }
    else if (mNextMotion == eMudMotions::M_Chisel_11_4732D0)
    {
        mCurrentMotion = eMudMotions::M_StartChisel_12_473500;
        mNextMotion = -1;
    }
    else if (mNextMotion != -1)
    {
        mCurrentMotion = mNextMotion;
        mNextMotion = -1;
    }
}

void Mudokon::M_CrouchTurn_16_4740E0()
{
    CheckFloorGone_472320();

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_CrouchIdle_15_474040;
        field_20_animation.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
    }
}

void Mudokon::M_StandToCrouch_17_474120()
{
    CheckFloorGone_472320();
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_CrouchIdle_15_474040;
    }
}

void Mudokon::M_CrouchToStand_18_474150()
{
    CheckFloorGone_472320();
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::M_WalkToRun_19_473600()
{
    Event_Broadcast(kEventNoise, this);
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_RunLoop_21_473720;
    }

    if (WallHit((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        MoveOnLine_4720D0();
    }
}

void Mudokon::M_MidWalkToRun_20_4736D0()
{
    M_WalkToRun_19_473600();

    if (mCurrentMotion == eMudMotions::M_RunLoop_21_473720)
    {
        mCurrentMotion = eMudMotions::M_MidWalkToRun_20_4736D0;
        mPreviousMotion = eMudMotions::M_RunLoop_21_473720;
        mBaseAliveGameObjectLastAnimFrame = 8;
        field_192_return_to_previous_motion = 1;
    }
}

void Mudokon::M_RunLoop_21_473720()
{
    Event_Broadcast(kEventNoise, this);

    if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
    {
        Event_Broadcast(kEventSuspiciousNoise, this);
    }

    if (WallHit(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        ToKnockback_473190();
        return;
    }

    MoveOnLine_4720D0();

    if (mCurrentMotion == eMudMotions::M_RunLoop_21_473720)
    {
        if (field_20_animation.field_92_current_frame == 0 || field_20_animation.field_92_current_frame == 8)
        {
            MapFollowMe(TRUE);

            if (mNextMotion == eMudMotions::M_RunJumpMid_36_474570)
            {
                mCurrentMotion = eMudMotions::M_RunJumpBegin_35_474460;
                mNextMotion = -1;
            }
        }
        else if (field_20_animation.field_92_current_frame == 4 || field_20_animation.field_92_current_frame == 12)
        {
            Environment_SFX_457A40(EnvironmentSfx::eRunningFootstep_2, 0, 32767, this);
            MapFollowMe(TRUE);

            switch (mNextMotion)
            {
                case eMudMotions::M_WalkLoop_1_4728B0:
                    mNextMotion = -1;
                    if (field_20_animation.field_92_current_frame == 4)
                    {
                        mCurrentMotion = eMudMotions::M_MidRunToWalk_23_4739B0;
                    }
                    else
                    {
                        mCurrentMotion = eMudMotions::M_RunToWalk_22_4738E0;
                    }
                    return;

                case eMudMotions::M_Idle_0_4724E0:
                    mCurrentMotion = eMudMotions::M_RunSlideStop_24_473A00;
                    mNextMotion = -1;
                    Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
                    return;

                case eMudMotions::M_RunSlideTurn_25_473AA0:
                    mCurrentMotion = eMudMotions::M_RunSlideTurn_25_473AA0;
                    mNextMotion = -1;
                    Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
                    return;

                case eMudMotions::M_RunJumpMid_36_474570:
                    mCurrentMotion = eMudMotions::M_RunJumpBegin_35_474460;
                    mNextMotion = -1;
                    break;
            }
        }
    }
}

void Mudokon::M_RunToWalk_22_4738E0()
{
    Event_Broadcast(kEventNoise, this);
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_WalkLoop_1_4728B0;
    }

    if (WallHit((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        MoveOnLine_4720D0();
    }
}

void Mudokon::M_MidRunToWalk_23_4739B0()
{
    M_RunToWalk_22_4738E0();

    if (mCurrentMotion == eMudMotions::M_WalkLoop_1_4728B0)
    {
        mCurrentMotion = eMudMotions::M_MidRunToWalk_23_4739B0;
        mPreviousMotion = eMudMotions::M_WalkLoop_1_4728B0;
        mBaseAliveGameObjectLastAnimFrame = 9;
        field_192_return_to_previous_motion = 1;
    }
}

void Mudokon::M_RunSlideStop_24_473A00()
{
    Event_Broadcast(kEventNoise, this);
    if (WallHit((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        ToKnockback_473190();
    }
    else
    {
        ReduceXVelocityBy_472260(FP_FromDouble(0.375));

        if (mCurrentMotion == eMudMotions::M_RunSlideStop_24_473A00)
        {
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);
                MapFollowMe(TRUE);
                Mudokon::ToStand_4724A0();
            }
        }
    }
}

void Mudokon::M_RunSlideTurn_25_473AA0()
{
    Event_Broadcast(kEventNoise, this);

    if (WallHit((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        ToKnockback_473190();
    }
    else
    {
        ReduceXVelocityBy_472260(FP_FromDouble(0.375));

        if (mCurrentMotion == eMudMotions::M_RunSlideTurn_25_473AA0)
        {
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe(TRUE);
                if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
                    mCurrentMotion = eMudMotions::M_RunTurnToRun_26_473BB0;
                }
                else
                {
                    mCurrentMotion = eMudMotions::M_RunTurnToRun_26_473BB0;
                    field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
                }
            }
        }
    }
}

void Mudokon::M_RunTurnToRun_26_473BB0()
{
    Event_Broadcast(kEventNoise, this);

    if (WallHit((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        MoveOnLine_4720D0();

        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = eMudMotions::M_RunLoop_21_473720;
            field_20_animation.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        }
    }
}

void Mudokon::M_SneakLoop_27_473C40()
{
    if (WallHit((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        ToKnockback_473190();
    }
    else
    {
        MoveOnLine_4720D0();
        if (mCurrentMotion == eMudMotions::M_SneakLoop_27_473C40)
        {
            if (field_20_animation.field_92_current_frame == 3 || field_20_animation.field_92_current_frame == 13)
            {
                if (mNextMotion == eMudMotions::M_Idle_0_4724E0)
                {
                    mNextMotion = -1;
                    mCurrentMotion = (field_20_animation.field_92_current_frame != 3) ? eMudMotions::M_SneakToIdle_33_473FF0 : eMudMotions::M_MidSneakToIdle_34_474020;
                }
            }
            else if (field_20_animation.field_92_current_frame == 6 || field_20_animation.field_92_current_frame == 16)
            {
                Environment_SFX_457A40(EnvironmentSfx::eSneakFootstep_3, 0, 32767, this);
                MapFollowMe(TRUE);

                if (mNextMotion == eMudMotions::M_WalkLoop_1_4728B0)
                {
                    mNextMotion = -1;
                    mCurrentMotion = field_20_animation.field_92_current_frame != 6 ? eMudMotions::M_MidSneakToWalk_31_473F30 : eMudMotions::M_SneakToWalk_29_473E20;
                }
            }
        }
    }
}

void Mudokon::M_MidWalkToSneak_28_473D60()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(10));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(10));
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_SneakLoop_27_473C40;
    }

    if (WallHit((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        MoveOnLine_4720D0();
    }
}

void Mudokon::M_SneakToWalk_29_473E20()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_WalkLoop_1_4728B0;
    }

    if (WallHit((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        MoveOnLine_4720D0();
    }
}

void Mudokon::M_WalkToSneak_30_473EE0()
{
    M_MidWalkToSneak_28_473D60();

    if (mCurrentMotion == eMudMotions::M_SneakLoop_27_473C40)
    {
        mCurrentMotion = eMudMotions::M_WalkToSneak_30_473EE0;
        mPreviousMotion = eMudMotions::M_SneakLoop_27_473C40;
        mBaseAliveGameObjectLastAnimFrame = 10;
        field_192_return_to_previous_motion = 1;
    }
}

void Mudokon::M_MidSneakToWalk_31_473F30()
{
    M_SneakToWalk_29_473E20();

    if (mCurrentMotion == eMudMotions::M_WalkLoop_1_4728B0)
    {
        mCurrentMotion = eMudMotions::M_MidSneakToWalk_31_473F30;
        mPreviousMotion = eMudMotions::M_WalkLoop_1_4728B0;
        mBaseAliveGameObjectLastAnimFrame = 9;
        field_192_return_to_previous_motion = 1;
    }
}

void Mudokon::M_SneakBegin_32_473F80()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_SneakLoop_27_473C40;
    }

    if (WallHit((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        MoveOnLine_4720D0();
    }
}

void Mudokon::M_SneakToIdle_33_473FF0()
{
    MoveOnLine_4720D0();

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe(TRUE);
        ToStand_4724A0();
    }
}

void Mudokon::M_MidSneakToIdle_34_474020()
{
    M_SneakToIdle_33_473FF0();
}

void Mudokon::M_RunJumpBegin_35_474460()
{
    Event_Broadcast(kEventNoise, this);

    mBaseAnimatedWithPhysicsGameObject_XPos += field_C4_velx;

    if (!field_20_animation.field_92_current_frame)
    {
        Environment_SFX_457A40(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 32767, this);
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = (field_CC_sprite_scale * -FP_FromDouble(7.6)); // TODO: Correct FP value?
        }
        else
        {
            field_C4_velx = (field_CC_sprite_scale * FP_FromDouble(7.6)); // TODO: Correct FP value?
        }
        field_C8_vely = (field_CC_sprite_scale * -FP_FromDouble(9.6));
        mBaseAnimatedWithPhysicsGameObject_YPos += field_C8_vely;
        VOnTrapDoorOpen();
        mCurrentMotion = eMudMotions::M_RunJumpMid_36_474570;
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Mudokon::M_RunJumpMid_36_474570()
{
    Event_Broadcast(kEventNoise, this);

    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds.Find_Impl(field_11C_bird_portal_id));
    if (field_20_animation.field_92_current_frame == 5)
    {
        SFX_Play_Pitch(SoundEffect::PossessEffect_17, 40, 2400);
    }

    PSX_RECT rect = {};
    VGetBoundingRect(&rect, 1);

    if ((field_C4_velx > FP_FromInteger(0) && FP_FromInteger(rect.x) > pBirdPortal->field_2C_xpos) || (field_C4_velx < FP_FromInteger(0) && FP_FromInteger(rect.w) < pBirdPortal->field_2C_xpos))
    {
        field_16A_flags.Clear(Flags_16A::eBit1_not_rescued);
        field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);

        mBaseGameObjectFlags.Set(BaseGameObject::eDead);

        field_C8_vely = FP_FromInteger(0);
        field_C4_velx = FP_FromInteger(0);

        SND_SEQ_Play(SeqId::SaveTriggerMusic_31, 1, 127, 127);

        relive_new MusicTrigger(MusicTriggerMusicType::eChime_5, TriggeredBy::eTimer_0, 0, 0);

        sRescuedMudokons_5C1BC2++;

        if (sZulagNumber_5C1A20 < ALIVE_COUNTOF(sSavedKilledMudsPerZulag_5C1B50.mData))
        {
            sSavedKilledMudsPerZulag_5C1B50.mData[sZulagNumber_5C1A20]++;
        }

        if (pBirdPortal)
        {
            pBirdPortal->VMudSaved();
        }

        if (field_17A_rescue_switch_id)
        {
            SwitchStates_Set(field_17A_rescue_switch_id, 1);
        }
    }

    field_C8_vely += (field_CC_sprite_scale * FP_FromDouble(1.8));
    mBaseAnimatedWithPhysicsGameObject_XPos += field_C4_velx;
    mBaseAnimatedWithPhysicsGameObject_YPos += field_C8_vely;
}

void Mudokon::M_StandToRun_37_4749A0()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_RunLoop_21_473720;
    }

    Event_Broadcast(kEventNoise, this);

    if (WallHit((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        MoveOnLine_4720D0();
    }
}

void Mudokon::M_Punch_38_474AA0()
{
    SetPal(field_180_emo_tbl);

    if (field_20_animation.field_92_current_frame == 5)
    {
        if (field_180_emo_tbl == Mud_Emotion::eAngry_1 || field_180_emo_tbl == Mud_Emotion::eAggressive_2)
        {
            BaseGameObject* pSlapTarget = nullptr;
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                pSlapTarget = FindObjectOfType(ReliveTypes::eAbe, mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5));
            }
            else
            {
                pSlapTarget = FindObjectOfType(ReliveTypes::eAbe, mBaseAnimatedWithPhysicsGameObject_XPos + ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5));
            }

            if (!pSlapTarget)
            {
                if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    pSlapTarget = FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5));
                }
                else
                {
                    pSlapTarget = FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos + ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5));
                }
            }

            if (pSlapTarget)
            {
                static_cast<BaseAliveGameObject*>(pSlapTarget)->VTakeDamage(this);
            }
        }
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetPal(Mud_Emotion::eNormal_0);
        ToStand_4724A0();
    }
}

void Mudokon::M_HoistBegin_39_4748E0()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        field_C8_vely = (field_CC_sprite_scale * -FP_FromInteger(8));
        mBaseAnimatedWithPhysicsGameObject_YPos += field_C8_vely;
        VOnTrapDoorOpen();
        mCurrentMotion = eMudMotions::M_HoistLand_40_474780;
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Mudokon::M_HoistLand_40_474780()
{
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        PSX_RECT bRect = {};
        switch (pLine->field_8_type) // TODO: Strongly type
        {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_NoFractional(hitY + FP_FromDouble(0.5));
                MapFollowMe(TRUE);
                BaseAliveGameObjectCollisionLine = pLine;
                mCurrentMotion = eMudMotions::M_LandSoft1_41_474960;
                VGetBoundingRect(&bRect, 1);
                VOnCollisionWith(
                    {bRect.x, static_cast<s16>(bRect.y + 5)},
                    {bRect.w, static_cast<s16>(bRect.h + 5)},
                    ObjList_5C1B78,
                    1,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
                break;
            default:
                return;
        }
    }
}

void Mudokon::M_LandSoft1_41_474960()
{
    if (field_20_animation.field_92_current_frame == 2)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::M_LandSoft2_42_4743F0()
{
    CheckFloorGone_472320();

    if (field_20_animation.field_92_current_frame == 2)
    {
        if (mPreviousMotion == eMudMotions::M_Fall_49_472C60)
        {
            Environment_SFX_457A40(EnvironmentSfx::eLandingSoft_5, 0, 32767, this);
        }
        else
        {
            Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);
        }
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::M_DunnoBegin_43_472790()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_DunnoEnd_44_4727B0;
    }
}

void Mudokon::M_DunnoEnd_44_4727B0()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::M_KnockForward_45_474180()
{
    if (field_20_animation.field_92_current_frame == 12)
    {
        CheckKnockedOntoABomb_4723C0();
    }

    if (field_20_animation.field_92_current_frame == 4)
    {
        Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 32767, this);
    }

    if ((gMap.mCurrentLevel == EReliveLevelIds::eMines
        || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz
        || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot
        || gMap.mCurrentLevel == EReliveLevelIds::eBarracks
        || gMap.mCurrentLevel == EReliveLevelIds::eBrewery)
        && field_20_animation.field_92_current_frame == 7)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (mNextMotion != -1 && mHealth > FP_FromInteger(0))
    {
        mCurrentMotion = mNextMotion;
        mNextMotion = -1;
    }
}

void Mudokon::M_Knockback_46_4742A0()
{
    if (field_20_animation.field_92_current_frame == 12)
    {
        CheckKnockedOntoABomb_4723C0();
    }

    Event_Broadcast(kEventNoise, this);

    if ((gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBrewery) && field_20_animation.field_92_current_frame == 7)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (!BaseAliveGameObjectCollisionLine)
    {
        M_Fall_49_472C60();

        if (mCurrentMotion == eMudMotions::M_KnockForward_45_474180 || mCurrentMotion == eMudMotions::M_LandSoft2_42_4743F0)
        {
            mCurrentMotion = eMudMotions::M_Knockback_46_4742A0;
        }
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            mCurrentMotion = eMudMotions::M_KnockbackGetUp_47_474380;
        }
    }
}

void Mudokon::M_KnockbackGetUp_47_474380()
{
    CheckFloorGone_472320();

    Event_Broadcast(kEventNoise, this);
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::M_WalkOffEdge_48_4743C0()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_Fall_49_472C60;
    }

    M_Fall_49_472C60();
}

void Mudokon::M_Fall_49_472C60()
{
    if (field_C4_velx > FP_FromInteger(0))
    {
        field_C4_velx = field_C4_velx - (field_CC_sprite_scale * field_134_xVelSlowBy);
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        field_C4_velx = (field_CC_sprite_scale * field_134_xVelSlowBy) + field_C4_velx;
        if (field_C4_velx > FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        switch (pLine->field_8_type) // TODO: Strongly type line types
        {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                BaseAliveGameObjectCollisionLine = pLine;

                MapFollowMe(TRUE);

                if ((sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                         FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                         FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                         FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                         FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                         TlvTypes::SoftLanding_75)
                     && mHealth > FP_FromInteger(0))
                    || (mBaseAnimatedWithPhysicsGameObject_YPos - BaseAliveGameObjectLastLineYPos < (field_CC_sprite_scale * FP_FromInteger(180)) && (mHealth > FP_FromInteger(0) || gAbeBulletProof_5C1BDA)))
                {
                    mCurrentMotion = eMudMotions::M_LandSoft2_42_4743F0;
                }
                else
                {
                    mHealth = FP_FromInteger(0);
                    ToKnockback_473190();
                    field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);
                    field_18E_brain_state = Mud_Brain_State::Brain_5_ShrivelDeath_4714A0;
                    mCurrentMotion = eMudMotions::M_KnockForward_45_474180;
                    field_194_timer = sGnFrame + 90;
                }

                mPreviousMotion = eMudMotions::M_Fall_49_472C60;

                VOnCollisionWith(
                    {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(10)), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(10))},
                    {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(10)), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(10))},
                    ObjList_5C1B78,
                    1,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
                break;

            case 1u:
            case 2u:
            case 5u:
            case 6u:
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                ToKnockback_473190();
                break;
        }
    }
}

void Mudokon::M_Chant_50_473040()
{
    if (!(static_cast<s32>(sGnFrame) % 4))
    {
        New_RandomizedChant_Particle(this);
    }

    if (!SND_SsIsEos_DeInlined(11u))
    {
        SND_SEQ_Play(SeqId::MudokonChant_11, 1, 50, 50);
    }

    if (mNextMotion == eMudMotions::M_Idle_0_4724E0)
    {
        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SND_SEQ_Stop(SeqId::MudokonChant_11);
            mCurrentMotion = eMudMotions::M_ChantEnd_51_4730D0;
            mNextMotion = -1;
        }
    }
}

void Mudokon::M_ChantEnd_51_4730D0()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::M_ToDuck_52_474A20()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_Duck_53_474A40;
    }
}

void Mudokon::M_Duck_53_474A40()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mNextMotion == eMudMotions::M_CrouchIdle_15_474040)
        {
            mNextMotion = -1;
            mCurrentMotion = eMudMotions::M_DuckToCrouch_54_474A80;
        }
    }
}

void Mudokon::M_DuckToCrouch_54_474A80()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_CrouchIdle_15_474040;
    }
}

void Mudokon::M_DuckKnockback_55_474250()
{
    if (field_20_animation.field_92_current_frame == 1)
    {
        Environment_SFX_457A40(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::M_Duck_53_474A40;
        mNextMotion = -1;
    }
}

void Mudokon::M_SlapOwnHead_56_4727D0()
{
    if (field_20_animation.field_92_current_frame == 5)
    {
        HurtSound_475DB0();
        mHealth -= FP_FromDouble(0.033);
    }

    if (mHealth > FP_FromInteger(0))
    {
        if (mNextMotion == eMudMotions::M_Idle_0_4724E0 || mNextMotion == eMudMotions::M_Speak_6_472FA0)
        {
            ToStand_4724A0();
        }
    }
    else
    {
        Mudokon_SFX(MudSounds::eHurt2_9, 0, 1000, this);
        Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
        Event_Broadcast(kEventMudokonDied, this);
        field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);
        mHealth = FP_FromInteger(0);
        field_18E_brain_state = Mud_Brain_State::Brain_5_ShrivelDeath_4714A0;
        field_194_timer = sGnFrame + 90;
        mCurrentMotion = eMudMotions::M_KnockForward_45_474180;
        mNextMotion = -1;
    }
}

void Mudokon::M_TurnWheelBegin_57_474C00()
{
    if (field_DA_xOffset > 0)
    {
        field_DA_xOffset--;
    }
    else if (field_DA_xOffset < 0)
    {
        field_DA_xOffset++;
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        auto pWheel = static_cast<WorkWheel*>(FindObjectOfType(ReliveTypes::eWheel, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - (field_CC_sprite_scale * FP_FromInteger(50))));
        if (pWheel)
        {
            pWheel->VStartTurning();
            field_158_wheel_id = pWheel->field_8_object_id;
        }
        mCurrentMotion = eMudMotions::M_TurnWheelLoop_58_474CC0;
    }
}

void Mudokon::M_TurnWheelLoop_58_474CC0()
{
    if (mNextMotion == eMudMotions::M_Idle_0_4724E0)
    {
        auto pWheel = static_cast<WorkWheel*>(sObjectIds.Find_Impl(field_158_wheel_id));
        if (pWheel)
        {
            pWheel->VStopTurning(TRUE);
            field_158_wheel_id = -1;
        }
        mCurrentMotion = eMudMotions::M_TurnWheelEnd_59_474D30;
        mNextMotion = -1;
    }
}

void Mudokon::M_TurnWheelEnd_59_474D30()
{
    if (field_DA_xOffset > field_162_maxXOffset)
    {
        field_DA_xOffset--;
    }
    else if (field_DA_xOffset < field_162_maxXOffset)
    {
        field_DA_xOffset++;
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

s16 Mudokon::StableDelay_477570()
{
    s32 maxIdx = sAlertedMudCount_5C3010;
    if (sAlertedMudCount_5C3010 >= ALIVE_COUNTOF(kDelayTable_55CF7C))
    {
        maxIdx = ALIVE_COUNTOF(kDelayTable_55CF7C);
    }

    if (sDelayIdx_5C3014 >= maxIdx)
    {
        sDelayIdx_5C3014 = 0;
    }
    return kDelayTable_55CF7C[sDelayIdx_5C3014++];
}

s16 Mudokon::CheckForPortal_4775E0()
{
    if (sObjectIds.Find_Impl(field_11C_bird_portal_id))
    {
        return 0;
    }

    // TODO: Refactor duplication

    auto pOpenPortal = static_cast<BirdPortal*>(Event_Get(kEventPortalOpen));
    if (pOpenPortal)
    {
        const FP xDist = pOpenPortal->field_2C_xpos - mBaseAnimatedWithPhysicsGameObject_XPos;
        if (FP_Abs(xDist) < FP_FromInteger(368))
        {
            if (FP_Abs(pOpenPortal->field_3C_YPos - mBaseAnimatedWithPhysicsGameObject_YPos) < FP_FromInteger(10))
            {
                if (!WallHit(field_CC_sprite_scale * FP_FromInteger(50), xDist))
                {
                    if (pOpenPortal->field_24_portal_type == PortalType::eWorker_1 || pOpenPortal->field_24_portal_type == PortalType::eShrykull_2)
                    {
                        sActiveHero->ChangeChantState_45BB90(1);
                        field_11C_bird_portal_id = pOpenPortal->field_8_object_id;
                        sGoingToBirdPortalMudCount_5C3012++;
                        field_16C_flags.Set(Flags_16C::eBit3_Unknown);
                        return 1;
                    }
                }
            }
        }
    }

    auto pPortal20 = static_cast<BirdPortal*>(Event_Get(kEventOtherPortalOpen));
    if (pPortal20)
    {
        const FP xDist = pPortal20->field_2C_xpos - mBaseAnimatedWithPhysicsGameObject_XPos;
        if (FP_Abs(xDist) < FP_FromInteger(368))
        {
            if (FP_Abs(pPortal20->field_3C_YPos - mBaseAnimatedWithPhysicsGameObject_YPos) < FP_FromInteger(10))
            {
                if (!WallHit(field_CC_sprite_scale * FP_FromInteger(50), xDist))
                {
                    if (pPortal20->field_24_portal_type == PortalType::eWorker_1 || pPortal20->field_24_portal_type == PortalType::eShrykull_2)
                    {
                        sActiveHero->ChangeChantState_45BB90(1);
                        field_11C_bird_portal_id = pPortal20->field_8_object_id;

                        sGoingToBirdPortalMudCount_5C3012++;
                        field_16C_flags.Set(Flags_16C::eBit3_Unknown);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

GameSpeakEvents Mudokon::LastGameSpeak_476FF0()
{
    GameSpeakEvents actualEvent = GameSpeakEvents::eNone_m1;

    const s32 lastEventIdx = pEventSystem_5BC11C->field_28_last_event_index;
    if (field_140_last_event_index == lastEventIdx)
    {
        if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eNone_m1)
        {
            actualEvent = GameSpeakEvents::eNone_m1;
        }
        else
        {
            actualEvent = GameSpeakEvents::eSameAsLast_m2;
        }
    }
    else
    {
        field_140_last_event_index = lastEventIdx;
        actualEvent = pEventSystem_5BC11C->field_20_last_event;
    }

    // Not valid if not in the same camera
    if (Is_In_Current_Camera() != CameraPos::eCamCurrent_0)
    {
        return GameSpeakEvents::eNone_m1;
    }

    // Look out works on any scale
    if (actualEvent == GameSpeakEvents::Slig_LookOut_6)
    {
        return GameSpeakEvents::Slig_LookOut_6;
    }

    // Check in valid range and on same scale
    if (actualEvent < GameSpeakEvents::eUnknown_1 || actualEvent > GameSpeakEvents::eSorry_24 || sActiveHero->field_CC_sprite_scale == field_CC_sprite_scale)
    {
        return actualEvent;
    }

    return GameSpeakEvents::eNone_m1;
}

s16 Mudokon::LaughingGasInCurrentScreen_4774A0()
{
    return gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) != 0 ? gLaughingGasOn_5C1BA4 : 0;
}

void Mudokon::Sound_475EC0(MudSounds idx)
{
    if (!gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        if (field_180_emo_tbl != Mud_Emotion::eSick_7)
        {
            // TODO: Pass down the stronger types - assuming they map directly
            Mudokon_SFX(idx, 80, field_13C_voice_pitch, this);
            return;
        }

        if (idx != MudSounds::eSick_24)
        {
            Mudokon_SFX(idx, 80, -field_13C_voice_pitch, this);
            return;
        }

        s16 pitch1 = Math_RandomRange(-5, 0);
        pitch1 *= 100;
        pitch1 = pitch1 - field_13C_voice_pitch;
        Mudokon_SFX(MudSounds::eSick_24, Math_RandomRange(20, 50), pitch1, this);

        s16 pitch2 = Math_RandomRange(-5, 0);
        pitch2 *= 100;
        pitch2 = pitch2 - field_13C_voice_pitch;
        Mudokon_SFX(MudSounds::eSadUgh_28, Math_RandomRange(20, 60), pitch2, this);
        return;
    }

    if (field_180_emo_tbl == Mud_Emotion::eSick_7)
    {
        if (idx == MudSounds::eSick_24)
        {
            s16 pitch1 = Math_RandomRange(-5, 0);
            pitch1 *= 100;
            pitch1 = pitch1 - field_13C_voice_pitch;
            Mudokon_SFX(MudSounds::eSick_24, Math_RandomRange(55, 90), pitch1, this);

            s16 pitch2 = Math_RandomRange(-5, 0);
            pitch2 *= 100;
            pitch2 = pitch2 - field_13C_voice_pitch;
            Mudokon_SFX(MudSounds::eSadUgh_28, Math_RandomRange(60, 110), pitch2, this);
            return;
        }
        Mudokon_SFX(idx, 0, field_13C_voice_pitch, this);
    }
    else
    {
        Mudokon_SFX(idx, 0, field_13C_voice_pitch, this);
    }
}

s16 Mudokon::CanRespond_4770B0()
{
    const s32 myDistToPlayer = Math_Distance(
        FP_GetExponent(sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos));

    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Is there another object that isn't us on the same scale?
        if (pObj != this && pObj->field_CC_sprite_scale == sActiveHero->field_CC_sprite_scale)
        {
            // Is it a mud who isn't currently talking to abe and is in the same screen?
            if ((pObj->Type() == ReliveTypes::eRingOrLiftMud || pObj->Type() == ReliveTypes::eMudokon) && static_cast<Mudokon*>(pObj)->field_18E_brain_state != Mud_Brain_State::Brain_4_ListeningToAbe_477B40 && gMap.Is_Point_In_Current_Camera_4810D0(pObj->field_C2_lvl_number, pObj->field_C0_path_number, pObj->mBaseAnimatedWithPhysicsGameObject_XPos, pObj->mBaseAnimatedWithPhysicsGameObject_YPos, 0))
            {
                if (sActiveHero->VIsFacingMe(pObj) && !sActiveHero->VIsFacingMe(this))
                {
                    // Abe is facing the other guy and not us :(
                    return FALSE;
                }

                const s32 otherMudDistanceToPlayer = Math_Distance(
                    FP_GetExponent(sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos),
                    FP_GetExponent(sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos),
                    FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_XPos),
                    FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_YPos));

                if (myDistToPlayer > otherMudDistanceToPlayer)
                {
                    if (sActiveHero->VIsFacingMe(pObj) == sActiveHero->VIsFacingMe(this))
                    {
                        return FALSE;
                    }
                }
            }
        }
    }
    return TRUE;
}

u8** Mudokon::AnimBlockForMotion_474DC0(s16 motion)
{
    if (motion < eMudMotions::M_LeverUse_10_473020)
    {
        return field_10_resources_array.ItemAt(0);
    }
    else if (motion < eMudMotions::M_Chisel_11_4732D0)
    {
        if (!field_10_resources_array.ItemAt(1))
        {
            field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbepullResID, TRUE, FALSE));
        }
        return field_10_resources_array.ItemAt(1);
    }
    else if (motion < eMudMotions::M_CrouchScrub_14_473560)
    {
        return field_10_resources_array.ItemAt(2);
    }
    else if (motion < eMudMotions::M_CrouchIdle_15_474040)
    {
        return field_10_resources_array.ItemAt(3);
    }
    else if (motion < eMudMotions::M_KnockForward_45_474180)
    {
        return field_10_resources_array.ItemAt(4);
    }
    else if (motion < eMudMotions::M_Knockback_46_4742A0)
    {
        return field_10_resources_array.ItemAt(5);
    }
    else if (motion < eMudMotions::M_WalkOffEdge_48_4743C0)
    {
        return field_10_resources_array.ItemAt(6);
    }
    else if (motion < eMudMotions::M_Chant_50_473040)
    {
        return field_10_resources_array.ItemAt(7);
    }
    else if (motion < eMudMotions::M_ToDuck_52_474A20)
    {
        return field_10_resources_array.ItemAt(8);
    }
    else if (motion < eMudMotions::M_DuckKnockback_55_474250)
    {
        return field_10_resources_array.ItemAt(9);
    }
    else if (motion < eMudMotions::M_SlapOwnHead_56_4727D0)
    {
        return field_10_resources_array.ItemAt(10);
    }
    else if (motion < eMudMotions::M_TurnWheelBegin_57_474C00)
    {
        return field_10_resources_array.ItemAt(11);
    }
    else if (motion < 60) // MAX ??
    {
        if (!field_10_resources_array.ItemAt(12))
        {
            field_10_resources_array.SetAt(12, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, TRUE, FALSE));
        }
        return field_10_resources_array.ItemAt(12);
    }
    else
    {
        LOG_ERROR("Out of bounds ??");
        return nullptr;
    }
}

void Mudokon::CheckFloorGone_472320()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        VOnTrapDoorOpen();
        mCurrentMotion = eMudMotions::M_Fall_49_472C60;
    }
}

void Mudokon::ToStand_4724A0()
{
    field_134_xVelSlowBy = FP_FromInteger(0);
    field_138_unused = 0;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    mCurrentMotion = eMudMotions::M_Idle_0_4724E0;
}

s16 Mudokon::FindWheel_4777B0(FP xpos, FP ypos)
{
    auto* pWheelTlv = static_cast<Path_WorkWheel*>(
        sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(xpos), FP_GetExponent(ypos), FP_GetExponent(xpos), FP_GetExponent(ypos), TlvTypes::WorkWheel_79));

    if (pWheelTlv)
    {
        if (!SwitchStates_Get(pWheelTlv->field_12_switch_id))
        {
            return FindObjectOfType(ReliveTypes::eWheel, xpos, ypos - (field_CC_sprite_scale * FP_FromInteger(50))) != 0;
        }
    }
    return 0;
}

s16 Mudokon::GetBrainSubStateResponse_477AF0(MudAction action)
{
    field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, action);
    if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags_16A::eBit11_get_depressed))
    {
        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
    }
    return field_188_pTblEntry->field_6_sub_state;
}

s16 Mudokon::StopAtWheel_477880()
{
    FP offset = {};
    FP_Rect fpRect = {};

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        offset = -ScaleToGridSize(field_CC_sprite_scale) * FP_FromDouble(0.4);
        fpRect.x = mBaseAnimatedWithPhysicsGameObject_XPos + (offset * FP_FromInteger(2));
        fpRect.w = mBaseAnimatedWithPhysicsGameObject_XPos;
    }
    else
    {
        offset = ScaleToGridSize(field_CC_sprite_scale) * FP_FromDouble(0.4);
        fpRect.x = mBaseAnimatedWithPhysicsGameObject_XPos;
        fpRect.w = mBaseAnimatedWithPhysicsGameObject_XPos + (offset * FP_FromInteger(2));
    }

    fpRect.y = mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5);
    fpRect.h = mBaseAnimatedWithPhysicsGameObject_YPos;

    if (!FindWheel_4777B0(mBaseAnimatedWithPhysicsGameObject_XPos + offset, mBaseAnimatedWithPhysicsGameObject_YPos))
    {
        // No wheel so don't stop
        return 0;
    }

    const PSX_RECT ourRect = {
        FP_GetExponent(fpRect.x),
        FP_GetExponent(fpRect.y),
        FP_GetExponent(fpRect.w),
        FP_GetExponent(fpRect.h)};


    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Found another mud who isn't us
        if (pObj != this && pObj->Type() == ReliveTypes::eMudokon)
        {
            PSX_RECT bRect = {};
            pObj->VGetBoundingRect(&bRect, 1);

            auto pOtherMud = static_cast<Mudokon*>(pObj);

            // Check if the other mud has already take the spot of this work wheel
            if (bRect.x <= ourRect.w && bRect.w >= ourRect.x && bRect.h >= ourRect.y && bRect.y <= ourRect.h)
            {
                if (pOtherMud->field_16A_flags.Get(Flags_16A::eBit7_stopped_at_wheel) || (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_YPos), FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_YPos), TlvTypes::WorkWheel_79) && pOtherMud->field_C4_velx == FP_FromInteger(0)))
                {
                    // Another mud has stolen this wheel so don't stop
                    return 0;
                }
            }
        }
    }
    return 1;
}

void Mudokon::ToKnockback_473190()
{
    auto pWheel = static_cast<WorkWheel*>(sObjectIds.Find_Impl(field_158_wheel_id));
    if (pWheel)
    {
        pWheel->VStopTurning(TRUE);
        field_158_wheel_id = -1;
    }

    Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 32767, this);

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mBaseAnimatedWithPhysicsGameObject_XPos += (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2)) + field_C4_velx;
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_XPos -= (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2)) - field_C4_velx;
        }
    }

    MapFollowMe(TRUE);

    field_C4_velx = FP_FromInteger(0);

    if (field_C8_vely < FP_FromInteger(0))
    {
        field_C8_vely = FP_FromInteger(0);
    }

    Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 32767, this);
    mCurrentMotion = eMudMotions::M_Knockback_46_4742A0;
}

// TODO: Abe sneaking or walking check ??
s16 Mudokon::IsMotionUnknown_4730F0()
{
    return sActiveHero->mCurrentMotion == eAbeMotions::Motion_40_SneakLoop_450550 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_45_SneakBegin_4507A0 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_41_WalkToSneak_450250 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_43_MidWalkToSneak_450380 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_42_SneakToWalk_4503D0 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_44_MidSneakToWalk_450500;
}

void Mudokon::MoveOnLine_4720D0()
{
    PlatformBase* pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));

    CheckFloorGone_472320();

    const FP oldXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, field_C4_velx);
    }

    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObjectId = -1;
    }

    if (BaseAliveGameObjectCollisionLine)
    {
        if (BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_32 || BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_36)
        {
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);

            VOnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {bRect.w, static_cast<s16>(bRect.h + 5)},
                ObjList_5C1B78, 1, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
        }
    }
    else
    {
        field_134_xVelSlowBy = FP_FromDouble(0.3); // TODO: or 2.99 ??
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        mCurrentMotion = eMudMotions::M_WalkOffEdge_48_4743C0;
        mBaseAnimatedWithPhysicsGameObject_XPos = oldXPos + field_C4_velx;
    }
}

const struct MudEmotionTableEntry* Mudokon::ResponseTo_471730(Mud_Emotion emotion, MudAction action)
{
    return &kMudEmoTable_55C790.field_0_data[static_cast<s32>(emotion)][static_cast<s32>(action)];
}

void Mudokon::AddAlerted()
{
    if (!field_16A_flags.Get(Flags_16A::eBit3_alerted))
    {
        field_16A_flags.Set(Flags_16A::eBit3_alerted);
        sAlertedMudCount_5C3010++;
        LOG_INFO("Alert added " << sAlertedMudCount_5C3010);
    }
}

void Mudokon::RemoveAlerted()
{
    if (field_16A_flags.Get(Flags_16A::eBit3_alerted))
    {
        sAlertedMudCount_5C3010--;
        field_16A_flags.Clear(Flags_16A::eBit3_alerted);
        LOG_INFO("Alert removed " << sAlertedMudCount_5C3010);
    }
}

s16 Mudokon::BrainStartWheelTurning()
{
    mNextMotion = eMudMotions::M_TurnWheelBegin_57_474C00;
    RemoveAlerted();
    field_16A_flags.Set(Flags_16A::eBit7_stopped_at_wheel);
    field_18E_brain_state = Mud_Brain_State::Brain_3_TurnWheel_47E0D0;
    return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
}

bool Mudokon::NeedToTurnAround()
{
    Mudokon* pAboveMud = static_cast<Mudokon*>(FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)));
    if (sAlertedMudCount_5C3010 > 1 && pAboveMud && !VIsFacingMe(pAboveMud) && pAboveMud->mCurrentMotion != eMudMotions::M_StandingTurn_2_472BF0)
    {
        // A mud is in the same grid us and not turning, so we turn to face it. This causes "stacked" muds to all face the same way bar 1.
        // When telling them all to "wait" they will face Abe and then spin around again :D.
        return true;
    }
    else if (!VIsFacingMe(sActiveHero) && (sAlertedMudCount_5C3010 <= 1 || !FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5))))
    {
        // We are not facing Abe and there isn't a mud in the same grid and we are the only mud following Abe
        // so stop being so damn rude and face him.
        return true;
    }
    else
    {
        return false;
    }
}

void Mudokon::ReduceXVelocityBy_472260(FP velXScaleBy)
{
    if (field_C4_velx > FP_FromInteger(0))
    {
        field_C4_velx = field_C4_velx - (field_CC_sprite_scale * velXScaleBy);
        if (field_C4_velx >= FP_FromInteger(0))
        {
            if (FP_GetExponent(field_C4_velx))
            {
                MoveOnLine_4720D0();
            }
        }
        else
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        field_C4_velx = (field_CC_sprite_scale * velXScaleBy) + field_C4_velx;
        if (field_C4_velx <= FP_FromInteger(0))
        {
            if (FP_GetExponent(field_C4_velx))
            {
                MoveOnLine_4720D0();
            }
        }
    }
    else
    {
        field_C4_velx = FP_FromInteger(0);
    }
}

void Mudokon::CheckKnockedOntoABomb_4723C0()
{
    PSX_RECT bMudRect = {};
    VGetBoundingRect(&bMudRect, 1);

    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eMine || pObj->Type() == ReliveTypes::eUXB)
        {
            PSX_RECT bBombRect = {};
            pObj->VGetBoundingRect(&bBombRect, 1);

            // TODO: Almost same as PSX_Rects_overlap_no_adjustment but checks < rather than <=
            if (bMudRect.x <= bBombRect.w && bMudRect.w >= bBombRect.x && bMudRect.h >= bBombRect.y && bMudRect.y <= bBombRect.h)
            {
                pObj->VTakeDamage(this);
            }
        }
    }
}

void Mudokon::HurtSound_475DB0()
{
    const u8 random = Math_RandomRange(0, 127) >= 64 ? 16 : 9;
    const s16 min = FP_GetExponent((FP_FromInteger(1) - mHealth) / FP_FromDouble(0.15));
    const s16 bInCamera = gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0);
    Mudokon_SFX(static_cast<MudSounds>(random), bInCamera ? 0 : 80, Math_RandomRange(200 * min, 40 * (5 * min + 5)), this);
}

void Mudokon::TakeASlap_476090(BaseGameObject* pFrom)
{
    if (field_180_emo_tbl != Mud_Emotion::eSick_7)
    {
        mHealth -= FP_FromDouble(0.077);
    }

    if (mHealth > FP_FromInteger(0))
    {
        HurtSound_475DB0();
        field_DA_xOffset = field_162_maxXOffset;
        field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eSlapOrWater_7);
        field_194_timer = sGnFrame + 30;
        field_17E_delayed_speak = MudAction::eNone_17;

        if (field_188_pTblEntry->field_6_sub_state)
        {
            AddAlerted();

            if (field_16A_flags.Get(Flags_16A::eBit5_following) && (field_180_emo_tbl == Mud_Emotion::eHappy_5 || field_180_emo_tbl == Mud_Emotion::eWired_6))
            {
                field_178_brain_sub_state2 = 4;
            }
            else
            {
                field_178_brain_sub_state2 = 7;
                field_16A_flags.Clear(Flags_16A::eBit5_following);
            }

            field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe_477B40;
            field_190_brain_sub_state = field_188_pTblEntry->field_6_sub_state;
        }

        if (field_180_emo_tbl != Mud_Emotion::eSad_3 || pFrom == sActiveHero)
        {
            field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
        }
    }
    else
    {
        Mudokon_SFX(MudSounds::eHurt2_9, 0, 1000, this);
        Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 32767, this);
        Event_Broadcast(kEventMudokonDied, sActiveHero);
        field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);
        mHealth = FP_FromInteger(0);
        field_18E_brain_state = Mud_Brain_State::Brain_5_ShrivelDeath_4714A0;
        mCurrentMotion = eMudMotions::M_KnockForward_45_474180;
        field_194_timer = sGnFrame + 90;
        mNextMotion = -1;
        VUpdateAnimRes();
    }
}

s32 Mudokon::GetResponseEntryIdx_471760()
{
    if (field_188_pTblEntry)
    {
        const size_t idx = field_188_pTblEntry - &kMudEmoTable_55C790.field_0_data[0][0];
        return static_cast<s32>(idx); // size_t for 64bit, but we know there isnt over s32 limits of entries in the table
    }
    return -1;
}

const MudEmotionTableEntry* Mudokon::GetResponseEntry_471790(s32 idx)
{
    if (idx == -1)
    {
        return nullptr;
    }

    return &kMudEmoTable_55C790.field_0_data[0][0] + idx;
}
