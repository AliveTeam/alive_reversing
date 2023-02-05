#include "stdafx.h"
#include "Mudokon.hpp"
#include "../relive_lib/Function.hpp"
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
#include "../relive_lib/SwitchStates.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Spark.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Lever.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "PossessionFlicker.hpp"
#include "ScreenShake.hpp"
#include "MainMenu.hpp"
#include "LCDStatusBoard.hpp"
#include "MusicTrigger.hpp"
#include "Blood.hpp"
#include "Gibs.hpp"
#include "Bullet.hpp"
#include "LaughingGas.hpp"
#include "Grid.hpp"
#include "AnimationCallBacks.hpp"
#include "Map.hpp"
#include "../relive_lib/FatalError.hpp"
#include "../relive_lib/FixedPoint.hpp"

s16 sGoingToBirdPortalMudCount_5C3012 = 0;

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

const TintEntry kMudTints_55C744[16] = {
    {EReliveLevelIds::eMenu, 87u, 103u, 67u},
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
    {EReliveLevelIds::eCredits, 87u, 103u, 67u}};

using TMudBrainStateFunction = decltype(&Mudokon::Brain_0_GiveRings);

const TMudBrainStateFunction sMudokonBrainTable[10] = {
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

const TMudStateFunction sMudokonMotionTable[60] = {
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
            ALIVE_FATAL("Don't know about emo type %d", static_cast<s32>(emo));
    }
}

Mudokon::Mudokon(relive::Path_Mudokon* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(18)
{
    field_140_last_event_index = -1;

    field_11C_bird_portal_id = Guid{};
    field_158_wheel_id = Guid{};
    mBaseGameObjectTlvInfo = tlvId;
    field_194_timer = 0;
    mBrainState = Mud_Brain_State::Brain_0_GiveRings;
    mBrainSubState = 0;
    mNextMotion = -1;
    mReturnToPreviousMotion = false;
    field_13C_voice_pitch = 0;

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Mudokon_Idle));

    GetAnimation().SetSemiTrans(true);

    SetDoPurpleLightEffect(true);

    SetRestoredFromQuickSave(false);
    SetCanSetOffExplosives(true);

    SetTint(kMudTints_55C744, mCurrentLevel);

    field_17E_delayed_speak = MudAction::eNone_17;

    mPersistAndResetOffscreen = false;
    mAlerted = false;
    mBlind = pTlv->mBlind == relive::reliveChoice::eYes ? true : false;
    mFollowingAbe = false;
    mStandingForSadOrAngry = false;
    mStoppedAtWheel = false;
    mDoAngry = false;
    mSeenWhileSick = false;

    mAlertEnemies = true;
    mNoiseUnknown = false;
    mMakeSadNoise = false;

    field_180_emo_tbl = TLV_Emo_To_Internal_Emo(pTlv->mEmotion);
    field_188_pTblEntry = nullptr;
    field_182_speak_event = GameSpeakEvents::eNone_m1;
    field_120_angry_switch_id = pTlv->mAngrySwitchId;
    mIsMudStandingUp2 = false;
    mEscaping = false;
    field_198_turning_wheel_timer = 0;

    switch (pTlv->mJob)
    {
        case relive::Path_Mudokon::MudJobs::eChisle:
            mBrainState = Mud_Brain_State::Brain_1_Chisel;
            mChiselMud = true;
            break;

        case relive::Path_Mudokon::MudJobs::eSitScrub:
            mBrainState = Mud_Brain_State::Brain_2_CrouchScrub;
            break;

        case relive::Path_Mudokon::MudJobs::eAngryWorker:
            mBrainState = Mud_Brain_State::Brain_8_AngryWorker;
            field_180_emo_tbl = Mud_Emotion::eAngry_1;
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
            mGiveRingWithoutPassword = pTlv->mGiveRingWithoutPassword == relive::reliveChoice::eYes ? true : false;
            mAbeHasRing = false;
            mBrainState = Mud_Brain_State::Brain_0_GiveRings;
            break;

        default:
            break;
    }

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        mBrainState = Mud_Brain_State::Brain_4_ListeningToAbe;
    }

    mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::BlindMud));
    mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::AngryMud));
    mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::SadMud));
    mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::WiredMud));
    mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::SickMud));

    if (mBlind)
    {
        SetPal(Mud_Emotion::eNormal_0);
    }

    GetAnimation().SetFlipX(pTlv->mFacing == relive::reliveXDirection::eLeft);

    SetType(ReliveTypes::eMudokon);

    field_13C_voice_pitch = pTlv->mVoicePitch;
    field_17A_rescue_switch_id = pTlv->mRescueSwitchId;

    mPersistAndResetOffscreen = pTlv->mPersistAndResetOffscreen == relive::reliveChoice::eYes ? true : false;
    mWorkAfterTurningWheel = pTlv->mWorkAfterTurningWheel == relive::reliveChoice::eYes ? true : false;
    mGetDepressed = pTlv->mGetsDepressed == relive::reliveChoice::eYes ? true : false;
    mRingAndAngryMudTimeout = pTlv->mRingPulseInterval ? true : false;

    field_17C_stand_idle_timer = 0;

    VStackOnObjectsOfType(ReliveTypes::eMudokon);

    if (field_180_emo_tbl == Mud_Emotion::eSick_7)
    {
        mBrainState = Mud_Brain_State::Brain_9_Sick;
        mbMotionChanged = true;
        mCurrentMotion = eMudMotions::Motion_15_CrouchIdle;
    }
    else
    {
        mCurrentMotion = eMudMotions::Motion_0_Idle;
    }

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_ZapLinesMudsElum_Half_9);
        SetScale(Scale::Bg);
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_ZapLinesElumMuds_28);
        SetScale(Scale::Fg);
    }

    FP hitX = {};
    FP hitY = {};
    const auto bCollision = gCollisions->Raycast(
        FP_FromInteger(pTlv->mTopLeftX),
        FP_FromInteger(pTlv->mTopLeftY),
        FP_FromInteger(pTlv->mBottomRightX),
        FP_FromInteger(pTlv->mBottomRightY),
        &BaseAliveGameObjectCollisionLine,
        &hitX,
        &hitY,
        GetScale() == Scale::Fg ? kFgFloor : kBgFloor);

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    if (bCollision)
    {
        mYPos = hitY;
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
            BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            const PSX_RECT bRect = VGetBoundingRect();
            OnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {bRect.w, static_cast<s16>(bRect.h + 5)},
                gPlatformsArray);
        }
    }

    GetAnimation().SetFnPtrArray(gAbe_Anim_Frame_Fns);
    BaseAliveGameObjectLastLineYPos = mYPos;

    if (mBrainState == Mud_Brain_State::Brain_2_CrouchScrub || mBrainState == Mud_Brain_State::Brain_1_Chisel)
    {
        MapFollowMe(true);
    }

    mNotRescued = true;
    mBrainSubState = 0;
    field_118_tlvInfo = mBaseGameObjectTlvInfo;

    field_128_angry_timer = 0;
    field_160_delayed_speak = MudSounds::eNone;
    field_162_maxXOffset = mXOffset;

    CreateShadow();

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

s16 sAlertedMudCount_5C3010 = 0;
static bool sIsMudStandingUp = false;

void Mudokon::LoadAnimations()
{
    for (auto& animId : kMudMotionAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

s32 Mudokon::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const MudokonSaveState*>(pBuffer);

    auto pTlv = static_cast<relive::Path_Mudokon*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_40_tlvInfo));

    const auto oldCount = sAlertedMudCount_5C3010;
    auto pMud = relive_new Mudokon(pTlv, pState->field_40_tlvInfo);

    if (sAlertedMudCount_5C3010 != oldCount)
    {
        sAlertedMudCount_5C3010 = oldCount;
        LOG_INFO("Override alert count to %d", oldCount);
    }

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

        pMud->mCurrentPath = pState->field_14_path_number;
        pMud->mCurrentLevel = pState->field_16_lvl_number;
        pMud->SetSpriteScale(pState->field_18_sprite_scale);

        pMud->mRGB.SetRGB(pState->field_1C_r, pState->field_1E_g, pState->field_20_b);

        pMud->mCurrentMotion = pState->field_24_current_motion;

        pMud->GetAnimation().Set_Animation_Data(pMud->GetAnimRes(kMudMotionAnimIds[pMud->mCurrentMotion]));

        pMud->GetAnimation().SetCurrentFrame(pState->field_26_anim_current_frame);
        pMud->GetAnimation().SetFrameChangeCounter(pState->field_28_anim_frame_change_counter);

        pMud->GetAnimation().SetFlipX(pState->field_22_bFlipX & 1);
        pMud->GetAnimation().SetRender(pState->field_2A_bAnimRender & 1);

        pMud->SetDrawable(pState->field_2B_bDrawable & 1);

        if (IsLastFrame(&pMud->GetAnimation()))
        {
            pMud->GetAnimation().SetIsLastFrame(true);
        }

        pMud->mHealth = pState->field_2C_health;
        pMud->mCurrentMotion = pState->field_30_current_motion;
        pMud->mNextMotion = pState->field_32_next_motion;
        pMud->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->field_34_lastLineYPos);

        pMud->SetCanBePossessed(pState->field_3C_can_be_possessed & 1);
        pMud->SetRestoredFromQuickSave(true);

        pMud->BaseAliveGameObjectCollisionLineType = pState->field_36_line_type;
        pMud->field_11C_bird_portal_id = pState->field_4C_portal_id;
        pMud->field_120_angry_switch_id = pState->field_50_angry_trigger;
        pMud->field_124_laugh_and_crouch_timer = pState->field_54_laugh_and_crouch_timer;
        pMud->field_128_angry_timer = pState->field_58_angry_timer;
        pMud->field_13C_voice_pitch = pState->field_5E_voice_pitch;
        pMud->field_158_wheel_id = pState->field_60_wheel_id;
        pMud->field_160_delayed_speak = pState->field_68;
        pMud->field_162_maxXOffset = pState->field_6A_maxXOffset;

        pMud->mNotRescued = pState->mNotRescued;
        pMud->mPersistAndResetOffscreen = pState->mPersistAndResetOffscreen;
        pMud->mAlerted = pState->mAlerted;
        pMud->mBlind = pState->mBlind;
        pMud->mFollowingAbe = pState->mFollowingAbe;
        pMud->mStandingForSadOrAngry = pState->mStandingForSadOrAngry;
        pMud->mStoppedAtWheel = pState->mStoppedAtWheel;
        pMud->mDoAngry = pState->mDoAngry;
        pMud->mSeenWhileSick = pState->mSeenWhileSick;
        pMud->mWorkAfterTurningWheel = pState->mWorkAfterTurningWheel;

        pMud->mReturnToPreviousMotion = pState->mReturnToPreviousMotion;
        pMud->mGetDepressed = pState->mGetDepressed;
        pMud->mAlertEnemies = pState->mAlertEnemies;
        pMud->mNoiseUnknown = pState->mNoiseUnknown;
        pMud->mMakeSadNoise = pState->mMakeSadNoise;
        pMud->mRingAndAngryMudTimeout = pState->mRingAndAngryMudTimeout;
        pMud->mAbeHasRing = pState->mAbeHasRing;
        pMud->mIsMudStandingUp2 = pState->mIsMudStandingUp2;

        if (pMud->mAlerted)
        {
            sAlertedMudCount_5C3010++;
            LOG_INFO("Alerted added from save state %d", sAlertedMudCount_5C3010);
        }

        pMud->field_178_brain_sub_state2 = pState->field_70_brain_sub_state2;
        pMud->field_17C_stand_idle_timer = pState->field_72_stand_idle_timer;
        pMud->field_17E_delayed_speak = pState->field_74_delayed_speak;
        pMud->field_180_emo_tbl = pState->field_76_emo_tlb;
        pMud->field_182_speak_event = pState->field_78_speak_event;
        pMud->field_184_next_motion2 = pState->field_7A_motion;
        pMud->mBrainState = pState->field_7C_brain_state;
        pMud->mBrainSubState = pState->field_7E_brain_sub_state;
        pMud->field_194_timer = pState->field_80_timer;

        if (pState->mIsMudStandingUp2)
        {
            sIsMudStandingUp = true;
        }

        pMud->field_188_pTblEntry = GetResponseEntry(pState->field_84_response_entry_idx);
    }

    return sizeof(MudokonSaveState);
}

s32 Mudokon::VGetSaveState(u8* pSaveBuffer)
{
    if (GetElectrocuted())
    {
        return 0;
    }

    auto pState = reinterpret_cast<MudokonSaveState*>(pSaveBuffer);

    pState->mType = ReliveTypes::eRingOrLiftMud;

    pState->field_4_xpos = mXPos;
    pState->field_8_ypos = mYPos;
    pState->field_C_velx = mVelX;
    pState->field_10_vely = mVelY;

    pState->field_44_velx_slow_by = field_134_xVelSlowBy;

    pState->field_14_path_number = mCurrentPath;
    pState->field_16_lvl_number = mCurrentLevel;
    pState->field_18_sprite_scale = GetSpriteScale();

    pState->field_1C_r = mRGB.r;
    pState->field_1E_g = mRGB.g;
    pState->field_20_b = mRGB.b;

    pState->field_22_bFlipX = GetAnimation().GetFlipX();
    pState->field_24_current_motion = mCurrentMotion;
    pState->field_26_anim_current_frame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    pState->field_28_anim_frame_change_counter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());
    pState->field_2B_bDrawable = GetDrawable();
    pState->field_2A_bAnimRender = GetAnimation().GetRender();
    pState->field_2C_health = mHealth;
    pState->field_30_current_motion = mCurrentMotion;
    pState->field_32_next_motion = mNextMotion;
    pState->field_34_lastLineYPos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);
    pState->field_36_line_type = -1;

    pState->field_3C_can_be_possessed = GetCanBePossessed();

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
    pState->field_54_laugh_and_crouch_timer = field_124_laugh_and_crouch_timer;
    pState->field_58_angry_timer = field_128_angry_timer;
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

    pState->field_68 = field_160_delayed_speak;
    pState->field_6A_maxXOffset = field_162_maxXOffset;

    pState->mNotRescued = mNotRescued;
    pState->mPersistAndResetOffscreen = mPersistAndResetOffscreen;
    pState->mAlerted = mAlerted;
    pState->mBlind = mBlind;
    pState->mFollowingAbe = mFollowingAbe;
    pState->mStandingForSadOrAngry = mStandingForSadOrAngry;
    pState->mStoppedAtWheel = mStoppedAtWheel;
    pState->mDoAngry = mDoAngry;
    pState->mSeenWhileSick = mSeenWhileSick;
    pState->mWorkAfterTurningWheel = mWorkAfterTurningWheel;
    pState->mReturnToPreviousMotion = mReturnToPreviousMotion;
    pState->mGetDepressed = mGetDepressed;
    pState->mAlertEnemies = mAlertEnemies;
    pState->mNoiseUnknown = mNoiseUnknown;
    pState->mMakeSadNoise = mMakeSadNoise;
    pState->mRingAndAngryMudTimeout = mRingAndAngryMudTimeout;
    pState->mAbeHasRing = mAbeHasRing;
    pState->mIsMudStandingUp2 = mIsMudStandingUp2;

    pState->field_70_brain_sub_state2 = field_178_brain_sub_state2;
    pState->field_72_stand_idle_timer = field_17C_stand_idle_timer;
    pState->field_74_delayed_speak = field_17E_delayed_speak;
    pState->field_76_emo_tlb = field_180_emo_tbl;
    pState->field_78_speak_event = field_182_speak_event;
    pState->field_7A_motion = field_184_next_motion2;
    pState->field_7C_brain_state = mBrainState;
    pState->field_7E_brain_sub_state = mBrainSubState;
    pState->field_80_timer = field_194_timer;

    pState->field_84_response_entry_idx = GetResponseEntryIdx();

    return sizeof(MudokonSaveState);
}

