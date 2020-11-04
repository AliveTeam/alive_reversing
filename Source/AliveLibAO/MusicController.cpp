#include "stdafx_ao.h"
#include "Function.hpp"
#include "MusicController.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Events.hpp"

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
    field_3A = 0;
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

    if (field_3A > 1 && (counter_507B9C / 2) - field_40 >= 160)
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

__int16 CC MusicController::sub_443840(SeqId * seq1, SeqId * seq2, WORD* seqTime)
{
    if (!pMusicController_507B98)
    {
        return -1;
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
        if (!pMusicController_507B98->field_3A)
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

void CC MusicController::EnableMusic_443900(__int16 /*bEnable*/)
{
    NOT_IMPLEMENTED();
}

int CC MusicController::OnRootCounter_4437D0()
{
    counter_507B9C++;
    return 0;
}

void MusicController::sub_443460(MusicTypes /*a2*/, BaseGameObject* /*a3*/, __int16 /*a4*/, __int16 /*a5*/)
{
    NOT_IMPLEMENTED();
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
