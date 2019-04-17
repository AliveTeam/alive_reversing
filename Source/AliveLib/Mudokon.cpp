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
#include "Midi.hpp"
#include "ScreenManager.hpp"
#include "Switch.hpp"

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

using TMudAIStateFunction = decltype(&Mudokon::AI_Give_rings_0_470C10);

const TMudAIStateFunction sMudokon_AI_Table_55CDF0[10] =
{
    &Mudokon::AI_Give_rings_0_470C10,
    &Mudokon::AI_Chisel_1_47C5F0,
    &Mudokon::AI_Scrub_2_47D270,
    &Mudokon::AI_State_3_47E0D0,
    &Mudokon::AI_Wired_4_477B40,
    &Mudokon::AI_ShrivelDeath_5_4714A0,
    &Mudokon::AI_HelloAlerted_6_47A560,
    &Mudokon::AI_FallAndSmackDeath_7_471600,
    &Mudokon::AI_AngryWorker_8_47E910,
    &Mudokon::AI_Sick_9_47A910
};

#define MUD_MOTION_STATES_ENUM(ENTRY) \
    ENTRY(StandIdle_0_4724E0) \
    ENTRY(Walking_1_4728B0) \
    ENTRY(TurnAroundStanding_2_472BF0) \
    ENTRY(Speak_Generic_472FA0) \
    ENTRY(Speak_Generic_472FA0) \
    ENTRY(Speak_Generic_472FA0) \
    ENTRY(Speak_Generic_472FA0) \
    ENTRY(StandToWalk_7_472AB0) \
    ENTRY(WalkingToStand_8_472B30) \
    ENTRY(jWalkingToStand_8_472BD0) \
    ENTRY(PullLever_10_473020) \
    ENTRY(Chisel_11_4732D0) \
    ENTRY(StartChisel_12_473500) \
    ENTRY(StopChisel_13_473530) \
    ENTRY(CrouchScrub_14_473560) \
    ENTRY(CrouchIdle_15_474040) \
    ENTRY(CrouchTurn_16_4740E0) \
    ENTRY(StandToCrouch_17_474120) \
    ENTRY(CrouchToStand_18_474150) \
    ENTRY(StandingToRun1_19_473600) \
    ENTRY(StandingToRun2_20_4736D0) \
    ENTRY(Running_21_473720) \
    ENTRY(RunToWalk1_22_4738E0) \
    ENTRY(RunToWalk2_23_4739B0) \
    ENTRY(RunSlideStop_24_473A00) \
    ENTRY(RunSlideTurn_25_473AA0) \
    ENTRY(RunTurn_26_473BB0) \
    ENTRY(Sneaking_27_473C40) \
    ENTRY(WalkToSneak1_28_473D60) \
    ENTRY(SneakingToWalk_29_473E20) \
    ENTRY(WalkToSneak2_30_473EE0) \
    ENTRY(SneakingToWalk2_31_473F30) \
    ENTRY(StandToSneaking_32_473F80) \
    ENTRY(SneakingToStand1_33_473FF0) \
    ENTRY(SneakingToStand2_34_474020) \
    ENTRY(JumpStart_35_474460) \
    ENTRY(JumpMid_36_474570) \
    ENTRY(WalkToRun_37_4749A0) \
    ENTRY(Slap_38_474AA0) \
    ENTRY(StartHoistJumpUp_39_4748E0) \
    ENTRY(HoistFallToFloor_40_474780) \
    ENTRY(HitFloorStanding1_41_474960) \
    ENTRY(HitFloorStanding2_42_4743F0) \
    ENTRY(StandToDunno_43_472790) \
    ENTRY(DunnoToStand_44_4727B0) \
    ENTRY(KnockForward_45_474180) \
    ENTRY(StandToKnockBack_46_4742A0) \
    ENTRY(KnockBackToStand_47_474380) \
    ENTRY(FallLedgeBegin_48_4743C0) \
    ENTRY(Fall_49_472C60) \
    ENTRY(Chanting_50_473040) \
    ENTRY(ToChant_51_4730D0) \
    ENTRY(ToDuck_52_474A20) \
    ENTRY(Duck_53_474A40) \
    ENTRY(DuckToCrouch_54_474A80) \
    ENTRY(DuckKnockBack_55_474250) \
    ENTRY(SlapOwnHead_56_4727D0) \
    ENTRY(TurnWheelBegin_57_474C00) \
    ENTRY(TurnWheelLoop_58_474CC0) \
    ENTRY(TurnWheelEnd_59_474D30)

const char* const sMudMotionStateNames[60] =
{
    MUD_MOTION_STATES_ENUM(MAKE_STRINGS)
};


