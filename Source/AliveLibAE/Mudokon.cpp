#include "stdafx.h"
#include "Mudokon.hpp"
#include "Function.hpp"
#include "../relive_lib/Shadow.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "AbilityRing.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "DDCheat.hpp"
#include "WorkWheel.hpp"
#include "BirdPortal.hpp"
#include "SwitchStates.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Spark.hpp"
#include "Sound/Midi.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Lever.hpp"
#include "../relive_lib/Particle.hpp"
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
#include "AnimationCallBacks.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"

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

using TMudBrainStateFunction = decltype(&Mudokon::Brain_0_GiveRings);

const TMudBrainStateFunction sMudokon_Brain_Table_55CDF0[10] = {
    &Mudokon::Brain_0_GiveRings,
    &Mudokon::Brain_1_Chisel,
    &Mudokon::Brain_2_CrouchScrub,
    &Mudokon::Brain_3_TurnWheel,
    &Mudokon::Brain_4_ListeningToAbe,
    &Mudokon::Brain_5_ShrivelDeath,
    &Mudokon::Brain_6_Escape,
    &Mudokon::Brain_7_FallAndSmackDeath,
    &Mudokon::Brain_8_AngryWorker,
    &Mudokon::Brain_9_Sick};

#define MUD_MOTION_STATES_ENUM(ENTRY) \
    ENTRY(Motion_0_Idle)            \
    ENTRY(Motion_1_WalkLoop)        \
    ENTRY(Motion_2_StandingTurn)    \
    ENTRY(Motion_Speak)             \
    ENTRY(Motion_Speak)             \
    ENTRY(Motion_Speak)             \
    ENTRY(Motion_Speak)             \
    ENTRY(Motion_7_WalkBegin)       \
    ENTRY(Motion_8_WalkToIdle)      \
    ENTRY(Motion_9_MidWalkToIdle)   \
    ENTRY(Motion_10_LeverUse)       \
    ENTRY(Motion_11_Chisel)         \
    ENTRY(Motion_12_StartChisel)    \
    ENTRY(Motion_13_StopChisel)     \
    ENTRY(Motion_14_CrouchScrub)    \
    ENTRY(Motion_15_CrouchIdle)     \
    ENTRY(Motion_16_CrouchTurn)     \
    ENTRY(Motion_17_StandToCrouch)  \
    ENTRY(Motion_18_CrouchToStand)  \
    ENTRY(Motion_19_WalkToRun)      \
    ENTRY(Motion_20_MidWalkToRun)   \
    ENTRY(Motion_21_RunLoop)        \
    ENTRY(Motion_22_RunToWalk)      \
    ENTRY(Motion_23_MidRunToWalk)   \
    ENTRY(Motion_24_RunSlideStop)   \
    ENTRY(Motion_25_RunSlideTurn)   \
    ENTRY(Motion_26_RunTurnToRun)   \
    ENTRY(Motion_27_SneakLoop)      \
    ENTRY(Motion_28_MidWalkToSneak)    \
    ENTRY(Motion_29_SneakToWalk)    \
    ENTRY(Motion_30_WalkToSneak) \
    ENTRY(Motion_31_MidSneakToWalk) \
    ENTRY(Motion_32_SneakBegin)     \
    ENTRY(Motion_33_SneakToIdle)    \
    ENTRY(Motion_34_MidSneakToIdle) \
    ENTRY(Motion_35_RunJumpBegin)      \
    ENTRY(Motion_36_RunJumpMid)        \
    ENTRY(Motion_37_StandToRun)      \
    ENTRY(Motion_38_Punch)          \
    ENTRY(Motion_39_HoistBegin)     \
    ENTRY(Motion_40_HoistLand)      \
    ENTRY(Motion_41_LandSoft1)      \
    ENTRY(Motion_42_LandSoft2)      \
    ENTRY(Motion_43_DunnoBegin)     \
    ENTRY(Motion_44_DunnoEnd)       \
    ENTRY(Motion_45_KnockForward)   \
    ENTRY(Motion_46_Knockback)      \
    ENTRY(Motion_47_KnockbackGetUp) \
    ENTRY(Motion_48_WalkOffEdge)    \
    ENTRY(Motion_49_Fall)           \
    ENTRY(Motion_50_Chant)          \
    ENTRY(Motion_51_ChantEnd)       \
    ENTRY(Motion_52_ToDuck)         \
    ENTRY(Motion_53_Duck)           \
    ENTRY(Motion_54_DuckToCrouch)   \
    ENTRY(Motion_55_DuckKnockback)  \
    ENTRY(Motion_56_SlapOwnHead)    \
    ENTRY(Motion_57_TurnWheelBegin) \
    ENTRY(Motion_58_TurnWheelLoop)  \
    ENTRY(Motion_59_TurnWheelEnd)

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
      {MudSounds::eNone, eMudMotions::Motion_43_DunnoBegin, Mud_Emotion::eNormal_0, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eFartPuh_23, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eNormal_0, 12},
      {MudSounds::eAnger_5, eMudMotions::Motion_38_Punch, Mud_Emotion::eAggressive_2, 13},
      {MudSounds::eNone, eMudMotions::Motion_43_DunnoBegin, Mud_Emotion::eNormal_0, 12},
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
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eAngry_1, 0},
      {MudSounds::eNoAngry_21, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNone, eMudMotions::Motion_43_DunnoBegin, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eAnger_5, eMudMotions::Motion_38_Punch, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eAnger_5, eMudMotions::Motion_38_Punch, Mud_Emotion::eAggressive_2, 13},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eNormal_0, 15},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eNormal_0, 15},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eAngry_1, 0},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12}},
     {// Correlates to MudEmotion::eAggressive_2.
      {MudSounds::eHiAngry_18, eMudMotions::M_Speak_4_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNoAngry_21, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eAngry_1, 0},
      {MudSounds::eNoAngry_21, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eAnger_5, eMudMotions::Motion_38_Punch, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eAnger_5, eMudMotions::Motion_38_Punch, Mud_Emotion::eAggressive_2, 13},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eNormal_0, 15},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eNormal_0, 15},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eAngry_1, 0},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eAngry_1, 12}},
     {// Correlates to MudEmotion::eSad_3;
      {MudSounds::eHiSad_20, eMudMotions::M_Speak_4_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSad_3, 0},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNone, eMudMotions::Motion_43_DunnoBegin, Mud_Emotion::eSad_3, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 13},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eNormal_0, 15},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eNormal_0, 0},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12}},
     {// Correlates to MudEmotion::eSuicidal_4.
      {MudSounds::eHiSad_20, eMudMotions::M_Speak_4_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSuicidal_4, 0},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eSadUgh_28, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSuicidal_4, 0},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 13},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eNormal_0, 0},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSuicidal_4, 12},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSuicidal_4, 0},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSuicidal_4, 0},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSuicidal_4, 0},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSuicidal_4, 0},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSuicidal_4, 0}},
     {// Correlates to MudEmotion::eHappy_5.
      {MudSounds::eHiHappy_19, eMudMotions::M_Speak_4_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eLaugh_10, eMudMotions::Motion_39_HoistBegin, Mud_Emotion::eWired_6, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eNuhUh_13, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eNormal_0, 13},
      {MudSounds::eNone, eMudMotions::Motion_43_DunnoBegin, Mud_Emotion::eWired_6, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eWired_6, 0},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eNoSad_22, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSad_3, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12}},
     {// Correlates to MudEmotion::eWired_6.
      {MudSounds::eHiHappy_19, eMudMotions::M_Speak_4_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eLaugh_10, eMudMotions::Motion_39_HoistBegin, Mud_Emotion::eWired_6, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eNuhUh_13, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eAnger_5, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eLaugh_10, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eWired_6, 12},
      {MudSounds::eOkay_12, eMudMotions::M_Speak_3_472FA0, Mud_Emotion::eNormal_0, 13},
      {MudSounds::eNone, eMudMotions::Motion_43_DunnoBegin, Mud_Emotion::eWired_6, 12},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eWired_6, 0},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eWired_6, 0},
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
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSick_7, 0},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSick_7, 0},
      {MudSounds::eSadUgh_28, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSick_7, 13},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSick_7, 0},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSick_7, 0},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSick_7, 0},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSick_7, 0},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSick_7, 0},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSick_7, 0},
      {MudSounds::eNone, eMudMotions::Motion_0_Idle, Mud_Emotion::eSick_7, 0},
      {MudSounds::eSick_24, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSick_7, 12},
      {MudSounds::eSick_24, eMudMotions::M_Speak_6_472FA0, Mud_Emotion::eSick_7, 12}}}};



using TMudStateFunction = decltype(&Mudokon::Motion_0_Idle);

const TMudStateFunction sMudokon_motion_states_55CE18[60] = {
    &Mudokon::Motion_0_Idle,
    &Mudokon::Motion_1_WalkLoop,
    &Mudokon::Motion_2_StandingTurn,
    &Mudokon::Motion_Speak,
    &Mudokon::Motion_Speak,
    &Mudokon::Motion_Speak,
    &Mudokon::Motion_Speak,
    &Mudokon::Motion_7_WalkBegin,
    &Mudokon::Motion_8_WalkToIdle,
    &Mudokon::Motion_9_MidWalkToIdle,
    &Mudokon::Motion_10_LeverUse,
    &Mudokon::Motion_11_Chisel,
    &Mudokon::Motion_12_StartChisel,
    &Mudokon::Motion_13_StopChisel,
    &Mudokon::Motion_14_CrouchScrub,
    &Mudokon::Motion_15_CrouchIdle,
    &Mudokon::Motion_16_CrouchTurn,
    &Mudokon::Motion_17_StandToCrouch,
    &Mudokon::Motion_18_CrouchToStand,
    &Mudokon::Motion_19_WalkToRun,
    &Mudokon::Motion_20_MidWalkToRun,
    &Mudokon::Motion_21_RunLoop,
    &Mudokon::Motion_22_RunToWalk,
    &Mudokon::Motion_23_MidRunToWalk,
    &Mudokon::Motion_24_RunSlideStop,
    &Mudokon::Motion_25_RunSlideTurn,
    &Mudokon::Motion_26_RunTurnToRun,
    &Mudokon::Motion_27_SneakLoop,
    &Mudokon::Motion_28_MidWalkToSneak,
    &Mudokon::Motion_29_SneakToWalk,
    &Mudokon::Motion_30_WalkToSneak,
    &Mudokon::Motion_31_MidSneakToWalk,
    &Mudokon::Motion_32_SneakBegin,
    &Mudokon::Motion_33_SneakToIdle,
    &Mudokon::Motion_34_MidSneakToIdle,
    &Mudokon::Motion_35_RunJumpBegin,
    &Mudokon::Motion_36_RunJumpMid,
    &Mudokon::Motion_37_StandToRun,
    &Mudokon::Motion_38_Punch,
    &Mudokon::Motion_39_HoistBegin,
    &Mudokon::Motion_40_HoistLand,
    &Mudokon::Motion_41_LandSoft1,
    &Mudokon::Motion_42_LandSoft2,
    &Mudokon::Motion_43_DunnoBegin,
    &Mudokon::Motion_44_DunnoEnd,
    &Mudokon::Motion_45_KnockForward,
    &Mudokon::Motion_46_Knockback,
    &Mudokon::Motion_47_KnockbackGetUp,
    &Mudokon::Motion_48_WalkOffEdge,
    &Mudokon::Motion_49_Fall,
    &Mudokon::Motion_50_Chant,
    &Mudokon::Motion_51_ChantEnd,
    &Mudokon::Motion_52_ToDuck,
    &Mudokon::Motion_53_Duck,
    &Mudokon::Motion_54_DuckToCrouch,
    &Mudokon::Motion_55_DuckKnockback,
    &Mudokon::Motion_56_SlapOwnHead,
    &Mudokon::Motion_57_TurnWheelBegin,
    &Mudokon::Motion_58_TurnWheelLoop,
    &Mudokon::Motion_59_TurnWheelEnd};


// This is used rather than the un-typesafe word_55CF08 array
static Mud_Emotion TLV_Emo_To_Internal_Emo(relive::Path_Mudokon::Mud_TLV_Emotion emo)
{
    switch (emo)
    {
        case relive::Path_Mudokon::Mud_TLV_Emotion::eNormal:
            return Mud_Emotion::eNormal_0;

        case relive::Path_Mudokon::Mud_TLV_Emotion::eAngry:
            return Mud_Emotion::eAngry_1;

        case relive::Path_Mudokon::Mud_TLV_Emotion::eSad:
            return Mud_Emotion::eSad_3;

        case relive::Path_Mudokon::Mud_TLV_Emotion::eWired:
            return Mud_Emotion::eWired_6;

        case relive::Path_Mudokon::Mud_TLV_Emotion::eSick:
            return Mud_Emotion::eSick_7;

        default:
            LOG_ERROR("Don't know about emo type " << static_cast<s32>(emo));
            ALIVE_FATAL("Unknown emo");
    }
}

Mudokon::Mudokon(relive::Path_Mudokon* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(18)
{
    field_154_unused = 0;
    field_140_last_event_index = -1;
    field_156_unused = -1;

    field_11C_bird_portal_id = Guid{};
    field_12C_unused = -1;
    field_158_wheel_id = Guid{};
    mBaseGameObjectTlvInfo = tlvId;
    field_194_timer = 0;
    field_18E_brain_state = Mud_Brain_State::Brain_0_GiveRings;
    field_190_brain_sub_state = 0;
    mNextMotion = -1;
    field_192_return_to_previous_motion = 0;
    field_13C_voice_pitch = 0;

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, TRUE, FALSE));
    field_10_resources_array.SetAt(1, nullptr);

    const AnimRecord& rec = AnimRec(AnimId::Mudokon_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::Mudokon_Idle, ppRes);

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);

    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_18C_unused = 0;
    SetTint(kMudTints_55C744, mCurrentLevel);

    field_17E_delayed_speak = MudAction::eNone_17;

    field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);
    field_16A_flags.Clear(Flags_16A::eBit3_alerted);
    field_16A_flags.Set(Flags_16A::eBit4_blind, pTlv->mBlind == relive::reliveChoice::eYes);
    field_16A_flags.Clear(Flags_16A::eBit5_following);
    field_16A_flags.Clear(Flags_16A::eBit6_standing_for_sad_or_angry);
    field_16A_flags.Clear(Flags_16A::eBit7_stopped_at_wheel);
    field_16A_flags.Clear(Flags_16A::eBit8_do_angry);
    field_16A_flags.Clear(Flags_16A::eBit9_seen_while_sick);
    // 10 and 11 ??
    field_16A_flags.Set(Flags_16A::eBit12_alert_enemies);
    field_16A_flags.Clear(Flags_16A::eBit13);
    field_16A_flags.Clear(Flags_16A::eBit14_make_sad_noise);

    field_180_emo_tbl = TLV_Emo_To_Internal_Emo(pTlv->mEmotion);
    field_188_pTblEntry = nullptr;
    field_182 = GameSpeakEvents::eNone_m1;
    field_120_angry_switch_id = pTlv->mAngrySwitchId;
    field_16C_flags.Clear(Flags_16C::eBit2_Unknown);
    field_16C_flags.Clear(Flags_16C::eBit3_Unknown);
    field_198_turning_wheel_timer = 0;

    switch (pTlv->mJob)
    {
        case relive::Path_Mudokon::MudJobs::eChisle:
            field_18E_brain_state = Mud_Brain_State::Brain_1_Chisel;
            field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMudchslResID, TRUE, FALSE));
            break;

        case relive::Path_Mudokon::MudJobs::eSitScrub:
            field_18E_brain_state = Mud_Brain_State::Brain_2_CrouchScrub;
            field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMudscrubResID, TRUE, FALSE));
            break;

        case relive::Path_Mudokon::MudJobs::eAngryWorker:
            field_18E_brain_state = Mud_Brain_State::Brain_8_AngryWorker;
            field_180_emo_tbl = Mud_Emotion::eAngry_1;
            field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMudscrubResID, TRUE, FALSE));
            break;

        case relive::Path_Mudokon::MudJobs::eDamageRingGiver:
        case relive::Path_Mudokon::MudJobs::eHealthRingGiver:
            if (pTlv->mJob == relive::Path_Mudokon::MudJobs::eDamageRingGiver)
            {
                field_168_ring_type = RingTypes::eExplosive_Emit_Effect_2;
            }
            else
            {
                field_168_ring_type = RingTypes::eHealing_Emit_Effect_11;
            }
            field_164_ring_pulse_interval = pTlv->mRingPulseInterval;
            field_16A_flags.Set(Flags_16A::eBit16_give_ring_without_password, pTlv->mGiveRingWithoutPassword == relive::reliveChoice::eYes);
            field_16C_flags.Clear(Flags_16C::eBit1_Unknown);
            field_18E_brain_state = Mud_Brain_State::Brain_0_GiveRings;
            field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeommResID, TRUE, FALSE));
            break;

        default:
            break;
    }

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe;
    }

    field_10_resources_array.SetAt(9, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMudoduckResID, TRUE, FALSE));
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMudbtlnkResID, TRUE, FALSE));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, TRUE, FALSE));
    field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeknfdResID, TRUE, FALSE));
    field_10_resources_array.SetAt(6, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeknbkResID, TRUE, FALSE));
    field_10_resources_array.SetAt(7, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeedgeResID, TRUE, FALSE));
    field_10_resources_array.SetAt(11, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMudidleResID, TRUE, FALSE));
    field_10_resources_array.SetAt(13, ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AEResourceID::kMudangryResID, TRUE, FALSE));
    field_10_resources_array.SetAt(14, ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AEResourceID::kMudsadResID, TRUE, FALSE));
    field_10_resources_array.SetAt(15, ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AEResourceID::kMudwiredResID, TRUE, FALSE));
    field_10_resources_array.SetAt(16, ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AEResourceID::kMudblindResID, TRUE, FALSE));
    field_10_resources_array.SetAt(17, ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AEResourceID::kMudsickResID, TRUE, FALSE));

    if (field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        SetPal(Mud_Emotion::eNormal_0);
    }

    mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, pTlv->mFacing == relive::reliveXDirection::eLeft);

    SetType(ReliveTypes::eMudokon);

    field_13C_voice_pitch = pTlv->mVoicePitch;
    field_17A_rescue_switch_id = pTlv->mRescueSwitchId;

    field_16A_flags.Set(Flags_16A::eBit2_persist_and_reset_offscreen, pTlv->mPersistAndResetOffscreen == relive::reliveChoice::eYes);
    field_16A_flags.Set(Flags_16A::eBit10_work_after_turning_wheel, pTlv->mWorkAfterTurningWheel == relive::reliveChoice::eYes);
    field_16A_flags.Set(Flags_16A::eBit11_get_depressed, pTlv->mGetsDepressed == relive::reliveChoice::eYes);
    field_16A_flags.Set(Flags_16A::eBit15_ring_and_angry_mud_timeout, pTlv->mRingPulseInterval & 1);

    field_17C_stand_idle_timer = 0;

    VStackOnObjectsOfType(ReliveTypes::eMudokon);

    if (field_180_emo_tbl == Mud_Emotion::eSick_7)
    {
        field_18E_brain_state = Mud_Brain_State::Brain_9_Sick;
        mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
        mCurrentMotion = eMudMotions::Motion_15_CrouchIdle;
    }
    else
    {
        mCurrentMotion = eMudMotions::Motion_0_Idle;
    }

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_ZapLinesMudsElum_Half_9;
        mScale = Scale::Bg;
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_ZapLinesElumMuds_28;
        mScale = Scale::Fg;
    }

    FP hitX = {};
    FP hitY = {};
    const auto bCollision = sCollisions->Raycast(
        FP_FromInteger(pTlv->mTopLeftX),
        FP_FromInteger(pTlv->mTopLeftY),
        FP_FromInteger(pTlv->mBottomRightX),
        FP_FromInteger(pTlv->mBottomRightY),
        &BaseAliveGameObjectCollisionLine,
        &hitX,
        &hitY,
        mScale == Scale::Fg ? kFgFloor : kBgFloor);

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    if (bCollision)
    {
        mYPos = hitY;
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
            BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            const PSX_RECT bRect = VGetBoundingRect();
            VOnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {bRect.w, static_cast<s16>(bRect.h + 5)},
                ObjList_5C1B78,
                (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
        }
    }

    mAnim.mFnPtrArray = kAbe_Anim_Frame_Fns_55EF98;
    BaseAliveGameObjectLastLineYPos = mYPos;

    if (field_18E_brain_state == Mud_Brain_State::Brain_2_CrouchScrub || field_18E_brain_state == Mud_Brain_State::Brain_1_Chisel)
    {
        MapFollowMe(TRUE);
    }

    field_16A_flags.Set(Flags_16A::eBit1_not_rescued);
    field_190_brain_sub_state = 0;
    field_118_tlvInfo = mBaseGameObjectTlvInfo;

    field_128_angry_timer = 0;
    field_160_delayed_speak = MudSounds::eNone;
    field_162_maxXOffset = mXOffset;

    mShadow = relive_new Shadow();

    VUpdate();
}

