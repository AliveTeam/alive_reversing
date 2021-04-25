#include "stdafx.h"
#include "MusicController.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Sound/Midi.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "Map.hpp"
#include "ObjectIds.hpp"
#include "PathData.hpp"
#include "Sys.hpp"

ALIVE_VAR(1, 0x5C3020, MusicController*, pMusicController_5C3020, nullptr);
ALIVE_VAR(1, 0x5C301C, u32, sMusicControllerBaseTimeStamp_5C301C, 0);
ALIVE_VAR(1, 0x5c3024, u32, sMusicTime_5C3024, 0);

struct MusicController_Record3_Sub
{
    s16 field_0;
    s8 field_2_min;
    s8 field_3_max;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record3_Sub, 4);

struct MusicController_Record3
{
    MusicController_Record3_Sub field_0[3];
    u8 field_C_repeat_count;
    u8 field_D_padding;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record3, 0xE);

// TODO: 17 = max levels?

const MusicController_Record3 stru_55D1E0[17] =
{
    { { { 2048, 107, 108 },   { -1, 109, 109 },     { -1, 0, 0     } }, 1u, 0u },
    { { { 320, 103, 103 },    { 1280, 102, 102 },   { -1, 101, 101 } }, 1u, 0u },
    { { { 512, 8, 8 },        { 512, 7, 7 },        { -1, 6, 6     } }, 1u, 0u },
    { { { 512, 17, 17 },      { 512, 16, 16 },      { -1, 15, 15   } }, 1u, 0u },
    { { { 1024, 33, 33 },     { 512, 32, 32 },      { -1, 31, 31   } }, 1u, 0u },
    { { { 640, 49, 49 },      { 640, 48, 48 },      { -1, 47, 47   } }, 1u, 0u },
    { { { 640, 62, 62 },      { 640, 61, 61 },      { -1, 60, 60   } }, 1u, 0u },
    { { { -1, 40, 40 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u },
    { { { 320, 72, 72 },      { 320, 71, 71 },      { -1, 70, 70   } }, 1u, 0u },
    { { { 320, 88, 88 },      { 320, 87, 87 },      { -1, 86, 86   } }, 1u, 0u },
    { { { 512, 97, 97 },      { -1, 96, 97 },       { -1, -1, -1   } }, 1u, 0u },
    { { { -1, 24, 24 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u },
    { { { -1, 56, 56 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u },
    { { { -1, 66, 66 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u },
    { { { -1, 79, 79 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u },
    { { { 320, 103, 103 },    { 1280, 102, 102 },   { -1, 101, 101 } }, 1u, 0u },
    { { { -1, 111, 111 },     { -1, 0, 0 },         { -1, 0, 0     } }, 1u, 0u }
};

enum class AmbientMusic : s8
{
    eOff = 0,
    eOn = 1
};

struct MusicController_Record
{
    s8 field_0_seq_id_idx;
    s8 field_1_unused;
    AmbientMusic field_2_bAmbient_music_enabled;
    s8 field_3_padding;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record, 0x4);

const u8 unused_55D468[17] =
{
    16u,
    20u,
    16u,
    16u,
    16u,
    20u,
    20u,
    28u,
    20u,
    20u,
    16u,
    28u,
    28u,
    28u,
    28u,
    20u,
    16u
};

const MusicController_Record tension_55D2D0[17] =
{
    { SeqId::BonewerkzEnderChase2_111, 16u, AmbientMusic::eOff, 0u },
    { SeqId::BonewerkzChase2_104, 20u, AmbientMusic::eOff, 0u },
    { SeqId::HitBottomOfDeathPit_9, 20u, AmbientMusic::eOff, 0u },
    { SeqId::AbeStopIt_18, 16u, AmbientMusic::eOff, 0u },
    { SeqId::NecrumCalm2_34, 16u, AmbientMusic::eOff, 0u },
    { SeqId::MudomoSafeTension_50, 20u, AmbientMusic::eOff, 0u },
    { SeqId::MudancheePossess_63, 20u, AmbientMusic::eOff, 0u },
    { SeqId::NecrumSafeTension_41, 28u, AmbientMusic::eOff, 0u },
    { SeqId::MudancheeEnderSafeTension_73, 20u, AmbientMusic::eOff, 0u },
    { SeqId::BarracksCalm1_89, 20u, AmbientMusic::eOff, 0u },
    { SeqId::BonewerkzCalm2_98, 20u, AmbientMusic::eOff, 0u },
    { SeqId::ParamiteNearby_25, 28u, AmbientMusic::eOff, 0u },
    { SeqId::MudomoEnderSafeTension_57, 28u, AmbientMusic::eOff, 0u },
    { SeqId::MudancheeEnderCalm_67, 28u, AmbientMusic::eOff, 0u },
    { SeqId::FeecoTension2_80, 28u, AmbientMusic::eOff, 0u },
    { SeqId::BonewerkzChase2_104, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOn, 0u }
};

const MusicController_Record chase_55D314[17] =
{
    { SeqId::BonewerkzEnderSafeTension_112, 16u, AmbientMusic::eOff, 0u },
    { SeqId::BonewerkzSafeTension_105, 20u, AmbientMusic::eOff, 0u },
    { SeqId::MudokonChant1_10, 20u, AmbientMusic::eOff, 0u },
    { SeqId::WheelSqueak_19, 16u, AmbientMusic::eOff, 0u },
    { SeqId::NecrumCalm1_35, 16u, AmbientMusic::eOff, 0u },
    { SeqId::MudomoEnderCalm_51, 20u, AmbientMusic::eOff, 0u },
    { SeqId::MudancheeTension2_64, 20u, AmbientMusic::eOff, 0u },
    { SeqId::MudomoCalm3_42, 28u, AmbientMusic::eOff, 0u },
    { SeqId::FeecoCalm3_74, 20u, AmbientMusic::eOff, 0u },
    { SeqId::BarracksTension_90, 20u, AmbientMusic::eOff, 0u },
    { SeqId::BonewerkzCalm1_99, 20u, AmbientMusic::eOff, 0u },
    { SeqId::SleepingFleechOffscreen_26, 28u, AmbientMusic::eOff, 0u },
    { SeqId::MudancheeCalm3_58, 28u, AmbientMusic::eOff, 0u },
    { SeqId::MudancheeEnderTension1_68, 28u, AmbientMusic::eOff, 0u },
    { SeqId::FeecoChase2_81, 28u, AmbientMusic::eOff, 0u },
    { SeqId::BonewerkzSafeTension_105, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOn, 0u }
};

const MusicController_Record possessed_55D358[17] =
{
    { -1, 1u, AmbientMusic::eOn, 0u },
    { SeqId::BonewerkzEnderCalm_106, 20u, AmbientMusic::eOff, 0u },
    { SeqId::MudokonChant_11, 20u, AmbientMusic::eOff, 0u },
    { SeqId::MainMenuParamiteAttack_20, 16u, AmbientMusic::eOff, 0u },
    { SeqId::NecrumTension1_36, 16u, AmbientMusic::eOff, 0u },
    { SeqId::MudomoEnderTension1_52, 20u, AmbientMusic::eOff, 0u },
    { SeqId::MudancheeChase2_65, 20u, AmbientMusic::eOff, 0u },
    { SeqId::MudomoCalm2_43, 28u, AmbientMusic::eOff, 0u },
    { SeqId::FeecoCalm2_75, 20u, AmbientMusic::eOff, 0u },
    { SeqId::BarracksPossess_91, 20u, AmbientMusic::eOff, 0u },
    { SeqId::BonewerkzTension1_100, 20u, AmbientMusic::eOff, 0u },
    { SeqId::ToCalm1_27, 28u, AmbientMusic::eOff, 0u },
    { SeqId::MudancheeCalm2_59, 28u, AmbientMusic::eOff, 0u },
    { SeqId::MudancheePossess_69, 28u, AmbientMusic::eOff, 0u },
    { SeqId::FeecoSafeTension_82, 28u, AmbientMusic::eOff, 0u },
    { SeqId::BonewerkzEnderCalm_106, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOn, 0u }
};

const MusicController_Record slogTension_55D39C[17] =
{
    { -1, 1u, AmbientMusic::eOn, 0u },
    { SeqId::BreweryCalm3_113, 20u, AmbientMusic::eOff, 0u },
    { SeqId::Scratching_12, 20u, AmbientMusic::eOff, 0u },
    { SeqId::WalkingSligOffscreen_21, 20u, AmbientMusic::eOff, 0u },
    { SeqId::NecrumPossess_37, 20u, AmbientMusic::eOff, 0u },
    { SeqId::MudomoEnderPossess_53, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOff, 0u },
    { SeqId::MudomoCalm1_44, 20u, AmbientMusic::eOff, 0u },
    { SeqId::FeecoCalm1_76, 20u, AmbientMusic::eOff, 0u },
    { SeqId::BarracksChase_92, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOff, 0u },
    { SeqId::ToCalm2_28, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOff, 0u },
    { SeqId::FeecoEnderCalm_83, 20u, AmbientMusic::eOff, 0u },
    { SeqId::BreweryCalm3_113, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOn, 0u }
};

const MusicController_Record slogChase_55D3E0[17] =
{
    { -1, 1u, AmbientMusic::eOn, 0u },
    { SeqId::BreweryCalm2_114, 20u, AmbientMusic::eOff, 0u },
    { SeqId::Empty_13, 20u, AmbientMusic::eOff, 0u },
    { SeqId::SleepingSligOffscreen_22, 20u, AmbientMusic::eOff, 0u },
    { SeqId::NecrumChase1_38, 20u, AmbientMusic::eOff, 0u },
    { SeqId::MudomoEnderChase1_54, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOff, 0u },
    { SeqId::MudomoTension1_45, 20u, AmbientMusic::eOff, 0u },
    { SeqId::FeecoTension1_77, 20u, AmbientMusic::eOff, 0u },
    { SeqId::BarracksEnderCalm_93, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOff, 0u },
    { SeqId::DeathDrums_29, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOff, 0u },
    { SeqId::FeecoEnderTension_84, 20u, AmbientMusic::eOff, 0u },
    { SeqId::BreweryCalm2_114, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOn, 0u }
};

const MusicController_Record slogChaseTension_55D424[17] =
{
    { -1, 1u, AmbientMusic::eOn, 0u },
    { SeqId::BreweryCalm1_115, 20u, AmbientMusic::eOff, 0u },
    { SeqId::Explosion1_14, 20u, AmbientMusic::eOff, 0u },
    { SeqId::AwakeSlogOffscreen_23, 20u, AmbientMusic::eOff, 0u },
    { SeqId::NecrumTension2_39, 20u, AmbientMusic::eOff, 0u },
    { SeqId::MudomoEnderTension2_55, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOff, 0u },
    { SeqId::MudomoChase1_46, 20u, AmbientMusic::eOff, 0u },
    { SeqId::FeecoPossess_78, 20u, AmbientMusic::eOff, 0u },
    { SeqId::BarracksEnderTension_94, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOff, 0u },
    { SeqId::AbeDeathDrums_30, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOff, 0u },
    { SeqId::FeecoEnderPossess_85, 20u, AmbientMusic::eOff, 0u },
    { SeqId::BreweryCalm1_115, 20u, AmbientMusic::eOff, 0u },
    { -1, 1u, AmbientMusic::eOn, 0u }
};

// TODO: 118 = max SEQs?
struct MusicController_Record2
{
    WORD field_0_idx;
    WORD field_2_duration;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record2, 4);

const MusicController_Record2 stru_55D008[118] =
{
    { SeqId::ToCalm1_27, 320u },
    { SeqId::ToCalm2_28, 320u },
    { SeqId::DeathDrums_29, 320u },
    { SeqId::AbeDeathDrums_30, 320u },
    { SeqId::SaveTriggerMusic_31, 320u },
    { SeqId::SecretMusic_32, 320u },
    { SeqId::NecrumCalm3_33, 512u },
    { SeqId::NecrumCalm2_34, 512u },
    { SeqId::NecrumCalm1_35, 256u },
    { SeqId::NecrumTension1_36, 320u },
    { SeqId::NecrumChase1_38, 320u },
    { SeqId::NecrumPossess_37, 320u },
    { SeqId::NecrumTension2_39, 160u },
    { SeqId::NecrumChase2_40, 640u },
    { SeqId::NecrumSafeTension_41, 160u },
    { SeqId::MudomoCalm3_42, 512u },
    { SeqId::MudomoCalm2_43, 512u },
    { SeqId::MudomoCalm1_44, 512u },
    { SeqId::MudomoTension1_45, 256u },
    { SeqId::MudomoChase1_46, 512u },
    { SeqId::MudomoPossess_47, 256u },
    { SeqId::MudomoTension2_48, 160u },
    { SeqId::MudomoChase2_49, 640u },
    { SeqId::MudomoSafeTension_50, 160u },
    { SeqId::MudomoEnderCalm_51, 448u },
    { SeqId::MudomoEnderTension1_52, 224u },
    { SeqId::MudomoEnderChase1_54, 112u },
    { SeqId::MudomoEnderPossess_53, 448u },
    { SeqId::MudomoEnderTension2_55, 160u },
    { SeqId::MudomoEnderChase2_56, 640u },
    { SeqId::MudomoEnderSafeTension_57, 160u },
    { SeqId::MudancheeCalm3_58, 512u },
    { SeqId::MudancheeCalm2_59, 512u },
    { SeqId::MudancheeCalm1_60, 1024u },
    { SeqId::MudancheeTension1_61, 256u },
    { SeqId::MudancheeChase1_62, 1024u },
    { SeqId::MudancheePossess_63, 256u },
    { SeqId::MudancheeTension2_64, 160u },
    { SeqId::MudancheeChase2_65, 640u },
    { SeqId::MudancheeSafeTension_66, 160u },
    { SeqId::MudancheeEnderCalm_67, 448u },
    { SeqId::MudancheeEnderTension1_68, 224u },
    { SeqId::MudancheeEnderChase1_70, 112u },
    { SeqId::MudancheePossess_69, 448u },
    { SeqId::MudancheeTension2_71, 160u },
    { SeqId::MudancheeEnderChase2_72, 640u },
    { SeqId::MudancheeEnderSafeTension_73, 160u },
    { SeqId::FeecoCalm3_74, 320u },
    { SeqId::FeecoCalm2_75, 640u },
    { SeqId::FeecoCalm1_76, 640u },
    { SeqId::FeecoTension1_77, 320u },
    { SeqId::FeecoChase1_79, 320u },
    { SeqId::FeecoPossess_78, 320u },
    { SeqId::FeecoTension2_80, 160u },
    { SeqId::FeecoChase2_81, 640u },
    { SeqId::FeecoSafeTension_82, 160u },
    { SeqId::FeecoEnderCalm_83, 448u },
    { SeqId::FeecoEnderTension_84, 112u },
    { SeqId::FeecoEnderChase_86, 448u },
    { SeqId::FeecoEnderPossess_85, 224u },
    { SeqId::BarracksCalm3_87, 640u },
    { SeqId::BarracksCalm2_88, 640u },
    { SeqId::BarracksCalm1_89, 640u },
    { SeqId::BarracksTension_90, 320u },
    { SeqId::BarracksChase_92, 320u },
    { SeqId::BarracksPossess_91, 320u },
    { SeqId::BarracksEnderCalm_93, 448u },
    { SeqId::BarracksEnderTension_94, 112u },
    { SeqId::BarracksEnderChase_96, 448u },
    { SeqId::BarracksEnderPossess_95, 224u },
    { SeqId::BonewerkzCalm3_97, 320u },
    { SeqId::BonewerkzCalm2_98, 320u },
    { SeqId::BonewerkzCalm1_99, 320u },
    { SeqId::BonewerkzTension1_100, 320u },
    { SeqId::BonewerkzChase1_102, 320u },
    { SeqId::BonewerkzPossess_101, 320u },
    { SeqId::BonewerkzTension2_103, 160u },
    { SeqId::BonewerkzChase2_104, 640u },
    { SeqId::BonewerkzSafeTension_105, 160u },
    { SeqId::BonewerkzEnderCalm_106, 448u },
    { SeqId::BonewerkzEnderTension1_107, 112u },
    { SeqId::BonewerkzEnderChase1_109, 448u },
    { SeqId::BonewerkzEnderPossess_108, 224u },
    { SeqId::BonewerkzEnderTension2_110, 160u },
    { SeqId::BonewerkzEnderChase2_111, 640u },
    { SeqId::BonewerkzEnderSafeTension_112, 160u },
    { SeqId::BreweryCalm3_113, 320u },
    { SeqId::BreweryCalm2_114, 320u },
    { SeqId::BreweryCalm1_115, 320u },
    { SeqId::BreweryTension1_116, 320u },
    { SeqId::BreweryChase1_118, 320u },
    { SeqId::BreweryPossess_117, 320u },
    { SeqId::BreweryTension2_119, 160u },
    { SeqId::BreweryChase2_120, 640u },
    { SeqId::BrewerySafeTension_121, 160u },
    { SeqId::BoilerCalm3_122, 512u },
    { SeqId::BoilerCalm2_123, 512u },
    { SeqId::BoilerCalm1_124, 512u },
    { SeqId::BoilerTension_125, 320u },
    { SeqId::BoilerChase_127, 320u },
    { SeqId::BoilerPossess_126, 320u },
    { SeqId::MinesCalm3_128, 320u },
    { SeqId::MinesCalm2_129, 1280u },
    { SeqId::MinesCalm1_130, 320u },
    { SeqId::MinesTension1_131, 320u },
    { SeqId::MinesChase1_133, 320u },
    { SeqId::MinesPossess_132, 320u },
    { SeqId::MenuMainTheme1_134, 256u },
    { SeqId::MenuMainTheme2_135, 2048u },
    { SeqId::MenuMainTheme3_136, 2048u },
    { SeqId::MenuBeginTheme2_137, 256u },
    { SeqId::MenuSubTheme_138, 2048u },
    { SeqId::MenuBeginTheme1_139, 256u },
    { SeqId::MinesTension2_140, 160u },
    { SeqId::MinesChase2_141, 640u },
    { SeqId::MinesSafeTension_142, 160u },
    { SeqId::MinesChase2_141, 640u },
    { SeqId::MainMenuAmbient_0, 0u }
};


s32 CC MusicController::Create_47FC40()
{
    if (pMusicController_5C3020)
    {
        return 0;
    }

    pMusicController_5C3020 = ae_new<MusicController>();
    if (pMusicController_5C3020)
    {
        // NOTE: OG bug - only ctor was guarded by null check
        pMusicController_5C3020->ctor_47EE80();

        MusicController::SetBaseTimeStamp_47FD00();
        pMusicController_5C3020->EnableMusic_47FB80(FALSE);
    }
    return 1;
}

void CC MusicController::SetBaseTimeStamp_47FD00()
{
    sMusicControllerBaseTimeStamp_5C301C = SYS_GetTicks();
}

void CC MusicController::UpdateMusicTime_47F8B0()
{
    sMusicTime_5C3024 = (3 * SYS_GetTicks() - 3 * sMusicControllerBaseTimeStamp_5C301C) / 100;
}

void CC MusicController::Shutdown_47FD20()
{
    if (pMusicController_5C3020)
    {
        pMusicController_5C3020->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        pMusicController_5C3020 = nullptr;
        //nullsub_8(); // TODO: Check if PSX stub
        //sub_4FBAD0(dword_5C3028); // Some other likely strange PSX specific thing that does nothing
        //nullsub_27(); // TODO: Check if PSX stub
    }
}

MusicController::MusicTypes MusicController::GetMusicType_47FA80(WORD* seq, WORD* seq2, u32* seqTime)
{
    MusicController::UpdateMusicTime_47F8B0();
    if (seq)
    {
        *seq = field_2C_flags_and_seq_idx;
    }

    if (seq2)
    {
        *seq2 = field_40_flags_and_idx;
    }

    if (seqTime)
    {
        if (field_42_type != MusicTypes::eNone_0)
        {
            *seqTime = field_30_music_time - sMusicTime_5C3024;
            return field_42_type;
        }
        *seqTime = field_44 - sMusicTime_5C3024;
    }

    return field_42_type;
}

MusicController::MusicTypes CC MusicController::GetMusicType_47FDA0(WORD* seq1, WORD* seq2, u32* seqTime)
{
    if (pMusicController_5C3020)
    {
        return pMusicController_5C3020->GetMusicType_47FA80(seq1, seq2, seqTime);
    }
    return MusicTypes::eTypeNull;
}

BaseGameObject* MusicController::VDestructor(s32 flags)
{
    return vdtor_47EF20(flags);
}

MusicController* MusicController::ctor_47EE80()
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x5463C0); // vTbl_MusicController_5463C0

    field_6_flags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    field_40_flags_and_idx = -1;
    field_24_currentLevelID = LevelIds::eNone;
    field_28_object_id = -1;
    field_2C_flags_and_seq_idx = -1;
    field_42_type = MusicTypes::eNone_0;
    field_30_music_time = 0;

    field_58_flags.Clear(Flags_58::e58_Dead_Bit3);
    field_58_flags.Set(Flags_58::e58_MusicEnabled_Bit1);
    field_58_flags.Set(Flags_58::e58_ScreenChanged_Bit2);
    field_58_flags.Set(Flags_58::e58_Bit4);
    field_58_flags.Set(Flags_58::e58_AmbientMusicEnabled_Bit5);

    field_34_music_start_time = 0;
    field_48_last_music_frame = 0;
    field_38_unused = 0;
    field_4C_state = 0;

    field_4E_starting_volume = 0;
    field_50_current_vol = 0;
    field_52_target_volume = 0;
    field_54_music_volume_change_time = 0;

    field_4_typeId = AETypes::eMusicController_93;

    field_20_vol = 100;
    field_22_vol = 127;
    field_3C_unused = 1;
    return this;
}

BaseGameObject* MusicController::vdtor_47EF20(s32 flags)
{
    dtor_47EF50();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void MusicController::dtor_47EF50()
{
    SetVTable(this, 0x5463C0); // vTbl_MusicController_5463C0

    if (field_40_flags_and_idx > 0)
    {
        SND_SEQ_Stop_4CAE60(field_40_flags_and_idx);
    }

    BaseGameObject_dtor_4DBEC0();
}

void MusicController::EnableMusic_47FB80(s16 bEnable)
{
    bool enableMusic = bEnable != 0 ? true : false;
    MusicController::UpdateMusicTime_47F8B0();

    // If enable flag has changed
    if (field_58_flags.Get(Flags_58::e58_MusicEnabled_Bit1) != enableMusic)
    {
        // Flip the flag
        field_58_flags.Toggle(Flags_58::e58_MusicEnabled_Bit1);

        // Is it enabled?
        if (field_58_flags.Get(Flags_58::e58_MusicEnabled_Bit1))
        {
            // Yes enable volume
            SetMusicVolumeDelayed_47FB00(field_20_vol, 0);

            field_44 = 0;
            field_30_music_time = 0;
            field_48_last_music_frame = sMusicTime_5C3024;

            if (field_42_type == MusicTypes::eTension_4 || field_42_type == MusicTypes::eSlogChase_7 || field_42_type == MusicTypes::ePossessed_9)
            {
                field_58_flags.Set(Flags_58::e58_UnPause_Bit6);
            }
        }
        else
        {
            // No so turn off volume to disable music
            SetMusicVolumeDelayed_47FB00(0, 0);
        }
        UpdateVolumeState_47EFD0();
    }
}

void MusicController::SetMusicVolumeDelayed_47FB00(s16 vol, s16 delay)
{
    if (vol != field_50_current_vol)
    {
        field_4E_starting_volume = field_50_current_vol;
        field_52_target_volume = (vol <= 0) ? 0 : vol; // Clamp to 0
        field_34_music_start_time = sMusicTime_5C3024;
        field_54_music_volume_change_time = sMusicTime_5C3024 + delay;

        if (vol)
        {
            // vol state on?
            field_4C_state = 3;
        }
        else
        {
            // vol state off ?
            field_4C_state = 2;
        }
    }
}

void MusicController::UpdateVolumeState_47EFD0()
{
    switch (field_4C_state)
    {
    case 0:
        field_30_music_time = 0;
        field_34_music_start_time = sMusicTime_5C3024;
        field_48_last_music_frame = sMusicTime_5C3024;
        break;

    case 1:
        // volume off ?
        break;

    case 2:
        field_4C_state = 0;
        field_50_current_vol = 0;

        if (field_2C_flags_and_seq_idx > 0)
        {
            SND_SEQ_Stop_4CAE60(field_2C_flags_and_seq_idx);
            field_2C_flags_and_seq_idx = -1;
        }

        if (field_40_flags_and_idx > 0)
        {
            SND_SEQ_Stop_4CAE60(field_40_flags_and_idx);
            field_40_flags_and_idx = -1;
        }
        break;

    case 3:
        // change volume ?
        field_50_current_vol = field_52_target_volume;
        field_4C_state = 1; // Playing ?

        if (field_2C_flags_and_seq_idx > 0)
        {
            SND_SEQ_SetVol_4CAD20(field_2C_flags_and_seq_idx, field_50_current_vol, field_50_current_vol);
        }

        if (field_40_flags_and_idx > 0)
        {
            SND_SEQ_SetVol_4CAD20(field_40_flags_and_idx, field_50_current_vol, field_50_current_vol);
        }
        break;

    default:
        break;
    }
}

void MusicController::VScreenChanged()
{
    vsub_ScreenChange_47F8F0();
}

void MusicController::vsub_ScreenChange_47F8F0()
{
    field_58_flags.Set(Flags_58::e58_ScreenChanged_Bit2);
}

void MusicController::VUpdate()
{
    Update_47F730();
}

void MusicController::Update_47F730()
{
    MusicController::UpdateMusicTime_47F8B0();

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_58_flags.Set(Flags_58::e58_Dead_Bit3);
        field_28_object_id = sActiveHero_5C1B68->field_8_object_id;
    }

    if (field_58_flags.Get(Flags_58::e58_ScreenChanged_Bit2))
    {
        field_58_flags.Clear(Flags_58::e58_ScreenChanged_Bit2);

        if (gMap_5C3030.field_0_current_level != field_24_currentLevelID)
        {
            field_44 = 0;
            field_3C_unused = 1;
            field_30_music_time = 0;
            field_58_flags.Clear(Flags_58::e58_Dead_Bit3);
            field_58_flags.Set(Flags_58::e58_Bit4);
            field_48_last_music_frame = sMusicTime_5C3024;
            field_28_object_id = -1;

            if (field_2C_flags_and_seq_idx > 0)
            {
                SND_SEQ_Stop_4CAE60(field_2C_flags_and_seq_idx);
                field_2C_flags_and_seq_idx = -1;
            }

            if (field_40_flags_and_idx > 0)
            {
                SND_SEQ_Stop_4CAE60(field_40_flags_and_idx);
                field_40_flags_and_idx = -1;
            }

            field_24_currentLevelID = gMap_5C3030.field_0_current_level;

            // music on flag ?
            if (field_58_flags.Get(Flags_58::e58_MusicEnabled_Bit1))
            {
                SetMusicVolumeDelayed_47FB00(field_20_vol, 0);
                PlayMusic_47F910(MusicTypes::eNone_0, 0, 1, 0);
            }
        }
    }

    if (field_42_type > MusicTypes::eType1 && sMusicTime_5C3024 - field_48_last_music_frame >= 160)
    {
        PlayMusic_47F910(MusicTypes::eNone_0, 0, 1, 0);
    }

    UpdateVolumeState_47EFD0();

    if (field_58_flags.Get(Flags_58::e58_MusicEnabled_Bit1))
    {
        UpdateMusic_47F260();
        UpdateAmbiance_47F0B0();
    }
}

void MusicController::PlayMusic_47F910(MusicTypes typeToSet, const BaseGameObject* pObj, s16 bFlag4, s8 bFlag0x20)
{

    MusicController::UpdateMusicTime_47F8B0();

    if (typeToSet <= MusicTypes::eType1 || pObj)
    {
        if (!sObjectIds_5C1B70.Find_449CF0(field_28_object_id))
        {
            field_28_object_id = -1;
        }

        if (field_42_type == typeToSet)
        {
            if (typeToSet != MusicTypes::eNone_0)
            {
                field_48_last_music_frame = sMusicTime_5C3024;
            }

            if (field_28_object_id != -1 && field_28_object_id == pObj->field_8_object_id)
            {
                field_58_flags.Clear(Flags_58::e58_Dead_Bit3);
                field_58_flags.Set(Flags_58::e58_Dead_Bit3, (bFlag4 & 1));
            }

            if (!(field_58_flags.Get(Flags_58::e58_UnPause_Bit6)))
            {
                field_58_flags.Clear(Flags_58::e58_UnPause_Bit6);
                field_58_flags.Set(Flags_58::e58_UnPause_Bit6, (bFlag0x20 & 1));
            }
            return;
        }

        if (!pObj)
        {
            if (field_58_flags.Get(Flags_58::e58_Dead_Bit3))
            {
                return;
            }

            if (bFlag4)
            {
                field_28_object_id = -1;
            }

            field_58_flags.Set(Flags_58::e58_UnPause_Bit6);
            field_48_last_music_frame = sMusicTime_5C3024;
            field_42_type = typeToSet;
            field_44 = 0;
        }
        else if (pObj->field_8_object_id == field_28_object_id || field_28_object_id == -1 || (!(field_58_flags.Get(Flags_58::e58_Dead_Bit3)) && (bFlag4 || typeToSet >= field_42_type)))
        {
            field_28_object_id = pObj->field_8_object_id;
            field_58_flags.Clear(Flags_58::e58_Dead_Bit3);
            field_58_flags.Set(Flags_58::e58_Dead_Bit3, (bFlag4 & 1));
            field_58_flags.Set(Flags_58::e58_UnPause_Bit6);
            field_48_last_music_frame = sMusicTime_5C3024;
            field_42_type = typeToSet;
            field_44 = 0;
        }

    }
}

void MusicController::UpdateMusic_47F260()
{
    s32 idx = 0;
    const MusicController_Record* pRecord = nullptr;

    if (field_40_flags_and_idx < 0
        || !SND_SsIsEos_DeInlined_4CACD0(field_40_flags_and_idx)
        || (field_58_flags.Get(Flags_58::e58_UnPause_Bit6)
        && ( field_42_type == MusicTypes::eChime_2
            || field_42_type == MusicTypes::eType3
            || field_42_type == MusicTypes::eDeathShort_10
            || field_42_type == MusicTypes::eDeathLong_11
            || field_42_type == MusicTypes::eSecretAreaShort_12
            || field_42_type == MusicTypes::eSecretAreaLong_13
            || field_42_type == MusicTypes::eSlogChase_7
            || field_42_type == MusicTypes::eChase_8)))
    {
        if (field_40_flags_and_idx > 0)
        {
            SND_SEQ_Stop_4CAE60(field_40_flags_and_idx);
        }

        switch (field_42_type)
        {
        case MusicTypes::eChime_2: // Silence/base line only?
            field_58_flags.Clear(Flags_58::e58_AmbientMusicEnabled_Bit5);
            field_3C_unused = 1;
            SetMusicVolumeDelayed_47FB00(field_22_vol, 0);
            break;
        case MusicTypes::eType3: // The rupture farms screen change random ambiance?
            if (field_58_flags.Get(Flags_58::e58_UnPause_Bit6))
            {
                idx = Math_RandomRange_496AB0(0, 1);
            }
            else
            {
                idx = -1;
            }
            field_58_flags.Clear(Flags_58::e58_AmbientMusicEnabled_Bit5);
            field_3C_unused = 1;
            SetMusicVolumeDelayed_47FB00(field_22_vol, 0);
            break;
        case MusicTypes::eTension_4: // danger near music - when slig is near
        case MusicTypes::eSlogTension_5: // slog tension
        case MusicTypes::eSlogChaseTension_6: // slog tension 2?
            if (field_42_type == MusicTypes::eTension_4)
            {
                pRecord = &tension_55D2D0[static_cast<s32>(field_24_currentLevelID)];
            }
            else if (field_42_type == MusicTypes::eSlogTension_5)
            {
                pRecord = &slogTension_55D39C[static_cast<s32>(field_24_currentLevelID)];
            }
            else if (field_42_type == MusicTypes::eSlogChaseTension_6)
            {
                pRecord = &slogChaseTension_55D424[static_cast<s32>(field_24_currentLevelID)];
            }
            idx = pRecord->field_0_seq_id_idx;
            field_3C_unused = pRecord->field_1_unused;
            field_58_flags.Clear(Flags_58::e58_AmbientMusicEnabled_Bit5);
            field_58_flags.Set(Flags_58::e58_AmbientMusicEnabled_Bit5, (pRecord->field_2_bAmbient_music_enabled == AmbientMusic::eOn));
            SetMusicVolumeDelayed_47FB00(sSeqData_558D50.mSeqs[stru_55D008[pRecord->field_0_seq_id_idx].field_0_idx].field_9_volume, 0);
            break;
        case MusicTypes::eSlogChase_7: // chase music
            pRecord = &slogChase_55D3E0[static_cast<s32>(field_24_currentLevelID)];
            idx = pRecord->field_0_seq_id_idx;
            field_3C_unused = pRecord->field_1_unused;
            field_58_flags.Clear(Flags_58::e58_AmbientMusicEnabled_Bit5);
            field_58_flags.Set(Flags_58::e58_AmbientMusicEnabled_Bit5, (pRecord->field_2_bAmbient_music_enabled == AmbientMusic::eOn));
            SetMusicVolumeDelayed_47FB00(sSeqData_558D50.mSeqs[stru_55D008[idx].field_0_idx].field_9_volume, 0);
            break;
        case MusicTypes::eChase_8: // slig chase?
            pRecord = &chase_55D314[static_cast<s32>(field_24_currentLevelID)];
            field_3C_unused = pRecord->field_1_unused;
            idx = pRecord->field_0_seq_id_idx;
            field_58_flags.Clear(Flags_58::e58_AmbientMusicEnabled_Bit5);
            field_58_flags.Set(Flags_58::e58_AmbientMusicEnabled_Bit5, (pRecord->field_2_bAmbient_music_enabled == AmbientMusic::eOn));
            SetMusicVolumeDelayed_47FB00(sSeqData_558D50.mSeqs[stru_55D008[idx].field_0_idx].field_9_volume, 0);
            break;
        case MusicTypes::ePossessed_9: // slig possesed
            if (field_58_flags.Get(Flags_58::e58_UnPause_Bit6))
            {
                pRecord = &possessed_55D358[static_cast<s32>(field_24_currentLevelID)];
                field_3C_unused = pRecord->field_1_unused;
                idx = possessed_55D358[static_cast<s32>(field_24_currentLevelID)].field_0_seq_id_idx;
                field_58_flags.Clear(Flags_58::e58_AmbientMusicEnabled_Bit5);
                field_58_flags.Set(Flags_58::e58_AmbientMusicEnabled_Bit5, (pRecord->field_2_bAmbient_music_enabled == AmbientMusic::eOn));
                SetMusicVolumeDelayed_47FB00(sSeqData_558D50.mSeqs[stru_55D008[idx].field_0_idx].field_9_volume, 0);
            }
            else
            {
                field_58_flags.Set(Flags_58::e58_AmbientMusicEnabled_Bit5);
                field_3C_unused = 20;
                SetMusicVolumeDelayed_47FB00(field_20_vol, 30);
                field_58_flags.Set(Flags_58::e58_Bit7);
            }
            break;
        case MusicTypes::eDeathShort_10: // Death jingle s16
            field_3C_unused = 1;
            idx = field_58_flags.Get(Flags_58::e58_UnPause_Bit6)? 2 : -1;
            field_58_flags.Clear(Flags_58::e58_AmbientMusicEnabled_Bit5);
            SetMusicVolumeDelayed_47FB00(field_22_vol, 0);
            break;
        case MusicTypes::eDeathLong_11: // Death jingle long
            field_3C_unused = 1;
            idx = field_58_flags.Get(Flags_58::e58_UnPause_Bit6)? 3 : -1;
            field_58_flags.Clear(Flags_58::e58_AmbientMusicEnabled_Bit5);
            SetMusicVolumeDelayed_47FB00(field_22_vol, 0);
            break;
        case MusicTypes::eSecretAreaShort_12: // secret area s16
            field_3C_unused = 120;
            idx = field_58_flags.Get(Flags_58::e58_UnPause_Bit6)? 4 : -1;
            field_58_flags.Clear(Flags_58::e58_AmbientMusicEnabled_Bit5);
            SetMusicVolumeDelayed_47FB00(127, 0);
            break;
        case MusicTypes::eSecretAreaLong_13: // secret area long
            field_3C_unused = 120;
            idx = field_58_flags.Get(Flags_58::e58_UnPause_Bit6)? 5 : -1;
            field_58_flags.Clear(Flags_58::e58_AmbientMusicEnabled_Bit5);
            SetMusicVolumeDelayed_47FB00(80, 0);
            break;
        default: // no change ?
            SetMusicVolumeDelayed_47FB00(field_20_vol, 30);
            field_58_flags.Set(Flags_58::e58_AmbientMusicEnabled_Bit5);
            field_3C_unused = unused_55D468[static_cast<s32>(field_24_currentLevelID)];
            break;
        }

        // Don't play if -1 or 0 (for some reason 0 is special cased)
        if (idx > 0)
        {
            field_40_flags_and_idx = stru_55D008[idx].field_0_idx;
            field_44 = sMusicTime_5C3024 + stru_55D008[idx].field_2_duration;
            SND_SEQ_Play_4CAB10(field_40_flags_and_idx, 1, field_50_current_vol, field_50_current_vol);
        }
        else
        {
            field_40_flags_and_idx = -1;
            field_44 = 0;
        }

        field_38_unused = sMusicTime_5C3024;

        if (field_58_flags.Get(Flags_58::e58_UnPause_Bit6))
        {
            field_58_flags.Clear(Flags_58::e58_UnPause_Bit6);

            if (field_58_flags.Get(Flags_58::e58_Bit7))
            {
                field_58_flags.Clear(Flags_58::e58_Bit7);
            }
            else
            {
                field_58_flags.Set(Flags_58::e58_Bit4);
            }
        }
    }
}

void MusicController::UpdateAmbiance_47F0B0()
{
    if (field_58_flags.Get(Flags_58::e58_AmbientMusicEnabled_Bit5) || (field_2C_flags_and_seq_idx < 0))
    {
        if (field_58_flags.Get(Flags_58::e58_Bit4))
        {
            field_30_music_time = 0;
            field_58_flags.Clear(Flags_58::e58_Bit4);
            field_34_music_start_time = sMusicTime_5C3024;
        }

        s32 musicTime = sMusicTime_5C3024;
        if (sMusicTime_5C3024 >= field_30_music_time && field_58_flags.Get(Flags_58::e58_AmbientMusicEnabled_Bit5))
        {
            if (field_2C_flags_and_seq_idx > 0)
            {
                SND_SEQ_Stop_4CAE60(field_2C_flags_and_seq_idx);
                musicTime = sMusicTime_5C3024;
            }

            s16 random = 0; // TODO: minus 1 ??
            // TODO: Check this logic, seems to have changed a lot since AO so might
            // be bugged
            if (field_42_type != MusicTypes::eNone_0)
            {
                random = Math_RandomRange_496AB0(
                    stru_55D1E0[static_cast<s32>(field_24_currentLevelID)].field_0[0].field_2_min,
                    stru_55D1E0[static_cast<s32>(field_24_currentLevelID)].field_0[0].field_3_max);
            }
            else
            {
                const s32 calculatedTime = musicTime - field_34_music_start_time;
                s32 total = 0;
                bool found = false;
                for (const MusicController_Record3_Sub& rec : stru_55D1E0[static_cast<s32>(field_24_currentLevelID)].field_0)
                {
                    total += rec.field_0;
                    if (rec.field_0 < 0 || calculatedTime < total)
                    {
                        random = Math_RandomRange_496AB0(rec.field_2_min, rec.field_3_max);
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    field_2C_flags_and_seq_idx = -1;
                    field_30_music_time = sMusicTime_5C3024;
                    return;
                }
            }

            if (random < 0)
            {
                field_2C_flags_and_seq_idx = -1;
                field_30_music_time = sMusicTime_5C3024;
            }
            else
            {
                field_2C_flags_and_seq_idx = stru_55D008[random].field_0_idx;
                SND_SEQ_Play_4CAB10(field_2C_flags_and_seq_idx, stru_55D1E0[static_cast<s32>(field_24_currentLevelID)].field_C_repeat_count, field_50_current_vol, field_50_current_vol);
                field_30_music_time = sMusicTime_5C3024 + stru_55D008[random].field_2_duration;
            }
        }
    }
    else
    {
        SND_SEQ_Stop_4CAE60(field_2C_flags_and_seq_idx);
        field_2C_flags_and_seq_idx = -1;
    }
}

void CC MusicController::PlayMusic_47FD60(MusicTypes typeToSet, const BaseGameObject* pObj, s16 bFlag4, s8 bFlag0x20)
{
    if (pMusicController_5C3020)
    {
        pMusicController_5C3020->PlayMusic_47F910(typeToSet, pObj, bFlag4, bFlag0x20);
    }
}

void CC MusicController::EnableMusic_47FE10(s16 bOn)
{
    pMusicController_5C3020->EnableMusic_47FB80(bOn);
}