enum class MudSounds : __int16
{
    eNone = -1,
    e0 = 0,
    e1 = 1,
    e2 = 2,
    e3 = 3,
    e4 = 4,
    e5 = 5,
    e6 = 6,
    e7 = 7,
    e8 = 8,
    e9 = 9,
    e10 = 10,
    e11 = 11,
    e12 = 12,
    e13 = 13,
    e14 = 14,
    e15 = 15,
    e16 = 16,
    e17 = 17,
    e18 = 18,
    e19 = 19,
    e20 = 20,
    e21 = 21,
    e22 = 22,
    e23 = 23,
    e24 = 24,
    e25 = 25,
    e26 = 26,
    e27 = 27,
    e28 = 28
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
            { MudSounds::e3,  Mud_Motion::Speak_Generic_4_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::e12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::e12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::e12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::eNone, Mud_Motion::StandToDunno_43_472790,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::e23, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::e5,  Mud_Motion::Slap_38_474AA0,             Mud_Emotion::eUnknown_2, 13 },
            { MudSounds::eNone, Mud_Motion::StandToDunno_43_472790,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::e10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::e28, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  12 }
        },
        {
            { MudSounds::e18, Mud_Motion::Speak_Generic_4_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::e21, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eAngry_1,   0 },
            { MudSounds::e21, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eNone, Mud_Motion::StandToDunno_43_472790,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::e5,  Mud_Motion::Slap_38_474AA0,             Mud_Emotion::eAngry_1,   12 },
            { MudSounds::e5,  Mud_Motion::Slap_38_474AA0,             Mud_Emotion::eUnknown_2, 13 },
            { MudSounds::e12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eAngry_1,   0 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 }
        },
        {
            { MudSounds::e18, Mud_Motion::Speak_Generic_4_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::e21, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eAngry_1,   0 },
            { MudSounds::e21, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::e5,  Mud_Motion::Slap_38_474AA0,             Mud_Emotion::eAngry_1,   12 },
            { MudSounds::e5,  Mud_Motion::Slap_38_474AA0,             Mud_Emotion::eUnknown_2, 13 },
            { MudSounds::e12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eAngry_1,   0 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 }
        },
        {
            { MudSounds::e20, Mud_Motion::Speak_Generic_4_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSad_3,     0 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNone, Mud_Motion::StandToDunno_43_472790,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 13 },
            { MudSounds::e12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eNormal_0,  0 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 }
        },
        {
            { MudSounds::e20, Mud_Motion::Speak_Generic_4_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eUnknown_4, 0 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { MudSounds::e28, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eUnknown_4, 0 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 13 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eNormal_0,  0 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eUnknown_4, 0 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eUnknown_4, 0 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eUnknown_4, 0 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eUnknown_4, 0 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eUnknown_4, 0 }
        },
        {
            { MudSounds::e19, Mud_Motion::Speak_Generic_4_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e10, Mud_Motion::StartHoistJumpUp_39_4748E0, Mud_Emotion::eWired_6,   12 },
            { MudSounds::e12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e13, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  13 },
            { MudSounds::eNone, Mud_Motion::StandToDunno_43_472790,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eWired_6,   0 },
            { MudSounds::e10, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e10, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 }
        },
        {
            { MudSounds::e19, Mud_Motion::Speak_Generic_4_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e10, Mud_Motion::StartHoistJumpUp_39_4748E0, Mud_Emotion::eWired_6,   12 },
            { MudSounds::e12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e13, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  13 },
            { MudSounds::eNone, Mud_Motion::StandToDunno_43_472790,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eWired_6,   0 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eWired_6,   0 },
            { MudSounds::e10, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e10, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { MudSounds::e10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 }
        },
        {
            { MudSounds::e24, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSick_7, 12 },
            { MudSounds::e24, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSick_7, 12 },
            { MudSounds::e24, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eSick_7, 12 },
            { MudSounds::e24, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eSick_7, 12 },
            { MudSounds::e24, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSick_7, 12 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::e28, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSick_7, 13 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::eNone, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { MudSounds::e24, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSick_7, 12 },
            { MudSounds::e24, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSick_7, 12 }
        }
    }
};



using TMudStateFunction = decltype(&Mudokon::StandIdle_0_4724E0);

const TMudStateFunction sMudokon_motion_states_55CE18[60] =
{
    &Mudokon::StandIdle_0_4724E0,
    &Mudokon::Walking_1_4728B0,
    &Mudokon::TurnAroundStanding_2_472BF0,
    &Mudokon::Speak_Generic_472FA0,
    &Mudokon::Speak_Generic_472FA0,
    &Mudokon::Speak_Generic_472FA0,
    &Mudokon::Speak_Generic_472FA0,
    &Mudokon::StandToWalk_7_472AB0,
    &Mudokon::WalkingToStand_8_472B30,
    &Mudokon::jWalkingToStand_8_472BD0,
    &Mudokon::PullLever_10_473020,
    &Mudokon::Chisel_11_4732D0,
    &Mudokon::StartChisel_12_473500,
    &Mudokon::StopChisel_13_473530,
    &Mudokon::CrouchScrub_14_473560,
    &Mudokon::CrouchIdle_15_474040,
    &Mudokon::CrouchTurn_16_4740E0,
    &Mudokon::StandToCrouch_17_474120,
    &Mudokon::CrouchToStand_18_474150,
    &Mudokon::StandingToRun1_19_473600,
    &Mudokon::StandingToRun2_20_4736D0,
    &Mudokon::Running_21_473720,
    &Mudokon::RunToWalk1_22_4738E0,
    &Mudokon::RunToWalk2_23_4739B0,
    &Mudokon::RunSlideStop_24_473A00,
    &Mudokon::RunSlideTurn_25_473AA0,
    &Mudokon::RunTurn_26_473BB0,
    &Mudokon::Sneaking_27_473C40,
    &Mudokon::WalkToSneak1_28_473D60,
    &Mudokon::SneakingToWalk_29_473E20,
    &Mudokon::WalkToSneak2_30_473EE0,
    &Mudokon::SneakingToWalk2_31_473F30,
    &Mudokon::StandToSneaking_32_473F80,
    &Mudokon::SneakingToStand1_33_473FF0,
    &Mudokon::SneakingToStand2_34_474020,
    &Mudokon::JumpStart_35_474460,
    &Mudokon::JumpMid_36_474570,
    &Mudokon::WalkToRun_37_4749A0,
    &Mudokon::Slap_38_474AA0,
    &Mudokon::StartHoistJumpUp_39_4748E0,
    &Mudokon::HoistFallToFloor_40_474780,
    &Mudokon::HitFloorStanding1_41_474960,
    &Mudokon::HitFloorStanding2_42_4743F0,
    &Mudokon::StandToDunno_43_472790,
    &Mudokon::DunnoToStand_44_4727B0,
    &Mudokon::KnockForward_45_474180,
    &Mudokon::StandToKnockBack_46_4742A0,
    &Mudokon::KnockBackToStand_47_474380,
    &Mudokon::FallLedgeBegin_48_4743C0,
    &Mudokon::Fall_49_472C60,
    &Mudokon::Chanting_50_473040,
    &Mudokon::ToChant_51_4730D0,
    &Mudokon::ToDuck_52_474A20,
    &Mudokon::Duck_53_474A40,
    &Mudokon::DuckToCrouch_54_474A80,
    &Mudokon::DuckKnockBack_55_474250,
    &Mudokon::SlapOwnHead_56_4727D0,
    &Mudokon::TurnWheelBegin_57_474C00,
    &Mudokon::TurnWheelLoop_58_474CC0,
    &Mudokon::TurnWheelEnd_59_474D30
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

    field_154 = 0;
    field_140 = -1;
    field_156 = -1;

    SetVTable(this, 0x5462E4);

    field_11C_bird_portal_id = -1;
    field_12C = -1;
    field_158_wheel_id = -1;
    field_4_typeId = Types::eMudokon2_81; // TODO: Set to 110 later, what is 81 ??
    field_C_objectId = tlvInfo;
    field_194_timer = 0;
    field_18E_ai_state = Mud_AI_State::AI_Give_rings_0_470C10;
    field_190_sub_state = 0;
    field_108_next_motion = -1;
    field_192 = 0;
    field_13C_voice_pitch = 0;

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebsic1ResID, TRUE, FALSE));
    field_10_resources_array.SetAt(1, nullptr);
    Animation_Init_424E10(58888, 135, 80, field_10_resources_array.ItemAt(0), 1, 1);
    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_DC_bApplyShadows |= 2u;

    field_114_flags.Clear(Flags_114::e114_Bit9);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_18C = 0;
    SetTint_425600(kMudTints_55C744, field_C2_lvl_number);

    field_17E_delayed_speak = MudAction::eUnknown_17;

    field_16A_flags.Clear(Flags::eBit2_save_state);
    field_16A_flags.Clear(Flags::eBit3_Alerted);
    field_16A_flags.Set(Flags::eBit4_blind, pTlv->field_22_bBlind & 1);
    field_16A_flags.Clear(Flags::eBit5);
    field_16A_flags.Clear(Flags::eBit6_StandingForSad);
    field_16A_flags.Clear(Flags::eBit7);
    field_16A_flags.Clear(Flags::eBit8_DoAngry);
    field_16A_flags.Clear(Flags::eBit9);
    // 10 and 11 ??
    field_16A_flags.Set(Flags::eBit12);
    field_16A_flags.Clear(Flags::eBit13);
    field_16A_flags.Clear(Flags::eBit14);

    field_180_emo_tbl = TLV_Emo_To_Internal_Emo(pTlv->field_20_emotion);
    field_188_pTblEntry = nullptr;
    field_182 = -1;
    field_120_angry_trigger = pTlv->field_24_angry_trigger;
    field_16C &= ~6u;
    field_198 = 0;

    switch (pTlv->field_12_state)
    {
    case Mud_State::eChisle_0:
        field_18E_ai_state = Mud_AI_State::AI_Chisel_1_47C5F0;
        field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMudchslResID, TRUE, FALSE));
        break;

    case Mud_State::eScrub_1:
        field_18E_ai_state = Mud_AI_State::AI_Scrub_2_47D270;
        field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMudscrubResID, TRUE, FALSE));
        break;

    case Mud_State::eAngryWorker_2:
        field_18E_ai_state = Mud_AI_State::AI_AngryWorker_8_47E910;
        field_180_emo_tbl = Mud_Emotion::eAngry_1;
        field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMudscrubResID, TRUE, FALSE));
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
        field_18E_ai_state = Mud_AI_State::AI_Give_rings_0_470C10;
        field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeommResID, TRUE, FALSE));
        break;

    default:
        break;
    }

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        field_18E_ai_state = Mud_AI_State::AI_Wired_4_477B40;
    }

    field_10_resources_array.SetAt(9, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMudoduckResID, TRUE, FALSE));
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMudbtlnkResID, TRUE, FALSE));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebasicResID, TRUE, FALSE));
    field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeknfdResID, TRUE, FALSE));
    field_10_resources_array.SetAt(6, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeknbkResID, TRUE, FALSE));
    field_10_resources_array.SetAt(7, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeedgeResID, TRUE, FALSE));
    field_10_resources_array.SetAt(11, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMudidleResID, TRUE, FALSE));
    field_10_resources_array.SetAt(13, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, ResourceID::kMudangryResID, TRUE, FALSE));
    field_10_resources_array.SetAt(14, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, ResourceID::kMudsadResID, TRUE, FALSE));
    field_10_resources_array.SetAt(15, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, ResourceID::kMudwiredResID, TRUE, FALSE));
    field_10_resources_array.SetAt(16, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, ResourceID::kMudblindResID, TRUE, FALSE));
    field_10_resources_array.SetAt(17, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, ResourceID::kMudsickResID, TRUE, FALSE));

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

    field_17C = 0;

    vStackOnObjectsOfType_425840(Types::eMudokon_110);

    if (field_180_emo_tbl == Mud_Emotion::eSick_7)
    {
        field_18E_ai_state = Mud_AI_State::AI_Sick_9_47A910;
        field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
        field_106_current_motion = Mud_Motion::CrouchIdle_15_474040;
    }
    else
    {
        field_106_current_motion = Mud_Motion::StandIdle_0_4724E0;
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
    const short bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
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
    field_F8 = field_BC_ypos;

    if (field_18E_ai_state == Mud_AI_State::AI_Scrub_2_47D270 || field_18E_ai_state == Mud_AI_State::AI_Chisel_1_47C5F0)
    {
        MapFollowMe_408D10(TRUE);
    }

    field_16A_flags.Set(Flags::eBit1);
    field_190_sub_state = 0;
    field_118 = field_C_objectId;

    field_128_angry_timer = 0;
    field_160_delayed_speak = -1;
    field_162 = field_DA_xOffset;
    
    field_E0_pShadow = alive_new<Shadow>();
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
                    { static_cast<short>(bRect.x + 5), bRect.y },
                    { static_cast<short>(bRect.w + 5), bRect.h }, 
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
                    if (field_18E_ai_state == Mud_AI_State::AI_HelloAlerted_6_47A560 && field_190_sub_state == 3)
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
        field_6_flags.Set(BaseGameObject::eDead);
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
    else if (field_192)
    {
        field_106_current_motion = field_F4;
        vUpdateAnimRes_474D80();
        field_20_animation.SetFrame_409D50(field_F6_anim_frame);
        field_192 = 0;
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
    case Mud_Emotion::eUnknown_2:
        SetRGB(63, 63, 63);
        field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(13), 0);
        break;

    case Mud_Emotion::eSad_3:
    case Mud_Emotion::eUnknown_4:
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
    NOT_IMPLEMENTED();

    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        if (!field_114_flags.Get(Flags_114::e114_Bit1))
        {
            VSetMotion_4081C0(Mud_Motion::FallLedgeBegin_48_4743C0);
        }

        pPlatform->VRemove(this);
        field_110_id = -1;
    }
}

const int kMudFrameTableOffsets_55CD00[60] =
{
    58888,    58808,    59064,    59028,    58956,
    58992,    58920,    58768,    58788,    58748,
    11396,    5276,     5308,     5320,     9388,
    270092,   270428,   269876,   270060,   270120,
    270268,   270748,   270668,   270820,   270860,
    270328,   270408,   270508,   270688,   270596,
    270840,   270728,   270288,   270160,   270308,
    270468,   269804,   270252,   271152,   269976,
    270024,   269928,   269928,   270180,   270616,
    17240,    49740,    49800,    32040,    32012,
    9992,     10040,    5236,     5256,     5280,
    5328,     9640,     11856,    11816,    11888
};


void Mudokon::vUpdateAnimRes_474D80()
{
    BYTE** ppRes = AnimBlockForMotion_474DC0(field_106_current_motion);
    if (!ppRes)
    {
        LOG_ERROR("No res for " << field_106_current_motion);
    }
    field_20_animation.Set_Animation_Data_409C80(kMudFrameTableOffsets_55CD00[field_106_current_motion], ppRes);
}

__int16 Mudokon::AI_Give_rings_0_470C10()
{
    NOT_IMPLEMENTED();
    return 0;
}

enum BrainStates1
{
    eState_StandToCrouch_0 = 0,
    eState_ChisleTheFloor_1 = 1,
    eState_CrouchToStand_2 = 2,
    eState_3 = 3,
    eState_SadStandUp_4 = 4,
    eState_Duck_5 = 5,
    eState_OutOfDuck_6 = 6,
    eState_DuckToChisle_7 = 7,
    eState_DuckKnockBack_8 = 8
};


enum BrainStates4
{
    eState4_0 = 0,
    eState4_1 = 1,
    eState4_2 = 2,
    eState4_3 = 3,
    eState4_4 = 4,
    eState4_5 = 5,
    eState4_6_RunAfterAbe = 6,
    eState4_7 = 7,
    eState4_8 = 8,
    eState4_9 = 9,
    eState4_10 = 10,
    eState4_11 = 11,
    eState4_12 = 12,
    eState4_13 = 13,
    eState4_14 = 14,
    eState4_15 = 15,
    eState4_16 = 16,
    eState4_17 = 17,
    eState4_18 = 18,
    eState4_19 = 19,
    eState4_20 = 20,
    eState4_21 = 21,
    eState4_22 = 22,
};


const __int16 kDelayTable_55CF7C[6] = { 0, 6, 12, 18, 24, 30, };
ALIVE_VAR(1, 0x5C3014, short, sDelayIdx_5C3014, 0);
ALIVE_VAR(1, 0x5C3010, short, sAlertedMudCount_5C3010, 0);

ALIVE_VAR(1, 0x5C3018, short, word_5C3018, 0);

