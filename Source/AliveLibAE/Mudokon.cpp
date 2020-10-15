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
#include "Switch.hpp"
#include "Particle.hpp"
#include "PossessionFlicker.hpp"
#include "ScreenShake.hpp"
#include "MainMenu.hpp"
#include "StatsSign.hpp"
#include "MusicTrigger.hpp"
#include "Blood.hpp"
#include "Gibs.hpp"
#include "Bullet.hpp"
#include "LaughingGas.hpp"
#include "Sys_common.hpp"

ALIVE_VAR(1, 0x5C3012, short, word_5C3012, 0);

const TintEntry kMudTints_55C744[18] =
{
    { 1, 87u, 103u, 67u },
    { 2, 87u, 103u, 67u },
    { 3, 87u, 103u, 67u },
    { 4, 87u, 103u, 67u },
    { 5, 87u, 103u, 67u },
    { 6, 87u, 103u, 67u },
    { 7, 87u, 103u, 67u },
    { 8, 87u, 103u, 67u },
    { 9, 87u, 103u, 67u },
    { 10, 87u, 103u, 67u },
    { 11, 87u, 103u, 67u },
    { 12, 87u, 103u, 67u },
    { 13, 87u, 103u, 67u },
    { 14, 87u, 103u, 67u },
    { -1, 87u, 103u, 67u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};

using TMudAIStateFunction = decltype(&Mudokon::AI_GiveRings_0_470C10);

const TMudAIStateFunction sMudokon_AI_Table_55CDF0[10] =
{
    &Mudokon::AI_GiveRings_0_470C10,
    &Mudokon::AI_Chisel_1_47C5F0,
    &Mudokon::AI_Scrub_2_47D270,
    &Mudokon::AI_TurnWheel_3_47E0D0,
    &Mudokon::AI_ListeningToAbe_4_477B40,
    &Mudokon::AI_ShrivelDeath_5_4714A0,
    &Mudokon::AI_Escape_6_47A560,
    &Mudokon::AI_FallAndSmackDeath_7_471600,
    &Mudokon::AI_AngryWorker_8_47E910,
    &Mudokon::AI_Sick_9_47A910
};

#define MUD_MOTION_STATES_ENUM(ENTRY) \
    ENTRY(M_Idle_0_4724E0) \
    ENTRY(M_WalkLoop_1_4728B0) \
    ENTRY(M_StandingTurn_2_472BF0) \
    ENTRY(M_Speak_472FA0) \
    ENTRY(M_Speak_472FA0) \
    ENTRY(M_Speak_472FA0) \
    ENTRY(M_Speak_472FA0) \
    ENTRY(M_WalkBegin_7_472AB0) \
    ENTRY(M_WalkToIdle_8_472B30) \
    ENTRY(M_MidWalkToIdle_9_472BD0) \
    ENTRY(M_LeverUse_10_473020) \
    ENTRY(M_Chisel_11_4732D0) \
    ENTRY(M_StartChisel_12_473500) \
    ENTRY(M_StopChisel_13_473530) \
    ENTRY(M_CrouchScrub_14_473560) \
    ENTRY(M_CrouchIdle_15_474040) \
    ENTRY(M_CrouchTurn_16_4740E0) \
    ENTRY(M_StandToCrouch_17_474120) \
    ENTRY(M_CrouchToStand_18_474150) \
    ENTRY(M_WalkToRun_19_473600) \
    ENTRY(M_MidWalkToRun_20_4736D0) \
    ENTRY(M_RunLoop_21_473720) \
    ENTRY(M_RunToWalk_22_4738E0) \
    ENTRY(M_MidRunToWalk_23_4739B0) \
    ENTRY(M_RunSlideStop_24_473A00) \
    ENTRY(M_RunTurn_25_473AA0) \
    ENTRY(M_RunTurnToRun_26_473BB0) \
    ENTRY(M_SneakLoop_27_473C40) \
    ENTRY(M_WalkToSneak_28_473D60) \
    ENTRY(M_SneakToWalk_29_473E20) \
    ENTRY(M_MidWalkToSneak_30_473EE0) \
    ENTRY(M_MidSneakToWalk_31_473F30) \
    ENTRY(M_SneakBegin_32_473F80) \
    ENTRY(M_SneakToIdle_33_473FF0) \
    ENTRY(M_MidSneakToIdle_34_474020) \
    ENTRY(M_JumpBegin_35_474460) \
    ENTRY(M_JumpMid_36_474570) \
    ENTRY(M_WalkToRun_37_4749A0) \
    ENTRY(M_Punch_38_474AA0) \
    ENTRY(M_HoistBegin_39_4748E0) \
    ENTRY(M_HoistLand_40_474780) \
    ENTRY(M_LandSoft1_41_474960) \
    ENTRY(M_LandSoft2_42_4743F0) \
    ENTRY(M_DunnoBegin_43_472790) \
    ENTRY(M_DunnoEnd_44_4727B0) \
    ENTRY(M_KnockForward_45_474180) \
    ENTRY(M_Knockback_46_4742A0) \
    ENTRY(M_KnockbackGetUp_47_474380) \
    ENTRY(M_FallOffEdge_48_4743C0) \
    ENTRY(M_Fall_49_472C60) \
    ENTRY(M_Chant_50_473040) \
    ENTRY(M_ChantEnd_51_4730D0) \
    ENTRY(M_ToDuck_52_474A20) \
    ENTRY(M_Duck_53_474A40) \
    ENTRY(M_DuckToCrouch_54_474A80) \
    ENTRY(M_DuckKnockback_55_474250) \
    ENTRY(M_SlapOwnHead_56_4727D0) \
    ENTRY(M_TurnWheelBegin_57_474C00) \
    ENTRY(M_TurnWheelLoop_58_474CC0) \
    ENTRY(M_TurnWheelEnd_59_474D30)

const char* const sMudMotionStateNames[60] =
{
    MUD_MOTION_STATES_ENUM(MAKE_STRINGS)
};

struct MudEmotionTableEntry
{
    MudSounds field_0_sound;
    Mud_Motion field_2_next_motion;
    Mud_Emotion field_4_emo_tbl;
    __int16 field_6_sub_state;
};

struct MudEmotionTable
{
    MudEmotionTableEntry field_0_data[8][17];
};

const MudEmotionTable kMudEmoTable_55C790 =
{
    {
        {
            // Correlates to MudEmotion::eNormal_0.
            { MudSounds::eHelloNeutral_3,  Mud_Motion::M_Speak_4_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::eOkay_12, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::eOkay_12, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::eOkay_12, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::eNone, Mud_Motion::M_DunnoBegin_43_472790,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eFartPuh_23, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Punch_38_474AA0,             Mud_Emotion::eAggressive_2, 13 },
            { MudSounds::eNone, Mud_Motion::M_DunnoBegin_43_472790,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eLaugh_10, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::eLaugh_10, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::eSadUgh_28, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eNormal_0,  12 }
        },
        {
            // Correlates to MudEmotion::eAngry_1.
            { MudSounds::eHiAngry_18, Mud_Motion::M_Speak_4_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eNoAngry_21, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eAngry_1,   0 },
            { MudSounds::eNoAngry_21, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eNone, Mud_Motion::M_DunnoBegin_43_472790,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Punch_38_474AA0,             Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Punch_38_474AA0,             Mud_Emotion::eAggressive_2, 13 },
            { MudSounds::eOkay_12, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eLaugh_10, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eAngry_1,   0 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eAngry_1,   12 }
        },
        {
            // Correlates to MudEmotion::eAggressive_2.
            { MudSounds::eHiAngry_18, Mud_Motion::M_Speak_4_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eNoAngry_21, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eAngry_1,   0 },
            { MudSounds::eNoAngry_21, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Punch_38_474AA0,             Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Punch_38_474AA0,             Mud_Emotion::eAggressive_2, 13 },
            { MudSounds::eOkay_12, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eLaugh_10, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eAngry_1,   0 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eAngry_1,   12 }
        },
        {
            // Correlates to MudEmotion::eSad_3;
            { MudSounds::eHiSad_20, Mud_Motion::M_Speak_4_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSad_3,     0 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNone, Mud_Motion::M_DunnoBegin_43_472790,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSuicidal_4, 13 },
            { MudSounds::eOkay_12, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eNormal_0,  0 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSuicidal_4, 12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSuicidal_4, 12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSuicidal_4, 12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 }
        },
        {
            // Correlates to MudEmotion::eSuicidal_4.
            { MudSounds::eHiSad_20, Mud_Motion::M_Speak_4_472FA0,     Mud_Emotion::eSuicidal_4, 12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSuicidal_4, 12 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSuicidal_4, 0 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSuicidal_4, 12 },
            { MudSounds::eSadUgh_28, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSuicidal_4, 12 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSuicidal_4, 0 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSuicidal_4, 13 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eSuicidal_4, 12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSuicidal_4, 12 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eNormal_0,  0 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSuicidal_4, 12 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSuicidal_4, 0 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSuicidal_4, 0 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSuicidal_4, 0 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSuicidal_4, 0 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSuicidal_4, 0 }
        },
        {
            // Correlates to MudEmotion::eHappy_5.
            { MudSounds::eHiHappy_19, Mud_Motion::M_Speak_4_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eLaugh_10, Mud_Motion::M_HoistBegin_39_4748E0, Mud_Emotion::eWired_6,   12 },
            { MudSounds::eOkay_12, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eOkay_12, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eNuhUh_13, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eLaugh_10, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eOkay_12, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eNormal_0,  13 },
            { MudSounds::eNone, Mud_Motion::M_DunnoBegin_43_472790,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eWired_6,   0 },
            { MudSounds::eLaugh_10, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eLaugh_10, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eLaugh_10, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eLaugh_10, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eWired_6,   12 }
        },
        {
            // Correlates to MudEmotion::eWired_6.
            { MudSounds::eHiHappy_19, Mud_Motion::M_Speak_4_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eLaugh_10, Mud_Motion::M_HoistBegin_39_4748E0, Mud_Emotion::eWired_6,   12 },
            { MudSounds::eOkay_12, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eOkay_12, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eNuhUh_13, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eAnger_5,  Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eLaugh_10, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eOkay_12, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eNormal_0,  13 },
            { MudSounds::eNone, Mud_Motion::M_DunnoBegin_43_472790,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eWired_6,   0 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eWired_6,   0 },
            { MudSounds::eLaugh_10, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eLaugh_10, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eLaugh_10, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNoSad_22, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eLaugh_10, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eWired_6,   12 }
        },
        {
            // Correlates to MudEmotion::eSick_7.
            { MudSounds::eSick_24, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSick_7, 12 },
            { MudSounds::eSick_24, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSick_7, 12 },
            { MudSounds::eSick_24, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eSick_7, 12 },
            { MudSounds::eSick_24, Mud_Motion::M_Speak_3_472FA0,     Mud_Emotion::eSick_7, 12 },
            { MudSounds::eSick_24, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSick_7, 12 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eSadUgh_28, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSick_7, 13 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eNone, Mud_Motion::M_Idle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eSick_24, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSick_7, 12 },
            { MudSounds::eSick_24, Mud_Motion::M_Speak_6_472FA0,     Mud_Emotion::eSick_7, 12 }
        }
    }
};



using TMudStateFunction = decltype(&Mudokon::M_Idle_0_4724E0);

const TMudStateFunction sMudokon_motion_states_55CE18[60] =
{
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
    &Mudokon::M_RunTurn_25_473AA0,
    &Mudokon::M_RunTurnToRun_26_473BB0,
    &Mudokon::M_SneakLoop_27_473C40,
    &Mudokon::M_WalkToSneak_28_473D60,
    &Mudokon::M_SneakToWalk_29_473E20,
    &Mudokon::M_MidWalkToSneak_30_473EE0,
    &Mudokon::M_MidSneakToWalk_31_473F30,
    &Mudokon::M_SneakBegin_32_473F80,
    &Mudokon::M_SneakToIdle_33_473FF0,
    &Mudokon::M_MidSneakToIdle_34_474020,
    &Mudokon::M_JumpBegin_35_474460,
    &Mudokon::M_JumpMid_36_474570,
    &Mudokon::M_WalkToRun_37_4749A0,
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
    &Mudokon::M_FallOffEdge_48_4743C0,
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
    &Mudokon::M_TurnWheelEnd_59_474D30
};


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
        LOG_ERROR("Don't know about emo type " << static_cast<int>(emo));
        ALIVE_FATAL("Unknown emo");
    }
}

Mudokon* Mudokon::ctor_474F30(Path_Mudokon* pTlv, int tlvInfo)
{
    ctor_408240(18);

    field_154_unused = 0;
    field_140_last_event_index = -1;
    field_156_unused = -1;

    SetVTable(this, 0x5462E4);

    field_11C_bird_portal_id = -1;
    field_12C_unused = -1;
    field_158_wheel_id = -1;
    field_4_typeId = Types::eMudokon2_81; // TODO: Set to 110 later, what is 81 ??
    field_C_objectId = tlvInfo;
    field_194_timer = 0;
    field_18E_ai_state = Mud_AI_State::AI_GiveRings_0_470C10;
    field_190_sub_state = 0;
    field_108_next_motion = -1;
    field_192_reset_to_previous_motion = 0;
    field_13C_voice_pitch = 0;

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, TRUE, FALSE));
    field_10_resources_array.SetAt(1, nullptr);
    
    const AnimRecord& rec = AnimRec(AnimId::Mudokon_Idle);
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_DC_bApplyShadows |= 2u;

    field_114_flags.Clear(Flags_114::e114_Bit9);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_18C_unused = 0;
    SetTint_425600(kMudTints_55C744, field_C2_lvl_number);

    field_17E_delayed_speak = MudAction::eNone_17;

    field_16A_flags.Clear(Flags::eBit2_save_state);
    field_16A_flags.Clear(Flags::eBit3_alerted);
    field_16A_flags.Set(Flags::eBit4_blind, pTlv->field_22_bBlind & 1);
    field_16A_flags.Clear(Flags::eBit5_following);
    field_16A_flags.Clear(Flags::eBit6_standing_for_sad_or_angry);
    field_16A_flags.Clear(Flags::eBit7_stopped_at_wheel);
    field_16A_flags.Clear(Flags::eBit8_do_angry);
    field_16A_flags.Clear(Flags::eBit9_seen_while_sick);
    // 10 and 11 ??
    field_16A_flags.Set(Flags::eBit12_alert_enemies);
    field_16A_flags.Clear(Flags::eBit13);
    field_16A_flags.Clear(Flags::eBit14_make_sad_noise);

    field_180_emo_tbl = TLV_Emo_To_Internal_Emo(pTlv->field_20_emotion);
    field_188_pTblEntry = nullptr;
    field_182 = GameSpeakEvents::eNone_m1;
    field_120_angry_trigger = pTlv->field_24_angry_trigger;
    field_16C &= ~6u;
    field_198_turning_wheel_timer = 0;

    switch (pTlv->field_12_state)
    {
    case Mud_State::eChisle_0:
        field_18E_ai_state = Mud_AI_State::AI_Chisel_1_47C5F0;
        field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMudchslResID, TRUE, FALSE));
        break;

    case Mud_State::eScrub_1:
        field_18E_ai_state = Mud_AI_State::AI_Scrub_2_47D270;
        field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMudscrubResID, TRUE, FALSE));
        break;

    case Mud_State::eAngryWorker_2:
        field_18E_ai_state = Mud_AI_State::AI_AngryWorker_8_47E910;
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
        field_164_ring_timeout = pTlv->field_2A_ring_timeout;
        field_16A_flags.Set(Flags::eBit16_instant_power_up, pTlv->field_2C_bInstant_power_up & 1);
        field_16C &= ~1;
        field_18E_ai_state = Mud_AI_State::AI_GiveRings_0_470C10;
        field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeommResID, TRUE, FALSE));
        break;

    default:
        break;
    }

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        field_18E_ai_state = Mud_AI_State::AI_ListeningToAbe_4_477B40;
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

    if (field_16A_flags.Get(Flags::eBit4_blind))
    {
        SetPal_4772D0(Mud_Emotion::eNormal_0);
    }

    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_14_direction == Mud_Direction::eLeft_0);

    field_4_typeId = Types::eMudokon_110;

    field_13C_voice_pitch = pTlv->field_16_voice_pitch;
    field_17A_rescue_id = pTlv->field_18_rescue_id;

    field_16A_flags.Set(Flags::eBit2_save_state, pTlv->field_1E_save_state & 1);
    field_16A_flags.Set(Flags::eBit10_stop_trigger, pTlv->field_26_stop_trigger & 1);
    field_16A_flags.Set(Flags::eBit11_get_depressed, pTlv->field_28_bGets_depressed & 1);
    field_16A_flags.Set(Flags::eBit15_ring_timeout, pTlv->field_2A_ring_timeout & 1);

    field_17C_stand_idle_timer = 0;

    vStackOnObjectsOfType_425840(Types::eMudokon_110);

    if (field_180_emo_tbl == Mud_Emotion::eSick_7)
    {
        field_18E_ai_state = Mud_AI_State::AI_Sick_9_47A910;
        field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
        field_106_current_motion = Mud_Motion::M_CrouchIdle_15_474040;
    }
    else
    {
        field_106_current_motion = Mud_Motion::M_Idle_0_4724E0;
    }

    if (pTlv->field_10_scale == TLV_Scale::Half_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 9;
        field_D6_scale = 0;
    }
    else if (pTlv->field_10_scale == TLV_Scale::Full_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 28;
        field_D6_scale = 1;
    }

    FP hitX = {};
    FP hitY = {};
    const auto bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
        FP_FromInteger(pTlv->field_8_top_left.field_0_x),
        FP_FromInteger(pTlv->field_8_top_left.field_2_y),
        FP_FromInteger(pTlv->field_C_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_C_bottom_right.field_2_y),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16);

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    if (bCollision)
    {
        field_BC_ypos = hitY;
        if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
        {
            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);
            vOnCollisionWith_424EE0(
                { bRect.x, static_cast<short>(bRect.y + 5) },
                { bRect.w, static_cast<short>(bRect.h + 5) },
                ObjList_5C1B78,
                1,
                (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
        }
    }

    field_20_animation.field_1C_fn_ptr_array = kAbe_Anim_Frame_Fns_55EF98;
    field_F8_LastLineYPos = field_BC_ypos;

    if (field_18E_ai_state == Mud_AI_State::AI_Scrub_2_47D270 || field_18E_ai_state == Mud_AI_State::AI_Chisel_1_47C5F0)
    {
        MapFollowMe_408D10(TRUE);
    }

    field_16A_flags.Set(Flags::eBit1_not_rescued);
    field_190_sub_state = 0;
    field_118_tlvInfo = field_C_objectId;

    field_128_angry_timer = 0;
    field_160_delayed_speak = MudSounds::eNone;
    field_162_maxXOffset = field_DA_xOffset;
    
    field_E0_pShadow = ae_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    vUpdate_4757A0();

    return this;
}

void Mudokon::VUpdate()
{
    vUpdate_4757A0();
}

void Mudokon::VScreenChanged()
{
    vScreenChanged_476F30();
}


const AnimId kMudFrameTableOffsets_55CD00[60] =
{
    AnimId::Mudokon_Idle,    AnimId::Mudokon_Walk,    AnimId::Mudokon_Turn_Around,    AnimId::Mudokon_Talk_A,    AnimId::Mudokon_Talk_B,
    AnimId::Mudokon_Talk_C,    AnimId::Mudokon_Talk_D,    AnimId::Mudokon_Unknown_A,    AnimId::Mudokon_Unknown_B,    AnimId::Mudokon_Unknown_C,
    AnimId::Mudokon_Lever_Pull,    AnimId::Mudokon_Chisel_Mining,     AnimId::Mudokon_Unknown_E,     AnimId::Mudokon_Unknown_F,     AnimId::Mudokon_Scrub_Cleaning,
    AnimId::Mudokon_Crouch,   AnimId::Mudokon_Crouch_Turn_Around,   AnimId::Mudokon_Stand_To_Crouch,   AnimId::Mudokon_Crouch_To_Stand,   AnimId::Mudokon_Unknown_G,
    AnimId::Mudokon_Unknown_H,   AnimId::Mudokon_Run,   AnimId::Mudokon_Unknown_I,   AnimId::Mudokon_Unknown_J,   AnimId::Mudokon_Run_End_A,
    AnimId::Mudokon_Run_End_B,   AnimId::Mudokon_Unknown_K,   AnimId::Mudokon_Sneak,   AnimId::Mudokon_Sneak_Start,   AnimId::Mudokon_Unknown_L,
    AnimId::Mudokon_Unknown_M,   AnimId::Mudokon_Unknown_N,   AnimId::Mudokon_Unknown_O,   AnimId::Mudokon_Unknown_P,   AnimId::Mudokon_Unknown_Q,
    AnimId::Mudokon_Run_Jump_Start,   AnimId::Mudokon_Run_Jump,   AnimId::Mudokon_Unknown_R,   AnimId::Mudokon_Slap,   AnimId::Mudokon_Hoist_Start,
    AnimId::Mudokon_Hoist,   AnimId::Mudokon_Hoist_End,   AnimId::Mudokon_Hoist_End,   AnimId::Mudokon_Shrug_Start,   AnimId::Mudokon_Shrug_End, // duplicates: 269928
    AnimId::Mudokon_Knocked_Back_Face_Down,    AnimId::Mudokon_Knocked_Back_Face_Up,    AnimId::Mudokon_Get_Up,    AnimId::Mudokon_Fall,    AnimId::Mudokon_Unknown_S,
    AnimId::Mudokon_Chant,     AnimId::Mudokon_Chant_End,    AnimId::Mudokon_Crouch_To_Duck,     AnimId::Mudokon_Duck,     AnimId::Mudokon_Duck_To_Crouch,
    AnimId::Mudokon_Unknown_T,     AnimId::Mudokon_Hit_Self,     AnimId::Mudokon_Wheel_Start,    AnimId::Mudokon_Wheel,    AnimId::Mudokon_Wheel_End
};

ALIVE_VAR(1, 0x5C3010, short, sAlertedMudCount_5C3010, 0);
ALIVE_VAR(1, 0x5C3018, short, word_5C3018, 0);


int CC Mudokon::CreateFromSaveState_4717C0(const BYTE* pBuffer)
{
    auto pState = reinterpret_cast<const Mudokon_State*>(pBuffer);

    auto pTlv = static_cast<Path_Mudokon*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_40_tlvInfo));
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

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kRockShadowResID, FALSE, FALSE))
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

    if (pState->field_24_current_motion == Mud_Motion::M_LeverUse_10_473020 && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbepullResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEPULL.BAN", nullptr);
    }

    if (pState->field_24_current_motion == Mud_Motion::M_TurnWheelBegin_57_474C00 && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEWORK.BAN", nullptr);
    }

    if (pState->field_24_current_motion == Mud_Motion::M_TurnWheelLoop_58_474CC0 && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEWORK.BAN", nullptr);
    }

    if (pState->field_24_current_motion == Mud_Motion::M_TurnWheelEnd_59_474D30 && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEWORK.BAN", nullptr);
    }

    auto pMud = ae_new<Mudokon>();
    pMud->ctor_474F30(pTlv, pState->field_40_tlvInfo);

    if (pState->field_3D_bIsPlayer)
    {
        sControlledCharacter_5C1B8C = pMud;
    }

    pMud->field_FC_pPathTLV = nullptr;
    pMud->field_100_pCollisionLine = nullptr;

    pMud->field_B8_xpos = pState->field_4_xpos;
    pMud->field_BC_ypos = pState->field_8_ypos;
    pMud->field_C4_velx = pState->field_C_velx;
    pMud->field_C8_vely = pState->field_10_vely;

    pMud->field_134_xVelSlowBy = pState->field_44_velx_slow_by;
    pMud->field_138_unused = pState->field_48_unused;

    pMud->field_C0_path_number = pState->field_14_path_number;
    pMud->field_C2_lvl_number = pState->field_16_lvl_number;
    pMud->field_CC_sprite_scale = pState->field_18_sprite_scale;

    pMud->field_D0_r = pState->field_1C_r;
    pMud->field_D2_g = pState->field_1E_g;
    pMud->field_D4_b = pState->field_20_b;

    pMud->field_106_current_motion = pState->field_24_current_motion;

    const AnimRecord& animRec = AnimRec(kMudFrameTableOffsets_55CD00[pMud->field_106_current_motion]);
    //BYTE** ppRes = pMud->AnimBlockForMotion_474DC0(static_cast<short>(animRec.mResourceId));
    BYTE** ppRes = pMud->AnimBlockForMotion_474DC0(pState->field_24_current_motion);
    
    pMud->field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, ppRes);

    pMud->field_20_animation.field_92_current_frame = pState->field_26_anim_current_frame;
    pMud->field_20_animation.field_E_frame_change_counter = pState->field_28_anim_frame_change_counter;

    pMud->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pState->field_22_bFlipX & 1);
    pMud->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_2A_bAnimRender & 1);

    pMud->field_6_flags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2B_bDrawable & 1);

    if (IsLastFrame(&pMud->field_20_animation))
    {
        pMud->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pMud->field_10C_health = pState->field_2C_health;
    pMud->field_106_current_motion = pState->field_30_current_motion;
    pMud->field_108_next_motion = pState->field_32_next_motion;
    pMud->field_F8_LastLineYPos = FP_FromInteger(pState->field_34_lastLineYPos);

    pMud->field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed, pState->field_3C_can_be_possessed & 1);
    pMud->field_114_flags.Set(Flags_114::e114_Bit9);

    pMud->field_104_collision_line_type = pState->field_36_line_type;
    pMud->field_11C_bird_portal_id = pState->field_4C_portal_id;
    pMud->field_120_angry_trigger = pState->field_50_angry_trigger;
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

    pMud->field_16A_flags.Set(Flags::eBit1_not_rescued, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit4_not_rescued));
    pMud->field_16A_flags.Set(Flags::eBit2_save_state, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit5_save_state));
    pMud->field_16A_flags.Set(Flags::eBit3_alerted, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit6_alerted));
    pMud->field_16A_flags.Set(Flags::eBit4_blind, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit7_blind));
    pMud->field_16A_flags.Set(Flags::eBit5_following, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit8_following));
    pMud->field_16A_flags.Set(Flags::eBit6_standing_for_sad_or_angry, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit9_standing_for_sad_or_angry));
    pMud->field_16A_flags.Set(Flags::eBit7_stopped_at_wheel, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit10_stopped_at_wheel));
    pMud->field_16A_flags.Set(Flags::eBit8_do_angry, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit11_do_angry));
    pMud->field_16A_flags.Set(Flags::eBit9_seen_while_sick, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit12_seen_while_sick));
    pMud->field_16A_flags.Set(Flags::eBit10_stop_trigger, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit13_stop_trigger));
    
    pMud->field_18C_unused = pState->field_6C.Get(Mudokon_State::Flags_6A::eBit14_unused);
    pMud->field_192_reset_to_previous_motion = pState->field_6C.Get(Mudokon_State::Flags_6A::eBit15_reset_to_previous_motion);

    pMud->field_16A_flags.Set(Flags::eBit11_get_depressed, pState->field_6C.Get(Mudokon_State::Flags_6A::eBit16_get_depressed));

    pMud->field_16A_flags.Set(Flags::eBit12_alert_enemies, pState->field_6E.Get(Mudokon_State::Flags_6E::e6E_Bit1_alert_enemies));
    pMud->field_16A_flags.Set(Flags::eBit13, pState->field_6E.Get(Mudokon_State::Flags_6E::e6E_Bit2));
    pMud->field_16A_flags.Set(Flags::eBit14_make_sad_noise, pState->field_6E.Get(Mudokon_State::Flags_6E::e6E_Bit3_make_sad_noise));
    pMud->field_16A_flags.Set(Flags::eBit15_ring_timeout, pState->field_6E.Get(Mudokon_State::Flags_6E::e6E_Bit4_ring_timeout));

    if (pState->field_6E.Get(Mudokon_State::Flags_6E::e6E_Bit5))
    {
        pMud->field_16C |= 1;
    }
    else
    {
        pMud->field_16C &= ~1;
    }

    if (pState->field_6E.Get(Mudokon_State::Flags_6E::e6E_Bit6))
    {
        pMud->field_16C |= 2;
    }
    else
    {
        pMud->field_16C &= ~2;
    }

    if (pMud->field_16A_flags.Get(Flags::eBit3_alerted))
    {
        sAlertedMudCount_5C3010++;
    }

    pMud->field_178_sub_state2 = pState->field_70_sub_state2;
    pMud->field_17C_stand_idle_timer = pState->field_72_stand_idle_timer;
    pMud->field_17E_delayed_speak = pState->field_74_delayed_speak;
    pMud->field_180_emo_tbl = pState->field_76_emo_tlb;
    pMud->field_182 = pState->field_78;
    pMud->field_184_next_motion2 = pState->field_7A_motion;
    pMud->field_18E_ai_state = pState->field_7C_ai_state;
    pMud->field_190_sub_state = pState->field_7E_sub_state;
    pMud->field_194_timer = pState->field_80_timer;

    if (pState->field_6E.Get(Mudokon_State::Flags_6E::e6E_Bit6))
    {
        word_5C3018 = 1;
    }

    pMud->field_188_pTblEntry = GetResponseEntry_471790(pState->field_84_response_entry_idx);
    return sizeof(Mudokon_State);
}