const AnimId kMudMotionAnimIds[60] = {
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

    auto pTlv = static_cast<relive::Path_Mudokon*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_40_tlvInfo));
    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBSIC1.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeknfdResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEKNFD.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeknbkResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEKNBK.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeedgeResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEEDGE.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SHADOW.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMudidleResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("MUDIDLE.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AEResourceID::kMudangryResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("MUDPAL.BND", nullptr);
    }

    if (pTlv->mJob != relive::Path_Mudokon::MudJobs::eChisle)
    {
        if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMudscrubResID, FALSE, FALSE))
        {
            ResourceManager::LoadResourceFile_49C170("MUDSCRUB.BAN", nullptr);
        }
    }
    else if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMudchslResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("MUDCHSL.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMudoduckResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("MUDWORK.BND", nullptr);
    }

    if (pState->field_24_current_motion == eMudMotions::Motion_10_LeverUse && !ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbepullResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEPULL.BAN", nullptr);
    }

    if (pState->field_24_current_motion == eMudMotions::Motion_57_TurnWheelBegin && !ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEWORK.BAN", nullptr);
    }

    if (pState->field_24_current_motion == eMudMotions::Motion_58_TurnWheelLoop && !ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEWORK.BAN", nullptr);
    }

    if (pState->field_24_current_motion == eMudMotions::Motion_59_TurnWheelEnd && !ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEWORK.BAN", nullptr);
    }

    auto pMud = relive_new Mudokon(pTlv, pState->field_40_tlvInfo);
    if (pMud)
    {
        if (pState->field_3D_bIsPlayer)
        {
            sControlledCharacter = pMud;
        }

        pMud->BaseAliveGameObjectPathTLV = nullptr;
        pMud->BaseAliveGameObjectCollisionLine = nullptr;

        pMud->mXPos = pState->field_4_xpos;
        pMud->mYPos = pState->field_8_ypos;
        pMud->mVelX = pState->field_C_velx;
        pMud->mVelY = pState->field_10_vely;

        pMud->field_134_xVelSlowBy = pState->field_44_velx_slow_by;
        pMud->field_138_unused = pState->field_48_unused;

        pMud->mCurrentPath = pState->field_14_path_number;
        pMud->mCurrentLevel = MapWrapper::FromAESaveData(pState->field_16_lvl_number);
        pMud->mSpriteScale = pState->field_18_sprite_scale;

        pMud->mRGB.SetRGB(pState->field_1C_r, pState->field_1E_g, pState->field_20_b);

        pMud->mCurrentMotion = pState->field_24_current_motion;

        u8** ppRes = pMud->GetResBlockForMotion(pState->field_24_current_motion);

        pMud->mAnim.Set_Animation_Data(kMudMotionAnimIds[pMud->mCurrentMotion], ppRes);

        pMud->mAnim.mCurrentFrame = pState->field_26_anim_current_frame;
        pMud->mAnim.mFrameChangeCounter = pState->field_28_anim_frame_change_counter;

        pMud->mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, pState->field_22_bFlipX & 1);
        pMud->mAnim.mFlags.Set(AnimFlags::eBit3_Render, pState->field_2A_bAnimRender & 1);

        pMud->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2B_bDrawable & 1);

        if (IsLastFrame(&pMud->mAnim))
        {
            pMud->mAnim.mFlags.Set(AnimFlags::eBit18_IsLastFrame);
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

        pMud->field_188_pTblEntry = GetResponseEntry(pState->field_84_response_entry_idx);
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

    pState->field_4_xpos = mXPos;
    pState->field_8_ypos = mYPos;
    pState->field_C_velx = mVelX;
    pState->field_10_vely = mVelY;

    pState->field_44_velx_slow_by = field_134_xVelSlowBy;
    pState->field_48_unused = field_138_unused;

    pState->field_14_path_number = mCurrentPath;
    pState->field_16_lvl_number = MapWrapper::ToAE(mCurrentLevel);
    pState->field_18_sprite_scale = mSpriteScale;

    pState->field_1C_r = mRGB.r;
    pState->field_1E_g = mRGB.g;
    pState->field_20_b = mRGB.b;

    pState->field_22_bFlipX = mAnim.mFlags.Get(AnimFlags::eBit5_FlipX);
    pState->field_24_current_motion = mCurrentMotion;
    pState->field_26_anim_current_frame = static_cast<s16>(mAnim.mCurrentFrame);
    pState->field_28_anim_frame_change_counter = static_cast<s16>(mAnim.mFrameChangeCounter);
    pState->field_2B_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2A_bAnimRender = mAnim.mFlags.Get(AnimFlags::eBit3_Render);
    pState->field_2C_health = mHealth;
    pState->field_30_current_motion = mCurrentMotion;
    pState->field_32_next_motion = mNextMotion;
    pState->field_34_lastLineYPos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);
    pState->field_36_line_type = -1;

    pState->field_3C_can_be_possessed = mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit3_Can_Be_Possessed);

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_36_line_type = BaseAliveGameObjectCollisionLine->mLineType;
    }

    pState->field_3D_bIsPlayer = this == sControlledCharacter;
    pState->field_40_tlvInfo = field_118_tlvInfo;
    pState->field_4C_portal_id = Guid{};

    if (field_11C_bird_portal_id != Guid{})
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
    pState->field_60_wheel_id = Guid{};

    if (field_158_wheel_id != Guid{})
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

    pState->field_84_response_entry_idx = GetResponseEntryIdx();

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
                mXPos,
                mYPos - FP_FromInteger(20),
                mXPos,
                mYPos + FP_FromInteger(20),
                &BaseAliveGameObjectCollisionLine,
                &mXPos,
                &mYPos,
                CollisionMask(static_cast<eLineTypes>(BaseAliveGameObjectCollisionLineType)));

            if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
                BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                const PSX_RECT bRect = VGetBoundingRect();
                VOnCollisionWith(
                    {bRect.x, static_cast<s16>(bRect.y + 5)},
                    {bRect.w, static_cast<s16>(bRect.h + 5)},
                    ObjList_5C1B78,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
            }
        }

        BaseAliveGameObjectCollisionLineType = 0;

        if (field_11C_bird_portal_id != Guid{})
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
                    field_11C_bird_portal_id = pObj->mBaseGameObjectId;
                    sGoingToBirdPortalMudCount_5C3012++;
                    field_16C_flags.Set(Flags_16C::eBit3_Unknown);
                    if (field_18E_brain_state == Mud_Brain_State::Brain_6_Escape && field_190_brain_sub_state == 3)
                    {
                        static_cast<BirdPortal*>(pObj)->VPortalClipper(1);
                        mAnim.mRenderLayer = mSpriteScale != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30;
                    }
                    break;
                }
            }
        }

        if (field_158_wheel_id != Guid{})
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
                    field_158_wheel_id = pObj->mBaseGameObjectId;
                    static_cast<WorkWheel*>(pObj)->VStartTurning();
                    break;
                }
            }
        }
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    const FP xDistFromPlayer = FP_Abs(mXPos - sControlledCharacter->mXPos);
    if (xDistFromPlayer > FP_FromInteger(750))
    {
        mAnim.mFlags.Clear(AnimFlags::eBit2_Animate);
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
        return;
    }

    const FP yDistanceFromPlayer = FP_Abs(mYPos - sControlledCharacter->mYPos);
    if (yDistanceFromPlayer > FP_FromInteger(520))
    {
        mAnim.mFlags.Clear(AnimFlags::eBit2_Animate);
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
        return;
    }

    if (mHealth > FP_FromInteger(0))
    {
        mAnim.mFlags.Set(AnimFlags::eBit2_Animate);
        mAnim.mFlags.Set(AnimFlags::eBit3_Render);
    }

    //DDCheat::DebugStr_4F5560("\nMotion = %s BrainState = %s\n", sMudMotionStateNames[mCurrentMotion], sMudAiStateNames[static_cast<s32>(field_18E_brain_state)]);
    //LOG_INFO(sMudMotionStateNames[mCurrentMotion] << " " << static_cast<s32>(field_18E_brain_state));
    //LOG_INFO(sMudMotionStateNames[mCurrentMotion] << " " << sMudAiStateNames[static_cast<s32>(field_18E_brain_state)]);

    const s16 oldMotion = mCurrentMotion;

    field_190_brain_sub_state = (this->*sMudokon_Brain_Table_55CDF0[field_18E_brain_state])();

    const FP oldXPos = mXPos;
    const FP oldYPos = mYPos;

    (this->*sMudokon_motion_states_55CE18[mCurrentMotion])();

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

    if (oldMotion != mCurrentMotion || mBaseAliveGameObjectFlags.Get(Flags_114::e114_MotionChanged_Bit2))
    {
        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_MotionChanged_Bit2);
        VUpdateResBlock();
    }
    else if (field_192_return_to_previous_motion)
    {
        mCurrentMotion = mPreviousMotion;
        VUpdateResBlock();
        mAnim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
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
                mRGB.SetRGB(63, 63, 63);
                mAnim.LoadPal(field_10_resources_array.ItemAt(16), 0);
            }
            else
            {
                mRGB.SetRGB(87, 103, 67);
                FrameInfoHeader* pFrameInfoHeader = mAnim.Get_FrameHeader(-1);
                FrameHeader* pHeader = reinterpret_cast<FrameHeader*>(&(*mAnim.field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);
                mAnim.LoadPal(mAnim.field_20_ppBlock, pHeader->field_0_clut_offset);
            }
            break;

        case Mud_Emotion::eAngry_1:
        case Mud_Emotion::eAggressive_2:
            mRGB.SetRGB(63, 63, 63);
            mAnim.LoadPal(field_10_resources_array.ItemAt(13), 0);
            break;

        case Mud_Emotion::eSad_3:
        case Mud_Emotion::eSuicidal_4:
            mRGB.SetRGB(63, 63, 63);
            mAnim.LoadPal(field_10_resources_array.ItemAt(14), 0);
            break;

        case Mud_Emotion::eHappy_5:
        case Mud_Emotion::eWired_6:
            mRGB.SetRGB(74, 74, 74);
            mAnim.LoadPal(field_10_resources_array.ItemAt(15), 0);
            break;

        case Mud_Emotion::eSick_7:
            mRGB.SetRGB(63, 63, 63);
            mAnim.LoadPal(field_10_resources_array.ItemAt(17), 0);
            break;
    }
}

void Mudokon::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        if (!mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit1_bShot))
        {
            VSetMotion(eMudMotions::Motion_48_WalkOffEdge);
        }

        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
    }
}

void Mudokon::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    relive::Path_TLV* pTlvIter = pTlv;
    while (pTlvIter)
    {
        if (pTlvIter->mTlvType == ReliveTypes::eDeathDrop)
        {
            if (mHealth > FP_FromInteger(0))
            {
                field_18E_brain_state = Mud_Brain_State::Brain_7_FallAndSmackDeath;
                field_190_brain_sub_state = 0;
                mHealth = FP_FromInteger(0);
                EventBroadcast(kEventMudokonDied, this);
                break;
            }
        }

        pTlvIter = sPathInfo->TlvGetAt(
            pTlvIter,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }
}

s16 Mudokon::FacingBirdPortal(BirdPortal* pTarget)
{
    if (pTarget->mXPos == mXPos)
    {
        return TRUE;
    }
    else if (pTarget->mXPos > mXPos && !mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }
    else if (pTarget->mXPos < mXPos && mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
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
        field_158_wheel_id = Guid{};
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
        field_11C_bird_portal_id = Guid{};
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
            sActiveHero->mRingPulseTimer = sGnFrame + field_164_ring_pulse_interval;
        }
        else
        {
            sActiveHero->mRingPulseTimer = sGnFrame + 200000;
        }

        sActiveHero->mHaveShrykull = FALSE;

        if (field_168_ring_type == RingTypes::eHealing_Emit_Effect_11)
        {
            sActiveHero->field_1AC_flags.Set(Abe::e1AC_eBit15_have_healing);
        }
    }

    if (mCurrentMotion == eMudMotions::Motion_50_Chant)
    {
        SND_SEQ_Stop(SeqId::MudokonChant_11);
    }
}

void Mudokon::VScreenChanged()
{
    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds.Find_Impl(field_11C_bird_portal_id));
    BaseGameObject::VScreenChanged();

    if (field_18E_brain_state == Mud_Brain_State::Brain_0_GiveRings ||
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
            field_11C_bird_portal_id = Guid{};
        }
    }
}