void Mudokon::VUpdate()
{
    if (GetRestoredFromQuickSave())
    {
        SetRestoredFromQuickSave(false);
        if (BaseAliveGameObjectCollisionLineType != -1)
        {
            gCollisions->Raycast(
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
                OnCollisionWith(
                    {bRect.x, static_cast<s16>(bRect.y + 5)},
                    {bRect.w, static_cast<s16>(bRect.h + 5)},
                    gPlatformsArray);
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
                    mEscaping = true;
                    if (mBrainState == Mud_Brain_State::Brain_6_Escape && mBrainSubState == 3)
                    {
                        static_cast<BirdPortal*>(pObj)->VPortalClipper(1);
                        GetAnimation().SetRenderLayer(GetSpriteScale() != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30);
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
        SetDead(true);
        return;
    }

    if (!sControlledCharacter || FP_Abs(mXPos - sControlledCharacter->mXPos) > FP_FromInteger(750))
    {
        GetAnimation().SetAnimate(false);
        GetAnimation().SetRender(false);
        return;
    }

    if (!sControlledCharacter || FP_Abs(mYPos - sControlledCharacter->mYPos) > FP_FromInteger(520))
    {
        GetAnimation().SetAnimate(false);
        GetAnimation().SetRender(false);
        return;
    }

    if (mHealth > FP_FromInteger(0))
    {
        GetAnimation().SetAnimate(true);
        GetAnimation().SetRender(true);
    }

    //DDCheat::DebugStr_4F5560("\nMotion = %s BrainState = %s\n", sMudMotionStateNames[mCurrentMotion], sMudAiStateNames[static_cast<s32>(mBrainState)]);
    //LOG_INFO(sMudMotionStateNames[mCurrentMotion] << " " << static_cast<s32>(mBrainState));
    //LOG_INFO(sMudMotionStateNames[mCurrentMotion] << " " << sMudAiStateNames[static_cast<s32>(mBrainState)]);

    const s16 oldMotion = mCurrentMotion;

    mBrainSubState = InvokeMemberFunction(this, sMudokonBrainTable, mBrainState);

    const FP oldXPos = mXPos;
    const FP oldYPos = mYPos;

    InvokeMemberFunction(this, sMudokonMotionTable, mCurrentMotion);

    if (oldXPos != mXPos || oldYPos != mYPos)
    {
        BaseAliveGameObjectPathTLV = gPathInfo->TlvGetAt(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
        VOnTlvCollision(BaseAliveGameObjectPathTLV);
    }

    if (oldMotion != mCurrentMotion || mbMotionChanged)
    {
        mbMotionChanged = false;
        VUpdateResBlock();
    }
    else if (mReturnToPreviousMotion)
    {
        mCurrentMotion = mPreviousMotion;
        VUpdateResBlock();
        GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
        mReturnToPreviousMotion = false;
    }
}


void Mudokon::SetPal(Mud_Emotion emotion)
{
    switch (emotion)
    {
        case Mud_Emotion::eNormal_0:
            if (mBlind)
            {
                mRGB.SetRGB(63, 63, 63);
                GetAnimation().LoadPal(GetPalRes(PalId::BlindMud));
            }
            else
            {
                mRGB.SetRGB(87, 103, 67);
                GetAnimation().ReloadPal();
            }
            break;

        case Mud_Emotion::eAngry_1:
        case Mud_Emotion::eAggressive_2:
            mRGB.SetRGB(63, 63, 63);
            GetAnimation().LoadPal(GetPalRes(PalId::AngryMud));
            break;

        case Mud_Emotion::eSad_3:
        case Mud_Emotion::eSuicidal_4:
            mRGB.SetRGB(63, 63, 63);
            GetAnimation().LoadPal(GetPalRes(PalId::SadMud));
            break;

        case Mud_Emotion::eHappy_5:
        case Mud_Emotion::eWired_6:
            mRGB.SetRGB(74, 74, 74);
            GetAnimation().LoadPal(GetPalRes(PalId::WiredMud));
            break;

        case Mud_Emotion::eSick_7:
            mRGB.SetRGB(63, 63, 63);
            GetAnimation().LoadPal(GetPalRes(PalId::SickMud));
            break;
    }
}

void Mudokon::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        if (!mbGotShot)
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
                mBrainState = Mud_Brain_State::Brain_7_FallAndSmackDeath;
                mBrainSubState = 0;
                mHealth = FP_FromInteger(0);
                EventBroadcast(kEventMudokonDied, this);
                break;
            }
        }

        pTlvIter = gPathInfo->TlvGetAt(
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
        return true;
    }
    else if (pTarget->mXPos > mXPos && !GetAnimation().GetFlipX())
    {
        return true;
    }
    else if (pTarget->mXPos < mXPos && GetAnimation().GetFlipX())
    {
        return true;
    }
    return false;
}

Mudokon::~Mudokon()
{
    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds.Find_Impl(field_11C_bird_portal_id));
    if (mHealth <= FP_FromInteger(0))
    {
        // TODO: Refactor all access to helpers
        gKilledMudokons++;
        if (gZulagNumber < ALIVE_COUNTOF(sSavedKilledMudsPerZulag_5C1B50.mData))
        {
            sSavedKilledMudsPerZulag_5C1B50.mData[gZulagNumber]++;
        }
    }

    auto pWheel = static_cast<WorkWheel*>(sObjectIds.Find_Impl(field_158_wheel_id));
    if (pWheel)
    {
        pWheel->VStopTurning(true);
        field_158_wheel_id = Guid{};
    }

    RemoveAlerted();

    if (mEscaping)
    {
        sGoingToBirdPortalMudCount_5C3012--;
        mEscaping = false;
        if (!sGoingToBirdPortalMudCount_5C3012)
        {
            if (pBirdPortal)
            {
                pBirdPortal->VKillPortalClipper();
                pBirdPortal->VGiveShrykull(true);
            }
        }
        field_11C_bird_portal_id = Guid{};
    }

    if (!mNotRescued || mHealth <= FP_FromInteger(0) || GetElectrocuted())
    {
        Path::TLV_Reset(field_118_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(field_118_tlvInfo, -1, 0, 0);
    }

    if (!mBrainState && mBrainSubState > 4u)
    {
        if (field_164_ring_pulse_interval > 0)
        {
            sActiveHero->mRingPulseTimer = sGnFrame + field_164_ring_pulse_interval;
        }
        else
        {
            sActiveHero->mRingPulseTimer = sGnFrame + 200000;
        }

        sActiveHero->mHaveShrykull = false;

        if (field_168_ring_type == RingTypes::eHealing_Emit_Effect_11)
        {
            sActiveHero->mHaveHealing = true;
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

    if (mBrainState == Mud_Brain_State::Brain_0_GiveRings ||
        !mPersistAndResetOffscreen)
    {
        SetDead(true);
    }

    if (GetDead())
    {
        // TODO: Duplicated in dtors + other places
        if (mEscaping)
        {
            sGoingToBirdPortalMudCount_5C3012--;
            mEscaping = false;
            if (!sGoingToBirdPortalMudCount_5C3012)
            {
                if (pBirdPortal)
                {
                    pBirdPortal->VKillPortalClipper();
                    pBirdPortal->VGiveShrykull(true);
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

bool Mudokon::VTakeDamage(BaseGameObject* pFrom)
{
    switch (pFrom->Type())
    {
        case ReliveTypes::eBullet:
        {
            mbGotShot = true;
            if (mHealth <= FP_FromInteger(0))
            {
                return true;
            }

            auto pBullet = static_cast<Bullet*>(pFrom);
            switch (pBullet->mBulletType)
            {
                case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
                case BulletType::eNormalBullet_2:
                {
                    relive_new Blood(
                        mXPos,
                        mYPos - (FP_FromInteger(30) * GetSpriteScale()),
                        pBullet->XDistance() <= FP_FromInteger(0) ? FP_FromInteger(-24) : FP_FromInteger(24),
                        FP_FromInteger(0),
                        GetSpriteScale(),
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
                    mbGotShot = false;
                    mHealth = FP_FromInteger(1);
                    return false;
                }

                // Nothing saved us, get shot
                relive_new Blood(
                    mXPos,
                    mYPos - (FP_FromInteger(30) * GetSpriteScale()),
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    GetSpriteScale(),
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
                return false;
            }
            mPersistAndResetOffscreen = false;
            mHealth = FP_FromInteger(0);
            mBrainState = Mud_Brain_State::Brain_5_ShrivelDeath;
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
            return true;

        case ReliveTypes::eDrill:
        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eMeatSaw:
        case ReliveTypes::eAirExplosion:
            if (mHealth <= FP_FromInteger(0) || (FindObjectOfType(ReliveTypes::eTorturedMud, mXPos, mYPos - FP_FromInteger(50)) && mBrainState == Mud_Brain_State::Brain_9_Sick))
            {
                return true;
            }

            mHealth = FP_FromInteger(0);

            relive_new Gibs(
                mBlind ? GibType::BlindMud_4 : GibType::Mud_3,
                mXPos,
                mYPos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                GetSpriteScale(),
                0);

            relive_new Gibs(
                mBlind ? GibType::BlindMud_4 : GibType::Mud_3,
                mXPos,
                mYPos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                GetSpriteScale(),
                0);

            SetDead(true);
            SetPal(Mud_Emotion::eNormal_0);
            EventBroadcast(kEventMudokonDied, sActiveHero);
            return true;

        case ReliveTypes::eElectricWall:
            Mudokon_SFX(MudSounds::eDeathDropScream_15, 0, 0, this);
            EventBroadcast(kEventMudokonDied, this);
            return true;

        case ReliveTypes::eFleech:
            if (mHealth <= FP_FromInteger(0))
            {
                return true;
            }

            mHealth -= FP_FromDouble(0.200988769531); // TODO Do we need this level of accuracy?? 0x3374;
            if (mHealth <= FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
                Mudokon_SFX(MudSounds::eHurt1_16, 0, 1000, this);
                mPersistAndResetOffscreen = false;
                mBrainState = Mud_Brain_State::Brain_5_ShrivelDeath;
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
                    GetSpriteScale(),
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
            return true;

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
            return true;

        case ReliveTypes::eAbilityRing:
            return false;

        case ReliveTypes::eMudokon:
            if (static_cast<Mudokon*>(pFrom)->mCurrentMotion != eMudMotions::Motion_38_Punch || mHealth <= FP_FromInteger(0))
            {
                return true;
            }
            SetPal(Mud_Emotion::eNormal_0);
            TakeASlap(pFrom);
            return true;

        case ReliveTypes::eShrykull:
        case ReliveTypes::eElectrocute:
            if (mHealth <= FP_FromInteger(0))
            {
                return true;
            }
            mHealth = FP_FromInteger(0);
            EventBroadcast(kEventMudokonDied, this);
            SetPal(Mud_Emotion::eNormal_0);
            SetDead(true);
            return true;

        case ReliveTypes::eSlamDoor:
            if (mHealth <= FP_FromInteger(0) || mCurrentMotion == eMudMotions::Motion_36_RunJumpMid)
            {
                return true;
            }
            ToKnockback();
            VUpdateResBlock();
            return true;

        case ReliveTypes::eSlog:
            if (mHealth <= FP_FromInteger(0))
            {
                return true;
            }
            mPersistAndResetOffscreen = false;
            mHealth = FP_FromInteger(0);
            mBrainState = Mud_Brain_State::Brain_5_ShrivelDeath;
            field_194_timer = sGnFrame + 90;
            mCurrentMotion = eMudMotions::Motion_45_KnockForward;
            EventBroadcast(kEventMudokonDied, this);
            VUpdateResBlock();
            SetPal(Mud_Emotion::eNormal_0);
            return true;

        default:
            if (mHealth <= FP_FromInteger(0))
            {
                return true;
            }
            HurtSoundPitchedToHealth();
            mCurrentMotion = eMudMotions::Motion_45_KnockForward;
            mNextMotion = -1;
            field_194_timer = sGnFrame + 30;
            VUpdateResBlock();
            if (mChiselMud)
            {
                mBrainState = Mud_Brain_State::Brain_1_Chisel;
                mBrainSubState = 8;
            }
            else
            {
                mBrainState = Mud_Brain_State::Brain_2_CrouchScrub;
                mBrainSubState = 7;
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
                    mPersistAndResetOffscreen = false;
                    mBrainState = Mud_Brain_State::Brain_5_ShrivelDeath;
                    field_194_timer = sGnFrame + 90;
                    EventBroadcast(kEventMudokonDied, this);
                }
            }
            SetPal(Mud_Emotion::eNormal_0);
            return true;
    }
}

s16 Mudokon::TurningWheelHelloOrAllYaResponse()
{
    if (!mWorkAfterTurningWheel)
    {
        return mBrainSubState;
    }

    auto pWheelTlv = static_cast<relive::Path_WorkWheel*>(gPathInfo->TLV_Get_At(
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
        return mBrainSubState;
    }

    field_198_turning_wheel_timer = 0;
    mStoppedAtWheel = false;
    field_194_timer = sGnFrame + 45;
    return 1;
}

void Mudokon::VUpdateResBlock()
{
    GetAnimation().Set_Animation_Data(GetAnimRes(kMudMotionAnimIds[mCurrentMotion]));
}


s16 Mudokon::Brain_0_GiveRings()
{
    switch (mBrainSubState)
    {
        case Brain_0_GiveRings::eBrain0_Inactive_0:
            if (gMap.Is_Point_In_Current_Camera(
                    sActiveHero->mCurrentLevel,
                    sActiveHero->mCurrentPath,
                    sActiveHero->mXPos,
                    sActiveHero->mYPos,
                    0))
            {
                mAbeHasRing = false;
                field_194_timer = sGnFrame + 60;
                mNextMotion = eMudMotions::Motion_0_Idle;

                if (sActiveHero->mRingPulseTimer)
                {
                    mAbeHasRing = true;
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
            if (mGiveRingWithoutPassword && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && gMap.Is_Point_In_Current_Camera(sActiveHero->mCurrentLevel, sActiveHero->mCurrentPath, sActiveHero->mXPos, sActiveHero->mYPos, 0)
                && !sActiveHero->mRingPulseTimer)
            {
                field_194_timer = MudResponseDelay() + sGnFrame + 20;
                return Brain_0_GiveRings::eBrain0_SaysOkay_6;
            }
            else
            {
                const s32 lastEventIdx = gEventSystem->mLastEventIndex;
                const bool bSameAsLastIdx = field_140_last_event_index == lastEventIdx;
                if (!bSameAsLastIdx)
                {
                    field_140_last_event_index = lastEventIdx;
                }

                if (bSameAsLastIdx || gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1 || gEventSystem->mLastEvent == GameSpeakEvents::eSameAsLast_m2)
                {
                    if (!mAbeHasRing && static_cast<s32>(sGnFrame) > field_194_timer)
                    {
                        return Brain_0_GiveRings::eBrain0_Shrug_5;
                    }
                }
                else
                {
                    field_194_timer = MudResponseDelay() + sGnFrame + 20;
                    if (gEventSystem->mLastEvent == GameSpeakEvents::eAbe_Hello_9)
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
                    GetSpriteScale());

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
                    sActiveHero->GetSpriteScale());

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

                sActiveHero->mHaveShrykull = false;

                if (field_168_ring_type == RingTypes::eHealing_Emit_Effect_11)
                {
                    sActiveHero->mHaveHealing = true;
                }

                mNextMotion = eMudMotions::Motion_0_Idle;
                return Brain_0_GiveRings::eBrain0_RecievingRing2_10;
            }
            break;

        case Brain_0_GiveRings::eBrain0_RecievingRing2_10:
            mAbeHasRing = true;
            return 2;

        default:
            break;
    }

    return mBrainSubState;
}

const s16 kDelayTable_55CF7C[6] = {
    0,
    6,
    12,
    18,
    24,
    30,
};
s16 sDelayIdx_5C3014 = 0;

s16 Mudokon::Brain_1_Chisel()
{
    if (FindBirdPortal())
    {
        AddAlerted();
        mBrainState = Mud_Brain_State::Brain_6_Escape;
        return Brain_1_Chisle::eBrain1_StartToChisle_0;
    }

    if (mIsMudStandingUp2)
    {
        if (mBrainSubState != Brain_1_Chisle::eBrain1_StandUp_3)
        {
            sIsMudStandingUp = false;
            mIsMudStandingUp2 = false;
        }
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak();
    if (lastSpeak == GameSpeakEvents::Slig_LookOut_6 || EventGet(kEventShooting))
    {
        if (mBrainSubState != Brain_1_Chisle::eBrain1_StandUp_3 && mBrainSubState != Brain_1_Chisle::eBrain1_Duck_5 && mBrainSubState != Brain_1_Chisle::eBrain1_DuckKnockback_8 && mBrainSubState != Brain_1_Chisle::eBrain1_OutOfDuck_6)
        {
            mBrainSubState = Brain_1_Chisle::eBrain1_Duck_5;
        }
    }

    BaseAnimatedWithPhysicsGameObject* pAbuseEvent = IsEventInRange(kEventMudokonAbuse, mXPos, mYPos, AsEventScale(GetScale()));
    BaseAnimatedWithPhysicsGameObject* pDeadMudEvent = IsEventInRange(kEventMudokonDied, mXPos, mYPos, AsEventScale(GetScale()));
    BaseAnimatedWithPhysicsGameObject* pLoudNoiseEvent = IsEventInRange(kEventLoudNoise, mXPos, mYPos, AsEventScale(GetScale()));

    const bool reactToAbused = (pAbuseEvent && pAbuseEvent != this && mBrainSubState != Brain_1_Chisle::eBrain1_StandUp_3 && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0));

    const bool reactToDeadMud = (pDeadMudEvent && pDeadMudEvent != this && mBrainSubState != Brain_1_Chisle::eBrain1_StandUp_3 && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0));

    const bool reactToLoudNoise = (pLoudNoiseEvent && pLoudNoiseEvent->Type() == ReliveTypes::eGlukkon && mBrainSubState != Brain_1_Chisle::eBrain1_StandUp_3 && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0));

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
        if (mNoiseUnknown)
        {
            field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_16);
        }
        else
        {
            mNoiseUnknown = true;
            field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_15);
        }
    }

    if (reactToAbused || reactToDeadMud || reactToLoudNoise)
    {
        if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || mGetDepressed)
        {
            field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
        }

        if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
        {
            AddAlerted();

            if (field_180_emo_tbl == Mud_Emotion::eSad_3)
            {
                mStandingForSadOrAngry = true;
            }

            mBrainSubState = Brain_1_Chisle::eBrain1_StandUp_3;
            field_194_timer = MudResponseDelay() + sGnFrame + 10;
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (LaughingGasInCurrentScreen())
        {
            field_180_emo_tbl = Mud_Emotion::eWired_6;

            AddAlerted();

            mBrainSubState = Brain_1_Chisle::eBrain1_StandUp_3;
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

    if (!mDoAngry)
    {
        if (field_128_angry_timer > 0)
        {
            if (field_128_angry_timer <= static_cast<s32>(sGnFrame))
            {
                mDoAngry = true;
                field_180_emo_tbl = Mud_Emotion::eAngry_1;
                mBrainSubState = Brain_1_Chisle::eBrain1_StandUp_3;
                field_194_timer = sGnFrame;
            }
        }
    }

    switch (mBrainSubState)
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
            if (lastSpeak == GameSpeakEvents::eAbe_Hello_9)
            {
                if (sIsMudStandingUp || !IAmNearestToAbe())
                {
                    ignoreHellOrAllYa = true;
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAbe_AllYa_23)
            {
                ignoreHellOrAllYa = true;
            }

            if (!ignoreHellOrAllYa && sActiveHero->GetSpriteScale() == GetSpriteScale())
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
                field_194_timer = MudResponseDelay() + sGnFrame + 10;
                sIsMudStandingUp = true;
                mIsMudStandingUp2 = true;

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
                return mBrainSubState;
            }

            if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && (Math_NextRandom() < 120))
            {
                AddAlerted();

                mMakeSadNoise = true;
                mStandingForSadOrAngry = true;

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
            if (lastSpeak == GameSpeakEvents::eAbe_Hello_9)
            {
                if (sIsMudStandingUp || !IAmNearestToAbe())
                {
                    ignoreHellOrAllYa = true;
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAbe_AllYa_23)
            {
                ignoreHellOrAllYa = true;
            }

            if (!ignoreHellOrAllYa && sActiveHero->GetSpriteScale() == GetSpriteScale())
            {
                AddAlerted();

                mNextMotion = eMudMotions::Motion_0_Idle;
                field_194_timer = MudResponseDelay() + sGnFrame + 10;
                sIsMudStandingUp = true;
                mIsMudStandingUp2 = true;
                return Brain_1_Chisle::eBrain1_StandUp_3;
            }

            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                mNextMotion = eMudMotions::Motion_17_StandToCrouch;
            }

            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return mBrainSubState;
            }

            field_194_timer = (Math_NextRandom() % 64) + (sGnFrame + 35);
            mNextMotion = eMudMotions::Motion_11_Chisel;
        }
            return Brain_1_Chisle::eBrain1_ChisleTheFloor_1;

        case Brain_1_Chisle::eBrain1_StandUp_3:
        {
            bool skip = false;
            bool bForce = false;
            if (lastSpeak != GameSpeakEvents::eAbe_Hello_9)
            {
                if (lastSpeak == GameSpeakEvents::eAbe_AllYa_23)
                {
                    bForce = true;
                }
                else
                {
                    skip = true;
                }
            }

            if (!skip && (bForce || (!sIsMudStandingUp && IAmNearestToAbe())))
            {
                if (sActiveHero->GetSpriteScale() == GetSpriteScale())
                {
                    AddAlerted();
                    sIsMudStandingUp = true;
                    mIsMudStandingUp2 = true;

                    mMakeSadNoise = false;
                    mStandingForSadOrAngry = false;
                }
            }

            if (static_cast<s32>(sGnFrame) <= field_194_timer || mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                if (mNextMotion != -1)
                {
                    return mBrainSubState;
                }

                if (mCurrentMotion == eMudMotions::Motion_11_Chisel || mCurrentMotion == eMudMotions::Motion_15_CrouchIdle)
                {
                    mNextMotion = eMudMotions::Motion_0_Idle;
                }

                if (mCurrentMotion != eMudMotions::Motion_53_Duck)
                {
                    return mBrainSubState;
                }
                else
                {
                    mNextMotion = eMudMotions::Motion_15_CrouchIdle;
                    return mBrainSubState;
                }
            }

            if (field_180_emo_tbl == Mud_Emotion::eSad_3 && mMakeSadNoise)
            {
                mAlertEnemies = false;
                mMakeSadNoise = false;
                mNextMotion = eMudMotions::M_Speak_6_472FA0;
                MudEmotionSound(MudSounds::eSadUgh_28);
                return Brain_1_Chisle::eBrain1_SadNoise_4;
            }
            else
            {
                sIsMudStandingUp = false;
                mIsMudStandingUp2 = false;
                if (field_180_emo_tbl != Mud_Emotion::eAngry_1 || !mDoAngry)
                {
                    mBrainState = Mud_Brain_State::Brain_4_ListeningToAbe;
                    return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
                }
                else
                {
                    field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
                    mBrainState = Mud_Brain_State::Brain_4_ListeningToAbe;
                    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
                    return Brain_4_ListeningToAbe::eBrain4_Knockback_14;
                }
            }
        }
        break;

        case Brain_1_Chisle::eBrain1_SadNoise_4:
            if (mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            mStandingForSadOrAngry = false;
            mNextMotion = eMudMotions::Motion_17_StandToCrouch;
            return Brain_1_Chisle::eBrain1_StartToChisle_0;

        case Brain_1_Chisle::eBrain1_Duck_5:
            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                mNextMotion = eMudMotions::Motion_17_StandToCrouch;
                return mBrainSubState;
            }

            if (mCurrentMotion == eMudMotions::Motion_11_Chisel)
            {
                mNextMotion = eMudMotions::Motion_15_CrouchIdle;
                return mBrainSubState;
            }

            if (mCurrentMotion != eMudMotions::Motion_15_CrouchIdle)
            {
                return mBrainSubState;
            }

            mNextMotion = eMudMotions::Motion_53_Duck;
            field_194_timer = sGnFrame + 60;
            return Brain_1_Chisle::eBrain1_OutOfDuck_6;

        case Brain_1_Chisle::eBrain1_OutOfDuck_6:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return mBrainSubState;
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
                return mBrainSubState;
            }
            mNextMotion = eMudMotions::Motion_11_Chisel;
            return mBrainSubState;

        case Brain_1_Chisle::eBrain1_DuckKnockback_8:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return mBrainSubState;
            }
            mNextMotion = eMudMotions::Motion_55_DuckKnockback;
            field_194_timer = sGnFrame + 60;
            return Brain_1_Chisle::eBrain1_OutOfDuck_6;

        default:
            return mBrainSubState;
    }
}

s16 Mudokon::Brain_2_CrouchScrub()
{
    if (FindBirdPortal())
    {
        AddAlerted();
        mBrainState = Mud_Brain_State::Brain_6_Escape;
        return Brain_6_Escape::eBrain6_PortalOppened_0;
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak();
    if (mIsMudStandingUp2)
    {
        if (mBrainSubState != Brain_2_CrouchScrub::eBrain2_StandUp_3)
        {
            sIsMudStandingUp = false;
            mIsMudStandingUp2 = false;
        }
    }

    if (lastSpeak == GameSpeakEvents::Slig_LookOut_6 || EventGet(kEventShooting))
    {
        mNextMotion = eMudMotions::Motion_53_Duck;
        field_194_timer = sGnFrame + 60;
        return Brain_2_CrouchScrub::eBrain2_Duck_6;
    }

    BaseAnimatedWithPhysicsGameObject* pAbuse = IsEventInRange(
        kEventMudokonAbuse,
        mXPos,
        mYPos,
        AsEventScale(GetScale()));
    if (pAbuse)
    {
        if (pAbuse != this)
        {
            if (mBrainSubState != Brain_2_CrouchScrub::eBrain2_StandUp_3
                && mBrainSubState != Brain_2_CrouchScrub::eBrain2_DuckKnockback_7
                && gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eMudAbuse_9);
                if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || mGetDepressed)
                {
                    field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
                }

                if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    AddAlerted();

                    if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                    {
                        mStandingForSadOrAngry = true;
                    }

                    mBrainSubState = Brain_2_CrouchScrub::eBrain2_StandUp_3;
                    field_194_timer = MudResponseDelay() + sGnFrame + 10;
                }
            }
        }
    }

    BaseAnimatedWithPhysicsGameObject* pDied = IsEventInRange(
        kEventMudokonDied,
        mXPos,
        mYPos,
        AsEventScale(GetScale()));
    if (pDied)
    {
        if (pDied != this)
        {
            if (mBrainSubState != Brain_2_CrouchScrub::eBrain2_StandUp_3
                && mBrainSubState != Brain_2_CrouchScrub::eBrain2_DuckKnockback_7
                && gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eMudDied_14);
                if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || mGetDepressed)
                {
                    field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
                }

                if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    AddAlerted();

                    if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                    {
                        mStandingForSadOrAngry = true;
                    }

                    mBrainSubState = Brain_2_CrouchScrub::eBrain2_StandUp_3;
                    field_194_timer = MudResponseDelay() + sGnFrame + 10;
                }
            }
        }
    }

    BaseAnimatedWithPhysicsGameObject* pLoudNoise = IsEventInRange(
        kEventLoudNoise,
        mXPos,
        mYPos,
        AsEventScale(GetScale()));
    if (pLoudNoise)
    {
        if (pLoudNoise->Type() == ReliveTypes::eGlukkon)
        {
            if (mBrainSubState != Brain_2_CrouchScrub::eBrain2_StandUp_3
                && mBrainSubState != Brain_2_CrouchScrub::eBrain2_DuckKnockback_7
                && gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                if (mNoiseUnknown)
                {
                    field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_16);
                }
                else
                {
                    field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_15);
                    mNoiseUnknown = true;
                }

                if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || mGetDepressed)
                {
                    field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
                }

                if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    AddAlerted();

                    if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                    {
                        mStandingForSadOrAngry = true;
                    }

                    mBrainSubState = Brain_2_CrouchScrub::eBrain2_StandUp_3;
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
            mBrainSubState = Brain_2_CrouchScrub::eBrain2_StandUp_3;
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

    if (!mDoAngry)
    {
        if (field_128_angry_timer)
        {
            if (field_128_angry_timer <= (s32) sGnFrame)
            {
                mDoAngry = true;
                field_180_emo_tbl = Mud_Emotion::eAngry_1;
                mBrainSubState = Brain_2_CrouchScrub::eBrain2_StandUp_3;
                field_194_timer = sGnFrame;
            }
        }
    }

    switch (mBrainSubState)
    {
        case Brain_2_CrouchScrub::eBrain2_StartToScrub_0:
            mCurrentMotion = eMudMotions::Motion_14_CrouchScrub;
            mNextMotion = -1;
            field_194_timer = Math_NextRandom() % 64 + sGnFrame + 15;
            field_124_laugh_and_crouch_timer = Math_NextRandom() % 64 + sGnFrame + 240;
            return Brain_2_CrouchScrub::eBrain2_Scrubbing_2;

        case Brain_2_CrouchScrub::eBrain2_SmallBreak_1:
        {
            bool checkAlerted = true;
            if (lastSpeak == GameSpeakEvents::eAbe_Hello_9)
            {
                if (sIsMudStandingUp || !IAmNearestToAbe())
                {
                    checkAlerted = false;
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAbe_AllYa_23)
            {
                checkAlerted = false;
            }

            if (checkAlerted)
            {
                if (sActiveHero->GetSpriteScale() == GetSpriteScale())
                {
                    AddAlerted();
                    sIsMudStandingUp = true;
                    mIsMudStandingUp2 = true;
                    field_194_timer = MudResponseDelay() + sGnFrame + 10;
                    return Brain_2_CrouchScrub::eBrain2_StandUp_3;
                }
            }

            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                mNextMotion = eMudMotions::Motion_14_CrouchScrub;
            }

            if (mCurrentMotion == eMudMotions::Motion_15_CrouchIdle && static_cast<s32>(sGnFrame) > field_124_laugh_and_crouch_timer)
            {
                field_124_laugh_and_crouch_timer = Math_NextRandom() % 64 + sGnFrame + 240;
                mNextMotion = eMudMotions::Motion_16_CrouchTurn;
            }

            if (mCurrentMotion != eMudMotions::Motion_15_CrouchIdle || static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                if (mCurrentMotion != eMudMotions::Motion_14_CrouchScrub)
                {
                    return mBrainSubState;
                }
                return Brain_2_CrouchScrub::eBrain2_Scrubbing_2;
            }

            if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && Math_NextRandom() < 120)
            {
                AddAlerted();
                mMakeSadNoise = true;
                mStandingForSadOrAngry = true;
                field_194_timer = MudResponseDelay() + sGnFrame + 10;
                return Brain_2_CrouchScrub::eBrain2_StandUp_3;
            }
            else
            {
                field_194_timer = Math_NextRandom() % 64 + sGnFrame + 35;
                mNextMotion = eMudMotions::Motion_14_CrouchScrub;
                if (mCurrentMotion != eMudMotions::Motion_14_CrouchScrub)
                {
                    return mBrainSubState;
                }
                return Brain_2_CrouchScrub::eBrain2_Scrubbing_2;
            }
            break;
        }

        case Brain_2_CrouchScrub::eBrain2_Scrubbing_2:
        {
            bool checkAlerted = true;
            if (lastSpeak == GameSpeakEvents::eAbe_Hello_9)
            {
                if (sIsMudStandingUp || !IAmNearestToAbe())
                {
                    checkAlerted = false;
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAbe_AllYa_23)
            {
                checkAlerted = false;
            }

            if (checkAlerted)
            {
                if (sActiveHero->GetSpriteScale() == GetSpriteScale())
                {
                    AddAlerted();
                    sIsMudStandingUp = true;
                    mIsMudStandingUp2 = true;
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
                if (!IsEventInRange(kEventSpeaking, mXPos, mYPos, AsEventScale(GetScale())))
                {
                    return mBrainSubState;
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
            if (lastSpeak == GameSpeakEvents::eAbe_Hello_9 || lastSpeak == GameSpeakEvents::eAbe_AllYa_23)
            {
                bool checkAlerted = lastSpeak == GameSpeakEvents::eAbe_Hello_9;
                if (lastSpeak == GameSpeakEvents::eAbe_AllYa_23)
                {
                    if (!sIsMudStandingUp && IAmNearestToAbe())
                    {
                        checkAlerted = true;
                    }
                }

                if (checkAlerted)
                {
                    if (sActiveHero->GetSpriteScale() == GetSpriteScale())
                    {
                        AddAlerted();
                        sIsMudStandingUp = true;
                        mIsMudStandingUp2 = true;
                        mMakeSadNoise = false;
                        mStandingForSadOrAngry = false;
                    }
                }
            }

            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return mBrainSubState;
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
                return mBrainSubState;
            }

            if (field_180_emo_tbl == Mud_Emotion::eSad_3 && mMakeSadNoise)
            {
                mMakeSadNoise = false;
                mAlertEnemies = false;
                mNextMotion = eMudMotions::M_Speak_6_472FA0;
                MudEmotionSound(MudSounds::eSadUgh_28);
                return Brain_2_CrouchScrub::eBrain2_SadNoise_4;
            }
            else
            {
                sIsMudStandingUp = false;
                mIsMudStandingUp2 = false;
                if (field_180_emo_tbl != Mud_Emotion::eAngry_1 || !mDoAngry)
                {
                    mBrainState = Mud_Brain_State::Brain_4_ListeningToAbe;
                    return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
                }
                else
                {
                    field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
                    mBrainState = Mud_Brain_State::Brain_4_ListeningToAbe;
                    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
                    return Brain_4_ListeningToAbe::eBrain4_Knockback_14;
                }
            }
            break;
        }

        case Brain_2_CrouchScrub::eBrain2_SadNoise_4:
            if (mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            mStandingForSadOrAngry = false;
            mNextMotion = eMudMotions::Motion_17_StandToCrouch;
            return Brain_2_CrouchScrub::eBrain2_SmallBreak_1;

        case Brain_2_CrouchScrub::eBrain2_SmallBreakThroughGameSpeak_5:
        {
            bool checkAlerted = true;
            if (lastSpeak == GameSpeakEvents::eAbe_Hello_9)
            {
                if (sIsMudStandingUp || !IAmNearestToAbe())
                {
                    checkAlerted = false;
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAbe_AllYa_23)
            {
                checkAlerted = false;
            }

            if (checkAlerted)
            {
                if (sActiveHero->GetSpriteScale() == GetSpriteScale())
                {
                    AddAlerted();
                    sIsMudStandingUp = true;
                    mIsMudStandingUp2 = true;
                    field_194_timer = MudResponseDelay() + sGnFrame + 10;
                    return Brain_2_CrouchScrub::eBrain2_StandUp_3;
                }
            }

            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return mBrainSubState;
            }
            return Brain_2_CrouchScrub::eBrain2_SmallBreak_1;
        }

        case Brain_2_CrouchScrub::eBrain2_Duck_6:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return mBrainSubState;
            }
            mNextMotion = eMudMotions::Motion_15_CrouchIdle;
            field_194_timer = Math_NextRandom() % 64 + sGnFrame + 15;
            field_124_laugh_and_crouch_timer = Math_NextRandom() % 64 + sGnFrame + 240;
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(1);
            }
            return Brain_2_CrouchScrub::eBrain2_Scrubbing_2;

        case Brain_2_CrouchScrub::eBrain2_DuckKnockback_7:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return mBrainSubState;
            }
            mNextMotion = eMudMotions::Motion_55_DuckKnockback;
            field_194_timer = sGnFrame + 60;
            return Brain_2_CrouchScrub::eBrain2_Duck_6;

        default:
            return mBrainSubState;
    }
}