BaseGameObject* Mudokon::VDestructor(signed int flags)
{
    return vdtor_475770(flags);
}

int Mudokon::vGetSaveState_47B080(Mudokon_State* pState)
{
    if (field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    pState->field_0_type = Types::eMudokon2_81;

    pState->field_4_xpos = field_B8_xpos;
    pState->field_8_ypos = field_BC_ypos;
    pState->field_C_velx = field_C4_velx;
    pState->field_10_vely = field_C8_vely;

    pState->field_44_velx_slow_by = field_134_xVelSlowBy;
    pState->field_48_unused = field_138_unused;

    pState->field_14_path_number = field_C0_path_number;
    pState->field_16_lvl_number = field_C2_lvl_number;
    pState->field_18_sprite_scale = field_CC_sprite_scale;

    pState->field_1C_r = field_D0_r;
    pState->field_1E_g = field_D2_g;
    pState->field_20_b = field_D4_b;

    pState->field_22_bFlipX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pState->field_24_current_motion = field_106_current_motion;
    pState->field_26_anim_current_frame = field_20_animation.field_92_current_frame;
    pState->field_28_anim_frame_change_counter = field_20_animation.field_E_frame_change_counter;
    pState->field_2B_bDrawable = field_6_flags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2A_bAnimRender = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);
    pState->field_2C_health = field_10C_health;
    pState->field_30_current_motion = field_106_current_motion;
    pState->field_32_next_motion = field_108_next_motion;
    pState->field_34_lastLineYPos = FP_GetExponent(field_F8_LastLineYPos);
    pState->field_36_line_type = -1;

    pState->field_3C_can_be_possessed = field_114_flags.Get(Flags_114::e114_Bit3_Can_Be_Possessed);

    if (field_100_pCollisionLine)
    {
        pState->field_36_line_type = field_100_pCollisionLine->field_8_type;
    }

    pState->field_3D_bIsPlayer = this == sControlledCharacter_5C1B8C;
    pState->field_40_tlvInfo = field_118_tlvInfo;
    pState->field_4C_portal_id = -1;

    if (field_11C_bird_portal_id != -1)
    {
        BaseGameObject* pBirdPortal = sObjectIds_5C1B70.Find_449CF0(field_11C_bird_portal_id);
        if (pBirdPortal)
        {
            pState->field_4C_portal_id = pBirdPortal->field_C_objectId;
        }
    }

    pState->field_50_angry_trigger = field_120_angry_trigger;
    pState->field_54_savedfield124 = field_124;
    pState->field_58_angry_timer = field_128_angry_timer;
    pState->field_5C_unused = field_130_unused;
    pState->field_5E_voice_pitch = field_13C_voice_pitch;
    pState->field_60_wheel_id = -1;

    if (field_158_wheel_id != -1)
    {
        BaseGameObject* pWheel = sObjectIds_5C1B70.Find_449CF0(field_158_wheel_id);
        if (pWheel)
        {
            pState->field_60_wheel_id = pWheel->field_C_objectId;
        }
    }

    pState->field_64_unused = field_15C_unused;
    pState->field_68 = field_160_delayed_speak;
    pState->field_6A_maxXOffset = field_162_maxXOffset;

    // bit1 never used ??
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit2_unused, field_12C_unused != -1);
    // bit3 never used ??
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit4_not_rescued, field_16A_flags.Get(Flags::eBit1_not_rescued));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit5_save_state, field_16A_flags.Get(Flags::eBit2_save_state));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit6_alerted, field_16A_flags.Get(Flags::eBit3_alerted));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit7_blind, field_16A_flags.Get(Flags::eBit4_blind));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit8_following, field_16A_flags.Get(Flags::eBit5_following));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit9_standing_for_sad_or_angry, field_16A_flags.Get(Flags::eBit6_standing_for_sad_or_angry));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit10_stopped_at_wheel, field_16A_flags.Get(Flags::eBit7_stopped_at_wheel));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit11_do_angry, field_16A_flags.Get(Flags::eBit8_do_angry));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit12_seen_while_sick, field_16A_flags.Get(Flags::eBit9_seen_while_sick));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit13_stop_trigger, field_16A_flags.Get(Flags::eBit10_stop_trigger));
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit14_unused, field_18C_unused & 1);
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit15_reset_to_previous_motion, field_192_reset_to_previous_motion & 1);
    pState->field_6C.Set(Mudokon_State::Flags_6A::eBit16_get_depressed, field_16A_flags.Get(Flags::eBit11_get_depressed));

    pState->field_6E.Set(Mudokon_State::Flags_6E::e6E_Bit1_alert_enemies, field_16A_flags.Get(Flags::eBit12_alert_enemies));
    pState->field_6E.Set(Mudokon_State::Flags_6E::e6E_Bit2, field_16A_flags.Get(Flags::eBit13));
    pState->field_6E.Set(Mudokon_State::Flags_6E::e6E_Bit3_make_sad_noise, field_16A_flags.Get(Flags::eBit14_make_sad_noise));
    pState->field_6E.Set(Mudokon_State::Flags_6E::e6E_Bit4_ring_timeout, field_16A_flags.Get(Flags::eBit15_ring_timeout));
    pState->field_6E.Set(Mudokon_State::Flags_6E::e6E_Bit5, field_16C & 1);
    pState->field_6E.Set(Mudokon_State::Flags_6E::e6E_Bit6, field_16C & 2);

    pState->field_70_sub_state2 = field_178_sub_state2;
    pState->field_72_stand_idle_timer = field_17C_stand_idle_timer;
    pState->field_74_delayed_speak = field_17E_delayed_speak;
    pState->field_76_emo_tlb = field_180_emo_tbl;
    pState->field_78 = field_182;
    pState->field_7A_motion = field_184_next_motion2;
    pState->field_7C_ai_state = field_18E_ai_state;
    pState->field_7E_sub_state = field_190_sub_state;
    pState->field_80_timer = field_194_timer;

    pState->field_84_response_entry_idx = GetResponseEntryIdx_471760();

    return sizeof(Mudokon_State);
}

void Mudokon::vUpdate_4757A0()
{
    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9);
        if (field_104_collision_line_type != -1)
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(20),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(20),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104_collision_line_type);

            if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
            {
                PSX_RECT bRect = {};
                vGetBoundingRect_424FD0(&bRect, 1);

                vOnCollisionWith_424EE0(
                    { bRect.x, static_cast<short>(bRect.y + 5) },
                    { bRect.w, static_cast<short>(bRect.h + 5) },
                    ObjList_5C1B78,
                    1,
                    (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
            }
        }

        field_104_collision_line_type = 0;

        if (field_11C_bird_portal_id != -1)
        {
            for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_C_objectId == field_11C_bird_portal_id)
                {
                    field_11C_bird_portal_id = pObj->field_8_object_id;
                    word_5C3012++;
                    field_16C |= 4u;
                    if (field_18E_ai_state == Mud_AI_State::AI_Escape_6_47A560 && field_190_sub_state == 3)
                    {
                        static_cast<BirdPortal*>(pObj)->Vsub_499430(1);
                        field_20_animation.field_C_render_layer = field_CC_sprite_scale != FP_FromInteger(1) ? 11 : 30;
                    }
                    break;
                }
            }
        }

        if (field_158_wheel_id != -1)
        {
            for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_C_objectId == field_158_wheel_id)
                {
                    field_158_wheel_id = pObj->field_8_object_id;
                    static_cast<WorkWheel*>(pObj)->VStartTurning();
                    break;
                }
            }
        }
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return;
    }

    const FP xDistFromPlayer = FP_Abs(field_B8_xpos - sControlledCharacter_5C1B8C->field_B8_xpos);
    if (xDistFromPlayer > FP_FromInteger(750))
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        return;
    }

    const FP yDistanceFromPlayer = FP_Abs(field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos);
    if (yDistanceFromPlayer > FP_FromInteger(520))
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        return;
    }

    if (field_10C_health > FP_FromInteger(0))
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
        field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
    }

    //DDCheat::DebugStr_4F5560("\nMotion = %s BrainState = %s\n", sMudMotionStateNames[field_106_current_motion], sMudAiStateNames[static_cast<int>(field_18E_ai_state)]);
    //LOG_INFO(sMudMotionStateNames[field_106_current_motion] << " " << static_cast<int>(field_18E_ai_state));
    //LOG_INFO(sMudMotionStateNames[field_106_current_motion] << " " << sMudAiStateNames[static_cast<int>(field_18E_ai_state)]);

    const __int16 oldMotion = field_106_current_motion;

    field_190_sub_state = (this->*sMudokon_AI_Table_55CDF0[field_18E_ai_state])();
   
    const FP oldXPos = field_B8_xpos;
    const FP oldYPos = field_BC_ypos;

    (this->*sMudokon_motion_states_55CE18[field_106_current_motion])();

    if (oldXPos != field_B8_xpos || oldYPos != field_BC_ypos)
    {
        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            nullptr,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);
        VOn_TLV_Collision_4087F0(field_FC_pPathTLV);
    }

    if (oldMotion != field_106_current_motion || field_114_flags.Get(Flags_114::e114_MotionChanged_Bit2))
    {
        field_114_flags.Clear(Flags_114::e114_MotionChanged_Bit2);
        vUpdateAnimRes_474D80();
    }
    else if (field_192_reset_to_previous_motion)
    {
        field_106_current_motion = field_F4_previous_motion;
        vUpdateAnimRes_474D80();
        field_20_animation.SetFrame_409D50(field_F6_anim_frame);
        field_192_reset_to_previous_motion = 0;
    }
}


void Mudokon::SetPal_4772D0(Mud_Emotion emotion)
{
    switch (emotion)
    {
    case Mud_Emotion::eNormal_0:
        if (field_16A_flags.Get(Flags::eBit4_blind))
        {
            SetRGB(63, 63, 63);
            field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(16), 0);
        }
        else
        {
            SetRGB(87, 103, 67);
            FrameInfoHeader* pFrameInfoHeader = field_20_animation.Get_FrameHeader_40B730(-1);
            FrameHeader* pHeader = reinterpret_cast<FrameHeader*>(&(*field_20_animation.field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);
            field_20_animation.Load_Pal_40A530(field_20_animation.field_20_ppBlock, pHeader->field_0_clut_offset);
        }
        break;

    case Mud_Emotion::eAngry_1:
    case Mud_Emotion::eAggressive_2:
        SetRGB(63, 63, 63);
        field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(13), 0);
        break;

    case Mud_Emotion::eSad_3:
    case Mud_Emotion::eSuicidal_4:
        SetRGB(63, 63, 63);
        field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(14), 0);
        break;

    case Mud_Emotion::eHappy_5:
    case Mud_Emotion::eWired_6:
        SetRGB(74, 74, 74);
        field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(15), 0);
        break;

    case Mud_Emotion::eSick_7:
        SetRGB(63, 63, 63);
        field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(17), 0);
        break;
    }
}

void Mudokon::vOnTrapDoorOpen_472350()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        if (!field_114_flags.Get(Flags_114::e114_Bit1_bShot))
        {
            VSetMotion_4081C0(Mud_Motion::M_FallOffEdge_48_4743C0);
        }

        pPlatform->VRemove(this);
        field_110_id = -1;
    }
}

void Mudokon::vOnTlvCollision_476EA0(Path_TLV* pTlv)
{
    Path_TLV* pTlvIter = pTlv;
    while (pTlvIter)
    {
        if (pTlvIter->field_4_type == TlvTypes::DeathDrop_4)
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                field_18E_ai_state = Mud_AI_State::AI_FallAndSmackDeath_7_471600;
                field_190_sub_state = 0;
                field_10C_health = FP_FromInteger(0);
                Event_Broadcast_422BC0(kEventMudokonDied, this);
                break;
            }
        }

        pTlvIter = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            pTlvIter,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);
    }
}

short Mudokon::FacingTarget_473140(BirdPortal* pTarget)
{
    if (pTarget->field_2C_xpos == field_B8_xpos)
    {
        return TRUE;
    }
    else if (pTarget->field_2C_xpos > field_B8_xpos && !field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }
    else if (pTarget->field_2C_xpos < field_B8_xpos && field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }
    return FALSE;
}

Mudokon* Mudokon::vdtor_475770(signed int flags)
{
    Mudokon::dtor_475B60();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Mudokon::dtor_475B60()
{
    SetVTable(this, 0x5462E4);

    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds_5C1B70.Find_449CF0(field_11C_bird_portal_id));
    if (field_10C_health <= FP_FromInteger(0))
    {
        // TODO: Refactor all access to helpers
        sKilledMudokons_5C1BC0++;
        if (sStatsSignCurrentArea_5C1A20 < ALIVE_COUNTOF(sSavedKilledMudsPerPath_5C1B50.mData))
        {
            sSavedKilledMudsPerPath_5C1B50.mData[sStatsSignCurrentArea_5C1A20]++;
        }
    }

    auto pWheel = static_cast<WorkWheel*>(sObjectIds_5C1B70.Find_449CF0(field_158_wheel_id));
    if (pWheel)
    {
        pWheel->VStopTurning(TRUE);
        field_158_wheel_id = -1;
    }

    RemoveAlerted();

    if (field_16C & 4)
    {
        word_5C3012--;
        field_16C &= ~4u;
        if (!word_5C3012)
        {
            if (pBirdPortal)
            {
                pBirdPortal->Vsub_499610();
                pBirdPortal->VGiveShrukul_499680(TRUE);
            }
        }
        field_11C_bird_portal_id = -1;
    }

    if (!(field_16A_flags.Get(Flags::eBit1_not_rescued)) || field_10C_health <= FP_FromInteger(0) || field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    }

    if (!field_18E_ai_state && field_190_sub_state > 4u)
    {
        if (field_164_ring_timeout > 0)
        {
            sActiveHero_5C1B68->field_168_ring_pulse_timer = sGnFrame_5C1B84 + field_164_ring_timeout;
        }
        else
        {
            sActiveHero_5C1B68->field_168_ring_pulse_timer = sGnFrame_5C1B84 + 200000;
        }

        sActiveHero_5C1B68->field_16C_bHaveShrykull = FALSE;

        if (field_168_ring_type == RingTypes::eHealing_Emit_Effect_11)
        {
            sActiveHero_5C1B68->field_1AC_flags.Set(Abe::e1AC_eBit15_bHaveHealing);
        }
    }

    if (field_106_current_motion == Mud_Motion::M_Chant_50_473040)
    {
        SND_SEQ_Stop_4CAE60(SeqId::MudokonChant_11);
    }

    dtor_4080B0();
}

void Mudokon::vScreenChanged_476F30()
{
    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds_5C1B70.Find_449CF0(field_11C_bird_portal_id));
    BaseGameObject::VScreenChanged();

    if (field_18E_ai_state == Mud_AI_State::AI_GiveRings_0_470C10 || !(field_16A_flags.Get(Flags::eBit2_save_state)))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        // TODO: Duplicated in dtors + other places
        if (field_16C & 4)
        {
            word_5C3012--;
            field_16C &= ~4u;
            if (!word_5C3012)
            {
                if (pBirdPortal)
                {
                    pBirdPortal->Vsub_499610();
                    pBirdPortal->VGiveShrukul_499680(TRUE);
                }
            }
            field_11C_bird_portal_id = -1;
        }
    }
}

void Mudokon::vPossessed_4774F0()
{
    sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
    if (field_180_emo_tbl == Mud_Emotion::eSick_7 && 
        !FindObjectOfType_425180(Types::eTorturedMud_141, field_B8_xpos, field_BC_ypos - FP_FromInteger(50)))
    {
        field_180_emo_tbl = Mud_Emotion::eNormal_0;
        if (field_188_pTblEntry)
        {
            field_188_pTblEntry = Mudokon::ResponseTo_471730(Mud_Emotion::eNormal_0, MudAction::eHelloOrAllYa_0);
        }
    }
}

__int16 Mudokon::vTakeDamage_476270(BaseGameObject* pFrom)
{
    switch (pFrom->field_4_typeId)
    {
    case Types::eBullet_15:
    {
        field_114_flags.Set(Flags_114::e114_Bit1_bShot);
        if (field_10C_health <= FP_FromInteger(0))
        {
            return 1;
        }

        auto pBullet = static_cast<Bullet*>(pFrom);
        switch (pBullet->field_20_type)
        {
        case BulletType::Type_0:
        case BulletType::Type_2:
        {
            auto pBloodFromShot = ae_new<Blood>();
            if (pBloodFromShot)
            {
                pBloodFromShot->ctor_40F0B0(
                    field_B8_xpos,
                    field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale),
                    pBullet->field_30 <= FP_FromInteger(0) ? FP_FromInteger(-24) : FP_FromInteger(24),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    50);
            }
            SetPal_4772D0(Mud_Emotion::eNormal_0);
            break;
        }

        default:
            break;
        }

        if (pBullet->field_20_type == BulletType::Type_1 || pBullet->field_20_type == BulletType::ZBullet_3)
        {
            PSX_RECT v11 = {};
            vGetBoundingRect_424FD0(&v11, 1);
            const FP tlvYPos = FP_FromInteger(v11.h);

            if (Bullet::InZBulletCover(field_B8_xpos, tlvYPos, v11) || !gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                field_B8_xpos,
                tlvYPos,
                0))
            {
                // ZCover saved us, or somehow we've not in the current camera
                field_114_flags.Clear(Flags_114::e114_Bit1_bShot);
                field_10C_health = FP_FromInteger(1);
                return 0;
            }

            // Nothing saved us, get shot
            auto pBloodMem = ae_new<Blood>();
            if (pBloodMem)
            {
                pBloodMem->ctor_40F0B0(
                    field_B8_xpos,
                    field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale),
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    50);
            }

            SetPal_4772D0(Mud_Emotion::eNormal_0);
            // Fall though to other cases below
        }
    }

    case Types::eGasClock_23:
    case Types::eRockSpawner_48:
    case Types::eMineCar_89:
    case Types::eParamite_96:
    case Types::eScrab_112:
        if (field_10C_health <= FP_FromInteger(0))
        {
            return 0;
        }
        field_16A_flags.Clear(Flags::eBit2_save_state);
        field_10C_health = FP_FromInteger(0);
        field_18E_ai_state = Mud_AI_State::AI_ShrivelDeath_5_4714A0;
        field_106_current_motion = Mud_Motion::M_KnockForward_45_474180;
        field_108_next_motion = -1;
        field_194_timer = sGnFrame_5C1B84 + 90;
        VUpdateAnimRes_474D80();
        Event_Broadcast_422BC0(kEventMudokonDied, this);
        if (pFrom->field_4_typeId == Types::eGasClock_23)
        {
            SFX_Play_46FBA0(SoundEffect::Choke_81, 127, 128);
        }
        else
        {
            Mudokon_SFX_457EC0(MudSounds::eHurt2_9, 0, Math_RandomRange_496AB0(-127, 127), this);
        }
        SetPal_4772D0(Mud_Emotion::eNormal_0);
        return 1;

    case Types::eGrinder_30:
    case Types::eBaseBomb_46:
    case Types::eType_86:
    case Types::eExplosion_109:
        if (field_10C_health <= FP_FromInteger(0) ||
            (FindObjectOfType_425180(Types::eTorturedMud_141,field_B8_xpos, field_BC_ypos - FP_FromInteger(50))
            && field_18E_ai_state == Mud_AI_State::AI_Sick_9_47A910))
        {
            return 1;
        }

        field_10C_health = FP_FromInteger(0);

        if (field_16A_flags.Get(Flags::eBit4_blind))
        {
            auto pMudGibs1 = ae_new<Gibs>();
            if (pMudGibs1)
            {
                pMudGibs1->ctor_40FB40(
                    GibType::BlindMud_4,
                    field_B8_xpos,
                    field_BC_ypos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    0);
            }

            auto pMudGibs2 = ae_new<Gibs>();
            if (pMudGibs2)
            {
                pMudGibs2->ctor_40FB40(
                    GibType::BlindMud_4,
                    field_B8_xpos,
                    field_BC_ypos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    0);
            }
        }
        else
        {
            auto pMudGibs1 = ae_new<Gibs>();
            if (pMudGibs1)
            {
                pMudGibs1->ctor_40FB40(
                    GibType::Mud_3,
                    field_B8_xpos,
                    field_BC_ypos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    0);
            }

            auto pMudGibs2 = ae_new<Gibs>();
            if (pMudGibs2)
            {
                pMudGibs2->ctor_40FB40(
                    GibType::Mud_3,
                    field_B8_xpos,
                    field_BC_ypos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    0);
            }
        }

        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        SetPal_4772D0(Mud_Emotion::eNormal_0);
        Event_Broadcast_422BC0(kEventMudokonDied, sActiveHero_5C1B68);
        return 1;

    case Types::eElectricWall_39:
        Mudokon_SFX_457EC0(MudSounds::eDeathDropScream_15, 0, 0, this);
        Event_Broadcast_422BC0(kEventMudokonDied, this);
        return 1;

    case Types::eFleech_50:
        if (field_10C_health <= FP_FromInteger(0))
        {
            return 1;
        }

        field_10C_health -= FP_FromDouble(0.200988769531); // TODO Do we need this level of accuracy?? 0x3374;
        if (field_10C_health <= FP_FromInteger(0))
        {
            field_10C_health = FP_FromInteger(0);
            Mudokon_SFX_457EC0(MudSounds::eHurt1_16, 0, 1000, this);
            field_16A_flags.Clear(Flags::eBit2_save_state);
            field_18E_ai_state = Mud_AI_State::AI_ShrivelDeath_5_4714A0;
            field_194_timer = sGnFrame_5C1B84 + 90;
            field_106_current_motion = Mud_Motion::M_Knockback_46_4742A0;
            field_108_next_motion = -1;
            Event_Broadcast_422BC0(kEventMudokonDied, this);
            SetPal_4772D0(Mud_Emotion::eNormal_0);

            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            auto pFleech = static_cast<BaseAliveGameObject*>(pFrom);
            auto pBloodFromFleech = ae_new<Blood>();
            if (pBloodFromFleech)
            {
                pBloodFromFleech->ctor_40F0B0(
                    field_B8_xpos,
                    (FP_FromInteger(bRect.y + bRect.h) / FP_FromInteger(2)),
                    field_B8_xpos - pFleech->field_B8_xpos < FP_FromInteger(0) ? FP_FromInteger(-24) : FP_FromInteger(24),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    50);
            }

            // TODO: Only set if pFrom->field_B8_xpos != field_B8_xpos ??
            field_106_current_motion = Mud_Motion::M_KnockForward_45_474180;

            VUpdateAnimRes_474D80();
            SFX_Play_46FA90(SoundEffect::KillEffect_64, 127);
            SFX_Play_46FA90(SoundEffect::FallingItemHit_47, 90);
        }
        else
        {
            HurtSound_475DB0();
        }
        return 1;

    case Types::eAbe_69:
        if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_62_Punch_454750)
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                SetPal_4772D0(Mud_Emotion::eNormal_0);
                TakeASlap_476090(pFrom);
            }
        }
        else if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_63_Sorry_454670)
        {
            field_17E_delayed_speak = MudAction::eSorry_8;
        }
        return 1;

    case Types::eAbilityRing_104:
        return 0;

    case Types::eMudokon_110:
        if (static_cast<Mudokon*>(pFrom)->field_106_current_motion != Mud_Motion::M_Punch_38_474AA0 || field_10C_health <= FP_FromInteger(0))
        {
            return 1;
        }
        SetPal_4772D0(Mud_Emotion::eNormal_0);
        TakeASlap_476090(pFrom);
        return 1;

    case Types::eShrykull_121:
    case Types::eElectrocute_150:
        if (field_10C_health <= FP_FromInteger(0))
        {
            return 1;
        }
        field_10C_health = FP_FromInteger(0);
        Event_Broadcast_422BC0(kEventMudokonDied, this);
        SetPal_4772D0(Mud_Emotion::eNormal_0);
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return 1;

    case Types::eSlamDoor_122:
        if (field_10C_health <= FP_FromInteger(0) || field_106_current_motion == Mud_Motion::M_JumpMid_36_474570)
        {
            return 1;
        }
        StandingKnockBack_473190();
        VUpdateAnimRes_474D80();
        return 1;

    case Types::eSlog_126:
        if (field_10C_health <= FP_FromInteger(0))
        {
            return 1;
        }
        field_16A_flags.Clear(Flags::eBit2_save_state);
        field_10C_health = FP_FromInteger(0);
        field_18E_ai_state = Mud_AI_State::AI_ShrivelDeath_5_4714A0;
        field_194_timer = sGnFrame_5C1B84 + 90;
        field_106_current_motion = Mud_Motion::M_KnockForward_45_474180;
        Event_Broadcast_422BC0(kEventMudokonDied, this);
        VUpdateAnimRes_474D80();
        SetPal_4772D0(Mud_Emotion::eNormal_0);
        return 1;

    default:
        if (field_10C_health <= FP_FromInteger(0))
        {
            return 1;
        }
        HurtSound_475DB0();
        field_106_current_motion = Mud_Motion::M_KnockForward_45_474180;
        field_108_next_motion = -1;
        field_194_timer = sGnFrame_5C1B84 + 30;
        VUpdateAnimRes_474D80();
        if (field_10_resources_array.ItemAt(2))
        {
            field_18E_ai_state = Mud_AI_State::AI_Chisel_1_47C5F0;
            field_190_sub_state = 8;
        }
        else
        {
            field_18E_ai_state = Mud_AI_State::AI_Scrub_2_47D270;
            field_190_sub_state = 7;
        }

        field_DA_xOffset = field_162_maxXOffset;

        RemoveAlerted();

        if (pFrom == sControlledCharacter_5C1B8C)
        {
            field_10C_health -= FP_FromDouble(0.06);
            if (field_10C_health > FP_FromInteger(0))
            {
                Event_Broadcast_422BC0(kEventMudokonAbuse, this);
            }
            else
            {
                field_16A_flags.Clear(Flags::eBit2_save_state);
                field_18E_ai_state = Mud_AI_State::AI_ShrivelDeath_5_4714A0;
                field_194_timer = sGnFrame_5C1B84 + 90;
                Event_Broadcast_422BC0(kEventMudokonDied, this);
            }
        }
        SetPal_4772D0(Mud_Emotion::eNormal_0);
        return 1;
    }
}