void Mudokon::VPossessed()
{
    sControlledCharacter = sActiveHero;
    if (field_180_emo_tbl == Mud_Emotion::eSick_7 && !FindObjectOfType(ReliveTypes::eTorturedMud, mXPos, mYPos - FP_FromInteger(50)))
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
            switch (pBullet->mBulletType)
            {
                case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
                case BulletType::eNormalBullet_2:
                {
                    relive_new Blood(
                        mXPos,
                        mYPos - (FP_FromInteger(30) * mSpriteScale),
                        pBullet->mXDistance <= FP_FromInteger(0) ? FP_FromInteger(-24) : FP_FromInteger(24),
                        FP_FromInteger(0),
                        mSpriteScale,
                        50);
                    SetPal(Mud_Emotion::eNormal_0);
                    break;
                }

                default:
                    break;
            }

            if (pBullet->mBulletType == BulletType::ePossessedSligZBullet_1 || pBullet->mBulletType == BulletType::eZBullet_3)
            {
                const PSX_RECT v11 = VGetBoundingRect();
                const FP tlvYPos = FP_FromInteger(v11.h);

                if (Bullet::InZBulletCover(mXPos, tlvYPos, v11) || !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, tlvYPos, 0))
                {
                    // ZCover saved us, or somehow we've not in the current camera
                    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
                    mHealth = FP_FromInteger(1);
                    return 0;
                }

                // Nothing saved us, get shot
                relive_new Blood(
                    mXPos,
                    mYPos - (FP_FromInteger(30) * mSpriteScale),
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    mSpriteScale,
                    50);

                SetPal(Mud_Emotion::eNormal_0);
                // Fall though to other cases below
            }

            [[fallthrough]];
        }

        case ReliveTypes::eGasCountDown:
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
            field_18E_brain_state = Mud_Brain_State::Brain_5_ShrivelDeath;
            mCurrentMotion = eMudMotions::Motion_45_KnockForward;
            mNextMotion = -1;
            field_194_timer = sGnFrame + 90;
            VUpdateResBlock();
            EventBroadcast(kEventMudokonDied, this);
            if (pFrom->Type() == ReliveTypes::eGasCountDown)
            {
                SFX_Play_Pitch(relive::SoundEffects::Choke, 127, 128);
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
            if (mHealth <= FP_FromInteger(0) || (FindObjectOfType(ReliveTypes::eTorturedMud, mXPos, mYPos - FP_FromInteger(50)) && field_18E_brain_state == Mud_Brain_State::Brain_9_Sick))
            {
                return 1;
            }

            mHealth = FP_FromInteger(0);

            if (field_16A_flags.Get(Flags_16A::eBit4_blind))
            {
                relive_new Gibs(
                    GibType::BlindMud_4,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    mSpriteScale,
                    0);

                relive_new Gibs(
                    GibType::BlindMud_4,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    mSpriteScale,
                    0);
            }
            else
            {
                relive_new Gibs(
                    GibType::Mud_3,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    mSpriteScale,
                    0);

                relive_new Gibs(
                    GibType::Mud_3,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    mSpriteScale,
                    0);
            }

            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            SetPal(Mud_Emotion::eNormal_0);
            EventBroadcast(kEventMudokonDied, sActiveHero);
            return 1;

        case ReliveTypes::eElectricWall:
            Mudokon_SFX(MudSounds::eDeathDropScream_15, 0, 0, this);
            EventBroadcast(kEventMudokonDied, this);
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
                field_18E_brain_state = Mud_Brain_State::Brain_5_ShrivelDeath;
                field_194_timer = sGnFrame + 90;
                mCurrentMotion = eMudMotions::Motion_46_Knockback;
                mNextMotion = -1;
                EventBroadcast(kEventMudokonDied, this);
                SetPal(Mud_Emotion::eNormal_0);

                const PSX_RECT bRect = VGetBoundingRect();

                auto pFleech = static_cast<BaseAliveGameObject*>(pFrom);
                relive_new Blood(
                    mXPos,
                    (FP_FromInteger(bRect.y + bRect.h) / FP_FromInteger(2)),
                    mXPos - pFleech->mXPos < FP_FromInteger(0) ? FP_FromInteger(-24) : FP_FromInteger(24),
                    FP_FromInteger(0),
                    mSpriteScale,
                    50);

                // TODO: Only set if pFrom->mXPos != mXPos ??
                mCurrentMotion = eMudMotions::Motion_45_KnockForward;

                VUpdateResBlock();
                SfxPlayMono(relive::SoundEffects::KillEffect, 127);
                SfxPlayMono(relive::SoundEffects::FallingItemHit, 90);
            }
            else
            {
                HurtSoundPitchedToHealth();
            }
            return 1;

        case ReliveTypes::eAbe:
            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_62_Punch_454750)
            {
                if (mHealth > FP_FromInteger(0))
                {
                    SetPal(Mud_Emotion::eNormal_0);
                    TakeASlap(pFrom);
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
            if (static_cast<Mudokon*>(pFrom)->mCurrentMotion != eMudMotions::Motion_38_Punch || mHealth <= FP_FromInteger(0))
            {
                return 1;
            }
            SetPal(Mud_Emotion::eNormal_0);
            TakeASlap(pFrom);
            return 1;

        case ReliveTypes::eShrykull:
        case ReliveTypes::eElectrocute:
            if (mHealth <= FP_FromInteger(0))
            {
                return 1;
            }
            mHealth = FP_FromInteger(0);
            EventBroadcast(kEventMudokonDied, this);
            SetPal(Mud_Emotion::eNormal_0);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            return 1;

        case ReliveTypes::eSlamDoor:
            if (mHealth <= FP_FromInteger(0) || mCurrentMotion == eMudMotions::Motion_36_RunJumpMid)
            {
                return 1;
            }
            ToKnockback();
            VUpdateResBlock();
            return 1;

        case ReliveTypes::eSlog:
            if (mHealth <= FP_FromInteger(0))
            {
                return 1;
            }
            field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);
            mHealth = FP_FromInteger(0);
            field_18E_brain_state = Mud_Brain_State::Brain_5_ShrivelDeath;
            field_194_timer = sGnFrame + 90;
            mCurrentMotion = eMudMotions::Motion_45_KnockForward;
            EventBroadcast(kEventMudokonDied, this);
            VUpdateResBlock();
            SetPal(Mud_Emotion::eNormal_0);
            return 1;

        default:
            if (mHealth <= FP_FromInteger(0))
            {
                return 1;
            }
            HurtSoundPitchedToHealth();
            mCurrentMotion = eMudMotions::Motion_45_KnockForward;
            mNextMotion = -1;
            field_194_timer = sGnFrame + 30;
            VUpdateResBlock();
            if (field_10_resources_array.ItemAt(2))
            {
                field_18E_brain_state = Mud_Brain_State::Brain_1_Chisel;
                field_190_brain_sub_state = 8;
            }
            else
            {
                field_18E_brain_state = Mud_Brain_State::Brain_2_CrouchScrub;
                field_190_brain_sub_state = 7;
            }

            mXOffset = field_162_maxXOffset;

            RemoveAlerted();

            if (pFrom == sControlledCharacter)
            {
                mHealth -= FP_FromDouble(0.06);
                if (mHealth > FP_FromInteger(0))
                {
                    EventBroadcast(kEventMudokonAbuse, this);
                }
                else
                {
                    field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);
                    field_18E_brain_state = Mud_Brain_State::Brain_5_ShrivelDeath;
                    field_194_timer = sGnFrame + 90;
                    EventBroadcast(kEventMudokonDied, this);
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

    auto pWheelTlv = static_cast<relive::Path_WorkWheel*>(sPathInfo->TLV_Get_At_4DB4B0(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eWorkWheel));

    if (SwitchStates_Get(pWheelTlv->mSwitchId))
    {
        if (field_198_turning_wheel_timer == 0)
        {
            field_198_turning_wheel_timer = sGnFrame + 20;
        }
    }

    // OG Bug: The second condition can never resolve to true because field_198_turning_wheel_timer will always be reset to zero before it can happen.
    if (!SwitchStates_Get(pWheelTlv->mSwitchId) || field_198_turning_wheel_timer > static_cast<s32>(sGnFrame))
    {
        return field_190_brain_sub_state;
    }

    field_198_turning_wheel_timer = 0;
    field_16A_flags.Clear(Flags_16A::eBit7_stopped_at_wheel);
    field_194_timer = sGnFrame + 45;
    return 1;
}

void Mudokon::VUpdateResBlock()
{
    u8** ppRes = GetResBlockForMotion(mCurrentMotion);
    if (!ppRes)
    {
        LOG_ERROR("No res for " << mCurrentMotion);
    }
    mAnim.Set_Animation_Data(kMudMotionAnimIds[mCurrentMotion], ppRes);
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

enum Brain_2_CrouchScrub
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

s16 Mudokon::Brain_0_GiveRings()
{
    switch (field_190_brain_sub_state)
    {
        case Brain_0_GiveRings::eBrain0_Inactive_0:
            if (gMap.Is_Point_In_Current_Camera(
                    sActiveHero->mCurrentLevel,
                    sActiveHero->mCurrentPath,
                    sActiveHero->mXPos,
                    sActiveHero->mYPos,
                    0))
            {
                field_16C_flags.Clear(Flags_16C::eBit1_Unknown);
                field_194_timer = sGnFrame + 60;
                mNextMotion = eMudMotions::Motion_0_Idle;

                if (sActiveHero->mRingPulseTimer)
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
            if (static_cast<s32>(sGnFrame) >= field_194_timer && mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                if (VIsFacingMe(sActiveHero))
                {
                    MudEmotionSound(MudSounds::eHelloNeutral_3);
                    mNextMotion = eMudMotions::M_Speak_4_472FA0;
                    field_194_timer = MudResponseDelay() + sGnFrame + 150;
                    return Brain_0_GiveRings::eBrain0_Idle_2;
                }
                mNextMotion = eMudMotions::Motion_2_StandingTurn;
            }
            break;

        case Brain_0_GiveRings::eBrain0_Idle_2:
            if (field_16A_flags.Get(Flags_16A::eBit16_give_ring_without_password) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && gMap.Is_Point_In_Current_Camera(sActiveHero->mCurrentLevel, sActiveHero->mCurrentPath, sActiveHero->mXPos, sActiveHero->mYPos, 0)
                && !sActiveHero->mRingPulseTimer)
            {
                field_194_timer = MudResponseDelay() + sGnFrame + 20;
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
                    field_194_timer = MudResponseDelay() + sGnFrame + 20;
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
            if (static_cast<s32>(sGnFrame) >= field_194_timer && mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                if (VIsFacingMe(sActiveHero))
                {
                    if (sActiveHero->mRingPulseTimer > 0)
                    {
                        MudEmotionSound(MudSounds::eHelloNeutral_3);
                    }
                    else
                    {
                        MudEmotionSound(MudSounds::eOkay_12);
                    }
                    mNextMotion = eMudMotions::M_Speak_4_472FA0;
                    return Brain_0_GiveRings::eBrain0_SaysOkay_6;
                }
                mNextMotion = eMudMotions::Motion_2_StandingTurn;
            }
            break;

        case Brain_0_GiveRings::eBrain0_WrongGameSpeak_4:
            if (static_cast<s32>(sGnFrame) >= field_194_timer && mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                if (VIsFacingMe(sActiveHero))
                {
                    mNextMotion = eMudMotions::Motion_43_DunnoBegin;
                    return Brain_0_GiveRings::eBrain0_Idle_2;
                }
                mNextMotion = eMudMotions::Motion_2_StandingTurn;
            }
            break;

        case Brain_0_GiveRings::eBrain0_Shrug_5:
            if (static_cast<s32>(sGnFrame) >= field_194_timer && mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                if (VIsFacingMe(sActiveHero))
                {
                    mNextMotion = eMudMotions::Motion_43_DunnoBegin;
                    field_194_timer = MudResponseDelay() + sGnFrame + 20;
                    return Brain_0_GiveRings::eBrain0_PreIdle_1;
                }
                mNextMotion = eMudMotions::Motion_2_StandingTurn;
            }
            break;

        case Brain_0_GiveRings::eBrain0_SaysOkay_6:
            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                if (sActiveHero->mRingPulseTimer <= 0)
                {
                    mNextMotion = eMudMotions::Motion_50_Chant;
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
                const PSX_RECT bRect = VGetBoundingRect();

                AbilityRing::Factory(
                    FP_FromInteger((bRect.x + bRect.w) / 2),
                    FP_FromInteger((bRect.y + bRect.h) / 2),
                    field_168_ring_type,
                    mSpriteScale);

                // Create a ring that locks onto abe
                const PSX_RECT bRectAbe = sActiveHero->VGetBoundingRect();

                RingTypes ringTypeToGive = RingTypes::eExplosive_Give_3;
                if (field_168_ring_type == RingTypes::eHealing_Emit_Effect_11)
                {
                    ringTypeToGive = RingTypes::eHealing_Give_13;
                }

                AbilityRing* pRing = AbilityRing::Factory(
                    FP_FromInteger((bRectAbe.x + bRectAbe.w) / 2),
                    FP_FromInteger((bRectAbe.y + bRectAbe.h) / 2),
                    ringTypeToGive,
                    sActiveHero->mSpriteScale);

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
                    sActiveHero->mRingPulseTimer = sGnFrame + field_164_ring_pulse_interval;
                }
                else
                {
                    sActiveHero->mRingPulseTimer = sGnFrame + 200000;
                }

                sActiveHero->mHaveShrykull = FALSE;

                if (field_168_ring_type == RingTypes::eHealing_Emit_Effect_11)
                {
                    sActiveHero->field_1AC_flags.Set(Abe::e1AC_eBit15_have_healing);
                }

                mNextMotion = eMudMotions::Motion_0_Idle;
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

s16 Mudokon::Brain_1_Chisel()
{
    if (FindBirdPortal())
    {
        AddAlerted();
        field_18E_brain_state = Mud_Brain_State::Brain_6_Escape;
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

    const GameSpeakEvents lastSpeak = LastGameSpeak();
    if (lastSpeak == GameSpeakEvents::Slig_LookOut_6 || EventGet(kEventShooting))
    {
        if (field_190_brain_sub_state != Brain_1_Chisle::eBrain1_StandUp_3 && field_190_brain_sub_state != Brain_1_Chisle::eBrain1_Duck_5 && field_190_brain_sub_state != Brain_1_Chisle::eBrain1_DuckKnockback_8 && field_190_brain_sub_state != Brain_1_Chisle::eBrain1_OutOfDuck_6)
        {
            field_190_brain_sub_state = Brain_1_Chisle::eBrain1_Duck_5;
        }
    }

    IBaseAnimatedWithPhysicsGameObject* pAbuseEvent = IsEventInRange(kEventMudokonAbuse, mXPos, mYPos, AsEventScale(mScale));
    IBaseAnimatedWithPhysicsGameObject* pDeadMudEvent = IsEventInRange(kEventMudokonDied, mXPos, mYPos, AsEventScale(mScale));
    IBaseAnimatedWithPhysicsGameObject* pLoudNoiseEvent = IsEventInRange(kEventLoudNoise, mXPos, mYPos, AsEventScale(mScale));

    const bool reactToAbused = (pAbuseEvent && pAbuseEvent != this && field_190_brain_sub_state != Brain_1_Chisle::eBrain1_StandUp_3 && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0));

    const bool reactToDeadMud = (pDeadMudEvent && pDeadMudEvent != this && field_190_brain_sub_state != Brain_1_Chisle::eBrain1_StandUp_3 && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0));

    const bool reactToLoudNoise = (pLoudNoiseEvent && pLoudNoiseEvent->Type() == ReliveTypes::eGlukkon && field_190_brain_sub_state != Brain_1_Chisle::eBrain1_StandUp_3 && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0));

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
            field_194_timer = MudResponseDelay() + sGnFrame + 10;
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (LaughingGasInCurrentScreen())
        {
            field_180_emo_tbl = Mud_Emotion::eWired_6;

            AddAlerted();

            field_190_brain_sub_state = Brain_1_Chisle::eBrain1_StandUp_3;
            field_194_timer = MudResponseDelay() + sGnFrame + 10;
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
            if (mCurrentMotion != eMudMotions::Motion_17_StandToCrouch)
            {
                mCurrentMotion = eMudMotions::Motion_11_Chisel;
                mNextMotion = -1;
            }
            field_194_timer = (Math_NextRandom() % 64) + (sGnFrame + 35);
            return Brain_1_Chisle::eBrain1_ChisleTheFloor_1;

        case Brain_1_Chisle::eBrain1_ChisleTheFloor_1:
        {
            bool ignoreHellOrAllYa = false;
            if (lastSpeak == GameSpeakEvents::eHello_9)
            {
                if (sIsMudStandingUp_5C3018 || !IAmNearestToAbe())
                {
                    ignoreHellOrAllYa = true;
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAllYa_23)
            {
                ignoreHellOrAllYa = true;
            }

            if (!ignoreHellOrAllYa && sActiveHero->mSpriteScale == mSpriteScale)
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
                field_194_timer = MudResponseDelay() + sGnFrame + 10;
                sIsMudStandingUp_5C3018 = 1;
                field_16C_flags.Set(Flags_16C::eBit2_Unknown);

                AddAlerted();
                return Brain_1_Chisle::eBrain1_StandUp_3;
            }

            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                mNextMotion = eMudMotions::Motion_11_Chisel;
            }

            if (static_cast<s32>(sGnFrame) > field_194_timer && mCurrentMotion == eMudMotions::Motion_11_Chisel)
            {
                mNextMotion = eMudMotions::Motion_15_CrouchIdle;
            }

            if (mCurrentMotion != eMudMotions::Motion_15_CrouchIdle)
            {
                return field_190_brain_sub_state;
            }

            if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && (Math_NextRandom() < 120))
            {
                AddAlerted();

                field_16A_flags.Set(Flags_16A::eBit14_make_sad_noise);
                field_16A_flags.Set(Flags_16A::eBit6_standing_for_sad_or_angry);

                mNextMotion = eMudMotions::Motion_0_Idle;
                field_194_timer = MudResponseDelay() + sGnFrame + 10;
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
                if (sIsMudStandingUp_5C3018 || !IAmNearestToAbe())
                {
                    ignoreHellOrAllYa = true;
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAllYa_23)
            {
                ignoreHellOrAllYa = true;
            }

            if (!ignoreHellOrAllYa && sActiveHero->mSpriteScale == mSpriteScale)
            {
                AddAlerted();

                mNextMotion = eMudMotions::Motion_0_Idle;
                field_194_timer = MudResponseDelay() + sGnFrame + 10;
                sIsMudStandingUp_5C3018 = 1;
                field_16C_flags.Set(Flags_16C::eBit2_Unknown);
                return Brain_1_Chisle::eBrain1_StandUp_3;
            }

            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                mNextMotion = eMudMotions::Motion_17_StandToCrouch;
            }

            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }

            field_194_timer = (Math_NextRandom() % 64) + (sGnFrame + 35);
            mNextMotion = eMudMotions::Motion_11_Chisel;
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

            if (!skip && (bForce || (!sIsMudStandingUp_5C3018 && IAmNearestToAbe())))
            {
                if (sActiveHero->mSpriteScale == mSpriteScale)
                {
                    AddAlerted();
                    sIsMudStandingUp_5C3018 = 1;
                    field_16C_flags.Set(Flags_16C::eBit2_Unknown);

                    field_16A_flags.Clear(Flags_16A::eBit14_make_sad_noise);
                    field_16A_flags.Clear(Flags_16A::eBit6_standing_for_sad_or_angry);
                }
            }

            if (static_cast<s32>(sGnFrame) <= field_194_timer || mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                if (mNextMotion != -1)
                {
                    return field_190_brain_sub_state;
                }

                if (mCurrentMotion == eMudMotions::Motion_11_Chisel || mCurrentMotion == eMudMotions::Motion_15_CrouchIdle)
                {
                    mNextMotion = eMudMotions::Motion_0_Idle;
                }

                if (mCurrentMotion != eMudMotions::Motion_53_Duck)
                {
                    return field_190_brain_sub_state;
                }
                else
                {
                    mNextMotion = eMudMotions::Motion_15_CrouchIdle;
                    return field_190_brain_sub_state;
                }
            }

            if (field_180_emo_tbl == Mud_Emotion::eSad_3 && field_16A_flags.Get(Flags_16A::eBit14_make_sad_noise))
            {
                field_16A_flags.Clear(Flags_16A::eBit12_alert_enemies);
                field_16A_flags.Clear(Flags_16A::eBit14_make_sad_noise);
                mNextMotion = eMudMotions::M_Speak_6_472FA0;
                MudEmotionSound(MudSounds::eSadUgh_28);
                return Brain_1_Chisle::eBrain1_SadNoise_4;
            }
            else
            {
                sIsMudStandingUp_5C3018 = 0;
                field_16C_flags.Clear(Flags_16C::eBit2_Unknown);
                if (field_180_emo_tbl != Mud_Emotion::eAngry_1 || !field_16A_flags.Get(Flags_16A::eBit8_do_angry))
                {
                    field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe;
                    return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
                }
                else
                {
                    field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
                    field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe;
                    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
                    return Brain_4_ListeningToAbe::eBrain4_Knockback_14;
                }
            }
        }
        break;

        case Brain_1_Chisle::eBrain1_SadNoise_4:
            if (mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                return field_190_brain_sub_state;
            }
            field_16A_flags.Clear(Flags_16A::eBit6_standing_for_sad_or_angry);
            mNextMotion = eMudMotions::Motion_17_StandToCrouch;
            return Brain_1_Chisle::eBrain1_StartToChisle_0;

        case Brain_1_Chisle::eBrain1_Duck_5:
            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                mNextMotion = eMudMotions::Motion_17_StandToCrouch;
                return field_190_brain_sub_state;
            }

            if (mCurrentMotion == eMudMotions::Motion_11_Chisel)
            {
                mNextMotion = eMudMotions::Motion_15_CrouchIdle;
                return field_190_brain_sub_state;
            }

            if (mCurrentMotion != eMudMotions::Motion_15_CrouchIdle)
            {
                return field_190_brain_sub_state;
            }

            mNextMotion = eMudMotions::Motion_53_Duck;
            field_194_timer = sGnFrame + 60;
            return Brain_1_Chisle::eBrain1_OutOfDuck_6;

        case Brain_1_Chisle::eBrain1_OutOfDuck_6:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::Motion_15_CrouchIdle;
            return Brain_1_Chisle::eBrain1_DuckToChisle_7;

        case Brain_1_Chisle::eBrain1_DuckToChisle_7:
            if (mCurrentMotion == eMudMotions::Motion_11_Chisel)
            {
                return Brain_1_Chisle::eBrain1_StartToChisle_0;
            }
            if (mCurrentMotion != eMudMotions::Motion_15_CrouchIdle)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::Motion_11_Chisel;
            return field_190_brain_sub_state;

        case Brain_1_Chisle::eBrain1_DuckKnockback_8:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::Motion_55_DuckKnockback;
            field_194_timer = sGnFrame + 60;
            return Brain_1_Chisle::eBrain1_OutOfDuck_6;

        default:
            return field_190_brain_sub_state;
    }
}

s16 Mudokon::Brain_2_CrouchScrub()
{
    if (FindBirdPortal())
    {
        AddAlerted();
        field_18E_brain_state = Mud_Brain_State::Brain_6_Escape;
        return Brain_6_Escape::eBrain6_PortalOppened_0;
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak();
    if (field_16C_flags.Get(Flags_16C::eBit2_Unknown))
    {
        if (field_190_brain_sub_state != Brain_2_CrouchScrub::eBrain2_StandUp_3)
        {
            sIsMudStandingUp_5C3018 = 0;
            field_16C_flags.Clear(Flags_16C::eBit2_Unknown);
        }
    }

    if (lastSpeak == GameSpeakEvents::Slig_LookOut_6 || EventGet(kEventShooting))
    {
        mNextMotion = eMudMotions::Motion_53_Duck;
        field_194_timer = sGnFrame + 60;
        return Brain_2_CrouchScrub::eBrain2_Duck_6;
    }

    IBaseAnimatedWithPhysicsGameObject* pAbuse = IsEventInRange(
        kEventMudokonAbuse,
        mXPos,
        mYPos,
        AsEventScale(mScale));
    if (pAbuse)
    {
        if (pAbuse != this)
        {
            if (field_190_brain_sub_state != Brain_2_CrouchScrub::eBrain2_StandUp_3
                && field_190_brain_sub_state != Brain_2_CrouchScrub::eBrain2_DuckKnockback_7
                && gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
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

                    field_190_brain_sub_state = Brain_2_CrouchScrub::eBrain2_StandUp_3;
                    field_194_timer = MudResponseDelay() + sGnFrame + 10;
                }
            }
        }
    }

    IBaseAnimatedWithPhysicsGameObject* pDied = IsEventInRange(
        kEventMudokonDied,
        mXPos,
        mYPos,
        AsEventScale(mScale));
    if (pDied)
    {
        if (pDied != this)
        {
            if (field_190_brain_sub_state != Brain_2_CrouchScrub::eBrain2_StandUp_3
                && field_190_brain_sub_state != Brain_2_CrouchScrub::eBrain2_DuckKnockback_7
                && gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
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

                    field_190_brain_sub_state = Brain_2_CrouchScrub::eBrain2_StandUp_3;
                    field_194_timer = MudResponseDelay() + sGnFrame + 10;
                }
            }
        }
    }

    IBaseAnimatedWithPhysicsGameObject* pLoudNoise = IsEventInRange(
        kEventLoudNoise,
        mXPos,
        mYPos,
        AsEventScale(mScale));
    if (pLoudNoise)
    {
        if (pLoudNoise->Type() == ReliveTypes::eGlukkon)
        {
            if (field_190_brain_sub_state != Brain_2_CrouchScrub::eBrain2_StandUp_3
                && field_190_brain_sub_state != Brain_2_CrouchScrub::eBrain2_DuckKnockback_7
                && gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
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

                    field_190_brain_sub_state = Brain_2_CrouchScrub::eBrain2_StandUp_3;
                    field_194_timer = MudResponseDelay() + sGnFrame + 10;
                }
            }
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (LaughingGasInCurrentScreen())
        {
            field_180_emo_tbl = Mud_Emotion::eWired_6;

            AddAlerted();
            field_190_brain_sub_state = Brain_2_CrouchScrub::eBrain2_StandUp_3;
            field_194_timer = MudResponseDelay() + sGnFrame + 10;
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
                field_190_brain_sub_state = Brain_2_CrouchScrub::eBrain2_StandUp_3;
                field_194_timer = sGnFrame;
            }
        }
    }

    switch (field_190_brain_sub_state)
    {
        case Brain_2_CrouchScrub::eBrain2_StartToScrub_0:
            mCurrentMotion = eMudMotions::Motion_14_CrouchScrub;
            mNextMotion = -1;
            field_194_timer = Math_NextRandom() % 64 + sGnFrame + 15;
            field_124 = Math_NextRandom() % 64 + sGnFrame + 240;
            return Brain_2_CrouchScrub::eBrain2_Scrubbing_2;

        case Brain_2_CrouchScrub::eBrain2_SmallBreak_1:
        {
            bool checkAlerted = true;
            if (lastSpeak == GameSpeakEvents::eHello_9)
            {
                if (sIsMudStandingUp_5C3018 || !IAmNearestToAbe())
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
                if (sActiveHero->mSpriteScale == mSpriteScale)
                {
                    AddAlerted();
                    sIsMudStandingUp_5C3018 = 1;
                    field_16C_flags.Set(Flags_16C::eBit2_Unknown);
                    field_194_timer = MudResponseDelay() + sGnFrame + 10;
                    return Brain_2_CrouchScrub::eBrain2_StandUp_3;
                }
            }

            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                mNextMotion = eMudMotions::Motion_14_CrouchScrub;
            }

            if (mCurrentMotion == eMudMotions::Motion_15_CrouchIdle && static_cast<s32>(sGnFrame) > field_124)
            {
                field_124 = Math_NextRandom() % 64 + sGnFrame + 240;
                mNextMotion = eMudMotions::Motion_16_CrouchTurn;
            }

            if (mCurrentMotion != eMudMotions::Motion_15_CrouchIdle || static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                if (mCurrentMotion != eMudMotions::Motion_14_CrouchScrub)
                {
                    return field_190_brain_sub_state;
                }
                return Brain_2_CrouchScrub::eBrain2_Scrubbing_2;
            }

            if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && Math_NextRandom() < 120)
            {
                AddAlerted();
                field_16A_flags.Set(Flags_16A::eBit14_make_sad_noise);
                field_16A_flags.Set(Flags_16A::eBit6_standing_for_sad_or_angry);
                field_194_timer = MudResponseDelay() + sGnFrame + 10;
                return Brain_2_CrouchScrub::eBrain2_StandUp_3;
            }
            else
            {
                field_194_timer = Math_NextRandom() % 64 + sGnFrame + 35;
                mNextMotion = eMudMotions::Motion_14_CrouchScrub;
                if (mCurrentMotion != eMudMotions::Motion_14_CrouchScrub)
                {
                    return field_190_brain_sub_state;
                }
                return Brain_2_CrouchScrub::eBrain2_Scrubbing_2;
            }
            break;
        }

        case Brain_2_CrouchScrub::eBrain2_Scrubbing_2:
        {
            bool checkAlerted = true;
            if (lastSpeak == GameSpeakEvents::eHello_9)
            {
                if (sIsMudStandingUp_5C3018 || !IAmNearestToAbe())
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
                if (sActiveHero->mSpriteScale == mSpriteScale)
                {
                    AddAlerted();
                    sIsMudStandingUp_5C3018 = 1;
                    field_16C_flags.Set(Flags_16C::eBit2_Unknown);
                    field_194_timer = MudResponseDelay() + sGnFrame + 10;
                    return Brain_2_CrouchScrub::eBrain2_StandUp_3;
                }
            }

            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                mNextMotion = eMudMotions::Motion_14_CrouchScrub;
            }

            bool bUnknown = false;
            if (mCurrentMotion != eMudMotions::Motion_15_CrouchIdle)
            {
                bUnknown = true;
            }
            else
            {
                if (static_cast<s32>(sGnFrame) <= field_194_timer)
                {
                    mCurrentMotion = eMudMotions::Motion_14_CrouchScrub;
                    bUnknown = true;
                }
            }

            if (bUnknown)
            {
                if (!IsEventInRange(kEventSpeaking, mXPos, mYPos, AsEventScale(mScale)))
                {
                    return field_190_brain_sub_state;
                }
                field_194_timer = Math_NextRandom() % 64 + sGnFrame + 15;
                return Brain_2_CrouchScrub::eBrain2_SmallBreakThroughGameSpeak_5;
            }
            else
            {
                field_194_timer = Math_NextRandom() % 64 + sGnFrame + 15;
                return Brain_2_CrouchScrub::eBrain2_SmallBreak_1;
            }
            break;
        }

        case Brain_2_CrouchScrub::eBrain2_StandUp_3:
        {
            if (lastSpeak == GameSpeakEvents::eHello_9 || lastSpeak == GameSpeakEvents::eAllYa_23)
            {
                bool checkAlerted = lastSpeak == GameSpeakEvents::eHello_9;
                if (lastSpeak == GameSpeakEvents::eAllYa_23)
                {
                    if (!sIsMudStandingUp_5C3018 && IAmNearestToAbe())
                    {
                        checkAlerted = true;
                    }
                }

                if (checkAlerted)
                {
                    if (sActiveHero->mSpriteScale == mSpriteScale)
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

            if (mCurrentMotion == eMudMotions::Motion_53_Duck)
            {
                mNextMotion = eMudMotions::Motion_15_CrouchIdle;
            }

            if (mCurrentMotion == eMudMotions::Motion_15_CrouchIdle)
            {
                mNextMotion = eMudMotions::Motion_18_CrouchToStand;
            }

            if (mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                return field_190_brain_sub_state;
            }

            if (field_180_emo_tbl == Mud_Emotion::eSad_3 && field_16A_flags.Get(Flags_16A::eBit14_make_sad_noise))
            {
                field_16A_flags.Clear(Flags_16A::eBit14_make_sad_noise);
                field_16A_flags.Clear(Flags_16A::eBit12_alert_enemies);
                mNextMotion = eMudMotions::M_Speak_6_472FA0;
                MudEmotionSound(MudSounds::eSadUgh_28);
                return Brain_2_CrouchScrub::eBrain2_SadNoise_4;
            }
            else
            {
                sIsMudStandingUp_5C3018 = 0;
                field_16C_flags.Clear(Flags_16C::eBit2_Unknown);
                if (field_180_emo_tbl != Mud_Emotion::eAngry_1 || !field_16A_flags.Get(Flags_16A::eBit8_do_angry))
                {
                    field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe;
                    return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
                }
                else
                {
                    field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
                    field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe;
                    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
                    return Brain_4_ListeningToAbe::eBrain4_Knockback_14;
                }
            }
            break;
        }

        case Brain_2_CrouchScrub::eBrain2_SadNoise_4:
            if (mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                return field_190_brain_sub_state;
            }
            field_16A_flags.Clear(Flags_16A::eBit6_standing_for_sad_or_angry);
            mNextMotion = eMudMotions::Motion_17_StandToCrouch;
            return Brain_2_CrouchScrub::eBrain2_SmallBreak_1;

        case Brain_2_CrouchScrub::eBrain2_SmallBreakThroughGameSpeak_5:
        {
            bool checkAlerted = true;
            if (lastSpeak == GameSpeakEvents::eHello_9)
            {
                if (sIsMudStandingUp_5C3018 || !IAmNearestToAbe())
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
                if (sActiveHero->mSpriteScale == mSpriteScale)
                {
                    AddAlerted();
                    sIsMudStandingUp_5C3018 = 1;
                    field_16C_flags.Set(Flags_16C::eBit2_Unknown);
                    field_194_timer = MudResponseDelay() + sGnFrame + 10;
                    return Brain_2_CrouchScrub::eBrain2_StandUp_3;
                }
            }

            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }
            return Brain_2_CrouchScrub::eBrain2_SmallBreak_1;
        }

        case Brain_2_CrouchScrub::eBrain2_Duck_6:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::Motion_15_CrouchIdle;
            field_194_timer = Math_NextRandom() % 64 + sGnFrame + 15;
            field_124 = Math_NextRandom() % 64 + sGnFrame + 240;
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(1);
            }
            return Brain_2_CrouchScrub::eBrain2_Scrubbing_2;

        case Brain_2_CrouchScrub::eBrain2_DuckKnockback_7:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::Motion_55_DuckKnockback;
            field_194_timer = sGnFrame + 60;
            return Brain_2_CrouchScrub::eBrain2_Duck_6;

        default:
            return field_190_brain_sub_state;
    }
}

