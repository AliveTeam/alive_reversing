#include "stdafx_common.h"
#include "AnimResources.hpp"
#include "Sys_common.hpp"

constexpr PalDetails kNullPalDetails = {};
constexpr CombinedPalRecord kPalRecords[] = {
    {PalId::BlindMud, {"MUDPAL.BND", kMudblindResID}, kNullPalDetails },
};

constexpr AnimDetails kNullAnimDetails = {};

constexpr CombinedAnimRecord kAnimRecords[] = {
    {AnimId::Abe_Head_Gib, { "ABEBLOW.BAN", 7732, 50, 25, kAbeblowResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Abe_Arm_Gib, { "ABEBLOW.BAN", 7772, 50, 25, kAbeblowResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Abe_Body_Gib, { "ABEBLOW.BAN", 7812, 50, 25, kAbeblowResID, PalId::Default}, kNullAnimDetails },
    {AnimId::BlindMud_Head_Gib, { "ABEBLOW.BAN", 7732, 50, 25, kAbeblowResID, PalId::BlindMud}, kNullAnimDetails },
    {AnimId::BlindMud_Arm_Gib, { "ABEBLOW.BAN", 7772, 50, 25, kAbeblowResID, PalId::BlindMud}, kNullAnimDetails },
    {AnimId::BlindMud_Body_Gib, { "ABEBLOW.BAN", 7812, 50, 25, kAbeblowResID, PalId::BlindMud}, kNullAnimDetails },
    {AnimId::Glukkon_Head_Gib, { "GLUKKON.BND", 8140, 48, 28, kGlukkonResID_801, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Arm_Gib, { "GLUKKON.BND", 8188, 48, 28, kGlukkonResID_801, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Body_Gib, { "GLUKKON.BND", 8188, 48, 28, kGlukkonResID_801, PalId::Default}, kNullAnimDetails },
    {AnimId::Aslik_Head_Gib, { "ASLIK.BND", 8140, 48, 28, kAslikResID_803, PalId::Default}, kNullAnimDetails },
    {AnimId::Aslik_Arm_Gib, { "ASLIK.BND", 8188, 48, 28, kAslikResID_803, PalId::Default}, kNullAnimDetails },
    {AnimId::Aslik_Body_Gib, { "ASLIK.BND", 8188, 48, 28, kAslikResID_803, PalId::Default}, kNullAnimDetails },
    {AnimId::Dripik_Head_Gib, { "DRIPIK.BND", 8140, 48, 28, kDripikResID_805, PalId::Default}, kNullAnimDetails },
    {AnimId::Dripik_Arm_Gib, { "DRIPIK.BND", 8188, 48, 28, kDripikResID_805, PalId::Default}, kNullAnimDetails },
    {AnimId::Dripik_Body_Gib, { "DRIPIK.BND", 8188, 48, 28, kDripikResID_805, PalId::Default}, kNullAnimDetails },
    {AnimId::Phleg_Head_Gib, { "PHLEG.BND", 8140, 48, 28, kPhlegResID_807, PalId::Default}, kNullAnimDetails },
    {AnimId::Phleg_Arm_Gib, { "PHLEG.BND", 8188, 48, 28, kPhlegResID_807, PalId::Default}, kNullAnimDetails },
    {AnimId::Phleg_Body_Gib, { "PHLEG.BND", 8188, 48, 28, kPhlegResID_807, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_Head_Gib, { "FLEEBLOW.BAN", 1088, 19, 8, kFleeBlowResID_580, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_Body_Gib, { "FLEEBLOW.BAN", 1128, 19, 8, kFleeBlowResID_580, PalId::Default}, kNullAnimDetails },
    
    {AnimId::Meat_Gib, kNullAnimDetails, { "R1MEAT.BAN", 8908, 69, 30, kMeatGibResID_6014, PalId::Default} },
    {AnimId::Metal_Gib, { "METAL.BAN", 2244, 38, 29, kMetalGib, PalId::Default}, kNullAnimDetails },
    {AnimId::Rock_Gib, kNullAnimDetails, { "DEBRIS00.BAN", 8108, 71, 36, kDebrisID00, PalId::Default} },
    {AnimId::Slig_Head_Gib, { "SLGBLOW.BAN", 6480, 44, 26, kSligBlowResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Arm_Gib, { "SLGBLOW.BAN", 6560, 44, 26, kSligBlowResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Body_Gib, { "SLGBLOW.BAN", 6520, 44, 26, kSligBlowResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_Head_Gib, { "DOGBLOW.BAN", 7504, 53, 28, kSlogBlowResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_Body_Gib, { "DOGBLOW.BAN", 7544, 53, 28, kSlogBlowResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Stick_Gib, kNullAnimDetails, { "STICK.BAN", 2800, 47, 29, kStickGib, PalId::Default} },

    {AnimId::Bat, kNullAnimDetails, { "BATBASIC.BAN", 6560, 48, 17, kBatBasicResID, PalId::Default} },
    {AnimId::Bee_Swarm, kNullAnimDetails, { "WASP.BAN", 636, 7, 4, kWaspResID, PalId::Default} },

    // Note: Fleech animations are sorted by order of appearance under Fleech.cpp (line 235)
    {AnimId::Fleech_Sleeping, { "FLEECH.BAN", 37808, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_WakingUp, { "FLEECH.BAN", 37884, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_Unused, { "FLEECH.BAN", 37896, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails }, // Slow broken animation, probably unused
    {AnimId::Fleech_Idle, { "FLEECH.BAN", 37704, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_Crawl, { "FLEECH.BAN", 37748, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_PatrolCry, { "FLEECH.BAN", 37784, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_Knockback, { "FLEECH.BAN", 37924, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_StopCrawling, { "FLEECH.BAN", 37936, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_StopMidCrawlCycle, { "FLEECH.BAN", 37960, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_Fall, { "FLEECH.BAN", 37984, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_Land, { "FLEECH.BAN", 38060, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_RaiseHead, { "FLEECH.BAN", 38112, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_Climb, { "FLEECH.BAN", 38156, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_SettleOnGround, { "FLEECH.BAN", 38208, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_ExtendTongueFromEnemy, { "FLEECH.BAN", 38260, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_RetractTongueFromEnemey, { "FLEECH.BAN", 37848, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_DeathByFalling, { "FLEECH.BAN", 38248, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_SleepingWithTongue, { "FLEECH.BAN", 38396, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fleech_Consume, { "FLEECH.BAN", 38276, 73, 35u, kFleechResID, PalId::Default}, kNullAnimDetails },

    {AnimId::Flying_Slig_Idle, { "FLYSLIG.BND", 116888, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Move_Horizontal, { "FLYSLIG.BND", 116912, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Idle_Turn_Around, { "FLYSLIG.BND", 117084, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Move_Down, { "FLYSLIG.BND", 116988, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Move_Down_Turn_Around, { "FLYSLIG.BND", 117584, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Move_Up, { "FLYSLIG.BND", 117012, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Move_Up_Turn_Around, { "FLYSLIG.BND", 117616, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Pull_Lever, { "FLYSLIG.BND", 117188, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Speak, { "FLYSLIG.BND", 117132, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Possession, { "FLYSLIG.BND", 117524, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Move_Horizontal_End, { "FLYSLIG.BND", 117060, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Move_Up_Start, { "FLYSLIG.BND", 117316, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Move_Horizontal_To_Down, { "FLYSLIG.BND", 117276, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Move_Up_To_Horizontal, { "FLYSLIG.BND", 117444, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Move_Down_To_Horizontal, { "FLYSLIG.BND", 117376, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Turn_Quick, { "FLYSLIG.BND", 116936, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Idle_To_Horizontal, { "FLYSLIG.BND", 117036, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Move_Down_Start, { "FLYSLIG.BND", 117336, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Move_Down_End, { "FLYSLIG.BND", 117356, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Knockback_Down, { "FLYSLIG.BND", 117396, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Knockback_Up, { "FLYSLIG.BND", 117464, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Move_Up_End, { "FLYSLIG.BND", 117424, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Up_Turn_Instant, { "FLYSLIG.BND", 117552, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Down_Turn_Instant, { "FLYSLIG.BND", 117492, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Move_Horizontal_To_Up, { "FLYSLIG.BND", 117296, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Flying_Slig_Turn_Horizontal, { "FLYSLIG.BND", 117752, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },

    // Note: Glukkon animations are sorted by order of appearance under Glukkon.ccp (line 74)
    {AnimId::Glukkon_Normal_Idle, { "GLUKKON.BND", 169608, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_Walk, { "GLUKKON.BND", 169644, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_Turn, { "GLUKKON.BND", 169724, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_KnockBack, { "GLUKKON.BND", 169772, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_Jump, { "GLUKKON.BND", 169828, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_JumpToFall, { "GLUKKON.BND", 169900, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_WalkToFall, { "GLUKKON.BND", 169920, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_Fall, { "GLUKKON.BND", 169952, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_DeathFall, { "GLUKKON.BND", 169968, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_Land, { "GLUKKON.BND", 170000, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_ChantShake, { "GLUKKON.BND", 170152, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_Speak1, { "GLUKKON.BND", 170216, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails }, // Speak: "Hey", "Do it", "Help"
    {AnimId::Glukkon_Normal_Speak2, { "GLUKKON.BND", 170268, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails }, // Speak: "Come here", "Kill em", "All of ya"
    {AnimId::Glukkon_Normal_LongLaugh, { "GLUKKON.BND", 170056, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_BeginWalk, { "GLUKKON.BND", 170312, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_EndWalk, { "GLUKKON.BND", 170332, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_StandToJump, { "GLUKKON.BND", 170352, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_JumpToStand, { "GLUKKON.BND", 170372, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_WalkToJump, { "GLUKKON.BND", 170392, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_JumpToWalk, { "GLUKKON.BND", 170412, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_KnockBackStandBegin, { "GLUKKON.BND", 170432, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_GetShot, { "GLUKKON.BND", 170152, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_KnockBackStandEnd, { "GLUKKON.BND", 170472, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Normal_Speak3, { "GLUKKON.BND", 170504, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },     // Speak: "Stay here"
    {AnimId::Glukkon_Normal_EndSingleStep, { "GLUKKON.BND", 170560, 163, 79, kGlkbasicResID, PalId::Default}, kNullAnimDetails },

    {AnimId::Glukkon_Aslik_Idle, { "ASLIK.BND", 171356, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_Walk, { "ASLIK.BND", 171392, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_Turn, { "ASLIK.BND", 171472, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_KnockBack, { "ASLIK.BND", 171520, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_Jump, { "ASLIK.BND", 171576, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_JumpToFall, { "ASLIK.BND", 171648, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_WalkToFall, { "ASLIK.BND", 171668, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_Fall, { "ASLIK.BND", 171700, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_DeathFall, { "ASLIK.BND", 171716, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_Land, { "ASLIK.BND", 171748, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_ChantShake, { "ASLIK.BND", 171900, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_Speak1, { "ASLIK.BND", 171964, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_Speak2, { "ASLIK.BND", 172016, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_LongLaugh, { "ASLIK.BND", 171804, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_BeginWalk, { "ASLIK.BND", 172060, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_EndWalk, { "ASLIK.BND", 172080, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_StandToJump, { "ASLIK.BND", 172100, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_JumpToStand, { "ASLIK.BND", 172120, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_WalkToJump, { "ASLIK.BND", 172140, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_JumpToWalk, { "ASLIK.BND", 172160, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_KnockBackStandBegin, { "ASLIK.BND", 172180, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_GetShot, { "ASLIK.BND", 171900, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_KnockBackStandEnd, { "ASLIK.BND", 172220, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_Speak3, { "ASLIK.BND", 172252, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Aslik_EndSingleStep, { "ASLIK.BND", 172308, 157, 76, kGlukAslikResID, PalId::Default}, kNullAnimDetails },

    {AnimId::Glukkon_Phleg_Idle, { "PHLEG.BND", 199500, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_Walk, { "PHLEG.BND", 199536, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_Turn, { "PHLEG.BND", 199616, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_KnockBack, { "PHLEG.BND", 199664, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_Jump, { "PHLEG.BND", 199720, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_JumpToFall, { "PHLEG.BND", 199792, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_WalkToFall, { "PHLEG.BND", 199812, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_Fall, { "PHLEG.BND", 199844, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_DeathFall, { "PHLEG.BND", 199860, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_Land, { "PHLEG.BND", 199892, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_ChantShake, { "PHLEG.BND", 200044, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_Speak1, { "PHLEG.BND", 200108, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_Speak2, { "PHLEG.BND", 200160, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_LongLaugh, { "PHLEG.BND", 199948, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_BeginWalk, { "PHLEG.BND", 200204, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_EndWalk, { "PHLEG.BND", 200224, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_StandToJump, { "PHLEG.BND", 200244, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_JumpToStand, { "PHLEG.BND", 200264, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_WalkToJump, { "PHLEG.BND", 200284, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_JumpToWalk, { "PHLEG.BND", 200304, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_KnockBackStandBegin, { "PHLEG.BND", 200324, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_GetShot, { "PHLEG.BND", 200044, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_KnockBackStandEnd, { "PHLEG.BND", 200364, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_Speak3, { "PHLEG.BND", 200396, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Phleg_EndSingleStep, { "PHLEG.BND", 200452, 145, 72, kGlukPhlegResID, PalId::Default}, kNullAnimDetails },

    {AnimId::Glukkon_Dripik_Idle, { "DRIPIK.BND", 193064, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_Walk, { "DRIPIK.BND", 193100, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_Turn, { "DRIPIK.BND", 193180, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_KnockBack, { "DRIPIK.BND", 193228, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_Jump, { "DRIPIK.BND", 193284, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_JumpToFall, { "DRIPIK.BND", 193356, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_WalkToFall, { "DRIPIK.BND", 193376, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_Fall, { "DRIPIK.BND", 193408, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_DeathFall, { "DRIPIK.BND", 193424, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_Land, { "DRIPIK.BND", 193456, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_ChantShake, { "DRIPIK.BND", 193608, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_Speak1, { "DRIPIK.BND", 193672, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_Speak2, { "DRIPIK.BND", 193724, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_LongLaugh, { "DRIPIK.BND", 193512, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_BeginWalk, { "DRIPIK.BND", 193768, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_EndWalk, { "DRIPIK.BND", 193788, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_StandToJump, { "DRIPIK.BND", 193808, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_JumpToStand, { "DRIPIK.BND", 193828, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_WalkToJump, { "DRIPIK.BND", 193848, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_JumpToWalk, { "DRIPIK.BND", 193868, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_KnockBackStandBegin, { "DRIPIK.BND", 193888, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_GetShot, { "DRIPIK.BND", 193608, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_KnockBackStandEnd, { "DRIPIK.BND", 193928, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_Speak3, { "DRIPIK.BND", 193960, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Glukkon_Dripik_EndSingleStep, { "DRIPIK.BND", 194016, 145, 72, kGlukDripikResID, PalId::Default}, kNullAnimDetails },

    {AnimId::Greeter_Moving, { "GREETER.BAN", 50028, 65, 60, kGreeterResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Greeter_Turn, { "GREETER.BAN", 50072, 65, 60, kGreeterResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Greeter_Hit, { "GREETER.BAN", 50236, 65, 60, kGreeterResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Greeter_Speak, { "GREETER.BAN", 50104, 65, 60, kGreeterResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Greeter_Chase, { "GREETER.BAN", 50144, 65, 60, kGreeterResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Greeter_Falling, { "GREETER.BAN", 50212, 65, 60, kGreeterResID, PalId::Default}, kNullAnimDetails },

    {AnimId::Mine_Car_Closed, { "BAYROLL.BAN", 20836, 130, 62u, kBayrollResID_6013, PalId::Default}, kNullAnimDetails },
    {AnimId::Mine_Car_Open, { "BAYROLL.BAN", 20812, 130, 62u, kBayrollResID_6013, PalId::Default}, kNullAnimDetails },
    {AnimId::Mine_Car_Shake_A, { "BAYROLL.BAN", 20872, 130, 62u, kBayrollResID_6013, PalId::Default}, kNullAnimDetails },
    {AnimId::Mine_Car_Shake_B, { "BAYROLL.BAN", 20900, 130, 62u, kBayrollResID_6013, PalId::Default}, kNullAnimDetails },
    {AnimId::Mine_Car_Tread_Idle, { "BAYROLL.BAN", 20824, 130, 62u, kBayrollResID_6013, PalId::Default}, kNullAnimDetails },
    {AnimId::Mine_Car_Tread_Move_A, { "BAYROLL.BAN", 20848, 130, 62u, kBayrollResID_6013, PalId::Default}, kNullAnimDetails },
    {AnimId::Mine_Car_Tread_Move_B, { "BAYROLL.BAN", 20788, 130, 62u, kBayrollResID_6013, PalId::Default}, kNullAnimDetails },

    // The Mudokon section includes all of abe's animations as well since they share many animations
    {AnimId::Mudokon_ToSpeak, { "ABEBSIC1.BAN", 59112, 135, 80, kAbebsic1ResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Null, { "ABEBSIC.BAN", 270240, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_CrouchSpeak, { "ABEBSIC.BAN", 271080, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_ToCrouchSpeak, { "ABEBSIC.BAN", 271120, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RollBegin, { "ABEBSIC.BAN", 269688, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Rolling, { "ABEBSIC.BAN", 269708, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RollingEnd_Unused, { "ABEBSIC.BAN", 269764, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_HopBegin, { "ABEBSIC.BAN", 271004, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_HopMid, { "ABEBSIC.BAN", 271056, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_HopLand, { "ABEBSIC.BAN", 270964, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RunJumpLand, { "ABEBSIC.BAN", 270488, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_DunnoEnd, { "ABEBSIC.BAN", 270616, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RunToRoll, { "ABEBSIC.BAN", 270708, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RunTurnToWalk, { "ABEBSIC.BAN", 270140, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RunJumpLandRun, { "ABEBSIC.BAN", 269784, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RunJumpLandWalk, { "ABEBSIC.BAN", 269856, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_TurnToRun, { "ABEBSIC.BAN", 270944, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Sorry, { "ABEBSIC.BAN", 271192, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_AfterSorry, { "ABEBSIC.BAN", 271248, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_LedgeAscend, { "ABEHOIST.BAN", 57440, 135, 80, kAbehoistResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_LedgeDescend, { "ABEHOIST.BAN", 57580, 135, 80, kAbehoistResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_LedgeHang, { "ABEHOIST.BAN", 57540, 135, 80, kAbehoistResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_LedgeHangWobble, { "ABEHOIST.BAN", 57712, 135, 80, kAbehoistResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RingRopePullHang, { "ABEHOIST.BAN", 57680, 135, 80, kAbehoistResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_PushWall, { "ABEKNBK.BAN", 49912, 135, 80, kAbeknbkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RollingKnockback, { "ABEKNBK.BAN", 49856, 135, 80, kAbeknbkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_JumpIntoWell, { "ABEWELL.BAN", 20572, 135, 80, kAbewellResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Well_Idle, { "ABEWELL.BAN", 20552, 135, 80, kAbewellResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_WellBegin, { "ABEWELL.BAN", 20404, 135, 80, kAbewellResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_FallLandDie, { "ABESMASH.BAN", 8104, 135, 80, kAbesmashResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Fall, { "ABEFALL.BAN", 5724, 135, 80, kAbefallResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_HandstoneBegin, { "ABESTONE.BAN", 16096, 135, 80, kAbestoneResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_HandstoneEnd, { "ABESTONE.BAN", 16040, 135, 80, kAbestoneResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_GrenadeMachineUse, { "ABESTONE.BAN", 16220, 135, 80, kAbestoneResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RunOffEdge, { "ABEEDGE.BAN", 32068, 135, 80, kAbeedgeResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_SneakOffEdge, { "ABEEDGE.BAN", 32112, 135, 80, kAbeedgeResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_HopToFall, { "ABEEDGE.BAN", 32168, 135, 80, kAbeedgeResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RunJumpToFall, { "ABEEDGE.BAN", 32152, 135, 80, kAbeedgeResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RollOffEdge, { "ABEEDGE.BAN", 32132, 135, 80, kAbeedgeResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_SlapBomb, { "ABEBOMB.BAN", 6520, 135, 80, kAbebombResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RollingKnockForward, { "ABEKNFD.BAN", 17180, 135, 80, kAbeknfdResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RockThrowStandingHold, { "ABETHROW.BAN", 31504, 135, 80, kAbethrowResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RockThrowStandingThrow, { "ABETHROW.BAN", 31392, 135, 80, kAbethrowResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RockThrowStandingEnd, { "ABETHROW.BAN", 31632, 135, 80, kAbethrowResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RockThrowCrouchingHold, { "ABETHROW.BAN", 31576, 135, 80, kAbethrowResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RockThrowCrouchingThrow, { "ABETHROW.BAN", 31432, 135, 80, kAbethrowResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_ZShotRolling, { "ABEKNOKZ.BAN", 24580, 135, 80, kAbeknokzResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_ZShot, { "ABEKNOKZ.BAN", 24628, 135, 80, kAbeknokzResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_PickupItem, { "ABEPICK.BAN", 6004, 135, 80, kAbepickResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_DoorEnter, { "ABEDOOR.BAN", 19040, 135, 80, kAbedoorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_DoorExit, { "ABEDOOR.BAN", 19088, 135, 80, kAbedoorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_MineCarEnter, { "ABECAR.BAN", 8540, 135, 80, kAbeCarResId, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_MineCarExit, { "ABECAR.BAN", 8588, 135, 80, kAbeCarResId, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_ToShrykull, 
        { "ABEMORPH.BAN", 8732, 135, 80, kAbemorphResID, PalId::Default},
        { "ABEMORPH.BAN", 92004, 121, 79, kShrmorphResID, PalId::Default} 
    },
    {AnimId::Mudokon_EndShrykull, { "ABEMORPH.BAN", 8772, 135, 80, kAbemorphResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_LiftGrabBegin, { "ABELIFT.BAN", 22548, 135, 80, kAbeliftResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_LiftGrabEnd, { "ABELIFT.BAN", 22572, 135, 80, kAbeliftResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_LiftGrabIdle, { "ABELIFT.BAN", 22464, 135, 80, kAbeliftResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_LiftUseUp, { "ABELIFT.BAN", 22596, 135, 80, kAbeliftResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_LiftUseDown, { "ABELIFT.BAN", 22496, 135, 80, kAbeliftResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_PoisonGasDeath, { "ABEGAS.BAN", 28824, 135, 80, kAbegasResID, PalId::Default}, kNullAnimDetails },

    // TODO: check AO maxW/maxH
    {AnimId::Mudokon_Idle, 
        { "ABEBSIC1.BAN", 58888, 135, 80, kAbebsic1ResID, PalId::Default}, 
        {"ABEBSIC1.BAN", 55968, 135, 80, kAbebsic1ResID, PalId::Default}},
    {AnimId::Mudokon_Walk, { "ABEBSIC1.BAN", 58808, 135, 80, kAbebsic1ResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_StandingTurn, { "ABEBSIC1.BAN", 59064, 135, 80, kAbebsic1ResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Speak1, { "ABEBSIC1.BAN", 59028, 135, 80, kAbebsic1ResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Speak3, { "ABEBSIC1.BAN", 58956, 135, 80, kAbebsic1ResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Speak2, { "ABEBSIC1.BAN", 58992, 135, 80, kAbebsic1ResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_SpeakFart, { "ABEBSIC1.BAN", 58920, 135, 80, kAbebsic1ResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_WalkBegin, { "ABEBSIC1.BAN", 58768, 135, 80, kAbebsic1ResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_WalkToIdle, { "ABEBSIC1.BAN", 58788, 135, 80, kAbebsic1ResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_MidWalkToIdle, { "ABEBSIC1.BAN", 58748, 135, 80, kAbebsic1ResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_LeverUse, { "ABEPULL.BAN", 11396, 135, 80, kAbepullResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Chisel, { "MUDCHSL.BAN", 5276, 135, 80, kMudchslResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_StartChisel, { "MUDCHSL.BAN", 5308, 135, 80, kMudchslResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_StopChisel, { "MUDCHSL.BAN", 5320, 135, 80, kMudchslResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_CrouchScrub, { "MUDSCRUB.BAN", 9388, 135, 80, kMudscrubResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_CrouchIdle, { "ABEBSIC.BAN", 270092, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_CrouchTurn, { "ABEBSIC.BAN", 270428, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_StandToCrouch, { "ABEBSIC.BAN", 269876, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_CrouchToStand, { "ABEBSIC.BAN", 270060, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_WalkToRun, { "ABEBSIC.BAN", 270120, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_MidWalkToRun, { "ABEBSIC.BAN", 270268, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Run, { "ABEBSIC.BAN", 270748, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RunToWalk, { "ABEBSIC.BAN", 270668, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_MidRunToWalk, { "ABEBSIC.BAN", 270820, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RunSlideStop, { "ABEBSIC.BAN", 270860, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RunTurn, { "ABEBSIC.BAN", 270328, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RunTurnToRun, { "ABEBSIC.BAN", 270408, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Sneak, { "ABEBSIC.BAN", 270508, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_WalkToSneak, { "ABEBSIC.BAN", 270688, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_SneakToWalk, { "ABEBSIC.BAN", 270596, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_MidWalkToSneak, { "ABEBSIC.BAN", 270840, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_MidSneakToWalk, { "ABEBSIC.BAN", 270728, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_SneakBegin, { "ABEBSIC.BAN", 270288, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_SneakToIdle, { "ABEBSIC.BAN", 270160, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_MidSneakToIdle, { "ABEBSIC.BAN", 270308, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RunJumpBegin, { "ABEBSIC.BAN", 270468, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RunJumpMid, { "ABEBSIC.BAN", 269804, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_StandingToRun, { "ABEBSIC.BAN", 270252, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Punch, { "ABEBSIC.BAN", 271152, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_HoistBegin, { "ABEBSIC.BAN", 269976, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_HoistIdle, { "ABEBSIC.BAN", 270024, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_LandSoft, { "ABEBSIC.BAN", 269928, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_DunnoBegin, { "ABEBSIC.BAN", 270180, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Shrug_End, { "ABEBSIC.BAN", 270616, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_KnockForward, { "ABEKNFD.BAN", 17240, 135, 80, kAbeknfdResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Knockback, { "ABEKNBK.BAN", 49740, 135, 80, kAbeknbkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_KnockbackGetUp, { "ABEKNBK.BAN", 49800, 135, 80, kAbeknbkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_WalkOffEdge, { "ABEEDGE.BAN", 32040, 135, 80, kAbeedgeResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_FallingFromGrab, { "ABEEDGE.BAN", 32012, 135, 80, kAbeedgeResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Chant, { "ABEOMM.BAN", 9992, 135, 80, kAbeommResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_ChantEnd, { "ABEOMM.BAN", 10040, 135, 80, kAbeommResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_ToDuck, { "MUDWORK.BND", 5236, 135, 80, kMudoduckResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Duck, { "MUDWORK.BND", 5256, 135, 80, kMudoduckResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_DuckToCrouch, { "MUDWORK.BND", 5280, 135, 80, kMudoduckResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_DuckKnockback, { "MUDBTLNK.BAN", 5328, 135, 80, kMudbtlnkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_SlapOwnHead, { "MUDIDLE.BAN", 9640, 135, 80, kMudidleResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_TurnWheelBegin, { "ABEWORK.BAN", 11856, 135, 80, kAbeworkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_TurnWheel, { "ABEWORK.BAN", 11816, 135, 80, kAbeworkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_TurnWheelEnd, { "ABEWORK.BAN", 11888, 135, 80, kAbeworkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_Sling, kNullAnimDetails, { "MUDSLING.BAN", 22636, 96, 55, kSlingmudResID_508, PalId::Default} },

    {AnimId::Paramite_Idle, { "PARAMITE.BND", 96696, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_WalkBegin, { "PARAMITE.BND", 96676, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Walking, { "PARAMITE.BND", 96548, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Running, { "PARAMITE.BND", 96612, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Turn, { "PARAMITE.BND", 96792, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Hop, { "PARAMITE.BND", 96728, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Unused, { "PARAMITE.BND", 97164, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails }, // crashes the game when used
    {AnimId::Paramite_WalkRunTransition, { "PARAMITE.BND", 97172, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_WalkEnd, { "PARAMITE.BND", 96828, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_RunBegin, { "PARAMITE.BND", 96868, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_RunEnd, { "PARAMITE.BND", 96848, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Falling, { "PARAMITE.BND", 96888, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_JumpUpBegin, { "PARAMITE.BND", 96904, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_JumpUpMidair, { "PARAMITE.BND", 96932, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_JumpUpLand, { "PARAMITE.BND", 96944, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_RopePull, { "PARAMITE.BND", 96972, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_CloseAttack, { "PARAMITE.BND", 97192, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Landing, { "PARAMITE.BND", 97096, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Unused2, { "PARAMITE.BND", 97120, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Knockback, { "PARAMITE.BND", 97136, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_GameSpeakBegin, { "PARAMITE.BND", 17032, 137, 65u, kArjpumpResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_PreHiss, { "PARAMITE.BND", 17072, 137, 65u, kArjpumpResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Hiss, { "PARAMITE.BND", 17088, 137, 65u, kArjpumpResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_AllOYaGameSpeakBegin, { "PARAMITE.BND", 17192, 137, 65u, kArjpumpResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_PostHiss, { "PARAMITE.BND", 17120, 137, 65u, kArjpumpResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_GameSpeakEnd, { "PARAMITE.BND", 17152, 137, 65u, kArjpumpResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_RunningAttack, { "PARAMITE.BND", 19068, 137, 65u, kArjponceResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_SurpriseWeb, { "PARAMITE.BND", 52312, 137, 65u, kArjclimbResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_WebLeaveDown, { "PARAMITE.BND", 52344, 137, 65u, kArjclimbResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_WebIdle, { "PARAMITE.BND", 52476, 137, 65u, kArjclimbResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_WebGoingUp, { "PARAMITE.BND", 52428, 137, 65u, kArjclimbResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_WebGoingDown, { "PARAMITE.BND", 52380, 137, 65u, kArjclimbResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_WebGrab, { "PARAMITE.BND", 52548, 137, 65u, kArjclimbResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_WebLeaveUp, { "PARAMITE.BND", 52588, 137, 65u, kArjclimbResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Eating, { "PARAMITE.BND", 15628, 137, 65u, kArjeatResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Death, { "PARAMITE.BND", 8108, 137, 65u, kArjfalrkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Squawk, { "PARAMITE.BND", 9636, 137, 65u, kArjwaspResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Attack, { "PARAMITE.BND", 10948, 137, 65u, kArjscrchResID, PalId::Default}, kNullAnimDetails },

    {AnimId::Scrab_Idle, { "SCRAB.BND", 224764, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_Walk, { "SCRAB.BND", 224668, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_Run, { "SCRAB.BND", 224796, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_Turn, { "SCRAB.BND", 224860, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_RunToStand, { "SCRAB.BND", 224916, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_HopBegin, { "SCRAB.BND", 224964, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_Jump, { "SCRAB.BND", 224988, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails }, 
    {AnimId::Scrab_Landing, { "SCRAB.BND", 225028, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_JumpAndRunToFall, { "SCRAB.BND", 225112, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_StandToWalk, { "SCRAB.BND", 225052, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_StandToRun, { "SCRAB.BND", 225072, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_WalkToStand, { "SCRAB.BND", 225092, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_WalkToFall, { "SCRAB.BND", 225132, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_WalkToRun, { "SCRAB.BND", 225168, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_RunToWalk, { "SCRAB.BND", 225188, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_Knockback, { "SCRAB.BND", 225208, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_GetEaten, { "SCRAB.BND", 225280, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_Dance, { "SCRAB.BND", 24480, 156, 69, kArsdanceResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_DeathEnd, { "SCRAB.BND", 1340, 156, 69, kArsdeadResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_Empty, { "SCRAB.BND", 11228, 156, 69, kArsgrwlResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_HowlBegin, { "SCRAB.BND", 17260, 156, 69, kArshowlResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_HowlEnd, { "SCRAB.BND", 17328, 156, 69, kArshowlResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_Shriek, { "SCRAB.BND", 11532, 156, 69, kArsroarResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_ScrabBattleAnim, { "SCRAB.BND", 31648, 156, 69, kArswhirlResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_AttackSpin, { "SCRAB.BND", 31704, 156, 69, kArswhirlResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_FeedToGulp, { "SCRAB.BND", 35180, 156, 69, kArschewResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_GulpToStand, { "SCRAB.BND", 35248, 156, 69, kArschewResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_StandToFeed, { "SCRAB.BND", 20840, 156, 69, kArseatResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_Feed, { "SCRAB.BND", 20896, 156, 69, kArseatResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_AttackLunge, { "SCRAB.BND", 8356, 156, 69, kArsprceResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_LegKick, { "SCRAB.BND", 14508, 156, 69, kArsskwrResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_DeathBegin, { "SCRAB.BND", 15600, 156, 69, kArscrshResID, PalId::Default}, kNullAnimDetails },

    { AnimId::Crawling_Slig_Idle, { "CRAWLSLG.BND", 53684, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_UsingButton, { "CRAWLSLG.BND", 53852, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_WakingUp, { "CRAWLSLG.BND", 53964, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_Crawling, { "CRAWLSLG.BND", 53560, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_StartFalling, { "CRAWLSLG.BND", 53720, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_Falling, { "CRAWLSLG.BND", 53736, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_Landing, { "CRAWLSLG.BND", 53752, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_ToShakingToIdle, { "CRAWLSLG.BND", 53780, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_Speaking, { "CRAWLSLG.BND", 53816, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_Snoozing, { "CRAWLSLG.BND", 53928, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_PushingWall, { "CRAWLSLG.BND", 54032, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_TurnAround, { "CRAWLSLG.BND", 53616, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_Shaking, { "CRAWLSLG.BND", 54000, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_Empty, { "CRAWLSLG.BND", 53780, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_ShakingToIdle, { "CRAWLSLG.BND", 54096, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_EndCrawling, { "CRAWLSLG.BND", 54076, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_IdleToPushingWall, { "CRAWLSLG.BND", 54160, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::Crawling_Slig_EndPushingWall, { "CRAWLSLG.BND", 54188, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },

    {AnimId::Slig_Idle, { "SLIG.BND", 135512, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_StandToWalk, { "SLIG.BND", 135804, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Walking, { "SLIG.BND", 135360, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_StandToRun, { "SLIG.BND", 135824, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Running, { "SLIG.BND", 135440, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_TurnAroundStanding, { "SLIG.BND", 136088, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Shoot, { "SLIG.BND", 135544, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_SlidingToStand, { "SLIG.BND", 135696, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_SlidingTurn, { "SLIG.BND", 135576, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_SlidingTurnToWalk, { "SLIG.BND", 135916, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_SlidingTurnToRun, { "SLIG.BND", 135632, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_ReloadGun, { "SLIG.BND", 135652, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_ShootToStand, { "SLIG.BND", 135768, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_SteppingToStand, { "SLIG.BND", 135844, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_StandingToStep, { "SLIG.BND", 135876, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Gamespeak, { "SLIG.BND", 135788, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_WalkToStand, { "SLIG.BND", 135896, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Recoil, { "SLIG.BND", 135936, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Speak1, { "SLIG.BND", 136048, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Speak2, { "SLIG.BND", 135976, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Speak3, { "SLIG.BND", 136132, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Speak4, { "SLIG.BND", 136012, 160, 68, kSlgbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Sleeping, { "SLGSLEEP.BAN", 33552, 160, 68, kSlgsleepResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_SleepingToStand, { "SLGSLEEP.BAN", 33448, 160, 68, kSlgsleepResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Knockeback, { "SLIG.BND", 33184, 160, 68, kSlgknbkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_KnockbackToStand, { "SLIG.BND", 33228, 160, 68, kSlgknbkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_PossessShake, { "SLIG.BND", 33348, 160, 68, kSlgknbkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_OutToFall, { "SLGEDGE.BAN", 30560, 160, 68, kSlgedgeResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_FallingInitiate, { "SLGEDGE.BAN", 30592, 160, 68, kSlgedgeResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_LandingSoft, { "SLGEDGE.BAN", 30628, 160, 68, kSlgedgeResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_LandingFatal, { "SLGSMASH.BAN", 9204, 160, 68, kSlgsmashResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_ShootZ, { "SLIGZ.BND", 9208, 160, 68, kSlgzshotResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_ShootZtoStand, { "SLIGZ.BND", 9260, 160, 68, kSlgzshotResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Smash, { "SLGKNFD.BAN", 13016, 160, 68, kSlgknfdResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_PullLever, { "SLGLEVER.BAN", 12612, 160, 68, kSlgleverResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_LiftGrip, { "SLGLIFT.BAN", 23048, 160, 68, kSlgliftResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_LiftUngrip, { "SLGLIFT.BAN", 23072, 160, 68, kSlgliftResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_LiftGripping, { "SLGLIFT.BAN", 23200, 160, 68, kSlgliftResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_LiftUp, { "SLGLIFT.BAN", 23148, 160, 68, kSlgliftResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_LiftDown, { "SLGLIFT.BAN", 23096, 160, 68, kSlgliftResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slig_Beat, { "SLGBEAT.BAN", 12660, 160, 68, kSlgbeatResID, PalId::Default}, kNullAnimDetails },

    // apparently these both look exactly the same?
    {AnimId::CrawlingSligButton, {"CSLGBUTN.BAN", 408, 25, 12, kCrawlingSligButtonResID_1057, PalId::Default}, kNullAnimDetails},
    {AnimId::CrawlingSligButtonUse, {"CSLGBUTN.BAN", 420, 25, 12, kCrawlingSligButtonResID_1057, PalId::Default}, kNullAnimDetails},

    // main menu gamespeak
    // use normal abespeak resource id?
    {AnimId::MenuAbeSpeak_Laugh, kNullAnimDetails, {"ABESPEAK.BAN", 2115576, 134, 90, kAbespeakResID, PalId::Default}},
    {AnimId::MenuAbeSpeak_WhistleHigh, kNullAnimDetails, {"ABESPEAK.BAN", 2115712, 134, 90, kAbespeakResID, PalId::Default}},
    {AnimId::MenuAbeSpeak_WhistleLow, kNullAnimDetails, {"ABESPEAK.BAN", 2115824, 134, 90, kAbespeakResID, PalId::Default}},
    {AnimId::MenuAbeSpeak_Fart, kNullAnimDetails, {"ABESPEAK.BAN", 2115368, 134, 90, kAbespeakResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_Hello, 
        {"ABESPEK2.BAN", 247620, 233, 125, kAbespek2ResID, PalId::Default},
        {"STARTANM.BND", 201320, 134, 90, kAbespek2ResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_Idle,
        {"ABESPEK2.BAN", 247808, 233, 125, kAbespek2ResID, PalId::Default},
        {"STARTANM.BND", 201508, 134, 90, kAbespek2ResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_IdleBlink,
        {"ABESPEK2.BAN", 247684, 233, 125, kAbespek2ResID, PalId::Default},
        {"STARTANM.BND", 201384, 134, 90, kAbespek2ResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_Ok, 
        {"ABESPEK2.BAN", 247924, 233, 125, kAbespek2ResID, PalId::Default},
        {"STARTANM.BND", 201632, 134, 90, kAbespek2ResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_FollowMe, 
        {"ABESPEAK.BAN", 693672, 233, 125, kAbespeakResID, PalId::Default},
        {"ABESPEAK.BAN", 2115300, 134, 90, kAbespeakResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_Wait,
        {"ABESPEAK.BAN", 693912, 233, 125, kAbespeakResID, PalId::Default},
        {"ABESPEAK.BAN", 2115912, 134, 90, kAbespeakResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_Work, { "ABESPEAK.BAN", 694084, 233, 125, kAbespeakResID, PalId::Default}, kNullAnimDetails },

    { AnimId::MenuAbeSpeak_Anger, // Anger in AE and Angry in AO
        {"ABESPEAK.BAN", 693556, 233, 125, kAbespeakResID, PalId::Default},
        {"ABESPEAK.BAN", 2115180, 134, 90, kAbespeakResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_AllYa, { "ABESPEAK.BAN", 694132, 233, 125, kAbespeakResID, PalId::Default}, kNullAnimDetails },
    { AnimId::MenuAbeSpeak_Sympathy, { "ABESPEAK.BAN", 693956, 233, 125, kAbespeakResID, PalId::Default}, kNullAnimDetails },
    { AnimId::MenuAbeSpeak_StopIt, { "ABESPEAK.BAN", 694200, 233, 125, kAbespeakResID, PalId::Default }, kNullAnimDetails },

    { AnimId::MenuAbeSpeak_Chant,
        { "ABESPEAK.BAN", 693772, 233, 125, kAbespeakResID, PalId::Default },
        {"ABESPEAK.BAN", 2115472, 134, 90, kAbespeakResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_ChantEnd,
        {"ABESPEAK.BAN", 693748, 233, 125, kAbespeakResID, PalId::Default },
        {"ABESPEAK.BAN", 2115452, 134, 90, kAbespeakResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_Goodbye,
        {"ABESPEAK.BAN", 693504, 233, 125, kAbespeakResID, PalId::Default },
        {"ABESPEAK.BAN", 2115120, 134, 90, kAbespeakResID, PalId::Default}},

    { AnimId::MenuGlukkonSpeak_Idle, { "GLKSPEAK.BAN", 1371728, 233, 125, kGlukkonSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuGlukkonSpeak_IdleVariation, { "GLKSPEAK.BAN", 1371876, 233, 125, kGlukkonSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuGlukkonSpeak_Hey, { "GLKSPEAK.BAN", 1371660, 233, 125, kGlukkonSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuGlukkonSpeak_Commere, { "GLKSPEAK.BAN", 1371452, 233, 125, kGlukkonSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuGlukkonSpeak_StayHere, { "GLKSPEAK.BAN", 1372192, 233, 125, kGlukkonSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuGlukkonSpeak_DoIt, { "GLKSPEAK.BAN", 1371520, 233, 125, kGlukkonSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuGlukkonSpeak_KillEm, { "GLKSPEAK.BAN", 1372024, 233, 125, kGlukkonSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuGlukkonSpeak_AllOYa, { "GLKSPEAK.BAN", 1372292, 233, 125, kGlukkonSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuGlukkonSpeak_Help, { "GLKSPEAK.BAN", 1371580, 233, 125, kGlukkonSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuGlukkonSpeak_Laugh, { "GLKSPEAK.BAN", 1372108, 233, 125, kGlukkonSpeakResID, PalId::Default }, kNullAnimDetails },

    { AnimId::MenuScrabSpeak_Idle, { "SCRSPEAK.BAN", 506044, 233, 125, kScrabSpeakResID, PalId::Default}, kNullAnimDetails },
    { AnimId::MenuScrabSpeak_IdleVariation, { "SCRSPEAK.BAN", 506160, 233, 125, kScrabSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuScrabSpeak_ShredPower, { "SCRSPEAK.BAN", 505836, 233, 125, kScrabSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuScrabSpeak_Howl, { "SCRSPEAK.BAN", 505756, 233, 125, kScrabSpeakResID, PalId::Default }, kNullAnimDetails },

    { AnimId::MenuParamiteSpeak_Howdy, { "PARSPEAK.BAN", 888240, 233, 125, kParamiteSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuParamiteSpeak_Idle, { "PARSPEAK.BAN", 888448, 233, 125, kParamiteSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuParamiteSpeak_IdleVariation, { "PARSPEAK.BAN", 888572, 233, 125, kParamiteSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuParamiteSpeak_Cmon, { "PARSPEAK.BAN", 888308, 233, 125, kParamiteSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuParamiteSpeak_Stay, { "PARSPEAK.BAN", 888696, 233, 125, kParamiteSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuParamiteSpeak_DoIt, { "PARSPEAK.BAN", 888824, 233, 125, kParamiteSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuParamiteSpeak_Attack, { "PARSPEAK.BAN", 888936, 233, 125, kParamiteSpeakResID, PalId::Default }, kNullAnimDetails },
    { AnimId::MenuParamiteSpeak_AllAYa, { "PARSPEAK.BAN", 889016, 233, 125, kParamiteSpeakResID, PalId::Default }, kNullAnimDetails },
    
    {AnimId::MenuSligSpeak_Idle, { "SLGSPEAK.BAN", 1105688, 233, 125, kSligSpeakResID, PalId::Default}, kNullAnimDetails },
    {AnimId::MenuSligSpeak_Hi, { "SLGSPEAK.BAN", 1106072, 233, 125, kSligSpeakResID, PalId::Default}, kNullAnimDetails },
    {AnimId::MenuSligSpeak_HereBoy, { "SLGSPEAK.BAN", 1105600, 233, 125, kSligSpeakResID, PalId::Default}, kNullAnimDetails },
    {AnimId::MenuSligSpeak_Freeze, { "SLGSPEAK.BAN", 1105420, 233, 125, kSligSpeakResID, PalId::Default}, kNullAnimDetails },
    {AnimId::MenuSligSpeak_GetEm, { "SLGSPEAK.BAN", 1105516, 233, 125, kSligSpeakResID, PalId::Default}, kNullAnimDetails },
    {AnimId::MenuSligSpeak_SmoBs, { "SLGSPEAK.BAN", 1106020, 233, 125, kSligSpeakResID, PalId::Default}, kNullAnimDetails },
    {AnimId::MenuSligSpeak_Bs, { "SLGSPEAK.BAN", 1105964, 233, 125, kSligSpeakResID, PalId::Default}, kNullAnimDetails },
    {AnimId::MenuSligSpeak_LookOut, { "SLGSPEAK.BAN", 1105884, 233, 125, kSligSpeakResID, PalId::Default}, kNullAnimDetails },
    {AnimId::MenuSligSpeak_Laugh, { "SLGSPEAK.BAN", 1105816, 233, 125, kSligSpeakResID, PalId::Default }, kNullAnimDetails },

    {AnimId::MenuHighlight_Circle,
        {"HIGHLITE.BAN", 13912, 150, 65, kHighliteResID, PalId::Default },
        {"HIGHLITE.BAN", 6152, 99, 43, kHighliteResID, PalId::Default}},

    {AnimId::MenuHighlight_ButtonRemapSquare,
     {"HIGHLITE.BAN", 13936, 150, 65, kHighliteResID, PalId::Default},
     {"HIGHLITE.BAN", 6176, 99, 43, kHighliteResID, PalId::Default}},

    // just guessing the AnimId names
    // probably STARTANIM.BND as well?
    {AnimId::MenuHighlight_Square, { "HIGHLITE.BAN", 13924, 150, 65, kHighliteResID, PalId::Default }, kNullAnimDetails },
    {AnimId::MenuHighlight_Triangle, { "HIGHLITE.BAN", 13900, 150, 65, kHighliteResID, PalId::Default }, kNullAnimDetails },

    {AnimId::MenuDoor, 
        { "STDOOR.BAN", 50212, 233, 125, kMenuDoorResID, PalId::Default },
        { "DOOR.BAN", 41420, 134, 90, kMenuDoorResID, PalId::Default}},

    {AnimId::AbeIntro,
        {"ABEINTRO.BAN", 25888, 233, 125, kAbeintroResID, PalId::Default },
        {"ABEINTRO.BAN", 37364, 134, 90, kAbeintroResID, PalId::Default}},

    {AnimId::Slog_Idle, { "SLOG.BND", 96464, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_Walk, { "SLOG.BND", 96344, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_Run, { "SLOG.BND", 96424, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_TurnAround, { "SLOG.BND", 96764, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_Fall, { "SLOG.BND", 96692, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_MoveHeadUpwards, { "SLOG.BND", 96496, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_StopRunning, { "SLOG.BND", 96532, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_SlideTurn, { "SLOG.BND", 96580, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_StartWalking, { "SLOG.BND", 96640, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_EndWalking, { "SLOG.BND", 96660, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_Land, { "SLOG.BND", 96876, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_Unused, { "SLOG.BND", 96716, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails }, // Just a still frame
    {AnimId::Slog_StartFastBarking, { "SLOG.BND", 96728, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_EndFastBarking, { "SLOG.BND", 96752, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_AngryBark, { "SLOG.BND", 15068, 121, 57, kDogrstnResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_Sleeping, { "SLOG.BND", 15108, 121, 57, kDogrstnResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_MoveHeadDownwards, { "SLOG.BND", 15156, 121, 57, kDogrstnResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_Bark, { "SLOG.BND", 15132, 121, 57, kDogrstnResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_JumpForwards, { "SLOG.BND", 38904, 121, 57, kDogattkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_JumpUpwards, { "SLOG.BND", 38960, 121, 57, kDogattkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_Eating, { "SLOG.BND", 39064, 121, 57, kDogattkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_Dying, { "SLOG.BND", 12412, 121, 57, kDogknfdResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_Scratch, { "SLOG.BND", 12724, 121, 57, kDogidleResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slog_Growl, { "SLOG.BND", 12812, 121, 57, kDogidleResID, PalId::Default}, kNullAnimDetails },

    {AnimId::Slurg_Move, { "SLURG.BAN", 2708, 46, 15, kSlurgResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slurg_Burst, { "SLURG.BAN", 2808, 46, 15, kSlurgResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slurg_Turn_Around, { "SLURG.BAN", 2740, 46, 15, kSlurgResID, PalId::Default}, kNullAnimDetails },

    {AnimId::Background_Glukkon, kNullAnimDetails, { "GLUKKON.BND", 46096, 68, 60, kGlkbasicResID, PalId::Default} },

    {AnimId::BirdPortal_Sparks, {"PORTAL.BND", 4256, 32, 69, kPortliteResID, PalId::Default}, kNullAnimDetails},
    {AnimId::BirdPortal_Flash, {"PORTAL.BND", 13576, 145, 74, kPortlitResID, PalId::Default}, kNullAnimDetails},
    // grow/shrink might be swapped
    {AnimId::BirdPortal_TerminatorGrow, {"PORTAL.BND", 4168, 32, 18, kPortalTerminatorID, PalId::Default}, kNullAnimDetails},
    {AnimId::BirdPortal_TerminatorIdle, {"PORTAL.BND", 4068, 32, 18, kPortalTerminatorID, PalId::Default}, kNullAnimDetails},
    {AnimId::BirdPortal_TerminatorShrink,
        { "PORTAL.BND", 4144, 32, 18, kPortalTerminatorID, PalId::Default},
        { "PORTAL.BND", 3860, 31, 18, kPortalTerminatorID, PalId::Default}
    },

    {AnimId::Dust_Particle, {"DUST.BAN", 4488, 61, 44, kDustResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Loading_Icon, {"LOADING.BAN", 900, 150, 65, kLoadingResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Vaporize_Particle, {"VAPOR.BAN", 5264, 61, 44, kVaporResID, PalId::Default}, kNullAnimDetails},
    {AnimId::ShootingFire_Particle, {"BIGFLASH.BAN", 960, 86, 17, kBigflashResID, PalId::Default}, kNullAnimDetails},
    {AnimId::ChantOrb_Particle, 
        {"OMMFLARE.BAN", 1632, 39, 21, kOmmflareResID, PalId::Default},
        {"OMMFLARE.BAN", 1492, 38, 21, kOmmflareResID, PalId::Default}},
    {AnimId::SquibSmoke_Particle, {"SQBSMK.BAN", 5084, 61, 44, kSquibSmokeResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Explosion_Rocks, {"DEBRIS00.BAN", 6484, 71, 36, kDebrisID00, PalId::Default}, kNullAnimDetails},
    {AnimId::Explosion_Sticks, {"STICK.BAN", 1704, 49, 29, kStickGib, PalId::Default}, kNullAnimDetails},
    {AnimId::Mine_Flash, {"MINE.BND", 400, 36, 8, kMineflshResID, PalId::Default}, kNullAnimDetails},
    {AnimId::OptionChantOrb_Particle,
        {"STARTANIM.BND", 4176, 92, 47, kOptionFlare, PalId::Default},
        {"STARTANIM.BND", 7152, 92, 47, kOptionFlare, PalId::Default}},

    // figure out the correct BAN/BND
    {AnimId::ShootingZFire_Particle, {"STARTANIM.BND", 2832, 126, 44, kZflashResID, PalId::Default}, kNullAnimDetails},

    {AnimId::WaterDrop, {"WDROP.BAN", 104, 11, 7, kWaterDrop, PalId::Default}, kNullAnimDetails},
    {AnimId::WaterSplash, {"SPLASH.BAN", 332, 17, 7, kSplashResID, PalId::Default}, kNullAnimDetails},

    {AnimId::HoistRock1, {"DRPROCK.BAN", 204, 7, 4, kHoistRocks, PalId::Default}, kNullAnimDetails},
    {AnimId::HoistRock2, {"DRPROCK.BAN", 180, 7, 4, kHoistRocks, PalId::Default}, kNullAnimDetails},
    {AnimId::HoistRock3, {"DRPROCK.BAN", 192, 7, 4, kHoistRocks, PalId::Default}, kNullAnimDetails},

    {AnimId::BrewMachine_Button, {"BREWBTN.BAN", 316, 19, 11, kBrewButtonResID_6016, PalId::Default}, kNullAnimDetails},

    {AnimId::PullRingRope_Idle, {"PULLRING.BAN", 3020, 35, 22, kPullringResID, PalId::Default}, kNullAnimDetails},
    {AnimId::PullRingRope_UseBegin, {"PULLRING.BAN", 3060, 35, 22, kPullringResID, PalId::Default}, kNullAnimDetails},
    {AnimId::PullRingRope_UseEnd, {"PULLRING.BAN", 3092, 35, 22, kPullringResID, PalId::Default}, kNullAnimDetails},

    {AnimId::CrawlingSligLocker_Closed, {"LOCKER.BAN", 4352, 37, 70, kCrawlingSligLockerResID_448, PalId::Default}, kNullAnimDetails},
    {AnimId::CrawlingSligLocker_Open, {"LOCKER.BAN", 4364, 37, 70, kCrawlingSligLockerResID_448, PalId::Default}, kNullAnimDetails},

    {AnimId::LiftPlatform_Mines, {"MILIFT.BND", 1936, 136, 31, kLiftResID, PalId::Default}, kNullAnimDetails},
    {AnimId::LiftBottomWheel_Mines, {"MILIFT.BND", 4760, 47, 24, kLiftWheelsResID, PalId::Default}, kNullAnimDetails},
    {AnimId::LiftTopWheel_Mines, {"MILIFT.BND", 4784, 47, 24, kLiftWheelsResID, PalId::Default}, kNullAnimDetails},

    {AnimId::LiftPlatform_Necrum, {"NELIFT.BND", 1312, 135, 17, kLiftResID, PalId::Default}, kNullAnimDetails},
    {AnimId::LiftBottomWheel_Necrum, {"NELIFT.BND", 8664, 69, 34, kLiftWheelsResID, PalId::Default}, kNullAnimDetails},
    {AnimId::LiftTopWheel_Necrum, {"NELIFT.BND", 8700, 69, 34, kLiftWheelsResID, PalId::Default}, kNullAnimDetails},

    // TODO: figure out if this is the correct BAN/BND
    {AnimId::ShrykullStart, {"SHRYPORT.BND", 82676, 123, 79, kShrmorphResID, PalId::Default}, kNullAnimDetails},
    {AnimId::ShrykullTransform, {"SHRYPORT.BND", 82712, 123, 79, kAbemorphResID, PalId::Default}, kNullAnimDetails},
    {AnimId::ShrykullDetransform, {"SHRYPORT.BND", 82824, 123, 79, kAbemorphResID, PalId::Default}, kNullAnimDetails},

    {AnimId::NormalMudIcon, {"EMONORM.BAN", 1248, 54, 47, kNormaliconResID, PalId::Default}, kNullAnimDetails},
    {AnimId::AngryMudIcon, {"EMOANGRY.BAN", 1076, 54, 47, kAngryiconResID, PalId::Default}, kNullAnimDetails},
    {AnimId::HappyMudIcon, {"EMOHAP.BAN", 1252, 54, 47, kHappyiconResID, PalId::Default}, kNullAnimDetails},

    {AnimId::SpotLight, {"BLOODROP.BAN", 1536, 57, 32, kSpotliteResID, PalId::Default}, kNullAnimDetails},
        
    // blood drop and blood are not the same animations.
    // TODO: check if AO blood.ban is blood or blood drop
    {AnimId::BloodDrop, {"BLOODROP.BAN", 308, 11, 7, kBloodropResID, PalId::Default}, kNullAnimDetails},

    {AnimId::Blood, kNullAnimDetails, { "BLOOD.BAN", 276, 6, 7, kBloodropResID, PalId::Default} },
    {AnimId::Bone, { "BTHROW.BND", 456, 15, 9, kBoneResID, PalId::Default}, kNullAnimDetails },
    {AnimId::BoneBag_Idle, { "BONEBAG.BAN", 8748, 64, 60, kBoneBagResID_590, PalId::Default}, kNullAnimDetails },
    {AnimId::BoneBag_HardHit, { "BONEBAG.BAN", 8708, 64, 60, kBoneBagResID_590, PalId::Default}, kNullAnimDetails },
    {AnimId::BoneBag_SoftHit, { "BONEBAG.BAN", 8788, 64, 60, kBoneBagResID_590, PalId::Default}, kNullAnimDetails },
    {AnimId::Drill_Horizontal_Off, { "DRILL.BAN", 6736, 65, 33, kDrillResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Drill_Horizontal_On, { "DRILL.BAN", 6712, 65, 33, kDrillResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Drill_Vertical_Off, { "DRILL.BAN", 6676, 65, 33, kDrillResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Drill_Vertical_On, { "DRILL.BAN", 6688, 65, 33, kDrillResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Bullet_Shell, 
        { "SHELL.BAN", 320, 5, 5u, kShellResID, PalId::Default},
        { "SHELL.BAN", 308, 6, 5, kShellResID, PalId::Default} 
    },
    {AnimId::Chime_A, kNullAnimDetails, { "CHIMES.BAN", 19240, 49, 46, kChimeResID, PalId::Default} },
    {AnimId::Chime_B, kNullAnimDetails, { "CHIMES.BAN", 19368, 49, 46, kChimeResID, PalId::Default} },
    {AnimId::Chime_C, kNullAnimDetails, { "CHIMES.BAN", 19252, 49, 46, kChimeResID, PalId::Default} },
    {AnimId::Chime_Ball, kNullAnimDetails, { "F2CHMBAL.BAN", 136, 16, 8, kF2ChimeBallResID, PalId::Default} },
    {AnimId::Circular_Fade, kNullAnimDetails, { "SPOTLITE.BAN", 2472, 57, 32, kSpotliteResID, PalId::Default} },
    {AnimId::DeathFlare_1,
        { "MFLARE.BAN", 9940, 122, 43, kDeathFlareResID, PalId::Default},
        { "MFLARE.BAN", 6312, 70, 43, kDeathFlareResID, PalId::Default}
    },
    {AnimId::DeathFlare_2,
        { "MFLARE.BAN", 9912, 122, 43, kDeathFlareResID, PalId::Default},
        { "MFLARE.BAN", 6284, 70, 43, kDeathFlareResID, PalId::Default} },

    {AnimId::Door_BarracksMetal_Closed, {"SHDOOR.BAN", 6604, 51, 62, kF2p3dorResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Door_BarracksMetal_Open, {"SHDOOR.BAN", 6616, 51, 62, kF2p3dorResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Door_Barracks_Closed, { "BADOOR.BAN", 5652, 56, 62, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Barracks_Open, { "BADOOR.BAN", 5664, 56, 62, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Bonewerkz_Closed, { "BWDOOR.BAN", 6304, 59, 58, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Bonewerkz_Open, { "BWDOOR.BAN", 6316, 59, 58, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Brewery_Closed, { "BRDOOR.BAN", 6224, 54, 71, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Brewery_Open, { "BRDOOR.BAN", 6236, 54, 71, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Feeco_Closed, { "FDDOOR.BAN", 7636, 62, 70, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Feeco_Open, { "FDDOOR.BAN", 7648, 62, 70, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Flame, 
        { "FIRE.BAN", 5156, 22, 21u, kHubFireResID, PalId::Default},
        { "FIRE.BAN", 5072, 51, 24, kHubFireResID, PalId::Default}
    },
    {AnimId::Door_Flame_Glow,
        { "FIRE.BAN", 1400, 52, 30, kGlowResID, PalId::Default},
        { "FIRE.BAN", 1344, 52, 30, kGlowResID, PalId::Default} 
    },
    {AnimId::Door_Flame_Spark, 
        { "FIRE.BAN", 1672, 39, 21, kOmmflareResID, PalId::Default},
        { "FIRE.BAN", 1532, 38, 21, kOmmflareResID, PalId::Default} 
    },
    {AnimId::Door_Lock_Idle, { "DOORLOCK.BAN", 1624, 87, 14, kDoorLockResID_374, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Lock_Open, { "DOORLOCK.BAN", 1608, 87, 14, kDoorLockResID_374, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Mines_Closed, { "DOOR.BAN", 8692, 77, 69, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Mines_Open, { "DOOR.BAN", 8704, 77, 69, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Temple_Closed, { "SVZDOOR.BAN", 5436, 67, 62, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Temple_Open, { "SVZDOOR.BAN", 5448, 67, 62, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Train_Closed, { "TRAINDOR.BAN", 4740, 44, 56u, kTrainDoorResID_2013, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Train_Closing, { "TRAINDOR.BAN", 4752, 44, 56u, kTrainDoorResID_2013, PalId::Default}, kNullAnimDetails },

    {AnimId::Dove_Idle, {"DOVE.BAN", 5580, 41, 20, kDovbasicResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Dove_Flying, {"DOVE.BAN", 5516, 41, 20, kDovbasicResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Electric_Wall,
        { "ELECWALL.BAN", 15384, 50, 80, kElecwallResID, PalId::Default},
        { "ELECWALL.BAN", 14536, 50, 80, kElecwallResID, PalId::Default} 
    },
    {AnimId::Explosion, 
        { "EXPLO2.BAN", 51156, 202, 91, kExplo2ResID, PalId::Default},
        { "EXPLO2.BAN", 27376, 200, 91, kExplo2ResID, PalId::Default} 
    },
    {AnimId::Explosion_Mine, 
        { "EXPLODE.BND", 51588, 214, 0x31u, kBgexpldResID, PalId::Default},
        { "EXPLODE.BND", 51600, 214, 49, kBgexpldResID, PalId::Default}
    },
    {AnimId::Explosion_Small, { "SMEXP.BAN", 14108, 99, 46, kSmallExplo2ResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Falling_Crate_A, { "FALLBONZ.BAN", 8076, 86, 47, kF2rockResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Falling_Crate_B, { "FALLBONZ.BAN", 8100, 86, 47, kF2rockResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Falling_Rock_A, { "FALLROCK.BAN", 4232, 64, 28, kF2rockResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Falling_Rock_B, { "FALLROCK.BAN", 4256, 64, 28, kF2rockResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Fart, { "EVILFART.BAN", 3532, 61, 39, kEvilFartResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Foot_Switch_Bonewerkz_Idle, { "BWTRIGGR.BAN", 548, 46, 11, kTriggerResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Foot_Switch_Bonewerkz_Pressed, { "BWTRIGGR.BAN", 568, 46, 11, kTriggerResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Foot_Switch_Industrial_Idle, { "TRIGGER.BAN", 568, 45, 11, kTriggerResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Foot_Switch_Industrial_Pressed, { "TRIGGER.BAN", 588, 45, 11, kTriggerResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Foot_Switch_Temple, kNullAnimDetails, { "TRIGGER.BAN", 744, 68, 15, kTriggerResID, PalId::Default} },
    {AnimId::Foot_Switch_Vault_Idle, { "VLTTRIGR.BAN", 972, 72, 18, kTriggerResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Foot_Switch_Vault_Pressed, { "VLTTRIGR.BAN", 992, 72, 18, kTriggerResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Grenade, 
        { "GRENADE.BAN", 1252, 17, 11, kGrenadeResID, PalId::Default},
        { "GRENADE.BAN", 1132, 17, 11, kGrenadeResID, PalId::Default}
    },
    {AnimId::Grenade_Machine_Button_Off, { "GMACHINE.BND1", 1736, 26, 17u, kR1buttonResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Grenade_Machine_Button_On, 
        { "GMACHINE.BND1", 1708, 26, 17u, kR1buttonResID, PalId::Default},
        { "GMACHINE.BND1", 1600, 26, 17, kR1buttonResID, PalId::Default} 
    },
    {AnimId::Grenade_Machine_Nozzle,
        { "GMACHINE.BND1", 3700, 67, 36u, kR1bpipeResID, PalId::Default},
        { "GMACHINE.BND1", 3616, 66, 36, kR1bpipeResID, PalId::Default}
    },
    {AnimId::Grenade_Machine_Nozzle_Drop_Grenade, { "GMACHINE.BND1", 3672, 67, 36u, kR1bpipeResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Hint_Fly, kNullAnimDetails, { "HINTFLY.BAN", 556, 10, 7, kHintflyResID, PalId::Default} },
    {AnimId::Honey, kNullAnimDetails, { "HONEY.BAN", 3680, 62, 126, kHoneyResID, PalId::Default} },
    {AnimId::Honey_Drip, kNullAnimDetails, { "HONEY.BAN", 3628, 62, 126, kHoneyResID, PalId::Default} },
    {AnimId::Honey_Hive, kNullAnimDetails, { "HIVE.BND", 9312, 140, 38, kP2c3HiveResID, PalId::Default} },
    {AnimId::Lift_Rope, { "ROPES.BAN", 748, 9, 16, kRopesResID, PalId::Default}, kNullAnimDetails },
    // TODO: more rope BAN'S?

    {AnimId::Meat, 
        { "MEAT.BAN", 488, 17, 9, kMeatResID, PalId::Default},
        { "MEAT.BAN", 500, 18, 9, kMeatResID, PalId::Default}
    },
    {AnimId::MeatSack_Idle,
        { "MEATSACK.BAN", 15848, 93, 86, kMeatSackResID, PalId::Default},
        { "D2ELUM.BAN", 15688, 92, 86, kMeatSackResID, PalId::Default}
    },
    {AnimId::MeatSack_Hit, {"MEATSACK.BAN", 15888, 93, 86, kMeatSackResID, PalId::Default}, kNullAnimDetails},
    {AnimId::MeatSaw, kNullAnimDetails, { "R1METSAW.BAN", 15200, 104, 36, kMeatSawResID, PalId::Default} },

    {AnimId::Mine, 
        { "MINE.BND", 784, 38, 13, kLandmineResID, PalId::Default },
        { "MINE.BND", 748, 38, 13, kLandmineResID, PalId::Default }
    },

    {AnimId::MotionDetector_Flare, 
        { "MFLARE.BAN", 1736, 55, 22, kMflareResID, PalId::Default},
        { "MFLARE.BAN", 1108, 32, 22, kMflareResID, PalId::Default}
    },
    {AnimId::MotionDetector_Laser, 
        { "MOTION.BAN", 23280, 37, 60, kMotionResID, PalId::Default},
        { "MOTION.BAN", 23660, 37, 60, kMotionResID, PalId::Default}
    },

    {AnimId::MovingBomb, 
        { "MOVEBOMB.BAN", 17548, 76, 30, kMovebombResID, PalId::Default},
        { "MOVEBOMB.BAN", 17140, 76, 30, kMovebombResID, PalId::Default}
    },

    {AnimId::ParamiteWeb, 
        { "WEB.BAN", 148, 5, 16, kWebResID, PalId::Default},
        { "WEB.BAN", 88, 3, 16, kWebResID, PalId::Default}
    },
    {AnimId::Pullring_Desert, kNullAnimDetails, { "PULLRING.BAN", 2912, 32, 22, kPullringResID, PalId::Default} },
    {AnimId::Pullring_Farms, kNullAnimDetails, { "R1PULRNG.BAN", 4832, 33, 35, kPullringResID, PalId::Default} },

    {AnimId::Rock, 
        { "RTHROW.BND", 488, 17, 9, kAberockResID, PalId::Default},
        { "RTHROW.BND", 500, 18, 9, kAberockResID, PalId::Default}
    },
    {AnimId::RockSack_Idle, 
        { "ROCKBAG.BAN", 29748, 71, 60u, kP2c2bagResID, PalId::Default},
        { "E1BAG.BAN", 13756, 71, 60, kP2c2bagResID, PalId::Default}
    },
    {AnimId::RockSack_SoftHit, 
        { "ROCKBAG.BAN", 29772, 71, 60u, kP2c2bagResID, PalId::Default},
        { "E1BAG.BAN", 13780, 71, 60, kP2c2bagResID, PalId::Default} },
    {AnimId::RockSack_HardHit,
        { "ROCKBAG.BAN", 29700, 71, 60u, kP2c2bagResID, PalId::Default},
        { "E1BAG.BAN", 13708, 71, 60, kP2c2bagResID, PalId::Default} },

    {AnimId::Rope_R1, kNullAnimDetails, { "R1ROPES.BAN", 636, 4, 16, kRopesResID, PalId::Default} },
    {AnimId::Rope_Lines, kNullAnimDetails, { "D1ROPES.BAN", 680, 5, 16, kRopesResID, PalId::Default} },

    {AnimId::Security_Claw_Lower, kNullAnimDetails, { "F2EYEORB.BAN", 22468, 152, 31, kF2eyeorbResID, PalId::Default} },
    {AnimId::Security_Claw_Upper, kNullAnimDetails, { "F2EYEORB.BAN", 22480, 152, 31, kF2eyeorbResID, PalId::Default} },
    {AnimId::Security_Door, kNullAnimDetails, { "R1SDOS.BAN", 976, 70, 19, kR1sdosResID_6027, PalId::Default} },
    {AnimId::Security_Door_Idle, { "SECDOOR.BAN", 1512, 70, 19u, kR1sdosResID_6027, PalId::Default}, kNullAnimDetails },
    {AnimId::Security_Door_Speak, { "SECDOOR.BAN", 1528, 70, 19u, kR1sdosResID_6027, PalId::Default}, kNullAnimDetails },
    {AnimId::Security_Orb, 
        { "MAIMORB.BAN", 2228, 53, 28u, kMaimGameResID, PalId::Default},
        { "F2MAMORB.BAN", 10864, 53, 28, kMaimGameResID, PalId::Default}
    },
    {AnimId::Slam_Door_Industrial_Closed, { "SLAM.BAN", 2688, 29, 68, kSlamResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slam_Door_Industrial_Closing, { "SLAM.BAN", 2672, 29, 68, kSlamResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slam_Door_Industrial_Opening, { "SLAM.BAN", 2640, 29, 68, kSlamResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slam_Door_Vault_Closed, { "SLAMVLTS.BAN", 2384, 32, 68, kSlamResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slam_Door_Vault_Closing, { "SLAMVLTS.BAN", 2368, 32, 68, kSlamResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slam_Door_Vault_Opening, { "SLAMVLTS.BAN", 2396, 32, 68, kSlamResID, PalId::Default}, kNullAnimDetails },

    {AnimId::SlapLock_Initiate, { "GHOSTTRP.BAN", 7068, 58, 44, kGhostTrpResID_1053, PalId::Default}, kNullAnimDetails },
    {AnimId::SlapLock_Punched, { "GHOSTTRP.BAN", 7056, 58, 44, kGhostTrpResID_1053, PalId::Default}, kNullAnimDetails },
    {AnimId::SlapLock_Shaking, { "GHOSTTRP.BAN", 6976, 58, 44, kGhostTrpResID_1053, PalId::Default}, kNullAnimDetails },

    {AnimId::Sparks, { "SPARKS.BAN", 808, 19, 11, kSparksResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Star, kNullAnimDetails, { "STAR.BAN", 748, 20, 12, kBGStarResID, PalId::Default} },

    {AnimId::Status_Light_Green, { "STATUSLT.BAN", 240, 14, 9u, kStatusLightResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Status_Light_Red, { "STATUSLT.BAN", 252, 14, 9u, kStatusLightResID, PalId::Default}, kNullAnimDetails },

    {AnimId::Stone_Ball, kNullAnimDetails, { "F2STNBAL.BAN", 15596, 131, 64, kF2stnbalResID, PalId::Default} },
    {AnimId::Stone_Ball_Stopper, kNullAnimDetails, { "F2STNSCK.BAN", 1256, 106, 73, kF2stnsckResID, PalId::Default} },
    {AnimId::Swinging_Ball_Fast, kNullAnimDetails, { "F2ZBALL.BAN", 72288, 143, 182, kF2zballResID, PalId::Default} },
    {AnimId::Swinging_Ball_Normal, kNullAnimDetails, { "F2ZBALL.BAN", 72172, 143, 182, kF2zballResID, PalId::Default} },
    {AnimId::Swinging_Ball_Slow, kNullAnimDetails, { "F2ZBALL.BAN", 72400, 143, 182, kF2zballResID, PalId::Default} },
    {AnimId::Switch_Idle, { "SWITCH1.BAN", 5696, 66, 40, kSwitchResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Switch_Pull_Left_A, { "SWITCH1.BAN", 5708, 66, 40, kSwitchResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Switch_Pull_Left_B, { "SWITCH1.BAN", 5760, 66, 40, kSwitchResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Switch_Pull_Right_A, { "SWITCH1.BAN", 5796, 66, 40, kSwitchResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Switch_Pull_Right_B, { "SWITCH1.BAN", 5848, 66, 40, kSwitchResID, PalId::Default}, kNullAnimDetails },
    {AnimId::TimedMine_Activated, { "BOMB.BND", 848, 35, 17, kBombResID, PalId::Default}, kNullAnimDetails },
    {AnimId::TimedMine_Idle, 
        { "BOMB.BND", 836, 35, 17, kBombResID, PalId::Default},
        { "BOMB.BND", 792, 35, 17, kBombResID, PalId::Default}
    },
    {AnimId::Tortured_Mudokon, { "MUDTORT.BAN", 20892, 101, 0x44u, kTorturedMud, PalId::Default}, kNullAnimDetails },
    {AnimId::Tortured_Mudokon_Released, { "MUDTORT.BAN", 20864, 101, 0x44u, kTorturedMud, PalId::Default}, kNullAnimDetails },
    {AnimId::Tortured_Mudokon_Zap, { "MUDTORT.BAN", 21000, 101, 0x44u, kTorturedMud, PalId::Default}, kNullAnimDetails },

    { AnimId::Trap_Door_Open, { "TRAPDOOR.BAN", 4800, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Trap_Door_Closed, { "TRAPDOOR.BAN", 4788, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Trap_Door_Closing, { "TRAPDOOR.BAN", 4812, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Trap_Door_Opening, { "TRAPDOOR.BAN", 4840, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Trap_Door_Tribal_Closed, { "VLTSTRAP.BAN", 5328, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Trap_Door_Tribal_Closing, { "VLTSTRAP.BAN", 5368, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Trap_Door_Tribal_Open, { "VLTSTRAP.BAN", 5316, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Trap_Door_Tribal_Opening, { "VLTSTRAP.BAN", 5340, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },

    {AnimId::R1_TrapDoor_Open, kNullAnimDetails, { "R1TRAP.BAN", 4700, 72, 41, kP6c1trapResID, PalId::Default} },
    {AnimId::R1_TrapDoor_Closed, kNullAnimDetails, { "R1TRAP.BAN", 4688, 72, 41, kP6c1trapResID, PalId::Default} },
    {AnimId::R1_TrapDoor_Opening, kNullAnimDetails, { "R1TRAP.BAN", 4740, 72, 41, kP6c1trapResID, PalId::Default} },
    {AnimId::R1_TrapDoor_Closing, kNullAnimDetails, { "R1TRAP.BAN", 4712, 72, 41, kP6c1trapResID, PalId::Default} },
    {AnimId::Lines_TrapDoor_Open, kNullAnimDetails, { "P6C1TRAP.BAN", 4644, 155, 59, kP6c1trapResID, PalId::Default} },
    {AnimId::Lines_TrapDoor_Closed, kNullAnimDetails, { "P6C1TRAP.BAN", 4632, 155, 59, kP6c1trapResID, PalId::Default} },
    {AnimId::Lines_TrapDoor_Opening, kNullAnimDetails, { "P6C1TRAP.BAN", 4680, 155, 59, kP6c1trapResID, PalId::Default} },
    {AnimId::Lines_TrapDoor_Closing, kNullAnimDetails, { "P6C1TRAP.BAN", 4656, 155, 59, kP6c1trapResID, PalId::Default} },
    {AnimId::Desert_TrapDoor_Open, kNullAnimDetails, { "D2TRAP.BAN", 1976, 99, 56, kP6c1trapResID, PalId::Default} },
    {AnimId::Desert_TrapDoor_Closed, kNullAnimDetails, { "D2TRAP.BAN", 1964, 99, 56, kP6c1trapResID, PalId::Default} },
    {AnimId::Desert_TrapDoor_Opening, kNullAnimDetails, { "D2TRAP.BAN", 2012, 99, 56, kP6c1trapResID, PalId::Default} },
    {AnimId::Desert_TrapDoor_Closing, kNullAnimDetails, { "D2TRAP.BAN", 1988, 99, 56, kP6c1trapResID, PalId::Default} },

    {AnimId::Bomb_Flash, {"BOMB.BND", 556, 36, 21, kBombflshResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Bomb_RedGreenTick, {"BOMB.BND", 544, 36, 21, kBombflshResID, PalId::Default}, kNullAnimDetails},

    {AnimId::UXB_Active,
        { "UXB.BND", 8048, 59, 19, kUXBResID, PalId::Default},
        { "UXB.BND", 7740, 59, 19, kUXBResID, PalId::Default}
    },
    {AnimId::UXB_Toggle, { "UXB.BND", 8120, 59, 19, kUXBResID, PalId::Default}, kNullAnimDetails },
    {AnimId::UXB_Disabled, { "UXB.BND", 8192, 59, 19, kUXBResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Well_Leaf, { "WELLLEAF.BAN", 476, 16, 11, kWellLeafResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Work_Wheel_Idle, { "WORKWHEL.BAN", 2716, 49, 24, kWorkwhelResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Work_Wheel_Turning, { "WORKWHEL.BAN", 2728, 49, 24, kWorkwhelResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Zap_Line_Blue, { "SPLINE.BAN", 276, 14, 9, kSplineResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Zap_Line_Red, { "SPLINE.BAN", 288, 14, 9, kSplineResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Zap_Sparks, kNullAnimDetails, { "OMMFLARE.BAN", 1532, 38, 21, kOmmflareResID, PalId::Default} },

    // Background animations
    {AnimId::BG_Brewery_Barrel_Anim, { "BRP02C10.CAM", 42112, 130, 55, 1206, PalId::Default}, kNullAnimDetails },
    {AnimId::BG_Mine_Fan, { "MINEFAN.BAN", 15632, 113, 55, 1201, PalId::Default}, kNullAnimDetails },
    {AnimId::BG_Feeco_Small_Fan, { "FDP01C01.CAM", 4208, 57, 28, 1202, PalId::Default}, kNullAnimDetails },
    {AnimId::BG_Heat_Extractor_Fan, { "MIP04C18.CAM", 7104, 89, 24, 1204, PalId::Default}, kNullAnimDetails },


    {AnimId::GoldGlow, kNullAnimDetails, {"GOLDGLOW.BAN", 236, 19, 15, kGoldGlowResID_6011, PalId::Default}},
    {AnimId::GreenGlow, kNullAnimDetails, {"GRENGLOW.BAN", 96, 5, 6, kGreenGlowResID_6010, PalId::Default}},
    {AnimId::FlintGlow, kNullAnimDetails, {"FLNT.BAN", 756, 59, 54, kFlintGlowResID_6028, PalId::Default}},

    {AnimId::GreenDoorLight, kNullAnimDetails, {"HUBLIGHT.BAN", 448, 20, 11, kGreenDoorLightResID_6031, PalId::Default}},
    {AnimId::GreenHubLight, kNullAnimDetails, {"HUBLIGHT.BAN", 460, 20, 11, kGreenDoorLightResID_6031, PalId::Default}},

    {AnimId::RedDoorLight, kNullAnimDetails, {"HUBRED.BAN", 460, 20, 11, kRedDoorLightResID_6032, PalId::Default}},
    {AnimId::RedHubLight, kNullAnimDetails, {"HUBRED.BAN", 448, 20, 11, kRedDoorLightResID_6032, PalId::Default}},


    // search for "Animation test code" in Abe.cpp and uncomment the code below to activate the anim tester
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    {AnimId::Anim_Tester,
     {"PORTAL.BND", 4144, 32, 18, kPortalTerminatorID, PalId::Default},
    { "WASP.BAN", 636, 7, 4, kWaspResID, PalId::Default}
    },
    ////////////////////////////////////////////////////////////////////////////////////////////////////
};

void FrameTableOffsetExists(int frameTableOffset, bool isAe)
{
    for (const auto& entry : kAnimRecords)
    {
        if (isAe)
        {
            if (entry.mAEData.mFrameTableOffset == frameTableOffset)
            {
                return;
            }
        }
        else
        {
            if (entry.mAOData.mFrameTableOffset == frameTableOffset)
            {
                return;
            }
        }
    }
    LOG_INFO("couldn't find AnimId for framtableoffset: " << frameTableOffset);
}

static const PalRecord PalRec(bool isAe, PalId toFind)
{
    for (const CombinedPalRecord& pal : kPalRecords)
    {
        if (pal.mId == toFind)
        {
            const PalDetails& data = isAe ? pal.mAEData : pal.mAOData;
            return PalRecord{pal.mId, data.mBanName, data.mResourceId };
        }
    }
    ALIVE_FATAL("Missing pal entry");
}


const PalRecord PalRec(PalId toFind)
{
    return PalRec(true, toFind);
}

static const AnimRecord AnimRec(bool isAe, AnimId toFind)
{
    for (const CombinedAnimRecord& anim : kAnimRecords)
    {
        if (anim.mId == toFind)
        {
            const AnimDetails& data = isAe ? anim.mAEData : anim.mAOData;
            return AnimRecord{ anim.mId, data.mBanName, data.mFrameTableOffset, data.mMaxW, data.mMaxH, data.mResourceId, data.mPalOverride };
        }
    }
    ALIVE_FATAL("Missing animation entry");
}

const AnimRecord AnimRec(AnimId toFind)
{
    return AnimRec(true, toFind);
}

namespace AO
{
    const PalRecord PalRec(PalId toFind)
    {
        return PalRec(false, toFind);
    }

    const AnimRecord AnimRec(AnimId toFind)
    {
        return AnimRec(false, toFind);
    }
}