__int16 Mudokon::TurningWheelHelloOrAllYaResponse()
{
    if (!field_16A_flags.Get(Flags::eBit10_stop_trigger))
    {
        return field_190_sub_state;
    }

    auto pWheelTlv = static_cast<Path_Wheel*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        TlvTypes::WorkWheel_79));

    if (SwitchStates_Get_466020(pWheelTlv->field_12_id))
    {
        if (field_198_turning_wheel_timer == 0)
        {
            field_198_turning_wheel_timer = sGnFrame_5C1B84 + 20;
        }
    }

    // OG Bug: The second condition can never resolve to true because field_198_turning_wheel_timer will always be reset to zero before it can happen.
    if (!SwitchStates_Get_466020(pWheelTlv->field_12_id) || field_198_turning_wheel_timer > static_cast<int>(sGnFrame_5C1B84))
    {
        return field_190_sub_state;
    }

    field_198_turning_wheel_timer = 0;
    field_16A_flags.Clear(Flags::eBit7_stopped_at_wheel);
    field_194_timer = sGnFrame_5C1B84 + 45;
    return 1;
}

void Mudokon::vUpdateAnimRes_474D80()
{
    BYTE** ppRes = AnimBlockForMotion_474DC0(field_106_current_motion);
    if (!ppRes)
    {
        LOG_ERROR("No res for " << field_106_current_motion);
    }
    const AnimRecord& animRec = AnimRec(kMudFrameTableOffsets_55CD00[field_106_current_motion]);
    field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, ppRes);
}

enum AI_GiveRings
{
    eState0_Inactive_0 = 0,
    eState0_PreIdle_1 = 1,
    eState0_Idle_2 = 2,
    eState0_GoodGameSpeak_3 = 3,
    eState0_WrongGameSpeak_4 = 4,
    eState0_Shrug_5 = 5,
    eState0_SaysOkay_6 = 6,
    eState0_Chanting_7 = 7,
    eState0_GivingRing_8 = 8,
    eState0_RecievingRing1_9 = 9,
    eState0_RecievingRing2_10 = 10
};

enum AI_Chisle
{
    eState1_StartToChisle_0 = 0,
    eState1_ChisleTheFloor_1 = 1,
    eState1_SmallBreak_2 = 2,
    eState1_StandUp_3 = 3,
    eState1_SadNoise_4 = 4,
    eState1_Duck_5 = 5,
    eState1_OutOfDuck_6 = 6,
    eState1_DuckToChisle_7 = 7,
    eState1_DuckKnockback_8 = 8
};

enum AI_Scrub
{
    eState2_StartToScrub_0 = 0,
    eState2_SmallBreak_1 = 1,
    eState2_Scrubbing_2 = 2,
    eState2_StandUp_3 = 3,
    eState2_SadNoise_4 = 4,
    eState2_SmallBreakThroughGameSpeak_5 = 5,
    eState2_Duck_6 = 6,
    eState2_DuckKnockback_7 = 7
};

enum AI_TurnWheel
{
    eState3_TurningWheel_0 = 0,
    eState3_SittingDown_1 = 1,
    eState3_InterruptAction_2 = 2,
    eState3_TurningWheelToDuck_3 = 3,
    eState3_Duck_4 = 4,
    eState3_DuckToTurningWheel_5 = 5
};

enum AI_ListeningToAbe
{
    eState4_Inactive_0 = 0,
    eState4_GetsAttentive_1 = 1,
    eState4_CrazyFollowMe_2 = 2,
    eState4_SteppingBack_3 = 3,
    eState4_FollowingIdle_4 = 4,
    eState4_Walking_5 = 5,
    eState4_Running_6 = 6,
    eState4_Idle_7 = 7,
    eState4_CrazySlideTurn_8 = 8,
    eState4_GetsWaitWhileMoving_9 = 9,
    eState4_InitializeMovement_10 = 10,
    eState4_PullingLever_11 = 11,
    eState4_GetsCommand_12 = 12,
    eState4_Slapped_13 = 13,
    eState4_Knockback_14 = 14,
    eState4_GettingSorry_15 = 15,
    eState4_Suiciding_16 = 16,
    eState4_RageSlap_17 = 17,
    eState4_RageTurn_18 = 18,
    eState4_CrazyDeny_19 = 19,
    eState4_StopsWhileMoving_20 = 20,
    eState4_CrazyLaugh_21 = 21,
    eState4_LostAttention_22 = 22
};

enum AI_Escape
{
    eState6_PortalOppened_0 = 0,
    eState6_StandingUp_1 = 1,
    eState6_Running_2 = 2,
    eState6_Jumping_3 = 3,
    eState6_Replacing_4 = 4
};

enum AI_AngryWorker
{
    eState8_Inactive_0 = 0,
    eState8_LeverIdel_1 = 1,
    eState8_PullingLever_2 = 2,
    eState8_Speaking_3 = 3,
    eState8_WheelIdle_4 = 4,
    eState8_UsingWheel_5 = 5,
    eState8_Unused_6 = 6,
    eState8_Unused_7 = 7
};

enum AI_Sick
{
    eState9_Inactive_0 = 0,
    eState9_Idle_1 = 1,
    eState9_Duck_2 = 2,
    eState9_StandingUp_3 = 3,
    eState9_Farting_4 = 4,
};

__int16 Mudokon::AI_GiveRings_0_470C10()
{
    switch (field_190_sub_state)
    {
    case AI_GiveRings::eState0_Inactive_0:
        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            sActiveHero_5C1B68->field_C2_lvl_number,
            sActiveHero_5C1B68->field_C0_path_number,
            sActiveHero_5C1B68->field_B8_xpos,
            sActiveHero_5C1B68->field_BC_ypos,
            0))
        {
            field_16C &= ~1u;
            field_194_timer = sGnFrame_5C1B84 + 60;
            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;

            if (sActiveHero_5C1B68->field_168_ring_pulse_timer)
            {
                field_16C |= 1;
                return AI_GiveRings::eState0_Idle_2;
            }
            else
            {
                return AI_GiveRings::eState0_PreIdle_1;
            }
        }
        break;

    case AI_GiveRings::eState0_PreIdle_1:
        if (static_cast<int>(sGnFrame_5C1B84) >= field_194_timer && field_106_current_motion == Mud_Motion::M_Idle_0_4724E0)
        {
            if (vIsFacingMe_4254A0(sActiveHero_5C1B68))
            {
                Sound_475EC0(MudSounds::eHelloNeutral_3);
                field_108_next_motion = Mud_Motion::M_Speak_4_472FA0;
                field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 150;
                return AI_GiveRings::eState0_Idle_2;
            }
            field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
        }
        break;

    case AI_GiveRings::eState0_Idle_2:
        if (field_16A_flags.Get(Flags::eBit16_instant_power_up) &&
            gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                field_B8_xpos,
                field_BC_ypos,
                0) &&
            gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                sActiveHero_5C1B68->field_C2_lvl_number,
                sActiveHero_5C1B68->field_C0_path_number,
                sActiveHero_5C1B68->field_B8_xpos,
                sActiveHero_5C1B68->field_BC_ypos,
                0)
            && !sActiveHero_5C1B68->field_168_ring_pulse_timer)
        {
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
            return AI_GiveRings::eState0_SaysOkay_6;
        }
        else
        {
            const int lastEventIdx = pEventSystem_5BC11C->field_28_last_event_index;
            const bool bSameAsLastIdx = field_140_last_event_index == lastEventIdx;
            if (!bSameAsLastIdx)
            {
                field_140_last_event_index = lastEventIdx;
            }

            if (bSameAsLastIdx || pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eNone_m1 || pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eSameAsLast_m2)
            {
                if (!(field_16C & 1) && static_cast<int>(sGnFrame_5C1B84) > field_194_timer)
                {
                    return AI_GiveRings::eState0_Shrug_5;
                }
            }
            else
            {
                field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
                if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eHello_9)
                {
                    return AI_GiveRings::eState0_GoodGameSpeak_3;
                }
                else
                {
                    return AI_GiveRings::eState0_WrongGameSpeak_4;
                }
            }
        }
        break;

    case AI_GiveRings::eState0_GoodGameSpeak_3:
        if (static_cast<int>(sGnFrame_5C1B84) >= field_194_timer && field_106_current_motion == Mud_Motion::M_Idle_0_4724E0)
        {
            if (vIsFacingMe_4254A0(sActiveHero_5C1B68))
            {
                if (sActiveHero_5C1B68->field_168_ring_pulse_timer > 0)
                {
                    Sound_475EC0(MudSounds::eHelloNeutral_3);
                }
                else
                {
                    Sound_475EC0(MudSounds::eOkay_12);
                }
                field_108_next_motion = Mud_Motion::M_Speak_4_472FA0;
                return AI_GiveRings::eState0_SaysOkay_6;
            }
            field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
        }
        break;

    case AI_GiveRings::eState0_WrongGameSpeak_4:
        if (static_cast<int>(sGnFrame_5C1B84) >= field_194_timer && field_106_current_motion == Mud_Motion::M_Idle_0_4724E0)
        {
            if (vIsFacingMe_4254A0(sActiveHero_5C1B68))
            {
                field_108_next_motion = Mud_Motion::M_DunnoBegin_43_472790;
                return AI_GiveRings::eState0_Idle_2;
            }
            field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
        }
        break;

    case AI_GiveRings::eState0_Shrug_5:
        if (static_cast<int>(sGnFrame_5C1B84) >= field_194_timer && field_106_current_motion == Mud_Motion::M_Idle_0_4724E0)
        {
            if (vIsFacingMe_4254A0(sActiveHero_5C1B68))
            {
                field_108_next_motion = Mud_Motion::M_DunnoBegin_43_472790;
                field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
                return AI_GiveRings::eState0_PreIdle_1;
            }
            field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
        }
        break;

    case AI_GiveRings::eState0_SaysOkay_6:
        if (field_106_current_motion == Mud_Motion::M_Idle_0_4724E0)
        {
            if (sActiveHero_5C1B68->field_168_ring_pulse_timer <= 0)
            {
                field_108_next_motion = 50;
                field_194_timer = sGnFrame_5C1B84 + 30;
                return AI_GiveRings::eState0_Chanting_7;
            }
            else
            {
                return AI_GiveRings::eState0_Idle_2;
            }
        }
        break;

    case AI_GiveRings::eState0_Chanting_7:
        if (static_cast<int>(sGnFrame_5C1B84) > field_194_timer)
        {
            if (field_168_ring_type == RingTypes::eExplosive_Emit_Effect_2)
            {
                // Red flicker
                auto pFlicker = ae_new<PossessionFlicker>();
                if (pFlicker)
                {
                    pFlicker->ctor_4319E0(this, 10, 255, 128, 128);
                }
            }
            else
            {
                // Greenish flicker
                auto pFlicker = ae_new<PossessionFlicker>();
                if (pFlicker)
                {
                    pFlicker->ctor_4319E0(this, 10, 255, 255, 32);
                }
            }
            field_194_timer = sGnFrame_5C1B84 + 15;
            return AI_GiveRings::eState0_GivingRing_8;
        }
        break;

    case AI_GiveRings::eState0_GivingRing_8:
        if (static_cast<int>(sGnFrame_5C1B84) > field_194_timer)
        {
            // Create a ring emitting from us
            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            AbilityRing::Factory_482F80(
                FP_FromInteger((bRect.x + bRect.w) / 2),
                FP_FromInteger((bRect.y + bRect.h) / 2),
                field_168_ring_type,
                field_CC_sprite_scale);

            // Create a ring that locks onto abe
            PSX_RECT bRectAbe = {};
            sActiveHero_5C1B68->vGetBoundingRect_424FD0(&bRectAbe, 1);

            RingTypes ringTypeToGive = RingTypes::eExplosive_Give_3;
            if (field_168_ring_type == RingTypes::eHealing_Emit_Effect_11)
            {
                ringTypeToGive = RingTypes::eHealing_Give_13;
            }

            AbilityRing* pRing = AbilityRing::Factory_482F80(
                FP_FromInteger((bRectAbe.x + bRectAbe.w) / 2),
                FP_FromInteger((bRectAbe.y + bRectAbe.h) / 2),
                ringTypeToGive,
                sActiveHero_5C1B68->field_CC_sprite_scale);

            // Must set abe as the target to "lock on" to abe
            if (pRing)
            {
                pRing->VSetTarget(sActiveHero_5C1B68);
            }

            field_194_timer = sGnFrame_5C1B84 + 30;
            return AI_GiveRings::eState0_RecievingRing1_9;
        }
        break;

    case AI_GiveRings::eState0_RecievingRing1_9:
        if (static_cast<int>(sGnFrame_5C1B84) > field_194_timer)
        {
            if (field_164_ring_timeout > 0)
            {
                sActiveHero_5C1B68->field_168_ring_pulse_timer = sGnFrame_5C1B84 + field_164_ring_timeout;
            }
            else
            {
                sActiveHero_5C1B68->field_168_ring_pulse_timer = sGnFrame_5C1B84 + 200000;
            }

            sActiveHero_5C1B68->field_16C_bHaveShrykull = FALSE;

            if (field_168_ring_type == RingTypes::eHealing_Emit_Effect_11)
            {
                sActiveHero_5C1B68->field_1AC_flags.Set(Abe::e1AC_eBit15_bHaveHealing);
            }

            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            return AI_GiveRings::eState0_RecievingRing2_10;
        }
        break;

    case AI_GiveRings::eState0_RecievingRing2_10:
        field_16C |= 1u;
        return 2;

    default:
        break;
    }

    return field_190_sub_state;
}

const __int16 kDelayTable_55CF7C[6] = { 0, 6, 12, 18, 24, 30, };
ALIVE_VAR(1, 0x5C3014, short, sDelayIdx_5C3014, 0);

signed __int16 Mudokon::AI_Chisel_1_47C5F0()
{
    if (CheckForPortal_4775E0())
    {
        AddAlerted();
        field_18E_ai_state = Mud_AI_State::AI_Escape_6_47A560;
        return AI_Chisle::eState1_StartToChisle_0;
    }

    if (field_16C & 2)
    {
        if (field_190_sub_state != AI_Chisle::eState1_StandUp_3)
        {
            word_5C3018 = 0;
            field_16C &= ~2u;
        }
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
    if (lastSpeak == GameSpeakEvents::Slig_LookOut_6 || Event_Get_422C00(kEventShooting))
    {
        if (field_190_sub_state != AI_Chisle::eState1_StandUp_3 && field_190_sub_state != AI_Chisle::eState1_Duck_5 && field_190_sub_state != AI_Chisle::eState1_DuckKnockback_8 && field_190_sub_state != AI_Chisle::eState1_OutOfDuck_6)
        {
            field_190_sub_state = AI_Chisle::eState1_Duck_5;
        }
    }

    BaseAnimatedWithPhysicsGameObject* pAbuseEvent = Event_Is_Event_In_Range_422C30(kEventMudokonAbuse, field_B8_xpos, field_BC_ypos, field_D6_scale);
    BaseAnimatedWithPhysicsGameObject* pDeadMudEvent = Event_Is_Event_In_Range_422C30(kEventMudokonDied, field_B8_xpos, field_BC_ypos, field_D6_scale);
    BaseAnimatedWithPhysicsGameObject* pLoudNoiseEvent = Event_Is_Event_In_Range_422C30(kEventLoudNoise, field_B8_xpos, field_BC_ypos, field_D6_scale);

    const bool reactToAbused = (pAbuseEvent && pAbuseEvent != this && field_190_sub_state != AI_Chisle::eState1_StandUp_3 && 
        gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0));

    const bool reactToDeadMud = (pDeadMudEvent && pDeadMudEvent != this && field_190_sub_state != AI_Chisle::eState1_StandUp_3 && 
        gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0));

    const bool reactToLoudNoise = (pLoudNoiseEvent && pLoudNoiseEvent->field_4_typeId == Types::eGlukkon_67 && field_190_sub_state != AI_Chisle::eState1_StandUp_3 && 
        gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0));

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
        if (field_16A_flags.Get(Flags::eBit13))
        {
            field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_16);
        }
        else
        {
            field_16A_flags.Set(Flags::eBit13);
            field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_15);
        }
    }

    if (reactToAbused || reactToDeadMud || reactToLoudNoise)
    {
        if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags::eBit11_get_depressed))
        {
            field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
        }

        if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
        {
            AddAlerted();

            if (field_180_emo_tbl == Mud_Emotion::eSad_3)
            {
                field_16A_flags.Set(Flags::eBit6_standing_for_sad_or_angry);
            }

            field_190_sub_state = AI_Chisle::eState1_StandUp_3;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (LaughingGasInCurrentScreen_4774A0())
        {
            field_180_emo_tbl = Mud_Emotion::eWired_6;

            AddAlerted();

            field_190_sub_state = AI_Chisle::eState1_StandUp_3;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eAngry_1 && !field_128_angry_timer)
    {
        if (SwitchStates_Get_466020(field_120_angry_trigger))
        {
            field_128_angry_timer = sGnFrame_5C1B84 + 20;
        }
    }

    if (!field_16A_flags.Get(Flags::eBit8_do_angry))
    {
        if (field_128_angry_timer > 0)
        {
            if (field_128_angry_timer <= static_cast<int>(sGnFrame_5C1B84))
            {
                field_16A_flags.Set(Flags::eBit8_do_angry);
                field_180_emo_tbl = Mud_Emotion::eAngry_1;
                field_190_sub_state = AI_Chisle::eState1_StandUp_3;
                field_194_timer = sGnFrame_5C1B84;
            }
        }
    }

    switch (field_190_sub_state)
    {
    case AI_Chisle::eState1_StartToChisle_0:
        if (field_106_current_motion != Mud_Motion::M_StandToCrouch_17_474120)
        {
            field_106_current_motion = Mud_Motion::M_Chisel_11_4732D0;
            field_108_next_motion = -1;
        }
        field_194_timer = (Math_NextRandom() % 64) + (sGnFrame_5C1B84 + 35);
        return AI_Chisle::eState1_ChisleTheFloor_1;

    case AI_Chisle::eState1_ChisleTheFloor_1:
    {
        bool ignoreHellOrAllYa = false;
        if (lastSpeak == GameSpeakEvents::eHello_9)
        {
            if (word_5C3018 || !CanRespond_4770B0())
            {
                ignoreHellOrAllYa = true;
            }
        }
        else if (lastSpeak != GameSpeakEvents::eAllYa_23)
        {
            ignoreHellOrAllYa = true;
        }

        if (!ignoreHellOrAllYa && sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
        {
            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
            word_5C3018 = 1;
            field_16C |= 2u;

            AddAlerted();
            return AI_Chisle::eState1_StandUp_3;
        }

        if (field_106_current_motion == Mud_Motion::M_Idle_0_4724E0)
        {
            field_108_next_motion = Mud_Motion::M_Chisel_11_4732D0;
        }

        if (static_cast<int>(sGnFrame_5C1B84) > field_194_timer && field_106_current_motion == Mud_Motion::M_Chisel_11_4732D0)
        {
            field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
        }

        if (field_106_current_motion != Mud_Motion::M_CrouchIdle_15_474040)
        {
            return field_190_sub_state;
        }

        if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && (Math_NextRandom() < 120))
        {
            AddAlerted();

            field_16A_flags.Set(Flags::eBit14_make_sad_noise);
            field_16A_flags.Set(Flags::eBit6_standing_for_sad_or_angry);

            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
            return AI_Chisle::eState1_StandUp_3;
        }
        else
        {
            field_194_timer = (Math_NextRandom() % 64) + (sGnFrame_5C1B84 + 35);
            return AI_Chisle::eState1_SmallBreak_2;
        }
    }
    break;

    case AI_Chisle::eState1_SmallBreak_2:
    {
        bool ignoreHellOrAllYa = false;
        if (lastSpeak == GameSpeakEvents::eHello_9)
        {
            if (word_5C3018 || !CanRespond_4770B0())
            {
                ignoreHellOrAllYa = true;
            }
        }
        else if (lastSpeak != GameSpeakEvents::eAllYa_23)
        {
            ignoreHellOrAllYa = true;
        }

        if (!ignoreHellOrAllYa && sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
        {
            AddAlerted();

            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
            word_5C3018 = 1;
            field_16C |= 2u;
            return AI_Chisle::eState1_StandUp_3;
        }

        if (field_106_current_motion == Mud_Motion::M_Idle_0_4724E0)
        {
            field_108_next_motion = Mud_Motion::M_StandToCrouch_17_474120;
        }

        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }

        field_194_timer = (Math_NextRandom() % 64) + (sGnFrame_5C1B84 + 35);
        field_108_next_motion = Mud_Motion::M_Chisel_11_4732D0;
    }
    return AI_Chisle::eState1_ChisleTheFloor_1;

    case AI_Chisle::eState1_StandUp_3:
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

        if (!skip && (bForce || (!word_5C3018 && CanRespond_4770B0())))
        {
            if (sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                AddAlerted();
                word_5C3018 = 1;
                field_16C |= 2u;

                field_16A_flags.Clear(Flags::eBit14_make_sad_noise);
                field_16A_flags.Clear(Flags::eBit6_standing_for_sad_or_angry);
            }
        }

        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer || field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
        {
            if (field_108_next_motion != -1)
            {
                return field_190_sub_state;
            }

            if (field_106_current_motion == Mud_Motion::M_Chisel_11_4732D0 || field_106_current_motion == Mud_Motion::M_CrouchIdle_15_474040)
            {
                field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            }

            if (field_106_current_motion != Mud_Motion::M_Duck_53_474A40)
            {
                return field_190_sub_state;
            }
            else
            {
                field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
                return field_190_sub_state;
            }
        }

        if (field_180_emo_tbl == Mud_Emotion::eSad_3 && field_16A_flags.Get(Flags::eBit14_make_sad_noise))
        {
            field_16A_flags.Clear(Flags::eBit12_alert_enemies);
            field_16A_flags.Clear(Flags::eBit14_make_sad_noise);
            field_108_next_motion = Mud_Motion::M_Speak_6_472FA0;
            Sound_475EC0(MudSounds::eSadUgh_28);
            return AI_Chisle::eState1_SadNoise_4;
        }
        else
        {
            word_5C3018 = 0;
            field_16C &= ~2u;
            if (field_180_emo_tbl != Mud_Emotion::eAngry_1 || !field_16A_flags.Get(Flags::eBit8_do_angry))
            {
                field_18E_ai_state = Mud_AI_State::AI_ListeningToAbe_4_477B40;
                return AI_ListeningToAbe::eState4_Inactive_0;
            }
            else
            {
                field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
                field_18E_ai_state = Mud_AI_State::AI_ListeningToAbe_4_477B40;
                field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
                return AI_ListeningToAbe::eState4_Knockback_14;
            }
        }
    }
    break;

    case AI_Chisle::eState1_SadNoise_4:
        if (field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
        {
            return field_190_sub_state;
        }
        field_16A_flags.Clear(Flags::eBit6_standing_for_sad_or_angry);
        field_108_next_motion = Mud_Motion::M_StandToCrouch_17_474120;
        return AI_Chisle::eState1_StartToChisle_0;

    case AI_Chisle::eState1_Duck_5:
        if (field_106_current_motion == Mud_Motion::M_Idle_0_4724E0)
        {
            field_108_next_motion = Mud_Motion::M_StandToCrouch_17_474120;
            return field_190_sub_state;
        }

        if (field_106_current_motion == Mud_Motion::M_Chisel_11_4732D0)
        {
            field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
            return field_190_sub_state;
        }

        if (field_106_current_motion != Mud_Motion::M_CrouchIdle_15_474040)
        {
            return field_190_sub_state;
        }

        field_108_next_motion = Mud_Motion::M_Duck_53_474A40;
        field_194_timer = sGnFrame_5C1B84 + 60;
        return AI_Chisle::eState1_OutOfDuck_6;

    case AI_Chisle::eState1_OutOfDuck_6:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
        return AI_Chisle::eState1_DuckToChisle_7;

    case AI_Chisle::eState1_DuckToChisle_7:
        if (field_106_current_motion == Mud_Motion::M_Chisel_11_4732D0)
        {
            return AI_Chisle::eState1_StartToChisle_0;
        }
        if (field_106_current_motion != Mud_Motion::M_CrouchIdle_15_474040)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::M_Chisel_11_4732D0;
        return field_190_sub_state;

    case AI_Chisle::eState1_DuckKnockback_8:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::M_DuckKnockback_55_474250;
        field_194_timer = sGnFrame_5C1B84 + 60;
        return AI_Chisle::eState1_OutOfDuck_6;

    default:
        return field_190_sub_state;
    }
}