s16 Mudokon::Brain_3_TurnWheel()
{
    if (FindBirdPortal())
    {
        AddAlerted();
        field_18E_brain_state = Mud_Brain_State::Brain_6_Escape;
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_6_Escape::eBrain6_PortalOppened_0;
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak();
    if (field_16C_flags.Get(Flags_16C::eBit2_Unknown))
    {
        if (field_190_brain_sub_state != Brain_3_TurnWheel::eBrain3_InterruptAction_2)
        {
            sIsMudStandingUp_5C3018 = 0;
            field_16C_flags.Clear(Flags_16C::eBit2_Unknown);
        }
    }

    if (lastSpeak == GameSpeakEvents::Slig_LookOut_6 || EventGet(kEventShooting))
    {
        if (field_190_brain_sub_state != Brain_3_TurnWheel::eBrain3_TurningWheelToDuck_3 && field_190_brain_sub_state != Brain_3_TurnWheel::eBrain3_Duck_4)
        {
            field_190_brain_sub_state = Brain_3_TurnWheel::eBrain3_TurningWheelToDuck_3;
        }
    }

    IBaseAnimatedWithPhysicsGameObject* pMudAbuseEvent = IsEventInRange(
        kEventMudokonAbuse,
        mXPos,
        mYPos,
        AsEventScale(mScale));

    if (pMudAbuseEvent)
    {
        if (pMudAbuseEvent != this
            && field_190_brain_sub_state != Brain_3_TurnWheel::eBrain3_InterruptAction_2
            && gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
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

                mNextMotion = eMudMotions::Motion_0_Idle;
                field_190_brain_sub_state = Brain_3_TurnWheel::eBrain3_InterruptAction_2;
                field_194_timer = MudResponseDelay() + sGnFrame + 10;
            }
        }
    }

    IBaseAnimatedWithPhysicsGameObject* pLoudNoiseEvent = IsEventInRange(
        kEventLoudNoise,
        mXPos,
        mYPos,
        AsEventScale(mScale));
    if (pLoudNoiseEvent)
    {
        if (pLoudNoiseEvent->Type() == ReliveTypes::eGlukkon
            && field_190_brain_sub_state != Brain_3_TurnWheel::eBrain3_InterruptAction_2
            && gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
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

                mNextMotion = eMudMotions::Motion_0_Idle;
                field_190_brain_sub_state = Brain_3_TurnWheel::eBrain3_InterruptAction_2;
                field_194_timer = MudResponseDelay() + sGnFrame + 10;
            }
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (LaughingGasInCurrentScreen())
        {
            field_180_emo_tbl = Mud_Emotion::eWired_6;
            AddAlerted();
            mNextMotion = eMudMotions::Motion_0_Idle;
            field_194_timer = MudResponseDelay() + sGnFrame + 10;
            field_190_brain_sub_state = Brain_3_TurnWheel::eBrain3_InterruptAction_2;
        }
    }

    switch (field_190_brain_sub_state)
    {
        case Brain_3_TurnWheel::eBrain3_TurningWheel_0:
            if (lastSpeak == GameSpeakEvents::eHello_9)
            {
                if (sIsMudStandingUp_5C3018 || !IAmNearestToAbe())
                {
                    return TurningWheelHelloOrAllYaResponse();
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAllYa_23)
            {
                return TurningWheelHelloOrAllYaResponse();
            }

            if (sActiveHero->mSpriteScale == mSpriteScale)
            {
                AddAlerted();
                mNextMotion = eMudMotions::Motion_0_Idle;
                field_194_timer = MudResponseDelay() + sGnFrame + 10;
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

            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                mNextMotion = eMudMotions::Motion_17_StandToCrouch;
            }
            else
            {
                if (mCurrentMotion != eMudMotions::Motion_17_StandToCrouch && mCurrentMotion != eMudMotions::Motion_15_CrouchIdle)
                {
                    mNextMotion = eMudMotions::Motion_0_Idle;
                }

                if (mCurrentMotion == eMudMotions::Motion_0_Idle)
                {
                    mNextMotion = eMudMotions::Motion_17_StandToCrouch;
                }
            }

            if (mCurrentMotion != eMudMotions::Motion_15_CrouchIdle)
            {
                return field_190_brain_sub_state;
            }

            if (field_10_resources_array.ItemAt(2))
            {
                field_18E_brain_state = Mud_Brain_State::Brain_1_Chisel;
            }
            else
            {
                field_18E_brain_state = Mud_Brain_State::Brain_2_CrouchScrub;
            }
            return Brain_3_TurnWheel::eBrain3_TurningWheel_0;

        case Brain_3_TurnWheel::eBrain3_InterruptAction_2:
            field_16A_flags.Clear(Flags_16A::eBit7_stopped_at_wheel);
            if (static_cast<s32>(sGnFrame) <= field_194_timer || mCurrentMotion)
            {
                if (mNextMotion != -1 || mCurrentMotion != eMudMotions::Motion_58_TurnWheelLoop)
                {
                    return field_190_brain_sub_state;
                }
                mNextMotion = eMudMotions::Motion_0_Idle;
                return field_190_brain_sub_state;
            }
            else
            {
                field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe;
                sIsMudStandingUp_5C3018 = 0;
                field_16C_flags.Clear(Flags_16C::eBit2_Unknown);
                return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
            }
            break;

        case Brain_3_TurnWheel::eBrain3_TurningWheelToDuck_3:
            if (mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                if (mCurrentMotion != eMudMotions::Motion_15_CrouchIdle)
                {
                    if (mCurrentMotion != eMudMotions::Motion_58_TurnWheelLoop)
                    {
                        return field_190_brain_sub_state;
                    }
                    else
                    {
                        mNextMotion = eMudMotions::Motion_0_Idle;
                        return field_190_brain_sub_state;
                    }
                }
                else
                {
                    mNextMotion = eMudMotions::Motion_53_Duck;
                    field_194_timer = sGnFrame + 60;
                    return Brain_3_TurnWheel::eBrain3_Duck_4;
                }
            }
            else
            {
                mNextMotion = eMudMotions::Motion_15_CrouchIdle;
                return field_190_brain_sub_state;
            }
            break;

        case Brain_3_TurnWheel::eBrain3_Duck_4:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::Motion_15_CrouchIdle;
            return Brain_3_TurnWheel::eBrain3_DuckToTurningWheel_5;

        case Brain_3_TurnWheel::eBrain3_DuckToTurningWheel_5:
            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                mNextMotion = eMudMotions::Motion_57_TurnWheelBegin;
                return field_190_brain_sub_state;
            }

            if (mCurrentMotion == eMudMotions::Motion_15_CrouchIdle)
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
                return field_190_brain_sub_state;
            }
            else
            {
                if (mCurrentMotion != eMudMotions::Motion_58_TurnWheelLoop)
                {
                    return field_190_brain_sub_state;
                }
                return Brain_3_TurnWheel::eBrain3_TurningWheel_0;
            }
            break;
    }
    return field_190_brain_sub_state;
}

s16 Mudokon::Brain_4_ListeningToAbe()
{
    if (FindBirdPortal())
    {
        field_18E_brain_state = Mud_Brain_State::Brain_6_Escape;
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_6_Escape::eBrain6_PortalOppened_0;
    }

    if (IsEventInRange(
            kEventMudokonAbuse,
            mXPos,
            mYPos,
            AsEventScale(mScale)))
    {
        field_17E_delayed_speak = MudAction::eMudAbuse_9;
    }

    if (IsEventInRange(
            kEventMudokonDied,
            mXPos,
            mYPos,
            AsEventScale(mScale)))
    {
        field_17E_delayed_speak = MudAction::eMudDied_14;
    }

    if (IsEventInRange(
            kEventMudokonComfort,
            mXPos,
            mYPos,
            AsEventScale(mScale)))
    {
        field_17E_delayed_speak = MudAction::eComfort_10;
    }

    if (IsEventInRange(
            kEventMudokonLaugh,
            mXPos,
            mYPos,
            AsEventScale(mScale)))
    {
        field_17E_delayed_speak = MudAction::eLaugh_12;
    }

    if (IsEventInRange(kEventAbeDead,
            mXPos,
            mYPos,
                                AsEventScale(mScale)))
    {
        field_17E_delayed_speak = MudAction::eDuck_13;
    }

    IBaseAnimatedWithPhysicsGameObject* pNoiseEvent = IsEventInRange(
        kEventLoudNoise,
        mXPos,
        mYPos,
        AsEventScale(mScale));

    if (pNoiseEvent)
    {
        if (pNoiseEvent->Type() == ReliveTypes::eGlukkon)
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
        if (LaughingGasInCurrentScreen())
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

                if (sPathInfo->TLV_Get_At_4DB4B0(
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos - FP_FromInteger(5)),
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos - FP_FromInteger(5)),
                        ReliveTypes::eWater))
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
    ToStand();
    field_178_brain_sub_state2 = -1;
    return Brain_4_ListeningToAbe::eBrain4_GetsAttentive_1;
}

s16 Mudokon::Brain_ListeningToAbe_State_1()
{
    if (mCurrentMotion != eMudMotions::Motion_0_Idle)
    {
        return field_190_brain_sub_state;
    }

    if (!VIsFacingMe(sActiveHero))
    {
        mNextMotion = eMudMotions::Motion_2_StandingTurn;
        return field_190_brain_sub_state;
    }

    switch (field_180_emo_tbl)
    {
        case Mud_Emotion::eNormal_0:
            MudEmotionSound(MudSounds::eHelloNeutral_3);
            break;

        case Mud_Emotion::eAngry_1:
        case Mud_Emotion::eAggressive_2:
            if (field_16A_flags.Get(Flags_16A::eBit6_standing_for_sad_or_angry))
            {
                MudEmotionSound(MudSounds::eAnger_5);
            }
            else
            {
                MudEmotionSound(MudSounds::eHiAngry_18);
            }
            break;

        case Mud_Emotion::eSad_3:
        case Mud_Emotion::eSick_7:
            if (field_16A_flags.Get(Flags_16A::eBit6_standing_for_sad_or_angry))
            {
                MudEmotionSound(MudSounds::eNoSad_22);
            }
            else
            {
                MudEmotionSound(MudSounds::eHiSad_20);
            }
            break;

        case Mud_Emotion::eSuicidal_4:
            MudEmotionSound(MudSounds::eNoSad_22);
            break;

        case Mud_Emotion::eHappy_5:
        case Mud_Emotion::eWired_6:
            MudEmotionSound(MudSounds::eLaugh_10);
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
        field_184_next_motion2 = eMudMotions::Motion_11_Chisel;
    }
    else
    {
        field_184_next_motion2 = eMudMotions::Motion_15_CrouchIdle;
    }
    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
    field_16A_flags.Clear(Flags_16A::eBit5_following);
    return Brain_4_ListeningToAbe::eBrain4_InitializeMovement_10;
}

s16 Mudokon::Brain_ListeningToAbe_State_2()
{
    if (!VIsFacingMe(sActiveHero))
    {
        mNextMotion = eMudMotions::Motion_2_StandingTurn;
        return field_190_brain_sub_state;
    }

    if (mCurrentMotion != eMudMotions::Motion_0_Idle)
    {
        if (mCurrentMotion == eMudMotions::Motion_1_WalkLoop)
        {
            if (field_16A_flags.Get(Flags_16A::eBit4_blind) || !VIsObjNearby(ScaleToGridSize(mSpriteScale), sActiveHero))
            {
                return Brain_4_ListeningToAbe::eBrain4_Walking_5;
            }
        }
        else
        {
            if (mCurrentMotion != eMudMotions::Motion_46_Knockback)
            {
                return field_190_brain_sub_state;
            }
        }
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }
    else
    {
        mNextMotion = eMudMotions::Motion_1_WalkLoop;
    }
    return field_190_brain_sub_state;
}

