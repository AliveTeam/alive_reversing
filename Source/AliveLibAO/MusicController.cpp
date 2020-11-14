#include "stdafx_ao.h"
#include "Function.hpp"
#include "MusicController.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Events.hpp"
#include "Elum.hpp"
#include "Math.hpp"

START_NS_AO

ALIVE_VAR(1, 0x507B98, MusicController*, pMusicController_507B98, nullptr);

using TRootCallBackFn = decltype(&MusicController::OnRootCounter_4437D0);

constexpr auto RCntCNT3 = 0xf2000003; // VSync (VBlank)

// TODO: Move out PSX emu parts
ALIVE_VAR(1, 0x507BA0, int, psx_root_event_507BA0, 0);
ALIVE_VAR(1, 0xAC0BE0, TRootCallBackFn, dword_AC0BE0, nullptr);
ALIVE_VAR(1, 0x507B9C, int, counter_507B9C, 0);


EXPORT int CC Psx_Root_Counter_49C280(int event, int unknown1, int unknown2, TRootCallBackFn pFn)
{
    if (event == RCntCNT3 && unknown1 == 2 && unknown2 == 4096)
    {
        dword_AC0BE0 = pFn;
    }
    return 1;
}

EXPORT int CC Psx_Root_Counter_Event_Free_49C2B0(int event)
{
    if (event == RCntCNT3)
    {
        dword_AC0BE0 = nullptr;
    }
    return 1;
}

EXPORT int CC Psx_Root_Counter_49C3B0(int /*not_used*/)
{
    return 0;
}

EXPORT int CC Psx_Root_Counter_49C340(int /*a1*/, int /*a2*/)
{
    return 0;
}

EXPORT int Psx_Root_Counter_49C360(int /*not_used*/)
{
    return 0;
}

EXPORT int CC Psx_Root_Counter_49C370(int /*counter*/)
{
    return 0;
}

struct MusicController_Record3_Sub
{
    int field_0_count;
    __int16 field_4_min;
    __int16 field_6_max;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record3_Sub, 8);

struct MusicController_Record3
{
    MusicController_Record3_Sub field_0[3];
    __int16 field_18;
    __int16 field_1A;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record3, 0x1C);