__int16 Mudokon::AI_Scrub_2_47D270()
{
    if (CheckForPortal_4775E0())
    {
        AddAlerted();
        field_18E_ai_state = Mud_AI_State::AI_Escape_6_47A560;
        return AI_Escape::eState6_PortalOppened_0;
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
    if (field_16C & 2)
    {
        if (field_190_sub_state != AI_Scrub::eState2_StandUp_3)
        {
            word_5C3018 = 0;
            field_16C &= ~2u;
        }
    }

    if (lastSpeak == GameSpeakEvents::Slig_LookOut_6 || Event_Get_422C00(kEventShooting))
    {
        field_108_next_motion = Mud_Motion::M_Duck_53_474A40;
        field_194_timer = sGnFrame_5C1B84 + 60;
        return AI_Scrub::eState2_Duck_6;
    }

    BaseAnimatedWithPhysicsGameObject* pAbuse = Event_Is_Event_In_Range_422C30(
        kEventMudokonAbuse,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale);
    if (pAbuse)
    {
        if (pAbuse != this)
        {
            if (field_190_sub_state != AI_Scrub::eState2_StandUp_3
                && field_190_sub_state != AI_Scrub::eState2_DuckKnockback_7
                && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    field_B8_xpos,
                    field_BC_ypos,
                    0))
            {
                field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eMudAbuse_9);
                if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags::eBit11_get_depressed))
                {
                    field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
                }

                if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    AddAlerted();

                    if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                    {
                        field_16A_flags.Set(Flags::eBit6_standing_for_sad_or_angry);
                    }

                    field_190_sub_state = AI_Scrub::eState2_StandUp_3;
                    field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
                }
            }
        }
    }

    BaseAnimatedWithPhysicsGameObject* pDied = Event_Is_Event_In_Range_422C30(
        kEventMudokonDied,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale);
    if (pDied)
    {
        if (pDied != this)
        {
            if (field_190_sub_state != AI_Scrub::eState2_StandUp_3
                && field_190_sub_state != AI_Scrub::eState2_DuckKnockback_7
                && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    field_B8_xpos,
                    field_BC_ypos,
                    0))
            {
                field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eMudDied_14);
                if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags::eBit11_get_depressed))
                {
                    field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
                }

                if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    AddAlerted();

                    if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                    {
                        field_16A_flags.Set(Flags::eBit6_standing_for_sad_or_angry);
                    }

                    field_190_sub_state = AI_Scrub::eState2_StandUp_3;
                    field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
                }
            }
        }
    }

    BaseAnimatedWithPhysicsGameObject* pLoudNoise = Event_Is_Event_In_Range_422C30(
        kEventLoudNoise,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale);
    if (pLoudNoise)
    {
        if (pLoudNoise->field_4_typeId == Types::eGlukkon_67)
        {
            if (field_190_sub_state != AI_Scrub::eState2_StandUp_3
                && field_190_sub_state != AI_Scrub::eState2_DuckKnockback_7
                && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    field_B8_xpos,
                    field_BC_ypos,
                    0))
            {
                if (field_16A_flags.Get(Flags::eBit13))
                {
                    field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_16);
                }
                else
                {
                    field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_15);
                    field_16A_flags.Set(Flags::eBit13);
                }

                if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags::eBit11_get_depressed))
                {
                    field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
                }

                if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    AddAlerted();

                    if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                    {
                        field_16A_flags.Set(Flags::eBit6_standing_for_sad_or_angry);
                    }

                    field_190_sub_state = AI_Scrub::eState2_StandUp_3;
                    field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
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
            field_190_sub_state = AI_Scrub::eState2_StandUp_3;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eAngry_1 && !field_128_angry_timer)
    {
        if (SwitchStates_Get_466020(field_120_angry_trigger))
        {
            field_128_angry_timer = sGnFrame_5C1B84 + 20;
        }
    }

    if (!field_16A_flags.Get(Flags::eBit8_do_angry))
    {
        if (field_128_angry_timer)
        {
            if (field_128_angry_timer <= (signed int)sGnFrame_5C1B84)
            {
                field_16A_flags.Set(Flags::eBit8_do_angry);
                field_180_emo_tbl = Mud_Emotion::eAngry_1;
                field_190_sub_state = AI_Scrub::eState2_StandUp_3;
                field_194_timer = sGnFrame_5C1B84;
            }
        }
    }

    switch (field_190_sub_state)
    {
    case AI_Scrub::eState2_StartToScrub_0:
        field_106_current_motion = Mud_Motion::M_CrouchScrub_14_473560;
        field_108_next_motion = -1;
        field_194_timer = Math_NextRandom() % 64 + sGnFrame_5C1B84 + 15;
        field_124 = Math_NextRandom() % 64 + sGnFrame_5C1B84 + 240;
        return AI_Scrub::eState2_Scrubbing_2;

    case AI_Scrub::eState2_SmallBreak_1:
    {
        bool checkAlerted = true;
        if (lastSpeak == GameSpeakEvents::eHello_9)
        {
            if (word_5C3018 || !CanRespond_4770B0())
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
            if (sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                AddAlerted();
                word_5C3018 = 1;
                field_16C |= 2u;
                field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
                return AI_Scrub::eState2_StandUp_3;
            }
        }

        if (field_106_current_motion == Mud_Motion::M_Idle_0_4724E0)
        {
            field_108_next_motion = Mud_Motion::M_CrouchScrub_14_473560;
        }

        if (field_106_current_motion == Mud_Motion::M_CrouchIdle_15_474040 && static_cast<int>(sGnFrame_5C1B84) > field_124)
        {
            field_124 = Math_NextRandom() % 64 + sGnFrame_5C1B84 + 240;
            field_108_next_motion = Mud_Motion::M_CrouchTurn_16_4740E0;
        }

        if (field_106_current_motion != Mud_Motion::M_CrouchIdle_15_474040 || static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            if (field_106_current_motion != Mud_Motion::M_CrouchScrub_14_473560)
            {
                return field_190_sub_state;
            }
            return AI_Scrub::eState2_Scrubbing_2;
        }

        if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && Math_NextRandom() < 120)
        {
            AddAlerted();
            field_16A_flags.Set(Flags::eBit14_make_sad_noise);
            field_16A_flags.Set(Flags::eBit6_standing_for_sad_or_angry);
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
            return AI_Scrub::eState2_StandUp_3;
        }
        else
        {
            field_194_timer = Math_NextRandom() % 64 + sGnFrame_5C1B84 + 35;
            field_108_next_motion = Mud_Motion::M_CrouchScrub_14_473560;
            if (field_106_current_motion != Mud_Motion::M_CrouchScrub_14_473560)
            {
                return field_190_sub_state;
            }
            return AI_Scrub::eState2_Scrubbing_2;
        }
        break;
    }

    case AI_Scrub::eState2_Scrubbing_2:
    {
        bool checkAlerted = true;
        if (lastSpeak == GameSpeakEvents::eHello_9)
        {
            if (word_5C3018 || !CanRespond_4770B0())
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
            if (sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                AddAlerted();
                word_5C3018 = 1;
                field_16C |= 2u;
                field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
                return AI_Scrub::eState2_StandUp_3;
            }
        }

        if (field_106_current_motion == Mud_Motion::M_Idle_0_4724E0)
        {
            field_108_next_motion = Mud_Motion::M_CrouchScrub_14_473560;
        }
        
        bool bUnknown = false;
        if (field_106_current_motion != Mud_Motion::M_CrouchIdle_15_474040)
        {
            bUnknown = true;
        }
        else
        {
            if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
            {
                field_106_current_motion = Mud_Motion::M_CrouchScrub_14_473560;
                bUnknown = true;
            }
        }

        if (bUnknown)
        {
            if (!Event_Is_Event_In_Range_422C30(kEventSpeaking, field_B8_xpos, field_BC_ypos, field_D6_scale))
            {
                return field_190_sub_state;
            }
            field_194_timer = Math_NextRandom() % 64 + sGnFrame_5C1B84 + 15;
            return AI_Scrub::eState2_SmallBreakThroughGameSpeak_5;
        }
        else
        {
            field_194_timer = Math_NextRandom() % 64 + sGnFrame_5C1B84 + 15;
            return AI_Scrub::eState2_SmallBreak_1;
        }
        break;
    }

    case AI_Scrub::eState2_StandUp_3:
    {
        if (lastSpeak == GameSpeakEvents::eHello_9 || lastSpeak == GameSpeakEvents::eAllYa_23)
        {
            bool checkAlerted = lastSpeak == GameSpeakEvents::eHello_9;
            if (lastSpeak == GameSpeakEvents::eAllYa_23)
            {
                if (!word_5C3018 && CanRespond_4770B0())
                {
                    checkAlerted = true;
                }
            }

            if (checkAlerted)
            {
                if (sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
                {
                    AddAlerted();
                    word_5C3018 = 1;
                    field_16C |= 2u;
                    field_16A_flags.Clear(Flags::eBit14_make_sad_noise);
                    field_16A_flags.Clear(Flags::eBit6_standing_for_sad_or_angry);
                }
            }
        }

        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }

        if (field_106_current_motion == Mud_Motion::M_Duck_53_474A40)
        {
            field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
        }

        if (field_106_current_motion == Mud_Motion::M_CrouchIdle_15_474040)
        {
            field_108_next_motion = Mud_Motion::M_CrouchToStand_18_474150;
        }

        if (field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
        {
            return field_190_sub_state;
        }

        if (field_180_emo_tbl == Mud_Emotion::eSad_3 && field_16A_flags.Get(Flags::eBit14_make_sad_noise))
        {
            field_16A_flags.Clear(Flags::eBit14_make_sad_noise);
            field_16A_flags.Clear(Flags::eBit12_alert_enemies);
            field_108_next_motion = Mud_Motion::M_Speak_6_472FA0;
            Sound_475EC0(MudSounds::eSadUgh_28);
            return AI_Scrub::eState2_SadNoise_4;
        }
        else
        {
            word_5C3018 = 0;
            field_16C &= ~2u;
            if (field_180_emo_tbl != Mud_Emotion::eAngry_1 || !field_16A_flags.Get(Flags::eBit8_do_angry))
            {
                field_18E_ai_state = Mud_AI_State::AI_ListeningToAbe_4_477B40;
                return AI_ListeningToAbe::eState4_Inactive_0;
            }
            else
            {
                field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
                field_18E_ai_state = Mud_AI_State::AI_ListeningToAbe_4_477B40;
                field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
                return AI_ListeningToAbe::eState4_Knockback_14;
            }
        }
        break;
    }

    case AI_Scrub::eState2_SadNoise_4:
        if (field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
        {
            return field_190_sub_state;
        }
        field_16A_flags.Clear(Flags::eBit6_standing_for_sad_or_angry);
        field_108_next_motion = Mud_Motion::M_StandToCrouch_17_474120;
        return AI_Scrub::eState2_SmallBreak_1;

    case AI_Scrub::eState2_SmallBreakThroughGameSpeak_5:
    {
        bool checkAlerted = true;
        if (lastSpeak == GameSpeakEvents::eHello_9)
        {
            if (word_5C3018 || !CanRespond_4770B0())
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
            if (sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                AddAlerted();
                word_5C3018 = 1;
                field_16C |= 2u;
                field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
                return AI_Scrub::eState2_StandUp_3;
            }
        }

        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }
        return AI_Scrub::eState2_SmallBreak_1;
    }

    case AI_Scrub::eState2_Duck_6:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
        field_194_timer = Math_NextRandom() % 64 + sGnFrame_5C1B84 + 15;
        field_124 = Math_NextRandom() % 64 + sGnFrame_5C1B84 + 240;
        if (field_10C_health > FP_FromInteger(0))
        {
            field_10C_health = FP_FromInteger(1);
        }
        return AI_Scrub::eState2_Scrubbing_2;

    case AI_Scrub::eState2_DuckKnockback_7:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::M_DuckKnockback_55_474250;
        field_194_timer = sGnFrame_5C1B84 + 60;
        return AI_Scrub::eState2_Duck_6;

    default:
        return field_190_sub_state;
    }
}

__int16 Mudokon::AI_TurnWheel_3_47E0D0()
{
    if (CheckForPortal_4775E0())
    {
        AddAlerted();
        field_18E_ai_state = Mud_AI_State::AI_Escape_6_47A560;
        field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
        return AI_Escape::eState6_PortalOppened_0;
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
    if (field_16C & 2)
    {
        if (field_190_sub_state != AI_TurnWheel::eState3_InterruptAction_2)
        {
            word_5C3018 = 0;
            field_16C &= ~2u;
        }
    }

    if (lastSpeak == GameSpeakEvents::Slig_LookOut_6 || Event_Get_422C00(kEventShooting))
    {
        if (field_190_sub_state != AI_TurnWheel::eState3_TurningWheelToDuck_3 && field_190_sub_state != AI_TurnWheel::eState3_Duck_4)
        {
            field_190_sub_state = AI_TurnWheel::eState3_TurningWheelToDuck_3;
        }
    }

    BaseAnimatedWithPhysicsGameObject* pMudAbuseEvent = Event_Is_Event_In_Range_422C30(
        kEventMudokonAbuse,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale);

    if (pMudAbuseEvent)
    {
        if (pMudAbuseEvent != this
            && field_190_sub_state != AI_TurnWheel::eState3_InterruptAction_2
            && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                field_B8_xpos,
                field_BC_ypos,
                0))
        {
            field_188_pTblEntry = Mudokon::ResponseTo_471730(field_180_emo_tbl, MudAction::eMudAbuse_9);
            if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags::eBit11_get_depressed))
            {
                field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
            }

            if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
            {
                AddAlerted();

                if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    field_16A_flags.Set(Flags::eBit6_standing_for_sad_or_angry);
                }

                field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
                field_190_sub_state = AI_TurnWheel::eState3_InterruptAction_2;
                field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
            }
        }
    }

    BaseAnimatedWithPhysicsGameObject* pLoudNoiseEvent = Event_Is_Event_In_Range_422C30(
        kEventLoudNoise,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale);
    if (pLoudNoiseEvent)
    {
        if (pLoudNoiseEvent->field_4_typeId == Types::eGlukkon_67
            && field_190_sub_state != AI_TurnWheel::eState3_InterruptAction_2
            && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                field_B8_xpos,
                field_BC_ypos,
                0))
        {

            if (field_16A_flags.Get(Flags::eBit13))
            {
                field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_16);
            }
            else
            {
                field_16A_flags.Set(Flags::eBit13);
                field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_15);
            }

            if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags::eBit11_get_depressed))
            {
                field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
            }

            if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
            {
                AddAlerted();

                if (field_180_emo_tbl == Mud_Emotion::eSad_3)
                {
                    field_16A_flags.Set(Flags::eBit6_standing_for_sad_or_angry);
                }

                field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
                field_190_sub_state = AI_TurnWheel::eState3_InterruptAction_2;
                field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
            }
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (LaughingGasInCurrentScreen_4774A0())
        {
            field_180_emo_tbl = Mud_Emotion::eWired_6;
            AddAlerted();
            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
            field_190_sub_state = AI_TurnWheel::eState3_InterruptAction_2;
        }
    }

    switch (field_190_sub_state)
    {
    case AI_TurnWheel::eState3_TurningWheel_0:
        if (lastSpeak == GameSpeakEvents::eHello_9)
        {
            if (word_5C3018 || !CanRespond_4770B0())
            {
                return TurningWheelHelloOrAllYaResponse();
            }
        }
        else if (lastSpeak != GameSpeakEvents::eAllYa_23)
        {
            return TurningWheelHelloOrAllYaResponse();
        }

        if (sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
        {
            AddAlerted();
            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
            word_5C3018 = 1;
            field_16C |= 2u;
            return AI_TurnWheel::eState3_InterruptAction_2;
        }

        return TurningWheelHelloOrAllYaResponse();

    case AI_TurnWheel::eState3_SittingDown_1:
        if (field_194_timer <= static_cast<int>(sGnFrame_5C1B84))
        {
            return field_190_sub_state;
        }

        if (field_106_current_motion == Mud_Motion::M_Idle_0_4724E0)
        {
            field_108_next_motion = Mud_Motion::M_StandToCrouch_17_474120;
        }
        else
        {
            if (field_106_current_motion != Mud_Motion::M_StandToCrouch_17_474120 && field_106_current_motion != Mud_Motion::M_CrouchIdle_15_474040)
            {
                field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            }

            if (field_106_current_motion == Mud_Motion::M_Idle_0_4724E0)
            {
                field_108_next_motion = Mud_Motion::M_StandToCrouch_17_474120;
            }
        }

        if (field_106_current_motion != Mud_Motion::M_CrouchIdle_15_474040)
        {
            return field_190_sub_state;
        }

        if (field_10_resources_array.ItemAt(2))
        {
            field_18E_ai_state = Mud_AI_State::AI_Chisel_1_47C5F0;
        }
        else
        {
            field_18E_ai_state = Mud_AI_State::AI_Scrub_2_47D270;
        }
        return AI_TurnWheel::eState3_TurningWheel_0;

    case AI_TurnWheel::eState3_InterruptAction_2:
        field_16A_flags.Clear(Flags::eBit7_stopped_at_wheel);
        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer || field_106_current_motion)
        {
            if (field_108_next_motion != -1 || field_106_current_motion != Mud_Motion::M_TurnWheelLoop_58_474CC0)
            {
                return field_190_sub_state;
            }
            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            return field_190_sub_state;
        }
        else
        {
            field_18E_ai_state = Mud_AI_State::AI_ListeningToAbe_4_477B40;
            word_5C3018 = 0;
            field_16C &= ~2u;
            return AI_ListeningToAbe::eState4_Inactive_0;
        }
        break;

    case AI_TurnWheel::eState3_TurningWheelToDuck_3:
        if (field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
        {
            if (field_106_current_motion != Mud_Motion::M_CrouchIdle_15_474040)
            {
                if (field_106_current_motion != Mud_Motion::M_TurnWheelLoop_58_474CC0)
                {
                    return field_190_sub_state;
                }
                else
                {
                    field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
                    return field_190_sub_state;
                }
            }
            else
            {
                field_108_next_motion = Mud_Motion::M_Duck_53_474A40;
                field_194_timer = sGnFrame_5C1B84 + 60;
                return AI_TurnWheel::eState3_Duck_4;
            }
        }
        else
        {
            field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
            return field_190_sub_state;
        }
        break;

    case AI_TurnWheel::eState3_Duck_4:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
        return AI_TurnWheel::eState3_DuckToTurningWheel_5;

    case AI_TurnWheel::eState3_DuckToTurningWheel_5:
        if (field_106_current_motion == Mud_Motion::M_Idle_0_4724E0)
        {
            field_108_next_motion = Mud_Motion::M_TurnWheelBegin_57_474C00;
            return field_190_sub_state;
        }

        if (field_106_current_motion == Mud_Motion::M_CrouchIdle_15_474040)
        {
            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            return field_190_sub_state;
        }
        else
        {
            if (field_106_current_motion != Mud_Motion::M_TurnWheelLoop_58_474CC0)
            {
                return field_190_sub_state;
            }
            return AI_TurnWheel::eState3_TurningWheel_0;
        }
        break;
    }
    return field_190_sub_state;
}

__int16 Mudokon::AI_ListeningToAbe_4_Real_477B40()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::AI_ListeningToAbe_4_477B40()
{
    if (CheckForPortal_4775E0())
    {
        field_18E_ai_state = Mud_AI_State::AI_Escape_6_47A560;
        field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
        return AI_Escape::eState6_PortalOppened_0;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventMudokonAbuse,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eMudAbuse_9;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventMudokonDied,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eMudDied_14;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventMudokonComfort,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eComfort_10;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventMudokonLaugh,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eLaugh_12;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventShooting | kEventSpeaking,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eDuck_13;
    }

    BaseAnimatedWithPhysicsGameObject* pNoiseEvent = Event_Is_Event_In_Range_422C30(
        kEventLoudNoise,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale);

    if (pNoiseEvent)
    {
        if (pNoiseEvent->field_4_typeId == Types::eGlukkon_67)
        {
            if (field_16A_flags.Get(Flags::eBit13))
            {
                field_17E_delayed_speak = MudAction::eUnknown_16;
            }
            else
            {
                field_16A_flags.Set(Flags::eBit13);
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
        if (SwitchStates_Get_466020(field_120_angry_trigger))
        {
            field_128_angry_timer = sGnFrame_5C1B84 + 15;
        }
    }

    if (!field_16A_flags.Get(Flags::eBit8_do_angry))
    {
        if (field_128_angry_timer > 0)
        {
            if (field_128_angry_timer <= static_cast<int>(sGnFrame_5C1B84))
            {
                field_16A_flags.Set(Flags::eBit8_do_angry);

                if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos - FP_FromInteger(5)),
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos - FP_FromInteger(5)),
                    TlvTypes::Water_77))
                {
                    field_180_emo_tbl = Mud_Emotion::eAngry_1;
                    field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
                    field_190_sub_state = AI_ListeningToAbe::eState4_Knockback_14;
                    field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
                }
            }
        }
    }

    switch (field_190_sub_state)
    {
    case AI_ListeningToAbe::eState4_Inactive_0:
        return AI_ListeningToAbe_State_0();

    case AI_ListeningToAbe::eState4_GetsAttentive_1:
        return AI_ListeningToAbe_State_1();

    case AI_ListeningToAbe::eState4_CrazyFollowMe_2:
        return AI_ListeningToAbe_State_2();

    case AI_ListeningToAbe::eState4_SteppingBack_3:
        return AI_ListeningToAbe_State_3();

    case AI_ListeningToAbe::eState4_FollowingIdle_4:
        return AI_ListeningToAbe_State_4();

    case AI_ListeningToAbe::eState4_Walking_5:
        return AI_ListeningToAbe_State_5();

    case AI_ListeningToAbe::eState4_Running_6:
        return AI_ListeningToAbe_State_6();

    case AI_ListeningToAbe::eState4_Idle_7:
        return AI_ListeningToAbe_State_7();

    case AI_ListeningToAbe::eState4_CrazySlideTurn_8:
        return AI_ListeningToAbe_State_8();

    case AI_ListeningToAbe::eState4_GetsWaitWhileMoving_9:
        return AI_ListeningToAbe_State_9();

    case AI_ListeningToAbe::eState4_InitializeMovement_10:
        return AI_ListeningToAbe_State_10();

    case AI_ListeningToAbe::eState4_PullingLever_11:
        return AI_ListeningToAbe_State_11();

    case AI_ListeningToAbe::eState4_GetsCommand_12:
        return AI_ListeningToAbe_State_12();

    case AI_ListeningToAbe::eState4_Slapped_13:
        return AI_ListeningToAbe_State_13();

    case AI_ListeningToAbe::eState4_Knockback_14:
        return AI_ListeningToAbe_State_14();

    case AI_ListeningToAbe::eState4_GettingSorry_15:
        return AI_ListeningToAbe_State_15();

    case AI_ListeningToAbe::eState4_Suiciding_16:
        return AI_ListeningToAbe_State_16();

    case AI_ListeningToAbe::eState4_RageSlap_17:
        return AI_ListeningToAbe_State_17();

    case AI_ListeningToAbe::eState4_RageTurn_18:
        return AI_ListeningToAbe_State_18();

    case AI_ListeningToAbe::eState4_CrazyDeny_19:
        return AI_ListeningToAbe_State_19();

    case AI_ListeningToAbe::eState4_StopsWhileMoving_20:
        return AI_ListeningToAbe_State_20();

    case AI_ListeningToAbe::eState4_CrazyLaugh_21:
        return AI_ListeningToAbe_State_21();

    case AI_ListeningToAbe::eState4_LostAttention_22:
        return AI_ListeningToAbe_State_22();

    default:
        return field_190_sub_state;
    }
}

__int16 Mudokon::AI_ListeningToAbe_State_0()
{
    field_108_next_motion = -1;
    ToStand_4724A0();
    field_178_sub_state2 = -1;
    return AI_ListeningToAbe::eState4_GetsAttentive_1;
}