s16 Mudokon::Brain_3_TurnWheel()
{
    if (FindBirdPortal())
    {
        AddAlerted();
        mBrainState = Mud_Brain_State::Brain_6_Escape;
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_6_Escape::eBrain6_PortalOppened_0;
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak();
    if (mIsMudStandingUp2)
    {
        if (mBrainSubState != Brain_3_TurnWheel::eBrain3_InterruptAction_2)
        {
            sIsMudStandingUp = false;
            mIsMudStandingUp2 = false;
        }
    }

    if (lastSpeak == GameSpeakEvents::Slig_LookOut_6 || EventGet(kEventShooting))
    {
        if (mBrainSubState != Brain_3_TurnWheel::eBrain3_TurningWheelToDuck_3 && mBrainSubState != Brain_3_TurnWheel::eBrain3_Duck_4)
        {
            mBrainSubState = Brain_3_TurnWheel::eBrain3_TurningWheelToDuck_3;
        }
    }

    BaseAnimatedWithPhysicsGameObject* pMudAbuseEvent = IsEventInRange(
        kEventMudokonAbuse,
        mXPos,
        mYPos,
        AsEventScale(GetScale()));

    if (pMudAbuseEvent)
    {
        if (pMudAbuseEvent != this
            && mBrainSubState != Brain_3_TurnWheel::eBrain3_InterruptAction_2
            && gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            field_188_pTblEntry = Mudokon::ResponseTo_471730(field_180_emo_tbl, MudAction::eMudAbuse_9);
            if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || mGetDepressed)
            {
                field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
            }

            if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
            {
                AddAlerted();

                if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    mStandingForSadOrAngry = true;
                }

                mNextMotion = eMudMotions::Motion_0_Idle;
                mBrainSubState = Brain_3_TurnWheel::eBrain3_InterruptAction_2;
                field_194_timer = MudResponseDelay() + sGnFrame + 10;
            }
        }
    }

    BaseAnimatedWithPhysicsGameObject* pLoudNoiseEvent = IsEventInRange(
        kEventLoudNoise,
        mXPos,
        mYPos,
        AsEventScale(GetScale()));
    if (pLoudNoiseEvent)
    {
        if (pLoudNoiseEvent->Type() == ReliveTypes::eGlukkon
            && mBrainSubState != Brain_3_TurnWheel::eBrain3_InterruptAction_2
            && gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            if (mNoiseUnknown)
            {
                field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_16);
            }
            else
            {
                mNoiseUnknown = true;
                field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_15);
            }

            if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || mGetDepressed)
            {
                field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
            }

            if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
            {
                AddAlerted();

                if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    mStandingForSadOrAngry = true;
                }

                mNextMotion = eMudMotions::Motion_0_Idle;
                mBrainSubState = Brain_3_TurnWheel::eBrain3_InterruptAction_2;
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
            mBrainSubState = Brain_3_TurnWheel::eBrain3_InterruptAction_2;
        }
    }

    switch (mBrainSubState)
    {
        case Brain_3_TurnWheel::eBrain3_TurningWheel_0:
            if (lastSpeak == GameSpeakEvents::eAbe_Hello_9)
            {
                if (sIsMudStandingUp || !IAmNearestToAbe())
                {
                    return TurningWheelHelloOrAllYaResponse();
                }
            }
            else if (lastSpeak != GameSpeakEvents::eAbe_AllYa_23)
            {
                return TurningWheelHelloOrAllYaResponse();
            }

            if (sActiveHero->GetSpriteScale() == GetSpriteScale())
            {
                AddAlerted();
                mNextMotion = eMudMotions::Motion_0_Idle;
                field_194_timer = MudResponseDelay() + sGnFrame + 10;
                sIsMudStandingUp = true;
                mIsMudStandingUp2 = true;
                return Brain_3_TurnWheel::eBrain3_InterruptAction_2;
            }

            return TurningWheelHelloOrAllYaResponse();

        case Brain_3_TurnWheel::eBrain3_SittingDown_1:
            if (field_194_timer <= static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
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
                return mBrainSubState;
            }

            if (mChiselMud)
            {
                mBrainState = Mud_Brain_State::Brain_1_Chisel;
            }
            else
            {
                mBrainState = Mud_Brain_State::Brain_2_CrouchScrub;
            }
            return Brain_3_TurnWheel::eBrain3_TurningWheel_0;

        case Brain_3_TurnWheel::eBrain3_InterruptAction_2:
            mStoppedAtWheel = false;
            if (static_cast<s32>(sGnFrame) <= field_194_timer || mCurrentMotion)
            {
                if (mNextMotion != -1 || mCurrentMotion != eMudMotions::Motion_58_TurnWheelLoop)
                {
                    return mBrainSubState;
                }
                mNextMotion = eMudMotions::Motion_0_Idle;
                return mBrainSubState;
            }
            else
            {
                mBrainState = Mud_Brain_State::Brain_4_ListeningToAbe;
                sIsMudStandingUp = false;
                mIsMudStandingUp2 = false;
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
                        return mBrainSubState;
                    }
                    else
                    {
                        mNextMotion = eMudMotions::Motion_0_Idle;
                        return mBrainSubState;
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
                return mBrainSubState;
            }
            break;

        case Brain_3_TurnWheel::eBrain3_Duck_4:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return mBrainSubState;
            }
            mNextMotion = eMudMotions::Motion_15_CrouchIdle;
            return Brain_3_TurnWheel::eBrain3_DuckToTurningWheel_5;

        case Brain_3_TurnWheel::eBrain3_DuckToTurningWheel_5:
            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                mNextMotion = eMudMotions::Motion_57_TurnWheelBegin;
                return mBrainSubState;
            }

            if (mCurrentMotion == eMudMotions::Motion_15_CrouchIdle)
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
                return mBrainSubState;
            }
            else
            {
                if (mCurrentMotion != eMudMotions::Motion_58_TurnWheelLoop)
                {
                    return mBrainSubState;
                }
                return Brain_3_TurnWheel::eBrain3_TurningWheel_0;
            }
            break;
    }
    return mBrainSubState;
}

