#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "MusicController.hpp"
#include "Midi.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Abe.hpp"
#include "../relive_lib/Events.hpp"
#include "Elum.hpp"
#include "Math.hpp"
#include "GameAutoPlayer.hpp"

namespace AO {

MusicController* pMusicController_507B98 = nullptr;

using TRootCallBackFn = decltype(&MusicController::OnRootCounter);

constexpr auto RCntCNT3 = 0xf2000003; // VSync (VBlank)

// TODO: Move out PSX emu parts
s32 psx_root_event_507BA0 = 0;
TRootCallBackFn sRootCounerFn_AC0BE0 = nullptr;
s32 sMusicTime_507B9C = 0;


s32 Psx_Root_Counter_49C280(s32 event, s32 unknown1, s32 unknown2, TRootCallBackFn pFn)
{
    if (event == RCntCNT3 && unknown1 == 2 && unknown2 == 4096)
    {
        sRootCounerFn_AC0BE0 = pFn;
    }
    return 1;
}

s32 Psx_Root_Counter_Event_Free_49C2B0(s32 event)
{
    if (event == RCntCNT3)
    {
        sRootCounerFn_AC0BE0 = nullptr;
    }
    return 1;
}

struct MusicController_Record3_Sub final
{
    s32 field_0_count;
    s16 field_4_min;
    s16 field_6_max;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record3_Sub, 8);

struct MusicController_Record3 final
{
    MusicController_Record3_Sub field_0[3];
    s16 field_18;
    s16 field_1A;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record3, 0x1C);

const MusicController_Record3 rec3s_4CD798[16] = {
    {{{640, 112, 116}, {-1, 107, 111}, {-1, 0, 0}}, 1u, 0u},
    {{{320, 97, 100}, {640, 94, 96}, {-1, 90, 93}}, 1u, 0u},
    {{{512, 81, 85}, {1024, 76, 80}, {-1, 71, 75}}, 1u, 0u},
    {{{640, 54, 57}, {1280, 51, 53}, {-1, 48, 50}}, 4u, 0u},
    {{{512, 64, 64}, {1024, 63, 63}, {-1, 62, 62}}, 1u, 0u},
    {{{640, 40, 44}, {-1, 35, 39}, {-1, 0, 0}}, 1u, 0u},
    {{{640, 40, 44}, {-1, 35, 39}, {-1, 0, 0}}, 1u, 0u},
    {{{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, 1u, 0u},
    {{{640, 14, 18}, {1280, 9, 13}, {-1, 6, 8}}, 1u, 0u},
    {{{448, 27, 28}, {896, 25, 26}, {-1, 23, 24}}, 1u, 0u},
    {{{640, 112, 116}, {-1, 107, 111}, {-1, 0, 0}}, 1u, 0u},
    {{{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, 1u, 0u},
    {{{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, 1u, 0u},
    {{{320, 97, 100}, {640, 94, 96}, {-1, 90, 93}}, 1u, 0u},
    {{{-1, 68, 68}, {-1, -1, -1}, {-1, -1, -1}}, 1u, 0u},
    {{{-1, 32, 32}, {-1, -1, -1}, {-1, -1, -1}}, 1u, 0u}};

struct MusicController_Record final
{
    s16 field_0_seqIdx;
    s16 field_2;
    s32 field_4;
    s16 field_8_bAmibentEnabled;
    s16 field_A;
    s16 field_C;
    s16 field_E;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record, 0x10);

const MusicController_Record array_1_stru_4CD958[] = {
    {-1, 0, 1, 1, 0, 0, 0},
    {101, 0, 20, 0, 0, 0, 0},
    {87, 0, 16, 0, 0, 0, 0},
    {59, 0, 20, 0, 0, 0, 0},
    {65, 0, 16, 0, 0, 0, 0},
    {45, 0, 16, 0, 0, 0, 0},
    {45, 0, 16, 0, 0, 0, 0},
    {-1, 0, 1, 1, 0, 0, 0},
    {20, 0, 20, 0, 0, 0, 0},
    {29, 0, 16, 0, 0, 0, 0},
    {-1, 0, 1, 1, 0, 0, 0},
    {-1, 0, 1, 1, 0, 0, 0},
    {-1, 0, 1, 1, 0, 0, 0},
    {101, 0, 20, 0, 0, 0, 0},
    {65, 0, 16, 0, 0, 0, 0},
    {29, 0, 16, 0, 0, 0, 0}};

const MusicController_Record array_2_stru_4CDA58[] = {
    {-1, 0, 1, 1, 0, 0, 0},
    {102, 0, 20, 0, 0, 0, 0},
    {88, 0, 16, 0, 0, 0, 0},
    {60, 0, 20, 0, 0, 0, 0},
    {66, 0, 16, 0, 0, 0, 0},
    {46, 0, 16, 0, 0, 0, 0},
    {46, 0, 16, 0, 0, 0, 0},
    {-1, 0, 1, 1, 0, 0, 0},
    {21, 0, 20, 0, 0, 0, 0},
    {30, 0, 16, 0, 0, 0, 0},
    {-1, 0, 1, 1, 0, 0, 0},
    {-1, 0, 1, 1, 0, 0, 0},
    {-1, 0, 1, 1, 0, 0, 0},
    {102, 0, 20, 0, 0, 0, 0},
    {66, 0, 16, 0, 0, 0, 0},
    {30, 0, 16, 0, 0, 0, 0}};

const MusicController_Record array_3_stru_4CDB58[] = {
    {-1, 0, 1, 1, 0, 0, 0},
    {103, 0, 20, 0, 0, 0, 0},
    {89, 0, 16, 0, 0, 0, 0},
    {61, 0, 20, 0, 0, 0, 0},
    {67, 0, 16, 0, 0, 0, 0},
    {47, 0, 16, 0, 0, 0, 0},
    {47, 0, 16, 0, 0, 0, 0},
    {-1, 0, 1, 1, 0, 0, 0},
    {22, 0, 20, 0, 0, 0, 0},
    {31, 0, 16, 0, 0, 0, 0},
    {-1, 0, 1, 1, 0, 0, 0},
    {-1, 0, 1, 1, 0, 0, 0},
    {-1, 0, 1, 1, 0, 0, 0},
    {103, 0, 20, 0, 0, 0, 0},
    {67, 0, 16, 0, 0, 0, 0},
    {31, 0, 16, 0, 0, 0, 0},

    // TODO: Are these part of something else?
    {20, 20, 1310736, 16, 16, 16, 1},
    {20, 16, 65556, 1, 20, 16, 16}};

struct MusicController_Record2 final
{
    SeqId field_0_idx;
    u16 field_2_duration;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record2, 4);

const MusicController_Record2 rec2s_4CD5A8[124] = {
    {SeqId::Unknown_41, 320},
    {SeqId::Unknown_42, 320},
    {SeqId::Unknown_43, 320},
    {SeqId::Unknown_44, 320},
    {SeqId::eSaveTriggerMusic_45, 320},
    {SeqId::eSecretMusic_46, 320},
    {SeqId::Unknown_47, 640},
    {SeqId::Unknown_48, 640},
    {SeqId::Unknown_49, 640},
    {SeqId::Unknown_50, 640},
    {SeqId::Unknown_51, 640},
    {SeqId::Unknown_52, 640},
    {SeqId::Unknown_53, 640},
    {SeqId::Unknown_54, 640},
    {SeqId::Unknown_55, 640},
    {SeqId::Unknown_56, 640},
    {SeqId::Unknown_57, 640},
    {SeqId::Unknown_58, 640},
    {SeqId::Unknown_59, 640},
    {SeqId::Unknown_60, 160},
    {SeqId::Unknown_61, 256},
    {SeqId::Unknown_62, 256},
    {SeqId::Unknown_63, 512},
    {SeqId::Unknown_64, 448},
    {SeqId::Unknown_65, 448},
    {SeqId::Unknown_66, 448},
    {SeqId::Unknown_67, 448},
    {SeqId::Unknown_68, 448},
    {SeqId::Unknown_69, 448},
    {SeqId::Unknown_70, 256},
    {SeqId::Unknown_71, 256},
    {SeqId::Unknown_72, 512},
    {SeqId::Unknown_73, 512},
    {SeqId::Unknown_74, 512},
    {SeqId::Unknown_75, 1024},
    {SeqId::Unknown_76, 640},
    {SeqId::Unknown_77, 640},
    {SeqId::Unknown_78, 640},
    {SeqId::Unknown_79, 640},
    {SeqId::Unknown_80, 640},
    {SeqId::Unknown_81, 640},
    {SeqId::Unknown_82, 640},
    {SeqId::Unknown_83, 640},
    {SeqId::Unknown_84, 640},
    {SeqId::Unknown_85, 640},
    {SeqId::Unknown_86, 256},
    {SeqId::Unknown_87, 256},
    {SeqId::Unknown_88, 512},
    {SeqId::Unknown_89, 640},
    {SeqId::Unknown_90, 640},
    {SeqId::Unknown_91, 640},
    {SeqId::Unknown_92, 640},
    {SeqId::Unknown_93, 640},
    {SeqId::Unknown_94, 640},
    {SeqId::Unknown_95, 640},
    {SeqId::Unknown_96, 640},
    {SeqId::Unknown_97, 640},
    {SeqId::Unknown_98, 640},
    {SeqId::Unknown_99, 160},
    {SeqId::Unknown_100, 320},
    {SeqId::Unknown_101, 320},
    {SeqId::Unknown_102, 320},
    {SeqId::Unknown_103, 512},
    {SeqId::Unknown_104, 512},
    {SeqId::Unknown_105, 512},
    {SeqId::Unknown_106, 256},
    {SeqId::Unknown_107, 256},
    {SeqId::Unknown_108, 512},
    {SeqId::Unknown_109, 512},
    {SeqId::Unknown_110, 256},
    {SeqId::Unknown_111, 1024},
    {SeqId::Unknown_112, 512},
    {SeqId::Unknown_113, 512},
    {SeqId::Unknown_114, 512},
    {SeqId::Unknown_115, 512},
    {SeqId::Unknown_116, 512},
    {SeqId::Unknown_117, 512},
    {SeqId::Unknown_118, 512},
    {SeqId::Unknown_119, 512},
    {SeqId::Unknown_120, 512},
    {SeqId::Unknown_121, 512},
    {SeqId::Unknown_122, 512},
    {SeqId::Unknown_123, 512},
    {SeqId::Unknown_124, 512},
    {SeqId::Unknown_125, 512},
    {SeqId::Unknown_126, 512},
    {SeqId::Unknown_127, 256},
    {SeqId::Unknown_128, 256},
    {SeqId::Unknown_129, 256},
    {SeqId::Unknown_130, 512},
    {SeqId::Unknown_RF_131, 320},
    {SeqId::Unknown_RF_132, 320},
    {SeqId::Unknown_RF_133, 320},
    {SeqId::Unknown_134, 320},
    {SeqId::Unknown_135, 320},
    {SeqId::Unknown_136, 320},
    {SeqId::Unknown_RF_137, 320},
    {SeqId::Unknown_138, 320},
    {SeqId::Unknown_139, 320},
    {SeqId::Unknown_RF_140, 320},
    {SeqId::Unknown_141, 320},
    {SeqId::Unknown_142, 320},
    {SeqId::Unknown_143, 320},
    {SeqId::Unknown_144, 320},
    {SeqId::Unknown_145, 768},
    {SeqId::Unknown_146, 384},
    {SeqId::Unknown_147, 384},
    {SeqId::Unknown_148, 640},
    {SeqId::Unknown_149, 640},
    {SeqId::Unknown_150, 640},
    {SeqId::Unknown_151, 640},
    {SeqId::Unknown_152, 640},
    {SeqId::Unknown_153, 640},
    {SeqId::Unknown_154, 640},
    {SeqId::Unknown_155, 640},
    {SeqId::Unknown_156, 640},
    {SeqId::Unknown_157, 640},
    {SeqId::Unknown_158, 176},
    {SeqId::Unknown_159, 176},
    {SeqId::Unknown_160, 176},
    {SeqId::Unknown_161, 176},
    {SeqId::Unknown_162, 176},
    {SeqId::Unknown_163, 220},
    {SeqId::Unknown_0, 0}};

s16 MusicController::Create()
{
    if (pMusicController_507B98)
    {
        return 0;
    }

    pMusicController_507B98 = relive_new MusicController();
    if (pMusicController_507B98)
    {
        MusicController::SetBaseTimeStamp();

        psx_root_event_507BA0 = Psx_Root_Counter_49C280(RCntCNT3, 2, 4096, MusicController::OnRootCounter);

        if (pMusicController_507B98->field_10_bEnableMusic)
        {
            pMusicController_507B98->field_10_bEnableMusic = 0;
            if (pMusicController_507B98->field_4C_current_vol)
            {
                pMusicController_507B98->field_4A_starting_volume = pMusicController_507B98->field_4C_current_vol;
                pMusicController_507B98->field_4E_vol = 0;
                pMusicController_507B98->field_2C_music_start_time = GetMusicTime();
                pMusicController_507B98->field_50_music_volume_change_time = GetMusicTime();
                pMusicController_507B98->field_48_vol_state = 2;
            }
            pMusicController_507B98->UpdateVolumeState();
        }
    }

    return 1;
}

static u32 sMusicControllerBaseTimeStamp = 0;

void MusicController::SetBaseTimeStamp()
{
    sMusicControllerBaseTimeStamp = GetGameAutoPlayer().SysGetTicks();
}

void MusicController::UpdateMusicTime()
{
    sMusicTime_507B9C = (3 * GetGameAutoPlayer().SysGetTicks() - 3 * sMusicControllerBaseTimeStamp) / 100;
}

s32 MusicController::GetMusicTime()
{
    return sMusicTime_507B9C;
}

MusicController::MusicController()
    : BaseGameObject(true, 0)
{
    SetSurviveDeathReset(true);
    SetType(ReliveTypes::eNone);

    field_10_bEnableMusic = 1;
    field_3A_type = MusicTypes::eType0;
    field_16_bScreenChanged = 1;
    field_1C_pObj = nullptr;
    field_20 = 0;
    field_28_amibent_seq_duration = 0;
    field_22 = 1;
    field_24_bAmbientMusicEnabled = 1;
    field_2C_music_start_time = 0;
    field_40_started_time = 0;
    field_30_music_timer = 0;
    field_34_sync_after_beats = 1;
    field_48_vol_state = 0;
    field_4A_starting_volume = 0;
    field_4C_current_vol = 0;
    field_4E_vol = 0;
    field_50_music_volume_change_time = 0;
    field_12_target_volume = 70;
    field_14 = 100;
    field_38_music_seq = SeqId::None_M1;
    field_18_level = EReliveLevelIds::eNone;
    field_26_ambient_seq = SeqId::None_M1;
}

MusicController::~MusicController()
{
    if (field_38_music_seq != SeqId::None_M1)
    {
        SND_SEQ_Stop(field_38_music_seq);
    }
}

void MusicController::VScreenChanged()
{
    field_16_bScreenChanged = 1;
}

void MusicController::VUpdate()
{
    MusicController::UpdateMusicTime();

    if (EventGet(kEventDeathReset))
    {
        field_20 = 1;
        field_1C_pObj = sActiveHero;
    }

    if (field_16_bScreenChanged)
    {
        field_16_bScreenChanged = false;

        if (gMap.mCurrentLevel != field_18_level)
        {
            field_3C_music_seq_duration = 0;
            field_34_sync_after_beats = 1;
            field_40_started_time = GetMusicTime();
            field_28_amibent_seq_duration = 0;
            field_22 = 1;
            field_1C_pObj = nullptr;
            field_20 = 0;

            if (field_26_ambient_seq > SeqId::None_M1)
            {
                SND_SEQ_Stop(field_26_ambient_seq);
                field_26_ambient_seq = SeqId::None_M1;
            }

            if (field_38_music_seq > SeqId::None_M1)
            {
                SND_SEQ_Stop(field_38_music_seq);
                field_38_music_seq = SeqId::None_M1;
            }

            field_18_level = gMap.mCurrentLevel;

            if (field_10_bEnableMusic)
            {
                SetMusicVolumeDelayed(field_12_target_volume, 0);
                PlayMusic(MusicTypes::eType0, nullptr, 1, 0);
            }
        }
    }

    if (field_3A_type > MusicTypes::eAbeOnElum_1 && GetMusicTime() - field_40_started_time >= 160)
    {
        PlayMusic(MusicTypes::eType0, nullptr, 1, 0);
    }

    UpdateVolumeState();

    if (field_10_bEnableMusic)
    {
        UpdateMusic();
        UpdateAmbiance();
    }
}

void MusicController::static_PlayMusic(MusicTypes musicType, BaseGameObject* pObj, s16 a3, s16 a4)
{
    if (pMusicController_507B98)
    {
        pMusicController_507B98->PlayMusic(musicType, pObj, a3, a4);
    }
}

void MusicController::ClearObject(BaseGameObject* pObj)
{
    if (pMusicController_507B98)
    {
        pMusicController_507B98->DoClearObject(pObj);
    }
}

MusicController::MusicTypes MusicController::GetAbmientAndMusicInfo(SeqId* ambientSeq, SeqId* musicSeq, u16* ambientOrMusicDuration)
{
    MusicController::UpdateMusicTime();

    if (!pMusicController_507B98)
    {
        return MusicTypes::eTypeNull;
    }

    if (ambientSeq)
    {
        *ambientSeq = pMusicController_507B98->field_26_ambient_seq;
    }

    if (musicSeq)
    {
        *musicSeq = pMusicController_507B98->field_38_music_seq;
    }

    if (ambientOrMusicDuration)
    {
        if (pMusicController_507B98->field_3A_type == MusicTypes::eType0)
        {
            *ambientOrMusicDuration = static_cast<u16>(pMusicController_507B98->field_28_amibent_seq_duration - GetMusicTime());
        }
        else
        {
            *ambientOrMusicDuration = static_cast<u16>(pMusicController_507B98->field_3C_music_seq_duration - GetMusicTime());
        }
    }
    return pMusicController_507B98->field_3A_type;
}

void MusicController::UpdateVolumeState()
{
    switch (field_48_vol_state)
    {
        case 0:
            field_28_amibent_seq_duration = 0;
            field_2C_music_start_time = GetMusicTime();
            field_40_started_time = GetMusicTime();
            break;

        case 1:
            break;

        case 2:
            field_48_vol_state = 0;
            field_4C_current_vol = 0;

            if (field_26_ambient_seq > SeqId::None_M1)
            {
                SND_SEQ_Stop(field_26_ambient_seq);
                field_26_ambient_seq = SeqId::None_M1;
            }

            if (field_38_music_seq > SeqId::None_M1)
            {
                SND_SEQ_Stop(field_38_music_seq);
                field_38_music_seq = SeqId::None_M1;
            }
            break;

        case 3:
            field_4C_current_vol = field_4E_vol;
            field_48_vol_state = 1;
            if (field_26_ambient_seq > SeqId::None_M1)
            {
                SND_SEQ_SetVol(field_26_ambient_seq, field_4C_current_vol, field_4C_current_vol);
            }

            if (field_38_music_seq > SeqId::None_M1)
            {
                SND_SEQ_SetVol(field_38_music_seq, field_4C_current_vol, field_4C_current_vol);
            }
            break;

        default:
            break;
    }
}

void MusicController::Shutdown()
{
    if (pMusicController_507B98)
    {
        pMusicController_507B98->SetDead(true);
        pMusicController_507B98 = nullptr;
        //nullsub_5();
        Psx_Root_Counter_Event_Free_49C2B0(psx_root_event_507BA0);
        //nullsub_6();
    }
}

void MusicController::EnableMusic(s16 bEnable)
{
    MusicController::UpdateMusicTime();

    if (pMusicController_507B98->field_10_bEnableMusic != bEnable)
    {
        pMusicController_507B98->field_10_bEnableMusic = bEnable;
        if (bEnable)
        {
            pMusicController_507B98->SetMusicVolumeDelayed(pMusicController_507B98->field_12_target_volume, 0);

            pMusicController_507B98->field_3C_music_seq_duration = 0;
            pMusicController_507B98->field_28_amibent_seq_duration = 0;
            pMusicController_507B98->field_40_started_time = GetMusicTime();

            if (pMusicController_507B98->field_3A_type == MusicTypes::eType0 || pMusicController_507B98->field_3A_type == MusicTypes::eIntenseChase_8 || pMusicController_507B98->field_3A_type == MusicTypes::eType11)
            {
                pMusicController_507B98->field_44_bTypeChanged = 1;
            }
        }
        else
        {
            pMusicController_507B98->SetMusicVolumeDelayed(0, 0);
        }
        pMusicController_507B98->UpdateVolumeState();
    }
}

s32 MusicController::OnRootCounter()
{
    sMusicTime_507B9C++;
    return 0;
}

void MusicController::PlayMusic(MusicTypes musicType, BaseGameObject* pObj, s16 a4, s16 a5)
{
    MusicController::UpdateMusicTime();

    if (musicType == MusicTypes::eType0 || musicType == MusicTypes::eAbeOnElum_1 || pObj)
    {
        if (musicType == MusicTypes::eType0)
        {
            if (gElum)
            {
                if (sControlledCharacter == gElum)
                {
                    musicType = MusicTypes::eAbeOnElum_1;
                }
            }
        }

        if (musicType == field_3A_type)
        {
            if (musicType != MusicTypes::eType0)
            {
                field_40_started_time = GetMusicTime();
            }

            if (field_1C_pObj)
            {
                if (field_1C_pObj == pObj) // Comparing a deleted ptr
                {
                    field_20 = a4;
                }
            }

            if (!field_44_bTypeChanged)
            {
                field_44_bTypeChanged = a5;
            }
            return;
        }

        if (!pObj)
        {
            if (field_20)
            {
                return;
            }

            if (a4)
            {
                field_1C_pObj = nullptr;
            }

            field_3A_type = musicType;
            field_40_started_time = GetMusicTime();
            field_3C_music_seq_duration = 0;
            field_44_bTypeChanged = 1;
            return;
        }

        if (pObj == field_1C_pObj || !field_1C_pObj || (!field_20 && (a4 || musicType >= field_3A_type)))
        {
            field_1C_pObj = pObj;
            field_20 = a4;
            field_3A_type = musicType;
            field_40_started_time = GetMusicTime();
            field_3C_music_seq_duration = 0;
            field_44_bTypeChanged = 1;
            return;
        }
    }
}

void MusicController::DoClearObject(BaseGameObject* pObj)
{
    if (field_1C_pObj && field_1C_pObj == pObj)
    {
        // HACK: Here we want to clear a pointer to an object that has been deleted, this is because sometimes a new object
        // can be allocated that has the same address as the deleted object leading to undeterminsitic behaviour.
        // However the game logic also depends on the pointer value not being nullptr, hence we hack it to a value of 1.
        field_1C_pObj = reinterpret_cast<BaseGameObject*>(0x1);
    }
}

s16 MusicController::SetMusicVolumeDelayed(s16 vol, s16 delay)
{
    s16 ret = 0;
    if (vol != field_4C_current_vol)
    {
        const s32 counterVal = GetMusicTime();

        field_4A_starting_volume = field_4C_current_vol;
        ret = (vol <= 0) ? 0 : vol;

        field_2C_music_start_time = counterVal;
        field_50_music_volume_change_time = counterVal + delay;

        if (vol)
        {
            // vol state on?
            field_48_vol_state = 3;
        }
        else
        {
            // vol state off ?
            field_48_vol_state = 2;
        }
    }
    return ret;
}

void MusicController::UpdateMusic()
{
    const s32 counterVal = GetMusicTime();

    if (counterVal >= field_3C_music_seq_duration && !((counterVal - field_30_music_timer) % field_34_sync_after_beats))
    {
        if (field_38_music_seq != SeqId::None_M1)
        {
            SND_SEQ_Stop(field_38_music_seq);
        }

        s32 idx = 0;
        switch (field_3A_type)
        {
            case MusicTypes::eAbeOnElum_1:
                if (field_18_level == EReliveLevelIds::eLines)
                {
                    idx = 86;
                    field_34_sync_after_beats = 16;
                }
                else if (field_18_level == EReliveLevelIds::eForest)
                {
                    idx = 58;
                    field_34_sync_after_beats = 20;
                }
                else if (field_18_level == EReliveLevelIds::eDesert)
                {
                    idx = 19;
                    field_34_sync_after_beats = 20;
                }
                else
                {
                    idx = -1;
                    field_34_sync_after_beats = 20;
                }
                field_4E_vol = SetMusicVolumeDelayed(field_12_target_volume, 30);
                field_24_bAmbientMusicEnabled = 1;
                field_22 = 1;
                break;

            case MusicTypes::eChime_2:
                field_34_sync_after_beats = 1;
                idx = -1;
                field_24_bAmbientMusicEnabled = 0;
                field_4E_vol = SetMusicVolumeDelayed(field_14, 0);
                break;

            case MusicTypes::eDrumAmbience_3:
                idx = field_44_bTypeChanged ? Math_RandomRange(0, 1) : -1;
                field_34_sync_after_beats = 1;
                field_24_bAmbientMusicEnabled = 0;
                field_4E_vol = SetMusicVolumeDelayed(field_14, 0);
                break;

            case MusicTypes::eChase_4:
                [[fallthrough]];
            case MusicTypes::eSlogChase_5:
            {
                const MusicController_Record* pRec = field_3A_type == MusicTypes::eChase_4 ? &array_1_stru_4CD958[static_cast<s32>(MapWrapper::ToAO(field_18_level))] : &array_2_stru_4CDA58[static_cast<s32>(MapWrapper::ToAO(field_18_level))];
                idx = pRec->field_0_seqIdx;
                field_24_bAmbientMusicEnabled = pRec->field_8_bAmibentEnabled;
                field_34_sync_after_beats = pRec->field_4;
                field_4E_vol = SetMusicVolumeDelayed(field_14, 0);
                break;
            }

            case MusicTypes::ePossessed_6:
                if (field_44_bTypeChanged)
                {
                    const MusicController_Record* pRec = &array_3_stru_4CDB58[static_cast<s32>(MapWrapper::ToAO(field_18_level))];
                    idx = pRec->field_0_seqIdx;
                    field_34_sync_after_beats = pRec->field_4;
                    field_24_bAmbientMusicEnabled = pRec->field_8_bAmibentEnabled;
                    field_4E_vol = SetMusicVolumeDelayed(field_14, 0);
                }
                else
                {
                    idx = -1;
                    field_34_sync_after_beats = 20;
                    field_24_bAmbientMusicEnabled = 1;
                    field_4E_vol = SetMusicVolumeDelayed(field_12_target_volume, 30);
                    field_46_restart_track = 1;
                }
                break;

            case MusicTypes::eSlogTension:
                idx = 117;
                field_34_sync_after_beats = 22;
                field_4E_vol = SetMusicVolumeDelayed(field_14, 30);
                field_24_bAmbientMusicEnabled = 0;
                break;

            case MusicTypes::eIntenseChase_8:
                if (gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom)
                {
                    idx = 102;
                    field_34_sync_after_beats = 1;
                }
                else
                {
                    if (field_44_bTypeChanged)
                    {
                        idx = 122;
                    }
                    else
                    {
                        idx = Math_RandomRange(118, 120);
                    }
                    field_34_sync_after_beats = 22;
                }

                field_4E_vol = SetMusicVolumeDelayed(field_14, 0);
                field_24_bAmbientMusicEnabled = 0;
                break;

            case MusicTypes::eSlogChaseTension:
                [[fallthrough]];

            case MusicTypes::eType10:
                idx = field_3A_type == MusicTypes::eSlogChaseTension ? 121 : 122;
                field_34_sync_after_beats = 22;
                field_4E_vol = SetMusicVolumeDelayed(field_14, 0);
                field_24_bAmbientMusicEnabled = 0;
                break;

            case MusicTypes::eType11:
                if (field_44_bTypeChanged)
                {
                    if (field_18_level == EReliveLevelIds::eForestChase)
                    {
                        idx = 69;
                        field_34_sync_after_beats = 16;
                    }
                    else if (field_18_level == EReliveLevelIds::eDesertEscape)
                    {
                        idx = 33;
                        field_34_sync_after_beats = 16;
                    }
                    else
                    {
                        idx = 117;
                        field_34_sync_after_beats = 22;
                    }

                    field_46_restart_track = 1;
                    field_24_bAmbientMusicEnabled = 0;
                    field_4E_vol = SetMusicVolumeDelayed(field_14, 0);
                }
                else
                {
                    idx = -1;
                    field_34_sync_after_beats = 16;
                    field_24_bAmbientMusicEnabled = 1;
                    field_4E_vol = SetMusicVolumeDelayed(field_12_target_volume, 30);
                    field_46_restart_track = 1;
                }
                break;

            case MusicTypes::eType12:
                if (field_18_level == EReliveLevelIds::eForestChase)
                {
                    idx = 70;
                    field_34_sync_after_beats = 16;
                }
                else if (field_18_level == EReliveLevelIds::eDesertEscape)
                {
                    idx = 34;
                    field_34_sync_after_beats = 32;
                }
                else
                {
                    idx = 122;
                    field_34_sync_after_beats = 22;
                }
                field_24_bAmbientMusicEnabled = 0;
                field_4E_vol = SetMusicVolumeDelayed(field_14, 0);
                break;

            case MusicTypes::eDeathDrumShort_13:
                idx = field_44_bTypeChanged != 0 ? 2 : -1;
                field_34_sync_after_beats = 1;
                field_24_bAmbientMusicEnabled = 0;
                field_4E_vol = SetMusicVolumeDelayed(field_14, 0);
                break;

            case MusicTypes::eDeathLong_14:
                field_34_sync_after_beats = 1;
                field_24_bAmbientMusicEnabled = 0;
                idx = field_44_bTypeChanged != 0 ? 3 : -1;
                field_4E_vol = SetMusicVolumeDelayed(field_14, 0);
                break;

            case MusicTypes::eSecretAreaShort_15:
                [[fallthrough]];

            case MusicTypes::eSecretAreaLong_16:
                if (field_3A_type == MusicTypes::eSecretAreaShort_15)
                {
                    idx = field_44_bTypeChanged != 0 ? 4 : -1;
                }
                else
                {
                    idx = field_44_bTypeChanged != 0 ? 5 : -1;
                }

                field_34_sync_after_beats = 1;
                field_24_bAmbientMusicEnabled = 0;

                if (field_4C_current_vol != 127)
                {
                    field_4A_starting_volume = field_4C_current_vol;
                    field_4E_vol = 127;
                    field_2C_music_start_time = counterVal;
                    field_50_music_volume_change_time = counterVal;
                    field_48_vol_state = 3;
                }
                break;

            default:
                idx = -1;
                field_4E_vol = SetMusicVolumeDelayed(field_12_target_volume, 30);
                field_34_sync_after_beats = 1;
                field_24_bAmbientMusicEnabled = 1;
                break;
        }

        if (idx >= 0)
        {
            field_38_music_seq = rec2s_4CD5A8[idx].field_0_idx;
            field_3C_music_seq_duration = (counterVal) + rec2s_4CD5A8[idx].field_2_duration;
            SND_SEQ_Play(field_38_music_seq, 1, field_4C_current_vol, field_4C_current_vol);
        }
        else
        {
            field_38_music_seq = SeqId::None_M1;
            field_3C_music_seq_duration = 0;
        }

        field_30_music_timer = counterVal;

        if (field_44_bTypeChanged)
        {
            field_44_bTypeChanged = 0;

            if (field_46_restart_track)
            {
                field_46_restart_track = 0;
            }
            else
            {
                field_22 = 1;
            }
        }
    }
}

void MusicController::UpdateAmbiance()
{
    if (field_24_bAmbientMusicEnabled || field_26_ambient_seq == SeqId::None_M1)
    {
        if (field_22)
        {
            field_22 = 0;
            field_28_amibent_seq_duration = 0;
            field_2C_music_start_time = GetMusicTime();
        }

        if (GetMusicTime() >= field_28_amibent_seq_duration && field_24_bAmbientMusicEnabled)
        {
            // Stop current if there is one
            if (field_26_ambient_seq != SeqId::None_M1)
            {
                SND_SEQ_Stop(field_26_ambient_seq);
            }

            s16 random = -1;
            if (field_3A_type == MusicTypes::eType0)
            {
                const MusicController_Record3* pRec = &rec3s_4CD798[static_cast<s32>(MapWrapper::ToAO(field_18_level))];
                for (const MusicController_Record3_Sub& rec : pRec->field_0)
                {
                    const auto diff = GetMusicTime() - field_2C_music_start_time;
                    if (rec.field_0_count < 0 || diff < rec.field_0_count)
                    {
                        random = Math_RandomRange(rec.field_4_min, rec.field_6_max);
                        break;
                    }
                }
            }
            else
            {
                const MusicController_Record3* pRec = &rec3s_4CD798[static_cast<s32>(MapWrapper::ToAO(field_18_level))];
                random = Math_RandomRange(pRec->field_0[0].field_4_min, pRec->field_0[0].field_6_max);
            }

            if (random < 0)
            {
                // Nothing to play
                field_26_ambient_seq = SeqId::None_M1;
                field_28_amibent_seq_duration = GetMusicTime();
            }
            else
            {
                // Play new track
                field_26_ambient_seq = rec2s_4CD5A8[random].field_0_idx;
                SND_SEQ_Play(field_26_ambient_seq, rec3s_4CD798[static_cast<s32>(MapWrapper::ToAO(field_18_level))].field_18, field_4C_current_vol, field_4C_current_vol);
                field_28_amibent_seq_duration = GetMusicTime() + rec2s_4CD5A8[random].field_2_duration;
            }
        }
    }
    else
    {
        SND_SEQ_Stop(field_26_ambient_seq);
        field_26_ambient_seq = SeqId::None_M1;
    }
}

} // namespace AO