__int16 Mudokon::AI_ListeningToAbe_State_1()
{
    if (field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
    {
        return field_190_sub_state;
    }

    if (!vIsFacingMe_4254A0(sActiveHero_5C1B68))
    {
        field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
        return field_190_sub_state;
    }

    switch (field_180_emo_tbl)
    {
    case Mud_Emotion::eNormal_0:
        Sound_475EC0(MudSounds::eHelloNeutral_3);
        break;

    case Mud_Emotion::eAngry_1:
    case Mud_Emotion::eAggressive_2:
        if (field_16A_flags.Get(Flags::eBit6_standing_for_sad_or_angry))
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
        if (field_16A_flags.Get(Flags::eBit6_standing_for_sad_or_angry))
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

    field_108_next_motion = Mud_Motion::M_Speak_4_472FA0;
    if (!field_16A_flags.Get(Flags::eBit6_standing_for_sad_or_angry) || field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        return AI_ListeningToAbe::eState4_Idle_7;
    }

    field_16A_flags.Clear(Flags::eBit6_standing_for_sad_or_angry);
    field_16A_flags.Clear(Flags::eBit12_alert_enemies);

    if (field_10_resources_array.ItemAt(2))
    {
        field_184_next_motion2 = Mud_Motion::M_Chisel_11_4732D0;
    }
    else
    {
        field_184_next_motion2 = Mud_Motion::M_CrouchIdle_15_474040;
    }
    field_178_sub_state2 = AI_ListeningToAbe::eState4_LostAttention_22;
    field_16A_flags.Clear(Flags::eBit5_following);
    return AI_ListeningToAbe::eState4_InitializeMovement_10;
}

__int16 Mudokon::AI_ListeningToAbe_State_2()
{
    if (!vIsFacingMe_4254A0(sActiveHero_5C1B68))
    {
        field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
        return field_190_sub_state;
    }

    if (field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
    {
        if (field_106_current_motion == Mud_Motion::M_WalkLoop_1_4728B0)
        {
            if (field_16A_flags.Get(Flags::eBit4_blind) ||
                !vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68))
            {
                return AI_ListeningToAbe::eState4_Walking_5;
            }
        }
        else
        {
            if (field_106_current_motion != Mud_Motion::M_Knockback_46_4742A0)
            {
                return field_190_sub_state;
            }
        }
        field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
        return AI_ListeningToAbe::eState4_FollowingIdle_4;
    }
    else
    {
        field_108_next_motion = Mud_Motion::M_WalkLoop_1_4728B0;
    }
    return field_190_sub_state;
}

__int16 Mudokon::AI_ListeningToAbe_State_3()
{
    if (field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
    {
        if (field_106_current_motion != Mud_Motion::M_WalkLoop_1_4728B0)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;

        if (field_16A_flags.Get(Flags::eBit5_following))
        {
            //TODO possibly wrong enum value
            LOG_WARNING("bit5 to state 4 wrong?");
            return AI_ListeningToAbe::eState4_Walking_5;
        }
        else
        {
            LOG_WARNING("not bit5 to state 7 wrong?");
            return AI_ListeningToAbe::eState4_Idle_7;
        }
    }
    else
    {
        if (vIsFacingMe_4254A0(sActiveHero_5C1B68))
        {
            field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::M_WalkLoop_1_4728B0;
        return field_190_sub_state;
    }
}

__int16 Mudokon::AI_ListeningToAbe_State_4()
{
    if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && field_180_emo_tbl != Mud_Emotion::eHappy_5 && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        field_16A_flags.Clear(Flags::eBit5_following);
        return AI_ListeningToAbe::eState4_Idle_7;
    }
    else
    {
        if (field_106_current_motion == Mud_Motion::M_RunLoop_21_473720)
        {
            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
        }
        if (field_BC_ypos > pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(260) && field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
            if (field_10_resources_array.ItemAt(2))
            {
                field_16A_flags.Clear(Flags::eBit5_following);
                field_108_next_motion = Mud_Motion::M_Chisel_11_4732D0;
                return AI_ListeningToAbe::eState4_LostAttention_22;
            }
            else
            {
                field_16A_flags.Clear(Flags::eBit5_following);
                field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
                return AI_ListeningToAbe::eState4_LostAttention_22;
            }
        }

        FP scaleToGridSizeAbs = {};
        if (sActiveHero_5C1B68->field_B8_xpos >= field_B8_xpos)
        {
            scaleToGridSizeAbs = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            scaleToGridSizeAbs = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        const int v44 = WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), scaleToGridSizeAbs);

        if (field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
        {
            field_17C_stand_idle_timer = 0;
            field_124 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 60);
            return field_190_sub_state;
        }
        else
        {
            field_16A_flags.Clear(Flags::eBit7_stopped_at_wheel);
            if (++field_17C_stand_idle_timer <= 540 || field_180_emo_tbl == Mud_Emotion::eWired_6)
            {
                if (NeedToTurnAround())
                {
                    field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
                    return field_190_sub_state;
                }

                const FP v48 = ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2);
                if ((vIsObjNearby_4253B0(v48, sActiveHero_5C1B68) && !(field_16A_flags.Get(Flags::eBit4_blind))) || v44 || FindWheel_4777B0(field_B8_xpos, field_BC_ypos))
                {
                    const GameSpeakEvents lastSpeak_1 = LastGameSpeak_476FF0();
                    if (field_17E_delayed_speak != MudAction::eNone_17)
                    {
                        __int16 v18 = GetAISubStateResponse_477AF0(field_17E_delayed_speak);
                        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
                        {
                            field_194_timer = sGnFrame_5C1B84 + 20;
                        }
                        else
                        {
                            field_194_timer = (unsigned __int16)StableDelay_477570() + sGnFrame_5C1B84 + 20;
                        }
                        field_178_sub_state2 = field_190_sub_state;
                        field_17E_delayed_speak = MudAction::eNone_17;
                        if (!v18)
                        {
                            return field_190_sub_state;
                        }
                        return v18;
                    }

                    if (lastSpeak_1 == GameSpeakEvents::eNone_m1)
                    {
                        if (field_180_emo_tbl != Mud_Emotion::eWired_6 || field_124 > (signed int)sGnFrame_5C1B84)
                        {
                            return field_190_sub_state;
                        }
                        field_194_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
                        field_178_sub_state2 = field_190_sub_state;
                        return AI_ListeningToAbe::eState4_CrazyLaugh_21;
                    }
                    else
                    {
                        field_178_sub_state2 = field_190_sub_state;
                        field_194_timer = (unsigned __int16)StableDelay_477570() + sGnFrame_5C1B84 + 20;
                        switch (lastSpeak_1)
                        {
                        case GameSpeakEvents::eFart_3:
                        {
                            __int16 v18 = GetAISubStateResponse_477AF0(MudAction::eFart_6);
                            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68))
                            {
                                if (vOnSameYLevel_425520(sActiveHero_5C1B68))
                                {
                                    field_178_sub_state2 = AI_ListeningToAbe::eState4_SteppingBack_3;
                                }
                            }
                            if (!v18)
                            {
                                return field_190_sub_state;
                            }
                            return v18;
                        }

                        case GameSpeakEvents::eHello_9:
                        case GameSpeakEvents::eAllYa_23:
                        {
                            __int16 result = GetAISubStateResponse_477AF0(MudAction::eHelloOrAllYa_0);
                            if (result)
                            {
                                return result;
                            }
                            return field_190_sub_state;
                        }

                        case GameSpeakEvents::eFollowMe_10:
                        {
                            if (v44)
                            {
                                return AI_ListeningToAbe::eState4_CrazyDeny_19;
                            }
                            __int16 result = GetAISubStateResponse_477AF0(MudAction::eFollowMe_1);
                            field_178_sub_state2 = AI_ListeningToAbe::eState4_CrazyFollowMe_2;
                            if (result)
                            {
                                return result;
                            }
                            return field_190_sub_state;
                        }

                        case GameSpeakEvents::eAnger_11:
                        case GameSpeakEvents::eSorry_24:
                            field_17C_stand_idle_timer = 0;
                            return field_190_sub_state;

                        case GameSpeakEvents::eWait_12:
                        {
                            if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                            {
                                return AI_ListeningToAbe::eState4_CrazyDeny_19;
                            }
                            field_16A_flags.Clear(Flags::eBit5_following);
                            __int16 result = GetAISubStateResponse_477AF0(MudAction::eWait_2);
                            field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
                            if (result)
                            {
                                return result;
                            }
                            return field_190_sub_state;
                        }

                        case GameSpeakEvents::eWork_21:
                            if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                            {
                                return AI_ListeningToAbe::eState4_CrazyDeny_19;
                            }
                            field_16A_flags.Clear(Flags::eBit5_following);
                            field_182 = GameSpeakEvents::eNone_m1;
                            if (FindWheel_4777B0(field_B8_xpos, field_BC_ypos))
                            {
                                Mudokon* v35 = static_cast<Mudokon*>(FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
                                if (!v35)
                                {
                                    return AIStartWheelTurning();
                                }

                                while (!(v35->field_16A_flags.Get(Flags::eBit7_stopped_at_wheel)) && v35->field_106_current_motion != Mud_Motion::M_TurnWheelLoop_58_474CC0)
                                {
                                    v35 = static_cast<Mudokon*>(GetStackedSlapTarget_425290(v35->field_8_object_id, Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
                                    if (!v35)
                                    {
                                        return AIStartWheelTurning();
                                    }
                                }
                                if (v35)
                                {
                                    if (field_10_resources_array.ItemAt(2))
                                    {
                                        field_108_next_motion = Mud_Motion::M_Chisel_11_4732D0;
                                        return AI_ListeningToAbe::eState4_LostAttention_22;
                                    }
                                    else
                                    {
                                        field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
                                        return AI_ListeningToAbe::eState4_LostAttention_22;
                                    }
                                }
                                return AIStartWheelTurning();
                            }

                            if (FindObjectOfType_425180(Types::eLever_139, ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos, field_BC_ypos - FP_FromInteger(5)))
                            {
                                return AI_ListeningToAbe::eState4_PullingLever_11;
                            }
                            if (FindObjectOfType_425180(Types::eLever_139, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
                            {
                                return AI_ListeningToAbe::eState4_PullingLever_11;
                            }

                            if (field_10_resources_array.ItemAt(2))
                            {
                                field_108_next_motion = Mud_Motion::M_Chisel_11_4732D0;
                                return AI_ListeningToAbe::eState4_LostAttention_22;
                            }
                            else
                            {
                                field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
                                return AI_ListeningToAbe::eState4_LostAttention_22;
                            }
                            break;

                        case GameSpeakEvents::eStopIt_22:
                        {
                            __int16 v18 = GetAISubStateResponse_477AF0(MudAction::eStopIt_4);
                            field_194_timer = ((unsigned int)(unsigned __int16)StableDelay_477570() >> 1) + sGnFrame_5C1B84 + 20;
                            if (!v18)
                            {
                                return field_190_sub_state;
                            }
                            return v18;
                        }

                        default:
                            return field_190_sub_state;
                        }
                    }
                }
                else
                {
                    field_194_timer = sGnFrame_5C1B84 + (unsigned __int16)Mudokon::StableDelay_477570();
                    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                    {
                        field_184_next_motion2 = Mud_Motion::M_RunLoop_21_473720;
                        field_178_sub_state2 = AI_ListeningToAbe::eState4_Running_6;
                        return AI_ListeningToAbe::eState4_InitializeMovement_10;
                    }
                    else if (field_16A_flags.Get(Flags::eBit4_blind) || !IsMotionUnknown_4730F0())
                    {
                        field_184_next_motion2 = Mud_Motion::M_WalkLoop_1_4728B0;
                        field_178_sub_state2 = AI_ListeningToAbe::eState4_Walking_5;
                        return AI_ListeningToAbe::eState4_InitializeMovement_10;
                    }
                    else
                    {
                        field_184_next_motion2 = Mud_Motion::M_SneakLoop_27_473C40;
                        field_178_sub_state2 = AI_ListeningToAbe::eState4_Walking_5;
                        return AI_ListeningToAbe::eState4_InitializeMovement_10;
                    }
                }
            }
            else
            {
                field_16A_flags.Clear(Flags::eBit5_following);
                if (!field_10_resources_array.ItemAt(2))
                {
                    field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
                    return AI_ListeningToAbe::eState4_LostAttention_22;
                }
                else
                {
                    field_108_next_motion = Mud_Motion::M_Chisel_11_4732D0;
                    return AI_ListeningToAbe::eState4_LostAttention_22;
                }
            }
        }
    }
}

__int16 Mudokon::AI_ListeningToAbe_State_5()
{
    if (field_106_current_motion == Mud_Motion::M_Idle_0_4724E0 && field_108_next_motion != Mud_Motion::M_WalkLoop_1_4728B0)
    {
        if (!field_16A_flags.Get(Flags::eBit4_blind))
        {
            return AI_ListeningToAbe::eState4_FollowingIdle_4;
        }
        field_108_next_motion = Mud_Motion::M_WalkLoop_1_4728B0;
    }

    if (field_106_current_motion == Mud_Motion::M_Knockback_46_4742A0)
    {
        Sound_475EC0(MudSounds::eHurt1_16);
        __int16 result = GetAISubStateResponse_477AF0(MudAction::eSmackIntoWall_11);
        field_17E_delayed_speak = MudAction::eNone_17;
        field_194_timer = sGnFrame_5C1B84 + 20;
        if (field_16A_flags.Get(Flags::eBit4_blind))
        {
            field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
            field_16A_flags.Clear(Flags::eBit5_following);
        }
        else
        {
            field_178_sub_state2 = AI_ListeningToAbe::eState4_FollowingIdle_4;
        }
        if (result)
        {
            return result;
        }
    }

    if (field_106_current_motion != Mud_Motion::M_WalkLoop_1_4728B0 && field_106_current_motion != Mud_Motion::M_SneakLoop_27_473C40)
    {
        return field_190_sub_state;
    }

    FP v65 = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        v65 = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }
    else
    {
        v65 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }

    const int v67 = WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), v65);
    if (field_16A_flags.Get(Flags::eBit4_blind))
    {
        if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0)
            && !vIsFacingMe_4254A0(sActiveHero_5C1B68))
        {
            const int v68 = FP_GetExponent(field_B8_xpos) % 375;
            if (v68 > 10u && v68 < 362u)
            {
                field_16A_flags.Clear(Flags::eBit5_following);
                field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
                return AI_ListeningToAbe::eState4_Idle_7;
            }
        }
    }
    if (v67 && !field_16A_flags.Get(Flags::eBit4_blind))
    {
        field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
        return AI_ListeningToAbe::eState4_FollowingIdle_4;
    }

    if (StopAtWheel_477880())
    {
        field_16A_flags.Set(Flags::eBit7_stopped_at_wheel);
        field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
        return AI_ListeningToAbe::eState4_FollowingIdle_4;
    }

    if (!vIsFacingMe_4254A0(sActiveHero_5C1B68) && !field_16A_flags.Get(Flags::eBit4_blind))
    {
        field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
        return AI_ListeningToAbe::eState4_FollowingIdle_4;
    }

    if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2), sActiveHero_5C1B68) && !field_16A_flags.Get(Flags::eBit4_blind))
    {
        field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
        return AI_ListeningToAbe::eState4_FollowingIdle_4;
    }

    if (LastGameSpeak_476FF0() == GameSpeakEvents::eWait_12)
    {
        field_16A_flags.Clear(Flags::eBit5_following);
        field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
        return AI_ListeningToAbe::eState4_GetsWaitWhileMoving_9;
    }

    bool bWalkOrSneak = false;
    if (sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_33_RunLoop_4508E0 &&
        sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_23_RollLoop_453A90 &&
        field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        bWalkOrSneak = true;
    }
    else if (field_106_current_motion == Mud_Motion::M_SneakLoop_27_473C40 || field_16A_flags.Get(Flags::eBit4_blind))
    {
        field_108_next_motion = Mud_Motion::M_WalkLoop_1_4728B0;
        bWalkOrSneak = true;
    }

    if (bWalkOrSneak)
    {
        if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_40_SneakLoop_450550
            && field_106_current_motion == Mud_Motion::M_WalkLoop_1_4728B0
            && !field_16A_flags.Get(Flags::eBit4_blind))
        {
            field_108_next_motion = Mud_Motion::M_SneakLoop_27_473C40;
        }
        if (sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_1_WalkLoop_44FBA0 || field_106_current_motion != Mud_Motion::M_SneakLoop_27_473C40)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::M_WalkLoop_1_4728B0;
        return field_190_sub_state;
    }
    else
    {
        field_108_next_motion = Mud_Motion::M_RunLoop_21_473720;
        return AI_ListeningToAbe::eState4_Running_6;
    }
}

__int16 Mudokon::AI_ListeningToAbe_State_6()
{
    if (field_106_current_motion == Mud_Motion::M_Idle_0_4724E0)
    {
        if (!field_16A_flags.Get(Flags::eBit4_blind))
        {
            return AI_ListeningToAbe::eState4_FollowingIdle_4;
        }
        field_108_next_motion = Mud_Motion::M_RunLoop_21_473720;
    }

    if (field_106_current_motion == Mud_Motion::M_Knockback_46_4742A0)
    {
        Sound_475EC0(MudSounds::eHurt1_16);
        __int16 result = GetAISubStateResponse_477AF0(MudAction::eSmackIntoWall_11);
        field_17E_delayed_speak = MudAction::eNone_17;
        field_194_timer = sGnFrame_5C1B84 + 20;
        if (field_16A_flags.Get(Flags::eBit4_blind))
        {
            field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
            field_16A_flags.Clear(Flags::eBit5_following);
        }
        else
        {
            field_178_sub_state2 = AI_ListeningToAbe::eState4_FollowingIdle_4;
        }
        if (result > 0)
        {
            return result;
        }
    }
    if (field_106_current_motion != Mud_Motion::M_RunLoop_21_473720)
    {
        return field_190_sub_state;
    }

    FP scaleToGridSizeAbs;
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        scaleToGridSizeAbs = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }
    else
    {
        scaleToGridSizeAbs = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), scaleToGridSizeAbs * FP_FromInteger(3)) && !field_16A_flags.Get(Flags::eBit4_blind))
    {
        field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
        if (field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
            return AI_ListeningToAbe::eState4_FollowingIdle_4;
        }
        field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;

        if (Math_NextRandom() < 20)
        {
            return AI_ListeningToAbe::eState4_FollowingIdle_4;
        }
        field_178_sub_state2 = AI_ListeningToAbe::eState4_FollowingIdle_4;
        return AI_ListeningToAbe::eState4_CrazyLaugh_21;
    }

    bool gotoTurn = false;
    if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_40_SneakLoop_450550 || field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        if (field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
            field_108_next_motion = Mud_Motion::M_WalkLoop_1_4728B0;
            return AI_ListeningToAbe::eState4_Walking_5;
        }
        if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_25_RunSlideStop_451330 ||
            sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_0_Idle_44EEB0 ||
            sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_71_Knockback_455090 ||
            sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_17_CrouchIdle_456BC0)
        {
            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
            if (Math_NextRandom() < 40)
            {
                return AI_ListeningToAbe::eState4_FollowingIdle_4;
            }
            field_178_sub_state2 = AI_ListeningToAbe::eState4_FollowingIdle_4;
            return AI_ListeningToAbe::eState4_CrazyLaugh_21;
        }
        gotoTurn = true;
    }

    if (!gotoTurn)
    {
        if (sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_33_RunLoop_4508E0)
        {
            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(4), sActiveHero_5C1B68))
            {
                field_108_next_motion = Mud_Motion::M_WalkLoop_1_4728B0;
                return AI_ListeningToAbe::eState4_Walking_5;
            }
        }

        if (LastGameSpeak_476FF0() == GameSpeakEvents::eWait_12)
        {
            field_16A_flags.Clear(Flags::eBit5_following);
            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            return AI_ListeningToAbe::eState4_GetsWaitWhileMoving_9;
        }
    }

    if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68))
    {
        if (vIsFacingMe_4254A0(sActiveHero_5C1B68))
        {
            return field_190_sub_state;
        }
    }
    else
    {
        if (!vIsObj_GettingNear_425420(sActiveHero_5C1B68))
        {
            if (vIsFacingMe_4254A0(sActiveHero_5C1B68))
            {
                return field_190_sub_state;
            }
        }
    }

    if (!field_16A_flags.Get(Flags::eBit4_blind))
    {
        field_108_next_motion = Mud_Motion::M_RunTurn_25_473AA0;
        return AI_ListeningToAbe::eState4_CrazySlideTurn_8;
    }

    return field_190_sub_state;
}

__int16 Mudokon::AI_ListeningToAbe_State_7()
{
    if (field_BC_ypos > pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(260) && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (field_10_resources_array.ItemAt(2))
        {
            field_16A_flags.Clear(Flags::eBit5_following);
            field_108_next_motion = Mud_Motion::M_Chisel_11_4732D0;
            return AI_ListeningToAbe::eState4_LostAttention_22;
        }
        else
        {
            field_16A_flags.Clear(Flags::eBit5_following);
            field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
            return AI_ListeningToAbe::eState4_LostAttention_22;
        }
    }

    if (field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
    {
        field_17C_stand_idle_timer = 0;
        field_124 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 60);
        return field_190_sub_state;
    }

    if (field_180_emo_tbl == Mud_Emotion::eSuicidal_4)
    {
        field_16A_flags.Clear(Flags::eBit5_following);
        return AI_ListeningToAbe::eState4_Suiciding_16;
    }

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        field_16A_flags.Set(Flags::eBit5_following);
        return AI_ListeningToAbe::eState4_FollowingIdle_4;
    }

    if ((++field_17C_stand_idle_timer > 540 || Is_In_Current_Camera_424A70() != CameraPos::eCamCurrent_0) && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (field_10_resources_array.ItemAt(2))
        {
            field_16A_flags.Clear(Flags::eBit5_following);
            field_108_next_motion = Mud_Motion::M_Chisel_11_4732D0;
            return AI_ListeningToAbe::eState4_LostAttention_22;
        }
        else
        {
            field_16A_flags.Clear(Flags::eBit5_following);
            field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
            return AI_ListeningToAbe::eState4_LostAttention_22;
        }
    }

    if (NeedToTurnAround())
    {
        field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
        return field_190_sub_state;
    }
    
    if (field_17E_delayed_speak != MudAction::eNone_17)
    {
        __int16 v18 = GetAISubStateResponse_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
        {
            field_194_timer = sGnFrame_5C1B84 + 20;
            field_178_sub_state2 = field_190_sub_state;
        }
        else
        {
            field_178_sub_state2 = field_190_sub_state;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
        }
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return field_190_sub_state;
        }
        return v18;
    }
    GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
    if (lastSpeak == GameSpeakEvents::eNone_m1
        && ((lastSpeak = field_182, lastSpeak == GameSpeakEvents::eNone_m1)
            || (field_180_emo_tbl != Mud_Emotion::eNormal_0 && field_180_emo_tbl != Mud_Emotion::eHappy_5 && field_180_emo_tbl != Mud_Emotion::eWired_6)))
    {
        if (field_180_emo_tbl != Mud_Emotion::eWired_6 || field_124 > (signed int)sGnFrame_5C1B84)
        {
            return field_190_sub_state;
        }
        field_194_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
        field_178_sub_state2 = field_190_sub_state;
        return AI_ListeningToAbe::eState4_CrazyLaugh_21;
    }
    else
    {
        field_178_sub_state2 = field_190_sub_state;
        field_194_timer = (unsigned __int16)StableDelay_477570() + sGnFrame_5C1B84 + 20;
        switch (lastSpeak)
        {
        case GameSpeakEvents::eFart_3:
        {
            field_182 = GameSpeakEvents::eNone_m1;
            const __int16 v18 = GetAISubStateResponse_477AF0(MudAction::eFart_6);
            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68))
            {
                if (vOnSameYLevel_425520(sActiveHero_5C1B68))
                {
                    field_178_sub_state2 = AI_ListeningToAbe::eState4_SteppingBack_3;
                }
            }
            if (!v18)
            {
                return field_190_sub_state;
            }
            return v18;
        }

        case GameSpeakEvents::eHello_9:
        case GameSpeakEvents::eAllYa_23:
        {
            field_182 = GameSpeakEvents::eNone_m1;
            __int16 result = GetAISubStateResponse_477AF0(MudAction::eHelloOrAllYa_0);
            if (result)
            {
                return result;
            }
            return field_190_sub_state;
        }

        case GameSpeakEvents::eFollowMe_10:
        {
            const __int16 v26 = GetAISubStateResponse_477AF0(MudAction::eFollowMe_1);
            if (field_188_pTblEntry->field_0_sound == MudSounds::eOkay_12 || field_188_pTblEntry->field_0_sound == MudSounds::eLaugh_10)
            {
                field_16A_flags.Set(Flags::eBit5_following);
                field_182 = GameSpeakEvents::eNone_m1;
                // TODO: Wrong ??
                if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2), sActiveHero_5C1B68) ||
                    vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68))
                {
                    field_178_sub_state2 = AI_ListeningToAbe::eState4_FollowingIdle_4;
                }
                else
                {
                    field_178_sub_state2 = AI_ListeningToAbe::eState4_CrazyFollowMe_2;
                }
            }
            else
            {
                field_182 = lastSpeak;
            }

            if (!v26)
            {
                return field_190_sub_state;
            }

            return v26;
        }

        case GameSpeakEvents::eAnger_11:
        case GameSpeakEvents::eSorry_24:
            field_17C_stand_idle_timer = 0;
            return field_190_sub_state;

        case GameSpeakEvents::eWait_12:
            field_16A_flags.Clear(Flags::eBit5_following);
            field_182 = GameSpeakEvents::eNone_m1;
            field_184_next_motion2 = Mud_Motion::M_DunnoBegin_43_472790;
            return AI_ListeningToAbe::eState4_InitializeMovement_10;

        case GameSpeakEvents::eWork_21:
            if (field_180_emo_tbl == Mud_Emotion::eWired_6)
            {
                return AI_ListeningToAbe::eState4_CrazyDeny_19;
            }
            field_16A_flags.Clear(Flags::eBit5_following);
            field_182 = GameSpeakEvents::eNone_m1;

            if (FindWheel_4777B0(field_B8_xpos, field_BC_ypos))
            {
                Mudokon* v35 = static_cast<Mudokon*>(FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
                if (!v35)
                {
                    return AIStartWheelTurning();
                }

                while (!(v35->field_16A_flags.Get(Flags::eBit7_stopped_at_wheel)) && v35->field_106_current_motion != Mud_Motion::M_TurnWheelLoop_58_474CC0)
                {
                    v35 = static_cast<Mudokon*>(GetStackedSlapTarget_425290(v35->field_8_object_id, Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
                    if (!v35)
                    {
                        return AIStartWheelTurning();
                    }
                }
                if (v35)
                {
                    if (field_10_resources_array.ItemAt(2))
                    {
                        field_108_next_motion = Mud_Motion::M_Chisel_11_4732D0;
                        return AI_ListeningToAbe::eState4_LostAttention_22;
                    }
                    else
                    {
                        field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
                        return AI_ListeningToAbe::eState4_LostAttention_22;
                    }
                }
                return AIStartWheelTurning();
            }

            if (FindObjectOfType_425180(Types::eLever_139, ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos, field_BC_ypos - FP_FromInteger(5)))
            {
                return AI_ListeningToAbe::eState4_PullingLever_11;
            }
            if (FindObjectOfType_425180(Types::eLever_139, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
            {
                return AI_ListeningToAbe::eState4_PullingLever_11;
            }

            break;

        case GameSpeakEvents::eStopIt_22:
        {
            __int16 v18 = GetAISubStateResponse_477AF0(MudAction::eStopIt_4);
            field_194_timer = ((unsigned int)(unsigned __int16)StableDelay_477570() >> 1) + sGnFrame_5C1B84 + 20;
            if (!v18)
            {
                return field_190_sub_state;
            }
            return v18;
        }

        default:
            return field_190_sub_state;
        }

        if (field_10_resources_array.ItemAt(2))
        {
            field_108_next_motion = Mud_Motion::M_Chisel_11_4732D0;
            return AI_ListeningToAbe::eState4_LostAttention_22;
        }
        else
        {
            field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
            return AI_ListeningToAbe::eState4_LostAttention_22;
        }
    }
}

__int16 Mudokon::AI_ListeningToAbe_State_8()
{
    if (field_106_current_motion == Mud_Motion::M_RunLoop_21_473720)
    {
        return AI_ListeningToAbe::eState4_Running_6;
    }
    if (field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
    {
        return field_190_sub_state;
    }
    return AI_ListeningToAbe::eState4_FollowingIdle_4;
}

__int16 Mudokon::AI_ListeningToAbe_State_9()
{
    if (field_106_current_motion)
    {
        return field_190_sub_state;
    }
    field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
    field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
    return AI_ListeningToAbe::eState4_StopsWhileMoving_20;
}

__int16 Mudokon::AI_ListeningToAbe_State_10()
{
    if (!vIsFacingMe_4254A0(sActiveHero_5C1B68))
    {
        field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
        return field_190_sub_state;
    }
    field_108_next_motion = -1;

    if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
    {
        return field_190_sub_state;
    }
    field_108_next_motion = field_184_next_motion2;
    return field_178_sub_state2;
}

__int16 Mudokon::AI_ListeningToAbe_State_11()
{
    BaseGameObject* pLever = nullptr;
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        pLever = FindObjectOfType_425180(Types::eLever_139, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5));
    }
    else
    {
        pLever = FindObjectOfType_425180(Types::eLever_139, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5));
    }

    if (!pLever)
    {
        field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
        return field_190_sub_state;
    }

    if (field_106_current_motion == Mud_Motion::M_LeverUse_10_473020)
    {
        field_108_next_motion = -1;
        auto pSwitch = static_cast<Switch*>(pLever);
        pSwitch->VPull_4D6050(field_B8_xpos < pSwitch->field_B8_xpos);
        field_16A_flags.Clear(Flags::eBit5_following);

        if (field_10_resources_array.ItemAt(2))
        {
            field_108_next_motion = Mud_Motion::M_Chisel_11_4732D0;
        }
        else
        {
            field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
        }
        return AI_ListeningToAbe::eState4_LostAttention_22;
    }
    else
    {
        field_108_next_motion = Mud_Motion::M_LeverUse_10_473020;
        return field_190_sub_state;
    }
}