signed __int16 Mudokon::AI_Chisel_1_47C5F0()
{
    if (CheckForPortal_4775E0())
    {
        AddAlerted();
        field_18E_ai_state = Mud_AI_State::AI_HelloAlerted_6_47A560;
        return BrainStates1::eState_StandToCrouch_0;
    }

    if (field_16C & 2)
    {
        if (field_190_sub_state != BrainStates1::eState_3)
        {
            word_5C3018 = 0;
            field_16C &= ~2u;
        }
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
    if (lastSpeak == GameSpeakEvents::eLookOut_6 || Event_Get_422C00(kEventShooting))
    {
        if (field_190_sub_state != BrainStates1::eState_3 && field_190_sub_state != BrainStates1::eState_Duck_5 && field_190_sub_state != BrainStates1::eState_DuckKnockBack_8 && field_190_sub_state != BrainStates1::eState_OutOfDuck_6)
        {
            field_190_sub_state = BrainStates1::eState_Duck_5;
        }
    }

    BaseAnimatedWithPhysicsGameObject* pAbuseEvent = Event_Is_Event_In_Range_422C30(kEventMudokonAbuse, field_B8_xpos, field_BC_ypos, field_D6_scale);
    BaseAnimatedWithPhysicsGameObject* pDeadMudEvent = Event_Is_Event_In_Range_422C30(kEventMudokonDied, field_B8_xpos, field_BC_ypos, field_D6_scale);
    BaseAnimatedWithPhysicsGameObject* pLoudNoiseEvent = Event_Is_Event_In_Range_422C30(kEventLoudNoise, field_B8_xpos, field_BC_ypos, field_D6_scale);

    const bool reactToAbused = (pAbuseEvent && pAbuseEvent != this && field_190_sub_state != BrainStates1::eState_3 && 
        gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0));

    const bool reactToDeadMud = (pDeadMudEvent && pDeadMudEvent != this && field_190_sub_state != BrainStates1::eState_3 && 
        gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0));

    const bool reactToLoudNoise = (pLoudNoiseEvent && pLoudNoiseEvent->field_4_typeId == Types::eGlukkon_67 && field_190_sub_state != BrainStates1::eState_3 && 
        gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0));

    if (reactToAbused)
    {
        field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_9);
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

        if (field_180_emo_tbl == Mud_Emotion::eUnknown_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
        {
            AddAlerted();

            if (field_180_emo_tbl == Mud_Emotion::eSad_3)
            {
                field_16A_flags.Set(Flags::eBit6_StandingForSad);
            }

            field_190_sub_state = BrainStates1::eState_3;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (LaughingGasInCurrentScreen_4774A0())
        {
            field_180_emo_tbl = Mud_Emotion::eWired_6;

            AddAlerted();

            field_190_sub_state = BrainStates1::eState_3;
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

    if (!field_16A_flags.Get(Flags::eBit8_DoAngry))
    {
        if (field_128_angry_timer > 0)
        {
            if (field_128_angry_timer <= static_cast<int>(sGnFrame_5C1B84))
            {
                field_16A_flags.Set(Flags::eBit8_DoAngry);
                field_180_emo_tbl = Mud_Emotion::eAngry_1;
                field_190_sub_state = BrainStates1::eState_3;
                field_194_timer = sGnFrame_5C1B84;
            }
        }
    }

    switch (field_190_sub_state)
    {
    case BrainStates1::eState_StandToCrouch_0:
        if (field_106_current_motion != Mud_Motion::StandToCrouch_17_474120)
        {
            field_106_current_motion = Mud_Motion::Chisel_11_4732D0;
            field_108_next_motion = -1;
        }
        field_194_timer = (Math_NextRandom() % 64) + (sGnFrame_5C1B84 + 35);
        return BrainStates1::eState_ChisleTheFloor_1;

    case BrainStates1::eState_ChisleTheFloor_1:
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
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
            word_5C3018 = 1;
            field_16C |= 2u;

            AddAlerted();
            return BrainStates1::eState_3;
        }

        if (field_106_current_motion == Mud_Motion::StandIdle_0_4724E0)
        {
            field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
        }

        if (static_cast<int>(sGnFrame_5C1B84) > field_194_timer && field_106_current_motion == Mud_Motion::Chisel_11_4732D0)
        {
            field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
        }

        if (field_106_current_motion != Mud_Motion::CrouchIdle_15_474040)
        {
            return field_190_sub_state;
        }

        if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && (Math_NextRandom() < 120))
        {
            AddAlerted();

            field_16A_flags.Set(Flags::eBit14);
            field_16A_flags.Set(Flags::eBit6_StandingForSad);

            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
            return BrainStates1::eState_3;
        }
        else
        {
            field_194_timer = (Math_NextRandom() % 64) + (sGnFrame_5C1B84 + 35);
            return BrainStates1::eState_CrouchToStand_2;
        }
    }
    break;

    case BrainStates1::eState_CrouchToStand_2:
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

            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
            word_5C3018 = 1;
            field_16C |= 2u;
            return BrainStates1::eState_3;
        }

        if (field_106_current_motion == Mud_Motion::StandIdle_0_4724E0)
        {
            field_108_next_motion = Mud_Motion::StandToCrouch_17_474120;
        }

        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }

        field_194_timer = (Math_NextRandom() % 64) + (sGnFrame_5C1B84 + 35);
        field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
    }
    return BrainStates1::eState_ChisleTheFloor_1;

    case BrainStates1::eState_3:
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

        if (!skip && (bForce || !word_5C3018 && CanRespond_4770B0()))
        {
            if (sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                AddAlerted();
                word_5C3018 = 1;
                field_16C |= 2u;

                field_16A_flags.Clear(Flags::eBit14);
                field_16A_flags.Clear(Flags::eBit6_StandingForSad);
            }
        }

        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer || field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
        {
            if (field_108_next_motion != -1)
            {
                return field_190_sub_state;
            }

            if (field_106_current_motion == Mud_Motion::Chisel_11_4732D0 || field_106_current_motion == Mud_Motion::CrouchIdle_15_474040)
            {
                field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            }

            if (field_106_current_motion != Mud_Motion::Duck_53_474A40)
            {
                return field_190_sub_state;
            }
            else
            {
                field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
                return field_190_sub_state;
            }
        }

        if (field_180_emo_tbl == Mud_Emotion::eSad_3 && field_16A_flags.Get(Flags::eBit14))
        {
            field_16A_flags.Clear(Flags::eBit12);
            field_16A_flags.Clear(Flags::eBit14);
            field_108_next_motion = Mud_Motion::Speak_Generic_6_472FA0;
            Sound_475EC0(MudSounds::e28);
            return BrainStates1::eState_SadStandUp_4;
        }
        else
        {
            word_5C3018 = 0;
            field_16C &= ~2u;
            if (field_180_emo_tbl != Mud_Emotion::eAngry_1 || !field_16A_flags.Get(Flags::eBit8_DoAngry))
            {
                field_18E_ai_state = Mud_AI_State::AI_Wired_4_477B40;
                return BrainStates4::eState4_0;
            }
            else
            {
                field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
                field_18E_ai_state = Mud_AI_State::AI_Wired_4_477B40;
                field_178_sub_state2 = 7;
                return BrainStates4::eState4_14;
            }
        }
    }
    break;

    case BrainStates1::eState_SadStandUp_4:
        if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
        {
            return field_190_sub_state;
        }
        field_16A_flags.Clear(Flags::eBit6_StandingForSad);
        field_108_next_motion = Mud_Motion::StandToCrouch_17_474120;
        return BrainStates1::eState_StandToCrouch_0;

    case BrainStates1::eState_Duck_5:
        if (field_106_current_motion == Mud_Motion::StandIdle_0_4724E0)
        {
            field_108_next_motion = Mud_Motion::StandToCrouch_17_474120;
            return field_190_sub_state;
        }

        if (field_106_current_motion == Mud_Motion::Chisel_11_4732D0)
        {
            field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
            return field_190_sub_state;
        }

        if (field_106_current_motion != Mud_Motion::CrouchIdle_15_474040)
        {
            return field_190_sub_state;
        }

        field_108_next_motion = Mud_Motion::Duck_53_474A40;
        field_194_timer = sGnFrame_5C1B84 + 60;
        return BrainStates1::eState_OutOfDuck_6;

    case BrainStates1::eState_OutOfDuck_6:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
        return BrainStates1::eState_DuckToChisle_7;

    case BrainStates1::eState_DuckToChisle_7:
        if (field_106_current_motion == Mud_Motion::Chisel_11_4732D0)
        {
            return BrainStates1::eState_StandToCrouch_0;
        }
        if (field_106_current_motion != Mud_Motion::CrouchIdle_15_474040)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
        return field_190_sub_state;

    case BrainStates1::eState_DuckKnockBack_8:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::DuckKnockBack_55_474250;
        field_194_timer = sGnFrame_5C1B84 + 60;
        return BrainStates1::eState_OutOfDuck_6;

    default:
        return field_190_sub_state;
    }
}

__int16 Mudokon::AI_Scrub_2_47D270()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::AI_State_3_47E0D0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::AI_Wired_4_Real_477B40()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::AI_Wired_4_477B40()
{
    if (CheckForPortal_4775E0())
    {
        field_18E_ai_state = Mud_AI_State::AI_HelloAlerted_6_47A560;
        field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
        return 0;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventMudokonAbuse,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eUnknown_9;
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
        field_17E_delayed_speak = MudAction::eUnknown_10;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventMudokonLaugh,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eUnknown_12;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventShooting | kEventSpeaking,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eUnknown_13;
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

    if (!field_16A_flags.Get(Flags::eBit8_DoAngry))
    {
        if (field_128_angry_timer > 0)
        {
            if (field_128_angry_timer <= static_cast<int>(sGnFrame_5C1B84))
            {
                field_16A_flags.Set(Flags::eBit8_DoAngry);

                if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos - FP_FromInteger(5)),
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos - FP_FromInteger(5)),
                    TlvTypes::Water_77))
                {
                    field_180_emo_tbl = Mud_Emotion::eAngry_1;
                    field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
                    field_190_sub_state = BrainStates4::eState4_14;
                    field_178_sub_state2 = 7;
                }
            }
        }
    }

    switch (field_190_sub_state)
    {
    case BrainStates4::eState4_0:
        return BrainState4_State_0();

    case BrainStates4::eState4_1:
        return BrainState4_State_1();

    case BrainStates4::eState4_2:
        return BrainState4_State_2();

    case BrainStates4::eState4_3:
        return BrainState4_State_3();

    case BrainStates4::eState4_4:
        return BrainState4_State_4_FollowingAbe();

    case BrainStates4::eState4_5:
        return BrainState4_State_5();

    case BrainStates4::eState4_6_RunAfterAbe:
        return BrainState4_State_6_RunAfterAbe();

    case BrainStates4::eState4_7:
        return BrainState4_State_7_StandingForAbeCommand();
        //return AI_Wired_4_Real_477B40();

    case BrainStates4::eState4_8:
        return BrainState4_State_8();

    case BrainStates4::eState4_9:
        return BrainState4_State_9();

    case BrainStates4::eState4_10:
        return BrainState4_State_10();

    case BrainStates4::eState4_11:
        return BrainState4_State_11();

    case BrainStates4::eState4_12:
        return BrainState4_State_12();

    case BrainStates4::eState4_13:
        return BrainState4_State_13();

    case BrainStates4::eState4_14:
        return BrainState4_State_14();

    case BrainStates4::eState4_15:
        return BrainState4_State_15();

    case BrainStates4::eState4_16:
        return BrainState4_State_16();

    case BrainStates4::eState4_17:
        return BrainState4_State_17();

    case BrainStates4::eState4_18:
        return BrainState4_State_18();

    case BrainStates4::eState4_19:
        return BrainState4_State_19();

    case BrainStates4::eState4_20:
        return BrainState4_State_20();

    case BrainStates4::eState4_21:
        return BrainState4_State_21();

    case BrainStates4::eState4_22:
        return BrainState4_State_22();

    default:
        return field_190_sub_state;
    }
}