s16 Mudokon::Brain_4_ListeningToAbe()
{
    if (FindBirdPortal())
    {
        mBrainState = Mud_Brain_State::Brain_6_Escape;
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_6_Escape::eBrain6_PortalOppened_0;
    }

    if (IsEventInRange(
            kEventMudokonAbuse,
            mXPos,
            mYPos,
            AsEventScale(GetScale())))
    {
        field_17E_delayed_speak = MudAction::eMudAbuse_9;
    }

    if (IsEventInRange(
            kEventMudokonDied,
            mXPos,
            mYPos,
            AsEventScale(GetScale())))
    {
        field_17E_delayed_speak = MudAction::eMudDied_14;
    }

    if (IsEventInRange(
            kEventMudokonComfort,
            mXPos,
            mYPos,
            AsEventScale(GetScale())))
    {
        field_17E_delayed_speak = MudAction::eComfort_10;
    }

    if (IsEventInRange(
            kEventMudokonLaugh,
            mXPos,
            mYPos,
            AsEventScale(GetScale())))
    {
        field_17E_delayed_speak = MudAction::eLaugh_12;
    }

    if (IsEventInRange(kEventAbeDead,
            mXPos,
            mYPos,
                                AsEventScale(GetScale())))
    {
        field_17E_delayed_speak = MudAction::eDuck_13;
    }

    BaseAnimatedWithPhysicsGameObject* pNoiseEvent = IsEventInRange(
        kEventLoudNoise,
        mXPos,
        mYPos,
        AsEventScale(GetScale()));

    if (pNoiseEvent)
    {
        if (pNoiseEvent->Type() == ReliveTypes::eGlukkon)
        {
            if (mNoiseUnknown)
            {
                field_17E_delayed_speak = MudAction::eUnknown_16;
            }
            else
            {
                mNoiseUnknown = true;
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

    if (!mDoAngry)
    {
        if (field_128_angry_timer > 0)
        {
            if (field_128_angry_timer <= static_cast<s32>(sGnFrame))
            {
                mDoAngry = true;

                if (gPathInfo->TLV_Get_At(
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos - FP_FromInteger(5)),
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos - FP_FromInteger(5)),
                        ReliveTypes::eWater))
                {
                    field_180_emo_tbl = Mud_Emotion::eAngry_1;
                    field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
                    mBrainSubState = Brain_4_ListeningToAbe::eBrain4_Knockback_14;
                    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
                }
            }
        }
    }

    switch (mBrainSubState)
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
            return mBrainSubState;
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
        return mBrainSubState;
    }

    if (!VIsFacingMe(sActiveHero))
    {
        mNextMotion = eMudMotions::Motion_2_StandingTurn;
        return mBrainSubState;
    }

    switch (field_180_emo_tbl)
    {
        case Mud_Emotion::eNormal_0:
            MudEmotionSound(MudSounds::eHelloNeutral_3);
            break;

        case Mud_Emotion::eAngry_1:
        case Mud_Emotion::eAggressive_2:
            if (mStandingForSadOrAngry)
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
            if (mStandingForSadOrAngry)
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
    if (!mStandingForSadOrAngry || field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        return Brain_4_ListeningToAbe::eBrain4_Idle_7;
    }

    mStandingForSadOrAngry = false;
    mAlertEnemies = false;

    if (mChiselMud)
    {
        field_184_next_motion2 = eMudMotions::Motion_11_Chisel;
    }
    else
    {
        field_184_next_motion2 = eMudMotions::Motion_15_CrouchIdle;
    }
    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_LostAttention_22;
    mFollowingAbe = false;
    return Brain_4_ListeningToAbe::eBrain4_InitializeMovement_10;
}

s16 Mudokon::Brain_ListeningToAbe_State_2()
{
    if (!VIsFacingMe(sActiveHero))
    {
        mNextMotion = eMudMotions::Motion_2_StandingTurn;
        return mBrainSubState;
    }

    if (mCurrentMotion != eMudMotions::Motion_0_Idle)
    {
        if (mCurrentMotion == eMudMotions::Motion_1_WalkLoop)
        {
            if (mBlind || !VIsObjNearby(ScaleToGridSize(GetSpriteScale()), sActiveHero))
            {
                return Brain_4_ListeningToAbe::eBrain4_Walking_5;
            }
        }
        else
        {
            if (mCurrentMotion != eMudMotions::Motion_46_Knockback)
            {
                return mBrainSubState;
            }
        }
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }
    else
    {
        mNextMotion = eMudMotions::Motion_1_WalkLoop;
    }
    return mBrainSubState;
}

s16 Mudokon::Brain_ListeningToAbe_State_3()
{
    if (mCurrentMotion != eMudMotions::Motion_0_Idle)
    {
        if (mCurrentMotion != eMudMotions::Motion_1_WalkLoop)
        {
            return mBrainSubState;
        }
        mNextMotion = eMudMotions::Motion_0_Idle;

        if (mFollowingAbe)
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
            return mBrainSubState;
        }
        mNextMotion = eMudMotions::Motion_1_WalkLoop;
        return mBrainSubState;
    }
}