__int16 Mudokon::AI_ListeningToAbe_State_12()
{
    if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eMudAbuse_9)
    {
        __int16 v18 = GetAISubStateResponse_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
        {
            field_194_timer = sGnFrame_5C1B84 + 20;
            field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
            field_17E_delayed_speak = MudAction::eNone_17;
            if (!v18)
            {
                return field_190_sub_state;
            }
            return v18;
        }
        field_194_timer = (unsigned __int16)(StableDelay_477570() + sGnFrame_5C1B84 + 20);
        field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return field_190_sub_state;
        }
        return v18;
    }

    if (field_188_pTblEntry->field_2_next_motion == Mud_Motion::M_Punch_38_474AA0)
    {
        if (LastGameSpeak_476FF0() == GameSpeakEvents::eStopIt_22)
        {
            if (field_106_current_motion == Mud_Motion::M_Punch_38_474AA0 || field_108_next_motion == Mud_Motion::M_Punch_38_474AA0)
            {
                ToStand_4724A0();
                field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
                field_108_next_motion = -1;
            }
            __int16 result = GetAISubStateResponse_477AF0(MudAction::eStopIt_4);
            field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
            field_17E_delayed_speak = MudAction::eNone_17;
            field_194_timer = sGnFrame_5C1B84 + 20;
            if (result)
            {
                return result;
            }
        }
    }

    if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
    {
        return field_190_sub_state;
    }

    if (field_188_pTblEntry->field_0_sound != MudSounds::eFartPuh_23 || field_178_sub_state2 == AI_ListeningToAbe::eState4_SteppingBack_3)
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

    field_108_next_motion = field_188_pTblEntry->field_2_next_motion;
    if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags::eBit11_get_depressed))
    {
        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
    }

    if (field_180_emo_tbl == Mud_Emotion::eSick_7)
    {
        field_18E_ai_state = Mud_AI_State::AI_Sick_9_47A910;
        return AI_ListeningToAbe::eState4_Inactive_0;
    }

    if (field_108_next_motion != Mud_Motion::M_Punch_38_474AA0)
    {
        return field_178_sub_state2;
    }

    BaseGameObject* pMudInSameGridBlock = nullptr;
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (FindObjectOfType_425180(Types::eAbe_69, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
        {
            return AI_ListeningToAbe::eState4_RageTurn_18;
        }
        pMudInSameGridBlock = FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5));
    }
    else
    {
        if (FindObjectOfType_425180(Types::eAbe_69, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
        {
            return AI_ListeningToAbe::eState4_RageTurn_18;
        }
        pMudInSameGridBlock = FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5));
    }

    if (!pMudInSameGridBlock)
    {
        return field_178_sub_state2;
    }

    return AI_ListeningToAbe::eState4_RageTurn_18;
}

__int16 Mudokon::AI_ListeningToAbe_State_13()
{
    StandingKnockBack_473190();
    
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);
    field_108_next_motion = -1;

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        return AI_ListeningToAbe::eState4_CrazyLaugh_21;
    }

    if (field_188_pTblEntry->field_2_next_motion == Mud_Motion::M_Punch_38_474AA0)
    {
        return AI_ListeningToAbe::eState4_Knockback_14;
    }
    else
    {
        LOG_WARNING("State 12 might be wrong ??");
        return AI_ListeningToAbe::eState4_GetsCommand_12;
    }
}

__int16 Mudokon::AI_ListeningToAbe_State_14()
{
    if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eMudAbuse_9)
    {
        __int16 v18 = GetAISubStateResponse_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
        {
            field_194_timer = sGnFrame_5C1B84 + 20;
            field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
            field_17E_delayed_speak = MudAction::eNone_17;
            if (!v18)
            {
                return field_190_sub_state;
            }
            return v18;
        }
        field_194_timer = (unsigned __int16)(StableDelay_477570() + sGnFrame_5C1B84 + 20);
        field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return field_190_sub_state;
        }
        return v18;
    }
    if (LastGameSpeak_476FF0() == GameSpeakEvents::eStopIt_22)
    {
        if (field_106_current_motion == Mud_Motion::M_Punch_38_474AA0 || field_108_next_motion == Mud_Motion::M_Punch_38_474AA0)
        {
            ToStand_4724A0();
            field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
            field_108_next_motion = -1;
        }

        __int16 result = GetAISubStateResponse_477AF0(MudAction::eStopIt_4);
        field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        field_194_timer = sGnFrame_5C1B84 + 20;
        if (result)
        {
            return result;
        }
    }

    if (static_cast<int>(sGnFrame_5C1B84) < field_194_timer)
    {
        return field_190_sub_state;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (!FindObjectOfType_425180(Types::eAbe_69, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
        {
            if (FindObjectOfType_425180(Types::eAbe_69, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5))
                || (!FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5))
                && FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5))))
            {
                field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
                field_194_timer = sGnFrame_5C1B84 + 15;
                return AI_ListeningToAbe::eState4_GetsCommand_12;
            }
        }
    }
    else
    {
        if (!FindObjectOfType_425180(Types::eAbe_69, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
        {
            if (FindObjectOfType_425180(Types::eAbe_69, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
            {
                field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
                field_194_timer = sGnFrame_5C1B84 + 15;
                return AI_ListeningToAbe::eState4_GetsCommand_12;
            }

            if (!FindObjectOfType_425180(Types::eMudokon_110, ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos, field_BC_ypos - FP_FromInteger(5)))
            {
                if (FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
                {
                    field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
                    field_194_timer = sGnFrame_5C1B84 + 15;
                    return AI_ListeningToAbe::eState4_GetsCommand_12;
                }
            }
        }
    }
    return AI_ListeningToAbe::eState4_GetsCommand_12;
}

__int16 Mudokon::AI_ListeningToAbe_State_15()
{
    if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
    {
        return field_190_sub_state;
    }

    if (field_188_pTblEntry->field_0_sound != MudSounds::eNone)
    {
        Sound_475EC0(field_188_pTblEntry->field_0_sound);
    }

    field_108_next_motion = field_188_pTblEntry->field_2_next_motion;
    if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags::eBit11_get_depressed))
    {
        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
    }
    if (field_182 != GameSpeakEvents::eFollowMe_10)
    {
        return field_178_sub_state2;
    }

    field_16A_flags.Set(Flags::eBit5_following);
    field_182 = GameSpeakEvents::eNone_m1;
    return AI_ListeningToAbe::eState4_FollowingIdle_4;
}

__int16 Mudokon::AI_ListeningToAbe_State_16()
{
    if (field_106_current_motion != Mud_Motion::M_SlapOwnHead_56_4727D0)
    {
        field_108_next_motion = Mud_Motion::M_SlapOwnHead_56_4727D0;
    }

    if (LastGameSpeak_476FF0() != GameSpeakEvents::eStopIt_22 && field_17E_delayed_speak != MudAction::eSorry_8)
    {
        return field_190_sub_state;
    }

    __int16 result = GetAISubStateResponse_477AF0(MudAction::eStopIt_4);
    field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
    field_17E_delayed_speak = MudAction::eNone_17;
    field_194_timer = sGnFrame_5C1B84 + 20;
    if (!result)
    {
        return field_190_sub_state;
    }
    return result;
}

__int16 Mudokon::AI_ListeningToAbe_State_17()
{
    if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eMudAbuse_9)
    {
        __int16 v18 = GetAISubStateResponse_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
        {
            field_194_timer = sGnFrame_5C1B84 + 20;
            field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
            field_17E_delayed_speak = MudAction::eNone_17;
            if (!v18)
            {
                return field_190_sub_state;
            }
            return v18;
        }
        field_194_timer = (unsigned __int16)(StableDelay_477570() + sGnFrame_5C1B84 + 20);
        field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return field_190_sub_state;
        }
        return v18;
    }

    if (LastGameSpeak_476FF0() != GameSpeakEvents::eStopIt_22)
    {
        if (field_106_current_motion)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::M_Punch_38_474AA0;
        return AI_ListeningToAbe::eState4_Idle_7;
    }

    if (field_106_current_motion == Mud_Motion::M_Punch_38_474AA0 || field_108_next_motion == Mud_Motion::M_Punch_38_474AA0)
    {
        ToStand_4724A0();
        field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
        field_108_next_motion = -1;
    }

    __int16 result = GetAISubStateResponse_477AF0(MudAction::eStopIt_4);
    field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
    field_17E_delayed_speak = MudAction::eNone_17;
    field_194_timer = sGnFrame_5C1B84 + 20;
    if (result)
    {
        return result;
    }

    if (field_106_current_motion)
    {
        return field_190_sub_state;
    }
    field_108_next_motion = Mud_Motion::M_Punch_38_474AA0;
    return AI_ListeningToAbe::eState4_Idle_7;
}

__int16 Mudokon::AI_ListeningToAbe_State_18()
{
    if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eMudAbuse_9)
    {
        __int16 v18 = GetAISubStateResponse_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
        {
            field_194_timer = (short)(sGnFrame_5C1B84 + 20);
        }
        else
        {
            field_194_timer = (unsigned __int16)Mudokon::StableDelay_477570() + sGnFrame_5C1B84 + 20;
        }
        field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        if (!v18)
        {
            return field_190_sub_state;
        }
        return v18;
    }
    else
    {
        if (LastGameSpeak_476FF0() != GameSpeakEvents::eStopIt_22)
        {
            if (field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
            {
                return field_190_sub_state;
            }
            field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
            return  AI_ListeningToAbe::eState4_RageSlap_17;
        }

        if (field_106_current_motion == Mud_Motion::M_Punch_38_474AA0 || field_108_next_motion == Mud_Motion::M_Punch_38_474AA0)
        {
            ToStand_4724A0();
            field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
            field_108_next_motion = -1;
        }

        __int16  result = GetAISubStateResponse_477AF0(MudAction::eStopIt_4);
        field_178_sub_state2 = AI_ListeningToAbe::eState4_Idle_7;
        field_17E_delayed_speak = MudAction::eNone_17;
        field_194_timer = sGnFrame_5C1B84 + 20;

        if (!result)
        {
            if (field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
            {
                return field_190_sub_state;
            }
            field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
            result = AI_ListeningToAbe::eState4_RageSlap_17;
        }
        return result;
    }
}

__int16 Mudokon::AI_ListeningToAbe_State_19()
{
    if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
    {
        return field_190_sub_state;
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

    field_108_next_motion = Mud_Motion::M_Speak_6_472FA0;
    return AI_ListeningToAbe::eState4_FollowingIdle_4;
}

__int16 Mudokon::AI_ListeningToAbe_State_20()
{
    if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
    {
        return field_190_sub_state;
    }
    Sound_475EC0(MudSounds::eOkay_12);
    field_108_next_motion = Mud_Motion::M_Speak_3_472FA0;
    return field_178_sub_state2;
}

__int16 Mudokon::AI_ListeningToAbe_State_21()
{
    if (field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
    {
        return AI_ListeningToAbe::eState4_CrazyLaugh_21;
    }

    if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
    {
        return field_190_sub_state;
    }

    field_16A_flags.Clear(Flags::eBit12_alert_enemies);

    if (Math_NextRandom() >= 90u)
    {
        Sound_475EC0(MudSounds::eLaugh_10);
    }
    else
    {
        Sound_475EC0(MudSounds::eGiggle_8);
    }

    field_108_next_motion = Mud_Motion::M_Speak_6_472FA0;
    return field_178_sub_state2;
}

__int16 Mudokon::AI_ListeningToAbe_State_22()
{
    if (field_10_resources_array.ItemAt(2))
    {
        field_108_next_motion = Mud_Motion::M_Chisel_11_4732D0;
    }
    else
    {
        field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
    }

    if (field_106_current_motion != Mud_Motion::M_CrouchIdle_15_474040 && field_106_current_motion != Mud_Motion::M_Chisel_11_4732D0)
    {
        return field_190_sub_state;
    }

    RemoveAlerted();

    if (field_10_resources_array.ItemAt(2))
    {
        field_18E_ai_state = Mud_AI_State::AI_Chisel_1_47C5F0;
    }
    else
    {
        field_18E_ai_state = Mud_AI_State::AI_Scrub_2_47D270;
    }

    field_182 = GameSpeakEvents::eNone_m1;
    return AI_ListeningToAbe::eState4_Inactive_0;
}

__int16 Mudokon::AI_ShrivelDeath_5_4714A0()
{
    if (field_194_timer < static_cast<int>(sGnFrame_5C1B84 + 80))
    {
        field_CC_sprite_scale -= FP_FromDouble(0.008);
        field_D0_r -= 2;
        field_D2_g -= 2;
        field_D4_b -= 2;
    }

    if (static_cast<int>(sGnFrame_5C1B84) < field_194_timer - 24)
    {
        if ((sGnFrame_5C1B84 % 4) == 0)
        {
            // Fizzel steam/smoke
            New_Smoke_Particles_426C70(
                (FP_FromInteger(Math_RandomRange_496AB0(-24, 24)) * field_CC_sprite_scale) + field_B8_xpos, 
                field_BC_ypos - FP_FromInteger(6), 
                field_CC_sprite_scale / FP_FromInteger(2),
                2, 128, 128, 128);

            // Fizzle sound
            SFX_Play_46FBA0(SoundEffect::Vaporize_79, 25, FP_GetExponent(FP_FromInteger(2200) * field_CC_sprite_scale));
        }
    }

    // Finally fizzled out
    if (field_CC_sprite_scale < FP_FromInteger(0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    return 100;

}

__int16 Mudokon::AI_Escape_6_47A560()
{
    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds_5C1B70.Find_449CF0(field_11C_bird_portal_id));
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else
    {
        bool noBirdPortalOrPortalIsDead = false;
        if (!pBirdPortal || pBirdPortal->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            noBirdPortalOrPortalIsDead = true;
        }

        bool bOver60Away = false;
        if (pBirdPortal)
        {
            bOver60Away = FP_Abs(pBirdPortal->field_30_ypos - field_BC_ypos) > (field_CC_sprite_scale * FP_FromInteger(60));
        }

        if (bOver60Away || noBirdPortalOrPortalIsDead)
        {
            --word_5C3012;
            field_16C &= ~4u;

            if (pBirdPortal)
            {
                if (word_5C3012 == 0)
                {
                    pBirdPortal->Vsub_499610();
                    pBirdPortal->VGiveShrukul_499680(TRUE);
                }
                field_11C_bird_portal_id = -1;
            }

            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            field_18E_ai_state = Mud_AI_State::AI_ListeningToAbe_4_477B40;
            return AI_ListeningToAbe::eState4_Idle_7;
        }
        else
        {
            switch (field_190_sub_state)
            {
            case AI_Escape::eState6_PortalOppened_0:
                if (!pBirdPortal->VStateIs6_499830())
                {
                    return field_190_sub_state;
                }
                field_194_timer = sGnFrame_5C1B84 + Math_NextRandom() % 8;
                return AI_Escape::eState6_StandingUp_1;

            case AI_Escape::eState6_StandingUp_1:
                if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
                {
                    return field_190_sub_state;
                }
                return FP_Abs(pBirdPortal->field_2C_xpos - field_B8_xpos) >= ScaleToGridSize_4498B0(field_CC_sprite_scale) ? 2 : 4;

            case AI_Escape::eState6_Running_2:
                if (field_106_current_motion == Mud_Motion::M_CrouchIdle_15_474040)
                {
                    field_108_next_motion = Mud_Motion::M_CrouchToStand_18_474150;
                }
                if (field_106_current_motion == Mud_Motion::M_Chisel_11_4732D0)
                {
                    field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
                }
                if (field_106_current_motion == Mud_Motion::M_Duck_53_474A40)
                {
                    field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
                }

                if (field_106_current_motion == Mud_Motion::M_Idle_0_4724E0 || field_106_current_motion == Mud_Motion::M_WalkLoop_1_4728B0)
                {
                    if (FacingTarget_473140(pBirdPortal))
                    {
                        field_108_next_motion = Mud_Motion::M_RunLoop_21_473720;
                    }
                    else
                    {
                        field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
                    }
                }

                if (field_106_current_motion != Mud_Motion::M_RunLoop_21_473720)
                {
                    return field_190_sub_state;
                }

                if (!FacingTarget_473140(pBirdPortal))
                {
                    field_108_next_motion = Mud_Motion::M_RunTurn_25_473AA0;
                    return field_190_sub_state;
                }

                if (!VIntoBirdPortal_408FD0(3)) // Finds portal to jump into
                {
                    return field_190_sub_state;
                }

                field_190_sub_state = AI_Escape::eState6_Jumping_3;
                field_108_next_motion = Mud_Motion::M_JumpMid_36_474570;
                return field_190_sub_state;

            case AI_Escape::eState6_Replacing_4:
                if (field_106_current_motion == Mud_Motion::M_CrouchIdle_15_474040)
                {
                    field_108_next_motion = Mud_Motion::M_CrouchToStand_18_474150;
                }
                if (field_106_current_motion == Mud_Motion::M_Chisel_11_4732D0)
                {
                    field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
                }
                if (field_106_current_motion == Mud_Motion::M_Duck_53_474A40)
                {
                    field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
                }

                if (field_106_current_motion == Mud_Motion::M_Idle_0_4724E0 || field_106_current_motion == Mud_Motion::M_WalkLoop_1_4728B0)
                {
                    if ((pBirdPortal->field_26_side == PortalSide::eRight_0 && field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) ||
                        (pBirdPortal->field_26_side == PortalSide::eLeft_1 && !field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
                    {
                        field_108_next_motion = Mud_Motion::M_StandingTurn_2_472BF0;
                    }
                    else
                    {
                        field_108_next_motion = Mud_Motion::M_RunLoop_21_473720;
                    }
                }

                if (field_106_current_motion != Mud_Motion::M_RunLoop_21_473720)
                {
                    return field_190_sub_state;
                }

                if (FP_Abs(pBirdPortal->field_2C_xpos - field_B8_xpos) <= ScaleToGridSize_4498B0(field_CC_sprite_scale))
                {
                    return field_190_sub_state;
                }
                return AI_Escape::eState6_Running_2;
            }
        }
    }
    return field_190_sub_state;
}

__int16 Mudokon::AI_FallAndSmackDeath_7_471600()
{
    if (field_190_sub_state == 0)
    {
        // Wait for falling
        Mudokon_SFX_457EC0(MudSounds::eDeathDropScream_15, 0, 0, this);
        field_194_timer = sGnFrame_5C1B84 + 60;
        return 1;
    }

    if (field_190_sub_state == 1)
    {
        // Scream
        if (static_cast<int>(sGnFrame_5C1B84) == field_194_timer - 6)
        {
            SND_SEQ_Play_4CAB10(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
        }

        // Shake from ze floor slam & die
        if (static_cast<int>(sGnFrame_5C1B84) > field_194_timer)
        {
            Environment_SFX_457A40(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 32767, this);
            auto pShake = ae_new<ScreenShake>();
            if (pShake)
            {
                pShake->ctor_4ACF70(0, 0);
            }
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
    return field_190_sub_state;
}

const MudSounds sAngryWorkerResponses_55CFCA[16] =
{
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


__int16 Mudokon::AI_AngryWorker_8_47E910()
{
    switch (field_190_sub_state)
    {
    case AI_AngryWorker::eState8_Inactive_0:
        if (field_106_current_motion)
        {
            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
        }

        if (field_16A_flags.Get(Flags::eBit15_ring_timeout))
        {
            const GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
            if (lastSpeak < GameSpeakEvents::eHello_9 || lastSpeak > GameSpeakEvents::eSorry_24)
            {
                return field_190_sub_state;
            }
        }

        field_194_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 45);
        
        AddAlerted();

        return sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            TlvTypes::WorkWheel_79) != 0 ? 4 : 1;

    case AI_AngryWorker::eState8_LeverIdel_1:
    {
        if (field_17E_delayed_speak == MudAction::eSorry_8)
        {
            field_194_timer = sGnFrame_5C1B84 + 20;
            return AI_AngryWorker::eState8_Speaking_3;
        }

        const GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
        if (lastSpeak >= GameSpeakEvents::eHello_9
            && lastSpeak <= GameSpeakEvents::eSorry_24
            && sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
        {
            field_160_delayed_speak = sAngryWorkerResponses_55CFCA[static_cast<int>(lastSpeak)-9];
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
            return AI_AngryWorker::eState8_Speaking_3;
        }

        if (field_194_timer > static_cast<int>(sGnFrame_5C1B84))
        {
            return field_190_sub_state;
        }

        const FP ypos = field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
        FP xOff = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            xOff = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            xOff = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }

        auto pLever = static_cast<Switch*>(FindObjectOfType_425180(Types::eLever_139, xOff + field_B8_xpos, ypos));
        if (pLever)
        {
            field_108_next_motion = Mud_Motion::M_LeverUse_10_473020;
            pLever->VPull_4D6050(field_B8_xpos < pLever->field_B8_xpos);
        }
    }
        return AI_AngryWorker::eState8_PullingLever_2;

    case AI_AngryWorker::eState8_PullingLever_2:
        if (field_106_current_motion)
        {
            return field_190_sub_state;
        }

        if (gMap_5C3030.field_0_current_level == LevelIds::eBrewery_Ender_10)
        {
            field_194_timer = sGnFrame_5C1B84 + 90;
        }
        else
        {
            field_194_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 45);
        }
        return AI_AngryWorker::eState8_LeverIdel_1;

    case AI_AngryWorker::eState8_Speaking_3:
        if (field_194_timer > static_cast<int>(sGnFrame_5C1B84))
        {
            return field_190_sub_state;
        }

        if (field_17E_delayed_speak == MudAction::eSorry_8)
        {
            field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eSorry_8);
            field_17E_delayed_speak = MudAction::eNone_17;
            if (field_188_pTblEntry->field_0_sound != MudSounds::eNone)
            {
                Sound_475EC0(field_188_pTblEntry->field_0_sound);
            }
            field_108_next_motion = field_188_pTblEntry->field_2_next_motion;
            field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
            field_18E_ai_state = Mud_AI_State::AI_ListeningToAbe_4_477B40;
            return AI_ListeningToAbe::eState4_Idle_7;
        }
        else
        {
            field_108_next_motion = Mud_Motion::M_Speak_6_472FA0;
            Sound_475EC0(field_160_delayed_speak);
            field_194_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 45);
            return sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                TlvTypes::WorkWheel_79) != 0 ? 4 : 1;
        }
        break;

    case AI_AngryWorker::eState8_WheelIdle_4:
        if (field_194_timer > static_cast<int>(sGnFrame_5C1B84))
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::M_TurnWheelBegin_57_474C00;
        return AI_AngryWorker::eState8_UsingWheel_5;

    case AI_AngryWorker::eState8_UsingWheel_5:
    {
        const GameSpeakEvents lastSpeak2 = LastGameSpeak_476FF0();
        if (lastSpeak2 >= GameSpeakEvents::eHello_9
            && lastSpeak2 <= GameSpeakEvents::eSorry_24
            && sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
        {
            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            field_160_delayed_speak = sAngryWorkerResponses_55CFCA[static_cast<int>(lastSpeak2)-9];
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
            return AI_AngryWorker::eState8_Speaking_3;
        }

        if (field_17E_delayed_speak != MudAction::eSorry_8)
        {
            return field_190_sub_state;
        }

        field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
        field_194_timer = sGnFrame_5C1B84 + 20;
    }
        return AI_AngryWorker::eState8_Speaking_3;

    case AI_AngryWorker::eState8_Unused_6:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }

        if (field_106_current_motion != Mud_Motion::M_Idle_0_4724E0)
        {
            if (field_106_current_motion == Mud_Motion::M_CrouchIdle_15_474040)
            {
                field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
                return field_190_sub_state;
            }
            else
            {
                if (field_106_current_motion == Mud_Motion::M_Duck_53_474A40)
                {
                    field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
                }
                return field_190_sub_state;
            }
        }
        else
        {
            return AI_AngryWorker::eState8_Inactive_0;
        }
        break;

    case AI_AngryWorker::eState8_Unused_7:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::M_DuckKnockback_55_474250;
        field_194_timer = sGnFrame_5C1B84 + 60;
        return AI_AngryWorker::eState8_Unused_6;
    }

    return field_190_sub_state;
}

__int16 Mudokon::AI_Sick_9_47A910()
{
    // TODO: Lame hack, tortured muds make the "real" mud do nothing.
    if (FindObjectOfType_425180(Types::eTorturedMud_141, field_B8_xpos, field_BC_ypos - FP_FromInteger(50)) || 
        !gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        LastGameSpeak_476FF0(); // TODO: Pointless call ??
        return field_190_sub_state;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventMudokonAbuse,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eMudAbuse_9;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventMudokonComfort,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eComfort_10;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventMudokonLaugh,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eLaugh_12;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventShooting | kEventSpeaking,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eDuck_13;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventShooting,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eWait_2;
    }

    // Have Abe make a sad noise when he first sees sick Mudokons.
    if (!field_16A_flags.Get(Flags::eBit9_seen_while_sick) &&
        gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        if (sActiveHero_5C1B68->field_128.field_18_say == MudSounds::eNone)
        {
            sActiveHero_5C1B68->field_128.field_18_say = MudSounds::eSadUgh_28;
            sActiveHero_5C1B68->field_144_auto_say_timer = sGnFrame_5C1B84 + 10;
        }
        field_16A_flags.Set(Flags::eBit9_seen_while_sick);
    }

    switch (field_190_sub_state)
    {
    case AI_Sick::eState9_Inactive_0:
        if (field_106_current_motion != Mud_Motion::M_CrouchIdle_15_474040)
        {
            if (field_106_current_motion != Mud_Motion::M_StandToCrouch_17_474120 && field_108_next_motion != Mud_Motion::M_StandToCrouch_17_474120)
            {
                field_108_next_motion = Mud_Motion::M_StandToCrouch_17_474120;
            }
            return field_190_sub_state;
        }

        AddAlerted();

        field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
        return AI_Sick::eState9_Idle_1;

    case AI_Sick::eState9_Idle_1:
        if (field_106_current_motion == Mud_Motion::M_Fall_49_472C60)
        {
            return AI_Sick::eState9_Inactive_0;
        }

        if (field_106_current_motion != Mud_Motion::M_CrouchIdle_15_474040)
        {
            if (field_106_current_motion == Mud_Motion::M_Duck_53_474A40)
            {
                field_108_next_motion = Mud_Motion::M_CrouchIdle_15_474040;
            }
            return field_190_sub_state;
        }

        if (field_180_emo_tbl != Mud_Emotion::eSick_7)
        {
            field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
            field_108_next_motion = Mud_Motion::M_Idle_0_4724E0;
            auto pFlicker = ae_new<PossessionFlicker>();
            if (pFlicker)
            {
                pFlicker->ctor_4319E0(this, 8, 155, 255, 32);
            }
            return AI_Sick::eState9_StandingUp_3;
        }

        if (field_17E_delayed_speak == MudAction::eNone_17)
        {
            const GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
            if (lastSpeak == GameSpeakEvents::eNone_m1)
            {
                return field_190_sub_state;
            }

            field_178_sub_state2 = field_190_sub_state;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;

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
                return field_190_sub_state;
            }
        }
        else
        {
            field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eSick_7, field_17E_delayed_speak);
            if (field_188_pTblEntry->field_0_sound == MudSounds::eOops_14)
            {
                field_194_timer = sGnFrame_5C1B84 + 20;
            }
            else
            {
                field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
            }
            field_178_sub_state2 = field_190_sub_state;
            field_17E_delayed_speak = MudAction::eNone_17;
        }

        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
        if (field_188_pTblEntry->field_6_sub_state)
        {
            return AI_Sick::eState9_Duck_2;
        }
        break;

    case AI_Sick::eState9_Duck_2:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }

        if (field_188_pTblEntry->field_0_sound != MudSounds::eNone)
        {
            Sound_475EC0(field_188_pTblEntry->field_0_sound);
        }

        field_108_next_motion = Mud_Motion::M_Duck_53_474A40;
        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
        return field_178_sub_state2;

    case AI_Sick::eState9_StandingUp_3:
        if (field_106_current_motion)
        {
            return field_190_sub_state;
        }
        
        Sound_475EC0(MudSounds::eFart_7);
        field_108_next_motion = Mud_Motion::M_Speak_6_472FA0;
        
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            New_Smoke_Particles_426C70(
                field_B8_xpos + (FP_FromInteger(12) * field_CC_sprite_scale),
                field_BC_ypos - (FP_FromInteger(24) * field_CC_sprite_scale),
                (FP_FromDouble(0.5) * field_CC_sprite_scale),
                3, 32u, 128u, 32u);
        }
        else
        {
            New_Smoke_Particles_426C70(
                field_B8_xpos - (FP_FromInteger(12) * field_CC_sprite_scale),
                field_BC_ypos - (FP_FromInteger(24) * field_CC_sprite_scale),
                (FP_FromDouble(0.5) * field_CC_sprite_scale),
                3, 32u, 128u, 32u);
        }
        return AI_Sick::eState9_Farting_4;

    case AI_Sick::eState9_Farting_4:
        if (field_106_current_motion)
        {
            return field_190_sub_state;
        }
        field_18E_ai_state = Mud_AI_State::AI_ListeningToAbe_4_477B40;
        return AI_ListeningToAbe::eState4_Inactive_0;

    default:
        return field_190_sub_state;
    }
    return field_190_sub_state;
}

