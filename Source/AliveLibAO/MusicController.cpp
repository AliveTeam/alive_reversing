#include "stdafx_ao.h"
#include "Function.hpp"
#include "MusicController.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"

START_NS_AO

__int16 CC MusicController::Create_4436C0()
{
    NOT_IMPLEMENTED();
    return 0;
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
    field_1C = 0;
    field_20 = 0;
    field_28 = 0;
    field_22 = 1;
    field_24 = 1;
    field_2C = 0;
    field_40 = 0;
    field_30 = 0;
    field_34 = 1;
    field_48 = 0;
    field_4A = 0;
    field_4C = 0;
    field_4E = 0;
    field_50 = 0;
    field_12 = 70;
    field_14 = 100;
    field_38 = SeqId::None_M1;
    field_18 = -1;
    field_26 = -1;

    return this;
}

BaseGameObject* MusicController::dtor_4429B0()
{
    SetVTable(this, 0x4BBBA8);
    if (field_38 != SeqId::None_M1)
    {
        SND_Seq_Stop_477A60(field_38);
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
    NOT_IMPLEMENTED();
}

void MusicController::VUpdate()
{
    VUpdate_443300();
}

void CC MusicController::sub_443810(MusicTypes /*a1*/, BaseGameObject* /*a2*/, __int16 /*a3*/, __int16 /*a4*/)
{
    NOT_IMPLEMENTED();
}

__int16 CC MusicController::sub_443840(WORD* /*seq1*/, WORD* /*seq2*/, WORD* /*seqTime*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

void MusicController::Shutdown_4437E0()
{
    NOT_IMPLEMENTED();
}

void CC MusicController::EnableMusic_443900(__int16 /*bEnable*/)
{
    NOT_IMPLEMENTED();
}

END_NS_AO