s16 Mudokon::Brain_ListeningToAbe_State_4()
{
    if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && field_180_emo_tbl != Mud_Emotion::eHappy_5 && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        mFollowingAbe = false;
        return Brain_4_ListeningToAbe::eBrain4_Idle_7;
    }
    else
    {
        if (mCurrentMotion == eMudMotions::Motion_21_RunLoop)
        {
            mNextMotion = eMudMotions::Motion_0_Idle;
        }
        if (mYPos > gScreenManager->CamYPos() + FP_FromInteger(260) && field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
            mFollowingAbe = false;
            if (mChiselMud)
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

        FP scaleToGridSizeAbs = {};
        if (sActiveHero->mXPos >= mXPos)
        {
            scaleToGridSizeAbs = ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            scaleToGridSizeAbs = -ScaleToGridSize(GetSpriteScale());
        }
        const s32 v44 = WallHit(GetSpriteScale() * FP_FromInteger(50), scaleToGridSizeAbs);

        if (mCurrentMotion != eMudMotions::Motion_0_Idle)
        {
            field_17C_stand_idle_timer = 0;
            field_124_laugh_and_crouch_timer = sGnFrame + Math_RandomRange(30, 60);
            return mBrainSubState;
        }
        else
        {
            mStoppedAtWheel = false;
            if (++field_17C_stand_idle_timer <= 540 || field_180_emo_tbl == Mud_Emotion::eWired_6)
            {
                if (NeedToTurnAround())
                {
                    mNextMotion = eMudMotions::Motion_2_StandingTurn;
                    return mBrainSubState;
                }

                const FP v48 = ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2);
                if ((VIsObjNearby(v48, sActiveHero) && !mBlind) || v44 || FindWheel(mXPos, mYPos))
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
                        field_178_brain_sub_state2 = mBrainSubState;
                        field_17E_delayed_speak = MudAction::eNone_17;
                        if (!v18)
                        {
                            return mBrainSubState;
                        }
                        return v18;
                    }

                    if (lastSpeak_1 == GameSpeakEvents::eNone_m1)
                    {
                        if (field_180_emo_tbl != Mud_Emotion::eWired_6 || field_124_laugh_and_crouch_timer > (s32) sGnFrame)
                        {
                            return mBrainSubState;
                        }
                        field_194_timer = sGnFrame + Math_RandomRange(22, 30);
                        field_178_brain_sub_state2 = mBrainSubState;
                        return Brain_4_ListeningToAbe::eBrain4_CrazyLaugh_21;
                    }
                    else
                    {
                        field_178_brain_sub_state2 = mBrainSubState;
                        field_194_timer = (u16) MudResponseDelay() + sGnFrame + 20;
                        switch (lastSpeak_1)
                        {
                            case GameSpeakEvents::eAbe_Fart_3:
                            {
                                s16 v18 = GetBrainSubStateResponse(MudAction::eFart_6);
                                if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()), sActiveHero))
                                {
                                    if (VOnSameYLevel(sActiveHero))
                                    {
                                        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_SteppingBack_3;
                                    }
                                }
                                if (!v18)
                                {
                                    return mBrainSubState;
                                }
                                return v18;
                            }

                            case GameSpeakEvents::eAbe_Hello_9:
                            case GameSpeakEvents::eAbe_AllYa_23:
                            {
                                s16 result = GetBrainSubStateResponse(MudAction::eHelloOrAllYa_0);
                                if (result)
                                {
                                    return result;
                                }
                                return mBrainSubState;
                            }

                            case GameSpeakEvents::eAbe_FollowMe_10:
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
                                return mBrainSubState;
                            }

                            case GameSpeakEvents::eAbe_Anger_11:
                            case GameSpeakEvents::eAbe_Sorry_24:
                                field_17C_stand_idle_timer = 0;
                                return mBrainSubState;

                            case GameSpeakEvents::eAbe_Wait_12:
                            {
                                if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                                {
                                    return Brain_4_ListeningToAbe::eBrain4_CrazyDeny_19;
                                }
                                mFollowingAbe = false;
                                s16 result = GetBrainSubStateResponse(MudAction::eWait_2);
                                field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
                                if (result)
                                {
                                    return result;
                                }
                                return mBrainSubState;
                            }

                            case GameSpeakEvents::eAbe_Work_21:
                                if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                                {
                                    return Brain_4_ListeningToAbe::eBrain4_CrazyDeny_19;
                                }
                                mFollowingAbe = false;
                                field_182_speak_event = GameSpeakEvents::eNone_m1;
                                if (FindWheel(mXPos, mYPos))
                                {
                                    Mudokon* pMud = static_cast<Mudokon*>(FindObjectOfType(ReliveTypes::eMudokon, mXPos, mYPos - FP_FromInteger(5)));
                                    if (!pMud)
                                    {
                                        return BrainStartWheelTurning();
                                    }

                                    while (!pMud->mStoppedAtWheel && pMud->mCurrentMotion != eMudMotions::Motion_58_TurnWheelLoop)
                                    {
                                        pMud = static_cast<Mudokon*>(GetStackedSlapTarget(pMud->mBaseGameObjectId, ReliveTypes::eMudokon, mXPos, mYPos - FP_FromInteger(5)));
                                        if (!pMud)
                                        {
                                            return BrainStartWheelTurning();
                                        }
                                    }
                                    if (pMud)
                                    {
                                        if (mChiselMud)
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

                                if (FindObjectOfType(ReliveTypes::eLever, ScaleToGridSize(GetSpriteScale()) + mXPos, mYPos - FP_FromInteger(5)))
                                {
                                    return Brain_4_ListeningToAbe::eBrain4_PullingLever_11;
                                }
                                if (FindObjectOfType(ReliveTypes::eLever, mXPos - ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5)))
                                {
                                    return Brain_4_ListeningToAbe::eBrain4_PullingLever_11;
                                }

                                if (mChiselMud)
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

                            case GameSpeakEvents::eAbe_StopIt_22:
                            {
                                s16 v18 = GetBrainSubStateResponse(MudAction::eStopIt_4);
                                field_194_timer = ((u32)(u16) MudResponseDelay() >> 1) + sGnFrame + 20;
                                if (!v18)
                                {
                                    return mBrainSubState;
                                }
                                return v18;
                            }

                            default:
                                return mBrainSubState;
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
                    else if (mBlind || !CanAbeSneak())
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
                mFollowingAbe = false;
                if (!mChiselMud)
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
        if (!mBlind)
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
        if (mBlind)
        {
            field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
            mFollowingAbe = false;
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
        return mBrainSubState;
    }

    FP v65 = {};
    if (GetAnimation().GetFlipX())
    {
        v65 = -ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        v65 = ScaleToGridSize(GetSpriteScale());
    }

    const s32 v67 = WallHit(GetSpriteScale() * FP_FromInteger(50), v65);
    if (mBlind)
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
                mFollowingAbe = false;
                mNextMotion = eMudMotions::Motion_0_Idle;
                return Brain_4_ListeningToAbe::eBrain4_Idle_7;
            }
        }
    }
    if (v67 && !mBlind)
    {
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }

    if (StopAtWheel())
    {
        mStoppedAtWheel = true;
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }

    if (!VIsFacingMe(sActiveHero) && !mBlind)
    {
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }

    if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2), sActiveHero) && !mBlind)
    {
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }

    if (LastGameSpeak() == GameSpeakEvents::eAbe_Wait_12)
    {
        mFollowingAbe = false;
        mNextMotion = eMudMotions::Motion_0_Idle;
        return Brain_4_ListeningToAbe::eBrain4_GetsWaitWhileMoving_9;
    }

    bool bWalkOrSneak = false;
    if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_33_RunLoop_4508E0 && sActiveHero->mCurrentMotion != eAbeMotions::Motion_23_RollLoop_453A90 && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        bWalkOrSneak = true;
    }
    else if (mCurrentMotion == eMudMotions::Motion_27_SneakLoop || mBlind)
    {
        mNextMotion = eMudMotions::Motion_1_WalkLoop;
        bWalkOrSneak = true;
    }

    if (bWalkOrSneak)
    {
        if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_40_SneakLoop_450550
            && mCurrentMotion == eMudMotions::Motion_1_WalkLoop
            && !mBlind)
        {
            mNextMotion = eMudMotions::Motion_27_SneakLoop;
        }
        if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_1_WalkLoop_44FBA0 || mCurrentMotion != eMudMotions::Motion_27_SneakLoop)
        {
            return mBrainSubState;
        }
        mNextMotion = eMudMotions::Motion_1_WalkLoop;
        return mBrainSubState;
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
        if (!mBlind)
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
        if (mBlind)
        {
            field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
            mFollowingAbe = false;
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
        return mBrainSubState;
    }

    FP scaleToGridSizeAbs;
    if (GetAnimation().GetFlipX())
    {
        scaleToGridSizeAbs = -ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        scaleToGridSizeAbs = ScaleToGridSize(GetSpriteScale());
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), scaleToGridSizeAbs * FP_FromInteger(3)) && !mBlind)
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
            if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4), sActiveHero))
            {
                mNextMotion = eMudMotions::Motion_1_WalkLoop;
                return Brain_4_ListeningToAbe::eBrain4_Walking_5;
            }
        }

        if (LastGameSpeak() == GameSpeakEvents::eAbe_Wait_12)
        {
            mFollowingAbe = false;
            mNextMotion = eMudMotions::Motion_0_Idle;
            return Brain_4_ListeningToAbe::eBrain4_GetsWaitWhileMoving_9;
        }
    }

    if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()), sActiveHero))
    {
        if (VIsFacingMe(sActiveHero))
        {
            return mBrainSubState;
        }
    }
    else
    {
        if (!VIsObj_GettingNear_On_X(sActiveHero))
        {
            if (VIsFacingMe(sActiveHero))
            {
                return mBrainSubState;
            }
        }
    }

    if (!mBlind)
    {
        mNextMotion = eMudMotions::Motion_25_RunSlideTurn;
        return Brain_4_ListeningToAbe::eBrain4_CrazySlideTurn_8;
    }

    return mBrainSubState;
}

s16 Mudokon::Brain_ListeningToAbe_State_7()
{
    if (mYPos > gScreenManager->CamYPos() + FP_FromInteger(260) && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        mFollowingAbe = false;
        if (mChiselMud)
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

    if (mCurrentMotion != eMudMotions::Motion_0_Idle)
    {
        field_17C_stand_idle_timer = 0;
        field_124_laugh_and_crouch_timer = sGnFrame + Math_RandomRange(30, 60);
        return mBrainSubState;
    }

    if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4)
    {
        mFollowingAbe = false;
        return Brain_4_ListeningToAbe::eBrain4_Suiciding_16;
    }

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        mFollowingAbe = true;
        return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
    }

    if ((++field_17C_stand_idle_timer > 540 || Is_In_Current_Camera() != CameraPos::eCamCurrent_0) && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        mFollowingAbe = false;
        if (mChiselMud)
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

    if (NeedToTurnAround())
    {
        mNextMotion = eMudMotions::Motion_2_StandingTurn;
        return mBrainSubState;
    }

    if (field_17E_delayed_speak != MudAction::eNone_17)
    {
        s16 v18 = GetBrainSubStateResponse(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
        {
            field_194_timer = sGnFrame + 20;
            field_178_brain_sub_state2 = mBrainSubState;
        }
        else
        {
            field_178_brain_sub_state2 = mBrainSubState;
            field_194_timer = MudResponseDelay() + sGnFrame + 20;
        }
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return mBrainSubState;
        }
        return v18;
    }
    GameSpeakEvents lastSpeak = LastGameSpeak();
    if (lastSpeak == GameSpeakEvents::eNone_m1
        && ((lastSpeak = field_182_speak_event, lastSpeak == GameSpeakEvents::eNone_m1)
            || (field_180_emo_tbl != Mud_Emotion::eNormal_0 && field_180_emo_tbl != Mud_Emotion::eHappy_5 && field_180_emo_tbl != Mud_Emotion::eWired_6)))
    {
        if (field_180_emo_tbl != Mud_Emotion::eWired_6 || field_124_laugh_and_crouch_timer > (s32) sGnFrame)
        {
            return mBrainSubState;
        }
        field_194_timer = sGnFrame + Math_RandomRange(22, 30);
        field_178_brain_sub_state2 = mBrainSubState;
        return Brain_4_ListeningToAbe::eBrain4_CrazyLaugh_21;
    }
    else
    {
        field_178_brain_sub_state2 = mBrainSubState;
        field_194_timer = (u16) MudResponseDelay() + sGnFrame + 20;
        switch (lastSpeak)
        {
            case GameSpeakEvents::eAbe_Fart_3:
            {
                field_182_speak_event = GameSpeakEvents::eNone_m1;
                const s16 v18 = GetBrainSubStateResponse(MudAction::eFart_6);
                if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()), sActiveHero))
                {
                    if (VOnSameYLevel(sActiveHero))
                    {
                        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_SteppingBack_3;
                    }
                }
                if (!v18)
                {
                    return mBrainSubState;
                }
                return v18;
            }

            case GameSpeakEvents::eAbe_Hello_9:
            case GameSpeakEvents::eAbe_AllYa_23:
            {
                field_182_speak_event = GameSpeakEvents::eNone_m1;
                s16 result = GetBrainSubStateResponse(MudAction::eHelloOrAllYa_0);
                if (result)
                {
                    return result;
                }
                return mBrainSubState;
            }

            case GameSpeakEvents::eAbe_FollowMe_10:
            {
                const s16 v26 = GetBrainSubStateResponse(MudAction::eFollowMe_1);
                if (field_188_pTblEntry->field_0_sound == MudSounds::eOkay_12 || field_188_pTblEntry->field_0_sound == MudSounds::eLaugh_10)
                {
                    mFollowingAbe = true;
                    field_182_speak_event = GameSpeakEvents::eNone_m1;
                    // TODO: Wrong ??
                    if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2), sActiveHero) || VIsObjNearby(ScaleToGridSize(GetSpriteScale()), sActiveHero))
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
                    field_182_speak_event = lastSpeak;
                }

                if (!v26)
                {
                    return mBrainSubState;
                }

                return v26;
            }

            case GameSpeakEvents::eAbe_Anger_11:
            case GameSpeakEvents::eAbe_Sorry_24:
                field_17C_stand_idle_timer = 0;
                return mBrainSubState;

            case GameSpeakEvents::eAbe_Wait_12:
                mFollowingAbe = false;
                field_182_speak_event = GameSpeakEvents::eNone_m1;
                field_184_next_motion2 = eMudMotions::Motion_43_DunnoBegin;
                return Brain_4_ListeningToAbe::eBrain4_InitializeMovement_10;

            case GameSpeakEvents::eAbe_Work_21:
                if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                {
                    return Brain_4_ListeningToAbe::eBrain4_CrazyDeny_19;
                }
                mFollowingAbe = false;
                field_182_speak_event = GameSpeakEvents::eNone_m1;

                if (FindWheel(mXPos, mYPos))
                {
                    Mudokon* pMud = static_cast<Mudokon*>(FindObjectOfType(ReliveTypes::eMudokon, mXPos, mYPos - FP_FromInteger(5)));
                    if (!pMud)
                    {
                        return BrainStartWheelTurning();
                    }

                    while (!pMud->mStoppedAtWheel && pMud->mCurrentMotion != eMudMotions::Motion_58_TurnWheelLoop)
                    {
                        pMud = static_cast<Mudokon*>(GetStackedSlapTarget(pMud->mBaseGameObjectId, ReliveTypes::eMudokon, mXPos, mYPos - FP_FromInteger(5)));
                        if (!pMud)
                        {
                            return BrainStartWheelTurning();
                        }
                    }
                    if (pMud)
                    {
                        if (mChiselMud)
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

                if (FindObjectOfType(ReliveTypes::eLever, ScaleToGridSize(GetSpriteScale()) + mXPos, mYPos - FP_FromInteger(5)))
                {
                    return Brain_4_ListeningToAbe::eBrain4_PullingLever_11;
                }
                if (FindObjectOfType(ReliveTypes::eLever, mXPos - ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5)))
                {
                    return Brain_4_ListeningToAbe::eBrain4_PullingLever_11;
                }

                break;

            case GameSpeakEvents::eAbe_StopIt_22:
            {
                s16 v18 = GetBrainSubStateResponse(MudAction::eStopIt_4);
                field_194_timer = ((u32)(u16) MudResponseDelay() >> 1) + sGnFrame + 20;
                if (!v18)
                {
                    return mBrainSubState;
                }
                return v18;
            }

            default:
                return mBrainSubState;
        }

        if (mChiselMud)
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
        return mBrainSubState;
    }
    return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
}

s16 Mudokon::Brain_ListeningToAbe_State_9()
{
    if (mCurrentMotion)
    {
        return mBrainSubState;
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
        return mBrainSubState;
    }
    mNextMotion = -1;

    if (static_cast<s32>(sGnFrame) <= field_194_timer)
    {
        return mBrainSubState;
    }
    mNextMotion = field_184_next_motion2;
    return field_178_brain_sub_state2;
}