void Mudokon::M_Idle_0_4724E0()
{
    ToFalling_472320();

    field_F8_LastLineYPos = field_BC_ypos;

    if (field_100_pCollisionLine)
    {
        if ((field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36) && field_110_id == -1)
        {
            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            vOnCollisionWith_424EE0(
                { bRect.x, static_cast<short>(bRect.y + 5) },
                { bRect.w, static_cast<short>(bRect.h + 5) },
                ObjList_5C1B78, 1, (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
        }
    }

    switch (field_108_next_motion)
    {
    case Mud_Motion::M_WalkLoop_1_4728B0:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9);
        }
        else
        {
            field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9);
        }
        field_106_current_motion = Mud_Motion::M_WalkBegin_7_472AB0;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::M_SneakLoop_27_473C40:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(10);
        }
        else
        {
            field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(10);
        }
        field_106_current_motion = Mud_Motion::M_SneakBegin_32_473F80;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::M_RunLoop_21_473720:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
        }
        else
        {
            field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
        }

        field_106_current_motion = Mud_Motion::M_WalkToRun_37_4749A0;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::M_Chant_50_473040:
        SND_SEQ_PlaySeq_4CA960(SeqId::MudokonChant_11, 0, 1);
        field_106_current_motion = Mud_Motion::M_Chant_50_473040;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::M_CrouchIdle_15_474040:
        field_106_current_motion = Mud_Motion::M_StandToCrouch_17_474120;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::M_Chisel_11_4732D0:
    case Mud_Motion::M_CrouchScrub_14_473560:
        field_106_current_motion = Mud_Motion::M_StandToCrouch_17_474120;
        break;

    default:
        if (field_108_next_motion != -1)
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
        break;
    }
}

void Mudokon::M_WalkLoop_1_4728B0()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        StandingKnockBack_473190();
    }
    else if (sObjectIds_5C1B70.Find_449CF0(field_110_id) && field_16A_flags.Get(Flags::eBit4_blind) && (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(1), field_C4_velx)))
    {
        StandingKnockBack_473190();
    }
    else
    {
        MoveOnLine_4720D0();
        if (field_106_current_motion == Mud_Motion::M_WalkLoop_1_4728B0)
        {
            if (field_20_animation.field_92_current_frame == 2 || field_20_animation.field_92_current_frame == 11)
            {
                if (field_108_next_motion == Mud_Motion::M_Idle_0_4724E0)
                {
                    field_108_next_motion = -1;
                    if (field_20_animation.field_92_current_frame == 2)
                    {
                        field_106_current_motion = Mud_Motion::M_MidWalkToIdle_9_472BD0;
                    }
                    else
                    {
                        field_106_current_motion = Mud_Motion::M_WalkToIdle_8_472B30;
                    }
                }
                else if (field_108_next_motion == Mud_Motion::M_StandingTurn_2_472BF0)
                {
                    if (field_20_animation.field_92_current_frame != 2)
                    {
                        field_106_current_motion = Mud_Motion::M_MidWalkToIdle_9_472BD0;
                    }
                    else
                    {
                        field_106_current_motion = Mud_Motion::M_WalkToIdle_8_472B30;
                    }
                }
            }
            else if (field_20_animation.field_92_current_frame == 5 || field_20_animation.field_92_current_frame == 14)
            {
                Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);
                MapFollowMe_408D10(TRUE);
                if (field_108_next_motion == Mud_Motion::M_RunLoop_21_473720)
                {
                    field_108_next_motion = -1;
                    if (field_20_animation.field_92_current_frame == 5)
                    {
                        field_106_current_motion = Mud_Motion::M_MidWalkToRun_20_4736D0;
                    }
                    else
                    {
                        field_106_current_motion = Mud_Motion::M_WalkToRun_19_473600;
                    }
                }
                else if (field_108_next_motion == Mud_Motion::M_SneakLoop_27_473C40)
                {
                    auto curFrame2 = field_20_animation.field_92_current_frame;
                    field_108_next_motion = -1;
                    if (curFrame2 == 5)
                    {
                        field_106_current_motion = Mud_Motion::M_WalkToSneak_28_473D60;
                    }
                    else
                    {
                        field_106_current_motion = Mud_Motion::M_MidWalkToSneak_30_473EE0;
                    }
                }
            }
        }
    }
}

void Mudokon::M_StandingTurn_2_472BF0()
{
    ToFalling_472320();

    if (field_20_animation.field_92_current_frame == 0)
    {
        Environment_SFX_457A40(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        ToStand_4724A0();
    }
}

void Mudokon::M_Speak_472FA0()
{
    ToFalling_472320();
    SetPal_4772D0(field_180_emo_tbl);

    if (field_16A_flags.Get(Flags::eBit12_alert_enemies))
    {
        Event_Broadcast_422BC0(kEventNoise, this);
        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_16A_flags.Set(Flags::eBit12_alert_enemies);
        SetPal_4772D0(Mud_Emotion::eNormal_0);
        ToStand_4724A0();
    }
}

void Mudokon::M_WalkBegin_7_472AB0()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = Mud_Motion::M_WalkLoop_1_4728B0;
        }
        MoveOnLine_4720D0();
    }
}

void Mudokon::M_WalkToIdle_8_472B30()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        MoveOnLine_4720D0();

        if (!field_20_animation.field_92_current_frame)
        {
            Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);
            return;
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe_408D10(TRUE);
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
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::M_Chisel_11_4732D0()
{
    ToFalling_472320();

    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (sGnFrame_5C1B84 & 1)
            {
                SFX_Play_46FA90(SoundEffect::Chisel_91, 0, field_CC_sprite_scale);

                auto pSpark = ae_new<Spark>();
                if (pSpark)
                {
                    FP sparkY = {};
                    FP sparkX = {};
                    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        sparkY = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(3));
                        sparkX = field_B8_xpos - (field_CC_sprite_scale * FP_FromInteger(18));
                    }
                    else
                    {
                        sparkY = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(3));
                        sparkX = (field_CC_sprite_scale * FP_FromInteger(18)) + field_B8_xpos;
                    }

                    pSpark->ctor_4CBBB0(
                        sparkX + FP_FromInteger(field_DA_xOffset),
                        sparkY,
                        field_CC_sprite_scale,
                        9,
                        0,
                        255,
                        0);
                }
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        switch (field_108_next_motion)
        {
        case Mud_Motion::M_Idle_0_4724E0:
            field_106_current_motion = Mud_Motion::M_StopChisel_13_473530;
            return;

        case Mud_Motion::M_CrouchIdle_15_474040:
            field_106_current_motion = Mud_Motion::M_StopChisel_13_473530;
            break;

        case -1:
            return;

        default:
            field_106_current_motion = field_108_next_motion;
            break;
        }

        field_108_next_motion = -1;
    }
}

void Mudokon::M_StartChisel_12_473500()
{
    ToFalling_472320();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_Chisel_11_4732D0;
    }
}

void Mudokon::M_StopChisel_13_473530()
{
    ToFalling_472320();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_CrouchIdle_15_474040;
    }
}

void Mudokon::M_CrouchScrub_14_473560()
{
    ToFalling_472320();

    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        if (field_20_animation.field_92_current_frame == 2)
        {
            SFX_Play_46FA90(SoundEffect::Clean1_71, 0);
        }
        else if (field_20_animation.field_92_current_frame == 6)
        {
            SFX_Play_46FA90(SoundEffect::Clean2_72, 0);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_CrouchIdle_15_474040;
    }
}

void Mudokon::M_CrouchIdle_15_474040()
{
    ToFalling_472320();

    if (field_108_next_motion == Mud_Motion::M_Duck_53_474A40)
    {
        field_106_current_motion = Mud_Motion::M_ToDuck_52_474A20;
        field_108_next_motion = -1;
    }
    else if (field_108_next_motion == Mud_Motion::M_Idle_0_4724E0)
    {
        field_106_current_motion = Mud_Motion::M_CrouchToStand_18_474150;
        field_108_next_motion = -1;
    }
    else if (field_108_next_motion == Mud_Motion::M_Chisel_11_4732D0)
    {
        field_106_current_motion = Mud_Motion::M_StartChisel_12_473500;
        field_108_next_motion = -1;
    }
    else if (field_108_next_motion != -1)
    {
        field_106_current_motion = field_108_next_motion;
        field_108_next_motion = -1;
    }
}

void Mudokon::M_CrouchTurn_16_4740E0()
{
    ToFalling_472320();

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_CrouchIdle_15_474040;
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }
}

void Mudokon::M_StandToCrouch_17_474120()
{
    ToFalling_472320();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_CrouchIdle_15_474040;
    }
}

void Mudokon::M_CrouchToStand_18_474150()
{
    ToFalling_472320();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::M_WalkToRun_19_473600()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_RunLoop_21_473720;
    }

    if (WallHit_408750((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
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

    if (field_106_current_motion == Mud_Motion::M_RunLoop_21_473720)
    {
        field_106_current_motion = Mud_Motion::M_MidWalkToRun_20_4736D0;
        field_F4_previous_motion = Mud_Motion::M_RunLoop_21_473720;
        field_F6_anim_frame = 8;
        field_192_reset_to_previous_motion = 1;
    }
}

void Mudokon::M_RunLoop_21_473720()
{
    Event_Broadcast_422BC0(kEventNoise, this);

    if (Is_In_Current_Camera_424A70() == CameraPos::eCamCurrent_0)
    {
        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
    }

    if (WallHit_408750((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        StandingKnockBack_473190();
        return;
    }
    
    MoveOnLine_4720D0();

    if (field_106_current_motion == Mud_Motion::M_RunLoop_21_473720)
    {
        if (field_20_animation.field_92_current_frame == 0 || field_20_animation.field_92_current_frame == 8)
        {
            MapFollowMe_408D10(TRUE);

            if (field_108_next_motion == Mud_Motion::M_JumpMid_36_474570)
            {
                field_106_current_motion = Mud_Motion::M_JumpBegin_35_474460;
                field_108_next_motion = -1;
            }
        }
        else if (field_20_animation.field_92_current_frame == 4 || field_20_animation.field_92_current_frame == 12)
        {
            Environment_SFX_457A40(EnvironmentSfx::eRunningFootstep_2, 0, 32767, this);
            MapFollowMe_408D10(TRUE);

            switch (field_108_next_motion)
            {
            case Mud_Motion::M_WalkLoop_1_4728B0:
                field_108_next_motion = -1;
                field_106_current_motion = (field_20_animation.field_92_current_frame == 4) ? Mud_Motion::M_MidRunToWalk_23_4739B0 : Mud_Motion::M_RunToWalk_22_4738E0;
                return;

            case Mud_Motion::M_Idle_0_4724E0:
                field_106_current_motion = Mud_Motion::M_RunSlideStop_24_473A00;
                field_108_next_motion = -1;
                Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
                return;

            case Mud_Motion::M_RunTurn_25_473AA0:
                field_106_current_motion = Mud_Motion::M_RunTurn_25_473AA0;
                field_108_next_motion = -1;
                Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
                return;

            case Mud_Motion::M_JumpMid_36_474570:
                field_106_current_motion = Mud_Motion::M_JumpBegin_35_474460;
                field_108_next_motion = -1;
                break;
            }
        }
    }
}

void Mudokon::M_RunToWalk_22_4738E0()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_WalkLoop_1_4728B0;
    }

    if (WallHit_408750((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
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

    if (field_106_current_motion == Mud_Motion::M_WalkLoop_1_4728B0)
    {
        field_106_current_motion = Mud_Motion::M_MidRunToWalk_23_4739B0;
        field_F4_previous_motion = Mud_Motion::M_WalkLoop_1_4728B0;
        field_F6_anim_frame = 9;
        field_192_reset_to_previous_motion = 1;
    }
}

void Mudokon::M_RunSlideStop_24_473A00()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    if (WallHit_408750((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        StandingKnockBack_473190();
    }
    else
    {
        ReduceXVelocityBy_472260(FP_FromDouble(0.375));

        if (field_106_current_motion == Mud_Motion::M_RunSlideStop_24_473A00)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);
                MapFollowMe_408D10(TRUE);
                Mudokon::ToStand_4724A0();
            }
        }
    }
}

void Mudokon::M_RunTurn_25_473AA0()
{
    Event_Broadcast_422BC0(kEventNoise, this);

    if (WallHit_408750((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        StandingKnockBack_473190();
    }
    else
    {
        ReduceXVelocityBy_472260(FP_FromDouble(0.375));

        if (field_106_current_motion == Mud_Motion::M_RunTurn_25_473AA0)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_408D10(TRUE);
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                    field_106_current_motion = Mud_Motion::M_RunTurnToRun_26_473BB0;
                }
                else
                {
                    field_106_current_motion = Mud_Motion::M_RunTurnToRun_26_473BB0;
                    field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                }
            }
        }
    }
}

void Mudokon::M_RunTurnToRun_26_473BB0()
{
    Event_Broadcast_422BC0(kEventNoise, this);

    if (WallHit_408750((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        MoveOnLine_4720D0();

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = Mud_Motion::M_RunLoop_21_473720;
            field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        }
    }
}

void Mudokon::M_SneakLoop_27_473C40()
{
    if (WallHit_408750((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        StandingKnockBack_473190();
    }
    else
    {
        MoveOnLine_4720D0();
        if (field_106_current_motion == Mud_Motion::M_SneakLoop_27_473C40)
        {
            if (field_20_animation.field_92_current_frame == 3 || field_20_animation.field_92_current_frame == 13)
            {
                if (field_108_next_motion == Mud_Motion::M_Idle_0_4724E0)
                {
                    field_108_next_motion = -1;
                    field_106_current_motion = (field_20_animation.field_92_current_frame != 3) ? Mud_Motion::M_SneakToIdle_33_473FF0 : Mud_Motion::M_MidSneakToIdle_34_474020;
                }
            }
            else if (field_20_animation.field_92_current_frame == 6 || field_20_animation.field_92_current_frame == 16)
            {
                Environment_SFX_457A40(EnvironmentSfx::eSneakFootstep_3, 0, 32767, this);
                MapFollowMe_408D10(TRUE);

                if (field_108_next_motion == Mud_Motion::M_WalkLoop_1_4728B0)
                {
                    field_108_next_motion = -1;
                    field_106_current_motion = field_20_animation.field_92_current_frame != 6 ? Mud_Motion::M_MidSneakToWalk_31_473F30 : Mud_Motion::M_SneakToWalk_29_473E20;
                }
            }
        }
    }
}

void Mudokon::M_WalkToSneak_28_473D60()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(10));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(10));
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_SneakLoop_27_473C40;
    }

    if (WallHit_408750((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
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
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_WalkLoop_1_4728B0;
    }

    if (WallHit_408750((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        MoveOnLine_4720D0();
    }
}

void Mudokon::M_MidWalkToSneak_30_473EE0()
{
    M_WalkToSneak_28_473D60();

    if (field_106_current_motion == Mud_Motion::M_SneakLoop_27_473C40)
    {
        field_106_current_motion = Mud_Motion::M_MidWalkToSneak_30_473EE0;
        field_F4_previous_motion = Mud_Motion::M_SneakLoop_27_473C40;
        field_F6_anim_frame = 10;
        field_192_reset_to_previous_motion = 1;
    }
}

void Mudokon::M_MidSneakToWalk_31_473F30()
{
    M_SneakToWalk_29_473E20();

    if (field_106_current_motion == Mud_Motion::M_WalkLoop_1_4728B0)
    {
        field_106_current_motion = Mud_Motion::M_MidSneakToWalk_31_473F30;
        field_F4_previous_motion = Mud_Motion::M_WalkLoop_1_4728B0;
        field_F6_anim_frame = 9;
        field_192_reset_to_previous_motion = 1;
    }
}

void Mudokon::M_SneakBegin_32_473F80()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_SneakLoop_27_473C40;
    }

    if (WallHit_408750((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
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

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_408D10(TRUE);
        ToStand_4724A0();
    }
}

void Mudokon::M_MidSneakToIdle_34_474020()
{
    M_SneakToIdle_33_473FF0();
}

void Mudokon::M_JumpBegin_35_474460()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    
    field_B8_xpos += field_C4_velx;

    if (!field_20_animation.field_92_current_frame)
    {
        Environment_SFX_457A40(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 32767, this);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_F8_LastLineYPos = field_BC_ypos;
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = (field_CC_sprite_scale * -FP_FromDouble(7.6)); // TODO: Correct FP value?
        }
        else
        {
            field_C4_velx = (field_CC_sprite_scale * FP_FromDouble(7.6)); // TODO: Correct FP value?
        }
        field_C8_vely = (field_CC_sprite_scale * -FP_FromDouble(9.6));
        field_BC_ypos += field_C8_vely;
        VOnTrapDoorOpen();
        field_106_current_motion = Mud_Motion::M_JumpMid_36_474570;
        field_100_pCollisionLine = nullptr;
    }
}

void Mudokon::M_JumpMid_36_474570()
{
    Event_Broadcast_422BC0(kEventNoise, this);

    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds_5C1B70.Find_449CF0(field_11C_bird_portal_id));
    if (field_20_animation.field_92_current_frame == 5)
    {
        SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);
    }

    PSX_RECT rect = {};
    vGetBoundingRect_424FD0(&rect, 1);

    if ((field_C4_velx > FP_FromInteger(0) && FP_FromInteger(rect.x) > pBirdPortal->field_2C_xpos) ||
        (field_C4_velx < FP_FromInteger(0) && FP_FromInteger(rect.w) < pBirdPortal->field_2C_xpos))
    {
        field_16A_flags.Clear(Flags::eBit1_not_rescued);
        field_16A_flags.Clear(Flags::eBit2_save_state);

        field_6_flags.Set(BaseGameObject::eDead_Bit3);

        field_C8_vely = FP_FromInteger(0);
        field_C4_velx = FP_FromInteger(0);

        SND_SEQ_Play_4CAB10(SeqId::SaveTriggerMusic_31, 1, 127, 127);

        auto pMusicTrigger = ae_new<MusicTrigger>();
        if (pMusicTrigger)
        {
            pMusicTrigger->ctor_47FF10(5, 0, 0, 0);
        }

        sRescuedMudokons_5C1BC2++;

        if (sStatsSignCurrentArea_5C1A20 < ALIVE_COUNTOF(sSavedKilledMudsPerPath_5C1B50.mData))
        {
            sSavedKilledMudsPerPath_5C1B50.mData[sStatsSignCurrentArea_5C1A20]++;
        }

        if (pBirdPortal)
        {
            pBirdPortal->VMudSaved_499A50();
        }

        if (field_17A_rescue_id)
        {
            SwitchStates_Set_465FF0(field_17A_rescue_id, 1);
        }
    }

    field_C8_vely += (field_CC_sprite_scale * FP_FromDouble(1.8));
    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;
}

void Mudokon::M_WalkToRun_37_4749A0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_RunLoop_21_473720;
    }

    Event_Broadcast_422BC0(kEventNoise, this);

    if (WallHit_408750((field_CC_sprite_scale * FP_FromInteger(50)), field_C4_velx))
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
    SetPal_4772D0(field_180_emo_tbl);

    if (field_20_animation.field_92_current_frame == 5)
    {
        if (field_180_emo_tbl == Mud_Emotion::eAngry_1 || field_180_emo_tbl == Mud_Emotion::eAggressive_2)
        {
            BaseGameObject* pSlapTarget = nullptr;
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                pSlapTarget = FindObjectOfType_425180(Types::eAbe_69, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5));
            }
            else
            {
                pSlapTarget = FindObjectOfType_425180(Types::eAbe_69, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5));
            }

            if (!pSlapTarget)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    pSlapTarget = FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5));
                }
                else
                {
                    pSlapTarget = FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5));
                }
            }

            if (pSlapTarget)
            {
                static_cast<BaseAliveGameObject*>(pSlapTarget)->VTakeDamage_408730(this);
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetPal_4772D0(Mud_Emotion::eNormal_0);
        ToStand_4724A0();
    }
}

void Mudokon::M_HoistBegin_39_4748E0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_F8_LastLineYPos = field_BC_ypos;
        field_C8_vely = (field_CC_sprite_scale * -FP_FromInteger(8));
        field_BC_ypos += field_C8_vely;
        VOnTrapDoorOpen();
        field_106_current_motion = Mud_Motion::M_HoistLand_40_474780;
        field_100_pCollisionLine = nullptr;
    }
}

