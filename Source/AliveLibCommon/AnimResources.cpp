#include "AnimResources.hpp"
#include "logger.hpp"

[[noreturn]] void ALIVE_FATAL(const char_type* msg);

constexpr PalDetails kNullPalDetails = {};
constexpr CombinedPalRecord kPalRecords[] = {
    {PalId::BlindMud, {"MUDPAL.BND", kMudblindResID}, kNullPalDetails },
};

constexpr BgAnimDetails kNullBgAnimDetails = {};
constexpr CombinedBgAnimRecord kBgAnimRecords[] = {

    {AEBgAnimID::BG_Brewery_Barrel1, {"BRP02C10.CAM", 42112, 130, 55}, kNullBgAnimDetails},
    {AEBgAnimID::BG_Brewery_Barrel2, {"BRP11C02.CAM", 42112, 130, 55}, kNullBgAnimDetails},
    {AEBgAnimID::BG_Mine_Fan, {"MINEFAN.BAN", 15632, 113, 55}, kNullBgAnimDetails},
    {AEBgAnimID::BG_Feeco_Small_Fan, {"FDP01C01.CAM", 4208, 57, 28}, kNullBgAnimDetails},
    {AEBgAnimID::BG_Heat_Extractor_Fan, {"MIP04C18.CAM", 7104, 89, 24, }, kNullBgAnimDetails},

    {AOBgAnimID::BG_None, kNullBgAnimDetails, kNullBgAnimDetails},
    {AOBgAnimID::BG_RuptureFarms_Barrel1, kNullBgAnimDetails, {"R1P15C07.CAM", 24764, 155, 56, }},
    {AOBgAnimID::BG_RuptureFarms_Barrel2, kNullBgAnimDetails, {"R1P15C08.CAM", 28752, 155, 68}},
    {AOBgAnimID::BG_RuptureFarms_Barrel3, kNullBgAnimDetails, {"R1P18C01.CAM", 8056, 119, 30}},
    {AOBgAnimID::BG_RuptureFarms_Barrel4, kNullBgAnimDetails, {"R1P18C01.CAM", 15372, 89, 38}},
    {AOBgAnimID::BG_RuptureFarms_Barrel5, kNullBgAnimDetails, {"R1P18C02.CAM", 19416, 109, 40}},
    {AOBgAnimID::BG_RuptureFarms_Barrel6, kNullBgAnimDetails, {"R1P18C03.CAM", 6280, 82, 34}},
    {AOBgAnimID::BG_RuptureFarms_Barrel7, kNullBgAnimDetails, {"R1P18C03.CAM", 14904, 66, 57}},
    {AOBgAnimID::BG_RuptureFarms_Barrel8, kNullBgAnimDetails, {"R1P18C05.CAM", 12052, 72, 40}},
    {AOBgAnimID::BG_RuptureFarms_Barrel9, kNullBgAnimDetails, {"R1P18C10.CAM", 8024, 85, 37}},
    {AOBgAnimID::BG_RuptureFarms_Barrel10, kNullBgAnimDetails, {"R1P18C06.CAM", 18356, 107, 39}},
    {AOBgAnimID::BG_RuptureFarms_Barrel11, kNullBgAnimDetails, {"R1P18C07.CAM", 17960, 138, 42}},

    {AOBgAnimID::BG_Campfire1, kNullBgAnimDetails, {"E1P01C01.CAM", 38960, 151, 76}},
    {AOBgAnimID::BG_Campfire2, kNullBgAnimDetails, {"E1P01C07.CAM", 24048, 81, 38}},
    {AOBgAnimID::BG_Campfire3, kNullBgAnimDetails, {"E1P02C04.CAM", 28044, 76, 48}},
    {AOBgAnimID::BG_Campfire4, kNullBgAnimDetails, {"E1P03C02.CAM", 19848, 76, 51}},
    {AOBgAnimID::BG_Campfire5, kNullBgAnimDetails, {"E1P03C06.CAM", 30868, 101, 56}},
    {AOBgAnimID::BG_Campfire6, kNullBgAnimDetails, {"E1P04C01.CAM", 23020, 81, 46}},
    {AOBgAnimID::BG_Campfire7, kNullBgAnimDetails, {"E1P04C05.CAM", 35972, 101, 61}},
    {AOBgAnimID::BG_Campfire8, kNullBgAnimDetails, {"E1P04C07.CAM", 15920, 81, 38}},
    {AOBgAnimID::BG_Campfire9, kNullBgAnimDetails, {"E1P02C02.CAM", 19132, 86, 51}},

    {AOBgAnimID::BG_Well1, kNullBgAnimDetails, {"L1P01C10.CAM", 6528, 87, 82}},
    {AOBgAnimID::BG_Well2, kNullBgAnimDetails, {"L1P01C16.CAM", 3820, 87, 73}},
    {AOBgAnimID::BG_Well3, kNullBgAnimDetails, {"L1P01C16.CAM", 3908, 89, 72}},
    {AOBgAnimID::BG_Well4, kNullBgAnimDetails, {"L1P01C11.CAM", 3464, 83, 64}},
    {AOBgAnimID::BG_Well5, kNullBgAnimDetails, {"L1P02C10.CAM", 6020, 85, 90}},
    {AOBgAnimID::BG_Well6, kNullBgAnimDetails, {"L1P02C09.CAM", 6716, 93, 87}},
    {AOBgAnimID::BG_Well7, kNullBgAnimDetails, {"L1P06C05.CAM", 2712, 59, 53}},
    {AOBgAnimID::BG_Well8, kNullBgAnimDetails, {"L1P06C05.CAM", 2308, 49, 52}},
    {AOBgAnimID::BG_Well9, kNullBgAnimDetails, {"D1P01C01.CAM", 27004, 116, 58}},
    {AOBgAnimID::BG_Well10, kNullBgAnimDetails, {"D1P01C01.CAM", 3604, 88, 53}},
    {AOBgAnimID::BG_Well11, kNullBgAnimDetails, {"D1P01C01.CAM", 1616, 39, 45}},
    {AOBgAnimID::BG_Well12, kNullBgAnimDetails, {"D1P01C02.CAM", 2384, 67, 58}},
    {AOBgAnimID::BG_Well13, kNullBgAnimDetails, {"D1P01C03.CAM", 1628, 27, 52}},
    {AOBgAnimID::BG_Well14, kNullBgAnimDetails, {"D1P06C09.CAM", 2336, 57, 46}},
    {AOBgAnimID::BG_Well15, kNullBgAnimDetails, {"D1P06C08.CAM", 2468, 74, 58}},
    {AOBgAnimID::BG_Well16, kNullBgAnimDetails, {"D1P06C08.CAM", 2304, 69, 54}},
    {AOBgAnimID::BG_Well17, kNullBgAnimDetails, {"D1P06C01.CAM", 3212, 79, 49}},
    {AOBgAnimID::BG_Well18, kNullBgAnimDetails, {"D1P06C02.CAM", 3272, 78, 58}},
    {AOBgAnimID::BG_Well19, kNullBgAnimDetails, {"D1P08C01.CAM", 3380, 83, 70}},
    {AOBgAnimID::BG_Well20, kNullBgAnimDetails, {"D1P08C01.CAM", 1416, 29, 41}},
    {AOBgAnimID::BG_Well21, kNullBgAnimDetails, {"D1P08C03.CAM", 3084, 85, 59}},
    {AOBgAnimID::BG_Well22, kNullBgAnimDetails, {"D1P09C02.CAM", 3028, 82, 54}},
    {AOBgAnimID::BG_Well23, kNullBgAnimDetails, {"D1P09C02.CAM", 1184, 27, 39}},
    {AOBgAnimID::BG_Well24, kNullBgAnimDetails, {"D1P09C06.CAM", 1600, 32, 46}},
    {AOBgAnimID::BG_Well25, kNullBgAnimDetails, {"D1P09C06.CAM", 2876, 78, 56}},
    {AOBgAnimID::BG_Well26, kNullBgAnimDetails, {"D2P10C01.CAM", 1284, 32, 45}},
    {AOBgAnimID::BG_Well27, kNullBgAnimDetails, {"D2P10C01.CAM", 1612, 30, 52}},
    {AOBgAnimID::BG_Well28, kNullBgAnimDetails, {"D2P06C03.CAM", 1064, 24, 46}},
    {AOBgAnimID::BG_Well29, kNullBgAnimDetails, {"D2P06C07.CAM", 4888, 86, 58}},
    {AOBgAnimID::BG_Well30, kNullBgAnimDetails, {"D2P06C01.CAM", 1232, 20, 41}},
    {AOBgAnimID::BG_Well31, kNullBgAnimDetails, {"D2P04C01.CAM", 1592, 84, 56}},
    {AOBgAnimID::BG_Well32, kNullBgAnimDetails, {"D2P04C07.CAM", 3552, 67, 78}},
    {AOBgAnimID::BG_Well33, kNullBgAnimDetails, {"D2P04C02.CAM", 3908, 70, 86}},
    {AOBgAnimID::BG_Well34, kNullBgAnimDetails, {"D2P04C03.CAM", 2652, 74, 60}},
    {AOBgAnimID::BG_Well35, kNullBgAnimDetails, {"D2P04C03.CAM", 6236, 90, 93}},
    {AOBgAnimID::BG_Well36, kNullBgAnimDetails, {"D2P04C04.CAM", 1744, 88, 20}},
    {AOBgAnimID::BG_Well37, kNullBgAnimDetails, {"D2P05C04.CAM", 4988, 82, 106}},
    {AOBgAnimID::BG_Well38, kNullBgAnimDetails, {"D2P09C01.CAM", 1392, 31, 55}},
    {AOBgAnimID::BG_Well39, kNullBgAnimDetails, {"D2P09C01.CAM", 1060, 28, 39}},
    {AOBgAnimID::BG_Well40, kNullBgAnimDetails, {"D2P09C02.CAM", 2224, 67, 57}},
    {AOBgAnimID::BG_Well41, kNullBgAnimDetails, {"D2P09C02.CAM", 904, 27, 36}},
    {AOBgAnimID::BG_Well42, kNullBgAnimDetails, {"D2P09C02.CAM", 1280, 25, 41}},
    {AOBgAnimID::BG_Well43, kNullBgAnimDetails, {"D2P09C02.CAM", 3036, 64, 52}},
    {AOBgAnimID::BG_Well44, kNullBgAnimDetails, {"D2P09C03.CAM", 2824, 86, 55}},
    {AOBgAnimID::BG_Well45, kNullBgAnimDetails, {"D2P03C03.CAM", 1308, 29, 46}},
    {AOBgAnimID::BG_Well46, kNullBgAnimDetails, {"D2P03C04.CAM", 3120, 74, 39}},
    {AOBgAnimID::BG_Well47, kNullBgAnimDetails, {"D2P03C05.CAM", 2716, 85, 32}},
    {AOBgAnimID::BG_Well48, kNullBgAnimDetails, {"D7P11C01.CAM", 984, 21, 39}},
    {AOBgAnimID::BG_Well49, kNullBgAnimDetails, {"D7P11C01.CAM", 1652, 82, 61}},
    {AOBgAnimID::BG_Well50, kNullBgAnimDetails, {"D7P11C04.CAM", 1148, 30, 49}},
    {AOBgAnimID::BG_Well51, kNullBgAnimDetails, {"D7P11C05.CAM", 1536, 30, 45}},
    {AOBgAnimID::BG_Well52, kNullBgAnimDetails, {"D7P11C07.CAM", 1464, 31, 42}},
    {AOBgAnimID::BG_Well53, kNullBgAnimDetails, {"D7P11C11.CAM", 1512, 37, 32}},
    {AOBgAnimID::BG_Well54, kNullBgAnimDetails, {"F1P01C01.CAM", 58444, 140, 64}},
    {AOBgAnimID::BG_Well55, kNullBgAnimDetails, {"F1P01C03.CAM", 2712, 72, 51}},
    {AOBgAnimID::BG_Well56, kNullBgAnimDetails, {"F1P05C10.CAM", 4692, 72, 92}},
    {AOBgAnimID::BG_Well57, kNullBgAnimDetails, {"F1P06C04.CAM", 3312, 71, 50}},
    {AOBgAnimID::BG_Well58, kNullBgAnimDetails, {"F1P06C04.CAM", 1444, 28, 44}},
    {AOBgAnimID::BG_Well59, kNullBgAnimDetails, {"F1P09C02.CAM", 1244, 32, 46}},
    {AOBgAnimID::BG_Well60, kNullBgAnimDetails, {"F2P01C02.CAM", 4336, 72, 95}},
    {AOBgAnimID::BG_Well61, kNullBgAnimDetails, {"F2P01C02.CAM", 2108, 36, 52}},
    {AOBgAnimID::BG_Well62, kNullBgAnimDetails, {"F2P04C08.CAM", 1592, 31, 54}},
    {AOBgAnimID::BG_Well63, kNullBgAnimDetails, {"F2P04C04.CAM", 1396, 22, 41}},
    {AOBgAnimID::BG_Well64, kNullBgAnimDetails, {"F2P04C04.CAM", 1360, 21, 39}},
    {AOBgAnimID::BG_Well65, kNullBgAnimDetails, {"F2P04C04.CAM", 3048, 67, 43}},
    {AOBgAnimID::BG_Well66, kNullBgAnimDetails, {"F2P06C04.CAM", 4616, 81, 61}},
    {AOBgAnimID::BG_Well67, kNullBgAnimDetails, {"F4P09C01.CAM", 1428, 39, 62}},
    {AOBgAnimID::BG_Well68, kNullBgAnimDetails, {"F4P09C01.CAM", 2316, 41, 53}},
    {AOBgAnimID::BG_Well69, kNullBgAnimDetails, {"F4P09C01.CAM", 2436, 76, 63}},
    {AOBgAnimID::BG_Well70, kNullBgAnimDetails, {"F4P09C04.CAM", 1520, 42, 58}},
    {AOBgAnimID::BG_Well71, kNullBgAnimDetails, {"F4P09C08.CAM", 3180, 45, 65}},
    {AOBgAnimID::BG_Well72, kNullBgAnimDetails, {"F4P09C08.CAM", 2564, 42, 53}},
    {AOBgAnimID::BG_Well73, kNullBgAnimDetails, {"F4P09C09.CAM", 5488, 85, 98}},
    {AOBgAnimID::BG_Well74, kNullBgAnimDetails, {"F4P09C09.CAM", 1600, 39, 48}},
    {AOBgAnimID::BG_Well75, kNullBgAnimDetails, {"F4P09C09.CAM", 1404, 34, 46}},
    {AOBgAnimID::BG_Well76, kNullBgAnimDetails, {"F4P09C09.CAM", 1776, 38, 46}},
    {AOBgAnimID::BG_Well77, kNullBgAnimDetails, {"F4P09C10.CAM", 2488, 74, 82}},
    {AOBgAnimID::BG_Well78, kNullBgAnimDetails, {"F4P09C10.CAM", 2444, 88, 83}},
    {AOBgAnimID::BG_Well79, kNullBgAnimDetails, {"F4P09C10.CAM", 1940, 38, 50}},
    {AOBgAnimID::BG_Well80, kNullBgAnimDetails, {"F4P09C11.CAM", 3188, 78, 80}},
    {AOBgAnimID::BG_Well81, kNullBgAnimDetails, {"F4P09C11.CAM", 1584, 39, 53}},
    {AOBgAnimID::BG_Well82, kNullBgAnimDetails, {"D1P09C04.CAM", 2856, 65, 55}},
    {AOBgAnimID::BG_Well83, kNullBgAnimDetails, {"D1P09C04.CAM", 1504, 35, 50}},
    {AOBgAnimID::BG_Well84, kNullBgAnimDetails, {"D1P09C03.CAM", 2012, 34, 57}},

    {AOBgAnimID::BG_Windmill1, kNullBgAnimDetails, {"F1P05C03.CAM", 9392, 80, 32}},
    {AOBgAnimID::BG_Windmill2, kNullBgAnimDetails, {"F1P05C04.CAM", 12156, 60, 48}},
    {AOBgAnimID::BG_Windmill3, kNullBgAnimDetails, {"F1P09C02.CAM", 11184, 73, 37}},

    // TODO: names - one of these blinks faster than the other
    {AOBgAnimID::BG_RedEyes1, kNullBgAnimDetails, {"F2P02C04.CAM", 560, 10, 4}},
    {AOBgAnimID::BG_RedEyes2, kNullBgAnimDetails, {"F2P02C04.CAM", 620, 10, 4}},

    {AOBgAnimID::BG_PinkFlame1, kNullBgAnimDetails, {"R2P12C01.CAM", 22264, 96, 71}},
    {AOBgAnimID::BG_PinkFlame2, kNullBgAnimDetails, {"R2P12C03.CAM", 10164, 61, 46}},
};


constexpr AnimDetails kNullAnimDetails = {};