s16 Mudokon::Brain_ListeningToAbe_State_11()
{
    BaseGameObject* pLever = nullptr;
    if (GetAnimation().GetFlipX())
    {
        pLever = FindObjectOfType(ReliveTypes::eLever, mXPos - ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5));
    }
    else
    {
        pLever = FindObjectOfType(ReliveTypes::eLever, mXPos + ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5));
    }

    if (!pLever)
    {
        mNextMotion = eMudMotions::Motion_2_StandingTurn;
        return mBrainSubState;
    }

    if (mCurrentMotion == eMudMotions::Motion_10_LeverUse)
    {
        mNextMotion = -1;
        auto pSwitch = static_cast<Lever*>(pLever);
        pSwitch->VPull(mXPos < pSwitch->mXPos);
        mFollowingAbe = false;

        if (mChiselMud)
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
        return mBrainSubState;
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
                return mBrainSubState;
            }
            return response;
        }
        field_194_timer = (u16)(MudResponseDelay() + sGnFrame + 20);
        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!response)
        {
            return mBrainSubState;
        }
        return response;
    }

    if (field_188_pTblEntry->field_2_next_motion == eMudMotions::Motion_38_Punch)
    {
        if (LastGameSpeak() == GameSpeakEvents::eAbe_StopIt_22)
        {
            if (mCurrentMotion == eMudMotions::Motion_38_Punch || mNextMotion == eMudMotions::Motion_38_Punch)
            {
                ToStand();
                mbMotionChanged = true;
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
        return mBrainSubState;
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
    if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || mGetDepressed)
    {
        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
    }

    if (field_180_emo_tbl == Mud_Emotion::eSick_7)
    {
        mBrainState = Mud_Brain_State::Brain_9_Sick;
        return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
    }

    if (mNextMotion != eMudMotions::Motion_38_Punch)
    {
        return field_178_brain_sub_state2;
    }

    BaseGameObject* pMudInSameGridBlock = nullptr;
    if (GetAnimation().GetFlipX())
    {
        if (FindObjectOfType(ReliveTypes::eAbe, mXPos + ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5)))
        {
            return Brain_4_ListeningToAbe::eBrain4_RageTurn_18;
        }
        pMudInSameGridBlock = FindObjectOfType(ReliveTypes::eMudokon, mXPos + ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5));
    }
    else
    {
        if (FindObjectOfType(ReliveTypes::eAbe, mXPos - ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5)))
        {
            return Brain_4_ListeningToAbe::eBrain4_RageTurn_18;
        }
        pMudInSameGridBlock = FindObjectOfType(ReliveTypes::eMudokon, mXPos - ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5));
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

    GetAnimation().SetIsLastFrame(false);
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
                return mBrainSubState;
            }
            return v18;
        }
        field_194_timer = (u16)(MudResponseDelay() + sGnFrame + 20);
        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return mBrainSubState;
        }
        return v18;
    }
    if (LastGameSpeak() == GameSpeakEvents::eAbe_StopIt_22)
    {
        if (mCurrentMotion == eMudMotions::Motion_38_Punch || mNextMotion == eMudMotions::Motion_38_Punch)
        {
            ToStand();
            mbMotionChanged = true;
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
        return mBrainSubState;
    }

    if (GetAnimation().GetFlipX())
    {
        if (!FindObjectOfType(ReliveTypes::eAbe, mXPos - ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5)))
        {
            if (FindObjectOfType(ReliveTypes::eAbe, mXPos + ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5))
                || (!FindObjectOfType(ReliveTypes::eMudokon, mXPos - ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5))
                    && FindObjectOfType(ReliveTypes::eMudokon, mXPos + ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5))))
            {
                mNextMotion = eMudMotions::Motion_2_StandingTurn;
                field_194_timer = sGnFrame + 15;
                return Brain_4_ListeningToAbe::eBrain4_GetsCommand_12;
            }
        }
    }
    else
    {
        if (!FindObjectOfType(ReliveTypes::eAbe, mXPos + ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5)))
        {
            if (FindObjectOfType(ReliveTypes::eAbe, mXPos - ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5)))
            {
                mNextMotion = eMudMotions::Motion_2_StandingTurn;
                field_194_timer = sGnFrame + 15;
                return Brain_4_ListeningToAbe::eBrain4_GetsCommand_12;
            }

            if (!FindObjectOfType(ReliveTypes::eMudokon, ScaleToGridSize(GetSpriteScale()) + mXPos, mYPos - FP_FromInteger(5)))
            {
                if (FindObjectOfType(ReliveTypes::eMudokon, mXPos - ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5)))
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
        return mBrainSubState;
    }

    if (field_188_pTblEntry->field_0_sound != MudSounds::eNone)
    {
        MudEmotionSound(field_188_pTblEntry->field_0_sound);
    }

    mNextMotion = field_188_pTblEntry->field_2_next_motion;
    if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || mGetDepressed)
    {
        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
    }
    if (field_182_speak_event != GameSpeakEvents::eAbe_FollowMe_10)
    {
        return field_178_brain_sub_state2;
    }

    mFollowingAbe = true;

    field_182_speak_event = GameSpeakEvents::eNone_m1;
    return Brain_4_ListeningToAbe::eBrain4_FollowingIdle_4;
}

s16 Mudokon::Brain_ListeningToAbe_State_16()
{
    if (mCurrentMotion != eMudMotions::Motion_56_SlapOwnHead)
    {
        mNextMotion = eMudMotions::Motion_56_SlapOwnHead;
    }

    if (LastGameSpeak() != GameSpeakEvents::eAbe_StopIt_22 && field_17E_delayed_speak != MudAction::eSorry_8)
    {
        return mBrainSubState;
    }

    s16 result = GetBrainSubStateResponse(MudAction::eStopIt_4);
    field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
    field_17E_delayed_speak = MudAction::eNone_17;
    field_194_timer = sGnFrame + 20;
    if (!result)
    {
        return mBrainSubState;
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
                return mBrainSubState;
            }
            return v18;
        }
        field_194_timer = (u16)(MudResponseDelay() + sGnFrame + 20);
        field_178_brain_sub_state2 = Brain_4_ListeningToAbe::eBrain4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return mBrainSubState;
        }
        return v18;
    }

    if (LastGameSpeak() != GameSpeakEvents::eAbe_StopIt_22)
    {
        if (mCurrentMotion)
        {
            return mBrainSubState;
        }
        mNextMotion = eMudMotions::Motion_38_Punch;
        return Brain_4_ListeningToAbe::eBrain4_Idle_7;
    }

    if (mCurrentMotion == eMudMotions::Motion_38_Punch || mNextMotion == eMudMotions::Motion_38_Punch)
    {
        ToStand();
        mbMotionChanged = true;
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
        return mBrainSubState;
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
            return mBrainSubState;
        }
        return v18;
    }
    else
    {
        if (LastGameSpeak() != GameSpeakEvents::eAbe_StopIt_22)
        {
            if (mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            mNextMotion = eMudMotions::Motion_2_StandingTurn;
            return Brain_4_ListeningToAbe::eBrain4_RageSlap_17;
        }

        if (mCurrentMotion == eMudMotions::Motion_38_Punch || mNextMotion == eMudMotions::Motion_38_Punch)
        {
            ToStand();
            mbMotionChanged = true;
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
                return mBrainSubState;
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
        return mBrainSubState;
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
        return mBrainSubState;
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
        return mBrainSubState;
    }

    mAlertEnemies = false;

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
    if (mChiselMud)
    {
        mNextMotion = eMudMotions::Motion_11_Chisel;
    }
    else
    {
        mNextMotion = eMudMotions::Motion_15_CrouchIdle;
    }

    if (mCurrentMotion != eMudMotions::Motion_15_CrouchIdle && mCurrentMotion != eMudMotions::Motion_11_Chisel)
    {
        return mBrainSubState;
    }

    RemoveAlerted();

    if (mChiselMud)
    {
        mBrainState = Mud_Brain_State::Brain_1_Chisel;
    }
    else
    {
        mBrainState = Mud_Brain_State::Brain_2_CrouchScrub;
    }

    field_182_speak_event = GameSpeakEvents::eNone_m1;
    return Brain_4_ListeningToAbe::eBrain4_Inactive_0;
}

s16 Mudokon::Brain_5_ShrivelDeath()
{
    if (field_194_timer < static_cast<s32>(sGnFrame + 80))
    {
        SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.008));
        mRGB.r -= 2;
        mRGB.g -= 2;
        mRGB.b -= 2;
    }

    if (static_cast<s32>(sGnFrame) < field_194_timer - 24)
    {
        DeathSmokeEffect(true);
    }

    // Finally fizzled out
    if (GetSpriteScale() < FP_FromInteger(0))
    {
        SetDead(true);
    }

    return 100;
}

s16 Mudokon::Brain_6_Escape()
{
    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds.Find_Impl(field_11C_bird_portal_id));
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
    else
    {
        bool noBirdPortalOrPortalIsDead = false;
        if (!pBirdPortal || pBirdPortal->GetDead())
        {
            noBirdPortalOrPortalIsDead = true;
        }

        bool bOver60Away = false;
        if (pBirdPortal)
        {
            bOver60Away = FP_Abs(pBirdPortal->mYPos - mYPos) > (GetSpriteScale() * FP_FromInteger(60));
        }

        if (bOver60Away || noBirdPortalOrPortalIsDead)
        {
            sGoingToBirdPortalMudCount_5C3012--;
            mEscaping = false;

            if (pBirdPortal)
            {
                if (sGoingToBirdPortalMudCount_5C3012 == 0)
                {
                    pBirdPortal->VKillPortalClipper();
                    pBirdPortal->VGiveShrykull(true);
                }
                field_11C_bird_portal_id = Guid{};
            }

            mNextMotion = eMudMotions::Motion_0_Idle;
            mBrainState = Mud_Brain_State::Brain_4_ListeningToAbe;
            return Brain_4_ListeningToAbe::eBrain4_Idle_7;
        }
        else
        {
            switch (mBrainSubState)
            {
                case Brain_6_Escape::eBrain6_PortalOppened_0:
                    if (!pBirdPortal->VActivePortal())
                    {
                        return mBrainSubState;
                    }
                    field_194_timer = sGnFrame + Math_NextRandom() % 8;
                    return Brain_6_Escape::eBrain6_StandingUp_1;

                case Brain_6_Escape::eBrain6_StandingUp_1:
                    if (static_cast<s32>(sGnFrame) <= field_194_timer)
                    {
                        return mBrainSubState;
                    }
                    return FP_Abs(pBirdPortal->mXPos - mXPos) >= ScaleToGridSize(GetSpriteScale()) ? 2 : 4;

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

                    if (mCurrentMotion == eMudMotions::Motion_21_RunLoop)
                    {
                        if (!FacingBirdPortal(pBirdPortal))
                        {
                            mNextMotion = eMudMotions::Motion_25_RunSlideTurn;
                        }
                        else
                        {
                            if (VIntoBirdPortal(3)) // Finds portal to jump into
                            {
                                mBrainSubState = Brain_6_Escape::eBrain6_Jumping_3;
                                mNextMotion = eMudMotions::Motion_36_RunJumpMid;
                            }
                        }
                    }
                    return mBrainSubState;

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
                        if ((pBirdPortal->mEnterSide == relive::Path_BirdPortal::PortalSide::eRight && GetAnimation().GetFlipX()) || (pBirdPortal->mEnterSide == relive::Path_BirdPortal::PortalSide::eLeft && !GetAnimation().GetFlipX()))
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
                        return mBrainSubState;
                    }

                    if (FP_Abs(pBirdPortal->mXPos - mXPos) <= ScaleToGridSize(GetSpriteScale()))
                    {
                        return mBrainSubState;
                    }
                    return Brain_6_Escape::eBrain6_Running_2;
            }
        }
    }
    return mBrainSubState;
}

