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