__int16 Mudokon::BrainState4_State_0()
{
    field_108_next_motion = -1;
    ToStand_4724A0();
    field_178_sub_state2 = -1;
    return BrainStates4::eState4_1;
}

__int16 Mudokon::BrainState4_State_1()
{
    if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
    {
        return field_190_sub_state;
    }

    if (!vIsFacingMe_4254A0(sActiveHero_5C1B68))
    {
        field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
        return field_190_sub_state;
    }

    switch (field_180_emo_tbl)
    {
    case Mud_Emotion::eNormal_0:
        Sound_475EC0(MudSounds::e3);
        break;

    case Mud_Emotion::eAngry_1:
    case Mud_Emotion::eUnknown_2:
        if (field_16A_flags.Get(Flags::eBit6_StandingForSad))
        {
            Sound_475EC0(MudSounds::e5);
        }
        else
        {
            Sound_475EC0(MudSounds::e18);
        }
        break;

    case Mud_Emotion::eSad_3:
    case Mud_Emotion::eSick_7:
        if (field_16A_flags.Get(Flags::eBit6_StandingForSad))
        {
            Sound_475EC0(MudSounds::e22);
            break;
        }
        Sound_475EC0(MudSounds::e20);
        break;

    case Mud_Emotion::eUnknown_4:
        Sound_475EC0(MudSounds::e22);
        break;

    case Mud_Emotion::eHappy_5:
    case Mud_Emotion::eWired_6:
        Sound_475EC0(MudSounds::e10);
        break;

    default:
        break;
    }

    field_108_next_motion = Mud_Motion::Speak_Generic_4_472FA0;
    if (!field_16A_flags.Get(Flags::eBit6_StandingForSad) || field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        return BrainStates4::eState4_7;
    }

    field_16A_flags.Clear(Flags::eBit6_StandingForSad);
    field_16A_flags.Clear(Flags::eBit12);

    if (field_10_resources_array.ItemAt(2))
    {
        field_184_next_motion2 = Mud_Motion::Chisel_11_4732D0;
    }
    else
    {
        field_184_next_motion2 = Mud_Motion::CrouchIdle_15_474040;
    }
    field_178_sub_state2 = 22;
    field_16A_flags.Clear(Flags::eBit5);
    return BrainStates4::eState4_10;
}

__int16 Mudokon::BrainState4_State_2()
{
    if (!vIsFacingMe_4254A0(sActiveHero_5C1B68))
    {
        field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
        return field_190_sub_state;
    }

    if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
    {
        if (field_106_current_motion == Mud_Motion::Walking_1_4728B0)
        {
            if (field_16A_flags.Get(Flags::eBit4_blind))
            {
                return BrainStates4::eState4_5;
            }

            if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68))
            {
                return BrainStates4::eState4_5;
            }
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            return BrainStates4::eState4_4;
        }
        else
        {
            if (field_106_current_motion != Mud_Motion::StandToKnockBack_46_4742A0)
            {
                return field_190_sub_state;
            }
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            BrainStates4::eState4_4;
        }
    }
    else
    {
        field_108_next_motion = Mud_Motion::Walking_1_4728B0;
    }
    return field_190_sub_state;
}

__int16 Mudokon::BrainState4_State_3()
{
    if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
    {
        if (field_106_current_motion != Mud_Motion::Walking_1_4728B0)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;

        if (field_16A_flags.Get(Flags::eBit5))
        {
            LOG_WARNING("bit5 to state 4 wrong?");
            return BrainStates4::eState4_4;
        }
        else
        {
            LOG_WARNING("not bit5 to state 7 wrong?");
            return BrainStates4::eState4_7;
        }
    }
    else
    {
        if (vIsFacingMe_4254A0(sActiveHero_5C1B68))
        {
            field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::Walking_1_4728B0;
        return field_190_sub_state;
    }
}

__int16 Mudokon::BrainState4_State_4_FollowingAbe()
{
    if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && field_180_emo_tbl != Mud_Emotion::eHappy_5 && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        field_16A_flags.Clear(Flags::eBit5);
        return BrainStates4::eState4_7;
    }
    else
    {
        if (field_106_current_motion == Mud_Motion::Running_21_473720)
        {
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
        }
        if (field_BC_ypos > pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(260) && field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
            if (field_10_resources_array.ItemAt(2))
            {
                field_16A_flags.Clear(Flags::eBit5);
                field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
                return BrainStates4::eState4_22;
            }
            else
            {
                field_16A_flags.Clear(Flags::eBit5);
                field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
                return BrainStates4::eState4_22;
            }
        }

        FP v42 = {};
        if (sActiveHero_5C1B68->field_B8_xpos >= field_B8_xpos)
        {
            v42 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            v42 = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        const int v44 = Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), v42);

        if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
        {
            field_17C = 0;
            field_124 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 60);
            return field_190_sub_state;
        }
        else
        {
            field_16A_flags.Clear(Flags::eBit7);
            if (++field_17C <= 540 || field_180_emo_tbl == Mud_Emotion::eWired_6)
            {
                if (NeedToTurnAround())
                {
                    field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
                    return field_190_sub_state;
                }

                const FP v48 = ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2);
                if (vIsObjNearby_4253B0(v48, sActiveHero_5C1B68) && !(field_16A_flags.Get(Flags::eBit4_blind)) || v44 || FindWheel_4777B0(field_B8_xpos, field_BC_ypos))
                {
                    const GameSpeakEvents lastSpeak_1 = LastGameSpeak_476FF0();
                    if (field_17E_delayed_speak != MudAction::eUnknown_17)
                    {
                        __int16 v18 = sub_477AF0(field_17E_delayed_speak);
                        if (field_188_pTblEntry->field_0_sound == MudSounds::e14)
                        {
                            field_194_timer = sGnFrame_5C1B84 + 20;
                        }
                        else
                        {
                            field_194_timer = (unsigned __int16)StableDelay_477570() + sGnFrame_5C1B84 + 20;
                        }
                        field_178_sub_state2 = field_190_sub_state;
                        field_17E_delayed_speak = MudAction::eUnknown_17;
                        if (!v18)
                        {
                            return field_190_sub_state;
                        }
                        return v18;
                    }

                    if (lastSpeak_1 == GameSpeakEvents::eNone)
                    {
                        if (field_180_emo_tbl != Mud_Emotion::eWired_6 || field_124 > (signed int)sGnFrame_5C1B84)
                        {
                            return field_190_sub_state;
                        }
                        field_194_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
                        field_178_sub_state2 = field_190_sub_state;
                        return BrainStates4::eState4_21;
                    }
                    else
                    {
                        field_178_sub_state2 = field_190_sub_state;
                        field_194_timer = (unsigned __int16)StableDelay_477570() + sGnFrame_5C1B84 + 20;
                        switch (lastSpeak_1)
                        {
                        case GameSpeakEvents::eFart_3:
                        {
                            __int16 v18 = sub_477AF0(MudAction::eFart_6);
                            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68))
                            {
                                if (vOnSameYLevel_425520(sActiveHero_5C1B68))
                                {
                                    field_178_sub_state2 = 3;
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
                            __int16 result = sub_477AF0(MudAction::eHelloOrAllYa_0);
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
                                return BrainStates4::eState4_19;
                            }
                            __int16 result = sub_477AF0(MudAction::eFollowMe_1);
                            field_178_sub_state2 = 2;
                            if (result)
                            {
                                return result;
                            }
                            return field_190_sub_state;
                        }

                        case GameSpeakEvents::eAnger_11:
                        case GameSpeakEvents::eSorry_24:
                            field_17C = 0;
                            return field_190_sub_state;

                        case GameSpeakEvents::eWait_12:
                        {
                            if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                            {
                                return BrainStates4::eState4_19;
                            }
                            field_16A_flags.Clear(Flags::eBit5);
                            __int16 result = sub_477AF0(MudAction::eWait_2);
                            field_178_sub_state2 = 7;
                            if (result)
                            {
                                return result;
                            }
                            return field_190_sub_state;
                        }

                        case GameSpeakEvents::eWork_21:
                            if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                            {
                                return BrainStates4::eState4_19;
                            }
                            field_16A_flags.Clear(Flags::eBit5);
                            field_182 = -1;
                            if (FindWheel_4777B0(field_B8_xpos, field_BC_ypos))
                            {
                                Mudokon* v35 = static_cast<Mudokon*>(FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
                                if (!v35)
                                {
                                    return AIStartWheelTurning();
                                }

                                while (!(v35->field_16A_flags.Get(Flags::eBit7)) && v35->field_106_current_motion != Mud_Motion::TurnWheelLoop_58_474CC0)
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
                                        field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
                                        return BrainStates4::eState4_22;
                                    }
                                    else
                                    {
                                        field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
                                        return BrainStates4::eState4_22;
                                    }
                                }
                                return AIStartWheelTurning();
                            }

                            if (FindObjectOfType_425180(Types::eLever_139, ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos, field_BC_ypos - FP_FromInteger(5)))
                            {
                                return BrainStates4::eState4_11;
                            }
                            if (FindObjectOfType_425180(Types::eLever_139, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
                            {
                                return BrainStates4::eState4_11;
                            }

                            if (field_10_resources_array.ItemAt(2))
                            {
                                field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
                                return BrainStates4::eState4_22;
                            }
                            else
                            {
                                field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
                                return BrainStates4::eState4_22;
                            }
                            break;

                        case GameSpeakEvents::eStopIt_22:
                        {
                            __int16 v18 = sub_477AF0(MudAction::eUnknown_4);
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
                        field_184_next_motion2 = Mud_Motion::Running_21_473720;
                        field_178_sub_state2 = 6;
                        return BrainStates4::eState4_10;
                    }
                    else if (field_16A_flags.Get(Flags::eBit4_blind) || !IsMotionUnknown_4730F0())
                    {
                        field_184_next_motion2 = Mud_Motion::Walking_1_4728B0;
                        field_178_sub_state2 = 5;
                        return BrainStates4::eState4_10;
                    }
                    else
                    {
                        field_184_next_motion2 = Mud_Motion::Sneaking_27_473C40;
                        field_178_sub_state2 = 5;
                        return BrainStates4::eState4_10;
                    }
                }
            }
            else
            {
                field_16A_flags.Clear(Flags::eBit5);
                if (!field_10_resources_array.ItemAt(2))
                {
                    field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
                    return BrainStates4::eState4_22;
                }
                else
                {
                    field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
                    return BrainStates4::eState4_22;
                }
            }
        }
    }
}

