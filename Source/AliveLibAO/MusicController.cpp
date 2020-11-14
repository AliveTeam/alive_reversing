#include "stdafx_ao.h"
#include "Function.hpp"
#include "MusicController.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Events.hpp"
#include "Elum.hpp"

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
    WORD field_0_idx;
    WORD field_2_duration;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record2, 4);

// TODO: Add placeholder SeqId's
const MusicController_Record2 rec2s_4CD5A8[124] =
{
    { 41, 320 },
    { 42, 320 },
    { 43, 320 },
    { 44, 320 },
    { 45, 320 },
    { 46, 320 },
    { 47, 640 },
    { 48, 640 },
    { 49, 640 },
    { 50, 640 },
    { 51, 640 },
    { 52, 640 },
    { 53, 640 },
    { 54, 640 },
    { 55, 640 },
    { 56, 640 },
    { 57, 640 },
    { 58, 640 },
    { 59, 640 },
    { 60, 160 },
    { 61, 256 },
    { 62, 256 },
    { 63, 512 },
    { 64, 448 },
    { 65, 448 },
    { 66, 448 },
    { 67, 448 },
    { 68, 448 },
    { 69, 448 },
    { 70, 256 },
    { 71, 256 },
    { 72, 512 },
    { 73, 512 },
    { 74, 512 },
    { 75, 1024 },
    { 76, 640 },
    { 77, 640 },
    { 78, 640 },
    { 79, 640 },
    { 80, 640 },
    { 81, 640 },
    { 82, 640 },
    { 83, 640 },
    { 84, 640 },
    { 85, 640 },
    { 86, 256 },
    { 87, 256 },
    { 88, 512 },
    { 89, 640 },
    { 90, 640 },
    { 91, 640 },
    { 92, 640 },
    { 93, 640 },
    { 94, 640 },
    { 95, 640 },
    { 96, 640 },
    { 97, 640 },
    { 98, 640 },
    { 99, 160 },
    { 100, 320 },
    { 101, 320 },
    { 102, 320 },
    { 103, 512 },
    { 104, 512 },
    { 105, 512 },
    { 106, 256 },
    { 107, 256 },
    { 108, 512 },
    { 109, 512 },
    { 110, 256 },
    { 111, 1024 },
    { 112, 512 },
    { 113, 512 },
    { 114, 512 },
    { 115, 512 },
    { 116, 512 },
    { 117, 512 },
    { 118, 512 },
    { 119, 512 },
    { 120, 512 },
    { 121, 512 },
    { 122, 512 },
    { 123, 512 },
    { 124, 512 },
    { 125, 512 },
    { 126, 512 },
    { 127, 256 },
    { 128, 256 },
    { 129, 256 },
    { 130, 512 },
    { 131, 320 },
    { 132, 320 },
    { 133, 320 },
    { 134, 320 },
    { 135, 320 },
    { 136, 320 },
    { 137, 320 },
    { 138, 320 },
    { 139, 320 },
    { 140, 320 },
    { 141, 320 },
    { 142, 320 },
    { 143, 320 },
    { 144, 320 },
    { 145, 768 },
    { 146, 384 },
    { 147, 384 },
    { 148, 640 },
    { 149, 640 },
    { 150, 640 },
    { 151, 640 },
    { 152, 640 },
    { 153, 640 },
    { 154, 640 },
    { 155, 640 },
    { 156, 640 },
    { 157, 640 },
    { 158, 176 },
    { 159, 176 },
    { 160, 176 },
    { 161, 176 },
    { 162, 176 },
    { 163, 220 },
    { 0, 0 }
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
    NOT_IMPLEMENTED();
}

void MusicController::sub_442AC0()
{
    NOT_IMPLEMENTED();
}

END_NS_AO