s16 Mudokon::Brain_ListeningToAbe_State_3()
{
    if (mCurrentMotion != eMudMotions::Motion_0_Idle)
    {
        if (mCurrentMotion != eMudMotions::Motion_1_WalkLoop)
        {
            return field_190_brain_sub_state;
        }
        mNextMotion = eMudMotions::Motion_0_Idle;

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
            mNextMotion = eMudMotions::Motion_2_StandingTurn;
            return field_190_brain_sub_state;
        }
        mNextMotion = eMudMotions::Motion_1_WalkLoop;
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
        if (mCurrentMotion == eMudMotions::Motion_21_RunLoop)
        {
            mNextMotion = eMudMotions::Motion_0_Idle;
        }
        if (mYPos > pScreenManager->CamYPos() + FP_FromInteger(260) && field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
            if (field_10_resources_array.ItemAt(2))
            {
                field_16A_flags.Clear(Flags_16A::eBit5_following);
                mNextMotion = eMudMotions::Motion_11_Chisel;
                return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
            }
            else
            {
                field_16A_flags.Clear(Flags_16A::eBit5_following);
                mNextMotion = eMudMotions::Motion_15_CrouchIdle;
                return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
            }
        }

        FP scaleToGridSizeAbs = {};
        if (sActiveHero->mXPos >= mXPos)
        {
            scaleToGridSizeAbs = ScaleToGridSize(mSpriteScale);
        }
        else
        {
            scaleToGridSizeAbs = -ScaleToGridSize(mSpriteScale);
        }
        const s32 v44 = WallHit(mSpriteScale * FP_FromInteger(50), scaleToGridSizeAbs);

        if (mCurrentMotion != eMudMotions::Motion_0_Idle)
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
                    mNextMotion = eMudMotions::Motion_2_StandingTurn;
                    return field_190_brain_sub_state;
                }

                const FP v48 = ScaleToGridSize(mSpriteScale) * FP_FromInteger(2);
                if ((VIsObjNearby(v48, sActiveHero) && !(field_16A_flags.Get(Flags_16A::eBit4_blind))) || v44 || FindWheel(mXPos, mYPos))
                {
                    const GameSpeakEvents lastSpeak_1 = LastGameSpeak();
                    if (field_17E_delayed_speak != MudAction::eNone_17)
                    {
                        s16 v18 = GetBrainSubStateResponse(field_17E_delayed_speak);
                        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
                        {
                            field_194_timer = sGnFrame + 20;
                        }
                        else
                        {
                            field_194_timer = (u16) MudResponseDelay() + sGnFrame + 20;
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
                        field_194_timer = (u16) MudResponseDelay() + sGnFrame + 20;
                        switch (lastSpeak_1)
                        {
                            case GameSpeakEvents::eFart_3:
                            {
                                s16 v18 = GetBrainSubStateResponse(MudAction::eFart_6);
                                if (VIsObjNearby(ScaleToGridSize(mSpriteScale), sActiveHero))
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
                                s16 result = GetBrainSubStateResponse(MudAction::eHelloOrAllYa_0);
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
                                s16 result = GetBrainSubStateResponse(MudAction::eFollowMe_1);
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
                                s16 result = GetBrainSubStateResponse(MudAction::eWait_2);
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
                                if (FindWheel(mXPos, mYPos))
                                {
                                    Mudokon* v35 = static_cast<Mudokon*>(FindObjectOfType(ReliveTypes::eMudokon, mXPos, mYPos - FP_FromInteger(5)));
                                    if (!v35)
                                    {
                                        return BrainStartWheelTurning();
                                    }

                                    while (!(v35->field_16A_flags.Get(Flags_16A::eBit7_stopped_at_wheel)) && v35->mCurrentMotion != eMudMotions::Motion_58_TurnWheelLoop)
                                    {
                                        v35 = static_cast<Mudokon*>(GetStackedSlapTarget(v35->mBaseGameObjectId, ReliveTypes::eMudokon, mXPos, mYPos - FP_FromInteger(5)));
                                        if (!v35)
                                        {
                                            return BrainStartWheelTurning();
                                        }
                                    }
                                    if (v35)
                                    {
                                        if (field_10_resources_array.ItemAt(2))
                                        {
                                            mNextMotion = eMudMotions::Motion_11_Chisel;
                                            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                                        }
                                        else
                                        {
                                            mNextMotion = eMudMotions::Motion_15_CrouchIdle;
                                            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                                        }
                                    }
                                    return BrainStartWheelTurning();
                                }

                                if (FindObjectOfType(ReliveTypes::eLever, ScaleToGridSize(mSpriteScale) + mXPos, mYPos - FP_FromInteger(5)))
                                {
                                    return Brain_4_ListeningToAbe::eBrain4_PullingLever_11;
                                }
                                if (FindObjectOfType(ReliveTypes::eLever, mXPos - ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5)))
                                {
                                    return Brain_4_ListeningToAbe::eBrain4_PullingLever_11;
                                }

                                if (field_10_resources_array.ItemAt(2))
                                {
                                    mNextMotion = eMudMotions::Motion_11_Chisel;
                                    return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                                }
                                else
                                {
                                    mNextMotion = eMudMotions::Motion_15_CrouchIdle;
                                    return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                                }
                                break;

                            case GameSpeakEvents::eStopIt_22:
                            {
                                s16 v18 = GetBrainSubStateResponse(MudAction::eStopIt_4);
                                field_194_timer = ((u32)(u16) MudResponseDelay() >> 1) + sGnFrame + 20;
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
                    field_194_timer = sGnFrame + (u16) Mudokon::MudResponseDelay();
                    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                    {
                        field_184_next_motion2 = eMudMotions::Motion_21_RunLoop;
                        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Running_6;
                        return Brain_4_ListeningToAbe::eBrain4_InitializeMovement_10;
                    }
                    else if (field_16A_flags.Get(Flags_16A::eBit4_blind) || !CanAbeSneak())
                    {
                        field_184_next_motion2 = eMudMotions::Motion_1_WalkLoop;
                        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Walking_5;
                        return Brain_4_ListeningToAbe::eBrain4_InitializeMovement_10;
                    }
                    else
                    {
                        field_184_next_motion2 = eMudMotions::Motion_27_SneakLoop;
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
                    mNextMotion = eMudMotions::Motion_15_CrouchIdle;
                    return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                }
                else
                {
                    mNextMotion = eMudMotions::Motion_11_Chisel;
                    return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                }
            }
        }
    }
}

s16 Mudokon::Brain_ListeningToAbe_State_5()
{
    if (mCurrentMotion == eMudMotions::Motion_0_Idle && mNextMotion != eMudMotions::Motion_1_WalkLoop)
    {
        if (!field_16A_flags.Get(Flags_16A::eBit4_blind))
        {
            return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
        }
        mNextMotion = eMudMotions::Motion_1_WalkLoop;
    }

    if (mCurrentMotion == eMudMotions::Motion_46_Knockback)
    {
        MudEmotionSound(MudSounds::eHurt1_16);
        s16 result = GetBrainSubStateResponse(MudAction::eSmackIntoWall_11);
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

    if (mCurrentMotion != eMudMotions::Motion_1_WalkLoop && mCurrentMotion != eMudMotions::Motion_27_SneakLoop)
    {
        return field_190_brain_sub_state;
    }

    FP v65 = {};
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        v65 = -ScaleToGridSize(mSpriteScale);
    }
    else
    {
        v65 = ScaleToGridSize(mSpriteScale);
    }

    const s32 v67 = WallHit(mSpriteScale * FP_FromInteger(50), v65);
    if (field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        if (!gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0)
            && !VIsFacingMe(sActiveHero))
        {
            const s32 v68 = FP_GetExponent(mXPos) % 375;
            if (v68 > 10u && v68 < 362u)
            {
                field_16A_flags.Clear(Flags_16A::eBit5_following);
                mNextMotion = eMudMotions::Motion_0_Idle;
                return Brain_4_ListeningToAbe::eBrain4_Idle_7;
            }
        }
    }
    if (v67 && !field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }

    if (StopAtWheel())
    {
        field_16A_flags.Set(Flags_16A::eBit7_stopped_at_wheel);
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }

    if (!VIsFacingMe(sActiveHero) && !field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }

    if (VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(2), sActiveHero) && !field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }

    if (LastGameSpeak() == GameSpeakEvents::eWait_12)
    {
        field_16A_flags.Clear(Flags_16A::eBit5_following);
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_4_ListeningToAbe::eBrain4_GetsWaitWhileMoving_9;
    }

    bool bWalkOrSneak = false;
    if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_33_RunLoop_4508E0 && sActiveHero->mCurrentMotion != eAbeMotions::Motion_23_RollLoop_453A90 && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        bWalkOrSneak = true;
    }
    else if (mCurrentMotion == eMudMotions::Motion_27_SneakLoop || field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        mNextMotion = eMudMotions::Motion_1_WalkLoop;
        bWalkOrSneak = true;
    }

    if (bWalkOrSneak)
    {
        if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_40_SneakLoop_450550
            && mCurrentMotion == eMudMotions::Motion_1_WalkLoop
            && !field_16A_flags.Get(Flags_16A::eBit4_blind))
        {
            mNextMotion = eMudMotions::Motion_27_SneakLoop;
        }
        if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_1_WalkLoop_44FBA0 || mCurrentMotion != eMudMotions::Motion_27_SneakLoop)
        {
            return field_190_brain_sub_state;
        }
        mNextMotion = eMudMotions::Motion_1_WalkLoop;
        return field_190_brain_sub_state;
    }
    else
    {
        mNextMotion = eMudMotions::Motion_21_RunLoop;
        return Brain_4_ListeningToAbe::eBrain4_Running_6;
    }
}

s16 Mudokon::Brain_ListeningToAbe_State_6()
{
    if (mCurrentMotion == eMudMotions::Motion_0_Idle)
    {
        if (!field_16A_flags.Get(Flags_16A::eBit4_blind))
        {
            return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
        }
        mNextMotion = eMudMotions::Motion_21_RunLoop;
    }

    if (mCurrentMotion == eMudMotions::Motion_46_Knockback)
    {
        MudEmotionSound(MudSounds::eHurt1_16);
        s16 result = GetBrainSubStateResponse(MudAction::eSmackIntoWall_11);
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
    if (mCurrentMotion != eMudMotions::Motion_21_RunLoop)
    {
        return field_190_brain_sub_state;
    }

    FP scaleToGridSizeAbs;
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        scaleToGridSizeAbs = -ScaleToGridSize(mSpriteScale);
    }
    else
    {
        scaleToGridSizeAbs = ScaleToGridSize(mSpriteScale);
    }

    if (WallHit(mSpriteScale * FP_FromInteger(50), scaleToGridSizeAbs * FP_FromInteger(3)) && !field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        mNextMotion = eMudMotions::Motion_0_Idle;
        if (field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
            return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
        }
        field_194_timer = MudResponseDelay() + sGnFrame + 20;

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
            mNextMotion = eMudMotions::Motion_1_WalkLoop;
            return Brain_4_ListeningToAbe::eBrain4_Walking_5;
        }
        if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_25_RunSlideStop_451330 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_0_Idle_44EEB0 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_71_Knockback_455090 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_17_CrouchIdle_456BC0)
        {
            mNextMotion = eMudMotions::Motion_0_Idle;
            field_194_timer = MudResponseDelay() + sGnFrame + 20;
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
            if (VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(4), sActiveHero))
            {
                mNextMotion = eMudMotions::Motion_1_WalkLoop;
                return Brain_4_ListeningToAbe::eBrain4_Walking_5;
            }
        }

        if (LastGameSpeak() == GameSpeakEvents::eWait_12)
        {
            field_16A_flags.Clear(Flags_16A::eBit5_following);
            mNextMotion = eMudMotions::Motion_0_Idle;
            return Brain_4_ListeningToAbe::eBrain4_GetsWaitWhileMoving_9;
        }
    }

    if (!VIsObjNearby(ScaleToGridSize(mSpriteScale), sActiveHero))
    {
        if (VIsFacingMe(sActiveHero))
        {
            return field_190_brain_sub_state;
        }
    }
    else
    {
        if (!VIsObj_GettingNear_On_X(sActiveHero))
        {
            if (VIsFacingMe(sActiveHero))
            {
                return field_190_brain_sub_state;
            }
        }
    }

    if (!field_16A_flags.Get(Flags_16A::eBit4_blind))
    {
        mNextMotion = eMudMotions::Motion_25_RunSlideTurn;
        return Brain_4_ListeningToAbe::eBrain4_CrazySlideTurn_8;
    }

    return field_190_brain_sub_state;
}

s16 Mudokon::Brain_ListeningToAbe_State_7()
{
    if (mYPos > pScreenManager->CamYPos() + FP_FromInteger(260) && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (field_10_resources_array.ItemAt(2))
        {
            field_16A_flags.Clear(Flags_16A::eBit5_following);
            mNextMotion = eMudMotions::Motion_11_Chisel;
            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
        }
        else
        {
            field_16A_flags.Clear(Flags_16A::eBit5_following);
            mNextMotion = eMudMotions::Motion_15_CrouchIdle;
            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
        }
    }

    if (mCurrentMotion != eMudMotions::Motion_0_Idle)
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
            mNextMotion = eMudMotions::Motion_11_Chisel;
            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
        }
        else
        {
            field_16A_flags.Clear(Flags_16A::eBit5_following);
            mNextMotion = eMudMotions::Motion_15_CrouchIdle;
            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
        }
    }

    if (NeedToTurnAround())
    {
        mNextMotion = eMudMotions::Motion_2_StandingTurn;
        return field_190_brain_sub_state;
    }

    if (field_17E_delayed_speak != MudAction::eNone_17)
    {
        s16 v18 = GetBrainSubStateResponse(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
        {
            field_194_timer = sGnFrame + 20;
            field_178_brain_sub_state2 = field_190_brain_sub_state;
        }
        else
        {
            field_178_brain_sub_state2 = field_190_brain_sub_state;
            field_194_timer = MudResponseDelay() + sGnFrame + 20;
        }
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return field_190_brain_sub_state;
        }
        return v18;
    }
    GameSpeakEvents lastSpeak = LastGameSpeak();
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
        field_194_timer = (u16) MudResponseDelay() + sGnFrame + 20;
        switch (lastSpeak)
        {
            case GameSpeakEvents::eFart_3:
            {
                field_182 = GameSpeakEvents::eNone_m1;
                const s16 v18 = GetBrainSubStateResponse(MudAction::eFart_6);
                if (VIsObjNearby(ScaleToGridSize(mSpriteScale), sActiveHero))
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
                s16 result = GetBrainSubStateResponse(MudAction::eHelloOrAllYa_0);
                if (result)
                {
                    return result;
                }
                return field_190_brain_sub_state;
            }

            case GameSpeakEvents::eFollowMe_10:
            {
                const s16 v26 = GetBrainSubStateResponse(MudAction::eFollowMe_1);
                if (field_188_pTblEntry->field_0_sound == MudSounds::eOkay_12 || field_188_pTblEntry->field_0_sound == MudSounds::eLaugh_10)
                {
                    field_16A_flags.Set(Flags_16A::eBit5_following);
                    field_182 = GameSpeakEvents::eNone_m1;
                    // TODO: Wrong ??
                    if (!VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(2), sActiveHero) || VIsObjNearby(ScaleToGridSize(mSpriteScale), sActiveHero))
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
                field_184_next_motion2 = eMudMotions::Motion_43_DunnoBegin;
                return Brain_4_ListeningToAbe::eBrain4_InitializeMovement_10;

            case GameSpeakEvents::eWork_21:
                if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                {
                    return Brain_4_ListeningToAbe::eBrain4_CrazyDeny_19;
                }
                field_16A_flags.Clear(Flags_16A::eBit5_following);
                field_182 = GameSpeakEvents::eNone_m1;

                if (FindWheel(mXPos, mYPos))
                {
                    Mudokon* v35 = static_cast<Mudokon*>(FindObjectOfType(ReliveTypes::eMudokon, mXPos, mYPos - FP_FromInteger(5)));
                    if (!v35)
                    {
                        return BrainStartWheelTurning();
                    }

                    while (!(v35->field_16A_flags.Get(Flags_16A::eBit7_stopped_at_wheel)) && v35->mCurrentMotion != eMudMotions::Motion_58_TurnWheelLoop)
                    {
                        v35 = static_cast<Mudokon*>(GetStackedSlapTarget(v35->mBaseGameObjectId, ReliveTypes::eMudokon, mXPos, mYPos - FP_FromInteger(5)));
                        if (!v35)
                        {
                            return BrainStartWheelTurning();
                        }
                    }
                    if (v35)
                    {
                        if (field_10_resources_array.ItemAt(2))
                        {
                            mNextMotion = eMudMotions::Motion_11_Chisel;
                            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                        }
                        else
                        {
                            mNextMotion = eMudMotions::Motion_15_CrouchIdle;
                            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
                        }
                    }
                    return BrainStartWheelTurning();
                }

                if (FindObjectOfType(ReliveTypes::eLever, ScaleToGridSize(mSpriteScale) + mXPos, mYPos - FP_FromInteger(5)))
                {
                    return Brain_4_ListeningToAbe::eBrain4_PullingLever_11;
                }
                if (FindObjectOfType(ReliveTypes::eLever, mXPos - ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5)))
                {
                    return Brain_4_ListeningToAbe::eBrain4_PullingLever_11;
                }

                break;

            case GameSpeakEvents::eStopIt_22:
            {
                s16 v18 = GetBrainSubStateResponse(MudAction::eStopIt_4);
                field_194_timer = ((u32)(u16) MudResponseDelay() >> 1) + sGnFrame + 20;
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
            mNextMotion = eMudMotions::Motion_11_Chisel;
            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
        }
        else
        {
            mNextMotion = eMudMotions::Motion_15_CrouchIdle;
            return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
        }
    }
}

s16 Mudokon::Brain_ListeningToAbe_State_8()
{
    if (mCurrentMotion == eMudMotions::Motion_21_RunLoop)
    {
        return Brain_4_ListeningToAbe::eBrain4_Running_6;
    }
    if (mCurrentMotion != eMudMotions::Motion_0_Idle)
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
    field_194_timer = MudResponseDelay() + sGnFrame + 20;
    return Brain_4_ListeningToAbe::eBrain4_StopsWhileMoving_20;
}

s16 Mudokon::Brain_ListeningToAbe_State_10()
{
    if (!VIsFacingMe(sActiveHero))
    {
        mNextMotion = eMudMotions::Motion_2_StandingTurn;
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
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        pLever = FindObjectOfType(ReliveTypes::eLever, mXPos - ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5));
    }
    else
    {
        pLever = FindObjectOfType(ReliveTypes::eLever, mXPos + ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5));
    }

    if (!pLever)
    {
        mNextMotion = eMudMotions::Motion_2_StandingTurn;
        return field_190_brain_sub_state;
    }

    if (mCurrentMotion == eMudMotions::Motion_10_LeverUse)
    {
        mNextMotion = -1;
        auto pSwitch = static_cast<Lever*>(pLever);
        pSwitch->VPull(mXPos < pSwitch->mXPos);
        field_16A_flags.Clear(Flags_16A::eBit5_following);

        if (field_10_resources_array.ItemAt(2))
        {
            mNextMotion = eMudMotions::Motion_11_Chisel;
        }
        else
        {
            mNextMotion = eMudMotions::Motion_15_CrouchIdle;
        }
        return Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
    }
    else
    {
        mNextMotion = eMudMotions::Motion_10_LeverUse;
        return field_190_brain_sub_state;
    }
}

s16 Mudokon::Brain_ListeningToAbe_State_12()
{
    if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eMudAbuse_9)
    {
        s16 response = GetBrainSubStateResponse(field_17E_delayed_speak);
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
        field_194_timer = (u16)(MudResponseDelay() + sGnFrame + 20);
        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!response)
        {
            return field_190_brain_sub_state;
        }
        return response;
    }

    if (field_188_pTblEntry->field_2_next_motion == eMudMotions::Motion_38_Punch)
    {
        if (LastGameSpeak() == GameSpeakEvents::eStopIt_22)
        {
            if (mCurrentMotion == eMudMotions::Motion_38_Punch || mNextMotion == eMudMotions::Motion_38_Punch)
            {
                ToStand();
                mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
                mNextMotion = -1;
            }
            s16 result = GetBrainSubStateResponse(MudAction::eStopIt_4);
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
                MudEmotionSound(field_188_pTblEntry->field_0_sound);
            }
        }
        else
        {
            MudEmotionSound(MudSounds::eGiggle_8);
        }
    }
    else
    {
        if (Math_NextRandom() >= 80u)
        {
            MudEmotionSound(MudSounds::eLaugh_10);
        }
        else
        {
            MudEmotionSound(MudSounds::eGiggle_8);
        }
    }

    mNextMotion = field_188_pTblEntry->field_2_next_motion;
    if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags_16A::eBit11_get_depressed))
    {
        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
    }

    if (field_180_emo_tbl == Mud_Emotion::eSick_7)
    {
        field_18E_brain_state = Mud_Brain_State::Brain_9_Sick;
        return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
    }

    if (mNextMotion != eMudMotions::Motion_38_Punch)
    {
        return field_178_brain_sub_state2;
    }

    BaseGameObject* pMudInSameGridBlock = nullptr;
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        if (FindObjectOfType(ReliveTypes::eAbe, mXPos + ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5)))
        {
            return Brain_4_ListeningToAbe::eBrain4_RageTurn_18;
        }
        pMudInSameGridBlock = FindObjectOfType(ReliveTypes::eMudokon, mXPos + ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5));
    }
    else
    {
        if (FindObjectOfType(ReliveTypes::eAbe, mXPos - ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5)))
        {
            return Brain_4_ListeningToAbe::eBrain4_RageTurn_18;
        }
        pMudInSameGridBlock = FindObjectOfType(ReliveTypes::eMudokon, mXPos - ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5));
    }

    if (!pMudInSameGridBlock)
    {
        return field_178_brain_sub_state2;
    }

    return Brain_4_ListeningToAbe::eBrain4_RageTurn_18;
}

s16 Mudokon::Brain_ListeningToAbe_State_13()
{
    ToKnockback();

    mAnim.mFlags.Clear(AnimFlags::eBit18_IsLastFrame);
    mNextMotion = -1;

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        return Brain_4_ListeningToAbe::eBrain4_CrazyLaugh_21;
    }

    if (field_188_pTblEntry->field_2_next_motion == eMudMotions::Motion_38_Punch)
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
        s16 v18 = GetBrainSubStateResponse(field_17E_delayed_speak);
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
        field_194_timer = (u16)(MudResponseDelay() + sGnFrame + 20);
        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return field_190_brain_sub_state;
        }
        return v18;
    }
    if (LastGameSpeak() == GameSpeakEvents::eStopIt_22)
    {
        if (mCurrentMotion == eMudMotions::Motion_38_Punch || mNextMotion == eMudMotions::Motion_38_Punch)
        {
            ToStand();
            mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
            mNextMotion = -1;
        }

        s16 result = GetBrainSubStateResponse(MudAction::eStopIt_4);
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

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        if (!FindObjectOfType(ReliveTypes::eAbe, mXPos - ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5)))
        {
            if (FindObjectOfType(ReliveTypes::eAbe, mXPos + ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5))
                || (!FindObjectOfType(ReliveTypes::eMudokon, mXPos - ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5))
                    && FindObjectOfType(ReliveTypes::eMudokon, mXPos + ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5))))
            {
                mNextMotion = eMudMotions::Motion_2_StandingTurn;
                field_194_timer = sGnFrame + 15;
                return Brain_4_ListeningToAbe::eBrain4_GetsCommand_12;
            }
        }
    }
    else
    {
        if (!FindObjectOfType(ReliveTypes::eAbe, mXPos + ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5)))
        {
            if (FindObjectOfType(ReliveTypes::eAbe, mXPos - ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5)))
            {
                mNextMotion = eMudMotions::Motion_2_StandingTurn;
                field_194_timer = sGnFrame + 15;
                return Brain_4_ListeningToAbe::eBrain4_GetsCommand_12;
            }

            if (!FindObjectOfType(ReliveTypes::eMudokon, ScaleToGridSize(mSpriteScale) + mXPos, mYPos - FP_FromInteger(5)))
            {
                if (FindObjectOfType(ReliveTypes::eMudokon, mXPos - ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5)))
                {
                    mNextMotion = eMudMotions::Motion_2_StandingTurn;
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
        MudEmotionSound(field_188_pTblEntry->field_0_sound);
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
    if (mCurrentMotion != eMudMotions::Motion_56_SlapOwnHead)
    {
        mNextMotion = eMudMotions::Motion_56_SlapOwnHead;
    }

    if (LastGameSpeak() != GameSpeakEvents::eStopIt_22 && field_17E_delayed_speak != MudAction::eSorry_8)
    {
        return field_190_brain_sub_state;
    }

    s16 result = GetBrainSubStateResponse(MudAction::eStopIt_4);
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
        s16 v18 = GetBrainSubStateResponse(field_17E_delayed_speak);
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
        field_194_timer = (u16)(MudResponseDelay() + sGnFrame + 20);
        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return field_190_brain_sub_state;
        }
        return v18;
    }

    if (LastGameSpeak() != GameSpeakEvents::eStopIt_22)
    {
        if (mCurrentMotion)
        {
            return field_190_brain_sub_state;
        }
        mNextMotion = eMudMotions::Motion_38_Punch;
        return Brain_4_ListeningToAbe::eBrain4_Idle_7;
    }

    if (mCurrentMotion == eMudMotions::Motion_38_Punch || mNextMotion == eMudMotions::Motion_38_Punch)
    {
        ToStand();
        mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
        mNextMotion = -1;
    }

    s16 result = GetBrainSubStateResponse(MudAction::eStopIt_4);
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
    mNextMotion = eMudMotions::Motion_38_Punch;
    return Brain_4_ListeningToAbe::eBrain4_Idle_7;
}