constexpr CombinedAnimRecord kAnimRecords[] = {
    {AnimId::None, kNullAnimDetails, kNullAnimDetails},
    {AnimId::Abe_Head_Gib,
    { "ABEBLOW.BAN", 7732, 50, 25, kAbeblowResID, PalId::Default},
     {"ABEBLOW.BAN", 7208, 50, 25, AO::kAbeblowAOResID, PalId::Default}},
    {AnimId::Abe_Arm_Gib,
    { "ABEBLOW.BAN", 7772, 50, 25, kAbeblowResID, PalId::Default},
     {"ABEBLOW.BAN", 7248, 50, 25, AO::kAbeblowAOResID, PalId::Default}},
    {AnimId::Abe_Body_Gib,
    { "ABEBLOW.BAN", 7812, 50, 25, kAbeblowResID, PalId::Default},
     {"ABEBLOW.BAN", 7288, 50, 25, AO::kAbeblowAOResID, PalId::Default}},
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
    
    {AnimId::Elum_Head_Gib, kNullAnimDetails, {"ELMBLOW.BAN", 7440, 72, 43, AO::kElmblowAOResID_217, PalId::Default}},
    {AnimId::Elum_Arm_Gib, kNullAnimDetails, {"ELMBLOW.BAN", 7480, 72, 43, AO::kElmblowAOResID_217, PalId::Default}},
    {AnimId::Elum_Body_Gib, kNullAnimDetails, {"ELMBLOW.BAN", 7400, 72, 43, AO::kElmblowAOResID_217, PalId::Default}},

    {AnimId::Meat_Gib, kNullAnimDetails, { "R1MEAT.BAN", 8908, 69, 30, AO::kMeatGibAOResID_6014, PalId::Default} },
    {AnimId::Metal_Gib,
    { "METAL.BAN", 2244, 38, 29, kMetalGibResID, PalId::Default},
     {"METAL.BAN", 3928, 37, 30, AO::kMetalGibAOResID, PalId::Default}},

    {AnimId::Rock_Gib, kNullAnimDetails, { "DEBRIS00.BAN", 8108, 71, 36, AO::kDebrisID00AOResID, PalId::Default} },
    {AnimId::Slig_Head_Gib,
    {"SLGBLOW.BAN", 6480, 44, 26, kSligBlowResID, PalId::Default},
     {"SLGBLOW.BAN", 5948, 44, 26, AO::kSligBlowAOResID, PalId::Default}},
    {AnimId::Slig_Arm_Gib,
    { "SLGBLOW.BAN", 6560, 44, 26, kSligBlowResID, PalId::Default},
     {"SLGBLOW.BAN", 6028, 44, 26, AO::kSligBlowAOResID, PalId::Default}},
    {AnimId::Slig_Body_Gib,
    { "SLGBLOW.BAN", 6520, 44, 26, kSligBlowResID, PalId::Default},
     {"SLGBLOW.BAN", 5988, 44, 26, AO::kSligBlowAOResID, PalId::Default}},

    {AnimId::Slog_Head_Gib,
    { "DOGBLOW.BAN", 7504, 53, 28, kSlogBlowResID, PalId::Default},
     {"DOGBLOW.BAN", 28376, 111, 63, AO::kSlogBlowAOResID, PalId::Default}},
    {AnimId::Slog_Body_Gib,
    { "DOGBLOW.BAN", 7544, 53, 28, kSlogBlowResID, PalId::Default},
     {"DOGBLOW.BAN", 28416, 111, 63, AO::kSlogBlowAOResID, PalId::Default}},

    {AnimId::Stick_Gib, kNullAnimDetails, { "STICK.BAN", 2800, 47, 29, AO::kStickGibAOResID, PalId::Default} },
    
    {AnimId::Bat_Flying, kNullAnimDetails, { "BATBASIC.BAN", 6644, 48, 17, AO::kBatBasicAOResID, PalId::Default} },
    {AnimId::Bat, kNullAnimDetails, { "BATBASIC.BAN", 6560, 48, 17, AO::kBatBasicAOResID, PalId::Default} },
    {AnimId::Bee_Swarm, kNullAnimDetails, { "WASP.BAN", 636, 7, 4, AO::kWaspAOResID, PalId::Default} },

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

    {AnimId::FlyingSlig_Idle, { "FLYSLIG.BND", 116888, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_MoveHorizontal, { "FLYSLIG.BND", 116912, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_IdleTurnAround, { "FLYSLIG.BND", 117084, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_MoveDown, { "FLYSLIG.BND", 116988, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_MoveDownTurnAround, { "FLYSLIG.BND", 117584, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_MoveUp, { "FLYSLIG.BND", 117012, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_MoveUpTurnAround, { "FLYSLIG.BND", 117616, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_PullLever, { "FLYSLIG.BND", 117188, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_Speak, { "FLYSLIG.BND", 117132, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_Possession, { "FLYSLIG.BND", 117524, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_MoveHorizontalEnd, { "FLYSLIG.BND", 117060, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_MoveUpStart, { "FLYSLIG.BND", 117316, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_MoveHorizontalToDown, { "FLYSLIG.BND", 117276, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_MoveUpToHorizontal, { "FLYSLIG.BND", 117444, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_MoveDownToHorizontal, { "FLYSLIG.BND", 117376, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_TurnQuick, { "FLYSLIG.BND", 116936, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_IdleToHorizontal, { "FLYSLIG.BND", 117036, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_BeginDownMovement, { "FLYSLIG.BND", 117336, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_EndDownMovement, { "FLYSLIG.BND", 117356, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_DownKnockback, { "FLYSLIG.BND", 117396, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_UpKnockback, { "FLYSLIG.BND", 117464, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_EndUpMovement, { "FLYSLIG.BND", 117424, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_InstantUpXTurn, { "FLYSLIG.BND", 117552, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_InstantDownXTurn, { "FLYSLIG.BND", 117492, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_HorizontalToUpMovement, { "FLYSLIG.BND", 117296, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FlyingSlig_TurnToHorizontalMovement, { "FLYSLIG.BND", 117752, 107, 48u, kFlySligResID, PalId::Default}, kNullAnimDetails },

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
    // TODO: correct BAN/BND's and resourceID's for AO
    {AnimId::Mudokon_ToSpeak,
        { "ABEBSIC1.BAN", 59112, 135, 80, kAbebsic1ResID, PalId::Default},
     {"ABEBSIC1.BAN", 256136, 135, 80, AO::kAbebsic1AOResID, PalId::Default}},
    {AnimId::Mudokon_Null, { "ABEBSIC.BAN", 270240, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_CrouchSpeak, { "ABEBSIC.BAN", 271080, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_ToCrouchSpeak, { "ABEBSIC.BAN", 271120, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_RollBegin,
        { "ABEBSIC.BAN", 269688, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255196, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_Rolling,
        { "ABEBSIC.BAN", 269708, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255216, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_RollingEnd_Unused, // just unused in AE it's actually used in AO
        { "ABEBSIC.BAN", 269764, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255272, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_HopBegin,
        { "ABEBSIC.BAN", 271004, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256584, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_HopMid,
        { "ABEBSIC.BAN", 271056, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256636, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_HopLand,
        { "ABEBSIC.BAN", 270964, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256544, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_RunJumpLand,
        { "ABEBSIC.BAN", 270488, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256028, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_RunToRoll,
        { "ABEBSIC.BAN", 270708, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256272, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_RunTurnToWalk,
        { "ABEBSIC.BAN", 270140, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255648, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_RunJumpLandRun,
        { "ABEBSIC.BAN", 269784, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255292, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_RunJumpLandWalk,
        { "ABEBSIC.BAN", 269856, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255364, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_TurnToRun,
        { "ABEBSIC.BAN", 270944, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256524, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_LedgeAscend,
        { "ABEHOIST.BAN", 57440, 135, 80, kAbehoistResID, PalId::Default},
     {"ABEHOIST.BAN", 54208, 135, 80, AO::kAbehoistAOResID, PalId::Default}},
    {AnimId::Mudokon_LedgeDescend,
        { "ABEHOIST.BAN", 57580, 135, 80, kAbehoistResID, PalId::Default},
     {"ABEHOIST.BAN", 54348, 135, 80, AO::kAbehoistAOResID, PalId::Default}},
    {AnimId::Mudokon_LedgeHang,
        { "ABEHOIST.BAN", 57540, 135, 80, kAbehoistResID, PalId::Default},
     {"ABEHOIST.BAN", 54308, 135, 80, AO::kAbehoistAOResID, PalId::Default}},
    {AnimId::Mudokon_LedgeHangWobble,
        { "ABEHOIST.BAN", 57712, 135, 80, kAbehoistResID, PalId::Default},
     {"ABEHOIST.BAN", 54480, 135, 80, AO::kAbehoistAOResID, PalId::Default}},
    {AnimId::Mudokon_RingRopePullHang,
        { "ABEHOIST.BAN", 57680, 135, 80, kAbehoistResID, PalId::Default},
     {"ABEHOIST.BAN", 54448, 135, 80, AO::kAbehoistAOResID, PalId::Default}},
    {AnimId::Mudokon_PushWall,
        { "ABEKNBK.BAN", 49912, 135, 80, kAbeknbkResID, PalId::Default},
     {"ABEKNBK.BAN", 48460, 135, 80, AO::kAbeknbkAOResID, PalId::Default}},
    {AnimId::Mudokon_RollingKnockback,
        { "ABEKNBK.BAN", 49856, 135, 80, kAbeknbkResID, PalId::Default},
     {"ABEKNBK.BAN", 48404, 135, 80, AO::kAbeknbkAOResID, PalId::Default}},
    {AnimId::Mudokon_JumpIntoWell,
        { "ABEWELL.BAN", 20572, 135, 80, kAbewellResID, PalId::Default},
     {"ABEWELL.BAN", 19644, 135, 80, AO::kAbewellAOResID, PalId::Default}},
    {AnimId::Mudokon_Well_Idle,
        { "ABEWELL.BAN", 20552, 135, 80, kAbewellResID, PalId::Default},
     {"ABEWELL.BAN", 19624, 135, 80, AO::kAbewellAOResID, PalId::Default}},
    {AnimId::Mudokon_WellBegin,
        { "ABEWELL.BAN", 20404, 135, 80, kAbewellResID, PalId::Default},
     {"ABEWELL.BAN", 19476, 135, 80, AO::kAbewellAOResID, PalId::Default}},
    {AnimId::Mudokon_FallLandDie,
        { "ABENOELM.BND", 8104, 135, 80, kAbesmashResID, PalId::Default},
     {"ABENOELM.BND", 7880, 135, 80, AO::kAbesmashAOResID, PalId::Default}},
    {AnimId::Mudokon_Fall,
        { "ABENOELM.BND", 5724, 135, 80, kAbefallResID, PalId::Default},
     {"ABENOELM.BND", 5560, 135, 80, AO::kAbefallAOResID, PalId::Default}},
    {AnimId::Mudokon_HandstoneBegin,
        { "ABESTONE.BAN", 16096, 135, 80, kAbestoneResID, PalId::Default},
     {"ABESTONE.BAN", 15484, 135, 80, AO::kAbestoneAOResID, PalId::Default}},
    {AnimId::Mudokon_HandstoneEnd,
        { "ABESTONE.BAN", 16040, 135, 80, kAbestoneResID, PalId::Default},
     {"ABESTONE.BAN", 15428, 135, 80, AO::kAbestoneAOResID, PalId::Default}},
    {AnimId::Mudokon_GrenadeMachineUse,
        { "ABESTONE.BAN", 16220, 135, 80, kAbestoneResID, PalId::Default},
     {"ABESTONE.BAN", 15608, 135, 80, AO::kAbestoneAOResID, PalId::Default}},
    {AnimId::Mudokon_RunOffEdge,
        { "ABEEDGE.BAN", 32068, 135, 80, kAbeedgeResID, PalId::Default},
     {"ABEEDGE.BAN", 40708, 135, 80, AO::kAbeedgeAOResID, PalId::Default}},
    {AnimId::Mudokon_SneakOffEdge,
        { "ABEEDGE.BAN", 32112, 135, 80, kAbeedgeResID, PalId::Default},
     {"ABEEDGE.BAN", 40752, 135, 80, AO::kAbeedgeAOResID, PalId::Default}},
    {AnimId::Mudokon_HopToFall,
        { "ABEEDGE.BAN", 32168, 135, 80, kAbeedgeResID, PalId::Default},
     {"ABEEDGE.BAN", 40852, 135, 80, AO::kAbeedgeAOResID, PalId::Default}},
    {AnimId::Mudokon_RunJumpToFall,
        { "ABEEDGE.BAN", 32152, 135, 80, kAbeedgeResID, PalId::Default},
     {"ABEEDGE.BAN", 40836, 135, 80, AO::kAbeedgeAOResID, PalId::Default}},
    {AnimId::Mudokon_RollOffEdge, // rename to roll off ledge?
        { "ABEEDGE.BAN", 32132, 135, 80, kAbeedgeResID, PalId::Default}, 
        {"ABEEDGE.BAN", 40816, 135, 80, AO::kAbeedgeAOResID, PalId::Default}},
    {AnimId::Mudokon_SlapBomb,
        { "ABEBOMB.BAN", 6520, 135, 80, kAbebombResID, PalId::Default},
     {"ABEBOMB.BAN", 6360, 135, 80, AO::kAbebombAOResID, PalId::Default}},
    {AnimId::Mudokon_RollingKnockForward,
        { "ABEKNFD.BAN", 17180, 135, 80, kAbeknfdResID, PalId::Default},
     {"ABEKNFD.BAN", 16712, 135, 80, AO::kAbeknfdAOResID, PalId::Default}},
    {AnimId::Mudokon_RockThrowStandingHold,
        { "ABETHROW.BAN", 31504, 135, 80, kAbethrowResID, PalId::Default},
     {"ABETHROW.BAN", 30440, 135, 80, AO::kAbethrowAOResID, PalId::Default}},
    {AnimId::Mudokon_RockThrowStandingThrow,
        { "ABETHROW.BAN", 31392, 135, 80, kAbethrowResID, PalId::Default},
     {"ABETHROW.BAN", 30328, 135, 80, AO::kAbethrowAOResID, PalId::Default}},
    {AnimId::Mudokon_RockThrowStandingEnd,
        { "ABETHROW.BAN", 31632, 135, 80, kAbethrowResID, PalId::Default},
     {"ABETHROW.BAN", 30568, 135, 80, AO::kAbethrowAOResID, PalId::Default}},
    {AnimId::Mudokon_RockThrowCrouchingHold,
        { "ABETHROW.BAN", 31576, 135, 80, kAbethrowResID, PalId::Default},
     {"ABETHROW.BAN", 30512, 135, 80, AO::kAbethrowAOResID, PalId::Default}},
    {AnimId::Mudokon_RockThrowCrouchingThrow,
        { "ABETHROW.BAN", 31432, 135, 80, kAbethrowResID, PalId::Default},
     {"ABETHROW.BAN", 30368, 135, 80, AO::kAbethrowAOResID, PalId::Default}},
    {AnimId::Mudokon_ZShotRolling, // remove Z from the name or add it to the AO func name?
        { "ABEKNOKZ.BAN", 24580, 135, 80, kAbeknokzResID, PalId::Default},
     {"ABEKNOKZ.BAN", 23996, 135, 80, AO::kAbeknokzAOResID, PalId::Default}},
    {AnimId::Mudokon_ZShot, // // remove Z from the name or add it to the AO func name?
        { "ABEKNOKZ.BAN", 24628, 135, 80, kAbeknokzResID, PalId::Default},
     {"ABEKNOKZ.BAN", 24044, 135, 80, AO::kAbeknokzAOResID, PalId::Default}},
    {AnimId::Mudokon_PickupItem,
        { "ABEPICK.BAN", 6004, 135, 80, kAbepickResID, PalId::Default},
     {"ABEPICK.BAN", 5828, 135, 80, AO::kAbepickAOResID, PalId::Default}},
    {AnimId::Mudokon_DoorEnter,
        { "ABEDOOR.BAN", 19040, 135, 80, kAbedoorResID, PalId::Default},
     {"ABEDOOR.BAN", 17520, 135, 80, AO::kAbedoorAOResID, PalId::Default}},
    {AnimId::Mudokon_DoorExit,
        { "ABEDOOR.BAN", 19088, 135, 80, kAbedoorResID, PalId::Default},
     {"ABEDOOR.BAN", 17568, 135, 80, AO::kAbedoorAOResID, PalId::Default}},
    {AnimId::Mudokon_ToShrykull, 
        { "ABEMORPH.BAN", 8732, 135, 80, kAbemorphResID, PalId::Default},
        { "ABEMORPH.BAN", 92004, 121, 79, AO::kShrmorphAOResID, PalId::Default}
    },
    {AnimId::Mudokon_EndShrykull,
        { "ABEMORPH.BAN", 8772, 135, 80, kAbemorphResID, PalId::Default},
            kNullAnimDetails },

    {AnimId::Mudokon_LiftGrabBegin,
        { "ABELIFT.BAN", 22548, 135, 80, kAbeliftResID, PalId::Default},
     {"ABELIFT.BAN", 21696, 135, 80, AO::kAbeliftAOResID, PalId::Default}},
    {AnimId::Mudokon_LiftGrabEnd,
        { "ABELIFT.BAN", 22572, 135, 80, kAbeliftResID, PalId::Default},
     {"ABELIFT.BAN", 21720, 135, 80, AO::kAbeliftAOResID, PalId::Default}},
    {AnimId::Mudokon_LiftGrabIdle,
        { "ABELIFT.BAN", 22464, 135, 80, kAbeliftResID, PalId::Default},
     {"ABELIFT.BAN", 21612, 135, 80, AO::kAbeliftAOResID, PalId::Default}},
    {AnimId::Mudokon_LiftUseUp, { "ABELIFT.BAN", 22596, 135, 80, kAbeliftResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_LiftUseDown, { "ABELIFT.BAN", 22496, 135, 80, AO::kAbeliftAOResID, PalId::Default}, kNullAnimDetails },

    {AnimId::Mudokon_AO_LiftUse, kNullAnimDetails, {"ABELIFT.BAN", 21644, 135, 80, AO::kAbeliftAOResID, PalId::Default}},

    {AnimId::Mudokon_PoisonGasDeath,
        { "ABEGAS.BAN", 28824, 135, 80, kAbegasResID, PalId::Default},
     {"ABEGAS.BAN", 27748, 135, 80, AO::kAbegasAOResID, PalId::Default}},
    {AnimId::Mudokon_Idle, 
        { "ABEBSIC1.BAN", 58888, 135, 80, kAbebsic1ResID, PalId::Default}, 
        {"ABEBSIC1.BAN", 55968, 135, 80, AO::kAbebsic1AOResID, PalId::Default}},
    {AnimId::Mudokon_Walk,
        { "ABEBSIC1.BAN", 58808, 135, 80, kAbebsic1ResID, PalId::Default},
     {"ABEBSIC1.BAN", 55888, 135, 80, AO::kAbebsic1AOResID, PalId::Default}},
    {AnimId::Mudokon_StandingTurn,
        { "ABEBSIC1.BAN", 59064, 135, 80, kAbebsic1ResID, PalId::Default},
     {"ABEBSIC1.BAN", 56144, 135, 80, AO::kAbebsic1AOResID, PalId::Default}},
    {AnimId::Mudokon_Speak1,
        { "ABEBSIC1.BAN", 59028, 135, 80, kAbebsic1ResID, PalId::Default},
     {"ABEBSIC1.BAN", 56108, 135, 80, AO::kAbebsic1AOResID, PalId::Default}},
    {AnimId::Mudokon_Speak3,
        { "ABEBSIC1.BAN", 58956, 135, 80, kAbebsic1ResID, PalId::Default},
     {"ABEBSIC1.BAN", 56072, 135, 80, AO::kAbebsic1AOResID, PalId::Default}},
    {AnimId::Mudokon_Speak2,
        { "ABEBSIC1.BAN", 58992, 135, 80, kAbebsic1ResID, PalId::Default},
     {"ABEBSIC1.BAN", 56036, 135, 80, AO::kAbebsic1AOResID, PalId::Default}},
    {AnimId::Mudokon_SpeakFart,
        { "ABEBSIC1.BAN", 58920, 135, 80, kAbebsic1ResID, PalId::Default},
     {"ABEBSIC1.BAN", 56000, 135, 80, AO::kAbebsic1AOResID, PalId::Default}}, // speak 4 AO
    {AnimId::Mudokon_WalkBegin,
        { "ABEBSIC1.BAN", 58768, 135, 80, kAbebsic1ResID, PalId::Default},
     {"ABEBSIC1.BAN", 55848, 135, 80, AO::kAbebsic1AOResID, PalId::Default}},
    {AnimId::Mudokon_WalkToIdle,
        { "ABEBSIC1.BAN", 58788, 135, 80, kAbebsic1ResID, PalId::Default},
     {"ABEBSIC1.BAN", 55868, 135, 80, AO::kAbebsic1AOResID, PalId::Default}},
    {AnimId::Mudokon_MidWalkToIdle,
        { "ABEBSIC1.BAN", 58748, 135, 80, kAbebsic1ResID, PalId::Default},
     {"ABEBSIC1.BAN", 55828, 135, 80, AO::kAbebsic1AOResID, PalId::Default}},

    {AnimId::Mudokon_AO_Null, kNullAnimDetails, {"ABEBSIC1.BAN", 21744, 135, 80, AO::kAbebsic1AOResID, PalId::Default}},

    {AnimId::Mudokon_LeverUse,
        { "ABEPULL.BAN", 11396, 135, 80, kAbepullResID, PalId::Default}, 
        {"ABEPULL.BAN", 10988, 135, 80, AO::kAbepullAOResID, PalId::Default}},
    {AnimId::Mudokon_CrouchScrub,
        { "MUDSCRUB.BAN", 9388, 135, 80, kMudscrubResID, PalId::Default},
     {"MUDWORK.BND", 9056, 135, 80, AO::kMudscrubAOResID, PalId::Default}},
    {AnimId::Mudokon_CrouchIdle,
        { "ABEBSIC.BAN", 270092, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255600, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_CrouchTurn,
        { "ABEBSIC.BAN", 270428, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255968, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_StandToCrouch,
        { "ABEBSIC.BAN", 269876, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255384, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_CrouchToStand,
        { "ABEBSIC.BAN", 270060, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255568, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_WalkToRun,
        { "ABEBSIC.BAN", 270120, 135, 80, kAbebasicResID, PalId::Default},
     {"MUDLOTUS.BND", 255628, 96, 55, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_MidWalkToRun,
        { "ABEBSIC.BAN", 270268, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255808, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_Run,
        { "ABEBSIC.BAN", 270748, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256312, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_RunToWalk,
        { "ABEBSIC.BAN", 270668, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256232, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_MidRunToWalk,
        { "ABEBSIC.BAN", 270820, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256384, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_RunSlideStop,
        { "ABEBSIC.BAN", 270860, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256424, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_RunSlideTurn,
        { "ABEBSIC.BAN", 270328, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255868, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_RunTurnToRun,
        { "ABEBSIC.BAN", 270408, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255948, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_Sneak,
        { "ABEBSIC.BAN", 270508, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256048, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_WalkToSneak,
        { "ABEBSIC.BAN", 270688, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256252, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_SneakToWalk,
        { "ABEBSIC.BAN", 270596, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256160, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_MidWalkToSneak,
        { "ABEBSIC.BAN", 270840, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256404, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_MidSneakToWalk,
        { "ABEBSIC.BAN", 270728, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256292, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_SneakBegin,
        { "ABEBSIC.BAN", 270288, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255828, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_SneakToIdle,
        { "ABEBSIC.BAN", 270160, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255668, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_MidSneakToIdle,
        { "ABEBSIC.BAN", 270308, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255848, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_RunJumpBegin,
        { "ABEBSIC.BAN", 270468, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256008, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_RunJumpMid,
        { "ABEBSIC.BAN", 269804, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255312, 135, 80, AO::kAbebasicAOResID, PalId::Default}},


    // 11396 for AO means no frametableoffset specified yet
    {AnimId::Mudokon_StandToRun,
        { "ABEBSIC.BAN", 270252, 135, 80, kAbebasicResID, PalId::Default},
     {"MUDLOTUS.BND", 255788, 96, 55, AO::kAbebasicAOResID, PalId::Default}},

    {AnimId::Mudokon_HoistBegin,
        { "ABEBSIC.BAN", 269976, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255484, 135, 80, AO::kAbebasicAOResID, PalId::Default}},

    // this has been removed from AE. it is almost identical to Mudokon_HoistBegin
    {AnimId::Mudokon_AO_HoistBegin_Long, kNullAnimDetails, {"ABEEDGE.BAN", 40772, 135, 80, AO::kAbeedgeAOResID, PalId::Default} },

    {AnimId::Mudokon_HoistIdle,
        { "ABEBSIC.BAN", 270024, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255532, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_LandSoft,
        { "ABEBSIC.BAN", 269928, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255436, 135, 80, AO::kAbebasicAOResID, PalId::Default}},

    // this has been removed from AE.
    {AnimId::Mudokon_AO_LandSoft_Long, kNullAnimDetails, {"ABEEDGE.BAN", 40868, 135, 80, AO::kAbeedgeAOResID, PalId::Default}},

    {AnimId::Mudokon_DunnoBegin,
        { "ABEBSIC.BAN", 270180, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 255716, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_DunnoEnd,
     {"ABEBSIC.BAN", 270616, 135, 80, kAbebasicResID, PalId::Default},
     {"ABEBSIC.BAN", 256180, 135, 80, AO::kAbebasicAOResID, PalId::Default}},
    {AnimId::Mudokon_Knockback,
        { "ABEKNBK.BAN", 49740, 135, 80, kAbeknbkResID, PalId::Default},
     {"ABEKNBK.BAN", 48272, 135, 80, AO::kAbeknbkAOResID, PalId::Default}},
    {AnimId::Mudokon_KnockbackGetUp,
        { "ABEKNBK.BAN", 49800, 135, 80, kAbeknbkResID, PalId::Default},
     {"ABEKNBK.BAN", 48332, 135, 80, AO::kAbeknbkAOResID, PalId::Default}},
    {AnimId::Mudokon_WalkOffEdge,
        { "ABEEDGE.BAN", 32040, 135, 80, kAbeedgeResID, PalId::Default},
     {"ABEEDGE.BAN", 40680, 96, 55, AO::kAbeedgeAOResID, PalId::Default}},
    {AnimId::Mudokon_FallingFromGrab,
        { "ABEEDGE.BAN", 32012, 135, 80, kAbeedgeResID, PalId::Default},
     {"ABEEDGE.BAN", 40652, 135, 80, AO::kAbeedgeAOResID, PalId::Default} },
    {AnimId::Mudokon_Chant,
        { "ABEOMM.BAN", 9992, 135, 80, kAbeommResID, PalId::Default},
     {"ABEOMM.BAN", 9516, 135, 80, AO::kAbeommAOResID, PalId::Default}},
    {AnimId::Mudokon_ChantEnd,
        { "ABEOMM.BAN", 10040, 135, 80, kAbeommResID, PalId::Default},
     {"ABEOMM.BAN", 9564, 135, 80, AO::kAbeommAOResID, PalId::Default}},
    {AnimId::Mudokon_ToDuck,
        { "MUDWORK.BND", 5236, 135, 80, kMudoduckResID, PalId::Default}, 
        {"MUDWORK.BND", 5096, 135, 80, AO::kMudoduckAOResID, PalId::Default}},
    {AnimId::Mudokon_Duck,
        { "MUDWORK.BND", 5256, 135, 80, kMudoduckResID, PalId::Default},
     {"MUDWORK.BND", 5116, 135, 80, AO::kMudoduckAOResID, PalId::Default}},
    {AnimId::Mudokon_DuckToCrouch,
        { "MUDWORK.BND", 5280, 135, 80, kMudoduckResID, PalId::Default},
     {"MUDWORK.BND", 5140, 135, 80, AO::kMudoduckAOResID, PalId::Default}},
    {AnimId::Mudokon_DuckKnockback,
        { "MUDBTLNK.BAN", 5328, 135, 80, kMudbtlnkResID, PalId::Default},
     {"MUDWORK.BND", 5224, 135, 80, AO::kMudbtlnkAOResID, PalId::Default}},



    // AO only start
    { AnimId::Mudokon_Sling_Idle, kNullAnimDetails, {"MUDSLING.BAN", 22636, 96, 55, AO::kSlingmudAOResID_508, PalId::Default} },
    { AnimId::Mudokon_Sling_Angry, kNullAnimDetails, {"MUDSLING.BAN", 22676, 96, 55, AO::kSlingmudAOResID_508, PalId::Default} },
    { AnimId::Mudokon_Sling_Speak, kNullAnimDetails, {"MUDSLING.BAN", 22744, 96, 55, AO::kSlingmudAOResID_508, PalId::Default} },
    { AnimId::Mudokon_Sling_ShootStart, kNullAnimDetails, {"MUDSLING.BAN", 22700, 96, 55, AO::kSlingmudAOResID_508, PalId::Default} },
    { AnimId::Mudokon_Sling_ShootEnd, kNullAnimDetails, {"MUDSLING.BAN", 22720, 96, 55, AO::kSlingmudAOResID_508, PalId::Default} },
    { AnimId::Mudokon_Sling_AngryToIdle, kNullAnimDetails, {"MUDSLING.BAN", 22780, 96, 55, AO::kSlingmudAOResID_508, PalId::Default} },

    {AnimId::Mudokon_StandScrubLoop, kNullAnimDetails, {"MUDCHSL.BAN", 29112, 96, 55, AO::kMudchslAOResID, PalId::Default}},
    {AnimId::Mudokon_StandScrubLoopToPause, kNullAnimDetails, {"MUDCHSL.BAN", 29304, 96, 55, AO::kMudchslAOResID, PalId::Default}},
    {AnimId::Mudokon_StandScrubPauseToLoop, kNullAnimDetails, {"MUDCHSL.BAN", 29324, 96, 55, AO::kMudchslAOResID, PalId::Default}},
    {AnimId::Mudokon_StandScrubPause, kNullAnimDetails, {"MUDCHSL.BAN", 29336, 96, 55, AO::kMudchslAOResID, PalId::Default}},
    {AnimId::Mudokon_IdleToStandScrub, kNullAnimDetails, {"MUDCHSL.BAN", 29160, 96, 55, AO::kMudchslAOResID, PalId::Default}},
    {AnimId::Mudokon_StandScrubToIdle, kNullAnimDetails, {"MUDCHSL.BAN", 29232, 96, 55, AO::kMudchslAOResID, PalId::Default}},

    {AnimId::Mudokon_Struggle, kNullAnimDetails, {"ABEWASP.BAN", 21496, 96, 55, AO::kAbewaspAOResID, PalId::Default}},
    {AnimId::Mudokon_StruggleToCrouchChant, kNullAnimDetails, {"MUDLOTUS.BND", 10960, 96, 55, AO::kMudltusAOResID, PalId::Default}},
    {AnimId::Mudokon_CrouchChant, kNullAnimDetails, {"MUDLOTUS.BND", 11052, 96, 55, AO::kMudltusAOResID, PalId::Default}},
    {AnimId::Mudokon_CrouchChantToStruggle, kNullAnimDetails, {"MUDLOTUS.BND", 10996, 96, 55, AO::kMudltusAOResID, PalId::Default}},

     // DunnoMid only exists in AO for some reason
    {AnimId::Mudokon_DunnoMid, kNullAnimDetails, {"MUDLOTUS.BND", 255688, 135, 80, kMudltusResID, PalId::Default}}, // todo ban/bnd 

    {AnimId::Mudokon_AO_M_15_Null, kNullAnimDetails, {"MUDLOTUS.BND", 255776, 135, 80, AO::kMudltusAOResID, PalId::Default}}, // todo ban/bnd

    {AnimId::Mudokon_CrouchSpeak1, kNullAnimDetails, {"MUDLOTUS.BND", 256660, 135, 80, kMudltusResID, PalId::Default}}, // todo ban/bnd
    {AnimId::Mudokon_CrouchSpeak2, kNullAnimDetails, {"MUDLOTUS.BND", 256700, 135, 80, AO::kMudltusAOResID, PalId::Default}}, // todo ban/bnd

    {AnimId::Mudokon_ElumWalkLoop, kNullAnimDetails, {"ABEWELM.BND", 144920, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumIdle, kNullAnimDetails, {"ABEWELM.BND", 145132, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumRunSlideStop, kNullAnimDetails, {"ABEWELM.BND", 145736, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumRunTurn, kNullAnimDetails, {"ABEWELM.BND", 145804, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_AO_M_106_Null, kNullAnimDetails, {"ABEWELM.BND", 145000, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumHopBegin, kNullAnimDetails, {"ABEWELM.BND", 145104, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumHopMid, kNullAnimDetails, {"ABEWELM.BND", 145164, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumHopLand, kNullAnimDetails, {"ABEWELM.BND", 145196, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumRunJumpBegin, kNullAnimDetails, {"ABEWELM.BND", 145220, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumRunJumpMid, kNullAnimDetails, {"ABEWELM.BND", 145688, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumRunJumpLand, kNullAnimDetails, {"ABEWELM.BND", 145240, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumTurn, kNullAnimDetails, {"ABEWELM.BND", 145456, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumRunLoop, kNullAnimDetails, {"ABEWELM.BND", 145312, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumSpeak, kNullAnimDetails, {"ABEWELM.BND", 145512, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_AO_M_116_Null, kNullAnimDetails, {"ABEWELM.BND", 145588, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumWalkBegin, kNullAnimDetails, {"ABEWELM.BND", 145608, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumRunBegin, kNullAnimDetails, {"ABEWELM.BND", 145548, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_AO_M_119_Null, kNullAnimDetails, {"ABEWELM.BND", 145416, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumRunToWalk, kNullAnimDetails, {"ABEWELM.BND", 145436, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumMidRunToWalk, kNullAnimDetails, {"ABEWELM.BND", 145568, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumRunTurnToRun, kNullAnimDetails, {"ABEWELM.BND", 145272, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumRunTurnToWalk, kNullAnimDetails, {"ABEWELM.BND", 145292, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumWalkEnd, kNullAnimDetails, {"ABEWELM.BND", 145648, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumMidWalkEnd, kNullAnimDetails, {"ABEWELM.BND", 145668, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},
    {AnimId::Mudokon_ElumBeesStruggling, kNullAnimDetails, {"ABEWELM.BND", 145860, 135, 80, AO::kAbeWElmAOResID_100, PalId::Default}},

    {AnimId::Mudokon_ElumMountEnd, kNullAnimDetails, {"ABEWELM.BND", 23568, 135, 80, AO::kElumUnknownAOResID_110, PalId::Default}}, // todo ban/bnd
    {AnimId::Mudokon_ElumUnmountBegin, kNullAnimDetails, {"ANEMNT.BAN", 13716, 135, 80, AO::kElumUnknownAOResID_112, PalId::Default}}, // todo ban/bnd

    {AnimId::Mudokon_ElumUnmountEnd, kNullAnimDetails, {"ANEDSMNT.BND", 34676, 135, 80, AO::kAbeANEDSMNTAOResID_113, PalId::Default} },
    {AnimId::Mudokon_ElumMountBegin, kNullAnimDetails, {"ANEPRMNT.BAN", 16256, 135, 80, AO::kAneprmntAOResID, PalId::Default}},

    {AnimId::Mudokon_ElumFallOffEdge, kNullAnimDetails, {"ANEEDGE.BAN", 21672, 135, 80, AO::kAneedgeAOResID, PalId::Default}},
    {AnimId::Mudokon_ElumFall, kNullAnimDetails, {"ANEEDGE.BAN", 21700, 135, 80, AO::kAneedgeAOResID, PalId::Default}},
    {AnimId::Mudokon_ElumLand, kNullAnimDetails, {"ANEEDGE.BAN", 21724, 135, 80, AO::kAneedgeAOResID, PalId::Default}},
    {AnimId::Mudokon_ElumJumpToFall, kNullAnimDetails, {"ANEEDGE.BAN", 21780, 135, 80, AO::kAneedgeAOResID, PalId::Default}},

    {AnimId::Mudokon_ElumKnockback, kNullAnimDetails, {"ANEKNBK.BAN", 11652, 135, 80, AO::kAneknbkAOResID, PalId::Default}},

    {AnimId::Mudokon_Idle_RubEyes, kNullAnimDetails, {"ABERUB.BAN", 9860, 135, 80, AO::kAberubAOResID, PalId::Default}},
    {AnimId::Mudokon_Idle_StretchArms, kNullAnimDetails, {"ABESIZE.BAN", 13152, 135, 80, AO::kAbesizeAOResID, PalId::Default}},
    {AnimId::Mudokon_Idle_Yawn, kNullAnimDetails, {"ABEYAWN.BAN", 12392, 135, 80, AO::kAbeyawnAOResID, PalId::Default}},

    {AnimId::Mudokon_AO_ToShrykull, kNullAnimDetails, {"MUDLOTUS.BND", 8240, 135, 80, AO::kMudltusAOResID, PalId::Default}}, // todo ban/bnd + resource id
    {AnimId::Mudokon_AO_ShrykullEnd, kNullAnimDetails, {"MUDLOTUS.BND", 8280, 135, 80, AO::kMudltusAOResID, PalId::Default}}, // todo ban/bnd + resource id

    {AnimId::Mudokon_KnockForward,
     {"ABEKNFD.BAN", 17240, 135, 80, kAbeknfdResID, PalId::Default},
     {"ABEKNFD.BAN", 16772, 135, 80, AO::kAbeknfdAOResID, PalId::Default}},


    {AnimId::Mudokon_MineCarEnter, {"ABECAR.BAN", 8540, 135, 80, kAbeCarResId, PalId::Default}, kNullAnimDetails},
    {AnimId::Mudokon_MineCarExit, {"ABECAR.BAN", 8588, 135, 80, kAbeCarResId, PalId::Default}, kNullAnimDetails},
    {AnimId::Mudokon_Sorry, {"ABEBSIC.BAN", 271192, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Mudokon_AfterSorry, {"ABEBSIC.BAN", 271248, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails},

    {AnimId::Mudokon_Chisel, {"MUDCHSL.BAN", 5276, 135, 80, kMudchslResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Mudokon_StartChisel, {"MUDCHSL.BAN", 5308, 135, 80, kMudchslResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Mudokon_StopChisel, {"MUDCHSL.BAN", 5320, 135, 80, kMudchslResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Mudokon_Punch,{"ABEBSIC.BAN", 271152, 135, 80, kAbebasicResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Mudokon_SlapOwnHead, { "MUDIDLE.BAN", 9640, 135, 80, kMudidleResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_TurnWheelBegin, { "ABEWORK.BAN", 11856, 135, 80, kAbeworkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_TurnWheel, { "ABEWORK.BAN", 11816, 135, 80, kAbeworkResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Mudokon_TurnWheelEnd, { "ABEWORK.BAN", 11888, 135, 80, kAbeworkResID, PalId::Default}, kNullAnimDetails },

    // AE only end


    // TODO: check AO ResourceID's
    {AnimId::Paramite_Idle,
        { "PARAMITE.BND", 96696, 137, 65u, kArjbasicResID, PalId::Default},
     {"PARAMITE.BND", 57152, 138, 49, AO::kArjbasicAOResID, PalId::Default}},
    {AnimId::Paramite_WalkBegin,
        { "PARAMITE.BND", 96676, 137, 65u, kArjbasicResID, PalId::Default},
     {"PARAMITE.BND", 57132, 138, 49, AO::kArjbasicAOResID, PalId::Default}},
    {AnimId::Paramite_Walking,
        { "PARAMITE.BND", 96548, 137, 65u, kArjbasicResID, PalId::Default},
     {"PARAMITE.BND", 57004, 138, 49, AO::kArjbasicAOResID, PalId::Default}},
    {AnimId::Paramite_Running,
        { "PARAMITE.BND", 96612, 137, 65u, kArjbasicResID, PalId::Default},
     {"PARAMITE.BND", 57068, 138, 49, AO::kArjbasicAOResID, PalId::Default}},
    {AnimId::Paramite_Turn,
        { "PARAMITE.BND", 96792, 137, 65u, kArjbasicResID, PalId::Default},
     {"PARAMITE.BND", 57304, 138, 49, AO::kArjbasicAOResID, PalId::Default}},
    {AnimId::Paramite_Hop,
        { "PARAMITE.BND", 96728, 137, 65u, kArjbasicResID, PalId::Default},
     {"PARAMITE.BND", 57176, 138, 49, AO::kArjbasicAOResID, PalId::Default}},
    {AnimId::Paramite_Unused, { "PARAMITE.BND", 97164, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails }, // crashes the game when used
    {AnimId::Paramite_WalkRunTransition,
        { "PARAMITE.BND", 97172, 137, 65u, kArjbasicResID, PalId::Default},
     {"PARAMITE.BND", 57348, 138, 49, AO::kArjbasicAOResID, PalId::Default}},
    {AnimId::Paramite_WalkEnd,
        { "PARAMITE.BND", 96828, 137, 65u, kArjbasicResID, PalId::Default},
     {"PARAMITE.BND", 57368, 138, 49, AO::kArjbasicAOResID, PalId::Default}},
    {AnimId::Paramite_RunBegin,
        { "PARAMITE.BND", 96868, 137, 65u, kArjbasicResID, PalId::Default},
     {"PARAMITE.BND", 57408, 138, 49, AO::kArjbasicAOResID, PalId::Default}},
    {AnimId::Paramite_RunEnd,
        { "PARAMITE.BND", 96848, 137, 65u, kArjbasicResID, PalId::Default},
     {"PARAMITE.BND", 57388, 138, 49, AO::kArjbasicAOResID, PalId::Default}},
    {AnimId::Paramite_Falling,
        { "PARAMITE.BND", 96888, 137, 65u, kArjbasicResID, PalId::Default},
     {"PARAMITE.BND", 57428, 138, 49, AO::kArjbasicAOResID, PalId::Default}},
    {AnimId::Paramite_JumpUpBegin, { "PARAMITE.BND", 96904, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_JumpUpMidair, { "PARAMITE.BND", 96932, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_JumpUpLand, { "PARAMITE.BND", 96944, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_RopePull, { "PARAMITE.BND", 96972, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_CloseAttack, { "PARAMITE.BND", 97192, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Landing, { "PARAMITE.BND", 97096, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Unused2, { "PARAMITE.BND", 97120, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Knockback, { "PARAMITE.BND", 97136, 137, 65u, kArjbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_GameSpeakBegin,
        { "PARAMITE.BND", 17032, 137, 65u, kArjpumpResID, PalId::Default},
     {"PARAMITE.BND", 15860, 138, 49, AO::kArjpumpAOResID, PalId::Default}},
    {AnimId::Paramite_PreHiss,
        { "PARAMITE.BND", 17072, 137, 65u, kArjpumpResID, PalId::Default},
     {"PARAMITE.BND", 15900, 138, 49, AO::kArjpumpAOResID, PalId::Default}},
    {AnimId::Paramite_Hiss,
        { "PARAMITE.BND", 17088, 137, 65u, kArjpumpResID, PalId::Default},
     {"PARAMITE.BND", 15916, 138, 49, AO::kArjpumpAOResID, PalId::Default}},
    {AnimId::Paramite_AllOYaGameSpeakBegin, { "PARAMITE.BND", 17192, 137, 65u, kArjpumpResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_PostHiss,
        { "PARAMITE.BND", 17120, 137, 65u, kArjpumpResID, PalId::Default},
     {"PARAMITE.BND", 15948, 138, 49, AO::kArjpumpAOResID, PalId::Default}},
    {AnimId::Paramite_GameSpeakEnd,
        { "PARAMITE.BND", 17152, 137, 65u, kArjpumpResID, PalId::Default},
     {"PARAMITE.BND", 15980, 138, 49, AO::kArjpumpAOResID, PalId::Default}},
    {AnimId::Paramite_RunningAttack,
        { "PARAMITE.BND", 19068, 137, 65u, kArjponceResID, PalId::Default},
     {"PARAMITE.BND", 19120, 138, 49, AO::kArjponceAOResID, PalId::Default}},
    {AnimId::Paramite_SurpriseWeb,
        { "PARAMITE.BND", 52312, 137, 65u, kArjclimbResID, PalId::Default},
     {"PARAMITE.BND", 18084, 138, 49, AO::kArjclimbAOResID, PalId::Default}},
    {AnimId::Paramite_WebLeaveDown,
        { "PARAMITE.BND", 52344, 137, 65u, kArjclimbResID, PalId::Default},
     {"PARAMITE.BND", 18116, 138, 49, AO::kArjclimbAOResID, PalId::Default}},
    {AnimId::Paramite_WebIdle, { "PARAMITE.BND", 52476, 137, 65u, kArjclimbResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_WebGoingUp, { "PARAMITE.BND", 52428, 137, 65u, kArjclimbResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_WebGoingDown, { "PARAMITE.BND", 52380, 137, 65u, kArjclimbResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_WebGrab, { "PARAMITE.BND", 52548, 137, 65u, kArjclimbResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_WebLeaveUp, { "PARAMITE.BND", 52588, 137, 65u, kArjclimbResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Paramite_Eating,
        { "PARAMITE.BND", 15628, 137, 65u, kArjeatResID, PalId::Default},
     {"PARAMITE.BND", 14728, 138, 49, AO::kArjeatAOResID, PalId::Default}},
    {AnimId::Paramite_Death,
        { "PARAMITE.BND", 8108, 137, 65u, kArjfalrkResID, PalId::Default},
     {"PARAMITE.BND", 9132, 138, 49, AO::kArjfalrkAOResID, PalId::Default}},
    {AnimId::Paramite_Squawk,
        { "PARAMITE.BND", 9636, 137, 65u, kArjwaspResID, PalId::Default},
        kNullAnimDetails },
    {AnimId::Paramite_Attack, { "PARAMITE.BND", 10948, 137, 65u, kArjscrchResID, PalId::Default}, kNullAnimDetails },


    {AnimId::Paramite_Struggle, kNullAnimDetails, {"PARAMITE.BND", 11076, 138, 49, AO::kArjscrchAOResID, PalId::Default}}, // same as Paramite_Squawk?
    {AnimId::Paramite_AO_M_7_Unknown, kNullAnimDetails, {"PARAMITE.BND", 57340, 138, 49, AO::kArjscrchAOResID, PalId::Default}},
    {AnimId::Paramite_AO_M_22_Unknown, kNullAnimDetails, {"PARAMITE.BND", 10520, 138, 49, AO::kArjscrchAOResID, PalId::Default}},

    // TODO: check AO ResourceID's
    {AnimId::Scrab_Idle,
        { "SCRAB.BND", 224764, 156, 69, kArsbasicResID, PalId::Default},
     {"SCRAB.BND", 168644, 168, 69, AO::kArsbasicAOResID, PalId::Default}},
    {AnimId::Scrab_Walk,
        { "SCRAB.BND", 224668, 156, 69, kArsbasicResID, PalId::Default},
     {"SCRAB.BND", 168548, 168, 69, AO::kArsbasicAOResID, PalId::Default}},
    {AnimId::Scrab_Run,
        { "SCRAB.BND", 224796, 156, 69, kArsbasicResID, PalId::Default},
     {"SCRAB.BND", 168676, 168, 69, AO::kArsbasicAOResID, PalId::Default}},
    {AnimId::Scrab_Turn,
        { "SCRAB.BND", 224860, 156, 69, kArsbasicResID, PalId::Default},
     {"SCRAB.BND", 168740, 168, 69, AO::kArsbasicAOResID, PalId::Default}},
    {AnimId::Scrab_RunToStand,
        { "SCRAB.BND", 224916, 156, 69, kArsbasicResID, PalId::Default},
     {"SCRAB.BND", 168796, 168, 69, AO::kArsbasicAOResID, PalId::Default}},
    {AnimId::Scrab_HopBegin,
        { "SCRAB.BND", 224964, 156, 69, kArsbasicResID, PalId::Default},
     {"SCRAB.BND", 168844, 168, 69, AO::kArsbasicAOResID, PalId::Default}},
    {AnimId::Scrab_Jump,
        { "SCRAB.BND", 224988, 156, 69, kArsbasicResID, PalId::Default},
     {"SCRAB.BND", 168868, 168, 69, AO::kArsbasicAOResID, PalId::Default}},
    {AnimId::Scrab_Landing,
        { "SCRAB.BND", 225028, 156, 69, kArsbasicResID, PalId::Default},
     {"SCRAB.BND", 168908, 168, 69, AO::kArsbasicAOResID, PalId::Default}},
    {AnimId::Scrab_JumpAndRunToFall, { "SCRAB.BND", 225112, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_StandToWalk,
        { "SCRAB.BND", 225052, 156, 69, kArsbasicResID, PalId::Default},
     {"SCRAB.BND", 168932, 168, 69, AO::kArsbasicAOResID, PalId::Default}},
    {AnimId::Scrab_StandToRun,
        { "SCRAB.BND", 225072, 156, 69, kArsbasicResID, PalId::Default},
     {"SCRAB.BND", 168952, 168, 69, AO::kArsbasicAOResID, PalId::Default}},
    {AnimId::Scrab_WalkToStand,
        { "SCRAB.BND", 225092, 156, 69, kArsbasicResID, PalId::Default},
     {"SCRAB.BND", 168972, 168, 69, AO::kArsbasicAOResID, PalId::Default}},
    {AnimId::Scrab_WalkToFall, { "SCRAB.BND", 225132, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_WalkToRun, { "SCRAB.BND", 225168, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_RunToWalk, { "SCRAB.BND", 225188, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_Knockback, { "SCRAB.BND", 225208, 156, 69, kArsbasicResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_GetEaten,
        { "SCRAB.BND", 225280, 156, 69, kArsbasicResID, PalId::Default},
     {"SCRAB.BND", 1324, 168, 69, AO::kArsbasicAOResID, PalId::Default}},
    {AnimId::Scrab_Stamp,
        { "SCRAB.BND", 24480, 156, 69, kArsdanceResID, PalId::Default},
     {"SCRAB.BND", 24136, 168, 69, AO::kArsdanceAOResID, PalId::Default}},
    {AnimId::Scrab_DeathEnd, { "SCRAB.BND", 1340, 156, 69, kArsdeadResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_Empty, { "SCRAB.BND", 11228, 156, 69, kArsgrwlResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_HowlBegin,
        { "SCRAB.BND", 17260, 156, 69, kArshowlResID, PalId::Default},
     {"SCRAB.BND", 16944, 168, 69, AO::kArshowlAOResID, PalId::Default}},
    {AnimId::Scrab_HowlEnd,
        { "SCRAB.BND", 17328, 156, 69, kArshowlResID, PalId::Default},
     {"SCRAB.BND", 17012, 168, 69, AO::kArshowlAOResID, PalId::Default}},
    {AnimId::Scrab_Shriek,
        { "SCRAB.BND", 11532, 156, 69, kArsroarResID, PalId::Default},
     {"SCRAB.BND", 11380, 168, 69, AO::kArsroarAOResID, PalId::Default}},
    {AnimId::Scrab_ScrabBattleAnim,
        { "SCRAB.BND", 31648, 156, 69, kArswhirlResID, PalId::Default},
     {"SCRAB.BND", 30940, 168, 69, AO::kArswhirlAOResID, PalId::Default}},
    {AnimId::Scrab_AttackSpin, { "SCRAB.BND", 31704, 156, 69, kArswhirlResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_FeedToGulp,
        { "SCRAB.BND", 35180, 156, 69, kArschewResID, PalId::Default},
     {"SCRAB.BND", 26664, 168, 69, AO::kArschewAOResID, PalId::Default}},
    {AnimId::Scrab_GulpToStand, { "SCRAB.BND", 35248, 156, 69, kArschewResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_StandToFeed, { "SCRAB.BND", 20840, 156, 69, kArseatResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Scrab_Feed,
        { "SCRAB.BND", 20896, 156, 69, kArseatResID, PalId::Default},
     {"SCRAB.BND", 19600, 168, 69, AO::kArseatAOResID, PalId::Default}},
    {AnimId::Scrab_AttackLunge,
        { "SCRAB.BND", 8356, 156, 69, kArsprceResID, PalId::Default},
     {"SCRAB.BND", 8212, 168, 69, AO::kArsprceAOResID, PalId::Default}},
    {AnimId::Scrab_LegKick,
        { "SCRAB.BND", 14508, 156, 69, kArsskwrResID, PalId::Default},
     {"SCRAB.BND", 14228, 168, 69, AO::kArsskwrAOResID, PalId::Default}},
    {AnimId::Scrab_DeathBegin,
        { "SCRAB.BND", 15600, 156, 69, kArscrshResID, PalId::Default},
     {"SCRAB.BND", 12724, 168, 69, AO::kArscrshAOResID, PalId::Default}},

    {AnimId::Scrab_AO_ToFall, kNullAnimDetails, {"SCRAB.BND", 168992, 168, 69, AO::kArseatAOResID, PalId::Default}},
    {AnimId::Scrab_AO_ToFeed, kNullAnimDetails, {"SCRAB.BND", 19544, 168, 69, AO::kArseatAOResID, PalId::Default}},
    {AnimId::Scrab_AO_M_19_Unused, kNullAnimDetails, {"SCRAB.BND", 11060, 168, 69, AO::kArseatAOResID, PalId::Default}},

    { AnimId::CrawlingSlig_Idle, { "CRAWLSLG.BND", 53684, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_UsingButton, { "CRAWLSLG.BND", 53852, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_WakingUp, { "CRAWLSLG.BND", 53964, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_Crawling, { "CRAWLSLG.BND", 53560, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_StartFalling, { "CRAWLSLG.BND", 53720, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_Falling, { "CRAWLSLG.BND", 53736, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_Landing, { "CRAWLSLG.BND", 53752, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_ToShakingToIdle, { "CRAWLSLG.BND", 53780, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_Speaking, { "CRAWLSLG.BND", 53816, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_Snoozing, { "CRAWLSLG.BND", 53928, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_PushingWall, { "CRAWLSLG.BND", 54032, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_TurnAround, { "CRAWLSLG.BND", 53616, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_Shaking, { "CRAWLSLG.BND", 54000, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_Empty, { "CRAWLSLG.BND", 53780, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_ShakingToIdle, { "CRAWLSLG.BND", 54096, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_EndCrawling, { "CRAWLSLG.BND", 54076, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_IdleToPushingWall, { "CRAWLSLG.BND", 54160, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },
    { AnimId::CrawlingSlig_EndPushingWall, { "CRAWLSLG.BND", 54188, 109, 37, kCrawlingSligResID_449, PalId::Default}, kNullAnimDetails },


    // assuming the AO BAN/BND's and ResourceID's are the same as AE for now
    {AnimId::Slig_Idle,
        { "SLIG.BND", 135512, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 132740, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_StandToWalk,
        { "SLIG.BND", 135804, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 133032, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_Walking,
        { "SLIG.BND", 135360, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 132588, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_StandToRun,
        { "SLIG.BND", 135824, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 133052, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_Running,
        { "SLIG.BND", 135440, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 132668, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_TurnAroundStanding,
        { "SLIG.BND", 136088, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 133316, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_Shoot,
        { "SLIG.BND", 135544, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 132772, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_SlidingToStand,
        { "SLIG.BND", 135696, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 132924, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_SlidingTurn,
        { "SLIG.BND", 135576, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 132804, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_SlidingTurnToWalk,
        { "SLIG.BND", 135916, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 133144, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_SlidingTurnToRun,
        { "SLIG.BND", 135632, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 132860, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_ReloadGun,
        { "SLIG.BND", 135652, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 132880, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_ShootToStand,
        { "SLIG.BND", 135768, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 132996, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_SteppingToStand,
        { "SLIG.BND", 135844, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 133072, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_StandingToStep,
        { "SLIG.BND", 135876, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 133104, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_Gamespeak,
        { "SLIG.BND", 135788, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 133016, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_WalkToStand,
        { "SLIG.BND", 135896, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 133124, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_Recoil,
        { "SLIG.BND", 135936, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 133164, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_Speak1,
        { "SLIG.BND", 136048, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 133276, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_Speak2,
        { "SLIG.BND", 135976, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 133204, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_Speak3,
        { "SLIG.BND", 136132, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 133360, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_Speak4,
        { "SLIG.BND", 136012, 160, 68, kSlgbasicResID, PalId::Default},
     {"SLIG.BND", 133240, 160, 68, AO::kSlgbasicAOResID, PalId::Default}},
    {AnimId::Slig_Sleeping,
        { "SLGSLEEP.BAN", 33552, 160, 68, kSlgsleepResID, PalId::Default},
     {"SLGSLEEP.BAN", 32848, 160, 68, AO::kSlgsleepAOResID, PalId::Default}},
    {AnimId::Slig_SleepingToStand,
        { "SLGSLEEP.BAN", 33448, 160, 68, kSlgsleepResID, PalId::Default},
     {"SLGSLEEP.BAN", 32744, 160, 68, AO::kSlgsleepAOResID, PalId::Default}},
    {AnimId::Slig_Knockback,
        { "SLIG.BND", 33184, 160, 68, kSlgknbkResID, PalId::Default},
     {"SLIG.BND", 32240, 160, 68, AO::kSlgknbkAOResID, PalId::Default}},
    {AnimId::Slig_KnockbackToStand,
        { "SLIG.BND", 33228, 160, 68, kSlgknbkResID, PalId::Default},
     {"SLIG.BND", 32284, 160, 68, AO::kSlgknbkAOResID, PalId::Default}},
    {AnimId::Slig_PossessShake,
        { "SLIG.BND", 33348, 160, 68, kSlgknbkResID, PalId::Default},
     {"SLIG.BND", 32388, 160, 68, AO::kSlgknbkAOResID, PalId::Default}},
    {AnimId::Slig_OutToFall,
        { "SLGEDGE.BAN", 30560, 160, 68, kSlgedgeResID, PalId::Default},
     {"SLGEDGE.BAN", 29708, 160, 68, AO::kSlgedgeAOResID, PalId::Default}},
    {AnimId::Slig_FallingInitiate,
        { "SLGEDGE.BAN", 30592, 160, 68, kSlgedgeResID, PalId::Default},
     {"SLGEDGE.BAN", 29740, 160, 68, AO::kSlgedgeAOResID, PalId::Default}},
    {AnimId::Slig_LandingSoft,
        { "SLGEDGE.BAN", 30628, 160, 68, kSlgedgeResID, PalId::Default},
     {"SLGEDGE.BAN", 29776, 160, 68, AO::kSlgedgeAOResID, PalId::Default}},
    {AnimId::Slig_LandingFatal,
        { "SLGSMASH.BAN", 9204, 160, 68, kSlgsmashResID, PalId::Default},
     {"SLGSMASH.BAN", 9080, 160, 68, AO::kSlgsmashAOResID, PalId::Default}},
    {AnimId::Slig_ShootZ,
        { "SLIGZ.BND", 9208, 160, 68, kSlgzshotResID, PalId::Default},
     {"SLIGZ.BND", 8844, 160, 68, AO::kSlgzshotAOResID, PalId::Default}},
    {AnimId::Slig_ShootZtoStand,
        { "SLIGZ.BND", 9260, 160, 68, kSlgzshotResID, PalId::Default},
     {"SLIGZ.BND", 8888, 160, 68, AO::kSlgzshotAOResID, PalId::Default}},
    {AnimId::Slig_Smash,
        { "SLGKNFD.BAN", 13016, 160, 68, kSlgknfdResID, PalId::Default},
     {"SLGKNFD.BAN", 12752, 160, 68, AO::kSlgknfdAOResID, PalId::Default}},
    {AnimId::Slig_PullLever, { "SLGLEVER.BAN", 12612, 160, 68, kSlgleverResID, PalId::Default}, kNullAnimDetails }, // AO doesn't have this??
    {AnimId::Slig_LiftGrip,
        { "SLGLIFT.BAN", 23048, 160, 68, kSlgliftResID, PalId::Default},
     {"SLGLIFT.BAN", 22628, 160, 68, AO::kSlgliftAOResID, PalId::Default}},
    {AnimId::Slig_LiftUngrip,
        { "SLGLIFT.BAN", 23072, 160, 68, kSlgliftResID, PalId::Default},
     {"SLGLIFT.BAN", 22652, 160, 68, AO::kSlgliftAOResID, PalId::Default}},
    {AnimId::Slig_LiftGripping,
        { "SLGLIFT.BAN", 23200, 160, 68, kSlgliftResID, PalId::Default},
     {"SLGLIFT.BAN", 22780, 160, 68, AO::kSlgliftAOResID, PalId::Default}},
    {AnimId::Slig_LiftUp,
        { "SLGLIFT.BAN", 23148, 160, 68, kSlgliftResID, PalId::Default},
     {"SLGLIFT.BAN", 22728, 160, 68, AO::kSlgliftAOResID, PalId::Default}},
    {AnimId::Slig_LiftDown,
        { "SLGLIFT.BAN", 23096, 160, 68, kSlgliftResID, PalId::Default},
     {"SLGLIFT.BAN", 22676, 160, 68, AO::kSlgliftAOResID, PalId::Default}},
    {AnimId::Slig_Beat,
        { "SLGBEAT.BAN", 12660, 160, 68, kSlgbeatResID, PalId::Default},
     {"SLGBEAT.BAN", 12356, 160, 68, AO::kSlgbeatAOResID, PalId::Default}},

    // apparently these both look exactly the same?
    {AnimId::CrawlingSligButton, {"CSLGBUTN.BAN", 408, 25, 12, kCrawlingSligButtonResID_1057, PalId::Default}, kNullAnimDetails},
    {AnimId::CrawlingSligButtonUse, {"CSLGBUTN.BAN", 420, 25, 12, kCrawlingSligButtonResID_1057, PalId::Default}, kNullAnimDetails},

    // main menu gamespeak
    {AnimId::MenuAbeSpeak_Laugh, kNullAnimDetails, {"ABESPEAK.BAN", 2115576, 134, 90, AO::kAbespeakAOResID, PalId::Default}},
    {AnimId::MenuAbeSpeak_WhistleHigh, kNullAnimDetails, {"ABESPEAK.BAN", 2115712, 134, 90, AO::kAbespeakAOResID, PalId::Default}},
    {AnimId::MenuAbeSpeak_WhistleLow, kNullAnimDetails, {"ABESPEAK.BAN", 2115824, 134, 90, AO::kAbespeakAOResID, PalId::Default}},
    {AnimId::MenuAbeSpeak_Fart, kNullAnimDetails, {"ABESPEAK.BAN", 2115368, 134, 90, AO::kAbespeakAOResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_Hello, 
        {"ABESPEK2.BAN", 247620, 233, 125, kAbespek2ResID, PalId::Default},
        {"STARTANM.BND", 201320, 134, 90, AO::kAbespek2AOResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_Idle,
        {"ABESPEK2.BAN", 247808, 233, 125, kAbespek2ResID, PalId::Default},
        {"STARTANM.BND", 201508, 134, 90, AO::kAbespek2AOResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_IdleBlink,
        {"ABESPEK2.BAN", 247684, 233, 125, kAbespek2ResID, PalId::Default},
        {"STARTANM.BND", 201384, 134, 90, AO::kAbespek2AOResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_Ok, 
        {"ABESPEK2.BAN", 247924, 233, 125, kAbespek2ResID, PalId::Default},
        {"STARTANM.BND", 201632, 134, 90, AO::kAbespek2AOResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_FollowMe, 
        {"ABESPEAK.BAN", 693672, 233, 125, kAbespeakResID, PalId::Default},
        {"ABESPEAK.BAN", 2115300, 134, 90, AO::kAbespeakAOResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_Wait,
        {"ABESPEAK.BAN", 693912, 233, 125, kAbespeakResID, PalId::Default},
        {"ABESPEAK.BAN", 2115912, 134, 90, AO::kAbespeakAOResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_Work, { "ABESPEAK.BAN", 694084, 233, 125, kAbespeakResID, PalId::Default}, kNullAnimDetails },

    { AnimId::MenuAbeSpeak_Anger, // Anger in AE and Angry in AO
        {"ABESPEAK.BAN", 693556, 233, 125, kAbespeakResID, PalId::Default},
        {"ABESPEAK.BAN", 2115180, 134, 90, AO::kAbespeakAOResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_AllYa, { "ABESPEAK.BAN", 694132, 233, 125, kAbespeakResID, PalId::Default}, kNullAnimDetails },
    { AnimId::MenuAbeSpeak_Sympathy, { "ABESPEAK.BAN", 693956, 233, 125, kAbespeakResID, PalId::Default}, kNullAnimDetails },
    { AnimId::MenuAbeSpeak_StopIt, { "ABESPEAK.BAN", 694200, 233, 125, kAbespeakResID, PalId::Default }, kNullAnimDetails },

    { AnimId::MenuAbeSpeak_Chant,
        { "ABESPEAK.BAN", 693772, 233, 125, kAbespeakResID, PalId::Default },
        {"ABESPEAK.BAN", 2115472, 134, 90, AO::kAbespeakAOResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_ChantEnd,
        {"ABESPEAK.BAN", 693748, 233, 125, kAbespeakResID, PalId::Default },
        {"ABESPEAK.BAN", 2115452, 134, 90, AO::kAbespeakAOResID, PalId::Default}},

    { AnimId::MenuAbeSpeak_Goodbye,
        {"ABESPEAK.BAN", 693504, 233, 125, kAbespeakResID, PalId::Default },
        {"ABESPEAK.BAN", 2115120, 134, 90, AO::kAbespeakAOResID, PalId::Default}},

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
        {"HIGHLITE.BAN", 6152, 99, 43, AO::kHighliteAOResID, PalId::Default}},

    {AnimId::MenuHighlight_ButtonRemapSquare,
     {"HIGHLITE.BAN", 13936, 150, 65, kHighliteResID, PalId::Default},
     {"HIGHLITE.BAN", 6176, 99, 43, AO::kHighliteAOResID, PalId::Default}},

    {AnimId::MenuHighlight_Triangle,
        { "HIGHLITE.BAN", 13924, 150, 65, kHighliteResID, PalId::Default },
        {"HIGHLITE.BAN", 6164, 99, 43, AO::kHighliteAOResID, PalId::Default} },
    {AnimId::MenuHighlight_Square,
        { "HIGHLITE.BAN", 13900, 150, 65, kHighliteResID, PalId::Default },
        {"HIGHLITE.BAN", 6140, 99, 43, AO::kHighliteAOResID, PalId::Default} },

    {AnimId::MenuDoor, 
        { "STDOOR.BAN", 50212, 233, 125, kMenuDoorResID, PalId::Default },
        { "DOOR.BAN", 41420, 134, 90, AO::kMenuDoorAOResID, PalId::Default}},

    {AnimId::AbeIntro,
        {"ABEINTRO.BAN", 25888, 233, 125, kAbeintroResID, PalId::Default },
        {"ABEINTRO.BAN", 37364, 134, 90, AO::kAbeintroAOResID, PalId::Default}},

    // TODO: check AO ResourceID's
    {AnimId::Slog_Idle,
        { "SLOG.BND", 96464, 121, 57, kDogbasicResID, PalId::Default},
     {"SLOG.BND", 94456, 121, 57, AO::kDogbasicAOResID, PalId::Default}},
    {AnimId::Slog_Walk,
        { "SLOG.BND", 96344, 121, 57, kDogbasicResID, PalId::Default},
     {"SLOG.BND", 94336, 121, 57, AO::kDogbasicAOResID, PalId::Default}},
    {AnimId::Slog_Run,
        { "SLOG.BND", 96424, 121, 57, kDogbasicResID, PalId::Default},
     {"SLOG.BND", 94416, 121, 57, AO::kDogbasicAOResID, PalId::Default}},
    {AnimId::Slog_TurnAround,
        { "SLOG.BND", 96764, 121, 57, kDogbasicResID, PalId::Default},
     {"SLOG.BND", 94852, 121, 57, AO::kDogbasicAOResID, PalId::Default}},
    {AnimId::Slog_Fall,
        { "SLOG.BND", 96692, 121, 57, kDogbasicResID, PalId::Default},
     {"SLOG.BND", 94708, 121, 57, AO::kDogbasicAOResID, PalId::Default}},
    {AnimId::Slog_MoveHeadUpwards,
        { "SLOG.BND", 96496, 121, 57, kDogbasicResID, PalId::Default},
     {"SLOG.BND", 94488, 121, 57, AO::kDogbasicAOResID, PalId::Default}},
    {AnimId::Slog_StopRunning,
        { "SLOG.BND", 96532, 121, 57, kDogbasicResID, PalId::Default},
     {"SLOG.BND", 94572, 121, 57, AO::kDogbasicAOResID, PalId::Default}},
    {AnimId::Slog_SlideTurn,
        { "SLOG.BND", 96580, 121, 57, kDogbasicResID, PalId::Default},
     {"SLOG.BND", 94524, 121, 57, AO::kDogbasicAOResID, PalId::Default}},
    {AnimId::Slog_StartWalking,
        { "SLOG.BND", 96640, 121, 57, kDogbasicResID, PalId::Default},
     {"SLOG.BND", 94632, 121, 57, AO::kDogbasicAOResID, PalId::Default}},
    {AnimId::Slog_EndWalking,
        { "SLOG.BND", 96660, 121, 57, kDogbasicResID, PalId::Default},
     {"SLOG.BND", 94652, 121, 57, AO::kDogbasicAOResID, PalId::Default}},
    {AnimId::Slog_Land,
        { "SLOG.BND", 96876, 121, 57, kDogbasicResID, PalId::Default},
     {"SLOG.BND", 94684, 121, 57, AO::kDogbasicAOResID, PalId::Default}},
    {AnimId::Slog_Unused, { "SLOG.BND", 96716, 121, 57, kDogbasicResID, PalId::Default}, kNullAnimDetails }, // Just a still frame
    {AnimId::Slog_StartFastBarking,
        { "SLOG.BND", 96728, 121, 57, kDogbasicResID, PalId::Default},
     {"SLOG.BND", 94816, 121, 57, AO::kDogbasicAOResID, PalId::Default}},
    {AnimId::Slog_EndFastBarking,
        { "SLOG.BND", 96752, 121, 57, kDogbasicResID, PalId::Default},
     {"SLOG.BND", 94840, 121, 57, AO::kDogbasicAOResID, PalId::Default}},
    {AnimId::Slog_AngryBark,
        { "SLOG.BND", 15068, 121, 57, kDogrstnResID, PalId::Default},
     {"SLOG.BND", 14744, 121, 57, AO::kDogrstnAOResID, PalId::Default}},
    {AnimId::Slog_Sleeping,
        { "SLOG.BND", 15108, 121, 57, kDogrstnResID, PalId::Default},
     {"SLOG.BND", 14784, 121, 57, AO::kDogrstnAOResID, PalId::Default}},
    {AnimId::Slog_MoveHeadDownwards, // fall asleep better name and still accurate?
        { "SLOG.BND", 15156, 121, 57, kDogrstnResID, PalId::Default},
     {"SLOG.BND", 14832, 121, 57, AO::kDogrstnAOResID, PalId::Default}},
    {AnimId::Slog_WakeUp,
        { "SLOG.BND", 15132, 121, 57, kDogrstnResID, PalId::Default},
     {"SLOG.BND", 14808, 121, 57, AO::kDogrstnAOResID, PalId::Default}},
    {AnimId::Slog_JumpForwards,
        { "SLOG.BND", 38904, 121, 57, kDogattkResID, PalId::Default},
     {"SLOG.BND", 37952, 121, 57, AO::kDogattkAOResID, PalId::Default}},
    {AnimId::Slog_JumpUpwards,
        { "SLOG.BND", 38960, 121, 57, kDogattkResID, PalId::Default},
     {"SLOG.BND", 38008, 121, 57, AO::kDogattkAOResID, PalId::Default}},
    {AnimId::Slog_Eating,
        { "SLOG.BND", 39064, 121, 57, kDogattkResID, PalId::Default},
     {"SLOG.BND", 38112, 121, 57, AO::kDogattkAOResID, PalId::Default}},
    {AnimId::Slog_Dying,
        { "DOGKNFD.BAN", 12412, 121, 57, kDogknfdResID, PalId::Default},
     {"DOGKNFD.BAN", 12220, 121, 57, AO::kDogknfdAOResID, PalId::Default}},
    {AnimId::Slog_Scratch,
        { "SLOG.BND", 12724, 121, 57, kDogidleResID, PalId::Default},
     {"SLOG.BND", 12412, 121, 57, AO::kDogidleAOResID, PalId::Default}},
    {AnimId::Slog_Growl,
        { "SLOG.BND", 12812, 121, 57, kDogidleResID, PalId::Default},
     {"SLOG.BND", 12500, 121, 57, AO::kDogidleAOResID, PalId::Default}},
    {AnimId::Slog_AO_M_12_Unknown, kNullAnimDetails, {"SLOG.BND", 94804, 121, 57, AO::kDogbasicAOResID, PalId::Default}},


    {AnimId::Slurg_Move, { "SLURG.BAN", 2708, 46, 15, kSlurgResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slurg_Burst, { "SLURG.BAN", 2808, 46, 15, kSlurgResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Slurg_Turn_Around, { "SLURG.BAN", 2740, 46, 15, kSlurgResID, PalId::Default}, kNullAnimDetails },

    {AnimId::Background_Glukkon_Idle, kNullAnimDetails, { "GLUKKON.BND", 46096, 68, 60, AO::kGlkbasicAOResID, PalId::Default} },
    {AnimId::Background_Glukkon_Laugh, kNullAnimDetails, { "GLUKKON.BND", 46272, 68, 60, AO::kGlkbasicAOResID, PalId::Default} },
    {AnimId::Background_Glukkon_KillHim1, kNullAnimDetails, { "GLUKKON.BND", 46128, 68, 60, AO::kGlkbasicAOResID, PalId::Default} },
    {AnimId::Background_Glukkon_KillHim2, kNullAnimDetails, { "GLUKKON.BND", 46180, 68, 60, AO::kGlkbasicAOResID, PalId::Default} },

    {AnimId::BirdPortal_Sparks,
        {"PORTAL.BND", 4256, 32, 69, kPortliteResID, PalId::Default},
     {"PORTAL.BND", 3708, 31, 69, AO::kPortliteAOResID, PalId::Default}},
    {AnimId::BirdPortal_Flash,
        {"PORTAL.BND", 13576, 145, 74, kPortlitResID, PalId::Default},
     {"PORTAL.BND", 13352, 145, 74, AO::kPortlitAOResID, PalId::Default}},

    {AnimId::BirdPortal_TerminatorShrink,
        {"PORTAL.BND", 4168, 32, 18, kPortalTerminatorResID, PalId::Default},
        { "PORTAL.BND", 3884, 31, 18, AO::kPortalTerminatorAOResID, PalId::Default} },
    {AnimId::BirdPortal_TerminatorIdle,
        {"PORTAL.BND", 4068, 32, 18, kPortalTerminatorResID, PalId::Default},
        { "PORTAL.BND", 3784, 31, 18, AO::kPortalTerminatorAOResID, PalId::Default} },
    {AnimId::BirdPortal_TerminatorGrow,
        { "PORTAL.BND", 4144, 32, 18, kPortalTerminatorResID, PalId::Default},
        { "PORTAL.BND", 3860, 31, 18, AO::kPortalTerminatorAOResID, PalId::Default}
    },
    {AnimId::Dust_Particle, {"DUST.BAN", 4488, 61, 44, kDustResID, PalId::Default}, kNullAnimDetails},

    // the loading icons are apparently exactly the same except the maxW/maxH
    {AnimId::Loading_Icon, {"LOADING.BAN", 900, 150, 65, kLoadingResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Loading_Icon2,
        {"LOADING.BAN", 900, 50, 38, kLoadingResID, PalId::Default}, 
        {"LOADING.BAN", 652, 50, 38, AO::kLoadingAOResID, PalId::Default}},

    {AnimId::Vaporize_Particle, {"VAPOR.BAN", 5264, 61, 44, kVaporResID, PalId::Default}, kNullAnimDetails},
    {AnimId::ShootingFire_Particle,
        {"BIGFLASH.BAN", 960, 86, 17, kBigflashResID, PalId::Default},
        {"SLIG.BND", 804, 86, 17, AO::kBigflashAOResID, PalId::Default}},
    {AnimId::ChantOrb_Particle, 
        {"OMMFLARE.BAN", 1632, 39, 21, kOmmflareResID, PalId::Default},
        {"OMMFLARE.BAN", 1492, 38, 21, AO::kOmmflareAOResID, PalId::Default}},
    {AnimId::SquibSmoke_Particle,
        {"SQBSMK.BAN", 5084, 61, 44, kSquibSmokeResID, PalId::Default},
        {"SQBSMK.BAN", 4108, 61, 44, AO::kSquibSmokeAOResID, PalId::Default}},
    {AnimId::Explosion_Rocks, {"DEBRIS00.BAN", 6484, 71, 36, kDebrisID00ResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Explosion_Sticks, {"STICK.BAN", 1704, 49, 29, kStickGibResID, PalId::Default}, kNullAnimDetails},
    {AnimId::Mine_Flash,
        {"MINE.BND", 400, 36, 8, kMineflshResID, PalId::Default},
        {"MINE.BND", 772, 36, 8, AO::kMineflshAOResID, PalId::Default}},
    {AnimId::OptionChantOrb_Particle,
        {"STARTANM.BND", 4176, 92, 47, kOptionFlareResID, PalId::Default},
        {"STARTANM.BND", 7152, 92, 47, AO::kOptionFlareAOResID, PalId::Default}},

    {AnimId::ShootingZFire_Particle,
        {"SLIGZ.BND", 2832, 126, 44, kZflashResID, PalId::Default},
     {"SLIGZ.BND", 2348, 126, 44, AO::kZflashAOResID, PalId::Default}},

    {AnimId::WaterDrop, {"WDROP.BAN", 104, 11, 7, kWaterDropResID, PalId::Default}, kNullAnimDetails},
    {AnimId::WaterSplash, {"SPLASH.BAN", 332, 17, 7, kSplashResID, PalId::Default}, kNullAnimDetails},

    {AnimId::HoistRock1, {"DRPROCK.BAN", 204, 7, 4, kHoistRocks, PalId::Default}, kNullAnimDetails},
    {AnimId::HoistRock2, {"DRPROCK.BAN", 180, 7, 4, kHoistRocks, PalId::Default}, kNullAnimDetails},
    {AnimId::HoistRock3, {"DRPROCK.BAN", 192, 7, 4, kHoistRocks, PalId::Default}, kNullAnimDetails},

    // hoist rocks for any other area except rupture farms
    {AnimId::AO_HoistRock1, kNullAnimDetails, {"DRPROCK.BAN", 164, 7, 4, AO::kHoistRocksAOResID, PalId::Default}},
    {AnimId::AO_HoistRock2, kNullAnimDetails, {"DRPROCK.BAN", 176, 7, 4, AO::kHoistRocksAOResID, PalId::Default}},
    {AnimId::AO_HoistRock3, kNullAnimDetails, {"DRPROCK.BAN", 188, 7, 4, AO::kHoistRocksAOResID, PalId::Default}},

    // hoist rocks at rupture farms
    {AnimId::RuptureFarms_HoistRock1, kNullAnimDetails, {"DRPSPRK.BAN", 240, 5, 4, AO::kHoistRocksAOResID, PalId::Default}},
    {AnimId::RuptureFarms_HoistRock2, kNullAnimDetails, {"DRPSPRK.BAN", 260, 5, 4, AO::kHoistRocksAOResID, PalId::Default}},
    {AnimId::RuptureFarms_HoistRock3, kNullAnimDetails, {"DRPSPRK.BAN", 276, 5, 4, AO::kHoistRocksAOResID, PalId::Default}},

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

    {AnimId::SpotLight, {"SPOTLITE.BAN", 1536, 57, 32, kSpotliteResID, PalId::Default}, kNullAnimDetails},
        
    // blood drop and blood are not the same animations.
    // TODO: check if AO blood.ban is blood or blood drop
    {AnimId::BloodDrop, {"BLOODROP.BAN", 308, 11, 7, kBloodropResID, PalId::Default}, kNullAnimDetails},

    {AnimId::Blood, kNullAnimDetails, { "BLOOD.BAN", 276, 6, 7, AO::kBloodropAOResID, PalId::Default} },
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
        { "SHELL.BAN", 308, 6, 5, AO::kShellAOResID, PalId::Default}
    },
    {AnimId::BigChime, kNullAnimDetails, { "CHIMES.BAN", 19240, 49, 46, AO::kChimeAOResID, PalId::Default} },
    {AnimId::BigChime_Moving, kNullAnimDetails, { "CHIMES.BAN", 19380, 49, 46, AO::kChimeAOResID, PalId::Default} },
    {AnimId::MediumChime, kNullAnimDetails, { "CHIMES.BAN", 19368, 49, 46, AO::kChimeAOResID, PalId::Default} },
    {AnimId::MediumChime_Moving, kNullAnimDetails, { "CHIMES.BAN", 19312, 49, 46, AO::kChimeAOResID, PalId::Default} },
    {AnimId::SmallChime, kNullAnimDetails, { "CHIMES.BAN", 19252, 49, 46, AO::kChimeAOResID, PalId::Default} },
    {AnimId::SmallChime_Moving, kNullAnimDetails, { "CHIMES.BAN", 19264, 49, 46, AO::kChimeAOResID, PalId::Default} },
    {AnimId::Chime_Ball, kNullAnimDetails, { "F2CHMBAL.BAN", 136, 16, 8, AO::kF2ChimeBallAOResID, PalId::Default} },
    {AnimId::Circular_Fade, kNullAnimDetails, { "SPOTLITE.BAN", 2472, 57, 32, AO::kSpotliteAOResID, PalId::Default} },
    {AnimId::DeathFlare_1,
        { "DEADFLR.BAN", 9940, 122, 43, kDeathFlareResID, PalId::Default},
        { "DEADFLR.BAN", 6312, 70, 43, AO::kDeathFlareAOResID, PalId::Default}
    },
    {AnimId::DeathFlare_2,
        { "DEADFLR.BAN", 9912, 122, 43, kDeathFlareResID, PalId::Default},
        { "DEADFLR.BAN", 6284, 70, 43, AO::kDeathFlareAOResID, PalId::Default} },

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
    {AnimId::Fire, 
        { "FIRE.BAN", 5156, 22, 21u, kHubFireResID, PalId::Default},
        { "FIRE.BAN", 5072, 51, 24, AO::kHubFireAOResID, PalId::Default}
    },
    {AnimId::Door_FireBackgroundGlow,
        { "GLOW1.BAN", 1400, 52, 30, kGlowResID, PalId::Default},
        { "GLOW1.BAN", 1344, 52, 30, AO::kGlowAOResID, PalId::Default}
    },
    {AnimId::Door_Lock_Idle, { "DOORLOCK.BAN", 1624, 87, 14, kDoorLockResID_374, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Lock_Open, { "DOORLOCK.BAN", 1608, 87, 14, kDoorLockResID_374, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Mines_Closed, { "DOOR.BAN", 8692, 77, 69, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Mines_Open, { "DOOR.BAN", 8704, 77, 69, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Temple_Closed, { "SVZDOOR.BAN", 5436, 67, 62, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Temple_Open, { "SVZDOOR.BAN", 5448, 67, 62, kF2p3dorResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Train_Closed, { "TRAINDOR.BAN", 4740, 44, 56u, kTrainDoorResID_2013, PalId::Default}, kNullAnimDetails },
    {AnimId::Door_Train_Closing, { "TRAINDOR.BAN", 4752, 44, 56u, kTrainDoorResID_2013, PalId::Default}, kNullAnimDetails },

    {AnimId::Door_RuptureFarms_Open, kNullAnimDetails, { "RDOOR.BND", 6608, 56, 62, AO::kF2p3dorAOResID, PalId::Default}},
    {AnimId::Door_RuptureFarms_Closed, kNullAnimDetails, { "RDOOR.BND", 6632, 56, 62, AO::kF2p3dorAOResID, PalId::Default}},

    {AnimId::Door_Lines_Open, kNullAnimDetails, { "LDOOR.BND", 4760, 55, 48, AO::kF2p3dorAOResID, PalId::Default}},
    {AnimId::Door_Lines_Closed, kNullAnimDetails, { "LDOOR.BND", 4784, 55, 48, AO::kF2p3dorAOResID, PalId::Default}},

    {AnimId::Door_Forest_Open, kNullAnimDetails, { "FDOOR.BND", 6600, 63, 62, AO::kF2p3dorAOResID, PalId::Default}},
    {AnimId::Door_Forest_Closed, kNullAnimDetails, { "FDOOR.BND", 6624, 63, 62, AO::kF2p3dorAOResID, PalId::Default}},

    {AnimId::HubDoor_Forest_Open, kNullAnimDetails, { "FDOOR.BND", 2012, 34, 29, AO::kHubdoorAOResID, PalId::Default}},
    {AnimId::HubDoor_Forest_Closed, kNullAnimDetails, { "FDOOR.BND", 2036, 34, 29, AO::kHubdoorAOResID, PalId::Default}},

    {AnimId::FinalTestDoor_Forest_Open, kNullAnimDetails, { "FDOOR.BND", 2048, 51, 27, AO::kRockdoorAOResID, PalId::Default}},
    {AnimId::FinalTestDoor_Forest_Closed, kNullAnimDetails, { "FDOOR.BND", 2072, 51, 27, AO::kRockdoorAOResID, PalId::Default}},
    
    {AnimId::Door_Desert_Open, kNullAnimDetails, { "DDOOR.BND", 3152, 52, 69, AO::kF2p3dorAOResID, PalId::Default}},
    {AnimId::Door_Desert_Closed, kNullAnimDetails, { "DDOOR.BND", 3176, 52, 69, AO::kF2p3dorAOResID, PalId::Default}},

    {AnimId::HubDoor_Desert_Open, kNullAnimDetails, { "DDOOR.BND", 1024, 21, 29, AO::kHubdoorAOResID, PalId::Default}},
    {AnimId::HubDoor_Desert_Closed, kNullAnimDetails, { "DDOOR.BND", 1048, 21, 29, AO::kHubdoorAOResID, PalId::Default}},

    {AnimId::FinalTestDoor_Desert_Open, kNullAnimDetails, { "DDOOR.BND", 992, 26, 31, AO::kRockdoorAOResID, PalId::Default}},
    {AnimId::FinalTestDoor_Desert_Closed, kNullAnimDetails, { "DDOOR.BND", 1016, 26, 31, AO::kRockdoorAOResID, PalId::Default}},

    {AnimId::Dove_Idle,
        {"DOVBASIC.BAN", 5580, 41, 20, kDovbasicResID, PalId::Default},
        {"DOVBASIC.BAN", 5052, 41, 20, AO::kDovbasicAOResID, PalId::Default}},
    {AnimId::Dove_Flying,
        {"DOVBASIC.BAN", 5516, 41, 20, kDovbasicResID, PalId::Default},
        {"DOVBASIC.BAN", 4988, 41, 20, AO::kDovbasicAOResID, PalId::Default}},
    {AnimId::Electric_Wall,
        {"ELECWALL.BAN", 15384, 50, 80, kElecwallResID, PalId::Default},
        {"ELECWALL.BAN", 14536, 50, 80, AO::kElecwallAOResID, PalId::Default}},
    {AnimId::Explosion, 
        {"EXPLO2.BAN", 51156, 202, 91, kExplo2ResID, PalId::Default},
        {"EXPLO2.BAN", 27376, 200, 91, AO::kExplo2AOResID, PalId::Default}},
    {AnimId::Explosion_Mine, 
        {"EXPLODE.BND", 51588, 214, 49, kBgexpldResID, PalId::Default},
        {"EXPLODE.BND", 51600, 214, 49, AO::kBgexpldAOResID, PalId::Default}},

    {AnimId::Explosion_Small, { "SMEXP.BAN", 14108, 99, 46, kSmallExplo2ResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FallingCrate_Falling, { "FALLBONZ.BAN", 8076, 86, 47, kF2rockResID, PalId::Default}, kNullAnimDetails },
    {AnimId::FallingCrate_Waiting, { "FALLBONZ.BAN", 8100, 86, 47, kF2rockResID, PalId::Default}, kNullAnimDetails },
    {AnimId::AE_FallingRock_Falling, { "FALLROCK.BAN", 4232, 64, 28, kF2rockResID, PalId::Default}, kNullAnimDetails },
    {AnimId::AE_FallingRock_Waiting, { "FALLROCK.BAN", 4256, 64, 28, kF2rockResID, PalId::Default}, kNullAnimDetails },

    {AnimId::FallingMeat_Waiting, kNullAnimDetails, {"R1BARREL.BAN", 12148, 66, 42, AO::kF2rockAOResID, PalId::Default}},
    {AnimId::FallingMeat_Falling, kNullAnimDetails, {"R1BARREL.BAN", 12136, 66, 42, AO::kF2rockAOResID, PalId::Default}},

    {AnimId::AO_FallingRock_Falling, kNullAnimDetails, {"F2ROCK.BAN", 11504, 76, 38, AO::kF2rockAOResID, PalId::Default}},
    {AnimId::AO_FallingRock_Waiting, kNullAnimDetails, {"F2ROCK.BAN", 11544, 76, 38, AO::kF2rockAOResID, PalId::Default}},

    {AnimId::Fart, { "EVILFART.BAN", 3532, 61, 39, kEvilFartResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Foot_Switch_Bonewerkz_Idle, { "BWTRIGGR.BAN", 548, 46, 11, kTriggerResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Foot_Switch_Bonewerkz_Pressed, { "BWTRIGGR.BAN", 568, 46, 11, kTriggerResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Foot_Switch_Industrial_Idle, { "TRIGGER.BAN", 568, 45, 11, kTriggerResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Foot_Switch_Industrial_Pressed, { "TRIGGER.BAN", 588, 45, 11, kTriggerResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Foot_Switch_Temple, kNullAnimDetails, { "TRIGGER.BAN", 744, 68, 15, AO::kTriggerAOResID, PalId::Default} },
    {AnimId::Foot_Switch_Vault_Idle, { "VLTTRIGR.BAN", 972, 72, 18, kTriggerResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Foot_Switch_Vault_Pressed, { "VLTTRIGR.BAN", 992, 72, 18, kTriggerResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Grenade, 
        { "GTHROW.BND", 1252, 17, 11, kGrenadeResID, PalId::Default},
        { "GTHROW.BND", 1132, 17, 11, AO::kGrenadeAOResID, PalId::Default}
    },
    {AnimId::BoomMachine_Button_Off,
        { "GMACHINE.BND", 1736, 26, 17, kR1buttonResID, PalId::Default},
        { "GMACHINE.BND", 1600, 26, 17, AO::kR1buttonAOResID, PalId::Default} },
    {AnimId::BoomMachine_Button_On, 
        { "GMACHINE.BND", 1708, 26, 17, kR1buttonResID, PalId::Default},
        { "GMACHINE.BND", 1572, 26, 17, AO::kR1buttonAOResID, PalId::Default}
    },
    {AnimId::BoomMachine_Nozzle_Idle,
        { "GMACHINE.BND", 3700, 67, 36, kR1bpipeResID, PalId::Default},
        { "GMACHINE.BND", 3616, 66, 36, AO::kR1bpipeAOResID, PalId::Default}
    },
    {AnimId::BoomMachine_Nozzle_DropGrenade,
        { "GMACHINE.BND", 3672, 67, 36, kR1bpipeResID, PalId::Default},
        { "GMACHINE.BND", 3588, 66, 36, AO::kR1bpipeAOResID, PalId::Default} },
    {AnimId::HintFly, kNullAnimDetails, { "HINTFLY.BAN", 556, 10, 7, AO::kHintflyAOResID, PalId::Default} },
    {AnimId::Honey, kNullAnimDetails, { "HONEY.BAN", 3680, 62, 126, AO::kHoneyAOResID, PalId::Default} },
    {AnimId::Honey_Drip, kNullAnimDetails, { "HONEY.BAN", 3628, 62, 126, AO::kHoneyAOResID, PalId::Default} },
    {AnimId::HoneySack_Hanging, kNullAnimDetails, { "HIVE.BND", 9312, 140, 38, AO::kP2c3HiveAOResID, PalId::Default} },
    {AnimId::HoneySack_Falling, kNullAnimDetails, { "HIVE.BND", 9336, 140, 38, AO::kP2c3HiveAOResID, PalId::Default} },
    {AnimId::HoneySack_FallingToSmashed, kNullAnimDetails, { "HIVE.BND", 9280, 140, 38, AO::kP2c3HiveAOResID, PalId::Default} },
    {AnimId::HoneySack_OnGround, kNullAnimDetails, { "HIVE.BND", 9360, 140, 38, AO::kP2c3HiveAOResID, PalId::Default} },
    {AnimId::Lift_Rope, { "ROPES.BAN", 748, 9, 16, kRopesResID, PalId::Default}, kNullAnimDetails },
    // TODO: more rope BAN'S?

    {AnimId::Meat, 
        { "MTHROW.BND", 488, 17, 9, kMeatResID, PalId::Default},
        { "MTHROW.BND", 500, 18, 9, AO::kMeatAOResID, PalId::Default}
    },
    {AnimId::MeatSack_Idle,
        { "MEATSACK.BAN", 15848, 93, 86, kMeatSackResID, PalId::Default},
        { "D2ELUM.BAN", 15688, 92, 86, AO::kMeatSackAOResID, PalId::Default}
    },
    {AnimId::MeatSack_Hit,
        {"MEATSACK.BAN", 15888, 93, 86, kMeatSackResID, PalId::Default},
        { "D2ELUM.BAN", 15728, 92, 86, AO::kMeatSackAOResID, PalId::Default} }, // F2P04C07.CAM
    
    {AnimId::MeatSaw_Idle, kNullAnimDetails, { "R1METSAW.BAN", 15200, 104, 36, AO::kMeatSawAOResID, PalId::Default} },
    {AnimId::MeatSaw_Moving, kNullAnimDetails, { "R1METSAW.BAN", 15232, 104, 36, AO::kMeatSawAOResID, PalId::Default }}, // similar to the one above but this meatsaw anim has a higher rpm
    {AnimId::MeatSawMotor, kNullAnimDetails, {"R1METSAW.BAN", 15252, 104, 36, AO::kMeatSawAOResID, PalId::Default}},

    {AnimId::Mine, 
        { "MINE.BND", 784, 38, 13, kLandmineResID, PalId::Default },
        { "MINE.BND", 748, 38, 13, AO::kLandmineAOResID, PalId::Default }
    },

    {AnimId::MotionDetector_Flare, 
        { "MFLARE.BAN", 1736, 55, 22, kMflareResID, PalId::Default},
        { "MFLARE.BAN", 1108, 32, 22, AO::kMflareAOResID, PalId::Default}
    },
    {AnimId::MotionDetector_Laser, 
        { "MOTION.BAN", 23280, 37, 60, kMotionResID, PalId::Default},
        { "MOTION.BAN", 23660, 37, 60, AO::kMotionAOResID, PalId::Default}
    },

    {AnimId::MovingBomb, 
        { "MOVEBOMB.BAN", 17548, 76, 30, kMovebombResID, PalId::Default},
        { "MOVEBOMB.BAN", 17140, 76, 30, AO::kMovebombAOResID, PalId::Default}
    },

    {AnimId::ParamiteWeb, 
        { "WEB.BAN", 148, 5, 16, kWebResID, PalId::Default},
        { "WEB.BAN", 88, 3, 16, AO::kWebAOResID, PalId::Default}
    },
    {AnimId::Pullring_Desert_Idle, kNullAnimDetails, { "PULLRING.BAN", 2912, 32, 22, AO::kPullringAOResID, PalId::Default} },
    {AnimId::Pullring_Desert_UseBegin, kNullAnimDetails, { "PULLRING.BAN", 2952, 32, 22, AO::kPullringAOResID, PalId::Default} },
    {AnimId::Pullring_Desert_UseEnd, kNullAnimDetails, { "PULLRING.BAN", 2984, 32, 22, AO::kPullringAOResID, PalId::Default} },

    {AnimId::Pullring_Farms_Idle, kNullAnimDetails, { "R1PULRNG.BAN", 4832, 33, 35, AO::kPullringAOResID, PalId::Default} },
    {AnimId::Pullring_Farms_UseBegin, kNullAnimDetails, { "R1PULRNG.BAN", 4872, 33, 35, AO::kPullringAOResID, PalId::Default} },
    {AnimId::Pullring_Farms_UseEnd, kNullAnimDetails, { "R1PULRNG.BAN", 4904, 33, 35, AO::kPullringAOResID, PalId::Default} },

    {AnimId::Rock, 
        { "RTHROW.BND", 488, 17, 9, kAberockResID, PalId::Default},
        { "RTHROW.BND", 500, 18, 9, AO::kAberockAOResID, PalId::Default}
    },
    {AnimId::RockSack_Idle, 
        { "ROCKBAG.BAN", 29748, 71, 60u, kP2c2bagResID, PalId::Default},
        { "E1P02C03.CAM", 13756, 71, 60, AO::kP2c2bagAOResID, PalId::Default} // E1P02C03.CAM
    },
    {AnimId::RockSack_SoftHit, 
        { "ROCKBAG.BAN", 29772, 71, 60u, kP2c2bagResID, PalId::Default},
        { "E1P02C03.CAM", 13780, 71, 60, AO::kP2c2bagAOResID, PalId::Default} },
    {AnimId::RockSack_HardHit,
        { "ROCKBAG.BAN", 29700, 71, 60u, kP2c2bagResID, PalId::Default},
        { "E1P02C03.CAM", 13708, 71, 60, AO::kP2c2bagAOResID, PalId::Default} },

    {AnimId::Rope_R1, kNullAnimDetails, { "R1ROPES.BAN", 636, 4, 16, AO::kRopesAOResID, PalId::Default} },
    {AnimId::Rope_Lines, kNullAnimDetails, { "ROPES.BAN", 680, 5, 16, AO::kRopesAOResID, PalId::Default} },

    {AnimId::Security_Claw_Lower_Open, kNullAnimDetails, { "F2EYEORB.BAN", 22420, 152, 31, AO::kF2eyeorbAOResID, PalId::Default} },
    {AnimId::Security_Claw_Lower_Close, kNullAnimDetails, { "F2EYEORB.BAN", 22568, 152, 31, AO::kF2eyeorbAOResID, PalId::Default} },
    {AnimId::Security_Claw_Lower_Idle, kNullAnimDetails, { "F2EYEORB.BAN", 22468, 152, 31, AO::kF2eyeorbAOResID, PalId::Default} },
    {AnimId::Security_Claw_Upper_Rotating, kNullAnimDetails, { "F2EYEORB.BAN", 22480, 152, 31, AO::kF2eyeorbAOResID, PalId::Default} },
    {AnimId::Security_Claw_Upper_NoRotation, kNullAnimDetails, { "F2EYEORB.BAN", 22616, 152, 31, AO::kF2eyeorbAOResID, PalId::Default} },
    {AnimId::Security_Door, kNullAnimDetails, { "R1SDOS.BAN", 976, 70, 19, AO::kR1sdosAOResID_6027, PalId::Default} },
    {AnimId::Security_Door_Idle, { "SECDOOR.BAN", 1512, 70, 19u, kR1sdosResID_6027, PalId::Default}, kNullAnimDetails },
    {AnimId::Security_Door_Speak, { "SECDOOR.BAN", 1528, 70, 19u, kR1sdosResID_6027, PalId::Default}, kNullAnimDetails },
    {AnimId::Security_Orb, 
        { "MAIMORB.BAN", 2228, 53, 28u, kMaimGameResID, PalId::Default},
        { "F2MAMORB.BAN", 10864, 53, 28, AO::kMaimGameAOResID, PalId::Default}
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
    {AnimId::Star, kNullAnimDetails, { "STAR.BAN", 748, 20, 12, AO::kBGStarAOResID, PalId::Default} },

    {AnimId::Status_Light_Green, { "STATUSLT.BAN", 240, 14, 9u, kStatusLightResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Status_Light_Red, { "STATUSLT.BAN", 252, 14, 9u, kStatusLightResID, PalId::Default}, kNullAnimDetails },

    {AnimId::Stone_Ball, kNullAnimDetails, { "F2STNBAL.BAN", 15596, 131, 64, AO::kF2stnbalAOResID, PalId::Default} },
    {AnimId::Stone_Ball_Stopper, kNullAnimDetails, { "F2STNSCK.BAN", 1256, 106, 73, AO::kF2stnsckAOResID, PalId::Default} },
    {AnimId::Swinging_Ball_Fast, kNullAnimDetails, { "F2ZBALL.BAN", 72288, 143, 182, AO::kF2zballAOResID, PalId::Default} },
    {AnimId::Swinging_Ball_Normal, kNullAnimDetails, { "F2ZBALL.BAN", 72172, 143, 182, AO::kF2zballAOResID, PalId::Default} },
    {AnimId::Swinging_Ball_Slow, kNullAnimDetails, { "F2ZBALL.BAN", 72400, 143, 182, AO::kF2zballAOResID, PalId::Default} },

    {AnimId::Lever_Idle,
        { "SWITCH1.BAN", 5696, 66, 40, kLeverResID, PalId::Default},
     {"SWITCH1.BAN", 16272, 66, 41, AO::kLeverAOResID, PalId::Default}},
    {AnimId::Lever_Pull_Left,
        { "SWITCH1.BAN", 5708, 66, 40, kLeverResID, PalId::Default},
     {"SWITCH1.BAN", 16284, 66, 41, AO::kLeverAOResID, PalId::Default}},
    {AnimId::Lever_Pull_Release_Left,
        { "SWITCH1.BAN", 5760, 66, 40, kLeverResID, PalId::Default},
     {"SWITCH1.BAN", 16336, 66, 41, AO::kLeverAOResID, PalId::Default}},
    {AnimId::Lever_Pull_Right,
        { "SWITCH1.BAN", 5796, 66, 40, kLeverResID, PalId::Default},
     {"SWITCH1.BAN", 16372, 66, 41, AO::kLeverAOResID, PalId::Default}},
    {AnimId::Lever_Pull_Release_Right,
        { "SWITCH1.BAN", 5848, 66, 40, kLeverResID, PalId::Default},
     {"SWITCH1.BAN", 16424, 66, 41, AO::kLeverAOResID, PalId::Default}},

    {AnimId::RuptureFarms_Lever_Idle, kNullAnimDetails, {"R1SWITCH.BAN", 16168, 67, 40, AO::kLeverAOResID, PalId::Default}},
    {AnimId::RuptureFarms_Lever_Pull_Left, kNullAnimDetails, {"R1SWITCH.BAN", 16180, 67, 40, AO::kLeverAOResID, PalId::Default}},
    {AnimId::RuptureFarms_Lever_Pull_Release_Left, kNullAnimDetails, {"R1SWITCH.BAN", 16232, 67, 40, AO::kLeverAOResID, PalId::Default}},
    {AnimId::RuptureFarms_Lever_Pull_Right, kNullAnimDetails, {"R1SWITCH.BAN", 16268, 67, 40, AO::kLeverAOResID, PalId::Default}},
    {AnimId::RuptureFarms_Lever_Pull_Release_Right, kNullAnimDetails, {"R1SWITCH.BAN", 16320, 67, 40, AO::kLeverAOResID, PalId::Default}},

    {AnimId::Lines_Lever_Idle, kNullAnimDetails, {"L1SWITCH.BAN", 5440, 66, 41, AO::kLeverAOResID, PalId::Default}},
    {AnimId::Lines_Lever_Pull_Left, kNullAnimDetails, {"L1SWITCH.BAN", 5576, 66, 41, AO::kLeverAOResID, PalId::Default}},
    {AnimId::Lines_Lever_Pull_Release_Left, kNullAnimDetails, {"L1SWITCH.BAN", 5452, 66, 41, AO::kLeverAOResID, PalId::Default}},
    {AnimId::Lines_Lever_Pull_Right, kNullAnimDetails, {"L1SWITCH.BAN", 5488, 66, 41, AO::kLeverAOResID, PalId::Default}},
    {AnimId::Lines_Lever_Pull_Release_Right, kNullAnimDetails, {"L1SWITCH.BAN", 5540, 66, 41, AO::kLeverAOResID, PalId::Default}},

    {AnimId::TimedMine_Activated, { "BOMB.BND", 848, 35, 17, kBombResID, PalId::Default}, kNullAnimDetails },
    {AnimId::TimedMine_Idle, 
        { "BOMB.BND", 836, 35, 17, kBombResID, PalId::Default},
        { "BOMB.BND", 792, 35, 17, AO::kBombAOResID, PalId::Default}
    },

    {AnimId::Tortured_Mudokon, { "MUDTORT.BAN", 20892, 101, 68, kTorturedMud, PalId::Default}, kNullAnimDetails },
    {AnimId::Tortured_Mudokon_Released, { "MUDTORT.BAN", 20864, 101, 68, kTorturedMud, PalId::Default}, kNullAnimDetails },
    {AnimId::Tortured_Mudokon_Zap, { "MUDTORT.BAN", 21000, 101, 68, kTorturedMud, PalId::Default}, kNullAnimDetails },
    {AnimId::Tortured_Mudokon_Tears, {"TEARS.BAN", 500, 17, 19, kTorturedMudTearsResID, PalId::Default}, kNullAnimDetails},

    { AnimId::Trap_Door_Open, { "TRAPDOOR.BAN", 4800, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Trap_Door_Closed, { "TRAPDOOR.BAN", 4788, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Trap_Door_Closing, { "TRAPDOOR.BAN", 4812, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Trap_Door_Opening, { "TRAPDOOR.BAN", 4840, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Trap_Door_Tribal_Closed, { "VLTSTRAP.BAN", 5328, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Trap_Door_Tribal_Closing, { "VLTSTRAP.BAN", 5368, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Trap_Door_Tribal_Open, { "VLTSTRAP.BAN", 5316, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Trap_Door_Tribal_Opening, { "VLTSTRAP.BAN", 5340, 72, 41, kP6c1trapResID, PalId::Default}, kNullAnimDetails },

    {AnimId::R1_TrapDoor_Open, kNullAnimDetails, { "R1TRAP.BAN", 4700, 72, 41, AO::kP6c1trapAOResID, PalId::Default} },
    {AnimId::R1_TrapDoor_Closed, kNullAnimDetails, { "R1TRAP.BAN", 4688, 72, 41, AO::kP6c1trapAOResID, PalId::Default} },
    {AnimId::R1_TrapDoor_Opening, kNullAnimDetails, { "R1TRAP.BAN", 4740, 72, 41, AO::kP6c1trapAOResID, PalId::Default} },
    {AnimId::R1_TrapDoor_Closing, kNullAnimDetails, { "R1TRAP.BAN", 4712, 72, 41, AO::kP6c1trapAOResID, PalId::Default} },
    {AnimId::Lines_TrapDoor_Open, kNullAnimDetails, { "P6C1TRAP.BAN", 4644, 155, 59, AO::kP6c1trapAOResID, PalId::Default} },
    {AnimId::Lines_TrapDoor_Closed, kNullAnimDetails, { "P6C1TRAP.BAN", 4632, 155, 59, AO::kP6c1trapAOResID, PalId::Default} },
    {AnimId::Lines_TrapDoor_Opening, kNullAnimDetails, { "P6C1TRAP.BAN", 4680, 155, 59, AO::kP6c1trapAOResID, PalId::Default} },
    {AnimId::Lines_TrapDoor_Closing, kNullAnimDetails, { "P6C1TRAP.BAN", 4656, 155, 59, AO::kP6c1trapAOResID, PalId::Default} },
    {AnimId::Desert_TrapDoor_Open, kNullAnimDetails, { "D2TRAP.BAN", 1976, 99, 56, AO::kP6c1trapAOResID, PalId::Default} },
    {AnimId::Desert_TrapDoor_Closed, kNullAnimDetails, { "D2TRAP.BAN", 1964, 99, 56, AO::kP6c1trapAOResID, PalId::Default} },
    {AnimId::Desert_TrapDoor_Opening, kNullAnimDetails, { "D2TRAP.BAN", 2012, 99, 56, AO::kP6c1trapAOResID, PalId::Default} },
    {AnimId::Desert_TrapDoor_Closing, kNullAnimDetails, { "D2TRAP.BAN", 1988, 99, 56, AO::kP6c1trapAOResID, PalId::Default} },

    {AnimId::Bomb_Flash,
        {"BOMB.BND", 556, 36, 21, kBombflshResID, PalId::Default},
        {"BOMB.BND", 384, 37, 21, AO::kBombflshAOResID, PalId::Default} },
    {AnimId::Bomb_RedGreenTick,
        {"BOMB.BND", 544, 36, 21, kBombflshResID, PalId::Default},
        {"BOMB.BND", 372, 37, 21, AO::kBombflshAOResID, PalId::Default}},

    {AnimId::UXB_Active,
        { "UXB.BND", 8048, 59, 19, kUXBResID, PalId::Default},
        { "UXB.BND", 7740, 59, 19, AO::kUXBAOResID, PalId::Default}
    },
    {AnimId::UXB_Toggle,
        { "UXB.BND", 8120, 59, 19, kUXBResID, PalId::Default},
        { "UXB.BND", 7812, 59, 19, AO::kUXBAOResID, PalId::Default} },
    {AnimId::UXB_Disabled,
        { "UXB.BND", 8192, 59, 19, kUXBResID, PalId::Default}, 
        { "UXB.BND", 7884, 59, 19, AO::kUXBAOResID, PalId::Default} },
    {AnimId::Well_Leaf,
        {"WELLLEAF.BAN", 476, 16, 11, kWellLeafResID, PalId::Default},
        {"WELLLEAF.BAN", 428, 9, 11, AO::kWellLeafAOResID, PalId::Default}},

    {AnimId::Work_Wheel_Idle, { "WORKWHEL.BAN", 2716, 49, 24, kWorkwhelResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Work_Wheel_Turning, { "WORKWHEL.BAN", 2728, 49, 24, kWorkwhelResID, PalId::Default}, kNullAnimDetails },
    {AnimId::Zap_Line_Blue,
        {"SPLINE.BAN", 276, 14, 9, kSplineResID, PalId::Default},
        {"SPLINE.BAN", 228, 9, 9, AO::kSplineAOResID, PalId::Default}},
    {AnimId::Zap_Line_Red,
        {"SPLINE.BAN", 288, 14, 9, kSplineResID, PalId::Default},
        {"SPLINE.BAN", 240, 9, 9, AO::kSplineAOResID, PalId::Default}},

    {AnimId::Zap_Sparks,
        {"OMMFLARE.BAN", 1672, 39, 21, kOmmflareResID, PalId::Default},
        {"OMMFLARE.BAN", 1532, 38, 21, kOmmflareResID, PalId::Default}},

    {AnimId::GoldGlow, kNullAnimDetails, {"GOLDGLOW.BAN", 236, 19, 15, AO::kGoldGlowAOResID_6011, PalId::Default}},
    {AnimId::GreenGlow, kNullAnimDetails, {"GRENGLOW.BAN", 96, 5, 6, AO::kGreenGlowAOResID_6010, PalId::Default}},
    {AnimId::FlintGlow, kNullAnimDetails, {"FLNT.BAN", 756, 59, 54, AO::kFlintGlowAOResID_6028, PalId::Default}},

    {AnimId::GreenDoorLight, kNullAnimDetails, {"HUBLIGHT.BAN", 448, 20, 11, AO::kGreenDoorLightAOResID_6031, PalId::Default}},
    {AnimId::GreenHubLight, kNullAnimDetails, {"HUBLIGHT.BAN", 460, 20, 11, AO::kGreenDoorLightAOResID_6031, PalId::Default}},

    {AnimId::RedDoorLight, kNullAnimDetails, {"HUBRED.BAN", 460, 20, 11, AO::kRedDoorLightAOResID_6032, PalId::Default}},
    {AnimId::RedHubLight, kNullAnimDetails, {"HUBRED.BAN", 448, 20, 11, AO::kRedDoorLightAOResID_6032, PalId::Default}},


    {AnimId::FlintLock_Hammers_Activating, kNullAnimDetails, {"F2FLINT.BAN", 11848, 125, 59, AO::kFlintLockAOResID, PalId::Default}},
    {AnimId::FlintLock_Hammers_Disabled, kNullAnimDetails, {"F2FLINT.BAN", 11836, 125, 59, AO::kFlintLockAOResID, PalId::Default}},
    {AnimId::FlintLock_Gourd, kNullAnimDetails, {"GOURD.BAN", 9760, 105, 84, AO::kGourdAOResID, PalId::Default}},

    {AnimId::LiftPlatform_RuptureFarms, kNullAnimDetails, {"RLIFT.BND", 1260, 117, 19, AO::kLiftAOResID, PalId::Default}},
    {AnimId::LiftPlatform_Lines, kNullAnimDetails, {"LLIFT.BND", 1524, 114, 25, AO::kLiftAOResID, PalId::Default}},
    {AnimId::LiftPlatform_Forest, kNullAnimDetails, {"FLIFT.BND", 940, 122, 12, AO::kLiftAOResID, PalId::Default}},
    {AnimId::LiftPlatform_Desert, kNullAnimDetails, {"D1LIFT.BND", 1472, 116, 20, AO::kLiftAOResID, PalId::Default}},
    {AnimId::LiftPlatform_Desert2, kNullAnimDetails, {"D2LIFT.BND", 1472, 116, 20, AO::kLiftAOResID, PalId::Default}},

    {AnimId::LiftBottomWheel_RuptureFarms, kNullAnimDetails, {"RLIFT.BND", 24204, 124, 37, AO::kLiftWheelsAOResID, PalId::Default}},
    {AnimId::LiftBottomWheel_Lines, kNullAnimDetails, {"LLIFT.BND", 17108, 62, 46, AO::kLiftWheelsAOResID, PalId::Default}},
    {AnimId::LiftBottomWheel_Forest, kNullAnimDetails, {"FLIFT.BND", 17108, 62, 46, AO::kLiftWheelsAOResID, PalId::Default}},
    {AnimId::LiftBottomWheel_Desert, kNullAnimDetails, {"D1LIFT.BND", 21900, 73, 55, AO::kLiftWheelsAOResID, PalId::Default}},
    {AnimId::LiftBottomWheel_Desert2, kNullAnimDetails, {"D2LIFT.BND", 21900, 73, 55, AO::kLiftWheelsAOResID, PalId::Default}},

    {AnimId::LiftTopWheel_RuptureFarms, kNullAnimDetails, {"RLIFT.BND", 24240, 124, 37, AO::kLiftWheelsAOResID, PalId::Default}},
    {AnimId::LiftTopWheel_Lines, kNullAnimDetails, {"LLIFT.BND", 17144, 62, 46, AO::kLiftWheelsAOResID, PalId::Default}},
    {AnimId::LiftTopWheel_Forest, kNullAnimDetails, {"FLIFT.BND", 17144, 62, 46, AO::kLiftWheelsAOResID, PalId::Default}},
    {AnimId::LiftTopWheel_Desert, kNullAnimDetails, {"D1LIFT.BND", 21936, 73, 55, AO::kLiftWheelsAOResID, PalId::Default}},
    {AnimId::LiftTopWheel_Desert2, kNullAnimDetails, {"D2LIFT.BND", 21936, 73, 55, AO::kLiftWheelsAOResID, PalId::Default}},

    // files only exist in F1P01C04 and D1P01C03.CAM
    {AnimId::BellHammer_Idle, kNullAnimDetails, {"D1P01C03.CAM", 4488, 71, 69, AO::kHammerAOResID, PalId::Default}},
    {AnimId::BellHammer_Smashing, kNullAnimDetails, {"D1P01C03.CAM", 4500, 71, 69, AO::kHammerAOResID, PalId::Default}},

    {AnimId::ObjectShadow,
        {"SHADOW.BAN", 240, 64, 6, kObjectShadowResID, PalId::Default},
        {"SHADOW.BAN", 296, 37, 6, kObjectShadowResID, PalId::Default}},

    { AnimId::Elum_Idle, kNullAnimDetails, {"ELMBASIC.BAN", 117036, 169, 169, AO::kElmbasicAOResID_200, PalId::Default} },
    { AnimId::Elum_WalkLoop, kNullAnimDetails, {"ELMBASIC.BAN", 117152, 169, 169, AO::kElmbasicAOResID_200, PalId::Default} },
    { AnimId::Elum_Turn, kNullAnimDetails, {"ELMBASIC.BAN", 116980, 169, 169, AO::kElmbasicAOResID_200, PalId::Default} },
    { AnimId::Elum_WalkToIdle, kNullAnimDetails, {"ELMBASIC.BAN", 117112, 169, 169, AO::kElmbasicAOResID_200, PalId::Default} },
    { AnimId::Elum_MidWalkToIdle, kNullAnimDetails, {"ELMBASIC.BAN", 117132, 169, 169, AO::kElmbasicAOResID_200, PalId::Default} },
    { AnimId::Elum_IdleToWalk2, kNullAnimDetails, {"ELMBASIC.BAN", 116928, 169, 169, AO::kElmbasicAOResID_200, PalId::Default} },
    { AnimId::Elum_IdleToWalk1, kNullAnimDetails, {"ELMBASIC.BAN", 116948, 169, 169, AO::kElmbasicAOResID_200, PalId::Default} },
    { AnimId::Elum_ToYell, kNullAnimDetails, {"ELMBASIC.BAN", 117068, 169, 169, AO::kElmbasicAOResID_200, PalId::Default} },
    { AnimId::Elum_Yell, kNullAnimDetails, {"ELMBASIC.BAN", 117288, 169, 169, AO::kElmbasicAOResID_200, PalId::Default} },
    { AnimId::Elum_Unknown1, kNullAnimDetails, {"ELMBASIC.BAN", 117464, 169, 169, AO::kElmbasicAOResID_200, PalId::Default} },
    { AnimId::Elum_RunTurn, kNullAnimDetails, {"ELMBASIC.BAN", 117232, 169, 169, AO::kElmbasicAOResID_200, PalId::Default} },
    { AnimId::Elum_RunTurnToWalk, kNullAnimDetails, {"ELMBASIC.BAN", 117092, 169, 169, AO::kElmbasicAOResID_200, PalId::Default} },
    { AnimId::Elum_Speak, kNullAnimDetails, {"ELMBASIC.BAN", 117496, 169, 169, AO::kElmbasicAOResID_200, PalId::Default} },

    { AnimId::Elum_Land, kNullAnimDetails, {"ELMFALL.BAN", 40404, 169, 169, AO::kElmfallAOResID_216, PalId::Default} },
    { AnimId::Elum_RunOffEdge, kNullAnimDetails, {"ELMFALL.BAN", 40428, 169, 169, AO::kElmfallAOResID_216, PalId::Default} },
    { AnimId::Elum_WalkOffEdge, kNullAnimDetails, {"ELMFALL.BAN", 40376, 169, 169, AO::kElmfallAOResID_216, PalId::Default} },
    { AnimId::Elum_JumpToFall, kNullAnimDetails, {"ELMFALL.BAN", 40484, 169, 169, AO::kElmfallAOResID_216, PalId::Default} },

    { AnimId::Elum_LickingHoney, kNullAnimDetails, {"ELMHONEY.BAN", 19988, 169, 169, AO::kElmHoneyAOResID, PalId::Default} },
    { AnimId::Elum_LickingToStruggle, kNullAnimDetails, {"ELMHONEY.BAN", 20044, 169, 169, AO::kElmHoneyAOResID, PalId::Default} },

    { AnimId::Elum_AbeMountingEnd, kNullAnimDetails, {"ANEMOUNT.BND", 49620, 169, 169, AO::kElumAneMountAOResID_223, PalId::Default} }, // this file name might or might not be wrong?

    { AnimId::Elum_AbeUnmountingEnd, kNullAnimDetails, {"ANEDSMNT.BND", 47728, 169, 169, AO::kElumANEDSMNTAOResID_224, PalId::Default} }, // dito

    { AnimId::Elum_BeesStruggling, kNullAnimDetails, {"ELMWASP.BAN", 17568, 169, 169, AO::kElmWaspAOResID_204, PalId::Default} },

    { AnimId::Elum_HopBegin, kNullAnimDetails, {"ELMRIDE.BAN", 127896, 169, 169, AO::kElumRideAOResID_220, PalId::Default} },
    { AnimId::Elum_HopMid, kNullAnimDetails, {"ELMRIDE.BAN", 127924, 169, 169, AO::kElumRideAOResID_220, PalId::Default} },
    { AnimId::Elum_HopLand, kNullAnimDetails, {"ELMRIDE.BAN", 127956, 169, 169, AO::kElumRideAOResID_220, PalId::Default} },
    { AnimId::Elum_RunJumpBegin, kNullAnimDetails, {"ELMRIDE.BAN", 127980, 169, 169, AO::kElumRideAOResID_220, PalId::Default} },
    { AnimId::Elum_RunJumpMid, kNullAnimDetails, {"ELMRIDE.BAN", 128000, 169, 169, AO::kElumRideAOResID_220, PalId::Default} },
    { AnimId::Elum_RunJumpLand, kNullAnimDetails, {"ELMRIDE.BAN", 128048, 169, 169, AO::kElumRideAOResID_220, PalId::Default} },
    { AnimId::Elum_RunLoop, kNullAnimDetails, {"ELMRIDE.BAN", 128128, 169, 169, AO::kElumRideAOResID_220, PalId::Default} },
    { AnimId::Elum_RunSlideStop, kNullAnimDetails, {"ELMRIDE.BAN", 128168, 169, 169, AO::kElumRideAOResID_220, PalId::Default} },
    { AnimId::Elum_RunTurnToRun, kNullAnimDetails, {"ELMRIDE.BAN", 128068, 169, 169, AO::kElumRideAOResID_220, PalId::Default} },
    { AnimId::Elum_IdleToRun, kNullAnimDetails, {"ELMRIDE.BAN", 127876, 169, 169, AO::kElumRideAOResID_220, PalId::Default} },
    { AnimId::Elum_WalkToRun, kNullAnimDetails, {"ELMRIDE.BAN", 127816, 169, 169, AO::kElumRideAOResID_220, PalId::Default} },
    { AnimId::Elum_MidWalkToRun, kNullAnimDetails, {"ELMRIDE.BAN", 127796, 169, 169, AO::kElumRideAOResID_220, PalId::Default} },
    { AnimId::Elum_RunToWalk, kNullAnimDetails, {"ELMRIDE.BAN", 127836, 169, 169, AO::kElumRideAOResID_220, PalId::Default} },
    { AnimId::Elum_MidRunToWalk, kNullAnimDetails, {"ELMRIDE.BAN", 127856, 169, 169, AO::kElumRideAOResID_220, PalId::Default} },

    { AnimId::Elum_ScratchBegin, kNullAnimDetails, {"ELMALONE.BAN", 18132, 169, 169, AO::kElmaloneAOResID_230, PalId::Default} },
    { AnimId::Elum_ScratchLoop, kNullAnimDetails, {"ELMALONE.BAN", 18188, 169, 169, AO::kElmaloneAOResID_230, PalId::Default} },
    { AnimId::Elum_ScratchEnd, kNullAnimDetails, {"ELMALONE.BAN", 18160, 169, 169, AO::kElmaloneAOResID_230, PalId::Default} },
    { AnimId::Elum_ScratchUnknown, kNullAnimDetails, {"ELMALONE.BAN", 18236, 169, 169, AO::kElmaloneAOResID_230, PalId::Default} },

    { AnimId::Elum_MountUnmountBegin, kNullAnimDetails, {"ELMPDMNT.BAN", 7460, 169, 169, AO::kElumPdmntAOResID_221, PalId::Default} },

    { AnimId::Elum_Knockback, kNullAnimDetails, {"ELMKNBK.BAN", 21280, 169, 169, AO::kElmnknbkAOResID, PalId::Default} },

    // search for "Animation test code" in Abe.cpp and uncomment the code below to activate the anim tester
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    {AnimId::Anim_Tester,
     {"SLIGZ.BND", 2832, 126, 44, kZflashResID, PalId::Default},
     {"RLIFT.BND", 24240, 124, 37, AO::kLiftWheelsAOResID, PalId::Default}
    },
    ////////////////////////////////////////////////////////////////////////////////////////////////////
};

void FrameTableOffsetExists(int frameTableOffset, bool isAe, int maxW, int maxH)
{
    for (const auto& entry : kBgAnimRecords)
    {
        if (isAe)
        {
            if (entry.mAEData.mFrameTableOffset == frameTableOffset &&
                entry.mAEData.mMaxW == maxW &&
                entry.mAEData.mMaxH == maxH)
            {
                return;
            }
        }
        else
        {
            if (entry.mAOData.mFrameTableOffset == frameTableOffset &&
                entry.mAOData.mMaxW == maxW &&
                entry.mAOData.mMaxH == maxH)
            {
                return;
            }
        }
    }

    for (const auto& entry : kAnimRecords)
    {
        if (isAe)
        {
            if (entry.mAEData.mFrameTableOffset == frameTableOffset &&
                entry.mAEData.mMaxW == maxW &&
                entry.mAEData.mMaxH == maxH)
            {
                return;
            }

            // special handling for some weird OG behavior (see Explosion.cpp line 184)
            if (entry.mId == AnimId::Explosion_Small)
            {
                return;
            }
        }
        else
        {
            if (entry.mAOData.mFrameTableOffset == frameTableOffset &&
                entry.mAOData.mMaxW == maxW &&
                entry.mAOData.mMaxH == maxH)
            {
                return;
            }
        }
    }
    LOG_INFO("couldn't find AnimId for framtableoffset: " << frameTableOffset << " maxW " << maxW << " maxH " << maxH);
}

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

static const BgAnimRecord BgAnimRec(bool isAe, s32 toFind)
{
    for (const CombinedBgAnimRecord& anim : kBgAnimRecords)
    {
        if (anim.mBgAnimId == toFind)
        {
            const BgAnimDetails& data = isAe ? anim.mAEData : anim.mAOData;
            return BgAnimRecord{ anim.mBgAnimId, data.mBanName, data.mFrameTableOffset, data.mMaxW, data.mMaxH };
        }
    }
    LOG_ERROR("couldn't find bg anim id " << toFind);
    ALIVE_FATAL("missing background animation entry for anim id");
}

const BgAnimRecord BgAnimRec(s32 toFind)
{
    return BgAnimRec(true, toFind);
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
        return ::PalRec(false, toFind);
    }

    const AnimRecord AnimRec(AnimId toFind)
    {
        return ::AnimRec(false, toFind);
    }

    const BgAnimRecord BgAnimRec(s32 toFind)
    {
        return ::BgAnimRec(false, toFind);
    }
}