s16 Mudokon::Brain_7_FallAndSmackDeath()
{
    if (mBrainSubState == 0)
    {
        // Wait for falling
        Mudokon_SFX(MudSounds::eDeathDropScream_15, 0, 0, this);
        field_194_timer = sGnFrame + 60;
        return 1;
    }

    if (mBrainSubState == 1)
    {
        // Scream
        if (static_cast<s32>(sGnFrame) == field_194_timer - 6)
        {
            SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
        }

        // Shake from ze floor slam & die
        if (static_cast<s32>(sGnFrame) > field_194_timer)
        {
            Environment_SFX(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 32767, this);
            relive_new ScreenShake(0, 0);
            SetDead(true);
        }
    }
    return mBrainSubState;
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
    switch (mBrainSubState)
    {
        case Brain_8_AngryWorker::eBrain8_Inactive_0:
            if (mCurrentMotion)
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
            }

            if (mRingAndAngryMudTimeout)
            {
                const GameSpeakEvents lastSpeak = LastGameSpeak();
                if (lastSpeak < GameSpeakEvents::eAbe_Hello_9 || lastSpeak > GameSpeakEvents::eAbe_Sorry_24)
                {
                    return mBrainSubState;
                }
            }

            field_194_timer = sGnFrame + Math_RandomRange(30, 45);
            
            // adds mudokon lag when quicksaving/quickloading in the same screen
            AddAlerted();

            return gPathInfo->TLV_Get_At(
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
            if (lastSpeak >= GameSpeakEvents::eAbe_Hello_9
                && lastSpeak <= GameSpeakEvents::eAbe_Sorry_24
                && sActiveHero->GetSpriteScale() == GetSpriteScale())
            {
                field_160_delayed_speak = sAngryWorkerResponses_55CFCA[static_cast<s32>(lastSpeak) - 9];
                field_194_timer = MudResponseDelay() + sGnFrame + 20;
                return Brain_8_AngryWorker::eBrain8_Speaking_3;
            }

            if (field_194_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }

            const FP ypos = mYPos - ScaleToGridSize(GetSpriteScale());
            FP xOff = {};
            if (GetAnimation().GetFlipX())
            {
                xOff = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                xOff = ScaleToGridSize(GetSpriteScale());
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
                return mBrainSubState;
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
                return mBrainSubState;
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
                mBrainState = Mud_Brain_State::Brain_4_ListeningToAbe;
                return Brain_4_ListeningToAbe::eBrain4_Idle_7;
            }
            else
            {
                mNextMotion = eMudMotions::M_Speak_6_472FA0;
                MudEmotionSound(field_160_delayed_speak);
                field_194_timer = sGnFrame + Math_RandomRange(30, 45);
                return gPathInfo->TLV_Get_At(
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
                return mBrainSubState;
            }
            mNextMotion = eMudMotions::Motion_57_TurnWheelBegin;
            return Brain_8_AngryWorker::eBrain8_UsingWheel_5;

        case Brain_8_AngryWorker::eBrain8_UsingWheel_5:
        {
            const GameSpeakEvents lastSpeak2 = LastGameSpeak();
            if (lastSpeak2 >= GameSpeakEvents::eAbe_Hello_9
                && lastSpeak2 <= GameSpeakEvents::eAbe_Sorry_24
                && sActiveHero->GetSpriteScale() == GetSpriteScale())
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
                field_160_delayed_speak = sAngryWorkerResponses_55CFCA[static_cast<s32>(lastSpeak2) - 9];
                field_194_timer = MudResponseDelay() + sGnFrame + 20;
                return Brain_8_AngryWorker::eBrain8_Speaking_3;
            }

            if (field_17E_delayed_speak != MudAction::eSorry_8)
            {
                return mBrainSubState;
            }

            mNextMotion = eMudMotions::Motion_0_Idle;
            field_194_timer = sGnFrame + 20;
        }
            return Brain_8_AngryWorker::eBrain8_Speaking_3;

        case Brain_8_AngryWorker::eBrain8_Unused_6:
            if (static_cast<s32>(sGnFrame) <= field_194_timer)
            {
                return mBrainSubState;
            }

            if (mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                if (mCurrentMotion == eMudMotions::Motion_15_CrouchIdle)
                {
                    mNextMotion = eMudMotions::Motion_0_Idle;
                    return mBrainSubState;
                }
                else
                {
                    if (mCurrentMotion == eMudMotions::Motion_53_Duck)
                    {
                        mNextMotion = eMudMotions::Motion_15_CrouchIdle;
                    }
                    return mBrainSubState;
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
                return mBrainSubState;
            }
            mNextMotion = eMudMotions::Motion_55_DuckKnockback;
            field_194_timer = sGnFrame + 60;
            return Brain_8_AngryWorker::eBrain8_Unused_6;
    }

    return mBrainSubState;
}

s16 Mudokon::Brain_9_Sick()
{
    // TODO: Lame hack, tortured muds make the "real" mud do nothing.
    // Sometimes this hack fails which makes AddAlerted underministic. With it disabled it just means that muds will respond to game speak which they sometimes did anyway when in a tear x-tractor.
    /*
    if (FindObjectOfType(ReliveTypes::eTorturedMud, mXPos, mYPos - FP_FromInteger(50)) || !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        LastGameSpeak(); // TODO: Pointless call ??
        return mBrainSubState;
    }*/

    if (IsEventInRange(
            kEventMudokonAbuse,
            mXPos,
            mYPos,
            AsEventScale(GetScale())))
    {
        field_17E_delayed_speak = MudAction::eMudAbuse_9;
    }

    if (IsEventInRange(
            kEventMudokonComfort,
            mXPos,
            mYPos,
            AsEventScale(GetScale())))
    {
        field_17E_delayed_speak = MudAction::eComfort_10;
    }

    if (IsEventInRange(
            kEventMudokonLaugh,
            mXPos,
            mYPos,
            AsEventScale(GetScale())))
    {
        field_17E_delayed_speak = MudAction::eLaugh_12;
    }

    if (IsEventInRange(kEventAbeDead,
            mXPos,
            mYPos,
                                AsEventScale(GetScale())))
    {
        field_17E_delayed_speak = MudAction::eDuck_13;
    }

    if (IsEventInRange(
            kEventShooting,
            mXPos,
            mYPos,
            AsEventScale(GetScale())))
    {
        field_17E_delayed_speak = MudAction::eWait_2;
    }

    // Have Abe make a sad noise when he first sees sick Mudokons.
    if (!mSeenWhileSick && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        if (sActiveHero->mSay == MudSounds::eNone)
        {
            sActiveHero->mSay = MudSounds::eSadUgh_28;
            sActiveHero->mAutoSayTimer = sGnFrame + 10;
        }
        mSeenWhileSick = true;
    }

    switch (mBrainSubState)
    {
        case Brain_9_Sick::eBrain9_Inactive_0:
            if (mCurrentMotion != eMudMotions::Motion_15_CrouchIdle)
            {
                if (mCurrentMotion != eMudMotions::Motion_17_StandToCrouch && mNextMotion != eMudMotions::Motion_17_StandToCrouch)
                {
                    mNextMotion = eMudMotions::Motion_17_StandToCrouch;
                }
                return mBrainSubState;
            }

            // Causes mud lag
            AddAlerted();

            SetCanBePossessed(true);
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
                return mBrainSubState;
            }

            if (field_180_emo_tbl != Mud_Emotion::eSick_7)
            {
                SetCanBePossessed(false);
                mNextMotion = eMudMotions::Motion_0_Idle;
                relive_new PossessionFlicker(this, 8, 155, 255, 32);
                return Brain_9_Sick::eBrain9_StandingUp_3;
            }

            if (field_17E_delayed_speak == MudAction::eNone_17)
            {
                const GameSpeakEvents lastSpeak = LastGameSpeak();
                if (lastSpeak == GameSpeakEvents::eNone_m1)
                {
                    return mBrainSubState;
                }

                field_178_brain_sub_state2 = mBrainSubState;
                field_194_timer = MudResponseDelay() + sGnFrame + 20;

                switch (lastSpeak)
                {
                    case GameSpeakEvents::eAbe_Fart_3:
                        field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eFart_6);
                        break;
                    case GameSpeakEvents::Slig_BS_5:
                    case GameSpeakEvents::Slig_LookOut_6:
                    case GameSpeakEvents::Slig_BS2_7:
                    case GameSpeakEvents::Slig_Laugh_8:
                    case GameSpeakEvents::eAbe_Hello_9:
                    case GameSpeakEvents::eAbe_FollowMe_10:
                    case GameSpeakEvents::eAbe_Wait_12:
                    case GameSpeakEvents::eAbe_Work_21:
                    case GameSpeakEvents::eAbe_StopIt_22:
                    case GameSpeakEvents::eAbe_AllYa_23:
                    case GameSpeakEvents::eSlig_Hi_27:
                    case GameSpeakEvents::eSlig_HereBoy_28:
                    case GameSpeakEvents::eSlig_GetEm_29:
                    case GameSpeakEvents::eSlig_Freeze_31:
                        field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eWait_2);
                        break;
                    default:
                        return mBrainSubState;
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
                field_178_brain_sub_state2 = mBrainSubState;
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
                return mBrainSubState;
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
                return mBrainSubState;
            }

            MudEmotionSound(MudSounds::eFart_7);
            mNextMotion = eMudMotions::M_Speak_6_472FA0;

            if (GetAnimation().GetFlipX())
            {
                New_Smoke_Particles(
                    mXPos + (FP_FromInteger(12) * GetSpriteScale()),
                    mYPos - (FP_FromInteger(24) * GetSpriteScale()),
                    (FP_FromDouble(0.5) * GetSpriteScale()),
                    3, RGB16{ 32, 128, 32 });
            }
            else
            {
                New_Smoke_Particles(
                    mXPos - (FP_FromInteger(12) * GetSpriteScale()),
                    mYPos - (FP_FromInteger(24) * GetSpriteScale()),
                    (FP_FromDouble(0.5) * GetSpriteScale()),
                    3, RGB16{ 32, 128, 32 });
            }
            return Brain_9_Sick::eBrain9_Farting_4;

        case Brain_9_Sick::eBrain9_Farting_4:
            if (mCurrentMotion)
            {
                return mBrainSubState;
            }
            mBrainState = Mud_Brain_State::Brain_4_ListeningToAbe;
            return Brain_4_ListeningToAbe::eBrain4_Inactive_0;

        default:
            return mBrainSubState;
    }
    return mBrainSubState;
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
            OnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {bRect.w, static_cast<s16>(bRect.h + 5)},
                gPlatformsArray);
        }
    }

    switch (mNextMotion)
    {
        case eMudMotions::Motion_1_WalkLoop:
            if (GetAnimation().GetFlipX())
            {
                mVelX = -ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9);
            }
            else
            {
                mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9);
            }
            mCurrentMotion = eMudMotions::Motion_7_WalkBegin;
            mNextMotion = -1;
            break;

        case eMudMotions::Motion_27_SneakLoop:
            if (GetAnimation().GetFlipX())
            {
                mVelX = -ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10);
            }
            else
            {
                mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10);
            }
            mCurrentMotion = eMudMotions::Motion_32_SneakBegin;
            mNextMotion = -1;
            break;

        case eMudMotions::Motion_21_RunLoop:
            if (GetAnimation().GetFlipX())
            {
                mVelX = -ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
            }
            else
            {
                mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
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
    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback();
    }
    else if (sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId) && mBlind && (WallHit(GetSpriteScale() * FP_FromInteger(1), mVelX)))
    {
        ToKnockback();
    }
    else
    {
        MoveOnLine();
        if (mCurrentMotion == eMudMotions::Motion_1_WalkLoop)
        {
            if (GetAnimation().GetCurrentFrame() == 2 || GetAnimation().GetCurrentFrame() == 11)
            {
                if (mNextMotion == eMudMotions::Motion_0_Idle)
                {
                    mNextMotion = -1;
                    if (GetAnimation().GetCurrentFrame() == 2)
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
                    if (GetAnimation().GetCurrentFrame() != 2)
                    {
                        mCurrentMotion = eMudMotions::Motion_9_MidWalkToIdle;
                    }
                    else
                    {
                        mCurrentMotion = eMudMotions::Motion_8_WalkToIdle;
                    }
                }
            }
            else if (GetAnimation().GetCurrentFrame() == 5 || GetAnimation().GetCurrentFrame() == 14)
            {
                Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);
                MapFollowMe(true);
                if (mNextMotion == eMudMotions::Motion_21_RunLoop)
                {
                    mNextMotion = -1;
                    if (GetAnimation().GetCurrentFrame() == 5)
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
                    auto curFrame2 = GetAnimation().GetCurrentFrame();
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

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Environment_SFX(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();
        ToStand();
    }
}

void Mudokon::Motion_Speak()
{
    CheckFloorGone();
    SetPal(field_180_emo_tbl);

    if (mAlertEnemies)
    {
        EventBroadcast(kEventNoise, this);
        EventBroadcast(kEventSuspiciousNoise, this);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mAlertEnemies = true;
        SetPal(Mud_Emotion::eNormal_0);
        ToStand();
    }
}

void Mudokon::Motion_7_WalkBegin()
{
    EventBroadcast(kEventNoise, this);
    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToStand();
    }
    else
    {
        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eMudMotions::Motion_1_WalkLoop;
        }
        MoveOnLine();
    }
}

void Mudokon::Motion_8_WalkToIdle()
{
    EventBroadcast(kEventNoise, this);
    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();

        if (GetAnimation().GetCurrentFrame() == 0)
        {
            Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);
            return;
        }

        if (GetAnimation().GetIsLastFrame())
        {
            MapFollowMe(true);
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
    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Mudokon::Motion_11_Chisel()
{
    CheckFloorGone();

    if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        if (GetAnimation().GetIsLastFrame())
        {
            if (sGnFrame & 1)
            {
                SfxPlayMono(relive::SoundEffects::Chisel, 0, GetSpriteScale());

                FP sparkY = {};
                FP sparkX = {};
                if (GetAnimation().GetFlipX())
                {
                    sparkY = mYPos - (GetSpriteScale() * FP_FromInteger(3));
                    sparkX = mXPos - (GetSpriteScale() * FP_FromInteger(18));
                }
                else
                {
                    sparkY = mYPos - (GetSpriteScale() * FP_FromInteger(3));
                    sparkX = (GetSpriteScale() * FP_FromInteger(18)) + mXPos;
                }

                relive_new Spark(
                    sparkX + FP_FromInteger(mXOffset),
                    sparkY,
                    GetSpriteScale(),
                    9,
                    0,
                    255,
                    SparkType::eSmallChantParticle_0);
            }
        }
    }

    if (GetAnimation().GetIsLastFrame())
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
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_11_Chisel;
    }
}

void Mudokon::Motion_13_StopChisel()
{
    CheckFloorGone();
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_15_CrouchIdle;
    }
}

void Mudokon::Motion_14_CrouchScrub()
{
    CheckFloorGone();

    if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        if (GetAnimation().GetCurrentFrame() == 2)
        {
            SfxPlayMono(relive::SoundEffects::Clean1, 0);
        }
        else if (GetAnimation().GetCurrentFrame() == 6)
        {
            SfxPlayMono(relive::SoundEffects::Clean2, 0);
        }
    }

    if (GetAnimation().GetIsLastFrame())
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

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_15_CrouchIdle;
        GetAnimation().ToggleFlipX();
    }
}

void Mudokon::Motion_17_StandToCrouch()
{
    CheckFloorGone();
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_15_CrouchIdle;
    }
}

void Mudokon::Motion_18_CrouchToStand()
{
    CheckFloorGone();
    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Mudokon::Motion_19_WalkToRun()
{
    EventBroadcast(kEventNoise, this);
    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_21_RunLoop;
    }

    if (WallHit((GetSpriteScale() * FP_FromInteger(50)), mVelX))
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
        mReturnToPreviousMotion = true;
    }
}

void Mudokon::Motion_21_RunLoop()
{
    EventBroadcast(kEventNoise, this);

    if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
    {
        EventBroadcast(kEventSuspiciousNoise, this);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback();
        return;
    }

    MoveOnLine();

    if (mCurrentMotion == eMudMotions::Motion_21_RunLoop)
    {
        if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 8)
        {
            MapFollowMe(true);

            if (mNextMotion == eMudMotions::Motion_36_RunJumpMid)
            {
                mCurrentMotion = eMudMotions::Motion_35_RunJumpBegin;
                mNextMotion = -1;
            }
        }
        else if (GetAnimation().GetCurrentFrame() == 4 || GetAnimation().GetCurrentFrame() == 12)
        {
            Environment_SFX(EnvironmentSfx::eRunningFootstep_2, 0, 32767, this);
            MapFollowMe(true);

            switch (mNextMotion)
            {
                case eMudMotions::Motion_1_WalkLoop:
                    mNextMotion = -1;
                    if (GetAnimation().GetCurrentFrame() == 4)
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
                    Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
                    return;

                case eMudMotions::Motion_25_RunSlideTurn:
                    mCurrentMotion = eMudMotions::Motion_25_RunSlideTurn;
                    mNextMotion = -1;
                    Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
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
    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_1_WalkLoop;
    }

    if (WallHit((GetSpriteScale() * FP_FromInteger(50)), mVelX))
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
        mReturnToPreviousMotion = true;
    }
}

void Mudokon::Motion_24_RunSlideStop()
{
    EventBroadcast(kEventNoise, this);
    if (WallHit((GetSpriteScale() * FP_FromInteger(50)), mVelX))
    {
        ToKnockback();
    }
    else
    {
        ReduceXVelocityBy(FP_FromDouble(0.375));

        if (mCurrentMotion == eMudMotions::Motion_24_RunSlideStop)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);
                MapFollowMe(true);
                Mudokon::ToStand();
            }
        }
    }
}

void Mudokon::Motion_25_RunSlideTurn()
{
    EventBroadcast(kEventNoise, this);

    if (WallHit((GetSpriteScale() * FP_FromInteger(50)), mVelX))
    {
        ToKnockback();
    }
    else
    {
        ReduceXVelocityBy(FP_FromDouble(0.375));

        if (mCurrentMotion == eMudMotions::Motion_25_RunSlideTurn)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                MapFollowMe(true);
                if (GetAnimation().GetFlipX())
                {
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    mCurrentMotion = eMudMotions::Motion_26_RunTurnToRun;
                }
                else
                {
                    mCurrentMotion = eMudMotions::Motion_26_RunTurnToRun;
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                }
            }
        }
    }
}

void Mudokon::Motion_26_RunTurnToRun()
{
    EventBroadcast(kEventNoise, this);

    if (WallHit((GetSpriteScale() * FP_FromInteger(50)), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();

        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eMudMotions::Motion_21_RunLoop;
            GetAnimation().ToggleFlipX();
        }
    }
}

void Mudokon::Motion_27_SneakLoop()
{
    if (WallHit((GetSpriteScale() * FP_FromInteger(50)), mVelX))
    {
        ToKnockback();
    }
    else
    {
        MoveOnLine();
        if (mCurrentMotion == eMudMotions::Motion_27_SneakLoop)
        {
            if (GetAnimation().GetCurrentFrame() == 3 || GetAnimation().GetCurrentFrame() == 13)
            {
                if (mNextMotion == eMudMotions::Motion_0_Idle)
                {
                    mNextMotion = -1;
                    mCurrentMotion = (GetAnimation().GetCurrentFrame() != 3) ? eMudMotions::Motion_33_SneakToIdle : eMudMotions::Motion_34_MidSneakToIdle;
                }
            }
            else if (GetAnimation().GetCurrentFrame() == 6 || GetAnimation().GetCurrentFrame() == 16)
            {
                Environment_SFX(EnvironmentSfx::eSneakFootstep_3, 0, 32767, this);
                MapFollowMe(true);

                if (mNextMotion == eMudMotions::Motion_1_WalkLoop)
                {
                    mNextMotion = -1;
                    mCurrentMotion = GetAnimation().GetCurrentFrame() != 6 ? eMudMotions::Motion_31_MidSneakToWalk : eMudMotions::Motion_29_SneakToWalk;
                }
            }
        }
    }
}

void Mudokon::Motion_28_MidWalkToSneak()
{
    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_27_SneakLoop;
    }

    if (WallHit((GetSpriteScale() * FP_FromInteger(50)), mVelX))
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
    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_1_WalkLoop;
    }

    if (WallHit((GetSpriteScale() * FP_FromInteger(50)), mVelX))
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
        mReturnToPreviousMotion = true;
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
        mReturnToPreviousMotion = true;
    }
}