__int16 Mudokon::BrainState4_State_5()
{
    if (field_106_current_motion == Mud_Motion::StandIdle_0_4724E0 && field_108_next_motion != Mud_Motion::Walking_1_4728B0)
    {
        if (!field_16A_flags.Get(Flags::eBit4_blind))
        {
            return BrainStates4::eState4_4;
        }
        field_108_next_motion = Mud_Motion::Walking_1_4728B0;
    }

    if (field_106_current_motion == Mud_Motion::StandToKnockBack_46_4742A0)
    {
        Sound_475EC0(MudSounds::e16);
        __int16 result = sub_477AF0(MudAction::eUnknown_11);
        field_17E_delayed_speak = MudAction::eUnknown_17;
        field_194_timer = sGnFrame_5C1B84 + 20;
        if (field_16A_flags.Get(Flags::eBit4_blind))
        {
            field_178_sub_state2 = 7;
            field_16A_flags.Clear(Flags::eBit5);
        }
        else
        {
            field_178_sub_state2 = 4;
        }
        if (result)
        {
            return result;
        }
    }

    if (field_106_current_motion != Mud_Motion::Walking_1_4728B0 && field_106_current_motion != Mud_Motion::Sneaking_27_473C40)
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

    const int v67 = Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), v65);
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
                field_16A_flags.Clear(Flags::eBit5);
                field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
                return 7;
            }
        }
    }
    if (v67 && !field_16A_flags.Get(Flags::eBit4_blind))
    {
        field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
        return 4;
    }

    if (sub_477880())
    {
        field_16A_flags.Set(Flags::eBit7);
        field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
        return BrainStates4::eState4_4;
    }

    if (!vIsFacingMe_4254A0(sActiveHero_5C1B68) && !field_16A_flags.Get(Flags::eBit4_blind))
    {
        field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
        return BrainStates4::eState4_4;
    }

    if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2), sActiveHero_5C1B68) && !field_16A_flags.Get(Flags::eBit4_blind))
    {
        field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
        return BrainStates4::eState4_4;
    }

    if (LastGameSpeak_476FF0() == GameSpeakEvents::eWait_12)
    {
        field_16A_flags.Clear(Flags::eBit5);
        field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
        return BrainStates4::eState4_9;
    }

    bool bWalkOrSneak = false;
    if (sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_33_RunLoop_4508E0 &&
        sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_23_RollLoop_453A90 &&
        field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        bWalkOrSneak = true;
    }
    else if (field_106_current_motion == Mud_Motion::Sneaking_27_473C40 || field_16A_flags.Get(Flags::eBit4_blind))
    {
        field_108_next_motion = Mud_Motion::Walking_1_4728B0;
        bWalkOrSneak = true;
    }

    if (bWalkOrSneak)
    {
        if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_40_SneakLoop_450550
            && field_106_current_motion == Mud_Motion::Walking_1_4728B0
            && !field_16A_flags.Get(Flags::eBit4_blind))
        {
            field_108_next_motion = Mud_Motion::Sneaking_27_473C40;
        }
        if (sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_1_WalkLoop_44FBA0 || field_106_current_motion != Mud_Motion::Sneaking_27_473C40)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::Walking_1_4728B0;
        return field_190_sub_state;
    }
    else
    {
        field_108_next_motion = Mud_Motion::Running_21_473720;
        return BrainStates4::eState4_6_RunAfterAbe;
    }
}

__int16 Mudokon::BrainState4_State_6_RunAfterAbe()
{
    if (field_106_current_motion == Mud_Motion::StandIdle_0_4724E0)
    {
        if (!field_16A_flags.Get(Flags::eBit4_blind))
        {
            return BrainStates4::eState4_4;
        }
        field_108_next_motion = Mud_Motion::Running_21_473720;
    }

    if (field_106_current_motion == Mud_Motion::StandToKnockBack_46_4742A0)
    {
        Sound_475EC0(MudSounds::e16);
        __int16 result = sub_477AF0(MudAction::eUnknown_11);
        field_17E_delayed_speak = MudAction::eUnknown_17;
        field_194_timer = sGnFrame_5C1B84 + 20;
        if (field_16A_flags.Get(Flags::eBit4_blind))
        {
            field_178_sub_state2 = 7;
            field_16A_flags.Clear(Flags::eBit5);
        }
        else
        {
            field_178_sub_state2 = 4;
        }
        if (result)
        {
            return result;
        }
    }
    if (field_106_current_motion != Mud_Motion::Running_21_473720)
    {
        return field_190_sub_state;
    }

    FP v75;
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        v75 = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }
    else
    {
        v75 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), v75 * FP_FromInteger(3)) && !field_16A_flags.Get(Flags::eBit4_blind))
    {
        field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
        if (!(field_180_emo_tbl == Mud_Emotion::eWired_6))
        {
            return BrainStates4::eState4_4;
        }
        field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
        if (Math_NextRandom() < 20)
        {
            return BrainStates4::eState4_4;
        }
        field_178_sub_state2 = 4;
        return BrainStates4::eState4_21;
    }

    bool gotoTurn = false;
    if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_40_SneakLoop_450550)
    {
        if (field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
            field_108_next_motion = Mud_Motion::Walking_1_4728B0;
            return BrainStates4::eState4_5;
        }
        if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_25_RunSlideStop_451330 ||
            sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_0_Idle_44EEB0 ||
            sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_71_Knockback_455090 ||
            sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_17_CrouchIdle_456BC0)
        {
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
            if (Math_NextRandom() < 40)
            {
                return BrainStates4::eState4_4;
            }
            field_178_sub_state2 = 4;
            return BrainStates4::eState4_21;
        }
        gotoTurn = true;
    }

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_25_RunSlideStop_451330 ||
            sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_0_Idle_44EEB0 ||
            sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_71_Knockback_455090 ||
            sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_17_CrouchIdle_456BC0)
        {
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
            if (Math_NextRandom() < 40)
            {
                return BrainStates4::eState4_4;
            }
            field_178_sub_state2 = 4;
            return BrainStates4::eState4_21;
        }
        gotoTurn = true;
    }

    if (!gotoTurn)
    {
        if (sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_33_RunLoop_4508E0)
        {
            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(4), sActiveHero_5C1B68))
            {
                field_108_next_motion = Mud_Motion::Walking_1_4728B0;
                return BrainStates4::eState4_5;
            }
        }

        if (LastGameSpeak_476FF0() == GameSpeakEvents::eWait_12)
        {
            field_16A_flags.Clear(Flags::eBit5);
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            return BrainStates4::eState4_9;
        }
    }

    // TODO: Ordering
    if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68) ||
        !vIsObj_GettingNear_425420(sActiveHero_5C1B68) && vIsFacingMe_4254A0(sActiveHero_5C1B68) ||
        field_16A_flags.Get(Flags::eBit4_blind))
    {
        return field_190_sub_state;
    }
    field_108_next_motion = Mud_Motion::RunSlideTurn_25_473AA0;
    return BrainStates4::eState4_8;
}

__int16 Mudokon::BrainState4_State_7_StandingForAbeCommand()
{
    if (field_BC_ypos > pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(260) && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (field_10_resources_array.ItemAt(2))
        {
            field_16A_flags.Clear(Flags::eBit5);
            field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
            return BrainStates4::eState4_22;
        }
        else
        {
            field_16A_flags.Clear(Flags::eBit5);
            field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
            return BrainStates4::eState4_22;
        }
    }

    if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
    {
        field_17C = 0;
        field_124 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 60);
        return field_190_sub_state;
    }

    if (field_180_emo_tbl == Mud_Emotion::eUnknown_4)
    {
        field_16A_flags.Clear(Flags::eBit5);
        return BrainStates4::eState4_16;
    }

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        field_16A_flags.Set(Flags::eBit5);
        return BrainStates4::eState4_4;
    }

    if ((++field_17C > 540 || Is_In_Current_Camera_424A70() != CameraPos::eCamCurrent_0) && field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (field_10_resources_array.ItemAt(2))
        {
            field_16A_flags.Clear(Flags::eBit5);
            field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
            return BrainStates4::eState4_22;
        }
        else
        {
            field_16A_flags.Clear(Flags::eBit5);
            field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
            return BrainStates4::eState4_22;
        }
    }

    if (NeedToTurnAround())
    {
        field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
        return field_190_sub_state;
    }
    
    if (field_17E_delayed_speak != MudAction::eUnknown_17)
    {
        __int16 v18 = sub_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::e14)
        {
            field_194_timer = sGnFrame_5C1B84 + 20;
            field_178_sub_state2 = field_190_sub_state;
        }
        else
        {
            field_178_sub_state2 = field_190_sub_state;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
        }
        field_17E_delayed_speak = MudAction::eUnknown_17;
        if (!v18)
        {
            return field_190_sub_state;
        }
        return v18;
    }
    GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
    if (lastSpeak == GameSpeakEvents::eNone
        && ((lastSpeak = (GameSpeakEvents)field_182, lastSpeak == -1)
            || field_180_emo_tbl != Mud_Emotion::eNormal_0 && field_180_emo_tbl != Mud_Emotion::eHappy_5 && field_180_emo_tbl != Mud_Emotion::eWired_6))
    {
        if (field_180_emo_tbl != Mud_Emotion::eWired_6 || field_124 > (signed int)sGnFrame_5C1B84)
        {
            return field_190_sub_state;
        }
        field_194_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
        field_178_sub_state2 = field_190_sub_state;
        return BrainStates4::eState4_21;
    }
    else
    {
        field_178_sub_state2 = field_190_sub_state;
        field_194_timer = (unsigned __int16)StableDelay_477570() + sGnFrame_5C1B84 + 20;
        switch (lastSpeak)
        {
        case GameSpeakEvents::eFart_3:
        {
            field_182 = -1;
            const __int16 v18 = sub_477AF0(MudAction::eFart_6);
            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68))
            {
                if (vOnSameYLevel_425520(sActiveHero_5C1B68))
                {
                    field_178_sub_state2 = 3;
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
            field_182 = -1;
            __int16 result = sub_477AF0(MudAction::eHelloOrAllYa_0);
            if (result)
            {
                return result;
            }
            return field_190_sub_state;
        }

        case GameSpeakEvents::eFollowMe_10:
        {
            const __int16 v26 = sub_477AF0(MudAction::eFollowMe_1);
            if (field_188_pTblEntry->field_0_sound == MudSounds::e12 || field_188_pTblEntry->field_0_sound == MudSounds::e10)
            {
                field_16A_flags.Set(Flags::eBit5);
                field_182 = -1;
                // TODO: Wrong ??
                if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2), sActiveHero_5C1B68) ||
                    vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68))
                {
                    field_178_sub_state2 = 4;
                }
                else
                {
                    field_178_sub_state2 = 2;
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
            field_17C = 0;
            return field_190_sub_state;

        case GameSpeakEvents::eWait_12:
            field_16A_flags.Clear(Flags::eBit5);
            field_182 = -1;
            field_184_next_motion2 = Mud_Motion::StandToDunno_43_472790;
            return BrainStates4::eState4_10;

        case GameSpeakEvents::eWork_21:
            if (field_180_emo_tbl == Mud_Emotion::eWired_6)
            {
                return BrainStates4::eState4_19;
            }
            field_16A_flags.Clear(Flags::eBit5);
            field_182 = -1;

            if (FindWheel_4777B0(field_B8_xpos, field_BC_ypos))
            {
                Mudokon* v35 = static_cast<Mudokon*>(FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
                if (!v35)
                {
                    return AIStartWheelTurning();
                }

                while (!(v35->field_16A_flags.Get(Flags::eBit7)) && v35->field_106_current_motion != Mud_Motion::TurnWheelLoop_58_474CC0)
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
                        field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
                        return BrainStates4::eState4_22;
                    }
                    else
                    {
                        field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
                        return BrainStates4::eState4_22;
                    }
                }
                return AIStartWheelTurning();
            }

            if (FindObjectOfType_425180(Types::eLever_139, ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos, field_BC_ypos - FP_FromInteger(5)))
            {
                return BrainStates4::eState4_11;
            }
            if (FindObjectOfType_425180(Types::eLever_139, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
            {
                return BrainStates4::eState4_11;
            }

            break;

        case GameSpeakEvents::eStopIt_22:
        {
            __int16 v18 = sub_477AF0(MudAction::eUnknown_4);
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
            field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
            return BrainStates4::eState4_22;
        }
        else
        {
            field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
            return BrainStates4::eState4_22;
        }
    }
}