s16 Mudokon::Brain_ListeningToAbe_State_18()
{
    if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eMudAbuse_9)
    {
        s16 v18 = GetBrainSubStateResponse(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
        {
            field_194_timer = (s16)(sGnFrame + 20);
        }
        else
        {
            field_194_timer = (u16) Mudokon::MudResponseDelay() + sGnFrame + 20;
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
        if (LastGameSpeak() != GameSpeakEvents::eStopIt_22)
        {
            if (mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::Motion_2_StandingTurn;
            return Brain_4_ListeningToAbe::eBrain4_RageSlap_17;
        }

        if (mCurrentMotion == eMudMotions::Motion_38_Punch || mNextMotion == eMudMotions::Motion_38_Punch)
        {
            ToStand();
            mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
            mNextMotion = -1;
        }

        s16 result = GetBrainSubStateResponse(MudAction::eStopIt_4);
        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        field_194_timer = sGnFrame + 20;

        if (!result)
        {
            if (mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                return field_190_brain_sub_state;
            }
            mNextMotion = eMudMotions::Motion_2_StandingTurn;
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
        MudEmotionSound(MudSounds::eNoAngry_21);
    }
    else if (field_180_emo_tbl == Mud_Emotion::eSad_3 || field_180_emo_tbl == Mud_Emotion::eSuicidal_4)
    {
        MudEmotionSound(MudSounds::eNoSad_22);
    }
    else
    {
        MudEmotionSound(MudSounds::eNuhUh_13);
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
    MudEmotionSound(MudSounds::eOkay_12);
    mNextMotion = eMudMotions::M_Speak_3_472FA0;
    return field_178_brain_sub_state2;
}

s16 Mudokon::Brain_ListeningToAbe_State_21()
{
    if (mCurrentMotion != eMudMotions::Motion_0_Idle)
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
        MudEmotionSound(MudSounds::eLaugh_10);
    }
    else
    {
        MudEmotionSound(MudSounds::eGiggle_8);
    }

    mNextMotion = eMudMotions::M_Speak_6_472FA0;
    return field_178_brain_sub_state2;
}

s16 Mudokon::Brain_ListeningToAbe_State_22()
{
    if (field_10_resources_array.ItemAt(2))
    {
        mNextMotion = eMudMotions::Motion_11_Chisel;
    }
    else
    {
        mNextMotion = eMudMotions::Motion_15_CrouchIdle;
    }

    if (mCurrentMotion != eMudMotions::Motion_15_CrouchIdle && mCurrentMotion != eMudMotions::Motion_11_Chisel)
    {
        return field_190_brain_sub_state;
    }

    RemoveAlerted();

    if (field_10_resources_array.ItemAt(2))
    {
        field_18E_brain_state = Mud_Brain_State::Brain_1_Chisel;
    }
    else
    {
        field_18E_brain_state = Mud_Brain_State::Brain_2_CrouchScrub;
    }

    field_182 = GameSpeakEvents::eNone_m1;
    return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
}

s16 Mudokon::Brain_5_ShrivelDeath()
{
    if (field_194_timer < static_cast<s32>(sGnFrame + 80))
    {
        mSpriteScale -= FP_FromDouble(0.008);
        mRGB.r -= 2;
        mRGB.g -= 2;
        mRGB.b -= 2;
    }

    if (static_cast<s32>(sGnFrame) < field_194_timer - 24)
    {
        DeathSmokeEffect(true);
    }

    // Finally fizzled out
    if (mSpriteScale < FP_FromInteger(0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 100;
}

s16 Mudokon::Brain_6_Escape()
{
    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds.Find_Impl(field_11C_bird_portal_id));
    if (EventGet(kEventDeathReset))
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
            bOver60Away = FP_Abs(pBirdPortal->mYPos - mYPos) > (mSpriteScale * FP_FromInteger(60));
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
                field_11C_bird_portal_id = Guid{};
            }

            mNextMotion = eMudMotions::Motion_0_Idle;
            field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe;
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
                    return FP_Abs(pBirdPortal->mXPos - mXPos) >= ScaleToGridSize(mSpriteScale) ? 2 : 4;

                case Brain_6_Escape::eBrain6_Running_2:
                    if (mCurrentMotion == eMudMotions::Motion_15_CrouchIdle)
                    {
                        mNextMotion = eMudMotions::Motion_18_CrouchToStand;
                    }
                    if (mCurrentMotion == eMudMotions::Motion_11_Chisel)
                    {
                        mNextMotion = eMudMotions::Motion_0_Idle;
                    }
                    if (mCurrentMotion == eMudMotions::Motion_53_Duck)
                    {
                        mNextMotion = eMudMotions::Motion_15_CrouchIdle;
                    }

                    if (mCurrentMotion == eMudMotions::Motion_0_Idle || mCurrentMotion == eMudMotions::Motion_1_WalkLoop)
                    {
                        if (FacingBirdPortal(pBirdPortal))
                        {
                            mNextMotion = eMudMotions::Motion_21_RunLoop;
                        }
                        else
                        {
                            mNextMotion = eMudMotions::Motion_2_StandingTurn;
                        }
                    }

                    if (mCurrentMotion != eMudMotions::Motion_21_RunLoop)
                    {
                        return field_190_brain_sub_state;
                    }

                    if (!FacingBirdPortal(pBirdPortal))
                    {
                        mNextMotion = eMudMotions::Motion_25_RunSlideTurn;
                        return field_190_brain_sub_state;
                    }

                    if (!VIntoBirdPortal(3)) // Finds portal to jump into
                    {
                        return field_190_brain_sub_state;
                    }

                    field_190_brain_sub_state = Brain_6_Escape::eBrain6_Jumping_3;
                    mNextMotion = eMudMotions::Motion_36_RunJumpMid;
                    return field_190_brain_sub_state;

                case Brain_6_Escape::eBrain6_Replacing_4:
                    if (mCurrentMotion == eMudMotions::Motion_15_CrouchIdle)
                    {
                        mNextMotion = eMudMotions::Motion_18_CrouchToStand;
                    }
                    if (mCurrentMotion == eMudMotions::Motion_11_Chisel)
                    {
                        mNextMotion = eMudMotions::Motion_0_Idle;
                    }
                    if (mCurrentMotion == eMudMotions::Motion_53_Duck)
                    {
                        mNextMotion = eMudMotions::Motion_15_CrouchIdle;
                    }

                    if (mCurrentMotion == eMudMotions::Motion_0_Idle || mCurrentMotion == eMudMotions::Motion_1_WalkLoop)
                    {
                        if ((pBirdPortal->mEnterSide == relive::Path_BirdPortal::PortalSide::eRight && mAnim.mFlags.Get(AnimFlags::eBit5_FlipX)) || (pBirdPortal->mEnterSide == relive::Path_BirdPortal::PortalSide::eLeft && !mAnim.mFlags.Get(AnimFlags::eBit5_FlipX)))
                        {
                            mNextMotion = eMudMotions::Motion_2_StandingTurn;
                        }
                        else
                        {
                            mNextMotion = eMudMotions::Motion_21_RunLoop;
                        }
                    }

                    if (mCurrentMotion != eMudMotions::Motion_21_RunLoop)
                    {
                        return field_190_brain_sub_state;
                    }

                    if (FP_Abs(pBirdPortal->mXPos - mXPos) <= ScaleToGridSize(mSpriteScale))
                    {
                        return field_190_brain_sub_state;
                    }
                    return Brain_6_Escape::eBrain6_Running_2;
            }
        }
    }
    return field_190_brain_sub_state;
}

s16 Mudokon::Brain_7_FallAndSmackDeath()
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


s16 Mudokon::Brain_8_AngryWorker()
{
    switch (field_190_brain_sub_state)
    {
        case Brain_8_AngryWorker::eBrain8_Inactive_0:
            if (mCurrentMotion)
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
            }

            if (field_16A_flags.Get(Flags_16A::eBit15_ring_and_angry_mud_timeout))
            {
                const GameSpeakEvents lastSpeak = LastGameSpeak();
                if (lastSpeak < GameSpeakEvents::eHello_9 || lastSpeak > GameSpeakEvents::eSorry_24)
                {
                    return field_190_brain_sub_state;
                }
            }

            field_194_timer = sGnFrame + Math_RandomRange(30, 45);
            // adds mudokon lag when quicksaving/quickloading in the same screen
            AddAlerted();

            return sPathInfo->TLV_Get_At_4DB4B0(
                       FP_GetExponent(mXPos),
                       FP_GetExponent(mYPos),
                       FP_GetExponent(mXPos),
                       FP_GetExponent(mYPos),
                       ReliveTypes::eWorkWheel)
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

            const GameSpeakEvents lastSpeak = LastGameSpeak();
            if (lastSpeak >= GameSpeakEvents::eHello_9
                && lastSpeak <= GameSpeakEvents::eSorry_24
                && sActiveHero->mSpriteScale == mSpriteScale)
            {
                field_160_delayed_speak = sAngryWorkerResponses_55CFCA[static_cast<s32>(lastSpeak) - 9];
                field_194_timer = MudResponseDelay() + sGnFrame + 20;
                return Brain_8_AngryWorker::eBrain8_Speaking_3;
            }

            if (field_194_timer > static_cast<s32>(sGnFrame))
            {
                return field_190_brain_sub_state;
            }

            const FP ypos = mYPos - ScaleToGridSize(mSpriteScale);
            FP xOff = {};
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                xOff = -ScaleToGridSize(mSpriteScale);
            }
            else
            {
                xOff = ScaleToGridSize(mSpriteScale);
            }

            auto pLever = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, xOff + mXPos, ypos));
            if (pLever)
            {
                mNextMotion = eMudMotions::Motion_10_LeverUse;
                pLever->VPull(mXPos < pLever->mXPos);
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
                    MudEmotionSound(field_188_pTblEntry->field_0_sound);
                }
                mNextMotion = field_188_pTblEntry->field_2_next_motion;
                field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
                field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe;
                return Brain_4_ListeningToAbe::eBrain4_Idle_7;
            }
            else
            {
                mNextMotion = eMudMotions::M_Speak_6_472FA0;
                MudEmotionSound(field_160_delayed_speak);
                field_194_timer = sGnFrame + Math_RandomRange(30, 45);
                return sPathInfo->TLV_Get_At_4DB4B0(
                           FP_GetExponent(mXPos),
                           FP_GetExponent(mYPos),
                           FP_GetExponent(mXPos),
                           FP_GetExponent(mYPos),
                           ReliveTypes::eWorkWheel)
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
            mNextMotion = eMudMotions::Motion_57_TurnWheelBegin;
            return Brain_8_AngryWorker::eBrain8_UsingWheel_5;

        case Brain_8_AngryWorker::eBrain8_UsingWheel_5:
        {
            const GameSpeakEvents lastSpeak2 = LastGameSpeak();
            if (lastSpeak2 >= GameSpeakEvents::eHello_9
                && lastSpeak2 <= GameSpeakEvents::eSorry_24
                && sActiveHero->mSpriteScale == mSpriteScale)
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
                field_160_delayed_speak = sAngryWorkerResponses_55CFCA[static_cast<s32>(lastSpeak2) - 9];
                field_194_timer = MudResponseDelay() + sGnFrame + 20;
                return Brain_8_AngryWorker::eBrain8_Speaking_3;
            }

            if (field_17E_delayed_speak != MudAction::eSorry_8)
            {
                return field_190_brain_sub_state;
            }

            mNextMotion = eMudMotions::Motion_0_Idle;
            field_194_timer = sGnFrame + 20;
        }
            return Brain_8_AngryWorker::eBrain8_Speaking_3;

        case Brain_8_AngryWorker::eBrain8_Unused_6:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return field_190_brain_sub_state;
            }

            if (mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                if (mCurrentMotion == eMudMotions::Motion_15_CrouchIdle)
                {
                    mNextMotion = eMudMotions::Motion_0_Idle;
                    return field_190_brain_sub_state;
                }
                else
                {
                    if (mCurrentMotion == eMudMotions::Motion_53_Duck)
                    {
                        mNextMotion = eMudMotions::Motion_15_CrouchIdle;
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
            mNextMotion = eMudMotions::Motion_55_DuckKnockback;
            field_194_timer = sGnFrame + 60;
            return Brain_8_AngryWorker::eBrain8_Unused_6;
    }

    return field_190_brain_sub_state;
}

s16 Mudokon::Brain_9_Sick()
{
    // TODO: Lame hack, tortured muds make the "real" mud do nothing.
    if (FindObjectOfType(ReliveTypes::eTorturedMud, mXPos, mYPos - FP_FromInteger(50)) || !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        LastGameSpeak(); // TODO: Pointless call ??
        return field_190_brain_sub_state;
    }

    if (IsEventInRange(
            kEventMudokonAbuse,
            mXPos,
            mYPos,
            AsEventScale(mScale)))
    {
        field_17E_delayed_speak = MudAction::eMudAbuse_9;
    }

    if (IsEventInRange(
            kEventMudokonComfort,
            mXPos,
            mYPos,
            AsEventScale(mScale)))
    {
        field_17E_delayed_speak = MudAction::eComfort_10;
    }

    if (IsEventInRange(
            kEventMudokonLaugh,
            mXPos,
            mYPos,
            AsEventScale(mScale)))
    {
        field_17E_delayed_speak = MudAction::eLaugh_12;
    }

    if (IsEventInRange(kEventAbeDead,
            mXPos,
            mYPos,
                                AsEventScale(mScale)))
    {
        field_17E_delayed_speak = MudAction::eDuck_13;
    }

    if (IsEventInRange(
            kEventShooting,
            mXPos,
            mYPos,
            AsEventScale(mScale)))
    {
        field_17E_delayed_speak = MudAction::eWait_2;
    }

    // Have Abe make a sad noise when he first sees sick Mudokons.
    if (!field_16A_flags.Get(Flags_16A::eBit9_seen_while_sick) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        if (sActiveHero->field_128.mSay == MudSounds::eNone)
        {
            sActiveHero->field_128.mSay = MudSounds::eSadUgh_28;
            sActiveHero->mAutoSayTimer = sGnFrame + 10;
        }
        field_16A_flags.Set(Flags_16A::eBit9_seen_while_sick);
    }

    switch (field_190_brain_sub_state)
    {
        case Brain_9_Sick::eBrain9_Inactive_0:
            if (mCurrentMotion != eMudMotions::Motion_15_CrouchIdle)
            {
                if (mCurrentMotion != eMudMotions::Motion_17_StandToCrouch && mNextMotion != eMudMotions::Motion_17_StandToCrouch)
                {
                    mNextMotion = eMudMotions::Motion_17_StandToCrouch;
                }
                return field_190_brain_sub_state;
            }

            AddAlerted();

            mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
            return Brain_9_Sick::eBrain9_Idle_1;

        case Brain_9_Sick::eBrain9_Idle_1:
            if (mCurrentMotion == eMudMotions::Motion_49_Fall)
            {
                return Brain_9_Sick::eBrain9_Inactive_0;
            }

            if (mCurrentMotion != eMudMotions::Motion_15_CrouchIdle)
            {
                if (mCurrentMotion == eMudMotions::Motion_53_Duck)
                {
                    mNextMotion = eMudMotions::Motion_15_CrouchIdle;
                }
                return field_190_brain_sub_state;
            }

            if (field_180_emo_tbl != Mud_Emotion::eSick_7)
            {
                mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
                mNextMotion = eMudMotions::Motion_0_Idle;
                relive_new PossessionFlicker(this, 8, 155, 255, 32);
                return Brain_9_Sick::eBrain9_StandingUp_3;
            }

            if (field_17E_delayed_speak == MudAction::eNone_17)
            {
                const GameSpeakEvents lastSpeak = LastGameSpeak();
                if (lastSpeak == GameSpeakEvents::eNone_m1)
                {
                    return field_190_brain_sub_state;
                }

                field_178_brain_sub_state2 = field_190_brain_sub_state;
                field_194_timer = MudResponseDelay() + sGnFrame + 20;

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
                    field_194_timer = MudResponseDelay() + sGnFrame + 20;
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
                MudEmotionSound(field_188_pTblEntry->field_0_sound);
            }

            mNextMotion = eMudMotions::Motion_53_Duck;
            field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
            return field_178_brain_sub_state2;

        case Brain_9_Sick::eBrain9_StandingUp_3:
            if (mCurrentMotion)
            {
                return field_190_brain_sub_state;
            }

            MudEmotionSound(MudSounds::eFart_7);
            mNextMotion = eMudMotions::M_Speak_6_472FA0;

            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                New_Smoke_Particles(
                    mXPos + (FP_FromInteger(12) * mSpriteScale),
                    mYPos - (FP_FromInteger(24) * mSpriteScale),
                    (FP_FromDouble(0.5) * mSpriteScale),
                    3, 32u, 128u, 32u);
            }
            else
            {
                New_Smoke_Particles(
                    mXPos - (FP_FromInteger(12) * mSpriteScale),
                    mYPos - (FP_FromInteger(24) * mSpriteScale),
                    (FP_FromDouble(0.5) * mSpriteScale),
                    3, 32u, 128u, 32u);
            }
            return Brain_9_Sick::eBrain9_Farting_4;

        case Brain_9_Sick::eBrain9_Farting_4:
            if (mCurrentMotion)
            {
                return field_190_brain_sub_state;
            }
            field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe;
            return Brain_4_ListeningToAbe::eBrain4_Inactive_0;

        default:
            return field_190_brain_sub_state;
    }
    return field_190_brain_sub_state;
}

void Mudokon::Motion_0_Idle()
{
    CheckFloorGone();

    BaseAliveGameObjectLastLineYPos = mYPos;

    if (BaseAliveGameObjectCollisionLine)
    {
        if ((BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36) && BaseAliveGameObject_PlatformId == Guid{})
        {
            const PSX_RECT bRect = VGetBoundingRect();
            VOnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {bRect.w, static_cast<s16>(bRect.h + 5)},
                ObjList_5C1B78, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
        }
    }

    switch (mNextMotion)
    {
        case eMudMotions::Motion_1_WalkLoop:
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mVelX = -ScaleToGridSize(mSpriteScale) / FP_FromInteger(9);
            }
            else
            {
                mVelX = ScaleToGridSize(mSpriteScale) / FP_FromInteger(9);
            }
            mCurrentMotion = eMudMotions::Motion_7_WalkBegin;
            mNextMotion = -1;
            break;

        case eMudMotions::Motion_27_SneakLoop:
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mVelX = -ScaleToGridSize(mSpriteScale) / FP_FromInteger(10);
            }
            else
            {
                mVelX = ScaleToGridSize(mSpriteScale) / FP_FromInteger(10);
            }
            mCurrentMotion = eMudMotions::Motion_32_SneakBegin;
            mNextMotion = -1;
            break;

        case eMudMotions::Motion_21_RunLoop:
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mVelX = -ScaleToGridSize(mSpriteScale) / FP_FromInteger(4);
            }
            else
            {
                mVelX = ScaleToGridSize(mSpriteScale) / FP_FromInteger(4);
            }

            mCurrentMotion = eMudMotions::Motion_37_StandToRun;
            mNextMotion = -1;
            break;

        case eMudMotions::Motion_50_Chant:
            SND_SEQ_PlaySeq(SeqId::MudokonChant_11, 0, 1);
            mCurrentMotion = eMudMotions::Motion_50_Chant;
            mNextMotion = -1;
            break;

        case eMudMotions::Motion_15_CrouchIdle:
            mCurrentMotion = eMudMotions::Motion_17_StandToCrouch;
            mNextMotion = -1;
            break;

        case eMudMotions::Motion_11_Chisel:
        case eMudMotions::Motion_14_CrouchScrub:
            mCurrentMotion = eMudMotions::Motion_17_StandToCrouch;
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