void Mudokon::Motion_32_SneakBegin()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_27_SneakLoop;
    }

    if (WallHit((GetSpriteScale() * FP_FromInteger(50)), mVelX))
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

    if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(true);
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

    if (!GetAnimation().GetCurrentFrame())
    {
        Environment_SFX(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 32767, this);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        if (GetAnimation().GetFlipX())
        {
            mVelX = (GetSpriteScale() * -FP_FromDouble(7.6)); // TODO: Correct FP value?
        }
        else
        {
            mVelX = (GetSpriteScale() * FP_FromDouble(7.6)); // TODO: Correct FP value?
        }
        mVelY = (GetSpriteScale() * -FP_FromDouble(9.6));
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
    if (GetAnimation().GetCurrentFrame() == 5)
    {
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
    }

    const PSX_RECT rect = VGetBoundingRect();

    if ((mVelX > FP_FromInteger(0) && FP_FromInteger(rect.x) > pBirdPortal->mXPos) || (mVelX < FP_FromInteger(0) && FP_FromInteger(rect.w) < pBirdPortal->mXPos))
    {
        mNotRescued = false;
        mPersistAndResetOffscreen = false;

        SetDead(true);

        mVelY = FP_FromInteger(0);
        mVelX = FP_FromInteger(0);

        SND_SEQ_Play(SeqId::SaveTriggerMusic_31, 1, 127, 127);

        relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eChime, relive::Path_MusicTrigger::TriggeredBy::eTimer, 0);

        gRescuedMudokons++;

        if (gZulagNumber < ALIVE_COUNTOF(sSavedKilledMudsPerZulag_5C1B50.mData))
        {
            sSavedKilledMudsPerZulag_5C1B50.mData[gZulagNumber]++;
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

    mVelY += (GetSpriteScale() * FP_FromDouble(1.8));
    mXPos += mVelX;
    mYPos += mVelY;
}

void Mudokon::Motion_37_StandToRun()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_21_RunLoop;
    }

    EventBroadcast(kEventNoise, this);

    if (WallHit((GetSpriteScale() * FP_FromInteger(50)), mVelX))
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

    if (GetAnimation().GetCurrentFrame() == 5)
    {
        if (field_180_emo_tbl == Mud_Emotion::eAngry_1 || field_180_emo_tbl == Mud_Emotion::eAggressive_2)
        {
            BaseGameObject* pSlapTarget = nullptr;
            if (GetAnimation().GetFlipX())
            {
                pSlapTarget = FindObjectOfType(ReliveTypes::eAbe, mXPos - ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5));
            }
            else
            {
                pSlapTarget = FindObjectOfType(ReliveTypes::eAbe, mXPos + ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5));
            }

            if (!pSlapTarget)
            {
                if (GetAnimation().GetFlipX())
                {
                    pSlapTarget = FindObjectOfType(ReliveTypes::eMudokon, mXPos - ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5));
                }
                else
                {
                    pSlapTarget = FindObjectOfType(ReliveTypes::eMudokon, mXPos + ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5));
                }
            }

            if (pSlapTarget)
            {
                static_cast<BaseAliveGameObject*>(pSlapTarget)->VTakeDamage(this);
            }
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        SetPal(Mud_Emotion::eNormal_0);
        ToStand();
    }
}

void Mudokon::Motion_39_HoistBegin()
{
    if (GetAnimation().GetIsLastFrame())
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        mVelY = (GetSpriteScale() * -FP_FromInteger(8));
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
                MapFollowMe(true);
                BaseAliveGameObjectCollisionLine = pLine;
                mCurrentMotion = eMudMotions::Motion_41_LandSoft1;
                bRect = VGetBoundingRect();
                OnCollisionWith(
                    {bRect.x, static_cast<s16>(bRect.y + 5)},
                    {bRect.w, static_cast<s16>(bRect.h + 5)},
                    gPlatformsArray);
                break;
            default:
                return;
        }
    }
}

void Mudokon::Motion_41_LandSoft1()
{
    if (GetAnimation().GetCurrentFrame() == 2)
    {
        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Mudokon::Motion_42_LandSoft2()
{
    CheckFloorGone();

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (mPreviousMotion == eMudMotions::Motion_49_Fall)
        {
            Environment_SFX(EnvironmentSfx::eLandingSoft_5, 0, 32767, this);
        }
        else
        {
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Mudokon::Motion_43_DunnoBegin()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_44_DunnoEnd;
    }
}

void Mudokon::Motion_44_DunnoEnd()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Mudokon::Motion_45_KnockForward()
{
    if (GetAnimation().GetCurrentFrame() == 12)
    {
        CheckKnockedOntoABomb();
    }

    if (GetAnimation().GetCurrentFrame() == 4)
    {
        Environment_SFX(EnvironmentSfx::eKnockback_13, 0, 32767, this);
    }

    if ((gMap.mCurrentLevel == EReliveLevelIds::eMines
        || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz
        || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot
        || gMap.mCurrentLevel == EReliveLevelIds::eBarracks
        || gMap.mCurrentLevel == EReliveLevelIds::eBrewery)
        && GetAnimation().GetCurrentFrame() == 7)
    {
        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (mNextMotion != -1 && mHealth > FP_FromInteger(0))
    {
        mCurrentMotion = mNextMotion;
        mNextMotion = -1;
    }
}

void Mudokon::Motion_46_Knockback()
{
    if (GetAnimation().GetCurrentFrame() == 12)
    {
        CheckKnockedOntoABomb();
    }

    EventBroadcast(kEventNoise, this);

    if ((gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBrewery) && GetAnimation().GetCurrentFrame() == 7)
    {
        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_49_Fall();

        if (mCurrentMotion == eMudMotions::Motion_45_KnockForward || mCurrentMotion == eMudMotions::Motion_42_LandSoft2)
        {
            mCurrentMotion = eMudMotions::Motion_46_Knockback;
        }
    }

    if (GetAnimation().GetIsLastFrame())
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
    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Mudokon::Motion_48_WalkOffEdge()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_49_Fall;
    }

    Motion_49_Fall();
}

void Mudokon::Motion_49_Fall()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX = mVelX - (GetSpriteScale() * field_134_xVelSlowBy);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX = (GetSpriteScale() * field_134_xVelSlowBy) + mVelX;
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
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                mXPos = hitX;
                mYPos = hitY;
                BaseAliveGameObjectCollisionLine = pLine;

                MapFollowMe(true);

                if ((gPathInfo->TLV_Get_At(
                         FP_GetExponent(mXPos),
                         FP_GetExponent(mYPos),
                         FP_GetExponent(mXPos),
                         FP_GetExponent(mYPos),
                         ReliveTypes::eSoftLanding)
                     && mHealth > FP_FromInteger(0))
                    || (mYPos - BaseAliveGameObjectLastLineYPos < (GetSpriteScale() * FP_FromInteger(180)) && (mHealth > FP_FromInteger(0) || gAbeInvincible)))
                {
                    mCurrentMotion = eMudMotions::Motion_42_LandSoft2;
                }
                else
                {
                    mHealth = FP_FromInteger(0);
                    ToKnockback();
                    mPersistAndResetOffscreen = false;
                    mBrainState = Mud_Brain_State::Brain_5_ShrivelDeath;
                    mCurrentMotion = eMudMotions::Motion_45_KnockForward;
                    field_194_timer = sGnFrame + 90;
                }

                mPreviousMotion = eMudMotions::Motion_49_Fall;

                OnCollisionWith(
                    {FP_GetExponent(mXPos - FP_FromInteger(10)), FP_GetExponent(mYPos - FP_FromInteger(10))},
                    {FP_GetExponent(mXPos + FP_FromInteger(10)), FP_GetExponent(mYPos + FP_FromInteger(10))},
                    gPlatformsArray);
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
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
        if (GetAnimation().GetIsLastFrame())
        {
            SND_SEQ_Stop(SeqId::MudokonChant_11);
            mCurrentMotion = eMudMotions::Motion_51_ChantEnd;
            mNextMotion = -1;
        }
    }
}

void Mudokon::Motion_51_ChantEnd()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Mudokon::Motion_52_ToDuck()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_53_Duck;
    }
}

void Mudokon::Motion_53_Duck()
{
    if (GetAnimation().GetIsLastFrame())
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
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_15_CrouchIdle;
    }
}

void Mudokon::Motion_55_DuckKnockback()
{
    if (GetAnimation().GetCurrentFrame() == 1)
    {
        Environment_SFX(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eMudMotions::Motion_53_Duck;
        mNextMotion = -1;
    }
}

void Mudokon::Motion_56_SlapOwnHead()
{
    if (GetAnimation().GetCurrentFrame() == 5)
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
        Environment_SFX(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
        EventBroadcast(kEventMudokonDied, this);
        mPersistAndResetOffscreen = false;
        mHealth = FP_FromInteger(0);
        mBrainState = Mud_Brain_State::Brain_5_ShrivelDeath;
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

    if (GetAnimation().GetIsLastFrame())
    {
        auto pWheel = static_cast<WorkWheel*>(FindObjectOfType(ReliveTypes::eWorkWheel, mXPos, mYPos - (GetSpriteScale() * FP_FromInteger(50))));
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
            pWheel->VStopTurning(true);
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

    if (GetAnimation().GetIsLastFrame())
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
                if (!WallHit(GetSpriteScale() * FP_FromInteger(50), xDist))
                {
                    if (pOpenPortal->mPortalType == relive::Path_BirdPortal::PortalType::eWorker || pOpenPortal->mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
                    {
                        sActiveHero->ChangeChantState_45BB90(1);
                        field_11C_bird_portal_id = pOpenPortal->mBaseGameObjectId;
                        sGoingToBirdPortalMudCount_5C3012++;
                        mEscaping = true;
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
                if (!WallHit(GetSpriteScale() * FP_FromInteger(50), xDist))
                {
                    if (pPortal20->mPortalType == relive::Path_BirdPortal::PortalType::eWorker || pPortal20->mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
                    {
                        sActiveHero->ChangeChantState_45BB90(1);
                        field_11C_bird_portal_id = pPortal20->mBaseGameObjectId;

                        sGoingToBirdPortalMudCount_5C3012++;
                        mEscaping = true;
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

    const s32 lastEventIdx = gEventSystem->mLastEventIndex;
    if (field_140_last_event_index == lastEventIdx)
    {
        if (gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1)
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
        actualEvent = gEventSystem->mLastEvent;
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
    if (actualEvent < GameSpeakEvents::eUnknown_1 || actualEvent > GameSpeakEvents::eAbe_Sorry_24 || sActiveHero->GetSpriteScale() == GetSpriteScale())
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
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Is there another object that isn't us on the same scale?
        if (pObj != this && pObj->GetSpriteScale() == sActiveHero->GetSpriteScale())
        {
            // Is it a mud who isn't currently talking to abe and is in the same screen?
            if ((pObj->Type() == ReliveTypes::eRingOrLiftMud || pObj->Type() == ReliveTypes::eMudokon) && static_cast<Mudokon*>(pObj)->mBrainState != Mud_Brain_State::Brain_4_ListeningToAbe && gMap.Is_Point_In_Current_Camera(pObj->mCurrentLevel, pObj->mCurrentPath, pObj->mXPos, pObj->mYPos, 0))
            {
                if (sActiveHero->VIsFacingMe(pObj) && !sActiveHero->VIsFacingMe(this))
                {
                    // Abe is facing the other guy and not us :(
                    return false;
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
                        return false;
                    }
                }
            }
        }
    }
    return true;
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
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mCurrentMotion = eMudMotions::Motion_0_Idle;
}

s16 Mudokon::FindWheel(FP xpos, FP ypos)
{
    auto* pWheelTlv = static_cast<relive::Path_WorkWheel*>(
        gPathInfo->TLV_Get_At(
            FP_GetExponent(xpos), FP_GetExponent(ypos), FP_GetExponent(xpos), FP_GetExponent(ypos), ReliveTypes::eWorkWheel));

    if (pWheelTlv)
    {
        if (!SwitchStates_Get(pWheelTlv->mSwitchId))
        {
            return FindObjectOfType(ReliveTypes::eWorkWheel, xpos, ypos - (GetSpriteScale() * FP_FromInteger(50))) != 0;
        }
    }
    return 0;
}

s16 Mudokon::GetBrainSubStateResponse(MudAction action)
{
    field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, action);
    if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || mGetDepressed)
    {
        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
    }
    return field_188_pTblEntry->field_6_sub_state;
}

s16 Mudokon::StopAtWheel()
{
    FP offset = {};
    FP_Rect fpRect = {};

    if (GetAnimation().GetFlipX())
    {
        offset = -ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(0.4);
        fpRect.x = mXPos + (offset * FP_FromInteger(2));
        fpRect.w = mXPos;
    }
    else
    {
        offset = ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(0.4);
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
                if (pOtherMud->mStoppedAtWheel || (gPathInfo->TLV_Get_At(FP_GetExponent(pObj->mXPos), FP_GetExponent(pObj->mYPos), FP_GetExponent(pObj->mXPos), FP_GetExponent(pObj->mYPos), ReliveTypes::eWorkWheel) && pOtherMud->mVelX == FP_FromInteger(0)))
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
        pWheel->VStopTurning(true);
        field_158_wheel_id = Guid{};
    }

    Environment_SFX(EnvironmentSfx::eKnockback_13, 0, 32767, this);

    if (mVelX > FP_FromInteger(0))
    {
        if (GetAnimation().GetFlipX())
        {
            mXPos += (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2)) + mVelX;
        }
        else
        {
            mXPos -= (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2)) - mVelX;
        }
    }

    MapFollowMe(true);

    mVelX = FP_FromInteger(0);

    if (mVelY < FP_FromInteger(0))
    {
        mVelY = FP_FromInteger(0);
    }

    Environment_SFX(EnvironmentSfx::eKnockback_13, 0, 32767, this);
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
            OnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {bRect.w, static_cast<s16>(bRect.h + 5)},
                gPlatformsArray);
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
    if (!mAlerted)
    {
        mAlerted = true;
        sAlertedMudCount_5C3010++;
        LOG_INFO("Alert added %d", sAlertedMudCount_5C3010);
    }
}

void Mudokon::RemoveAlerted()
{
    if (mAlerted)
    {
        sAlertedMudCount_5C3010--;
        mAlerted = false;
        LOG_INFO("Alert removed %d", sAlertedMudCount_5C3010);
    }
}

s16 Mudokon::BrainStartWheelTurning()
{
    mNextMotion = eMudMotions::Motion_57_TurnWheelBegin;
    RemoveAlerted();
    mStoppedAtWheel = true;
    mBrainState = Mud_Brain_State::Brain_3_TurnWheel;
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
        mVelX = mVelX - (GetSpriteScale() * velXScaleBy);
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
        mVelX = (GetSpriteScale() * velXScaleBy) + mVelX;
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
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
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

            if (mFollowingAbe && (field_180_emo_tbl == Mud_Emotion::eHappy_5 || field_180_emo_tbl == Mud_Emotion::eWired_6))
            {
                field_178_brain_sub_state2 = 4;
            }
            else
            {
                field_178_brain_sub_state2 = 7;
                mFollowingAbe = false;
            }

            mBrainState = Mud_Brain_State::Brain_4_ListeningToAbe;
            mBrainSubState = field_188_pTblEntry->field_6_sub_state;
        }

        if (field_180_emo_tbl != Mud_Emotion::eSad_3 || IsActiveHero(pFrom))
        {
            field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
        }
    }
    else
    {
        Mudokon_SFX(MudSounds::eHurt2_9, 0, 1000, this);
        Environment_SFX(EnvironmentSfx::eDeathNoise_7, 0, 32767, this);
        EventBroadcast(kEventMudokonDied, sActiveHero);
        mPersistAndResetOffscreen = false;
        mHealth = FP_FromInteger(0);
        mBrainState = Mud_Brain_State::Brain_5_ShrivelDeath;
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