__int16 Mudokon::BrainState4_State_8()
{
    if (field_106_current_motion == Mud_Motion::Running_21_473720)
    {
        return BrainStates4::eState4_6_RunAfterAbe;
    }
    if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
    {
        return field_190_sub_state;
    }
    return BrainStates4::eState4_4;
}

__int16 Mudokon::BrainState4_State_9()
{
    if (field_106_current_motion)
    {
        return field_190_sub_state;
    }
    field_178_sub_state2 = 7;
    field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
    return BrainStates4::eState4_20;
}

__int16 Mudokon::BrainState4_State_10()
{
    if (!vIsFacingMe_4254A0(sActiveHero_5C1B68))
    {
        field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
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

__int16 Mudokon::BrainState4_State_11()
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
        field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
        return field_190_sub_state;
    }

    if (field_106_current_motion == Mud_Motion::PullLever_10_473020)
    {
        field_108_next_motion = -1;
        auto pSwitch = static_cast<Switch*>(pLever);
        pSwitch->Vsub_4D6050(field_B8_xpos < pSwitch->field_B8_xpos);
        field_16A_flags.Clear(Flags::eBit5);

        if (field_10_resources_array.ItemAt(2))
        {
            field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
        }
        else
        {
            field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
        }
        return BrainStates4::eState4_22;
    }
    else
    {
        field_108_next_motion = Mud_Motion::PullLever_10_473020;
        return field_190_sub_state;
    }
}

__int16 Mudokon::BrainState4_State_12()
{
    if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eUnknown_9)
    {
        __int16 v18 = sub_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::e14)
        {
            field_194_timer = sGnFrame_5C1B84 + 20;
            field_178_sub_state2 = 7;
            field_17E_delayed_speak = MudAction::eUnknown_17;
            if (!v18)
            {
                return field_190_sub_state;
            }
            return v18;
        }
        field_194_timer = (unsigned __int16)(StableDelay_477570() + sGnFrame_5C1B84 + 20);
        field_178_sub_state2 = 7;
        field_17E_delayed_speak = MudAction::eUnknown_17;
        if (!v18)
        {
            return field_190_sub_state;
        }
        return v18;
    }

    if (field_188_pTblEntry->field_2_next_motion == Mud_Motion::Slap_38_474AA0)
    {
        if (LastGameSpeak_476FF0() == GameSpeakEvents::eStopIt_22)
        {
            if (field_106_current_motion == Mud_Motion::Slap_38_474AA0 || field_108_next_motion == Mud_Motion::Slap_38_474AA0)
            {
                ToStand_4724A0();
                field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
                field_108_next_motion = -1;
            }
            __int16 result = sub_477AF0(MudAction::eUnknown_4);
            field_178_sub_state2 = 7;
            field_17E_delayed_speak = MudAction::eUnknown_17;
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

    if (field_188_pTblEntry->field_0_sound != MudSounds::e23 || field_178_sub_state2 == 3)
    {
        if (field_188_pTblEntry->field_0_sound != MudSounds::e10 || (Math_NextRandom() >= 0x50u))
        {
            if (field_188_pTblEntry->field_0_sound != MudSounds::eNone)
            {
                Sound_475EC0(field_188_pTblEntry->field_0_sound);
            }
        }
        else
        {
            Sound_475EC0(MudSounds::e8);
        }
    }
    else
    {
        if (Math_NextRandom() >= 80u)
        {
            Sound_475EC0(MudSounds::e10);
        }
        else
        {
            Sound_475EC0(MudSounds::e8);
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
        return BrainStates4::eState4_0;
    }

    if (field_108_next_motion != Mud_Motion::Slap_38_474AA0)
    {
        return field_178_sub_state2;
    }

    BaseGameObject* pMudInSameGridBlock = nullptr;
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (FindObjectOfType_425180(Types::eType_Abe_69, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
        {
            return BrainStates4::eState4_18;
        }
        pMudInSameGridBlock = FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5));
    }
    else
    {
        if (FindObjectOfType_425180(Types::eType_Abe_69, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
        {
            return BrainStates4::eState4_18;
        }
        pMudInSameGridBlock = FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5));
    }

    if (!pMudInSameGridBlock)
    {
        return field_178_sub_state2;
    }

    return BrainStates4::eState4_18;
}

__int16 Mudokon::BrainState4_State_13()
{
    StandingKnockBack_473190();
    
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);
    field_108_next_motion = -1;

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        return BrainStates4::eState4_21;
    }

    if (field_188_pTblEntry->field_2_next_motion == Mud_Motion::Slap_38_474AA0)
    {
        return BrainStates4::eState4_14;
    }
    else
    {
        LOG_WARNING("State 12 might be wrong ??");
        return BrainStates4::eState4_12;
    }
}

__int16 Mudokon::BrainState4_State_14()
{
    if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eUnknown_9)
    {
        __int16 v18 = sub_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::e14)
        {
            field_194_timer = sGnFrame_5C1B84 + 20;
            field_178_sub_state2 = 7;
            field_17E_delayed_speak = MudAction::eUnknown_17;
            if (!v18)
            {
                return field_190_sub_state;
            }
            return v18;
        }
        field_194_timer = (unsigned __int16)(StableDelay_477570() + sGnFrame_5C1B84 + 20);
        field_178_sub_state2 = 7;
        field_17E_delayed_speak = MudAction::eUnknown_17;
        if (!v18)
        {
            return field_190_sub_state;
        }
        return v18;
    }
    if (LastGameSpeak_476FF0() == GameSpeakEvents::eStopIt_22)
    {
        if (field_106_current_motion == Mud_Motion::Slap_38_474AA0 || field_108_next_motion == Mud_Motion::Slap_38_474AA0)
        {
            ToStand_4724A0();
            field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
            field_108_next_motion = -1;
        }

        __int16 result = sub_477AF0(MudAction::eUnknown_4);
        field_178_sub_state2 = 7;
        field_17E_delayed_speak = MudAction::eUnknown_17;
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
        if (!FindObjectOfType_425180(Types::eType_Abe_69, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
        {
            if (FindObjectOfType_425180(Types::eType_Abe_69, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5))
                || !FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5))
                && FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
            {
                field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
                field_194_timer = sGnFrame_5C1B84 + 15;
                return BrainStates4::eState4_12;
            }
        }
    }
    else
    {
        if (!FindObjectOfType_425180(Types::eType_Abe_69, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
        {
            if (FindObjectOfType_425180(Types::eType_Abe_69, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
            {
                field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
                field_194_timer = sGnFrame_5C1B84 + 15;
                return BrainStates4::eState4_12;
            }

            if (!FindObjectOfType_425180(Types::eMudokon_110, ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos, field_BC_ypos - FP_FromInteger(5)))
            {
                if (FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
                {
                    field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
                    field_194_timer = sGnFrame_5C1B84 + 15;
                    return BrainStates4::eState4_12;
                }
            }
        }
    }
    return BrainStates4::eState4_12;
}

__int16 Mudokon::BrainState4_State_15()
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
    if (field_182 != 10)
    {
        return field_178_sub_state2;
    }

    field_16A_flags.Set(Flags::eBit5);
    field_182 = -1;
    return BrainStates4::eState4_4;
}

__int16 Mudokon::BrainState4_State_16()
{
    if (field_106_current_motion != Mud_Motion::SlapOwnHead_56_4727D0)
    {
        field_108_next_motion = Mud_Motion::SlapOwnHead_56_4727D0;
    }

    if (LastGameSpeak_476FF0() != GameSpeakEvents::eStopIt_22 && field_17E_delayed_speak != MudAction::eSorry_8)
    {
        return field_190_sub_state;
    }

    __int16 result = sub_477AF0(MudAction::eUnknown_4);
    field_178_sub_state2 = 7;
    field_17E_delayed_speak = MudAction::eUnknown_17;
    field_194_timer = sGnFrame_5C1B84 + 20;
    if (!result)
    {
        return field_190_sub_state;
    }
    return result;
}

__int16 Mudokon::BrainState4_State_17()
{
    if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eUnknown_9)
    {
        __int16 v18 = sub_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::e14)
        {
            field_194_timer = sGnFrame_5C1B84 + 20;
            field_178_sub_state2 = 7;
            field_17E_delayed_speak = MudAction::eUnknown_17;
            if (!v18)
            {
                return field_190_sub_state;
            }
            return v18;
        }
        field_194_timer = (unsigned __int16)(StableDelay_477570() + sGnFrame_5C1B84 + 20);
        field_178_sub_state2 = 7;
        field_17E_delayed_speak = MudAction::eUnknown_17;
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
        field_108_next_motion = Mud_Motion::Slap_38_474AA0;
        return BrainStates4::eState4_7;
    }

    if (field_106_current_motion == Mud_Motion::Slap_38_474AA0 || field_108_next_motion == Mud_Motion::Slap_38_474AA0)
    {
        ToStand_4724A0();
        field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
        field_108_next_motion = -1;
    }

    __int16 result = sub_477AF0(MudAction::eUnknown_4);
    field_178_sub_state2 = 7;
    field_17E_delayed_speak = MudAction::eUnknown_17;
    field_194_timer = sGnFrame_5C1B84 + 20;
    if (result)
    {
        return result;
    }

    if (field_106_current_motion)
    {
        return field_190_sub_state;
    }
    field_108_next_motion = Mud_Motion::Slap_38_474AA0;
    return BrainStates4::eState4_7;
}

__int16 Mudokon::BrainState4_State_18()
{
    if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eUnknown_9)
    {
        __int16 v18 = sub_477AF0(field_17E_delayed_speak);
        if (field_188_pTblEntry->field_0_sound == MudSounds::e14)
        {
            field_194_timer = (short)(sGnFrame_5C1B84 + 20);
        }
        else
        {
            field_194_timer = (unsigned __int16)Mudokon::StableDelay_477570() + sGnFrame_5C1B84 + 20;
        }
        field_178_sub_state2 = 7;
        field_17E_delayed_speak = MudAction::eUnknown_17;
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
            if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
            {
                return field_190_sub_state;
            }
            field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
            return  BrainStates4::eState4_17;
        }

        if (field_106_current_motion == Mud_Motion::Slap_38_474AA0 || field_108_next_motion == Mud_Motion::Slap_38_474AA0)
        {
            ToStand_4724A0();
            field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
            field_108_next_motion = -1;
        }

        __int16  result = sub_477AF0(MudAction::eUnknown_4);
        field_178_sub_state2 = 7;
        field_17E_delayed_speak = MudAction::eUnknown_17;
        field_194_timer = sGnFrame_5C1B84 + 20;

        if (!result)
        {
            if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
            {
                return field_190_sub_state;
            }
            field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
            result = BrainStates4::eState4_17;
        }
        return result;
    }
}