void Mudokon::Motion_1_WalkLoop()
{
    EventBroadcast(kEventNoise, this);
    if (WallHit(mSpriteScale * FP_FromInteger(50), mVelX))
    {
        ToKnockback();
    }
    else if (sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId) && field_16A_flags.Get(Flags_16A::eBit4_blind) && (WallHit(mSpriteScale * FP_FromInteger(1), mVelX)))
    {
        ToKnockback();
    }
    else
    {
        MoveOnLine();
        if (mCurrentMotion == eMudMotions::Motion_1_WalkLoop)
        {
            if (mAnim.mCurrentFrame == 2 || mAnim.mCurrentFrame == 11)
            {
                if (mNextMotion == eMudMotions::Motion_0_Idle)
                {
                    mNextMotion = -1;
                    if (mAnim.mCurrentFrame == 2)
                    {
                        mCurrentMotion = eMudMotions::Motion_9_MidWalkToIdle;
                    }
                    else
                    {
                        mCurrentMotion = eMudMotions::Motion_8_WalkToIdle;
                    }
                }
                else if (mNextMotion == eMudMotions::Motion_2_StandingTurn)
                {
                    if (mAnim.mCurrentFrame != 2)
                    {
                        mCurrentMotion = eMudMotions::Motion_9_MidWalkToIdle;
                    }
                    else
                    {
                        mCurrentMotion = eMudMotions::Motion_8_WalkToIdle;
                    }
                }
            }
            else if (mAnim.mCurrentFrame == 5 || mAnim.mCurrentFrame == 14)
            {
                Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);
                MapFollowMe(TRUE);
                if (mNextMotion == eMudMotions::Motion_21_RunLoop)
                {
                    mNextMotion = -1;
                    if (mAnim.mCurrentFrame == 5)
                    {
                        mCurrentMotion = eMudMotions::Motion_20_MidWalkToRun;
                    }
                    else
                    {
                        mCurrentMotion = eMudMotions::Motion_19_WalkToRun;
                    }
                }
                else if (mNextMotion == eMudMotions::Motion_27_SneakLoop)
                {
                    auto curFrame2 = mAnim.mCurrentFrame;
                    mNextMotion = -1;
                    if (curFrame2 == 5)
                    {
                        mCurrentMotion = eMudMotions::Motion_28_MidWalkToSneak;
                    }
                    else
                    {
                        mCurrentMotion = eMudMotions::Motion_30_WalkToSneak;
                    }
                }
            }
        }
    }
}

void Mudokon::Motion_2_StandingTurn()
{
    CheckFloorGone();

    if (mAnim.mCurrentFrame == 0)
    {
        Environment_SFX_457A40(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);
        ToStand();
    }
}

void Mudokon::Motion_Speak()
{
    CheckFloorGone();
    SetPal(field_180_emo_tbl);

    if (field_16A_flags.Get(Flags_16A::eBit12_alert_enemies))
    {
        EventBroadcast(kEventNoise, this);
        EventBroadcast(kEventSuspiciousNoise, this);
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_16A_flags.Set(Flags_16A::eBit12_alert_enemies);
        SetPal(Mud_Emotion::eNormal_0);
        ToStand();
    }
}

void Mudokon::Motion_7_WalkBegin()
{
    EventBroadcast(kEventNoise, this);
    if (WallHit(mSpriteScale * FP_FromInteger(50), mVelX))
    {
        ToStand();
    }
    else
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = eMudMotions::Motion_1_WalkLoop;
        }
        MoveOnLine();
    }
}

void Mudokon::Motion_8_WalkToIdle()
{
    EventBroadcast(kEventNoise, this);
    if (WallHit(mSpriteScale * FP_FromInteger(50), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();

        if (mAnim.mCurrentFrame == 0)
        {
            Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);
            return;
        }

        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe(TRUE);
            ToStand();
        }
    }
}

void Mudokon::Motion_9_MidWalkToIdle()
{
    Motion_8_WalkToIdle();
}

void Mudokon::Motion_10_LeverUse()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

void Mudokon::Motion_11_Chisel()
{
    CheckFloorGone();

    if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (sGnFrame & 1)
            {
                SfxPlayMono(relive::SoundEffects::Chisel, 0, mSpriteScale);

                FP sparkY = {};
                FP sparkX = {};
                if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    sparkY = mYPos - (mSpriteScale * FP_FromInteger(3));
                    sparkX = mXPos - (mSpriteScale * FP_FromInteger(18));
                }
                else
                {
                    sparkY = mYPos - (mSpriteScale * FP_FromInteger(3));
                    sparkX = (mSpriteScale * FP_FromInteger(18)) + mXPos;
                }

                relive_new Spark(
                    sparkX + FP_FromInteger(mXOffset),
                    sparkY,
                    mSpriteScale,
                    9,
                    0,
                    255,
                    SparkType::eSmallChantParticle_0);
            }
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        switch (mNextMotion)
        {
            case eMudMotions::Motion_0_Idle:
                mCurrentMotion = eMudMotions::Motion_13_StopChisel;
                return;

            case eMudMotions::Motion_15_CrouchIdle:
                mCurrentMotion = eMudMotions::Motion_13_StopChisel;
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

void Mudokon::Motion_12_StartChisel()
{
    CheckFloorGone();
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_11_Chisel;
    }
}

void Mudokon::Motion_13_StopChisel()
{
    CheckFloorGone();
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_15_CrouchIdle;
    }
}

void Mudokon::Motion_14_CrouchScrub()
{
    CheckFloorGone();

    if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        if (mAnim.mCurrentFrame == 2)
        {
            SfxPlayMono(relive::SoundEffects::Clean1, 0);
        }
        else if (mAnim.mCurrentFrame == 6)
        {
            SfxPlayMono(relive::SoundEffects::Clean2, 0);
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_15_CrouchIdle;
    }
}

void Mudokon::Motion_15_CrouchIdle()
{
    CheckFloorGone();

    if (mNextMotion == eMudMotions::Motion_53_Duck)
    {
        mCurrentMotion = eMudMotions::Motion_52_ToDuck;
        mNextMotion = -1;
    }
    else if (mNextMotion == eMudMotions::Motion_0_Idle)
    {
        mCurrentMotion = eMudMotions::Motion_18_CrouchToStand;
        mNextMotion = -1;
    }
    else if (mNextMotion == eMudMotions::Motion_11_Chisel)
    {
        mCurrentMotion = eMudMotions::Motion_12_StartChisel;
        mNextMotion = -1;
    }
    else if (mNextMotion != -1)
    {
        mCurrentMotion = mNextMotion;
        mNextMotion = -1;
    }
}

void Mudokon::Motion_16_CrouchTurn()
{
    CheckFloorGone();

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_15_CrouchIdle;
        mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);
    }
}

void Mudokon::Motion_17_StandToCrouch()
{
    CheckFloorGone();
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_15_CrouchIdle;
    }
}

void Mudokon::Motion_18_CrouchToStand()
{
    CheckFloorGone();
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

void Mudokon::Motion_19_WalkToRun()
{
    EventBroadcast(kEventNoise, this);
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
    }
    else
    {
        mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_21_RunLoop;
    }

    if (WallHit((mSpriteScale * FP_FromInteger(50)), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_20_MidWalkToRun()
{
    Motion_19_WalkToRun();

    if (mCurrentMotion == eMudMotions::Motion_21_RunLoop)
    {
        mCurrentMotion = eMudMotions::Motion_20_MidWalkToRun;
        mPreviousMotion = eMudMotions::Motion_21_RunLoop;
        mBaseAliveGameObjectLastAnimFrame = 8;
        field_192_return_to_previous_motion = 1;
    }
}

void Mudokon::Motion_21_RunLoop()
{
    EventBroadcast(kEventNoise, this);

    if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
    {
        EventBroadcast(kEventSuspiciousNoise, this);
    }

    if (WallHit(mSpriteScale * FP_FromInteger(50), mVelX))
    {
        ToKnockback();
        return;
    }

    MoveOnLine();

    if (mCurrentMotion == eMudMotions::Motion_21_RunLoop)
    {
        if (mAnim.mCurrentFrame == 0 || mAnim.mCurrentFrame == 8)
        {
            MapFollowMe(TRUE);

            if (mNextMotion == eMudMotions::Motion_36_RunJumpMid)
            {
                mCurrentMotion = eMudMotions::Motion_35_RunJumpBegin;
                mNextMotion = -1;
            }
        }
        else if (mAnim.mCurrentFrame == 4 || mAnim.mCurrentFrame == 12)
        {
            Environment_SFX_457A40(EnvironmentSfx::eRunningFootstep_2, 0, 32767, this);
            MapFollowMe(TRUE);

            switch (mNextMotion)
            {
                case eMudMotions::Motion_1_WalkLoop:
                    mNextMotion = -1;
                    if (mAnim.mCurrentFrame == 4)
                    {
                        mCurrentMotion = eMudMotions::Motion_23_MidRunToWalk;
                    }
                    else
                    {
                        mCurrentMotion = eMudMotions::Motion_22_RunToWalk;
                    }
                    return;

                case eMudMotions::Motion_0_Idle:
                    mCurrentMotion = eMudMotions::Motion_24_RunSlideStop;
                    mNextMotion = -1;
                    Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
                    return;

                case eMudMotions::Motion_25_RunSlideTurn:
                    mCurrentMotion = eMudMotions::Motion_25_RunSlideTurn;
                    mNextMotion = -1;
                    Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
                    return;

                case eMudMotions::Motion_36_RunJumpMid:
                    mCurrentMotion = eMudMotions::Motion_35_RunJumpBegin;
                    mNextMotion = -1;
                    break;
            }
        }
    }
}

void Mudokon::Motion_22_RunToWalk()
{
    EventBroadcast(kEventNoise, this);
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
    }
    else
    {
        mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_1_WalkLoop;
    }

    if (WallHit((mSpriteScale * FP_FromInteger(50)), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_23_MidRunToWalk()
{
    Motion_22_RunToWalk();

    if (mCurrentMotion == eMudMotions::Motion_1_WalkLoop)
    {
        mCurrentMotion = eMudMotions::Motion_23_MidRunToWalk;
        mPreviousMotion = eMudMotions::Motion_1_WalkLoop;
        mBaseAliveGameObjectLastAnimFrame = 9;
        field_192_return_to_previous_motion = 1;
    }
}

void Mudokon::Motion_24_RunSlideStop()
{
    EventBroadcast(kEventNoise, this);
    if (WallHit((mSpriteScale * FP_FromInteger(50)), mVelX))
    {
        ToKnockback();
    }
    else
    {
        ReduceXVelocityBy(FP_FromDouble(0.375));

        if (mCurrentMotion == eMudMotions::Motion_24_RunSlideStop)
        {
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);
                MapFollowMe(TRUE);
                Mudokon::ToStand();
            }
        }
    }
}

void Mudokon::Motion_25_RunSlideTurn()
{
    EventBroadcast(kEventNoise, this);

    if (WallHit((mSpriteScale * FP_FromInteger(50)), mVelX))
    {
        ToKnockback();
    }
    else
    {
        ReduceXVelocityBy(FP_FromDouble(0.375));

        if (mCurrentMotion == eMudMotions::Motion_25_RunSlideTurn)
        {
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe(TRUE);
                if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
                    mCurrentMotion = eMudMotions::Motion_26_RunTurnToRun;
                }
                else
                {
                    mCurrentMotion = eMudMotions::Motion_26_RunTurnToRun;
                    mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
                }
            }
        }
    }
}

void Mudokon::Motion_26_RunTurnToRun()
{
    EventBroadcast(kEventNoise, this);

    if (WallHit((mSpriteScale * FP_FromInteger(50)), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();

        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = eMudMotions::Motion_21_RunLoop;
            mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);
        }
    }
}

void Mudokon::Motion_27_SneakLoop()
{
    if (WallHit((mSpriteScale * FP_FromInteger(50)), mVelX))
    {
        ToKnockback();
    }
    else
    {
        MoveOnLine();
        if (mCurrentMotion == eMudMotions::Motion_27_SneakLoop)
        {
            if (mAnim.mCurrentFrame == 3 || mAnim.mCurrentFrame == 13)
            {
                if (mNextMotion == eMudMotions::Motion_0_Idle)
                {
                    mNextMotion = -1;
                    mCurrentMotion = (mAnim.mCurrentFrame != 3) ? eMudMotions::Motion_33_SneakToIdle : eMudMotions::Motion_34_MidSneakToIdle;
                }
            }
            else if (mAnim.mCurrentFrame == 6 || mAnim.mCurrentFrame == 16)
            {
                Environment_SFX_457A40(EnvironmentSfx::eSneakFootstep_3, 0, 32767, this);
                MapFollowMe(TRUE);

                if (mNextMotion == eMudMotions::Motion_1_WalkLoop)
                {
                    mNextMotion = -1;
                    mCurrentMotion = mAnim.mCurrentFrame != 6 ? eMudMotions::Motion_31_MidSneakToWalk : eMudMotions::Motion_29_SneakToWalk;
                }
            }
        }
    }
}

void Mudokon::Motion_28_MidWalkToSneak()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(10));
    }
    else
    {
        mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(10));
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_27_SneakLoop;
    }

    if (WallHit((mSpriteScale * FP_FromInteger(50)), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_29_SneakToWalk()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
    }
    else
    {
        mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_1_WalkLoop;
    }

    if (WallHit((mSpriteScale * FP_FromInteger(50)), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_30_WalkToSneak()
{
    Motion_28_MidWalkToSneak();

    if (mCurrentMotion == eMudMotions::Motion_27_SneakLoop)
    {
        mCurrentMotion = eMudMotions::Motion_30_WalkToSneak;
        mPreviousMotion = eMudMotions::Motion_27_SneakLoop;
        mBaseAliveGameObjectLastAnimFrame = 10;
        field_192_return_to_previous_motion = 1;
    }
}

void Mudokon::Motion_31_MidSneakToWalk()
{
    Motion_29_SneakToWalk();

    if (mCurrentMotion == eMudMotions::Motion_1_WalkLoop)
    {
        mCurrentMotion = eMudMotions::Motion_31_MidSneakToWalk;
        mPreviousMotion = eMudMotions::Motion_1_WalkLoop;
        mBaseAliveGameObjectLastAnimFrame = 9;
        field_192_return_to_previous_motion = 1;
    }
}

void Mudokon::Motion_32_SneakBegin()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_27_SneakLoop;
    }

    if (WallHit((mSpriteScale * FP_FromInteger(50)), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_33_SneakToIdle()
{
    MoveOnLine();

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe(TRUE);
        ToStand();
    }
}

void Mudokon::Motion_34_MidSneakToIdle()
{
    Motion_33_SneakToIdle();
}

void Mudokon::Motion_35_RunJumpBegin()
{
    EventBroadcast(kEventNoise, this);

    mXPos += mVelX;

    if (!mAnim.mCurrentFrame)
    {
        Environment_SFX_457A40(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 32767, this);
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mVelX = (mSpriteScale * -FP_FromDouble(7.6)); // TODO: Correct FP value?
        }
        else
        {
            mVelX = (mSpriteScale * FP_FromDouble(7.6)); // TODO: Correct FP value?
        }
        mVelY = (mSpriteScale * -FP_FromDouble(9.6));
        mYPos += mVelY;
        VOnTrapDoorOpen();
        mCurrentMotion = eMudMotions::Motion_36_RunJumpMid;
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Mudokon::Motion_36_RunJumpMid()
{
    EventBroadcast(kEventNoise, this);

    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds.Find_Impl(field_11C_bird_portal_id));
    if (mAnim.mCurrentFrame == 5)
    {
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
    }

    const PSX_RECT rect = VGetBoundingRect();

    if ((mVelX > FP_FromInteger(0) && FP_FromInteger(rect.x) > pBirdPortal->mXPos) || (mVelX < FP_FromInteger(0) && FP_FromInteger(rect.w) < pBirdPortal->mXPos))
    {
        field_16A_flags.Clear(Flags_16A::eBit1_not_rescued);
        field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);

        mBaseGameObjectFlags.Set(BaseGameObject::eDead);

        mVelY = FP_FromInteger(0);
        mVelX = FP_FromInteger(0);

        SND_SEQ_Play(SeqId::SaveTriggerMusic_31, 1, 127, 127);

        relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eChime, relive::Path_MusicTrigger::TriggeredBy::eTimer, 0, 0);

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

    mVelY += (mSpriteScale * FP_FromDouble(1.8));
    mXPos += mVelX;
    mYPos += mVelY;
}

void Mudokon::Motion_37_StandToRun()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_21_RunLoop;
    }

    EventBroadcast(kEventNoise, this);

    if (WallHit((mSpriteScale * FP_FromInteger(50)), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_38_Punch()
{
    SetPal(field_180_emo_tbl);

    if (mAnim.mCurrentFrame == 5)
    {
        if (field_180_emo_tbl == Mud_Emotion::eAngry_1 || field_180_emo_tbl == Mud_Emotion::eAggressive_2)
        {
            BaseGameObject* pSlapTarget = nullptr;
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                pSlapTarget = FindObjectOfType(ReliveTypes::eAbe, mXPos - ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5));
            }
            else
            {
                pSlapTarget = FindObjectOfType(ReliveTypes::eAbe, mXPos + ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5));
            }

            if (!pSlapTarget)
            {
                if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    pSlapTarget = FindObjectOfType(ReliveTypes::eMudokon, mXPos - ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5));
                }
                else
                {
                    pSlapTarget = FindObjectOfType(ReliveTypes::eMudokon, mXPos + ScaleToGridSize(mSpriteScale), mYPos - FP_FromInteger(5));
                }
            }

            if (pSlapTarget)
            {
                static_cast<BaseAliveGameObject*>(pSlapTarget)->VTakeDamage(this);
            }
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetPal(Mud_Emotion::eNormal_0);
        ToStand();
    }
}

void Mudokon::Motion_39_HoistBegin()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        mVelY = (mSpriteScale * -FP_FromInteger(8));
        mYPos += mVelY;
        VOnTrapDoorOpen();
        mCurrentMotion = eMudMotions::Motion_40_HoistLand;
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Mudokon::Motion_40_HoistLand()
{
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        PSX_RECT bRect = {};
        switch (pLine->mLineType) // TODO: Strongly type
        {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                mXPos = hitX;
                mYPos = FP_NoFractional(hitY + FP_FromDouble(0.5));
                MapFollowMe(TRUE);
                BaseAliveGameObjectCollisionLine = pLine;
                mCurrentMotion = eMudMotions::Motion_41_LandSoft1;
                bRect = VGetBoundingRect();
                VOnCollisionWith(
                    {bRect.x, static_cast<s16>(bRect.y + 5)},
                    {bRect.w, static_cast<s16>(bRect.h + 5)},
                    ObjList_5C1B78,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
                break;
            default:
                return;
        }
    }
}

void Mudokon::Motion_41_LandSoft1()
{
    if (mAnim.mCurrentFrame == 2)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

void Mudokon::Motion_42_LandSoft2()
{
    CheckFloorGone();

    if (mAnim.mCurrentFrame == 2)
    {
        if (mPreviousMotion == eMudMotions::Motion_49_Fall)
        {
            Environment_SFX_457A40(EnvironmentSfx::eLandingSoft_5, 0, 32767, this);
        }
        else
        {
            Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

void Mudokon::Motion_43_DunnoBegin()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_44_DunnoEnd;
    }
}

void Mudokon::Motion_44_DunnoEnd()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

void Mudokon::Motion_45_KnockForward()
{
    if (mAnim.mCurrentFrame == 12)
    {
        CheckKnockedOntoABomb();
    }

    if (mAnim.mCurrentFrame == 4)
    {
        Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 32767, this);
    }

    if ((gMap.mCurrentLevel == EReliveLevelIds::eMines
        || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz
        || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot
        || gMap.mCurrentLevel == EReliveLevelIds::eBarracks
        || gMap.mCurrentLevel == EReliveLevelIds::eBrewery)
        && mAnim.mCurrentFrame == 7)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (mNextMotion != -1 && mHealth > FP_FromInteger(0))
    {
        mCurrentMotion = mNextMotion;
        mNextMotion = -1;
    }
}

void Mudokon::Motion_46_Knockback()
{
    if (mAnim.mCurrentFrame == 12)
    {
        CheckKnockedOntoABomb();
    }

    EventBroadcast(kEventNoise, this);

    if ((gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBrewery) && mAnim.mCurrentFrame == 7)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_49_Fall();

        if (mCurrentMotion == eMudMotions::Motion_45_KnockForward || mCurrentMotion == eMudMotions::Motion_42_LandSoft2)
        {
            mCurrentMotion = eMudMotions::Motion_46_Knockback;
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            mCurrentMotion = eMudMotions::Motion_47_KnockbackGetUp;
        }
    }
}

void Mudokon::Motion_47_KnockbackGetUp()
{
    CheckFloorGone();

    EventBroadcast(kEventNoise, this);
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

void Mudokon::Motion_48_WalkOffEdge()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_49_Fall;
    }

    Motion_49_Fall();
}