void Mudokon::M_HoistLand_40_474780()
{
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        PSX_RECT bRect = {};
        switch (pLine->field_8_type) // TODO: Strongly type
        {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            field_B8_xpos = hitX;
            field_BC_ypos = FP_NoFractional(hitY + FP_FromDouble(0.5));
            MapFollowMe_408D10(TRUE);
            field_100_pCollisionLine = pLine;
            field_106_current_motion = Mud_Motion::M_LandSoft1_41_474960;
            vGetBoundingRect_424FD0(&bRect, 1);
            vOnCollisionWith_424EE0(
                { bRect.x, static_cast<short>(bRect.y + 5) },
                { bRect.w, static_cast<short>(bRect.h + 5) },
                ObjList_5C1B78,
                1,
                (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
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

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::M_LandSoft2_42_4743F0()
{
    ToFalling_472320();

    if (field_20_animation.field_92_current_frame == 2)
    {
        if (field_F4_previous_motion == Mud_Motion::M_Fall_49_472C60)
        {
            Environment_SFX_457A40(EnvironmentSfx::eLandingSoft_5, 0, 32767, this);
        }
        else
        {
            Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }

}

void Mudokon::M_DunnoBegin_43_472790()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_DunnoEnd_44_4727B0;
    }
}

void Mudokon::M_DunnoEnd_44_4727B0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
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

    if ((gMap_5C3030.field_0_current_level == LevelIds::eMines_1 ||
        gMap_5C3030.field_0_current_level == LevelIds::eBonewerkz_8 ||
        gMap_5C3030.field_0_current_level == LevelIds::eFeeCoDepot_5 ||
        gMap_5C3030.field_0_current_level == LevelIds::eBarracks_6 ||
        gMap_5C3030.field_0_current_level == LevelIds::eBrewery_9)
        && field_20_animation.field_92_current_frame == 7)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (field_108_next_motion != -1 && field_10C_health > FP_FromInteger(0))
    {
        field_106_current_motion = field_108_next_motion;
        field_108_next_motion = -1;
    }
}

void Mudokon::M_Knockback_46_4742A0()
{
    if (field_20_animation.field_92_current_frame == 12)
    {
        CheckKnockedOntoABomb_4723C0();
    }

    Event_Broadcast_422BC0(kEventNoise, this);

    if ((gMap_5C3030.field_0_current_level == LevelIds::eMines_1||
        gMap_5C3030.field_0_current_level == LevelIds::eBonewerkz_8 ||
        gMap_5C3030.field_0_current_level == LevelIds::eFeeCoDepot_5 ||
        gMap_5C3030.field_0_current_level == LevelIds::eBarracks_6 ||
        gMap_5C3030.field_0_current_level == LevelIds::eBrewery_9) && 
        field_20_animation.field_92_current_frame == 7)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (!field_100_pCollisionLine)
    {
        M_Fall_49_472C60();

        if (field_106_current_motion == Mud_Motion::M_KnockForward_45_474180 || field_106_current_motion == Mud_Motion::M_LandSoft2_42_4743F0)
        {
            field_106_current_motion = Mud_Motion::M_Knockback_46_4742A0;
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_100_pCollisionLine)
        {
            field_106_current_motion = Mud_Motion::M_KnockbackGetUp_47_474380;
        }
    }
}

void Mudokon::M_KnockbackGetUp_47_474380()
{
    ToFalling_472320();
    
    Event_Broadcast_422BC0(kEventNoise, this);
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::M_FallOffEdge_48_4743C0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_Fall_49_472C60;
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
    if (InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        switch (pLine->field_8_type) // TODO: Strongly type line types
        {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            field_100_pCollisionLine = pLine;
            
            MapFollowMe_408D10(TRUE);

            if ((sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                TlvTypes::SoftLanding_75)  && field_10C_health > FP_FromInteger(0)) ||
                (field_BC_ypos - field_F8_LastLineYPos < (field_CC_sprite_scale * FP_FromInteger(180)) &&
                (field_10C_health > FP_FromInteger(0) || gAbeBulletProof_5C1BDA)))
            {
                field_106_current_motion = Mud_Motion::M_LandSoft2_42_4743F0;
            }
            else
            {
                field_10C_health = FP_FromInteger(0);
                StandingKnockBack_473190();
                field_16A_flags.Clear(Flags::eBit2_save_state);
                field_18E_ai_state = Mud_AI_State::AI_ShrivelDeath_5_4714A0;
                field_106_current_motion = Mud_Motion::M_KnockForward_45_474180;
                field_194_timer = sGnFrame_5C1B84 + 90;
            }

            field_F4_previous_motion = Mud_Motion::M_Fall_49_472C60;

            vOnCollisionWith_424EE0(
                { FP_GetExponent(field_B8_xpos - FP_FromInteger(10)), FP_GetExponent(field_BC_ypos - FP_FromInteger(10)) },
                { FP_GetExponent(field_B8_xpos + FP_FromInteger(10)), FP_GetExponent(field_BC_ypos + FP_FromInteger(10)) },
                ObjList_5C1B78,
                1,
                (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
            break;

        case 1u:
        case 2u:
        case 5u:
        case 6u:
            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            StandingKnockBack_473190();
            break;
        }
    }
}

void Mudokon::M_Chant_50_473040()
{
    if (!(static_cast<int>(sGnFrame_5C1B84) % 4))
    {
        New_RandomizedChant_Particle_45BC70(this);
    }

    if (!SND_SsIsEos_DeInlined_4CACD0(11u))
    {
        SND_SEQ_Play_4CAB10(SeqId::MudokonChant_11, 1, 50, 50);
    }

    if (field_108_next_motion == Mud_Motion::M_Idle_0_4724E0)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SND_SEQ_Stop_4CAE60(SeqId::MudokonChant_11);
            field_106_current_motion = Mud_Motion::M_ChantEnd_51_4730D0;
            field_108_next_motion = -1;
        }
    }
}

void Mudokon::M_ChantEnd_51_4730D0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::M_ToDuck_52_474A20()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_Duck_53_474A40;
    }
}

void Mudokon::M_Duck_53_474A40()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_108_next_motion == Mud_Motion::M_CrouchIdle_15_474040)
        {
            field_108_next_motion = -1;
            field_106_current_motion = Mud_Motion::M_DuckToCrouch_54_474A80;
        }
    }
}

void Mudokon::M_DuckToCrouch_54_474A80()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_CrouchIdle_15_474040;
    }
}

void Mudokon::M_DuckKnockback_55_474250()
{
    if (field_20_animation.field_92_current_frame == 1)
    {
        Environment_SFX_457A40(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::M_Duck_53_474A40;
        field_108_next_motion = -1;
    }
}

void Mudokon::M_SlapOwnHead_56_4727D0()
{
    if (field_20_animation.field_92_current_frame == 5)
    {
        HurtSound_475DB0();
        field_10C_health -= FP_FromDouble(0.033);
    }

    if (field_10C_health > FP_FromInteger(0))
    {
        if (field_108_next_motion == Mud_Motion::M_Idle_0_4724E0 || field_108_next_motion == Mud_Motion::M_Speak_6_472FA0)
        {
            ToStand_4724A0();
        }
    }
    else
    {
        Mudokon_SFX_457EC0(MudSounds::eHurt2_9, 0, 1000, this);
        Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
        Event_Broadcast_422BC0(kEventMudokonDied, this);
        field_16A_flags.Clear(Flags::eBit2_save_state);
        field_10C_health = FP_FromInteger(0);
        field_18E_ai_state = Mud_AI_State::AI_ShrivelDeath_5_4714A0;
        field_194_timer = sGnFrame_5C1B84 + 90;
        field_106_current_motion = Mud_Motion::M_KnockForward_45_474180;
        field_108_next_motion = -1;
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

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        auto pWheel = static_cast<WorkWheel*>(FindObjectOfType_425180(Types::eWheel_148, field_B8_xpos, field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(50))));
        if (pWheel)
        {
            pWheel->VStartTurning();
            field_158_wheel_id = pWheel->field_8_object_id;
        }
        field_106_current_motion = Mud_Motion::M_TurnWheelLoop_58_474CC0;
    }
}

void Mudokon::M_TurnWheelLoop_58_474CC0()
{
    if (field_108_next_motion == Mud_Motion::M_Idle_0_4724E0)
    {
        auto pWheel = static_cast<WorkWheel*>(sObjectIds_5C1B70.Find_449CF0(field_158_wheel_id));
        if (pWheel)
        {
            pWheel->VStopTurning(TRUE);
            field_158_wheel_id = -1;
        }
        field_106_current_motion = Mud_Motion::M_TurnWheelEnd_59_474D30;
        field_108_next_motion = -1;
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

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

__int16 Mudokon::StableDelay_477570()
{
    if (sAlertedMudCount_5C3010 >= ALIVE_COUNTOF(kDelayTable_55CF7C))
    {
        sAlertedMudCount_5C3010 = ALIVE_COUNTOF(kDelayTable_55CF7C);
    }

    if (sDelayIdx_5C3014 >= sAlertedMudCount_5C3010)
    {
        sDelayIdx_5C3014 = 0;
    }

    return kDelayTable_55CF7C[sDelayIdx_5C3014++];
}

__int16 Mudokon::CheckForPortal_4775E0()
{
    if (sObjectIds_5C1B70.Find_449CF0(field_11C_bird_portal_id))
    {
        return 0;
    }

    // TODO: Refactor duplication

    auto pOpenPortal = static_cast<BirdPortal*>(Event_Get_422C00(kEventPortalOpen));
    if (pOpenPortal)
    {
        const FP xDist = pOpenPortal->field_2C_xpos - field_B8_xpos;
        if (FP_Abs(xDist) < FP_FromInteger(368))
        {
            if (FP_Abs(pOpenPortal->field_3C_YPos - field_BC_ypos) < FP_FromInteger(10))
            {
                if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), xDist))
                {
                    if (pOpenPortal->field_24_portal_type == PortalType::eWorker_1 || pOpenPortal->field_24_portal_type == PortalType::eShrykull_2)
                    {
                        sActiveHero_5C1B68->sub_45BB90(1);
                        field_11C_bird_portal_id = pOpenPortal->field_8_object_id;
                        ++word_5C3012;
                        field_16C |= 4u;
                        return 1;
                    }
                }
            }
        }
    }

    auto pPortal20 = static_cast<BirdPortal*>(Event_Get_422C00(kEventUnknown20));
    if (pPortal20)
    {
        const FP xDist = pPortal20->field_2C_xpos - field_B8_xpos;
        if (FP_Abs(xDist) < FP_FromInteger(368))
        {
            if (FP_Abs(pPortal20->field_3C_YPos - field_BC_ypos) < FP_FromInteger(10))
            {
                if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), xDist))
                {
                    if (pPortal20->field_24_portal_type == PortalType::eWorker_1 || pPortal20->field_24_portal_type == PortalType::eShrykull_2)
                    {
                        sActiveHero_5C1B68->sub_45BB90(1);
                        field_11C_bird_portal_id = pPortal20->field_8_object_id;

                        ++word_5C3012;
                        field_16C |= 4u;
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

    const int lastEventIdx = pEventSystem_5BC11C->field_28_last_event_index;
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
    if (Is_In_Current_Camera_424A70() != CameraPos::eCamCurrent_0)
    {
        return GameSpeakEvents::eNone_m1;
    }
    
    // Look out works on any scale
    if (actualEvent == GameSpeakEvents::Slig_LookOut_6)
    {
        return GameSpeakEvents::Slig_LookOut_6;
    }

    // Check in valid range and on same scale
    if (actualEvent < GameSpeakEvents::eUnknown_1 || 
        actualEvent > GameSpeakEvents::eSorry_24 ||
        sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
    {
        return actualEvent;
    }

    return GameSpeakEvents::eNone_m1;
}

__int16 Mudokon::LaughingGasInCurrentScreen_4774A0()
{
    return gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0) != 0 ? gLaughingGasOn_5C1BA4 : 0;
}

void Mudokon::Sound_475EC0(MudSounds idx)
{
    if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        if (field_180_emo_tbl != Mud_Emotion::eSick_7)
        {
            // TODO: Pass down the stronger types - assuming they map directly
            Mudokon_SFX_457EC0(idx, 80, field_13C_voice_pitch, this);
            return;
        }

        if (idx != MudSounds::eSick_24)
        {
            Mudokon_SFX_457EC0(idx, 80, -field_13C_voice_pitch, this);
            return;
        }

        short pitch1 = Math_RandomRange_496AB0(-5, 0);
        pitch1 *= 100;
        pitch1 = pitch1 - field_13C_voice_pitch;
        Mudokon_SFX_457EC0(MudSounds::eSick_24, Math_RandomRange_496AB0(20, 50), pitch1, this);
       
        short pitch2 = Math_RandomRange_496AB0(-5, 0);
        pitch2 *= 100;
        pitch2 = pitch2 - field_13C_voice_pitch;
        Mudokon_SFX_457EC0(MudSounds::eSadUgh_28, Math_RandomRange_496AB0(20, 60), pitch2, this);
        return;
    }

    if (field_180_emo_tbl == Mud_Emotion::eSick_7)
    {
        if (idx == MudSounds::eSick_24)
        {
            short pitch1 = Math_RandomRange_496AB0(-5, 0);
            pitch1 *= 100;
            pitch1 = pitch1 - field_13C_voice_pitch;
            Mudokon_SFX_457EC0(MudSounds::eSick_24, Math_RandomRange_496AB0(55, 90), pitch1, this);

            short pitch2 = Math_RandomRange_496AB0(-5, 0);
            pitch2 *= 100;
            pitch2 = pitch2 - field_13C_voice_pitch;
            Mudokon_SFX_457EC0(MudSounds::eSadUgh_28, Math_RandomRange_496AB0(60, 110), pitch2, this);
            return;
        }
        Mudokon_SFX_457EC0(idx, 0, field_13C_voice_pitch, this);
    }
    else
    {
        Mudokon_SFX_457EC0(idx, 0, field_13C_voice_pitch, this);
    }
}

__int16 Mudokon::CanRespond_4770B0()
{
    const int myDistToPlayer = Math_Distance_496EB0(
        FP_GetExponent(sActiveHero_5C1B68->field_B8_xpos),
        FP_GetExponent(sActiveHero_5C1B68->field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos));

    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Is there another object that isn't us on the same scale?
        if (pObj != this && pObj->field_CC_sprite_scale == sActiveHero_5C1B68->field_CC_sprite_scale)
        {
            // Is it a mud who isn't currently talking to abe and is in the same screen?
            if ((pObj->field_4_typeId == Types::eMudokon2_81 || pObj->field_4_typeId == Types::eMudokon_110) &&
                 static_cast<Mudokon*>(pObj)->field_18E_ai_state != Mud_AI_State::AI_ListeningToAbe_4_477B40 &&
                gMap_5C3030.Is_Point_In_Current_Camera_4810D0(pObj->field_C2_lvl_number, pObj->field_C0_path_number, pObj->field_B8_xpos, pObj->field_BC_ypos, 0))
            {
                if (sActiveHero_5C1B68->vIsFacingMe_4254A0(pObj) && !sActiveHero_5C1B68->vIsFacingMe_4254A0(this))
                {
                    // Abe is facing the other guy and not us :(
                    return FALSE;
                }

                const int otherMudDistanceToPlayer = Math_Distance_496EB0(
                    FP_GetExponent(sActiveHero_5C1B68->field_B8_xpos),
                    FP_GetExponent(sActiveHero_5C1B68->field_BC_ypos),
                    FP_GetExponent(pObj->field_B8_xpos),
                    FP_GetExponent(pObj->field_BC_ypos));

                if (myDistToPlayer > otherMudDistanceToPlayer)
                {
                    if (sActiveHero_5C1B68->vIsFacingMe_4254A0(pObj) == sActiveHero_5C1B68->vIsFacingMe_4254A0(this))
                    {
                        return FALSE;
                    }
                }
            }
        }
    }
    return TRUE;
}

BYTE** Mudokon::AnimBlockForMotion_474DC0(short motion)
{
    if (motion < Mud_Motion::M_LeverUse_10_473020)
    {
        return field_10_resources_array.ItemAt(0);
    }
    else if (motion < Mud_Motion::M_Chisel_11_4732D0)
    {
        if (!field_10_resources_array.ItemAt(1))
        {
            field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbepullResID, TRUE, FALSE));
        }
        return field_10_resources_array.ItemAt(1);
    }
    else if (motion < Mud_Motion::M_CrouchScrub_14_473560)
    {
        return field_10_resources_array.ItemAt(2);
    }
    else if (motion < Mud_Motion::M_CrouchIdle_15_474040)
    {
        return field_10_resources_array.ItemAt(3);
    }
    else if (motion < Mud_Motion::M_KnockForward_45_474180)
    {
        return field_10_resources_array.ItemAt(4);
    }
    else if (motion < Mud_Motion::M_Knockback_46_4742A0)
    {
        return field_10_resources_array.ItemAt(5);
    }
    else if (motion < Mud_Motion::M_FallOffEdge_48_4743C0)
    {
        return field_10_resources_array.ItemAt(6);
    }
    else if (motion < Mud_Motion::M_Chant_50_473040)
    {
        return field_10_resources_array.ItemAt(7);
    }
    else if (motion < Mud_Motion::M_ToDuck_52_474A20)
    {
        return field_10_resources_array.ItemAt(8);
    }
    else if (motion < Mud_Motion::M_DuckKnockback_55_474250)
    {
        return field_10_resources_array.ItemAt(9);
    }
    else if (motion < Mud_Motion::M_SlapOwnHead_56_4727D0)
    {
        return field_10_resources_array.ItemAt(10);
    }
    else if (motion < Mud_Motion::M_TurnWheelBegin_57_474C00)
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

void Mudokon::ToFalling_472320()
{
    if (!field_100_pCollisionLine)
    {
        VOnTrapDoorOpen();
        field_106_current_motion = Mud_Motion::M_Fall_49_472C60;
    }
}

void Mudokon::ToStand_4724A0()
{
    field_134_xVelSlowBy = FP_FromInteger(0);
    field_138_unused = 0;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_106_current_motion = Mud_Motion::M_Idle_0_4724E0;
}

__int16 Mudokon::FindWheel_4777B0(FP xpos, FP ypos)
{
    auto* pWheelTlv = static_cast<Path_Wheel*>(
        sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(xpos), FP_GetExponent(ypos), FP_GetExponent(xpos), FP_GetExponent(ypos), TlvTypes::WorkWheel_79));

    if (pWheelTlv)
    {
        if (!SwitchStates_Get_466020(pWheelTlv->field_12_id))
        {
            return FindObjectOfType_425180(Types::eWheel_148, xpos, ypos - (field_CC_sprite_scale * FP_FromInteger(50))) != 0;
        }

    }
    return 0;
}

__int16 Mudokon::GetAISubStateResponse_477AF0(MudAction action)
{
    field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, action);
    if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags::eBit11_get_depressed))
    {
        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
    }
    return field_188_pTblEntry->field_6_sub_state;
}

__int16 Mudokon::StopAtWheel_477880()
{
    FP offset = {};
    FP_Rect fpRect = {};

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        offset = -ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromDouble(0.4);
        fpRect.x = field_B8_xpos + (offset * FP_FromInteger(2));
        fpRect.w = field_B8_xpos;
    }
    else
    {
        offset = ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromDouble(0.4);
        fpRect.x = field_B8_xpos;
        fpRect.w = field_B8_xpos + (offset * FP_FromInteger(2));
    }

    fpRect.y = field_BC_ypos - FP_FromInteger(5);
    fpRect.h = field_BC_ypos;

    if (!FindWheel_4777B0(field_B8_xpos + offset, field_BC_ypos))
    {
        // No wheel so don't stop
        return 0;
    }

    const PSX_RECT ourRect =
    {
        FP_GetExponent(fpRect.x),
        FP_GetExponent(fpRect.y),
        FP_GetExponent(fpRect.w),
        FP_GetExponent(fpRect.h)
    };


    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Found another mud who isn't us
        if (pObj != this && pObj->field_4_typeId == Types::eMudokon_110)
        {
            PSX_RECT bRect = {};
            pObj->vGetBoundingRect_424FD0(&bRect, 1);

            auto pOtherMud = static_cast<Mudokon*>(pObj);

            // Check if the other mud has already take the spot of this work wheel
            if (bRect.x <= ourRect.w && 
                bRect.w >= ourRect.x &&
                bRect.h >= ourRect.y &&
                bRect.y <= ourRect.h)
            {
                if (pOtherMud->field_16A_flags.Get(Flags::eBit7_stopped_at_wheel) ||
                    (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                        FP_GetExponent(pObj->field_B8_xpos),
                        FP_GetExponent(pObj->field_BC_ypos),
                        FP_GetExponent(pObj->field_B8_xpos),
                        FP_GetExponent(pObj->field_BC_ypos),
                        TlvTypes::WorkWheel_79) && pOtherMud->field_C4_velx == FP_FromInteger(0)))
                {
                    // Another mud has stolen this wheel so don't stop
                    return 0;
                }
            }
        }
    }
    return 1;
}

void Mudokon::StandingKnockBack_473190()
{
    auto pWheel = static_cast<WorkWheel*>(sObjectIds_5C1B70.Find_449CF0(field_158_wheel_id));
    if (pWheel)
    {
        pWheel->VStopTurning(TRUE);
        field_158_wheel_id = -1;
    }

    Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 32767, this);

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B8_xpos += (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2)) + field_C4_velx;
        }
        else
        {
            field_B8_xpos -= (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2)) - field_C4_velx;
        }
    }

    MapFollowMe_408D10(TRUE);

    field_C4_velx = FP_FromInteger(0);

    if (field_C8_vely < FP_FromInteger(0))
    {
        field_C8_vely = FP_FromInteger(0);
    }

    Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 32767, this);
    field_106_current_motion = Mud_Motion::M_Knockback_46_4742A0;
}

// TODO: Abe sneaking or walking check ??
__int16 Mudokon::IsMotionUnknown_4730F0()
{
    return 
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_40_SneakLoop_450550 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_45_SneakBegin_4507A0 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_41_WalkToSneak_450250 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_43_MidWalkToSneak_450380 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_42_SneakToWalk_4503D0 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_44_MidSneakToWalk_450500;
}

void Mudokon::MoveOnLine_4720D0()
{
    PlatformBase* pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    
    ToFalling_472320();

    const FP oldXPos = field_B8_xpos;
    if (field_100_pCollisionLine)
    {
        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
    }

    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
    }

    if (field_100_pCollisionLine)
    {
        if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
        {
            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            vOnCollisionWith_424EE0(
                { bRect.x, static_cast<short>(bRect.y + 5) },
                { bRect.w, static_cast<short>(bRect.h + 5) },
                ObjList_5C1B78, 1, (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
        }
    }
    else
    {
        field_134_xVelSlowBy = FP_FromDouble(0.3); // TODO: or 2.99 ??
        field_F8_LastLineYPos = field_BC_ypos;
        field_106_current_motion = Mud_Motion::M_FallOffEdge_48_4743C0;
        field_B8_xpos = oldXPos + field_C4_velx;
    }

}

const struct MudEmotionTableEntry* CC Mudokon::ResponseTo_471730(Mud_Emotion emotion, MudAction action)
{
    return &kMudEmoTable_55C790.field_0_data[static_cast<int>(emotion)][static_cast<int>(action)];
}

void Mudokon::AddAlerted()
{
    if (!field_16A_flags.Get(Flags::eBit3_alerted))
    {
        field_16A_flags.Set(Flags::eBit3_alerted);
        sAlertedMudCount_5C3010++;
        LOG_INFO("Alert added " << sAlertedMudCount_5C3010);
    }
}

void Mudokon::RemoveAlerted()
{
    if (field_16A_flags.Get(Flags::eBit3_alerted))
    {
        sAlertedMudCount_5C3010--;
        field_16A_flags.Clear(Flags::eBit3_alerted);
        LOG_INFO("Alert removed " << sAlertedMudCount_5C3010);
    }
}

__int16 Mudokon::AIStartWheelTurning()
{
    field_108_next_motion = Mud_Motion::M_TurnWheelBegin_57_474C00;
    RemoveAlerted();
    field_16A_flags.Set(Flags::eBit7_stopped_at_wheel);
    field_18E_ai_state = Mud_AI_State::AI_TurnWheel_3_47E0D0;
    return AI_ListeningToAbe::eState4_Inactive_0;
}

bool Mudokon::NeedToTurnAround()
{
    Mudokon* pAboveMud = static_cast<Mudokon*>(FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
    if (sAlertedMudCount_5C3010 > 1 && pAboveMud && !vIsFacingMe_4254A0(pAboveMud) && pAboveMud->field_106_current_motion != Mud_Motion::M_StandingTurn_2_472BF0)
    {
        // A mud is in the same grid us and not turning, so we turn to face it. This causes "stacked" muds to all face the same way bar 1.
        // When telling them all to "wait" they will face Abe and then spin around again :D.
        return true;
    }
    else if (!vIsFacingMe_4254A0(sActiveHero_5C1B68) && (sAlertedMudCount_5C3010 <= 1 || !FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5))))
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
    vGetBoundingRect_424FD0(&bMudRect, 1);

    for (int i=0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eMine_88 || pObj->field_4_typeId == Types::eUXB_143)
        {
            PSX_RECT bBombRect = {};
            pObj->vGetBoundingRect_424FD0(&bBombRect, 1);

            // TODO: Almost same as PSX_Rects_overlap_no_adjustment but checks < rather than <=
            if (bMudRect.x <= bBombRect.w &&
                bMudRect.w >= bBombRect.x &&
                bMudRect.h >= bBombRect.y &&
                bMudRect.y <= bBombRect.h)
            {
                pObj->VTakeDamage_408730(this);
            }
        }
    }
}

void Mudokon::HurtSound_475DB0()
{
    const unsigned __int8 random = Math_RandomRange_496AB0(0, 127) >= 64 ? 16 : 9;
    const short min = FP_GetExponent((FP_FromInteger(1) - field_10C_health) / FP_FromDouble(0.15));
    const __int16 bInCamera = gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0);
    Mudokon_SFX_457EC0(static_cast<MudSounds>(random), bInCamera ? 0 : 80, Math_RandomRange_496AB0(200 * min, 40 * (5 * min + 5)), this);
}

void Mudokon::TakeASlap_476090(BaseGameObject* pFrom)
{
    if (field_180_emo_tbl != Mud_Emotion::eSick_7)
    {
        field_10C_health -= FP_FromDouble(0.077);
    }

    if (field_10C_health > FP_FromInteger(0))
    {
        HurtSound_475DB0();
        field_DA_xOffset = field_162_maxXOffset;
        field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eSlapOrWater_7);
        field_194_timer = sGnFrame_5C1B84 + 30;
        field_17E_delayed_speak = MudAction::eNone_17;

        if (field_188_pTblEntry->field_6_sub_state)
        {
            AddAlerted();

            if (field_16A_flags.Get(Flags::eBit5_following) && (field_180_emo_tbl == Mud_Emotion::eHappy_5 || field_180_emo_tbl == Mud_Emotion::eWired_6))
            {
                field_178_sub_state2 = 4;
            }
            else
            {
                field_178_sub_state2 = 7;
                field_16A_flags.Clear(Flags::eBit5_following);
            }

            field_18E_ai_state = Mud_AI_State::AI_ListeningToAbe_4_477B40;
            field_190_sub_state = field_188_pTblEntry->field_6_sub_state;
        }

        if (field_180_emo_tbl != Mud_Emotion::eSad_3 || pFrom == sActiveHero_5C1B68)
        {
            field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
        }
    }
    else
    {
        Mudokon_SFX_457EC0(MudSounds::eHurt2_9, 0, 1000, this);
        Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 32767, this);
        Event_Broadcast_422BC0(kEventMudokonDied, sActiveHero_5C1B68);
        field_16A_flags.Clear(Flags::eBit2_save_state);
        field_10C_health = FP_FromInteger(0);
        field_18E_ai_state = Mud_AI_State::AI_ShrivelDeath_5_4714A0;
        field_106_current_motion = Mud_Motion::M_KnockForward_45_474180;
        field_194_timer = sGnFrame_5C1B84 + 90;
        field_108_next_motion = -1;
        vUpdateAnimRes_474D80();
    }
}

int Mudokon::GetResponseEntryIdx_471760()
{
    if (field_188_pTblEntry)
    {
        return field_188_pTblEntry - &kMudEmoTable_55C790.field_0_data[0][0];
    }
    return -1;
}

const MudEmotionTableEntry* CCSTD Mudokon::GetResponseEntry_471790(int idx)
{
    if (idx == -1)
    {
        return nullptr;
    }

    return &kMudEmoTable_55C790.field_0_data[0][0] + idx;
}