const MusicController_Record3 rec3s_4CD798[16] =
{
    { { { 640, 112, 116 },    { -1, 107, 111 },     { -1, 0, 0     } }, 1u, 0u },
    { { { 320, 97, 100 },     { 640, 94, 96 },      { -1, 90, 93   } }, 1u, 0u },
    { { { 512, 81, 85 },      { 1024, 76, 80 },     { -1, 71, 75   } }, 1u, 0u },
    { { { 640, 54, 57 },      { 1280, 51, 53 },     { -1, 48, 50   } }, 4u, 0u },
    { { { 512, 64, 64 },      { 1024, 63, 63 },     { -1, 62, 62   } }, 1u, 0u },
    { { { 640, 40, 44 },      { -1, 35, 39 },       { -1, 0, 0     } }, 1u, 0u },
    { { { 640, 40, 44 },      { -1, 35, 39 },       { -1, 0, 0     } }, 1u, 0u },
    { { { -1, -1, -1 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u },
    { { { 640, 14, 18 },      { 1280, 9, 13 },      { -1, 6, 8     } }, 1u, 0u },
    { { { 448, 27, 28 },      { 896, 25, 26 },      { -1, 23, 24   } }, 1u, 0u },
    { { { 640, 112, 116 },    { -1, 107, 111 },     { -1, 0, 0     } }, 1u, 0u },
    { { { -1, -1, -1 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u },
    { { { -1, -1, -1 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u },
    { { { 320, 97, 100 },     { 640, 94, 96 },      { -1, 90, 93   } }, 1u, 0u },
    { { { -1, 68, 68 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u },
    { { { -1, 32, 32 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u }
};

struct MusicController_Record
{
    __int16 field_0;
    __int16 field_2;
    int field_4;
    __int16 field_8;
    __int16 field_A;
    __int16 field_C;
    __int16 field_E;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record, 0x10);

const MusicController_Record array_1_stru_4CD958[] =
{
    { -1, 0, 1, 1, 0, 0, 0 },
    { 101, 0, 20, 0, 0, 0, 0 },
    { 87, 0, 16, 0, 0, 0, 0 },
    { 59, 0, 20, 0, 0, 0, 0 },
    { 65, 0, 16, 0, 0, 0, 0 },
    { 45, 0, 16, 0, 0, 0, 0 },
    { 45, 0, 16, 0, 0, 0, 0 },
    { -1, 0, 1, 1, 0, 0, 0 },
    { 20, 0, 20, 0, 0, 0, 0 },
    { 29, 0, 16, 0, 0, 0, 0 },
    { -1, 0, 1, 1, 0, 0, 0 },
    { -1, 0, 1, 1, 0, 0, 0 },
    { -1, 0, 1, 1, 0, 0, 0 },
    { 101, 0, 20, 0, 0, 0, 0 },
    { 65, 0, 16, 0, 0, 0, 0 },
    { 29, 0, 16, 0, 0, 0, 0 }
};

const MusicController_Record array_2_stru_4CDA58[] =
{
    { -1, 0, 1, 1, 0, 0, 0 },
    { 102, 0, 20, 0, 0, 0, 0 },
    { 88, 0, 16, 0, 0, 0, 0 },
    { 60, 0, 20, 0, 0, 0, 0 },
    { 66, 0, 16, 0, 0, 0, 0 },
    { 46, 0, 16, 0, 0, 0, 0 },
    { 46, 0, 16, 0, 0, 0, 0 },
    { -1, 0, 1, 1, 0, 0, 0 },
    { 21, 0, 20, 0, 0, 0, 0 },
    { 30, 0, 16, 0, 0, 0, 0 },
    { -1, 0, 1, 1, 0, 0, 0 },
    { -1, 0, 1, 1, 0, 0, 0 },
    { -1, 0, 1, 1, 0, 0, 0 },
    { 102, 0, 20, 0, 0, 0, 0 },
    { 66, 0, 16, 0, 0, 0, 0 },
    { 30, 0, 16, 0, 0, 0, 0 }
};

const MusicController_Record array_3_stru_4CDB58[] =
{
    { -1, 0, 1, 1, 0, 0, 0 },
    { 103, 0, 20, 0, 0, 0, 0 },
    { 89, 0, 16, 0, 0, 0, 0 },
    { 61, 0, 20, 0, 0, 0, 0 },
    { 67, 0, 16, 0, 0, 0, 0 },
    { 47, 0, 16, 0, 0, 0, 0 },
    { 47, 0, 16, 0, 0, 0, 0 },
    { -1, 0, 1, 1, 0, 0, 0 },
    { 22, 0, 20, 0, 0, 0, 0 },
    { 31, 0, 16, 0, 0, 0, 0 },
    { -1, 0, 1, 1, 0, 0, 0 },
    { -1, 0, 1, 1, 0, 0, 0 },
    { -1, 0, 1, 1, 0, 0, 0 },
    { 103, 0, 20, 0, 0, 0, 0 },
    { 67, 0, 16, 0, 0, 0, 0 },
    { 31, 0, 16, 0, 0, 0, 0 },

    // TODO: Are these part of something else? 
    { 20, 20, 1310736, 16, 16, 16, 1 },
    { 20, 16, 65556, 1, 20, 16, 16 }
};

struct MusicController_Record2
{
    SeqId field_0_idx;
    WORD field_2_duration;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record2, 4);

const MusicController_Record2 rec2s_4CD5A8[124] =
{
    { SeqId::Unknown_41, 320 },
    { SeqId::Unknown_42, 320 },
    { SeqId::Unknown_43, 320 },
    { SeqId::Unknown_44, 320 },
    { SeqId::Unknown_45, 320 },
    { SeqId::Unknown_46, 320 },
    { SeqId::Unknown_47, 640 },
    { SeqId::Unknown_48, 640 },
    { SeqId::Unknown_49, 640 },
    { SeqId::Unknown_50, 640 },
    { SeqId::Unknown_51, 640 },
    { SeqId::Unknown_52, 640 },
    { SeqId::Unknown_53, 640 },
    { SeqId::Unknown_54, 640 },
    { SeqId::Unknown_55, 640 },
    { SeqId::Unknown_56, 640 },
    { SeqId::Unknown_57, 640 },
    { SeqId::Unknown_58, 640 },
    { SeqId::Unknown_59, 640 },
    { SeqId::Unknown_60, 160 },
    { SeqId::Unknown_61, 256 },
    { SeqId::Unknown_62, 256 },
    { SeqId::Unknown_63, 512 },
    { SeqId::Unknown_64, 448 },
    { SeqId::Unknown_65, 448 },
    { SeqId::Unknown_66, 448 },
    { SeqId::Unknown_67, 448 },
    { SeqId::Unknown_68, 448 },
    { SeqId::Unknown_69, 448 },
    { SeqId::Unknown_70, 256 },
    { SeqId::Unknown_71, 256 },
    { SeqId::Unknown_72, 512 },
    { SeqId::Unknown_73, 512 },
    { SeqId::Unknown_74, 512 },
    { SeqId::Unknown_75, 1024 },
    { SeqId::Unknown_76, 640 },
    { SeqId::Unknown_77, 640 },
    { SeqId::Unknown_78, 640 },
    { SeqId::Unknown_79, 640 },
    { SeqId::Unknown_80, 640 },
    { SeqId::Unknown_81, 640 },
    { SeqId::Unknown_82, 640 },
    { SeqId::Unknown_83, 640 },
    { SeqId::Unknown_84, 640 },
    { SeqId::Unknown_85, 640 },
    { SeqId::Unknown_86, 256 },
    { SeqId::Unknown_87, 256 },
    { SeqId::Unknown_88, 512 },
    { SeqId::Unknown_89, 640 },
    { SeqId::Unknown_90, 640 },
    { SeqId::Unknown_91, 640 },
    { SeqId::Unknown_92, 640 },
    { SeqId::Unknown_93, 640 },
    { SeqId::Unknown_94, 640 },
    { SeqId::Unknown_95, 640 },
    { SeqId::Unknown_96, 640 },
    { SeqId::Unknown_97, 640 },
    { SeqId::Unknown_98, 640 },
    { SeqId::Unknown_99, 160 },
    { SeqId::Unknown_100, 320 },
    { SeqId::Unknown_101, 320 },
    { SeqId::Unknown_102, 320 },
    { SeqId::Unknown_103, 512 },
    { SeqId::Unknown_104, 512 },
    { SeqId::Unknown_105, 512 },
    { SeqId::Unknown_106, 256 },
    { SeqId::Unknown_107, 256 },
    { SeqId::Unknown_108, 512 },
    { SeqId::Unknown_109, 512 },
    { SeqId::Unknown_110, 256 },
    { SeqId::Unknown_111, 1024 },
    { SeqId::Unknown_112, 512 },
    { SeqId::Unknown_113, 512 },
    { SeqId::Unknown_114, 512 },
    { SeqId::Unknown_115, 512 },
    { SeqId::Unknown_116, 512 },
    { SeqId::Unknown_117, 512 },
    { SeqId::Unknown_118, 512 },
    { SeqId::Unknown_119, 512 },
    { SeqId::Unknown_120, 512 },
    { SeqId::Unknown_121, 512 },
    { SeqId::Unknown_122, 512 },
    { SeqId::Unknown_123, 512 },
    { SeqId::Unknown_124, 512 },
    { SeqId::Unknown_125, 512 },
    { SeqId::Unknown_126, 512 },
    { SeqId::Unknown_127, 256 },
    { SeqId::Unknown_128, 256 },
    { SeqId::Unknown_129, 256 },
    { SeqId::Unknown_130, 512 },
    { SeqId::Unknown_131, 320 },
    { SeqId::Unknown_132, 320 },
    { SeqId::Unknown_133, 320 },
    { SeqId::Unknown_134, 320 },
    { SeqId::Unknown_135, 320 },
    { SeqId::Unknown_136, 320 },
    { SeqId::Unknown_137, 320 },
    { SeqId::Unknown_138, 320 },
    { SeqId::Unknown_139, 320 },
    { SeqId::Unknown_140, 320 },
    { SeqId::Unknown_141, 320 },
    { SeqId::Unknown_142, 320 },
    { SeqId::Unknown_143, 320 },
    { SeqId::Unknown_144, 320 },
    { SeqId::Unknown_145, 768 },
    { SeqId::Unknown_146, 384 },
    { SeqId::Unknown_147, 384 },
    { SeqId::Unknown_148, 640 },
    { SeqId::Unknown_149, 640 },
    { SeqId::Unknown_150, 640 },
    { SeqId::Unknown_151, 640 },
    { SeqId::Unknown_152, 640 },
    { SeqId::Unknown_153, 640 },
    { SeqId::Unknown_154, 640 },
    { SeqId::Unknown_155, 640 },
    { SeqId::Unknown_156, 640 },
    { SeqId::Unknown_157, 640 },
    { SeqId::Unknown_158, 176 },
    { SeqId::Unknown_159, 176 },
    { SeqId::Unknown_160, 176 },
    { SeqId::Unknown_161, 176 },
    { SeqId::Unknown_162, 176 },
    { SeqId::Unknown_163, 220 },
    { SeqId::Unknown_0, 0 }
};

__int16 CC MusicController::Create_4436C0()
{
    if (pMusicController_507B98)
    {
        return 0;
    }

    pMusicController_507B98 = ao_new<MusicController>();
    if (pMusicController_507B98)
    {
        pMusicController_507B98->ctor_442930();
    }

    psx_root_event_507BA0 = Psx_Root_Counter_49C280(RCntCNT3, 2, 4096, MusicController::OnRootCounter_4437D0);
    Psx_Root_Counter_49C3B0(psx_root_event_507BA0);
    Psx_Root_Counter_49C340(RCntCNT3, 1);
    Psx_Root_Counter_49C360(RCntCNT3);
    Psx_Root_Counter_49C370(RCntCNT3);

    if (pMusicController_507B98->field_10)
    {
        pMusicController_507B98->field_10 = 0;
        if (pMusicController_507B98->field_4C)
        {
            pMusicController_507B98->field_4A = pMusicController_507B98->field_4C;
            pMusicController_507B98->field_4E = 0;
            pMusicController_507B98->field_2C = counter_507B9C / 2;
            pMusicController_507B98->field_50 = counter_507B9C / 2;
            pMusicController_507B98->field_48_state = 2;
        }
        pMusicController_507B98->sub_442A10();
    }
    return 1;
}


MusicController* MusicController::ctor_442930()
{
    ctor_487E10(1);
    SetVTable(this, 0x4BBBA8);

    field_6_flags.Set(Options::eSurviveDeathReset_Bit9);
    field_4_typeId = Types::eNone_0;

    field_10 = 1;
    field_3A = MusicTypes::eType0;
    field_16 = 1;
    field_1C_pObj = nullptr;
    field_20 = 0;
    field_28 = 0;
    field_22 = 1;
    field_24 = 1;
    field_2C = 0;
    field_40 = 0;
    field_30 = 0;
    field_34 = 1;
    field_48_state = 0;
    field_4A = 0;
    field_4C = 0;
    field_4E = 0;
    field_50 = 0;
    field_12 = 70;
    field_14 = 100;
    field_38_seq = SeqId::None_M1;
    field_18_level = LevelIds::eNone;
    field_26_seq = SeqId::None_M1;

    return this;
}

BaseGameObject* MusicController::dtor_4429B0()
{
    SetVTable(this, 0x4BBBA8);
    if (field_38_seq != SeqId::None_M1)
    {
        SND_Seq_Stop_477A60(field_38_seq);
    }
    return dtor_487DF0();
}

void MusicController::VScreenChanged()
{
    VScreenChanged_443450();
}

void MusicController::VScreenChanged_443450()
{
    field_16 = 1;
}

MusicController* MusicController::Vdtor_4439D0(signed int flags)
{
    dtor_4429B0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* MusicController::VDestructor(signed int flags)
{
    return Vdtor_4439D0(flags);
}

void MusicController::VUpdate_443300()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_20 = 1;
        field_1C_pObj = sActiveHero_507678;
    }

    if (field_16)
    {
        field_16 = 0;

        if (gMap_507BA8.field_0_current_level != field_18_level)
        {
            field_3C = 0;
            field_34 = 1;
            field_40 = counter_507B9C / 2;
            field_28 = 0;
            field_22 = 1;
            field_1C_pObj = nullptr;
            field_20 = 0;

            if (field_26_seq > SeqId::None_M1)
            {
                SND_Seq_Stop_477A60(field_26_seq);
                field_26_seq = SeqId::None_M1;
            }

            if (field_38_seq > SeqId::None_M1)
            {
                SND_Seq_Stop_477A60(field_38_seq);
                field_38_seq = SeqId::None_M1;
            }

            field_18_level = gMap_507BA8.field_0_current_level;

            if (field_10)
            {
                if (field_12 != field_4C)
                {
                    field_4A = field_4C;

                    __int16 v10 = 0;
                    if (field_12 <= 0)
                    {
                        v10 = 0;
                    }
                    else
                    {
                        v10 = field_12;
                    }

                    field_4E = v10;
                    field_2C = counter_507B9C / 2;
                    field_50 = counter_507B9C / 2;

                    if (field_12)
                    {
                        field_48_state = 3;
                    }
                    else
                    {
                        field_48_state = 2;
                    }
                }

                sub_443460(MusicTypes::eType0, nullptr, 1, 0);
            }
        }
    }

    if (field_3A > MusicTypes::eType1 && (counter_507B9C / 2) - field_40 >= 160)
    {
        sub_443460(MusicTypes::eType0, nullptr, 1, 0);
    }

    sub_442A10();
    
    if (field_10)
    {
        sub_442C20();
        sub_442AC0();
    }
}

void MusicController::VUpdate()
{
    VUpdate_443300();
}

void CC MusicController::sub_443810(MusicTypes a1, BaseGameObject* a2, __int16 a3, __int16 a4)
{
    if (pMusicController_507B98)
    {
        pMusicController_507B98->sub_443460(a1, a2, a3, a4);
    }
}

MusicController::MusicTypes CC MusicController::sub_443840(SeqId * seq1, SeqId * seq2, WORD* seqTime)
{
    if (!pMusicController_507B98)
    {
        return MusicTypes::eTypeNull;
    }

    if (seq1)
    {
        *seq1 = pMusicController_507B98->field_26_seq;
    }

    if (seq2)
    {
        *seq2 = pMusicController_507B98->field_38_seq;
    }

    if (seqTime)
    {
        if (pMusicController_507B98->field_3A == MusicTypes::eType0)
        {
            *seqTime = static_cast<WORD>(pMusicController_507B98->field_28 - (counter_507B9C / 2));
        }
        else
        {
            *seqTime = static_cast<WORD>(pMusicController_507B98->field_3C - (counter_507B9C / 2));
        }
    }
    return pMusicController_507B98->field_3A;
}

void MusicController::sub_442A10()
{
    if (!field_48_state)
    {
        field_28 = 0;
        field_2C = counter_507B9C / 2;
        field_40 = counter_507B9C / 2;
    }
    else if (field_48_state == 2)
    {
        field_48_state = 0;
        field_4C = 0;

        if (field_26_seq > SeqId::None_M1)
        {
            SND_Seq_Stop_477A60(field_26_seq);
            field_26_seq = SeqId::None_M1;
        }

        if (field_38_seq > SeqId::None_M1)
        {
            SND_Seq_Stop_477A60(field_38_seq);
            field_38_seq = SeqId::None_M1;
        }
    }
    else if (field_48_state == 3)
    {
        field_4C = field_4E;
        field_48_state = 1;
        if (field_26_seq > SeqId::None_M1)
        {
            SND_SEQ_SetVol_477970(field_26_seq, field_4C, field_4C);
        }

        if (field_38_seq > SeqId::None_M1)
        {
            SND_SEQ_SetVol_477970(field_38_seq, field_4C, field_4C);
        }
    }
}

void MusicController::Shutdown_4437E0()
{
    if (pMusicController_507B98)
    {
        pMusicController_507B98->field_6_flags.Set(Options::eDead_Bit3);
        pMusicController_507B98 = nullptr;
        //nullsub_5();
        Psx_Root_Counter_Event_Free_49C2B0(psx_root_event_507BA0);
        //nullsub_6();
    }
}

void CC MusicController::EnableMusic_443900(__int16 bEnable)
{
    if (pMusicController_507B98->field_10 != bEnable)
    {
        pMusicController_507B98->field_10 = bEnable;
        if (bEnable)
        {
            if (pMusicController_507B98->field_12 != pMusicController_507B98->field_4C)
            {
                pMusicController_507B98->field_4A = pMusicController_507B98->field_4C;

                __int16 v5 = 0;
                if (pMusicController_507B98->field_12 <= 0)
                {
                    v5 = 0;
                }
                else
                {
                    v5 = pMusicController_507B98->field_12;
                }

                pMusicController_507B98->field_4E = v5;
                pMusicController_507B98->field_2C = counter_507B9C / 2;
                pMusicController_507B98->field_50 = counter_507B9C / 2;
                if (pMusicController_507B98->field_12)
                {
                    pMusicController_507B98->field_48_state = 3;
                }
                else
                {
                    pMusicController_507B98->field_48_state = 2;
                }
            }
            
            pMusicController_507B98->field_3C = 0;
            pMusicController_507B98->field_28 = 0;
            pMusicController_507B98->field_40 = counter_507B9C / 2;

            if (pMusicController_507B98->field_3A == MusicTypes::eType0 ||
                pMusicController_507B98->field_3A == MusicTypes::eType8 ||
                pMusicController_507B98->field_3A == MusicTypes::eType11)
            {
                pMusicController_507B98->field_44 = 1;
                pMusicController_507B98->sub_442A10();
            }
        }
        else
        {
            if (pMusicController_507B98->field_4C)
            {
                pMusicController_507B98->field_4A = pMusicController_507B98->field_4C;
                pMusicController_507B98->field_4E = 0;
                pMusicController_507B98->field_2C = counter_507B9C / 2;
                pMusicController_507B98->field_50 = counter_507B9C / 2;
                pMusicController_507B98->field_48_state = 2;
            }
        }
        pMusicController_507B98->sub_442A10();
    }
}

int CC MusicController::OnRootCounter_4437D0()
{
    counter_507B9C++;
    return 0;
}

void MusicController::sub_443460(MusicTypes musicType, BaseGameObject* pObj, __int16 a4, __int16 a5)
{
    if (musicType == MusicTypes::eType0 || musicType == MusicTypes::eType1 || pObj)
    {
        if (musicType == MusicTypes::eType0)
        {
            if (gElum_507680)
            {
                if (sControlledCharacter_50767C == gElum_507680)
                {
                    musicType = MusicTypes::eType1;
                }
            }
        }

        if (musicType == field_3A)
        {
            if (musicType != MusicTypes::eType0)
            {
                field_40 = counter_507B9C / 2;
            }

            if (field_1C_pObj)
            {
                if (field_1C_pObj == pObj)
                {
                    field_20 = a4;
                }
            }

            if (!field_44)
            {
                field_44 = a5;
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
                field_1C_pObj = 0;
            }

            field_3A = musicType;
            field_40 = counter_507B9C / 2;
            field_3C = 0;
            field_44 = 1;
            return;
        }

        if (pObj == field_1C_pObj || !field_1C_pObj || !field_20 && (a4 || musicType >= field_3A))
        {
            field_1C_pObj = pObj;
            field_20 = a4;
            field_3A = musicType;
            field_40 = counter_507B9C / 2;
            field_3C = 0;
            field_44 = 1;
            return;
        }
    }
}

void MusicController::sub_442C20()
{
    const int counterVal = counter_507B9C >> 1;

    if (counterVal >= field_3C  && !(((counterVal) - field_30) % field_34))
    {
        if (field_38_seq != SeqId::None_M1)
        {
            SND_Seq_Stop_477A60(field_38_seq);
        }

        const MusicController_Record* v15 = nullptr;
        int v4 = 0;
        switch (field_3A)
        {
        case MusicTypes::eType1:
            if (field_18_level == LevelIds::eLines_2)
            {
                //LOWORD(v4) = 86;
                field_34 = 16;
            }
            else if (field_18_level == LevelIds::eForest_3)
            {
                //LOWORD(v4) = 58;
                field_34 = 20;
            }
            else
            {
                if (field_18_level == LevelIds::eDesert_8)
                {
                    //LOWORD(v4) = 19;
                }
                else
                {
                    //LOWORD(v4) = -1;
                }
                field_34 = 20;
            }

            if (field_12 == field_4C)
            {
                field_24 = 1;
                field_22 = 1;
                goto LABEL_129;
            }
            field_4A = field_4C;

            if (field_12 <= 0)
            {
                field_4E = 0;
            }
            else
            {
                field_4E = field_12;
            }

            field_2C = counterVal;
            field_50 = (counterVal) + 30;

            if (field_12)
            {
                field_48_state = 3;
                field_24 = 1;
                field_22 = 1;
            }
            else
            {
                field_48_state = 2;
                field_24 = 1;
                field_22 = 1;
            }
            goto LABEL_129;

        case MusicTypes::eType2:
            field_34 = 1;
            //LOWORD(v4) = -1;
            field_24 = 0;
            if (field_14 != field_4C)
            {
                field_4A = field_4C;
                if (field_14 <= 0)
                {
                    field_4E = 0;
                }
                else
                {
                    field_4E = field_14;
                }
                field_2C = counterVal;
                field_50 = counterVal;
                if (field_14)
                {
                    goto LABEL_104;
                }
                field_48_state = 2;
            }
            goto LABEL_129;

        case MusicTypes::eType3:
            if (field_44)
            {
                //LOWORD(v4) = Math_RandomRange_450F20(0, 1);
            }
            else
            {
                //LOWORD(v4) = -1;
            }

            field_34 = 1;
            field_24 = 0;
            if (field_14 != field_4C)
            {
                field_4A = field_4C;
                if (field_14 <= 0)
                {
                    field_4E = 0;
                }
                else
                {
                    field_4E = field_14;
                }
                field_2C = counterVal;
                field_50 = counterVal;
                if (field_14)
                {
                    goto LABEL_104;
                }
                field_48_state = 2;
            }
            goto LABEL_129;

        case MusicTypes::eType4:
            v15 = &array_1_stru_4CD958[static_cast<int>(field_18_level)];
            goto LABEL_39;

        case MusicTypes::eType5:
            v15 = &array_2_stru_4CDA58[static_cast<int>(field_18_level)];
        LABEL_39:
            //LOWORD(v4) = v15->field_0;
            field_24 = v15->field_8;
            field_34 = v15->field_4;
            if (field_14 == field_4C)
            {
                goto LABEL_129;
            }
            
            field_4A = field_4C;

            if (field_14 <= 0)
            {
                field_4E = 0;
            }
            else
            {
                field_4E = field_14;
            }

            field_2C = counterVal;
            field_50 = counterVal;
            if (field_14)
            {
                goto LABEL_104;
            }
            field_48_state = 2;
            goto LABEL_129;

        case MusicTypes::eType6:
            if (field_44)
            {
                const MusicController_Record* v19 = &array_3_stru_4CDB58[static_cast<int>(field_18_level)];
                //LOWORD(v4) = v19->field_0;
                field_34 = v19->field_4;
                field_24 = v19->field_8;
                goto LABEL_98;
            }
            //LOWORD(v4) = -1;
            field_34 = 20;
            goto LABEL_84;

        case MusicTypes::eType7:
            //LOWORD(v4) = 117;
            field_34 = 22;
            if (field_14 == field_4C)
            {
                goto LABEL_75;
            }
            field_4A = field_4C;
            if (field_14 <= 0)
            {
                field_4E = 0;
            }
            else
            {
                field_4E = field_14;
            }
            field_2C = counterVal;
            field_50 = counterVal;
            if (field_14)
            {
                goto LABEL_74;
            }
            field_48_state = 2;
            field_24 = 0;
            goto LABEL_129;

        case MusicTypes::eType8:
            if (gMap_507BA8.field_0_current_level == LevelIds::eBoardRoom_12)
            {
                //LOWORD(v4) = 102;
                field_34 = 1;
            }
            else
            {
                if (field_44)
                {
                    //LOWORD(v4) = 122;
                }
                else
                {
                    //LOWORD(v4) = Math_RandomRange_450F20(118, 120);
                }
                field_34 = 22;
            }

            if (field_14 == field_4C)
            {
                goto LABEL_75;
            }
            
            field_4A = field_4C;

            if (field_14 <= 0)
            {
                field_4E = 0;
            }
            else
            {
                field_4E = field_14;
            }

            field_2C = counterVal;
            field_50 = counterVal;
            if (field_14)
            {
                goto LABEL_74;
            }
            field_48_state = 2;
            field_24 = 0;
            goto LABEL_129;

        case MusicTypes::eType9:
            //LOWORD(v4) = 121;
            goto LABEL_68;

        case MusicTypes::eType10:
            //LOWORD(v4) = 122;
        LABEL_68:
            field_34 = 22;
            if (field_14 == field_4C)
            {
                goto LABEL_75;
            }
            field_4A = field_4C;
            if (field_14 <= 0)
            {
                field_4E = 0;
            }
            else
            {
                field_4E = field_14;
            }

            field_2C = counterVal;
            field_50 = counterVal;

            if (field_14)
            {
            LABEL_74:
                field_48_state = 3;
            LABEL_75:
                field_24 = 0;
            }
            else
            {
                field_48_state = 2;
                field_24 = 0;
            }
            goto LABEL_129;

        case MusicTypes::eType11:
            if (field_44)
            {
                if (field_18_level == LevelIds::eForestChase)
                {
                    //LOWORD(v4) = 69;
                }
                else
                {
                    if (field_18_level != LevelIds::eDesertEscape)
                    {
                        //LOWORD(v4) = 117;
                        field_34 = 22;
                        field_24 = 0;
                        field_46 = 1;
                        goto LABEL_98;
                    }
                    //LOWORD(v4) = 33;
                }
                field_34 = 16;
                field_24 = 0;
                field_46 = 1;

            LABEL_98:
                if (field_14 != field_4C)
                {
                    field_4A = field_4C;
                    if (field_14 <= 0)
                    {
                        field_4E = 0;
                    }
                    else
                    {
                        field_4E = field_14;
                    }

                    field_2C = counterVal;
                    field_50 = counterVal;

                    if (field_14)
                    {
                    LABEL_104:
                        field_48_state = 3;
                    }
                    else
                    {
                        field_48_state = 2;
                    }
                }
                goto LABEL_129;
            }
            //LOWORD(v4) = -1;
            field_34 = 16;

        LABEL_84:
            field_24 = 1;
            if (field_12 != field_4C)
            {
                field_4A = field_4C;
                if (field_12 <= 0)
                {
                    field_4E = 0;
                }
                else
                {
                    field_4E = field_12;
                }

                field_2C = counterVal;
                field_50 = (counterVal) + 30;

                if (!field_12)
                {
                    field_48_state = 2;
                    field_46 = 1;
                    goto LABEL_129;
                }
                field_48_state = 3;
            }
            field_46 = 1;

        LABEL_129:
            if (v4 < 0)
            {
                field_38_seq = SeqId::None_M1;
                field_3C = 0;
            }
            else
            {
                field_3C = (counterVal) + rec2s_4CD5A8[v4].field_2_duration;
                field_38_seq = rec2s_4CD5A8[v4].field_0_idx;
                SND_SEQ_Play_477760(field_38_seq, 1, field_4C, field_4C);
            }

            field_30 = counterVal;
            if (field_44)
            {
                if (field_46)
                {
                    field_46 = 0;
                }
                else
                {
                    field_22 = 1;
                }
                field_44 = 0;
            }
            break;

        case MusicTypes::eType12:
            if (field_18_level == LevelIds::eForestChase)
            {
                //LOWORD(v4) = 70;
                field_34 = 16;
            }
            else if (field_18_level == LevelIds::eDesertEscape)
            {
                //LOWORD(v4) = 34;
                field_34 = 32;
            }
            else
            {
                //LOWORD(v4) = 122;
                field_34 = 22;
            }
            field_24 = 0;
            goto LABEL_98;

        case MusicTypes::eType13:
            //v4 = -(field_44 != 0);
            field_34 = 1;

            //LOWORD(v4) = (v4 & 3) - 1;

            field_24 = 0;

            if (field_14 == field_4C)
            {
                goto LABEL_129;
            }

            field_4A = field_4C;

            if (field_14 <= 0)
            {
                field_4E = 0;
            }
            else
            {
                field_4E = field_14;
            }

            field_2C = counterVal;
            field_50 = counterVal;
            if (field_14)
            {
                goto LABEL_104;
            }
            field_48_state = 2;
            goto LABEL_129;

        case MusicTypes::eType14:
            field_34 = 1;
            field_24 = 0;
            //v4 = field_44 != 0 ? 3 : -1;
            if (field_14 == field_4C)
            {
                goto LABEL_129;
            }
            field_4A = field_4C;
            if (field_14 <= 0)
            {
                field_4E = 0;
            }
            else
            {
                field_4E = field_14;
            }
            field_2C = counterVal;
            field_50 = counterVal;
            if (field_14)
            {
                goto LABEL_104;
            }
            field_48_state = 2;
            goto LABEL_129;

        case MusicTypes::eType15:
            //v4 = field_44 != 0 ? 5 : 0;
            goto LABEL_29;

        case MusicTypes::eType16:
            //v4 = field_44 != 0 ? 6 : 0;

        LABEL_29:
            field_34 = 1;
            //LOWORD(v4) = v4 - 1;
            field_24 = 0;
            if (field_4C != 127)
            {
                field_4A = field_4C;
                field_4E = 127;
                field_2C = counterVal;
                field_50 = counterVal;
                field_48_state = 3;
            }
            goto LABEL_129;

        default:
            //LOWORD(v4) = -1;
            if (field_12 != field_4C)
            {
                field_4A = field_4C;
                if (field_12 <= 0)
                {
                    field_4E = 0;
                }
                else
                {
                    field_4E = field_12;
                }

                field_2C = counterVal;
                field_50 = (counterVal) + 30;

                if (field_12)
                {
                    field_48_state = 3;
                }
                else
                {
                    field_48_state = 2;
                }
            }
            field_34 = 1;
            field_24 = 1;
            goto LABEL_129;
        }
    }
}

void MusicController::sub_442AC0()
{
    if (field_24 || field_26_seq == SeqId::None_M1)
    {
        if (field_22)
        {
            field_22 = 0;
            field_28 = 0;
            field_2C = counter_507B9C >> 1;
        }

        if (counter_507B9C >> 1 >= field_28 && field_24)
        {
            if (field_26_seq != SeqId::None_M1)
            {
                SND_Seq_Stop_477A60(field_26_seq);
            }

            __int16 random = -1;
            if (field_3A != MusicTypes::eType0)
            {
                const MusicController_Record3* pRec = &rec3s_4CD798[static_cast<int>(field_18_level)];
                random = Math_RandomRange_450F20(pRec->field_0[0].field_4_min, pRec->field_0[0].field_6_max);
            }
            else
            {
                const MusicController_Record3* pRec = &rec3s_4CD798[static_cast<int>(field_18_level)];
                for (const MusicController_Record3_Sub& rec : pRec->field_0)
                {
                    const auto diff = (counter_507B9C >> 1) - field_2C;
                    if (rec.field_0_count < 0 || diff < rec.field_0_count)
                    {
                        random = Math_RandomRange_450F20(rec.field_4_min, rec.field_6_max);
                        break;
                    }
                }
            }

            if (random < 0)
            {
                field_26_seq = SeqId::None_M1;
                field_28 = counter_507B9C >> 1;
            }
            else
            {
                field_26_seq = rec2s_4CD5A8[random].field_0_idx;
                SND_SEQ_Play_477760(field_26_seq, rec3s_4CD798[static_cast<int>(field_18_level)].field_18, field_4C, field_4C);
                field_28 = (counter_507B9C >> 1)  + rec2s_4CD5A8[random].field_2_duration;
            }
        }
    }
    else
    {
        SND_Seq_Stop_477A60(field_26_seq);
        field_26_seq = SeqId::None_M1;
    }
}

END_NS_AO