void Mudokon::Motion_49_Fall()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX = mVelX - (mSpriteScale * field_134_xVelSlowBy);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX = (mSpriteScale * field_134_xVelSlowBy) + mVelX;
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        switch (pLine->mLineType) // TODO: Strongly type line types
        {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                mXPos = hitX;
                mYPos = hitY;
                BaseAliveGameObjectCollisionLine = pLine;

                MapFollowMe(TRUE);

                if ((sPathInfo->TLV_Get_At_4DB4B0(
                         FP_GetExponent(mXPos),
                         FP_GetExponent(mYPos),
                         FP_GetExponent(mXPos),
                         FP_GetExponent(mYPos),
                         ReliveTypes::eSoftLanding)
                     && mHealth > FP_FromInteger(0))
                    || (mYPos - BaseAliveGameObjectLastLineYPos < (mSpriteScale * FP_FromInteger(180)) && (mHealth > FP_FromInteger(0) || gAbeBulletProof_5C1BDA)))
                {
                    mCurrentMotion = eMudMotions::Motion_42_LandSoft2;
                }
                else
                {
                    mHealth = FP_FromInteger(0);
                    ToKnockback();
                    field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);
                    field_18E_brain_state = Mud_Brain_State::Brain_5_ShrivelDeath;
                    mCurrentMotion = eMudMotions::Motion_45_KnockForward;
                    field_194_timer = sGnFrame + 90;
                }

                mPreviousMotion = eMudMotions::Motion_49_Fall;

                VOnCollisionWith(
                    {FP_GetExponent(mXPos - FP_FromInteger(10)), FP_GetExponent(mYPos - FP_FromInteger(10))},
                    {FP_GetExponent(mXPos + FP_FromInteger(10)), FP_GetExponent(mYPos + FP_FromInteger(10))},
                    ObjList_5C1B78,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
                break;

            case 1u:
            case 2u:
            case 5u:
            case 6u:
                mXPos = hitX;
                mYPos = hitY;
                ToKnockback();
                break;
        }
    }
}

void Mudokon::Motion_50_Chant()
{
    if (!(static_cast<s32>(sGnFrame) % 4))
    {
        New_RandomizedChant_Particle(this);
    }

    if (!SND_SsIsEos_DeInlined(11u))
    {
        SND_SEQ_Play(SeqId::MudokonChant_11, 1, 50, 50);
    }

    if (mNextMotion == eMudMotions::Motion_0_Idle)
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SND_SEQ_Stop(SeqId::MudokonChant_11);
            mCurrentMotion = eMudMotions::Motion_51_ChantEnd;
            mNextMotion = -1;
        }
    }
}

void Mudokon::Motion_51_ChantEnd()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

void Mudokon::Motion_52_ToDuck()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_53_Duck;
    }
}

void Mudokon::Motion_53_Duck()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mNextMotion == eMudMotions::Motion_15_CrouchIdle)
        {
            mNextMotion = -1;
            mCurrentMotion = eMudMotions::Motion_54_DuckToCrouch;
        }
    }
}

void Mudokon::Motion_54_DuckToCrouch()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_15_CrouchIdle;
    }
}

void Mudokon::Motion_55_DuckKnockback()
{
    if (mAnim.mCurrentFrame == 1)
    {
        Environment_SFX_457A40(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_53_Duck;
        mNextMotion = -1;
    }
}

void Mudokon::Motion_56_SlapOwnHead()
{
    if (mAnim.mCurrentFrame == 5)
    {
        HurtSoundPitchedToHealth();
        mHealth -= FP_FromDouble(0.033);
    }

    if (mHealth > FP_FromInteger(0))
    {
        if (mNextMotion == eMudMotions::Motion_0_Idle || mNextMotion == eMudMotions::M_Speak_6_472FA0)
        {
            ToStand();
        }
    }
    else
    {
        Mudokon_SFX(MudSounds::eHurt2_9, 0, 1000, this);
        Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
        EventBroadcast(kEventMudokonDied, this);
        field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);
        mHealth = FP_FromInteger(0);
        field_18E_brain_state = Mud_Brain_State::Brain_5_ShrivelDeath;
        field_194_timer = sGnFrame + 90;
        mCurrentMotion = eMudMotions::Motion_45_KnockForward;
        mNextMotion = -1;
    }
}

void Mudokon::Motion_57_TurnWheelBegin()
{
    if (mXOffset > 0)
    {
        mXOffset--;
    }
    else if (mXOffset < 0)
    {
        mXOffset++;
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        auto pWheel = static_cast<WorkWheel*>(FindObjectOfType(ReliveTypes::eWheel, mXPos, mYPos - (mSpriteScale * FP_FromInteger(50))));
        if (pWheel)
        {
            pWheel->VStartTurning();
            field_158_wheel_id = pWheel->mBaseGameObjectId;
        }
        mCurrentMotion = eMudMotions::Motion_58_TurnWheelLoop;
    }
}

void Mudokon::Motion_58_TurnWheelLoop()
{
    if (mNextMotion == eMudMotions::Motion_0_Idle)
    {
        auto pWheel = static_cast<WorkWheel*>(sObjectIds.Find_Impl(field_158_wheel_id));
        if (pWheel)
        {
            pWheel->VStopTurning(TRUE);
            field_158_wheel_id = Guid{};
        }
        mCurrentMotion = eMudMotions::Motion_59_TurnWheelEnd;
        mNextMotion = -1;
    }
}

void Mudokon::Motion_59_TurnWheelEnd()
{
    if (mXOffset > field_162_maxXOffset)
    {
        mXOffset--;
    }
    else if (mXOffset < field_162_maxXOffset)
    {
        mXOffset++;
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

s16 Mudokon::MudResponseDelay()
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

s16 Mudokon::FindBirdPortal()
{
    if (sObjectIds.Find_Impl(field_11C_bird_portal_id))
    {
        return 0;
    }

    // TODO: Refactor duplication

    auto pOpenPortal = static_cast<BirdPortal*>(EventGet(kEventPortalOpen));
    if (pOpenPortal)
    {
        const FP xDist = pOpenPortal->mXPos - mXPos;
        if (FP_Abs(xDist) < FP_FromInteger(368))
        {
            if (FP_Abs(pOpenPortal->mHitY - mYPos) < FP_FromInteger(10))
            {
                if (!WallHit(mSpriteScale * FP_FromInteger(50), xDist))
                {
                    if (pOpenPortal->mPortalType == relive::Path_BirdPortal::PortalType::eWorker || pOpenPortal->mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
                    {
                        sActiveHero->ChangeChantState_45BB90(1);
                        field_11C_bird_portal_id = pOpenPortal->mBaseGameObjectId;
                        sGoingToBirdPortalMudCount_5C3012++;
                        field_16C_flags.Set(Flags_16C::eBit3_Unknown);
                        return 1;
                    }
                }
            }
        }
    }

    auto pPortal20 = static_cast<BirdPortal*>(EventGet(kEventOtherPortalOpen));
    if (pPortal20)
    {
        const FP xDist = pPortal20->mXPos - mXPos;
        if (FP_Abs(xDist) < FP_FromInteger(368))
        {
            if (FP_Abs(pPortal20->mHitY - mYPos) < FP_FromInteger(10))
            {
                if (!WallHit(mSpriteScale * FP_FromInteger(50), xDist))
                {
                    if (pPortal20->mPortalType == relive::Path_BirdPortal::PortalType::eWorker || pPortal20->mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
                    {
                        sActiveHero->ChangeChantState_45BB90(1);
                        field_11C_bird_portal_id = pPortal20->mBaseGameObjectId;

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

GameSpeakEvents Mudokon::LastGameSpeak()
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
    if (actualEvent < GameSpeakEvents::eUnknown_1 || actualEvent > GameSpeakEvents::eSorry_24 || sActiveHero->mSpriteScale == mSpriteScale)
    {
        return actualEvent;
    }

    return GameSpeakEvents::eNone_m1;
}

s16 Mudokon::LaughingGasInCurrentScreen()
{
    return gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) != 0 ? gLaughingGasOn_5C1BA4 : 0;
}

void Mudokon::MudEmotionSound(MudSounds idx)
{
    if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
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

s16 Mudokon::IAmNearestToAbe()
{
    const s32 myDistToPlayer = Math_Distance(
        FP_GetExponent(sActiveHero->mXPos),
        FP_GetExponent(sActiveHero->mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos));

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Is there another object that isn't us on the same scale?
        if (pObj != this && pObj->mSpriteScale == sActiveHero->mSpriteScale)
        {
            // Is it a mud who isn't currently talking to abe and is in the same screen?
            if ((pObj->Type() == ReliveTypes::eRingOrLiftMud || pObj->Type() == ReliveTypes::eMudokon) && static_cast<Mudokon*>(pObj)->field_18E_brain_state != Mud_Brain_State::Brain_4_ListeningToAbe && gMap.Is_Point_In_Current_Camera(pObj->mCurrentLevel, pObj->mCurrentPath, pObj->mXPos, pObj->mYPos, 0))
            {
                if (sActiveHero->VIsFacingMe(pObj) && !sActiveHero->VIsFacingMe(this))
                {
                    // Abe is facing the other guy and not us :(
                    return FALSE;
                }

                const s32 otherMudDistanceToPlayer = Math_Distance(
                    FP_GetExponent(sActiveHero->mXPos),
                    FP_GetExponent(sActiveHero->mYPos),
                    FP_GetExponent(pObj->mXPos),
                    FP_GetExponent(pObj->mYPos));

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

u8** Mudokon::GetResBlockForMotion(s16 motion)
{
    if (motion < eMudMotions::Motion_10_LeverUse)
    {
        return field_10_resources_array.ItemAt(0);
    }
    else if (motion < eMudMotions::Motion_11_Chisel)
    {
        if (!field_10_resources_array.ItemAt(1))
        {
            field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbepullResID, TRUE, FALSE));
        }
        return field_10_resources_array.ItemAt(1);
    }
    else if (motion < eMudMotions::Motion_14_CrouchScrub)
    {
        return field_10_resources_array.ItemAt(2);
    }
    else if (motion < eMudMotions::Motion_15_CrouchIdle)
    {
        return field_10_resources_array.ItemAt(3);
    }
    else if (motion < eMudMotions::Motion_45_KnockForward)
    {
        return field_10_resources_array.ItemAt(4);
    }
    else if (motion < eMudMotions::Motion_46_Knockback)
    {
        return field_10_resources_array.ItemAt(5);
    }
    else if (motion < eMudMotions::Motion_48_WalkOffEdge)
    {
        return field_10_resources_array.ItemAt(6);
    }
    else if (motion < eMudMotions::Motion_50_Chant)
    {
        return field_10_resources_array.ItemAt(7);
    }
    else if (motion < eMudMotions::Motion_52_ToDuck)
    {
        return field_10_resources_array.ItemAt(8);
    }
    else if (motion < eMudMotions::Motion_55_DuckKnockback)
    {
        return field_10_resources_array.ItemAt(9);
    }
    else if (motion < eMudMotions::Motion_56_SlapOwnHead)
    {
        return field_10_resources_array.ItemAt(10);
    }
    else if (motion < eMudMotions::Motion_57_TurnWheelBegin)
    {
        return field_10_resources_array.ItemAt(11);
    }
    else if (motion < 60) // MAX ??
    {
        if (!field_10_resources_array.ItemAt(12))
        {
            field_10_resources_array.SetAt(12, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, TRUE, FALSE));
        }
        return field_10_resources_array.ItemAt(12);
    }
    else
    {
        LOG_ERROR("Out of bounds ??");
        return nullptr;
    }
}

void Mudokon::CheckFloorGone()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        VOnTrapDoorOpen();
        mCurrentMotion = eMudMotions::Motion_49_Fall;
    }
}

void Mudokon::ToStand()
{
    field_134_xVelSlowBy = FP_FromInteger(0);
    field_138_unused = 0;
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mCurrentMotion = eMudMotions::Motion_0_Idle;
}

s16 Mudokon::FindWheel(FP xpos, FP ypos)
{
    auto* pWheelTlv = static_cast<relive::Path_WorkWheel*>(
        sPathInfo->TLV_Get_At_4DB4B0(
            FP_GetExponent(xpos), FP_GetExponent(ypos), FP_GetExponent(xpos), FP_GetExponent(ypos), ReliveTypes::eWorkWheel));

    if (pWheelTlv)
    {
        if (!SwitchStates_Get(pWheelTlv->mSwitchId))
        {
            return FindObjectOfType(ReliveTypes::eWheel, xpos, ypos - (mSpriteScale * FP_FromInteger(50))) != 0;
        }
    }
    return 0;
}

s16 Mudokon::GetBrainSubStateResponse(MudAction action)
{
    field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, action);
    if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags_16A::eBit11_get_depressed))
    {
        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
    }
    return field_188_pTblEntry->field_6_sub_state;
}

s16 Mudokon::StopAtWheel()
{
    FP offset = {};
    FP_Rect fpRect = {};

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        offset = -ScaleToGridSize(mSpriteScale) * FP_FromDouble(0.4);
        fpRect.x = mXPos + (offset * FP_FromInteger(2));
        fpRect.w = mXPos;
    }
    else
    {
        offset = ScaleToGridSize(mSpriteScale) * FP_FromDouble(0.4);
        fpRect.x = mXPos;
        fpRect.w = mXPos + (offset * FP_FromInteger(2));
    }

    fpRect.y = mYPos - FP_FromInteger(5);
    fpRect.h = mYPos;

    if (!FindWheel(mXPos + offset, mYPos))
    {
        // No wheel so don't stop
        return 0;
    }

    const PSX_RECT ourRect = {
        FP_GetExponent(fpRect.x),
        FP_GetExponent(fpRect.y),
        FP_GetExponent(fpRect.w),
        FP_GetExponent(fpRect.h)};


    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        auto pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Found another mud who isn't us
        if (pObj != this && pObj->Type() == ReliveTypes::eMudokon)
        {
            const PSX_RECT bRect = pObj->VGetBoundingRect();

            auto pOtherMud = static_cast<Mudokon*>(pObj);

            // Check if the other mud has already take the spot of this work wheel
            if (bRect.x <= ourRect.w && bRect.w >= ourRect.x && bRect.h >= ourRect.y && bRect.y <= ourRect.h)
            {
                if (pOtherMud->field_16A_flags.Get(Flags_16A::eBit7_stopped_at_wheel) || (sPathInfo->TLV_Get_At_4DB4B0(FP_GetExponent(pObj->mXPos), FP_GetExponent(pObj->mYPos), FP_GetExponent(pObj->mXPos), FP_GetExponent(pObj->mYPos), ReliveTypes::eWorkWheel) && pOtherMud->mVelX == FP_FromInteger(0)))
                {
                    // Another mud has stolen this wheel so don't stop
                    return 0;
                }
            }
        }
    }
    return 1;
}

void Mudokon::ToKnockback()
{
    auto pWheel = static_cast<WorkWheel*>(sObjectIds.Find_Impl(field_158_wheel_id));
    if (pWheel)
    {
        pWheel->VStopTurning(TRUE);
        field_158_wheel_id = Guid{};
    }

    Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 32767, this);

    if (mVelX > FP_FromInteger(0))
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mXPos += (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2)) + mVelX;
        }
        else
        {
            mXPos -= (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2)) - mVelX;
        }
    }

    MapFollowMe(TRUE);

    mVelX = FP_FromInteger(0);

    if (mVelY < FP_FromInteger(0))
    {
        mVelY = FP_FromInteger(0);
    }

    Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 32767, this);
    mCurrentMotion = eMudMotions::Motion_46_Knockback;
}

s16 Mudokon::CanAbeSneak()
{
    return sActiveHero->mCurrentMotion == eAbeMotions::Motion_40_SneakLoop_450550 
        || sActiveHero->mCurrentMotion == eAbeMotions::Motion_45_SneakBegin_4507A0 
        || sActiveHero->mCurrentMotion == eAbeMotions::Motion_41_WalkToSneak_450250 
        || sActiveHero->mCurrentMotion == eAbeMotions::Motion_43_MidWalkToSneak_450380 
        || sActiveHero->mCurrentMotion == eAbeMotions::Motion_42_SneakToWalk_4503D0 
        || sActiveHero->mCurrentMotion == eAbeMotions::Motion_44_MidSneakToWalk_450500;
}

void Mudokon::MoveOnLine()
{
    PlatformBase* pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    CheckFloorGone();

    const FP oldXPos = mXPos;
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
    }

    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
    }

    if (BaseAliveGameObjectCollisionLine)
    {
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            const PSX_RECT bRect = VGetBoundingRect();
            VOnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {bRect.w, static_cast<s16>(bRect.h + 5)},
                ObjList_5C1B78, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
        }
    }
    else
    {
        field_134_xVelSlowBy = FP_FromDouble(0.3); // TODO: or 2.99 ??
        BaseAliveGameObjectLastLineYPos = mYPos;
        mCurrentMotion = eMudMotions::Motion_48_WalkOffEdge;
        mXPos = oldXPos + mVelX;
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
    mNextMotion = eMudMotions::Motion_57_TurnWheelBegin;
    RemoveAlerted();
    field_16A_flags.Set(Flags_16A::eBit7_stopped_at_wheel);
    field_18E_brain_state = Mud_Brain_State::Brain_3_TurnWheel;
    return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
}

bool Mudokon::NeedToTurnAround()
{
    Mudokon* pAboveMud = static_cast<Mudokon*>(FindObjectOfType(ReliveTypes::eMudokon, mXPos, mYPos - FP_FromInteger(5)));
    if (sAlertedMudCount_5C3010 > 1 && pAboveMud && !VIsFacingMe(pAboveMud) && pAboveMud->mCurrentMotion != eMudMotions::Motion_2_StandingTurn)
    {
        // A mud is in the same grid us and not turning, so we turn to face it. This causes "stacked" muds to all face the same way bar 1.
        // When telling them all to "wait" they will face Abe and then spin around again :D.
        return true;
    }
    else if (!VIsFacingMe(sActiveHero) && (sAlertedMudCount_5C3010 <= 1 || !FindObjectOfType(ReliveTypes::eMudokon, mXPos, mYPos - FP_FromInteger(5))))
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

void Mudokon::ReduceXVelocityBy(FP velXScaleBy)
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX = mVelX - (mSpriteScale * velXScaleBy);
        if (mVelX >= FP_FromInteger(0))
        {
            if (FP_GetExponent(mVelX))
            {
                MoveOnLine();
            }
        }
        else
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX = (mSpriteScale * velXScaleBy) + mVelX;
        if (mVelX <= FP_FromInteger(0))
        {
            if (FP_GetExponent(mVelX))
            {
                MoveOnLine();
            }
        }
    }
    else
    {
        mVelX = FP_FromInteger(0);
    }
}

void Mudokon::CheckKnockedOntoABomb()
{
    const PSX_RECT bMudRect = VGetBoundingRect();

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eMine || pObj->Type() == ReliveTypes::eUXB)
        {
            const PSX_RECT bBombRect = pObj->VGetBoundingRect();

            // TODO: Almost same as PSX_Rects_overlap_no_adjustment but checks < rather than <=
            if (bMudRect.x <= bBombRect.w && bMudRect.w >= bBombRect.x && bMudRect.h >= bBombRect.y && bMudRect.y <= bBombRect.h)
            {
                pObj->VTakeDamage(this);
            }
        }
    }
}

void Mudokon::HurtSoundPitchedToHealth()
{
    const u8 random = Math_RandomRange(0, 127) >= 64 ? 16 : 9; // random hurt sound
    const s16 min = FP_GetExponent((FP_FromInteger(1) - mHealth) / FP_FromDouble(0.15));
    const s16 bInCamera = gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0);
    Mudokon_SFX(static_cast<MudSounds>(random), bInCamera ? 0 : 80, Math_RandomRange(200 * min, 40 * (5 * min + 5)), this);
}

void Mudokon::TakeASlap(BaseGameObject* pFrom)
{
    if (field_180_emo_tbl != Mud_Emotion::eSick_7)
    {
        mHealth -= FP_FromDouble(0.077);
    }

    if (mHealth > FP_FromInteger(0))
    {
        HurtSoundPitchedToHealth();
        mXOffset = field_162_maxXOffset;
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

            field_18E_brain_state = Mud_Brain_State::Brain_4_ListeningToAbe;
            field_190_brain_sub_state = field_188_pTblEntry->field_6_sub_state;
        }

        if (field_180_emo_tbl != Mud_Emotion::eSad_3 || IsActiveHero(pFrom))
        {
            field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
        }
    }
    else
    {
        Mudokon_SFX(MudSounds::eHurt2_9, 0, 1000, this);
        Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 32767, this);
        EventBroadcast(kEventMudokonDied, sActiveHero);
        field_16A_flags.Clear(Flags_16A::eBit2_persist_and_reset_offscreen);
        mHealth = FP_FromInteger(0);
        field_18E_brain_state = Mud_Brain_State::Brain_5_ShrivelDeath;
        mCurrentMotion = eMudMotions::Motion_45_KnockForward;
        field_194_timer = sGnFrame + 90;
        mNextMotion = -1;
        VUpdateResBlock();
    }
}

s32 Mudokon::GetResponseEntryIdx()
{
    if (field_188_pTblEntry)
    {
        const size_t idx = field_188_pTblEntry - &kMudEmoTable_55C790.field_0_data[0][0];
        return static_cast<s32>(idx); // size_t for 64bit, but we know there isnt over s32 limits of entries in the table
    }
    return -1;
}

const MudEmotionTableEntry* Mudokon::GetResponseEntry(s32 idx)
{
    if (idx == -1)
    {
        return nullptr;
    }

    return &kMudEmoTable_55C790.field_0_data[0][0] + idx;
}