__int16 Mudokon::BrainState4_State_19()
{
    if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
    {
        return field_190_sub_state;
    }

    if (field_180_emo_tbl == Mud_Emotion::eAngry_1 || field_180_emo_tbl == Mud_Emotion::eUnknown_2)
    {
        Sound_475EC0(MudSounds::e21);
    }
    else if (field_180_emo_tbl == Mud_Emotion::eSad_3 || field_180_emo_tbl == Mud_Emotion::eUnknown_4)
    {
        Sound_475EC0(MudSounds::e22);
    }
    else
    {
        Sound_475EC0(MudSounds::e13);
    }

    field_108_next_motion = Mud_Motion::Speak_Generic_6_472FA0;
    return BrainStates4::eState4_4;
}

__int16 Mudokon::BrainState4_State_20()
{
    if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
    {
        return field_190_sub_state;
    }
    Sound_475EC0(MudSounds::e12);
    field_108_next_motion = Mud_Motion::Speak_Generic_3_472FA0;
    return field_178_sub_state2;
}

__int16 Mudokon::BrainState4_State_21()
{
    if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
    {
        return BrainStates4::eState4_21;
    }

    if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
    {
        return field_190_sub_state;
    }

    field_16A_flags.Clear(Flags::eBit12);

    if (Math_NextRandom() >= 90u)
    {
        Sound_475EC0(MudSounds::e10);
    }
    else
    {
        Sound_475EC0(MudSounds::e8);
    }

    field_108_next_motion = Mud_Motion::Speak_Generic_6_472FA0;
    return field_178_sub_state2;
}

__int16 Mudokon::BrainState4_State_22()
{
    if (field_10_resources_array.ItemAt(2))
    {
        field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
    }
    else
    {
        field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
    }

    if (field_106_current_motion != Mud_Motion::CrouchIdle_15_474040 && field_106_current_motion != Mud_Motion::Chisel_11_4732D0)
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

    field_182 = -1;
    return BrainStates4::eState4_0;
}

__int16 Mudokon::AI_ShrivelDeath_5_4714A0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::AI_HelloAlerted_6_47A560()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::AI_FallAndSmackDeath_7_471600()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::AI_AngryWorker_8_47E910()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::AI_Sick_9_47A910()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Mudokon::StandIdle_0_4724E0()
{
    ToFalling_472320();

    field_F8 = field_BC_ypos;

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
    case Mud_Motion::Walking_1_4728B0:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9);
        }
        else
        {
            field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9);
        }
        field_106_current_motion = Mud_Motion::StandToWalk_7_472AB0;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::Sneaking_27_473C40:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(10);
        }
        else
        {
            field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(10);
        }
        field_106_current_motion = Mud_Motion::StandToSneaking_32_473F80;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::Running_21_473720:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
        }
        else
        {
            field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
        }

        field_106_current_motion = Mud_Motion::WalkToRun_37_4749A0;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::Chanting_50_473040:
        SND_SEQ_PlaySeq_4CA960(11, 0, 1);
        field_106_current_motion = Mud_Motion::Chanting_50_473040;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::CrouchIdle_15_474040:
        field_106_current_motion = Mud_Motion::StandToCrouch_17_474120;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::Chisel_11_4732D0:
    case Mud_Motion::CrouchScrub_14_473560:
        field_106_current_motion = Mud_Motion::StandToCrouch_17_474120;
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

void Mudokon::Walking_1_4728B0()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        StandingKnockBack_473190();
    }
    else if (sObjectIds_5C1B70.Find_449CF0(field_110_id) && field_16A_flags.Get(Flags::eBit4_blind) && (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(1), field_C4_velx)))
    {
        StandingKnockBack_473190();
    }
    else
    {
        MoveOnLine_4720D0();
        if (field_106_current_motion == 1)
        {
            if (field_20_animation.field_92_current_frame == 2 || field_20_animation.field_92_current_frame == 11)
            {
                if (field_108_next_motion == Mud_Motion::StandIdle_0_4724E0)
                {
                    field_108_next_motion = -1;
                    if (field_20_animation.field_92_current_frame == 2)
                    {
                        field_106_current_motion = Mud_Motion::jWalkingToStand_8_472BD0;
                    }
                    else
                    {
                        field_106_current_motion = Mud_Motion::WalkingToStand_8_472B30;
                    }
                }
                else if (field_108_next_motion == Mud_Motion::TurnAroundStanding_2_472BF0)
                {
                    if (field_20_animation.field_92_current_frame != 2)
                    {
                        field_106_current_motion = Mud_Motion::jWalkingToStand_8_472BD0;
                    }
                    else
                    {
                        field_106_current_motion = Mud_Motion::WalkingToStand_8_472B30;
                    }
                }
            }
            else if (field_20_animation.field_92_current_frame == 5 || field_20_animation.field_92_current_frame == 14)
            {
                Abe_SFX_2_457A40(1, 0, 32767, this);
                MapFollowMe_408D10(TRUE);
                if (field_108_next_motion == Mud_Motion::Running_21_473720)
                {
                    field_108_next_motion = -1;
                    if (field_20_animation.field_92_current_frame == 5)
                    {
                        field_106_current_motion = Mud_Motion::StandingToRun2_20_4736D0;
                    }
                    else
                    {
                        field_106_current_motion = Mud_Motion::StandingToRun1_19_473600;
                    }
                }
                else if (field_108_next_motion == Mud_Motion::Sneaking_27_473C40)
                {
                    auto curFrame2 = field_20_animation.field_92_current_frame;
                    field_108_next_motion = -1;
                    if (curFrame2 == 5)
                    {
                        field_106_current_motion = Mud_Motion::WalkToSneak1_28_473D60;
                    }
                    else
                    {
                        field_106_current_motion = Mud_Motion::WalkToSneak2_30_473EE0;
                    }
                }
            }
        }
    }
}

void Mudokon::TurnAroundStanding_2_472BF0()
{
    ToFalling_472320();

    if (field_20_animation.field_92_current_frame == 0)
    {
        Abe_SFX_2_457A40(9, 0, 32767, this);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        ToStand_4724A0();
    }
}

void Mudokon::Speak_Generic_472FA0()
{
    ToFalling_472320();
    SetPal_4772D0(field_180_emo_tbl);

    if (field_16A_flags.Get(Flags::eBit12))
    {
        Event_Broadcast_422BC0(kEventNoise, this);
        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_16A_flags.Set(Flags::eBit12);
        SetPal_4772D0(Mud_Emotion::eNormal_0);
        ToStand_4724A0();
    }
}

void Mudokon::StandToWalk_7_472AB0()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = Mud_Motion::Walking_1_4728B0;
        }
        MoveOnLine_4720D0();
    }
}

void Mudokon::WalkingToStand_8_472B30()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        ToStand_4724A0();
    }
    else
    {
        MoveOnLine_4720D0();

        if (!field_20_animation.field_92_current_frame)
        {
            Abe_SFX_2_457A40(1, 0, 32767, this);
            return;
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe_408D10(TRUE);
            ToStand_4724A0();
        }
    }
}

void Mudokon::jWalkingToStand_8_472BD0()
{
    WalkingToStand_8_472B30();
}

void Mudokon::PullLever_10_473020()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Chisel_11_4732D0()
{
    ToFalling_472320();

    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (sGnFrame_5C1B84 & 1)
            {
                SFX_Play_46FA90(91, 0, field_CC_sprite_scale);

                auto pSpark = alive_new<Spark>();
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
        case Mud_Motion::StandIdle_0_4724E0:
            field_106_current_motion = Mud_Motion::StopChisel_13_473530;
            return;

        case Mud_Motion::CrouchIdle_15_474040:
            field_106_current_motion = Mud_Motion::StopChisel_13_473530;
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

void Mudokon::StartChisel_12_473500()
{
    ToFalling_472320();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::Chisel_11_4732D0;
    }
}

void Mudokon::StopChisel_13_473530()
{
    ToFalling_472320();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::CrouchIdle_15_474040;
    }
}

void Mudokon::CrouchScrub_14_473560()
{
    NOT_IMPLEMENTED();
}

void Mudokon::CrouchIdle_15_474040()
{
    ToFalling_472320();

    if (field_108_next_motion == Mud_Motion::Duck_53_474A40)
    {
        field_106_current_motion = Mud_Motion::ToDuck_52_474A20;
        field_108_next_motion = -1;
    }
    else if (field_108_next_motion == Mud_Motion::StandIdle_0_4724E0)
    {
        field_106_current_motion = Mud_Motion::CrouchToStand_18_474150;
        field_108_next_motion = -1;
    }
    else if (field_108_next_motion == Mud_Motion::Chisel_11_4732D0)
    {
        field_106_current_motion = Mud_Motion::StartChisel_12_473500;
        field_108_next_motion = -1;
    }
    else if (field_108_next_motion != -1)
    {
        field_106_current_motion = field_108_next_motion;
        field_108_next_motion = -1;
    }
}

void Mudokon::CrouchTurn_16_4740E0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::StandToCrouch_17_474120()
{
    ToFalling_472320();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::CrouchIdle_15_474040;
    }
}

