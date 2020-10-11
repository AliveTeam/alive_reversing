#include "stdafx_common.h"
#include "AnimResources.hpp"
#include "Sys_common.hpp"

constexpr PalRecord kPalRecords[] =
{
    { PalId::BlindMud, "MUDPAL.BND", kMudblindResID }
};

constexpr AnimRecord kAnimRecords[] =
{
    { AnimId::Abe_Head_Gib, "ABEBLOW.BAN", 7732, 50, 25, kAbeblowResID, PalId::Default },
    { AnimId::Abe_Arm_Gib, "ABEBLOW.BAN", 7772, 50, 25, kAbeblowResID, PalId::Default },
    { AnimId::Abe_Body_Gib, "ABEBLOW.BAN", 7812, 50, 25, kAbeblowResID, PalId::Default },
    
    { AnimId::BlindMud_Head_Gib, "ABEBLOW.BAN", 7732, 50, 25, kAbeblowResID, PalId::BlindMud },
    { AnimId::BlindMud_Arm_Gib, "ABEBLOW.BAN", 7772, 50, 25, kAbeblowResID, PalId::BlindMud },
    { AnimId::BlindMud_Body_Gib, "ABEBLOW.BAN", 7812, 50, 25, kAbeblowResID, PalId::BlindMud },

    { AnimId::Glukkon_Head_Gib, "GLUKKON.BND", 8140, 48, 28, kUnknownResID_801, PalId::Default },
    { AnimId::Glukkon_Arm_Gib, "GLUKKON.BND", 8188, 48, 28, kUnknownResID_801, PalId::Default },
    { AnimId::Glukkon_Body_Gib, "GLUKKON.BND", 8188, 48, 28, kUnknownResID_801, PalId::Default },

    { AnimId::Aslik_Head_Gib, "ASLIK.BND", 8140, 48, 28, kUnknownResID_803, PalId::Default },
    { AnimId::Aslik_Arm_Gib, "ASLIK.BND", 8188, 48, 28, kUnknownResID_803, PalId::Default },
    { AnimId::Aslik_Body_Gib, "ASLIK.BND", 8188, 48, 28, kUnknownResID_803, PalId::Default },

    { AnimId::Dripik_Head_Gib, "DRIPIK.BND", 8140, 48, 28, kUnknownResID_805, PalId::Default },
    { AnimId::Dripik_Arm_Gib, "DRIPIK.BND", 8188, 48, 28, kUnknownResID_805, PalId::Default },
    { AnimId::Dripik_Body_Gib, "DRIPIK.BND", 8188, 48, 28, kUnknownResID_805, PalId::Default },

    { AnimId::Phleg_Head_Gib, "PHLEG.BND", 8140, 48, 28, kUnknownResID_807, PalId::Default },
    { AnimId::Phleg_Arm_Gib, "PHLEG.BND", 8188, 48, 28, kUnknownResID_807, PalId::Default },
    { AnimId::Phleg_Body_Gib, "PHLEG.BND", 8188, 48, 28, kUnknownResID_807, PalId::Default },

    { AnimId::Fleech_Head_Gib, "FLEEBLOW.BAN", 1088, 19, 8, kUnknownResID_580, PalId::Default },
    { AnimId::Fleech_Body_Gib, "FLEEBLOW.BAN", 1128, 19, 8, kUnknownResID_580, PalId::Default },
    // Note: Fleech hasn't got arms

    { AnimId::Metal_Gib, "METAL.BAN", 2244, 38, 29, kMetalResID, PalId::Default },

    { AnimId::Slig_Head_Gib, "SLGBLOW.BAN", 6480, 44, 26, kSligBlowResID, PalId::Default },
    { AnimId::Slig_Arm_Gib, "SLGBLOW.BAN", 6560, 44, 26, kSligBlowResID, PalId::Default },
    { AnimId::Slig_Body_Gib, "SLGBLOW.BAN", 6520, 44, 26, kSligBlowResID, PalId::Default },

    { AnimId::Slog_Head_Gib, "DOGBLOW.BAN", 7504, 53, 28, kSlogBlowResID, PalId::Default },
    { AnimId::Slog_Body_Gib, "DOGBLOW.BAN", 7544, 53, 28, kSlogBlowResID, PalId::Default },
    // Note: Slog hasn't got arms (apparently)

	// Note: Fleech animations are sorted by order of appearance under Fleech.ccp (line 242)
	// Lots of idle animations, they might be used for other things but I can't immediately identify their use otherwise.
    { AnimId::Fleech_Idle_A, "FLEECH.BAN", 37808, 73, 35u, kFleechResID, PalId::Default },
    { AnimId::Fleech_Unknown_A, "FLEECH.BAN", 37884, 73, 35u, kFleechResID, PalId::Default }, // Still frame of some kind
    { AnimId::Fleech_Unknown_B, "FLEECH.BAN", 37896, 73, 35u, kFleechResID, PalId::Default }, // Slow broken animation, probably unused
    { AnimId::Fleech_Idle_B, "FLEECH.BAN", 37704, 73, 35u, kFleechResID, PalId::Default },
    { AnimId::Fleech_Walk, "FLEECH.BAN", 37748, 73, 35u, kFleechResID, PalId::Default },
    { AnimId::Fleech_Speak, "FLEECH.BAN", 37784, 73, 35u, kFleechResID, PalId::Default },
    { AnimId::Fleech_Unknown_C, "FLEECH.BAN", 37924, 73, 35u, kFleechResID, PalId::Default }, // Another still frame
    { AnimId::Fleech_Idle_C, "FLEECH.BAN", 37936, 73, 35u, kFleechResID, PalId::Default },
    { AnimId::Fleech_Idle_D, "FLEECH.BAN", 37960, 73, 35u, kFleechResID, PalId::Default },
    { AnimId::Fleech_Falling, "FLEECH.BAN", 37984, 73, 35u, kFleechResID, PalId::Default },
    { AnimId::Fleech_Landing, "FLEECH.BAN", 38060, 73, 35u, kFleechResID, PalId::Default },
    { AnimId::Fleech_Tongue, "FLEECH.BAN", 38112, 73, 35u, kFleechResID, PalId::Default }, // Used when the fleech attacks or grabbing a ledge
    { AnimId::Fleech_Climb_A, "FLEECH.BAN", 38156, 73, 35u, kFleechResID, PalId::Default }, // Used while swinging up a ledge
    { AnimId::Fleech_Climb_B, "FLEECH.BAN", 38208, 73, 35u, kFleechResID, PalId::Default }, // Used when fleech has reached the top of a ledge
    { AnimId::Fleech_Unknown_D, "FLEECH.BAN", 38260, 73, 35u, kFleechResID, PalId::Default }, // Some sort of quick 2 frame animation
    { AnimId::Fleech_Unknown_E, "FLEECH.BAN", 37848, 73, 35u, kFleechResID, PalId::Default }, // Seems to be a much slowed down version of Fleech_Tongue
    { AnimId::Fleech_Unknown_F, "FLEECH.BAN", 38248, 73, 35u, kFleechResID, PalId::Default }, // Another still frame
    { AnimId::Fleech_Sleep, "FLEECH.BAN", 38396, 73, 35u, kFleechResID, PalId::Default },
    { AnimId::Fleech_Eat, "FLEECH.BAN", 38276, 73, 35u, kFleechResID, PalId::Default },

    // Note: Glukkon animations are sorted by order of appearance under Glukkon.ccp (line 80)
    { AnimId::Glukkon_Normal_Idle, "GLUKKON.BND", 169608, 163, 79, kGlkbasicResID, PalId::Default },
    { AnimId::Glukkon_Normal_Walk, "GLUKKON.BND", 169644, 163, 79, kGlkbasicResID, PalId::Default },
    { AnimId::Glukkon_Normal_Turn_Around, "GLUKKON.BND", 169724, 163, 79, kGlkbasicResID, PalId::Default },
    { AnimId::Glukkon_Normal_Fall_Over, "GLUKKON.BND", 169772, 163, 79, kGlkbasicResID, PalId::Default },
    { AnimId::Glukkon_Normal_Jump, "GLUKKON.BND", 169828, 163, 79, kGlkbasicResID, PalId::Default },
    { AnimId::Glukkon_Normal_Unknown_A, "GLUKKON.BND", 169900, 163, 79, kGlkbasicResID, PalId::Default }, // Segmented jump animation?
    { AnimId::Glukkon_Normal_Unknown_B, "GLUKKON.BND", 169920, 163, 79, kGlkbasicResID, PalId::Default }, // ^
    { AnimId::Glukkon_Normal_Unknown_C, "GLUKKON.BND", 169952, 163, 79, kGlkbasicResID, PalId::Default }, // ^
    { AnimId::Glukkon_Normal_Unknown_D, "GLUKKON.BND", 169968, 163, 79, kGlkbasicResID, PalId::Default }, // Segmented standing back up?
    { AnimId::Glukkon_Normal_Landing, "GLUKKON.BND", 170000, 163, 79, kGlkbasicResID, PalId::Default },
    { AnimId::Glukkon_Normal_Possessed_A, "GLUKKON.BND", 170152, 163, 79, kGlkbasicResID, PalId::Default },
    { AnimId::Glukkon_Normal_Speak_A, "GLUKKON.BND", 170216, 163, 79, kGlkbasicResID, PalId::Default }, // Speak: "Hey", "Do it", "Help"
    { AnimId::Glukkon_Normal_Speak_B, "GLUKKON.BND", 170268, 163, 79, kGlkbasicResID, PalId::Default }, // Speak: "Come here", "Kill em", "All of ya"
    { AnimId::Glukkon_Normal_Laugh, "GLUKKON.BND", 170056, 163, 79, kGlkbasicResID, PalId::Default },
    { AnimId::Glukkon_Normal_Unknown_E, "GLUKKON.BND", 170312, 163, 79, kGlkbasicResID, PalId::Default }, // No idea what this could be for...
    { AnimId::Glukkon_Normal_Unknown_F, "GLUKKON.BND", 170332, 163, 79, kGlkbasicResID, PalId::Default }, // ^
    { AnimId::Glukkon_Normal_Unknown_G, "GLUKKON.BND", 170352, 163, 79, kGlkbasicResID, PalId::Default }, // ^
    { AnimId::Glukkon_Normal_Unknown_H, "GLUKKON.BND", 170372, 163, 79, kGlkbasicResID, PalId::Default }, // ^
    { AnimId::Glukkon_Normal_Unknown_I, "GLUKKON.BND", 170392, 163, 79, kGlkbasicResID, PalId::Default }, // ^
    { AnimId::Glukkon_Normal_Unknown_J, "GLUKKON.BND", 170412, 163, 79, kGlkbasicResID, PalId::Default }, // Segmented landing animation?
    { AnimId::Glukkon_Normal_Stand_Up_A, "GLUKKON.BND", 170432, 163, 79, kGlkbasicResID, PalId::Default }, // First half of standing up
    { AnimId::Glukkon_Normal_Possessed_B, "GLUKKON.BND", 170152, 163, 79, kGlkbasicResID, PalId::Default }, // Same as Possessed_A?
    { AnimId::Glukkon_Normal_Stand_Up_B, "GLUKKON.BND", 170472, 163, 79, kGlkbasicResID, PalId::Default }, // Second half of standing up
    { AnimId::Glukkon_Normal_Speak_C, "GLUKKON.BND", 170504, 163, 79, kGlkbasicResID, PalId::Default }, // Speak: "Stay here"
    { AnimId::Glukkon_Normal_Unknown_K, "GLUKKON.BND", 170560, 163, 79, kGlkbasicResID, PalId::Default }, // No idea what this could be for...

    { AnimId::Glukkon_Aslik_Idle, "ASLIK.BND", 171356, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Walk, "ASLIK.BND", 171392, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Turn_Around, "ASLIK.BND", 171472, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Fall_Over, "ASLIK.BND", 171520, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Jump, "ASLIK.BND", 171576, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Unknown_A, "ASLIK.BND", 171648, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Unknown_B, "ASLIK.BND", 171668, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Unknown_C, "ASLIK.BND", 171700, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Unknown_D, "ASLIK.BND", 171716, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Landing, "ASLIK.BND", 171748, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Possessed_A, "ASLIK.BND", 171900, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Speak_A, "ASLIK.BND", 171964, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Speak_B, "ASLIK.BND", 172016, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Laugh, "ASLIK.BND", 171804, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Unknown_E, "ASLIK.BND", 172060, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Unknown_F, "ASLIK.BND", 172080, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Unknown_G, "ASLIK.BND", 172100, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Unknown_H, "ASLIK.BND", 172120, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Unknown_I, "ASLIK.BND", 172140, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Unknown_J, "ASLIK.BND", 172160, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Stand_Up_A, "ASLIK.BND", 172180, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Possessed_B, "ASLIK.BND", 171900, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Stand_Up_B, "ASLIK.BND", 172220, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Speak_C, "ASLIK.BND", 172252, 157, 76, kGlukAslikResID, PalId::Default },
    { AnimId::Glukkon_Aslik_Unknown_K, "ASLIK.BND", 172308, 157, 76, kGlukAslikResID, PalId::Default },

    { AnimId::Glukkon_Phleg_Idle, "PHLEG.BND", 199500, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Walk, "PHLEG.BND", 199536, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Turn_Around, "PHLEG.BND", 199616, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Fall_Over, "PHLEG.BND", 199664, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Jump, "PHLEG.BND", 199720, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Unknown_A, "PHLEG.BND", 199792, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Unknown_B, "PHLEG.BND", 199812, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Unknown_C, "PHLEG.BND", 199844, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Unknown_D, "PHLEG.BND", 199860, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Landing, "PHLEG.BND", 199892, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Possessed_A, "PHLEG.BND", 200044, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Speak_A, "PHLEG.BND", 200108, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Speak_B, "PHLEG.BND", 200160, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Laugh, "PHLEG.BND", 199948, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Unknown_E, "PHLEG.BND", 200204, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Unknown_F, "PHLEG.BND", 200224, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Unknown_G, "PHLEG.BND", 200244, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Unknown_H, "PHLEG.BND", 200264, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Unknown_I, "PHLEG.BND", 200284, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Unknown_J, "PHLEG.BND", 200304, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Stand_Up_A, "PHLEG.BND", 200324, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Possessed_B, "PHLEG.BND", 200044, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Stand_Up_B, "PHLEG.BND", 200364, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Speak_C, "PHLEG.BND", 200396, 145, 72, kGlukPhlegResID, PalId::Default },
    { AnimId::Glukkon_Phleg_Unknown_K, "PHLEG.BND", 200452, 145, 72, kGlukPhlegResID, PalId::Default },

    { AnimId::Glukkon_Dripik_Idle, "DRIPIK.BND", 193064, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Walk, "DRIPIK.BND", 193100, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Turn_Around, "DRIPIK.BND", 193180, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Fall_Over, "DRIPIK.BND", 193228, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Jump, "DRIPIK.BND", 193284, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Unknown_A, "DRIPIK.BND", 193356, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Unknown_B, "DRIPIK.BND", 193376, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Unknown_C, "DRIPIK.BND", 193408, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Unknown_D, "DRIPIK.BND", 193424, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Landing, "DRIPIK.BND", 193456, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Possessed_A, "DRIPIK.BND", 193608, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Speak_A, "DRIPIK.BND", 193672, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Speak_B, "DRIPIK.BND", 193724, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Laugh, "DRIPIK.BND", 193512, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Unknown_E, "DRIPIK.BND", 193768, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Unknown_F, "DRIPIK.BND", 193788, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Unknown_G, "DRIPIK.BND", 193808, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Unknown_H, "DRIPIK.BND", 193828, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Unknown_I, "DRIPIK.BND", 193848, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Unknown_J, "DRIPIK.BND", 193868, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Stand_Up_A, "DRIPIK.BND", 193888, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Possessed_B, "DRIPIK.BND", 193608, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Stand_Up_B, "DRIPIK.BND", 193928, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Speak_C, "DRIPIK.BND", 193960, 145, 72, kGlukDripikResID, PalId::Default },
    { AnimId::Glukkon_Dripik_Unknown_K, "DRIPIK.BND", 194016, 145, 72, kGlukDripikResID, PalId::Default },

    { AnimId::Greeter_Idle, "GREETER.BAN", 50028, 65, 60, kGreeterResID, PalId::Default },
	{ AnimId::Greeter_Turn_Around, "GREETER.BAN", 50072, 65, 60, kGreeterResID, PalId::Default },
	{ AnimId::Greeter_Hit, "GREETER.BAN", 50236, 65, 60, kGreeterResID, PalId::Default },
    { AnimId::Greeter_Speak, "GREETER.BAN", 50104, 65, 60, kGreeterResID, PalId::Default },
    { AnimId::Greeter_Chase, "GREETER.BAN", 50144, 65, 60, kGreeterResID, PalId::Default },
    { AnimId::Greeter_Falling, "GREETER.BAN", 50212, 65, 60, kGreeterResID, PalId::Default },

    { AnimId::Mine_Car_Closed, "BAYROLL.BAN", 20836, 130, 62u, kUnknownResID_6013, PalId::Default },
    { AnimId::Mine_Car_Open, "BAYROLL.BAN", 20812, 130, 62u, kUnknownResID_6013, PalId::Default },
    { AnimId::Mine_Car_Shake_A, "BAYROLL.BAN", 20872, 130, 62u, kUnknownResID_6013, PalId::Default },
    { AnimId::Mine_Car_Shake_B, "BAYROLL.BAN", 20900, 130, 62u, kUnknownResID_6013, PalId::Default },
    { AnimId::Mine_Car_Tread_Idle, "BAYROLL.BAN", 20824, 130, 62u, kUnknownResID_6013, PalId::Default },
    { AnimId::Mine_Car_Tread_Move_A, "BAYROLL.BAN", 20848, 130, 62u, kUnknownResID_6013, PalId::Default },
    { AnimId::Mine_Car_Tread_Move_B, "BAYROLL.BAN", 20788, 130, 62u, kUnknownResID_6013, PalId::Default },

////////////////////////////////////////////////////////////////////////////////////////////////////
    { AnimId::Anim_Tester, "BAYROLL.BAN", 20788, 130, 62u, kUnknownResID_6013, PalId::Default },
////////////////////////////////////////////////////////////////////////////////////////////////////

    { AnimId::Slog_Idle_A, "SLOG.BND", 96464, 121, 57, kDogbasicResID, PalId::Default },
    { AnimId::Slog_Walk, "SLOG.BND", 96344, 121, 57, kDogbasicResID, PalId::Default },
    { AnimId::Slog_Run, "SLOG.BND", 96424, 121, 57, kDogbasicResID, PalId::Default },
    { AnimId::Slog_Turn_Around, "SLOG.BND", 96764, 121, 57, kDogbasicResID, PalId::Default },
    { AnimId::Slog_Falling, "SLOG.BND", 96692, 121, 57, kDogbasicResID, PalId::Default },
    { AnimId::Slog_Bark, "SLOG.BND", 96496, 121, 57, kDogbasicResID, PalId::Default },
    { AnimId::Slog_Run_Stop, "SLOG.BND", 96532, 121, 57, kDogbasicResID, PalId::Default },
    { AnimId::Slog_Run_Turn_Around, "SLOG.BND", 96580, 121, 57, kDogbasicResID, PalId::Default },
    { AnimId::Slog_Unknown_A, "SLOG.BND", 96640, 121, 57, kDogbasicResID, PalId::Default }, // Very short animation
    { AnimId::Slog_Unknown_B, "SLOG.BND", 96660, 121, 57, kDogbasicResID, PalId::Default }, // ^
    { AnimId::Slog_Unknown_C, "SLOG.BND", 96876, 121, 57, kDogbasicResID, PalId::Default }, // ^
    { AnimId::Slog_Unknown_D, "SLOG.BND", 96716, 121, 57, kDogbasicResID, PalId::Default }, // Just a still frame
    { AnimId::Slog_Bark_Short_A, "SLOG.BND", 96728, 121, 57, kDogbasicResID, PalId::Default },
    { AnimId::Slog_Unknown_E, "SLOG.BND", 96752, 121, 57, kDogbasicResID, PalId::Default }, // Another still frame
    { AnimId::Slog_Bark_Short_B, "SLOG.BND", 15068, 121, 57, kDogrstnResID, PalId::Default },
    { AnimId::Slog_Sleep_A, "SLOG.BND", 15108, 121, 57, kDogrstnResID, PalId::Default },
    { AnimId::Slog_Sleep_B, "SLOG.BND", 15156, 121, 57, kDogrstnResID, PalId::Default },
    { AnimId::Slog_Sleep_C, "SLOG.BND", 15132, 121, 57, kDogrstnResID, PalId::Default },
    { AnimId::Slog_Lunge, "SLOG.BND", 38904, 121, 57, kDogattkResID, PalId::Default },
    { AnimId::Slog_Jump_Up, "SLOG.BND", 38960, 121, 57, kDogattkResID, PalId::Default },
    { AnimId::Slog_Eat, "SLOG.BND", 39064, 121, 57, kDogattkResID, PalId::Default },
    { AnimId::Slog_Die, "SLOG.BND", 12412, 121, 57, kDogknfdResID, PalId::Default },
    { AnimId::Slog_Scratch, "SLOG.BND", 12724, 121, 57, kDogidleResID, PalId::Default },
    { AnimId::Slog_Idle_B, "SLOG.BND", 12812, 121, 57, kDogidleResID, PalId::Default },

    { AnimId::Slurg_Move, "SLURG.BAN", 2708, 46, 15, kSlurgResID, PalId::Default },
    { AnimId::Slurg_Burst, "SLURG.BAN", 2808, 46, 15, kSlurgResID, PalId::Default },
    { AnimId::Slurg_Turn_Around, "SLURG.BAN", 2740, 46, 15, kSlurgResID, PalId::Default },

    { AnimId::Bone, "BTHROW.BND", 456, 15, 9, kBoneResID, PalId::Default },
    { AnimId::Bone_Bag_A, "BONEBAG.BAN", 8748, 64, 60, kUnknownResID_590, PalId::Default },
    { AnimId::Bone_Bag_B, "BONEBAG.BAN", 8708, 64, 60, kUnknownResID_590, PalId::Default },
    { AnimId::Bone_Bag_C, "BONEBAG.BAN", 8788, 64, 60, kUnknownResID_590, PalId::Default },
    { AnimId::Bone_Saw_Horizontal_Off, "DRILL.BAN", 6736, 65, 33, kDrillResID, PalId::Default },
    { AnimId::Bone_Saw_Horizontal_On, "DRILL.BAN", 6712, 65, 33, kDrillResID, PalId::Default },
    { AnimId::Bone_Saw_Vertical_Off, "DRILL.BAN", 6676, 65, 33, kDrillResID, PalId::Default },
    { AnimId::Bone_Saw_Vertical_On, "DRILL.BAN", 6688, 65, 33, kDrillResID, PalId::Default },
    { AnimId::Explosion_Mine, "EXPLODE.BND", 51588, 214, 0x31u, kBgexpldResID, PalId::Default },
    { AnimId::Grenade, "GRENADE.BAN", 1252, 17, 11, kGrenadeResID, PalId::Default },
    { AnimId::Grenade_Machine_Button_Off, "GMACHINE.BND1", 1736, 26, 17u, kR1buttonResID, PalId::Default },
    { AnimId::Grenade_Machine_Button_On, "GMACHINE.BND1", 1708, 26, 17u, kR1buttonResID, PalId::Default },
    { AnimId::Grenade_Machine_Nozzle, "GMACHINE.BND1", 3700, 67, 36u, kR1bpipeResID, PalId::Default },
    { AnimId::Grenade_Machine_Nozzle_Drop_Grenade, "GMACHINE.BND1", 3672, 67, 36u, kR1bpipeResID, PalId::Default },
    { AnimId::Lift_Rope, "ROPES.BAN", 748, 9, 16, kRopesResID, PalId::Default },
    { AnimId::Mine, "MINE.BND", 784, 38, 0, kLandmineResID, PalId::Default },
    { AnimId::Motion_Detector_Flare, "MFLARE.BAN", 1736, 55, 22, kMflareResID, PalId::Default },
    { AnimId::Motion_Detector_Laser, "MOTION.BAN", 23280, 37, 60, kMotionResID, PalId::Default },
    { AnimId::Moving_Bomb, "MOVEBOMB.BAN", 17548, 76, 30, kMovebombResID, PalId::Default },
    { AnimId::Security_Door_Idle, "SECDOOR.BAN", 1512, 70, 19u, kUnknownResID_6027, PalId::Default },
    { AnimId::Security_Door_Speak, "SECDOOR.BAN", 1528, 70, 19u, kUnknownResID_6027, PalId::Default },
    { AnimId::Security_Orb, "MAIMORB.BAN", 2228, 53, 28u, kMaimGameResID, PalId::Default },
    { AnimId::Slap_Lock_Idle_A, "GHOSTTRP.BAN", 7068, 58, 44, kUnknownResID_1053, PalId::Default },
    { AnimId::Slap_Lock_Idle_B, "GHOSTTRP.BAN", 7056, 58, 44, kUnknownResID_1053, PalId::Default },
    { AnimId::Slap_Lock_Shake, "GHOSTTRP.BAN", 6976, 58, 44, kUnknownResID_1053, PalId::Default },
    { AnimId::Sparks, "SPARKS.BAN", 808, 19, 11, kSparksResID, PalId::Default },
    { AnimId::Status_Light_Green, "STATUSLT.BAN", 240, 14, 9u, kStatusLightResID, PalId::Default },
    { AnimId::Status_Light_Red, "STATUSLT.BAN", 252, 14, 9u, kStatusLightResID, PalId::Default },
    { AnimId::Switch_Idle, "SWITCH1.BAN", 5696, 66, 40, kSwitchResID, PalId::Default },
    { AnimId::Switch_Pull_Left_A, "SWITCH1.BAN", 5708, 66, 40, kSwitchResID, PalId::Default },
    { AnimId::Switch_Pull_Left_B, "SWITCH1.BAN", 5760, 66, 40, kSwitchResID, PalId::Default },
    { AnimId::Switch_Pull_Right_A, "SWITCH1.BAN", 5796, 66, 40, kSwitchResID, PalId::Default },
    { AnimId::Switch_Pull_Right_B, "SWITCH1.BAN", 5848, 66, 40, kSwitchResID, PalId::Default },
    { AnimId::Timed_Mine_Armed, "BOMB.BND", 848, 35, 0x11u, kBombResID, PalId::Default },
    { AnimId::Timed_Mine_Idle, "BOMB.BND", 836, 35, 0x11u, kBombResID, PalId::Default },
    { AnimId::Tortured_Modukon, "MUDTORT.BAN", 20892, 101, 0x44u, kTorturedMud, PalId::Default },
    { AnimId::Tortured_Modukon_Released, "MUDTORT.BAN", 20864, 101, 0x44u, kTorturedMud, PalId::Default },
    { AnimId::Tortured_Modukon_Zap, "MUDTORT.BAN", 21000, 101, 0x44u, kTorturedMud, PalId::Default },
    { AnimId::Trap_Door_Closed, "TRAPDOOR.BAN", 4788, 72, 41, kP6c1trapResID, PalId::Default },         // These still need implementation somehow under line 24 of TrapDoor.cpp
    { AnimId::Trap_Door_Closing, "TRAPDOOR.BAN", 4812, 72, 41, kP6c1trapResID, PalId::Default },        // ^
    { AnimId::Trap_Door_Open, "TRAPDOOR.BAN", 4800, 72, 41, kP6c1trapResID, PalId::Default },           // ^
    { AnimId::Trap_Door_Opening, "TRAPDOOR.BAN", 4840, 72, 41, kP6c1trapResID, PalId::Default },        // ^
    { AnimId::Trap_Door_Tribal_Closed, "VLTSTRAP.BAN", 5328, 72, 41, kP6c1trapResID, PalId::Default },  // ^
    { AnimId::Trap_Door_Tribal_Closing, "VLTSTRAP.BAN", 5368, 72, 41, kP6c1trapResID, PalId::Default }, // ^
    { AnimId::Trap_Door_Tribal_Open, "VLTSTRAP.BAN", 5316, 72, 41, kP6c1trapResID, PalId::Default },    // ^
    { AnimId::Trap_Door_Tribal_Opening, "VLTSTRAP.BAN", 5340, 72, 41, kP6c1trapResID, PalId::Default }, // ^
    { AnimId::UXB_Active, "UXB.BND", 8048, 59, 19, kTbombResID, PalId::Default },
    { AnimId::UXB_Toggle, "UXB.BND", 8120, 59, 19, kTbombResID, PalId::Default },
    { AnimId::UXB_Disabled, "UXB.BND", 0x2000, 59, 19, kTbombResID, PalId::Default },
    { AnimId::Work_Wheel_Idle, "WORKWHEL.BAN", 2716, 49, 24, kWorkwhelResID, PalId::Default },
    { AnimId::Work_Wheel_Turning, "WORKWHEL.BAN", 2728, 49, 24, kWorkwhelResID, PalId::Default },
    { AnimId::Zap_Line_Blue, "SPLINE.BAN", 276, 14, 9, kSplineResID, PalId::Default },
    { AnimId::Zap_Line_Red, "SPLINE.BAN", 288, 14, 9, kSplineResID, PalId::Default },
};

const PalRecord& PalRec(PalId toFind)
{
    for (const auto& pal : kPalRecords)
    {
        if (pal.mId == toFind)
        {
            return pal;
        }
    }
    ALIVE_FATAL("Missing pal entry");
}

const AnimRecord& AnimRec(AnimId toFind)
{
    for (const auto& anim : kAnimRecords)
    {
        if (anim.mId == toFind)
        {
            return anim;
        }
    }
    ALIVE_FATAL("Missing animation entry");
}
