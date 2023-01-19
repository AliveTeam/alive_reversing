#pragma once

enum class Mud_Emotion : s16
{
    eNormal_0 = 0,
    eAngry_1 = 1,
    eAggressive_2 = 2,
    eSad_3 = 3,
    eSuicidal_4 = 4,
    eHappy_5 = 5, // TODO: Not used ??
    eWired_6 = 6,
    eSick_7 = 7
};

enum class MudAction : s16
{
    eHelloOrAllYa_0 = 0,
    eFollowMe_1 = 1,
    eWait_2 = 2,
    eUnused_3 = 3,
    eStopIt_4 = 4,
    eUnused_5 = 5,
    eFart_6 = 6,
    eSlapOrWater_7 = 7,
    eSorry_8 = 8,
    eMudAbuse_9 = 9,
    eComfort_10 = 10,
    eSmackIntoWall_11 = 11,
    eLaugh_12 = 12,
    eDuck_13 = 13,
    eMudDied_14 = 14,
    eUnknown_15 = 15,
    eUnknown_16 = 16,
    eNone_17 = 17,
};

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