void Mudokon::CrouchToStand_18_474150()
{
    ToFalling_472320();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::StandingToRun1_19_473600()
{
    NOT_IMPLEMENTED();
}

void Mudokon::StandingToRun2_20_4736D0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Running_21_473720()
{
    NOT_IMPLEMENTED();
}

void Mudokon::RunToWalk1_22_4738E0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::RunToWalk2_23_4739B0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::RunSlideStop_24_473A00()
{
    NOT_IMPLEMENTED();
}

void Mudokon::RunSlideTurn_25_473AA0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::RunTurn_26_473BB0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Sneaking_27_473C40()
{
    NOT_IMPLEMENTED();
}

void Mudokon::WalkToSneak1_28_473D60()
{
    NOT_IMPLEMENTED();
}

void Mudokon::SneakingToWalk_29_473E20()
{
    NOT_IMPLEMENTED();
}

void Mudokon::WalkToSneak2_30_473EE0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::SneakingToWalk2_31_473F30()
{
    NOT_IMPLEMENTED();
}

void Mudokon::StandToSneaking_32_473F80()
{
    NOT_IMPLEMENTED();
}

void Mudokon::SneakingToStand1_33_473FF0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::SneakingToStand2_34_474020()
{
    NOT_IMPLEMENTED();
}

void Mudokon::JumpStart_35_474460()
{
    NOT_IMPLEMENTED();
}

void Mudokon::JumpMid_36_474570()
{
    NOT_IMPLEMENTED();
}

void Mudokon::WalkToRun_37_4749A0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Slap_38_474AA0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::StartHoistJumpUp_39_4748E0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::HoistFallToFloor_40_474780()
{
    NOT_IMPLEMENTED();
}

void Mudokon::HitFloorStanding1_41_474960()
{
    NOT_IMPLEMENTED();
}

void Mudokon::HitFloorStanding2_42_4743F0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::StandToDunno_43_472790()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::DunnoToStand_44_4727B0;
    }
}

void Mudokon::DunnoToStand_44_4727B0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::KnockForward_45_474180()
{
    NOT_IMPLEMENTED();
}

void Mudokon::StandToKnockBack_46_4742A0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::KnockBackToStand_47_474380()
{
    NOT_IMPLEMENTED();
}

void Mudokon::FallLedgeBegin_48_4743C0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Fall_49_472C60()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Chanting_50_473040()
{
    NOT_IMPLEMENTED();
}

void Mudokon::ToChant_51_4730D0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::ToDuck_52_474A20()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Duck_53_474A40()
{
    NOT_IMPLEMENTED();
}

void Mudokon::DuckToCrouch_54_474A80()
{
    NOT_IMPLEMENTED();
}

void Mudokon::DuckKnockBack_55_474250()
{
    NOT_IMPLEMENTED();
}

void Mudokon::SlapOwnHead_56_4727D0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::TurnWheelBegin_57_474C00()
{
    NOT_IMPLEMENTED();
}

void Mudokon::TurnWheelLoop_58_474CC0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::TurnWheelEnd_59_474D30()
{
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
    return 0;
}

GameSpeakEvents Mudokon::LastGameSpeak_476FF0()
{
    NOT_IMPLEMENTED();

    __int16 actualEvent; // si

    const int lastEventIdx = pEventSystem_5BC11C->field_28_last_event_index;
    if (field_140 == lastEventIdx)
    {
        // TODO -2 is for ???
        // -1 = -1 - -2 = 1
        // 0 -2 = -2
        actualEvent = (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eNone) - 2;
    }
    else
    {
        field_140 = lastEventIdx;
        actualEvent = pEventSystem_5BC11C->field_20_last_event;
    }

    // Not valid if not in the same camera
    if (Is_In_Current_Camera_424A70() != CameraPos::eCamCurrent_0)
    {
        return GameSpeakEvents::eNone;
    }
    
    // Look out works on any scale
    if (actualEvent == GameSpeakEvents::eLookOut_6)
    {
        return GameSpeakEvents::eLookOut_6;
    }

    // Check in valid range and on same scale
    if (actualEvent < 1 || actualEvent > GameSpeakEvents::eSorry_24 || sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
    {
        return (GameSpeakEvents)actualEvent;
    }

    return GameSpeakEvents::eNone;
}

__int16 Mudokon::LaughingGasInCurrentScreen_4774A0()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Mudokon::Sound_475EC0(MudSounds idx)
{
    if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        if (field_180_emo_tbl != Mud_Emotion::eSick_7)
        {
            // TODO: Pass down the stronger types - assuming they map directly
            Abe_SFX_457EC0(static_cast<unsigned char>(idx), 80, field_13C_voice_pitch, this);
            return;
        }

        if (idx != MudSounds::e24)
        {
            Abe_SFX_457EC0(static_cast<unsigned char>(idx), 80, -field_13C_voice_pitch, this);
            return;
        }

        short pitch1 = Math_RandomRange_496AB0(-5, 0);
        pitch1 *= 100;
        pitch1 = pitch1 - field_13C_voice_pitch;
        Abe_SFX_457EC0(24u, Math_RandomRange_496AB0(20, 50), pitch1, this);
       
        short pitch2 = Math_RandomRange_496AB0(-5, 0);
        pitch2 *= 100;
        pitch2 = pitch2 - field_13C_voice_pitch;
        Abe_SFX_457EC0(28u, Math_RandomRange_496AB0(20, 60), pitch2, this);
        return;
    }

    if (field_180_emo_tbl == Mud_Emotion::eSick_7)
    {
        if (idx == MudSounds::e24)
        {
            short pitch1 = Math_RandomRange_496AB0(-5, 0);
            pitch1 *= 100;
            pitch1 = pitch1 - field_13C_voice_pitch;
            Abe_SFX_457EC0(24u, Math_RandomRange_496AB0(55, 90), pitch1, this);

            short pitch2 = Math_RandomRange_496AB0(-5, 0);
            pitch2 *= 100;
            pitch2 = pitch2 - field_13C_voice_pitch;
            Abe_SFX_457EC0(28u, Math_RandomRange_496AB0(60, 110), pitch2, this);
            return;
        }
        Abe_SFX_457EC0(static_cast<unsigned char>(idx), 0, field_13C_voice_pitch, this);
    }
    else
    {
        Abe_SFX_457EC0(static_cast<unsigned char>(idx), 0, field_13C_voice_pitch, this);
    }
}

__int16 Mudokon::CanRespond_4770B0()
{
    NOT_IMPLEMENTED();
    return 1;
}

BYTE** Mudokon::AnimBlockForMotion_474DC0(short motion)
{
    if (motion < Mud_Motion::PullLever_10_473020)
    {
        return field_10_resources_array.ItemAt(0);
    }
    else if (motion < Mud_Motion::Chisel_11_4732D0)
    {
        if (!field_10_resources_array.ItemAt(1))
        {
            field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbepullResID, TRUE, FALSE));
        }
        return field_10_resources_array.ItemAt(1);
    }
    else if (motion < Mud_Motion::CrouchScrub_14_473560)
    {
        return field_10_resources_array.ItemAt(2);
    }
    else if (motion < Mud_Motion::CrouchIdle_15_474040)
    {
        return field_10_resources_array.ItemAt(3);
    }
    else if (motion < Mud_Motion::KnockForward_45_474180)
    {
        return field_10_resources_array.ItemAt(4);
    }
    else if (motion < Mud_Motion::StandToKnockBack_46_4742A0)
    {
        return field_10_resources_array.ItemAt(5);
    }
    else if (motion < Mud_Motion::FallLedgeBegin_48_4743C0)
    {
        return field_10_resources_array.ItemAt(6);
    }
    else if (motion < Mud_Motion::Chanting_50_473040)
    {
        return field_10_resources_array.ItemAt(7);
    }
    else if (motion < Mud_Motion::ToDuck_52_474A20)
    {
        return field_10_resources_array.ItemAt(8);
    }
    else if (motion < Mud_Motion::DuckKnockBack_55_474250)
    {
        return field_10_resources_array.ItemAt(9);
    }
    else if (motion < Mud_Motion::SlapOwnHead_56_4727D0)
    {
        return field_10_resources_array.ItemAt(10);
    }
    else if (motion < Mud_Motion::TurnWheelBegin_57_474C00)
    {
        return field_10_resources_array.ItemAt(11);
    }
    else if (motion < 60) // MAX ??
    {
        if (!field_10_resources_array.ItemAt(12))
        {
            field_10_resources_array.SetAt(12, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeworkResID, TRUE, FALSE));
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
        vOnTrapDoorOpen_4081F0();
        field_106_current_motion = Mud_Motion::Fall_49_472C60;
    }
}

void Mudokon::ToStand_4724A0()
{
    field_134 = FP_FromInteger(0);
    field_138 = 0;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_106_current_motion = Mud_Motion::StandIdle_0_4724E0;
}

__int16 Mudokon::FindWheel_4777B0(FP /*xpos*/, FP /*ypos*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::sub_477AF0(MudAction action)
{
    field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, action);
    if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags::eBit11_get_depressed))
    {
        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
    }
    return field_188_pTblEntry->field_6_sub_state;

}

__int16 Mudokon::sub_477880()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Mudokon::StandingKnockBack_473190()
{
    NOT_IMPLEMENTED();
}

// TODO: Abe sneaking or walking check ??
__int16 Mudokon::IsMotionUnknown_4730F0()
{
    return 
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_40_SneakLoop_450550 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_45_SneakBegin_4507A0 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_41_450250 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_43_450380 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_42_4503D0 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_44_450500;
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
        field_134 = FP_FromDouble(0.3); // TODO: or 2.99 ??
        field_F8 = field_BC_ypos;
        field_106_current_motion = Mud_Motion::FallLedgeBegin_48_4743C0;
        field_B8_xpos = oldXPos + field_C4_velx;
    }

}

const struct MudEmotionTableEntry* CC Mudokon::ResponseTo_471730(Mud_Emotion emotion, MudAction action)
{
    return &kMudEmoTable_55C790.field_0_data[static_cast<int>(emotion)][static_cast<int>(action)];
}

void Mudokon::AddAlerted()
{
    if (!field_16A_flags.Get(Flags::eBit3_Alerted))
    {
        field_16A_flags.Set(Flags::eBit3_Alerted);
        sAlertedMudCount_5C3010++;
    }
}

void Mudokon::RemoveAlerted()
{
    if (field_16A_flags.Get(Flags::eBit3_Alerted))
    {
        sAlertedMudCount_5C3010--;
        field_16A_flags.Clear(Flags::eBit3_Alerted);
    }
}

__int16 Mudokon::AIStartWheelTurning()
{
    field_108_next_motion = Mud_Motion::TurnWheelBegin_57_474C00;
    RemoveAlerted();
    field_16A_flags.Set(Flags::eBit7);
    field_18E_ai_state = Mud_AI_State::AI_State_3_47E0D0;
    return BrainStates4::eState4_0;
}

bool Mudokon::NeedToTurnAround()
{
    Mudokon* pAboveMud = static_cast<Mudokon*>(FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
    if (sAlertedMudCount_5C3010 > 1 && pAboveMud && !(vIsFacingMe_4254A0(pAboveMud) && pAboveMud->field_106_current_motion != Mud_Motion::TurnAroundStanding_2_472BF0))
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
